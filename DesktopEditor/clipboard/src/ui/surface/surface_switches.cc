// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/surface/surface_switches.h"

namespace switches {

// Flags for enabling speculative fix for http://crbug.com/169848

// Use GDI to do every first Present after the tab is unhidden.
const char kDoAllShowPresentWithGDI[]       = "all-show-present-with-GDI";

// Use GDI to do the fist Present after the tab is unhidden.
const char kDoFirstShowPresentWithGDI[]     = "first-show-present-with-GDI";


}  // namespace switches

