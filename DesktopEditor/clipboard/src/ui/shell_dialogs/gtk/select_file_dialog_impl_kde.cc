// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <set>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/message_loop.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/nix/mime_util_xdg.h"
#include "base/nix/xdg_util.h"
#include "base/process_util.h"
#include "base/string_number_conversions.h"
#include "base/string_util.h"
#include "base/threading/thread_restrictions.h"
#include "base/threading/worker_pool.h"
#include "base/utf_string_conversions.h"
#include "grit/ui_strings.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/shell_dialogs/gtk/select_file_dialog_impl.h"

// These conflict with base/tracked_objects.h, so need to come last.
#include <gdk/gdkx.h>
#include <gtk/gtk.h>

namespace {

std::string GetTitle(const std::string& title, int message_id) {
  return title.empty() ? l10n_util::GetStringUTF8(message_id) : title;
}

const char kKdialogBinary[] = "kdialog";

// Implementation of SelectFileDialog that shows a KDE common dialog for
// choosing a file or folder. This acts as a modal dialog.
class SelectFileDialogImplKDE : public ui::SelectFileDialogImpl {
 public:
  SelectFileDialogImplKDE(Listener* listener,
                          ui::SelectFilePolicy* policy,
                          base::nix::DesktopEnvironment desktop);

 protected:
  virtual ~SelectFileDialogImplKDE();

  // SelectFileDialog implementation.
  // |params| is user data we pass back via the Listener interface.
  virtual void SelectFileImpl(
      Type type,
      const string16& title,
      const base::FilePath& default_path,
      const FileTypeInfo* file_types,
      int file_type_index,
      const base::FilePath::StringType& default_extension,
      gfx::NativeWindow owning_window,
      void* params) OVERRIDE;

 private:
  virtual bool HasMultipleFileTypeChoicesImpl() OVERRIDE;

  struct KDialogParams {
    // This constructor can only be run from the UI thread.
    KDialogParams(const std::string& type,
                  const std::string& title,
                  const base::FilePath& default_path,
                  gfx::NativeWindow parent,
                  bool file_operation,
                  bool multiple_selection,
                  void* kdialog_params,
                  void(SelectFileDialogImplKDE::* callback)(const std::string&,
                                                            int,
                                                            void*))
        : type(type),
          title(title),
          default_path(default_path),
          parent(parent),
          file_operation(file_operation),
          multiple_selection(multiple_selection),
          kdialog_params(kdialog_params),
          ui_loop_proxy(
              base::MessageLoopForUI::current()->message_loop_proxy()),
          callback(callback) {}

    std::string type;
    std::string title;
    base::FilePath default_path;
    gfx::NativeWindow parent;
    bool file_operation;
    bool multiple_selection;
    void* kdialog_params;
    scoped_refptr<base::MessageLoopProxy> ui_loop_proxy;

    void (SelectFileDialogImplKDE::*callback)(const std::string&, int, void*);
  };

  // Get the filters from |file_types_| and concatenate them into
  // |filter_string|.
  std::string GetMimeTypeFilterString();

  // Get KDialog command line representing the Argv array for KDialog.
  void GetKDialogCommandLine(const std::string& type, const std::string& title,
      const base::FilePath& default_path, gfx::NativeWindow parent,
      bool file_operation, bool multiple_selection, CommandLine* command_line);

  // Call KDialog on a worker thread and post results back to the caller
  // thread.
  void CallKDialogOutput(const KDialogParams& params);

  // Notifies the listener that a single file was chosen.
  void FileSelected(const base::FilePath& path, void* params);

  // Notifies the listener that multiple files were chosen.
  void MultiFilesSelected(const std::vector<base::FilePath>& files,
                          void* params);

  // Notifies the listener that no file was chosen (the action was canceled).
  // Dialog is passed so we can find that |params| pointer that was passed to
  // us when we were told to show the dialog.
  void FileNotSelected(void *params);

  void CreateSelectFolderDialog(const std::string& title,
                                const base::FilePath& default_path,
                                gfx::NativeWindow parent, void* params);

  void CreateFileOpenDialog(const std::string& title,
                                  const base::FilePath& default_path,
                                  gfx::NativeWindow parent, void* params);

  void CreateMultiFileOpenDialog(const std::string& title,
                                 const base::FilePath& default_path,
                                 gfx::NativeWindow parent, void* params);

