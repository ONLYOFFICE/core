// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_CLIPBOARD_CLIPBOARD_H_
#define UI_BASE_CLIPBOARD_CLIPBOARD_H_

#include <map>
#include <string>
#include <vector>

//#include "base/callback.h"
#include "base/compiler_specific.h"
//#include "base/gtest_prod_util.h"
#include "base/memory/shared_memory.h"
#include "base/process.h"
#include "base/string16.h"
#include "base/threading/platform_thread.h"
#include "base/threading/thread_checker.h"
#include "ui/base/clipboard/clipboard_sourcetag.h"
#include "ui/base/ui_export.h"



#if defined(TOOLKIT_GTK)
#include <gdk/gdk.h>
#endif

#if defined(OS_ANDROID)
#include <jni.h>

#include "base/android/jni_android.h"
#include "base/android/scoped_java_ref.h"
#endif

#if defined(USE_AURA) && defined(USE_X11)
#include "base/memory/scoped_ptr.h"
#endif

namespace base {
class FilePath;
}

namespace gfx {
class Size;
}

class SkBitmap;

#if defined(TOOLKIT_GTK)
typedef struct _GtkClipboard GtkClipboard;
#endif

#ifdef __OBJC__
@class NSPasteboard;
@class NSString;
#else
class NSPasteboard;
class NSString;
#endif

