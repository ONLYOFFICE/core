// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_TEST_TEST_VIEWS_DELEGATE_H_
#define UI_VIEWS_TEST_TEST_VIEWS_DELEGATE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/views/views_delegate.h"

namespace ui {
class Clipboard;
}

namespace views {
class View;
class Widget;

class TestViewsDelegate : public ViewsDelegate {
 public:
  TestViewsDelegate();
  virtual ~TestViewsDelegate();

  void SetUseTransparentWindows(bool transparent);

  // Overridden from ViewsDelegate:
  virtual void SaveWindowPlacement(const Widget* window,
                                   const std::string& window_name,
                                   const gfx::Rect& bounds,
                                   ui::WindowShowState show_state) OVERRIDE;
  virtual bool GetSavedWindowPlacement(
      const std::string& window_name,
      gfx::Rect* bounds,
      ui::WindowShowState* show_state) const OVERRIDE;

  virtual void NotifyAccessibilityEvent(
      View* view, ui::AccessibilityTypes::Event event_type) OVERRIDE {}

  virtual void NotifyMenuItemFocused(const string16& menu_name,
                                     const string16& menu_item_name,
                                     int item_index,
                                     int item_count,
                                     bool has_submenu) OVERRIDE {}
#if defined(OS_WIN)
  virtual HICON GetDefaultWindowIcon() const OVERRIDE {
    return NULL;
  }
#endif
  virtual NonClientFrameView* CreateDefaultNonClientFrameView(
      Widget* widget) OVERRIDE;
  virtual bool UseTransparentWindows() const OVERRIDE;
  virtual void AddRef() OVERRIDE {}
  virtual void ReleaseRef() OVERRIDE {}
  virtual content::WebContents* CreateWebContents(
      content::BrowserContext* browser_context,
      content::SiteInstance* site_instance) OVERRIDE;
  virtual void OnBeforeWidgetInit(
      Widget::InitParams* params,
      internal::NativeWidgetDelegate* delegate) OVERRIDE;

 private:
  bool use_transparent_windows_;

  DISALLOW_COPY_AND_ASSIGN(TestViewsDelegate);
};

}  // namespace views

#endif  // UI_VIEWS_TEST_TEST_VIEWS_DELEGATE_H_
