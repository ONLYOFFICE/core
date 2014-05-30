// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <d3d9.h>
#include <random>

#include "base/basictypes.h"
#include "base/file_util.h"
#include "base/hash.h"
#include "base/scoped_native_library.h"
#include "base/stringprintf.h"
#include "base/time.h"
#include "base/win/scoped_comptr.h"
#include "base/win/windows_version.h"
#include "media/base/simd/convert_rgb_to_yuv.h"
#include "media/base/yuv_convert.h"
#include "skia/ext/image_operations.h"
#include "testing/gtest/include/gtest/gtest-param-test.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/codec/png_codec.h"
#include "ui/gfx/rect.h"
#include "ui/surface/accelerated_surface_transformer_win.h"
#include "ui/surface/accelerated_surface_win.h"
#include "ui/surface/d3d9_utils_win.h"

namespace d3d_utils = ui_surface_d3d9_utils;

using base::win::ScopedComPtr;
using std::uniform_int_distribution;

namespace {

// Debug flag, useful when hacking on tests.
const bool kDumpImagesOnFailure = false;

SkBitmap ToSkBitmap(IDirect3DSurface9* surface, bool is_single_channel) {
  D3DLOCKED_RECT locked_rect;
  EXPECT_HRESULT_SUCCEEDED(
      surface->LockRect(&locked_rect, NULL, D3DLOCK_READONLY));

  SkBitmap result;
  gfx::Size size = d3d_utils::GetSize(surface);
  if (is_single_channel)
    size = gfx::Size(size.width() * 4, size.height());
  result.setConfig(SkBitmap::kARGB_8888_Config, size.width(), size.height());
  result.setIsOpaque(true);
  result.allocPixels();
  result.lockPixels();
  for (int y = 0; y < size.height(); ++y) {
    uint8* row8 = reinterpret_cast<uint8*>(locked_rect.pBits) +
        (y * locked_rect.Pitch);
    if (is_single_channel) {
      for (int x = 0; x < size.width(); ++x) {
        *result.getAddr32(x, y) = SkColorSetRGB(row8[x], row8[x], row8[x]);
      }
    } else {
      uint32* row32 = reinterpret_cast<uint32*>(row8);
      for (int x = 0; x < size.width(); ++x) {
        *result.getAddr32(x, y) = row32[x] | 0xFF000000;
      }
    }
  }
  result.unlockPixels();
  result.setImmutable();
  surface->UnlockRect();
  return result;
}

bool WritePNGFile(const SkBitmap& bitmap, const base::FilePath& file_path) {
  std::vector<unsigned char> png_data;
  const bool discard_transparency = true;
  if (gfx::PNGCodec::EncodeBGRASkBitmap(bitmap,
                                        discard_transparency,
                                        &png_data) &&
      file_util::CreateDirectory(file_path.DirName())) {
    char* data = reinterpret_cast<char*>(&png_data[0]);
    int size = static_cast<int>(png_data.size());
    return file_util::WriteFile(file_path, data, size) == size;
  }
  return false;
}

}  // namespace

// Test fixture for AcceleratedSurfaceTransformer.
//
// This class is parameterized so that it runs only on Vista+. See
// WindowsVersionIfVistaOrBetter() for details on this works.
class AcceleratedSurfaceTransformerTest : public testing::TestWithParam<int> {
 public:
  AcceleratedSurfaceTransformerTest() : color_error_tolerance_(0) {};

  IDirect3DDevice9Ex* device() { return device_.get(); }

  virtual void SetUp() {
    if (!d3d_module_.is_valid()) {
      if (!d3d_utils::LoadD3D9(&d3d_module_)) {
        GTEST_FAIL() << "Could not load d3d9.dll";
        return;
      }
    }
    if (!d3d_utils::CreateDevice(d3d_module_,
                                 D3DDEVTYPE_HAL,
                                 D3DPRESENT_INTERVAL_IMMEDIATE,
                                 device_.Receive())) {
      GTEST_FAIL() << "Could not create Direct3D device.";
      return;
    }

    SeedRandom("default");
  }

  virtual void TearDown() {
    device_ = NULL;
  }

  // Gets a human-readable identifier of the graphics hardware being used,
  // intended for use inside of SCOPED_TRACE().
  std::string GetAdapterInfo() {
    ScopedComPtr<IDirect3D9> d3d;
    EXPECT_HRESULT_SUCCEEDED(device()->GetDirect3D(d3d.Receive()));
    D3DADAPTER_IDENTIFIER9 info;
    EXPECT_HRESULT_SUCCEEDED(d3d->GetAdapterIdentifier(0, 0, &info));
    return base::StringPrintf(
        "Running on graphics hardware: %s", info.Description);
  }

