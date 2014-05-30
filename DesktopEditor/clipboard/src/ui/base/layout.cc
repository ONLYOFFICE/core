// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/layout.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "build/build_config.h"
#include "ui/base/touch/touch_device.h"
#include "ui/base/ui_base_switches.h"
#include "ui/gfx/display.h"
#include "ui/gfx/screen.h"

#if defined(OS_MACOSX) && !defined(OS_IOS)
#include "base/mac/mac_util.h"
#endif

#if defined(OS_WIN)
#include "base/win/metro.h"
#include "ui/base/win/dpi.h"
#include <Windows.h>
#endif  // defined(OS_WIN)

#if defined(OS_CHROMEOS)
#include "ui/base/resource/resource_bundle.h"
#endif

namespace ui {

namespace {

bool ScaleFactorComparator(const ScaleFactor& lhs, const ScaleFactor& rhs){
  return GetScaleFactorScale(lhs) < GetScaleFactorScale(rhs);
}

#if defined(OS_WIN)
// Helper function that determines whether we want to optimize the UI for touch.
bool UseTouchOptimizedUI() {
  // If --touch-optimized-ui is specified and not set to "auto", then override
  // the hardware-determined setting (eg. for testing purposes).
  static bool has_touch_optimized_ui = CommandLine::ForCurrentProcess()->
      HasSwitch(switches::kTouchOptimizedUI);
  if (has_touch_optimized_ui) {
    const std::string switch_value = CommandLine::ForCurrentProcess()->
        GetSwitchValueASCII(switches::kTouchOptimizedUI);

    // Note that simply specifying the switch is the same as enabled.
    if (switch_value.empty() ||
        switch_value == switches::kTouchOptimizedUIEnabled) {
      return true;
    } else if (switch_value == switches::kTouchOptimizedUIDisabled) {
      return false;
    } else if (switch_value != switches::kTouchOptimizedUIAuto) {
      LOG(ERROR) << "Invalid --touch-optimized-ui option: " << switch_value;
    }
  }

  // We use the touch layout only when we are running in Metro mode.
  return base::win::IsMetroProcess() && ui::IsTouchDevicePresent();
}
#endif  // defined(OS_WIN)

const float kScaleFactorScales[] = {1.0f, 1.0f, 1.33f, 1.4f, 1.5f, 1.8f, 2.0f};
COMPILE_ASSERT(NUM_SCALE_FACTORS == arraysize(kScaleFactorScales),
               kScaleFactorScales_incorrect_size);
const size_t kScaleFactorScalesLength = arraysize(kScaleFactorScales);

namespace {

// Returns the scale factor closest to |scale| from the full list of factors.
// Note that it does NOT rely on the list of supported scale factors.
// Finding the closest match is inefficient and shouldn't be done frequently.
ScaleFactor FindClosestScaleFactorUnsafe(float scale) {
  float smallest_diff =  std::numeric_limits<float>::max();
  ScaleFactor closest_match = SCALE_FACTOR_100P;
  for (int i = SCALE_FACTOR_100P; i < NUM_SCALE_FACTORS; ++i) {
    const ScaleFactor scale_factor = static_cast<ScaleFactor>(i);
    float diff = std::abs(kScaleFactorScales[scale_factor] - scale);
    if (diff < smallest_diff) {
      closest_match = scale_factor;
      smallest_diff = diff;
    }
  }
  return closest_match;
}

}  // namespace

std::vector<ScaleFactor>& GetSupportedScaleFactorsInternal() {
  static std::vector<ScaleFactor>* supported_scale_factors =
      new std::vector<ScaleFactor>();
  if (supported_scale_factors->empty()) {
#if !defined(OS_IOS)
    // On platforms other than iOS, 100P is always a supported scale factor.
    supported_scale_factors->push_back(SCALE_FACTOR_100P);
#endif

#if defined(OS_ANDROID)
    const gfx::Display display =
        gfx::Screen::GetNativeScreen()->GetPrimaryDisplay();
    const float display_density = display.device_scale_factor();
    const ScaleFactor closest = FindClosestScaleFactorUnsafe(display_density);
    if (closest != SCALE_FACTOR_100P)
      supported_scale_factors->push_back(closest);
#elif defined(OS_IOS)
    gfx::Display display = gfx::Screen::GetNativeScreen()->GetPrimaryDisplay();
    if (display.device_scale_factor() > 1.0) {
      DCHECK_EQ(2.0, display.device_scale_factor());
      supported_scale_factors->push_back(SCALE_FACTOR_200P);
    } else {
      supported_scale_factors->push_back(SCALE_FACTOR_100P);
    }
#elif defined(OS_MACOSX)
    if (base::mac::IsOSLionOrLater())
      supported_scale_factors->push_back(SCALE_FACTOR_200P);
#elif defined(OS_WIN)
    // Have high-DPI resources for 140% and 180% scaling on Windows based on
    // default scaling for Metro mode.  Round to nearest supported scale in
    // all cases.
    if (ui::IsInHighDPIMode()) {
      supported_scale_factors->push_back(SCALE_FACTOR_140P);
      supported_scale_factors->push_back(SCALE_FACTOR_180P);
    }
#elif defined(OS_CHROMEOS)
    // TODO(oshima): Include 200P only if the device support 200P
    supported_scale_factors->push_back(SCALE_FACTOR_200P);
#endif
    std::sort(supported_scale_factors->begin(),
              supported_scale_factors->end(),
              ScaleFactorComparator);
  }
  return *supported_scale_factors;
}

}  // namespace

DisplayLayout GetDisplayLayout() {
#if defined(OS_WIN)
  if (UseTouchOptimizedUI())
    return LAYOUT_TOUCH;
#endif
  return LAYOUT_DESKTOP;
}

ScaleFactor GetScaleFactorFromScale(float scale) {
  ScaleFactor closest_match = SCALE_FACTOR_100P;
  float smallest_diff =  std::numeric_limits<float>::max();
  const std::vector<ScaleFactor>& supported =
      GetSupportedScaleFactorsInternal();
  for (size_t i = 0; i < supported.size(); ++i) {
    ScaleFactor scale_factor = supported[i];
    float diff = std::abs(kScaleFactorScales[scale_factor] - scale);
    if (diff < smallest_diff) {
      closest_match = scale_factor;
      smallest_diff = diff;
    }
  }
  DCHECK_NE(closest_match, SCALE_FACTOR_NONE);
  return closest_match;
}

float GetScaleFactorScale(ScaleFactor scale_factor) {
  return kScaleFactorScales[scale_factor];
}

ScaleFactor GetMaxScaleFactor() {
#if defined(OS_CHROMEOS)
  return ResourceBundle::GetSharedInstance().max_scale_factor();
#else
  return GetSupportedScaleFactorsInternal().back();
#endif
}

std::vector<ScaleFactor> GetSupportedScaleFactors() {
  return GetSupportedScaleFactorsInternal();
}

bool IsScaleFactorSupported(ScaleFactor scale_factor) {
  const std::vector<ScaleFactor>& supported =
      GetSupportedScaleFactorsInternal();
  return std::find(supported.begin(), supported.end(), scale_factor) !=
      supported.end();
}

namespace test {

void SetSupportedScaleFactors(
    const std::vector<ui::ScaleFactor>& scale_factors) {
  std::vector<ui::ScaleFactor>& supported_scale_factors =
      GetSupportedScaleFactorsInternal();
  supported_scale_factors = scale_factors;
  std::sort(supported_scale_factors.begin(),
            supported_scale_factors.end(),
            ScaleFactorComparator);
}

ScopedSetSupportedScaleFactors::ScopedSetSupportedScaleFactors(
    const std::vector<ui::ScaleFactor>& new_scale_factors)
    : original_scale_factors_(GetSupportedScaleFactors()) {
  SetSupportedScaleFactors(new_scale_factors);
}

ScopedSetSupportedScaleFactors::~ScopedSetSupportedScaleFactors() {
  SetSupportedScaleFactors(original_scale_factors_);
}

}  // namespace test

#if !defined(OS_MACOSX)
ScaleFactor GetScaleFactorForNativeView(gfx::NativeView view) {
  gfx::Screen* screen = gfx::Screen::GetScreenFor(view);
  if (screen->IsDIPEnabled()) {
    gfx::Display display = screen->GetDisplayNearestWindow(view);
    return GetScaleFactorFromScale(display.device_scale_factor());
  }
  return ui::SCALE_FACTOR_100P;
}
#endif  // !defined(OS_MACOSX)

}  // namespace ui
