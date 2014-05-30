// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/window/custom_frame_view.h"

#include <algorithm>

#include "base/utf_string_conversions.h"
#include "grit/ui_resources.h"
#include "grit/ui_strings.h"
#include "ui/base/hit_test.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/path.h"
#include "ui/views/color_constants.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/window/client_view.h"
#include "ui/views/window/frame_background.h"
#include "ui/views/window/window_resources.h"
#include "ui/views/window/window_shape.h"

#if defined(USE_AURA)
#include "ui/views/widget/native_widget_aura.h"
#elif defined(OS_WIN)
#include "ui/views/widget/native_widget_win.h"
#endif

namespace views {

namespace {

// The frame border is only visible in restored mode and is hardcoded to 4 px on
// each side regardless of the system window border size.
const int kFrameBorderThickness = 4;
// Various edges of the frame border have a 1 px shadow along their edges; in a
// few cases we shift elements based on this amount for visual appeal.
const int kFrameShadowThickness = 1;
// While resize areas on Windows are normally the same size as the window
// borders, our top area is shrunk by 1 px to make it easier to move the window
// around with our thinner top grabbable strip.  (Incidentally, our side and
// bottom resize areas don't match the frame border thickness either -- they
// span the whole nonclient area, so there's no "dead zone" for the mouse.)
const int kTopResizeAdjust = 1;
// In the window corners, the resize areas don't actually expand bigger, but the
// 16 px at the end of each edge triggers diagonal resizing.
const int kResizeAreaCornerSize = 16;
// The titlebar never shrinks too short to show the caption button plus some
// padding below it.
const int kCaptionButtonHeightWithPadding = 19;
// The titlebar has a 2 px 3D edge along the top and bottom.
const int kTitlebarTopAndBottomEdgeThickness = 2;
// The icon is inset 2 px from the left frame border.
const int kIconLeftSpacing = 2;
// The icon never shrinks below 16 px on a side.
const int kIconMinimumSize = 16;
// The space between the window icon and the title text.
const int kTitleIconOffsetX = 4;
// The space between the title text and the caption buttons.
const int kTitleCaptionSpacing = 5;

#if defined(USE_AURA)
const SkColor kDefaultColorFrame = SkColorSetRGB(109, 109, 109);
const SkColor kDefaultColorFrameInactive = SkColorSetRGB(176, 176, 176);
#else
const SkColor kDefaultColorFrame = SkColorSetRGB(66, 116, 201);
const SkColor kDefaultColorFrameInactive = SkColorSetRGB(161, 182, 228);
#endif

const gfx::Font& GetTitleFont() {
  static gfx::Font* title_font = NULL;
  if (!title_font) {
#if defined(USE_AURA)
    title_font = new gfx::Font(NativeWidgetAura::GetWindowTitleFont());
#elif defined(OS_WIN)
    title_font = new gfx::Font(NativeWidgetWin::GetWindowTitleFont());
#elif defined(OS_LINUX)
    // TODO(ben): need to resolve what font this is.
    title_font = new gfx::Font();
#endif
  }
  return *title_font;
}

}  // namespace

///////////////////////////////////////////////////////////////////////////////
// CustomFrameView, public:

CustomFrameView::CustomFrameView()
    : frame_(NULL),
      window_icon_(NULL),
      minimize_button_(NULL),
      maximize_button_(NULL),
      restore_button_(NULL),
      close_button_(NULL),
      should_show_minmax_buttons_(false),
      frame_background_(new FrameBackground()) {
}

CustomFrameView::~CustomFrameView() {
}

void CustomFrameView::Init(Widget* frame) {
  frame_ = frame;

  close_button_ = new ImageButton(this);
  close_button_->SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_APP_ACCNAME_CLOSE));

  // Close button images will be set in LayoutWindowControls().
  AddChildView(close_button_);

  minimize_button_ = InitWindowCaptionButton(IDS_APP_ACCNAME_MINIMIZE,
      IDR_MINIMIZE, IDR_MINIMIZE_H, IDR_MINIMIZE_P);

  maximize_button_ = InitWindowCaptionButton(IDS_APP_ACCNAME_MAXIMIZE,
      IDR_MAXIMIZE, IDR_MAXIMIZE_H, IDR_MAXIMIZE_P);

  restore_button_ = InitWindowCaptionButton(IDS_APP_ACCNAME_RESTORE,
      IDR_RESTORE, IDR_RESTORE_H, IDR_RESTORE_P);

  should_show_minmax_buttons_ = frame_->widget_delegate()->CanMaximize();

  if (frame_->widget_delegate()->ShouldShowWindowIcon()) {
    window_icon_ = new ImageButton(this);
    AddChildView(window_icon_);
  }
}

