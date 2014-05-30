// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtk/gtk.h>
#include <map>
#include <set>
#include <vector>

#include "base/file_util.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/string_util.h"
#include "base/strings/sys_string_conversions.h"
#include "base/threading/thread.h"
#include "base/threading/thread_restrictions.h"
#include "base/utf_string_conversions.h"
#include "grit/ui_strings.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/shell_dialogs/gtk/select_file_dialog_impl.h"
#include "ui/shell_dialogs/select_file_dialog.h"

namespace {

// Makes sure that .jpg also shows .JPG.
gboolean FileFilterCaseInsensitive(const GtkFileFilterInfo* file_info,
                                   std::string* file_extension) {
  return EndsWith(file_info->filename, *file_extension, false);
}

// Deletes |data| when gtk_file_filter_add_custom() is done with it.
void OnFileFilterDataDestroyed(std::string* file_extension) {
  delete file_extension;
}

// Implementation of SelectFileDialog that shows a Gtk common dialog for
// choosing a file or folder. This acts as a modal dialog.
class SelectFileDialogImplGTK : public ui::SelectFileDialogImpl {
 public:
  explicit SelectFileDialogImplGTK(Listener* listener,
                                   ui::SelectFilePolicy* policy);

 protected:
  virtual ~SelectFileDialogImplGTK();

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

  // Add the filters from |file_types_| to |chooser|.
  void AddFilters(GtkFileChooser* chooser);

  // Notifies the listener that a single file was chosen.
  void FileSelected(GtkWidget* dialog, const base::FilePath& path);

  // Notifies the listener that multiple files were chosen.
  void MultiFilesSelected(GtkWidget* dialog,
                          const std::vector<base::FilePath>& files);

  // Notifies the listener that no file was chosen (the action was canceled).
  // Dialog is passed so we can find that |params| pointer that was passed to
  // us when we were told to show the dialog.
  void FileNotSelected(GtkWidget* dialog);

  GtkWidget* CreateSelectFolderDialog(const std::string& title,
      const base::FilePath& default_path, gfx::NativeWindow parent);

  GtkWidget* CreateFileOpenDialog(const std::string& title,
      const base::FilePath& default_path, gfx::NativeWindow parent);

  GtkWidget* CreateMultiFileOpenDialog(const std::string& title,
      const base::FilePath& default_path, gfx::NativeWindow parent);

  GtkWidget* CreateSaveAsDialog(const std::string& title,
      const base::FilePath& default_path, gfx::NativeWindow parent);

  // Removes and returns the |params| associated with |dialog| from
  // |params_map_|.
  void* PopParamsForDialog(GtkWidget* dialog);

  // Take care of internal data structures when a file dialog is destroyed.
  void FileDialogDestroyed(GtkWidget* dialog);

  // Check whether response_id corresponds to the user cancelling/closing the
  // dialog. Used as a helper for the below callbacks.
  bool IsCancelResponse(gint response_id);

  // Common function for OnSelectSingleFileDialogResponse and
  // OnSelectSingleFolderDialogResponse.
  void SelectSingleFileHelper(GtkWidget* dialog,
                              gint response_id,
                              bool allow_folder);

  // Common function for CreateFileOpenDialog and CreateMultiFileOpenDialog.
  GtkWidget* CreateFileOpenHelper(const std::string& title,
                                  const base::FilePath& default_path,
                                  gfx::NativeWindow parent);

  // Callback for when the user responds to a Save As or Open File dialog.
  CHROMEGTK_CALLBACK_1(SelectFileDialogImplGTK, void,
                       OnSelectSingleFileDialogResponse, int);

  // Callback for when the user responds to a Select Folder dialog.
  CHROMEGTK_CALLBACK_1(SelectFileDialogImplGTK, void,
                       OnSelectSingleFolderDialogResponse, int);

  // Callback for when the user responds to a Open Multiple Files dialog.
  CHROMEGTK_CALLBACK_1(SelectFileDialogImplGTK, void,
                       OnSelectMultiFileDialogResponse, int);

