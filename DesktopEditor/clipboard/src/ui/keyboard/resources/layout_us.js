// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview A simple English virtual keyboard implementation.
 */

/**
 * All keys for the rows of the keyboard.
 * NOTE: every row below should have an aspect of 12.6.
 * @type {Array.<Array.<BaseKey>>}
 */
var KEYS_US = [
  [
    new SvgKey('tab', 'Tab'),
    new Key(C('q'), C('Q'), C('1'), C('`')),
    new Key(C('w'), C('W'), C('2'), C('~')),
    new Key(C('e'), C('E'), C('3'), C('<', 'LessThan')),
    new Key(C('r'), C('R'), C('4'), C('>', 'GreaterThan')),
    new Key(C('t'), C('T'), C('5'), C('[')),
    new Key(C('y'), C('Y'), C('6'), C(']')),
    new Key(C('u'), C('U'), C('7'), C('{')),
    new Key(C('i'), C('I'), C('8'), C('}')),
    new Key(C('o'), C('O'), C('9'), C('\\')),
    new Key(C('p'), C('P'), C('0'), C('|')),
    new SvgKey('backspace', 'Backspace', true /* repeat */)
  ],
  [
    new SymbolKey(),
    new Key(C('a'), C('A'), C('!'), C('+')),
    new Key(C('s'), C('S'), C('@'), C('=')),
    new Key(C('d'), C('D'), C('#'), C(' ')),
    new Key(C('f'), C('F'), C('$'), C(' ')),
    new Key(C('g'), C('G'), C('%'), C(' ')),
    new Key(C('h'), C('H'), C('^'), C(' ')),
    new Key(C('j'), C('J'), C('&', 'Ampersand'), C(' ')),
    new Key(C('k'), C('K'), C('*'), C(' ')),
    new Key(C('l'), C('L'), C('('), C(' ')),
    new Key(C(';'), C('\''), C(')'), C(' ')),
    new SvgKey('return', 'Enter')
  ],
  [
    new ShiftKey('left-shift'),
    new Key(C('z'), C('Z'), C('/'), C(' ')),
    new Key(C('x'), C('X'), C('-'), C(' ')),
    new Key(C('c'), C('C'), C('\''), C(' ')),
    new Key(C('v'), C('V'), C('\"'), C(' ')),
    new Key(C('b'), C('B'), C(':'), C(' ')),
    new Key(C('n'), C('N'), C(';'), C(' ')),
    new Key(C('m'), C('M'), C('_'), C(' ')),
    new Key(C(','), C('@'), C(','), C(' ')),
    new Key(C('.'), C('!'), C('.'), C(' ')),
    new Key(C('?'), C('/'), C('?'), C(' ')),
    new ShiftKey('right-shift')
  ],
  [
    new MicKey(),
    new DotComKey(),
    new SpecialKey('space', ' ', 'Spacebar'),
    new SpecialKey('at', '@', '@'),
    new HideKeyboardKey()
  ]
];

// Add layout to KEYBOARDS, which is defined in common.js
KEYBOARDS['us'] = {
  'definition': KEYS_US,
  'aspect': 4.50
};
