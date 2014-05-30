// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/mesa/MesaLib/include/GL/osmesa.h"
#include "ui/gl/gl_context_cgl.h"
#include "ui/gl/gl_context_osmesa.h"
#include "ui/gl/gl_context_stub.h"
#include "ui/gl/gl_implementation.h"
#include "ui/gl/gl_surface.h"
#include "ui/gl/gl_switches.h"

#if defined(USE_AURA)
#include "ui/gl/gl_context_nsview.h"
#endif

namespace gfx {

class GLShareGroup;

scoped_refptr<GLContext> GLContext::CreateGLContext(
    GLShareGroup* share_group,
    GLSurface* compatible_surface,
    GpuPreference gpu_preference) {
  TRACE_EVENT0("gpu", "GLContext::CreateGLContext");
  switch (GetGLImplementation()) {
    case kGLImplementationDesktopGL:
    case kGLImplementationAppleGL: {
      scoped_refptr<GLContext> context;
#if defined(USE_AURA)
      if (compatible_surface->IsOffscreen())
        context = new GLContextCGL(share_group);
      else
        context = new GLContextNSView(share_group);
#else
      context = new GLContextCGL(share_group);
#endif // USE_AURA
      if (!context->Initialize(compatible_surface, gpu_preference))
        return NULL;

      return context;
    }
    case kGLImplementationOSMesaGL: {
      scoped_refptr<GLContext> context(new GLContextOSMesa(share_group));
      if (!context->Initialize(compatible_surface, gpu_preference))
        return NULL;

      return context;
    }
    case kGLImplementationMockGL:
      return new GLContextStub;
    default:
      NOTREACHED();
      return NULL;
  }
}

}  // namespace gfx