///////////////////////////////////////////////////////////////////////////////
// CustomFrameView, NonClientFrameView implementation:

gfx::Rect CustomFrameView::GetBoundsForClientView() const {
  return client_view_bounds_;
}

gfx::Rect CustomFrameView::GetWindowBoundsForClientBounds(
    const gfx::Rect& client_bounds) const {
  int top_height = NonClientTopBorderHeight();
  int border_thickness = NonClientBorderThickness();
  return gfx::Rect(std::max(0, client_bounds.x() - border_thickness),
                   std::max(0, client_bounds.y() - top_height),
                   client_bounds.width() + (2 * border_thickness),
                   client_bounds.height() + top_height + border_thickness);
}

int CustomFrameView::NonClientHitTest(const gfx::Point& point) {
  // Sanity check.
  if (!bounds().Contains(point))
    return HTNOWHERE;

  int frame_component = frame_->client_view()->NonClientHitTest(point);

  // See if we're in the sysmenu region.  (We check the ClientView first to be
  // consistent with OpaqueBrowserFrameView; it's not really necessary here.)
  gfx::Rect sysmenu_rect(IconBounds());
  // In maximized mode we extend the rect to the screen corner to take advantage
  // of Fitts' Law.
  if (frame_->IsMaximized())
    sysmenu_rect.SetRect(0, 0, sysmenu_rect.right(), sysmenu_rect.bottom());
  sysmenu_rect.set_x(GetMirroredXForRect(sysmenu_rect));
  if (sysmenu_rect.Contains(point))
    return (frame_component == HTCLIENT) ? HTCLIENT : HTSYSMENU;

  if (frame_component != HTNOWHERE)
    return frame_component;

  // Then see if the point is within any of the window controls.
  if (close_button_->GetMirroredBounds().Contains(point))
    return HTCLOSE;
  if (restore_button_->GetMirroredBounds().Contains(point))
    return HTMAXBUTTON;
  if (maximize_button_->GetMirroredBounds().Contains(point))
    return HTMAXBUTTON;
  if (minimize_button_->GetMirroredBounds().Contains(point))
    return HTMINBUTTON;
  if (window_icon_ && window_icon_->GetMirroredBounds().Contains(point))
    return HTSYSMENU;

  int window_component = GetHTComponentForFrame(point, FrameBorderThickness(),
      NonClientBorderThickness(), kResizeAreaCornerSize, kResizeAreaCornerSize,
      frame_->widget_delegate()->CanResize());
  // Fall back to the caption if no other component matches.
  return (window_component == HTNOWHERE) ? HTCAPTION : window_component;
}

void CustomFrameView::GetWindowMask(const gfx::Size& size,
                                    gfx::Path* window_mask) {
  DCHECK(window_mask);
  if (frame_->IsMaximized())
    return;

  GetDefaultWindowMask(size, window_mask);
}

void CustomFrameView::ResetWindowControls() {
  restore_button_->SetState(CustomButton::STATE_NORMAL);
  minimize_button_->SetState(CustomButton::STATE_NORMAL);
  maximize_button_->SetState(CustomButton::STATE_NORMAL);
  // The close button isn't affected by this constraint.
}

void CustomFrameView::UpdateWindowIcon() {
  if (window_icon_)
    window_icon_->SchedulePaint();
}

