// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/tooltip_controller.h"

#include <vector>

#include "base/command_line.h"
#include "base/location.h"
#include "base/strings/string_split.h"
#include "base/time.h"
#include "ui/aura/client/cursor_client.h"
#include "ui/aura/client/drag_drop_client.h"
#include "ui/aura/env.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window.h"
#include "ui/base/events/event.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/text/text_elider.h"
#include "ui/gfx/font.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/screen.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/controls/label.h"
#include "ui/views/corewm/corewm_switches.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_observer.h"

namespace {

const SkColor kTooltipBackground = 0xFFFFFFCC;
const SkColor kTooltipBorder = 0xFF646450;
const int kTooltipBorderWidth = 1;
const int kTooltipHorizontalPadding = 3;

// Max visual tooltip width. If a tooltip is greater than this width, it will
// be wrapped.
const int kTooltipMaxWidthPixels = 400;

// Maximum number of lines we allow in the tooltip.
const size_t kMaxLines = 10;

// TODO(derat): This padding is needed on Chrome OS devices but seems excessive
// when running the same binary on a Linux workstation; presumably there's a
// difference in font metrics.  Rationalize this.
const int kTooltipVerticalPadding = 2;
const int kTooltipTimeoutMs = 500;
const int kTooltipShownTimeoutMs = 10000;

// FIXME: get cursor offset from actual cursor size.
const int kCursorOffsetX = 10;
const int kCursorOffsetY = 15;

// Maximum number of characters we allow in a tooltip.
const size_t kMaxTooltipLength = 1024;

gfx::Font GetDefaultFont() {
  // TODO(varunjain): implementation duplicated in tooltip_manager_aura. Figure
  // out a way to merge.
  return ui::ResourceBundle::GetSharedInstance().GetFont(
      ui::ResourceBundle::BaseFont);
}

// Creates a widget of type TYPE_TOOLTIP
views::Widget* CreateTooltip(aura::Window* tooltip_window) {
  views::Widget* widget = new views::Widget;
  views::Widget::InitParams params;
  // For aura, since we set the type to TOOLTIP_TYPE, the widget will get
  // auto-parented to the MenuAndTooltipsContainer.
  params.type = views::Widget::InitParams::TYPE_TOOLTIP;
  params.context = tooltip_window;
  DCHECK(params.context);
  params.keep_on_top = true;
  params.accept_events = false;
  widget->Init(params);
  return widget;
}

}  // namespace

namespace views {
namespace corewm {

// Displays a widget with tooltip using a views::Label.
class TooltipController::Tooltip : public views::WidgetObserver {
 public:
  Tooltip(TooltipController* controller)
      : controller_(controller),
        widget_(NULL) {
    label_.set_background(
        views::Background::CreateSolidBackground(kTooltipBackground));
    if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kNoDropShadows)) {
      label_.set_border(
          views::Border::CreateSolidBorder(kTooltipBorderWidth,
                                           kTooltipBorder));
    }
    label_.set_owned_by_client();
  }

  virtual ~Tooltip() {
    if (widget_) {
      widget_->RemoveObserver(this);
      widget_->Close();
    }
  }

