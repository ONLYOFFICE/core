// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_CONTENT_CLIENT_EXAMPLES_CONTENT_BROWSER_CLIENT_H_
#define UI_VIEWS_EXAMPLES_CONTENT_CLIENT_EXAMPLES_CONTENT_BROWSER_CLIENT_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/content_browser_client.h"

namespace content {
class ShellBrowserContext;
}

namespace views {
namespace examples {

class ExamplesBrowserMainParts;

class ExamplesContentBrowserClient : public content::ContentBrowserClient {
 public:
  ExamplesContentBrowserClient();
  virtual ~ExamplesContentBrowserClient();

  // Overridden from content::ContentBrowserClient:
  virtual content::BrowserMainParts* CreateBrowserMainParts(
      const content::MainFunctionParams& parameters) OVERRIDE;

  content::ShellBrowserContext* browser_context();

 private:
  ExamplesBrowserMainParts* examples_browser_main_parts_;

  DISALLOW_COPY_AND_ASSIGN(ExamplesContentBrowserClient);
};

}  // namespace examples
}  // namespace views

#endif  // UI_VIEWS_EXAMPLES_CONTENT_CLIENT_EXAMPLES_CONTENT_BROWSER_CLIENT_H_
