// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/test/compositor_test_support.h"

#include "base/compiler_specific.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebKit.h"
#include "webkit/glue/webkitplatformsupport_impl.h"

namespace ui {

class CompositorTestPlatformSupport:
    public NON_EXPORTED_BASE(webkit_glue::WebKitPlatformSupportImpl) {
 public:
  virtual string16 GetLocalizedString(int message_id) OVERRIDE {
    return string16();
  }

  virtual base::StringPiece GetDataResource(
      int resource_id,
      ui::ScaleFactor scale_factor) OVERRIDE {
    return base::StringPiece();
  }

  virtual void GetPlugins(
      bool refresh, std::vector<webkit::WebPluginInfo>* plugins) OVERRIDE {
  }

  virtual webkit_glue::ResourceLoaderBridge* CreateResourceLoader(
      const webkit_glue::ResourceLoaderBridge::RequestInfo& request_info)
      OVERRIDE {
    NOTREACHED();
    return NULL;
  }

  virtual webkit_glue::WebSocketStreamHandleBridge* CreateWebSocketBridge(
      WebKit::WebSocketStreamHandle* handle,
      webkit_glue::WebSocketStreamHandleDelegate* delegate) OVERRIDE {
    NOTREACHED();
    return NULL;
  }
};

static CompositorTestPlatformSupport* g_webkit_support;

void CompositorTestSupport::Initialize() {
  DCHECK(!g_webkit_support);
  g_webkit_support = new CompositorTestPlatformSupport;
  WebKit::initialize(g_webkit_support);
}

void CompositorTestSupport::Terminate() {
  DCHECK(g_webkit_support);
  WebKit::shutdown();
  delete g_webkit_support;
  g_webkit_support = NULL;
}

}  // namespace ui
