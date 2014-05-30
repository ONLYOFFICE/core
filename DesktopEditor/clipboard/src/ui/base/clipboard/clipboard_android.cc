// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/clipboard/clipboard.h"

#include "base/android/jni_string.h"
#include "base/lazy_instance.h"
#include "base/stl_util.h"
#include "base/synchronization/lock.h"
#include "base/utf_string_conversions.h"
#include "jni/Clipboard_jni.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/clipboard/clipboard_android_initialization.h"
#include "ui/gfx/size.h"

// TODO:(andrewhayden) Support additional formats in Android: Bitmap, URI, HTML,
// HTML+text now that Android's clipboard system supports them, then nuke the
// legacy implementation note below.

// Legacy implementation note:
// The Android clipboard system used to only support text format. So we used the
// Android system when some text was added or retrieved from the system. For
// anything else, we STILL store the value in some process wide static
// variable protected by a lock. So the (non-text) clipboard will only work
// within the same process.

using base::android::AttachCurrentThread;
using base::android::ClearException;
using base::android::ConvertJavaStringToUTF8;
using base::android::ScopedJavaGlobalRef;
using base::android::ScopedJavaLocalRef;

namespace ui {

namespace {
// Various formats we support.
const char kPlainTextFormat[] = "text";
const char kHTMLFormat[] = "html";
const char kRTFFormat[] = "rtf";
const char kBitmapFormat[] = "bitmap";
const char kWebKitSmartPasteFormat[] = "webkit_smart";
const char kBookmarkFormat[] = "bookmark";
const char kMimeTypePepperCustomData[] = "chromium/x-pepper-custom-data";
const char kMimeTypeWebCustomData[] = "chromium/x-web-custom-data";
const char kSourceTagFormat[] = "source_tag";

class ClipboardMap {
 public:
  ClipboardMap();
  std::string Get(const std::string& format);
  bool HasFormat(const std::string& format);
  void Set(const std::string& format, const std::string& data);
  void Clear();

 private:
  void SyncWithAndroidClipboard();
  std::map<std::string, std::string> map_;
  base::Lock lock_;

