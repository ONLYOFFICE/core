// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WINDOW_DIALOG_DELEGATE_H_
#define UI_VIEWS_WINDOW_DIALOG_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/string16.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/ui_base_types.h"
#include "ui/views/widget/widget_delegate.h"

namespace views {

class DialogClientView;

///////////////////////////////////////////////////////////////////////////////
//
// DialogDelegate
//
//  DialogDelegate is an interface implemented by objects that wish to show a
//  dialog box Window. The window that is displayed uses this interface to
//  determine how it should be displayed and notify the delegate object of
//  certain events.
//
///////////////////////////////////////////////////////////////////////////////
class VIEWS_EXPORT DialogDelegate : public WidgetDelegate {
 public:
  virtual ~DialogDelegate();

  // Returns whether to use the new dialog style.
  static bool UseNewStyle();

  // Returns a mask specifying which of the available DialogButtons are visible
  // for the dialog. Note: Dialogs with just an OK button are frowned upon.
  virtual int GetDialogButtons() const;

  // Returns the default dialog button. This should not be a mask as only
  // one button should ever be the default button.  Return
  // ui::DIALOG_BUTTON_NONE if there is no default.  Default
  // behavior is to return ui::DIALOG_BUTTON_OK or
  // ui::DIALOG_BUTTON_CANCEL (in that order) if they are
  // present, ui::DIALOG_BUTTON_NONE otherwise.
  virtual int GetDefaultDialogButton() const;

  // Returns the label of the specified dialog button.
  virtual string16 GetDialogButtonLabel(ui::DialogButton button) const;

  // Returns whether the specified dialog button is enabled.
  virtual bool IsDialogButtonEnabled(ui::DialogButton button) const;

  // Override this function to display an extra view adjacent to the buttons.
  // Overrides may construct the view; this will only be called once per dialog.
  virtual View* CreateExtraView();

  // Override this function to display an extra view in the titlebar.
  // Overrides may construct the view; this will only be called once per dialog.
  // Note: this only works for new style dialogs.
  virtual View* CreateTitlebarExtraView();

  // Override this function to display a footnote view below the buttons.
  // Overrides may construct the view; this will only be called once per dialog.
  virtual View* CreateFootnoteView();

  // For Dialog boxes, if there is a "Cancel" button or no dialog button at all,
  // this is called when the user presses the "Cancel" button or the Close
  // button on the window or in the system menu, or presses the Esc key.
  // This function should return true if the window can be closed after it
  // returns, or false if it must remain open.
  virtual bool Cancel();

  // For Dialog boxes, this is called when the user presses the "OK" button,
  // or the Enter key.  Can also be called on Esc key or close button
  // presses if there is no "Cancel" button.  This function should return
  // true if the window can be closed after it returns, or false if it must
  // remain open.  If |window_closing| is true, it means that this handler is
  // being called because the window is being closed (e.g.  by Window::Close)
  // and there is no Cancel handler, so Accept is being called instead.
  virtual bool Accept(bool window_closing);
  virtual bool Accept();

  // Overridden from WidgetDelegate:
  virtual View* GetInitiallyFocusedView() OVERRIDE;
  virtual DialogDelegate* AsDialogDelegate() OVERRIDE;
  virtual ClientView* CreateClientView(Widget* widget) OVERRIDE;
  virtual NonClientFrameView* CreateNonClientFrameView(Widget* widget) OVERRIDE;

  // Create a frame view using the new dialog style.
  static NonClientFrameView* CreateNewStyleFrameView(Widget* widget);

  // Called when the window has been closed.
  virtual void OnClose() {}

  // A helper for accessing the DialogClientView object contained by this
  // delegate's Window.
  const DialogClientView* GetDialogClientView() const;
  DialogClientView* GetDialogClientView();

 protected:
  // Overridden from WidgetDelegate:
  virtual ui::AccessibilityTypes::Role GetAccessibleWindowRole() const OVERRIDE;
};

// A DialogDelegate implementation that is-a View. Used to override GetWidget()
// to call View's GetWidget() for the common case where a DialogDelegate
// implementation is-a View. Note that DialogDelegateView is not owned by
// view's hierarchy and is expected to be deleted on DeleteDelegate call.
class VIEWS_EXPORT DialogDelegateView : public DialogDelegate,
                                        public View {
 public:
  DialogDelegateView();
  virtual ~DialogDelegateView();

  // Create a |dialog| window Widget with the specified |context| or |parent|.
  static Widget* CreateDialogWidget(DialogDelegateView* dialog,
                                    gfx::NativeWindow context,
                                    gfx::NativeWindow parent);

  // Overridden from DialogDelegate:
  virtual void DeleteDelegate() OVERRIDE;
  virtual Widget* GetWidget() OVERRIDE;
  virtual const Widget* GetWidget() const OVERRIDE;
  virtual View* GetContentsView() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DialogDelegateView);
};

}  // namespace views

#endif  // UI_VIEWS_WINDOW_DIALOG_DELEGATE_H_
