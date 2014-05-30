// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

function logIfError() {
  if (chrome.runtime.lastError) {
    console.log(chrome.runtime.lastError);
  }
}

function sendKeyEvent(keyEvent) {
  keyEvent.type = 'keydown';
  chrome.experimental.input.virtualKeyboard.sendKeyboardEvent(keyEvent,
      logIfError);
  keyEvent.type = 'keyup';
  chrome.experimental.input.virtualKeyboard.sendKeyboardEvent(keyEvent,
      logIfError);
}
