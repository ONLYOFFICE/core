// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/combobox/native_combobox_win.h"

#include "base/i18n/rtl.h"
#include "base/utf_string_conversions.h"
#include "ui/base/events/event.h"
#include "ui/base/models/combobox_model.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/win/hwnd_util.h"
#include "ui/gfx/font.h"
#include "ui/native_theme/native_theme_win.h"
#include "ui/views/controls/combobox/combobox.h"
#include "ui/views/controls/combobox/native_combobox_views.h"
#include "ui/views/widget/widget.h"

namespace {

// Limit how small a combobox can be.
const int kMinComboboxWidth = 148;

// Add a couple extra pixels to the widths of comboboxes and combobox
// dropdowns so that text isn't too crowded.
const int kComboboxExtraPaddingX = 6;

}  // namespace

namespace views {

////////////////////////////////////////////////////////////////////////////////
// NativeComboboxWin, public:

NativeComboboxWin::NativeComboboxWin(Combobox* combobox)
    : combobox_(combobox),
      content_width_(0) {
  // Associates the actual HWND with the combobox so it is the one considered as
  // having the focus (not the wrapper) when the HWND is focused directly (with
  // a click for example).
  set_focus_view(combobox);
}

NativeComboboxWin::~NativeComboboxWin() {
}

////////////////////////////////////////////////////////////////////////////////
// NativeComboboxWin, NativeComboboxWrapper implementation:

void NativeComboboxWin::UpdateFromModel() {
  SendMessage(native_view(), CB_RESETCONTENT, 0, 0);
  const gfx::Font& font = Combobox::GetFont();
  int max_width = 0;
  int num_items = combobox_->model()->GetItemCount();
  for (int i = 0; i < num_items; ++i) {
    string16 text = combobox_->model()->GetItemAt(i);

    // Inserting the Unicode formatting characters if necessary so that the
    // text is displayed correctly in right-to-left UIs.
    base::i18n::AdjustStringForLocaleDirection(&text);

    SendMessage(native_view(), CB_ADDSTRING, 0,
                reinterpret_cast<LPARAM>(UTF16ToWide(text).c_str()));
    max_width = std::max(max_width, font.GetStringWidth(text));
  }
  content_width_ = max_width;

  if (num_items > 0) {
    SendMessage(native_view(), CB_SETCURSEL, combobox_->selected_index(), 0);

    // Set the width for the drop down while accounting for the scrollbar and
    // borders.
    if (num_items > ComboBox_GetMinVisible(native_view()))
      max_width += GetSystemMetrics(SM_CXVSCROLL);
    // SM_CXEDGE would not be correct here, since the dropdown is flat, not 3D.
    int kComboboxDropdownBorderSize = 1;
    max_width += 2 * kComboboxDropdownBorderSize + kComboboxExtraPaddingX;
    SendMessage(native_view(), CB_SETDROPPEDWIDTH, max_width, 0);
  }
}

void NativeComboboxWin::UpdateSelectedIndex() {
  // Note that we use CB_SETCURSEL and not CB_SELECTSTRING because on RTL
  // locales the strings we get from our ComboBox::Model might be augmented
  // with Unicode directionality marks before we insert them into the combobox
  // and therefore we can not assume that the string we get from
  // ui::ComboboxModel can be safely searched for and selected (which is what
  // CB_SELECTSTRING does).
  SendMessage(native_view(), CB_SETCURSEL, combobox_->selected_index(), 0);
}

void NativeComboboxWin::UpdateEnabled() {
  SetEnabled(combobox_->enabled());
}

int NativeComboboxWin::GetSelectedIndex() const {
  LRESULT selected_index = SendMessage(native_view(), CB_GETCURSEL, 0, 0);
  return selected_index != CB_ERR ? selected_index : -1;
}

bool NativeComboboxWin::IsDropdownOpen() const {
  return SendMessage(native_view(), CB_GETDROPPEDSTATE, 0, 0) != 0;
}

gfx::Size NativeComboboxWin::GetPreferredSize() {
  COMBOBOXINFO cbi = { 0 };
  cbi.cbSize = sizeof(cbi);
  // Note: Don't use CB_GETCOMBOBOXINFO since that crashes on WOW64 systems
  // when you have a global message hook installed.
  GetComboBoxInfo(native_view(), &cbi);
  gfx::Rect rect_item(cbi.rcItem);
  gfx::Rect rect_button(cbi.rcButton);
  gfx::Size border = ui::NativeThemeWin::instance()->GetThemeBorderSize(
      ui::NativeThemeWin::MENULIST);

  // The padding value of '3' is the xy offset from the corner of the control
  // to the corner of rcItem.  It does not seem to be queryable from the theme.
  // It is consistent on all versions of Windows from 2K to Vista, and is
  // invariant with respect to the combobox border size.  We could conceivably
  // get this number from rect_item.x, but it seems fragile to depend on
  // position here, inside of the layout code.
  const int kItemOffset = 3;
  int item_to_button_distance = std::max(kItemOffset - border.width(), 0);

  // The cx computation can be read as measuring from left to right.
  int pref_width = std::max(kItemOffset + content_width_ +
                                kComboboxExtraPaddingX +
                                item_to_button_distance + rect_button.width() +
                                 border.width(), kMinComboboxWidth);
  // The two arguments to ::max below should be typically be equal.
  int pref_height = std::max(rect_item.height() + 2 * kItemOffset,
                             rect_button.height() + 2 * border.height());
  return gfx::Size(pref_width, pref_height);
}

View* NativeComboboxWin::GetView() {
  return this;
}

void NativeComboboxWin::SetFocus() {
  OnFocus();
}

void NativeComboboxWin::ValidityStateChanged() {
  // TODO(estade): implement.
}

bool NativeComboboxWin::HandleKeyPressed(const ui::KeyEvent& event) {
  return false;
}

bool NativeComboboxWin::HandleKeyReleased(const ui::KeyEvent& event) {
  return false;
}

void NativeComboboxWin::HandleFocus() {
}

void NativeComboboxWin::HandleBlur() {
}

gfx::NativeView NativeComboboxWin::GetTestingHandle() const {
  return native_view();
}

////////////////////////////////////////////////////////////////////////////////
// NativeComboboxWin, NativeControlWin overrides:

bool NativeComboboxWin::ProcessMessage(UINT message,
                                       WPARAM w_param,
                                       LPARAM l_param,
                                       LRESULT* result) {
  if (message == WM_COMMAND && HIWORD(w_param) == CBN_SELCHANGE) {
    combobox_->SelectionChanged();
    *result = 0;
    return true;
  }
  return NativeControlWin::ProcessMessage(message, w_param, l_param, result);
}

void NativeComboboxWin::CreateNativeControl() {
  // It's ok to add WS_VSCROLL. The scrollbar will show up only when necessary
  // as long as we don't use CBS_DISABLENOSCROLL.
  // See http://msdn.microsoft.com/en-us/library/7h63bxbe(VS.80).aspx
  DWORD flags = WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                CBS_DROPDOWNLIST | WS_VSCROLL;
  HWND control_hwnd = ::CreateWindowEx(GetAdditionalExStyle(), L"COMBOBOX", L"",
                                       flags, 0, 0, width(), height(),
                                       GetWidget()->GetNativeView(), NULL, NULL,
                                       NULL);
  ui::CheckWindowCreated(control_hwnd);
  NativeControlCreated(control_hwnd);
}

void NativeComboboxWin::NativeControlCreated(HWND native_control) {
  NativeControlWin::NativeControlCreated(native_control);

  UpdateFont();
}

////////////////////////////////////////////////////////////////////////////////
// NativeComboboxWin, private:

void NativeComboboxWin::UpdateFont() {
  HFONT font = Combobox::GetFont().GetNativeFont();
  SendMessage(native_view(), WM_SETFONT, reinterpret_cast<WPARAM>(font), FALSE);
}

////////////////////////////////////////////////////////////////////////////////
// NativeComboboxWrapper, public:

// static
NativeComboboxWrapper* NativeComboboxWrapper::CreateWrapper(
    Combobox* combobox) {
#if defined(USE_AURA)
  return new NativeComboboxViews(combobox);
#else
  return new NativeComboboxWin(combobox);
#endif
}

}  // namespace views
