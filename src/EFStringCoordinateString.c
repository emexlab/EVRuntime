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
 *  EmexFoundation Headers
 * -------------------------------------------------------------------- */
#include <EmexFoundation/EFRuntime/EFRuntime.h>
#include <EmexFoundation/EFStringCoordinateString.h>
#include <EmexFoundation/EFStringCoordinateSpace.h>

typedef struct __EFStringCoordinateString {
    EFObject header;
    
    EFStringCoordinateSpaceRef coordinateSpace;
    EFStringRef virtualString;
    EFRange physicalRange;  /* range of source string */
} *__EFStringCoordinateString;

static void __EFStringCoordinateStringDeinit(EFObjectRef coordinateStringRef)
{
    EFStringCoordinateStringRef coordinateString = (EFStringCoordinateStringRef)coordinateStringRef;
    EFRelease(coordinateString->coordinateSpace);
    EFRelease(coordinateString->virtualString);
}

EF_HIDDEN EFClassDefinitionNewest EFStringCoordinateStringClass = {
    .header = {
        .version = EFCLASS_NEWEST_VERSION,
        .typeID = kEFTypeIDStringCoordinateString,
        .name = EFSTR_FILESCOPE("EFStringCoordinateString"),
    },
    .init = NULL,
    .deinit = NULL,
    .equal = NULL,
    .copyDescription = NULL,
    .hash = NULL,
};

EFTypeID EFStringCoordinateStringGetTypeID(void)
{
    return kEFTypeIDStringCoordinateString;
}

EF_HIDDEN EF_RETURNS_RETAINED EFStringCoordinateStringRef EFStringCoordinateStringCreate(EFAllocatorRef allocator,
                                                                                         EFStringCoordinateSpaceRef coordinateSpace,
                                                                                         EFStringRef virtualString,
                                                                                         EFRange physicalRange)
{
    if(coordinateSpace == NULL || virtualString == NULL)
    {
        return NULL;
    }

    EFStringCoordinateStringRef coordinateString = (EFStringCoordinateStringRef)EFObjectCreate(allocator, EFStringCoordinateStringGetTypeID(), (EFIndex)sizeof(struct __EFStringCoordinateString));
    if(coordinateString == NULL)
    {
        return NULL;
    }

    coordinateString->coordinateSpace = EFRetain(coordinateSpace);
    if(coordinateString->coordinateSpace == NULL)
    {
        EFRelease(coordinateString);
        return NULL;
    }

    coordinateString->virtualString = EFRetain(virtualString);
    if(coordinateString->virtualString == NULL)
    {
        EFRelease(coordinateString->coordinateSpace);
        EFRelease(coordinateString);
        return NULL;
    }

    coordinateString->physicalRange = physicalRange;
    return coordinateString;
}