  void SeedRandom(const char* seed) {
    rng_.seed(base::Hash(seed));
    random_dword_.reset();
  }

  // Driver workaround: on an Intel GPU (Mobile Intel 965 Express), it seems
  // necessary to flush between drawing and locking, for the synchronization
  // to behave properly.
  void BeforeLockWorkaround() {
    EXPECT_HRESULT_SUCCEEDED(
        device()->Present(0, 0, 0, 0));
  }

  void WarnOnMissingFeatures(AcceleratedSurfaceTransformer* gpu_ops) {
    // Prints a single warning line if some tests are feature-dependent
    // and the feature is not supported by the current GPU.
    if (!gpu_ops->device_supports_multiple_render_targets()) {
      LOG(WARNING) << "MRT not supported, some tests will be skipped. "
                   << GetAdapterInfo();
    }
  }

  // Locks and fills a surface with a checkerboard pattern where the colors
  // are random but the total image pattern is horizontally and vertically
  // symmetric.
  void FillSymmetricRandomCheckerboard(
      IDirect3DSurface9* lockable_surface,
      const gfx::Size& size,
      int checker_square_size) {

    D3DLOCKED_RECT locked_rect;
    ASSERT_HRESULT_SUCCEEDED(
        lockable_surface->LockRect(&locked_rect, NULL, D3DLOCK_DISCARD));
    DWORD* surface = reinterpret_cast<DWORD*>(locked_rect.pBits);
    ASSERT_EQ(0, locked_rect.Pitch % sizeof(DWORD));
    int pitch = locked_rect.Pitch / sizeof(DWORD);

    for (int y = 0; y < (size.height() + 1) / 2; y += checker_square_size) {
      for (int x = 0; x < (size.width() + 1) / 2; x += checker_square_size) {
        DWORD color = RandomColor();
        int y_limit = std::min(size.height() / 2, y + checker_square_size - 1);
        int x_limit = std::min(size.width() / 2, x + checker_square_size - 1);
        for (int y_lo = y; y_lo <= y_limit; y_lo++) {
          for (int x_lo = x; x_lo <= x_limit; x_lo++) {
            int y_hi = size.height() - 1 - y_lo;
            int x_hi = size.width() - 1 - x_lo;
            surface[x_lo + y_lo*pitch] = color;
            surface[x_lo + y_hi*pitch] = color;
            surface[x_hi + y_lo*pitch] = color;
            surface[x_hi + y_hi*pitch] = color;
          }
        }
      }
    }

    lockable_surface->UnlockRect();
  }

  void FillRandomCheckerboard(
      IDirect3DSurface9* lockable_surface,
      const gfx::Size& size,
      int checker_square_size) {

    D3DLOCKED_RECT locked_rect;
    ASSERT_HRESULT_SUCCEEDED(
        lockable_surface->LockRect(&locked_rect, NULL, D3DLOCK_DISCARD));
    DWORD* surface = reinterpret_cast<DWORD*>(locked_rect.pBits);
    ASSERT_EQ(0, locked_rect.Pitch % sizeof(DWORD));
    int pitch = locked_rect.Pitch / sizeof(DWORD);

    for (int y = 0; y <= size.height(); y += checker_square_size) {
      for (int x = 0; x <= size.width(); x += checker_square_size) {
        DWORD color = RandomColor();
        int y_limit = std::min(size.height(), y + checker_square_size);
        int x_limit = std::min(size.width(), x + checker_square_size);
        for (int square_y = y; square_y < y_limit; square_y++) {
          for (int square_x = x; square_x < x_limit; square_x++) {
            surface[square_x + square_y*pitch] = color;
          }
        }
      }
    }

    lockable_surface->UnlockRect();
  }

