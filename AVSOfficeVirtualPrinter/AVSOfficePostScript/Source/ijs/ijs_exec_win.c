/**
 * Copyright (C) 2001-2002 Artifex Software, Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#define STRICT
#include <windows.h>
#include <io.h>
#include <process.h>
#include "ijs.h"
#include "ijs_client.h"

/* Start child program with redirected standard input and output */
int 
ijs_exec_server(const char *server_cmd, int *pfd_to, int *pfd_from, 
    int *pchild_pid)
{
    SECURITY_ATTRIBUTES saAttr;
    STARTUPINFO siStartInfo;
    LPVOID env;
    HANDLE hPipeTemp;
    HANDLE hChildStdinRd = INVALID_HANDLE_VALUE;
    HANDLE hChildStdinWr = INVALID_HANDLE_VALUE;
    HANDLE hChildStdoutRd = INVALID_HANDLE_VALUE;
    HANDLE hChildStdoutWr = INVALID_HANDLE_VALUE;
    PROCESS_INFORMATION piProcInfo;
    BOOL flag;
    int fd_stdin_wr = -1;
    int fd_stdout_rd = -1;

    /* Set the bInheritHandle flag so pipe handles are inherited. */
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    /* Create anonymous inheritable pipes for STDIN and STDOUT
     * for child. For each pipe, create a noninheritable duplicate handle
     * of our end of the pipe, then close the inheritable handle.
     * Do not redirect STDERR.
     */
    flag = CreatePipe(&hChildStdinRd, &hPipeTemp, &saAttr, 0);
    if (flag) {
        flag = DuplicateHandle(GetCurrentProcess(), hPipeTemp,
            GetCurrentProcess(), &hChildStdinWr, 0,
            FALSE,       /* not inherited */
            DUPLICATE_SAME_ACCESS);
	CloseHandle(hPipeTemp);
    }
    if (flag)
	flag = CreatePipe(&hPipeTemp, &hChildStdoutWr, &saAttr, 0);
    if (flag) {
        flag = DuplicateHandle(GetCurrentProcess(), hPipeTemp,
            GetCurrentProcess(), &hChildStdoutRd, 0,
            FALSE,       /* not inherited */
            DUPLICATE_SAME_ACCESS);
        CloseHandle(hPipeTemp);
    }
    if (flag)
	flag = (fd_stdin_wr = _open_osfhandle((LONG)hChildStdinWr, 0)) != -1;
    if (flag)
	flag = (fd_stdout_rd = _open_osfhandle((LONG)hChildStdoutRd, 0)) != -1;

    /* Now create the child process. */
    if (flag) {
	/* Set up members of STARTUPINFO structure. */
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.lpReserved = NULL;
	siStartInfo.lpDesktop = NULL;
	siStartInfo.lpTitle = NULL;  /* use executable name as title */
	siStartInfo.dwX = siStartInfo.dwY = CW_USEDEFAULT;	/* ignored */
	siStartInfo.dwXSize = siStartInfo.dwYSize = CW_USEDEFAULT;/* ignored */
	siStartInfo.dwXCountChars = 80;
	siStartInfo.dwYCountChars = 25;
	siStartInfo.dwFillAttribute = 0;			/* ignored */
	siStartInfo.dwFlags = STARTF_USESTDHANDLES;
#ifdef VERBOSE
	siStartInfo.wShowWindow = SW_SHOWNORMAL;
	siStartInfo.dwFlags |= STARTF_USESHOWWINDOW;
#else
	siStartInfo.wShowWindow = SW_HIDE;
#endif
	siStartInfo.cbReserved2 = 0;
	siStartInfo.lpReserved2 = NULL;
	siStartInfo.hStdInput  = hChildStdinRd;
	siStartInfo.hStdOutput = hChildStdoutWr;
	siStartInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);

	env = NULL;

	/* Create the child process. */

	flag = CreateProcess(server_cmd,
	    NULL,  	   /* command line                       */
	    NULL,          /* process security attributes        */
	    NULL,          /* primary thread security attributes */
	    TRUE,          /* handles are inherited              */
	    0,             /* creation flags                     */
	    env,           /* environment                        */
	    NULL,          /* use parent's current directory     */
	    &siStartInfo,  /* STARTUPINFO pointer                */
	    &piProcInfo);  /* receives PROCESS_INFORMATION  */
	if (flag) {
	    CloseHandle(piProcInfo.hProcess);
	    CloseHandle(piProcInfo.hThread);
	}
    }

    if (hChildStdinRd != INVALID_HANDLE_VALUE)
        CloseHandle(hChildStdinRd);
    if (hChildStdoutWr != INVALID_HANDLE_VALUE)
	CloseHandle(hChildStdoutWr);

    if (flag) {
	*pfd_to = fd_stdin_wr;
	*pfd_from = fd_stdout_rd;
	*pchild_pid = (int)piProcInfo.dwProcessId;
    }
    else {
	if (fd_stdin_wr != -1)
	    close(fd_stdin_wr);
	else if (hChildStdinWr != INVALID_HANDLE_VALUE)
	    CloseHandle(hChildStdinWr);
	if (fd_stdout_rd != -1)
	    close(fd_stdout_rd);
	else if (hChildStdoutRd != INVALID_HANDLE_VALUE)
	    CloseHandle(hChildStdoutRd);
	return -1;
    }
    return flag ? 0 : -1;
}


