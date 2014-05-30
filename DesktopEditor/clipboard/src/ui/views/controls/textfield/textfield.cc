// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/textfield/textfield.h"

#include <string>

#include "base/command_line.h"
#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/events/event.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/range/range.h"
#include "ui/base/ui_base_switches.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/selection_model.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/controls/image_view.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/controls/textfield/native_textfield_views.h"
#include "ui/views/controls/textfield/native_textfield_wrapper.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/widget/widget.h"

#if defined(OS_WIN)
#include "base/win/win_util.h"
// TODO(beng): this should be removed when the OS_WIN hack from
// ViewHierarchyChanged is removed.
#include "ui/views/controls/textfield/native_textfield_win.h"
#endif

namespace {

// Default placeholder text color.
const SkColor kDefaultPlaceholderTextColor = SK_ColorLTGRAY;

}  // namespace

namespace views {

/////////////////////////////////////////////////////////////////////////////
// Textfield

// static
const char Textfield::kViewClassName[] = "views/Textfield";

// static
bool Textfield::IsViewsTextfieldEnabled() {
#if defined(OS_WIN) && !defined(USE_AURA)
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kDisableViewsTextfield))
    return false;
  if (command_line->HasSwitch(switches::kEnableViewsTextfield))
    return true;
  // The new dialog style cannot host native Windows textfield controls.
  if (command_line->HasSwitch(switches::kEnableNewDialogStyle))
    return true;
  return false;
#endif
  return true;
}

Textfield::Textfield()
    : native_wrapper_(NULL),
      controller_(NULL),
      style_(STYLE_DEFAULT),
      read_only_(false),
      default_width_in_chars_(0),
      draw_border_(true),
      text_color_(SK_ColorBLACK),
      use_default_text_color_(true),
      background_color_(SK_ColorWHITE),
      use_default_background_color_(true),
      border_color_(SK_ColorWHITE),
      use_default_border_color_(true),
      initialized_(false),
      horizontal_margins_were_set_(false),
      vertical_margins_were_set_(false),
      placeholder_text_color_(kDefaultPlaceholderTextColor),
      icon_view_(NULL),
      text_input_type_(ui::TEXT_INPUT_TYPE_TEXT) {
  set_focusable(true);
}

Textfield::Textfield(StyleFlags style)
    : native_wrapper_(NULL),
      controller_(NULL),
      style_(style),
      read_only_(false),
      default_width_in_chars_(0),
      draw_border_(true),
      text_color_(SK_ColorBLACK),
      use_default_text_color_(true),
      background_color_(SK_ColorWHITE),
      use_default_background_color_(true),
      border_color_(SK_ColorWHITE),
      use_default_border_color_(true),
      initialized_(false),
      horizontal_margins_were_set_(false),
      vertical_margins_were_set_(false),
      placeholder_text_color_(kDefaultPlaceholderTextColor),
      icon_view_(NULL),
      text_input_type_(ui::TEXT_INPUT_TYPE_TEXT) {
  set_focusable(true);
  if (IsObscured())
    SetTextInputType(ui::TEXT_INPUT_TYPE_PASSWORD);
}

Textfield::~Textfield() {
}

void Textfield::SetController(TextfieldController* controller) {
  controller_ = controller;
}

TextfieldController* Textfield::GetController() const {
  return controller_;
}

void Textfield::SetReadOnly(bool read_only) {
  // Update read-only without changing the focusable state (or active, etc.).
  read_only_ = read_only;
  if (native_wrapper_) {
    native_wrapper_->UpdateReadOnly();
    native_wrapper_->UpdateTextColor();
    native_wrapper_->UpdateBackgroundColor();
  }
}

bool Textfield::IsObscured() const {
  return style_ & STYLE_OBSCURED;
}

void Textfield::SetObscured(bool obscured) {
  if (obscured) {
    style_ = static_cast<StyleFlags>(style_ | STYLE_OBSCURED);
    SetTextInputType(ui::TEXT_INPUT_TYPE_PASSWORD);
  } else {
    style_ = static_cast<StyleFlags>(style_ & ~STYLE_OBSCURED);
    SetTextInputType(ui::TEXT_INPUT_TYPE_TEXT);
  }
  if (native_wrapper_)
    native_wrapper_->UpdateIsObscured();
}


ui::TextInputType Textfield::GetTextInputType() const {
  if (read_only() || !enabled())
    return ui::TEXT_INPUT_TYPE_NONE;
  return text_input_type_;
}

void Textfield::SetTextInputType(ui::TextInputType type) {
  text_input_type_ = type;
  bool should_be_obscured = type == ui::TEXT_INPUT_TYPE_PASSWORD;
  if (IsObscured() != should_be_obscured)
    SetObscured(should_be_obscured);
}

