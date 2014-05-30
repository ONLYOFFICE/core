// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/examples/content_client/examples_content_browser_client.h"

#include "ui/views/examples/content_client/examples_browser_main_parts.h"

namespace views {
namespace examples {

ExamplesContentBrowserClient::ExamplesContentBrowserClient()
    : examples_browser_main_parts_(NULL) {
}

ExamplesContentBrowserClient::~ExamplesContentBrowserClient() {
}

content::BrowserMainParts* ExamplesContentBrowserClient::CreateBrowserMainParts(
    const content::MainFunctionParams& parameters) {
  examples_browser_main_parts_ =  new ExamplesBrowserMainParts(parameters);
  return examples_browser_main_parts_;
}

content::ShellBrowserContext* ExamplesContentBrowserClient::browser_context() {
  return examples_browser_main_parts_->browser_context();
}

}  // namespace examples
}  // namespace views