  // Approximate color-equality check. Allows for some rounding error.
  bool AssertSameColor(DWORD color_a, DWORD color_b) {
    if (color_a == color_b)
      return true;
    uint8* a = reinterpret_cast<uint8*>(&color_a);
    uint8* b = reinterpret_cast<uint8*>(&color_b);
    int max_error = 0;
    for (int i = 0; i < 4; i++)
      max_error = std::max(max_error,
          std::abs(static_cast<int>(a[i]) - b[i]));

    if (max_error <= color_error_tolerance())
      return true;

    std::string expected_color =
        base::StringPrintf("%3d, %3d, %3d, %3d", a[0], a[1], a[2], a[3]);
    std::string actual_color =
        base::StringPrintf("%3d, %3d, %3d, %3d", b[0], b[1], b[2], b[3]);
    EXPECT_EQ(expected_color, actual_color)
        << "Componentwise color difference was "
        << max_error << "; max allowed is " << color_error_tolerance();

    return false;
  }

bool AssertSameColor(uint8 color_a, uint8 color_b) {
    if (color_a == color_b)
      return true;
    int max_error = std::abs((int) color_a - (int) color_b);
    if (max_error <= color_error_tolerance())
      return true;
    ADD_FAILURE() << "Colors not equal: "
                  << base::StringPrintf("0x%x", color_a)
                  << " vs. " << base::StringPrintf("0x%x", color_b);
    return false;
  }

  // Asserts that an image is symmetric with respect to itself: both
  // horizontally and vertically, within the tolerance of AssertSameColor.
  void AssertSymmetry(IDirect3DSurface9* lockable_surface,
                      const gfx::Size& size) {
    BeforeLockWorkaround();

    D3DLOCKED_RECT locked_rect;
    ASSERT_HRESULT_SUCCEEDED(
        lockable_surface->LockRect(&locked_rect, NULL, D3DLOCK_READONLY));
    ASSERT_EQ(0, locked_rect.Pitch % sizeof(DWORD));
    int pitch = locked_rect.Pitch / sizeof(DWORD);
    DWORD* surface = reinterpret_cast<DWORD*>(locked_rect.pBits);
    for (int y_lo = 0; y_lo < size.height() / 2; y_lo++) {
      int y_hi = size.height() - 1 - y_lo;
      for (int x_lo = 0; x_lo < size.width() / 2; x_lo++) {
        int x_hi = size.width() - 1 - x_lo;
        if (!AssertSameColor(surface[x_lo + y_lo*pitch],
                             surface[x_hi + y_lo*pitch])) {
          lockable_surface->UnlockRect();
          GTEST_FAIL() << "Pixels (" << x_lo << ", " << y_lo << ") vs. "
                       << "(" << x_hi << ", " << y_lo << ")";
        }
        if (!AssertSameColor(surface[x_hi + y_lo*pitch],
                             surface[x_hi + y_hi*pitch])) {
          lockable_surface->UnlockRect();
          GTEST_FAIL() << "Pixels (" << x_hi << ", " << y_lo << ") vs. "
                       << "(" << x_hi << ", " << y_hi << ")";
        }
        if (!AssertSameColor(surface[x_hi + y_hi*pitch],
                             surface[x_lo + y_hi*pitch])) {
          lockable_surface->UnlockRect();
          GTEST_FAIL() << "Pixels (" << x_hi << ", " << y_hi << ") vs. "
                       << "(" << x_lo << ", " << y_hi << ")";
        }
      }
    }
    lockable_surface->UnlockRect();
  }

  // Asserts that the actual image is a bit-identical, vertically mirrored
  // copy of the expected image.
  void AssertIsInvertedCopy(const gfx::Size& size,
                            IDirect3DSurface9* expected,
                            IDirect3DSurface9* actual) {
    BeforeLockWorkaround();

    D3DLOCKED_RECT locked_expected, locked_actual;
    ASSERT_HRESULT_SUCCEEDED(
        expected->LockRect(&locked_expected, NULL, D3DLOCK_READONLY));
    ASSERT_HRESULT_SUCCEEDED(
        actual->LockRect(&locked_actual, NULL, D3DLOCK_READONLY));
    ASSERT_EQ(0, locked_expected.Pitch % sizeof(DWORD));
    int pitch = locked_expected.Pitch / sizeof(DWORD);
    DWORD* expected_image = reinterpret_cast<DWORD*>(locked_expected.pBits);
    DWORD* actual_image = reinterpret_cast<DWORD*>(locked_actual.pBits);
    for (int y = 0; y < size.height(); y++) {
      int y_actual = size.height() - 1 - y;
      for (int x = 0; x < size.width(); ++x)
        if (!AssertSameColor(expected_image[y*pitch + x],
                             actual_image[y_actual*pitch + x])) {
          expected->UnlockRect();
          actual->UnlockRect();
          GTEST_FAIL() << "Pixels (" << x << ", " << y << ") vs. "
                       << "(" << x << ", " << y_actual << ")";
        }
    }
    expected->UnlockRect();
    actual->UnlockRect();
  }

 protected:
  DWORD RandomColor() {
    return random_dword_(rng_);
  }

  void set_color_error_tolerance(int value) {
    color_error_tolerance_ = value;
  }

