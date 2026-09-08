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

#ifndef EFDICTIONARY_H
#define EFDICTIONARY_H

/* ----------------------------------------------------------------------
 *  EmexFoundation Headers
 * -------------------------------------------------------------------- */
#include <EmexFoundation/EFRuntime/EFRuntime.h>

#define kEFDictionaryMetadataEmpty 0x00
#define kEFDictionaryMetadataDeleted 0x01
#define __EFDictionaryMetadataFull(h) ((UInt8)(0x80 | ((h) & 0x7F)))

typedef const void *(*EFDictionaryRetainCallback)(EFAllocatorRef allocatorRef, const void *value);
typedef void (*EFDictionaryReleaseCallback)(EFAllocatorRef allocatorRef, const void *value);
typedef EFStringRef (*EFDictionaryCopyDescriptionCallback)(const void *value);
typedef Boolean (*EFDictionaryEqualCallback)(const void *value1, const void *value2);
typedef EFHashCode (*EFDictionaryHashCallback)(const void *value);

typedef struct {
    EFDictionaryRetainCallback retain;
    EFDictionaryReleaseCallback release;
    EFDictionaryCopyDescriptionCallback copyDescription;
    EFDictionaryEqualCallback equal;
    EFDictionaryHashCallback hash;
} EFDictionaryKeyCallbacks;

typedef struct {
    EFDictionaryRetainCallback retain;
    EFDictionaryReleaseCallback release;
    EFDictionaryCopyDescriptionCallback copyDescription;
    EFDictionaryEqualCallback equal;
} EFDictionaryValueCallbacks;

extern const EFDictionaryKeyCallbacks kEFDictionaryKeyCallbacksObjectCallbacks;
extern const EFDictionaryKeyCallbacks kEFDictionaryKeyCallbacksCopyStringCallbacks;
extern const EFDictionaryValueCallbacks kEFDictionaryValueCallbacksObjectCallbacks;

EF_EXTERN EFTypeID EFDictionaryGetTypeID(void);

#endif /* EFDICTIONARY_H */
