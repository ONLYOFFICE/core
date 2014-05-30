// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/render_text.h"

#include "base/memory/scoped_ptr.h"
#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/break_list.h"

#if defined(OS_WIN)
#include "base/win/windows_version.h"
#endif

#if defined(OS_LINUX)
#include "ui/gfx/render_text_linux.h"
#endif

#if defined(TOOLKIT_GTK)
#include <gtk/gtk.h>
#endif

namespace gfx {

namespace {

// Various weak, LTR, RTL, and Bidi string cases with three characters each.
const wchar_t kWeak[] =      L" . ";
const wchar_t kLtr[] =       L"abc";
const wchar_t kLtrRtl[] =    L"a"L"\x5d0\x5d1";
const wchar_t kLtrRtlLtr[] = L"a"L"\x5d1"L"b";
const wchar_t kRtl[] =       L"\x5d0\x5d1\x5d2";
const wchar_t kRtlLtr[] =    L"\x5d0\x5d1"L"a";
const wchar_t kRtlLtrRtl[] = L"\x5d0"L"a"L"\x5d1";

// Checks whether |range| contains |index|. This is not the same as calling
// |range.Contains(ui::Range(index))| - as that would return true when
// |index| == |range.end()|.
bool IndexInRange(const ui::Range& range, size_t index) {
  return index >= range.start() && index < range.end();
}

#if !defined(OS_MACOSX)
// A test utility function to set the application default text direction.
void SetRTL(bool rtl) {
  // Override the current locale/direction.
  base::i18n::SetICUDefaultLocale(rtl ? "he" : "en");
#if defined(TOOLKIT_GTK)
  // Do the same for GTK, which does not rely on the ICU default locale.
  gtk_widget_set_default_direction(rtl ? GTK_TEXT_DIR_RTL : GTK_TEXT_DIR_LTR);
#endif
  EXPECT_EQ(rtl, base::i18n::IsRTL());
}
#endif

}  // namespace

class RenderTextTest : public testing::Test {
};

TEST_F(RenderTextTest, DefaultStyle) {
  // Check the default styles applied to new instances and adjusted text.
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  EXPECT_TRUE(render_text->text().empty());
  const wchar_t* const cases[] = { kWeak, kLtr, L"Hello", kRtl, L"", L"" };
  for (size_t i = 0; i < arraysize(cases); ++i) {
    EXPECT_TRUE(render_text->colors().EqualsValueForTesting(SK_ColorBLACK));
    for (size_t style = 0; style < NUM_TEXT_STYLES; ++style)
      EXPECT_TRUE(render_text->styles()[style].EqualsValueForTesting(false));
    render_text->SetText(WideToUTF16(cases[i]));
  }
}

TEST_F(RenderTextTest, SetColorAndStyle) {
  // Ensure custom default styles persist across setting and clearing text.
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  const SkColor color = SK_ColorRED;
  render_text->SetColor(color);
  render_text->SetStyle(BOLD, true);
  render_text->SetStyle(UNDERLINE, false);
  const wchar_t* const cases[] = { kWeak, kLtr, L"Hello", kRtl, L"", L"" };
  for (size_t i = 0; i < arraysize(cases); ++i) {
    EXPECT_TRUE(render_text->colors().EqualsValueForTesting(color));
    EXPECT_TRUE(render_text->styles()[BOLD].EqualsValueForTesting(true));
    EXPECT_TRUE(render_text->styles()[UNDERLINE].EqualsValueForTesting(false));
    render_text->SetText(WideToUTF16(cases[i]));

    // Ensure custom default styles can be applied after text has been set.
    if (i == 1)
      render_text->SetStyle(STRIKE, true);
    if (i >= 1)
      EXPECT_TRUE(render_text->styles()[STRIKE].EqualsValueForTesting(true));
  }
}

TEST_F(RenderTextTest, ApplyColorAndStyle) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(ASCIIToUTF16("012345678"));

  // Apply a ranged color and style and check the resulting breaks.
  render_text->ApplyColor(SK_ColorRED, ui::Range(1, 4));
  render_text->ApplyStyle(BOLD, true, ui::Range(2, 5));
  std::vector<std::pair<size_t, SkColor> > expected_color;
  expected_color.push_back(std::pair<size_t, SkColor>(0, SK_ColorBLACK));
  expected_color.push_back(std::pair<size_t, SkColor>(1, SK_ColorRED));
  expected_color.push_back(std::pair<size_t, SkColor>(4, SK_ColorBLACK));
  EXPECT_TRUE(render_text->colors().EqualsForTesting(expected_color));
  std::vector<std::pair<size_t, bool> > expected_style;
  expected_style.push_back(std::pair<size_t, bool>(0, false));
  expected_style.push_back(std::pair<size_t, bool>(2, true));
  expected_style.push_back(std::pair<size_t, bool>(5, false));
  EXPECT_TRUE(render_text->styles()[BOLD].EqualsForTesting(expected_style));

  // Ensure setting a color and style overrides the ranged colors and styles.
  render_text->SetColor(SK_ColorBLUE);
  EXPECT_TRUE(render_text->colors().EqualsValueForTesting(SK_ColorBLUE));
  render_text->SetStyle(BOLD, false);
  EXPECT_TRUE(render_text->styles()[BOLD].EqualsValueForTesting(false));

  // Apply a color and style over the text end and check the resulting breaks.
  // (INT_MAX should be used instead of the text length for the range end)
  const size_t text_length = render_text->text().length();
  render_text->ApplyColor(SK_ColorRED, ui::Range(0, text_length));
  render_text->ApplyStyle(BOLD, true, ui::Range(2, text_length));
  std::vector<std::pair<size_t, SkColor> > expected_color_end;
  expected_color_end.push_back(std::pair<size_t, SkColor>(0, SK_ColorRED));
  EXPECT_TRUE(render_text->colors().EqualsForTesting(expected_color_end));
  std::vector<std::pair<size_t, bool> > expected_style_end;
  expected_style_end.push_back(std::pair<size_t, bool>(0, false));
  expected_style_end.push_back(std::pair<size_t, bool>(2, true));
  EXPECT_TRUE(render_text->styles()[BOLD].EqualsForTesting(expected_style_end));

  // Ensure ranged values adjust to accommodate text length changes.
  render_text->ApplyStyle(ITALIC, true, ui::Range(0, 2));
  render_text->ApplyStyle(ITALIC, true, ui::Range(3, 6));
  render_text->ApplyStyle(ITALIC, true, ui::Range(7, text_length));
  std::vector<std::pair<size_t, bool> > expected_italic;
  expected_italic.push_back(std::pair<size_t, bool>(0, true));
  expected_italic.push_back(std::pair<size_t, bool>(2, false));
  expected_italic.push_back(std::pair<size_t, bool>(3, true));
  expected_italic.push_back(std::pair<size_t, bool>(6, false));
  expected_italic.push_back(std::pair<size_t, bool>(7, true));
  EXPECT_TRUE(render_text->styles()[ITALIC].EqualsForTesting(expected_italic));

