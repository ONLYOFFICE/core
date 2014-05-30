// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "build/build_config.h"

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/pickle.h"
#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "testing/platform_test.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/clipboard/scoped_clipboard_writer.h"
#include "ui/gfx/size.h"

#if defined(OS_WIN)
#include "ui/base/clipboard/clipboard_util_win.h"
#endif

#if defined(OS_ANDROID)
#include "base/android/jni_android.h"
#endif

#if defined(OS_WIN) || defined(USE_AURA)
#include "base/message_loop.h"
#endif

namespace ui {

class ClipboardTest : public PlatformTest {
 protected:
  Clipboard& clipboard() { return clipboard_; }

 private:
  base::MessageLoopForUI message_loop_;
  Clipboard clipboard_;
};

namespace {

bool MarkupMatches(const string16& expected_markup,
                   const string16& actual_markup) {
  return actual_markup.find(expected_markup) != string16::npos;
}

}  // namespace

TEST_F(ClipboardTest, ClearTest) {
  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteText(ASCIIToUTF16("clear me"));
  }

  clipboard().Clear(Clipboard::BUFFER_STANDARD);

  EXPECT_FALSE(clipboard().IsFormatAvailable(
      Clipboard::GetPlainTextWFormatType(), Clipboard::BUFFER_STANDARD));
  EXPECT_FALSE(clipboard().IsFormatAvailable(
      Clipboard::GetPlainTextFormatType(), Clipboard::BUFFER_STANDARD));
}

TEST_F(ClipboardTest, TextTest) {
  string16 text(ASCIIToUTF16("This is a string16!#$")), text_result;
  std::string ascii_text;

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteText(text);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(
      Clipboard::GetPlainTextWFormatType(), Clipboard::BUFFER_STANDARD));
  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetPlainTextFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  clipboard().ReadText(Clipboard::BUFFER_STANDARD, &text_result);

  EXPECT_EQ(text, text_result);
  clipboard().ReadAsciiText(Clipboard::BUFFER_STANDARD, &ascii_text);
  EXPECT_EQ(UTF16ToUTF8(text), ascii_text);
}

TEST_F(ClipboardTest, HTMLTest) {
  string16 markup(ASCIIToUTF16("<string>Hi!</string>")), markup_result;
  std::string url("http://www.example.com/"), url_result;

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteHTML(markup, url);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetHtmlFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  uint32 ignored;
  clipboard().ReadHTML(Clipboard::BUFFER_STANDARD, &markup_result, &url_result,
                     &ignored, &ignored);
  EXPECT_PRED2(MarkupMatches, markup, markup_result);
#if defined(OS_WIN)
  // TODO(playmobil): It's not clear that non windows clipboards need to support
  // this.
  EXPECT_EQ(url, url_result);
#endif  // defined(OS_WIN)
}

TEST_F(ClipboardTest, RTFTest) {
  std::string rtf =
      "{\\rtf1\\ansi{\\fonttbl\\f0\\fswiss Helvetica;}\\f0\\pard\n"
      "This is some {\\b bold} text.\\par\n"
      "}";

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteRTF(rtf);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetRtfFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  std::string result;
  clipboard().ReadRTF(Clipboard::BUFFER_STANDARD, &result);
  EXPECT_EQ(rtf, result);
}

#if defined(TOOLKIT_GTK)
TEST_F(ClipboardTest, MultipleBufferTest) {
  string16 text(ASCIIToUTF16("Standard")), text_result;
  string16 markup(ASCIIToUTF16("<string>Selection</string>")), markup_result;
  std::string url("http://www.example.com/"), url_result;

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteText(text);
  }

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_SELECTION);
    clipboard_writer.WriteHTML(markup, url);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetPlainTextFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  EXPECT_FALSE(clipboard().IsFormatAvailable(
      Clipboard::GetPlainTextFormatType(),
      Clipboard::BUFFER_SELECTION));

  EXPECT_FALSE(clipboard().IsFormatAvailable(Clipboard::GetHtmlFormatType(),
                                             Clipboard::BUFFER_STANDARD));
  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetHtmlFormatType(),
                                            Clipboard::BUFFER_SELECTION));

  clipboard().ReadText(Clipboard::BUFFER_STANDARD, &text_result);
  EXPECT_EQ(text, text_result);

  uint32 ignored;
  clipboard().ReadHTML(Clipboard::BUFFER_SELECTION, &markup_result, &url_result,
                       &ignored, &ignored);
  EXPECT_PRED2(MarkupMatches, markup, markup_result);
}
#endif

