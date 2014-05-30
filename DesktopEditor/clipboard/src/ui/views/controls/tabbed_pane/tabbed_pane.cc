// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/tabbed_pane/tabbed_pane.h"

#include "base/logging.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane_listener.h"
#include "ui/views/layout/layout_manager.h"
#include "ui/views/widget/widget.h"

namespace {

// TODO(markusheintz|msw): Use NativeTheme colors.
const SkColor kTabTitleColor_Inactive = SkColorSetRGB(0x66, 0x66, 0x66);
const SkColor kTabTitleColor_Active = SkColorSetRGB(0x20, 0x20, 0x20);
const SkColor kTabTitleColor_Pressed = SkColorSetRGB(0x33, 0x33, 0x33);
const SkColor kTabTitleColor_Hovered = SkColorSetRGB(0x22, 0x22, 0x22);
const SkColor kTabBackgroundColor = SK_ColorWHITE;
const SkColor kTabBorderColor = SkColorSetRGB(0xCC, 0xCC, 0xCC);
const SkScalar kTabBorderThickness = 1.0f;
const SkScalar kTabBorderRadius = 2.0f;

}  // namespace

namespace views {

// static
const char TabbedPane::kViewClassName[] = "views/TabbedPane";

// The tab view shown in the tab strip.
class Tab : public View {
 public:
  Tab(TabbedPane* tabbed_pane, const string16& title, View* contents);
  virtual ~Tab();

  View* contents() const { return contents_; }

  bool selected() const { return contents_->visible(); }
  void SetSelected(bool selected);

  // Overridden from View:
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  void SetTitleColor(SkColor color);
  void PaintTabBorder(gfx::Canvas* canvas);

  TabbedPane* tabbed_pane_;
  Label* title_;
  // The content view associated with this tab.
  View* contents_;

  DISALLOW_COPY_AND_ASSIGN(Tab);
};

// The tab strip shown above the tab contents.
class TabStrip : public View {
 public:
  TabStrip();
  virtual ~TabStrip();

