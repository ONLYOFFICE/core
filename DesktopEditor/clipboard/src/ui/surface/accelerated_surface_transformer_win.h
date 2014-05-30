// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SURFACE_ACCELERATED_SURFACE_TRANSFORMER_WIN_H_
#define UI_SURFACE_ACCELERATED_SURFACE_TRANSFORMER_WIN_H_

#include <d3d9.h>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/single_thread_task_runner.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "base/win/scoped_comptr.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/surface/surface_export.h"

namespace gfx {
class Size;
class Rect;
}  // namespace gfx

// Provides useful image filtering operations that are implemented
// efficiently on DirectX9-class hardware using fragment programs.
class SURFACE_EXPORT AcceleratedSurfaceTransformer {
 public:
  // Constructs an uninitialized surface transformer. Call Init() before
  // using the resulting object.
  AcceleratedSurfaceTransformer();

  // Init() initializes the transformer to operate on a device. This must be
  // called before any other method of this class, and it must be called
  // again after ReleaseAll() or DetachAll() before the class is used.
  //
  // Returns true if successful.
  bool Init(IDirect3DDevice9* device);

  // ReleaseAll() releases all direct3d resource references.
  void ReleaseAll();

  // DetachAll() leaks all direct3d resource references. This exists in order to
  // work around particular driver bugs, and should only be called at shutdown.
  // TODO(ncarter): Update the leak expectations before checkin.
  void DetachAll();

  // Draw a textured quad to a surface, flipping orientation in the y direction.
  bool CopyInverted(
      IDirect3DTexture9* src_texture,
      IDirect3DSurface9* dst_surface,
      const gfx::Size& dst_size);

  // Draw a textured quad to a surface.
  bool Copy(
      IDirect3DTexture9* src_texture,
      IDirect3DSurface9* dst_surface,
      const gfx::Size& dst_size);

  // Get an intermediate buffer of a particular |size|, that can be used as the
  // output of one transformation and the to another. The returned surface
  // belongs to an internal cache, and is invalidated by a subsequent call to
  // this method.
  bool GetIntermediateTexture(
      const gfx::Size& size,
      IDirect3DTexture9** texture,
      IDirect3DSurface9** texture_level_zero);

  // Resize a surface using repeated bilinear interpolation.
  bool ResizeBilinear(
    IDirect3DSurface9* src_surface,
    const gfx::Rect& src_subrect,
    IDirect3DSurface9* dst_surface,
    const gfx::Rect& dst_subrect);

  // Color format conversion from RGB to planar YV12 (also known as YUV420).
  //
  // YV12 is effectively a twelve bit per pixel format consisting of a full-
  // size y (luminance) plane and half-width, half-height u and v (blue and
  // red chrominance) planes. This method will allocate three lockable surfaces,
  // one for each plane, and return them via the arguments |dst_y|, |dst_u|,
  // and |dst_v|. These surface will be created with an ARGB D3DFORMAT, but
  // should be interpreted as the appropriate single-byte format when locking.
  //
  // The dimensions of the outputs (when interpreted as single-component data)
  // are as follows:
  //   |dst_y| : width and height exactly |dst_size|
  //   |dst_u| : width and height are each half of |dst_size|, rounded up.
  //   |dst_v| : width and height are each half of |dst_size|, rounded up.
  //
  // If |src_texture|'s dimensions do not match |dst_size|, the source will be
  // bilinearly interpolated during conversion.
  //
  // Returns true if successful. Caller must be certain to release the surfaces
  // even if this function returns false. The returned surfaces belong to an
  // internal cache, and are invalidated by a subsequent call to this method.
  bool TransformRGBToYV12(
      IDirect3DTexture9* src_texture,
      const gfx::Size& dst_size,
      IDirect3DSurface9** dst_y,
      IDirect3DSurface9** dst_u,
      IDirect3DSurface9** dst_v);

  // Synchronously copy from a D3D surface into a caller-allocated buffer. This
  // will dispatch to one of a couple techniques, depending on which is
  // determined to be the faster method for the current device.
  bool ReadFast(IDirect3DSurface9* gpu_surface,
                uint8* dst,
                int dst_bytes_per_row,
                int dst_num_rows,
                int dst_stride);

  // Do a read using a particular technique. Which of these is faster depends on
  // the hardware. Intended for testing; production code ought to call
  // ReadFast().
  bool ReadByLockAndCopy(IDirect3DSurface9* gpu_surface,
                         uint8* dst,
                         int dst_bytes_per_row,
                         int dst_num_rows,
                         int dst_stride);
  bool ReadByGetRenderTargetData(IDirect3DSurface9* gpu_surface,
                                 uint8* dst,
                                 int dst_bytes_per_row,
                                 int dst_num_rows,
                                 int dst_stride);

