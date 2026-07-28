/*
 * SPDX-License-Identifier: AGPL-3.0-or-later
 *
 * Copyright (C) 2026 emexlab
 *
 * This file is part of EmexFoundation.
 *
 * EmexFoundation is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EmexFoundation is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with EmexFoundation. If not, see <https://www.gnu.org/licenses/>.
 */

#if defined(__linux__)
#define _GNU_SOURCE
#endif /* __FreeBSD__ || __linux__ */

/* ----------------------------------------------------------------------
 *  System Headers
 * -------------------------------------------------------------------- */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#if defined(__FreeBSD__) || defined(__linux__)
#include <sys/mman.h>
#endif /* __FreeBSD__ || __linux__ */

/* ----------------------------------------------------------------------
 *  EmexFoundation Headers
 * -------------------------------------------------------------------- */
#include <EmexFoundation/CrossSupport/VFD.h>
#include <EmexFoundation/EFString.h>
#include <EmexFoundation/EFUUID.h>

SInt32 VFDCreate(UInt32 flags)
{
    /* creates file descriptor that "lives in memory" */
    EFAUTOREL EFUUIDRef uuid = EFUUIDCreate(kEFAllocatorDefault);
    EFAUTOREL EFStringRef string = EFUUIDCreateString(kEFAllocatorDefault, uuid);
    SInt32 fileDescriptor;

#if (defined(__FreeBSD__) || defined(__linux__)) && defined(MFD_CLOEXEC)
    fileDescriptor = memfd_create(EFStringGetCStringPtr(string, kEFStringEncodingUTF8), MFD_CLOEXEC);
    if(fileDescriptor >= 0)
    {
        return fileDescriptor;
    }
    /* fallback shall work regardless */
#endif /* (__FreeBSD__ || __linux__) && MFD_CLOEXEC */

    EFAUTOREL EFStringRef pathStr = EFStringCreateWithFormat(kEFAllocatorDefault, EFSTR("%s/%@"), getenv("TMPDIR")?: "/tmp", string);
    const char *pathStrC = EFStringGetCStringPtr(pathStr, kEFStringEncodingUTF8);
    fileDescriptor = open(pathStrC, flags | O_CREAT | O_TRUNC, 0777);
    unlink(pathStrC);   /* unlinking immediately keeps it in memory */
    return fileDescriptor;
}
