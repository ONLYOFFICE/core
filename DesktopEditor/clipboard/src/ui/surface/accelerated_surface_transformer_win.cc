// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/surface/accelerated_surface_transformer_win.h"

#include <vector>

#include "accelerated_surface_transformer_win_hlsl_compiled.h"
#include "base/debug/trace_event.h"
#include "base/memory/ref_counted.h"
#include "base/metrics/histogram.h"
#include "base/single_thread_task_runner.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/win/scoped_comptr.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"
#include "ui/surface/d3d9_utils_win.h"
#include "ui/surface/surface_export.h"

using base::win::ScopedComPtr;
using std::vector;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kPsConvertRGBtoY8UV44;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kPsConvertUV44toU2V2;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kPsOneTexture;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kVsFetch2Pixels;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kVsFetch4Pixels;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kVsOneTexture;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kVsFetch4PixelsScale2;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kPsConvertRGBtoY;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kPsConvertRGBtoU;
using ui_surface::AcceleratedSurfaceTransformerWinHLSL::kPsConvertRGBtoV;

namespace d3d_utils = ui_surface_d3d9_utils;

namespace {

struct Vertex {
  float x, y, z, w;
  float u, v;
};

const static D3DVERTEXELEMENT9 g_vertexElements[] = {
  { 0, 0, D3DDECLTYPE_FLOAT4, 0, D3DDECLUSAGE_POSITION, 0 },
  { 0, 16, D3DDECLTYPE_FLOAT2, 0, D3DDECLUSAGE_TEXCOORD, 0 },
  D3DDECL_END()
};

class ScopedRenderTargetRestorer {
 public:
  ScopedRenderTargetRestorer(IDirect3DDevice9* device,
                             int render_target_id)
    : device_(device),
      target_id_(render_target_id) {
    device_->GetRenderTarget(target_id_, original_render_target_.Receive());
  }
  ~ScopedRenderTargetRestorer() {
    device_->SetRenderTarget(target_id_, original_render_target_);
  }
 private:
  ScopedComPtr<IDirect3DDevice9> device_;
  int target_id_;
  ScopedComPtr<IDirect3DSurface9> original_render_target_;
};

// Calculate the number necessary to transform |src_subrect| into |dst_size|
// by repeating downsampling of the image of |src_subrect| by a factor no more
// than 2.
int GetResampleCount(const gfx::Rect& src_subrect,
                     const gfx::Size& dst_size) {
  // At least one copy is required, since the back buffer itself is not
  // lockable.
  int min_resample_count = 1;
  int width_count = 0;
  int width = src_subrect.width();
  while (width > dst_size.width()) {
    ++width_count;
    width >>= 1;
  }
  int height_count = 0;
  int height = src_subrect.height();
  while (height > dst_size.height()) {
    ++height_count;
    height >>= 1;
  }
  return std::max(std::max(width_count, height_count),
                  min_resample_count);
}

// Returns half the size of |size| no smaller than |min_size|.
gfx::Size GetHalfSizeNoLessThan(const gfx::Size& size,
                                const gfx::Size& min_size) {
  return gfx::Size(std::max(min_size.width(), size.width() / 2),
                   std::max(min_size.height(), size.height() / 2));
}

}  // namespace