  // Java class and methods for the Android ClipboardManager.
  ScopedJavaGlobalRef<jobject> clipboard_manager_;
};
base::LazyInstance<ClipboardMap>::Leaky g_map = LAZY_INSTANCE_INITIALIZER;

ClipboardMap::ClipboardMap() {
  JNIEnv* env = AttachCurrentThread();
  DCHECK(env);

  // Get the context.
  jobject context = base::android::GetApplicationContext();
  DCHECK(context);

  ScopedJavaLocalRef<jobject> local_ref =
      Java_Clipboard_create(env, context);
  DCHECK(local_ref.obj());
  clipboard_manager_.Reset(env, local_ref.Release());
}

std::string ClipboardMap::Get(const std::string& format) {
  base::AutoLock lock(lock_);
  SyncWithAndroidClipboard();
  std::map<std::string, std::string>::const_iterator it = map_.find(format);
  return it == map_.end() ? std::string() : it->second;
}

bool ClipboardMap::HasFormat(const std::string& format) {
  base::AutoLock lock(lock_);
  SyncWithAndroidClipboard();
  return ContainsKey(map_, format);
}

void ClipboardMap::Set(const std::string& format, const std::string& data) {
  JNIEnv* env = AttachCurrentThread();
  base::AutoLock lock(lock_);
  SyncWithAndroidClipboard();

  map_[format] = data;
  if (format == kPlainTextFormat) {
    ScopedJavaLocalRef<jstring> str(
        env, env->NewStringUTF(data.c_str()));
    DCHECK(str.obj() && !ClearException(env));
    Java_Clipboard_setText(env, clipboard_manager_.obj(), str.obj());
  }
}

void ClipboardMap::Clear() {
  JNIEnv* env = AttachCurrentThread();
  base::AutoLock lock(lock_);
  map_.clear();
  Java_Clipboard_setText(env, clipboard_manager_.obj(), NULL);
}

// If the internal map contains a plain-text entry and it does not match that
// in the Android clipboard, clear the map and insert the Android text into it.
void ClipboardMap::SyncWithAndroidClipboard() {
  lock_.AssertAcquired();
  JNIEnv* env = AttachCurrentThread();

  std::map<std::string, std::string>::const_iterator it =
    map_.find(kPlainTextFormat);

  if (!Java_Clipboard_hasPlainText(env, clipboard_manager_.obj())) {
    if (it != map_.end())
      // We have plain text on this side, but Android doesn't. Nuke ours.
      map_.clear();
    return;
  }

  ScopedJavaLocalRef<jstring> java_string =
      Java_Clipboard_getCoercedText(env, clipboard_manager_.obj());

  if (!java_string.obj()) {
    // Tolerate a null value from the Java side, even though that should not
    // happen since hasPlainText has already returned true.
    // Should only happen if someone is using the clipboard on multiple
    // threads and clears it out after hasPlainText but before we get here...
    if (it != map_.end())
      // We have plain text on this side, but Android doesn't. Nuke ours.
      map_.clear();
    return;
  }

  // If Android text differs from ours (or we have none), then copy Android's.
  std::string android_string = ConvertJavaStringToUTF8(java_string);
  if (it == map_.end() || it->second != android_string) {
    map_.clear();
    map_[kPlainTextFormat] = android_string;
  }
}

}  // namespace

Clipboard::FormatType::FormatType() {
}

Clipboard::FormatType::FormatType(const std::string& native_format)
    : data_(native_format) {
}

Clipboard::FormatType::~FormatType() {
}

std::string Clipboard::FormatType::Serialize() const {
  return data_;
}

// static
Clipboard::FormatType Clipboard::FormatType::Deserialize(
    const std::string& serialization) {
  return FormatType(serialization);
}

bool Clipboard::FormatType::Equals(const FormatType& other) const {
  return data_ == other.data_;
}

Clipboard::Clipboard() {
  DCHECK(CalledOnValidThread());
}

Clipboard::~Clipboard() {
  DCHECK(CalledOnValidThread());
}

// Main entry point used to write several values in the clipboard.
void Clipboard::WriteObjectsImpl(Buffer buffer,
                                 const ObjectMap& objects,
                                 SourceTag tag) {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  g_map.Get().Clear();
  for (ObjectMap::const_iterator iter = objects.begin();
       iter != objects.end(); ++iter) {
    DispatchObject(static_cast<ObjectType>(iter->first), iter->second);
  }
  WriteSourceTag(tag);
}

uint64 Clipboard::GetSequenceNumber(Clipboard::Buffer /* buffer */) {
  DCHECK(CalledOnValidThread());
  // TODO: implement this. For now this interface will advertise
  // that the clipboard never changes. That's fine as long as we
  // don't rely on this signal.
  return 0;
}

bool Clipboard::IsFormatAvailable(const Clipboard::FormatType& format,
                                  Clipboard::Buffer buffer) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  return g_map.Get().HasFormat(format.data());
}

void Clipboard::Clear(Buffer buffer) {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  g_map.Get().Clear();
}

void Clipboard::ReadAvailableTypes(Buffer buffer, std::vector<string16>* types,
                                   bool* contains_filenames) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);

  if (!types || !contains_filenames) {
    NOTREACHED();
    return;
  }

  NOTIMPLEMENTED();

  types->clear();
  *contains_filenames = false;
}

void Clipboard::ReadText(Clipboard::Buffer buffer, string16* result) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  std::string utf8;
  ReadAsciiText(buffer, &utf8);
  *result = UTF8ToUTF16(utf8);
}

void Clipboard::ReadAsciiText(Clipboard::Buffer buffer,
                              std::string* result) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  *result = g_map.Get().Get(kPlainTextFormat);
}

// Note: |src_url| isn't really used. It is only implemented in Windows
void Clipboard::ReadHTML(Clipboard::Buffer buffer,
                         string16* markup,
                         std::string* src_url,
                         uint32* fragment_start,
                         uint32* fragment_end) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  if (src_url)
    src_url->clear();

  std::string input = g_map.Get().Get(kHTMLFormat);
  *markup = UTF8ToUTF16(input);

  *fragment_start = 0;
  *fragment_end = static_cast<uint32>(markup->length());
}

void Clipboard::ReadRTF(Buffer buffer, std::string* result) const {
  DCHECK(CalledOnValidThread());
  NOTIMPLEMENTED();
}

