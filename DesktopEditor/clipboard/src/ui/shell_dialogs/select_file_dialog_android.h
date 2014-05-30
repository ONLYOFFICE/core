// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_ANDROID_SELECT_FILE_DIALOG_ANDROID_H_
#define UI_SHELL_DIALOGS_ANDROID_SELECT_FILE_DIALOG_ANDROID_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/files/file_path.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace ui {

class SelectFileDialogImpl : public ui::SelectFileDialog {
 public:
  static SelectFileDialogImpl* Create(
      Listener* listener,
      ui::SelectFilePolicy* policy);

  void OnFileSelected(JNIEnv* env, jobject java_object, jstring filepath);
  void OnFileNotSelected(JNIEnv* env, jobject java_object);

  // From SelectFileDialog
  virtual bool IsRunning(gfx::NativeWindow) const OVERRIDE;
  virtual void ListenerDestroyed() OVERRIDE;

  // Called when it is time to display the file picker.
  // params is expected to be a Vector<string16> with accept_types first and
  // the capture value as the last element of the vector.
  virtual void SelectFileImpl(
      ui::SelectFileDialog::Type type,
      const string16& title,
      const base::FilePath& default_path,
      const ui::SelectFileDialog::FileTypeInfo* file_types,
      int file_type_index,
      const std::string& default_extension,
      gfx::NativeWindow owning_window,
      void* params) OVERRIDE;

  static bool RegisterSelectFileDialog(JNIEnv* env);

 protected:
  virtual ~SelectFileDialogImpl();

 private:
  SelectFileDialogImpl(Listener* listener,  ui::SelectFilePolicy* policy);

  virtual bool HasMultipleFileTypeChoicesImpl() OVERRIDE;

  base::android::ScopedJavaGlobalRef<jobject> java_object_;

  // Stores the state whether select_file_dialog is running.
  bool is_running_;

  DISALLOW_COPY_AND_ASSIGN(SelectFileDialogImpl);
};

SelectFileDialog* CreateAndroidSelectFileDialog(
    SelectFileDialog::Listener* listener,
    SelectFilePolicy* policy);

}  // namespace ui

#endif  // UI_SHELL_DIALOGS_ANDROID_SELECT_FILE_DIALOG_ANDROID_H_

