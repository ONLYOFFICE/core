// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/debug/stack_trace.h"

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <unwind.h>  // TODO(dmikurube): Remove.  See http://crbug.com/236855.

#include "base/logging.h"

#ifdef __MIPSEL__
// SIGSTKFLT is not defined for MIPS.
#define SIGSTKFLT SIGSEGV
#endif

// TODO(dmikurube): Remove when Bionic's get_backtrace() gets popular.
// See http://crbug.com/236855.
namespace {

/* depends how the system includes define this */
#ifdef HAVE_UNWIND_CONTEXT_STRUCT
typedef struct _Unwind_Context __unwind_context;
#else
typedef _Unwind_Context __unwind_context;
#endif

struct stack_crawl_state_t {
  uintptr_t* frames;
  size_t frame_count;
  size_t max_depth;
  bool have_skipped_self;

  stack_crawl_state_t(uintptr_t* frames, size_t max_depth)
      : frames(frames),
        frame_count(0),
        max_depth(max_depth),
        have_skipped_self(false) {
  }
};

static _Unwind_Reason_Code tracer(__unwind_context* context, void* arg) {
  stack_crawl_state_t* state = static_cast<stack_crawl_state_t*>(arg);

#if defined(__clang__)
  // Vanilla Clang's unwind.h doesn't have _Unwind_GetIP for ARM.
  // See http://crbug.com/236855, too.
  uintptr_t ip = 0;
  _Unwind_VRS_Get(context, _UVRSC_CORE, 15, _UVRSD_UINT32, &ip);
  ip &= ~(uintptr_t)0x1;  // remove thumb mode bit
#else
  uintptr_t ip = _Unwind_GetIP(context);
#endif

  // The first stack frame is this function itself.  Skip it.
  if (ip != 0 && !state->have_skipped_self) {
    state->have_skipped_self = true;
    return _URC_NO_REASON;
  }

  state->frames[state->frame_count++] = ip;
  if (state->frame_count >= state->max_depth)
    return _URC_END_OF_STACK;
  else
    return _URC_NO_REASON;
}

}  // namespace

namespace base {
namespace debug {

bool EnableInProcessStackDumping() {
  // When running in an application, our code typically expects SIGPIPE
  // to be ignored.  Therefore, when testing that same code, it should run
  // with SIGPIPE ignored as well.
  // TODO(phajdan.jr): De-duplicate this SIGPIPE code.
  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_handler = SIG_IGN;
  sigemptyset(&action.sa_mask);
  return (sigaction(SIGPIPE, &action, NULL) == 0);
}

StackTrace::StackTrace() {
  // TODO(dmikurube): Replace it with Bionic's get_backtrace().
  // See http://crbug.com/236855.
  stack_crawl_state_t state(reinterpret_cast<uintptr_t*>(trace_), kMaxTraces);
  _Unwind_Backtrace(tracer, &state);
  count_ = state.frame_count;
  // TODO(dmikurube): Symbolize in Chrome.
}

// Sends fake SIGSTKFLT signals to let the Android linker and debuggerd dump
// stack. See inlined comments and Android bionic/linker/debugger.c and
// system/core/debuggerd/debuggerd.c for details.
void StackTrace::PrintBacktrace() const {
  // Get the current handler of SIGSTKFLT for later use.
  sighandler_t sig_handler = signal(SIGSTKFLT, SIG_DFL);
  signal(SIGSTKFLT, sig_handler);

  // The Android linker will handle this signal and send a stack dumping request
  // to debuggerd which will ptrace_attach this process. Before returning from
  // the signal handler, the linker sets the signal handler to SIG_IGN.
  kill(gettid(), SIGSTKFLT);

  // Because debuggerd will wait for the process to be stopped by the actual
  // signal in crashing scenarios, signal is sent again to met the expectation.
  // Debuggerd will dump stack into the system log and /data/tombstones/ files.
  // NOTE: If this process runs in the interactive shell, it will be put
  // in the background. To resume it in the foreground, use 'fg' command.
  kill(gettid(), SIGSTKFLT);

  // Restore the signal handler so that this method can work the next time.
  signal(SIGSTKFLT, sig_handler);
}

void StackTrace::OutputToStream(std::ostream* os) const {
  NOTIMPLEMENTED();
}

}  // namespace debug
}  // namespace base