  int color_error_tolerance() {
    return color_error_tolerance_;
  }

  void DoResizeBilinearTest(AcceleratedSurfaceTransformer* gpu_ops,
                            const gfx::Size& src_size,
                            const gfx::Size& dst_size,
                            int checkerboard_size) {

    SCOPED_TRACE(
        base::StringPrintf(
            "Resizing %dx%d -> %dx%d at checkerboard size of %d",
            src_size.width(), src_size.height(),
            dst_size.width(), dst_size.height(),
            checkerboard_size));

    set_color_error_tolerance(4);

    base::win::ScopedComPtr<IDirect3DSurface9> src, dst;
    ASSERT_TRUE(d3d_utils::CreateOrReuseLockableSurface(
        device(), src_size, &src))
            << "Could not create src render target";
    ASSERT_TRUE(d3d_utils::CreateOrReuseLockableSurface(
        device(), dst_size, &dst))
            << "Could not create dst render target";

    FillSymmetricRandomCheckerboard(src, src_size, checkerboard_size);

    ASSERT_TRUE(gpu_ops->ResizeBilinear(src, gfx::Rect(src_size), dst,
                                        gfx::Rect(dst_size)));

    AssertSymmetry(dst, dst_size);
  }

  void CreateRandomCheckerboardTexture(
      const gfx::Size& size,
      int checkerboard_size,
      base::win::ScopedComPtr<IDirect3DSurface9>* reference_surface,
      base::win::ScopedComPtr<IDirect3DTexture9>* result) {
    base::win::ScopedComPtr<IDirect3DSurface9> dst;
    ASSERT_TRUE(d3d_utils::CreateOrReuseLockableSurface(device(), size,
        reference_surface));
    ASSERT_TRUE(d3d_utils::CreateOrReuseRenderTargetTexture(device(), size,
        result, dst.Receive()));
    FillRandomCheckerboard(*reference_surface, size, checkerboard_size);
    ASSERT_HRESULT_SUCCEEDED(
        device()->StretchRect(
            *reference_surface, NULL, dst, NULL, D3DTEXF_NONE));
  }

