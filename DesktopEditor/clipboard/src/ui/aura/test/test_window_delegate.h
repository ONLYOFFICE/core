// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_TEST_WINDOW_DELEGATE_H_
#define UI_AURA_TEST_TEST_WINDOW_DELEGATE_H_

#include <string>

#include "base/compiler_specific.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/aura/window_delegate.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/gfx/rect.h"

namespace aura {
namespace test {

// WindowDelegate implementation with all methods stubbed out.
class TestWindowDelegate : public WindowDelegate {
 public:
  TestWindowDelegate();
  virtual ~TestWindowDelegate();

  // Returns a TestWindowDelegate that delete itself when
  // the associated window is destroyed.
  static TestWindowDelegate* CreateSelfDestroyingDelegate();

  void set_window_component(int window_component) {
    window_component_ = window_component;
  }

  void set_minimum_size(const gfx::Size& minimum_size) {
    minimum_size_ = minimum_size;
  }

  void set_maximum_size(const gfx::Size& maximum_size) {
    maximum_size_ = maximum_size;
  }

  // Sets the return value for CanFocus(). Default is true.
  void set_can_focus(bool can_focus) { can_focus_ = can_focus; }

  // Overridden from WindowDelegate:
  virtual gfx::Size GetMinimumSize() const OVERRIDE;
  virtual gfx::Size GetMaximumSize() const OVERRIDE;
  virtual void OnBoundsChanged(const gfx::Rect& old_bounds,
                               const gfx::Rect& new_bounds) OVERRIDE;
  virtual gfx::NativeCursor GetCursor(const gfx::Point& point) OVERRIDE;
  virtual int GetNonClientComponent(const gfx::Point& point) const OVERRIDE;
  virtual bool ShouldDescendIntoChildForEventHandling(
      Window* child,
      const gfx::Point& location) OVERRIDE;
  virtual bool CanFocus() OVERRIDE;
  virtual void OnCaptureLost() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual void OnWindowDestroying() OVERRIDE;
  virtual void OnWindowDestroyed() OVERRIDE;
  virtual void OnWindowTargetVisibilityChanged(bool visible) OVERRIDE;
  virtual bool HasHitTestMask() const OVERRIDE;
  virtual void GetHitTestMask(gfx::Path* mask) const OVERRIDE;
  virtual scoped_refptr<ui::Texture> CopyTexture() OVERRIDE;

 private:
  int window_component_;
  bool delete_on_destroyed_;
  gfx::Size minimum_size_;
  gfx::Size maximum_size_;
  bool can_focus_;

  DISALLOW_COPY_AND_ASSIGN(TestWindowDelegate);
};

// A simple WindowDelegate implementation for these tests. It owns itself
// (deletes itself when the Window it is attached to is destroyed).
class ColorTestWindowDelegate : public TestWindowDelegate {
 public:
  explicit ColorTestWindowDelegate(SkColor color);
  virtual ~ColorTestWindowDelegate();

  ui::KeyboardCode last_key_code() const { return last_key_code_; }

  // Overridden from TestWindowDelegate:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnWindowDestroyed() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  SkColor color_;
  ui::KeyboardCode last_key_code_;

  DISALLOW_COPY_AND_ASSIGN(ColorTestWindowDelegate);
};

// A simple WindowDelegate that has a hit-test mask.
class MaskedWindowDelegate : public TestWindowDelegate {
 public:
  explicit MaskedWindowDelegate(const gfx::Rect mask_rect);

  // Overridden from TestWindowDelegate:
  virtual bool HasHitTestMask() const OVERRIDE;
  virtual void GetHitTestMask(gfx::Path* mask) const OVERRIDE;

 private:
  gfx::Rect mask_rect_;

  DISALLOW_COPY_AND_ASSIGN(MaskedWindowDelegate);
};

// Keeps track of mouse/key events.
class EventCountDelegate : public TestWindowDelegate {
 public:
  EventCountDelegate();

  // Overridden from TestWindowDelegate:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;

  // Returns the counts of mouse motion events in the
  // form of "<enter> <move> <leave>".
  std::string GetMouseMotionCountsAndReset();

  // Returns the counts of mouse button events in the
  // form of "<press> <release>".
  std::string GetMouseButtonCountsAndReset();

  // Returns the counts of key events in the form of
  // "<press> <release>".
  std::string GetKeyCountsAndReset();

 private:
  int mouse_enter_count_;
  int mouse_move_count_;
  int mouse_leave_count_;
  int mouse_press_count_;
  int mouse_release_count_;
  int key_press_count_;
  int key_release_count_;

  DISALLOW_COPY_AND_ASSIGN(EventCountDelegate);
};

}  // namespace test
}  // namespace aura

#endif  // UI_AURA_TEST_TEST_WINDOW_DELEGATE_H_
