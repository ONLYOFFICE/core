// proclimits.h must be included in .cpp file
// only after #if[n]def _RWSTD_NO_SETRLIMIT line

/***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  License); you may  not use  this file
 * except in  compliance with the License.   You may obtain  a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the  License is distributed on an  "AS IS" BASIS,
 * WITHOUT  WARRANTIES OR CONDITIONS  OF ANY  KIND, either  express or
 * implied.   See  the License  for  the  specific language  governing
 * permissions and limitations under the License.
 *
 * Copyright 1999-2007 Rogue Wave Software, Inc.
 * 
 **************************************************************************/

#if !defined (_RWSTD_NO_SETRLIMIT)

#  include <sys/resource.h>   // for setrlimit()

void limit_memory (size_t limit)
{
    struct rlimit rl = { limit, limit };
    setrlimit (RLIMIT_DATA, &rl);
}

#elif defined (_WIN32)

#  ifndef _WIN32_WINNT
#    define _WIN32_WINNT 0x0500
#  endif

#  if _WIN32_WINNT >= 0x0500

// Job Objects available only in Windows 2000 and later

#    include <windows.h>

void limit_memory (size_t limit)
{
    if (sizeof(size_t) < 8)
        return;

    SYSTEM_INFO info;
    GetSystemInfo (&info);

    HANDLE hJob = CreateJobObject (NULL, NULL);
    if (hJob) {
        if (AssignProcessToJobObject (hJob, GetCurrentProcess ())) {
            JOBOBJECT_EXTENDED_LIMIT_INFORMATION job_info = { 0 };
            job_info.BasicLimitInformation.LimitFlags =
                JOB_OBJECT_LIMIT_PROCESS_MEMORY;
            job_info.ProcessMemoryLimit =
                limit > info.dwPageSize ? limit : info.dwPageSize;
            SetInformationJobObject (hJob,
                                     JobObjectExtendedLimitInformation,
                                     &job_info, sizeof (job_info));
        }
        CloseHandle (hJob);
    }
}

#  else   // _WIN32_WINNT < 0x0500

void limit_memory (size_t) { /* no-op */ }

#  endif  // _WIN32_WINNT

#else   // _RWSTD_NO_SETRLIMIT && !_WIN32

void limit_memory (size_t) { /* no-op */ }

#endif  // _RWSTD_NO_SETRLIMIT