void CustomFrameView::UpdateWindowTitle() {
  SchedulePaintInRect(title_bounds_);
}

///////////////////////////////////////////////////////////////////////////////
// CustomFrameView, View overrides:

void CustomFrameView::OnPaint(gfx::Canvas* canvas) {
  if (frame_->IsMaximized())
    PaintMaximizedFrameBorder(canvas);
  else
    PaintRestoredFrameBorder(canvas);
  PaintTitleBar(canvas);
  if (ShouldShowClientEdge())
    PaintRestoredClientEdge(canvas);
}

void CustomFrameView::Layout() {
  LayoutWindowControls();
  LayoutTitleBar();
  LayoutClientView();
}

gfx::Size CustomFrameView::GetPreferredSize() {
  return frame_->non_client_view()->GetWindowBoundsForClientBounds(
      gfx::Rect(frame_->client_view()->GetPreferredSize())).size();
}

///////////////////////////////////////////////////////////////////////////////
// CustomFrameView, ButtonListener implementation:

void CustomFrameView::ButtonPressed(Button* sender, const ui::Event& event) {
  if (sender == close_button_)
    frame_->Close();
  else if (sender == minimize_button_)
    frame_->Minimize();
  else if (sender == maximize_button_)
    frame_->Maximize();
  else if (sender == restore_button_)
    frame_->Restore();
}

///////////////////////////////////////////////////////////////////////////////
// CustomFrameView, private:

int CustomFrameView::FrameBorderThickness() const {
  return frame_->IsMaximized() ? 0 : kFrameBorderThickness;
}

int CustomFrameView::NonClientBorderThickness() const {
  // In maximized mode, we don't show a client edge.
  return FrameBorderThickness() +
      (ShouldShowClientEdge() ? kClientEdgeThickness : 0);
}

int CustomFrameView::NonClientTopBorderHeight() const {
  return std::max(FrameBorderThickness() + IconSize(),
                  CaptionButtonY() + kCaptionButtonHeightWithPadding) +
      TitlebarBottomThickness();
}

int CustomFrameView::CaptionButtonY() const {
  // Maximized buttons start at window top so that even if their images aren't
  // drawn flush with the screen edge, they still obey Fitts' Law.
  return frame_->IsMaximized() ? FrameBorderThickness() : kFrameShadowThickness;
}

int CustomFrameView::TitlebarBottomThickness() const {
  return kTitlebarTopAndBottomEdgeThickness +
      (ShouldShowClientEdge() ? kClientEdgeThickness : 0);
}

int CustomFrameView::IconSize() const {
#if defined(OS_WIN)
  // This metric scales up if either the titlebar height or the titlebar font
  // size are increased.
  return GetSystemMetrics(SM_CYSMICON);
#else
  return std::max(GetTitleFont().GetHeight(), kIconMinimumSize);
#endif
}

gfx::Rect CustomFrameView::IconBounds() const {
  int size = IconSize();
  int frame_thickness = FrameBorderThickness();
  // Our frame border has a different "3D look" than Windows'.  Theirs has a
  // more complex gradient on the top that they push their icon/title below;
  // then the maximized window cuts this off and the icon/title are centered
  // in the remaining space.  Because the apparent shape of our border is
  // simpler, using the same positioning makes things look slightly uncentered
  // with restored windows, so when the window is restored, instead of
  // calculating the remaining space from below the frame border, we calculate
  // from below the 3D edge.
  int unavailable_px_at_top = frame_->IsMaximized() ?
      frame_thickness : kTitlebarTopAndBottomEdgeThickness;
  // When the icon is shorter than the minimum space we reserve for the caption
  // button, we vertically center it.  We want to bias rounding to put extra
  // space above the icon, since the 3D edge (+ client edge, for restored
  // windows) below looks (to the eye) more like additional space than does the
  // 3D edge (or nothing at all, for maximized windows) above; hence the +1.
  int y = unavailable_px_at_top + (NonClientTopBorderHeight() -
      unavailable_px_at_top - size - TitlebarBottomThickness() + 1) / 2;
  return gfx::Rect(frame_thickness + kIconLeftSpacing, y, size, size);
}

