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

#ifndef EFBITWALKER_H
#define EFBITWALKER_H

/* ----------------------------------------------------------------------
 *  EmexFoundation Headers
 * -------------------------------------------------------------------- */
#include <EmexFoundation/EFRuntime/EFRuntime.h>

typedef struct {
    EFIndex bytePos;
    UInt8 bitIndex;
} EFBitWalkerPosition;

EF_EXTERN EFTypeID EFBitWalkerGetTypeID(void);

EF_EXTERN EF_RETURNS_RETAINED EFBitWalkerRef EFBitWalkerCreateWithHandle(EFAllocatorRef allocator, EFFileHandleRef fileHandle, EFEndian endian);

EF_EXTERN void EFBitWalkerReset(EFBitWalkerRef walker);

EF_EXTERN Boolean EFBitWalkerWrite(EFBitWalkerRef walker, UInt64 value, UInt8 numBits);
EF_EXTERN UInt64 EFBitWalkerRead(EFBitWalkerRef walker, UInt8 numBits);
EF_EXTERN EFIndex EFBitWalkerWriteBuffer(EFBitWalkerRef walker, const char *buffer, EFIndex length);
EF_EXTERN EFIndex EFBitWalkerReadBuffer(EFBitWalkerRef walker, char *buffer, EFIndex length);

EF_EXTERN void EFBitWalkerSeek(EFBitWalkerRef walker, EFIndex bytePos, UInt8 bitIndex);
EF_EXTERN void EFBitWalkerSkip(EFBitWalkerRef walker, EFIndex numBits);

EF_EXTERN EFIndex EFBitWalkerBytesUsed(EFBitWalkerRef walker);
EF_EXTERN void EFBitWalkerAlignByte(EFBitWalkerRef walker);
EF_EXTERN void EFBitWalkerSync(EFBitWalkerRef walker);

EF_EXTERN EFBitWalkerPosition EFBitWalkerGetPosition(EFBitWalkerRef walker);
EF_EXTERN void EFBitWalkerSetPosition(EFBitWalkerRef walker, EFBitWalkerPosition position);

EF_EXTERN EF_RETURNS_NOT_RETAINED EFFileHandleRef EFBitWalkerGetHandle(EFBitWalkerRef walker);

#endif /* EFBITWALKER_H */
