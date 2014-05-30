// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_PULSING_BLOCK_VIEW_H_
#define UI_APP_LIST_VIEWS_PULSING_BLOCK_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/timer.h"
#include "ui/views/view.h"

namespace gfx {
class Size;
}

namespace app_list {

// PulsingBlockView shows a pulsing white block via layer animation.
class PulsingBlockView : public views::View {
 public:
  // Constructs a PulsingBlockView of |size|. If |start_delay| is true,
  // starts the pulsing animation after a random delay.
  PulsingBlockView(const gfx::Size& size, bool start_delay);
  virtual ~PulsingBlockView();

 private:
  void OnStartDelayTimer();

  // views::View overrides:
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  base::OneShotTimer<PulsingBlockView> start_delay_timer_;

  DISALLOW_COPY_AND_ASSIGN(PulsingBlockView);
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_PULSING_BLOCK_VIEW_H_