void Textfield::SetText(const string16& text) {
  text_ = text;
  if (native_wrapper_)
    native_wrapper_->UpdateText();
}

void Textfield::AppendText(const string16& text) {
  text_ += text;
  if (native_wrapper_)
    native_wrapper_->AppendText(text);
}

void Textfield::ReplaceSelection(const string16& text) {
  if (native_wrapper_) {
    native_wrapper_->ReplaceSelection(text);
    text_ = native_wrapper_->GetText();
  }
}

base::i18n::TextDirection Textfield::GetTextDirection() const {
  return native_wrapper_ ? native_wrapper_->GetTextDirection() :
      base::i18n::UNKNOWN_DIRECTION;
}

void Textfield::SelectAll(bool reversed) {
  if (native_wrapper_)
    native_wrapper_->SelectAll(reversed);
}

string16 Textfield::GetSelectedText() const {
  if (native_wrapper_)
    return native_wrapper_->GetSelectedText();
  return string16();
}

void Textfield::ClearSelection() const {
  if (native_wrapper_)
    native_wrapper_->ClearSelection();
}

bool Textfield::HasSelection() const {
  if (native_wrapper_)
    return !native_wrapper_->GetSelectedRange().is_empty();
  return false;
}

SkColor Textfield::GetTextColor() const {
  if (!use_default_text_color_)
    return text_color_;

  return GetNativeTheme()->GetSystemColor(read_only() ?
      ui::NativeTheme::kColorId_TextfieldReadOnlyColor :
      ui::NativeTheme::kColorId_TextfieldDefaultColor);
}

void Textfield::SetTextColor(SkColor color) {
  text_color_ = color;
  use_default_text_color_ = false;
  if (native_wrapper_)
    native_wrapper_->UpdateTextColor();
}

void Textfield::UseDefaultTextColor() {
  use_default_text_color_ = true;
  if (native_wrapper_)
    native_wrapper_->UpdateTextColor();
}

SkColor Textfield::GetBackgroundColor() const {
  if (!use_default_background_color_)
    return background_color_;

  return GetNativeTheme()->GetSystemColor(read_only() ?
      ui::NativeTheme::kColorId_TextfieldReadOnlyBackground :
      ui::NativeTheme::kColorId_TextfieldDefaultBackground);
}

void Textfield::SetBackgroundColor(SkColor color) {
  background_color_ = color;
  use_default_background_color_ = false;
  if (native_wrapper_)
    native_wrapper_->UpdateBackgroundColor();
}

void Textfield::UseDefaultBackgroundColor() {
  use_default_background_color_ = true;
  if (native_wrapper_)
    native_wrapper_->UpdateBackgroundColor();
}

bool Textfield::GetCursorEnabled() const {
  return native_wrapper_ && native_wrapper_->GetCursorEnabled();
}

void Textfield::SetCursorEnabled(bool enabled) {
  if (native_wrapper_)
    native_wrapper_->SetCursorEnabled(enabled);
}

void Textfield::SetFont(const gfx::Font& font) {
  font_ = font;
  if (native_wrapper_)
    native_wrapper_->UpdateFont();
  PreferredSizeChanged();
}

void Textfield::SetHorizontalMargins(int left, int right) {
  margins_.Set(margins_.top(), left, margins_.bottom(), right);
  horizontal_margins_were_set_ = true;
  if (native_wrapper_)
    native_wrapper_->UpdateHorizontalMargins();
  PreferredSizeChanged();
}

void Textfield::SetVerticalMargins(int top, int bottom) {
  margins_.Set(top, margins_.left(), bottom, margins_.right());
  vertical_margins_were_set_ = true;
  if (native_wrapper_)
    native_wrapper_->UpdateVerticalMargins();
  PreferredSizeChanged();
}

void Textfield::RemoveBorder() {
  if (!draw_border_)
    return;

  draw_border_ = false;
  if (native_wrapper_)
    native_wrapper_->UpdateBorder();
}

void Textfield::SetBorderColor(SkColor color) {
  border_color_ = color;
  use_default_border_color_ = false;
  native_wrapper_->UpdateBorderColor();
}

void Textfield::UseDefaultBorderColor() {
  if (use_default_border_color_)
    return;

  use_default_border_color_ = true;
  native_wrapper_->UpdateBorderColor();
}

void Textfield::SetIcon(const gfx::ImageSkia& icon) {
  if (icon.isNull()) {
    if (icon_view_) {
      RemoveChildView(icon_view_);
      delete icon_view_;
      icon_view_ = NULL;
    }

    return;
  }

  if (!icon_view_) {
    icon_view_ = new ImageView();
    AddChildView(icon_view_);
  }

  icon_view_->SetImage(icon);
  PreferredSizeChanged();
}

