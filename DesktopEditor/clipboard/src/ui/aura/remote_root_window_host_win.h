// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_REMOTE_ROOT_WINDOW_HOST_WIN_H_
#define UI_AURA_REMOTE_ROOT_WINDOW_HOST_WIN_H_

#include <vector>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/string16.h"
#include "ui/aura/root_window_host.h"
#include "ui/base/events/event_constants.h"
#include "ui/gfx/native_widget_types.h"

namespace base {
class FilePath;
}

namespace ui {
class ViewProp;
}

namespace IPC {
class Message;
class Sender;
}

namespace aura {

typedef base::Callback<void(const base::FilePath&, int, void*)>
    OpenFileCompletion;

typedef base::Callback<void(const std::vector<base::FilePath>&, void*)>
    OpenMultipleFilesCompletion;

typedef base::Callback<void(const base::FilePath&, int, void*)>
    SaveFileCompletion;

typedef base::Callback<void(const base::FilePath&, int, void*)>
    SelectFolderCompletion;

// Handles the open file operation for Metro Chrome Ash. The callback passed in
// is invoked when we receive the opened file name from the metro viewer.
AURA_EXPORT void HandleOpenFile(
    const string16& title,
    const base::FilePath& default_path,
    const string16& filter,
    const OpenFileCompletion& callback);

// Handles the open multiple file operation for Metro Chrome Ash. The callback
// passed in is invoked when we receive the opened file names from the metro
// viewer.
AURA_EXPORT void HandleOpenMultipleFiles(
    const string16& title,
    const base::FilePath& default_path,
    const string16& filter,
    const OpenMultipleFilesCompletion& callback);

// Handles the save file operation for Metro Chrome Ash. The callback passed in
// is invoked when we receive the saved file name from the metro viewer.
AURA_EXPORT void HandleSaveFile(
    const string16& title,
    const base::FilePath& default_path,
    const string16& filter,
    int filter_index,
    const string16& default_extension,
    const SaveFileCompletion& callback);

// Handles the select folder for Metro Chrome Ash. The callback passed in
// is invoked when we receive the folder name from the metro viewer.
AURA_EXPORT void HandleSelectFolder(const string16& title,
                                    const SelectFolderCompletion& callback);

// RootWindowHost implementaton that receives events from a different
// process. In the case of Windows this is the Windows 8 (aka Metro)
// frontend process, which forwards input events to this class.
class AURA_EXPORT RemoteRootWindowHostWin : public RootWindowHost {
 public:
  static RemoteRootWindowHostWin* Instance();
  static RemoteRootWindowHostWin* Create(const gfx::Rect& bounds);

  // Called when the remote process has established its IPC connection.
  // The |host| can be used when we need to send a message to it.
  void Connected(IPC::Sender* host);
  // Called when the remote process has closed its IPC connection.
  void Disconnected();

  // Called when we have a message from the remote process.
  bool OnMessageReceived(const IPC::Message& message);

  void HandleOpenFile(
      const string16& title,
      const base::FilePath& default_path,
      const string16& filter,
      const OpenFileCompletion& callback);

  void HandleOpenMultipleFiles(
      const string16& title,
      const base::FilePath& default_path,
      const string16& filter,
      const OpenMultipleFilesCompletion& callback);

  void HandleSaveFile(
      const string16& title,
      const base::FilePath& default_path,
      const string16& filter,
      int filter_index,
      const string16& default_extension,
      const SaveFileCompletion& callback);

  void HandleSelectFolder(const string16& title,
                          const SelectFolderCompletion& callback);

 private:
  explicit RemoteRootWindowHostWin(const gfx::Rect& bounds);
  virtual ~RemoteRootWindowHostWin();

  // IPC message handing methods:
  void OnMouseMoved(int32 x, int32 y, int32 flags);
  void OnMouseButton(int32 x,
                     int32 y,
                     int32 extra,
                     ui::EventType type,
                     ui::EventFlags flags);
  void OnKeyDown(uint32 vkey,
                 uint32 repeat_count,
                 uint32 scan_code,
                 uint32 flags);
  void OnKeyUp(uint32 vkey,
               uint32 repeat_count,
               uint32 scan_code,
               uint32 flags);
  void OnChar(uint32 key_code,
              uint32 repeat_count,
              uint32 scan_code,
              uint32 flags);
  void OnVisibilityChanged(bool visible);
  void OnTouchDown(int32 x, int32 y, uint64 timestamp, uint32 pointer_id);
  void OnTouchUp(int32 x, int32 y, uint64 timestamp, uint32 pointer_id);
  void OnTouchMoved(int32 x, int32 y, uint64 timestamp, uint32 pointer_id);
  void OnFileSaveAsDone(bool success,
                        const base::FilePath& filename,
                        int filter_index);
  void OnFileOpenDone(bool success, const base::FilePath& filename);
  void OnMultiFileOpenDone(bool success,
                           const std::vector<base::FilePath>& files);
  void OnSelectFolderDone(bool success, const base::FilePath& folder);
  void OnWindowActivated(bool active);
  // RootWindowHost overrides:
  virtual void SetDelegate(RootWindowHostDelegate* delegate) OVERRIDE;
  virtual RootWindow* GetRootWindow() OVERRIDE;
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void ToggleFullScreen() OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual void SetInsets(const gfx::Insets& insets) OVERRIDE;
  virtual gfx::Point GetLocationOnNativeScreen() const OVERRIDE;
  virtual void SetCapture() OVERRIDE;
  virtual void ReleaseCapture() OVERRIDE;
  virtual void SetCursor(gfx::NativeCursor cursor) OVERRIDE;
  virtual bool QueryMouseLocation(gfx::Point* location_return) OVERRIDE;
  virtual bool ConfineCursorToRootWindow() OVERRIDE;
  virtual void UnConfineCursor() OVERRIDE;
  virtual void OnCursorVisibilityChanged(bool show) OVERRIDE;
  virtual void MoveCursorTo(const gfx::Point& location) OVERRIDE;
  virtual void SetFocusWhenShown(bool focus_when_shown) OVERRIDE;
  virtual bool CopyAreaToSkCanvas(const gfx::Rect& source_bounds,
                                  const gfx::Point& dest_offset,
                                  SkCanvas* canvas) OVERRIDE;
  virtual void PostNativeEvent(const base::NativeEvent& native_event) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual void PrepareForShutdown() OVERRIDE;

  // Helper function to dispatch a keyboard message to the desired target.
  // The default target is the RootWindowHostDelegate. For nested message loop
  // invocations we post a synthetic keyboard message directly into the message
  // loop. The dispatcher for the nested loop would then decide how this
  // message is routed.
  void DispatchKeyboardMessage(ui::EventType type,
                               uint32 vkey,
                               uint32 repeat_count,
                               uint32 scan_code,
                               uint32 flags,
                               bool is_character);

  RootWindowHostDelegate* delegate_;
  IPC::Sender* host_;
  scoped_ptr<ui::ViewProp> prop_;

  // Saved callbacks which inform the caller about the result of the open file/
  // save file/select operations.
  OpenFileCompletion file_open_completion_callback_;
  OpenMultipleFilesCompletion multi_file_open_completion_callback_;
  SaveFileCompletion file_saveas_completion_callback_;
  SelectFolderCompletion select_folder_completion_callback_;

  DISALLOW_COPY_AND_ASSIGN(RemoteRootWindowHostWin);
};

}  // namespace aura

#endif  // UI_AURA_REMOTE_ROOT_WINDOW_HOST_WIN_H_
