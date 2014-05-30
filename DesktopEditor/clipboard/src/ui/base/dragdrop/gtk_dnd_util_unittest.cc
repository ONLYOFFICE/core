// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/pickle.h"
#include "base/utf_string_conversions.h"
#include "googleurl/src/gurl.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/dragdrop/gtk_dnd_util.h"

namespace ui {

TEST(GtkDndUtilTest, ExtractNamedURLValid) {
  const std::string kTitle = "title";
  const std::string kUrl = "http://www.foobar.com/";
  Pickle pickle;
  pickle.WriteString(kTitle);
  pickle.WriteString(kUrl);

  GtkSelectionData data;
  scoped_ptr<guchar[]> test_data(new guchar[pickle.size()]);
  memcpy(test_data.get(), pickle.data(), pickle.size());
  data.data = test_data.get();
  data.length = pickle.size();

  GURL url;
  string16 title;
  ASSERT_EQ(true, ui::ExtractNamedURL(&data, &url, &title));
  EXPECT_EQ(UTF8ToUTF16(kTitle), title);
  EXPECT_EQ(GURL(kUrl), url);
}

TEST(GtkDndUtilTest, ExtractNamedURLInvalidURL) {
  const std::string kTitle = "title";
  const std::string kBadUrl = "foobar";
  Pickle pickle;
  pickle.WriteString(kTitle);
  pickle.WriteString(kBadUrl);

  GtkSelectionData data;
  scoped_ptr<guchar[]> test_data(new guchar[pickle.size()]);
  memcpy(test_data.get(), pickle.data(), pickle.size());
  data.data = test_data.get();
  data.length = pickle.size();

  GURL url;
  string16 title;
  EXPECT_FALSE(ui::ExtractNamedURL(&data, &url, &title));
}

TEST(GtkDndUtilTest, ExtractNamedURLInvalidInput) {
  GURL url;
  string16 title;
  GtkSelectionData data;
  data.data = NULL;
  data.length = 0;

  EXPECT_FALSE(ui::ExtractNamedURL(&data, &url, &title));

  guchar empty_data[] = "";
  data.data = empty_data;
  data.length = 0;

  EXPECT_FALSE(ui::ExtractNamedURL(&data, &url, &title));

  const std::string kTitle = "title";
  Pickle pickle;
  pickle.WriteString(kTitle);

  scoped_ptr<guchar[]> test_data(new guchar[pickle.size()]);
  memcpy(test_data.get(), pickle.data(), pickle.size());
  data.data = test_data.get();
  data.length = pickle.size();

  EXPECT_FALSE(ui::ExtractNamedURL(&data, &url, &title));
}

}  // namespace ui
