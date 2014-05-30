// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/render_text_win.h"

#include <algorithm>

#include "base/i18n/break_iterator.h"
#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "base/win/windows_version.h"
#include "ui/base/text/utf16_indexing.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font_fallback_win.h"
#include "ui/gfx/font_smoothing_win.h"
#include "ui/gfx/platform_font_win.h"

namespace gfx {

namespace {

// The maximum supported number of Uniscribe runs; a SCRIPT_ITEM is 8 bytes.
// TODO(msw): Review memory use/failure? Max string length? Alternate approach?
const int kGuessItems = 100;
const int kMaxItems = 10000;

// The maximum supported number of Uniscribe glyphs; a glyph is 1 word.
// TODO(msw): Review memory use/failure? Max string length? Alternate approach?
const int kMaxGlyphs = 100000;

// Callback to |EnumEnhMetaFile()| to intercept font creation.
int CALLBACK MetaFileEnumProc(HDC hdc,
                              HANDLETABLE* table,
                              CONST ENHMETARECORD* record,
                              int table_entries,
                              LPARAM log_font) {
  if (record->iType == EMR_EXTCREATEFONTINDIRECTW) {
    const EMREXTCREATEFONTINDIRECTW* create_font_record =
        reinterpret_cast<const EMREXTCREATEFONTINDIRECTW*>(record);
    *reinterpret_cast<LOGFONT*>(log_font) = create_font_record->elfw.elfLogFont;
  }
  return 1;
}

// Finds a fallback font to use to render the specified |text| with respect to
// an initial |font|. Returns the resulting font via out param |result|. Returns
// |true| if a fallback font was found.
// Adapted from WebKit's |FontCache::GetFontDataForCharacters()|.
// TODO(asvitkine): This should be moved to font_fallback_win.cc.
bool ChooseFallbackFont(HDC hdc,
                        const Font& font,
                        const wchar_t* text,
                        int text_length,
                        Font* result) {
  // Use a meta file to intercept the fallback font chosen by Uniscribe.
  HDC meta_file_dc = CreateEnhMetaFile(hdc, NULL, NULL, NULL);
  if (!meta_file_dc)
    return false;

  SelectObject(meta_file_dc, font.GetNativeFont());

  SCRIPT_STRING_ANALYSIS script_analysis;
  HRESULT hresult =
      ScriptStringAnalyse(meta_file_dc, text, text_length, 0, -1,
                          SSA_METAFILE | SSA_FALLBACK | SSA_GLYPHS | SSA_LINK,
                          0, NULL, NULL, NULL, NULL, NULL, &script_analysis);

  if (SUCCEEDED(hresult)) {
    hresult = ScriptStringOut(script_analysis, 0, 0, 0, NULL, 0, 0, FALSE);
    ScriptStringFree(&script_analysis);
  }

  bool found_fallback = false;
  HENHMETAFILE meta_file = CloseEnhMetaFile(meta_file_dc);
  if (SUCCEEDED(hresult)) {
    LOGFONT log_font;
    log_font.lfFaceName[0] = 0;
    EnumEnhMetaFile(0, meta_file, MetaFileEnumProc, &log_font, NULL);
    if (log_font.lfFaceName[0]) {
      *result = Font(UTF16ToUTF8(log_font.lfFaceName), font.GetFontSize());
      found_fallback = true;
    }
  }
  DeleteEnhMetaFile(meta_file);

  return found_fallback;
}

// Changes |font| to have the specified |font_size| (or |font_height| on Windows
// XP) and |font_style| if it is not the case already. Only considers bold and
// italic styles, since the underlined style has no effect on glyph shaping.
void DeriveFontIfNecessary(int font_size,
                           int font_height,
                           int font_style,
                           Font* font) {
  const int kStyleMask = (Font::BOLD | Font::ITALIC);
  const int target_style = (font_style & kStyleMask);

  // On Windows XP, the font must be resized using |font_height| instead of
  // |font_size| to match GDI behavior.
  if (base::win::GetVersion() < base::win::VERSION_VISTA) {
    PlatformFontWin* platform_font =
        static_cast<PlatformFontWin*>(font->platform_font());
    *font = platform_font->DeriveFontWithHeight(font_height, target_style);
    return;
  }

  const int current_style = (font->GetStyle() & kStyleMask);
  const int current_size = font->GetFontSize();
  if (current_style != target_style || current_size != font_size)
    *font = font->DeriveFont(font_size - current_size, target_style);
}

// Returns true if |c| is a Unicode BiDi control character.
bool IsUnicodeBidiControlCharacter(char16 c) {
  return c == base::i18n::kRightToLeftMark ||
         c == base::i18n::kLeftToRightMark ||
         c == base::i18n::kLeftToRightEmbeddingMark ||
         c == base::i18n::kRightToLeftEmbeddingMark ||
         c == base::i18n::kPopDirectionalFormatting ||
         c == base::i18n::kLeftToRightOverride ||
         c == base::i18n::kRightToLeftOverride;
}

}  // namespace

namespace internal {

TextRun::TextRun()
  : foreground(0),
    font_style(0),
    strike(false),
    diagonal_strike(false),
    underline(false),
    width(0),
    preceding_run_widths(0),
    glyph_count(0),
    script_cache(NULL) {
  memset(&script_analysis, 0, sizeof(script_analysis));
  memset(&abc_widths, 0, sizeof(abc_widths));
}

TextRun::~TextRun() {
  ScriptFreeCache(&script_cache);
}

// Returns the X coordinate of the leading or |trailing| edge of the glyph
// starting at |index|, relative to the left of the text (not the view).
int GetGlyphXBoundary(internal::TextRun* run, size_t index, bool trailing) {
  DCHECK_GE(index, run->range.start());
  DCHECK_LT(index, run->range.end() + (trailing ? 0 : 1));
  int x = 0;
  HRESULT hr = ScriptCPtoX(
      index - run->range.start(),
      trailing,
      run->range.length(),
      run->glyph_count,
      run->logical_clusters.get(),
      run->visible_attributes.get(),
      run->advance_widths.get(),
      &run->script_analysis,
      &x);
  DCHECK(SUCCEEDED(hr));
  return run->preceding_run_widths + x;
}

}  // namespace internal

// static
HDC RenderTextWin::cached_hdc_ = NULL;

// static
std::map<std::string, Font> RenderTextWin::successful_substitute_fonts_;

RenderTextWin::RenderTextWin()
    : RenderText(),
      common_baseline_(0),
      needs_layout_(false) {
  memset(&script_control_, 0, sizeof(script_control_));
  memset(&script_state_, 0, sizeof(script_state_));

  MoveCursorTo(EdgeSelectionModel(CURSOR_LEFT));
}

RenderTextWin::~RenderTextWin() {
}

Size RenderTextWin::GetStringSize() {
  EnsureLayout();
  return string_size_;
}

int RenderTextWin::GetBaseline() {
  EnsureLayout();
  return common_baseline_;
}

SelectionModel RenderTextWin::FindCursorPosition(const Point& point) {
  if (text().empty())
    return SelectionModel();

  EnsureLayout();
  // Find the run that contains the point and adjust the argument location.
  Point p(ToTextPoint(point));
  size_t run_index = GetRunContainingPoint(p);
  if (run_index == runs_.size())
    return EdgeSelectionModel((p.x() < 0) ? CURSOR_LEFT : CURSOR_RIGHT);
  internal::TextRun* run = runs_[run_index];

  int position = 0, trailing = 0;
  HRESULT hr = ScriptXtoCP(p.x() - run->preceding_run_widths,
                           run->range.length(),
                           run->glyph_count,
                           run->logical_clusters.get(),
                           run->visible_attributes.get(),
                           run->advance_widths.get(),
                           &(run->script_analysis),
                           &position,
                           &trailing);
  DCHECK(SUCCEEDED(hr));
  DCHECK_GE(trailing, 0);
  position += run->range.start();
  const size_t cursor = LayoutIndexToTextIndex(position + trailing);
  DCHECK_LE(cursor, text().length());
  return SelectionModel(cursor, trailing ? CURSOR_BACKWARD : CURSOR_FORWARD);
}

std::vector<RenderText::FontSpan> RenderTextWin::GetFontSpansForTesting() {
  EnsureLayout();

  std::vector<RenderText::FontSpan> spans;
  for (size_t i = 0; i < runs_.size(); ++i) {
    spans.push_back(RenderText::FontSpan(runs_[i]->font,
        ui::Range(LayoutIndexToTextIndex(runs_[i]->range.start()),
                  LayoutIndexToTextIndex(runs_[i]->range.end()))));
  }

  return spans;
}

SelectionModel RenderTextWin::AdjacentCharSelectionModel(
    const SelectionModel& selection,
    VisualCursorDirection direction) {
  DCHECK(!needs_layout_);
  internal::TextRun* run;
  size_t run_index = GetRunContainingCaret(selection);
  if (run_index >= runs_.size()) {
    // The cursor is not in any run: we're at the visual and logical edge.
    SelectionModel edge = EdgeSelectionModel(direction);
    if (edge.caret_pos() == selection.caret_pos())
      return edge;
    int visual_index = (direction == CURSOR_RIGHT) ? 0 : runs_.size() - 1;
    run = runs_[visual_to_logical_[visual_index]];
  } else {
    // If the cursor is moving within the current run, just move it by one
    // grapheme in the appropriate direction.
    run = runs_[run_index];
    size_t caret = selection.caret_pos();
    bool forward_motion =
        run->script_analysis.fRTL == (direction == CURSOR_LEFT);
    if (forward_motion) {
      if (caret < LayoutIndexToTextIndex(run->range.end())) {
        caret = IndexOfAdjacentGrapheme(caret, CURSOR_FORWARD);
        return SelectionModel(caret, CURSOR_BACKWARD);
      }
    } else {
      if (caret > LayoutIndexToTextIndex(run->range.start())) {
        caret = IndexOfAdjacentGrapheme(caret, CURSOR_BACKWARD);
        return SelectionModel(caret, CURSOR_FORWARD);
      }
    }
    // The cursor is at the edge of a run; move to the visually adjacent run.
    int visual_index = logical_to_visual_[run_index];
    visual_index += (direction == CURSOR_LEFT) ? -1 : 1;
    if (visual_index < 0 || visual_index >= static_cast<int>(runs_.size()))
      return EdgeSelectionModel(direction);
    run = runs_[visual_to_logical_[visual_index]];
  }
  bool forward_motion = run->script_analysis.fRTL == (direction == CURSOR_LEFT);
  return forward_motion ? FirstSelectionModelInsideRun(run) :
                          LastSelectionModelInsideRun(run);
}

// TODO(msw): Implement word breaking for Windows.
SelectionModel RenderTextWin::AdjacentWordSelectionModel(
    const SelectionModel& selection,
    VisualCursorDirection direction) {
  if (obscured())
    return EdgeSelectionModel(direction);

  base::i18n::BreakIterator iter(text(), base::i18n::BreakIterator::BREAK_WORD);
  bool success = iter.Init();
  DCHECK(success);
  if (!success)
    return selection;

  size_t pos;
  if (direction == CURSOR_RIGHT) {
    pos = std::min(selection.caret_pos() + 1, text().length());
    while (iter.Advance()) {
      pos = iter.pos();
      if (iter.IsWord() && pos > selection.caret_pos())
        break;
    }
  } else {  // direction == CURSOR_LEFT
    // Notes: We always iterate words from the beginning.
    // This is probably fast enough for our usage, but we may
    // want to modify WordIterator so that it can start from the
    // middle of string and advance backwards.
    pos = std::max<int>(selection.caret_pos() - 1, 0);
    while (iter.Advance()) {
      if (iter.IsWord()) {
        size_t begin = iter.pos() - iter.GetString().length();
        if (begin == selection.caret_pos()) {
          // The cursor is at the beginning of a word.
          // Move to previous word.
          break;
        } else if (iter.pos() >= selection.caret_pos()) {
          // The cursor is in the middle or at the end of a word.
          // Move to the top of current word.
          pos = begin;
          break;
        } else {
          pos = iter.pos() - iter.GetString().length();
        }
      }
    }
  }
  return SelectionModel(pos, CURSOR_FORWARD);
}

void RenderTextWin::SetSelectionModel(const SelectionModel& model) {
  RenderText::SetSelectionModel(model);
  // TODO(xji|msw): The text selection color is applied in ItemizeLogicalText().
  // So, the layout must be updated in order to draw the proper selection range.
  // Colors should be applied in DrawVisualText(), as done by RenderTextLinux.
  ResetLayout();
}

void RenderTextWin::GetGlyphBounds(size_t index,
                                   ui::Range* xspan,
                                   int* height) {
  const size_t run_index =
      GetRunContainingCaret(SelectionModel(index, CURSOR_FORWARD));
  DCHECK_LT(run_index, runs_.size());
  internal::TextRun* run = runs_[run_index];
  const size_t layout_index = TextIndexToLayoutIndex(index);
  xspan->set_start(GetGlyphXBoundary(run, layout_index, false));
  xspan->set_end(GetGlyphXBoundary(run, layout_index, true));
  *height = run->font.GetHeight();
}

std::vector<Rect> RenderTextWin::GetSubstringBounds(const ui::Range& range) {
  DCHECK(!needs_layout_);
  DCHECK(ui::Range(0, text().length()).Contains(range));
  ui::Range layout_range(TextIndexToLayoutIndex(range.start()),
                         TextIndexToLayoutIndex(range.end()));
  DCHECK(ui::Range(0, GetLayoutText().length()).Contains(layout_range));

  std::vector<Rect> bounds;
  if (layout_range.is_empty())
    return bounds;

  // Add a Rect for each run/selection intersection.
  // TODO(msw): The bounds should probably not always be leading the range ends.
  for (size_t i = 0; i < runs_.size(); ++i) {
    internal::TextRun* run = runs_[visual_to_logical_[i]];
    ui::Range intersection = run->range.Intersect(layout_range);
    if (intersection.IsValid()) {
      DCHECK(!intersection.is_reversed());
      ui::Range range_x(GetGlyphXBoundary(run, intersection.start(), false),
                        GetGlyphXBoundary(run, intersection.end(), false));
      Rect rect(range_x.GetMin(), 0, range_x.length(), run->font.GetHeight());
      // Center the rect vertically in the display area.
      rect.Offset(0, (display_rect().height() - rect.height()) / 2);
      rect.set_origin(ToViewPoint(rect.origin()));
      // Union this with the last rect if they're adjacent.
      if (!bounds.empty() && rect.SharesEdgeWith(bounds.back())) {
        rect.Union(bounds.back());
        bounds.pop_back();
      }
      bounds.push_back(rect);
    }
  }
  return bounds;
}

size_t RenderTextWin::TextIndexToLayoutIndex(size_t index) const {
  if (!obscured())
    return index;

  DCHECK_LE(index, text().length());
  const ptrdiff_t offset = ui::UTF16IndexToOffset(text(), 0, index);
  DCHECK_GE(offset, 0);
  DCHECK_LE(static_cast<size_t>(offset), GetLayoutText().length());
  return static_cast<size_t>(offset);
}

size_t RenderTextWin::LayoutIndexToTextIndex(size_t index) const {
  if (!obscured())
    return index;

  DCHECK_LE(index, GetLayoutText().length());
  const size_t text_index = ui::UTF16OffsetToIndex(text(), 0, index);
  DCHECK_LE(text_index, text().length());
  return text_index;
}

bool RenderTextWin::IsCursorablePosition(size_t position) {
  if (position == 0 || position == text().length())
    return true;

  EnsureLayout();
  const size_t run_index =
      GetRunContainingCaret(SelectionModel(position, CURSOR_FORWARD));
  if (run_index >= runs_.size())
    return false;

  internal::TextRun* run = runs_[run_index];
  const size_t start = run->range.start();
  const size_t layout_position = TextIndexToLayoutIndex(position);
  if (layout_position == start)
    return true;
  return run->logical_clusters[layout_position - start] !=
         run->logical_clusters[layout_position - start - 1];
}

void RenderTextWin::ResetLayout() {
  // Layout is performed lazily as needed for drawing/metrics.
  needs_layout_ = true;
}

void RenderTextWin::EnsureLayout() {
  if (!needs_layout_)
    return;
  // TODO(msw): Skip complex processing if ScriptIsComplex returns false.
  ItemizeLogicalText();
  if (!runs_.empty())
    LayoutVisualText();
  needs_layout_ = false;
}

void RenderTextWin::DrawVisualText(Canvas* canvas) {
  DCHECK(!needs_layout_);

  // Skia will draw glyphs with respect to the baseline.
  Vector2d offset(GetOffsetForDrawing() + Vector2d(0, common_baseline_));

  SkScalar x = SkIntToScalar(offset.x());
  SkScalar y = SkIntToScalar(offset.y());

  std::vector<SkPoint> pos;

  internal::SkiaTextRenderer renderer(canvas);
  ApplyFadeEffects(&renderer);
  ApplyTextShadows(&renderer);

  bool smoothing_enabled;
  bool cleartype_enabled;
  GetCachedFontSmoothingSettings(&smoothing_enabled, &cleartype_enabled);
  // Note that |cleartype_enabled| corresponds to Skia's |enable_lcd_text|.
  renderer.SetFontSmoothingSettings(
      smoothing_enabled, cleartype_enabled && !background_is_transparent());

  for (size_t i = 0; i < runs_.size(); ++i) {
    // Get the run specified by the visual-to-logical map.
    internal::TextRun* run = runs_[visual_to_logical_[i]];

    if (run->glyph_count == 0)
      continue;

    // Based on WebCore::skiaDrawText.
    pos.resize(run->glyph_count);
    SkScalar glyph_x = x;
    for (int glyph = 0; glyph < run->glyph_count; glyph++) {
      pos[glyph].set(glyph_x + run->offsets[glyph].du,
                     y + run->offsets[glyph].dv);
      glyph_x += SkIntToScalar(run->advance_widths[glyph]);
    }

    renderer.SetTextSize(run->font.GetFontSize());
    renderer.SetFontFamilyWithStyle(run->font.GetFontName(), run->font_style);
    renderer.SetForegroundColor(run->foreground);
    renderer.DrawPosText(&pos[0], run->glyphs.get(), run->glyph_count);
    renderer.DrawDecorations(x, y, run->width, run->underline, run->strike,
                             run->diagonal_strike);

    x = glyph_x;
  }
}

void RenderTextWin::ItemizeLogicalText() {
  runs_.clear();
  string_size_ = Size(0, GetFont().GetHeight());
  common_baseline_ = 0;

  // Set Uniscribe's base text direction.
  script_state_.uBidiLevel =
      (GetTextDirection() == base::i18n::RIGHT_TO_LEFT) ? 1 : 0;

  if (text().empty())
    return;

  HRESULT hr = E_OUTOFMEMORY;
  int script_items_count = 0;
  std::vector<SCRIPT_ITEM> script_items;
  const size_t text_length = GetLayoutText().length();
  for (size_t n = kGuessItems; hr == E_OUTOFMEMORY && n < kMaxItems; n *= 2) {
    // Derive the array of Uniscribe script items from the logical text.
    // ScriptItemize always adds a terminal array item so that the length of the
    // last item can be derived from the terminal SCRIPT_ITEM::iCharPos.
    script_items.resize(n);
    hr = ScriptItemize(GetLayoutText().c_str(),
                       text_length,
                       n - 1,
                       &script_control_,
                       &script_state_,
                       &script_items[0],
                       &script_items_count);
  }
  DCHECK(SUCCEEDED(hr));

  if (script_items_count <= 0)
    return;

  // Temporarily apply composition underlines and selection colors.
  ApplyCompositionAndSelectionStyles();

  // Build the list of runs from the script items and ranged colors/styles.
  // TODO(msw): Only break for bold/italic, not color etc. See TextRun comment.
  internal::StyleIterator style(colors(), styles());
  SCRIPT_ITEM* script_item = &script_items[0];
  const size_t layout_text_length = GetLayoutText().length();
  for (size_t run_break = 0; run_break < layout_text_length;) {
    internal::TextRun* run = new internal::TextRun();
    run->range.set_start(run_break);
    run->font = GetFont();
    run->font_style = (style.style(BOLD) ? Font::BOLD : 0) |
                      (style.style(ITALIC) ? Font::ITALIC : 0);
    DeriveFontIfNecessary(run->font.GetFontSize(), run->font.GetHeight(),
                          run->font_style, &run->font);
    run->foreground = style.color();
    run->strike = style.style(STRIKE);
    run->diagonal_strike = style.style(DIAGONAL_STRIKE);
    run->underline = style.style(UNDERLINE);
    run->script_analysis = script_item->a;

    // Find the next break and advance the iterators as needed.
    const size_t script_item_break = (script_item + 1)->iCharPos;
    run_break = std::min(script_item_break,
                         TextIndexToLayoutIndex(style.GetRange().end()));
    style.UpdatePosition(LayoutIndexToTextIndex(run_break));
    if (script_item_break == run_break)
      script_item++;
    run->range.set_end(run_break);
    runs_.push_back(run);
  }

  // Undo the temporarily applied composition underlines and selection colors.
  UndoCompositionAndSelectionStyles();
}

void RenderTextWin::LayoutVisualText() {
  DCHECK(!runs_.empty());

  if (!cached_hdc_)
    cached_hdc_ = CreateCompatibleDC(NULL);

  HRESULT hr = E_FAIL;
  string_size_.set_height(0);
  for (size_t i = 0; i < runs_.size(); ++i) {
    internal::TextRun* run = runs_[i];
    LayoutTextRun(run);

    string_size_.set_height(std::max(string_size_.height(),
                                     run->font.GetHeight()));
    common_baseline_ = std::max(common_baseline_, run->font.GetBaseline());

    if (run->glyph_count > 0) {
      run->advance_widths.reset(new int[run->glyph_count]);
      run->offsets.reset(new GOFFSET[run->glyph_count]);
      hr = ScriptPlace(cached_hdc_,
                       &run->script_cache,
                       run->glyphs.get(),
                       run->glyph_count,
                       run->visible_attributes.get(),
                       &(run->script_analysis),
                       run->advance_widths.get(),
                       run->offsets.get(),
                       &(run->abc_widths));
      DCHECK(SUCCEEDED(hr));
    }
  }

  // Build the array of bidirectional embedding levels.
  scoped_ptr<BYTE[]> levels(new BYTE[runs_.size()]);
  for (size_t i = 0; i < runs_.size(); ++i)
    levels[i] = runs_[i]->script_analysis.s.uBidiLevel;

  // Get the maps between visual and logical run indices.
  visual_to_logical_.reset(new int[runs_.size()]);
  logical_to_visual_.reset(new int[runs_.size()]);
  hr = ScriptLayout(runs_.size(),
                    levels.get(),
                    visual_to_logical_.get(),
                    logical_to_visual_.get());
  DCHECK(SUCCEEDED(hr));

  // Precalculate run width information.
  size_t preceding_run_widths = 0;
  for (size_t i = 0; i < runs_.size(); ++i) {
    internal::TextRun* run = runs_[visual_to_logical_[i]];
    run->preceding_run_widths = preceding_run_widths;
    const ABC& abc = run->abc_widths;
    run->width = abc.abcA + abc.abcB + abc.abcC;
    preceding_run_widths += run->width;
  }
  string_size_.set_width(preceding_run_widths);
}

void RenderTextWin::LayoutTextRun(internal::TextRun* run) {
  const size_t run_length = run->range.length();
  const wchar_t* run_text = &(GetLayoutText()[run->range.start()]);
  Font original_font = run->font;
  LinkedFontsIterator fonts(original_font);
  bool tried_cached_font = false;
  bool tried_fallback = false;
  // Keep track of the font that is able to display the greatest number of
  // characters for which ScriptShape() returned S_OK. This font will be used
  // in the case where no font is able to display the entire run.
  int best_partial_font_missing_char_count = INT_MAX;
  Font best_partial_font = original_font;
  bool using_best_partial_font = false;
  Font current_font;

  run->logical_clusters.reset(new WORD[run_length]);
  while (fonts.NextFont(&current_font)) {
    HRESULT hr = ShapeTextRunWithFont(run, current_font);

    bool glyphs_missing = false;
    if (hr == USP_E_SCRIPT_NOT_IN_FONT) {
      glyphs_missing = true;
    } else if (hr == S_OK) {
      // If |hr| is S_OK, there could still be missing glyphs in the output.
      // http://msdn.microsoft.com/en-us/library/windows/desktop/dd368564.aspx
      const int missing_count = CountCharsWithMissingGlyphs(run);
      // Track the font that produced the least missing glyphs.
      if (missing_count < best_partial_font_missing_char_count) {
        best_partial_font_missing_char_count = missing_count;
        best_partial_font = run->font;
      }
      glyphs_missing = (missing_count != 0);
    } else {
      NOTREACHED() << hr;
    }

    // Use the font if it had glyphs for all characters.
    if (!glyphs_missing) {
      // Save the successful fallback font that was chosen.
      if (tried_fallback)
        successful_substitute_fonts_[original_font.GetFontName()] = run->font;
      return;
    }

    // First, try the cached font from previous runs, if any.
    if (!tried_cached_font) {
      tried_cached_font = true;

      std::map<std::string, Font>::const_iterator it =
          successful_substitute_fonts_.find(original_font.GetFontName());
      if (it != successful_substitute_fonts_.end()) {
        fonts.SetNextFont(it->second);
        continue;
      }
    }

    // If there are missing glyphs, first try finding a fallback font using a
    // meta file, if it hasn't yet been attempted for this run.
    // TODO(msw|asvitkine): Support RenderText's font_list()?
    if (!tried_fallback) {
      tried_fallback = true;

      Font fallback_font;
      if (ChooseFallbackFont(cached_hdc_, run->font, run_text, run_length,
                             &fallback_font)) {
        fonts.SetNextFont(fallback_font);
        continue;
      }
    }
  }

  // If a font was able to partially display the run, use that now.
  if (best_partial_font_missing_char_count < static_cast<int>(run_length)) {
    // Re-shape the run only if |best_partial_font| differs from the last font.
    if (best_partial_font.GetNativeFont() != run->font.GetNativeFont())
      ShapeTextRunWithFont(run, best_partial_font);
    return;
  }

  // If no font was able to partially display the run, replace all glyphs
  // with |wgDefault| from the original font to ensure to they don't hold
  // garbage values.
  // First, clear the cache and select the original font on the HDC.
  ScriptFreeCache(&run->script_cache);
  run->font = original_font;
  SelectObject(cached_hdc_, run->font.GetNativeFont());

  // Now, get the font's properties.
  SCRIPT_FONTPROPERTIES properties;
  memset(&properties, 0, sizeof(properties));
  properties.cBytes = sizeof(properties);
  HRESULT hr = ScriptGetFontProperties(cached_hdc_, &run->script_cache,
                                       &properties);
  if (hr == S_OK) {
    // Finally, initialize |glyph_count|, |glyphs| and |visible_attributes| on
    // the run (since they may not have been set yet).
    run->glyph_count = run_length;
    memset(run->visible_attributes.get(), 0,
           run->glyph_count * sizeof(SCRIPT_VISATTR));
    for (int i = 0; i < run->glyph_count; ++i) {
      run->glyphs[i] = IsWhitespace(run_text[i]) ? properties.wgBlank :
                                                   properties.wgDefault;
    }
  }

  // TODO(msw): Don't use SCRIPT_UNDEFINED. Apparently Uniscribe can
  //            crash on certain surrogate pairs with SCRIPT_UNDEFINED.
  //            See https://bugzilla.mozilla.org/show_bug.cgi?id=341500
  //            And http://maxradi.us/documents/uniscribe/
  run->script_analysis.eScript = SCRIPT_UNDEFINED;
}

HRESULT RenderTextWin::ShapeTextRunWithFont(internal::TextRun* run,
                                            const Font& font) {
  // Update the run's font only if necessary. If the two fonts wrap the same
  // PlatformFontWin object, their native fonts will have the same value.
  if (run->font.GetNativeFont() != font.GetNativeFont()) {
    const int font_size = run->font.GetFontSize();
    const int font_height = run->font.GetHeight();
    run->font = font;
    DeriveFontIfNecessary(font_size, font_height, run->font_style, &run->font);
    ScriptFreeCache(&run->script_cache);
  }

  // Select the font desired for glyph generation.
  SelectObject(cached_hdc_, run->font.GetNativeFont());

  HRESULT hr = E_OUTOFMEMORY;
  const size_t run_length = run->range.length();
  const wchar_t* run_text = &(GetLayoutText()[run->range.start()]);
  // Max glyph guess: http://msdn.microsoft.com/en-us/library/dd368564.aspx
  size_t max_glyphs = static_cast<size_t>(1.5 * run_length + 16);
  while (hr == E_OUTOFMEMORY && max_glyphs < kMaxGlyphs) {
    run->glyph_count = 0;
    run->glyphs.reset(new WORD[max_glyphs]);
    run->visible_attributes.reset(new SCRIPT_VISATTR[max_glyphs]);
    hr = ScriptShape(cached_hdc_,
                     &run->script_cache,
                     run_text,
                     run_length,
                     max_glyphs,
                     &run->script_analysis,
                     run->glyphs.get(),
                     run->logical_clusters.get(),
                     run->visible_attributes.get(),
                     &run->glyph_count);
    max_glyphs *= 2;
  }
  return hr;
}

int RenderTextWin::CountCharsWithMissingGlyphs(internal::TextRun* run) const {
  int chars_not_missing_glyphs = 0;
  SCRIPT_FONTPROPERTIES properties;
  memset(&properties, 0, sizeof(properties));
  properties.cBytes = sizeof(properties);
  ScriptGetFontProperties(cached_hdc_, &run->script_cache, &properties);

  const wchar_t* run_text = &(GetLayoutText()[run->range.start()]);
  for (size_t char_index = 0; char_index < run->range.length(); ++char_index) {
    const int glyph_index = run->logical_clusters[char_index];
    DCHECK_GE(glyph_index, 0);
    DCHECK_LT(glyph_index, run->glyph_count);

    if (run->glyphs[glyph_index] == properties.wgDefault)
      continue;

    // Windows Vista sometimes returns glyphs equal to wgBlank (instead of
    // wgDefault), with fZeroWidth set. Treat such cases as having missing
    // glyphs if the corresponding character is not whitespace.
    // See: http://crbug.com/125629
    if (run->glyphs[glyph_index] == properties.wgBlank &&
        run->visible_attributes[glyph_index].fZeroWidth &&
        !IsWhitespace(run_text[char_index]) &&
        !IsUnicodeBidiControlCharacter(run_text[char_index])) {
      continue;
    }

    ++chars_not_missing_glyphs;
  }

  DCHECK_LE(chars_not_missing_glyphs, static_cast<int>(run->range.length()));
  return run->range.length() - chars_not_missing_glyphs;
}

size_t RenderTextWin::GetRunContainingCaret(const SelectionModel& caret) const {
  DCHECK(!needs_layout_);
  size_t layout_position = TextIndexToLayoutIndex(caret.caret_pos());
  LogicalCursorDirection affinity = caret.caret_affinity();
  size_t run = 0;
  for (; run < runs_.size(); ++run)
    if (RangeContainsCaret(runs_[run]->range, layout_position, affinity))
      break;
  return run;
}

size_t RenderTextWin::GetRunContainingPoint(const Point& point) const {
  DCHECK(!needs_layout_);
  // Find the text run containing the argument point (assumed already offset).
  size_t run = 0;
  for (; run < runs_.size(); ++run)
    if (runs_[run]->preceding_run_widths <= point.x() &&
        runs_[run]->preceding_run_widths + runs_[run]->width > point.x())
      break;
  return run;
}

SelectionModel RenderTextWin::FirstSelectionModelInsideRun(
    const internal::TextRun* run) {
  size_t position = LayoutIndexToTextIndex(run->range.start());
  position = IndexOfAdjacentGrapheme(position, CURSOR_FORWARD);
  return SelectionModel(position, CURSOR_BACKWARD);
}

SelectionModel RenderTextWin::LastSelectionModelInsideRun(
    const internal::TextRun* run) {
  size_t position = LayoutIndexToTextIndex(run->range.end());
  position = IndexOfAdjacentGrapheme(position, CURSOR_BACKWARD);
  return SelectionModel(position, CURSOR_FORWARD);
}

RenderText* RenderText::CreateInstance() {
  return new RenderTextWin;
}

}  // namespace gfx