  // Callback for when the file chooser gets destroyed.
  CHROMEGTK_CALLBACK_0(SelectFileDialogImplGTK, void, OnFileChooserDestroy);

  // Callback for when we update the preview for the selection.
  CHROMEGTK_CALLBACK_0(SelectFileDialogImplGTK, void, OnUpdatePreview);

  // A map from dialog windows to the |params| user data associated with them.
  std::map<GtkWidget*, void*> params_map_;

  // The GtkImage widget for showing previews of selected images.
  GtkWidget* preview_;

  // All our dialogs.
  std::set<GtkWidget*> dialogs_;

  DISALLOW_COPY_AND_ASSIGN(SelectFileDialogImplGTK);
};

// The size of the preview we display for selected image files. We set height
// larger than width because generally there is more free space vertically
// than horiztonally (setting the preview image will alway expand the width of
// the dialog, but usually not the height). The image's aspect ratio will always
// be preserved.
static const int kPreviewWidth = 256;
static const int kPreviewHeight = 512;

SelectFileDialogImplGTK::SelectFileDialogImplGTK(Listener* listener,
                                                 ui::SelectFilePolicy* policy)
    : SelectFileDialogImpl(listener, policy),
      preview_(NULL) {
}

SelectFileDialogImplGTK::~SelectFileDialogImplGTK() {
  while (dialogs_.begin() != dialogs_.end()) {
    gtk_widget_destroy(*(dialogs_.begin()));
  }
}

bool SelectFileDialogImplGTK::HasMultipleFileTypeChoicesImpl() {
  return file_types_.extensions.size() > 1;
}

// We ignore |default_extension|.
void SelectFileDialogImplGTK::SelectFileImpl(
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

  GtkWidget* dialog = NULL;
  switch (type) {
    case SELECT_FOLDER:
      dialog = CreateSelectFolderDialog(title_string, default_path,
                                        owning_window);
      break;
    case SELECT_OPEN_FILE:
      dialog = CreateFileOpenDialog(title_string, default_path, owning_window);
      break;
    case SELECT_OPEN_MULTI_FILE:
      dialog = CreateMultiFileOpenDialog(title_string, default_path,
                                         owning_window);
      break;
    case SELECT_SAVEAS_FILE:
      dialog = CreateSaveAsDialog(title_string, default_path, owning_window);
      break;
    default:
      NOTREACHED();
      return;
  }
  g_signal_connect(dialog, "delete-event",
                   G_CALLBACK(gtk_widget_hide_on_delete), NULL);
  dialogs_.insert(dialog);

  preview_ = gtk_image_new();
  g_signal_connect(dialog, "destroy",
                   G_CALLBACK(OnFileChooserDestroyThunk), this);
  g_signal_connect(dialog, "update-preview",
                   G_CALLBACK(OnUpdatePreviewThunk), this);
  gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(dialog), preview_);

  params_map_[dialog] = params;

  // Set window-to-parent modality by adding the dialog to the same window
  // group as the parent.
  gtk_window_group_add_window(gtk_window_get_group(owning_window),
                              GTK_WINDOW(dialog));
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

  gtk_widget_show_all(dialog);
}

