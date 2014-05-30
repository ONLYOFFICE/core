// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_FOCUS_MANAGER_H_
#define UI_AURA_FOCUS_MANAGER_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "ui/aura/aura_export.h"
#include "ui/aura/client/focus_client.h"

namespace ui {
class Event;
}

namespace aura {

class Window;

// An interface implemented by the Desktop to expose the focused window and
// allow for it to be changed.
class AURA_EXPORT FocusManager : public client::FocusClient {
 public:
  FocusManager();
  virtual ~FocusManager();

 private:
  // Overridden from client::FocusClient:
  virtual void AddObserver(client::FocusChangeObserver* observer) OVERRIDE;
  virtual void RemoveObserver(client::FocusChangeObserver* observer) OVERRIDE;
  virtual void FocusWindow(Window* window) OVERRIDE;
  virtual void ResetFocusWithinActiveWindow(Window* window) OVERRIDE;
  virtual Window* GetFocusedWindow() OVERRIDE;
  virtual void OnWindowHiddenInRootWindow(aura::Window* window,
                                          aura::RootWindow* root_window,
                                          bool destroyed) OVERRIDE;

 protected:
  aura::Window* focused_window_;

  ObserverList<client::FocusChangeObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(FocusManager);
};

}  // namespace aura

#endif  // UI_AURA_FOCUS_MANAGER_H_
