// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview A simple virtual keyboard implementation.
 */

var KEY_MODE = 'key';
var SHIFT_MODE = 'shift';
var NUMBER_MODE = 'number';
var SYMBOL_MODE = 'symbol';
// TODO(bryeung): tear out all of this mode switching code
var MODES = [KEY_MODE, SHIFT_MODE, NUMBER_MODE, SYMBOL_MODE];
var currentMode = KEY_MODE;
var enterShiftModeOnSpace = false;
var MODE_CODES = {};
var MODE_TRANSITIONS = {};

MODE_CODES[KEY_MODE] = 0;
MODE_CODES[SHIFT_MODE] = 1;
MODE_CODES[NUMBER_MODE] = 2;
MODE_CODES[SYMBOL_MODE] = 3;

MODE_TRANSITIONS[KEY_MODE + SHIFT_MODE] = SHIFT_MODE;
MODE_TRANSITIONS[KEY_MODE + NUMBER_MODE] = NUMBER_MODE;
MODE_TRANSITIONS[SHIFT_MODE + SHIFT_MODE] = KEY_MODE;
MODE_TRANSITIONS[SHIFT_MODE + NUMBER_MODE] = NUMBER_MODE;
MODE_TRANSITIONS[NUMBER_MODE + SHIFT_MODE] = SYMBOL_MODE;
MODE_TRANSITIONS[NUMBER_MODE + NUMBER_MODE] = KEY_MODE;
MODE_TRANSITIONS[SYMBOL_MODE + SHIFT_MODE] = NUMBER_MODE;
MODE_TRANSITIONS[SYMBOL_MODE + NUMBER_MODE] = KEY_MODE;

var KEYBOARDS = {};

/**
 * The long-press delay in milliseconds before long-press handler is invoked.
 * @type {number}
 */
var LONGPRESS_DELAY_MSEC = 500;

/**
 * The repeat delay in milliseconds before a key starts repeating. Use the same
 * rate as Chromebook. (See chrome/browser/chromeos/language_preferences.cc)
 * @type {number}
 */
var REPEAT_DELAY_MSEC = 500;

/**
 * The repeat interval or number of milliseconds between subsequent keypresses.
 * Use the same rate as Chromebook.
 * @type {number}
 */
var REPEAT_INTERVAL_MSEC = 50;

/**
 * The keyboard layout name currently in use.
 * @type {string}
 */
var currentKeyboardLayout = 'us';

/**
 * A structure to track the currently repeating key on the keyboard.
 */
var repeatKey = {
    /**
     * The timer for the delay before repeating behaviour begins.
     * @type {number|undefined}
     */
    timer: undefined,

    /**
     * The interval timer for issuing keypresses of a repeating key.
     * @type {number|undefined}
     */
    interval: undefined,

    /**
     * The key which is currently repeating.
     * @type {BaseKey|undefined}
     */
    key: undefined,

    /**
     * Cancel the repeat timers of the currently active key.
     */
    cancel: function() {
      clearTimeout(this.timer);
      clearInterval(this.interval);
      this.timer = undefined;
      this.interval = undefined;
      this.key = undefined;
    }
};

/**
 * Set the keyboard mode.
 * @param {string} mode The new mode.
 */
function setMode(mode) {
  currentMode = mode;

  var rows = KEYBOARDS[currentKeyboardLayout]['rows'];
  for (var i = 0; i < rows.length; ++i) {
    rows[i].showMode(currentMode);
  }
}

/**
 * Transition the mode according to the given transition.
 * @param {string} transition The transition to take.
 */
function transitionMode(transition) {
  setMode(MODE_TRANSITIONS[currentMode + transition]);
}

/**
 * Send the given key to chrome, via the experimental extension API.
 * @param {string} keyIdentifier The key to send.
 */
