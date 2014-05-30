// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_DRAGDROP_OS_EXCHANGE_DATA_PROVIDER_AURAX11_H_
#define UI_BASE_DRAGDROP_OS_EXCHANGE_DATA_PROVIDER_AURAX11_H_

#include <X11/Xlib.h>

// Get rid of a macro from Xlib.h that conflicts with Aura's RootWindow class.
#undef RootWindow

#include <map>

#include "base/files/file_path.h"
#include "base/message_pump_dispatcher.h"
#include "base/pickle.h"
#include "googleurl/src/gurl.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/x/selection_owner.h"
#include "ui/base/x/selection_requestor.h"
#include "ui/base/x/x11_atom_cache.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/vector2d.h"

namespace ui {

class Clipboard;
class DesktopSelectionProviderAuraX11;

// OSExchangeData::Provider implementation for aura on linux.
class UI_EXPORT OSExchangeDataProviderAuraX11
    : public OSExchangeData::Provider,
      public base::MessagePumpDispatcher {
 public:
  // Creates a Provider for drag receiving. |x_window| is the window the cursor
  // is over, and |targets| are the MIME types being offered by the other
  // process.
  OSExchangeDataProviderAuraX11(ui::DesktopSelectionProviderAuraX11* provider,
                                ::Window x_window,
                                const std::vector< ::Atom> targets);

  // Creates a Provider for sending drag information. This creates its own,
  // hidden X11 window to own send data.
  OSExchangeDataProviderAuraX11();

  virtual ~OSExchangeDataProviderAuraX11();

  // If we are receiving, we receive messages from a DesktopRootWindowHost
  // through this interface.
  void OnSelectionNotify(const XSelectionEvent& event);

  // Overridden from OSExchangeData::Provider:
  virtual void SetString(const string16& data) OVERRIDE;
  virtual void SetURL(const GURL& url, const string16& title) OVERRIDE;
  virtual void SetFilename(const base::FilePath& path) OVERRIDE;
  virtual void SetFilenames(
      const std::vector<OSExchangeData::FileInfo>& filenames) OVERRIDE;
  virtual void SetPickledData(OSExchangeData::CustomFormat format,
                              const Pickle& data) OVERRIDE;
  virtual bool GetString(string16* data) const OVERRIDE;
  virtual bool GetURLAndTitle(GURL* url, string16* title) const OVERRIDE;
  virtual bool GetFilename(base::FilePath* path) const OVERRIDE;
  virtual bool GetFilenames(
      std::vector<OSExchangeData::FileInfo>* filenames) const OVERRIDE;
  virtual bool GetPickledData(OSExchangeData::CustomFormat format,
                              Pickle* data) const OVERRIDE;
  virtual bool HasString() const OVERRIDE;
  virtual bool HasURL() const OVERRIDE;
  virtual bool HasFile() const OVERRIDE;
  virtual bool HasCustomFormat(
      OSExchangeData::CustomFormat format) const OVERRIDE;

  virtual void SetHtml(const string16& html, const GURL& base_url) OVERRIDE;
  virtual bool GetHtml(string16* html, GURL* base_url) const OVERRIDE;
  virtual bool HasHtml() const OVERRIDE;
  virtual void SetDragImage(const gfx::ImageSkia& image,
                            const gfx::Vector2d& cursor_offset) OVERRIDE;
  virtual const gfx::ImageSkia& GetDragImage() const OVERRIDE;
  virtual const gfx::Vector2d& GetDragImageOffset() const OVERRIDE;

  // Overridden from base::MessagePumpDispatcher:
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

 private:
  typedef std::map<OSExchangeData::CustomFormat, Pickle>  PickleData;

  // Returns true if |formats_| contains a string format and the string can be
  // parsed as a URL.
  bool GetPlainTextURL(GURL* url) const;

  // Drag image and offset data.
  gfx::ImageSkia drag_image_;
  gfx::Vector2d drag_image_offset_;

  // Our X11 state.
  Display* x_display_;
  ::Window x_root_window_;

  // In X11, because the IPC parts of drag operations are implemented by
  // XSelection, we require an x11 window to receive drag messages on. The
  // OSExchangeDataProvider system is modeled on the Windows implementation,
  // which does not require a window. We only sometimes have a valid window
  // available (in the case of drag receiving). Other times, we need to create
  // our own xwindow just to receive events on it.
  const bool own_window_;

  // When we don't own the window, we keep track of the object that does so we
  // can receive messages from it.
  ui::DesktopSelectionProviderAuraX11* selection_event_provider_;

  ::Window x_window_;

  X11AtomCache atom_cache_;

  mutable SelectionRequestor selection_requestor_;
  mutable SelectionOwner selection_owner_;

  // The mime types we have been offered by the source window.
  std::vector< ::Atom> targets_;

  DISALLOW_COPY_AND_ASSIGN(OSExchangeDataProviderAuraX11);
};

}  // namespace ui

#endif  // UI_BASE_DRAGDROP_OS_EXCHANGE_DATA_PROVIDER_AURAX11_H_