  void CreateSaveAsDialog(const std::string& title,
                          const base::FilePath& default_path,
                          gfx::NativeWindow parent, void* params);

  // Common function for OnSelectSingleFileDialogResponse and
  // OnSelectSingleFolderDialogResponse.
  void SelectSingleFileHelper(const std::string& output, int exit_code,
                              void* params, bool allow_folder);

  void OnSelectSingleFileDialogResponse(const std::string& output,
                                        int exit_code, void* params);
  void OnSelectMultiFileDialogResponse(const std::string& output,
                                       int exit_code, void* params);
  void OnSelectSingleFolderDialogResponse(const std::string& output,
                                          int exit_code, void* params);

  // Should be either DESKTOP_ENVIRONMENT_KDE3 or DESKTOP_ENVIRONMENT_KDE4.
  base::nix::DesktopEnvironment desktop_;

  DISALLOW_COPY_AND_ASSIGN(SelectFileDialogImplKDE);
};

SelectFileDialogImplKDE::SelectFileDialogImplKDE(
    Listener* listener,
    ui::SelectFilePolicy* policy,
    base::nix::DesktopEnvironment desktop)
    : SelectFileDialogImpl(listener, policy),
      desktop_(desktop) {
  DCHECK(desktop_ == base::nix::DESKTOP_ENVIRONMENT_KDE3 ||
         desktop_ == base::nix::DESKTOP_ENVIRONMENT_KDE4);
}

SelectFileDialogImplKDE::~SelectFileDialogImplKDE() {
}

// We ignore |default_extension|.
void SelectFileDialogImplKDE::SelectFileImpl(
    Type type,
    const string16& title,
    const base::FilePath& default_path,
    const FileTypeInfo* file_types,
    int file_type_index,
    const base::FilePath::StringType& default_extension,
    gfx::NativeWindow owning_window,
    void* params) {
  type_ = type;
  // |owning_window| can be null when user right-clicks on a downloadable item
  // and chooses 'Open Link in New Tab' when 'Ask where to save each file
  // before downloading.' preference is turned on. (http://crbug.com/29213)
  if (owning_window)
    parents_.insert(owning_window);

  std::string title_string = UTF16ToUTF8(title);

  file_type_index_ = file_type_index;
  if (file_types)
    file_types_ = *file_types;
  else
    file_types_.include_all_files = true;

  switch (type) {
    case SELECT_FOLDER:
      CreateSelectFolderDialog(title_string, default_path,
                               owning_window, params);
      return;
    case SELECT_OPEN_FILE:
      CreateFileOpenDialog(title_string, default_path, owning_window,
                           params);
      return;
    case SELECT_OPEN_MULTI_FILE:
      CreateMultiFileOpenDialog(title_string, default_path,
                                owning_window, params);
      return;
    case SELECT_SAVEAS_FILE:
      CreateSaveAsDialog(title_string, default_path, owning_window,
                         params);
      return;
    default:
      NOTREACHED();
      return;
  }
}

bool SelectFileDialogImplKDE::HasMultipleFileTypeChoicesImpl() {
  return file_types_.extensions.size() > 1;
}

std::string SelectFileDialogImplKDE::GetMimeTypeFilterString() {
  std::string filter_string;
  // We need a filter set because the same mime type can appear multiple times.
  std::set<std::string> filter_set;
  for (size_t i = 0; i < file_types_.extensions.size(); ++i) {
    for (size_t j = 0; j < file_types_.extensions[i].size(); ++j) {
      if (!file_types_.extensions[i][j].empty()) {
        std::string mime_type = base::nix::GetFileMimeType(
            base::FilePath("name").ReplaceExtension(
                file_types_.extensions[i][j]));
        filter_set.insert(mime_type);
      }
    }
  }
  // Add the *.* filter, but only if we have added other filters (otherwise it
  // is implied).
  if (file_types_.include_all_files && !file_types_.extensions.empty())
    filter_set.insert("application/octet-stream");
  // Create the final output string.
  filter_string.clear();
  for (std::set<std::string>::iterator it = filter_set.begin();
       it != filter_set.end(); ++it) {
    filter_string.append(*it + " ");
  }
  return filter_string;
}

void SelectFileDialogImplKDE::CallKDialogOutput(const KDialogParams& params) {
  CommandLine::StringVector cmd_vector;
  cmd_vector.push_back(kKdialogBinary);
  CommandLine command_line(cmd_vector);
  GetKDialogCommandLine(params.type, params.title, params.default_path,
                        params.parent, params.file_operation,
                        params.multiple_selection, &command_line);
  std::string output;
  int exit_code;
  // Get output from KDialog
  base::GetAppOutputWithExitCode(command_line, &output, &exit_code);
  if (!output.empty())
    output.erase(output.size() - 1);
  // Now the dialog is no longer showing. We can erase its parent from the
  // parent set.
  std::set<GtkWindow*>::iterator iter = parents_.find(params.parent);
  if (iter != parents_.end())
    parents_.erase(iter);
  params.ui_loop_proxy->PostTask(FROM_HERE,
      base::Bind(params.callback, this, output, exit_code,
                 params.kdialog_params));
}

void SelectFileDialogImplKDE::GetKDialogCommandLine(const std::string& type,
    const std::string& title, const base::FilePath& path,
    gfx::NativeWindow parent, bool file_operation, bool multiple_selection,
    CommandLine* command_line) {
  CHECK(command_line);

  // Attach to the current Chrome window.
  GdkWindow* gdk_window = gtk_widget_get_window(GTK_WIDGET((parent)));
  int window_id = GDK_DRAWABLE_XID(gdk_window);
  command_line->AppendSwitchNative(
      desktop_ == base::nix::DESKTOP_ENVIRONMENT_KDE3 ? "--embed" : "--attach",
      base::IntToString(window_id));
  // Set the correct title for the dialog.
  if (!title.empty())
    command_line->AppendSwitchNative("--title", title);
  // Enable multiple file selection if we need to.
  if (multiple_selection) {
    command_line->AppendSwitch("--multiple");
    command_line->AppendSwitch("--separate-output");
  }
  command_line->AppendSwitch(type);
  // The path should never be empty. If it is, set it to PWD.
  if (path.empty())
    command_line->AppendArgPath(base::FilePath("."));
  else
    command_line->AppendArgPath(path);
  // Depending on the type of the operation we need, get the path to the
  // file/folder and set up mime type filters.
  if (file_operation)
    command_line->AppendArg(GetMimeTypeFilterString());
  VLOG(1) << "KDialog command line: " << command_line->GetCommandLineString();
}

void SelectFileDialogImplKDE::FileSelected(const base::FilePath& path,
                                           void* params) {
  if (type_ == SELECT_SAVEAS_FILE)
    *last_saved_path_ = path.DirName();
  else if (type_ == SELECT_OPEN_FILE)
    *last_opened_path_ = path.DirName();
  else if (type_ == SELECT_FOLDER)
    *last_opened_path_ = path;
  else
    NOTREACHED();
  if (listener_) {  // What does the filter index actually do?
    // TODO(dfilimon): Get a reasonable index value from somewhere.
    listener_->FileSelected(path, 1, params);
  }
}

void SelectFileDialogImplKDE::MultiFilesSelected(
    const std::vector<base::FilePath>& files, void* params) {
  *last_opened_path_ = files[0].DirName();
  if (listener_)
    listener_->MultiFilesSelected(files, params);
}

void SelectFileDialogImplKDE::FileNotSelected(void* params) {
  if (listener_)
    listener_->FileSelectionCanceled(params);
}

void SelectFileDialogImplKDE::CreateSelectFolderDialog(
    const std::string& title, const base::FilePath& default_path,
    gfx::NativeWindow parent, void *params) {
  base::WorkerPool::PostTask(FROM_HERE,
      base::Bind(
          &SelectFileDialogImplKDE::CallKDialogOutput,
          this,
          KDialogParams(
              "--getexistingdirectory",
              GetTitle(title, IDS_SELECT_FOLDER_DIALOG_TITLE),
              default_path.empty() ? *last_opened_path_ : default_path,
              parent, false, false, params,
              &SelectFileDialogImplKDE::OnSelectSingleFolderDialogResponse)),
      true);
}

void SelectFileDialogImplKDE::CreateFileOpenDialog(
    const std::string& title, const base::FilePath& default_path,
    gfx::NativeWindow parent, void* params) {
  base::WorkerPool::PostTask(FROM_HERE,
      base::Bind(
          &SelectFileDialogImplKDE::CallKDialogOutput,
          this,
          KDialogParams(
              "--getopenfilename",
              GetTitle(title, IDS_OPEN_FILE_DIALOG_TITLE),
              default_path.empty() ? *last_opened_path_ : default_path,
              parent, true, false, params,
              &SelectFileDialogImplKDE::OnSelectSingleFileDialogResponse)),
      true);
}

void SelectFileDialogImplKDE::CreateMultiFileOpenDialog(
    const std::string& title, const base::FilePath& default_path,
    gfx::NativeWindow parent, void* params) {
  base::WorkerPool::PostTask(FROM_HERE,
      base::Bind(
          &SelectFileDialogImplKDE::CallKDialogOutput,
          this,
          KDialogParams(
              "--getopenfilename",
              GetTitle(title, IDS_OPEN_FILES_DIALOG_TITLE),
              default_path.empty() ? *last_opened_path_ : default_path,
              parent, true, true, params,
              &SelectFileDialogImplKDE::OnSelectMultiFileDialogResponse)),
      true);
}

void SelectFileDialogImplKDE::CreateSaveAsDialog(
    const std::string& title, const base::FilePath& default_path,
    gfx::NativeWindow parent, void* params) {
  base::WorkerPool::PostTask(FROM_HERE,
      base::Bind(
          &SelectFileDialogImplKDE::CallKDialogOutput,
          this,
          KDialogParams(
              "--getsavefilename",
              GetTitle(title, IDS_SAVE_AS_DIALOG_TITLE),
              default_path.empty() ? *last_saved_path_ : default_path,
              parent, true, false, params,
              &SelectFileDialogImplKDE::OnSelectSingleFileDialogResponse)),
      true);
}

void SelectFileDialogImplKDE::SelectSingleFileHelper(const std::string& output,
    int exit_code, void* params, bool allow_folder) {
  VLOG(1) << "[kdialog] SingleFileResponse: " << output;
  if (exit_code != 0 || output.empty()) {
    FileNotSelected(params);
    return;
  }

  base::FilePath path(output);
  if (allow_folder) {
    FileSelected(path, params);
    return;
  }

  if (CallDirectoryExistsOnUIThread(path))
    FileNotSelected(params);
  else
    FileSelected(path, params);
}

void SelectFileDialogImplKDE::OnSelectSingleFileDialogResponse(
    const std::string& output, int exit_code, void* params) {
  SelectSingleFileHelper(output, exit_code, params, false);
}

void SelectFileDialogImplKDE::OnSelectSingleFolderDialogResponse(
    const std::string& output, int exit_code, void* params) {
  SelectSingleFileHelper(output, exit_code, params, true);
}

void SelectFileDialogImplKDE::OnSelectMultiFileDialogResponse(
    const std::string& output, int exit_code, void* params) {
  VLOG(1) << "[kdialog] MultiFileResponse: " << output;

  if (exit_code != 0 || output.empty()) {
    FileNotSelected(params);
    return;
  }

  std::vector<std::string> filenames;
  Tokenize(output, "\n", &filenames);
  std::vector<base::FilePath> filenames_fp;
  for (std::vector<std::string>::iterator iter = filenames.begin();
       iter != filenames.end(); ++iter) {
    base::FilePath path(*iter);
    if (CallDirectoryExistsOnUIThread(path))
      continue;
    filenames_fp.push_back(path);
  }

  if (filenames_fp.empty()) {
    FileNotSelected(params);
    return;
  }
  MultiFilesSelected(filenames_fp, params);
}

}  // namespace

namespace ui {

// static
bool SelectFileDialogImpl::CheckKDEDialogWorksOnUIThread() {
  // No choice. UI thread can't continue without an answer here. Fortunately we
  // only do this once, the first time a file dialog is displayed.
  base::ThreadRestrictions::ScopedAllowIO allow_io;

  CommandLine::StringVector cmd_vector;
  cmd_vector.push_back(kKdialogBinary);
  cmd_vector.push_back("--version");
  CommandLine command_line(cmd_vector);
  std::string dummy;
  return base::GetAppOutput(command_line, &dummy);
}

// static
SelectFileDialogImpl* SelectFileDialogImpl::NewSelectFileDialogImplKDE(
    Listener* listener,
    ui::SelectFilePolicy* policy,
    base::nix::DesktopEnvironment desktop) {
  return new SelectFileDialogImplKDE(listener, policy, desktop);
}

}  // namespace ui