function sendKey(keyIdentifier) {
  var keyEvent = {
    keyIdentifier: keyIdentifier
  };
  sendKeyEvent(keyEvent);

  // Exit shift mode after pressing any key but space.
  if (currentMode == SHIFT_MODE && keyIdentifier != 'Spacebar') {
    transitionMode(SHIFT_MODE);
  }
  // Enter shift mode after typing a closing punctuation and then a space for a
  // new sentence.
  if (enterShiftModeOnSpace) {
    enterShiftModeOnSpace = false;
    if (currentMode != SHIFT_MODE && keyIdentifier == 'Spacebar') {
      setMode(SHIFT_MODE);
    }
  }
  if (currentMode != SHIFT_MODE &&
      (keyIdentifier == '.' || keyIdentifier == '?' || keyIdentifier == '!')) {
    enterShiftModeOnSpace = true;
  }
}

/**
 * Add a child div element that represents the content of the given element.
 * A child div element that represents a text content is added if
 * opt_textContent is given. Otherwise a child element that represents an image
 * content is added. If the given element already has a child, the child element
 * is modified.
 * @param {Element} element The DOM Element to which the content is added.
 * @param {string} opt_textContent The text to be inserted.
 */
function addContent(element, opt_textContent) {
  if (element.childNodes.length > 0) {
    var content = element.childNodes[0];
    if (opt_textContent) {
      content.textContent = opt_textContent;
    }
    return;
  }

  var content = document.createElement('div');
  if (opt_textContent) {
    content.textContent = opt_textContent;
    content.className = 'text-key';
  } else {
    content.className = 'image-key';
  }
  element.appendChild(content);
}

/**
 * Set up the event handlers necessary to respond to mouse and touch events on
 * the virtual keyboard.
 * @param {BaseKey} key The BaseKey object corresponding to this key.
 * @param {Element} element The top-level DOM Element to set event handlers on.
 * @param {Object.<string, function()>} handlers The object that contains key
 *     event handlers in the following form.
 *
 *     { 'up': keyUpHandler,
 *       'down': keyDownHandler,
 *       'long': keyLongHandler }
 *
 *     keyDownHandler: Called when the key is pressed. This will be called
 *         repeatedly when holding a repeating key.
 *     keyUpHandler: Called when the key is released. This is only called
 *         once per actual key press.
 *     keyLongHandler: Called when the key is long-pressed for
 *         |LONGPRESS_DELAY_MSEC| milliseconds.
 *
 *     The object does not necessarily contain all the handlers above, but
 *     needs to contain at least one of them.
 */
