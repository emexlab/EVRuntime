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

#ifndef EFPROCESS_H
#define EFPROCESS_H

/* ----------------------------------------------------------------------
 *  EmexFoundation Headers
 * -------------------------------------------------------------------- */
#include <EmexFoundation/EFRuntime/EFRuntime.h>

EF_EXTERN EFTypeID EFProcessGetTypeID(void);

EF_EXTERN EF_RETURNS_NOT_RETAINED EFProcessRef EFProcessGetCurrentProcess(void);

EF_EXTERN EF_RETURNS_RETAINED EFProcessRef EFProcessCreateWithCommand(EFAllocatorRef allocator, EFStringRef command, EFArrayRef arguments);
EF_EXTERN EF_RETURNS_RETAINED EFProcessRef EFProcessCreateWithPath(EFAllocatorRef allocator, EFStringRef path, EFArrayRef arguments);
EF_EXTERN EF_RETURNS_RETAINED EFProcessRef EFProcessCreateWithProcessIdentifier(EFAllocatorRef allocator, SInt32 processIdentifier);

EF_EXTERN SInt32 EFProcessGetProcessIdentifier(EFProcessRef process);
EF_EXTERN SInt32 EFProcessGetParentProcessIdentifier(EFProcessRef process);
EF_EXTERN SInt32 EFProcessGetUserIdentifier(EFProcessRef process);
EF_EXTERN SInt32 EFProcessGetGroupIdentifier(EFProcessRef process);
EF_EXTERN SInt32 EFProcessGetProcessGroupIdentifier(EFProcessRef process);
EF_EXTERN SInt32 EFProcessGetSessionIdentifier(EFProcessRef process);

EF_EXTERN EF_RETURNS_RETAINED EFStringRef EFProcessCopyUserName(EFAllocatorRef allocator, EFProcessRef process);
EF_EXTERN EF_RETURNS_RETAINED EFStringRef EFProcessCopyGroupName(EFAllocatorRef allocator, EFProcessRef process);

EF_EXTERN Boolean EFProcessSendSignal(EFProcessRef process, SInt32 signal);
EF_EXTERN Boolean EFProcessSuspend(EFProcessRef process);
EF_EXTERN Boolean EFProcessResume(EFProcessRef process);
EF_EXTERN Boolean EFProcessTerminate(EFProcessRef process);
EF_EXTERN Boolean EFProcessForceKill(EFProcessRef process);

EF_EXTERN Boolean EFProcessIsAlive(EFProcessRef process);

EF_EXTERN EF_RETURNS_NOT_RETAINED EFStringRef EFProcessGetCommand(EFProcessRef process);
EF_EXTERN EF_RETURNS_NOT_RETAINED EFStringRef EFProcessGetExecutablePath(EFProcessRef process);
EF_EXTERN EF_RETURNS_NOT_RETAINED EFArrayRef EFProcessGetArguments(EFProcessRef process);

EF_EXTERN SInt32 EFProcessWaitPID(EFProcessRef process, SInt32 *status, SInt32 options);

#endif /* EFPROCESS_H */
