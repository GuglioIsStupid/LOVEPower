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

#include <iostream>
#include <sstream>
#include <algorithm>

#include "common/utf8.h"
#include "common/b64.h"

#include "Filesystem.h"
#include "File.h"
#include "PhysfsIo.h"

// PhysFS
#include <physfs.h>

// For great CWD. (Current Working Directory)
// Using this instead of boost::filesystem which totally
// cramped our style.
#include <sys/param.h>
#include <unistd.h>

#include <string>

namespace love
{
namespace filesystem
{
namespace physfs
{

static std::string normalize(const std::string &input)
{
	std::stringstream out;
	bool seenSep = false, isSep = false;
	for (size_t i = 0; i < input.size(); ++i)
	{
		isSep = (input[i] == LOVE_PATH_SEPARATOR[0]);
		if (!isSep || !seenSep)
			out << input[i];
		seenSep = isSep;
	}

	return out.str();
}

static const Filesystem::CommonPath appCommonPaths[] =
{
	Filesystem::COMMONPATH_APP_SAVEDIR,
	Filesystem::COMMONPATH_APP_DOCUMENTS
};

static bool isAppCommonPath(Filesystem::CommonPath path)
{
	switch (path)
	{
	case Filesystem::COMMONPATH_APP_SAVEDIR:
	case Filesystem::COMMONPATH_APP_DOCUMENTS:
		return true;
	default:
		return false;
	}
}

static bool isMounted(const std::string &path)
{
	char **mountedpaths = PHYSFS_getSearchPath();
	if (mountedpaths == nullptr)
		return false;

	bool mounted = false;
	for (char **p = mountedpaths; *p != nullptr; p++)
	{
		char *mountedpath = *p;
		if (strcmp(path.c_str(), mountedpath) == 0)
		{
			mounted = true;
			break;
		}
	}

	PHYSFS_freeList(mountedpaths);
	return mounted;
}

Filesystem::Filesystem()
	: love::filesystem::Filesystem("love.filesystem.physfs")
	, appendIdentityToPath(false)
	, fused(false)
	, fusedSet(false)
	, fullPaths()
	, commonPathMountInfo()
	, saveDirectoryNeedsMounting(false)
{
	requirePath = {"?.lua", "?/init.lua"};
	cRequirePath = {"??"};
}

Filesystem::~Filesystem()
{
	if (PHYSFS_isInit())
		PHYSFS_deinit();
}

void Filesystem::init(const char *arg0)
{
	if (!PHYSFS_init(arg0))
		throw love::Exception("Failed to initialize filesystem: %s", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	// Enable symlinks by default.
	setSymlinksEnabled(true);
}

void Filesystem::setFused(bool fused)
{
	if (fusedSet)
		return;
	this->fused = fused;
	fusedSet = true;
}

bool Filesystem::isFused() const
{
	if (!fusedSet)
		return false;
	return fused;
}

bool Filesystem::setIdentity(const char *ident, bool appendToPath)
{
	if (!PHYSFS_isInit())
		return false;

	if (ident == nullptr || strlen(ident) == 0)
		return false;

	// Validate whether re-mounting will work.
	for (CommonPath p : appCommonPaths)
	{
		if (!commonPathMountInfo[p].mounted)
			continue;

		// If a file is still open, unmount will fail.
		std::string fullpath = getFullCommonPath(p);
		
		if (!fullpath.empty())
		{
			std::string canonpath = canonicalizeRealPath(fullpath);
			PHYSFS_unmount(canonpath.c_str());
		}
	}

	bool oldMountedCommonPaths[COMMONPATH_MAX_ENUM] = {false};

	// We don't want old save paths to accumulate when we set a new identity.
	for (CommonPath p : appCommonPaths)
	{
		oldMountedCommonPaths[p] = commonPathMountInfo[p].mounted;
		if (commonPathMountInfo[p].mounted)
			unmount(p);
	}

	// These will be re-populated by getFullCommonPath.
	for (CommonPath p : appCommonPaths)
		fullPaths[p].clear();

	// Store the save directory. getFullCommonPath(COMMONPATH_APP_*) uses this.
	saveIdentity = std::string(ident);
	appendIdentityToPath = appendToPath;

	// Try to mount as readwrite without creating missing directories in the
	// path hierarchy. If this fails, setupWriteDirectory will attempt to create
	// them and try again.
	// This is done so the save directory is only created on-demand.
	if (!mountCommonPathInternal(COMMONPATH_APP_SAVEDIR, nullptr, MOUNT_PERMISSIONS_READWRITE, appendToPath, false))
		saveDirectoryNeedsMounting = true;
	else
		saveDirectoryNeedsMounting = false;

	// Mount any other app common paths with directory creation immediately
	// instead of on-demand, since to get to this point they would have to be
	// explicitly mounted already beforehand.
	for (CommonPath p : appCommonPaths)
	{
		if (oldMountedCommonPaths[p] && p != COMMONPATH_APP_SAVEDIR)
		{
			// TODO: error handling?
			auto info = commonPathMountInfo[p];
			mountCommonPathInternal(p, info.mountPoint.c_str(), info.permissions, appendToPath, true);
		}
	}

	return true;
}

const char *Filesystem::getIdentity() const
{
	return saveIdentity.c_str();
}

bool Filesystem::setSource(const char *source)
{
	if (!PHYSFS_isInit())
		return false;

	// Check whether directory is already set.
	if (!gameSource.empty())
		return false;

	std::string new_search_path = canonicalizeRealPath(source);

	if (isMounted(new_search_path))
		return false;

	// Add the directory.
	if (!PHYSFS_mount(new_search_path.c_str(), nullptr, 1))
	{
		// It's possible there is additional data at the end of the fused executable,
		// e.g. for signed windows executables (the signature).
		// In this case let's try a little bit harder to find the zip file.
		// This is not used by default because I assume that the physfs IOs are probably
		// more robust and more performant, so they should be favored, if possible.
		auto io = StripSuffixIo::create(new_search_path);
		if (!io->determineStrippedLength())
		{
			delete io;
			return false;
		}
		if (!PHYSFS_mountIo(io, io->filename.c_str(), nullptr, 1))
		{
			// If PHYSFS_mountIo fails, io->destroy(io) is not called and we have
			// to delete ourselves.
			delete io;
			return false;
		}
		return true;
	}

	// Save the game source.
	gameSource = new_search_path;

	return true;
}

const char *Filesystem::getSource() const
{
	return gameSource.c_str();
}

bool Filesystem::setupWriteDirectory()
{
	if (!PHYSFS_isInit())
		return false;

	if (!saveDirectoryNeedsMounting)
		return true;

	if (saveIdentity.empty())
		return false;

	// Only the save directory is mounted on-demand if it doesn't exist yet.
	// Other app common paths are immediately re-mounted in setIdentity.
	bool createdir = true;
	if (!mountCommonPathInternal(COMMONPATH_APP_SAVEDIR, nullptr, MOUNT_PERMISSIONS_READWRITE, appendIdentityToPath, createdir))
		return false;

	saveDirectoryNeedsMounting = false;
	return true;
}

bool Filesystem::mount(const char *archive, const char *mountpoint, bool appendToPath)
{
	if (!PHYSFS_isInit() || !archive)
		return false;

	std::string realPath;
	std::string sourceBase = getSourceBaseDirectory();

	// Check whether the given archive path is in the list of allowed full paths.
	auto it = std::find(allowedMountPaths.begin(), allowedMountPaths.end(), archive);

	if (it != allowedMountPaths.end())
		realPath = *it;
	else if (isFused() && sourceBase.compare(archive) == 0)
	{
		// Special case: if the game is fused and the archive is the source's
		// base directory, mount it even though it's outside of the save dir.
		realPath = sourceBase;
	}
	else
	{
		// Not allowed for safety reasons.
		if (strlen(archive) == 0 || strstr(archive, "..") || strcmp(archive, "/") == 0)
			return false;

		const char *realDir = PHYSFS_getRealDir(archive);
		if (!realDir)
			return false;

		realPath = realDir;

		// Always disallow mounting of files inside the game source, since it
		// won't work anyway if the game source is a zipped .love file.
		if (realPath.find(gameSource) == 0)
			return false;

		realPath += LOVE_PATH_SEPARATOR;
		realPath += archive;
	}

	return mountFullPath(realPath.c_str(), mountpoint, MOUNT_PERMISSIONS_READ, appendToPath);
}

bool Filesystem::mountFullPath(const char *archive, const char *mountpoint, MountPermissions permissions, bool appendToPath)
{
	if (!PHYSFS_isInit() || !archive)
		return false;

	std::string canonarchive = canonicalizeRealPath(archive);

	if (isMounted(canonarchive))
		return false;

	if (permissions == MOUNT_PERMISSIONS_READWRITE)
	{
		// Try to mount readwrite by first attempting to mount normally.
		if (PHYSFS_mount(canonarchive.c_str(), mountpoint, appendToPath))
			return true;

		// If that fails, try to remount as readwrite by unmounting first.
		// This is a workaround for platforms where PHYSFS_mountRW is not available.
		if (PHYSFS_unmount(canonarchive.c_str()) && PHYSFS_mount(canonarchive.c_str(), mountpoint, appendToPath))
			return true;

		return false;
	}

	return PHYSFS_mount(canonarchive.c_str(), mountpoint, appendToPath) != 0;
}

bool Filesystem::mountCommonPathInternal(CommonPath path, const char *mountpoint, MountPermissions permissions, bool appendToPath, bool createDir)
{
	std::string fullpath = getFullCommonPath(path);
	if (fullpath.empty())
		return false;

	if (createDir && isAppCommonPath(path) && !isRealDirectory(fullpath))
	{
		if (!createRealDirectory(fullpath))
			return false;
	}

	if (mountFullPath(fullpath.c_str(), mountpoint, permissions, appendToPath))
	{
		std::string mp = mountpoint != nullptr ? mountpoint : "/";
		commonPathMountInfo[path] = {true, mp, permissions};
		return true;
	}

	return false;
}

bool Filesystem::mountCommonPath(CommonPath path, const char *mountpoint, MountPermissions permissions, bool appendToPath)
{
	return mountCommonPathInternal(path, mountpoint, permissions, appendToPath, true);
}

bool Filesystem::mount(Data *data, const char *archivename, const char *mountpoint, bool appendToPath)
{
	if (!PHYSFS_isInit())
		return false;

	if (isMounted(archivename))
		return false;

	if (PHYSFS_mountMemory(data->getData(), data->getSize(), nullptr, archivename, mountpoint, appendToPath) != 0)
	{
		mountedData[archivename] = data;
		return true;
	}

	return false;
}

bool Filesystem::unmount(const char *archive)
{
	if (!PHYSFS_isInit() || !archive)
		return false;

	auto datait = mountedData.find(archive);

	if (datait != mountedData.end() && PHYSFS_unmount(archive) != 0)
	{
		mountedData.erase(datait);
		return true;
	}

	auto it = std::find(allowedMountPaths.begin(), allowedMountPaths.end(), archive);
	if (it != allowedMountPaths.end())
		return unmountFullPath(archive);

	std::string sourceBase = getSourceBaseDirectory();
	if (isFused() && sourceBase.compare(archive) == 0)
		return unmountFullPath(archive);

	if (strlen(archive) == 0 || strstr(archive, "..") || strcmp(archive, "/") == 0)
		return false;

	const char *realDir = PHYSFS_getRealDir(archive);
	if (!realDir)
		return false;

	std::string realPath = realDir;
	realPath += LOVE_PATH_SEPARATOR;
	realPath += archive;

	std::string canonpath = canonicalizeRealPath(realPath.c_str());

	if (PHYSFS_getMountPoint(canonpath.c_str()) == nullptr)
		return false;

	return PHYSFS_unmount(canonpath.c_str()) != 0;
}

bool Filesystem::unmountFullPath(const char *fullpath)
{
	if (!PHYSFS_isInit() || !fullpath)
		return false;

	std::string canonpath = canonicalizeRealPath(fullpath);

	return PHYSFS_unmount(canonpath.c_str()) != 0;
}

bool Filesystem::unmount(CommonPath path)
{
	std::string fullpath = getFullCommonPath(path);

	if (!fullpath.empty() && unmountFullPath(fullpath.c_str()))
	{
		commonPathMountInfo[path].mounted = false;
		return true;
	}

	return false;
}

bool Filesystem::unmount(Data *data)
{
	for (const auto &datapair : mountedData)
	{
		if (datapair.second.get() == data)
		{
			std::string archive = datapair.first;
			return unmount(archive.c_str());
		}
	}

	return false;
}

love::filesystem::File *Filesystem::openFile(const char *filename, File::Mode mode) const
{
	return new File(filename, mode);
}

std::string Filesystem::getFullCommonPath(CommonPath path)
{
	if (!fullPaths[path].empty())
		return fullPaths[path];

	if (isAppCommonPath(path))
	{
		if (saveIdentity.empty())
			return fullPaths[path];

		std::string rootpath;
		switch (path)
		{
		case COMMONPATH_APP_SAVEDIR:
			rootpath = getFullCommonPath(COMMONPATH_USER_APPDATA);
			break;
		case COMMONPATH_APP_DOCUMENTS:
			rootpath = getFullCommonPath(COMMONPATH_USER_DOCUMENTS);
			break;
		default:
			break;
		}

		if (rootpath.empty())
			return fullPaths[path];

		std::string suffix;
		if (isFused())
			suffix = std::string(LOVE_PATH_SEPARATOR) + saveIdentity;
		else
			suffix = std::string(LOVE_PATH_SEPARATOR LOVE_APPDATA_FOLDER LOVE_PATH_SEPARATOR) + saveIdentity;

		// TODO: do we still need the normalize?
		fullPaths[path] = normalize(rootpath + suffix);
		fullPaths[path] = canonicalizeRealPath(fullPaths[path].c_str());

		return fullPaths[path];
	}
	// paths are on sd: (maybe usb: in future)
	switch (path)
	{
	case COMMONPATH_USER_HOME:
	case COMMONPATH_USER_APPDATA:
	case COMMONPATH_USER_DESKTOP:
	case COMMONPATH_USER_DOCUMENTS:
	case COMMONPATH_APP_SAVEDIR:
	case COMMONPATH_APP_DOCUMENTS:
	case COMMONPATH_MAX_ENUM: // erm
		fullPaths[path] = "sd:/";
		break;
	}

	if (!fullPaths[path].empty())
		fullPaths[path] = canonicalizeRealPath(fullPaths[path].c_str());

	return fullPaths[path];
}

const char *Filesystem::getWorkingDirectory()
{
	if (cwd.empty())
	{
		char temp[MAXPATHLEN];
		if (getcwd(temp, MAXPATHLEN) != nullptr)
		{
			cwd = std::string(temp);
		}
	}

	return cwd.c_str();
}

std::string Filesystem::getUserDirectory()
{
	return getFullCommonPath(COMMONPATH_USER_HOME);
}

std::string Filesystem::getAppdataDirectory()
{
	return getFullCommonPath(COMMONPATH_USER_APPDATA);
}

std::string Filesystem::getSaveDirectory()
{
	return getFullCommonPath(COMMONPATH_APP_SAVEDIR);
}

std::string Filesystem::getSourceBaseDirectory() const
{
	size_t source_len = gameSource.length();

	if (source_len == 0)
		return "";

	size_t base_end_pos = gameSource.find_last_of('/', source_len - 2);

	if (base_end_pos == std::string::npos)
		return "";

	// If the source is in the unix root (aka '/'), we want to keep the '/'.
	if (base_end_pos == 0)
		base_end_pos = 1;

	return gameSource.substr(0, base_end_pos);
}

std::string Filesystem::getRealDirectory(const char *filename) const
{
	if (!PHYSFS_isInit())
		throw love::Exception("PhysFS is not initialized.");

	const char *dir = PHYSFS_getRealDir(filename);

	if (dir == nullptr)
		throw love::Exception("File does not exist on disk.");

	return std::string(dir);
}

bool Filesystem::exists(const char *filepath) const
{
	if (!PHYSFS_isInit())
		return false;

	return PHYSFS_exists(filepath) != 0;
}

bool Filesystem::getInfo(const char *filepath, Info &info) const
{
	if (!PHYSFS_isInit())
		return false;

	PHYSFS_Stat stat = {};
	if (!PHYSFS_stat(filepath, &stat))
		return false;

	info.size = (int64) stat.filesize;
	info.modtime = (int64) stat.modtime;
	info.readonly = stat.readonly != 0;

	if (stat.filetype == PHYSFS_FILETYPE_REGULAR)
		info.type = FILETYPE_FILE;
	else if (stat.filetype == PHYSFS_FILETYPE_DIRECTORY)
		info.type = FILETYPE_DIRECTORY;
	else if (stat.filetype == PHYSFS_FILETYPE_SYMLINK)
		info.type = FILETYPE_SYMLINK;
	else
		info.type = FILETYPE_OTHER;

	return true;
}

bool Filesystem::createDirectory(const char *dir)
{
	if (!PHYSFS_isInit())
		return false;

	if (!setupWriteDirectory())
		return false;

	if (!PHYSFS_mkdir(dir))
		return false;

	return true;
}

bool Filesystem::remove(const char *file)
{
	if (!PHYSFS_isInit())
		return false;

	if (!setupWriteDirectory())
		return false;

	if (!PHYSFS_delete(file))
		return false;

	return true;
}

FileData *Filesystem::read(const char *filename, int64 size) const
{
	File file(filename, File::MODE_READ);

	// close() is called in the File destructor.
	return file.read(size);
}

FileData* Filesystem::read(const char* filename) const
{
	File file(filename, File::MODE_READ);

	// close() is called in the File destructor.
	return file.read();
}

void Filesystem::write(const char *filename, const void *data, int64 size) const
{
	File file(filename, File::MODE_WRITE);

	// close() is called in the File destructor.
	if (!file.write(data, size))
		throw love::Exception("Data could not be written.");
}

void Filesystem::append(const char *filename, const void *data, int64 size) const
{
	File file(filename, File::MODE_APPEND);

	// close() is called in the File destructor.
	if (!file.write(data, size))
		throw love::Exception("Data could not be written.");
}

bool Filesystem::getDirectoryItems(const char *dir, std::vector<std::string> &items)
{
	if (!PHYSFS_isInit())
		return false;

	char **rc = PHYSFS_enumerateFiles(dir);

	if (rc == nullptr)
		return false;

	for (char **i = rc; *i != 0; i++)
		items.push_back(*i);

	PHYSFS_freeList(rc);
	return true;
}

void Filesystem::setSymlinksEnabled(bool enable)
{
	if (!PHYSFS_isInit())
		return;

	PHYSFS_permitSymbolicLinks(enable ? 1 : 0);
}

bool Filesystem::areSymlinksEnabled() const
{
	if (!PHYSFS_isInit())
		return false;

	return PHYSFS_symbolicLinksPermitted() != 0;
}

std::vector<std::string> &Filesystem::getRequirePath()
{
	return requirePath;
}

std::vector<std::string> &Filesystem::getCRequirePath()
{
	return cRequirePath;
}

void Filesystem::allowMountingForPath(const std::string &path)
{
	if (std::find(allowedMountPaths.begin(), allowedMountPaths.end(), path) == allowedMountPaths.end())
		allowedMountPaths.push_back(path);
}

} // physfs
} // filesystem
} // love
