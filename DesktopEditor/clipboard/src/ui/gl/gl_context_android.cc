// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_context.h"

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/sys_info.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context_egl.h"
#include "ui/gl/gl_context_stub.h"
#include "ui/gl/gl_implementation.h"
#include "ui/gl/gl_surface.h"

namespace gfx {

namespace {

// Used to render into an already current context+surface,
// that we do not have ownership of (draw callback).
class GLNonOwnedContext : public GLContext {
 public:
  GLNonOwnedContext(GLShareGroup* share_group);

  // Implement GLContext.
  virtual bool Initialize(GLSurface* compatible_surface,
                          GpuPreference gpu_preference) OVERRIDE {
    return true;
  }
  virtual void Destroy() OVERRIDE {}
  virtual bool MakeCurrent(GLSurface* surface) OVERRIDE;
  virtual void ReleaseCurrent(GLSurface* surface) OVERRIDE {}
  virtual bool IsCurrent(GLSurface* surface) OVERRIDE { return true; }
  virtual void* GetHandle() OVERRIDE { return NULL; }
  virtual void SetSwapInterval(int interval) OVERRIDE {}
  virtual std::string GetExtensions() OVERRIDE;

 protected:
  virtual ~GLNonOwnedContext() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(GLNonOwnedContext);
};

GLNonOwnedContext::GLNonOwnedContext(GLShareGroup* share_group)
  : GLContext(share_group) {}

bool GLNonOwnedContext::MakeCurrent(GLSurface* surface) {
  SetCurrent(this, surface);
  SetRealGLApi();
  return true;
}

std::string GLNonOwnedContext::GetExtensions() {
  return GLContext::GetExtensions();
}

}  // anonymous namespace

// static
scoped_refptr<GLContext> GLContext::CreateGLContext(
    GLShareGroup* share_group,
    GLSurface* compatible_surface,
    GpuPreference gpu_preference) {
  if (GetGLImplementation() == kGLImplementationMockGL)
    return scoped_refptr<GLContext>(new GLContextStub());

  scoped_refptr<GLContext> context;
  if (compatible_surface->GetHandle())
    context = new GLContextEGL(share_group);
  else
    context = new GLNonOwnedContext(share_group);
  if (!context->Initialize(compatible_surface, gpu_preference))
    return NULL;
  return context;
}

bool GLContextEGL::GetTotalGpuMemory(size_t* bytes) {
  DCHECK(bytes);
  *bytes = 0;
  // We can't query available GPU memory from the system on Android,
  // but the dalvik heap size give us a good estimate of available
  // GPU memory on a wide range of devices.
  //
  // The heap size tends to be about 1/4 of total ram on higher end
  // devices, so we use 1/2 of that by default. For example both the
  // Nexus 4/10 have 2GB of ram and 512MB Dalvik heap size. For lower
  // end devices, 1/2 of the heap size can be too high, but this
  // correlates well with having a small heap-growth-limit. So for
  // devices with less ram, we factor in the growth limit.
  //
  // This is the result of the calculation below:
  // Droid DNA 1080P  128MB
  // Nexus S           56MB
  // Galaxy Nexus     112MB
  // Nexus 4/10       256MB
  // Xoom              88MB
  size_t dalvik_limit = 0;
  if (!dalvik_limit) {
    size_t heap_size   = static_cast<size_t>(base::SysInfo::DalvikHeapSizeMB());
    size_t heap_growth = static_cast<size_t>(
                             base::SysInfo::DalvikHeapGrowthLimitMB());
    size_t limit = 0;
    if (heap_size >= 350)
        limit = heap_size / 2;
    else
        limit = (heap_size + (heap_growth * 2)) / 4;
    dalvik_limit = limit * 1024 * 1024;
  }
  *bytes = dalvik_limit;
  return true;
}

}
