// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_switches.h"
#include "base/basictypes.h"

namespace gfx {

const char kGLImplementationDesktopName[]     = "desktop";
const char kGLImplementationOSMesaName[]      = "osmesa";
const char kGLImplementationAppleName[]       = "apple";
const char kGLImplementationEGLName[]         = "egl";
const char kGLImplementationSwiftShaderName[] = "swiftshader";
const char kGLImplementationMockName[]        = "mock";

}  // namespace gfx

namespace switches {

// Stop the GPU from synchronizing on the vsync before presenting.
const char kDisableGpuVsync[]               = "disable-gpu-vsync";

// Turns on GPU logging (debug build only).
const char kEnableGPUServiceLogging[]       = "enable-gpu-service-logging";
const char kEnableGPUClientLogging[]        = "enable-gpu-client-logging";

// Turns on calling TRACE for every GL call.
const char kEnableGPUServiceTracing[]       = "enable-gpu-service-tracing";

// Select which implementation of GL the GPU process should use. Options are:
//  desktop: whatever desktop OpenGL the user has installed (Linux and Mac
//           default).
//  egl: whatever EGL / GLES2 the user has installed (Windows default - actually
//       ANGLE).
//  osmesa: The OSMesa software renderer.
const char kUseGL[]                         = "use-gl";

const char kSwiftShaderPath[]               = "swiftshader-path";

// Inform Chrome that a GPU context will not be lost in power saving mode,
// screen saving mode, etc.  Note that this flag does not ensure that a GPU
// context will never be lost in any situations, say, a GPU reset.
const char kGpuNoContextLost[]              = "gpu-no-context-lost";

// Add a delay in milliseconds to the gpu swap buffer completion signal.
// Simulates a slow GPU.
const char kGpuSwapDelay[]                  = "gpu-swap-delay";

// Indicates whether the dual GPU switching is supported or not.
const char kSupportsDualGpus[]              = "supports-dual-gpus";

// Overwrite the default GPU automatic switching behavior to force on
// integrated GPU or discrete GPU.
const char kGpuSwitching[]                  = "gpu-switching";

const char kGpuSwitchingOptionNameForceIntegrated[] = "force_integrated";
const char kGpuSwitchingOptionNameForceDiscrete[]   = "force_discrete";
const char kGpuSwitchingOptionNameAutomatic[]   = "automatic";

// Flag used for Linux tests: for desktop GL bindings, try to load this GL
// library first, but fall back to regular library if loading fails.
const char kTestGLLib[]                     = "test-gl-lib";

// This is the list of switches passed from this file that are passed from the
// GpuProcessHost to the GPU Process. Add your switch to this list if you need
// to read it in the GPU process, else don't add it.
const char* kGLSwitchesCopiedFromGpuProcessHost[] = {
  kDisableGpuVsync,
  kEnableGPUServiceLogging,
  kEnableGPUServiceTracing,
  kGpuNoContextLost,
  kGpuSwitching,
};
const int kGLSwitchesCopiedFromGpuProcessHostNumSwitches =
    arraysize(kGLSwitchesCopiedFromGpuProcessHost);

}  // namespace switches