  // Updates the text on the tooltip and resizes to fit.
  void SetText(aura::Window* window,
               const string16& tooltip_text,
               const gfx::Point& location) {
    int max_width, line_count;
    string16 trimmed_text(tooltip_text);
    controller_->TrimTooltipToFit(
        controller_->GetMaxWidth(location), &trimmed_text, &max_width,
        &line_count);
    label_.SetText(trimmed_text);

    int width = max_width + 2 * kTooltipHorizontalPadding;
    int height = label_.GetPreferredSize().height() +
        2 * kTooltipVerticalPadding;
    if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kNoDropShadows)) {
      width += 2 * kTooltipBorderWidth;
      height += 2 * kTooltipBorderWidth;
    }
    CreateWidgetIfNecessary(window);
    SetTooltipBounds(location, width, height);
  }

  // Shows the tooltip.
  void Show() {
    if (widget_)
      widget_->Show();
  }

  // Hides the tooltip.
  void Hide() {
    if (widget_)
      widget_->Hide();
  }

  bool IsVisible() {
    return widget_ && widget_->IsVisible();
  }

  // Overriden from views::WidgetObserver.
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE {
    DCHECK_EQ(widget_, widget);
    widget_ = NULL;
  }

 private:
  // Adjusts the bounds given by the arguments to fit inside the desktop
  // and applies the adjusted bounds to the label_.
  void SetTooltipBounds(const gfx::Point& mouse_pos,
                        int tooltip_width,
                        int tooltip_height) {
    gfx::Rect tooltip_rect(mouse_pos.x(), mouse_pos.y(), tooltip_width,
                           tooltip_height);

    tooltip_rect.Offset(kCursorOffsetX, kCursorOffsetY);
    gfx::Rect display_bounds = controller_->GetBoundsForTooltip(mouse_pos);

    // If tooltip is out of bounds on the x axis, we simply shift it
    // horizontally by the offset.
    if (tooltip_rect.right() > display_bounds.right()) {
      int h_offset = tooltip_rect.right() - display_bounds.right();
      tooltip_rect.Offset(-h_offset, 0);
    }

    // If tooltip is out of bounds on the y axis, we flip it to appear above the
    // mouse cursor instead of below.
    if (tooltip_rect.bottom() > display_bounds.bottom())
      tooltip_rect.set_y(mouse_pos.y() - tooltip_height);

    tooltip_rect.AdjustToFit(display_bounds);
    widget_->SetBounds(tooltip_rect);
  }

  void CreateWidgetIfNecessary(aura::Window* tooltip_window) {
    if (widget_)
      return;
    widget_ = CreateTooltip(tooltip_window);
    widget_->SetContentsView(&label_);
    widget_->AddObserver(this);
  }

  views::Label label_;
  TooltipController* controller_;
  views::Widget* widget_;

  DISALLOW_COPY_AND_ASSIGN(Tooltip);
};

////////////////////////////////////////////////////////////////////////////////
// TooltipController public:

TooltipController::TooltipController(gfx::ScreenType screen_type)
    : screen_type_(screen_type),
      tooltip_window_(NULL),
      tooltip_window_at_mouse_press_(NULL),
      mouse_pressed_(false),
      tooltips_enabled_(true) {
  tooltip_timer_.Start(FROM_HERE,
      base::TimeDelta::FromMilliseconds(kTooltipTimeoutMs),
      this, &TooltipController::TooltipTimerFired);
}

TooltipController::~TooltipController() {
  if (tooltip_window_)
    tooltip_window_->RemoveObserver(this);
}

void TooltipController::UpdateTooltip(aura::Window* target) {
  // If tooltip is visible, we may want to hide it. If it is not, we are ok.
  if (tooltip_window_ == target && GetTooltip()->IsVisible())
    UpdateIfRequired();

  // If we had stopped the tooltip timer for some reason, we must restart it if
  // there is a change in the tooltip.
  if (!tooltip_timer_.IsRunning()) {
    if (tooltip_window_ != target || (tooltip_window_ &&
        tooltip_text_ != aura::client::GetTooltipText(tooltip_window_))) {
      tooltip_timer_.Start(FROM_HERE,
          base::TimeDelta::FromMilliseconds(kTooltipTimeoutMs),
          this, &TooltipController::TooltipTimerFired);
    }
  }
}

void TooltipController::SetTooltipsEnabled(bool enable) {
  if (tooltips_enabled_ == enable)
    return;
  tooltips_enabled_ = enable;
  UpdateTooltip(tooltip_window_);
}

void TooltipController::OnKeyEvent(ui::KeyEvent* event) {
  // On key press, we want to hide the tooltip and not show it until change.
  // This is the same behavior as hiding tooltips on timeout. Hence, we can
  // simply simulate a timeout.
  if (tooltip_shown_timer_.IsRunning()) {
    tooltip_shown_timer_.Stop();
    TooltipShownTimerFired();
  }
}

void TooltipController::OnMouseEvent(ui::MouseEvent* event) {
  aura::Window* target = static_cast<aura::Window*>(event->target());
  switch (event->type()) {
    case ui::ET_MOUSE_MOVED:
    case ui::ET_MOUSE_DRAGGED:
      if (tooltip_window_ != target) {
        if (tooltip_window_)
          tooltip_window_->RemoveObserver(this);
        tooltip_window_ = target;
        tooltip_window_->AddObserver(this);
      }
      curr_mouse_loc_ = event->location();
      if (tooltip_timer_.IsRunning())
        tooltip_timer_.Reset();

      if (GetTooltip()->IsVisible())
        UpdateIfRequired();
      break;
    case ui::ET_MOUSE_PRESSED:
      if ((event->flags() & ui::EF_IS_NON_CLIENT) == 0) {
        // We don't get a release for non-client areas.
        mouse_pressed_ = true;
        tooltip_window_at_mouse_press_ = target;
        if (target)
          tooltip_text_at_mouse_press_ = aura::client::GetTooltipText(target);
      }
      GetTooltip()->Hide();
      break;
    case ui::ET_MOUSE_RELEASED:
      mouse_pressed_ = false;
      break;
    case ui::ET_MOUSE_CAPTURE_CHANGED:
      // We will not received a mouse release, so reset mouse pressed state.
      mouse_pressed_ = false;
    case ui::ET_MOUSEWHEEL:
      // Hide the tooltip for click, release, drag, wheel events.
      if (GetTooltip()->IsVisible())
        GetTooltip()->Hide();
      break;
    default:
      break;
  }
}