function setupKeyEventHandlers(key, element, handlers) {
  var keyDownHandler = handlers['down'];
  var keyUpHandler = handlers['up'];
  var keyLongHandler = handlers['long'];
  if (!(keyDownHandler || keyUpHandler || keyLongPressHandler)) {
    throw new Error('Invalid handlers passed to setupKeyEventHandlers');
  }

  /**
   * Handle a key down event on the virtual key.
   * @param {UIEvent} evt The UI event which triggered the key down.
   */
  var downHandler = function(evt) {
    // Prevent any of the system gestures from happening.
    evt.preventDefault();

    // Don't process a key down if the key is already down.
    if (key.pressed) {
      return;
    }
    key.pressed = true;
    if (keyDownHandler) {
      keyDownHandler();
    }
    repeatKey.cancel();

    // Start a repeating timer if there is a repeat interval and a function to
    // process key down events.
    if (key.repeat && keyDownHandler) {
      repeatKey.key = key;
      // The timeout for the repeating timer occurs at
      // REPEAT_DELAY_MSEC - REPEAT_INTERVAL_MSEC so that the interval
      // function can handle all repeat keypresses and will get the first one
      // at the correct time.
      repeatKey.timer = setTimeout(function() {
            repeatKey.timer = undefined;
            repeatKey.interval = setInterval(function() {
                  keyDownHandler();
                }, REPEAT_INTERVAL_MSEC);
          }, Math.max(0, REPEAT_DELAY_MSEC - REPEAT_INTERVAL_MSEC));
    }

    if (keyLongHandler) {
      // Copy the currentTarget of event, which is neccessary because |evt| can
      // be modified before |keyLongHandler| is called.
      var evtCopy = {};
      evtCopy.currentTarget = evt.currentTarget;
      key.longPressTimer = setTimeout(function() {
          keyLongHandler(evtCopy),
          clearTimeout(key.longPressTimer);
          delete key.longPressTimer;
          key.pressed = false;
        }, LONGPRESS_DELAY_MSEC);
    }
  };

  /**
   * Handle a key up event on the virtual key.
   * @param {UIEvent} evt The UI event which triggered the key up.
   */
  var upHandler = function(evt) {
    // Prevent any of the system gestures from happening.
    evt.preventDefault();

    // Reset long-press timer.
    if (key.longPressTimer) {
      clearTimeout(key.longPressTimer);
      delete key.longPressTimer;
    }

    // If they key was not actually pressed do not send a key up event.
    if (!key.pressed) {
      return;
    }
    key.pressed = false;

    // Cancel running repeat timer for the released key only.
    if (repeatKey.key == key) {
      repeatKey.cancel();
    }

    if (keyUpHandler) {
      keyUpHandler();
    }
  };

  // Setup mouse event handlers.
  element.addEventListener('mousedown', downHandler);
  element.addEventListener('mouseup', upHandler);

  // Setup touch handlers.
  element.addEventListener('touchstart', downHandler);
  element.addEventListener('touchend', upHandler);
}

/**
 * Create closure for the sendKey function.
 * @param {string} key The key paramater to sendKey.
 * @return {function()} A function which calls sendKey(key).
 */
function sendKeyFunction(key) {
  return function() {
    sendKey(key);
  };
}

/**
 * Plain-old-data class to represent a character.
 * @param {string} display The HTML to be displayed.
 * @param {string} id The key identifier for this Character.
 * @constructor
 */
function Character(display, id) {
  this.display = display;
  this.keyIdentifier = id;
}

/**
 * Convenience function to make the keyboard data more readable.
 * @param {string} display The display for the created Character.
 * @param {string} opt_id The id for the created Character.
 * @return {Character} A character that contains display and opt_id. If
 *     opt_id is omitted, display is used as the id.
 */
function C(display, opt_id) {
  var id = opt_id || display;
  return new Character(display, id);
}

/**
 * An abstract base-class for all keys on the keyboard.
 * @constructor
 */
function BaseKey() {}

BaseKey.prototype = {
  /**
   * The cell type of this key.  Determines the background colour.
   * @type {string}
   */
  cellType_: '',

  /**
   * If true, holding this key will issue repeat keypresses.
   * @type {boolean}
   */
  repeat_: false,

  /**
   * Track the pressed state of the key. This is true if currently pressed.
   * @type {boolean}
   */
  pressed_: false,

  /**
   * Get the repeat behaviour of the key.
   * @return {boolean} True if the key will repeat.
   */
  get repeat() {
    return this.repeat_;
  },

  /**
   * Set the repeat behaviour of the key
   * @param {boolean} repeat True if the key should repeat.
   */
  set repeat(repeat) {
    this.repeat_ = repeat;
  },

  /**
   * Get the pressed state of the key.
   * @return {boolean} True if the key is currently pressed.
   */
  get pressed() {
    return this.pressed_;
  },

  /**
   * Set the pressed state of the key.
   * @param {boolean} pressed True if the key is currently pressed.
   */
  set pressed(pressed) {
    this.pressed_ = pressed;
  },

  /**
   * Create the DOM elements for the given keyboard mode.  Must be overridden.
   * @param {string} mode The keyboard mode to create elements for.
   * @return {Element} The top-level DOM Element for the key.
   */
  makeDOM: function(mode) {
    throw new Error('makeDOM not implemented in BaseKey');
  },
};