  void AssertSame(int width_in_bytes, int height, uint8* reference,
                  IDirect3DSurface9* lockable) {
    BeforeLockWorkaround();

    D3DLOCKED_RECT locked_rect;
    ASSERT_HRESULT_SUCCEEDED(
        lockable->LockRect(&locked_rect, NULL, D3DLOCK_READONLY));
    uint8* actual = reinterpret_cast<uint8*>(locked_rect.pBits);
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width_in_bytes; ++x) {
        if (!AssertSameColor(reference[y * width_in_bytes + x],
                             actual[y * locked_rect.Pitch + x])) {
          lockable->UnlockRect();
          GTEST_FAIL() << "At pixel (" << x << ", " << y << ")";
        }
      }
    }
    lockable->UnlockRect();
  }

  void DoCopyInvertedTest(AcceleratedSurfaceTransformer* gpu_ops,
                          const gfx::Size& size) {

    SCOPED_TRACE(base::StringPrintf(
        "CopyInverted @ %dx%d", size.width(), size.height()));

    set_color_error_tolerance(0);

    base::win::ScopedComPtr<IDirect3DSurface9> dst, reference_pattern;
    base::win::ScopedComPtr<IDirect3DTexture9> src;

    CreateRandomCheckerboardTexture(size, 1, &reference_pattern, &src);

    // Alloc a slightly larger image 75% of the time, to test that the
    // viewport is set properly.
    const int kAlign = 4;
    gfx::Size alloc_size((size.width() + kAlign - 1) / kAlign * kAlign,
                         (size.height() + kAlign - 1) / kAlign * kAlign);

    ASSERT_TRUE(d3d_utils::CreateOrReuseLockableSurface(device(), alloc_size,
        &dst)) << "Could not create dst render target.";

    ASSERT_TRUE(gpu_ops->CopyInverted(src, dst, size));
    AssertIsInvertedCopy(size, reference_pattern, dst);
  }


  void DoYUVConversionTest(AcceleratedSurfaceTransformer* gpu_ops,
                           const gfx::Size& src_size,
                           int checkerboard_size) {
    // Test the non-MRT implementation, and the MRT implementation as well
    // (if supported by the device).
    ASSERT_NO_FATAL_FAILURE(
        DoYUVConversionTest(gpu_ops, src_size, src_size,
                            checkerboard_size, false));
    if (gpu_ops->device_supports_multiple_render_targets()) {
      ASSERT_NO_FATAL_FAILURE(
          DoYUVConversionTest(gpu_ops, src_size, src_size,
                              checkerboard_size, true));
    }
  }

  void DoYUVConversionScaleTest(AcceleratedSurfaceTransformer* gpu_ops,
                                const gfx::Size& src_size,
                                const gfx::Size& dst_size) {
    // Test the non-MRT implementation, and the MRT implementation as well
    // (if supported by the device).
    if (gpu_ops->device_supports_multiple_render_targets()) {
      ASSERT_NO_FATAL_FAILURE(
          DoYUVConversionTest(gpu_ops, src_size, dst_size, 4, true));
    }
    ASSERT_NO_FATAL_FAILURE(
        DoYUVConversionTest(gpu_ops, src_size, dst_size, 4, false));
  }

  void DoYUVConversionTest(AcceleratedSurfaceTransformer* gpu_ops,
                           const gfx::Size& src_size,
                           const gfx::Size& dst_size,
                           int checkerboard_size,
                           boolean use_multi_render_targets) {
    SCOPED_TRACE(
        base::StringPrintf(
            "YUV Converting %dx%d at checkerboard size of %d; MRT %s",
            src_size.width(), src_size.height(),
            checkerboard_size,
            use_multi_render_targets ? "enabled" : "disabled"));


    base::win::ScopedComPtr<IDirect3DTexture9> src;
    base::win::ScopedComPtr<IDirect3DSurface9> reference;
    base::win::ScopedComPtr<IDirect3DSurface9> dst_y, dst_u, dst_v;

    // TODO(ncarter): Use a better error metric that measures aggregate error
    // rather than simply max error. There seems to be slightly more error at
    // higher resolutions, maybe due to precision issues during rasterization
    // (or maybe more pixels = more test trials). Results are usually to an
    // error of 1, but we must use a tolerance of 3.
    set_color_error_tolerance(3);
    CreateRandomCheckerboardTexture(src_size, checkerboard_size, &reference,
                                    &src);

    gfx::Size packed_y_size, packed_uv_size;

    ASSERT_TRUE(gpu_ops->AllocYUVBuffers(dst_size,
                                         &packed_y_size,
                                         &packed_uv_size,
                                         dst_y.Receive(),
                                         dst_u.Receive(),
                                         dst_v.Receive()));

    // Actually do the conversion.
    if (use_multi_render_targets) {
      ASSERT_TRUE(gpu_ops->TransformRGBToYV12_MRT(src,
                                                  dst_size,
                                                  packed_y_size,
                                                  packed_uv_size,
                                                  dst_y,
                                                  dst_u,
                                                  dst_v));
    } else {
      ASSERT_TRUE(gpu_ops->TransformRGBToYV12_WithoutMRT(src,
                                                         dst_size,
                                                         packed_y_size,
                                                         packed_uv_size,
                                                         dst_y,
                                                         dst_u,
                                                         dst_v));
    }

    // UV size (in bytes/samples) is half, rounded up.
    gfx::Size uv_size((dst_size.width() + 1) / 2,
                      (dst_size.height() + 1) / 2);

    // Generate a reference bitmap by calling a software implementation.
    SkBitmap reference_rgb = ToSkBitmap(reference, false);
    SkBitmap reference_rgb_scaled;
    if (dst_size == src_size) {
      reference_rgb_scaled = reference_rgb;
    } else {
      // We'll call Copy to do the bilinear scaling if needed.
      base::win::ScopedComPtr<IDirect3DSurface9> reference_scaled;
      ASSERT_TRUE(
          d3d_utils::CreateOrReuseLockableSurface(
              device(), dst_size, &reference_scaled));
      ASSERT_TRUE(gpu_ops->Copy(src, reference_scaled, dst_size));
      BeforeLockWorkaround();
      reference_rgb_scaled = ToSkBitmap(reference_scaled, false);
    }

    scoped_ptr<uint8[]> reference_y(new uint8[dst_size.GetArea()]);
    scoped_ptr<uint8[]> reference_u(new uint8[uv_size.GetArea()]);
    scoped_ptr<uint8[]> reference_v(new uint8[uv_size.GetArea()]);
    reference_rgb_scaled.lockPixels();
    media::ConvertRGB32ToYUV_SSE2_Reference(
        reinterpret_cast<uint8*>(reference_rgb_scaled.getAddr32(0, 0)),
        &reference_y[0],
        &reference_u[0],
        &reference_v[0],
        dst_size.width(),
        dst_size.height(),
        reference_rgb_scaled.rowBytes(),
        dst_size.width(),
        uv_size.width());
    reference_rgb_scaled.unlockPixels();

    // Check for equality of the reference and the actual.
    AssertSame(dst_size.width(), dst_size.height(), &reference_y[0], dst_y);
    AssertSame(uv_size.width(), uv_size.height(), &reference_u[0], dst_u);
    AssertSame(uv_size.width(), uv_size.height(), &reference_v[0], dst_v);

    if (kDumpImagesOnFailure && HasFatalFailure()) {
      // Note that this will dump the full u and v buffers, including
      // extra columns added due to packing. That means up to 7 extra
      // columns for uv, and up to 3 extra columns for y.
      WritePNGFile(reference_rgb,
                   base::FilePath(FILE_PATH_LITERAL("test_fail_src.png")));
      WritePNGFile(reference_rgb_scaled,
                   base::FilePath(
                       FILE_PATH_LITERAL("test_fail_src_scaled.png")));
      WritePNGFile(ToSkBitmap(dst_y, true),
                   base::FilePath(FILE_PATH_LITERAL("test_fail_y.png")));
      WritePNGFile(ToSkBitmap(dst_u, true),
                   base::FilePath(FILE_PATH_LITERAL("test_fail_u.png")));
      WritePNGFile(ToSkBitmap(dst_v, true),
                   base::FilePath(FILE_PATH_LITERAL("test_fail_v.png")));
    }
  }

  int color_error_tolerance_;
  uniform_int_distribution<DWORD> random_dword_;
  std::mt19937 rng_;
  base::ScopedNativeLibrary d3d_module_;
  base::win::ScopedComPtr<IDirect3DDevice9Ex> device_;
};

