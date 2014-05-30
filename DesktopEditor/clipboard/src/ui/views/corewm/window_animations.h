// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_WINDOW_ANIMATIONS_H_
#define UI_VIEWS_COREWM_WINDOW_ANIMATIONS_H_

#include <vector>

#include "ui/views/views_export.h"

namespace aura {
class Window;
}
namespace base {
class TimeDelta;
}
namespace gfx {
class Rect;
}
namespace ui {
class ImplicitAnimationObserver;
class Layer;
class LayerAnimationSequence;
}

namespace views {
namespace corewm {

// A variety of canned animations for window transitions.
enum WindowVisibilityAnimationType {
  WINDOW_VISIBILITY_ANIMATION_TYPE_DEFAULT = 0,     // Default. Lets the system
                                                    // decide based on window
                                                    // type.
  WINDOW_VISIBILITY_ANIMATION_TYPE_DROP,            // Window shrinks in.
  WINDOW_VISIBILITY_ANIMATION_TYPE_VERTICAL,        // Vertical Glenimation.
  WINDOW_VISIBILITY_ANIMATION_TYPE_FADE,            // Fades in/out.
  WINDOW_VISIBILITY_ANIMATION_TYPE_ROTATE,          // Window rotates in.

  // Downstream library animations start above this point.
  WINDOW_VISIBILITY_ANIMATION_MAX
};

// Canned animations that take effect once but don't have a symmetric pair as
// visibility animations do.
enum WindowAnimationType {
  WINDOW_ANIMATION_TYPE_BOUNCE = 0,  // Window scales up and down.
};

// Type of visibility change transition that a window should animate.
// Default behavior is to animate both show and hide.
enum WindowVisibilityAnimationTransition {
  ANIMATE_SHOW = 0x1,
  ANIMATE_HIDE = 0x2,
  ANIMATE_BOTH = ANIMATE_SHOW | ANIMATE_HIDE,
  ANIMATE_NONE = 0x4,
};

// These two methods use int for type rather than WindowVisibilityAnimationType
// since downstream libraries can extend the set of animations.
VIEWS_EXPORT void SetWindowVisibilityAnimationType(aura::Window* window,
                                                   int type);
VIEWS_EXPORT int GetWindowVisibilityAnimationType(aura::Window* window);

VIEWS_EXPORT void SetWindowVisibilityAnimationTransition(
    aura::Window* window,
    WindowVisibilityAnimationTransition transition);

VIEWS_EXPORT bool HasWindowVisibilityAnimationTransition(
    aura::Window* window,
    WindowVisibilityAnimationTransition transition);

VIEWS_EXPORT void SetWindowVisibilityAnimationDuration(
    aura::Window* window,
    const base::TimeDelta& duration);

VIEWS_EXPORT void SetWindowVisibilityAnimationVerticalPosition(
    aura::Window* window,
    float position);

// Creates an ImplicitAnimationObserver that takes ownership of the layers
// associated with a Window so that the animation can continue after the Window
// has been destroyed.
// The returned object deletes itself when the animations are done.
VIEWS_EXPORT ui::ImplicitAnimationObserver* CreateHidingWindowAnimationObserver(
    aura::Window* window);

// Returns false if the |window| didn't animate.
VIEWS_EXPORT bool AnimateOnChildWindowVisibilityChanged(aura::Window* window,
                                                        bool visible);
VIEWS_EXPORT bool AnimateWindow(aura::Window* window, WindowAnimationType type);

// Returns true if window animations are disabled for |window|. Window
// animations are enabled by default. If |window| is NULL, this just checks
// if the global flag disabling window animations is present.
VIEWS_EXPORT bool WindowAnimationsDisabled(aura::Window* window);

}  // namespace corewm
}  // namespace views

#endif  // UI_VIEWS_COREWM_WINDOW_ANIMATIONS_H_
