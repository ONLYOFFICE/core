// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>

#include "base/base_paths.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/native_library.h"
#include "base/path_service.h"
#include "base/threading/thread_restrictions.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_egl_api_implementation.h"
#include "ui/gl/gl_gl_api_implementation.h"
#include "ui/gl/gl_glx_api_implementation.h"
#include "ui/gl/gl_implementation.h"
#include "ui/gl/gl_osmesa_api_implementation.h"
#include "ui/gl/gl_switches.h"

namespace gfx {
namespace {

// TODO(piman): it should be Desktop GL marshalling from double to float. Today
// on native GLES, we do float->double->float.
void GL_BINDING_CALL MarshalClearDepthToClearDepthf(GLclampd depth) {
  glClearDepthf(static_cast<GLclampf>(depth));
}

void GL_BINDING_CALL MarshalDepthRangeToDepthRangef(GLclampd z_near,
                                                    GLclampd z_far) {
  glDepthRangef(static_cast<GLclampf>(z_near), static_cast<GLclampf>(z_far));
}

// Load a library, printing an error message on failure.
base::NativeLibrary LoadLibrary(const base::FilePath& filename) {
  std::string error;
  base::NativeLibrary library = base::LoadNativeLibrary(filename,
                                                        &error);
  if (!library) {
    DVLOG(1) << "Failed to load " << filename.MaybeAsASCII() << ": " << error;
    return NULL;
  }
  return library;
}

base::NativeLibrary LoadLibrary(const char* filename) {
  return LoadLibrary(base::FilePath(filename));
}

}  // namespace

void GetAllowedGLImplementations(std::vector<GLImplementation>* impls) {
  impls->push_back(kGLImplementationDesktopGL);
  impls->push_back(kGLImplementationEGLGLES2);
  impls->push_back(kGLImplementationOSMesaGL);
}

bool InitializeGLBindings(GLImplementation implementation) {
  // Prevent reinitialization with a different implementation. Once the gpu
  // unit tests have initialized with kGLImplementationMock, we don't want to
  // later switch to another GL implementation.
  if (GetGLImplementation() != kGLImplementationNone)
    return true;

  // Allow the main thread or another to initialize these bindings
  // after instituting restrictions on I/O. Going forward they will
  // likely be used in the browser process on most platforms. The
  // one-time initialization cost is small, between 2 and 5 ms.
  base::ThreadRestrictions::ScopedAllowIO allow_io;

  switch (implementation) {
    case kGLImplementationOSMesaGL: {
      base::FilePath module_path;
      if (!PathService::Get(base::DIR_MODULE, &module_path)) {
        LOG(ERROR) << "PathService::Get failed.";
        return false;
      }

      base::NativeLibrary library = LoadLibrary(
          module_path.Append("libosmesa.so"));
      if (!library)
        return false;

      GLGetProcAddressProc get_proc_address =
          reinterpret_cast<GLGetProcAddressProc>(
              base::GetFunctionPointerFromNativeLibrary(
                  library, "OSMesaGetProcAddress"));
      if (!get_proc_address) {
        LOG(ERROR) << "OSMesaGetProcAddress not found.";
        base::UnloadNativeLibrary(library);
        return false;
      }

      SetGLGetProcAddressProc(get_proc_address);
      AddGLNativeLibrary(library);
      SetGLImplementation(kGLImplementationOSMesaGL);

      InitializeGLBindingsGL();
      InitializeGLBindingsOSMESA();
      break;
    }
    case kGLImplementationDesktopGL: {
      base::NativeLibrary library = NULL;
      const CommandLine* command_line = CommandLine::ForCurrentProcess();

      if (command_line->HasSwitch(switches::kTestGLLib))
        library = LoadLibrary(command_line->GetSwitchValueASCII(
            switches::kTestGLLib).c_str());

      if (!library) {
#if defined(OS_OPENBSD)
        library = LoadLibrary("libGL.so");
#else
        library = LoadLibrary("libGL.so.1");
#endif
      }

      if (!library)
        return false;

      GLGetProcAddressProc get_proc_address =
          reinterpret_cast<GLGetProcAddressProc>(
              base::GetFunctionPointerFromNativeLibrary(
                  library, "glXGetProcAddress"));
      if (!get_proc_address) {
        LOG(ERROR) << "glxGetProcAddress not found.";
        base::UnloadNativeLibrary(library);
        return false;
      }

      SetGLGetProcAddressProc(get_proc_address);
      AddGLNativeLibrary(library);
      SetGLImplementation(kGLImplementationDesktopGL);

      InitializeGLBindingsGL();
      InitializeGLBindingsGLX();
      break;
    }
    case kGLImplementationEGLGLES2: {
      base::NativeLibrary gles_library = LoadLibrary("libGLESv2.so.2");
      if (!gles_library)
        return false;
      base::NativeLibrary egl_library = LoadLibrary("libEGL.so.1");
      if (!egl_library) {
        base::UnloadNativeLibrary(gles_library);
        return false;
      }

      GLGetProcAddressProc get_proc_address =
          reinterpret_cast<GLGetProcAddressProc>(
              base::GetFunctionPointerFromNativeLibrary(
                  egl_library, "eglGetProcAddress"));
      if (!get_proc_address) {
        LOG(ERROR) << "eglGetProcAddress not found.";
        base::UnloadNativeLibrary(egl_library);
        base::UnloadNativeLibrary(gles_library);
        return false;
      }

      SetGLGetProcAddressProc(get_proc_address);
      AddGLNativeLibrary(egl_library);
      AddGLNativeLibrary(gles_library);
      SetGLImplementation(kGLImplementationEGLGLES2);

      InitializeGLBindingsGL();
      InitializeGLBindingsEGL();

      // These two functions take single precision float rather than double
      // precision float parameters in GLES.
      ::gfx::g_driver_gl.fn.glClearDepthFn = MarshalClearDepthToClearDepthf;
      ::gfx::g_driver_gl.fn.glDepthRangeFn = MarshalDepthRangeToDepthRangef;
      break;
    }
    case kGLImplementationMockGL: {
      SetGLGetProcAddressProc(GetMockGLProcAddress);
      SetGLImplementation(kGLImplementationMockGL);
      InitializeGLBindingsGL();
      break;
    }
    default:
      return false;
  }


  return true;
}

bool InitializeGLExtensionBindings(GLImplementation implementation,
    GLContext* context) {
  switch (implementation) {
    case kGLImplementationOSMesaGL:
      InitializeGLExtensionBindingsGL(context);
      InitializeGLExtensionBindingsOSMESA(context);
      break;
    case kGLImplementationDesktopGL:
      InitializeGLExtensionBindingsGL(context);
      InitializeGLExtensionBindingsGLX(context);
      break;
    case kGLImplementationEGLGLES2:
      InitializeGLExtensionBindingsGL(context);
      InitializeGLExtensionBindingsEGL(context);
      break;
    case kGLImplementationMockGL:
      InitializeGLExtensionBindingsGL(context);
      break;
    default:
      return false;
  }

  return true;
}

void InitializeDebugGLBindings() {
  InitializeDebugGLBindingsEGL();
  InitializeDebugGLBindingsGL();
  InitializeDebugGLBindingsGLX();
  InitializeDebugGLBindingsOSMESA();
}

void ClearGLBindings() {
  ClearGLBindingsEGL();
  ClearGLBindingsGL();
  ClearGLBindingsGLX();
  ClearGLBindingsOSMESA();
  SetGLImplementation(kGLImplementationNone);

  UnloadGLNativeLibraries();
}

}  // namespace gfx