void SelectFileDialogImplGTK::AddFilters(GtkFileChooser* chooser) {
  for (size_t i = 0; i < file_types_.extensions.size(); ++i) {
    GtkFileFilter* filter = NULL;
    std::set<std::string> fallback_labels;

    for (size_t j = 0; j < file_types_.extensions[i].size(); ++j) {
      const std::string& current_extension = file_types_.extensions[i][j];
      if (!current_extension.empty()) {
        if (!filter)
          filter = gtk_file_filter_new();
        scoped_ptr<std::string> file_extension(
            new std::string("." + current_extension));
        fallback_labels.insert(std::string("*").append(*file_extension));
        gtk_file_filter_add_custom(
            filter,
            GTK_FILE_FILTER_FILENAME,
            reinterpret_cast<GtkFileFilterFunc>(FileFilterCaseInsensitive),
            file_extension.release(),
            reinterpret_cast<GDestroyNotify>(OnFileFilterDataDestroyed));
      }
    }
    // We didn't find any non-empty extensions to filter on.
    if (!filter)
      continue;

    // The description vector may be blank, in which case we are supposed to
    // use some sort of default description based on the filter.
    if (i < file_types_.extension_description_overrides.size()) {
      gtk_file_filter_set_name(filter, UTF16ToUTF8(
          file_types_.extension_description_overrides[i]).c_str());
    } else {
      // There is no system default filter description so we use
      // the extensions themselves if the description is blank.
      std::vector<std::string> fallback_labels_vector(fallback_labels.begin(),
                                                      fallback_labels.end());
      std::string fallback_label = JoinString(fallback_labels_vector, ',');
      gtk_file_filter_set_name(filter, fallback_label.c_str());
    }

    gtk_file_chooser_add_filter(chooser, filter);
    if (i == file_type_index_ - 1)
      gtk_file_chooser_set_filter(chooser, filter);
  }

  // Add the *.* filter, but only if we have added other filters (otherwise it
  // is implied).
  if (file_types_.include_all_files && !file_types_.extensions.empty()) {
    GtkFileFilter* filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*");
    gtk_file_filter_set_name(filter,
        l10n_util::GetStringUTF8(IDS_SAVEAS_ALL_FILES).c_str());
    gtk_file_chooser_add_filter(chooser, filter);
  }
}

void SelectFileDialogImplGTK::FileSelected(GtkWidget* dialog,
                                           const base::FilePath& path) {
  if (type_ == SELECT_SAVEAS_FILE)
    *last_saved_path_ = path.DirName();
  else if (type_ == SELECT_OPEN_FILE || type_ == SELECT_FOLDER)
    *last_opened_path_ = path.DirName();
  else
    NOTREACHED();

  if (listener_) {
    GtkFileFilter* selected_filter =
        gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(dialog));
    GSList* filters = gtk_file_chooser_list_filters(GTK_FILE_CHOOSER(dialog));
    int idx = g_slist_index(filters, selected_filter);
    g_slist_free(filters);
    listener_->FileSelected(path, idx + 1, PopParamsForDialog(dialog));
  }
  gtk_widget_destroy(dialog);
}

void SelectFileDialogImplGTK::MultiFilesSelected(GtkWidget* dialog,
    const std::vector<base::FilePath>& files) {
    *last_opened_path_ = files[0].DirName();

  if (listener_)
    listener_->MultiFilesSelected(files, PopParamsForDialog(dialog));
  gtk_widget_destroy(dialog);
}

void SelectFileDialogImplGTK::FileNotSelected(GtkWidget* dialog) {
  void* params = PopParamsForDialog(dialog);
  if (listener_)
    listener_->FileSelectionCanceled(params);
  gtk_widget_destroy(dialog);
}

GtkWidget* SelectFileDialogImplGTK::CreateFileOpenHelper(
    const std::string& title,
    const base::FilePath& default_path,
    gfx::NativeWindow parent) {
  GtkWidget* dialog =
      gtk_file_chooser_dialog_new(title.c_str(), parent,
                                  GTK_FILE_CHOOSER_ACTION_OPEN,
                                  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                  NULL);
  AddFilters(GTK_FILE_CHOOSER(dialog));

  if (!default_path.empty()) {
    if (CallDirectoryExistsOnUIThread(default_path)) {
      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                          default_path.value().c_str());
    } else {
      // If the file doesn't exist, this will just switch to the correct
      // directory. That's good enough.
      gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),
                                    default_path.value().c_str());
    }
  } else if (!last_opened_path_->empty()) {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        last_opened_path_->value().c_str());
  }
  return dialog;
}