bool CustomFrameView::ShouldShowClientEdge() const {
  return !frame_->IsMaximized();
}

void CustomFrameView::PaintRestoredFrameBorder(gfx::Canvas* canvas) {
  frame_background_->set_frame_color(GetFrameColor());
  const gfx::ImageSkia* frame_image = GetFrameImage();
  frame_background_->set_theme_image(frame_image);
  frame_background_->set_top_area_height(frame_image->height());

  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();

  frame_background_->SetCornerImages(
      rb.GetImageNamed(IDR_WINDOW_TOP_LEFT_CORNER).ToImageSkia(),
      rb.GetImageNamed(IDR_WINDOW_TOP_RIGHT_CORNER).ToImageSkia(),
      rb.GetImageNamed(IDR_WINDOW_BOTTOM_LEFT_CORNER).ToImageSkia(),
      rb.GetImageNamed(IDR_WINDOW_BOTTOM_RIGHT_CORNER).ToImageSkia());
  frame_background_->SetSideImages(
      rb.GetImageNamed(IDR_WINDOW_LEFT_SIDE).ToImageSkia(),
      rb.GetImageNamed(IDR_WINDOW_TOP_CENTER).ToImageSkia(),
      rb.GetImageNamed(IDR_WINDOW_RIGHT_SIDE).ToImageSkia(),
      rb.GetImageNamed(IDR_WINDOW_BOTTOM_CENTER).ToImageSkia());

  frame_background_->PaintRestored(canvas, this);
}

void CustomFrameView::PaintMaximizedFrameBorder(gfx::Canvas* canvas) {
  const gfx::ImageSkia* frame_image = GetFrameImage();
  frame_background_->set_theme_image(frame_image);
  frame_background_->set_top_area_height(frame_image->height());
  frame_background_->PaintMaximized(canvas, this);

  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();

  // TODO(jamescook): Migrate this into FrameBackground.
  // The bottom of the titlebar actually comes from the top of the Client Edge
  // graphic, with the actual client edge clipped off the bottom.
  const gfx::ImageSkia* titlebar_bottom = rb.GetImageNamed(
      IDR_APP_TOP_CENTER).ToImageSkia();
  int edge_height = titlebar_bottom->height() -
      (ShouldShowClientEdge() ? kClientEdgeThickness : 0);
  canvas->TileImageInt(*titlebar_bottom, 0,
      frame_->client_view()->y() - edge_height, width(), edge_height);
}

void CustomFrameView::PaintTitleBar(gfx::Canvas* canvas) {
  WidgetDelegate* delegate = frame_->widget_delegate();

  // It seems like in some conditions we can be asked to paint after the window
  // that contains us is WM_DESTROYed. At this point, our delegate is NULL. The
  // correct long term fix may be to shut down the RootView in WM_DESTROY.
  if (!delegate)
    return;

  canvas->DrawStringInt(delegate->GetWindowTitle(), GetTitleFont(),
                        SK_ColorWHITE, GetMirroredXForRect(title_bounds_),
                        title_bounds_.y(), title_bounds_.width(),
                        title_bounds_.height());
}

