// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/clipboard/clipboard.h"

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/mac/mac_util.h"
#include "base/mac/scoped_cftyperef.h"
#include "base/memory/scoped_nsobject.h"
#include "base/stl_util.h"
#include "base/strings/sys_string_conversions.h"
#include "base/utf_string_conversions.h"
#import "third_party/mozilla/NSPasteboard+Utils.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/clipboard/custom_data_helper.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/scoped_ns_graphics_context_save_gstate_mac.h"
#include "ui/gfx/size.h"

namespace ui {

namespace {

// Source tag format type.
NSString* const kSourceTagPboardType = @"org.chromium.source-tag-data";

// Would be nice if this were in UTCoreTypes.h, but it isn't
NSString* const kUTTypeURLName = @"public.url-name";

// Tells us if WebKit was the last to write to the pasteboard. There's no
// actual data associated with this type.
NSString* const kWebSmartPastePboardType = @"NeXT smart paste pasteboard type";

// Pepper custom data format type.
NSString* const kPepperCustomDataPboardType =
    @"org.chromium.pepper-custom-data";

NSPasteboard* GetPasteboard() {
  // The pasteboard should not be nil in a UI session, but this handy DCHECK
  // can help track down problems if someone tries using clipboard code outside
  // of a UI session.
  NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
  DCHECK(pasteboard);
  return pasteboard;
}

void WriteDataImpl(NSPasteboard* pb,
                   NSString* format,
                   const char* data_data,
                   size_t data_len) {
  [pb addTypes:[NSArray arrayWithObject:format] owner:nil];
  [pb setData:[NSData dataWithBytes:data_data length:data_len]
      forType:format];
}

}  // namespace

Clipboard::FormatType::FormatType() : data_(nil) {
}

Clipboard::FormatType::FormatType(NSString* native_format)
    : data_([native_format retain]) {
}

Clipboard::FormatType::FormatType(const FormatType& other)
    : data_([other.data_ retain]) {
}

Clipboard::FormatType& Clipboard::FormatType::operator=(
    const FormatType& other) {
  if (this != &other) {
    [data_ release];
    data_ = [other.data_ retain];
  }
  return *this;
}

Clipboard::FormatType::~FormatType() {
  [data_ release];
}

std::string Clipboard::FormatType::Serialize() const {
  return base::SysNSStringToUTF8(data_);
}

// static
Clipboard::FormatType Clipboard::FormatType::Deserialize(
    const std::string& serialization) {
  return FormatType(base::SysUTF8ToNSString(serialization));
}

Clipboard::Clipboard() {
  DCHECK(CalledOnValidThread());
}

Clipboard::~Clipboard() {
  DCHECK(CalledOnValidThread());
}

void Clipboard::WriteObjectsImpl(Buffer buffer,
                                 const ObjectMap& objects,
                                 SourceTag tag) {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);

  NSPasteboard* pb = GetPasteboard();
  [pb declareTypes:[NSArray array] owner:nil];

  for (ObjectMap::const_iterator iter = objects.begin();
       iter != objects.end(); ++iter) {
    DispatchObject(static_cast<ObjectType>(iter->first), iter->second);
  }
  WriteSourceTag(tag);
}

void Clipboard::WriteText(const char* text_data, size_t text_len) {
  std::string text_str(text_data, text_len);
  NSString *text = base::SysUTF8ToNSString(text_str);
  NSPasteboard* pb = GetPasteboard();
  [pb addTypes:[NSArray arrayWithObject:NSStringPboardType] owner:nil];
  [pb setString:text forType:NSStringPboardType];
}

void Clipboard::WriteHTML(const char* markup_data,
                          size_t markup_len,
                          const char* url_data,
                          size_t url_len) {
  // We need to mark it as utf-8. (see crbug.com/11957)
  std::string html_fragment_str("<meta charset='utf-8'>");
  html_fragment_str.append(markup_data, markup_len);
  NSString *html_fragment = base::SysUTF8ToNSString(html_fragment_str);

  // TODO(avi): url_data?
  NSPasteboard* pb = GetPasteboard();
  [pb addTypes:[NSArray arrayWithObject:NSHTMLPboardType] owner:nil];
  [pb setString:html_fragment forType:NSHTMLPboardType];
}

