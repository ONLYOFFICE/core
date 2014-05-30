// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TEST_TEST_VIEWS_H_
#define UI_VIEWS_TEST_TEST_VIEWS_H_

#include "ui/views/view.h"

namespace views {

// A view that requests a set amount of space.
class StaticSizedView : public View {
 public:
  explicit StaticSizedView(const gfx::Size& size);
  virtual ~StaticSizedView();

  virtual gfx::Size GetPreferredSize() OVERRIDE;

 private:
  gfx::Size size_;

  DISALLOW_COPY_AND_ASSIGN(StaticSizedView);
};

// A view that accomodates testing layouts that use GetHeightForWidth.
class ProportionallySizedView : public View {
 public:
  explicit ProportionallySizedView(int factor);
  virtual ~ProportionallySizedView();

  virtual int GetHeightForWidth(int w) OVERRIDE;

 private:
  // The multiplicative factor between width and height, i.e.
  // height = width * factor_.
  int factor_;

  DISALLOW_COPY_AND_ASSIGN(ProportionallySizedView);
};

}  // namespace views

#endif  // UI_VIEWS_TEST_TEST_VIEWS_H_
