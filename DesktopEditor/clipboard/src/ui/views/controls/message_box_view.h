// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MESSAGE_BOX_VIEW_H_
#define UI_VIEWS_CONTROLS_MESSAGE_BOX_VIEW_H_

#include <vector>

#include "base/string16.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/views/view.h"

namespace gfx {
class ImageSkia;
}

namespace views {

class Checkbox;
class ImageView;
class Label;
class Textfield;

// This class displays the contents of a message box. It is intended for use
// within a constrained window, and has options for a message, prompt, OK
// and Cancel buttons.
class VIEWS_EXPORT MessageBoxView : public View {
 public:
  enum Options {
    NO_OPTIONS = 0,
    // For a message from a web page (not from Chrome's UI), such as script
    // dialog text, each paragraph's directionality is auto-detected using the
    // directionality of the paragraph's first strong character's. Please refer
    // to HTML5 spec for details.
    // http://dev.w3.org/html5/spec/Overview.html#text-rendered-in-native-user-interfaces:
    // The spec does not say anything about alignment. And we choose to
    // align all paragraphs according to the direction of the first paragraph.
    DETECT_DIRECTIONALITY = 1 << 0,
    HAS_PROMPT_FIELD = 1 << 1,
  };

  struct VIEWS_EXPORT InitParams {
    explicit InitParams(const string16& message);
    ~InitParams();

    uint16 options;
    string16 message;
    string16 default_prompt;
    int message_width;
    int inter_row_vertical_spacing;
    ui::SourceTag clipboard_source_tag;
  };

  explicit MessageBoxView(const InitParams& params);

  virtual ~MessageBoxView();

  // Returns the text box.
  views::Textfield* text_box() { return prompt_field_; }

  // Returns user entered data in the prompt field.
  string16 GetInputText();

  // Returns true if a checkbox is selected, false otherwise. (And false if
  // the message box has no checkbox.)
  bool IsCheckBoxSelected();

  // Adds |icon| to the upper left of the message box or replaces the current
  // icon. To start out, the message box has no icon.
  void SetIcon(const gfx::ImageSkia& icon);

  // Adds a checkbox with the specified label to the message box if this is the
  // first call. Otherwise, it changes the label of the current checkbox. To
  // start, the message box has no checkbox until this function is called.
  void SetCheckBoxLabel(const string16& label);

  // Sets the state of the check-box.
  void SetCheckBoxSelected(bool selected);

  // View:
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 protected:
  // View:
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child) OVERRIDE;
  // Handles Ctrl-C and writes the message in the system clipboard.
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

 private:
  // Sets up the layout manager and initializes the message labels and prompt
  // field. This should only be called once, from the constructor.
  void Init(const InitParams& params);

  // Sets up the layout manager based on currently initialized views. Should be
  // called when a view is initialized or changed.
  void ResetLayoutManager();

  // Message for the message box.
  std::vector<Label*> message_labels_;

  // Input text field for the message box.
  Textfield* prompt_field_;

  // Icon displayed in the upper left corner of the message box.
  ImageView* icon_;

  // Checkbox for the message box.
  Checkbox* checkbox_;

  // Maximum width of the message label.
  int message_width_;

  // Spacing between rows in the grid layout.
  int inter_row_vertical_spacing_;

  // Source tag to be written to the clipboard when Ctrl-C pressed.
  ui::SourceTag source_tag_;

  DISALLOW_COPY_AND_ASSIGN(MessageBoxView);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_MESSAGE_BOX_VIEW_H_
