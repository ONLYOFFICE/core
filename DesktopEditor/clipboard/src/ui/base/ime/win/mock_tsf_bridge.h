// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_WIN_MOCK_TSF_BRIDGE_H_
#define UI_BASE_IME_WIN_MOCK_TSF_BRIDGE_H_

#include <msctf.h>

#include "base/compiler_specific.h"
#include "base/win/scoped_comptr.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/ime/win/tsf_bridge.h"

namespace ui {

class MockTSFBridge : public TSFBridge {
 public:
  MockTSFBridge();
  virtual ~MockTSFBridge();

  // TSFBridge:
  virtual void Shutdown() OVERRIDE;
  virtual bool CancelComposition() OVERRIDE;
  virtual void OnTextInputTypeChanged(TextInputClient* client) OVERRIDE;
  virtual void SetFocusedClient(HWND focused_window,
                                TextInputClient* client) OVERRIDE;
  virtual void RemoveFocusedClient(TextInputClient* client) OVERRIDE;
  virtual base::win::ScopedComPtr<ITfThreadMgr> GetThreadManager() OVERRIDE;
  virtual TextInputClient* GetFocusedTextInputClient() const OVERRIDE;

  // Resets MockTSFBridge state including function call counter.
  void Reset();

  // Call count of Shutdown().
  int shutdown_call_count() const { return shutdown_call_count_; }

  // Call count of EnableIME().
  int enable_ime_call_count() const { return enable_ime_call_count_; }

  // Call count of DisableIME().
  int disalbe_ime_call_count() const { return disalbe_ime_call_count_; }

  // Call count of CancelComposition().
  int cancel_composition_call_count() const {
    return cancel_composition_call_count_;
  }

  // Call count of AssociateFocus().
  int associate_focus_call_count() const { return associate_focus_call_count_; }

  // Call count of SetFocusClient().
  int set_focused_client_call_count() const {
    return set_focused_client_call_count_;
  }

  // Call count of Shutdown().
  int remove_focused_client_call_count() const {
    return remove_focused_client_call_count_;
  }

  // Returns current TextInputClient.
  TextInputClient* text_input_clinet() const { return text_input_client_; }

  // Returns currently focused window handle.
  HWND focused_window() const { return focused_window_; }

  // Returns latest text input type.
  TextInputType latest_text_iput_type() const {
    return latest_text_input_type_;
  }

 private:
  int shutdown_call_count_;
  int enable_ime_call_count_;
  int disalbe_ime_call_count_;
  int cancel_composition_call_count_;
  int associate_focus_call_count_;
  int set_focused_client_call_count_;
  int remove_focused_client_call_count_;
  TextInputClient* text_input_client_;
  HWND focused_window_;
  TextInputType latest_text_input_type_;
  base::win::ScopedComPtr<ITfThreadMgr> thread_manager_;

  DISALLOW_COPY_AND_ASSIGN(MockTSFBridge);
};

}  // namespace ui

#endif  // UI_BASE_IME_WIN_MOCK_TSF_BRIDGE_H_