void Clipboard::WriteRTF(const char* rtf_data, size_t data_len) {
  WriteData(GetRtfFormatType(), rtf_data, data_len);
}

void Clipboard::WriteBookmark(const char* title_data,
                              size_t title_len,
                              const char* url_data,
                              size_t url_len) {
  std::string title_str(title_data, title_len);
  NSString *title =  base::SysUTF8ToNSString(title_str);
  std::string url_str(url_data, url_len);
  NSString *url =  base::SysUTF8ToNSString(url_str);

  // TODO(playmobil): In the Windows version of this function, an HTML
  // representation of the bookmark is also added to the clipboard, to support
  // drag and drop of web shortcuts.  I don't think we need to do this on the
  // Mac, but we should double check later on.
  NSURL* nsurl = [NSURL URLWithString:url];

  NSPasteboard* pb = GetPasteboard();
  // passing UTIs into the pasteboard methods is valid >= 10.5
  [pb addTypes:[NSArray arrayWithObjects:NSURLPboardType,
                                         kUTTypeURLName,
                                         nil]
         owner:nil];
  [nsurl writeToPasteboard:pb];
  [pb setString:title forType:kUTTypeURLName];
}

void Clipboard::WriteBitmap(const char* pixel_data, const char* size_data) {
  const gfx::Size* size = reinterpret_cast<const gfx::Size*>(size_data);

  // Safe because the image goes away before the call returns.
  base::mac::ScopedCFTypeRef<CFDataRef> data(
      CFDataCreateWithBytesNoCopy(kCFAllocatorDefault,
                                  reinterpret_cast<const UInt8*>(pixel_data),
                                  size->width()*size->height()*4,
                                  kCFAllocatorNull));

  base::mac::ScopedCFTypeRef<CGDataProviderRef> data_provider(
      CGDataProviderCreateWithCFData(data));

  base::mac::ScopedCFTypeRef<CGImageRef> cgimage(
      CGImageCreate(size->width(),
                    size->height(),
                    8,
                    32,
                    size->width()*4,
                    base::mac::GetSRGBColorSpace(),  // TODO(avi): do better
                    kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host,
                    data_provider,
                    NULL,
                    false,
                    kCGRenderingIntentDefault));
  // Aggressively free storage since image buffers can potentially be very
  // large.
  data_provider.reset();
  data.reset();

  scoped_nsobject<NSBitmapImageRep> bitmap(
      [[NSBitmapImageRep alloc] initWithCGImage:cgimage]);
  cgimage.reset();

  scoped_nsobject<NSImage> image([[NSImage alloc] init]);
  [image addRepresentation:bitmap];

  // An API to ask the NSImage to write itself to the clipboard comes in 10.6 :(
  // For now, spit out the image as a TIFF.
  NSPasteboard* pb = GetPasteboard();
  [pb addTypes:[NSArray arrayWithObject:NSTIFFPboardType] owner:nil];
  NSData *tiff_data = [image TIFFRepresentation];
  LOG_IF(ERROR, tiff_data == NULL) << "Failed to allocate image for clipboard";
  if (tiff_data) {
    [pb setData:tiff_data forType:NSTIFFPboardType];
  }
}

void Clipboard::WriteData(const FormatType& format,
                          const char* data_data,
                          size_t data_len) {
  WriteDataImpl(GetPasteboard(), format.ToNSString(), data_data, data_len);
}

void Clipboard::WriteSourceTag(SourceTag tag) {
  if (tag != SourceTag())
    WriteSourceTag(GetPasteboard(), tag);
}

// static
void Clipboard::WriteSourceTag(NSPasteboard* pb, SourceTag tag) {
  if (tag != SourceTag()) {
    ObjectMapParam binary = SourceTag2Binary(tag);
    WriteDataImpl(pb,
                  GetSourceTagFormatType().ToNSString(),
                  &binary[0],
                  binary.size());
  }
}