void CustomFrameView::PaintRestoredClientEdge(gfx::Canvas* canvas) {
  gfx::Rect client_area_bounds = frame_->client_view()->bounds();
  int client_area_top = client_area_bounds.y();

  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  const gfx::ImageSkia* top_left = rb.GetImageNamed(
      IDR_APP_TOP_LEFT).ToImageSkia();
  const gfx::ImageSkia* top = rb.GetImageNamed(
      IDR_APP_TOP_CENTER).ToImageSkia();
  const gfx::ImageSkia* top_right = rb.GetImageNamed(
      IDR_APP_TOP_RIGHT).ToImageSkia();
  const gfx::ImageSkia* right = rb.GetImageNamed(
      IDR_CONTENT_RIGHT_SIDE).ToImageSkia();
  const gfx::ImageSkia* bottom_right = rb.GetImageNamed(
      IDR_CONTENT_BOTTOM_RIGHT_CORNER).ToImageSkia();
  const gfx::ImageSkia* bottom = rb.GetImageNamed(
      IDR_CONTENT_BOTTOM_CENTER).ToImageSkia();
  const gfx::ImageSkia* bottom_left = rb.GetImageNamed(
      IDR_CONTENT_BOTTOM_LEFT_CORNER).ToImageSkia();
  const gfx::ImageSkia* left = rb.GetImageNamed(
      IDR_CONTENT_LEFT_SIDE).ToImageSkia();

  // Top.
  int top_edge_y = client_area_top - top->height();
  canvas->DrawImageInt(*top_left, client_area_bounds.x() - top_left->width(),
                       top_edge_y);
  canvas->TileImageInt(*top, client_area_bounds.x(), top_edge_y,
                       client_area_bounds.width(), top->height());
  canvas->DrawImageInt(*top_right, client_area_bounds.right(), top_edge_y);

  // Right.
  int client_area_bottom =
      std::max(client_area_top, client_area_bounds.bottom());
  int client_area_height = client_area_bottom - client_area_top;
  canvas->TileImageInt(*right, client_area_bounds.right(), client_area_top,
                       right->width(), client_area_height);

  // Bottom.
  canvas->DrawImageInt(*bottom_right, client_area_bounds.right(),
                       client_area_bottom);
  canvas->TileImageInt(*bottom, client_area_bounds.x(), client_area_bottom,
                       client_area_bounds.width(), bottom_right->height());
  canvas->DrawImageInt(*bottom_left,
      client_area_bounds.x() - bottom_left->width(), client_area_bottom);

  // Left.
  canvas->TileImageInt(*left, client_area_bounds.x() - left->width(),
      client_area_top, left->width(), client_area_height);

  // Draw the color to fill in the edges.
  canvas->FillRect(gfx::Rect(client_area_bounds.x() - 1, client_area_top - 1,
      client_area_bounds.width() + 1, client_area_bottom - client_area_top + 1),
      kClientEdgeColor);
}

SkColor CustomFrameView::GetFrameColor() const {
  return frame_->IsActive() ? kDefaultColorFrame : kDefaultColorFrameInactive;
}

const gfx::ImageSkia* CustomFrameView::GetFrameImage() const {
  return ui::ResourceBundle::GetSharedInstance().GetImageNamed(
      frame_->IsActive() ? IDR_FRAME : IDR_FRAME_INACTIVE).ToImageSkia();
}

