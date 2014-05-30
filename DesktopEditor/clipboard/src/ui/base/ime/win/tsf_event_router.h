// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_WIN_TSF_EVENT_ROUTER_H_
#define UI_BASE_IME_WIN_TSF_EVENT_ROUTER_H_

#include <atlbase.h>
#include <atlcom.h>
#include <msctf.h>

#include <set>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/range/range.h"
#include "ui/base/ui_export.h"

struct ITfDocumentMgr;

namespace ui {

class TSFEventRouterObserver {
 public:
  TSFEventRouterObserver() {}

  // Called when the number of currently opened candidate windows changes.
  virtual void OnCandidateWindowCountChanged(size_t window_count) {}

  // Called when a composition is started.
  virtual void OnTSFStartComposition() {}

  // Called when the text contents are updated. If there is no composition,
  // ui::Range::InvalidRange is passed to |composition_range|.
  virtual void OnTextUpdated(const ui::Range& composition_range) {}

  // Called when a composition is terminated.
  virtual void OnTSFEndComposition() {}

 protected:
  virtual ~TSFEventRouterObserver() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(TSFEventRouterObserver);
};

// This class monitores TSF related events and forwards them to given
// |observer|.
class UI_EXPORT TSFEventRouter {
 public:
  // Do not pass NULL to |observer|.
  explicit TSFEventRouter(TSFEventRouterObserver* observer);
  virtual ~TSFEventRouter();

  // Returns true if the IME is composing text.
  bool IsImeComposing();

  // Callbacks from the TSFEventRouterDelegate:
  void OnCandidateWindowCountChanged(size_t window_count);
  void OnTSFStartComposition();
  void OnTextUpdated(const ui::Range& composition_range);
  void OnTSFEndComposition();

  // Sets |thread_manager| to be monitored. |thread_manager| can be NULL.
  void SetManager(ITfThreadMgr* thread_manager);

 private:
  class Delegate;

  CComPtr<Delegate> delegate_;

  TSFEventRouterObserver* observer_;

  DISALLOW_COPY_AND_ASSIGN(TSFEventRouter);
};

}  // namespace ui

#endif  // UI_BASE_IME_WIN_TSF_EVENT_ROUTER_H_