TEST_F(ClipboardTest, TrickyHTMLTest) {
  string16 markup(ASCIIToUTF16("<em>Bye!<!--EndFragment --></em>")),
      markup_result;
  std::string url, url_result;

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteHTML(markup, url);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetHtmlFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  uint32 ignored;
  clipboard().ReadHTML(Clipboard::BUFFER_STANDARD, &markup_result, &url_result,
                       &ignored, &ignored);
  EXPECT_PRED2(MarkupMatches, markup, markup_result);
#if defined(OS_WIN)
  // TODO(playmobil): It's not clear that non windows clipboards need to support
  // this.
  EXPECT_EQ(url, url_result);
#endif  // defined(OS_WIN)
}

#if defined(OS_WIN)
TEST_F(ClipboardTest, UniodeHTMLTest) {
  string16 markup(UTF8ToUTF16("<div>A \xc3\xb8 \xe6\xb0\xb4</div>")),
      markup_result;
  std::string url, url_result;

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteHTML(markup, url);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetHtmlFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  uint32 fragment_start;
  uint32 fragment_end;
  clipboard().ReadHTML(Clipboard::BUFFER_STANDARD, &markup_result, &url_result,
                       &fragment_start, &fragment_end);
  EXPECT_PRED2(MarkupMatches, markup, markup_result);
  EXPECT_EQ(url, url_result);
  // Make sure that fragment indices were adjusted when converting.
  EXPECT_EQ(36, fragment_start);
  EXPECT_EQ(52, fragment_end);
}
#endif  // defined(OS_WIN)

#if defined(TOOLKIT_GTK)
// Regression test for crbug.com/56298 (pasting empty HTML crashes Linux).
TEST_F(ClipboardTest, EmptyHTMLTest) {
  // ScopedClipboardWriter doesn't let us write empty data to the clipboard.
  clipboard().clipboard_data_ = new Clipboard::TargetMap();
  // The 1 is so the compiler doesn't warn about allocating an empty array.
  char* empty = new char[1];
  clipboard().InsertMapping("text/html", empty, 0U);
  clipboard().SetGtkClipboard(Clipboard::BUFFER_STANDARD);

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetHtmlFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  string16 markup_result;
  std::string url_result;
  uint32 ignored;
  clipboard().ReadHTML(Clipboard::BUFFER_STANDARD, &markup_result, &url_result,
                       &ignored, &ignored);
  EXPECT_PRED2(MarkupMatches, string16(), markup_result);
}
#endif

// TODO(estade): Port the following test (decide what target we use for urls)
#if !defined(OS_POSIX) || defined(OS_MACOSX)
TEST_F(ClipboardTest, BookmarkTest) {
  string16 title(ASCIIToUTF16("The Example Company")), title_result;
  std::string url("http://www.example.com/"), url_result;

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteBookmark(title, url);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetUrlWFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  clipboard().ReadBookmark(&title_result, &url_result);
  EXPECT_EQ(title, title_result);
  EXPECT_EQ(url, url_result);
}
#endif  // defined(OS_WIN)

TEST_F(ClipboardTest, MultiFormatTest) {
  string16 text(ASCIIToUTF16("Hi!")), text_result;
  string16 markup(ASCIIToUTF16("<strong>Hi!</string>")), markup_result;
  std::string url("http://www.example.com/"), url_result;
  std::string ascii_text;

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteHTML(markup, url);
    clipboard_writer.WriteText(text);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetHtmlFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  EXPECT_TRUE(clipboard().IsFormatAvailable(
      Clipboard::GetPlainTextWFormatType(), Clipboard::BUFFER_STANDARD));
  EXPECT_TRUE(clipboard().IsFormatAvailable(
      Clipboard::GetPlainTextFormatType(), Clipboard::BUFFER_STANDARD));
  uint32 ignored;
  clipboard().ReadHTML(Clipboard::BUFFER_STANDARD, &markup_result, &url_result,
                       &ignored, &ignored);
  EXPECT_PRED2(MarkupMatches, markup, markup_result);
#if defined(OS_WIN)
  // TODO(playmobil): It's not clear that non windows clipboards need to support
  // this.
  EXPECT_EQ(url, url_result);
#endif  // defined(OS_WIN)
  clipboard().ReadText(Clipboard::BUFFER_STANDARD, &text_result);
  EXPECT_EQ(text, text_result);
  clipboard().ReadAsciiText(Clipboard::BUFFER_STANDARD, &ascii_text);
  EXPECT_EQ(UTF16ToUTF8(text), ascii_text);
}