// Fails on some bots because Direct3D isn't allowed.
TEST_P(AcceleratedSurfaceTransformerTest, Init) {
  SCOPED_TRACE(GetAdapterInfo());
  AcceleratedSurfaceTransformer gpu_ops;
  ASSERT_TRUE(gpu_ops.Init(device()));

  WarnOnMissingFeatures(&gpu_ops);
};

// Fails on some bots because Direct3D isn't allowed.
TEST_P(AcceleratedSurfaceTransformerTest, TestConsistentRandom) {
  // This behavior should be the same for every execution on every machine.
  // Otherwise tests might be flaky and impossible to debug.
  SeedRandom("AcceleratedSurfaceTransformerTest.TestConsistentRandom");
  ASSERT_EQ(2922058934, RandomColor());

  SeedRandom("AcceleratedSurfaceTransformerTest.TestConsistentRandom");
  ASSERT_EQ(2922058934, RandomColor());
  ASSERT_EQ(4050239976, RandomColor());

  SeedRandom("DifferentSeed");
  ASSERT_EQ(3904108833, RandomColor());
}

// Fails on some bots because Direct3D isn't allowed.
TEST_P(AcceleratedSurfaceTransformerTest, CopyInverted) {
  // This behavior should be the same for every execution on every machine.
  // Otherwise tests might be flaky and impossible to debug.
  SCOPED_TRACE(GetAdapterInfo());
  SeedRandom("CopyInverted");

  AcceleratedSurfaceTransformer t;
  ASSERT_TRUE(t.Init(device()));

  uniform_int_distribution<int> size(1, 512);

  for (int i = 0; i < 100; ++i) {
    ASSERT_NO_FATAL_FAILURE(
        DoCopyInvertedTest(&t, gfx::Size(size(rng_), size(rng_))))
            << "At iteration " << i;
  }
}

// Fails on some bots because Direct3D isn't allowed.
// Fails on other bots because of ResizeBilinear symmetry failures.
// Should pass, at least, on NVIDIA Quadro 600.
TEST_P(AcceleratedSurfaceTransformerTest, MixedOperations) {
  SCOPED_TRACE(GetAdapterInfo());
  SeedRandom("MixedOperations");

  AcceleratedSurfaceTransformer t;
  ASSERT_TRUE(t.Init(device()));

  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(256, 256), gfx::Size(255, 255), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(256, 256), gfx::Size(255, 255), 2));
  ASSERT_NO_FATAL_FAILURE(
      DoCopyInvertedTest(&t, gfx::Size(20, 107)));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(256, 256), gfx::Size(255, 255), 5));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(256, 256), gfx::Size(64, 64), 5));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&t, gfx::Size(128, 128), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(255, 255), gfx::Size(3, 3), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoCopyInvertedTest(&t, gfx::Size(1412, 124)));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&t, gfx::Size(100, 200), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(255, 255), gfx::Size(257, 257), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(255, 255), gfx::Size(257, 257), 2));

  ASSERT_NO_FATAL_FAILURE(
      DoCopyInvertedTest(&t, gfx::Size(1512, 7)));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(255, 255), gfx::Size(257, 257), 5));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(150, 256), gfx::Size(126, 256), 8));
  ASSERT_NO_FATAL_FAILURE(
      DoCopyInvertedTest(&t, gfx::Size(1521, 3)));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&t, gfx::Size(140, 181), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(150, 256), gfx::Size(126, 256), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoCopyInvertedTest(&t, gfx::Size(33, 712)));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(150, 256), gfx::Size(126, 8), 8));
  ASSERT_NO_FATAL_FAILURE(
      DoCopyInvertedTest(&t, gfx::Size(33, 2)));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&t, gfx::Size(200, 256), gfx::Size(126, 8), 8));
}