AcceleratedSurfaceTransformer::AcceleratedSurfaceTransformer()
    : device_supports_multiple_render_targets_(false),
      vertex_shader_sources_(),
      pixel_shader_sources_() {

  // Associate passes with actual shader programs.
  vertex_shader_sources_[ONE_TEXTURE] = kVsOneTexture;
  pixel_shader_sources_[ONE_TEXTURE] = kPsOneTexture;

  vertex_shader_sources_[RGB_TO_YV12_FAST__PASS_1_OF_2] = kVsFetch4Pixels;
  pixel_shader_sources_[RGB_TO_YV12_FAST__PASS_1_OF_2] = kPsConvertRGBtoY8UV44;

  vertex_shader_sources_[RGB_TO_YV12_FAST__PASS_2_OF_2] = kVsFetch2Pixels;
  pixel_shader_sources_[RGB_TO_YV12_FAST__PASS_2_OF_2] = kPsConvertUV44toU2V2;

  vertex_shader_sources_[RGB_TO_YV12_SLOW__PASS_1_OF_3] = kVsFetch4Pixels;
  pixel_shader_sources_[RGB_TO_YV12_SLOW__PASS_1_OF_3] = kPsConvertRGBtoY;

  vertex_shader_sources_[RGB_TO_YV12_SLOW__PASS_2_OF_3] = kVsFetch4PixelsScale2;
  pixel_shader_sources_[RGB_TO_YV12_SLOW__PASS_2_OF_3] = kPsConvertRGBtoU;

  vertex_shader_sources_[RGB_TO_YV12_SLOW__PASS_3_OF_3] = kVsFetch4PixelsScale2;
  pixel_shader_sources_[RGB_TO_YV12_SLOW__PASS_3_OF_3] = kPsConvertRGBtoV;

  COMPILE_ASSERT(NUM_SHADERS == 6, must_initialize_shader_sources);
}

bool AcceleratedSurfaceTransformer::Init(IDirect3DDevice9* device) {
  bool result = DoInit(device);
  if (!result) {
    ReleaseAll();
  }
  return result;
}

bool AcceleratedSurfaceTransformer::DoInit(IDirect3DDevice9* device) {
  device_ = device;

  {
    D3DCAPS9 caps;
    HRESULT hr = device->GetDeviceCaps(&caps);
    if (FAILED(hr))
      return false;

    device_supports_multiple_render_targets_ = (caps.NumSimultaneousRTs >= 2);

    // Log statistics about which paths we take.
    UMA_HISTOGRAM_BOOLEAN("GPU.AcceleratedSurfaceTransformerCanUseMRT",
                          device_supports_multiple_render_targets());
  }

  // Force compilation of all shaders that could be used on this GPU.
  if (!CompileShaderCombo(ONE_TEXTURE))
    return false;

  if (device_supports_multiple_render_targets()) {
    if (!CompileShaderCombo(RGB_TO_YV12_FAST__PASS_1_OF_2) ||
        !CompileShaderCombo(RGB_TO_YV12_FAST__PASS_2_OF_2)) {
      return false;
    }
  } else {
    if (!CompileShaderCombo(RGB_TO_YV12_SLOW__PASS_1_OF_3) ||
        !CompileShaderCombo(RGB_TO_YV12_SLOW__PASS_2_OF_3) ||
        !CompileShaderCombo(RGB_TO_YV12_SLOW__PASS_3_OF_3)) {
      return false;
    }
  }
  COMPILE_ASSERT(NUM_SHADERS == 6, must_compile_at_doinit);

  ScopedComPtr<IDirect3DVertexDeclaration9> vertex_declaration;
  HRESULT hr = device_->CreateVertexDeclaration(g_vertexElements,
                                                vertex_declaration.Receive());
  if (FAILED(hr))
    return false;
  hr = device_->SetVertexDeclaration(vertex_declaration);
  if (FAILED(hr))
    return false;

  return true;
}

bool AcceleratedSurfaceTransformer::CompileShaderCombo(
    ShaderCombo shader) {
  if (!vertex_shaders_[shader]) {
    HRESULT hr = device_->CreateVertexShader(
        reinterpret_cast<const DWORD*>(vertex_shader_sources_[shader]),
        vertex_shaders_[shader].Receive());

    if (FAILED(hr))
      return false;

    for (int i = 0; i < NUM_SHADERS; ++i) {
      if (vertex_shader_sources_[i] == vertex_shader_sources_[shader] &&
          i != shader) {
        vertex_shaders_[i] = vertex_shaders_[shader];
      }
    }
  }

  if (!pixel_shaders_[shader]) {
    HRESULT hr = device_->CreatePixelShader(
        reinterpret_cast<const DWORD*>(pixel_shader_sources_[shader]),
        pixel_shaders_[shader].Receive());

    if (FAILED(hr))
      return false;

    for (int i = 0; i < NUM_SHADERS; ++i) {
      if (pixel_shader_sources_[i] == pixel_shader_sources_[shader] &&
          i != shader) {
        pixel_shaders_[i] = pixel_shaders_[shader];
      }
    }
  }

  return true;
}

