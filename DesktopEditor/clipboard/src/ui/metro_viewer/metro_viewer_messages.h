// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Multiply-included message file, no include guard.

#include <vector>

#include "base/basictypes.h"
#include "ipc/ipc_message_macros.h"
#include "ui/base/events/event_constants.h"
#include "ui/gfx/native_widget_types.h"

#define IPC_MESSAGE_START MetroViewerMsgStart

IPC_ENUM_TRAITS(ui::EventType)
IPC_ENUM_TRAITS(ui::EventFlags)

// Messages sent from the viewer to the browser:

// Inform the browser of the surface to target for compositing.
IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_SetTargetSurface,
                     gfx::NativeViewId /* target hwnd */)
// Informs the browser that the mouse moved.
IPC_MESSAGE_CONTROL3(MetroViewerHostMsg_MouseMoved,
                     int32,       /* x-coordinate */
                     int32,       /* y-coordinate */
                     int32        /* flags */)
// Informs the brower that a mouse button was pressed.
IPC_MESSAGE_CONTROL5(MetroViewerHostMsg_MouseButton,
                     int32,           /* x-coordinate */
                     int32,           /* y-coordinate */
                     int32,           /* extra */
                     ui::EventType,   /* event type */
                     ui::EventFlags   /* event flags */)
// Informs the browser that a key was pressed.
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_KeyDown,
                     uint32,       /* virtual key */
                     uint32,       /* repeat count */
                     uint32,       /* scan code */
                     uint32        /* key state */);
// Informs the browser that a key was released.
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_KeyUp,
                     uint32,       /* virtual key */
                     uint32,       /* repeat count */
                     uint32,       /* scan code */
                     uint32        /* key state */);
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_Character,
                     uint32,       /* virtual key */
                     uint32,       /* repeat count */
                     uint32,       /* scan code */
                     uint32        /* key state */);
// Informs the browser that the visibiliy of the viewer has changed.
IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_VisibilityChanged,
                     bool          /* visible */);

IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_TouchDown,
                     int32,           /* x-coordinate */
                     int32,           /* y-coordinate */
                     uint64,          /* timestamp */
                     uint32)          /* pointer_id */
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_TouchUp,
                     int32,           /* x-coordinate */
                     int32,           /* y-coordinate */
                     uint64,          /* timestamp */
                     uint32)          /* pointer_id */
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_TouchMoved,
                     int32,           /* x-coordinate */
                     int32,           /* y-coordinate */
                     uint64,          /* timestamp */
                     uint32)          /* pointer_id */

// Informs the browser of the result of a file save as operation.
IPC_MESSAGE_CONTROL3(MetroViewerHostMsg_FileSaveAsDone,
                     bool,           /* success */
                     base::FilePath, /* filename */
                     int)            /* filter_index */

// Informs the browser of the result of a file open operation.
IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_FileOpenDone,
                     bool,           /* success */
                     base::FilePath) /* filename */

IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_MultiFileOpenDone,
                     bool,                    /* success */
                     std::vector<base::FilePath>)   /* filenames */

// Informs the browser of the result of a select folder operation.
IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_SelectFolderDone,
                     bool,           /* success */
                     base::FilePath) /* filepath*/

// Messages sent from the browser to the viewer:

// Requests the viewer to change the pointer to a new cursor.
IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_SetCursor,
                     int64         /* cursor */);

// This structure contains the parameters sent to the viewer process to display
// the file save dialog.
IPC_STRUCT_BEGIN(MetroViewerHostMsg_SaveAsDialogParams)

  // The title of the file save dialog if any.
  IPC_STRUCT_MEMBER(string16, title)

  // The suggested file name.
  IPC_STRUCT_MEMBER(base::FilePath, suggested_name)

  // The save as filter to be used.
  IPC_STRUCT_MEMBER(string16, filter)

  // The filter index.
  IPC_STRUCT_MEMBER(uint32, filter_index)

  // The default extension.
  IPC_STRUCT_MEMBER(string16, default_extension)

IPC_STRUCT_END()

// Requests the viewer to display the file save dialog.
IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_DisplayFileSaveAs,
                     MetroViewerHostMsg_SaveAsDialogParams)

// Requests the viewer to display the file open dialog.
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_DisplayFileOpen,
                     string16,       /* title */
                     string16,       /* filter */
                     base::FilePath, /* Default path */
                     bool)           /* allow_multi_select */

// Requests the viewer to display the select folder dialog.
IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_DisplaySelectFolder,
                     string16)   /* title */

// Informs the browser about the viewer activation state, i.e. active, lost
// activation etc.
IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_WindowActivated,
                     bool) /* active */