GtkWidget* SelectFileDialogImplGTK::CreateSelectFolderDialog(
    const std::string& title,
    const base::FilePath& default_path,
    gfx::NativeWindow parent) {
  std::string title_string = !title.empty() ? title :
        l10n_util::GetStringUTF8(IDS_SELECT_FOLDER_DIALOG_TITLE);

  GtkWidget* dialog =
      gtk_file_chooser_dialog_new(title_string.c_str(), parent,
                                  GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                  NULL);

  if (!default_path.empty()) {
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),
                                  default_path.value().c_str());
  } else if (!last_opened_path_->empty()) {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        last_opened_path_->value().c_str());
  }
  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), FALSE);
  g_signal_connect(dialog, "response",
                   G_CALLBACK(OnSelectSingleFolderDialogResponseThunk), this);
  return dialog;
}

GtkWidget* SelectFileDialogImplGTK::CreateFileOpenDialog(
    const std::string& title,
    const base::FilePath& default_path,
    gfx::NativeWindow parent) {
  std::string title_string = !title.empty() ? title :
        l10n_util::GetStringUTF8(IDS_OPEN_FILE_DIALOG_TITLE);

  GtkWidget* dialog = CreateFileOpenHelper(title_string, default_path, parent);
  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), FALSE);
  g_signal_connect(dialog, "response",
                   G_CALLBACK(OnSelectSingleFileDialogResponseThunk), this);
  return dialog;
}

GtkWidget* SelectFileDialogImplGTK::CreateMultiFileOpenDialog(
    const std::string& title,
    const base::FilePath& default_path,
    gfx::NativeWindow parent) {
  std::string title_string = !title.empty() ? title :
        l10n_util::GetStringUTF8(IDS_OPEN_FILES_DIALOG_TITLE);

  GtkWidget* dialog = CreateFileOpenHelper(title_string, default_path, parent);
  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), TRUE);
  g_signal_connect(dialog, "response",
                   G_CALLBACK(OnSelectMultiFileDialogResponseThunk), this);
  return dialog;
}

GtkWidget* SelectFileDialogImplGTK::CreateSaveAsDialog(const std::string& title,
    const base::FilePath& default_path, gfx::NativeWindow parent) {
  std::string title_string = !title.empty() ? title :
        l10n_util::GetStringUTF8(IDS_SAVE_AS_DIALOG_TITLE);

  GtkWidget* dialog =
      gtk_file_chooser_dialog_new(title_string.c_str(), parent,
                                  GTK_FILE_CHOOSER_ACTION_SAVE,
                                  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                  NULL);

  AddFilters(GTK_FILE_CHOOSER(dialog));
  if (!default_path.empty()) {
    // Since the file may not already exist, we use
    // set_current_folder() followed by set_current_name(), as per the
    // recommendation of the GTK docs.
    if (CallDirectoryExistsOnUIThread(default_path)) {
      gtk_file_chooser_set_current_folder(
          GTK_FILE_CHOOSER(dialog), default_path.value().c_str());
      gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "");
    } else {
      gtk_file_chooser_set_current_folder(
          GTK_FILE_CHOOSER(dialog), default_path.DirName().value().c_str());
      gtk_file_chooser_set_current_name(
          GTK_FILE_CHOOSER(dialog), default_path.BaseName().value().c_str());
    }
  } else if (!last_saved_path_->empty()) {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),
                                        last_saved_path_->value().c_str());
  }
  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), FALSE);
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),
                                                 TRUE);
  g_signal_connect(dialog, "response",
                   G_CALLBACK(OnSelectSingleFileDialogResponseThunk), this);
  return dialog;
}

void* SelectFileDialogImplGTK::PopParamsForDialog(GtkWidget* dialog) {
  std::map<GtkWidget*, void*>::iterator iter = params_map_.find(dialog);
  DCHECK(iter != params_map_.end());
  void* params = iter->second;
  params_map_.erase(iter);
  return params;
}

