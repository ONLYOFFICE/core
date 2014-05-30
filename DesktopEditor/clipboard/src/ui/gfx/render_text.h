// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RENDER_TEXT_H_
#define UI_GFX_RENDER_TEXT_H_

#include <algorithm>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/i18n/rtl.h"
#include "base/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "third_party/skia/include/core/SkRect.h"
#include "ui/base/range/range.h"
#include "ui/gfx/break_list.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/selection_model.h"
#include "ui/gfx/shadow_value.h"
#include "ui/gfx/text_constants.h"
#include "ui/gfx/vector2d.h"

class SkCanvas;
class SkDrawLooper;
struct SkPoint;
class SkShader;
class SkTypeface;

namespace gfx {

class Canvas;
class Font;
class RenderTextTest;

namespace internal {

// Internal helper class used by derived classes to draw text through Skia.
class SkiaTextRenderer {
 public:
  explicit SkiaTextRenderer(Canvas* canvas);
  ~SkiaTextRenderer();

  void SetDrawLooper(SkDrawLooper* draw_looper);
  void SetFontSmoothingSettings(bool enable_smoothing, bool enable_lcd_text);
  void SetTypeface(SkTypeface* typeface);
  void SetTextSize(SkScalar size);
  void SetFontFamilyWithStyle(const std::string& family, int font_style);
  void SetForegroundColor(SkColor foreground);
  void SetShader(SkShader* shader, const Rect& bounds);
  // Sets underline metrics to use if the text will be drawn with an underline.
  // If not set, default values based on the size of the text will be used. The
  // two metrics must be set together.
  void SetUnderlineMetrics(SkScalar thickness, SkScalar position);
  void DrawSelection(const std::vector<Rect>& selection, SkColor color);
  void DrawPosText(const SkPoint* pos,
                   const uint16* glyphs,
                   size_t glyph_count);
  // Draw underline and strike-through text decorations.
  // Based on |SkCanvas::DrawTextDecorations()| and constants from:
  //   third_party/skia/src/core/SkTextFormatParams.h
  void DrawDecorations(int x, int y, int width, bool underline, bool strike,
                       bool diagonal_strike);
  void DrawUnderline(int x, int y, int width);
  void DrawStrike(int x, int y, int width) const;
  void DrawDiagonalStrike(int x, int y, int width) const;

 private:
  SkCanvas* canvas_skia_;
  bool started_drawing_;
  SkPaint paint_;
  SkRect bounds_;
  SkRefPtr<SkShader> deferred_fade_shader_;
  SkScalar underline_thickness_;
  SkScalar underline_position_;

  DISALLOW_COPY_AND_ASSIGN(SkiaTextRenderer);
};

// Internal helper class used by derived classes to iterate colors and styles.
class StyleIterator {
 public:
  StyleIterator(const BreakList<SkColor>& colors,
                const std::vector<BreakList<bool> >& styles);
  ~StyleIterator();

  // Get the colors and styles at the current iterator position.
  SkColor color() const { return color_->second; }
  bool style(TextStyle s) const { return style_[s]->second; }

  // Get the intersecting range of the current iterator set.
  ui::Range GetRange() const;

  // Update the iterator to point to colors and styles applicable at |position|.
  void UpdatePosition(size_t position);

 private:
  BreakList<SkColor> colors_;
  std::vector<BreakList<bool> > styles_;

  BreakList<SkColor>::const_iterator color_;
  std::vector<BreakList<bool>::const_iterator> style_;

  DISALLOW_COPY_AND_ASSIGN(StyleIterator);
};

}  // namespace internal

// RenderText represents an abstract model of styled text and its corresponding
// visual layout. Support is built in for a cursor, a selection, simple styling,
// complex scripts, and bi-directional text. Implementations provide mechanisms
// for rendering and translation between logical and visual data.
class UI_EXPORT RenderText {
 public:
  virtual ~RenderText();

  // Creates a platform-specific RenderText instance.
  static RenderText* CreateInstance();

  const string16& text() const { return text_; }
  void SetText(const string16& text);

