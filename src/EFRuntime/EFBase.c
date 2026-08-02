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
#include <EmexFoundation/EFRuntime/EFRuntime.h>
#include <EmexFoundation/EFString.h>

EF_HIDDEN EFClass __EFClassGetByID(EFTypeID id);

EFRange EFRangeZero = {
    .location = 0,
    .length = 0,
};

EFTypeID EFGetTypeID(EFObjectRef ref)
{
    EFObject *object = (EFObject*)ref;
    if(object == NULL)
    {
        return kEFTypeIDNone;
    }
    return object->typeID;
}

extern EFRootType EFGetRootType(EFObjectRef ref)
{
    EFObject *object = (EFObject*)ref;
    if(object == NULL)
    {
        return kEFRootTypeNotARootType;
    }
    return object->_rt;
}

Boolean EFEqual(EFObjectRef ref1,
                EFObjectRef ref2)
{
    if(ref1 == NULL || ref2 == NULL)
    {
        return false;
    }

    if(ref1 == ref2)
    {
        return true;
    }

    /* types must match */
    EFTypeID typeID = EFGetTypeID(ref1);
    if(typeID != EFGetTypeID(ref2))
    {
        return false;
    }

    EFClass class = __EFClassGetByID(typeID);
    if(class->equal != NULL)
    {
        return class->equal(ref1, ref2);
    }

    /* no handler == not equal */
    return false;
}

EFObjectRef EFRetain(EFObjectRef ref)
{
    EFObject *object = (EFObject*)ref;
    assert(object != NULL);
    if(object->_rt == kEFRootTypeStaticObject)
    {
        return ref;
    }
    atomic_fetch_add_explicit(&object->refcount, 1, memory_order_relaxed);
    return ref;
}

void EFRelease(EFObjectRef ref)
{
    EFObject *object = (EFObject*)ref;
    assert(object != NULL);
    if(object->_rt == kEFRootTypeStaticObject)
    {
        return;
    }

    /* releasing and trying to get the old reference count */
    EFIndex old = atomic_fetch_sub_explicit(&object->refcount, 1, memory_order_release);
    if(old == 1)
    {
        atomic_thread_fence(memory_order_acquire);
        /* trigger handler */
        EFClass class = __EFClassGetByID(object->typeID);
        assert(class != NULL);
        if(class->deinit != NULL)
        {
            class->deinit(ref);
        }
        EFAllocatorDeallocate(object->allocatorRef, (void*)ref);
    }
    else if(old <= 0)
    {
        fprintf(stderr, "EFRelease: fatal error occured, reference underflow\n");
        abort();
    }
}

EFObjectRef EFRetainTry(EFObjectRef ref)
{
    if(ref == NULL)
    {
        return NULL;
    }
    return EFRetain(ref);
}

Boolean EFReleaseTry(EFObjectRef ref)
{
    if(ref == NULL)
    {
        return false;
    }
    EFRelease(ref);
    return true;
}

Boolean EFReleaseTryHelper(void *ref)
{
    EFObjectRef *refp = (EFObjectRef*)ref;
    if(refp == NULL || *refp == NULL)
    {
        return false;
    }
    EFRelease(*refp);
    return true;
}

EFIndex EFGetRetainCount(EFObjectRef ref)
{
    EFObject *object = (EFObject*)ref;
    assert(object != NULL);
    if(object->_rt == kEFRootTypeStaticObject)
    {
        return 1;   /* static.. */
    }
    return atomic_load(&object->refcount);
}

EFAllocatorRef EFGetAllocator(EFObjectRef ref)
{
    EFObject *object = (EFObject*)ref;
    if(object == NULL)
    {
        return kEFAllocatorDefault;
    }
    return object->allocatorRef;
}

EFStringRef EFCopyDescription(EFObjectRef ref)
{
    EFObject *object = (EFObject*)ref;
    if(object == NULL)
    {
        return EFSTR("<nil>");
    }

    if(object->_rt == kEFRootTypeAllocator)
    {
        EFAllocator *allocator = (EFAllocator*)ref;
        EFStringRef string = EFStringCreateWithFormat(object->allocatorRef, EFSTR("<%s %p>"), allocator->name, ref);
        if(string == NULL)
        {
            return EFSTR("<unknown>");
        }
        return string;
    }
    else if(object->_rt == kEFRootTypeObject || object->_rt == kEFRootTypeStaticObject)
    {
        EFClass class = __EFClassGetByID(object->typeID);
        if(class == NULL)
        {
            return EFSTR("<nil>");
        }

        if(class->copyDescription)
        {
            EFStringRef descriptionRef = class->copyDescription(ref);
            if(descriptionRef != NULL)
            {
                return descriptionRef;
            }
        }

        EFStringRef descriptionFallbackRef = EFStringCreateWithFormat(object->allocatorRef, EFSTR("<%@ %p>"), class->header.name, ref);
        if(descriptionFallbackRef == NULL)
        {
            return EFSTR("<nil>");
        }
        return descriptionFallbackRef;
    }

    return EFSTR("<unknown>");
}

void EFLog(EFStringRef formatStringRef, ...)
{
    va_list arguments;
    va_start(arguments, formatStringRef);
    EFAUTOREL EFStringRef resultRef = EFStringCreateWithFormatAndArguments(NULL, formatStringRef, arguments);
    va_end(arguments);
    if(resultRef == NULL)
    {
        return;
    }

    const char *resultCptr = EFStringGetCStringPtr(resultRef, kEFStringEncodingUTF8);
    if(resultCptr)
    {
        fprintf(stderr, "%s", resultCptr);
    }
}
