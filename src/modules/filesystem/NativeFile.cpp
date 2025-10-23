/**
 * Copyright (c) 2006-2025 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

// Ported to SDL2

// LOVE
#include "NativeFile.h"

// C
#include <cstring>
#include <cstdio>

// SDL
#include <SDL2/SDL.h>

namespace love
{
namespace filesystem
{

NativeFile::NativeFile(const std::string &filename, Mode mode)
: filename(filename)
, file(nullptr)
, mode(MODE_CLOSED)
, buffer(nullptr)
, bufferMode(BUFFER_NONE)
, bufferSize(0)
, bufferUsed(0)
{
    if (!open(mode))
        throw love::Exception("Could not open file at path %s", filename.c_str());
}

NativeFile::NativeFile(const NativeFile &other)
: filename(other.filename)
, file(nullptr)
, mode(MODE_CLOSED)
, buffer(nullptr)
, bufferMode(other.bufferMode)
, bufferSize(other.bufferSize)
, bufferUsed(0)
{
    if (!open(other.mode))
        throw love::Exception("Could not open file at path %s", filename.c_str());
}

NativeFile::~NativeFile()
{
    if (mode != MODE_CLOSED)
        close();
}

NativeFile *NativeFile::clone()
{
    return new NativeFile(*this);
}

bool NativeFile::open(Mode newmode)
{
    if (newmode == MODE_CLOSED)
    {
        close();
        return true;
    }

    // File already open?
    if (file != nullptr)
        return false;

    const char* modeString = getModeString(newmode);
    file = std::fopen(filename.c_str(), modeString);
    if (file == nullptr)
        throw love::Exception("Could not open file %s: %s", filename.c_str(), SDL_GetError());

    mode = newmode;

    if (!setupBuffering(bufferMode, bufferSize))
    {
        std::fclose(file);
        file = nullptr;
        mode = MODE_CLOSED;
        throw love::Exception("Could not open file %s: cannot setup buffering", filename.c_str());
    }

    return file != nullptr;
}

bool NativeFile::close()
{
    if (file == nullptr)
        return false;

    bool success = flush();
    success = std::fclose(file) == 0 && success;
    mode = MODE_CLOSED;
    file = nullptr;
    setupBuffering(BUFFER_NONE, 0);

    return success;
}

bool NativeFile::isOpen() const
{
    return mode != MODE_CLOSED && file != nullptr;
}

int64 NativeFile::getSize()
{
    int64 size = -1;
    if (!file)
    {
        open(MODE_READ);
        std::fseek(file, 0, SEEK_END);
        size = std::ftell(file);
        close();
    }
    else
    {
        std::fseek(file, 0, SEEK_END);
        size = std::ftell(file);
        std::fseek(file, 0, SEEK_SET);
    }

    return std::max<int64>(size, -1);
}

int64 NativeFile::read(void *dst, int64 size)
{
    if (!file || mode != MODE_READ)
        throw love::Exception("File is not opened for reading.");

    if (size < 0)
        throw love::Exception("Invalid read size.");

    // Are we using buffers?
    if (buffer)
        return bufferedRead(dst, size);

    // No buffering.
    size_t read = std::fread(dst, 1, (size_t) size, file);
    return (int64) read;
}

bool NativeFile::write(const void *data, int64 size)
{
    if (!file || (mode != MODE_WRITE && mode != MODE_APPEND))
        throw love::Exception("File is not opened for writing.");

    if (size < 0)
        throw love::Exception("Invalid write size.");

    if (buffer)
    {
        if (!bufferedWrite(data, size))
            return false;

        // There's newline? force flush
        if (bufferMode == BUFFER_LINE && memchr(data, '\n', size) != nullptr)
        {
            if (!flush())
                return false;
        }
    }
    else
    {
        return std::fwrite(data, 1, (size_t) size, file) == (size_t) size;
    }

    return true;
}

bool NativeFile::flush()
{
    switch (mode)
    {
    case MODE_READ:
        if (buffer)
        {
            // Seek to already consumed buffer
            if (std::fseek(file, bufferUsed - bufferSize, SEEK_CUR) < 0)
                return false;

            // Mark as depleted
            bufferUsed = bufferSize;
        }
        return true;

    case MODE_WRITE:
    case MODE_APPEND:
        if (buffer && bufferUsed > 0)
        {
            size_t written = std::fwrite(buffer, 1, (size_t) bufferUsed, file);
            memmove(buffer, buffer + written, (size_t) bufferSize - written);
            bufferUsed = std::max<int64>(bufferUsed - (int64) written, 0);
        }

        return std::fflush(file) == 0;

    default:
        throw love::Exception("Invalid file mode.");
    }

    return true;
}

bool NativeFile::isEOF()
{
    return file == nullptr || std::ftell(file) >= getSize();
}

int64 NativeFile::tell()
{
    if (file == nullptr)
        return -1;

    int64 offset = 0;
    if (buffer)
    {
        switch (mode)
        {
        case MODE_READ:
            offset = bufferUsed - bufferSize;
            break;
        case MODE_WRITE:
        case MODE_APPEND:
            offset = bufferUsed;
            break;
        }
    }

    return std::ftell(file) + offset;
}

bool NativeFile::seek(int64 pos, SeekOrigin origin)
{
    if (file == nullptr)
        return false;

    if (mode == MODE_APPEND)
        return false;

    int whence = SEEK_SET;
    if (origin == SEEKORIGIN_CURRENT)
        whence = SEEK_CUR;
    else if (origin == SEEKORIGIN_END)
        whence = SEEK_END;

    if (mode == MODE_READ && whence == SEEK_SET && buffer)
    {
        int64 offset = pos - tell();
        if (offset >= 0 && (offset + bufferUsed) < bufferSize)
        {
            bufferUsed += offset;
            return true;
        }
    }

    return std::fseek(file, pos, whence) == 0;
}

bool NativeFile::setBuffer(BufferMode bufmode, int64 size)
{
    constexpr int64 DEFAULT_BUFFER_SIZE = std::max<int64>(BUFSIZ, 4096);

    if (size < 0)
        return false;
    else if (sizeof(uintptr_t) == 4 && size > 0x80000000LL)
        return false;

    if (bufmode == BUFFER_NONE)
        size = 0;
    else if (size == 0)
        size = DEFAULT_BUFFER_SIZE;

    if (file)
    {
        if (!flush())
            return false;

        if (!setupBuffering(bufmode, size))
            return false;
    }

    bufferMode = bufmode;
    bufferSize = size;

    return true;
}

File::BufferMode NativeFile::getBuffer(int64 &size) const
{
    size = bufferSize;
    return bufferMode;
}

const std::string &NativeFile::getFilename() const
{
    return filename;
}

File::Mode NativeFile::getMode() const
{
    return mode;
}

bool NativeFile::setupBuffering(BufferMode mode, int64 bufferSize)
{
    int8 *newbuf = nullptr;
    if (mode != BUFFER_NONE)
    {
        newbuf = new (std::nothrow) int8[(size_t) bufferSize];
        if (newbuf == nullptr)
            return false;
    }

    delete[] buffer;
    buffer = newbuf;
    bufferUsed = this->mode == MODE_READ ? bufferSize : 0;
    return true;
}

int64 NativeFile::bufferedRead(void *dst, int64 size)
{
    int8 *ptr = (int8 *) dst;
    int64 readed = 0;

    while (size > 0)
    {
        int64 available = bufferSize - bufferUsed;

        if (available > 0)
        {
            size_t copy = (size_t) std::min(size, available);
            memcpy(ptr, buffer + (size_t) bufferUsed, copy);
            ptr += copy;
            size -= (int64) copy;
            bufferUsed += (int64) copy;
            readed += copy;
        }
        else
        {
            size_t ureaded = std::fread(buffer, 1, (size_t) bufferSize, file);
            bufferUsed = bufferSize - (int64) ureaded;

            if (ureaded == 0)
                break;

            if (bufferUsed > 0)
                memmove(buffer + bufferUsed, buffer, ureaded);
        }
    }

    return readed;
}

bool NativeFile::bufferedWrite(const void *data, int64 size)
{
    const int8 *ptr = (const int8 *) data;

    int64 inBuffer = std::min<int64>(size, bufferSize - bufferUsed);
    if (inBuffer > 0)
    {
        memcpy(buffer + bufferUsed, ptr, (size_t) inBuffer);
        bufferUsed += inBuffer;
        size -= inBuffer;
        ptr += (size_t) inBuffer;
    }

    if (size > 0)
    {
        size_t bufferWritten = std::fwrite(buffer, 1, (size_t) bufferSize, file);
        bufferUsed -= bufferWritten;

        if (bufferWritten < (size_t) bufferSize)
        {
            memmove(buffer, buffer + bufferWritten, (size_t) bufferSize - bufferWritten);
            return false;
        }

        int64 directWriteCount = size / bufferSize;
        if (directWriteCount > 0)
        {
            size_t targetDirectBuffer = (size_t) (directWriteCount * bufferSize);
            if (std::fwrite(ptr, 1, targetDirectBuffer, file) < targetDirectBuffer)
                return false;

            ptr += targetDirectBuffer;
            size -= targetDirectBuffer;
        }

        if (size > 0)
        {
            memcpy(buffer, ptr, size);
            bufferUsed = size;
        }

        return std::fflush(file) == 0;
    }

    return true;
}

const char *NativeFile::getModeString(Mode mode)
{
    switch (mode)
    {
    case File::MODE_CLOSED:
    default:
        return "c";
    case File::MODE_READ:
        return "rb";
    case File::MODE_WRITE:
        return "wb";
    case File::MODE_APPEND:
        return "ab";
    }
}

} // filesystem
} // love
