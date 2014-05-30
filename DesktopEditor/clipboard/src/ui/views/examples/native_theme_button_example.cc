// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/examples/native_theme_button_example.h"

#include <string>

#include "base/logging.h"
#include "base/stringprintf.h"
#include "base/utf_string_conversions.h"
#include "ui/base/animation/throb_animation.h"
#include "ui/base/models/combobox_model.h"
#include "ui/gfx/canvas.h"
#include "ui/views/controls/combobox/combobox.h"
#include "ui/views/controls/label.h"
#include "ui/views/examples/example_combobox_model.h"
#include "ui/views/layout/grid_layout.h"
#include "ui/views/native_theme_painter.h"

namespace {

const char* kParts[] = {
    "PushButton",
    "RadioButton",
    "Checkbox",
};

const char* kStates[] = {
    "Disabled",
    "Normal",
    "Hot",
    "Pressed",
    "<Dynamic>",
};

}  // namespace

namespace views {
namespace examples {

ExampleNativeThemeButton::ExampleNativeThemeButton(
    ButtonListener* listener,
    Combobox* cb_part,
    Combobox* cb_state)
    : CustomButton(listener),
      cb_part_(cb_part),
      cb_state_(cb_state),
      count_(0),
      is_checked_(false),
      is_indeterminate_(false) {
  cb_part_->set_listener(this);
  cb_state_->set_listener(this);

  painter_.reset(new NativeThemePainter(this, this));
  set_background(Background::CreateBackgroundPainter(
      false, painter_.get()));
}

ExampleNativeThemeButton::~ExampleNativeThemeButton() {
}

std::string ExampleNativeThemeButton::MessWithState() {
  const char* message = NULL;
  switch (GetThemePart()) {
    case ui::NativeTheme::kPushButton:
      message = "Pressed! count:%d";
      break;
    case ui::NativeTheme::kRadio:
      is_checked_ = !is_checked_;
      message = is_checked_ ? "Checked! count:%d" : "Unchecked! count:%d";
      break;
    case ui::NativeTheme::kCheckbox:
      if (is_indeterminate_) {
        is_checked_ = false;
        is_indeterminate_ = false;
      } else if (!is_checked_) {
        is_checked_ = true;
      } else {
        is_checked_ = false;
        is_indeterminate_ = true;
      }
      if (is_checked_)
        message = "Checked! count:%d";
      else if (is_indeterminate_)
        message = "Indeterminate! count:%d";
      else
        message = "Unchecked! count:%d";
      break;
    default:
      NOTREACHED();
  }

  return base::StringPrintf(message, ++count_);
}

void ExampleNativeThemeButton::OnSelectedIndexChanged(Combobox* combobox) {
  SchedulePaint();
}

ui::NativeTheme::Part ExampleNativeThemeButton::GetThemePart() const {
  switch (cb_part_->selected_index()) {
    case 0:
      return ui::NativeTheme::kPushButton;
    case 1:
      return ui::NativeTheme::kRadio;
    case 2:
      return ui::NativeTheme::kCheckbox;
    default:
      NOTREACHED();
  }
  return ui::NativeTheme::kPushButton;
}

gfx::Rect ExampleNativeThemeButton::GetThemePaintRect() const {
  ui::NativeTheme::ExtraParams extra;
  ui::NativeTheme::State state = GetThemeState(&extra);
  gfx::Size size(GetNativeTheme()->GetPartSize(GetThemePart(),
                                               state,
                                               extra));
  gfx::Rect rect(size);
  rect.set_x(GetMirroredXForRect(rect));
  return rect;
}

ui::NativeTheme::State ExampleNativeThemeButton::GetThemeState(
    ui::NativeTheme::ExtraParams* params) const {
  GetExtraParams(params);

  int selected = cb_state_->selected_index();
  if (selected > 3) {
    switch (state()) {
      case STATE_DISABLED:
        return ui::NativeTheme::kDisabled;
      case STATE_NORMAL:
        return ui::NativeTheme::kNormal;
      case STATE_HOVERED:
        return ui::NativeTheme::kHovered;
      case STATE_PRESSED:
        return ui::NativeTheme::kPressed;
      default:
        NOTREACHED();
    }
  }

  switch (selected) {
    case 0:
      return ui::NativeTheme::kDisabled;
    case 1:
      return ui::NativeTheme::kNormal;
    case 2:
      return ui::NativeTheme::kHovered;
    case 3:
      return ui::NativeTheme::kPressed;
    default:
      NOTREACHED();
  }
  return ui::NativeTheme::kNormal;
}

void ExampleNativeThemeButton::GetExtraParams(
    ui::NativeTheme::ExtraParams* params) const {

  params->button.checked = is_checked_;
  params->button.indeterminate = is_indeterminate_;
  params->button.is_default = false;
  params->button.has_border = false;
  params->button.classic_state = 0;
  params->button.background_color = SkColorSetARGB(0, 0, 0, 0);
}

const ui::Animation* ExampleNativeThemeButton::GetThemeAnimation() const {
  return cb_state_->selected_index() <= 3 ? NULL : hover_animation_.get();
}

ui::NativeTheme::State ExampleNativeThemeButton::GetBackgroundThemeState(
    ui::NativeTheme::ExtraParams* params) const {
  GetExtraParams(params);
  return ui::NativeTheme::kNormal;
}

ui::NativeTheme::State ExampleNativeThemeButton::GetForegroundThemeState(
    ui::NativeTheme::ExtraParams* params) const {
  GetExtraParams(params);
  return ui::NativeTheme::kHovered;
}

gfx::Size ExampleNativeThemeButton::GetPreferredSize() {
  return painter_.get() == NULL ? gfx::Size() : painter_->GetPreferredSize();
}

void ExampleNativeThemeButton::OnPaintBackground(gfx::Canvas* canvas) {
  // Fill the background with a known colour so that we know where the bounds
  // of the View are.
  canvas->FillRect(GetLocalBounds(), SkColorSetRGB(255, 128, 128));
  CustomButton::OnPaintBackground(canvas);
}

////////////////////////////////////////////////////////////////////////////////

NativeThemeButtonExample::NativeThemeButtonExample()
    : ExampleBase("Native Theme Button") {
}

NativeThemeButtonExample::~NativeThemeButtonExample() {
}

void NativeThemeButtonExample::CreateExampleView(View* container) {
  GridLayout* layout = new GridLayout(container);
  container->SetLayoutManager(layout);

  layout->AddPaddingRow(0, 8);

  ColumnSet* column_set = layout->AddColumnSet(0);
  column_set->AddPaddingColumn(0, 8);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::FILL,
                        0.1f, GridLayout::USE_PREF, 0, 0);
  column_set->AddColumn(GridLayout::FILL, GridLayout::FILL,
                        0.9f, GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 8);

