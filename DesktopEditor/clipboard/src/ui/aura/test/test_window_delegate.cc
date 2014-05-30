// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/test/test_window_delegate.h"

#include "base/stringprintf.h"
#include "ui/aura/window.h"
#include "ui/base/events/event.h"
#include "ui/base/hit_test.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/path.h"
#include "ui/gfx/skia_util.h"

namespace aura {
namespace test {

////////////////////////////////////////////////////////////////////////////////
// TestWindowDelegate

TestWindowDelegate::TestWindowDelegate()
    : window_component_(HTCLIENT),
      delete_on_destroyed_(false),
      can_focus_(true) {
}

TestWindowDelegate::~TestWindowDelegate() {
}

// static
TestWindowDelegate* TestWindowDelegate::CreateSelfDestroyingDelegate() {
  TestWindowDelegate* delegate = new TestWindowDelegate;
  delegate->delete_on_destroyed_ = true;
  return delegate;
}

gfx::Size TestWindowDelegate::GetMinimumSize() const {
  return minimum_size_;
}

gfx::Size TestWindowDelegate::GetMaximumSize() const {
  return maximum_size_;
}

void TestWindowDelegate::OnBoundsChanged(const gfx::Rect& old_bounds,
                                         const gfx::Rect& new_bounds) {
}

gfx::NativeCursor TestWindowDelegate::GetCursor(const gfx::Point& point) {
  return gfx::kNullCursor;
}

int TestWindowDelegate::GetNonClientComponent(const gfx::Point& point) const {
  return window_component_;
}

bool TestWindowDelegate::ShouldDescendIntoChildForEventHandling(
      Window* child,
      const gfx::Point& location) {
  return true;
}

bool TestWindowDelegate::CanFocus() {
  return can_focus_;
}

void TestWindowDelegate::OnCaptureLost() {
}

void TestWindowDelegate::OnPaint(gfx::Canvas* canvas) {
}

void TestWindowDelegate::OnDeviceScaleFactorChanged(
    float device_scale_factor) {
}

void TestWindowDelegate::OnWindowDestroying() {
}

void TestWindowDelegate::OnWindowDestroyed() {
  if (delete_on_destroyed_)
    delete this;
}

void TestWindowDelegate::OnWindowTargetVisibilityChanged(bool visible) {
}

bool TestWindowDelegate::HasHitTestMask() const {
  return false;
}

void TestWindowDelegate::GetHitTestMask(gfx::Path* mask) const {
}

scoped_refptr<ui::Texture> TestWindowDelegate::CopyTexture() {
  return scoped_refptr<ui::Texture>();
}

////////////////////////////////////////////////////////////////////////////////
// ColorTestWindowDelegate

ColorTestWindowDelegate::ColorTestWindowDelegate(SkColor color)
    : color_(color),
      last_key_code_(ui::VKEY_UNKNOWN) {
}

ColorTestWindowDelegate::~ColorTestWindowDelegate() {
}

void ColorTestWindowDelegate::OnKeyEvent(ui::KeyEvent* event) {
  last_key_code_ = event->key_code();
  event->SetHandled();
}

void ColorTestWindowDelegate::OnWindowDestroyed() {
  delete this;
}

void ColorTestWindowDelegate::OnPaint(gfx::Canvas* canvas) {
  canvas->DrawColor(color_, SkXfermode::kSrc_Mode);
}

////////////////////////////////////////////////////////////////////////////////
// MaskedWindowDelegate

MaskedWindowDelegate::MaskedWindowDelegate(const gfx::Rect mask_rect)
    : mask_rect_(mask_rect) {
}

bool MaskedWindowDelegate::HasHitTestMask() const {
  return true;
}

void MaskedWindowDelegate::GetHitTestMask(gfx::Path* mask) const {
  mask->addRect(RectToSkRect(mask_rect_));
}

////////////////////////////////////////////////////////////////////////////////
// EventCountDelegate

EventCountDelegate::EventCountDelegate()
  : mouse_enter_count_(0),
    mouse_move_count_(0),
    mouse_leave_count_(0),
    mouse_press_count_(0),
    mouse_release_count_(0),
    key_press_count_(0),
    key_release_count_(0) {
}

void EventCountDelegate::OnKeyEvent(ui::KeyEvent* event) {
  switch (event->type()) {
    case ui::ET_KEY_PRESSED:
      key_press_count_++;
      break;
    case ui::ET_KEY_RELEASED:
      key_release_count_++;
    default:
      break;
  }
}

void EventCountDelegate::OnMouseEvent(ui::MouseEvent* event) {
  switch (event->type()) {
    case ui::ET_MOUSE_MOVED:
      mouse_move_count_++;
      break;
    case ui::ET_MOUSE_ENTERED:
      mouse_enter_count_++;
      break;
    case ui::ET_MOUSE_EXITED:
      mouse_leave_count_++;
      break;
    case ui::ET_MOUSE_PRESSED:
      mouse_press_count_++;
      break;
    case ui::ET_MOUSE_RELEASED:
      mouse_release_count_++;
      break;
    default:
      break;
  }
}

std::string EventCountDelegate::GetMouseMotionCountsAndReset() {
  std::string result = base::StringPrintf("%d %d %d",
                                          mouse_enter_count_,
                                          mouse_move_count_,
                                          mouse_leave_count_);
  mouse_enter_count_ = 0;
  mouse_move_count_ = 0;
  mouse_leave_count_ = 0;
  return result;
}

std::string EventCountDelegate::GetMouseButtonCountsAndReset() {
  std::string result = base::StringPrintf("%d %d",
                                          mouse_press_count_,
                                          mouse_release_count_);
  mouse_press_count_ = 0;
  mouse_release_count_ = 0;
  return result;
}


std::string EventCountDelegate::GetKeyCountsAndReset() {
  std::string result = base::StringPrintf("%d %d",
                                          key_press_count_,
                                          key_release_count_);
  key_press_count_ = 0;
  key_release_count_ = 0;
  return result;
}

}  // namespace test
}  // namespace aura
