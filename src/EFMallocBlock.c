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

/* ----------------------------------------------------------------------
 *  System Headers
 * -------------------------------------------------------------------- */
#include <pthread.h>
#include <assert.h>

/* ----------------------------------------------------------------------
 *  EmexFoundation Headers
 * -------------------------------------------------------------------- */
#include <EmexFoundation/EFMallocBlock.h>

typedef struct __EFMallocBlock {
    EFObject super;
    EFSize size;
    EFObjectDeinitCallback deinitCallback;
} *__EFMallocBlock;

static void __EFMallocBlockDeinit(EFObjectRef blockRef)
{
    __EFMallocBlock block = (__EFMallocBlock)blockRef;
    if(block->deinitCallback != NULL)
    {
        block->deinitCallback(blockRef);
    }
}

static EFClassDefinition EFMallocBlockClass = {
    .name = "EFMallocBlock",
    .typeID = kEFNotATypeID,
    .init = NULL,
    .deinit = __EFMallocBlockDeinit,
    .equal = NULL,
    .copyDescription = NULL,
    .hash = NULL,
};

static void EFMallocBlockRegisterClass(void)
{
    EFClassRegister(&EFMallocBlockClass);
}

EFTypeID EFMallocBlockGetTypeID(void)
{
    static pthread_once_t once;
    pthread_once(&once, EFMallocBlockRegisterClass);
    return EFMallocBlockClass.typeID;
}

EFMallocBlockRef EFMallocBlockCreate(EFAllocatorRef allocatorRef,
                                     EFSize size)
{
    return EFMallocBlockCreateWithDeinitHandler(allocatorRef, size, NULL);
}

EFMallocBlockRef EFMallocBlockCreateWithDeinitHandler(EFAllocatorRef allocatorRef,
                                                      EFSize size,
                                                      EFObjectDeinitCallback deinitCallback)
{
    assert(size > sizeof(struct __EFMallocBlock));

    __EFMallocBlock block = (__EFMallocBlock)EFObjectCreate(allocatorRef, EFMallocBlockGetTypeID(), size);
    if(block == NULL)
    {
        return NULL;
    }

    block->size = size;
    block->deinitCallback = deinitCallback;

    return (EFMallocBlockRef)block;
}

EFSize EFMallocBlockGetSize(EFMallocBlockRef blockRef)
{
    __EFMallocBlock block = (__EFMallocBlock)blockRef;
    if(block == NULL)
    {
        return 0;
    }

    return block->size;
}