 private:
  friend class AcceleratedSurfaceTransformerTest;
  FRIEND_TEST_ALL_PREFIXES(AcceleratedSurfaceTransformerTest, Init);

  enum ShaderCombo {
    ONE_TEXTURE,
    RGB_TO_YV12_FAST__PASS_1_OF_2,
    RGB_TO_YV12_FAST__PASS_2_OF_2,
    RGB_TO_YV12_SLOW__PASS_1_OF_3,
    RGB_TO_YV12_SLOW__PASS_2_OF_3,
    RGB_TO_YV12_SLOW__PASS_3_OF_3,
    NUM_SHADERS
  };

  // Efficient RGB->YV12 in two passes, but requires a device capable of writing
  // multiple render targets at the same time.
  //
  // Returns true if successful.
  bool TransformRGBToYV12_MRT(
      IDirect3DTexture9* src_surface,
      const gfx::Size& dst_size,
      const gfx::Size& packed_y_size,
      const gfx::Size& packed_uv_size,
      IDirect3DSurface9* dst_y,
      IDirect3DSurface9* dst_u,
      IDirect3DSurface9* dst_v);

  // Slower, less efficient RGB->YV12; does not require the device to have
  // multiple render target capability. Runs at about half speed of the fast
  // path.
  //
  // Returns true if successful.
  bool TransformRGBToYV12_WithoutMRT(
      IDirect3DTexture9* src_surface,
      const gfx::Size& dst_size,
      const gfx::Size& packed_y_size,
      const gfx::Size& packed_uv_size,
      IDirect3DSurface9* dst_y,
      IDirect3DSurface9* dst_u,
      IDirect3DSurface9* dst_v);

  // Helper to allocate appropriately size YUV buffers, accounting for various
  // roundings. The sizes of the buffers (in terms of ARGB pixels) are returned
  // as |packed_y_size| and |packed_uv_size|.
  //
  // Returns true if successful. Caller must be certain to release the surfaces
  // even if this function returns false. The returned belong to an internal
  // cache.
  bool AllocYUVBuffers(
      const gfx::Size& dst_size,
      gfx::Size* packed_y_size,
      gfx::Size* packed_uv_size,
      IDirect3DSurface9** dst_y,
      IDirect3DSurface9** dst_u,
      IDirect3DSurface9** dst_v);

  bool CopyWithTextureScale(
      IDirect3DTexture9* src_texture,
      IDirect3DSurface9* dst_surface,
      const gfx::Size& dst_size,
      float texture_scale_x,
      float texture_scale_y);

  // Set the active vertex and pixel shader combination.
  //
  // Returns true if successful.
  bool SetShaderCombo(ShaderCombo combo);

  // Compiles a vertex and pixel shader combination, if not already compiled.
  //
  // Returns true if successful.
  bool CompileShaderCombo(ShaderCombo shader_combo_name);

  bool DoInit(IDirect3DDevice9* device);

  void DrawScreenAlignedQuad(const gfx::Size& dst_size);

  bool device_supports_multiple_render_targets() const {
    return device_supports_multiple_render_targets_;
  }

  IDirect3DDevice9* device();

  base::win::ScopedComPtr<IDirect3DDevice9> device_;
  base::win::ScopedComPtr<IDirect3DVertexShader9> vertex_shaders_[NUM_SHADERS];
  base::win::ScopedComPtr<IDirect3DPixelShader9> pixel_shaders_[NUM_SHADERS];

  // Temporary and scratch surfaces; cached to avoid frequent reallocation.
  base::win::ScopedComPtr<IDirect3DTexture9> user_scratch_texture_;
  base::win::ScopedComPtr<IDirect3DTexture9> uv_scratch_texture_;
  base::win::ScopedComPtr<IDirect3DSurface9> y_scratch_surface_;
  base::win::ScopedComPtr<IDirect3DSurface9> u_scratch_surface_;
  base::win::ScopedComPtr<IDirect3DSurface9> v_scratch_surface_;
  base::win::ScopedComPtr<IDirect3DSurface9> scaler_scratch_surfaces_[2];

  bool device_supports_multiple_render_targets_;
  const BYTE* vertex_shader_sources_[NUM_SHADERS];
  const BYTE* pixel_shader_sources_[NUM_SHADERS];
  DISALLOW_COPY_AND_ASSIGN(AcceleratedSurfaceTransformer);
};

#endif  // UI_SURFACE_ACCELERATED_SURFACE_TRANSFORMER_WIN_H_