void CustomFrameView::LayoutWindowControls() {
  close_button_->SetImageAlignment(ImageButton::ALIGN_LEFT,
                                   ImageButton::ALIGN_BOTTOM);
  int caption_y = CaptionButtonY();
  bool is_maximized = frame_->IsMaximized();
  // There should always be the same number of non-shadow pixels visible to the
  // side of the caption buttons.  In maximized mode we extend the rightmost
  // button to the screen corner to obey Fitts' Law.
  int right_extra_width = is_maximized ?
      (kFrameBorderThickness - kFrameShadowThickness) : 0;
  gfx::Size close_button_size = close_button_->GetPreferredSize();
  close_button_->SetBounds(width() - FrameBorderThickness() -
      right_extra_width - close_button_size.width(), caption_y,
      close_button_size.width() + right_extra_width,
      close_button_size.height());

  // When the window is restored, we show a maximized button; otherwise, we show
  // a restore button.
  bool is_restored = !is_maximized && !frame_->IsMinimized();
  ImageButton* invisible_button = is_restored ? restore_button_
                                              : maximize_button_;
  invisible_button->SetVisible(false);

  ImageButton* visible_button = is_restored ? maximize_button_
                                            : restore_button_;
  FramePartImage normal_part, hot_part, pushed_part;
  if (should_show_minmax_buttons_) {
    visible_button->SetVisible(true);
    visible_button->SetImageAlignment(ImageButton::ALIGN_LEFT,
                                      ImageButton::ALIGN_BOTTOM);
    gfx::Size visible_button_size = visible_button->GetPreferredSize();
    visible_button->SetBounds(close_button_->x() - visible_button_size.width(),
                              caption_y, visible_button_size.width(),
                              visible_button_size.height());

    minimize_button_->SetVisible(true);
    minimize_button_->SetImageAlignment(ImageButton::ALIGN_LEFT,
                                        ImageButton::ALIGN_BOTTOM);
    gfx::Size minimize_button_size = minimize_button_->GetPreferredSize();
    minimize_button_->SetBounds(
        visible_button->x() - minimize_button_size.width(), caption_y,
        minimize_button_size.width(),
        minimize_button_size.height());

    normal_part = IDR_CLOSE;
    hot_part = IDR_CLOSE_H;
    pushed_part = IDR_CLOSE_P;
  } else {
    visible_button->SetVisible(false);
    minimize_button_->SetVisible(false);

    normal_part = IDR_CLOSE_SA;
    hot_part = IDR_CLOSE_SA_H;
    pushed_part = IDR_CLOSE_SA_P;
  }

  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();

  close_button_->SetImage(CustomButton::STATE_NORMAL,
                          rb.GetImageNamed(normal_part).ToImageSkia());
  close_button_->SetImage(CustomButton::STATE_HOVERED,
                          rb.GetImageNamed(hot_part).ToImageSkia());
  close_button_->SetImage(CustomButton::STATE_PRESSED,
                          rb.GetImageNamed(pushed_part).ToImageSkia());
}

void CustomFrameView::LayoutTitleBar() {
  // The window title position is calculated based on the icon position, even
  // when there is no icon.
  gfx::Rect icon_bounds(IconBounds());
  bool show_window_icon = window_icon_ != NULL;
  if (show_window_icon)
    window_icon_->SetBoundsRect(icon_bounds);

  // The offset between the window left edge and the title text.
  int title_x = show_window_icon ? icon_bounds.right() + kTitleIconOffsetX
                                 : icon_bounds.x();
  int title_height = GetTitleFont().GetHeight();
  // We bias the title position so that when the difference between the icon and
  // title heights is odd, the extra pixel of the title is above the vertical
  // midline rather than below.  This compensates for how the icon is already
  // biased downwards (see IconBounds()) and helps prevent descenders on the
  // title from overlapping the 3D edge at the bottom of the titlebar.
  title_bounds_.SetRect(title_x,
      icon_bounds.y() + ((icon_bounds.height() - title_height - 1) / 2),
      std::max(0, (should_show_minmax_buttons_ ?
          minimize_button_->x() : close_button_->x()) - kTitleCaptionSpacing -
      title_x), title_height);
}

void CustomFrameView::LayoutClientView() {
  int top_height = NonClientTopBorderHeight();
  int border_thickness = NonClientBorderThickness();
  client_view_bounds_.SetRect(border_thickness, top_height,
      std::max(0, width() - (2 * border_thickness)),
      std::max(0, height() - top_height - border_thickness));
}

ImageButton* CustomFrameView::InitWindowCaptionButton(
    int accessibility_string_id,
    int normal_image_id,
    int hot_image_id,
    int pushed_image_id) {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  ImageButton* button = new ImageButton(this);
  button->SetAccessibleName(l10n_util::GetStringUTF16(accessibility_string_id));
  button->SetImage(CustomButton::STATE_NORMAL,
                   rb.GetImageNamed(normal_image_id).ToImageSkia());
  button->SetImage(CustomButton::STATE_HOVERED,
                   rb.GetImageNamed(hot_image_id).ToImageSkia());
  button->SetImage(CustomButton::STATE_PRESSED,
                   rb.GetImageNamed(pushed_image_id).ToImageSkia());
  AddChildView(button);
  return button;
}

}  // namespace views