void AcceleratedSurfaceTransformer::ReleaseAll() {
  for (int i = 0; i < NUM_SHADERS; i++) {
    vertex_shaders_[i] = NULL;
    pixel_shaders_[i] = NULL;
  }

  user_scratch_texture_ = NULL;
  uv_scratch_texture_ = NULL;
  y_scratch_surface_ = NULL;
  u_scratch_surface_ = NULL;
  v_scratch_surface_ = NULL;
  for (int i = 0; i < arraysize(scaler_scratch_surfaces_); i++)
    scaler_scratch_surfaces_[i] = NULL;

  device_ = NULL;
}
void AcceleratedSurfaceTransformer::DetachAll() {
  for (int i = 0; i < NUM_SHADERS; i++) {
    vertex_shaders_[i].Detach();
    pixel_shaders_[i].Detach();
  }

  user_scratch_texture_.Detach();
  uv_scratch_texture_.Detach();
  y_scratch_surface_.Detach();
  u_scratch_surface_.Detach();
  v_scratch_surface_.Detach();
  for (int i = 0; i < arraysize(scaler_scratch_surfaces_); i++)
    scaler_scratch_surfaces_[i].Detach();

  device_.Detach();
}

bool AcceleratedSurfaceTransformer::CopyInverted(
    IDirect3DTexture9* src_texture,
    IDirect3DSurface9* dst_surface,
    const gfx::Size& dst_size) {
  return CopyWithTextureScale(src_texture, dst_surface, dst_size, 1.0f, -1.0f);
}

bool AcceleratedSurfaceTransformer::Copy(
    IDirect3DTexture9* src_texture,
    IDirect3DSurface9* dst_surface,
    const gfx::Size& dst_size) {
  return CopyWithTextureScale(src_texture, dst_surface, dst_size, 1.0f, 1.0f);
}

bool AcceleratedSurfaceTransformer::CopyWithTextureScale(
    IDirect3DTexture9* src_texture,
    IDirect3DSurface9* dst_surface,
    const gfx::Size& dst_size,
    float texture_scale_x,
    float texture_scale_y) {

  if (!SetShaderCombo(ONE_TEXTURE))
    return false;

  // Set the kTextureScale vertex shader constant, which is assigned to
  // register 1.
  float texture_scale[4] = {texture_scale_x, texture_scale_y, 0, 0};
  device()->SetVertexShaderConstantF(1, texture_scale, 1);

  ScopedRenderTargetRestorer render_target_restorer(device(), 0);
  device()->SetRenderTarget(0, dst_surface);
  device()->SetTexture(0, src_texture);

  D3DVIEWPORT9 viewport = {
    0, 0,
    dst_size.width(), dst_size.height(),
    0, 1
  };
  device()->SetViewport(&viewport);

  if (d3d_utils::GetSize(src_texture) == dst_size) {
    device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
  } else {
    device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  }
  device()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  device()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

  DrawScreenAlignedQuad(dst_size);

  // Clear surface references.
  device()->SetTexture(0, NULL);
  return true;
}

