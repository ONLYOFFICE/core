// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/notifier_settings.h"

namespace message_center {

#if !defined(TOOLKIT_VIEWS)
NotifierSettingsDelegate* ShowSettings(NotifierSettingsProvider* provider,
                                       gfx::NativeView context) {
  NOTIMPLEMENTED();
  return NULL;
}
#endif

Notifier::Notifier(const std::string& id,
                   const string16& name,
                   bool enabled)
    : id(id),
      system_component_type(NONE),
      name(name),
      enabled(enabled),
      type(APPLICATION) {
}

Notifier::Notifier(const GURL& url, const string16& name, bool enabled)
    : url(url),
      system_component_type(NONE),
      name(name),
      enabled(enabled),
      type(WEB_PAGE) {
}

Notifier::Notifier(SystemComponentNotifierType system_component_type,
                   const string16& name,
                   bool enabled)
    : system_component_type(system_component_type),
      name(name),
      enabled(enabled),
      type(SYSTEM_COMPONENT) {
}

Notifier::~Notifier() {
}

std::string ToString(
    Notifier::SystemComponentNotifierType type) {
  switch (type) {
    case Notifier::SCREENSHOT:
      return "screenshot";
    default:
      DCHECK(false);
      return "";
  }
}

Notifier::SystemComponentNotifierType
ParseSystemComponentName(const std::string& name) {
  if (name == "screenshot") {
    return Notifier::SCREENSHOT;
  } else {
    DCHECK(false);
    return Notifier::NONE;
  }
}
}  // namespace message_center
