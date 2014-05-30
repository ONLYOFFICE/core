// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/layer_owner.h"

namespace ui {

LayerOwner::LayerOwner()
    : layer_(NULL) {
}

LayerOwner::~LayerOwner() {
}

Layer* LayerOwner::AcquireLayer() {
  return layer_owner_.release();
}

}  // namespace ui