void AcceleratedSurfaceTransformer::DrawScreenAlignedQuad(
    const gfx::Size& size) {
  const float target_size[4] = { size.width(), size.height(), 0, 0};

  // Set the uniform shader constant |kRenderTargetSize|, which is bound
  // to register c0.
  device()->SetVertexShaderConstantF(0, target_size, 1);

  // We always send down the same vertices. The vertex program will take
  // care of doing resolution-dependent position adjustment.
  Vertex vertices[] = {
    { -1, +1, 0.5f, 1, 0, 0 },
    { +1, +1, 0.5f, 1, 1, 0 },
    { +1, -1, 0.5f, 1, 1, 1 },
    { -1, -1, 0.5f, 1, 0, 1 }
  };

  device()->BeginScene();
  device()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,
                            2,
                            vertices,
                            sizeof(vertices[0]));
  device()->EndScene();

}

bool AcceleratedSurfaceTransformer::GetIntermediateTexture(
    const gfx::Size& size,
    IDirect3DTexture9** texture,
    IDirect3DSurface9** texture_level_zero) {
  if (!d3d_utils::CreateOrReuseRenderTargetTexture(device(),
                                                   size,
                                                   &user_scratch_texture_,
                                                   texture_level_zero))
    return false;

  *texture = ScopedComPtr<IDirect3DTexture9>(user_scratch_texture_).Detach();
  return true;
}

// Resize an RGB surface using repeated linear interpolation.
bool AcceleratedSurfaceTransformer::ResizeBilinear(
    IDirect3DSurface9* src_surface,
    const gfx::Rect& src_subrect,
    IDirect3DSurface9* dst_surface,
    const gfx::Rect& dst_rect) {
  COMPILE_ASSERT(arraysize(scaler_scratch_surfaces_) == 2, surface_count);

  gfx::Size src_size = src_subrect.size();
  gfx::Size dst_size = dst_rect.size();

  if (src_size.IsEmpty() || dst_size.IsEmpty())
    return false;

  HRESULT hr = S_OK;
  // Set up intermediate buffers needed for downsampling.
  const int resample_count = GetResampleCount(src_subrect, dst_size);
  const gfx::Size half_size =
      GetHalfSizeNoLessThan(src_subrect.size(), dst_size);
  if (resample_count > 1) {
    if (!d3d_utils::CreateOrReuseLockableSurface(device(),
                                                 half_size,
                                                 &scaler_scratch_surfaces_[0]))
      return false;
  }
  if (resample_count > 2) {
    const gfx::Size quarter_size = GetHalfSizeNoLessThan(half_size, dst_size);
    if (!d3d_utils::CreateOrReuseLockableSurface(device(),
                                                 quarter_size,
                                                 &scaler_scratch_surfaces_[1]))
      return false;
  }

  // Repeat downsampling the surface until its size becomes identical to
  // |dst_size|. We keep the factor of each downsampling no more than two
  // because using a factor more than two can introduce aliasing.
  RECT read_rect = src_subrect.ToRECT();
  gfx::Size write_size = half_size;
  int read_buffer_index = 1;
  int write_buffer_index = 0;
  for (int i = 0; i < resample_count; ++i) {
    TRACE_EVENT0("gpu", "StretchRect");
    IDirect3DSurface9* read_buffer =
        (i == 0) ? src_surface : scaler_scratch_surfaces_[read_buffer_index];
    IDirect3DSurface9* write_buffer;
    RECT write_rect;
    if (i == resample_count - 1) {
      write_buffer = dst_surface;
      write_rect = dst_rect.ToRECT();
    } else {
      write_buffer = scaler_scratch_surfaces_[write_buffer_index];
      write_rect = gfx::Rect(write_size).ToRECT();
    }

    hr = device()->StretchRect(read_buffer,
                               &read_rect,
                               write_buffer,
                               &write_rect,
                               D3DTEXF_LINEAR);

    if (FAILED(hr))
      return false;
    read_rect = write_rect;
    write_size = GetHalfSizeNoLessThan(write_size, dst_size);
    std::swap(read_buffer_index, write_buffer_index);
  }

  return true;
}