TEST_F(ClipboardTest, URLTest) {
  string16 url(ASCIIToUTF16("http://www.google.com/"));

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteURL(url);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(
      Clipboard::GetPlainTextWFormatType(), Clipboard::BUFFER_STANDARD));
  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetPlainTextFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  string16 text_result;
  clipboard().ReadText(Clipboard::BUFFER_STANDARD, &text_result);

  EXPECT_EQ(text_result, url);

  std::string ascii_text;
  clipboard().ReadAsciiText(Clipboard::BUFFER_STANDARD, &ascii_text);
  EXPECT_EQ(UTF16ToUTF8(url), ascii_text);

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID)
  ascii_text.clear();
  clipboard().ReadAsciiText(Clipboard::BUFFER_SELECTION, &ascii_text);
  EXPECT_EQ(UTF16ToUTF8(url), ascii_text);
#endif
}

// TODO(erg): Reenable this everywhere once linux_aura learns what bitmaps are.
#if !(defined(USE_AURA) && !defined(OS_CHROMEOS))
TEST_F(ClipboardTest, SharedBitmapTest) {
  unsigned int fake_bitmap[] = {
    0x46155189, 0xF6A55C8D, 0x79845674, 0xFA57BD89,
    0x78FD46AE, 0x87C64F5A, 0x36EDC5AF, 0x4378F568,
    0x91E9F63A, 0xC31EA14F, 0x69AB32DF, 0x643A3FD1,
  };
  gfx::Size fake_bitmap_size(3, 4);
  uint32 bytes = sizeof(fake_bitmap);

  // Create shared memory region.
  base::SharedMemory shared_buf;
  ASSERT_TRUE(shared_buf.CreateAndMapAnonymous(bytes));
  memcpy(shared_buf.memory(), fake_bitmap, bytes);
  base::SharedMemoryHandle handle_to_share;
  base::ProcessHandle current_process = base::kNullProcessHandle;
#if defined(OS_WIN)
  current_process = GetCurrentProcess();
#endif
  shared_buf.ShareToProcess(current_process, &handle_to_share);
  ASSERT_TRUE(shared_buf.Unmap());

  // Setup data for clipboard().
  Clipboard::ObjectMapParam placeholder_param;
  Clipboard::ObjectMapParam size_param;
  const char* size_data = reinterpret_cast<const char*>(&fake_bitmap_size);
  for (size_t i = 0; i < sizeof(fake_bitmap_size); ++i)
    size_param.push_back(size_data[i]);

  Clipboard::ObjectMapParams params;
  params.push_back(placeholder_param);
  params.push_back(size_param);

  Clipboard::ObjectMap objects;
  objects[Clipboard::CBF_SMBITMAP] = params;
  Clipboard::ReplaceSharedMemHandle(&objects, handle_to_share, current_process);

  clipboard().WriteObjects(Clipboard::BUFFER_STANDARD,
                           objects,
                           SourceTag());

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetBitmapFormatType(),
                                            Clipboard::BUFFER_STANDARD));
}
#endif

// The following test somehow fails on GTK. The image when read back from the
// clipboard has the alpha channel set to 0xFF for some reason. The other
// channels stay intact. So I am turning this on only for aura.
//
// TODO(erg): This also crashes in linux_aura. Investigate once bitmap writing
// is implemented.
#if (defined(USE_AURA) && !(defined(OS_WIN) || !defined(OS_CHROMEOS))) || \
    defined(OS_ANDROID)
