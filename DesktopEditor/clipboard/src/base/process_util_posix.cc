// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iterator>
#include <limits>
#include <set>

#include "base/allocator/type_profiler_control.h"
#include "base/command_line.h"
#include "base/compiler_specific.h"
#include "base/debug/debugger.h"
#include "base/debug/stack_trace.h"
#include "base/file_util.h"
#include "base/files/dir_reader_posix.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/posix/eintr_wrapper.h"
#include "base/process_util.h"
#include "base/stringprintf.h"
#include "base/synchronization/waitable_event.h"
#include "base/third_party/dynamic_annotations/dynamic_annotations.h"
#include "base/threading/platform_thread.h"
#include "base/threading/thread_restrictions.h"

#if defined(OS_CHROMEOS)
#include <sys/ioctl.h>
#endif

#if defined(OS_FREEBSD)
#include <sys/event.h>
#include <sys/ucontext.h>
#endif

#if defined(OS_MACOSX)
#include <crt_externs.h>
#include <sys/event.h>
#else
extern char** environ;
#endif

namespace base {

namespace {

// Get the process's "environment" (i.e. the thing that setenv/getenv
// work with).
char** GetEnvironment() {
#if defined(OS_MACOSX)
  return *_NSGetEnviron();
#else
  return environ;
#endif
}

// Set the process's "environment" (i.e. the thing that setenv/getenv
// work with).
void SetEnvironment(char** env) {
#if defined(OS_MACOSX)
  *_NSGetEnviron() = env;
#else
  environ = env;
#endif
}

int WaitpidWithTimeout(ProcessHandle handle, int64 wait_milliseconds,
                       bool* success) {
  // This POSIX version of this function only guarantees that we wait no less
  // than |wait_milliseconds| for the process to exit.  The child process may
  // exit sometime before the timeout has ended but we may still block for up
  // to 256 milliseconds after the fact.
  //
  // waitpid() has no direct support on POSIX for specifying a timeout, you can
  // either ask it to block indefinitely or return immediately (WNOHANG).
  // When a child process terminates a SIGCHLD signal is sent to the parent.
  // Catching this signal would involve installing a signal handler which may
  // affect other parts of the application and would be difficult to debug.
  //
  // Our strategy is to call waitpid() once up front to check if the process
  // has already exited, otherwise to loop for wait_milliseconds, sleeping for
  // at most 256 milliseconds each time using usleep() and then calling
  // waitpid().  The amount of time we sleep starts out at 1 milliseconds, and
  // we double it every 4 sleep cycles.
  //
  // usleep() is speced to exit if a signal is received for which a handler
  // has been installed.  This means that when a SIGCHLD is sent, it will exit
  // depending on behavior external to this function.
  //
  // This function is used primarily for unit tests, if we want to use it in
  // the application itself it would probably be best to examine other routes.
  int status = -1;
  pid_t ret_pid = HANDLE_EINTR(waitpid(handle, &status, WNOHANG));
  static const int64 kMaxSleepInMicroseconds = 1 << 18;  // ~256 milliseconds.
  int64 max_sleep_time_usecs = 1 << 10;  // ~1 milliseconds.
  int64 double_sleep_time = 0;

  // If the process hasn't exited yet, then sleep and try again.
  TimeTicks wakeup_time = TimeTicks::Now() +
      TimeDelta::FromMilliseconds(wait_milliseconds);
  while (ret_pid == 0) {
    TimeTicks now = TimeTicks::Now();
    if (now > wakeup_time)
      break;
    // Guaranteed to be non-negative!
    int64 sleep_time_usecs = (wakeup_time - now).InMicroseconds();
    // Sleep for a bit while we wait for the process to finish.
    if (sleep_time_usecs > max_sleep_time_usecs)
      sleep_time_usecs = max_sleep_time_usecs;

    // usleep() will return 0 and set errno to EINTR on receipt of a signal
    // such as SIGCHLD.
    usleep(sleep_time_usecs);
    ret_pid = HANDLE_EINTR(waitpid(handle, &status, WNOHANG));

    if ((max_sleep_time_usecs < kMaxSleepInMicroseconds) &&
        (double_sleep_time++ % 4 == 0)) {
      max_sleep_time_usecs *= 2;
    }
  }

  if (success)
    *success = (ret_pid != -1);

  return status;
}

#if !defined(OS_LINUX) || \
    (!defined(__i386__) && !defined(__x86_64__) && !defined(__arm__))
void ResetChildSignalHandlersToDefaults() {
  // The previous signal handlers are likely to be meaningless in the child's
  // context so we reset them to the defaults for now. http://crbug.com/44953
  // These signal handlers are set up at least in browser_main_posix.cc:
  // BrowserMainPartsPosix::PreEarlyInitialization and stack_trace_posix.cc:
  // EnableInProcessStackDumping.
  signal(SIGHUP, SIG_DFL);
  signal(SIGINT, SIG_DFL);
  signal(SIGILL, SIG_DFL);
  signal(SIGABRT, SIG_DFL);
  signal(SIGFPE, SIG_DFL);
  signal(SIGBUS, SIG_DFL);
  signal(SIGSEGV, SIG_DFL);
  signal(SIGSYS, SIG_DFL);
  signal(SIGTERM, SIG_DFL);
}

#else

// TODO(jln): remove the Linux special case once kernels are fixed.

// Internally the kernel makes sigset_t an array of long large enough to have
// one bit per signal.
typedef uint64_t kernel_sigset_t;

// This is what struct sigaction looks like to the kernel at least on X86 and
// ARM. MIPS, for instance, is very different.
struct kernel_sigaction {
  void* k_sa_handler;  // For this usage it only needs to be a generic pointer.
  unsigned long k_sa_flags;
  void* k_sa_restorer;  // For this usage it only needs to be a generic pointer.
  kernel_sigset_t k_sa_mask;
};

// glibc's sigaction() will prevent access to sa_restorer, so we need to roll
// our own.
int sys_rt_sigaction(int sig, const struct kernel_sigaction* act,
                     struct kernel_sigaction* oact) {
  return syscall(SYS_rt_sigaction, sig, act, oact, sizeof(kernel_sigset_t));
}

// This function is intended to be used in between fork() and execve() and will
// reset all signal handlers to the default.
// The motivation for going through all of them is that sa_restorer can leak
// from parents and help defeat ASLR on buggy kernels.  We reset it to NULL.
// See crbug.com/177956.
void ResetChildSignalHandlersToDefaults(void) {
  for (int signum = 1; ; ++signum) {
    struct kernel_sigaction act = {0};
    int sigaction_get_ret = sys_rt_sigaction(signum, NULL, &act);
    if (sigaction_get_ret && errno == EINVAL) {
#if !defined(NDEBUG)
      // Linux supports 32 real-time signals from 33 to 64.
      // If the number of signals in the Linux kernel changes, someone should
      // look at this code.
      const int kNumberOfSignals = 64;
      RAW_CHECK(signum == kNumberOfSignals + 1);
#endif  // !defined(NDEBUG)
      break;
    }
    // All other failures are fatal.
    if (sigaction_get_ret) {
      RAW_LOG(FATAL, "sigaction (get) failed.");
    }

    // The kernel won't allow to re-set SIGKILL or SIGSTOP.
    if (signum != SIGSTOP && signum != SIGKILL) {
      act.k_sa_handler = reinterpret_cast<void*>(SIG_DFL);
      act.k_sa_restorer = NULL;
      if (sys_rt_sigaction(signum, &act, NULL)) {
        RAW_LOG(FATAL, "sigaction (set) failed.");
      }
    }
#if !defined(NDEBUG)
    // Now ask the kernel again and check that no restorer will leak.
    if (sys_rt_sigaction(signum, NULL, &act) || act.k_sa_restorer) {
      RAW_LOG(FATAL, "Cound not fix sa_restorer.");
    }
#endif  // !defined(NDEBUG)
  }
}
#endif  // !defined(OS_LINUX) ||
        // (!defined(__i386__) && !defined(__x86_64__) && !defined(__arm__))

TerminationStatus GetTerminationStatusImpl(ProcessHandle handle,
                                           bool can_block,
                                           int* exit_code) {
  int status = 0;
  const pid_t result = HANDLE_EINTR(waitpid(handle, &status,
                                            can_block ? 0 : WNOHANG));
  if (result == -1) {
    DPLOG(ERROR) << "waitpid(" << handle << ")";
    if (exit_code)
      *exit_code = 0;
    return TERMINATION_STATUS_NORMAL_TERMINATION;
  } else if (result == 0) {
    // the child hasn't exited yet.
    if (exit_code)
      *exit_code = 0;
    return TERMINATION_STATUS_STILL_RUNNING;
  }

  if (exit_code)
    *exit_code = status;

  if (WIFSIGNALED(status)) {
    switch (WTERMSIG(status)) {
      case SIGABRT:
      case SIGBUS:
      case SIGFPE:
      case SIGILL:
      case SIGSEGV:
        return TERMINATION_STATUS_PROCESS_CRASHED;
      case SIGINT:
      case SIGKILL:
      case SIGTERM:
        return TERMINATION_STATUS_PROCESS_WAS_KILLED;
      default:
        break;
    }
  }

  if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    return TERMINATION_STATUS_ABNORMAL_TERMINATION;

  return TERMINATION_STATUS_NORMAL_TERMINATION;
}

}  // anonymous namespace

ProcessId GetCurrentProcId() {
  return getpid();
}

ProcessHandle GetCurrentProcessHandle() {
  return GetCurrentProcId();
}

bool OpenProcessHandle(ProcessId pid, ProcessHandle* handle) {
  // On Posix platforms, process handles are the same as PIDs, so we
  // don't need to do anything.
  *handle = pid;
  return true;
}

bool OpenPrivilegedProcessHandle(ProcessId pid, ProcessHandle* handle) {
  // On POSIX permissions are checked for each operation on process,
  // not when opening a "handle".
  return OpenProcessHandle(pid, handle);
}

bool OpenProcessHandleWithAccess(ProcessId pid,
                                 uint32 access_flags,
                                 ProcessHandle* handle) {
  // On POSIX permissions are checked for each operation on process,
  // not when opening a "handle".
  return OpenProcessHandle(pid, handle);
}

void CloseProcessHandle(ProcessHandle process) {
  // See OpenProcessHandle, nothing to do.
  return;
}

ProcessId GetProcId(ProcessHandle process) {
  return process;
}

// Attempts to kill the process identified by the given process
// entry structure.  Ignores specified exit_code; posix can't force that.
// Returns true if this is successful, false otherwise.
bool KillProcess(ProcessHandle process_id, int exit_code, bool wait) {
  DCHECK_GT(process_id, 1) << " tried to kill invalid process_id";
  if (process_id <= 1)
    return false;
  bool result = kill(process_id, SIGTERM) == 0;
  if (result && wait) {
    int tries = 60;

    if (RunningOnValgrind()) {
      // Wait for some extra time when running under Valgrind since the child
      // processes may take some time doing leak checking.
      tries *= 2;
    }

    unsigned sleep_ms = 4;

    // The process may not end immediately due to pending I/O
    bool exited = false;
    while (tries-- > 0) {
      pid_t pid = HANDLE_EINTR(waitpid(process_id, NULL, WNOHANG));
      if (pid == process_id) {
        exited = true;
        break;
      }
      if (pid == -1) {
        if (errno == ECHILD) {
          // The wait may fail with ECHILD if another process also waited for
          // the same pid, causing the process state to get cleaned up.
          exited = true;
          break;
        }
        DPLOG(ERROR) << "Error waiting for process " << process_id;
      }

      usleep(sleep_ms * 1000);
      const unsigned kMaxSleepMs = 1000;
      if (sleep_ms < kMaxSleepMs)
        sleep_ms *= 2;
    }

    // If we're waiting and the child hasn't died by now, force it
    // with a SIGKILL.
    if (!exited)
      result = kill(process_id, SIGKILL) == 0;
  }

  if (!result)
    DPLOG(ERROR) << "Unable to terminate process " << process_id;

  return result;
}

bool KillProcessGroup(ProcessHandle process_group_id) {
  bool result = kill(-1 * process_group_id, SIGKILL) == 0;
  if (!result)
    DPLOG(ERROR) << "Unable to terminate process group " << process_group_id;
  return result;
}

// A class to handle auto-closing of DIR*'s.
class ScopedDIRClose {
 public:
  inline void operator()(DIR* x) const {
    if (x) {
      closedir(x);
    }
  }
};
typedef scoped_ptr_malloc<DIR, ScopedDIRClose> ScopedDIR;

#if defined(OS_LINUX)
  static const rlim_t kSystemDefaultMaxFds = 8192;
  static const char kFDDir[] = "/proc/self/fd";
#elif defined(OS_MACOSX)
  static const rlim_t kSystemDefaultMaxFds = 256;
  static const char kFDDir[] = "/dev/fd";
#elif defined(OS_SOLARIS)
  static const rlim_t kSystemDefaultMaxFds = 8192;
  static const char kFDDir[] = "/dev/fd";
#elif defined(OS_FREEBSD)
  static const rlim_t kSystemDefaultMaxFds = 8192;
  static const char kFDDir[] = "/dev/fd";
#elif defined(OS_OPENBSD)
  static const rlim_t kSystemDefaultMaxFds = 256;
  static const char kFDDir[] = "/dev/fd";
#elif defined(OS_ANDROID)
  static const rlim_t kSystemDefaultMaxFds = 1024;
  static const char kFDDir[] = "/proc/self/fd";
#endif

void CloseSuperfluousFds(const base::InjectiveMultimap& saved_mapping) {
  // DANGER: no calls to malloc are allowed from now on:
  // http://crbug.com/36678

  // Get the maximum number of FDs possible.
  struct rlimit nofile;
  rlim_t max_fds;
  if (getrlimit(RLIMIT_NOFILE, &nofile)) {
    // getrlimit failed. Take a best guess.
    max_fds = kSystemDefaultMaxFds;
    RAW_LOG(ERROR, "getrlimit(RLIMIT_NOFILE) failed");
  } else {
    max_fds = nofile.rlim_cur;
  }

  if (max_fds > INT_MAX)
    max_fds = INT_MAX;

  DirReaderPosix fd_dir(kFDDir);

  if (!fd_dir.IsValid()) {
    // Fallback case: Try every possible fd.
    for (rlim_t i = 0; i < max_fds; ++i) {
      const int fd = static_cast<int>(i);
      if (fd == STDIN_FILENO || fd == STDOUT_FILENO || fd == STDERR_FILENO)
        continue;
      InjectiveMultimap::const_iterator j;
      for (j = saved_mapping.begin(); j != saved_mapping.end(); j++) {
        if (fd == j->dest)
          break;
      }
      if (j != saved_mapping.end())
        continue;

      // Since we're just trying to close anything we can find,
      // ignore any error return values of close().
      ignore_result(HANDLE_EINTR(close(fd)));
    }
    return;
  }

  const int dir_fd = fd_dir.fd();

  for ( ; fd_dir.Next(); ) {
    // Skip . and .. entries.
    if (fd_dir.name()[0] == '.')
      continue;

    char *endptr;
    errno = 0;
    const long int fd = strtol(fd_dir.name(), &endptr, 10);
    if (fd_dir.name()[0] == 0 || *endptr || fd < 0 || errno)
      continue;
    if (fd == STDIN_FILENO || fd == STDOUT_FILENO || fd == STDERR_FILENO)
      continue;
    InjectiveMultimap::const_iterator i;
    for (i = saved_mapping.begin(); i != saved_mapping.end(); i++) {
      if (fd == i->dest)
        break;
    }
    if (i != saved_mapping.end())
      continue;
    if (fd == dir_fd)
      continue;

    // When running under Valgrind, Valgrind opens several FDs for its
    // own use and will complain if we try to close them.  All of
    // these FDs are >= |max_fds|, so we can check against that here
    // before closing.  See https://bugs.kde.org/show_bug.cgi?id=191758
    if (fd < static_cast<int>(max_fds)) {
      int ret = HANDLE_EINTR(close(fd));
      DPCHECK(ret == 0);
    }
  }
}

char** AlterEnvironment(const EnvironmentVector& changes,
                        const char* const* const env) {
  unsigned count = 0;
  unsigned size = 0;

  // First assume that all of the current environment will be included.
  for (unsigned i = 0; env[i]; i++) {
    const char *const pair = env[i];
    count++;
    size += strlen(pair) + 1 /* terminating NUL */;
  }

  for (EnvironmentVector::const_iterator j = changes.begin();
       j != changes.end();
       ++j) {
    bool found = false;
    const char *pair;

    for (unsigned i = 0; env[i]; i++) {
      pair = env[i];
      const char *const equals = strchr(pair, '=');
      if (!equals)
        continue;
      const unsigned keylen = equals - pair;
      if (keylen == j->first.size() &&
          memcmp(pair, j->first.data(), keylen) == 0) {
        found = true;
        break;
      }
    }

    // if found, we'll either be deleting or replacing this element.
    if (found) {
      count--;
      size -= strlen(pair) + 1;
      if (j->second.size())
        found = false;
    }

    // if !found, then we have a new element to add.
    if (!found && !j->second.empty()) {
      count++;
      size += j->first.size() + 1 /* '=' */ + j->second.size() + 1 /* NUL */;
    }
  }

  count++;  // for the final NULL
  uint8_t *buffer = new uint8_t[sizeof(char*) * count + size];
  char **const ret = reinterpret_cast<char**>(buffer);
  unsigned k = 0;
  char *scratch = reinterpret_cast<char*>(buffer + sizeof(char*) * count);

  for (unsigned i = 0; env[i]; i++) {
    const char *const pair = env[i];
    const char *const equals = strchr(pair, '=');
    if (!equals) {
      const unsigned len = strlen(pair);
      ret[k++] = scratch;
      memcpy(scratch, pair, len + 1);
      scratch += len + 1;
      continue;
    }
    const unsigned keylen = equals - pair;
    bool handled = false;
    for (EnvironmentVector::const_iterator
         j = changes.begin(); j != changes.end(); j++) {
      if (j->first.size() == keylen &&
          memcmp(j->first.data(), pair, keylen) == 0) {
        if (!j->second.empty()) {
          ret[k++] = scratch;
          memcpy(scratch, pair, keylen + 1);
          scratch += keylen + 1;
          memcpy(scratch, j->second.c_str(), j->second.size() + 1);
          scratch += j->second.size() + 1;
        }
        handled = true;
        break;
      }
    }

    if (!handled) {
      const unsigned len = strlen(pair);
      ret[k++] = scratch;
      memcpy(scratch, pair, len + 1);
      scratch += len + 1;
    }
  }

  // Now handle new elements
  for (EnvironmentVector::const_iterator
       j = changes.begin(); j != changes.end(); j++) {
    if (j->second.empty())
      continue;

    bool found = false;
    for (unsigned i = 0; env[i]; i++) {
      const char *const pair = env[i];
      const char *const equals = strchr(pair, '=');
      if (!equals)
        continue;
      const unsigned keylen = equals - pair;
      if (keylen == j->first.size() &&
          memcmp(pair, j->first.data(), keylen) == 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      ret[k++] = scratch;
      memcpy(scratch, j->first.data(), j->first.size());
      scratch += j->first.size();
      *scratch++ = '=';
      memcpy(scratch, j->second.c_str(), j->second.size() + 1);
      scratch += j->second.size() + 1;
     }
  }

  ret[k] = NULL;
  return ret;
}

bool LaunchProcess(const std::vector<std::string>& argv,
                   const LaunchOptions& options,
                   ProcessHandle* process_handle) {
  size_t fd_shuffle_size = 0;
  if (options.fds_to_remap) {
    fd_shuffle_size = options.fds_to_remap->size();
  }

  InjectiveMultimap fd_shuffle1;
  InjectiveMultimap fd_shuffle2;
  fd_shuffle1.reserve(fd_shuffle_size);
  fd_shuffle2.reserve(fd_shuffle_size);

  scoped_ptr<char*[]> argv_cstr(new char*[argv.size() + 1]);
  scoped_ptr<char*[]> new_environ;
  if (options.environ)
    new_environ.reset(AlterEnvironment(*options.environ, GetEnvironment()));

  pid_t pid;
#if defined(OS_LINUX)
  if (options.clone_flags) {
    pid = syscall(__NR_clone, options.clone_flags, 0, 0, 0);
  } else
#endif
  {
    pid = fork();
  }

  if (pid < 0) {
    DPLOG(ERROR) << "fork";
    return false;
  } else if (pid == 0) {
    // Child process

    // DANGER: fork() rule: in the child, if you don't end up doing exec*(),
    // you call _exit() instead of exit(). This is because _exit() does not
    // call any previously-registered (in the parent) exit handlers, which
    // might do things like block waiting for threads that don't even exist
    // in the child.

    if (options.debug) {
      RAW_LOG(INFO, "Right after fork");
    }

    // If a child process uses the readline library, the process block forever.
    // In BSD like OSes including OS X it is safe to assign /dev/null as stdin.
    // See http://crbug.com/56596.
    int null_fd = HANDLE_EINTR(open("/dev/null", O_RDONLY));
    if (null_fd < 0) {
      RAW_LOG(ERROR, "Failed to open /dev/null");
      _exit(127);
    }

    file_util::ScopedFD null_fd_closer(&null_fd);
    int new_fd = HANDLE_EINTR(dup2(null_fd, STDIN_FILENO));
    if (new_fd != STDIN_FILENO) {
      RAW_LOG(ERROR, "Failed to dup /dev/null for stdin");
      _exit(127);
    }

    if (options.new_process_group) {
      // Instead of inheriting the process group ID of the parent, the child
      // starts off a new process group with pgid equal to its process ID.
      if (setpgid(0, 0) < 0) {
        RAW_LOG(ERROR, "setpgid failed");
        _exit(127);
      }
    }

    if (options.debug) {
      RAW_LOG(INFO, "Right before base::type_profiler::Controller::Stop()");
    }

    // Stop type-profiler.
    // The profiler should be stopped between fork and exec since it inserts
    // locks at new/delete expressions.  See http://crbug.com/36678.
    base::type_profiler::Controller::Stop();

    if (options.debug) {
      RAW_LOG(INFO, "Right after base::type_profiler::Controller::Stop()");
    }

    if (options.maximize_rlimits) {
      // Some resource limits need to be maximal in this child.
      std::set<int>::const_iterator resource;
      for (resource = options.maximize_rlimits->begin();
           resource != options.maximize_rlimits->end();
           ++resource) {
        struct rlimit limit;
        if (getrlimit(*resource, &limit) < 0) {
          RAW_LOG(WARNING, "getrlimit failed");
        } else if (limit.rlim_cur < limit.rlim_max) {
          limit.rlim_cur = limit.rlim_max;
          if (setrlimit(*resource, &limit) < 0) {
            RAW_LOG(WARNING, "setrlimit failed");
          }
        }
      }
    }

#if defined(OS_MACOSX)
    RestoreDefaultExceptionHandler();
#endif  // defined(OS_MACOSX)

    ResetChildSignalHandlersToDefaults();

    if (options.debug) {
      RAW_LOG(INFO, "Right after signal/exception handler restoration.");
    }

#if 0
    // When debugging it can be helpful to check that we really aren't making
    // any hidden calls to malloc.
    void *malloc_thunk =
        reinterpret_cast<void*>(reinterpret_cast<intptr_t>(malloc) & ~4095);
    mprotect(malloc_thunk, 4096, PROT_READ | PROT_WRITE | PROT_EXEC);
    memset(reinterpret_cast<void*>(malloc), 0xff, 8);
#endif  // 0

    // DANGER: no calls to malloc are allowed from now on:
    // http://crbug.com/36678

#if defined(OS_CHROMEOS)
    if (options.ctrl_terminal_fd >= 0) {
      // Set process' controlling terminal.
      if (HANDLE_EINTR(setsid()) != -1) {
        if (HANDLE_EINTR(
                ioctl(options.ctrl_terminal_fd, TIOCSCTTY, NULL)) == -1) {
          RAW_LOG(WARNING, "ioctl(TIOCSCTTY), ctrl terminal not set");
        }
      } else {
        RAW_LOG(WARNING, "setsid failed, ctrl terminal not set");
      }
    }
#endif  // defined(OS_CHROMEOS)

    if (options.fds_to_remap) {
      for (FileHandleMappingVector::const_iterator
               it = options.fds_to_remap->begin();
           it != options.fds_to_remap->end(); ++it) {
        fd_shuffle1.push_back(InjectionArc(it->first, it->second, false));
        fd_shuffle2.push_back(InjectionArc(it->first, it->second, false));
      }
    }

    if (options.debug) {
      RAW_LOG(INFO, "Right after fd_shuffle push_backs.");
    }

    if (options.environ)
      SetEnvironment(new_environ.get());

    // fd_shuffle1 is mutated by this call because it cannot malloc.
    if (!ShuffleFileDescriptors(&fd_shuffle1))
      _exit(127);

    if (options.debug) {
      RAW_LOG(INFO, "Right after ShuffleFileDescriptors");
    }

    CloseSuperfluousFds(fd_shuffle2);

    if (options.debug) {
      RAW_LOG(INFO, "Right after CloseSuperfluousFds");
    }

    if (options.debug) {
      RAW_LOG(INFO, "Right before execvp");
    }

    for (size_t i = 0; i < argv.size(); i++)
      argv_cstr[i] = const_cast<char*>(argv[i].c_str());
    argv_cstr[argv.size()] = NULL;
    execvp(argv_cstr[0], argv_cstr.get());

    RAW_LOG(ERROR, "LaunchProcess: failed to execvp:");
    RAW_LOG(ERROR, argv_cstr[0]);
    _exit(127);
  } else {
    // Parent process
    if (options.wait) {
      // While this isn't strictly disk IO, waiting for another process to
      // finish is the sort of thing ThreadRestrictions is trying to prevent.
      base::ThreadRestrictions::AssertIOAllowed();
      pid_t ret = HANDLE_EINTR(waitpid(pid, 0, 0));
      DPCHECK(ret > 0);
    }

    if (process_handle)
      *process_handle = pid;
  }

  return true;
}


bool LaunchProcess(const CommandLine& cmdline,
                   const LaunchOptions& options,
                   ProcessHandle* process_handle) {
  return LaunchProcess(cmdline.argv(), options, process_handle);
}

ProcessMetrics::~ProcessMetrics() { }

void RaiseProcessToHighPriority() {
  // On POSIX, we don't actually do anything here.  We could try to nice() or
  // setpriority() or sched_getscheduler, but these all require extra rights.
}

TerminationStatus GetTerminationStatus(ProcessHandle handle, int* exit_code) {
  return GetTerminationStatusImpl(handle, false /* can_block */, exit_code);
}

TerminationStatus WaitForTerminationStatus(ProcessHandle handle,
                                           int* exit_code) {
  return GetTerminationStatusImpl(handle, true /* can_block */, exit_code);
}

bool WaitForExitCode(ProcessHandle handle, int* exit_code) {
  int status;
  if (HANDLE_EINTR(waitpid(handle, &status, 0)) == -1) {
    NOTREACHED();
    return false;
  }

  if (WIFEXITED(status)) {
    *exit_code = WEXITSTATUS(status);
    return true;
  }

  // If it didn't exit cleanly, it must have been signaled.
  DCHECK(WIFSIGNALED(status));
  return false;
}

bool WaitForExitCodeWithTimeout(ProcessHandle handle, int* exit_code,
                                base::TimeDelta timeout) {
  bool waitpid_success = false;
  int status = WaitpidWithTimeout(handle, timeout.InMilliseconds(),
                                  &waitpid_success);
  if (status == -1)
    return false;
  if (!waitpid_success)
    return false;
  if (WIFSIGNALED(status)) {
    *exit_code = -1;
    return true;
  }
  if (WIFEXITED(status)) {
    *exit_code = WEXITSTATUS(status);
    return true;
  }
  return false;
}

#if defined(OS_MACOSX)
// Using kqueue on Mac so that we can wait on non-child processes.
// We can't use kqueues on child processes because we need to reap
// our own children using wait.
static bool WaitForSingleNonChildProcess(ProcessHandle handle,
                                         base::TimeDelta wait) {
  DCHECK_GT(handle, 0);
  DCHECK(wait.InMilliseconds() == base::kNoTimeout || wait > base::TimeDelta());

  int kq = kqueue();
  if (kq == -1) {
    DPLOG(ERROR) << "kqueue";
    return false;
  }
  file_util::ScopedFD kq_closer(&kq);

  struct kevent change = {0};
  EV_SET(&change, handle, EVFILT_PROC, EV_ADD, NOTE_EXIT, 0, NULL);
  int result = HANDLE_EINTR(kevent(kq, &change, 1, NULL, 0, NULL));
  if (result == -1) {
    if (errno == ESRCH) {
      // If the process wasn't found, it must be dead.
      return true;
    }

    DPLOG(ERROR) << "kevent (setup " << handle << ")";
    return false;
  }

  // Keep track of the elapsed time to be able to restart kevent if it's
  // interrupted.
  bool wait_forever = wait.InMilliseconds() == base::kNoTimeout;
  base::TimeDelta remaining_delta;
  base::TimeTicks deadline;
  if (!wait_forever) {
    remaining_delta = wait;
    deadline = base::TimeTicks::Now() + remaining_delta;
  }

  result = -1;
  struct kevent event = {0};

  while (wait_forever || remaining_delta > base::TimeDelta()) {
    struct timespec remaining_timespec;
    struct timespec* remaining_timespec_ptr;
    if (wait_forever) {
      remaining_timespec_ptr = NULL;
    } else {
      remaining_timespec = remaining_delta.ToTimeSpec();
      remaining_timespec_ptr = &remaining_timespec;
    }

    result = kevent(kq, NULL, 0, &event, 1, remaining_timespec_ptr);

    if (result == -1 && errno == EINTR) {
      if (!wait_forever) {
        remaining_delta = deadline - base::TimeTicks::Now();
      }
      result = 0;
    } else {
      break;
    }
  }

  if (result < 0) {
    DPLOG(ERROR) << "kevent (wait " << handle << ")";
    return false;
  } else if (result > 1) {
    DLOG(ERROR) << "kevent (wait " << handle << "): unexpected result "
                << result;
    return false;
  } else if (result == 0) {
    // Timed out.
    return false;
  }

  DCHECK_EQ(result, 1);

  if (event.filter != EVFILT_PROC ||
      (event.fflags & NOTE_EXIT) == 0 ||
      event.ident != static_cast<uintptr_t>(handle)) {
    DLOG(ERROR) << "kevent (wait " << handle
                << "): unexpected event: filter=" << event.filter
                << ", fflags=" << event.fflags
                << ", ident=" << event.ident;
    return false;
  }

  return true;
}
#endif  // OS_MACOSX

bool WaitForSingleProcess(ProcessHandle handle, base::TimeDelta wait) {
  ProcessHandle parent_pid = GetParentProcessId(handle);
  ProcessHandle our_pid = Process::Current().handle();
  if (parent_pid != our_pid) {
#if defined(OS_MACOSX)
    // On Mac we can wait on non child processes.
    return WaitForSingleNonChildProcess(handle, wait);
#else
    // Currently on Linux we can't handle non child processes.
    NOTIMPLEMENTED();
#endif  // OS_MACOSX
  }

  bool waitpid_success;
  int status = -1;
  if (wait.InMilliseconds() == base::kNoTimeout) {
    waitpid_success = (HANDLE_EINTR(waitpid(handle, &status, 0)) != -1);
  } else {
    status = WaitpidWithTimeout(
        handle, wait.InMilliseconds(), &waitpid_success);
  }

  if (status != -1) {
    DCHECK(waitpid_success);
    return WIFEXITED(status);
  } else {
    return false;
  }
}

int64 TimeValToMicroseconds(const struct timeval& tv) {
  static const int kMicrosecondsPerSecond = 1000000;
  int64 ret = tv.tv_sec;  // Avoid (int * int) integer overflow.
  ret *= kMicrosecondsPerSecond;
  ret += tv.tv_usec;
  return ret;
}

// Return value used by GetAppOutputInternal to encapsulate the various exit
// scenarios from the function.
enum GetAppOutputInternalResult {
  EXECUTE_FAILURE,
  EXECUTE_SUCCESS,
  GOT_MAX_OUTPUT,
};

// Executes the application specified by |argv| and wait for it to exit. Stores
// the output (stdout) in |output|. If |do_search_path| is set, it searches the
// path for the application; in that case, |envp| must be null, and it will use
// the current environment. If |do_search_path| is false, |argv[0]| should fully
// specify the path of the application, and |envp| will be used as the
// environment. Redirects stderr to /dev/null.
// If we successfully start the application and get all requested output, we
// return GOT_MAX_OUTPUT, or if there is a problem starting or exiting
// the application we return RUN_FAILURE. Otherwise we return EXECUTE_SUCCESS.
// The GOT_MAX_OUTPUT return value exists so a caller that asks for limited
// output can treat this as a success, despite having an exit code of SIG_PIPE
// due to us closing the output pipe.
// In the case of EXECUTE_SUCCESS, the application exit code will be returned
// in |*exit_code|, which should be checked to determine if the application
// ran successfully.
static GetAppOutputInternalResult GetAppOutputInternal(
    const std::vector<std::string>& argv,
    char* const envp[],
    std::string* output,
    size_t max_output,
    bool do_search_path,
    int* exit_code) {
  // Doing a blocking wait for another command to finish counts as IO.
  base::ThreadRestrictions::AssertIOAllowed();
  // exit_code must be supplied so calling function can determine success.
  DCHECK(exit_code);
  *exit_code = EXIT_FAILURE;

  int pipe_fd[2];
  pid_t pid;
  InjectiveMultimap fd_shuffle1, fd_shuffle2;
  scoped_ptr<char*[]> argv_cstr(new char*[argv.size() + 1]);

  fd_shuffle1.reserve(3);
  fd_shuffle2.reserve(3);

  // Either |do_search_path| should be false or |envp| should be null, but not
  // both.
  DCHECK(!do_search_path ^ !envp);

  if (pipe(pipe_fd) < 0)
    return EXECUTE_FAILURE;

  switch (pid = fork()) {
    case -1:  // error
      close(pipe_fd[0]);
      close(pipe_fd[1]);
      return EXECUTE_FAILURE;
    case 0:  // child
      {
#if defined(OS_MACOSX)
        RestoreDefaultExceptionHandler();
#endif
        // DANGER: no calls to malloc are allowed from now on:
        // http://crbug.com/36678

        // Obscure fork() rule: in the child, if you don't end up doing exec*(),
        // you call _exit() instead of exit(). This is because _exit() does not
        // call any previously-registered (in the parent) exit handlers, which
        // might do things like block waiting for threads that don't even exist
        // in the child.
        int dev_null = open("/dev/null", O_WRONLY);
        if (dev_null < 0)
          _exit(127);

        // Stop type-profiler.
        // The profiler should be stopped between fork and exec since it inserts
        // locks at new/delete expressions.  See http://crbug.com/36678.
        base::type_profiler::Controller::Stop();

        fd_shuffle1.push_back(InjectionArc(pipe_fd[1], STDOUT_FILENO, true));
        fd_shuffle1.push_back(InjectionArc(dev_null, STDERR_FILENO, true));
        fd_shuffle1.push_back(InjectionArc(dev_null, STDIN_FILENO, true));
        // Adding another element here? Remeber to increase the argument to
        // reserve(), above.

        std::copy(fd_shuffle1.begin(), fd_shuffle1.end(),
                  std::back_inserter(fd_shuffle2));

        if (!ShuffleFileDescriptors(&fd_shuffle1))
          _exit(127);

        CloseSuperfluousFds(fd_shuffle2);

        for (size_t i = 0; i < argv.size(); i++)
          argv_cstr[i] = const_cast<char*>(argv[i].c_str());
        argv_cstr[argv.size()] = NULL;
        if (do_search_path)
          execvp(argv_cstr[0], argv_cstr.get());
        else
          execve(argv_cstr[0], argv_cstr.get(), envp);
        _exit(127);
      }
    default:  // parent
      {
        // Close our writing end of pipe now. Otherwise later read would not
        // be able to detect end of child's output (in theory we could still
        // write to the pipe).
        close(pipe_fd[1]);

        output->clear();
        char buffer[256];
        size_t output_buf_left = max_output;
        ssize_t bytes_read = 1;  // A lie to properly handle |max_output == 0|
                                 // case in the logic below.

        while (output_buf_left > 0) {
          bytes_read = HANDLE_EINTR(read(pipe_fd[0], buffer,
                                    std::min(output_buf_left, sizeof(buffer))));
          if (bytes_read <= 0)
            break;
          output->append(buffer, bytes_read);
          output_buf_left -= static_cast<size_t>(bytes_read);
        }
        close(pipe_fd[0]);

        // Always wait for exit code (even if we know we'll declare
        // GOT_MAX_OUTPUT).
        bool success = WaitForExitCode(pid, exit_code);

        // If we stopped because we read as much as we wanted, we return
        // GOT_MAX_OUTPUT (because the child may exit due to |SIGPIPE|).
        if (!output_buf_left && bytes_read > 0)
          return GOT_MAX_OUTPUT;
        else if (success)
          return EXECUTE_SUCCESS;
        return EXECUTE_FAILURE;
      }
  }
}

bool GetAppOutput(const CommandLine& cl, std::string* output) {
  return GetAppOutput(cl.argv(), output);
}

bool GetAppOutput(const std::vector<std::string>& argv, std::string* output) {
  // Run |execve()| with the current environment and store "unlimited" data.
  int exit_code;
  GetAppOutputInternalResult result = GetAppOutputInternal(
      argv, NULL, output, std::numeric_limits<std::size_t>::max(), true,
      &exit_code);
  return result == EXECUTE_SUCCESS && exit_code == EXIT_SUCCESS;
}

// TODO(viettrungluu): Conceivably, we should have a timeout as well, so we
// don't hang if what we're calling hangs.
bool GetAppOutputRestricted(const CommandLine& cl,
                            std::string* output, size_t max_output) {
  // Run |execve()| with the empty environment.
  char* const empty_environ = NULL;
  int exit_code;
  GetAppOutputInternalResult result = GetAppOutputInternal(
      cl.argv(), &empty_environ, output, max_output, false, &exit_code);
  return result == GOT_MAX_OUTPUT || (result == EXECUTE_SUCCESS &&
                                      exit_code == EXIT_SUCCESS);
}

bool GetAppOutputWithExitCode(const CommandLine& cl,
                              std::string* output,
                              int* exit_code) {
  // Run |execve()| with the current environment and store "unlimited" data.
  GetAppOutputInternalResult result = GetAppOutputInternal(
      cl.argv(), NULL, output, std::numeric_limits<std::size_t>::max(), true,
      exit_code);
  return result == EXECUTE_SUCCESS;
}

bool WaitForProcessesToExit(const FilePath::StringType& executable_name,
                            base::TimeDelta wait,
                            const ProcessFilter* filter) {
  bool result = false;

  // TODO(port): This is inefficient, but works if there are multiple procs.
  // TODO(port): use waitpid to avoid leaving zombies around

  base::TimeTicks end_time = base::TimeTicks::Now() + wait;
  do {
    NamedProcessIterator iter(executable_name, filter);
    if (!iter.NextProcessEntry()) {
      result = true;
      break;
    }
    base::PlatformThread::Sleep(base::TimeDelta::FromMilliseconds(100));
  } while ((end_time - base::TimeTicks::Now()) > base::TimeDelta());

  return result;
}

bool CleanupProcesses(const FilePath::StringType& executable_name,
                      base::TimeDelta wait,
                      int exit_code,
                      const ProcessFilter* filter) {
  bool exited_cleanly = WaitForProcessesToExit(executable_name, wait, filter);
  if (!exited_cleanly)
    KillProcesses(executable_name, exit_code, filter);
  return exited_cleanly;
}

#if !defined(OS_MACOSX)

namespace {

// Return true if the given child is dead. This will also reap the process.
// Doesn't block.
static bool IsChildDead(pid_t child) {
  const pid_t result = HANDLE_EINTR(waitpid(child, NULL, WNOHANG));
  if (result == -1) {
    DPLOG(ERROR) << "waitpid(" << child << ")";
    NOTREACHED();
  } else if (result > 0) {
    // The child has died.
    return true;
  }

  return false;
}

// A thread class which waits for the given child to exit and reaps it.
// If the child doesn't exit within a couple of seconds, kill it.
class BackgroundReaper : public PlatformThread::Delegate {
 public:
  BackgroundReaper(pid_t child, unsigned timeout)
      : child_(child),
        timeout_(timeout) {
  }

  // Overridden from PlatformThread::Delegate:
  virtual void ThreadMain() OVERRIDE {
    WaitForChildToDie();
    delete this;
  }

  void WaitForChildToDie() {
    // Wait forever case.
    if (timeout_ == 0) {
      pid_t r = HANDLE_EINTR(waitpid(child_, NULL, 0));
      if (r != child_) {
        DPLOG(ERROR) << "While waiting for " << child_
                     << " to terminate, we got the following result: " << r;
      }
      return;
    }

    // There's no good way to wait for a specific child to exit in a timed
    // fashion. (No kqueue on Linux), so we just loop and sleep.

    // Wait for 2 * timeout_ 500 milliseconds intervals.
    for (unsigned i = 0; i < 2 * timeout_; ++i) {
      PlatformThread::Sleep(TimeDelta::FromMilliseconds(500));
      if (IsChildDead(child_))
        return;
    }

    if (kill(child_, SIGKILL) == 0) {
      // SIGKILL is uncatchable. Since the signal was delivered, we can
      // just wait for the process to die now in a blocking manner.
      if (HANDLE_EINTR(waitpid(child_, NULL, 0)) < 0)
        DPLOG(WARNING) << "waitpid";
    } else {
      DLOG(ERROR) << "While waiting for " << child_ << " to terminate we"
                  << " failed to deliver a SIGKILL signal (" << errno << ").";
    }
  }

 private:
  const pid_t child_;
  // Number of seconds to wait, if 0 then wait forever and do not attempt to
  // kill |child_|.
  const unsigned timeout_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundReaper);
};

}  // namespace

void EnsureProcessTerminated(ProcessHandle process) {
  // If the child is already dead, then there's nothing to do.
  if (IsChildDead(process))
    return;

  const unsigned timeout = 2;  // seconds
  BackgroundReaper* reaper = new BackgroundReaper(process, timeout);
  PlatformThread::CreateNonJoinable(0, reaper);
}

void EnsureProcessGetsReaped(ProcessHandle process) {
  // If the child is already dead, then there's nothing to do.
  if (IsChildDead(process))
    return;

  BackgroundReaper* reaper = new BackgroundReaper(process, 0);
  PlatformThread::CreateNonJoinable(0, reaper);
}

#endif  // !defined(OS_MACOSX)

}  // namespace base
