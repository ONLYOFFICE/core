// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_BOUNDED_LABEL_H_
#define UI_MESSAGE_CENTER_BOUNDED_LABEL_H_

#include <list>
#include <map>

#include "base/memory/scoped_ptr.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/message_center/message_center_export.h"
#include "ui/views/view.h"

namespace gfx {
class Font;
}

namespace message_center {

class InnerBoundedLabel;

namespace test {
class BoundedLabelTest;
}

// BoundedLabels display left aligned text up to a maximum number of lines, with
// ellipsis at the end of the last line for any omitted text. BoundedLabel is a
// direct subclass of views::Views rather than a subclass of views::Label
// to avoid exposing some of views::Label's methods that can't be made to work
// with BoundedLabel. See the description of InnerBoundedLabel in the
// bounded_label.cc file for details.
class MESSAGE_CENTER_EXPORT BoundedLabel : public views::View {
 public:
  BoundedLabel(const string16& text, gfx::Font font);
  BoundedLabel(const string16& text);
  virtual ~BoundedLabel();

  void SetColors(SkColor textColor, SkColor backgroundColor);
  void SetLineHeight(int height);  // Pass in 0 for default height.
  void SetLineLimit(int lines);  // Pass in -1 for no limit.

  int GetLineHeight() const;
  int GetLineLimit() const;

  // Pass in a -1 width to use the preferred width, a -1 limit to skip limits.
  int GetLinesForWidthAndLimit(int width, int limit);
  gfx::Size GetSizeForWidthAndLines(int width, int lines);

  // Overridden from views::View.
  virtual int GetBaseline() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual int GetHeightForWidth(int width) OVERRIDE;
  virtual void Paint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 protected:
  // Overridden from views::View.
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) OVERRIDE;

 private:
  friend class test::BoundedLabelTest;

  string16 GetWrappedTextForTest(int width, int lines);

  scoped_ptr<InnerBoundedLabel> label_;
  int line_limit_;

  DISALLOW_COPY_AND_ASSIGN(BoundedLabel);
};

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_BOUNDED_LABEL_H_