/**
 * A simple key which displays Characters.
 * @param {Object} key The Character for KEY_MODE.
 * @param {Object} shift The Character for SHIFT_MODE.
 * @param {Object} num The Character for NUMBER_MODE.
 * @param {Object} symbol The Character for SYMBOL_MODE.
 * @param {string} className An optional class name for the key.
 * @constructor
 * @extends {BaseKey}
 */
function Key(key, shift, num, symbol, className) {
  this.modeElements_ = {};
  this.cellType_ = '';
  this.className_ = (className) ? 'key ' + className : 'key';

  this.modes_ = {};
  this.modes_[KEY_MODE] = key;
  this.modes_[SHIFT_MODE] = shift;
  this.modes_[NUMBER_MODE] = num;
  this.modes_[SYMBOL_MODE] = symbol;
}

Key.prototype = {
  __proto__: BaseKey.prototype,

  /** @override */
  makeDOM: function(mode) {
    if (!this.modes_[mode]) {
      return null;
    }

    this.modeElements_[mode] = document.createElement('div');
    var element = this.modeElements_[mode];
    element.className = this.className_;

    addContent(element, this.modes_[mode].display);

    setupKeyEventHandlers(this, element,
        { 'up': sendKeyFunction(this.modes_[mode].keyIdentifier) });
    return element;
  }
};

/**
 * A key which displays an SVG image.
 * @param {string} className The class that provides the image.
 * @param {string} keyId The key identifier for the key.
 * @param {boolean} opt_repeat True if the key should repeat.
 * @constructor
 * @extends {BaseKey}
 */
function SvgKey(className, keyId, opt_repeat) {
  this.modeElements_ = {};
  this.cellType_ = 'nc';
  this.className_ = className;
  this.keyId_ = keyId;
  this.repeat_ = opt_repeat || false;
}

SvgKey.prototype = {
  __proto__: BaseKey.prototype,

  /** @override */
  makeDOM: function(mode) {
    this.modeElements_[mode] = document.createElement('div');
    this.modeElements_[mode].className = 'key dark';
    this.modeElements_[mode].classList.add(this.className_);
    addContent(this.modeElements_[mode]);

    // send the key event on key down if key repeat is enabled
    var handler = this.repeat_ ? { 'down' : sendKeyFunction(this.keyId_) } :
                                 { 'up' : sendKeyFunction(this.keyId_) };
    setupKeyEventHandlers(this, this.modeElements_[mode], handler);

    return this.modeElements_[mode];
  }
};

/**
 * A Key that remains the same through all modes.
 * @param {string} className The class name for the key.
 * @param {string} content The display text for the key.
 * @param {string} keyId The key identifier for the key.
 * @constructor
 * @extends {BaseKey}
 */
function SpecialKey(className, content, keyId) {
  this.modeElements_ = {};
  this.cellType_ = 'nc';
  this.content_ = content;
  this.keyId_ = keyId;
  this.className_ = className;
}

SpecialKey.prototype = {
  __proto__: BaseKey.prototype,

  /** @override */
  makeDOM: function(mode) {
    this.modeElements_[mode] = document.createElement('div');
    this.modeElements_[mode].className = 'key dark';
    this.modeElements_[mode].classList.add(this.className_);
    addContent(this.modeElements_[mode], this.content_);

    setupKeyEventHandlers(this, this.modeElements_[mode],
        { 'up': sendKeyFunction(this.keyId_) });

    return this.modeElements_[mode];
  }
};

/**
 * A shift key.
 * @constructor
 * @param {string} className The class name for the key.
 * @extends {BaseKey}
 */
function ShiftKey(className) {
  this.modeElements_ = {};
  this.cellType_ = 'nc';
  this.className_ = className;
}

