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
#include <EmexFoundation/EFStringCoordinateSpace.h>

typedef struct __EFStringCoordinateSpace {
    EFObject header;

    EFStringRef spaceString;
} *__EFStringCoordinateSpace;

static void __EFStringCoordinateSpaceDeinit(EFObjectRef coordinateSpaceRef)
{
    EFStringCoordinateSpaceRef coordinateSpace = (EFStringCoordinateSpaceRef)coordinateSpaceRef;
    EFReleaseTry(coordinateSpace->spaceString);
}

EF_HIDDEN EFClassDefinitionNewest EFStringCoordinateSpaceClass = {
    .header = {
        .version = EFCLASS_NEWEST_VERSION,
        .typeID = kEFTypeIDStringCoordinateSpace,
        .name = EFSTR_FILESCOPE("EFStringCoordinateSpace"),
    },
    .init = NULL,
    .deinit = __EFStringCoordinateSpaceDeinit,
    .equal = NULL,
    .copyDescription = NULL,
    .hash = NULL,
};

EFTypeID EFStringCoordinateSpaceGetTypeID(void)
{
    return kEFTypeIDStringCoordinateSpace;
}

EFStringCoordinateSpaceRef EFStringCoordinateSpaceCreateWithString(EFAllocatorRef allocator,
                                                                   EFStringRef string)
{
    if(string == NULL)
    {
        return NULL;
    }

    EFStringCoordinateSpaceRef coordinateSpace = (EFStringCoordinateSpaceRef)EFObjectCreate(allocator, EFStringCoordinateSpaceGetTypeID(), (EFIndex)sizeof(struct __EFStringCoordinateSpace));
    if(coordinateSpace == NULL)
    {
        return NULL;
    }

    coordinateSpace->spaceString = EFStringCreateCopy(allocator, string);
    if(coordinateSpace->spaceString == NULL)
    {
        EFRelease(coordinateSpace);
        return NULL;
    }

    return coordinateSpace;
}

EFStringCoordinateStringRef EFStringCoordinateSpaceCreateCoordinateString(EFAllocatorRef allocator,
                                                                          EFRange range)
{
    return NULL;
}