// Tests ResizeBilinear with 16K wide/hight src and dst surfaces.
//
// Fails on some bots because Direct3D isn't allowed.
// Should pass, at least, on NVIDIA Quadro 600.
TEST_P(AcceleratedSurfaceTransformerTest, LargeSurfaces) {
  SCOPED_TRACE(GetAdapterInfo());
  SeedRandom("LargeSurfaces");

  AcceleratedSurfaceTransformer gpu_ops;
  ASSERT_TRUE(gpu_ops.Init(device()));

  D3DCAPS9 caps;
  ASSERT_HRESULT_SUCCEEDED(
      device()->GetDeviceCaps(&caps));

  SCOPED_TRACE(base::StringPrintf(
     "max texture size: %dx%d, max texture aspect: %d",
      caps.MaxTextureWidth, caps.MaxTextureHeight, caps.MaxTextureAspectRatio));

  const int w = caps.MaxTextureWidth;
  const int h = caps.MaxTextureHeight;
  const int lo = 256;

  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&gpu_ops, gfx::Size(w, lo), gfx::Size(lo, lo), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&gpu_ops, gfx::Size(lo, h), gfx::Size(lo, lo), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&gpu_ops, gfx::Size(lo, lo), gfx::Size(w, lo), lo));
  ASSERT_NO_FATAL_FAILURE(
      DoResizeBilinearTest(&gpu_ops, gfx::Size(lo, lo), gfx::Size(lo, h), lo));
  ASSERT_NO_FATAL_FAILURE(
      DoCopyInvertedTest(&gpu_ops, gfx::Size(w, lo)));
  ASSERT_NO_FATAL_FAILURE(
      DoCopyInvertedTest(&gpu_ops, gfx::Size(lo, h)));

  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(w, lo), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(lo, h), 1));

}

// Exercises ResizeBilinear with random minification cases where the
// aspect ratio does not change.
//
// Fails on some bots because Direct3D isn't allowed.
// Fails on other bots because of StretchRect symmetry failures.
// Should pass, at least, on NVIDIA Quadro 600.
TEST_P(AcceleratedSurfaceTransformerTest, MinifyUniform) {
  SCOPED_TRACE(GetAdapterInfo());
  SeedRandom("MinifyUniform");

  AcceleratedSurfaceTransformer gpu_ops;
  ASSERT_TRUE(gpu_ops.Init(device()));

  const int dims[] = {21, 63, 64, 65, 99, 127, 128, 129, 192, 255, 256, 257};
  const int checkerboards[] = {1, 2, 3, 9};
  uniform_int_distribution<int> dim(0, arraysize(dims) - 1);
  uniform_int_distribution<int> checkerboard(0, arraysize(checkerboards) - 1);

  for (int i = 0; i < 300; i++) {
    // Widths are picked so that dst is smaller than src.
    int dst_width = dims[dim(rng_)];
    int src_width = dims[dim(rng_)];
    if (src_width < dst_width)
      std::swap(dst_width, src_width);

    // src_height is picked to preserve aspect ratio.
    int dst_height = dims[dim(rng_)];
    int src_height = static_cast<int>(
        static_cast<int64>(src_width) * dst_height / dst_width);

    int checkerboard_size = checkerboards[checkerboard(rng_)];

    ASSERT_NO_FATAL_FAILURE(
        DoResizeBilinearTest(&gpu_ops,
            gfx::Size(src_width, src_height),  // Src size (larger)
            gfx::Size(dst_width, dst_height),  // Dst size (smaller)
            checkerboard_size)) << "Failed on iteration " << i;
  }
};