TEST_F(ClipboardTest, MultipleBitmapReadWriteTest) {
  // Test first bitmap
  unsigned int fake_bitmap_1[] = {
    0x46155189, 0xF6A55C8D, 0x79845674, 0xFA57BD89,
    0x78FD46AE, 0x87C64F5A, 0x36EDC5AF, 0x4378F568,
    0x91E9F63A, 0xC31EA14F, 0x69AB32DF, 0x643A3FD1,
  };
  gfx::Size fake_bitmap_1_size(3, 4);
  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteBitmapFromPixels(fake_bitmap_1, fake_bitmap_1_size);
  }
  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetBitmapFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  SkBitmap image_1 = clipboard().ReadImage(Clipboard::BUFFER_STANDARD);
  EXPECT_EQ(fake_bitmap_1_size, gfx::Size(image_1.width(), image_1.height()));
  unsigned int* pixels_1 = reinterpret_cast<unsigned int*>(image_1.getPixels());
  for (int i = 0; i < fake_bitmap_1_size.width(); ++i) {
    for (int j = 0; j < fake_bitmap_1_size.height(); ++j) {
      int id = i * fake_bitmap_1_size.height() + j;
      EXPECT_EQ(fake_bitmap_1[id], pixels_1[id]);
    }
  }

  // Test second bitmap
  unsigned int fake_bitmap_2[] = {
    0x46155189, 0xF6A55C8D,
    0x79845674, 0xFA57BD89,
    0x78FD46AE, 0x87C64F5A,
    0x36EDC5AF, 0x4378F568,
    0x91E9F63A, 0xC31EA14F,
    0x69AB32DF, 0x643A3FD1,
    0xA6DF041D, 0x83046278,
  };
  gfx::Size fake_bitmap_2_size(7, 2);
  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteBitmapFromPixels(fake_bitmap_2, fake_bitmap_2_size);
  }
  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetBitmapFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  SkBitmap image_2 = clipboard().ReadImage(Clipboard::BUFFER_STANDARD);
  EXPECT_EQ(fake_bitmap_2_size, gfx::Size(image_2.width(), image_2.height()));
  unsigned int* pixels_2 = reinterpret_cast<unsigned int*>(image_2.getPixels());
  for (int i = 0; i < fake_bitmap_2_size.width(); ++i) {
    for (int j = 0; j < fake_bitmap_2_size.height(); ++j) {
      int id = i * fake_bitmap_2_size.height() + j;
      EXPECT_EQ(fake_bitmap_2[id], pixels_2[id]);
    }
  }
}
#endif

TEST_F(ClipboardTest, DataTest) {
  const ui::Clipboard::FormatType kFormat =
      ui::Clipboard::GetFormatType("chromium/x-test-format");
  std::string payload("test string");
  Pickle write_pickle;
  write_pickle.WriteString(payload);

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WritePickledData(write_pickle, kFormat);
  }

  ASSERT_TRUE(clipboard().IsFormatAvailable(
      kFormat, Clipboard::BUFFER_STANDARD));
  std::string output;
  clipboard().ReadData(kFormat, &output);
  ASSERT_FALSE(output.empty());

  Pickle read_pickle(output.data(), output.size());
  PickleIterator iter(read_pickle);
  std::string unpickled_string;
  ASSERT_TRUE(read_pickle.ReadString(&iter, &unpickled_string));
  EXPECT_EQ(payload, unpickled_string);
}