SkBitmap Clipboard::ReadImage(Buffer buffer) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  std::string input = g_map.Get().Get(kBitmapFormat);

  SkBitmap bmp;
  if (!input.empty()) {
    DCHECK_LE(sizeof(gfx::Size), input.size());
    const gfx::Size* size = reinterpret_cast<const gfx::Size*>(input.data());

    bmp.setConfig(
        SkBitmap::kARGB_8888_Config, size->width(), size->height(), 0);
    bmp.allocPixels();

    int bm_size = size->width() * size->height() * 4;
    DCHECK_EQ(sizeof(gfx::Size) + bm_size, input.size());

    memcpy(bmp.getPixels(), input.data() + sizeof(gfx::Size), bm_size);
  }
  return bmp;
}

void Clipboard::ReadCustomData(Buffer buffer,
                               const string16& type,
                               string16* result) const {
  DCHECK(CalledOnValidThread());
  NOTIMPLEMENTED();
}

void Clipboard::ReadBookmark(string16* title, std::string* url) const {
  DCHECK(CalledOnValidThread());
  NOTIMPLEMENTED();
}

void Clipboard::ReadData(const Clipboard::FormatType& format,
                         std::string* result) const {
  DCHECK(CalledOnValidThread());
  *result = g_map.Get().Get(format.data());
}

SourceTag Clipboard::ReadSourceTag(Buffer buffer) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  std::string result;
  ReadData(GetSourceTagFormatType(), &result);
  return Binary2SourceTag(result);
}

// static
Clipboard::FormatType Clipboard::GetFormatType(
    const std::string& format_string) {
  return FormatType::Deserialize(format_string);
}

// static
const Clipboard::FormatType& Clipboard::GetPlainTextFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kPlainTextFormat));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetPlainTextWFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kPlainTextFormat));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetWebKitSmartPasteFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kWebKitSmartPasteFormat));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetHtmlFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kHTMLFormat));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetRtfFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kRTFFormat));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetBitmapFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kBitmapFormat));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetWebCustomDataFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypeWebCustomData));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetPepperCustomDataFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypePepperCustomData));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetSourceTagFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kSourceTagFormat));
  return type;
}

void Clipboard::WriteText(const char* text_data, size_t text_len) {
  g_map.Get().Set(kPlainTextFormat, std::string(text_data, text_len));
}

void Clipboard::WriteHTML(const char* markup_data,
                          size_t markup_len,
                          const char* url_data,
                          size_t url_len) {
  g_map.Get().Set(kHTMLFormat, std::string(markup_data, markup_len));
}

void Clipboard::WriteRTF(const char* rtf_data, size_t data_len) {
  NOTIMPLEMENTED();
}

// Note: according to other platforms implementations, this really writes the
// URL spec.
void Clipboard::WriteBookmark(const char* title_data, size_t title_len,
                              const char* url_data, size_t url_len) {
  g_map.Get().Set(kBookmarkFormat, std::string(url_data, url_len));
}

// Write an extra flavor that signifies WebKit was the last to modify the
// pasteboard. This flavor has no data.
void Clipboard::WriteWebSmartPaste() {
  g_map.Get().Set(kWebKitSmartPasteFormat, std::string());
}

// All platforms use gfx::Size for size data but it is passed as a const char*
// Further, pixel_data is expected to be 32 bits per pixel
// Note: we implement this to pass all unit tests but it is currently unclear
// how some code would consume this.
void Clipboard::WriteBitmap(const char* pixel_data, const char* size_data) {
  const gfx::Size* size = reinterpret_cast<const gfx::Size*>(size_data);
  int bm_size = size->width() * size->height() * 4;

  std::string packed(size_data, sizeof(gfx::Size));
  packed += std::string(pixel_data, bm_size);
  g_map.Get().Set(kBitmapFormat, packed);
}

void Clipboard::WriteData(const Clipboard::FormatType& format,
                          const char* data_data, size_t data_len) {
  g_map.Get().Set(format.data(), std::string(data_data, data_len));
}

void Clipboard::WriteSourceTag(SourceTag tag) {
  if (tag != SourceTag()) {
    ObjectMapParam binary = SourceTag2Binary(tag);
    WriteData(GetSourceTagFormatType(), &binary[0], binary.size());
  }
}

// See clipboard_android_initialization.h for more information.
bool RegisterClipboardAndroid(JNIEnv* env) {
  return RegisterNativesImpl(env);
}

} // namespace ui