// Exercises ResizeBilinear with random magnification cases where the
// aspect ratio does not change.
//
// This test relies on an assertion that resizing preserves symmetry in the
// image, but for the current implementation of ResizeBilinear, this does not
// seem to be true (fails on NVIDIA Quadro 600; passes on
// Intel Mobile 965 Express)
TEST_P(AcceleratedSurfaceTransformerTest, DISABLED_MagnifyUniform) {
  SCOPED_TRACE(GetAdapterInfo());
  SeedRandom("MagnifyUniform");

  AcceleratedSurfaceTransformer gpu_ops;
  ASSERT_TRUE(gpu_ops.Init(device()));

  const int dims[] = {63, 64, 65, 99, 127, 128, 129, 192, 255, 256, 257};
  const int checkerboards[] = {1, 2, 3, 9};
  uniform_int_distribution<int> dim(0, arraysize(dims) - 1);
  uniform_int_distribution<int> checkerboard(0, arraysize(checkerboards) - 1);

  for (int i = 0; i < 50; i++) {
    // Widths are picked so that src is smaller than dst.
    int dst_width = dims[dim(rng_)];
    int src_width = dims[dim(rng_)];
    if (dst_width < src_width)
      std::swap(src_width, dst_width);

    int dst_height = dims[dim(rng_)];
    int src_height = static_cast<int>(
        static_cast<int64>(src_width) * dst_height / dst_width);

    int checkerboard_size = checkerboards[checkerboard(rng_)];

    ASSERT_NO_FATAL_FAILURE(
        DoResizeBilinearTest(&gpu_ops,
            gfx::Size(src_width, src_height),  // Src size (smaller)
            gfx::Size(dst_width, dst_height),  // Dst size (larger)
            checkerboard_size)) << "Failed on iteration " << i;
  }
};

TEST_P(AcceleratedSurfaceTransformerTest, RGBtoYUV) {
  SeedRandom("RGBtoYUV");

  AcceleratedSurfaceTransformer gpu_ops;
  ASSERT_TRUE(gpu_ops.Init(device()));

  // Start with some easy-to-debug cases. A checkerboard size of 1 is the
  // best test, but larger checkerboard sizes give more insight into where
  // a bug might be.
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(32, 32), 4));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(32, 32), 2));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(32, 32), 3));

  // All cases of width (mod 8) and height (mod 8), using 1x1 checkerboard.
  for (int w = 32; w < 40; ++w) {
    for (int h = 32; h < 40; ++h) {
      ASSERT_NO_FATAL_FAILURE(
          DoYUVConversionTest(&gpu_ops, gfx::Size(w, h), 1));
    }
  }

  // All the very small sizes which require the most shifting in the
  // texture coordinates when doing alignment.
  for (int w = 1; w <= 9; ++w) {
    for (int h = 1; h <= 9; ++h) {
      ASSERT_NO_FATAL_FAILURE(
          DoYUVConversionTest(&gpu_ops, gfx::Size(w, h), 1));
    }
  }

  // Random medium dimensions.
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(10, 142), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(124, 333), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(853, 225), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(231, 412), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(512, 128), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(1024, 768), 1));

  // Common video/monitor resolutions
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(800, 768), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(1024, 768), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(1280, 720), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(1280, 720), 2));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(1920, 1080), 1));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(1920, 1080), 2));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionTest(&gpu_ops, gfx::Size(2048, 1536), 1));
}

TEST_P(AcceleratedSurfaceTransformerTest, RGBtoYUVScaled) {
  SeedRandom("RGBtoYUVScaled");

  AcceleratedSurfaceTransformer gpu_ops;
  ASSERT_TRUE(gpu_ops.Init(device()));

  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionScaleTest(&gpu_ops, gfx::Size(32, 32), gfx::Size(64, 64)));

  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionScaleTest(&gpu_ops, gfx::Size(32, 32), gfx::Size(16, 16)));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionScaleTest(&gpu_ops, gfx::Size(32, 32), gfx::Size(24, 24)));
  ASSERT_NO_FATAL_FAILURE(
      DoYUVConversionScaleTest(&gpu_ops, gfx::Size(32, 32), gfx::Size(48, 48)));
}

namespace {

// Used to suppress test on Windows versions prior to Vista.
std::vector<int> WindowsVersionIfVistaOrBetter() {
  std::vector<int> result;
  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    result.push_back(base::win::GetVersion());
  }
  return result;
}

}  // namespace

INSTANTIATE_TEST_CASE_P(VistaAndUp,
                        AcceleratedSurfaceTransformerTest,
                        ::testing::ValuesIn(WindowsVersionIfVistaOrBetter()));
