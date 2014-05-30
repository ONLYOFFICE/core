// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/examples/examples_window_with_content.h"

#include <string>

#include "base/command_line.h"
#include "base/memory/scoped_vector.h"
#include "base/utf_string_conversions.h"
#include "content/public/browser/browser_context.h"
#include "ui/base/models/combobox_model.h"
#include "ui/base/ui_base_paths.h"
#include "ui/views/controls/button/text_button.h"
#include "ui/views/controls/combobox/combobox.h"
#include "ui/views/controls/label.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane.h"
#include "ui/views/examples/bubble_example.h"
#include "ui/views/examples/button_example.h"
#include "ui/views/examples/combobox_example.h"
#include "ui/views/examples/double_split_view_example.h"
#include "ui/views/examples/label_example.h"
#include "ui/views/examples/link_example.h"
#include "ui/views/examples/menu_example.h"
#include "ui/views/examples/message_box_example.h"
#include "ui/views/examples/native_theme_button_example.h"
#include "ui/views/examples/native_theme_checkbox_example.h"
#include "ui/views/examples/progress_bar_example.h"
#include "ui/views/examples/radio_button_example.h"
#include "ui/views/examples/scroll_view_example.h"
#include "ui/views/examples/single_split_view_example.h"
#include "ui/views/examples/slider_example.h"
#include "ui/views/examples/tabbed_pane_example.h"
#include "ui/views/examples/table_example.h"
#include "ui/views/examples/text_example.h"
#include "ui/views/examples/textfield_example.h"
#include "ui/views/examples/throbber_example.h"
#include "ui/views/examples/tree_view_example.h"
#include "ui/views/examples/webview_example.h"
#include "ui/views/examples/widget_example.h"
#include "ui/views/focus/accelerator_handler.h"
#include "ui/views/layout/fill_layout.h"
#include "ui/views/layout/grid_layout.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

#if defined(USE_AURA)
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"
#endif

namespace views {
namespace examples {

// Model for the examples that are being added via AddExample().
class ComboboxModelExampleList : public ui::ComboboxModel {
 public:
  ComboboxModelExampleList() {}
  virtual ~ComboboxModelExampleList() {}

  // Overridden from ui::ComboboxModel:
  virtual int GetItemCount() const OVERRIDE { return example_list_.size(); }
  virtual string16 GetItemAt(int index) OVERRIDE {
    return UTF8ToUTF16(example_list_[index]->example_title());
  }

  View* GetItemViewAt(int index) {
    return example_list_[index]->example_view();
  }

  void AddExample(ExampleBase* example) {
    example_list_.push_back(example);
  }

 private:
  ScopedVector<ExampleBase> example_list_;