bool Textfield::GetHorizontalMargins(int* left, int* right) {
  if (!horizontal_margins_were_set_)
    return false;

  // Add the width of the icon as well as a margin for the icon.
  int icon_width = 0;
  if (icon_view_)
    icon_width = icon_view_->GetPreferredSize().width() + margins_.right();

  *left = margins_.left();
  *right = margins_.right();
  if (base::i18n::IsRTL())
    *left += icon_width;
  else
    *right += icon_width;

  return true;
}

bool Textfield::GetVerticalMargins(int* top, int* bottom) {
  if (!vertical_margins_were_set_)
    return false;
  *top = margins_.top();
  *bottom = margins_.bottom();
  return true;
}

void Textfield::UpdateAllProperties() {
  if (native_wrapper_) {
    native_wrapper_->UpdateText();
    native_wrapper_->UpdateTextColor();
    native_wrapper_->UpdateBackgroundColor();
    native_wrapper_->UpdateReadOnly();
    native_wrapper_->UpdateFont();
    native_wrapper_->UpdateEnabled();
    native_wrapper_->UpdateBorder();
    native_wrapper_->UpdateBorderColor();
    native_wrapper_->UpdateIsObscured();
    native_wrapper_->UpdateHorizontalMargins();
    native_wrapper_->UpdateVerticalMargins();
  }
}

void Textfield::SyncText() {
  if (native_wrapper_) {
    string16 new_text = native_wrapper_->GetText();
    if (new_text != text_) {
      text_ = new_text;
      if (controller_)
        controller_->ContentsChanged(this, text_);
    }
  }
}

bool Textfield::IsIMEComposing() const {
  return native_wrapper_ && native_wrapper_->IsIMEComposing();
}

ui::Range Textfield::GetSelectedRange() const {
  DCHECK(native_wrapper_);
  return native_wrapper_->GetSelectedRange();
}

void Textfield::SelectRange(const ui::Range& range) {
  DCHECK(native_wrapper_);
  native_wrapper_->SelectRange(range);
}

gfx::SelectionModel Textfield::GetSelectionModel() const {
  DCHECK(native_wrapper_);
  return native_wrapper_->GetSelectionModel();
}

void Textfield::SelectSelectionModel(const gfx::SelectionModel& sel) {
  DCHECK(native_wrapper_);
  native_wrapper_->SelectSelectionModel(sel);
}

size_t Textfield::GetCursorPosition() const {
  DCHECK(native_wrapper_);
  return native_wrapper_->GetCursorPosition();
}

void Textfield::SetColor(SkColor value) {
  DCHECK(native_wrapper_);
  return native_wrapper_->SetColor(value);
}

void Textfield::ApplyColor(SkColor value, const ui::Range& range) {
  DCHECK(native_wrapper_);
  return native_wrapper_->ApplyColor(value, range);
}

void Textfield::SetStyle(gfx::TextStyle style, bool value) {
  DCHECK(native_wrapper_);
  return native_wrapper_->SetStyle(style, value);
}

void Textfield::ApplyStyle(gfx::TextStyle style,
                           bool value,
                           const ui::Range& range) {
  DCHECK(native_wrapper_);
  return native_wrapper_->ApplyStyle(style, value, range);
}

void Textfield::ClearEditHistory() {
  DCHECK(native_wrapper_);
  native_wrapper_->ClearEditHistory();
}

void Textfield::SetAccessibleName(const string16& name) {
  accessible_name_ = name;
}

void Textfield::ExecuteCommand(int command_id) {
  native_wrapper_->ExecuteTextCommand(command_id);
}

bool Textfield::HasTextBeingDragged() {
  return native_wrapper_->HasTextBeingDragged();
}

////////////////////////////////////////////////////////////////////////////////
// Textfield, View overrides:

void Textfield::Layout() {
  if (native_wrapper_) {
    native_wrapper_->GetView()->SetBoundsRect(GetLocalBounds());
    native_wrapper_->GetView()->Layout();
  }
  if (icon_view_) {
    gfx::Rect bounds = GetLocalBounds();
    bounds.Inset(margins_);

    // Flush right, vertically centered.
    gfx::Size pref_size = icon_view_->GetPreferredSize();
    gfx::Rect icon_bounds(
        gfx::Point(bounds.right() - pref_size.width(),
                   bounds.y() + (bounds.height() - pref_size.height()) / 2),
        pref_size);
    icon_view_->SetBoundsRect(icon_bounds);
  }
}

int Textfield::GetBaseline() const {
  gfx::Insets insets;
  if (draw_border_ && native_wrapper_)
    insets = native_wrapper_->CalculateInsets();
  const int baseline = native_wrapper_ ?
      native_wrapper_->GetTextfieldBaseline() : font_.GetBaseline();
  return insets.top() + baseline;
}

