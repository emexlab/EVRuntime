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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdatomic.h>

/* ----------------------------------------------------------------------
 *  EmexFoundation Headers
 * -------------------------------------------------------------------- */
#include <EmexFoundation/EFRuntime/EFClass.h>

extern EFClassDefinition EFStringClass;
extern EFClassDefinition EFNumberClass;
extern EFClassDefinition EFURLClass;
extern EFClassDefinition EFUUIDClass;
extern EFClassDefinition EFDataClass;
extern EFClassDefinition EFFileHandleClass;
extern EFClassDefinition EFFileClass;
extern EFClassDefinition EFBitWalkerClass;
extern EFClassDefinition EFMappingClass;
extern EFClassDefinition EFProcessClass;
extern EFClassDefinition EFMallocBlockClass;
extern EFClassDefinition EFArrayClass;

static _Atomic(EFClass) ev_class_table[EFCLASS_MAX] = {
    NULL,
    &EFStringClass,
    &EFNumberClass,
    &EFURLClass,
    &EFUUIDClass,
    &EFDataClass,
    &EFFileHandleClass,
    &EFFileClass,
    &EFBitWalkerClass,
    &EFMappingClass,
    &EFProcessClass,
    &EFMallocBlockClass,
    &EFArrayClass,
    NULL,                   /* dictionary is unimplemented, because we first need hashing */
};
static _Atomic(EFTypeID) ev_class_next = kEFTypeIDDictionary;

EF_HIDDEN EFClass __EFClassGetByID(EFTypeID id)
{
    if(id >= EFCLASS_MAX)
    {
        return NULL;
    }
    return atomic_load_explicit(&ev_class_table[id], memory_order_acquire);
}

EFTypeID EFClassRegister(EFClassDefinition *classDefinition)
{
    assert(classDefinition != NULL);
    EFTypeID id = atomic_fetch_add_explicit(&ev_class_next, 1, memory_order_relaxed);
    if(id >= EFCLASS_MAX)
    {
        return kEFTypeIDNone;
    }

    classDefinition->header.typeID = id;
    atomic_store_explicit(&ev_class_table[id], classDefinition, memory_order_release);
    return id;
}
