// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/events/key_identifier_conversion.h"

#include <string.h>
#include <utility>

#include "base/basictypes.h"
#include "base/hash_tables.h"
#include "ui/base/events/event.h"
#include "ui/base/keycodes/keyboard_codes.h"

namespace {

const int kNumIdentifierTypes = 3;

typedef struct KeyIdentifier {
  // In order: key identifier, character and unicode codepoint.  They are
  // searched in that order as well.
  // These are all placed into a single array as they are treated uniformly and
  // we never refer to a specific type of identifier.  This reduces code
  // duplication below.
  const char* identifiers[kNumIdentifierTypes];
  const ui::KeyboardCode key_code;
  const int event_flags;
} KeyIdentifier;

// Taken from Section 6.3.3 here:
// http://www.w3.org/TR/DOM-Level-3-Events/#keyset-keyidentifiers
// WARNING: keycodes could be wrong, as they are based soley on keydown
const KeyIdentifier kKeyIdentifiers[] = {
  { {"Accept", "", ""}, ui::VKEY_ACCEPT, 0 },
  { {"Add", "", ""}, ui::VKEY_ADD, 0 },
  { {"Again", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"AllCandidates", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Alphanumeric", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Alt", "", ""}, ui::VKEY_MENU, 0 },
  { {"AltGraph", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Apps", "", ""}, ui::VKEY_APPS, 0 },
  { {"Attn", "", ""}, ui::VKEY_ATTN, 0 },
  { {"BrowserBack", "", ""}, ui::VKEY_BROWSER_BACK, 0 },
  { {"BrowserFavorites", "", ""}, ui::VKEY_BROWSER_FAVORITES, 0 },
  { {"BrowserForward", "", ""}, ui::VKEY_BROWSER_FORWARD, 0 },
  { {"BrowserHome", "", ""}, ui::VKEY_BROWSER_HOME, 0 },
  { {"BrowserRefresh", "", ""}, ui::VKEY_BROWSER_REFRESH, 0 },
  { {"BrowserSearch", "", ""}, ui::VKEY_BROWSER_SEARCH, 0 },
  { {"BrowserStop", "", ""}, ui::VKEY_BROWSER_STOP, 0 },
  { {"CapsLock", "", ""}, ui::VKEY_CAPITAL, 0 },
  { {"Clear", "", ""}, ui::VKEY_OEM_CLEAR, 0 },
  { {"CodeInput", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Compose", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Control", "", ""}, ui::VKEY_CONTROL, 0 },
  { {"Crsel", "", ""}, ui::VKEY_CRSEL, 0 },
  { {"Convert", "", ""}, ui::VKEY_CONVERT, 0 },
  { {"Copy", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Cut", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Decimal", "", ""}, ui::VKEY_DECIMAL, 0 },
  { {"Divide", "", ""}, ui::VKEY_DIVIDE, 0 },
  { {"Down", "", ""}, ui::VKEY_DOWN, 0 },
  { {"DownLeft", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"DownRight", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"End", "", ""}, ui::VKEY_END, 0 },
  { {"Enter", "", ""}, ui::VKEY_RETURN, 0 },
  { {"EraseEof", "", ""}, ui::VKEY_EREOF, 0 },
  { {"Execute", "", ""}, ui::VKEY_EXECUTE, 0 },
  { {"Exsel", "", ""}, ui::VKEY_EXSEL, 0 },
  { {"Fn", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"F1", "", ""}, ui::VKEY_F1, 0 },
  { {"F2", "", ""}, ui::VKEY_F2, 0 },
  { {"F3", "", ""}, ui::VKEY_F3, 0 },
  { {"F4", "", ""}, ui::VKEY_F4, 0 },
  { {"F5", "", ""}, ui::VKEY_F5, 0 },
  { {"F6", "", ""}, ui::VKEY_F6, 0 },
  { {"F7", "", ""}, ui::VKEY_F7, 0 },
  { {"F8", "", ""}, ui::VKEY_F8, 0 },
  { {"F9", "", ""}, ui::VKEY_F9, 0 },
  { {"F10", "", ""}, ui::VKEY_F10, 0 },
  { {"F11", "", ""}, ui::VKEY_F11, 0 },
  { {"F12", "", ""}, ui::VKEY_F12, 0 },
  { {"F13", "", ""}, ui::VKEY_F13, 0 },
  { {"F14", "", ""}, ui::VKEY_F14, 0 },
  { {"F15", "", ""}, ui::VKEY_F15, 0 },
  { {"F16", "", ""}, ui::VKEY_F16, 0 },
  { {"F17", "", ""}, ui::VKEY_F17, 0 },
  { {"F18", "", ""}, ui::VKEY_F18, 0 },
  { {"F19", "", ""}, ui::VKEY_F19, 0 },
  { {"F20", "", ""}, ui::VKEY_F20, 0 },
  { {"F21", "", ""}, ui::VKEY_F21, 0 },
  { {"F22", "", ""}, ui::VKEY_F22, 0 },
  { {"F23", "", ""}, ui::VKEY_F23, 0 },
  { {"F24", "", ""}, ui::VKEY_F24, 0 },
  { {"FinalMode", "", ""}, ui::VKEY_FINAL, 0 },
  { {"Find", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"FullWidth", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"HalfWidth", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"HangulMode", "", ""}, ui::VKEY_HANGUL, 0 },
  { {"HanjaMode", "", ""}, ui::VKEY_HANJA, 0 },
  { {"Help", "", ""}, ui::VKEY_HELP, 0 },
  { {"Hiragana", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Home", "", ""}, ui::VKEY_HOME, 0 },
  { {"Insert", "", ""}, ui::VKEY_INSERT, 0 },
  { {"JapaneseHiragana", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"JapaneseKatakana", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"JapaneseRomaji", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"JunjaMode", "", ""}, ui::VKEY_JUNJA, 0 },
  { {"KanaMode", "", ""}, ui::VKEY_KANA, 0 },
  { {"KanjiMode", "", ""}, ui::VKEY_KANJI, 0 },
  { {"Katakana", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"LaunchApplication1", "", ""}, ui::VKEY_MEDIA_LAUNCH_APP1, 0 },
  { {"LaunchApplication2", "", ""}, ui::VKEY_MEDIA_LAUNCH_APP2, 0 },
  { {"LaunchMail", "", ""}, ui::VKEY_MEDIA_LAUNCH_MAIL, 0 },
  { {"Left", "", ""}, ui::VKEY_LEFT, 0 },
  { {"Menu", "", ""}, ui::VKEY_MENU, 0 },
  { {"Meta", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"MediaNextTrack", "", ""}, ui::VKEY_MEDIA_NEXT_TRACK, 0 },
  { {"MediaPlayPause", "", ""}, ui::VKEY_MEDIA_PLAY_PAUSE, 0 },
  { {"MediaPreviousTrack", "", ""}, ui::VKEY_MEDIA_PREV_TRACK, 0 },
  { {"MediaStop", "", ""}, ui::VKEY_MEDIA_STOP, 0 },
  { {"ModeChange", "", ""}, ui::VKEY_MODECHANGE, 0 },
  { {"Multiply", "", ""}, ui::VKEY_MULTIPLY, 0 },
  { {"NextCandidate", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Nonconvert", "", ""}, ui::VKEY_NONCONVERT, 0 },
  { {"NumLock", "", ""}, ui::VKEY_NUMLOCK, 0 },
  { {"PageDown", "", ""}, ui::VKEY_NEXT, 0 },
  { {"PageUp", "", ""}, ui::VKEY_PRIOR, 0 },
  { {"Paste", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Pause", "", ""}, ui::VKEY_PAUSE, 0 },
  { {"Play", "", ""}, ui::VKEY_PLAY, 0 },
  { {"PreviousCandidate", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"PrintScreen", "", ""}, ui::VKEY_SNAPSHOT, 0 },
  { {"Process", "", ""}, ui::VKEY_PROCESSKEY, 0 },
  { {"Props", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Right", "", ""}, ui::VKEY_RIGHT, 0 },
  { {"RomanCharacters", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Scroll", "", ""}, ui::VKEY_SCROLL, 0 },
  { {"Select", "", ""}, ui::VKEY_SELECT, 0 },
  { {"SelectMedia", "", ""}, ui::VKEY_MEDIA_LAUNCH_MEDIA_SELECT, 0 },
  { {"Separator", "", ""}, ui::VKEY_SEPARATOR, 0 },
  { {"Shift", "", ""}, ui::VKEY_SHIFT, 0 },
  { {"Soft1", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Soft2", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Soft3", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Soft4", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Stop", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Subtract", "", ""}, ui::VKEY_SUBTRACT, 0 },
  { {"Up", "", ""}, ui::VKEY_UP, 0 },
  { {"UpLeft", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"UpRight", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"Undo", "", ""}, ui::VKEY_UNKNOWN, 0 },
  { {"VolumeDown", "", ""}, ui::VKEY_VOLUME_DOWN, 0 },
  { {"VolumeMute", "", ""}, ui::VKEY_VOLUME_MUTE, 0 },
  { {"VolumeUp", "", ""}, ui::VKEY_VOLUME_UP, 0 },
  { {"Win", "", ""}, ui::VKEY_LWIN, 0 },
  { {"Zoom", "", ""}, ui::VKEY_ZOOM, 0 },
  { {"Backspace", "", "U+0008"}, ui::VKEY_BACK, 0 },
  { {"Tab", "", "U+0009"}, ui::VKEY_TAB, 0 },
  { {"Cancel", "", "U+0018"}, ui::VKEY_UNKNOWN, 0 },
  { {"Esc", "", "U+001B"}, ui::VKEY_ESCAPE, 0 },
  { {"Spacebar", " ", "U+0020"}, ui::VKEY_SPACE, 0 },
  { {"Exclamation", "!", "U+0021"}, ui::VKEY_1, ui::EF_SHIFT_DOWN },
  { {"DoubleQuote", "\"", "U+0022"},
      ui::VKEY_OEM_7, ui::EF_SHIFT_DOWN },
  { {"Hash", "#", "U+0023"}, ui::VKEY_3, ui::EF_SHIFT_DOWN },
  { {"Dollar", "$", "U+0024"}, ui::VKEY_4, ui::EF_SHIFT_DOWN },
  { {"Ampersand", "&", "U+0026"}, ui::VKEY_7, ui::EF_SHIFT_DOWN },
  { {"Apostrophe", "\'", "U+0027"}, ui::VKEY_OEM_7, 0 },
  { {"LeftParen", "(", "U+0028"}, ui::VKEY_9, ui::EF_SHIFT_DOWN },
  { {"RightParen", ")", "U+0029"}, ui::VKEY_0, ui::EF_SHIFT_DOWN },
  { {"Asterisk", "*", "U+002A"}, ui::VKEY_8, ui::EF_SHIFT_DOWN },
  { {"Plus", "+", "U+002B"}, ui::VKEY_OEM_PLUS, ui::EF_SHIFT_DOWN },
  { {"Percent", "%", "U+0025"}, ui::VKEY_5, ui::EF_SHIFT_DOWN },
  { {"Comma", ",", "U+002C"}, ui::VKEY_OEM_COMMA, 0 },
  { {"HyphenMinus", "-", "U+002D"}, ui::VKEY_OEM_MINUS, 0 },
  { {"Period", ".", "U+002E"}, ui::VKEY_OEM_PERIOD, 0 },
  { {"Solidus", "/", "U+002F"}, ui::VKEY_OEM_2, 0 },
  { {"", "0", "U+0030"}, ui::VKEY_0, 0 },
  { {"", "1", "U+0031"}, ui::VKEY_1, 0 },
  { {"", "2", "U+0032"}, ui::VKEY_2, 0 },
  { {"", "3", "U+0033"}, ui::VKEY_3, 0 },
  { {"", "4", "U+0034"}, ui::VKEY_4, 0 },
  { {"", "5", "U+0035"}, ui::VKEY_5, 0 },
  { {"", "6", "U+0036"}, ui::VKEY_6, 0 },
  { {"", "7", "U+0037"}, ui::VKEY_7, 0 },
  { {"", "8", "U+0038"}, ui::VKEY_8, 0 },
  { {"", "9", "U+0039"}, ui::VKEY_9, 0 },
  { {"Colon", ":", "U+003A"}, ui::VKEY_OEM_1, ui::EF_SHIFT_DOWN },
  { {"Semicolon", ";", "U+003B"}, ui::VKEY_OEM_1, 0 },
  { {"LessThan", "<", "U+003C"},
      ui::VKEY_OEM_COMMA, ui::EF_SHIFT_DOWN },
  { {"Equals", "=", "U+003D"}, ui::VKEY_OEM_PLUS, 0 },
  { {"GreaterThan", ">", "U+003E"},
      ui::VKEY_OEM_PERIOD, ui::EF_SHIFT_DOWN },
  { {"QuestionMark", "?", "U+003F"},
      ui::VKEY_OEM_2, ui::EF_SHIFT_DOWN },
  { {"At", "@", "U+0040"}, ui::VKEY_2, ui::EF_SHIFT_DOWN },
  { {"", "A", "U+0041"}, ui::VKEY_A, ui::EF_SHIFT_DOWN },
  { {"", "B", "U+0042"}, ui::VKEY_B, ui::EF_SHIFT_DOWN },
  { {"", "C", "U+0043"}, ui::VKEY_C, ui::EF_SHIFT_DOWN },
  { {"", "D", "U+0044"}, ui::VKEY_D, ui::EF_SHIFT_DOWN },
  { {"", "E", "U+0045"}, ui::VKEY_E, ui::EF_SHIFT_DOWN },
  { {"", "F", "U+0046"}, ui::VKEY_F, ui::EF_SHIFT_DOWN },
  { {"", "G", "U+0047"}, ui::VKEY_G, ui::EF_SHIFT_DOWN },
  { {"", "H", "U+0048"}, ui::VKEY_H, ui::EF_SHIFT_DOWN },
  { {"", "I", "U+0049"}, ui::VKEY_I, ui::EF_SHIFT_DOWN },
  { {"", "J", "U+004A"}, ui::VKEY_J, ui::EF_SHIFT_DOWN },
  { {"", "K", "U+004B"}, ui::VKEY_K, ui::EF_SHIFT_DOWN },
  { {"", "L", "U+004C"}, ui::VKEY_L, ui::EF_SHIFT_DOWN },
  { {"", "M", "U+004D"}, ui::VKEY_M, ui::EF_SHIFT_DOWN },
  { {"", "N", "U+004E"}, ui::VKEY_N, ui::EF_SHIFT_DOWN },
  { {"", "O", "U+004F"}, ui::VKEY_O, ui::EF_SHIFT_DOWN },
  { {"", "P", "U+0050"}, ui::VKEY_P, ui::EF_SHIFT_DOWN },
  { {"", "Q", "U+0051"}, ui::VKEY_Q, ui::EF_SHIFT_DOWN },
  { {"", "R", "U+0052"}, ui::VKEY_R, ui::EF_SHIFT_DOWN },
  { {"", "S", "U+0053"}, ui::VKEY_S, ui::EF_SHIFT_DOWN },
  { {"", "T", "U+0054"}, ui::VKEY_T, ui::EF_SHIFT_DOWN },
  { {"", "U", "U+0055"}, ui::VKEY_U, ui::EF_SHIFT_DOWN },
  { {"", "V", "U+0056"}, ui::VKEY_V, ui::EF_SHIFT_DOWN },
  { {"", "W", "U+0057"}, ui::VKEY_W, ui::EF_SHIFT_DOWN },
  { {"", "X", "U+0058"}, ui::VKEY_X, ui::EF_SHIFT_DOWN },
  { {"", "Y", "U+0059"}, ui::VKEY_Y, ui::EF_SHIFT_DOWN },
  { {"", "Z", "U+005A"}, ui::VKEY_Z, ui::EF_SHIFT_DOWN },
  { {"", "a", "U+0061"}, ui::VKEY_A, 0 },
  { {"", "b", "U+0062"}, ui::VKEY_B, 0 },
  { {"", "c", "U+0063"}, ui::VKEY_C, 0 },
  { {"", "d", "U+0064"}, ui::VKEY_D, 0 },
  { {"", "e", "U+0065"}, ui::VKEY_E, 0 },
  { {"", "f", "U+0066"}, ui::VKEY_F, 0 },
  { {"", "g", "U+0067"}, ui::VKEY_G, 0 },
  { {"", "h", "U+0068"}, ui::VKEY_H, 0 },
  { {"", "i", "U+0069"}, ui::VKEY_I, 0 },
  { {"", "j", "U+006A"}, ui::VKEY_J, 0 },
  { {"", "k", "U+006B"}, ui::VKEY_K, 0 },
  { {"", "l", "U+006C"}, ui::VKEY_L, 0 },
  { {"", "m", "U+006D"}, ui::VKEY_M, 0 },
  { {"", "n", "U+006E"}, ui::VKEY_N, 0 },
  { {"", "o", "U+006F"}, ui::VKEY_O, 0 },
  { {"", "p", "U+0070"}, ui::VKEY_P, 0 },
  { {"", "q", "U+0071"}, ui::VKEY_Q, 0 },
  { {"", "r", "U+0072"}, ui::VKEY_R, 0 },
  { {"", "s", "U+0073"}, ui::VKEY_S, 0 },
  { {"", "t", "U+0074"}, ui::VKEY_T, 0 },
  { {"", "u", "U+0075"}, ui::VKEY_U, 0 },
  { {"", "v", "U+0076"}, ui::VKEY_V, 0 },
  { {"", "w", "U+0077"}, ui::VKEY_W, 0 },
  { {"", "x", "U+0078"}, ui::VKEY_X, 0 },
  { {"", "y", "U+0079"}, ui::VKEY_Y, 0 },
  { {"", "z", "U+007A"}, ui::VKEY_Z, 0 },
  { {"LeftSquareBracket", "[", "U+005B"}, ui::VKEY_OEM_4, 0 },
  { {"Backslash", "\\", "U+005C"}, ui::VKEY_OEM_5, 0 },
  { {"RightSquareBracket", "]", "U+005D"}, ui::VKEY_OEM_6, 0 },
  { {"Circumflex", "^", "U+005E"}, ui::VKEY_6, ui::EF_SHIFT_DOWN },
  { {"Underscore", "_", "U+005F"},
      ui::VKEY_OEM_MINUS, ui::EF_SHIFT_DOWN },
  { {"Grave", "`", "U+0060"}, ui::VKEY_OEM_3, 0 },
  { {"Tilde", "~", "U+007E"}, ui::VKEY_OEM_3, ui::EF_SHIFT_DOWN },
  { {"LeftCurlyBracket", "{", "U+007B"},
      ui::VKEY_OEM_4, ui::EF_SHIFT_DOWN },
  { {"Pipe", "|", "U+007C"}, ui::VKEY_OEM_5, ui::EF_SHIFT_DOWN },
  { {"RightCurlyBracket", "}", "U+007D"},
      ui::VKEY_OEM_6, ui::EF_SHIFT_DOWN },
  { {"Del", "", "U+007F"}, ui::VKEY_DELETE, 0 },
  { {"InvertedExclamation", "", "U+00A1"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadGrave", "", "U+0300"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadEacute", "", "U+0301"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadCircumflex", "", "U+0302"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadTilde", "", "U+0303"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadMacron", "", "U+0304"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadBreve", "", "U+0306"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadAboveDot", "", "U+0307"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadUmlaut", "", "U+0308"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadAboveRing", "", "U+030A"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadDoubleacute", "", "U+030B"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadCaron", "", "U+030C"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadCedilla", "", "U+0327"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadOgonek", "", "U+0328"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadIota", "", "U+0345"}, ui::VKEY_UNKNOWN, 0 },
  { {"Euro", "", "U+20AC"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadVoicedSound", "", "U+3099"}, ui::VKEY_UNKNOWN, 0 },
  { {"DeadSemivoicedSound", "", "U+309A"}, ui::VKEY_UNKNOWN, 0 }
};

const int kNumKeyIdentifiers = arraysize(kKeyIdentifiers);

typedef base::hash_map<std::string, const ui::KeyEvent*> IdentifierMap;
typedef std::pair<std::string, const ui::KeyEvent*> IdentifierPair;
IdentifierMap* identifierMaps[kNumIdentifierTypes] = { NULL };

ui::KeyEvent* kUnknownKeyEvent = NULL;

void InitializeMaps() {
  if (identifierMaps[0])
    return;

  kUnknownKeyEvent = new ui::KeyEvent(ui::ET_KEY_PRESSED,
                                      ui::VKEY_UNKNOWN,
                                      0,
                                      false);

  for (int i = 0; i < kNumIdentifierTypes; ++i)
    identifierMaps[i] = new IdentifierMap;

  for (int i = 0; i < kNumKeyIdentifiers; ++i) {
    const KeyIdentifier& key = kKeyIdentifiers[i];

    ui::KeyEvent* event = new ui::KeyEvent(ui::ET_KEY_PRESSED,
                                           key.key_code,
                                           key.event_flags,
                                           false);
    for (int j = 0; j < kNumIdentifierTypes; ++j) {
      if (key.identifiers[j][0] != '\0') {
        std::pair<IdentifierMap::iterator, bool> result =
            identifierMaps[j]->insert(
                IdentifierPair(key.identifiers[j], event));
        DCHECK(result.second);
      }
    }
  }
}

}  // namespace

namespace ui {

const ui::KeyEvent& KeyEventFromKeyIdentifier(
    const std::string& key_identifier) {
  // This lazily initializes lookup tables for the conversion.  To prevent
  // races, it should only ever be called from a single thread.
  InitializeMaps();

  for (int i = 0; i < kNumIdentifierTypes; ++i) {
    const IdentifierMap& map = *identifierMaps[i];

    IdentifierMap::const_iterator iter = map.find(key_identifier);
    if (iter != map.end())
      return *iter->second;
  }

  return *kUnknownKeyEvent;
}

}  // namespace ui