ShiftKey.prototype = {
  __proto__: BaseKey.prototype,

  /** @override */
  makeDOM: function(mode) {
    this.modeElements_[mode] = document.createElement('div');
    this.modeElements_[mode].className = 'key shift dark';
    this.modeElements_[mode].classList.add(this.className_);

    if (mode == KEY_MODE || mode == SHIFT_MODE) {
      addContent(this.modeElements_[mode]);
    } else if (mode == NUMBER_MODE) {
      addContent(this.modeElements_[mode], 'more');
    } else if (mode == SYMBOL_MODE) {
      addContent(this.modeElements_[mode], '#123');
    }

    if (mode == SHIFT_MODE || mode == SYMBOL_MODE) {
      this.modeElements_[mode].classList.add('moddown');
    } else {
      this.modeElements_[mode].classList.remove('moddown');
    }

    setupKeyEventHandlers(this, this.modeElements_[mode],
        { 'down': function() {
          transitionMode(SHIFT_MODE);
        }});

    return this.modeElements_[mode];
  },
};

/**
 * The symbol key: switches the keyboard into symbol mode.
 * @constructor
 * @extends {BaseKey}
 */
function SymbolKey() {
  this.modeElements_ = {};
  this.cellType_ = 'nc';
}

SymbolKey.prototype = {
  __proto__: BaseKey.prototype,

  /** @override */
  makeDOM: function(mode, height) {
    this.modeElements_[mode] = document.createElement('div');
    this.modeElements_[mode].className = 'key symbol dark';

    if (mode == KEY_MODE || mode == SHIFT_MODE) {
      addContent(this.modeElements_[mode], '#123');
    } else if (mode == NUMBER_MODE || mode == SYMBOL_MODE) {
      addContent(this.modeElements_[mode], 'abc');
    }

    if (mode == NUMBER_MODE || mode == SYMBOL_MODE) {
      this.modeElements_[mode].classList.add('moddown');
    } else {
      this.modeElements_[mode].classList.remove('moddown');
    }

    setupKeyEventHandlers(this, this.modeElements_[mode],
        { 'down': function() {
          transitionMode(NUMBER_MODE);
        }});

    return this.modeElements_[mode];
  }
};

/**
 * The ".com" key.
 * @constructor
 * @extends {BaseKey}
 */
function DotComKey() {
  this.modeElements_ = {};
  this.cellType_ = 'nc';
}

DotComKey.prototype = {
  __proto__: BaseKey.prototype,

  /** @override */
  makeDOM: function(mode) {
    this.modeElements_[mode] = document.createElement('div');
    this.modeElements_[mode].className = 'key com dark';
    addContent(this.modeElements_[mode], '.com');

    setupKeyEventHandlers(this, this.modeElements_[mode],
        { 'up': function() {
          sendKey('.');
          sendKey('c');
          sendKey('o');
          sendKey('m');
        }});

    return this.modeElements_[mode];
  }
};

/**
 * The key that hides the keyboard.
 * @constructor
 * @extends {BaseKey}
 */
function HideKeyboardKey() {
  this.modeElements_ = {};
  this.cellType_ = 'nc';
}

HideKeyboardKey.prototype = {
  __proto__: BaseKey.prototype,

  /** @override */
  makeDOM: function(mode) {
    this.modeElements_[mode] = document.createElement('div');
    this.modeElements_[mode].className = 'key hide dark';
    addContent(this.modeElements_[mode]);

    setupKeyEventHandlers(this, this.modeElements_[mode],
        { 'down': function() { console.log('Hide the keyboard!'); } });

    return this.modeElements_[mode];
  }
};

/**
 * The mic key: activate speech input.
 * @constructor
 * @extends {BaseKey}
 */
function MicKey() {
  this.modeElements_ = {};
  this.recognition_ = new webkitSpeechRecognition();
  this.recognition_.onstart = this.onStartHandler.bind(this);
  this.recognition_.onresult = this.onResultHandler.bind(this);
  this.recognition_.onerror = this.onErrorHandler.bind(this);
  this.recognition_.onend = this.onEndHandler.bind(this);
  this.finalResult_ = '';
  this.recognizing_ = false;
  this.cellType_ = 'nc';
}