namespace ui {
class ClipboardTest;

class UI_EXPORT Clipboard 
	/*: NON_EXPORTED_BASE(public base::ThreadChecker) */
	{
 public:
  // MIME type constants.
  static const char kMimeTypeText[];
  static const char kMimeTypeURIList[];
  static const char kMimeTypeDownloadURL[];
  static const char kMimeTypeHTML[];
  static const char kMimeTypeRTF[];
  static const char kMimeTypePNG[];

  // Platform neutral holder for native data representation of a clipboard type.
  struct UI_EXPORT FormatType {
    FormatType();
    ~FormatType();

    std::string Serialize() const;
    static FormatType Deserialize(const std::string& serialization);

    // FormatType can be used as the key in a map on some platforms.
#if defined(OS_WIN) || defined(USE_AURA)
    bool operator<(const FormatType& other) const {
      return data_ < other.data_;
    }
#endif

   private:
    friend class Clipboard;

    bool Equals(const FormatType& other) const;

#if defined(OS_WIN)
    explicit FormatType(UINT native_format);
    UINT ToUINT() const { return data_; }
    UINT data_;
#elif defined(OS_MACOSX)
   public:
    FormatType(const FormatType& other);
    FormatType& operator=(const FormatType& other);
   private:
    explicit FormatType(NSString* native_format);
    NSString* ToNSString() const { return data_; }
    NSString* data_;
#elif defined(USE_AURA)
    explicit FormatType(const std::string& native_format);
   public:
    const std::string& ToString() const { return data_; }
   private:
    std::string data_;
#elif defined(TOOLKIT_GTK)
    explicit FormatType(const std::string& native_format);
    explicit FormatType(const GdkAtom& native_format);
    const GdkAtom& ToGdkAtom() const { return data_; }
    GdkAtom data_;
#elif defined(OS_ANDROID)
    explicit FormatType(const std::string& native_format);
    const std::string& data() const { return data_; }
    int compare(const std::string& str) const { return data_.compare(str); }
    std::string data_;
#else
#error No FormatType definition.
#endif
  };

  // ObjectType designates the type of data to be stored in the clipboard. This
  // designation is shared across all OSes. The system-specific designation
  // is defined by FormatType. A single ObjectType might be represented by
  // several system-specific FormatTypes. For example, on Linux the CBF_TEXT
  // ObjectType maps to "text/plain", "STRING", and several other formats. On
  // windows it maps to CF_UNICODETEXT.
  enum ObjectType {
    CBF_TEXT,
    CBF_HTML,
    CBF_RTF,
    CBF_BOOKMARK,
    CBF_FILES,
    CBF_WEBKIT,
    CBF_BITMAP,
    CBF_SMBITMAP,  // Bitmap from shared memory.
    CBF_DATA,  // Arbitrary block of bytes.
  };

  // ObjectMap is a map from ObjectType to associated data.
  // The data is organized differently for each ObjectType. The following
  // table summarizes what kind of data is stored for each key.
  // * indicates an optional argument.
  //
  // Key           Arguments    Type
  // -------------------------------------
  // CBF_TEXT      text         char array
  // CBF_HTML      html         char array
  //               url*         char array
  // CBF_RTF       data         byte array
  // CBF_BOOKMARK  html         char array
  //               url          char array
  // CBF_LINK      html         char array
  //               url          char array
  // CBF_FILES     files        char array representing multiple files.
  //                            Filenames are separated by null characters and
  //                            the final filename is double null terminated.
  //                            The filenames are encoded in platform-specific
  //                            encoding.
  // CBF_WEBKIT    none         empty vector
  // CBF_BITMAP    pixels       byte array
  //               size         gfx::Size struct
  // CBF_SMBITMAP  shared_mem   A pointer to an unmapped base::SharedMemory
  //                            object containing the bitmap data.
  //               size         gfx::Size struct
  // CBF_DATA      format       char array
  //               data         byte array
  typedef std::vector<char> ObjectMapParam;
  typedef std::vector<ObjectMapParam> ObjectMapParams;
  typedef std::map<int /* ObjectType */, ObjectMapParams> ObjectMap;

  static ObjectMapParam SourceTag2Binary(SourceTag tag);
  static SourceTag Binary2SourceTag(const std::string& serialization);

  // Buffer designates which clipboard the action should be applied to.
  // Only platforms that use the X Window System support the selection
  // buffer.
  enum Buffer {
    BUFFER_STANDARD,
    BUFFER_SELECTION,
  };

  /*
  // The callback is called after Clipboard::WriteObjects().
  // Don't use it for notification about changed OS clipboard.
  void set_write_objects_callback_for_testing(
      const base::Callback<void(Buffer)>& cb) {
    write_objects_callback_ = cb;
  }
  */

  static bool IsValidBuffer(int32 buffer) {
    switch (buffer) {
      case BUFFER_STANDARD:
        return true;
#if defined(USE_X11) && !defined(OS_CHROMEOS)
      case BUFFER_SELECTION:
        return true;
#endif
    }
    return false;
  }

  static Buffer FromInt(int32 buffer) {
    return static_cast<Buffer>(buffer);
  }

  /*
  // Sets the list of threads that are allowed to access the clipboard.
  static void SetAllowedThreads(
      const std::vector<base::PlatformThreadId>& allowed_threads);
	  */

  // Returns the clipboard object for the current thread.
  //
  // Most implementations will have at most one clipboard which will live on
  // the main UI thread, but Windows has tricky semantics where there have to
  // be two clipboards: one that lives on the UI thread and one that lives on
  // the IO thread.
  static Clipboard* GetForCurrentThread();

  // Destroys the clipboard for the current thread. Usually, this will clean up
  // all clipboards, except on Windows. (Previous code leaks the IO thread
  // clipboard, so it shouldn't be a problem.)
  static void DestroyClipboardForCurrentThread(Clipboard*);

  // Write a bunch of objects to the system clipboard. Copies are made of the
  // contents of |objects|. On Windows they are copied to the system clipboard.
  // On linux they are copied into a structure owned by the Clipboard object and
  // kept until the system clipboard is set again.
  // SourceTag is optional value to be stored in the clipboard, NULL won't be
  // stored.
  void WriteObjects(Buffer buffer, const ObjectMap& objects, SourceTag tag);

  // Returns a sequence number which uniquely identifies clipboard state.
  // This can be used to version the data on the clipboard and determine
  // whether it has changed.
  uint64 GetSequenceNumber(Buffer buffer);

  // Tests whether the clipboard contains a certain format
  bool IsFormatAvailable(const FormatType& format, Buffer buffer) const;

  // Clear the clipboard data.
  void Clear(Buffer buffer);

  void ReadAvailableTypes(Buffer buffer, std::vector<string16>* types,
                          bool* contains_filenames) const;

  // Reads UNICODE text from the clipboard, if available.
  void ReadText(Buffer buffer, string16* result) const;

  // Reads ASCII text from the clipboard, if available.
  void ReadAsciiText(Buffer buffer, std::string* result) const;

  // Reads HTML from the clipboard, if available. If the HTML fragment requires
  // context to parse, |fragment_start| and |fragment_end| are indexes into
  // markup indicating the beginning and end of the actual fragment. Otherwise,
  // they will contain 0 and markup->size().
  void ReadHTML(Buffer buffer, string16* markup, std::string* src_url,
                uint32* fragment_start, uint32* fragment_end) const;

  void ReadDoct(Buffer buffer, base::string16* content) const;

  // Reads RTF from the clipboard, if available. Stores the result as a byte
  // vector.
  void ReadRTF(Buffer buffer, std::string* result) const;

  // Reads an image from the clipboard, if available.
  /*SkBitmap*/
  bool ReadImage(Buffer buffer) const;

  void ReadCustomData(Buffer buffer,
                      const string16& type,
                      string16* result) const;

  // Reads a bookmark from the clipboard, if available.
  void ReadBookmark(string16* title, std::string* url) const;

  // Reads raw data from the clipboard with the given format type. Stores result
  // as a byte vector.
  void ReadData(const FormatType& format, std::string* result) const;

  // Reads Source tag from the clipboard, if available.
  SourceTag ReadSourceTag(Buffer buffer) const;

  // Gets the FormatType corresponding to an arbitrary format string,
  // registering it with the system if needed. Due to Windows/Linux
  // limitiations, |format_string| must never be controlled by the user.
  static FormatType GetFormatType(const std::string& format_string);

  // Get format identifiers for various types.
  static const FormatType& GetUrlFormatType();
  static const FormatType& GetUrlWFormatType();
  static const FormatType& GetMozUrlFormatType();
  static const FormatType& GetPlainTextFormatType();
  static const FormatType& GetPlainTextWFormatType();
  static const FormatType& GetFilenameFormatType();
  static const FormatType& GetFilenameWFormatType();
  static const FormatType& GetWebKitSmartPasteFormatType();
  // Win: MS HTML Format, Other: Generic HTML format
  static const FormatType& GetHtmlFormatType();
  static const FormatType& GetRtfFormatType();
  static const FormatType& GetBitmapFormatType();
  // TODO(raymes): Unify web custom data and pepper custom data:
  // crbug.com/158399.
  static const FormatType& GetWebCustomDataFormatType();
  static const FormatType& GetPepperCustomDataFormatType();
  static const FormatType& GetSourceTagFormatType();

  // Embeds a pointer to a SharedMemory object pointed to by |bitmap_handle|
  // belonging to |process| into a shared bitmap [CBF_SMBITMAP] slot in
  // |objects|.  The pointer is deleted by DispatchObjects().
  //
  // On non-Windows platforms, |process| is ignored.
  static void ReplaceSharedMemHandle(ObjectMap* objects,
                                     base::SharedMemoryHandle bitmap_handle,
                                     base::ProcessHandle process);
#if defined(OS_WIN)
  // Firefox text/html
  static const FormatType& GetTextHtmlFormatType();
  static const FormatType& GetCFHDropFormatType();
  static const FormatType& GetFileDescriptorFormatType();
  static const FormatType& GetFileContentFormatZeroType();
#endif

#if defined(OS_MACOSX)
  // Allows code that directly manipulates the clipboard and doesn't use this
  // class to write a SourceTag.
  static void WriteSourceTag(NSPasteboard* pb, SourceTag tag);
#endif

 private:
  //FRIEND_TEST_ALL_PREFIXES(ClipboardTest, SharedBitmapTest);
  //FRIEND_TEST_ALL_PREFIXES(ClipboardTest, EmptyHTMLTest);
  //friend class ClipboardTest;

  Clipboard();
  ~Clipboard();

  void DispatchObject(ObjectType type, const ObjectMapParams& params);

  void WriteObjectsImpl(Buffer buffer, const ObjectMap& objects, SourceTag tag);

  void WriteText(const char* text_data, size_t text_len);

  void WriteHTML(const char* markup_data,
                 size_t markup_len,
                 const char* url_data,
                 size_t url_len);

  void WriteRTF(const char* rtf_data, size_t data_len);

  void WriteBookmark(const char* title_data,
                     size_t title_len,
                     const char* url_data,
                     size_t url_len);

  void WriteWebSmartPaste();

  void WriteBitmap(const char* pixel_data, const char* size_data);

  void WriteData(const FormatType& format,
                 const char* data_data,
                 size_t data_len);

  void WriteSourceTag(SourceTag tag);
#if defined(OS_WIN)
  void WriteBitmapFromHandle(HBITMAP source_hbitmap,
                             const gfx::Size& size);

  // Safely write to system clipboard. Free |handle| on failure.
  void WriteToClipboard(unsigned int format, HANDLE handle);

  static void ParseBookmarkClipboardFormat(const string16& bookmark,
                                           string16* title,
                                           std::string* url);

  // Free a handle depending on its type (as intuited from format)
  static void FreeData(unsigned int format, HANDLE data);

  // Return the window that should be the clipboard owner, creating it
  // if neccessary.  Marked const for lazily initialization by const methods.
  HWND GetClipboardWindow() const;

  // Mark this as mutable so const methods can still do lazy initialization.
  mutable HWND clipboard_owner_;

  // True if we can create a window.
  bool create_window_;
#elif defined(TOOLKIT_GTK)
  // The public API is via WriteObjects() which dispatches to multiple
  // Write*() calls, but on GTK we must write all the clipboard types
  // in a single GTK call.  To support this we store the current set
  // of data we intend to put on the clipboard on clipboard_data_ as
  // WriteObjects is running, and then at the end call SetGtkClipboard
  // which replaces whatever is on the system clipboard with the
  // contents of clipboard_data_.

 public:
  typedef std::map<std::string, std::pair<char*, size_t> > TargetMap;

 private:
  // Write changes to gtk clipboard.
  void SetGtkClipboard(Buffer buffer);
  // Insert a mapping into clipboard_data_.
  void InsertMapping(const char* key, char* data, size_t data_len);

  // Find the gtk clipboard for the passed buffer enum.
  GtkClipboard* LookupBackingClipboard(Buffer clipboard) const;
  // Reads raw data from the specified clipboard with the given format type.
  void ReadDataImpl(Buffer buffer,
                    const FormatType& format,
                    std::string* result) const;

  TargetMap* clipboard_data_;
  GtkClipboard* clipboard_;
  GtkClipboard* primary_selection_;
#elif defined(USE_AURA) && defined(USE_X11) && !defined(OS_CHROMEOS)
 private:
  // Reads raw data from the specified clipboard with the given format type.
  void ReadDataImpl(Buffer buffer,
                    const FormatType& format,
                    std::string* result) const;
  // We keep our implementation details private because otherwise we bring in
  // the X11 headers and break chrome compile.
  class AuraX11Details;
  scoped_ptr<AuraX11Details> aurax11_details_;
#endif
  //base::Callback<void(Buffer)> write_objects_callback_;

  DISALLOW_COPY_AND_ASSIGN(Clipboard);
};

}  // namespace ui

#endif  // UI_BASE_CLIPBOARD_CLIPBOARD_H_
