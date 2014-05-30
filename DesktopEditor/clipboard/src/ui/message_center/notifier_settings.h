// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_NOTIFIER_SETTINGS_H_
#define UI_MESSAGE_CENTER_NOTIFIER_SETTINGS_H_

#include <string>

#include "base/string16.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/image/image.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {

class NotifierSettingsDelegate;
class NotifierSettingsProvider;

// Brings up the settings dialog and returns a weak reference to the delegate,
// which is typically the view. If the dialog already exists, it is brought to
// the front, otherwise it is created.
MESSAGE_CENTER_EXPORT NotifierSettingsDelegate* ShowSettings(
    NotifierSettingsProvider* provider,
    gfx::NativeView context);

// The struct to hold the information of notifiers. The information will be
// used by NotifierSettingsView.
struct MESSAGE_CENTER_EXPORT Notifier {
  enum NotifierType {
    APPLICATION,
    WEB_PAGE,
    SYSTEM_COMPONENT,
  };

  enum SystemComponentNotifierType {
    NONE,
    SCREENSHOT,
  };

  // Constructor for APPLICATION type.
  Notifier(const std::string& id, const string16& name, bool enabled);

  // Constructor for WEB_PAGE type.
  Notifier(const GURL& url, const string16& name, bool enabled);

  // Constructor for SYSTEM_COMPONENT type.
  Notifier(SystemComponentNotifierType type,
           const string16& name, bool enabled);

  ~Notifier();

  // The identifier of the app notifier. Empty if it's URL_PATTERN.
  std::string id;

  // The URL pattern of the notifer.
  GURL url;

  // The type of system component notifier.
  SystemComponentNotifierType system_component_type;

  // The human-readable name of the notifier such like the extension name.
  // It can be empty.
  string16 name;

  // True if the source is allowed to send notifications. True is default.
  bool enabled;

  // The type of notifier: Chrome app, URL pattern, or System component.
  NotifierType type;

  // The icon image of the notifier. The extension icon or favicon.
  gfx::Image icon;

 private:
  DISALLOW_COPY_AND_ASSIGN(Notifier);
};

MESSAGE_CENTER_EXPORT std::string ToString(
    Notifier::SystemComponentNotifierType type);
MESSAGE_CENTER_EXPORT Notifier::SystemComponentNotifierType
    ParseSystemComponentName(const std::string& name);

// A class used by NotifierSettingsView to integrate with a setting system
// for the clients of this module.
class MESSAGE_CENTER_EXPORT NotifierSettingsProvider {
 public:
  // Collects the current notifier list and fills to |notifiers|. Caller takes
  // the ownership of the elements of |notifiers|.
  virtual void GetNotifierList(std::vector<Notifier*>* notifiers) = 0;

  // Called when the |enabled| for the |notifier| has been changed by user
  // operation.
  virtual void SetNotifierEnabled(const Notifier& notifier, bool enabled) = 0;

  // Called when the settings window is closed.
  virtual void OnNotifierSettingsClosing() = 0;
};

// A delegate class implemented by the view of the NotifierSettings to get
// notified when the controller has changed data.
class MESSAGE_CENTER_EXPORT NotifierSettingsDelegate {
 public:
  // Called when an icon in the controller has been updated.
  virtual void UpdateIconImage(const std::string& id,
                               const gfx::Image& icon) = 0;

  // Called when the controller detects that a favicon has changed.
  virtual void UpdateFavicon(const GURL& url, const gfx::Image& icon) = 0;
};

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_NOTIFIER_SETTINGS_H_