void SelectFileDialogImplGTK::FileDialogDestroyed(GtkWidget* dialog) {
  dialogs_.erase(dialog);

  // Parent may be NULL in a few cases: 1) on shutdown when
  // AllBrowsersClosed() trigger this handler after all the browser
  // windows got destroyed, or 2) when the parent tab has been opened by
  // 'Open Link in New Tab' context menu on a downloadable item and
  // the tab has no content (see the comment in SelectFile as well).
  GtkWindow* parent = gtk_window_get_transient_for(GTK_WINDOW(dialog));
  if (!parent)
    return;
  std::set<GtkWindow*>::iterator iter = parents_.find(parent);
  if (iter != parents_.end())
    parents_.erase(iter);
  else
    NOTREACHED();
}

bool SelectFileDialogImplGTK::IsCancelResponse(gint response_id) {
  bool is_cancel = response_id == GTK_RESPONSE_CANCEL ||
                   response_id == GTK_RESPONSE_DELETE_EVENT;
  if (is_cancel)
    return true;

  DCHECK(response_id == GTK_RESPONSE_ACCEPT);
  return false;
}

void SelectFileDialogImplGTK::SelectSingleFileHelper(GtkWidget* dialog,
    gint response_id,
    bool allow_folder) {
  if (IsCancelResponse(response_id)) {
    FileNotSelected(dialog);
    return;
  }

  gchar* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  if (!filename) {
    FileNotSelected(dialog);
    return;
  }

  base::FilePath path(filename);
  g_free(filename);

  if (allow_folder) {
    FileSelected(dialog, path);
    return;
  }

  if (CallDirectoryExistsOnUIThread(path))
    FileNotSelected(dialog);
  else
    FileSelected(dialog, path);
}

void SelectFileDialogImplGTK::OnSelectSingleFileDialogResponse(
    GtkWidget* dialog, int response_id) {
  SelectSingleFileHelper(dialog, response_id, false);
}

void SelectFileDialogImplGTK::OnSelectSingleFolderDialogResponse(
    GtkWidget* dialog, int response_id) {
  SelectSingleFileHelper(dialog, response_id, true);
}

void SelectFileDialogImplGTK::OnSelectMultiFileDialogResponse(GtkWidget* dialog,
                                                              int response_id) {
  if (IsCancelResponse(response_id)) {
    FileNotSelected(dialog);
    return;
  }

  GSList* filenames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));
  if (!filenames) {
    FileNotSelected(dialog);
    return;
  }

  std::vector<base::FilePath> filenames_fp;
  for (GSList* iter = filenames; iter != NULL; iter = g_slist_next(iter)) {
    base::FilePath path(static_cast<char*>(iter->data));
    g_free(iter->data);
    if (CallDirectoryExistsOnUIThread(path))
      continue;
    filenames_fp.push_back(path);
  }
  g_slist_free(filenames);

  if (filenames_fp.empty()) {
    FileNotSelected(dialog);
    return;
  }
  MultiFilesSelected(dialog, filenames_fp);
}

void SelectFileDialogImplGTK::OnFileChooserDestroy(GtkWidget* dialog) {
  FileDialogDestroyed(dialog);
}

void SelectFileDialogImplGTK::OnUpdatePreview(GtkWidget* chooser) {
  gchar* filename = gtk_file_chooser_get_preview_filename(
      GTK_FILE_CHOOSER(chooser));
  if (!filename)
    return;
  // This will preserve the image's aspect ratio.
  GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_size(filename, kPreviewWidth,
                                                       kPreviewHeight, NULL);
  g_free(filename);
  if (pixbuf) {
    gtk_image_set_from_pixbuf(GTK_IMAGE(preview_), pixbuf);
    g_object_unref(pixbuf);
  }
  gtk_file_chooser_set_preview_widget_active(GTK_FILE_CHOOSER(chooser),
                                             pixbuf ? TRUE : FALSE);
}

}  // namespace

namespace ui {

SelectFileDialogImpl* SelectFileDialogImpl::NewSelectFileDialogImplGTK(
    Listener* listener, ui::SelectFilePolicy* policy) {
  return new SelectFileDialogImplGTK(listener, policy);
}

}  // namespace ui
