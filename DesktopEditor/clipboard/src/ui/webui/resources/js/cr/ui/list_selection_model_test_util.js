// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

function range(start, end) {
  var a = [];
  for (var i = start; i <= end; i++) {
    a.push(i);
  }
  return a;
}

function adjust(sm, index, removed, added) {
  var permutation = [];
  for (var i = 0; i < index; i++) {
    permutation.push(i);
  }
  for (var i = 0; i < removed; i++) {
    permutation.push(-1);
  }
  for (var i = index + removed; i < sm.length; i++) {
    permutation.push(i - removed + added);
  }
  sm.adjustLength(sm.length - removed + added);
  sm.adjustToReordering(permutation);
}
