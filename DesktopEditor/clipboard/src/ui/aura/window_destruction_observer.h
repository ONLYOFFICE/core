// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_WINDOW_DESTRUCTION_OBSERVER_H_
#define UI_AURA_WINDOW_DESTRUCTION_OBSERVER_H_

#include <set>

#include "ui/aura/aura_export.h"
#include "ui/aura/window.h"
#include "ui/aura/window_observer.h"

namespace aura {

// A class for observing window (self) destruction. A side effect of a
// window member funtion may be that the object, i.e. |this|, gets
// deleted. This class can be used to detect such cases and to exit
// the member function without any further access to member data. See
// for example: aura::Window::NotifyWindowVisibilityChangedAtReceiver.

class AURA_EXPORT WindowDestructionObserver : public WindowObserver {
 public:
  // Addes this observer to window.
  explicit WindowDestructionObserver(aura::Window* window);

  // Removes this observer from |window_| unless |window_| has been
  // observed to be destroyed.
  virtual ~WindowDestructionObserver();

  // Returns true if |window_| has been observed to be destroyed.
  bool destroyed() const { return destroyed_; }

  private:
  // aura::WindowObserver overrides:
  virtual void OnWindowDestroyed(Window* window) OVERRIDE;

  Window* window_;
  bool destroyed_;

  DISALLOW_COPY_AND_ASSIGN(WindowDestructionObserver);
};

} // namespace aura

#endif