  HorizontalAlignment horizontal_alignment() const {
    return horizontal_alignment_;
  }
  void SetHorizontalAlignment(HorizontalAlignment alignment);

  const FontList& font_list() const { return font_list_; }
  void SetFontList(const FontList& font_list);
  void SetFont(const Font& font);

  // Set the font size to |size| in pixels.
  void SetFontSize(int size);

  // Get the first font in |font_list_|.
  const Font& GetFont() const;

  bool cursor_enabled() const { return cursor_enabled_; }
  void SetCursorEnabled(bool cursor_enabled);

  bool cursor_visible() const { return cursor_visible_; }
  void set_cursor_visible(bool visible) { cursor_visible_ = visible; }

  bool insert_mode() const { return insert_mode_; }
  void ToggleInsertMode();

  SkColor cursor_color() const { return cursor_color_; }
  void set_cursor_color(SkColor color) { cursor_color_ = color; }

  SkColor selection_color() const { return selection_color_; }
  void set_selection_color(SkColor color) { selection_color_ = color; }

  SkColor selection_background_focused_color() const {
    return selection_background_focused_color_;
  }
  void set_selection_background_focused_color(SkColor color) {
    selection_background_focused_color_ = color;
  }

  SkColor selection_background_unfocused_color() const {
    return selection_background_unfocused_color_;
  }
  void set_selection_background_unfocused_color(SkColor color) {
    selection_background_unfocused_color_ = color;
  }

  bool focused() const { return focused_; }
  void set_focused(bool focused) { focused_ = focused; }

  bool clip_to_display_rect() const { return clip_to_display_rect_; }
  void set_clip_to_display_rect(bool clip) { clip_to_display_rect_ = clip; }

  // In an obscured (password) field, all text is drawn as asterisks or bullets.
  bool obscured() const { return obscured_; }
  void SetObscured(bool obscured);

  const Rect& display_rect() const { return display_rect_; }
  void SetDisplayRect(const Rect& r);

  void set_fade_head(bool fade_head) { fade_head_ = fade_head; }
  bool fade_head() const { return fade_head_; }
  void set_fade_tail(bool fade_tail) { fade_tail_ = fade_tail; }
  bool fade_tail() const { return fade_tail_; }

  bool background_is_transparent() const { return background_is_transparent_; }
  void set_background_is_transparent(bool transparent) {
    background_is_transparent_ = transparent;
  }

  const SelectionModel& selection_model() const { return selection_model_; }

  const ui::Range& selection() const { return selection_model_.selection(); }

  size_t cursor_position() const { return selection_model_.caret_pos(); }
  void SetCursorPosition(size_t position);

  // Moves the cursor left or right. Cursor movement is visual, meaning that
  // left and right are relative to screen, not the directionality of the text.
  // If |select| is false, the selection start is moved to the same position.
  void MoveCursor(BreakType break_type,
                  VisualCursorDirection direction,
                  bool select);

  // Set the selection_model_ to the value of |selection|.
  // The selection range is clamped to text().length() if out of range.
  // Returns true if the cursor position or selection range changed.
  // If any index in |selection_model| is not a cursorable position (not on a
  // grapheme boundary), it is a no-op and returns false.
  bool MoveCursorTo(const SelectionModel& selection_model);

  // Move the cursor to the position associated with the clicked point.
  // If |select| is false, the selection start is moved to the same position.
  // Returns true if the cursor position or selection range changed.
  bool MoveCursorTo(const Point& point, bool select);

  // Set the selection_model_ based on |range|.
  // If the |range| start or end is greater than text length, it is modified
  // to be the text length.
  // If the |range| start or end is not a cursorable position (not on grapheme
  // boundary), it is a NO-OP and returns false. Otherwise, returns true.
  bool SelectRange(const ui::Range& range);

  // Returns true if the local point is over selected text.
  bool IsPointInSelection(const Point& point);

  // Selects no text, keeping the current cursor position and caret affinity.
  void ClearSelection();

  // Select the entire text range. If |reversed| is true, the range will end at
  // the logical beginning of the text; this generally shows the leading portion
  // of text that overflows its display area.
  void SelectAll(bool reversed);

