// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_WINDOW_MODALITY_CONTROLLER_H_
#define UI_VIEWS_COREWM_WINDOW_MODALITY_CONTROLLER_H_

#include <vector>

#include "base/compiler_specific.h"
#include "ui/aura/env_observer.h"
#include "ui/aura/window_observer.h"
#include "ui/base/events/event_handler.h"
#include "ui/views/views_export.h"

namespace ui {
class LocatedEvent;
}

namespace views {
namespace corewm {

// Sets the modal parent for the child.
VIEWS_EXPORT void SetModalParent(aura::Window* child, aura::Window* parent);

// Returns the modal transient child of |window|, or NULL if |window| does not
// have any modal transient children.
VIEWS_EXPORT aura::Window* GetModalTransient(aura::Window* window);

// WindowModalityController is an event filter that consumes events sent to
// windows that are the transient parents of window-modal windows. This filter
// must be added to the CompoundEventFilter so that activation works properly.
class VIEWS_EXPORT WindowModalityController : public ui::EventHandler,
                                              public aura::EnvObserver,
                                              public aura::WindowObserver {
 public:
  WindowModalityController();
  virtual ~WindowModalityController();

  // Overridden from ui::EventHandler:
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;

  // Overridden from aura::EnvObserver:
  virtual void OnWindowInitialized(aura::Window* window) OVERRIDE;

  // Overridden from aura::WindowObserver:
  virtual void OnWindowPropertyChanged(aura::Window* window,
                                       const void* key,
                                       intptr_t old) OVERRIDE;
  virtual void OnWindowVisibilityChanged(aura::Window* window,
                                         bool visible) OVERRIDE;
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

 private:
  // Processes a mouse/touch event, and returns true if the event should be
  // consumed.
  bool ProcessLocatedEvent(aura::Window* target,
                           ui::LocatedEvent* event);

  std::vector<aura::Window*> windows_;

  DISALLOW_COPY_AND_ASSIGN(WindowModalityController);
};

}  // namespace corewm
}  // namespace views

#endif  // UI_VIEWS_COREWM_WINDOW_MODALITY_CONTROLLER_H_
