// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview A simple virtual keyboard implementation.
 */

/**
 * The ratio of the row height to the font size.
 * @type {number}
 */
/** @const */ var kFontSizeRatio = 3.5;

/**
 * Alias for document.getElementById.
 * @param {string} id The ID of the element to find.
 * @return {HTMLElement} The found element or null if not found.
 */
function $(id) {
  return document.getElementById(id);
}

/**
 * Return the id attribute of the keyboard element for the given layout.
 * @param {string} layout The keyboard layout.
 * @return {string} The id attribute of the keyboard element.
 */
function getKeyboardId(layout) {
  return 'keyboard_' + layout;
}

/**
 * Return the aspect ratio of the current keyboard.
 * @return {number} The aspect ratio of the current keyboard.
 */
function getKeyboardAspect() {
  return KEYBOARDS[currentKeyboardLayout]['aspect'];
}

/**
 * Calculate the height of the keyboard based on the size of the page.
 * @return {number} The height of the keyboard in pixels.
 */
function getKeyboardHeight() {
  var x = window.innerWidth;
  var y = window.innerHeight;
  return (x > getKeyboardAspect() * y) ?
      y : Math.floor(x / getKeyboardAspect());
}

/**
 * Create a DOM of the keyboard rows for the given keyboard layout.
 * Do nothing if the DOM is already created.
 * @param {string} layout The keyboard layout for which rows are created.
 * @param {Element} element The DOM Element to which rows are appended.
 * @param {boolean} autoPadding True if padding needs to be added to both side
 *     of the rows that have less keys.
 */
function initRows(layout, element, autoPadding) {
  var keyboard = KEYBOARDS[layout];
  if ('rows' in keyboard) {
    return;
  }
  var def = keyboard['definition'];
  var rows = [];
  for (var i = 0; i < def.length; ++i) {
    rows.push(new Row(i, def[i]));
  }
  keyboard['rows'] = rows;

  var maxRowLength = -1;
  for (var i = 0; i < rows.length; ++i) {
    if (rows[i].length > maxRowLength) {
      maxRowLength = rows[i].length;
    }
  }

  // A div element which holds rows for the layout.
  var rowsDiv = document.createElement('div');
  rowsDiv.className = 'rows';
  for (var i = 0; i < rows.length; ++i) {
    var rowDiv = rows[i].makeDOM();
    if (autoPadding && rows[i].length < maxRowLength) {
      var padding = 50 * (maxRowLength - rows[i].length) / maxRowLength;
      rowDiv.style.paddingLeft = padding + '%';
      rowDiv.style.paddingRight = padding + '%';
    }
    rowsDiv.appendChild(rowDiv);
    rows[i].showMode(currentMode);
  }
  keyboard['rowsDiv'] = rowsDiv;
  element.appendChild(rowsDiv);
}

/**
 * Create a DOM of the keyboard for the given keyboard layout.
 * Do nothing if the DOM is already created.
 * @param {string} layout The keyboard layout for which keyboard is created.
 * @param {Element} element The DOM Element to which keyboard is appended.
 */
function initKeyboard(layout, element) {
  var keyboard = KEYBOARDS[layout];
  if (!keyboard || keyboard['keyboardDiv']) {
    return;
  }
  var keyboardDiv = document.createElement('div');
  keyboardDiv.id = getKeyboardId(layout);
  keyboardDiv.className = 'keyboard';
  initRows(layout, keyboardDiv);
  keyboard['keyboardDiv'] = keyboardDiv;
  window.onresize();
  element.appendChild(keyboardDiv);
}

/**
 * Resize the keyboard according to the new window size.
 */
window.onresize = function() {
  var keyboardDiv = KEYBOARDS[currentKeyboardLayout]['keyboardDiv'];
  var height = getKeyboardHeight();
  keyboardDiv.style.height = height + 'px';
  var mainDiv = $('main');
  mainDiv.style.width = Math.floor(getKeyboardAspect() * height) + 'px';
  var rowsLength = KEYBOARDS[currentKeyboardLayout]['rows'].length;
  keyboardDiv.style.fontSize = (height / kFontSizeRatio / rowsLength) + 'px';
};

/**
 * Init the keyboard.
 */
var mainDiv = null;

/**
 * Initialize keyboard.
 */
window.onload = function() {
  var body = $('b');

  // Catch all unhandled touch events and prevent default, to prevent the
  // keyboard from responding to gestures like double tap.
  function disableGestures(evt) {
    evt.preventDefault();
  }
  body.addEventListener('touchstart', disableGestures);
  body.addEventListener('touchmove', disableGestures);
  body.addEventListener('touchend', disableGestures);

  mainDiv = document.createElement('div');
  mainDiv.className = 'main';
  mainDiv.id = 'main';
  body.appendChild(mainDiv);

  initKeyboard(currentKeyboardLayout, mainDiv);

  window.onhashchange();
};

/**
 * Switch the keyboard layout based on the current URL hash.
 */
window.onhashchange = function() {
  var oldLayout = currentKeyboardLayout;
  var newLayout = location.hash.replace(/^#/, '');
  if (oldLayout == newLayout) {
    return;
  }

  if (KEYBOARDS[newLayout] === undefined) {
    // Unsupported layout.
    newLayout = 'us';
  }
  currentKeyboardLayout = newLayout;

  var mainDiv = $('main');
  initKeyboard(currentKeyboardLayout, mainDiv);

  [newLayout, oldLayout].forEach(function(layout) {
      var visible = (layout == newLayout);
      var keyboardDiv = KEYBOARDS[layout]['keyboardDiv'];
      keyboardDiv.className = visible ? 'keyboard' : 'nodisplay';
      var canvas = KEYBOARDS[layout]['canvas'];
      if (canvas !== undefined) {
        if (!visible) {
          canvas.clear();
        }
      }
      if (visible) {
        window.onresize();
      }
    });
};
