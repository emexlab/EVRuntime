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

#ifndef EFFILEHANDLE_H
#define EFFILEHANDLE_H

/* ----------------------------------------------------------------------
 *  System Headers
 * -------------------------------------------------------------------- */
#include <fcntl.h>

/* ----------------------------------------------------------------------
 *  EmexFoundation Headers
 * -------------------------------------------------------------------- */
#include <EmexFoundation/EFRuntime/EFRuntime.h>

typedef enum: UInt8 {
    kEFFileHandleSeekTypeSet,
    kEFFileHandleSeekTypeCur,
    kEFFileHandleSeekTypeEnd,
    /* fuck darwin no SEEK_HOLE (only Torvalds should seek the hole of his mom right back into the birth canal he came out of) */
} EFFileHandleSeekType;

EF_EXTERN EFTypeID EFFileHandleGetTypeID(void);

EF_EXTERN EF_RETURNS_RETAINED EFFileHandleRef EFFileHandleCreate(EFAllocatorRef allocator);
EF_EXTERN EF_RETURNS_RETAINED EFFileHandleRef EFFileHandleCreateWithFileDescriptor(EFAllocatorRef allocator, SInt32 fd);
EF_EXTERN EF_RETURNS_RETAINED EFFileHandleRef EFFileHandleCreateWithPathAndOptions(EFAllocatorRef allocator, EFStringRef pathString, SInt32 flg, ...);
EF_EXTERN EF_RETURNS_RETAINED EFFileHandleRef EFFileHandleCreateWithURLAndOptions(EFAllocatorRef allocator, EFURLRef url, SInt32 flg, ...);
EF_EXTERN EF_RETURNS_RETAINED EFFileHandleRef EFFileHandleCreateCopy(EFAllocatorRef allocator, EFFileHandleRef fileHandle);

EF_EXTERN EF_RETURNS_RETAINED EFDataRef EFFileHandleReadData(EFFileHandleRef fileHandle, EFIndex length);
EF_EXTERN Boolean EFFileHandleWriteData(EFFileHandleRef fileHandle, EFDataRef data);

EF_EXTERN EFIndex EFFileHandleRead(EFFileHandleRef fileHandle, UInt8 *buffer, EFIndex length);
EF_EXTERN EFIndex EFFileHandleWrite(EFFileHandleRef fileHandle, const UInt8 *buffer, EFIndex length);
EF_EXTERN EFIndex EFFileHandleTruncate(EFFileHandleRef fileHandle, EFIndex length);

EF_EXTERN EFIndex EFFileHandleSeek(EFFileHandleRef fileHandle, EFIndex offset, EFFileHandleSeekType seekType);
EF_EXTERN void EFFileHandleSync(EFFileHandleRef fileHandle);

EF_EXTERN EFIndex EFFileHandleGetLength(EFFileHandleRef fileHandle);
EF_EXTERN Boolean EFFileHandleIsReadable(EFFileHandleRef fileHandle);
EF_EXTERN Boolean EFFileHandleIsWritable(EFFileHandleRef fileHandle);

EF_EXTERN EF_RETURNS_RETAINED EFDataRef EFFileHandleCopyDataForRange(EFAllocatorRef allocator, EFFileHandleRef fileHandle, EFRange range);

EF_EXTERN char *EFFileHandleGets(EFFileHandleRef fileHandle, char *s, SInt32 n);
EF_EXTERN void EFFileHandlePutc(EFFileHandleRef fileHandle, char c);
EF_EXTERN void EFFileHandlePuts(EFFileHandleRef fileHandle, const char *s);
EF_EXTERN void EFFileHandlePrintf(EFFileHandleRef fileHandle, const char *format, ...);

EF_EXTERN SInt32 EFFileHandleGetFileDescriptor(EFFileHandleRef fileHandle);

EF_EXTERN EF_RETURNS_RETAINED EFMappingRef EFFileHandleCopyMapping(EFAllocatorRef allocator, EFFileHandleRef fileHandle);

#endif /* EFFILEHANDLE_H */
