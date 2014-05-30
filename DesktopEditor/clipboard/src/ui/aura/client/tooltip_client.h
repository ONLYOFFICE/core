// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_TOOLTIP_CLIENT_H_
#define UI_AURA_CLIENT_TOOLTIP_CLIENT_H_

#include "ui/aura/aura_export.h"
#include "ui/gfx/font.h"

namespace aura {
class RootWindow;
class Window;
namespace client {

class AURA_EXPORT TooltipClient {
 public:
  // Informs the shell tooltip manager of change in tooltip for window |target|.
  virtual void UpdateTooltip(Window* target) = 0;

  // Enables/Disables tooltips.
  virtual void SetTooltipsEnabled(bool enable) = 0;
};

AURA_EXPORT void SetTooltipClient(RootWindow* root_window,
                                  TooltipClient* client);
AURA_EXPORT TooltipClient* GetTooltipClient(RootWindow* root_window);
AURA_EXPORT void SetTooltipText(Window* window, string16* tooltip_text);
AURA_EXPORT const string16 GetTooltipText(Window* window);

}  // namespace client
}  // namespace aura

#endif  // UI_AURA_CLIENT_TOOLTIP_CLIENT_H_