TEST_F(ClipboardTest, MultipleDataTest) {
  const ui::Clipboard::FormatType kFormat1 =
      ui::Clipboard::GetFormatType("chromium/x-test-format1");
  std::string payload1("test string1");
  Pickle write_pickle1;
  write_pickle1.WriteString(payload1);

  const ui::Clipboard::FormatType kFormat2 =
      ui::Clipboard::GetFormatType("chromium/x-test-format2");
  std::string payload2("test string2");
  Pickle write_pickle2;
  write_pickle2.WriteString(payload2);

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WritePickledData(write_pickle1, kFormat1);
    // overwrite the previous pickle for fun
    clipboard_writer.WritePickledData(write_pickle2, kFormat2);
  }

  ASSERT_TRUE(clipboard().IsFormatAvailable(
      kFormat2, Clipboard::BUFFER_STANDARD));

  // Check string 2.
  std::string output2;
  clipboard().ReadData(kFormat2, &output2);
  ASSERT_FALSE(output2.empty());

  Pickle read_pickle2(output2.data(), output2.size());
  PickleIterator iter2(read_pickle2);
  std::string unpickled_string2;
  ASSERT_TRUE(read_pickle2.ReadString(&iter2, &unpickled_string2));
  EXPECT_EQ(payload2, unpickled_string2);

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WritePickledData(write_pickle2, kFormat2);
    // overwrite the previous pickle for fun
    clipboard_writer.WritePickledData(write_pickle1, kFormat1);
  }

  ASSERT_TRUE(clipboard().IsFormatAvailable(
      kFormat1, Clipboard::BUFFER_STANDARD));

  // Check string 1.
  std::string output1;
  clipboard().ReadData(kFormat1, &output1);
  ASSERT_FALSE(output1.empty());

  Pickle read_pickle1(output1.data(), output1.size());
  PickleIterator iter1(read_pickle1);
  std::string unpickled_string1;
  ASSERT_TRUE(read_pickle1.ReadString(&iter1, &unpickled_string1));
  EXPECT_EQ(payload1, unpickled_string1);
}

#if defined(OS_WIN)  // Windows only tests.
TEST_F(ClipboardTest, HyperlinkTest) {
  const std::string kTitle("The Example Company");
  const std::string kUrl("http://www.example.com/");
  const std::string kExpectedHtml("<a href=\"http://www.example.com/\">"
                                  "The Example Company</a>");
  std::string url_result;
  string16 html_result;

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteHyperlink(ASCIIToUTF16(kTitle), kUrl);
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetHtmlFormatType(),
                                            Clipboard::BUFFER_STANDARD));
  uint32 ignored;
  clipboard().ReadHTML(Clipboard::BUFFER_STANDARD, &html_result, &url_result,
                       &ignored, &ignored);
  EXPECT_PRED2(MarkupMatches, ASCIIToUTF16(kExpectedHtml), html_result);
}

TEST_F(ClipboardTest, WebSmartPasteTest) {
  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteWebSmartPaste();
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(
      Clipboard::GetWebKitSmartPasteFormatType(), Clipboard::BUFFER_STANDARD));
}

TEST_F(ClipboardTest, BitmapTest) {
  unsigned int fake_bitmap[] = {
    0x46155189, 0xF6A55C8D, 0x79845674, 0xFA57BD89,
    0x78FD46AE, 0x87C64F5A, 0x36EDC5AF, 0x4378F568,
    0x91E9F63A, 0xC31EA14F, 0x69AB32DF, 0x643A3FD1,
  };

  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteBitmapFromPixels(fake_bitmap, gfx::Size(3, 4));
  }

  EXPECT_TRUE(clipboard().IsFormatAvailable(Clipboard::GetBitmapFormatType(),
                                            Clipboard::BUFFER_STANDARD));
}

void HtmlTestHelper(const std::string& cf_html,
                    const std::string& expected_html) {
  std::string html;
  ClipboardUtil::CFHtmlToHtml(cf_html, &html, NULL);
  EXPECT_EQ(html, expected_html);
}

TEST_F(ClipboardTest, HtmlTest) {
  // Test converting from CF_HTML format data with <!--StartFragment--> and
  // <!--EndFragment--> comments, like from MS Word.
  HtmlTestHelper("Version:1.0\r\n"
                 "StartHTML:0000000105\r\n"
                 "EndHTML:0000000199\r\n"
                 "StartFragment:0000000123\r\n"
                 "EndFragment:0000000161\r\n"
                 "\r\n"
                 "<html>\r\n"
                 "<body>\r\n"
                 "<!--StartFragment-->\r\n"
                 "\r\n"
                 "<p>Foo</p>\r\n"
                 "\r\n"
                 "<!--EndFragment-->\r\n"
                 "</body>\r\n"
                 "</html>\r\n\r\n",
                 "<p>Foo</p>");

  // Test converting from CF_HTML format data without <!--StartFragment--> and
  // <!--EndFragment--> comments, like from OpenOffice Writer.
  HtmlTestHelper("Version:1.0\r\n"
                 "StartHTML:0000000105\r\n"
                 "EndHTML:0000000151\r\n"
                 "StartFragment:0000000121\r\n"
                 "EndFragment:0000000131\r\n"
                 "<html>\r\n"
                 "<body>\r\n"
                 "<p>Foo</p>\r\n"
                 "</body>\r\n"
                 "</html>\r\n\r\n",
                 "<p>Foo</p>");
}
#endif  // defined(OS_WIN)