  // Truncating the text should trim any corresponding breaks.
  render_text->SetText(ASCIIToUTF16("0123456"));
  expected_italic.resize(4);
  EXPECT_TRUE(render_text->styles()[ITALIC].EqualsForTesting(expected_italic));
  render_text->SetText(ASCIIToUTF16("01234"));
  expected_italic.resize(3);
  EXPECT_TRUE(render_text->styles()[ITALIC].EqualsForTesting(expected_italic));

  // Appending text should extend the terminal styles without changing breaks.
  render_text->SetText(ASCIIToUTF16("012345678"));
  EXPECT_TRUE(render_text->styles()[ITALIC].EqualsForTesting(expected_italic));
}

#if defined(OS_LINUX)
TEST_F(RenderTextTest, PangoAttributes) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(ASCIIToUTF16("012345678"));

  // Apply ranged BOLD/ITALIC styles and check the resulting Pango attributes.
  render_text->ApplyStyle(BOLD, true, ui::Range(2, 4));
  render_text->ApplyStyle(ITALIC, true, ui::Range(1, 3));

  struct {
    int start;
    int end;
    bool bold;
    bool italic;
  } cases[] = {
    { 0, 1,       false, false },
    { 1, 2,       false, true  },
    { 2, 3,       true,  true  },
    { 3, 4,       true,  false },
    { 4, INT_MAX, false, false },
  };

  int start = 0, end = 0;
  RenderTextLinux* rt_linux = static_cast<RenderTextLinux*>(render_text.get());
  rt_linux->EnsureLayout();
  PangoAttrList* attributes = pango_layout_get_attributes(rt_linux->layout_);
  PangoAttrIterator* iter = pango_attr_list_get_iterator(attributes);
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    pango_attr_iterator_range(iter, &start, &end);
    EXPECT_EQ(cases[i].start, start);
    EXPECT_EQ(cases[i].end, end);
    PangoFontDescription* font = pango_font_description_new();
    pango_attr_iterator_get_font(iter, font, NULL, NULL);
    char* description_string = pango_font_description_to_string(font);
    const string16 desc = ASCIIToUTF16(description_string);
    const bool bold = desc.find(ASCIIToUTF16("Bold")) != std::string::npos;
    EXPECT_EQ(cases[i].bold, bold);
    const bool italic = desc.find(ASCIIToUTF16("Italic")) != std::string::npos;
    EXPECT_EQ(cases[i].italic, italic);
    pango_attr_iterator_next(iter);
    pango_font_description_free(font);
    g_free(description_string);
  }
  EXPECT_FALSE(pango_attr_iterator_next(iter));
  pango_attr_iterator_destroy(iter);
}
#endif

// TODO(asvitkine): Cursor movements tests disabled on Mac because RenderTextMac
//                  does not implement this yet. http://crbug.com/131618
#if !defined(OS_MACOSX)
void TestVisualCursorMotionInObscuredField(RenderText* render_text,
                                           const string16& text,
                                           bool select) {
  ASSERT_TRUE(render_text->obscured());
  render_text->SetText(text);
  int len = text.length();
  render_text->MoveCursor(LINE_BREAK, CURSOR_RIGHT, select);
  EXPECT_EQ(SelectionModel(ui::Range(select ? 0 : len, len), CURSOR_FORWARD),
            render_text->selection_model());
  render_text->MoveCursor(LINE_BREAK, CURSOR_LEFT, select);
  EXPECT_EQ(SelectionModel(0, CURSOR_BACKWARD), render_text->selection_model());
  for (int j = 1; j <= len; ++j) {
    render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, select);
    EXPECT_EQ(SelectionModel(ui::Range(select ? 0 : j, j), CURSOR_BACKWARD),
              render_text->selection_model());
  }
  for (int j = len - 1; j >= 0; --j) {
    render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, select);
    EXPECT_EQ(SelectionModel(ui::Range(select ? 0 : j, j), CURSOR_FORWARD),
              render_text->selection_model());
  }
  render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, select);
  EXPECT_EQ(SelectionModel(ui::Range(select ? 0 : len, len), CURSOR_FORWARD),
            render_text->selection_model());
  render_text->MoveCursor(WORD_BREAK, CURSOR_LEFT, select);
  EXPECT_EQ(SelectionModel(0, CURSOR_BACKWARD), render_text->selection_model());
}

TEST_F(RenderTextTest, ObscuredText) {
  const string16 seuss = ASCIIToUTF16("hop on pop");
  const string16 no_seuss = ASCIIToUTF16("**********");
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());

  // GetLayoutText() returns asterisks when the obscured bit is set.
  render_text->SetText(seuss);
  render_text->SetObscured(true);
  EXPECT_EQ(seuss, render_text->text());
  EXPECT_EQ(no_seuss, render_text->GetLayoutText());
  render_text->SetObscured(false);
  EXPECT_EQ(seuss, render_text->text());
  EXPECT_EQ(seuss, render_text->GetLayoutText());

  render_text->SetObscured(true);

  // Surrogate pairs are counted as one code point.
  const char16 invalid_surrogates[] = {0xDC00, 0xD800, 0};
  render_text->SetText(invalid_surrogates);
  EXPECT_EQ(ASCIIToUTF16("**"), render_text->GetLayoutText());
  const char16 valid_surrogates[] = {0xD800, 0xDC00, 0};
  render_text->SetText(valid_surrogates);
  EXPECT_EQ(ASCIIToUTF16("*"), render_text->GetLayoutText());
  EXPECT_EQ(0U, render_text->cursor_position());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(2U, render_text->cursor_position());

  // Test index conversion and cursor validity with a valid surrogate pair.
  EXPECT_EQ(0U, render_text->TextIndexToLayoutIndex(0U));
  EXPECT_EQ(1U, render_text->TextIndexToLayoutIndex(1U));
  EXPECT_EQ(1U, render_text->TextIndexToLayoutIndex(2U));
  EXPECT_EQ(0U, render_text->LayoutIndexToTextIndex(0U));
  EXPECT_EQ(2U, render_text->LayoutIndexToTextIndex(1U));
  EXPECT_TRUE(render_text->IsCursorablePosition(0U));
  EXPECT_FALSE(render_text->IsCursorablePosition(1U));
  EXPECT_TRUE(render_text->IsCursorablePosition(2U));

  // FindCursorPosition() should not return positions between a surrogate pair.
  render_text->SetDisplayRect(Rect(0, 0, 20, 20));
  EXPECT_EQ(render_text->FindCursorPosition(Point(0, 0)).caret_pos(), 0U);
  EXPECT_EQ(render_text->FindCursorPosition(Point(20, 0)).caret_pos(), 2U);
  for (int x = -1; x <= 20; ++x) {
    SelectionModel selection = render_text->FindCursorPosition(Point(x, 0));
    EXPECT_TRUE(selection.caret_pos() == 0U || selection.caret_pos() == 2U);
  }

  // GetGlyphBounds() should yield the entire string bounds for text index 0.
  int height = 0;
  ui::Range bounds;
  render_text->GetGlyphBounds(0U, &bounds, &height);
  EXPECT_EQ(render_text->GetStringSize().width(),
            static_cast<int>(bounds.length()));

  // Cursoring is independent of underlying characters when text is obscured.
  const wchar_t* const texts[] = {
    kWeak, kLtr, kLtrRtl, kLtrRtlLtr, kRtl, kRtlLtr, kRtlLtrRtl,
    L"hop on pop",                              // Check LTR word boundaries.
    L"\x05d0\x05d1 \x05d0\x05d2 \x05d1\x05d2",  // Check RTL word boundaries.
  };
  for (size_t i = 0; i < arraysize(texts); ++i) {
    string16 text = WideToUTF16(texts[i]);
    TestVisualCursorMotionInObscuredField(render_text.get(), text, false);
    TestVisualCursorMotionInObscuredField(render_text.get(), text, true);
  }
}

