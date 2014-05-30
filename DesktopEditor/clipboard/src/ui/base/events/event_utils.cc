// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/events/event_utils.h"

#include "ui/base/events/event.h"

namespace ui {

namespace {
int g_custom_event_types = ET_LAST;
}  // namespace

bool EventCanceledDefaultHandling(const Event& event) {
  return event.phase() == EP_POSTTARGET && event.result() != ER_UNHANDLED;
}

int RegisterCustomEventType() {
  return ++g_custom_event_types;
}

base::TimeDelta EventTimeForNow() {
  return base::TimeDelta::FromInternalValue(
      base::TimeTicks::Now().ToInternalValue());
}

}  // namespace ui
