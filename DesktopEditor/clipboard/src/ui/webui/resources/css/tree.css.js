// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

(function() {
  var a = 7;
  var a2 = a / 2;
  var ctx = document.getCSSCanvasContext('2d', 'tree-triangle', a + 1, a2 + 2);

  ctx.fillStyle = '#000';
  ctx.translate(.5, .5);

  ctx.beginPath();
  ctx.moveTo(0, 0);
  ctx.lineTo(0, 1);
  ctx.lineTo(a2, 1 + a2);
  ctx.lineTo(a, 1);
  ctx.lineTo(a, 0);
  ctx.closePath();
  ctx.fill();
  ctx.stroke();
})();
