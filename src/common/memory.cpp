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

#include "config.h"
#include "memory.h"

#include <stdlib.h>

/* #include <unistd.h> // Assume POSIX support. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
// wii memory

namespace love
{

bool alignedMalloc(void **mem, size_t size, size_t alignment)
{
	if (alignment & (alignment - 1))
		return false; // alignment is not a power of two

	*mem = memalign(alignment, size);
	return *mem != nullptr;
}

void alignedFree(void *mem)
{
	free(mem);
}

size_t getPageSize()
{
	/* static const long size = sysconf(_SC_PAGESIZE);
	return size > 0 ? (size_t) size : 4096; */
	return 4096; // just an estimate
}

size_t alignUp(size_t size, size_t alignment)
{
	return (size + alignment - 1) & (~(alignment - 1));
}

} // love