// Write an extra flavor that signifies WebKit was the last to modify the
// pasteboard. This flavor has no data.
void Clipboard::WriteWebSmartPaste() {
  NSPasteboard* pb = GetPasteboard();
  NSString* format = GetWebKitSmartPasteFormatType().ToNSString();
  [pb addTypes:[NSArray arrayWithObject:format] owner:nil];
  [pb setData:nil forType:format];
}

uint64 Clipboard::GetSequenceNumber(Buffer buffer) {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);

  NSPasteboard* pb = GetPasteboard();
  return [pb changeCount];
}

bool Clipboard::IsFormatAvailable(const FormatType& format,
                                  Buffer buffer) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);

  NSPasteboard* pb = GetPasteboard();
  NSArray* types = [pb types];

  // Safari only places RTF on the pasteboard, never HTML. We can convert RTF
  // to HTML, so the presence of either indicates success when looking for HTML.
  if ([format.ToNSString() isEqualToString:NSHTMLPboardType]) {
    return [types containsObject:NSHTMLPboardType] ||
           [types containsObject:NSRTFPboardType];
  }
  return [types containsObject:format.ToNSString()];
}

void Clipboard::Clear(Buffer buffer) {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);

  NSPasteboard* pb = GetPasteboard();
  [pb declareTypes:[NSArray array] owner:nil];
}

void Clipboard::ReadAvailableTypes(Clipboard::Buffer buffer,
                                   std::vector<string16>* types,
                                   bool* contains_filenames) const {
  DCHECK(CalledOnValidThread());
  types->clear();
  if (IsFormatAvailable(Clipboard::GetPlainTextFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypeText));
  if (IsFormatAvailable(Clipboard::GetHtmlFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypeHTML));
  if (IsFormatAvailable(Clipboard::GetRtfFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypeRTF));
  if ([NSImage canInitWithPasteboard:GetPasteboard()])
    types->push_back(UTF8ToUTF16(kMimeTypePNG));
  *contains_filenames = false;

  NSPasteboard* pb = GetPasteboard();
  if ([[pb types] containsObject:kWebCustomDataPboardType]) {
    NSData* data = [pb dataForType:kWebCustomDataPboardType];
    if ([data length])
      ReadCustomDataTypes([data bytes], [data length], types);
  }
}

void Clipboard::ReadText(Clipboard::Buffer buffer, string16* result) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  NSPasteboard* pb = GetPasteboard();
  NSString* contents = [pb stringForType:NSStringPboardType];

  UTF8ToUTF16([contents UTF8String],
              [contents lengthOfBytesUsingEncoding:NSUTF8StringEncoding],
              result);
}

void Clipboard::ReadAsciiText(Clipboard::Buffer buffer,
                              std::string* result) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);
  NSPasteboard* pb = GetPasteboard();
  NSString* contents = [pb stringForType:NSStringPboardType];

  if (!contents)
    result->clear();
  else
    result->assign([contents UTF8String]);
}

void Clipboard::ReadHTML(Clipboard::Buffer buffer, string16* markup,
                         std::string* src_url, uint32* fragment_start,
                         uint32* fragment_end) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);

  // TODO(avi): src_url?
  markup->clear();
  if (src_url)
    src_url->clear();

  NSPasteboard* pb = GetPasteboard();
  NSArray* supportedTypes = [NSArray arrayWithObjects:NSHTMLPboardType,
                                                      NSRTFPboardType,
                                                      NSStringPboardType,
                                                      nil];
  NSString* bestType = [pb availableTypeFromArray:supportedTypes];
  if (bestType) {
    NSString* contents = [pb stringForType:bestType];
    if ([bestType isEqualToString:NSRTFPboardType])
      contents = [pb htmlFromRtf];
    UTF8ToUTF16([contents UTF8String],
                [contents lengthOfBytesUsingEncoding:NSUTF8StringEncoding],
                markup);
  }

  *fragment_start = 0;
  DCHECK(markup->length() <= kuint32max);
  *fragment_end = static_cast<uint32>(markup->length());
}

