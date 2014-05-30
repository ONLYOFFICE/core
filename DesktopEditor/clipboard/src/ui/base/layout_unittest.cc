// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/layout.h"

#include "testing/gtest/include/gtest/gtest.h"

#if defined(OS_CHROMEOS)
#include "base/chromeos/chromeos_version.h"
#include "base/command_line.h"
#include "ui/base/ui_base_switches.h"
#endif

namespace ui {

TEST(LayoutTest, GetScaleFactorScale) {
  EXPECT_FLOAT_EQ(1.0f, GetScaleFactorScale(SCALE_FACTOR_100P));
  EXPECT_FLOAT_EQ(1.33f, GetScaleFactorScale(SCALE_FACTOR_133P));
  EXPECT_FLOAT_EQ(1.4f, GetScaleFactorScale(SCALE_FACTOR_140P));
  EXPECT_FLOAT_EQ(1.5f, GetScaleFactorScale(SCALE_FACTOR_150P));
  EXPECT_FLOAT_EQ(1.8f, GetScaleFactorScale(SCALE_FACTOR_180P));
  EXPECT_FLOAT_EQ(2.0f, GetScaleFactorScale(SCALE_FACTOR_200P));
}

TEST(LayoutTest, GetScaleFactorFromScalePartlySupported) {
  std::vector<ScaleFactor> supported_factors;
  supported_factors.push_back(SCALE_FACTOR_100P);
  supported_factors.push_back(SCALE_FACTOR_180P);
  test::ScopedSetSupportedScaleFactors scoped_supported(supported_factors);
  EXPECT_EQ(SCALE_FACTOR_100P, GetScaleFactorFromScale(0.1f));
  EXPECT_EQ(SCALE_FACTOR_100P, GetScaleFactorFromScale(0.9f));
  EXPECT_EQ(SCALE_FACTOR_100P, GetScaleFactorFromScale(1.0f));
  EXPECT_EQ(SCALE_FACTOR_100P, GetScaleFactorFromScale(1.39f));
  EXPECT_EQ(SCALE_FACTOR_180P, GetScaleFactorFromScale(1.41f));
  EXPECT_EQ(SCALE_FACTOR_180P, GetScaleFactorFromScale(1.8f));
  EXPECT_EQ(SCALE_FACTOR_180P, GetScaleFactorFromScale(2.0f));
  EXPECT_EQ(SCALE_FACTOR_180P, GetScaleFactorFromScale(999.0f));
}

TEST(LayoutTest, GetScaleFactorFromScaleAllSupported) {
  std::vector<ScaleFactor> supported_factors;
  for (int factor = SCALE_FACTOR_100P; factor < NUM_SCALE_FACTORS; ++factor) {
    supported_factors.push_back(static_cast<ScaleFactor>(factor));
  }
  test::ScopedSetSupportedScaleFactors scoped_supported(supported_factors);

  EXPECT_EQ(SCALE_FACTOR_100P, GetScaleFactorFromScale(0.1f));
  EXPECT_EQ(SCALE_FACTOR_100P, GetScaleFactorFromScale(0.9f));
  EXPECT_EQ(SCALE_FACTOR_100P, GetScaleFactorFromScale(1.0f));
  EXPECT_EQ(SCALE_FACTOR_133P, GetScaleFactorFromScale(1.19f));
  EXPECT_EQ(SCALE_FACTOR_133P, GetScaleFactorFromScale(1.21f));
  EXPECT_EQ(SCALE_FACTOR_133P, GetScaleFactorFromScale(1.3f));
  EXPECT_EQ(SCALE_FACTOR_140P, GetScaleFactorFromScale(1.4f));
  EXPECT_EQ(SCALE_FACTOR_150P, GetScaleFactorFromScale(1.59f));
  EXPECT_EQ(SCALE_FACTOR_150P, GetScaleFactorFromScale(1.61f));
  EXPECT_EQ(SCALE_FACTOR_180P, GetScaleFactorFromScale(1.7f));
  EXPECT_EQ(SCALE_FACTOR_180P, GetScaleFactorFromScale(1.89f));
  EXPECT_EQ(SCALE_FACTOR_200P, GetScaleFactorFromScale(1.91f));
  EXPECT_EQ(SCALE_FACTOR_200P, GetScaleFactorFromScale(2.0f));
  EXPECT_EQ(SCALE_FACTOR_200P, GetScaleFactorFromScale(2.1f));
  EXPECT_EQ(SCALE_FACTOR_200P, GetScaleFactorFromScale(999.0f));
}

TEST(LayoutTest, GetMaxScaleFactor) {
#if defined(OS_CHROMEOS)
  // On Chrome OS, the maximum scale factor is based on
  // the available resource pack. In testing environment,
  // we always have 200P.
  EXPECT_EQ(SCALE_FACTOR_200P, GetMaxScaleFactor());
#else
  {
    ScaleFactor scale_factors[] = { SCALE_FACTOR_100P };
    std::vector<ScaleFactor> supported_factors(
        scale_factors, scale_factors + arraysize(scale_factors));
    test::ScopedSetSupportedScaleFactors scoped_supported(supported_factors);
    EXPECT_EQ(SCALE_FACTOR_100P, GetMaxScaleFactor());
  }

  {
    ScaleFactor scale_factors[] = { SCALE_FACTOR_100P,
                                    SCALE_FACTOR_140P };
    std::vector<ScaleFactor> supported_factors(
        scale_factors, scale_factors + arraysize(scale_factors));
    test::ScopedSetSupportedScaleFactors scoped_supported(supported_factors);
    EXPECT_EQ(SCALE_FACTOR_140P, GetMaxScaleFactor());
  }

  {
    ScaleFactor scale_factors[] = { SCALE_FACTOR_200P,
                                    SCALE_FACTOR_180P,
                                    SCALE_FACTOR_140P,
                                    SCALE_FACTOR_100P };
    std::vector<ScaleFactor> supported_factors(
        scale_factors, scale_factors + arraysize(scale_factors));
    test::ScopedSetSupportedScaleFactors scoped_supported(supported_factors);
    EXPECT_EQ(SCALE_FACTOR_200P, GetMaxScaleFactor());
  }
#endif
}

}  // namespace ui