gfx::Size Textfield::GetPreferredSize() {
  gfx::Insets insets;
  if (draw_border_ && native_wrapper_)
    insets = native_wrapper_->CalculateInsets();
  // For NativeTextfieldViews, we might use a pre-defined font list (defined in
  // IDS_UI_FONT_FAMILY_CROS) as the fonts to render text. The fonts in the
  // list might be different (in name or in size) from |font_|, so we need to
  // use GetFontHeight() to get the height of the first font in the list to
  // guide textfield's height.
  const int font_height = native_wrapper_ ? native_wrapper_->GetFontHeight() :
                                            font_.GetHeight();
  return gfx::Size(font_.GetExpectedTextWidth(default_width_in_chars_) +
                       insets.width(), font_height + insets.height());
}

void Textfield::AboutToRequestFocusFromTabTraversal(bool reverse) {
  SelectAll(false);
}

bool Textfield::SkipDefaultKeyEventProcessing(const ui::KeyEvent& e) {
  // Skip any accelerator handling of backspace; textfields handle this key.
  // Also skip processing of [Alt]+<num-pad digit> Unicode alt key codes.
  return e.key_code() == ui::VKEY_BACK || e.IsUnicodeKeyCode();
}

void Textfield::OnPaintFocusBorder(gfx::Canvas* canvas) {
  if (NativeViewHost::kRenderNativeControlFocus)
    View::OnPaintFocusBorder(canvas);
}

bool Textfield::OnKeyPressed(const ui::KeyEvent& e) {
  return native_wrapper_ && native_wrapper_->HandleKeyPressed(e);
}

bool Textfield::OnKeyReleased(const ui::KeyEvent& e) {
  return native_wrapper_ && native_wrapper_->HandleKeyReleased(e);
}

bool Textfield::OnMouseDragged(const ui::MouseEvent& e) {
  if (!e.IsOnlyRightMouseButton())
    return View::OnMouseDragged(e);
  return true;
}

void Textfield::OnFocus() {
  if (native_wrapper_)
    native_wrapper_->HandleFocus();

  // Forward the focus to the wrapper if it exists.
  if (!native_wrapper_ || !native_wrapper_->SetFocus()) {
    // If there is no wrapper or the wrapper didn't take focus, call
    // View::Focus to clear the native focus so that we still get
    // keyboard messages.
    View::OnFocus();
  }
}

void Textfield::OnBlur() {
  if (native_wrapper_)
    native_wrapper_->HandleBlur();
}

void Textfield::GetAccessibleState(ui::AccessibleViewState* state) {
  state->role = ui::AccessibilityTypes::ROLE_TEXT;
  state->name = accessible_name_;
  if (read_only())
    state->state |= ui::AccessibilityTypes::STATE_READONLY;
  if (IsObscured())
    state->state |= ui::AccessibilityTypes::STATE_PROTECTED;
  state->value = text_;

  DCHECK(native_wrapper_);
  const ui::Range range = native_wrapper_->GetSelectedRange();
  state->selection_start = range.start();
  state->selection_end = range.end();
}

ui::TextInputClient* Textfield::GetTextInputClient() {
  return native_wrapper_ ? native_wrapper_->GetTextInputClient() : NULL;
}

void Textfield::OnEnabledChanged() {
  View::OnEnabledChanged();
  if (native_wrapper_)
    native_wrapper_->UpdateEnabled();
}

void Textfield::ViewHierarchyChanged(bool is_add, View* parent, View* child) {
  if (is_add && !native_wrapper_ && GetWidget() && !initialized_) {
    initialized_ = true;

    // The native wrapper's lifetime will be managed by the view hierarchy after
    // we call AddChildView.
    native_wrapper_ = NativeTextfieldWrapper::CreateWrapper(this);
    AddChildViewAt(native_wrapper_->GetView(), 0);
    // TODO(beng): Move this initialization to NativeTextfieldWin once it
    //             subclasses NativeControlWin.
    UpdateAllProperties();

#if defined(OS_WIN) && !defined(USE_AURA)
    // TODO(beng): Remove this once NativeTextfieldWin subclasses
    // NativeControlWin. This is currently called to perform post-AddChildView
    // initialization for the wrapper.
    //
    // Remove the include for native_textfield_win.h above when you fix this.
    if (!IsViewsTextfieldEnabled())
      static_cast<NativeTextfieldWin*>(native_wrapper_)->AttachHack();
#endif
  }
}

std::string Textfield::GetClassName() const {
  return kViewClassName;
}

////////////////////////////////////////////////////////////////////////////////
// NativeTextfieldWrapper, public:

// static
NativeTextfieldWrapper* NativeTextfieldWrapper::CreateWrapper(
    Textfield* field) {
#if defined(OS_WIN) && !defined(USE_AURA)
  if (!Textfield::IsViewsTextfieldEnabled())
    return new NativeTextfieldWin(field);
#endif
  return new NativeTextfieldViews(field);
}

}  // namespace views
