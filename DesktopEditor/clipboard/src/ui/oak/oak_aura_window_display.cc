// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/oak/oak_aura_window_display.h"

#include "base/logging.h"
#include "base/string_util.h"
#include "base/stringprintf.h"
#include "base/utf_string_conversions.h"
#include "ui/aura/window.h"
#include "ui/base/models/table_model_observer.h"
#include "ui/oak/oak_pretty_print.h"

namespace oak {
namespace internal {
namespace {
enum {
ROW_ID = 0,
ROW_DELEGATE,
ROW_TYPE,
ROW_NAME,
ROW_TITLE,
ROW_TRANSPARENT,
ROW_LAYER,
ROW_VISIBLE,
ROW_BOUNDS,
ROW_BOUNDSINROOTWINDOW,
ROW_TRANSFORM,
ROW_PARENT,
ROW_ROOTWINDOW,
ROW_TRANSIENTCHILDREN,
ROW_TRANSIENTPARENT,
ROW_USERDATA,
ROW_IGNOREEVENTS,
ROW_CANFOCUS,
ROW_HITTESTBOUNDSOVERRIDEOUTER,
ROW_HITTESTBOUNDSOVERRIDEINNER,
ROW_COUNT
};

// aura::Window-specific pretty printing.
base::string16 PropertyWithWindowType(int type) {
  std::string property = "Type: ";
  switch (type) {
    case aura::client::WINDOW_TYPE_UNKNOWN:
      property.append("WINDOW_TYPE_UNKNOWN");
      break;
    case aura::client::WINDOW_TYPE_NORMAL:
      property.append("WINDOW_TYPE_NORMAL");
      break;
    case aura::client::WINDOW_TYPE_POPUP:
      property.append("WINDOW_TYPE_POPUP");
      break;
    case aura::client::WINDOW_TYPE_CONTROL:
      property.append("WINDOW_TYPE_CONTROL");
      break;
    case aura::client::WINDOW_TYPE_PANEL:
      property.append("WINDOW_TYPE_PANEL");
      break;
    case aura::client::WINDOW_TYPE_MENU:
      property.append("WINDOW_TYPE_MENU");
      break;
    case aura::client::WINDOW_TYPE_TOOLTIP:
      property.append("WINDOW_TYPE_TOOLTIP");
      break;
    default:
      NOTREACHED();
      break;
  }
  return ASCIIToUTF16(property);
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// OakAuraWindowDisplay, public:

OakAuraWindowDisplay::OakAuraWindowDisplay() : observer_(NULL), window_(NULL) {
}

OakAuraWindowDisplay::~OakAuraWindowDisplay() {
}

////////////////////////////////////////////////////////////////////////////////
// OakAuraWindowDisplay, OakDetailsModel overrides:

void OakAuraWindowDisplay::SetValue(aura::Window* window) {
  window_ = window;
  observer_->OnModelChanged();
}

////////////////////////////////////////////////////////////////////////////////
// OakAuraWindowDisplay, ui::TableModel implementation:

int OakAuraWindowDisplay::RowCount() {
  return ROW_COUNT;
}

base::string16 OakAuraWindowDisplay::GetText(int row, int column_id) {
  if (!window_)
    return EmptyString16();

  base::string16 text;
  switch (row) {
    case ROW_ID:
      return PropertyWithInteger("Id: ", window_->id());
    case ROW_DELEGATE:
      return PropertyWithVoidStar("Delegate: ", window_->delegate());
    case ROW_TYPE:
      return PropertyWithWindowType(window_->type());
    case ROW_NAME:
      return ASCIIToUTF16("Name: " + window_->name());
    case ROW_TITLE:
      return ASCIIToUTF16("Title: ") + window_->title();
    case ROW_TRANSPARENT:
      return PropertyWithBool("Transparent: ", window_->transparent());
    case ROW_LAYER:
      return PropertyWithVoidStar("Layer: ", window_->layer());
    case ROW_VISIBLE:
      return PropertyWithBool("Visible: ", window_->IsVisible());
    case ROW_BOUNDS:
      return PropertyWithBounds("Bounds: ", window_->bounds());
    case ROW_BOUNDSINROOTWINDOW:
      return PropertyWithBounds("Bounds in Root Window: ",
                                window_->GetBoundsInRootWindow());
    case ROW_TRANSFORM:
      return ASCIIToUTF16("Transform:");
    case ROW_PARENT:
      return PropertyWithVoidStar("Parent: ", window_->parent());
    case ROW_ROOTWINDOW:
      return PropertyWithVoidStar("Root Window: ", window_->GetRootWindow());
    case ROW_TRANSIENTCHILDREN:
      return PropertyWithInteger("Transient Children: ",
                                 window_->transient_children().size());
    case ROW_TRANSIENTPARENT:
      return PropertyWithVoidStar("Transient Parent: ",
                                  window_->transient_parent());
    case ROW_USERDATA:
      return PropertyWithVoidStar("User Data: ", window_->user_data());
    case ROW_IGNOREEVENTS:
      return PropertyWithBool("Can receive events: ",
                              window_->CanReceiveEvents());
    case ROW_CANFOCUS:
      return PropertyWithBool("Can Focus: ", window_->CanFocus());
    case ROW_HITTESTBOUNDSOVERRIDEOUTER:
      return PropertyWithInsets("Hit test bounds override outer: ",
          window_->hit_test_bounds_override_outer_mouse());
    case ROW_HITTESTBOUNDSOVERRIDEINNER:
      return PropertyWithInsets("Hit test bounds override inner: ",
                                window_->hit_test_bounds_override_inner());
    default:
      NOTREACHED();
      break;
  }
  return EmptyString16();
}

void OakAuraWindowDisplay::SetObserver(ui::TableModelObserver* observer) {
  observer_ = observer;
}

}  // namespace internal
}  // namespace oak
