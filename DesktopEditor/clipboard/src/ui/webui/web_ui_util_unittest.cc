// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/webui/web_ui_util.h"

#include "googleurl/src/gurl.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(WebUIUtilTest, ParsePathAndScale) {
  std::vector<ui::ScaleFactor> supported_scale_factors;
  supported_scale_factors.push_back(ui::SCALE_FACTOR_100P);
  supported_scale_factors.push_back(ui::SCALE_FACTOR_140P);
  supported_scale_factors.push_back(ui::SCALE_FACTOR_200P);
  ui::test::ScopedSetSupportedScaleFactors scoped_supported(
      supported_scale_factors);

  std::string path;
  ui::ScaleFactor factor;

  GURL url("http://some/random/username@email/and/more");
  webui::ParsePathAndScale(url, &path, &factor);
  EXPECT_EQ("random/username@email/and/more", path);
  EXPECT_EQ(ui::SCALE_FACTOR_100P, factor);

  GURL url2("http://some/random/username/and/more");
  webui::ParsePathAndScale(url2, &path, &factor);
  EXPECT_EQ("random/username/and/more", path);
  EXPECT_EQ(ui::SCALE_FACTOR_100P, factor);

  GURL url3("http://some/random/username/and/more@2ax");
  webui::ParsePathAndScale(url3, &path, &factor);
  EXPECT_EQ("random/username/and/more@2ax", path);
  EXPECT_EQ(ui::SCALE_FACTOR_100P, factor);

  GURL url4("http://some/random/username/and/more@x");
  webui::ParsePathAndScale(url4, &path, &factor);
  EXPECT_EQ("random/username/and/more@x", path);
  EXPECT_EQ(ui::SCALE_FACTOR_100P, factor);

  GURL url5("http://some/random/username@email/and/more@2x");
  webui::ParsePathAndScale(url5, &path, &factor);
  EXPECT_EQ("random/username@email/and/more", path);
  EXPECT_EQ(ui::SCALE_FACTOR_200P, factor);

  GURL url6("http://some/random/username/and/more@1.4x");
  webui::ParsePathAndScale(url6, &path, &factor);
  EXPECT_EQ("random/username/and/more", path);
  EXPECT_EQ(ui::SCALE_FACTOR_140P, factor);

  GURL url7("http://some/random/username/and/more@1.3x");
  webui::ParsePathAndScale(url7, &path, &factor);
  EXPECT_EQ("random/username/and/more", path);
  EXPECT_EQ(ui::SCALE_FACTOR_140P, factor);
}
