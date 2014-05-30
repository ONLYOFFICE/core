// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/examples/label_example.h"

#include "base/utf_string_conversions.h"
#include "ui/views/border.h"
#include "ui/views/controls/label.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/view.h"

namespace views {
namespace examples {

namespace {

// A Label with a constrained preferred size to demonstrate eliding or wrapping.
class PreferredSizeLabel : public Label {
 public:
  PreferredSizeLabel();
  virtual ~PreferredSizeLabel();

  // Overridden from Label:
  virtual gfx::Size GetPreferredSize() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(PreferredSizeLabel);
};

PreferredSizeLabel::PreferredSizeLabel() : Label() {
  set_border(Border::CreateSolidBorder(2, SK_ColorCYAN));
}

PreferredSizeLabel::~PreferredSizeLabel() {}

gfx::Size PreferredSizeLabel::GetPreferredSize() { return gfx::Size(100, 40); }

}  // namespace

LabelExample::LabelExample() : ExampleBase("Label") {}

LabelExample::~LabelExample() {}

void LabelExample::CreateExampleView(View* container) {
  // A very simple label example, followed by additional helpful examples.
  container->SetLayoutManager(new BoxLayout(BoxLayout::kVertical, 0, 0, 10));
  Label* label = new Label(ASCIIToUTF16("Hello world!"));
  container->AddChildView(label);

  const wchar_t hello_world_hebrew[] =
      L"\x5e9\x5dc\x5d5\x5dd \x5d4\x5e2\x5d5\x5dc\x5dd!";
  label = new Label(WideToUTF16(hello_world_hebrew));
  label->SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  container->AddChildView(label);

  label = new Label(WideToUTF16(L"A UTF16 surrogate pair: \x5d0\x5b0"));
  label->SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  container->AddChildView(label);

  label = new Label(ASCIIToUTF16("A left-aligned blue label."));
  label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  label->SetEnabledColor(SK_ColorBLUE);
  container->AddChildView(label);

  label = new Label(ASCIIToUTF16("A Courier-18 label with a shadow."));
  label->SetFont(gfx::Font("Courier", 18));
  label->SetShadowColors(SK_ColorGRAY, SK_ColorLTGRAY);
  label->SetShadowOffset(1, 1);
  container->AddChildView(label);

  label = new PreferredSizeLabel();
  label->SetText(ASCIIToUTF16("A long label will elide toward its logical end "
      "if the text's width exceeds the label's available width."));
  container->AddChildView(label);

  label = new PreferredSizeLabel();
  label->SetText(ASCIIToUTF16("A multi-line label will wrap onto subsequent "
      "lines if the text's width exceeds the label's available width."));
  label->SetMultiLine(true);
  container->AddChildView(label);
}

}  // namespace examples
}  // namespace views