  layout->StartRow(0, 0);
  layout->AddView(new Label(ASCIIToUTF16("Part:")));
  combobox_model_part_.reset(
      new ExampleComboboxModel(kParts, arraysize(kParts)));
  Combobox* combobox_part = new Combobox(combobox_model_part_.get());
  combobox_part->SetSelectedIndex(0);
  layout->AddView(combobox_part);

  layout->StartRow(0, 0);
  layout->AddView(new Label(ASCIIToUTF16("State:")));
  combobox_model_state_.reset(
      new ExampleComboboxModel(kStates, arraysize(kStates)));
  Combobox* combobox_state = new Combobox(combobox_model_state_.get());
  combobox_state->SetSelectedIndex(0);
  layout->AddView(combobox_state);

  layout->AddPaddingRow(0, 32);

  button_ = new ExampleNativeThemeButton(this, combobox_part, combobox_state);

  column_set = layout->AddColumnSet(1);
  column_set->AddPaddingColumn(0, 16);
  column_set->AddColumn(GridLayout::FILL, GridLayout::FILL,
                        1, GridLayout::USE_PREF, 0, 0);
  column_set->AddPaddingColumn(0, 16);
  layout->StartRow(1, 1);
  layout->AddView(button_);

  layout->AddPaddingRow(0, 8);
}

void NativeThemeButtonExample::ButtonPressed(Button* sender,
                                             const ui::Event& event) {
  PrintStatus(button_->MessWithState().c_str());
}

}  // namespace examples
}  // namespace views
