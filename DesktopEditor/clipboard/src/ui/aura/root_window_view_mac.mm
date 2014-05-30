// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "ui/aura/root_window_view_mac.h"

@implementation RootWindowView
-(void)setCompositor:(ui::Compositor*)compositor {
  compositor_ = compositor;
}

- (void)drawRect:(NSRect)rect {
  if (compositor_)
    compositor_->Draw(false);
}
@end
