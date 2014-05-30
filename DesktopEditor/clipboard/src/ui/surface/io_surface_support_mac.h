// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SURFACE_IO_SURFACE_SUPPORT_MAC_H_
#define UI_SURFACE_IO_SURFACE_SUPPORT_MAC_H_

#include <CoreFoundation/CoreFoundation.h>
#include <CoreVideo/CoreVideo.h>
#include <mach/mach.h>
#include <OpenGL/OpenGL.h>

#include "base/basictypes.h"
#include "ui/surface/surface_export.h"

// This Mac OS X-specific class provides dynamically-linked access to
// IOSurface.framework, which is only available on 10.6 and later.
// Since Chromium is built on 10.5 we must dynamically look up all of
// the entry points we need in this framework.

// See IOSurface/IOSurfaceAPI.h and OpenGL/CGLIOSurface.h on 10.6 for
// documentation of the fields and methods of this class.

class SURFACE_EXPORT IOSurfaceSupport {
 public:
  // Returns an instance of the IOSurfaceSupport class if the
  // operating system supports it, NULL otherwise. It is safe to call
  // this multiple times.
  static IOSurfaceSupport* Initialize();

  virtual CFStringRef GetKIOSurfaceWidth() = 0;
  virtual CFStringRef GetKIOSurfaceHeight() = 0;
  virtual CFStringRef GetKIOSurfaceBytesPerElement() = 0;
  virtual CFStringRef GetKIOSurfaceIsGlobal() = 0;

  virtual CFTypeRef IOSurfaceCreate(CFDictionaryRef properties) = 0;

  // The following two APIs assume the IOSurface was created with the
  // kIOSurfaceIsGlobal key set to true
  virtual uint32 IOSurfaceGetID(CFTypeRef io_surface) = 0;
  virtual CFTypeRef IOSurfaceLookup(uint32 io_surface_id) = 0;

  // The following two APIs are more robust and secure, but
  // unfortunately it looks like it will be a lot of work to correctly
  // transmit a mach port from process to process (possibly requiring
  // a side channel for or extension of the Chrome IPC mechanism)
  virtual mach_port_t IOSurfaceCreateMachPort(CFTypeRef io_surface) = 0;
  virtual CFTypeRef IOSurfaceLookupFromMachPort(mach_port_t port) = 0;

  virtual size_t IOSurfaceGetWidth(CFTypeRef io_surface) = 0;
  virtual size_t IOSurfaceGetHeight(CFTypeRef io_surface) = 0;

  virtual CGLError CGLTexImageIOSurface2D(CGLContextObj ctx,
                                          GLenum target,
                                          GLenum internal_format,
                                          GLsizei width,
                                          GLsizei height,
                                          GLenum format,
                                          GLenum type,
                                          CFTypeRef io_surface,
                                          GLuint plane) = 0;

  virtual CFTypeRef CVPixelBufferGetIOSurface(
      CVPixelBufferRef pixel_buffer) = 0;

 protected:
  IOSurfaceSupport();
  virtual ~IOSurfaceSupport();

  DISALLOW_COPY_AND_ASSIGN(IOSurfaceSupport);
};

#endif  // UI_SURFACE_IO_SURFACE_SUPPORT_MAC_H_
