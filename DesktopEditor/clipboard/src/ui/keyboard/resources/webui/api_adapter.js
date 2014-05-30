// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

function sendKeyEvent(keyEvent) {
  keyEvent.type = 'keydown';
  chrome.send('sendKeyEvent', [ keyEvent ]);
  keyEvent.type = 'keyup';
  chrome.send('sendKeyEvent', [ keyEvent ]);
}
