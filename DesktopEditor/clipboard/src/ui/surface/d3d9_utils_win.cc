// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/surface/d3d9_utils_win.h"

#include "base/debug/trace_event.h"
#include "base/files/file_path.h"
#include "base/scoped_native_library.h"
#include "base/win/scoped_comptr.h"
#include "ui/gfx/size.h"

namespace {

const wchar_t kD3D9ModuleName[] = L"d3d9.dll";
const char kCreate3D9DeviceExName[] = "Direct3DCreate9Ex";
typedef HRESULT (WINAPI *Direct3DCreate9ExFunc)(UINT sdk_version,
                                                IDirect3D9Ex **d3d);
}  // namespace

namespace ui_surface_d3d9_utils {

bool LoadD3D9(base::ScopedNativeLibrary* storage) {
  storage->Reset(
      base::LoadNativeLibrary(base::FilePath(kD3D9ModuleName), NULL));
  return storage->is_valid();
}

bool CreateDevice(const base::ScopedNativeLibrary& d3d_module,
                  D3DDEVTYPE device_type,
                  uint32 presentation_interval,
                  IDirect3DDevice9Ex** device) {

  Direct3DCreate9ExFunc create_func = reinterpret_cast<Direct3DCreate9ExFunc>(
      d3d_module.GetFunctionPointer(kCreate3D9DeviceExName));
  if (!create_func)
    return false;

  base::win::ScopedComPtr<IDirect3D9Ex> d3d;
  HRESULT hr = create_func(D3D_SDK_VERSION, d3d.Receive());
  if (FAILED(hr))
    return false;

  // Any old window will do to create the device. In practice the window to
  // present to is an argument to IDirect3DDevice9::Present.
  HWND window = GetShellWindow();

  D3DPRESENT_PARAMETERS parameters = { 0 };
  parameters.BackBufferWidth = 1;
  parameters.BackBufferHeight = 1;
  parameters.BackBufferCount = 1;
  parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
  parameters.hDeviceWindow = window;
  parameters.Windowed = TRUE;
  parameters.Flags = 0;
  parameters.PresentationInterval = presentation_interval;
  parameters.SwapEffect = D3DSWAPEFFECT_COPY;

  hr = d3d->CreateDeviceEx(
      D3DADAPTER_DEFAULT,
      device_type,
      window,
      D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING |
          D3DCREATE_DISABLE_PSGP_THREADING | D3DCREATE_MULTITHREADED,
      &parameters,
      NULL,
      device);
  return SUCCEEDED(hr);
}

bool OpenSharedTexture(IDirect3DDevice9* device,
                       int64 surface_handle,
                       const gfx::Size& size,
                       IDirect3DTexture9** opened_texture) {
  TRACE_EVENT0("gpu", "OpenSharedTexture");
  HANDLE handle = reinterpret_cast<HANDLE>(surface_handle);
  HRESULT hr = device->CreateTexture(size.width(),
                                     size.height(),
                                     1,
                                     D3DUSAGE_RENDERTARGET,
                                     D3DFMT_A8R8G8B8,
                                     D3DPOOL_DEFAULT,
                                     opened_texture,
                                     &handle);
  return SUCCEEDED(hr);
}

bool CreateOrReuseLockableSurface(
    IDirect3DDevice9* device,
    const gfx::Size& size,
    base::win::ScopedComPtr<IDirect3DSurface9>* surface) {
  if (!*surface || GetSize(*surface) != size) {
    TRACE_EVENT0("gpu", "CreateRenderTarget");
    surface->Release();
    HRESULT hr = device->CreateRenderTarget(
          size.width(),
          size.height(),
          D3DFMT_A8R8G8B8,
          D3DMULTISAMPLE_NONE,
          0,
          TRUE,
          surface->Receive(),
          NULL);
    if (FAILED(hr))
      return false;
  }
  return true;
}

bool CreateOrReuseRenderTargetTexture(
    IDirect3DDevice9* device,
    const gfx::Size& size,
    base::win::ScopedComPtr<IDirect3DTexture9>* texture,
    IDirect3DSurface9** render_target) {
  if (!*texture || GetSize(*texture) != size) {
    TRACE_EVENT0("gpu", "CreateTexture");
    texture->Release();
    HRESULT hr = device->CreateTexture(
          size.width(),
          size.height(),
          1,  // Levels
          D3DUSAGE_RENDERTARGET,
          D3DFMT_A8R8G8B8,
          D3DPOOL_DEFAULT,
          texture->Receive(),
          NULL);
    if (!SUCCEEDED(hr))
      return false;
  }
  HRESULT hr = (*texture)->GetSurfaceLevel(0, render_target);
  return SUCCEEDED(hr);
}

gfx::Size GetSize(IDirect3DSurface9* surface) {
  D3DSURFACE_DESC surface_description;
  HRESULT hr = surface->GetDesc(&surface_description);
  if (FAILED(hr))
    return gfx::Size(0, 0);
  return gfx::Size(surface_description.Width, surface_description.Height);
}

gfx::Size GetSize(IDirect3DTexture9* texture) {
  D3DSURFACE_DESC surface_description;
  HRESULT hr = texture->GetLevelDesc(0, &surface_description);
  if (FAILED(hr))
    return gfx::Size(0, 0);
  return gfx::Size(surface_description.Width, surface_description.Height);
}

}  // namespace ui_surface_d3d9_utils