bool AcceleratedSurfaceTransformer::TransformRGBToYV12(
    IDirect3DTexture9* src_surface,
    const gfx::Size& dst_size,
    IDirect3DSurface9** dst_y,
    IDirect3DSurface9** dst_u,
    IDirect3DSurface9** dst_v) {
  gfx::Size packed_y_size;
  gfx::Size packed_uv_size;
  if (!AllocYUVBuffers(dst_size, &packed_y_size, &packed_uv_size,
                       dst_y, dst_u, dst_v)) {
    return false;
  }

  if (device_supports_multiple_render_targets()) {
    return TransformRGBToYV12_MRT(src_surface,
                                  dst_size,
                                  packed_y_size,
                                  packed_uv_size,
                                  *dst_y,
                                  *dst_u,
                                  *dst_v);
  } else {
    return TransformRGBToYV12_WithoutMRT(src_surface,
                                         dst_size,
                                         packed_y_size,
                                         packed_uv_size,
                                         *dst_y,
                                         *dst_u,
                                         *dst_v);
  }
}

bool AcceleratedSurfaceTransformer::ReadFast(IDirect3DSurface9* gpu_surface,
                                             uint8* dst,
                                             int dst_bytes_per_row,
                                             int dst_num_rows,
                                             int dst_stride) {
  // TODO(nick): Compared to GetRenderTargetData, LockRect+memcpy is 50% faster
  // on some systems, but 100x slower on others. We should have logic here to
  // choose the best path, probably by adaptively trying both and picking the
  // faster one. http://crbug.com/168532
  return ReadByGetRenderTargetData(gpu_surface, dst, dst_bytes_per_row,
                                   dst_num_rows, dst_stride);
}

bool AcceleratedSurfaceTransformer::ReadByLockAndCopy(
    IDirect3DSurface9* gpu_surface,
    uint8* dst,
    int dst_bytes_per_row,
    int dst_num_rows,
    int dst_stride) {
  D3DLOCKED_RECT locked_rect;
  {
    TRACE_EVENT0("gpu", "LockRect");
    HRESULT hr = gpu_surface->LockRect(&locked_rect, NULL,
                                       D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK);
    if (FAILED(hr)) {
      LOG(ERROR) << "Failed to lock surface";
      return false;
    }
  }

  {
    TRACE_EVENT0("gpu", "memcpy");
    uint8* dst_row = dst;
    uint8* src_row = reinterpret_cast<uint8*>(locked_rect.pBits);
    for (int i = 0; i < dst_num_rows; i++) {
      memcpy(dst_row, src_row, dst_bytes_per_row);
      src_row += locked_rect.Pitch;
      dst_row += dst_stride;
    }
  }
  gpu_surface->UnlockRect();
  return true;
}

bool AcceleratedSurfaceTransformer::ReadByGetRenderTargetData(
    IDirect3DSurface9* gpu_surface,
    uint8* dst,
    int dst_bytes_per_row,
    int dst_num_rows,
    int dst_stride) {
  HRESULT hr = 0;
  ScopedComPtr<IDirect3DSurface9> system_surface;
  gfx::Size src_size = d3d_utils::GetSize(gpu_surface);

  // Depending on pitch and alignment, we might be able to wrap |dst| in an
  // offscreen- plain surface for a direct copy.
  const bool direct_copy = (dst_stride == dst_bytes_per_row &&
                            src_size.width() * 4 == dst_bytes_per_row &&
                            dst_num_rows >= src_size.height());

  {
    TRACE_EVENT0("gpu", "CreateOffscreenPlainSurface");
    HANDLE handle = reinterpret_cast<HANDLE>(dst);
    hr = device()->CreateOffscreenPlainSurface(src_size.width(),
                                               src_size.height(),
                                               D3DFMT_A8R8G8B8,
                                               D3DPOOL_SYSTEMMEM,
                                               system_surface.Receive(),
                                               direct_copy ? &handle : NULL);
    if (!SUCCEEDED(hr)) {
      LOG(ERROR) << "Failed to create offscreen plain surface.";
      return false;
    }
  }

  {
    TRACE_EVENT0("gpu", "GetRenderTargetData");
    hr = device()->GetRenderTargetData(gpu_surface, system_surface);
    if (FAILED(hr)) {
      LOG(ERROR) << "Failed GetRenderTargetData";
      return false;
    }
  }

  if (direct_copy) {
    // We're done: |system_surface| is a wrapper around |dst|.
    return true;
  } else {
    // Extra memcpy required from |system_surface| to |dst|.
    return ReadByLockAndCopy(system_surface, dst, dst_bytes_per_row,
                             dst_num_rows, dst_stride);
  }
}