// Test writing all formats we have simultaneously.
TEST_F(ClipboardTest, WriteEverything) {
  {
    ScopedClipboardWriter writer(&clipboard(), Clipboard::BUFFER_STANDARD);
    writer.WriteText(UTF8ToUTF16("foo"));
    writer.WriteURL(UTF8ToUTF16("foo"));
    writer.WriteHTML(UTF8ToUTF16("foo"), "bar");
    writer.WriteBookmark(UTF8ToUTF16("foo"), "bar");
    writer.WriteHyperlink(ASCIIToUTF16("foo"), "bar");
    writer.WriteWebSmartPaste();
    // Left out: WriteFile, WriteFiles, WriteBitmapFromPixels, WritePickledData.
  }

  // Passes if we don't crash.
}

#if defined(OS_ANDROID)

// Test that if another application writes some text to the pasteboard the
// clipboard properly invalidates other types.
TEST_F(ClipboardTest, InternalClipboardInvalidation) {
  const unsigned int kFakeBitmap[] = {
    0x46155189, 0xF6A55C8D, 0x79845674, 0xFA57BD89,
    0x78FD46AE, 0x87C64F5A, 0x36EDC5AF, 0x4378F568,
    0x91E9F63A, 0xC31EA14F, 0x69AB32DF, 0x643A3FD1,
  };

  // Write a bitmap in our clipboard().
  {
    ScopedClipboardWriter clipboard_writer(&clipboard(),
                                           Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteBitmapFromPixels(kFakeBitmap, gfx::Size(3, 4));
  }

  //
  // Simulate that another application copied something in the Clipboard
  //
  std::string new_value("Some text copied by some other app");
  using base::android::MethodID;
  using base::android::ScopedJavaLocalRef;

  JNIEnv* env = base::android::AttachCurrentThread();
  ASSERT_TRUE(env);

  jobject context = base::android::GetApplicationContext();
  ASSERT_TRUE(context);

  ScopedJavaLocalRef<jclass> context_class =
      base::android::GetClass(env, "android/content/Context");

  jmethodID get_system_service = MethodID::Get<MethodID::TYPE_INSTANCE>(
      env, context_class.obj(), "getSystemService",
      "(Ljava/lang/String;)Ljava/lang/Object;");

  // Retrieve the system service.
  ScopedJavaLocalRef<jstring> service_name(env, env->NewStringUTF("clipboard"));
  ScopedJavaLocalRef<jobject> clipboard_manager(
      env, env->CallObjectMethod(
        context, get_system_service, service_name.obj()));
  ASSERT_TRUE(clipboard_manager.obj() && !base::android::ClearException(env));

  ScopedJavaLocalRef<jclass> clipboard_class =
      base::android::GetClass(env, "android/text/ClipboardManager");
  jmethodID set_text = MethodID::Get<MethodID::TYPE_INSTANCE>(
      env, clipboard_class.obj(), "setText", "(Ljava/lang/CharSequence;)V");

  // Will need to call toString as CharSequence is not always a String.
  env->CallVoidMethod(clipboard_manager.obj(),
                      set_text,
                      env->NewStringUTF(new_value.c_str()));

  // The bitmap that should have been available should be gone.
  EXPECT_FALSE(clipboard().IsFormatAvailable(Clipboard::GetBitmapFormatType(),
                                             Clipboard::BUFFER_STANDARD));

  // Make sure some text is available
  EXPECT_TRUE(clipboard().IsFormatAvailable(
      Clipboard::GetPlainTextWFormatType(), Clipboard::BUFFER_STANDARD));

  // Make sure the text is what we inserted while simulating the other app
  std::string contents;
  clipboard().ReadAsciiText(Clipboard::BUFFER_STANDARD, &contents);
  EXPECT_EQ(contents, new_value);
}
#endif
}  // namespace ui
