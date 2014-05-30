// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_VIEWS_NOTIFIER_SETTINGS_VIEW_H_
#define UI_MESSAGE_CENTER_VIEWS_NOTIFIER_SETTINGS_VIEW_H_

#include <set>
#include <string>

#include "base/string16.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/message_center/message_center_export.h"
#include "ui/message_center/notifier_settings.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/widget/widget_delegate.h"

namespace views {
class ScrollView;
}

namespace message_center {

// A class to show the list of notifier extensions / URL patterns and allow
// users to customize the settings.
class MESSAGE_CENTER_EXPORT NotifierSettingsView
    : public NotifierSettingsDelegate,
      public views::WidgetDelegateView,
      public views::ButtonListener {
 public:
  // Create a new widget of the notifier settings and returns it. Note that
  // the widget and the view is self-owned. It'll be deleted when it's closed
  // or the chrome's shutdown.
  static NotifierSettingsView* Create(NotifierSettingsProvider* delegate,
                                      gfx::NativeView context);

  // Overridden from NotifierSettingsDelegate:
  virtual void UpdateIconImage(const std::string& id,
                               const gfx::Image& icon) OVERRIDE;
  virtual void UpdateFavicon(const GURL& url, const gfx::Image& icon) OVERRIDE;

  void set_delegate(NotifierSettingsProvider* new_delegate) {
    delegate_ = new_delegate;
  }

 private:
  class NotifierButton;

  NotifierSettingsView(NotifierSettingsProvider* delegate);
  virtual ~NotifierSettingsView();

  // Overridden from views::WidgetDelegate:
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;
  virtual void WindowClosing() OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;
  virtual bool CanResize() const OVERRIDE;

  // Overridden from views::View:
  virtual void Layout() OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnMouseWheel(const ui::MouseWheelEvent& event) OVERRIDE;

  // Overridden from views::ButtonListener:
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  views::View* title_entry_;
  views::ScrollView* scroller_;
  NotifierSettingsProvider* delegate_;
  std::set<NotifierButton*> buttons_;

  DISALLOW_COPY_AND_ASSIGN(NotifierSettingsView);
};

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_VIEWS_NOTIFIER_SETTINGS_VIEW_H_