bool AcceleratedSurfaceTransformer::AllocYUVBuffers(
    const gfx::Size& dst_size,
    gfx::Size* y_size,
    gfx::Size* uv_size,
    IDirect3DSurface9** dst_y,
    IDirect3DSurface9** dst_u,
    IDirect3DSurface9** dst_v) {

  // Y is full height, packed into 4 components.
  *y_size = gfx::Size((dst_size.width() + 3) / 4, dst_size.height());

  // U and V are half the size (rounded up) of Y.
  *uv_size = gfx::Size((y_size->width() + 1) / 2, (y_size->height() + 1) / 2);

  if (!d3d_utils::CreateOrReuseLockableSurface(device(), *y_size,
                                               &y_scratch_surface_)) {
    return false;
  }
  if (!d3d_utils::CreateOrReuseLockableSurface(device(), *uv_size,
                                               &u_scratch_surface_)) {
    return false;
  }
  if (!d3d_utils::CreateOrReuseLockableSurface(device(), *uv_size,
                                               &v_scratch_surface_)) {
    return false;
  }

  *dst_y = ScopedComPtr<IDirect3DSurface9>(y_scratch_surface_).Detach();
  *dst_u = ScopedComPtr<IDirect3DSurface9>(u_scratch_surface_).Detach();
  *dst_v = ScopedComPtr<IDirect3DSurface9>(v_scratch_surface_).Detach();

  return true;
}

bool AcceleratedSurfaceTransformer::TransformRGBToYV12_MRT(
    IDirect3DTexture9* src_surface,
    const gfx::Size& dst_size,
    const gfx::Size& packed_y_size,
    const gfx::Size& packed_uv_size,
    IDirect3DSurface9* dst_y,
    IDirect3DSurface9* dst_u,
    IDirect3DSurface9* dst_v) {
  TRACE_EVENT0("gpu", "RGBToYV12_MRT");

  ScopedRenderTargetRestorer color0_restorer(device(), 0);
  ScopedRenderTargetRestorer color1_restorer(device(), 1);

  // Create an intermediate surface to hold the UUVV values. This is color
  // target 1 for the first pass, and texture 0 for the second pass. Its
  // values are not read afterwards.

  ScopedComPtr<IDirect3DSurface9> uv_as_surface;
  if (!d3d_utils::CreateOrReuseRenderTargetTexture(device(),
                                                   packed_y_size,
                                                   &uv_scratch_texture_,
                                                   uv_as_surface.Receive())) {
    return false;
  }

  // Clamping is required if (dst_size.width() % 8 != 0) or if
  // (dst_size.height != 0), so we set it always. Both passes rely on this.
  device()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  device()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

  /////////////////////////////////////////
  // Pass 1: RGB --(scaled)--> YYYY + UUVV
  SetShaderCombo(RGB_TO_YV12_FAST__PASS_1_OF_2);

  // Enable bilinear filtering if scaling is required. The filtering will take
  // place entirely in the first pass.
  if (d3d_utils::GetSize(src_surface) != dst_size) {
    device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  } else {
    device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
  }

  device()->SetTexture(0, src_surface);
  device()->SetRenderTarget(0, dst_y);
  device()->SetRenderTarget(1, uv_as_surface);
  DrawScreenAlignedQuad(dst_size);

  /////////////////////////////////////////
  // Pass 2: UUVV -> UUUU + VVVV
  SetShaderCombo(RGB_TO_YV12_FAST__PASS_2_OF_2);

  // The second pass uses bilinear minification to achieve vertical scaling,
  // so enable it always.
  device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
  device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

  device()->SetTexture(0, uv_scratch_texture_);
  device()->SetRenderTarget(0, dst_u);
  device()->SetRenderTarget(1, dst_v);
  DrawScreenAlignedQuad(packed_y_size);

  // Clear surface references.
  device()->SetTexture(0, NULL);
  return true;
}