  // Selects the word at the current cursor position.
  void SelectWord();

  const ui::Range& GetCompositionRange() const;
  void SetCompositionRange(const ui::Range& composition_range);

  // Set the text color over the entire text or a logical character range.
  // The |range| should be valid, non-reversed, and within [0, text().length()].
  void SetColor(SkColor value);
  void ApplyColor(SkColor value, const ui::Range& range);

  // Set various text styles over the entire text or a logical character range.
  // The respective |style| is applied if |value| is true, or removed if false.
  // The |range| should be valid, non-reversed, and within [0, text().length()].
  void SetStyle(TextStyle style, bool value);
  void ApplyStyle(TextStyle style, bool value, const ui::Range& range);

  // Set the text directionality mode and get the text direction yielded.
  void SetDirectionalityMode(DirectionalityMode mode);
  base::i18n::TextDirection GetTextDirection();

  // Returns the visual movement direction corresponding to the logical end
  // of the text, considering only the dominant direction returned by
  // |GetTextDirection()|, not the direction of a particular run.
  VisualCursorDirection GetVisualDirectionOfLogicalEnd();

  // Returns the size in pixels of the entire string. For the height, this will
  // return the maximum height among the different fonts in the text runs.
  // Note that this returns the raw size of the string, which does not include
  // the margin area of text shadows.
  virtual Size GetStringSize() = 0;

  // Returns the width of content, which reserves room for the cursor if
  // |cursor_enabled_| is true.
  int GetContentWidth();

  // Returns the common baseline of the text. The returned value is the vertical
  // offset from the top of |display_rect| to the text baseline, in pixels.
  virtual int GetBaseline() = 0;

  void Draw(Canvas* canvas);

  // Draws a cursor at |position|.
  void DrawCursor(Canvas* canvas, const SelectionModel& position);

  // Draw the selected text without a cursor or selection highlight.
  void DrawSelectedText(Canvas* canvas);

  // Gets the SelectionModel from a visual point in local coordinates.
  virtual SelectionModel FindCursorPosition(const Point& point) = 0;

  // Get the visual bounds of a cursor at |selection|. These bounds typically
  // represent a vertical line, but if |insert_mode| is true they contain the
  // bounds of the associated glyph. These bounds are in local coordinates, but
  // may be outside the visible region if the text is longer than the textfield.
  // Subsequent text, cursor, or bounds changes may invalidate returned values.
  Rect GetCursorBounds(const SelectionModel& selection, bool insert_mode);

  // Compute the current cursor bounds, panning the text to show the cursor in
  // the display rect if necessary. These bounds are in local coordinates.
  // Subsequent text, cursor, or bounds changes may invalidate returned values.
  const Rect& GetUpdatedCursorBounds();

  // Given an |index| in text(), return the next or previous grapheme boundary
  // in logical order (that is, the nearest index for which
  // |IsCursorablePosition(index)| returns true). The return value is in the
  // range 0 to text().length() inclusive (the input is clamped if it is out of
  // that range). Always moves by at least one character index unless the
  // supplied index is already at the boundary of the string.
  size_t IndexOfAdjacentGrapheme(size_t index,
                                 LogicalCursorDirection direction);

  // Return a SelectionModel with the cursor at the current selection's start.
  // The returned value represents a cursor/caret position without a selection.
  SelectionModel GetSelectionModelForSelectionStart();

  // Sets shadows to drawn with text.
  void SetTextShadows(const ShadowValues& shadows);

  typedef std::pair<Font, ui::Range> FontSpan;
  // For testing purposes, returns which fonts were chosen for which parts of
  // the text by returning a vector of Font and Range pairs, where each range
  // specifies the character range for which the corresponding font has been
  // chosen.
  virtual std::vector<FontSpan> GetFontSpansForTesting() = 0;

 protected:
  RenderText();

  const BreakList<SkColor>& colors() const { return colors_; }
  const std::vector<BreakList<bool> >& styles() const { return styles_; }

  const Vector2d& GetUpdatedDisplayOffset();