void Clipboard::ReadRTF(Buffer buffer, std::string* result) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);

  return ReadData(GetRtfFormatType(), result);
}

SkBitmap Clipboard::ReadImage(Buffer buffer) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);

  scoped_nsobject<NSImage> image(
      [[NSImage alloc] initWithPasteboard:GetPasteboard()]);
  if (!image.get())
    return SkBitmap();

  gfx::ScopedNSGraphicsContextSaveGState scoped_state;
  [image setFlipped:YES];
  int width = [image size].width;
  int height = [image size].height;

  gfx::Canvas canvas(gfx::Size(width, height), ui::SCALE_FACTOR_100P, false);
  {
    skia::ScopedPlatformPaint scoped_platform_paint(canvas.sk_canvas());
    CGContextRef gc = scoped_platform_paint.GetPlatformSurface();
    NSGraphicsContext* cocoa_gc =
        [NSGraphicsContext graphicsContextWithGraphicsPort:gc flipped:NO];
    [NSGraphicsContext setCurrentContext:cocoa_gc];
    [image drawInRect:NSMakeRect(0, 0, width, height)
             fromRect:NSZeroRect
            operation:NSCompositeCopy
             fraction:1.0];
  }
  return canvas.ExtractImageRep().sk_bitmap();
}

void Clipboard::ReadCustomData(Buffer buffer,
                               const string16& type,
                               string16* result) const {
  DCHECK(CalledOnValidThread());
  DCHECK_EQ(buffer, BUFFER_STANDARD);

  NSPasteboard* pb = GetPasteboard();
  if ([[pb types] containsObject:kWebCustomDataPboardType]) {
    NSData* data = [pb dataForType:kWebCustomDataPboardType];
    if ([data length])
      ReadCustomDataForType([data bytes], [data length], type, result);
  }
}

void Clipboard::ReadBookmark(string16* title, std::string* url) const {
  DCHECK(CalledOnValidThread());
  NSPasteboard* pb = GetPasteboard();

  if (title) {
    NSString* contents = [pb stringForType:kUTTypeURLName];
    UTF8ToUTF16([contents UTF8String],
                [contents lengthOfBytesUsingEncoding:NSUTF8StringEncoding],
                title);
  }

  if (url) {
    NSString* url_string = [[NSURL URLFromPasteboard:pb] absoluteString];
    if (!url_string)
      url->clear();
    else
      url->assign([url_string UTF8String]);
  }
}

void Clipboard::ReadData(const FormatType& format, std::string* result) const {
  DCHECK(CalledOnValidThread());
  NSPasteboard* pb = GetPasteboard();
  NSData* data = [pb dataForType:format.ToNSString()];
  if ([data length])
    result->assign(static_cast<const char*>([data bytes]), [data length]);
}

SourceTag Clipboard::ReadSourceTag(Buffer buffer) const {
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
const Clipboard::FormatType& Clipboard::GetUrlFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (NSURLPboardType));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetUrlWFormatType() {
  return GetUrlFormatType();
}

// static
const Clipboard::FormatType& Clipboard::GetPlainTextFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (NSStringPboardType));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetPlainTextWFormatType() {
  return GetPlainTextFormatType();
}

// static
const Clipboard::FormatType& Clipboard::GetFilenameFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (NSFilenamesPboardType));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetFilenameWFormatType() {
  return GetFilenameFormatType();
}

// static
const Clipboard::FormatType& Clipboard::GetHtmlFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (NSHTMLPboardType));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetRtfFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (NSRTFPboardType));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetBitmapFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (NSTIFFPboardType));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetWebKitSmartPasteFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kWebSmartPastePboardType));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetWebCustomDataFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kWebCustomDataPboardType));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetPepperCustomDataFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kPepperCustomDataPboardType));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetSourceTagFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kSourceTagPboardType));
  return type;
}

}  // namespace ui
