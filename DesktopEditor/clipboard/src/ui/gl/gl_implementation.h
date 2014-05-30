// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_IMPLEMENTATION_H_
#define UI_GL_GL_IMPLEMENTATION_H_

#include <string>
#include <vector>

#include "base/native_library.h"
#include "build/build_config.h"
#include "ui/gl/gl_export.h"
#include "ui/gl/gl_switches.h"

namespace gfx {

class GLContext;

// The GL implementation currently in use.
enum GLImplementation {
  kGLImplementationNone,
  kGLImplementationDesktopGL,
  kGLImplementationOSMesaGL,
  kGLImplementationAppleGL,
  kGLImplementationEGLGLES2,
  kGLImplementationMockGL
};

void GetAllowedGLImplementations(std::vector<GLImplementation>* impls);

#if defined(OS_WIN)
typedef void* (WINAPI *GLGetProcAddressProc)(const char* name);
#else
typedef void* (*GLGetProcAddressProc)(const char* name);
#endif

// Initialize a particular GL implementation.
GL_EXPORT bool InitializeGLBindings(GLImplementation implementation);

// Initialize extension function bindings for a GL implementation.
GL_EXPORT bool InitializeGLExtensionBindings(GLImplementation implementation,
    GLContext* context);

// Initialize Debug logging wrappers for GL bindings.
void InitializeDebugGLBindings();

void ClearGLBindings();

// Set the current GL implementation.
GL_EXPORT void SetGLImplementation(GLImplementation implementation);

// Get the current GL implementation.
GL_EXPORT GLImplementation GetGLImplementation();

// Does the underlying GL support all features from Desktop GL 2.0 that were
// removed from the ES 2.0 spec without requiring specific extension strings.
GL_EXPORT bool HasDesktopGLFeatures();

// Get the GL implementation with a given name.
GLImplementation GetNamedGLImplementation(const std::string& name);

// Get the name of a GL implementation.
const char* GetGLImplementationName(GLImplementation implementation);

// Add a native library to those searched for GL entry points.
void AddGLNativeLibrary(base::NativeLibrary library);

// Unloads all native libraries.
void UnloadGLNativeLibraries();

// Set an additional function that will be called to find GL entry points.
void SetGLGetProcAddressProc(GLGetProcAddressProc proc);

// Find a core (non-extension) entry point in the current GL implementation. On
// EGL based implementations core entry points will not be queried through
// GLGetProcAddressProc.
void* GetGLCoreProcAddress(const char* name);

// Find an entry point in the current GL implementation.
void* GetGLProcAddress(const char* name);

}  // namespace gfx

#endif  // UI_GL_GL_IMPLEMENTATION_H_