TEST_F(RenderTextTest, GetTextDirection) {
  struct {
    const wchar_t* text;
    const base::i18n::TextDirection text_direction;
  } cases[] = {
    // Blank strings and those with no/weak directionality default to LTR.
    { L"",        base::i18n::LEFT_TO_RIGHT },
    { kWeak,      base::i18n::LEFT_TO_RIGHT },
    // Strings that begin with strong LTR characters.
    { kLtr,       base::i18n::LEFT_TO_RIGHT },
    { kLtrRtl,    base::i18n::LEFT_TO_RIGHT },
    { kLtrRtlLtr, base::i18n::LEFT_TO_RIGHT },
    // Strings that begin with strong RTL characters.
    { kRtl,       base::i18n::RIGHT_TO_LEFT },
    { kRtlLtr,    base::i18n::RIGHT_TO_LEFT },
    { kRtlLtrRtl, base::i18n::RIGHT_TO_LEFT },
  };

  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  const bool was_rtl = base::i18n::IsRTL();

  for (size_t i = 0; i < 2; ++i) {
    // Toggle the application default text direction (to try each direction).
    SetRTL(!base::i18n::IsRTL());
    const base::i18n::TextDirection ui_direction = base::i18n::IsRTL() ?
        base::i18n::RIGHT_TO_LEFT : base::i18n::LEFT_TO_RIGHT;

    // Ensure that directionality modes yield the correct text directions.
    for (size_t j = 0; j < ARRAYSIZE_UNSAFE(cases); j++) {
      render_text->SetText(WideToUTF16(cases[j].text));
      render_text->SetDirectionalityMode(DIRECTIONALITY_FROM_TEXT);
      EXPECT_EQ(render_text->GetTextDirection(), cases[j].text_direction);
      render_text->SetDirectionalityMode(DIRECTIONALITY_FROM_UI);
      EXPECT_EQ(render_text->GetTextDirection(), ui_direction);
      render_text->SetDirectionalityMode(DIRECTIONALITY_FORCE_LTR);
      EXPECT_EQ(render_text->GetTextDirection(), base::i18n::LEFT_TO_RIGHT);
      render_text->SetDirectionalityMode(DIRECTIONALITY_FORCE_RTL);
      EXPECT_EQ(render_text->GetTextDirection(), base::i18n::RIGHT_TO_LEFT);
    }
  }

  EXPECT_EQ(was_rtl, base::i18n::IsRTL());

  // Ensure that text changes update the direction for DIRECTIONALITY_FROM_TEXT.
  render_text->SetDirectionalityMode(DIRECTIONALITY_FROM_TEXT);
  render_text->SetText(WideToUTF16(kLtr));
  EXPECT_EQ(render_text->GetTextDirection(), base::i18n::LEFT_TO_RIGHT);
  render_text->SetText(WideToUTF16(kRtl));
  EXPECT_EQ(render_text->GetTextDirection(), base::i18n::RIGHT_TO_LEFT);
}

void RunMoveCursorLeftRightTest(RenderText* render_text,
    const std::vector<SelectionModel>& expected,
    VisualCursorDirection direction) {
  for (size_t i = 0; i < expected.size(); ++i) {
    EXPECT_EQ(expected[i], render_text->selection_model());
    render_text->MoveCursor(CHARACTER_BREAK, direction, false);
  }
  // Check that cursoring is clamped at the line edge.
  EXPECT_EQ(expected.back(), render_text->selection_model());
  // Check that it is the line edge.
  render_text->MoveCursor(LINE_BREAK, direction, false);
  EXPECT_EQ(expected.back(), render_text->selection_model());
}