bool AcceleratedSurfaceTransformer::TransformRGBToYV12_WithoutMRT(
    IDirect3DTexture9* src_surface,
    const gfx::Size& dst_size,
    const gfx::Size& packed_y_size,
    const gfx::Size& packed_uv_size,
    IDirect3DSurface9* dst_y,
    IDirect3DSurface9* dst_u,
    IDirect3DSurface9* dst_v) {
  TRACE_EVENT0("gpu", "RGBToYV12_WithoutMRT");

  ScopedRenderTargetRestorer color0_restorer(device(), 0);

  ScopedComPtr<IDirect3DTexture9> scaled_src_surface;

  // If scaling is requested, do it to a temporary texture. The MRT path
  // gets a scale for free, so we need to support it here too (even though
  // it's an extra operation).
  if (d3d_utils::GetSize(src_surface) == dst_size) {
    scaled_src_surface = src_surface;
  } else {
    ScopedComPtr<IDirect3DSurface9> dst_level0;
    if (!d3d_utils::CreateOrReuseRenderTargetTexture(
            device(), dst_size, &uv_scratch_texture_, dst_level0.Receive())) {
      return false;
    }
    if (!Copy(src_surface, dst_level0, dst_size)) {
      return false;
    }
    scaled_src_surface = uv_scratch_texture_;
  }

  // Input texture is the same for all three passes.
  device()->SetTexture(0, scaled_src_surface);

  // Clamping is required if (dst_size.width() % 8 != 0) or if
  // (dst_size.height != 0), so we set it always. All passes rely on this.
  device()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  device()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

  /////////////////////
  // Pass 1: RGB -> Y.
  SetShaderCombo(RGB_TO_YV12_SLOW__PASS_1_OF_3);

  // Pass 1 just needs point sampling.
  device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
  device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

  device()->SetRenderTarget(0, dst_y);
  DrawScreenAlignedQuad(dst_size);

  // Passes 2 and 3 rely on bilinear minification to downsample U and V.
  device()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
  device()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

  /////////////////////
  // Pass 2: RGB -> U.
  SetShaderCombo(RGB_TO_YV12_SLOW__PASS_2_OF_3);
  device()->SetRenderTarget(0, dst_u);
  DrawScreenAlignedQuad(dst_size);

  /////////////////////
  // Pass 3: RGB -> V.
  SetShaderCombo(RGB_TO_YV12_SLOW__PASS_3_OF_3);
  device()->SetRenderTarget(0, dst_v);
  DrawScreenAlignedQuad(dst_size);

  // Clear surface references.
  device()->SetTexture(0, NULL);
  return true;
}

IDirect3DDevice9* AcceleratedSurfaceTransformer::device() {
  return device_;
}

bool AcceleratedSurfaceTransformer::SetShaderCombo(ShaderCombo combo) {
  // Compile shaders on first use, if needed. Normally the compilation should
  // already have happened at Init() time, but test code might force
  // us down an unusual path.
  if (!CompileShaderCombo(combo))
    return false;

  HRESULT hr = device()->SetVertexShader(vertex_shaders_[combo]);
  if (!SUCCEEDED(hr))
    return false;
  hr = device()->SetPixelShader(pixel_shaders_[combo]);
  if (!SUCCEEDED(hr))
    return false;
  return true;
}
