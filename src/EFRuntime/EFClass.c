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
#include <pthread.h>

/* ----------------------------------------------------------------------
 *  EmexFoundation Headers
 * -------------------------------------------------------------------- */
#include <EmexFoundation/EFRuntime/EFClass.h>
#include <EmexFoundation/EFRuntime/EFAllocator.h>

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

static pthread_mutex_t efClassLock = PTHREAD_MUTEX_INITIALIZER;
static EFClass *efClassTable = NULL;
static EFTypeID efClassNext = kEFTypeIDDictionary;
static EFSize efClassCapacity = 0;

static Boolean __EFClassTableExtendIfNeeded(void)
{
    if(pthread_mutex_trylock(&efClassLock) == 0)
    {
        pthread_mutex_unlock(&efClassLock);
        return false;
    }

    if(efClassTable == NULL)
    {
        efClassTable = EFAllocatorAllocate(kEFAllocatorDefault, sizeof(EFClass) * 1024, 0);
        if(efClassTable == NULL)
        {
            return false;
        }

        efClassTable[0] = NULL;
        efClassTable[1] = &EFStringClass;
        efClassTable[2] = &EFNumberClass;
        efClassTable[3] = &EFURLClass;
        efClassTable[4] = &EFUUIDClass;
        efClassTable[5] = &EFDataClass;
        efClassTable[6] = &EFFileHandleClass;
        efClassTable[7] = &EFFileClass;
        efClassTable[8] = &EFBitWalkerClass;
        efClassTable[9] = &EFMappingClass;
        efClassTable[10] = &EFProcessClass;
        efClassTable[11] = &EFMallocBlockClass;
        efClassTable[12] = &EFArrayClass;
        efClassTable[13] = NULL;
        efClassCapacity = 1024;
    }
    else if(efClassCapacity < efClassNext)
    {
        void *newp = EFAllocatorReallocate(kEFAllocatorDefault, efClassTable, sizeof(EFClass) * (efClassCapacity + 1024), 0);
        if(newp == NULL)
        {
            return false;
        }
        efClassTable = newp;
        efClassCapacity += 1024;
    }

    return true;
}

EF_HIDDEN EFClass __EFClassGetByID(EFTypeID id)
{
    pthread_mutex_lock(&efClassLock);
    if((efClassTable == NULL && !__EFClassTableExtendIfNeeded()) || id > efClassNext)
    {
        pthread_mutex_unlock(&efClassLock);
        return NULL;
    }
    EFClass class = efClassTable[id];
    pthread_mutex_unlock(&efClassLock);
    return class;
}

EFTypeID EFClassRegister(EFClassDefinition *classDefinition)
{
    assert(classDefinition != NULL);

    pthread_mutex_lock(&efClassLock);

    EFTypeID id = efClassNext + 1;
    if(id == 0 || !__EFClassTableExtendIfNeeded())
    {
        pthread_mutex_unlock(&efClassLock);
        return kEFTypeIDNone;
    }
    efClassNext++;

    classDefinition->header.typeID = id;
    efClassTable[id] = classDefinition;

    pthread_mutex_unlock(&efClassLock);
    return id;
}