void TooltipController::OnTouchEvent(ui::TouchEvent* event) {
  // TODO(varunjain): need to properly implement tooltips for
  // touch events.
  // Hide the tooltip for touch events.
  if (GetTooltip()->IsVisible())
    GetTooltip()->Hide();
  if (tooltip_window_)
    tooltip_window_->RemoveObserver(this);
  tooltip_window_ = NULL;
}

void TooltipController::OnCancelMode(ui::CancelModeEvent* event) {
  if (tooltip_.get() && tooltip_->IsVisible())
    tooltip_->Hide();
}

void TooltipController::OnWindowDestroyed(aura::Window* window) {
  if (tooltip_window_ == window) {
    tooltip_window_->RemoveObserver(this);
    tooltip_window_ = NULL;
  }
}

////////////////////////////////////////////////////////////////////////////////
// TooltipController private:

int TooltipController::GetMaxWidth(const gfx::Point& location) const {
  // TODO(varunjain): implementation duplicated in tooltip_manager_aura. Figure
  // out a way to merge.
  gfx::Rect display_bounds = GetBoundsForTooltip(location);
  return (display_bounds.width() + 1) / 2;
}

gfx::Rect TooltipController::GetBoundsForTooltip(
    const gfx::Point& origin) const {
  DCHECK(tooltip_window_);
  gfx::Rect widget_bounds;
  // For Desktop aura we constrain the tooltip to the bounds of the Widget
  // (which comes from the RootWindow).
  if (screen_type_ == gfx::SCREEN_TYPE_NATIVE &&
      gfx::SCREEN_TYPE_NATIVE != gfx::SCREEN_TYPE_ALTERNATE) {
    aura::RootWindow* root = tooltip_window_->GetRootWindow();
    widget_bounds = gfx::Rect(root->GetHostOrigin(), root->GetHostSize());
  }
  gfx::Screen* screen = gfx::Screen::GetScreenByType(screen_type_);
  gfx::Rect bounds(screen->GetDisplayNearestPoint(origin).bounds());
  if (!widget_bounds.IsEmpty())
    bounds.Intersect(widget_bounds);
  return bounds;
}

// static
void TooltipController::TrimTooltipToFit(int max_width,
                                         string16* text,
                                         int* width,
                                         int* line_count) {
  *width = 0;
  *line_count = 0;

  // Clamp the tooltip length to kMaxTooltipLength so that we don't
  // accidentally DOS the user with a mega tooltip.
  if (text->length() > kMaxTooltipLength)
    *text = text->substr(0, kMaxTooltipLength);

  // Determine the available width for the tooltip.
  int available_width = std::min(kTooltipMaxWidthPixels, max_width);

  std::vector<string16> lines;
  base::SplitString(*text, '\n', &lines);
  std::vector<string16> result_lines;

  // Format each line to fit.
  gfx::Font font = GetDefaultFont();
  for (std::vector<string16>::iterator l = lines.begin(); l != lines.end();
      ++l) {
    // We break the line at word boundaries, then stuff as many words as we can
    // in the available width to the current line, and move the remaining words
    // to a new line.
    std::vector<string16> words;
    base::SplitStringDontTrim(*l, ' ', &words);
    int current_width = 0;
    string16 line;
    for (std::vector<string16>::iterator w = words.begin(); w != words.end();
        ++w) {
      string16 word = *w;
      if (w + 1 != words.end())
        word.push_back(' ');
      int word_width = font.GetStringWidth(word);
      if (current_width + word_width > available_width) {
        // Current width will exceed the available width. Must start a new line.
        if (!line.empty())
          result_lines.push_back(line);
        current_width = 0;
        line.clear();
      }
      current_width += word_width;
      line.append(word);
    }
    result_lines.push_back(line);
  }

  // Clamp number of lines to |kMaxLines|.
  if (result_lines.size() > kMaxLines) {
    result_lines.resize(kMaxLines);
    // Add ellipses character to last line.
    result_lines[kMaxLines - 1] = ui::TruncateString(
        result_lines.back(), result_lines.back().length() - 1);
  }
  *line_count = result_lines.size();

  // Flatten the result.
  string16 result;
  for (std::vector<string16>::iterator l = result_lines.begin();
      l != result_lines.end(); ++l) {
    if (!result.empty())
      result.push_back('\n');
    int line_width = font.GetStringWidth(*l);
    // Since we only break at word boundaries, it could happen that due to some
    // very long word, line_width is greater than the available_width. In such
    // case, we simply truncate at available_width and add ellipses at the end.
    if (line_width > available_width) {
      *width = available_width;
      result.append(ui::ElideText(*l, font, available_width, ui::ELIDE_AT_END));
    } else {
      *width = std::max(*width, line_width);
      result.append(*l);
    }
  }
  *text = result;
}