MicKey.prototype = {
  __proto__: BaseKey.prototype,

  /**
   * Event handler for mouse/touch down events.
   */
  onDown: function() {
    if (this.recognizing_) {
      this.recognition_.stop();
      return;
    }
    this.recognition_.start();
  },

  /**
   * Speech recognition started. Change mic key's icon.
   */
  onStartHandler: function() {
    this.recognizing_ = true;
    this.finalResult_ = '';
    for (var i = 0; i < MODES.length; ++i)
      this.modeElements_[MODES[i]].classList.add('start');
  },

  /**
   * Speech recognizer returns a result.
   * @param{Event} e The SpeechRecognition event that is raised each time there
   *     are any changes to interim or final results.
   */
  onResultHandler: function(e) {
    for (var i = e.resultIndex; i < e.results.length; i++) {
      if (e.results[i].isFinal)
        this.finalResult_ = e.results[i][0].transcript;
    }
    for (var i = 0; i < this.finalResult_.length; i++) {
      var keyEvent = {
        keyIdentifier: this.finalResult_.charAt(i)
      };
      sendKeyEvent(keyEvent);
    }
  },

  /**
   * Speech recognizer returns an error.
   * @param{Event} e The SpeechRecognitionError event that is raised each time
   *     there is an error.
   */
  onErrorHandler: function(e) {
    console.error('error code = ' + e.error);
  },

  /**
   * Speech recognition ended. Reset mic key's icon.
   */
  onEndHandler: function() {
   for (var i = 0; i < MODES.length; ++i)
     this.modeElements_[MODES[i]].classList.remove('start');

   this.recognizing_ = false;
  },

  /** @override */
  makeDOM: function(mode) {
    this.modeElements_[mode] = document.createElement('div');
    this.modeElements_[mode].className = 'key mic';
    addContent(this.modeElements_[mode]);

    setupKeyEventHandlers(this, this.modeElements_[mode],
        { 'down': this.onDown.bind(this) });

    return this.modeElements_[mode];
  }
};

/**
 * A container for keys.
 * @param {number} position The position of the row (0-3).
 * @param {Array.<BaseKey>} keys The keys in the row.
 * @constructor
 */
function Row(position, keys) {
  this.position_ = position;
  this.keys_ = keys;
  this.element_ = null;
  this.modeElements_ = {};
}

Row.prototype = {
  /**
   * Create the DOM elements for the row.
   * @return {Element} The top-level DOM Element for the row.
   */
  makeDOM: function() {
    this.element_ = document.createElement('div');
    this.element_.className = 'row';
    for (var i = 0; i < MODES.length; ++i) {
      var mode = MODES[i];
      this.modeElements_[mode] = document.createElement('div');
      this.modeElements_[mode].style.display = 'none';
      this.element_.appendChild(this.modeElements_[mode]);
    }

    for (var j = 0; j < this.keys_.length; ++j) {
      var key = this.keys_[j];
      for (var i = 0; i < MODES.length; ++i) {
        var keyDom = key.makeDOM(MODES[i]);
        if (keyDom) {
          this.modeElements_[MODES[i]].appendChild(keyDom);
        }
      }
    }

    for (var i = 0; i < MODES.length; ++i) {
      var clearingDiv = document.createElement('div');
      clearingDiv.style.clear = 'both';
      this.modeElements_[MODES[i]].appendChild(clearingDiv);
    }

    return this.element_;
  },

  /**
   * Shows the given mode.
   * @param {string} mode The mode to show.
   */
  showMode: function(mode) {
    for (var i = 0; i < MODES.length; ++i) {
      this.modeElements_[MODES[i]].style.display = 'none';
    }
    this.modeElements_[mode].style.display = '-webkit-box';
  },

  /**
   * Returns the size of keys this row contains.
   * @return {number} The size of keys.
   */
  get length() {
    return this.keys_.length;
  }
};