  void set_cached_bounds_and_offset_valid(bool valid) {
    cached_bounds_and_offset_valid_ = valid;
  }

  // Get the selection model that visually neighbors |position| by |break_type|.
  // The returned value represents a cursor/caret position without a selection.
  SelectionModel GetAdjacentSelectionModel(const SelectionModel& current,
                                           BreakType break_type,
                                           VisualCursorDirection direction);

  // Get the selection model visually left/right of |selection| by one grapheme.
  // The returned value represents a cursor/caret position without a selection.
  virtual SelectionModel AdjacentCharSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) = 0;

  // Get the selection model visually left/right of |selection| by one word.
  // The returned value represents a cursor/caret position without a selection.
  virtual SelectionModel AdjacentWordSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) = 0;

  // Get the SelectionModels corresponding to visual text ends.
  // The returned value represents a cursor/caret position without a selection.
  SelectionModel EdgeSelectionModel(VisualCursorDirection direction);

  // Sets the selection model, the argument is assumed to be valid.
  virtual void SetSelectionModel(const SelectionModel& model);

  // Get the height and horizontal bounds (relative to the left of the text, not
  // the view) of the glyph starting at |index|. If the glyph is RTL then
  // xspan->is_reversed(). This does not return a Rect because a Rect can't have
  // a negative width.
  virtual void GetGlyphBounds(size_t index, ui::Range* xspan, int* height) = 0;

  // Get the visual bounds containing the logical substring within the |range|.
  // If |range| is empty, the result is empty. These bounds could be visually
  // discontinuous if the substring is split by a LTR/RTL level change.
  // These bounds are in local coordinates, but may be outside the visible
  // region if the text is longer than the textfield. Subsequent text, cursor,
  // or bounds changes may invalidate returned values.
  virtual std::vector<Rect> GetSubstringBounds(const ui::Range& range) = 0;

  // Convert between indices into |text_| and indices into |obscured_text_|,
  // which differ when the text is obscured. Regardless of whether or not the
  // text is obscured, the character (code point) offsets always match.
  virtual size_t TextIndexToLayoutIndex(size_t index) const = 0;
  virtual size_t LayoutIndexToTextIndex(size_t index) const = 0;

  // Return true if cursor can appear in front of the character at |position|,
  // which means it is a grapheme boundary or the first character in the text.
  virtual bool IsCursorablePosition(size_t position) = 0;

  // Reset the layout to be invalid.
  virtual void ResetLayout() = 0;

  // Ensure the text is laid out.
  virtual void EnsureLayout() = 0;

  // Draw the text.
  virtual void DrawVisualText(Canvas* canvas) = 0;

  // Returns the text used for layout, which may be |obscured_text_|.
  const string16& GetLayoutText() const;

  // Apply (and undo) temporary composition underlines and selection colors.
  void ApplyCompositionAndSelectionStyles();
  void UndoCompositionAndSelectionStyles();

  // Returns the text offset from the origin after applying text alignment and
  // display offset.
  Vector2d GetTextOffset();

  // Convert points from the text space to the view space and back.
  // Handles the display area, display offset, and the application LTR/RTL mode.
  Point ToTextPoint(const Point& point);
  Point ToViewPoint(const Point& point);

  // Returns display offset based on current text alignment.
  Vector2d GetAlignmentOffset();

  // Returns the offset for drawing text. Does not account for font
  // baseline, as needed by Skia.
  Vector2d GetOffsetForDrawing();

  // Applies fade effects to |renderer|.
  void ApplyFadeEffects(internal::SkiaTextRenderer* renderer);

  // Applies text shadows to |renderer|.
  void ApplyTextShadows(internal::SkiaTextRenderer* renderer);

  // A convenience function to check whether the glyph attached to the caret
  // is within the given range.
  static bool RangeContainsCaret(const ui::Range& range,
                                 size_t caret_pos,
                                 LogicalCursorDirection caret_affinity);

 private:
  friend class RenderTextTest;
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, DefaultStyle);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, SetColorAndStyle);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, ApplyColorAndStyle);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, ObscuredText);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, GraphemePositions);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, EdgeSelectionModels);
  FRIEND_TEST_ALL_PREFIXES(RenderTextTest, OriginForDrawing);

  // Set the cursor to |position|, with the caret trailing the previous
  // grapheme, or if there is no previous grapheme, leading the cursor position.
  // If |select| is false, the selection start is moved to the same position.
  // If the |position| is not a cursorable position (not on grapheme boundary),
  // it is a NO-OP.
  void MoveCursorTo(size_t position, bool select);

  // Updates |obscured_text_| if the text is obscured.
  void UpdateObscuredText();

  // Update the cached bounds and display offset to ensure that the current
  // cursor is within the visible display area.
  void UpdateCachedBoundsAndOffset();

  // Draw the selection.
  void DrawSelection(Canvas* canvas);

  // Logical UTF-16 string data to be drawn.
  string16 text_;

  // Horizontal alignment of the text with respect to |display_rect_|.
  HorizontalAlignment horizontal_alignment_;

  // The text directionality mode, defaults to DIRECTIONALITY_FROM_TEXT.
  DirectionalityMode directionality_mode_;

  // The cached text direction, potentially computed from the text or UI locale.
  // Use GetTextDirection(), do not use this potentially invalid value directly!
  base::i18n::TextDirection text_direction_;

  // A list of fonts used to render |text_|.
  FontList font_list_;

  // Logical selection range and visual cursor position.
  SelectionModel selection_model_;

  // The cached cursor bounds; get these bounds with GetUpdatedCursorBounds.
  Rect cursor_bounds_;

  // Specifies whether the cursor is enabled. If disabled, no space is reserved
  // for the cursor when positioning text.
  bool cursor_enabled_;

  // The cursor visibility and insert mode.
  bool cursor_visible_;
  bool insert_mode_;

  // The color used for the cursor.
  SkColor cursor_color_;

  // The color used for drawing selected text.
  SkColor selection_color_;

  // The background color used for drawing the selection when focused.
  SkColor selection_background_focused_color_;

  // The background color used for drawing the selection when not focused.
  SkColor selection_background_unfocused_color_;

  // The focus state of the text.
  bool focused_;

  // Composition text range.
  ui::Range composition_range_;

  // Color and style breaks, used to color and stylize ranges of text.
  // BreakList positions are stored with text indices, not layout indices.
  // TODO(msw): Expand to support cursor, selection, background, etc. colors.
  BreakList<SkColor> colors_;
  std::vector<BreakList<bool> > styles_;

  // Breaks saved without temporary composition and selection styling.
  BreakList<SkColor> saved_colors_;
  BreakList<bool> saved_underlines_;
  bool composition_and_selection_styles_applied_;

  // A flag and the text to display for obscured (password) fields.
  // Asterisks are used instead of the actual text glyphs when true.
  bool obscured_;
  string16 obscured_text_;

  // Fade text head and/or tail, if text doesn't fit into |display_rect_|.
  bool fade_head_;
  bool fade_tail_;

  // Is the background transparent (either partially or fully)?
  bool background_is_transparent_;

  // The local display area for rendering the text.
  Rect display_rect_;

  // Flag to work around a Skia bug with the PDF path (http://crbug.com/133548)
  // that results in incorrect clipping when drawing to the document margins.
  // This field allows disabling clipping to work around the issue.
  // TODO(asvitkine): Remove this when the underlying Skia bug is fixed.
  bool clip_to_display_rect_;

  // The offset for the text to be drawn, relative to the display area.
  // Get this point with GetUpdatedDisplayOffset (or risk using a stale value).
  Vector2d display_offset_;

  // The cached bounds and offset are invalidated by changes to the cursor,
  // selection, font, and other operations that adjust the visible text bounds.
  bool cached_bounds_and_offset_valid_;

  // Text shadows to be drawn.
  ShadowValues text_shadows_;

  DISALLOW_COPY_AND_ASSIGN(RenderText);
};

}  // namespace gfx

#endif  // UI_GFX_RENDER_TEXT_H_