void TooltipController::TooltipTimerFired() {
  UpdateIfRequired();
}

void TooltipController::TooltipShownTimerFired() {
  GetTooltip()->Hide();

  // Since the user presumably no longer needs the tooltip, we also stop the
  // tooltip timer so that tooltip does not pop back up. We will restart this
  // timer if the tooltip changes (see UpdateTooltip()).
  tooltip_timer_.Stop();
}

void TooltipController::UpdateIfRequired() {
  if (!tooltips_enabled_ || mouse_pressed_ || IsDragDropInProgress() ||
      !IsCursorVisible()) {
    GetTooltip()->Hide();
    return;
  }

  string16 tooltip_text;
  if (tooltip_window_)
    tooltip_text = aura::client::GetTooltipText(tooltip_window_);

  // If the user pressed a mouse button. We will hide the tooltip and not show
  // it until there is a change in the tooltip.
  if (tooltip_window_at_mouse_press_) {
    if (tooltip_window_ == tooltip_window_at_mouse_press_ &&
        tooltip_text == tooltip_text_at_mouse_press_) {
      GetTooltip()->Hide();
      return;
    }
    tooltip_window_at_mouse_press_ = NULL;
  }

  // We add the !GetTooltip()->IsVisible() below because when we come here from
  // TooltipTimerFired(), the tooltip_text may not have changed but we still
  // want to update the tooltip because the timer has fired.
  // If we come here from UpdateTooltip(), we have already checked for tooltip
  // visibility and this check below will have no effect.
  if (tooltip_text_ != tooltip_text || !GetTooltip()->IsVisible()) {
    tooltip_shown_timer_.Stop();
    tooltip_text_ = tooltip_text;
    if (tooltip_text_.empty()) {
      GetTooltip()->Hide();
    } else {
      gfx::Point widget_loc = curr_mouse_loc_ +
          tooltip_window_->GetBoundsInScreen().OffsetFromOrigin();
      gfx::Rect bounds(GetBoundsForTooltip(widget_loc));
      if (bounds.IsEmpty()) {
        tooltip_text_.clear();
        GetTooltip()->Hide();
      } else {
        GetTooltip()->SetText(tooltip_window_, tooltip_text_, widget_loc);
        GetTooltip()->Show();
        tooltip_shown_timer_.Start(FROM_HERE,
              base::TimeDelta::FromMilliseconds(kTooltipShownTimeoutMs),
              this, &TooltipController::TooltipShownTimerFired);
      }
    }
  }
}

bool TooltipController::IsTooltipVisible() {
  return GetTooltip()->IsVisible();
}

bool TooltipController::IsDragDropInProgress() {
  if (!tooltip_window_)
    return false;
  aura::client::DragDropClient* client =
      aura::client::GetDragDropClient(tooltip_window_->GetRootWindow());
  return client && client->IsDragDropInProgress();
}

TooltipController::Tooltip* TooltipController::GetTooltip() {
  if (!tooltip_.get())
    tooltip_.reset(new Tooltip(this));
  return tooltip_.get();
}

bool TooltipController::IsCursorVisible() {
  if (!tooltip_window_)
    return false;
  aura::RootWindow* root = tooltip_window_->GetRootWindow();
  if (!root)
    return false;
  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(root);
  // |cursor_client| may be NULL in tests, treat NULL as always visible.
  return !cursor_client || cursor_client->IsCursorVisible();
}

}  // namespace corewm
}  // namespace views
