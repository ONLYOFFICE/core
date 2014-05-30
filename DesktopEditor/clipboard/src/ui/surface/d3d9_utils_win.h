// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Helper functions that Direct3D 9Ex code a little easier to work with for
// the ui/surface code.

#ifndef UI_SURFACE_D3D9_UTILS_WIN_H_
#define UI_SURFACE_D3D9_UTILS_WIN_H_

#include <d3d9.h>

#include "base/basictypes.h"
#include "base/win/scoped_comptr.h"
#include "ui/surface/surface_export.h"

namespace base {
class ScopedNativeLibrary;
}

namespace gfx {
class Size;
}

namespace ui_surface_d3d9_utils {

// Visible for testing. Loads the Direct3D9 library. Returns true on success.
SURFACE_EXPORT
bool LoadD3D9(base::ScopedNativeLibrary* storage);

// Visible for testing. Creates a Direct3D9 device suitable for use with the
// accelerated surface code. Returns true on success.
SURFACE_EXPORT
bool CreateDevice(const base::ScopedNativeLibrary& d3d_module,
                  D3DDEVTYPE device_type,
                  uint32 presentation_interval,
                  IDirect3DDevice9Ex** device);

// Calls the Vista+ (WDDM1.0) variant of CreateTexture that semantically opens a
// texture allocated as shared. In this way textures allocated by another
// process can be used by a D3D context in this process. The shared texture is
// identified by its surface handle. The resulting texture is written into
// |opened_texture|.
//
// Returns true on success.
SURFACE_EXPORT
bool OpenSharedTexture(IDirect3DDevice9* device,
                       int64 surface_handle,
                       const gfx::Size& size,
                       IDirect3DTexture9** opened_texture);

// Ensures that |surface| is a lockable surface of a specified |size|. If
// |*surface| is non-null and has dimensions that match |size|, it is reused.
// Otherwise, a new resource is created and the old one (if any) is freed.
//
// Returns true on success.
SURFACE_EXPORT
bool CreateOrReuseLockableSurface(
    IDirect3DDevice9* device,
    const gfx::Size& size,
    base::win::ScopedComPtr<IDirect3DSurface9>* surface);

// Ensures that |texture| is a render target texture of a specified |size|. If
// |*texture| is non-null and has dimensions that match |size|, it is reused.
// Otherwise, a new resource is created and the old one (if any) is freed.
//
// A reference to level 0 of the resulting texture is placed into
// |render_target|.
//
// Returns true on success.
SURFACE_EXPORT
bool CreateOrReuseRenderTargetTexture(
    IDirect3DDevice9* device,
    const gfx::Size& size,
    base::win::ScopedComPtr<IDirect3DTexture9>* texture,
    IDirect3DSurface9** render_target);

SURFACE_EXPORT
gfx::Size GetSize(IDirect3DTexture9* texture);

SURFACE_EXPORT
gfx::Size GetSize(IDirect3DSurface9* surface);

}  // namespace ui_surface_d3d9_utils

#endif  // UI_SURFACE_D3D9_UTILS_WIN_H_
