// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/resource/resource_bundle.h"

#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/memory/ref_counted_memory.h"
#include "base/path_service.h"
#include "base/synchronization/lock.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/gtk/scoped_gobject.h"
#include "ui/base/layout.h"
#include "ui/base/resource/resource_handle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/gfx/image/image.h"

#include <gtk/gtk.h>

namespace ui {

namespace {

// Convert the raw image data into a GdkPixbuf.  The GdkPixbuf that is returned
// has a ref count of 1 so the caller must call g_object_unref to free the
// memory.
GdkPixbuf* LoadPixbuf(base::RefCountedStaticMemory* data, bool rtl_enabled) {
  ScopedGObject<GdkPixbufLoader>::Type loader(gdk_pixbuf_loader_new());
  bool ok = data && gdk_pixbuf_loader_write(loader.get(),
      reinterpret_cast<const guint8*>(data->front()), data->size(), NULL);
  if (!ok)
    return NULL;
  // Calling gdk_pixbuf_loader_close forces the data to be parsed by the
  // loader.  We must do this before calling gdk_pixbuf_loader_get_pixbuf.
  ok = gdk_pixbuf_loader_close(loader.get(), NULL);
  if (!ok)
    return NULL;
  GdkPixbuf* pixbuf = gdk_pixbuf_loader_get_pixbuf(loader.get());
  if (!pixbuf)
    return NULL;

  if (base::i18n::IsRTL() && rtl_enabled) {
    // |pixbuf| will get unreffed and destroyed (see below). The returned value
    // has ref count 1.
    return gdk_pixbuf_flip(pixbuf, TRUE);
  } else {
    // The pixbuf is owned by the loader, so add a ref so when we delete the
    // loader (when the ScopedGObject goes out of scope), the pixbuf still
    // exists.
    g_object_ref(pixbuf);
    return pixbuf;
  }
}

base::FilePath GetResourcesPakFilePath(const std::string& pak_name) {
  base::FilePath path;
  if (PathService::Get(base::DIR_MODULE, &path))
    return path.AppendASCII(pak_name.c_str());

  // Return just the name of the pack file.
  return base::FilePath(pak_name.c_str());
}

}  // namespace

void ResourceBundle::LoadCommonResources() {
  AddDataPackFromPath(GetResourcesPakFilePath("chrome.pak"),
                      SCALE_FACTOR_NONE);
  AddDataPackFromPath(GetResourcesPakFilePath(
                      "chrome_100_percent.pak"),
                      SCALE_FACTOR_100P);
}

gfx::Image& ResourceBundle::GetNativeImageNamed(int resource_id, ImageRTL rtl) {
  // Use the negative |resource_id| for the key for BIDI-aware images.
  int key = rtl == RTL_ENABLED ? -resource_id : resource_id;

  // Check to see if the image is already in the cache.
  {
    base::AutoLock lock_scope(*images_and_fonts_lock_);
    if (images_.count(key))
      return images_[key];
  }

  gfx::Image image;
  if (delegate_)
    image = delegate_->GetNativeImageNamed(resource_id, rtl);

  if (image.IsEmpty()) {
    scoped_refptr<base::RefCountedStaticMemory> data(
        LoadDataResourceBytesForScale(resource_id, SCALE_FACTOR_100P));
    GdkPixbuf* pixbuf = LoadPixbuf(data.get(), rtl == RTL_ENABLED);

    if (!pixbuf) {
      LOG(WARNING) << "Unable to load pixbuf with id " << resource_id;
      NOTREACHED();  // Want to assert in debug mode.
      return GetEmptyImage();
    }

    image = gfx::Image(pixbuf);  // Takes ownership.
  }

  base::AutoLock lock_scope(*images_and_fonts_lock_);

  // Another thread raced the load and has already cached the image.
  if (images_.count(key))
    return images_[key];

  images_[key] = image;
  return images_[key];
}

}  // namespace ui
