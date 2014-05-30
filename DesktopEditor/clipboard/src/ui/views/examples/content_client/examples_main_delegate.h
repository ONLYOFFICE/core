// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_EXAMPLES_CONTENT_CLIENT_EXAMPLES_MAIN_DELEGATE_H_
#define UI_VIEWS_EXAMPLES_CONTENT_CLIENT_EXAMPLES_MAIN_DELEGATE_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/app/content_main_delegate.h"
#include "content/shell/shell_content_client.h"

namespace views {
namespace examples {

class ExamplesContentBrowserClient;

class ExamplesMainDelegate : public content::ContentMainDelegate {
 public:
  ExamplesMainDelegate();
  virtual ~ExamplesMainDelegate();

  // content::ContentMainDelegate implementation
  virtual bool BasicStartupComplete(int* exit_code) OVERRIDE;
  virtual void PreSandboxStartup() OVERRIDE;
  virtual content::ContentBrowserClient* CreateContentBrowserClient() OVERRIDE;

 private:
  void InitializeResourceBundle();

  scoped_ptr<ExamplesContentBrowserClient> browser_client_;
  content::ShellContentClient content_client_;

  DISALLOW_COPY_AND_ASSIGN(ExamplesMainDelegate);
};

}  // namespace examples
}  // namespace views

#endif  // UI_VIEWS_EXAMPLES_CONTENT_CLIENT_EXAMPLES_MAIN_DELEGATE_H_
