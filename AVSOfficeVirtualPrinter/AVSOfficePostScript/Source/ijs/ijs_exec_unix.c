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

#include "unistd_.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "ijs.h"
#include "ijs_client.h"

int 
ijs_exec_server(const char *server_cmd, int *pfd_to, int *pfd_from, 
    int *pchild_pid)
{
  int fds_to[2], fds_from[2];
  int child_pid;

  if (pipe (fds_to) < 0)
    return -1;
  if (pipe (fds_from) < 0)
    {
      close (fds_to[0]);
      close (fds_to[1]);
      return -1;
    }

  child_pid = fork ();
  if (child_pid < 0)
    {
      close (fds_to[0]);
      close (fds_to[1]);
      close (fds_from[0]);
      close (fds_from[1]);
      return -1;
    }
  if (child_pid == 0)
    {
      int status;
      char *argv[8];
      int i = 0;

      close (fds_to[1]);
      close (fds_from[0]);

      dup2 (fds_to[0], STDIN_FILENO);
      dup2 (fds_from[1], STDOUT_FILENO);
#define noGDB
#ifdef GDB
      argv[i++] = "gdb";
#endif

      argv[i++] = "sh";
      argv[i++] = "-c";

      argv[i++] = (char *)server_cmd;
      argv[i++] = NULL;
      status = execvp (argv[0], argv);
      if (status < 0)
	exit (1);
    }

  /* Ignore SIGPIPE signals. This is the behaviour you'll want most of
     the time, as the attempt to read or write to the pipe will fail
     and the client will find out then. If the client needs to preserve
     the SIGPIPE signal, it can either install its own signal handler
     after this call, or hack the code.
  */
  signal (SIGPIPE, SIG_IGN);

#ifdef VERBOSE
  fprintf (stderr, "child_pid = %d; %d %d\n",
	   child_pid, fds_to[0], fds_from[1]);
#endif
  close (fds_to[0]);
  close (fds_from[1]);

  *pfd_to = fds_to[1];
  *pfd_from = fds_from[0];
  *pchild_pid = child_pid;

  return 0;
}