TEST_F(RenderTextTest, MoveCursorLeftRightInLtr) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());

  // Pure LTR.
  render_text->SetText(ASCIIToUTF16("abc"));
  // |expected| saves the expected SelectionModel when moving cursor from left
  // to right.
  std::vector<SelectionModel> expected;
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(1, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(2, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(3, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_RIGHT);

  expected.clear();
  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  expected.push_back(SelectionModel(2, CURSOR_FORWARD));
  expected.push_back(SelectionModel(1, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_LEFT);
}

TEST_F(RenderTextTest, MoveCursorLeftRightInLtrRtl) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  // LTR-RTL
  render_text->SetText(WideToUTF16(L"abc\x05d0\x05d1\x05d2"));
  // The last one is the expected END position.
  std::vector<SelectionModel> expected;
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(1, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(2, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(3, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(5, CURSOR_FORWARD));
  expected.push_back(SelectionModel(4, CURSOR_FORWARD));
  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  expected.push_back(SelectionModel(6, CURSOR_FORWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_RIGHT);

  expected.clear();
  expected.push_back(SelectionModel(6, CURSOR_FORWARD));
  expected.push_back(SelectionModel(4, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(5, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(6, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(2, CURSOR_FORWARD));
  expected.push_back(SelectionModel(1, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_LEFT);
}

TEST_F(RenderTextTest, MoveCursorLeftRightInLtrRtlLtr) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  // LTR-RTL-LTR.
  render_text->SetText(WideToUTF16(L"a"L"\x05d1"L"b"));
  std::vector<SelectionModel> expected;
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(1, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(1, CURSOR_FORWARD));
  expected.push_back(SelectionModel(3, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_RIGHT);

  expected.clear();
  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  expected.push_back(SelectionModel(2, CURSOR_FORWARD));
  expected.push_back(SelectionModel(2, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(0, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_LEFT);
}

TEST_F(RenderTextTest, MoveCursorLeftRightInRtl) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  // Pure RTL.
  render_text->SetText(WideToUTF16(L"\x05d0\x05d1\x05d2"));
  render_text->MoveCursor(LINE_BREAK, CURSOR_RIGHT, false);
  std::vector<SelectionModel> expected;

  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(1, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(2, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(3, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_LEFT);

  expected.clear();

  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  expected.push_back(SelectionModel(2, CURSOR_FORWARD));
  expected.push_back(SelectionModel(1, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_RIGHT);
}

TEST_F(RenderTextTest, MoveCursorLeftRightInRtlLtr) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  // RTL-LTR
  render_text->SetText(WideToUTF16(L"\x05d0\x05d1\x05d2"L"abc"));
  render_text->MoveCursor(LINE_BREAK, CURSOR_RIGHT, false);
  std::vector<SelectionModel> expected;
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(1, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(2, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(3, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(5, CURSOR_FORWARD));
  expected.push_back(SelectionModel(4, CURSOR_FORWARD));
  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  expected.push_back(SelectionModel(6, CURSOR_FORWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_LEFT);

  expected.clear();
  expected.push_back(SelectionModel(6, CURSOR_FORWARD));
  expected.push_back(SelectionModel(4, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(5, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(6, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(2, CURSOR_FORWARD));
  expected.push_back(SelectionModel(1, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_RIGHT);
}

TEST_F(RenderTextTest, MoveCursorLeftRightInRtlLtrRtl) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  // RTL-LTR-RTL.
  render_text->SetText(WideToUTF16(L"\x05d0"L"a"L"\x05d1"));
  render_text->MoveCursor(LINE_BREAK, CURSOR_RIGHT, false);
  std::vector<SelectionModel> expected;
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(1, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(1, CURSOR_FORWARD));
  expected.push_back(SelectionModel(3, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_LEFT);

  expected.clear();
  expected.push_back(SelectionModel(3, CURSOR_FORWARD));
  expected.push_back(SelectionModel(2, CURSOR_FORWARD));
  expected.push_back(SelectionModel(2, CURSOR_BACKWARD));
  expected.push_back(SelectionModel(0, CURSOR_FORWARD));
  expected.push_back(SelectionModel(0, CURSOR_BACKWARD));
  RunMoveCursorLeftRightTest(render_text.get(), expected, CURSOR_RIGHT);
}

// TODO(xji): temporarily disable in platform Win since the complex script
// characters turned into empty square due to font regression. So, not able
// to test 2 characters belong to the same grapheme.
#if defined(OS_LINUX)
TEST_F(RenderTextTest, MoveCursorLeftRight_ComplexScript) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());

  render_text->SetText(WideToUTF16(L"\x0915\x093f\x0915\x094d\x0915"));
  EXPECT_EQ(0U, render_text->cursor_position());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(2U, render_text->cursor_position());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(4U, render_text->cursor_position());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(5U, render_text->cursor_position());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(5U, render_text->cursor_position());

  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(4U, render_text->cursor_position());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(2U, render_text->cursor_position());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(0U, render_text->cursor_position());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(0U, render_text->cursor_position());
}
#endif

TEST_F(RenderTextTest, GraphemePositions) {
  // LTR 2-character grapheme, LTR abc, LTR 2-character grapheme.
  const string16 kText1 = WideToUTF16(L"\x0915\x093f"L"abc"L"\x0915\x093f");

  // LTR ab, LTR 2-character grapheme, LTR cd.
  const string16 kText2 = WideToUTF16(L"ab"L"\x0915\x093f"L"cd");

  // The below is 'MUSICAL SYMBOL G CLEF', which is represented in UTF-16 as
  // two characters forming the surrogate pair 0x0001D11E.
  const std::string kSurrogate = "\xF0\x9D\x84\x9E";

  // LTR ab, UTF16 surrogate pair, LTR cd.
  const string16 kText3 = UTF8ToUTF16("ab" + kSurrogate + "cd");

  struct {
    string16 text;
    size_t index;
    size_t expected_previous;
    size_t expected_next;
  } cases[] = {
    { string16(), 0, 0, 0 },
    { string16(), 1, 0, 0 },
    { string16(), 50, 0, 0 },
    { kText1, 0, 0, 2 },
    { kText1, 1, 0, 2 },
    { kText1, 2, 0, 3 },
    { kText1, 3, 2, 4 },
    { kText1, 4, 3, 5 },
    { kText1, 5, 4, 7 },
    { kText1, 6, 5, 7 },
    { kText1, 7, 5, 7 },
    { kText1, 8, 7, 7 },
    { kText1, 50, 7, 7 },
    { kText2, 0, 0, 1 },
    { kText2, 1, 0, 2 },
    { kText2, 2, 1, 4 },
    { kText2, 3, 2, 4 },
    { kText2, 4, 2, 5 },
    { kText2, 5, 4, 6 },
    { kText2, 6, 5, 6 },
    { kText2, 7, 6, 6 },
    { kText2, 50, 6, 6 },
    { kText3, 0, 0, 1 },
    { kText3, 1, 0, 2 },
    { kText3, 2, 1, 4 },
    { kText3, 3, 2, 4 },
    { kText3, 4, 2, 5 },
    { kText3, 5, 4, 6 },
    { kText3, 6, 5, 6 },
    { kText3, 7, 6, 6 },
    { kText3, 50, 6, 6 },
  };

  // TODO(asvitkine): Disable tests that fail on XP bots due to lack of complete
  //                  font support for some scripts - http://crbug.com/106450
#if defined(OS_WIN)
  if (base::win::GetVersion() < base::win::VERSION_VISTA)
    return;
#endif

  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); i++) {
    render_text->SetText(cases[i].text);

    size_t next = render_text->IndexOfAdjacentGrapheme(cases[i].index,
                                                       CURSOR_FORWARD);
    EXPECT_EQ(cases[i].expected_next, next);
    EXPECT_TRUE(render_text->IsCursorablePosition(next));

    size_t previous = render_text->IndexOfAdjacentGrapheme(cases[i].index,
                                                           CURSOR_BACKWARD);
    EXPECT_EQ(cases[i].expected_previous, previous);
    EXPECT_TRUE(render_text->IsCursorablePosition(previous));
  }
}

TEST_F(RenderTextTest, EdgeSelectionModels) {
  // Simple Latin text.
  const string16 kLatin = WideToUTF16(L"abc");
  // LTR 2-character grapheme.
  const string16 kLTRGrapheme = WideToUTF16(L"\x0915\x093f");
  // LTR 2-character grapheme, LTR a, LTR 2-character grapheme.
  const string16 kHindiLatin = WideToUTF16(L"\x0915\x093f"L"a"L"\x0915\x093f");
  // RTL 2-character grapheme.
  const string16 kRTLGrapheme = WideToUTF16(L"\x05e0\x05b8");
  // RTL 2-character grapheme, LTR a, RTL 2-character grapheme.
  const string16 kHebrewLatin = WideToUTF16(L"\x05e0\x05b8"L"a"L"\x05e0\x05b8");

  struct {
    string16 text;
    base::i18n::TextDirection expected_text_direction;
  } cases[] = {
    { string16(),   base::i18n::LEFT_TO_RIGHT },
    { kLatin,       base::i18n::LEFT_TO_RIGHT },
    { kLTRGrapheme, base::i18n::LEFT_TO_RIGHT },
    { kHindiLatin,  base::i18n::LEFT_TO_RIGHT },
    { kRTLGrapheme, base::i18n::RIGHT_TO_LEFT },
    { kHebrewLatin, base::i18n::RIGHT_TO_LEFT },
  };

  // TODO(asvitkine): Disable tests that fail on XP bots due to lack of complete
  //                  font support for some scripts - http://crbug.com/106450
#if defined(OS_WIN)
  if (base::win::GetVersion() < base::win::VERSION_VISTA)
    return;
#endif

  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); i++) {
    render_text->SetText(cases[i].text);
    bool ltr = (cases[i].expected_text_direction == base::i18n::LEFT_TO_RIGHT);

    SelectionModel start_edge =
        render_text->EdgeSelectionModel(ltr ? CURSOR_LEFT : CURSOR_RIGHT);
    EXPECT_EQ(start_edge, SelectionModel(0, CURSOR_BACKWARD));

    SelectionModel end_edge =
        render_text->EdgeSelectionModel(ltr ? CURSOR_RIGHT : CURSOR_LEFT);
    EXPECT_EQ(end_edge, SelectionModel(cases[i].text.length(), CURSOR_FORWARD));
  }
}

TEST_F(RenderTextTest, SelectAll) {
  const wchar_t* const cases[] =
      { kWeak, kLtr, kLtrRtl, kLtrRtlLtr, kRtl, kRtlLtr, kRtlLtrRtl };

  // Ensure that SelectAll respects the |reversed| argument regardless of
  // application locale and text content directionality.
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  const SelectionModel expected_reversed(ui::Range(3, 0), CURSOR_FORWARD);
  const SelectionModel expected_forwards(ui::Range(0, 3), CURSOR_BACKWARD);
  const bool was_rtl = base::i18n::IsRTL();

  for (size_t i = 0; i < 2; ++i) {
    SetRTL(!base::i18n::IsRTL());
    // Test that an empty string produces an empty selection model.
    render_text->SetText(string16());
    EXPECT_EQ(render_text->selection_model(), SelectionModel());

    // Test the weak, LTR, RTL, and Bidi string cases.
    for (size_t j = 0; j < ARRAYSIZE_UNSAFE(cases); j++) {
      render_text->SetText(WideToUTF16(cases[j]));
      render_text->SelectAll(false);
      EXPECT_EQ(render_text->selection_model(), expected_forwards);
      render_text->SelectAll(true);
      EXPECT_EQ(render_text->selection_model(), expected_reversed);
    }
  }

  EXPECT_EQ(was_rtl, base::i18n::IsRTL());
}

  TEST_F(RenderTextTest, MoveCursorLeftRightWithSelection) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(WideToUTF16(L"abc\x05d0\x05d1\x05d2"));
  // Left arrow on select ranging (6, 4).
  render_text->MoveCursor(LINE_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(ui::Range(6), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(ui::Range(4), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(ui::Range(5), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(ui::Range(6), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, true);
  EXPECT_EQ(ui::Range(6, 5), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, true);
  EXPECT_EQ(ui::Range(6, 4), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(ui::Range(6), render_text->selection());

  // Right arrow on select ranging (4, 6).
  render_text->MoveCursor(LINE_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(ui::Range(0), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(ui::Range(1), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(ui::Range(2), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(ui::Range(3), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(ui::Range(5), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(ui::Range(4), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, true);
  EXPECT_EQ(ui::Range(4, 5), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, true);
  EXPECT_EQ(ui::Range(4, 6), render_text->selection());
  render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(ui::Range(4), render_text->selection());
}
#endif  // !defined(OS_MACOSX)

// TODO(xji): Make these work on Windows.
#if defined(OS_LINUX)
void MoveLeftRightByWordVerifier(RenderText* render_text,
                                 const wchar_t* str) {
  render_text->SetText(WideToUTF16(str));

  // Test moving by word from left ro right.
  render_text->MoveCursor(LINE_BREAK, CURSOR_LEFT, false);
  bool first_word = true;
  while (true) {
    // First, test moving by word from a word break position, such as from
    // "|abc def" to "abc| def".
    SelectionModel start = render_text->selection_model();
    render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
    SelectionModel end = render_text->selection_model();
    if (end == start)  // reach the end.
      break;

    // For testing simplicity, each word is a 3-character word.
    int num_of_character_moves = first_word ? 3 : 4;
    first_word = false;
    render_text->MoveCursorTo(start);
    for (int j = 0; j < num_of_character_moves; ++j)
      render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
    EXPECT_EQ(end, render_text->selection_model());

    // Then, test moving by word from positions inside the word, such as from
    // "a|bc def" to "abc| def", and from "ab|c def" to "abc| def".
    for (int j = 1; j < num_of_character_moves; ++j) {
      render_text->MoveCursorTo(start);
      for (int k = 0; k < j; ++k)
        render_text->MoveCursor(CHARACTER_BREAK, CURSOR_RIGHT, false);
      render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
      EXPECT_EQ(end, render_text->selection_model());
    }
  }

  // Test moving by word from right to left.
  render_text->MoveCursor(LINE_BREAK, CURSOR_RIGHT, false);
  first_word = true;
  while (true) {
    SelectionModel start = render_text->selection_model();
    render_text->MoveCursor(WORD_BREAK, CURSOR_LEFT, false);
    SelectionModel end = render_text->selection_model();
    if (end == start)  // reach the end.
      break;

    int num_of_character_moves = first_word ? 3 : 4;
    first_word = false;
    render_text->MoveCursorTo(start);
    for (int j = 0; j < num_of_character_moves; ++j)
      render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
    EXPECT_EQ(end, render_text->selection_model());

    for (int j = 1; j < num_of_character_moves; ++j) {
      render_text->MoveCursorTo(start);
      for (int k = 0; k < j; ++k)
        render_text->MoveCursor(CHARACTER_BREAK, CURSOR_LEFT, false);
      render_text->MoveCursor(WORD_BREAK, CURSOR_LEFT, false);
      EXPECT_EQ(end, render_text->selection_model());
    }
  }
}

TEST_F(RenderTextTest, MoveLeftRightByWordInBidiText) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());

  // For testing simplicity, each word is a 3-character word.
  std::vector<const wchar_t*> test;
  test.push_back(L"abc");
  test.push_back(L"abc def");
  test.push_back(L"\x05E1\x05E2\x05E3");
  test.push_back(L"\x05E1\x05E2\x05E3 \x05E4\x05E5\x05E6");
  test.push_back(L"abc \x05E1\x05E2\x05E3");
  test.push_back(L"abc def \x05E1\x05E2\x05E3 \x05E4\x05E5\x05E6");
  test.push_back(L"abc def hij \x05E1\x05E2\x05E3 \x05E4\x05E5\x05E6"
                 L" \x05E7\x05E8\x05E9");

  test.push_back(L"abc \x05E1\x05E2\x05E3 hij");
  test.push_back(L"abc def \x05E1\x05E2\x05E3 \x05E4\x05E5\x05E6 hij opq");
  test.push_back(L"abc def hij \x05E1\x05E2\x05E3 \x05E4\x05E5\x05E6"
                 L" \x05E7\x05E8\x05E9"L" opq rst uvw");

  test.push_back(L"\x05E1\x05E2\x05E3 abc");
  test.push_back(L"\x05E1\x05E2\x05E3 \x05E4\x05E5\x05E6 abc def");
  test.push_back(L"\x05E1\x05E2\x05E3 \x05E4\x05E5\x05E6 \x05E7\x05E8\x05E9"
                 L" abc def hij");

  test.push_back(L"\x05D1\x05D2\x05D3 abc \x05E1\x05E2\x05E3");
  test.push_back(L"\x05D1\x05D2\x05D3 \x05D4\x05D5\x05D6 abc def"
                 L" \x05E1\x05E2\x05E3 \x05E4\x05E5\x05E6");
  test.push_back(L"\x05D1\x05D2\x05D3 \x05D4\x05D5\x05D6 \x05D7\x05D8\x05D9"
                 L" abc def hij \x05E1\x05E2\x05E3 \x05E4\x05E5\x05E6"
                 L" \x05E7\x05E8\x05E9");

  for (size_t i = 0; i < test.size(); ++i)
    MoveLeftRightByWordVerifier(render_text.get(), test[i]);
}

TEST_F(RenderTextTest, MoveLeftRightByWordInBidiText_TestEndOfText) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());

  render_text->SetText(WideToUTF16(L"ab\x05E1"));
  // Moving the cursor by word from "abC|" to the left should return "|abC".
  // But since end of text is always treated as a word break, it returns
  // position "ab|C".
  // TODO(xji): Need to make it work as expected.
  render_text->MoveCursor(LINE_BREAK, CURSOR_RIGHT, false);
  render_text->MoveCursor(WORD_BREAK, CURSOR_LEFT, false);
  // EXPECT_EQ(SelectionModel(), render_text->selection_model());

  // Moving the cursor by word from "|abC" to the right returns "abC|".
  render_text->MoveCursor(LINE_BREAK, CURSOR_LEFT, false);
  render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(SelectionModel(3, CURSOR_FORWARD), render_text->selection_model());

  render_text->SetText(WideToUTF16(L"\x05E1\x05E2"L"a"));
  // For logical text "BCa", moving the cursor by word from "aCB|" to the left
  // returns "|aCB".
  render_text->MoveCursor(LINE_BREAK, CURSOR_RIGHT, false);
  render_text->MoveCursor(WORD_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(SelectionModel(3, CURSOR_FORWARD), render_text->selection_model());

  // Moving the cursor by word from "|aCB" to the right should return "aCB|".
  // But since end of text is always treated as a word break, it returns
  // position "a|CB".
  // TODO(xji): Need to make it work as expected.
  render_text->MoveCursor(LINE_BREAK, CURSOR_LEFT, false);
  render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
  // EXPECT_EQ(SelectionModel(), render_text->selection_model());
}

TEST_F(RenderTextTest, MoveLeftRightByWordInTextWithMultiSpaces) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(WideToUTF16(L"abc     def"));
  render_text->MoveCursorTo(SelectionModel(5, CURSOR_FORWARD));
  render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(11U, render_text->cursor_position());

  render_text->MoveCursorTo(SelectionModel(5, CURSOR_FORWARD));
  render_text->MoveCursor(WORD_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(0U, render_text->cursor_position());
}

TEST_F(RenderTextTest, MoveLeftRightByWordInChineseText) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(WideToUTF16(L"\x6211\x4EEC\x53BB\x516C\x56ED\x73A9"));
  render_text->MoveCursor(LINE_BREAK, CURSOR_LEFT, false);
  EXPECT_EQ(0U, render_text->cursor_position());
  render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(2U, render_text->cursor_position());
  render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(3U, render_text->cursor_position());
  render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(5U, render_text->cursor_position());
  render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(6U, render_text->cursor_position());
  render_text->MoveCursor(WORD_BREAK, CURSOR_RIGHT, false);
  EXPECT_EQ(6U, render_text->cursor_position());
}
#endif

TEST_F(RenderTextTest, StringSizeSanity) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(UTF8ToUTF16("Hello World"));
  const Size string_size = render_text->GetStringSize();
  EXPECT_GT(string_size.width(), 0);
  EXPECT_GT(string_size.height(), 0);
}

// TODO(asvitkine): This test fails because PlatformFontMac uses point font
//                  sizes instead of pixel sizes like other implementations.
#if !defined(OS_MACOSX)
TEST_F(RenderTextTest, StringSizeEmptyString) {
  const Font font;
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetFont(font);

  render_text->SetText(string16());
  EXPECT_EQ(font.GetHeight(), render_text->GetStringSize().height());
  EXPECT_EQ(0, render_text->GetStringSize().width());

  render_text->SetText(UTF8ToUTF16(" "));
  EXPECT_EQ(font.GetHeight(), render_text->GetStringSize().height());
}
#endif  // !defined(OS_MACOSX)

TEST_F(RenderTextTest, SetFont) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetFont(Font("Arial", 12));
  EXPECT_EQ("Arial", render_text->GetFont().GetFontName());
  EXPECT_EQ(12, render_text->GetFont().GetFontSize());
}

TEST_F(RenderTextTest, StringSizeBoldWidth) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(UTF8ToUTF16("Hello World"));

  const int plain_width = render_text->GetStringSize().width();
  EXPECT_GT(plain_width, 0);

  // Apply a bold style and check that the new width is greater.
  render_text->SetStyle(gfx::BOLD, true);
  const int bold_width = render_text->GetStringSize().width();
  EXPECT_GT(bold_width, plain_width);

  // Now, apply a plain style over the first word only.
  render_text->ApplyStyle(gfx::BOLD, false, ui::Range(0, 5));
  const int plain_bold_width = render_text->GetStringSize().width();
  EXPECT_GT(plain_bold_width, plain_width);
  EXPECT_LT(plain_bold_width, bold_width);
}

TEST_F(RenderTextTest, StringSizeHeight) {
  string16 cases[] = {
    WideToUTF16(L"Hello World!"),  // English
    WideToUTF16(L"\x6328\x62f6"),  // Japanese
    WideToUTF16(L"\x0915\x093f"),  // Hindi
    WideToUTF16(L"\x05e0\x05b8"),  // Hebrew
  };

  Font default_font;
  Font larger_font = default_font.DeriveFont(24, default_font.GetStyle());
  EXPECT_GT(larger_font.GetHeight(), default_font.GetHeight());

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); i++) {
    scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
    render_text->SetFont(default_font);
    render_text->SetText(cases[i]);

    const int height1 = render_text->GetStringSize().height();
    EXPECT_GT(height1, 0);

    // Check that setting the larger font increases the height.
    render_text->SetFont(larger_font);
    const int height2 = render_text->GetStringSize().height();
    EXPECT_GT(height2, height1);
  }
}

TEST_F(RenderTextTest, GetBaselineSanity) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(UTF8ToUTF16("Hello World"));
  const int baseline = render_text->GetBaseline();
  EXPECT_GT(baseline, 0);
}

TEST_F(RenderTextTest, CursorBoundsInReplacementMode) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(ASCIIToUTF16("abcdefg"));
  render_text->SetDisplayRect(Rect(100, 17));
  SelectionModel sel_b(1, CURSOR_FORWARD);
  SelectionModel sel_c(2, CURSOR_FORWARD);
  Rect cursor_around_b = render_text->GetCursorBounds(sel_b, false);
  Rect cursor_before_b = render_text->GetCursorBounds(sel_b, true);
  Rect cursor_before_c = render_text->GetCursorBounds(sel_c, true);
  EXPECT_EQ(cursor_around_b.x(), cursor_before_b.x());
  EXPECT_EQ(cursor_around_b.right(), cursor_before_c.x());
}

// http://crbug.com/161902
#if defined(OS_LINUX)
#define MAYBE_OriginForDrawing DISABLED_OriginForDrawing
#else
#define MAYBE_OriginForDrawing OriginForDrawing
#endif
TEST_F(RenderTextTest, MAYBE_OriginForDrawing) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(ASCIIToUTF16("abcdefg"));
  render_text->SetFontList(FontList("Arial, 13px"));

  // Set display area's height equals to font height.
  const int font_height = render_text->GetStringSize().height();
  Rect display_rect(0, 0, 100, font_height);
  render_text->SetDisplayRect(display_rect);

  Vector2d offset = render_text->GetOffsetForDrawing();
  EXPECT_TRUE(offset.IsZero());

  // Set display area's height greater than font height.
  const int kEnlargement = 2;
  display_rect = Rect(0, 0, 100, font_height + kEnlargement);
  render_text->SetDisplayRect(display_rect);

  // Text should be vertically centered.
  offset = render_text->GetOffsetForDrawing();
  EXPECT_EQ(offset.x(), 0);
  EXPECT_EQ(offset.y(), kEnlargement / 2);
}

TEST_F(RenderTextTest, SameFontForParentheses) {
  struct {
    const char16 left_char;
    const char16 right_char;
  } punctuation_pairs[] = {
    { '(', ')' },
    { '{', '}' },
    { '<', '>' },
  };
  struct {
    string16 text;
  } cases[] = {
    // English(English)
    { WideToUTF16(L"Hello World(a)") },
    // English(English)English
    { WideToUTF16(L"Hello World(a)Hello World") },

    // Japanese(English)
    { WideToUTF16(L"\x6328\x62f6(a)") },
    // Japanese(English)Japanese
    { WideToUTF16(L"\x6328\x62f6(a)\x6328\x62f6") },
    // English(Japanese)English
    { WideToUTF16(L"Hello World(\x6328\x62f6)Hello World") },

    // Hindi(English)
    { WideToUTF16(L"\x0915\x093f(a)") },
    // Hindi(English)Hindi
    { WideToUTF16(L"\x0915\x093f(a)\x0915\x093f") },
    // English(Hindi)English
    { WideToUTF16(L"Hello World(\x0915\x093f)Hello World") },

    // Hebrew(English)
    { WideToUTF16(L"\x05e0\x05b8(a)") },
    // Hebrew(English)Hebrew
    { WideToUTF16(L"\x05e0\x05b8(a)\x05e0\x05b8") },
    // English(Hebrew)English
    { WideToUTF16(L"Hello World(\x05e0\x05b8)Hello World") },
  };

  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    string16 text = cases[i].text;
    const size_t start_paren_char_index = text.find('(');
    ASSERT_NE(string16::npos, start_paren_char_index);
    const size_t end_paren_char_index = text.find(')');
    ASSERT_NE(string16::npos, end_paren_char_index);

    for (size_t j = 0; j < ARRAYSIZE_UNSAFE(punctuation_pairs); ++j) {
      text[start_paren_char_index] = punctuation_pairs[j].left_char;
      text[end_paren_char_index] = punctuation_pairs[j].right_char;
      render_text->SetText(text);

      const std::vector<RenderText::FontSpan> spans =
          render_text->GetFontSpansForTesting();

      int start_paren_span_index = -1;
      int end_paren_span_index = -1;
      for (size_t k = 0; k < spans.size(); ++k) {
        if (IndexInRange(spans[k].second, start_paren_char_index))
          start_paren_span_index = k;
        if (IndexInRange(spans[k].second, end_paren_char_index))
          end_paren_span_index = k;
      }
      ASSERT_NE(-1, start_paren_span_index);
      ASSERT_NE(-1, end_paren_span_index);

      const Font& start_font = spans[start_paren_span_index].first;
      const Font& end_font = spans[end_paren_span_index].first;
      EXPECT_EQ(start_font.GetFontName(), end_font.GetFontName());
      EXPECT_EQ(start_font.GetFontSize(), end_font.GetFontSize());
      EXPECT_EQ(start_font.GetStyle(), end_font.GetStyle());
    }
  }
}

// Make sure the caret width is always >=1 so that the correct
// caret is drawn at high DPI. crbug.com/164100.
TEST_F(RenderTextTest, CaretWidth) {
  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(ASCIIToUTF16("abcdefg"));
  EXPECT_GE(render_text->GetUpdatedCursorBounds().width(), 1);
}

// Make sure the last word is selected when the cursor is at text.length().
TEST_F(RenderTextTest, LastWordSelected) {
  const std::string kTestURL1 = "http://www.google.com";
  const std::string kTestURL2 = "http://www.google.com/something/";

  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());

  render_text->SetText(ASCIIToUTF16(kTestURL1));
  render_text->SetCursorPosition(kTestURL1.length());
  render_text->SelectWord();
  EXPECT_EQ(ui::Range(kTestURL1.length() - 3, kTestURL1.length()),
            render_text->selection());

  render_text->SetText(ASCIIToUTF16(kTestURL2));
  render_text->SetCursorPosition(kTestURL2.length());
  render_text->SelectWord();
  EXPECT_EQ(ui::Range(kTestURL2.length() - 1, kTestURL2.length()),
            render_text->selection());
}

// TODO(asvitkine): Cursor movements tests disabled on Mac because RenderTextMac
//                  does not implement this yet. http://crbug.com/131618
#if !defined(OS_MACOSX)
TEST_F(RenderTextTest, DisplayRectShowsCursorLTR) {
  ASSERT_FALSE(base::i18n::IsRTL());
  ASSERT_FALSE(base::i18n::ICUIsRTL());

  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(WideToUTF16(L"abcdefghijklmnopqrstuvwxzyabcdefg"));
  render_text->MoveCursorTo(SelectionModel(render_text->text().length(),
                                           CURSOR_FORWARD));
  int width = render_text->GetStringSize().width();
  ASSERT_GT(width, 10);

  // Ensure that the cursor is placed at the width of its preceding text.
  render_text->SetDisplayRect(Rect(width + 10, 1));
  EXPECT_EQ(width, render_text->GetUpdatedCursorBounds().x());

  // Ensure that shrinking the display rectangle keeps the cursor in view.
  render_text->SetDisplayRect(Rect(width - 10, 1));
  EXPECT_EQ(render_text->display_rect().width() - 1,
            render_text->GetUpdatedCursorBounds().right());

  // TODO(msw): Investigate why this test passes with
  // |GetUpdateCursorBounds().x()|, while the above have to use
  // |.right()|.
  // Ensure that the text will pan to fill its expanding display rectangle.
  render_text->SetDisplayRect(Rect(width - 5, 1));
  EXPECT_EQ(render_text->display_rect().width() - 1,
            render_text->GetUpdatedCursorBounds().x());

  // Ensure that a sufficiently large display rectangle shows all the text.
  render_text->SetDisplayRect(Rect(width + 10, 1));
  EXPECT_EQ(width, render_text->GetUpdatedCursorBounds().x());

  // Repeat the test with RTL text.
  render_text->SetText(WideToUTF16(L"\x5d0\x5d1\x5d2\x5d3\x5d4\x5d5\x5d6\x5d7"
      L"\x5d8\x5d9\x5da\x5db\x5dc\x5dd\x5de\x5df"));
  render_text->MoveCursorTo(SelectionModel(0, CURSOR_FORWARD));
  width = render_text->GetStringSize().width();
  ASSERT_GT(width, 10);

  // Ensure that the cursor is placed at the width of its preceding text.
  render_text->SetDisplayRect(Rect(width + 10, 1));
  EXPECT_EQ(width, render_text->GetUpdatedCursorBounds().x());

  // Ensure that shrinking the display rectangle keeps the cursor in view.
  render_text->SetDisplayRect(Rect(width - 10, 1));
  EXPECT_EQ(render_text->display_rect().width() - 1,
            render_text->GetUpdatedCursorBounds().right());

  // Ensure that the text will pan to fill its expanding display rectangle.
  render_text->SetDisplayRect(Rect(width - 5, 1));
  EXPECT_EQ(render_text->display_rect().width() - 1,
            render_text->GetUpdatedCursorBounds().x());

  // Ensure that a sufficiently large display rectangle shows all the text.
  render_text->SetDisplayRect(Rect(width + 10, 1));
  EXPECT_EQ(width, render_text->GetUpdatedCursorBounds().x());
}

TEST_F(RenderTextTest, DisplayRectShowsCursorRTL) {
  // Set the application default text direction to RTL.
  const bool was_rtl = base::i18n::IsRTL();
  SetRTL(true);

  scoped_ptr<RenderText> render_text(RenderText::CreateInstance());
  render_text->SetText(WideToUTF16(L"abcdefghijklmnopqrstuvwxzyabcdefg"));
  render_text->MoveCursorTo(SelectionModel(0, CURSOR_FORWARD));
  int width = render_text->GetStringSize().width();
  ASSERT_GT(width, 10);

  // Ensure that the cursor is placed at the width of its preceding text.
  render_text->SetDisplayRect(Rect(width + 10, 1));
  EXPECT_EQ(render_text->display_rect().width() - width - 1,
            render_text->GetUpdatedCursorBounds().x());

  // Ensure that shrinking the display rectangle keeps the cursor in view.
  render_text->SetDisplayRect(Rect(width - 10, 1));
  EXPECT_EQ(0, render_text->GetUpdatedCursorBounds().x());

  // Ensure that the text will pan to fill its expanding display rectangle.
  render_text->SetDisplayRect(Rect(width - 5, 1));
  EXPECT_EQ(0, render_text->GetUpdatedCursorBounds().x());

  // Ensure that a sufficiently large display rectangle shows all the text.
  render_text->SetDisplayRect(Rect(width + 10, 1));
  EXPECT_EQ(render_text->display_rect().width() - width - 1,
            render_text->GetUpdatedCursorBounds().x());

  // Repeat the test with RTL text.
  render_text->SetText(WideToUTF16(L"\x5d0\x5d1\x5d2\x5d3\x5d4\x5d5\x5d6\x5d7"
      L"\x5d8\x5d9\x5da\x5db\x5dc\x5dd\x5de\x5df"));
  render_text->MoveCursorTo(SelectionModel(render_text->text().length(),
                                           CURSOR_FORWARD));
  width = render_text->GetStringSize().width();
  ASSERT_GT(width, 10);

  // Ensure that the cursor is placed at the width of its preceding text.
  render_text->SetDisplayRect(Rect(width + 10, 1));
  EXPECT_EQ(render_text->display_rect().width() - width - 1,
            render_text->GetUpdatedCursorBounds().x());

  // Ensure that shrinking the display rectangle keeps the cursor in view.
  render_text->SetDisplayRect(Rect(width - 10, 1));
  EXPECT_EQ(0, render_text->GetUpdatedCursorBounds().x());

  // Ensure that the text will pan to fill its expanding display rectangle.
  render_text->SetDisplayRect(Rect(width - 5, 1));
  EXPECT_EQ(0, render_text->GetUpdatedCursorBounds().x());

  // Ensure that a sufficiently large display rectangle shows all the text.
  render_text->SetDisplayRect(Rect(width + 10, 1));
  EXPECT_EQ(render_text->display_rect().width() - width - 1,
            render_text->GetUpdatedCursorBounds().x());

  // Reset the application default text direction to LTR.
  SetRTL(was_rtl);
  EXPECT_EQ(was_rtl, base::i18n::IsRTL());
}
#endif  // !defined(OS_MACOSX)

}  // namespace gfx
