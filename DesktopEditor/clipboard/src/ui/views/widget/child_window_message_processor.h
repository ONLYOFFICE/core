// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_CHILD_WINDOW_MESSAGE_PROCESSOR_H_
#define UI_VIEWS_WIDGET_CHILD_WINDOW_MESSAGE_PROCESSOR_H_

#include <windows.h>

namespace ui {
class ViewProp;
}

namespace views {

// Windows sends a handful of messages to the parent window rather than the
// window itself. For example, selection changes of a rich edit (EN_SELCHANGE)
// are sent to the parent, not the window. Typically such message are best
// dealt with by the window rather than the parent. NativeWidgetWin allows for
// registering a ChildWindowMessageProcessor to handle such messages.
class ChildWindowMessageProcessor {
 public:
  // Registers |processor| for |hwnd|. The caller takes ownership of the
  // returned object.
  static ui::ViewProp* Register(HWND hwnd,
                                 ChildWindowMessageProcessor* processor);

  // Returns the ChildWindowMessageProcessor for |hwnd|, NULL if there isn't
  // one.
  static ChildWindowMessageProcessor* Get(HWND hwnd);

  // Invoked for any messages that are sent to the parent and originated from
  // the HWND this ChildWindowMessageProcessor was registered for. Returns true
  // if the message was handled with a valid result in |result|. Returns false
  // if the message was not handled.
  virtual bool ProcessMessage(UINT message,
                              WPARAM w_param,
                              LPARAM l_param,
                              LRESULT* result) = 0;

 protected:
  virtual ~ChildWindowMessageProcessor() {}
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_CHILD_WINDOW_MESSAGE_PROCESSOR_H_