  DISALLOW_COPY_AND_ASSIGN(ComboboxModelExampleList);
};

class ExamplesWindowContents : public WidgetDelegateView,
                               public ComboboxListener {
 public:
  ExamplesWindowContents(Operation operation,
                         content::BrowserContext* browser_context)
      : combobox_(new Combobox(&combobox_model_)),
        example_shown_(new View),
        status_label_(new Label),
        operation_(operation),
        browser_context_(browser_context) {
    instance_ = this;
    combobox_->set_listener(this);
  }
  virtual ~ExamplesWindowContents() {}

  // Prints a message in the status area, at the bottom of the window.
  void SetStatus(const std::string& status) {
    status_label_->SetText(UTF8ToUTF16(status));
  }

  static ExamplesWindowContents* instance() { return instance_; }

 private:
  // Overridden from WidgetDelegateView:
  virtual bool CanResize() const OVERRIDE { return true; }
  virtual bool CanMaximize() const OVERRIDE { return true; }
  virtual string16 GetWindowTitle() const OVERRIDE {
    return ASCIIToUTF16("Views Examples");
  }
  virtual View* GetContentsView() OVERRIDE { return this; }
  virtual void WindowClosing() OVERRIDE {
    instance_ = NULL;
    if (operation_ == QUIT_ON_CLOSE)
      base::MessageLoopForUI::current()->Quit();
  }

  // Overridden from View:
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE {
    if (is_add && child == this)
      InitExamplesWindow();
  }

  // Overridden from ComboboxListener:
  virtual void OnSelectedIndexChanged(Combobox* combobox) OVERRIDE {
    DCHECK_EQ(combobox, combobox_);
    DCHECK(combobox->selected_index() < combobox_model_.GetItemCount());
    example_shown_->RemoveAllChildViews(false);
    example_shown_->AddChildView(combobox_model_.GetItemViewAt(
        combobox->selected_index()));
    example_shown_->RequestFocus();
    SetStatus(std::string());
    Layout();
  }

  // Creates the layout within the examples window.
  void InitExamplesWindow() {
    AddExamples();

    set_background(Background::CreateStandardPanelBackground());
    GridLayout* layout = new GridLayout(this);
    SetLayoutManager(layout);
    ColumnSet* column_set = layout->AddColumnSet(0);
    column_set->AddPaddingColumn(0, 5);
    column_set->AddColumn(GridLayout::FILL, GridLayout::FILL, 1,
                          GridLayout::USE_PREF, 0, 0);
    column_set->AddPaddingColumn(0, 5);
    layout->AddPaddingRow(0, 5);
    layout->StartRow(0 /* no expand */, 0);
    layout->AddView(combobox_);

    if (combobox_model_.GetItemCount() > 0) {
      layout->StartRow(1, 0);
      example_shown_->SetLayoutManager(new FillLayout());
      example_shown_->AddChildView(combobox_model_.GetItemViewAt(0));
      layout->AddView(example_shown_);
    }

    layout->StartRow(0 /* no expand */, 0);
    layout->AddView(status_label_);
    layout->AddPaddingRow(0, 5);
  }

  // Adds all the individual examples to the combobox model.
  void AddExamples() {
    // Please keep this list in alphabetical order!
    combobox_model_.AddExample(new BubbleExample);
    combobox_model_.AddExample(new ButtonExample);
    combobox_model_.AddExample(new ComboboxExample);
    combobox_model_.AddExample(new DoubleSplitViewExample);
    combobox_model_.AddExample(new LabelExample);
    combobox_model_.AddExample(new LinkExample);
    combobox_model_.AddExample(new MenuExample);
    combobox_model_.AddExample(new MessageBoxExample);
    combobox_model_.AddExample(new NativeThemeButtonExample);
    combobox_model_.AddExample(new NativeThemeCheckboxExample);
    combobox_model_.AddExample(new ProgressBarExample);
    combobox_model_.AddExample(new RadioButtonExample);
    combobox_model_.AddExample(new ScrollViewExample);
    combobox_model_.AddExample(new SingleSplitViewExample);
    combobox_model_.AddExample(new SliderExample);
    combobox_model_.AddExample(new TabbedPaneExample);
    combobox_model_.AddExample(new TableExample);
    combobox_model_.AddExample(new TextExample);
    combobox_model_.AddExample(new TextfieldExample);
    combobox_model_.AddExample(new ThrobberExample);
    combobox_model_.AddExample(new TreeViewExample);
    combobox_model_.AddExample(new WebViewExample(browser_context_));
    combobox_model_.AddExample(new WidgetExample);
  }

  static ExamplesWindowContents* instance_;
  ComboboxModelExampleList combobox_model_;
  Combobox* combobox_;
  View* example_shown_;
  Label* status_label_;
  const Operation operation_;
  content::BrowserContext* browser_context_;

  DISALLOW_COPY_AND_ASSIGN(ExamplesWindowContents);
};

// static
ExamplesWindowContents* ExamplesWindowContents::instance_ = NULL;

void ShowExamplesWindowWithContent(Operation operation,
                                   content::BrowserContext* browser_context) {
  if (ExamplesWindowContents::instance()) {
    ExamplesWindowContents::instance()->GetWidget()->Activate();
  } else {
    Widget* widget = new Widget;
    Widget::InitParams params;
    params.delegate = new ExamplesWindowContents(operation, browser_context);
    params.bounds = gfx::Rect(0, 0, 850, 300);
    params.top_level = true;
    widget->Init(params);
    widget->Show();
  }
}

void LogStatus(const std::string& string) {
  ExamplesWindowContents::instance()->SetStatus(string);
}

}  // namespace examples
}  // namespace views