  // Overridden from View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(TabStrip);
};

Tab::Tab(TabbedPane* tabbed_pane, const string16& title, View* contents)
    : tabbed_pane_(tabbed_pane),
      title_(new Label(title, gfx::Font().DeriveFont(0, gfx::Font::BOLD))),
      contents_(contents) {
  SetTitleColor(kTabTitleColor_Inactive);
  AddChildView(title_);
}

Tab::~Tab() {}

void Tab::SetSelected(bool selected) {
  contents_->SetVisible(selected);
  SetTitleColor(selected ? kTabTitleColor_Active : kTabTitleColor_Inactive);
}

bool Tab::OnMousePressed(const ui::MouseEvent& event) {
  SetTitleColor(kTabTitleColor_Pressed);
  return true;
}

void Tab::OnMouseReleased(const ui::MouseEvent& event) {
  SetTitleColor(selected() ? kTabTitleColor_Active : kTabTitleColor_Hovered);
  if (GetLocalBounds().Contains(event.location()))
    tabbed_pane_->SelectTab(this);
}

void Tab::OnMouseCaptureLost() {
  SetTitleColor(kTabTitleColor_Inactive);
}

void Tab::OnMouseEntered(const ui::MouseEvent& event) {
  SetTitleColor(selected() ? kTabTitleColor_Active : kTabTitleColor_Hovered);
}

void Tab::OnMouseExited(const ui::MouseEvent& event) {
  SetTitleColor(selected() ? kTabTitleColor_Active : kTabTitleColor_Inactive);
}

void Tab::OnGestureEvent(ui::GestureEvent* event) {
  switch (event->type()) {
    case ui::ET_GESTURE_TAP_DOWN:
      SetTitleColor(kTabTitleColor_Pressed);
      break;
    case ui::ET_GESTURE_TAP:
      // SelectTab also sets the right tab color.
      tabbed_pane_->SelectTab(this);
      break;
    case ui::ET_GESTURE_TAP_CANCEL:
      SetTitleColor(selected() ?
          kTabTitleColor_Active : kTabTitleColor_Inactive);
      break;
    default:
      break;
  }
  event->SetHandled();
}

gfx::Size Tab::GetPreferredSize() {
  gfx::Size size(title_->GetPreferredSize());
  size.Enlarge(30, 10);
  const int kTabMinWidth = 54;
  if (size.width() < kTabMinWidth)
    size.set_width(kTabMinWidth);
  return size;
}

void Tab::Layout() {
  gfx::Rect bounds = GetLocalBounds();
  bounds.ClampToCenteredSize(title_->GetPreferredSize());
  title_->SetBoundsRect(bounds);
}

void Tab::OnPaint(gfx::Canvas* canvas) {
  if (selected()) {
    canvas->DrawColor(kTabBackgroundColor);
    PaintTabBorder(canvas);
  }
  View::OnPaint(canvas);
}

void Tab::SetTitleColor(SkColor color) {
  title_->SetEnabledColor(color);
  SchedulePaint();
}

void Tab::PaintTabBorder(gfx::Canvas* canvas) {
  SkPath path;
  // Clip the bottom of the border by extending its rect height beyond the tab.
  const int tab_height = height() + kTabBorderThickness;
  SkRect bounds = { 0, 0, SkIntToScalar(width()), SkIntToScalar(tab_height) };
  SkScalar radii[8] = { kTabBorderRadius, kTabBorderRadius,
                        kTabBorderRadius, kTabBorderRadius,
                        0, 0,
                        0, 0 };
  path.addRoundRect(bounds, radii, SkPath::kCW_Direction);

  SkPaint paint;
  paint.setAntiAlias(true);
  paint.setStyle(SkPaint::kStroke_Style);
  paint.setColor(kTabBorderColor);
  paint.setStrokeWidth(kTabBorderThickness * 2);
  canvas->DrawPath(path, paint);
}

TabStrip::TabStrip() {
  const int kCount = 4;
  // This gradient is designed to mach WebUI tabbed panes.
  SkColor colors[kCount] = {
      SkColorSetRGB(0xff, 0xff, 0xff),
      SkColorSetRGB(0xff, 0xff, 0xff),
      SkColorSetRGB(0xfa, 0xfa, 0xfa),
      SkColorSetRGB(0xf2, 0xf2, 0xf2)
  };
  SkScalar pos[kCount] = { 0.0f, 0.6f, 0.8f, 1.0f };
  set_background(Background::CreateVerticalMultiColorGradientBackground(
      colors, pos, kCount));
}

TabStrip::~TabStrip() {}

gfx::Size TabStrip::GetPreferredSize() {
  gfx::Size size;
  for (int i = 0; i < child_count(); ++i) {
    const gfx::Size child_size = child_at(i)->GetPreferredSize();
    size.SetSize(size.width() + child_size.width(),
                 std::max(size.height(), child_size.height()));
  }
  return size;
}

void TabStrip::Layout() {
  const int kTabOffset = 18;
  int x = kTabOffset;  // Layout tabs with an offset to the tabstrip border.
  for (int i = 0; i < child_count(); ++i) {
    gfx::Size ps = child_at(i)->GetPreferredSize();
    child_at(i)->SetBounds(x, 0, ps.width(), ps.height());
    x = child_at(i)->bounds().right();
  }
}

void TabStrip::OnPaint(gfx::Canvas* canvas) {
  OnPaintBackground(canvas);

  // Draw the TabStrip border.
  SkPaint paint;
  paint.setColor(kTabBorderColor);
  paint.setStrokeWidth(kTabBorderThickness);
  SkScalar line_y = SkIntToScalar(height()) - kTabBorderThickness;
  SkScalar line_width = SkIntToScalar(width());
  canvas->sk_canvas()->drawLine(0, line_y, line_width, line_y, paint);
}

TabbedPane::TabbedPane()
  : listener_(NULL),
    tab_strip_(new TabStrip()),
    contents_(new View()),
    selected_tab_index_(-1) {
  set_focusable(true);
  AddChildView(tab_strip_);
  AddChildView(contents_);
}

TabbedPane::~TabbedPane() {}

int TabbedPane::GetTabCount() {
  DCHECK_EQ(tab_strip_->child_count(), contents_->child_count());
  return contents_->child_count();
}

View* TabbedPane::GetSelectedTab() {
  return selected_tab_index() < 0 ?
      NULL : GetTabAt(selected_tab_index())->contents();
}

void TabbedPane::AddTab(const string16& title, View* contents) {
  AddTabAtIndex(tab_strip_->child_count(), title, contents);
}

void TabbedPane::AddTabAtIndex(int index,
                               const string16& title,
                               View* contents) {
  DCHECK(index >= 0 && index <= GetTabCount());
  contents->SetVisible(false);

  tab_strip_->AddChildViewAt(new Tab(this, title, contents), index);
  contents_->AddChildViewAt(contents, index);
  if (selected_tab_index() < 0)
    SelectTabAt(index);

  PreferredSizeChanged();
}

void TabbedPane::SelectTabAt(int index) {
  DCHECK(index >= 0 && index < GetTabCount());
  if (index == selected_tab_index())
    return;

  if (selected_tab_index() >= 0)
    GetTabAt(selected_tab_index())->SetSelected(false);

  selected_tab_index_ = index;
  Tab* tab = GetTabAt(index);
  tab->SetSelected(true);
  tab_strip_->SchedulePaint();

  FocusManager* focus_manager = tab->contents()->GetFocusManager();
  if (focus_manager) {
    const View* focused_view = focus_manager->GetFocusedView();
    if (focused_view && contents_->Contains(focused_view) &&
        !tab->contents()->Contains(focused_view))
      focus_manager->SetFocusedView(tab->contents());
  }

  if (listener())
    listener()->TabSelectedAt(index);
}

void TabbedPane::SelectTab(Tab* tab) {
  const int index = tab_strip_->GetIndexOf(tab);
  if (index >= 0)
    SelectTabAt(index);
}

gfx::Size TabbedPane::GetPreferredSize() {
  gfx::Size size;
  for (int i = 0; i < contents_->child_count(); ++i)
    size.ClampToMin(contents_->child_at(i)->GetPreferredSize());
  size.Enlarge(0, tab_strip_->GetPreferredSize().height());
  return size;
}

Tab* TabbedPane::GetTabAt(int index) {
  return static_cast<Tab*>(tab_strip_->child_at(index));
}

void TabbedPane::Layout() {
  const gfx::Size size = tab_strip_->GetPreferredSize();
  tab_strip_->SetBounds(0, 0, width(), size.height());
  contents_->SetBounds(0, tab_strip_->bounds().bottom(), width(),
                       std::max(0, height() - size.height()));
  for (int i = 0; i < contents_->child_count(); ++i)
    contents_->child_at(i)->SetSize(contents_->size());
}

void TabbedPane::ViewHierarchyChanged(bool is_add, View* parent, View* child) {
  if (is_add) {
    // Support navigating tabs by Ctrl+Tab and Ctrl+Shift+Tab.
    AddAccelerator(ui::Accelerator(ui::VKEY_TAB,
                                   ui::EF_CONTROL_DOWN | ui::EF_SHIFT_DOWN));
    AddAccelerator(ui::Accelerator(ui::VKEY_TAB, ui::EF_CONTROL_DOWN));
  }
}

bool TabbedPane::AcceleratorPressed(const ui::Accelerator& accelerator) {
  // Handle Ctrl+Tab and Ctrl+Shift+Tab navigation of pages.
  DCHECK(accelerator.key_code() == ui::VKEY_TAB && accelerator.IsCtrlDown());
  const int tab_count = GetTabCount();
  if (tab_count <= 1)
    return false;
  const int increment = accelerator.IsShiftDown() ? -1 : 1;
  int next_tab_index = (selected_tab_index() + increment) % tab_count;
  // Wrap around.
  if (next_tab_index < 0)
    next_tab_index += tab_count;
  SelectTabAt(next_tab_index);
  return true;
}

std::string TabbedPane::GetClassName() const {
  return kViewClassName;
}

void TabbedPane::OnFocus() {
  View::OnFocus();

  View* selected_tab = GetSelectedTab();
  if (selected_tab) {
    selected_tab->NotifyAccessibilityEvent(
        ui::AccessibilityTypes::EVENT_FOCUS, true);
  }
}

void TabbedPane::GetAccessibleState(ui::AccessibleViewState* state) {
  state->role = ui::AccessibilityTypes::ROLE_PAGETABLIST;
}

}  // namespace views
