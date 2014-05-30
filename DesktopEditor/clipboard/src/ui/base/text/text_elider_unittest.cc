// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Unit tests for eliding and formatting utility functions.

#include "ui/base/text/text_elider.h"

#include "base/files/file_path.h"
#include "base/i18n/rtl.h"
#include "base/memory/scoped_ptr.h"
#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "googleurl/src/gurl.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/font.h"

namespace ui {

namespace {

struct Testcase {
  const std::string input;
  const std::string output;
};

struct FileTestcase {
  const base::FilePath::StringType input;
  const std::string output;
};

struct UTF16Testcase {
  const string16 input;
  const string16 output;
};

struct TestData {
  const std::string a;
  const std::string b;
  const int compare_result;
};

void RunUrlTest(Testcase* testcases, size_t num_testcases) {
  static const gfx::Font font;
  for (size_t i = 0; i < num_testcases; ++i) {
    const GURL url(testcases[i].input);
    // Should we test with non-empty language list?
    // That's kinda redundant with net_util_unittests.
    EXPECT_EQ(UTF8ToUTF16(testcases[i].output),
              ElideUrl(url, font,
                       font.GetStringWidth(UTF8ToUTF16(testcases[i].output)),
                       std::string()));
  }
}

}  // namespace

// TODO(ios): Complex eliding is off by one for some of those tests on iOS.
// See crbug.com/154019
#if defined(OS_IOS)
#define MAYBE_ElideEmail DISABLED_ElideEmail
#else
#define MAYBE_ElideEmail ElideEmail
#endif
TEST(TextEliderTest, MAYBE_ElideEmail) {
  const std::string kEllipsisStr(kEllipsis);

  // Test emails and their expected elided forms (from which the available
  // widths will be derived).
  // For elided forms in which both the username and domain must be elided:
  // the result (how many characters are left on each side) can be font
  // dependent. To avoid this, the username is prefixed with the characters
  // expected to remain in the domain.
  Testcase testcases[] = {
      {"g@g.c", "g@g.c"},
      {"g@g.c", kEllipsisStr},
      {"ga@co.ca", "ga@c" + kEllipsisStr + "a"},
      {"short@small.com", "s" + kEllipsisStr + "@s" + kEllipsisStr},
      {"short@small.com", "s" + kEllipsisStr + "@small.com"},
      {"short@longbutlotsofspace.com", "short@longbutlotsofspace.com"},
      {"short@longbutnotverymuchspace.com",
       "short@long" + kEllipsisStr + ".com"},
      {"la_short@longbutverytightspace.ca",
       "la" + kEllipsisStr + "@l" + kEllipsisStr + "a"},
      {"longusername@gmail.com", "long" + kEllipsisStr + "@gmail.com"},
      {"elidetothemax@justfits.com", "e" + kEllipsisStr + "@justfits.com"},
      {"thatom_somelongemail@thatdoesntfit.com",
       "thatom" + kEllipsisStr + "@tha" + kEllipsisStr + "om"},
      {"namefits@butthedomaindoesnt.com",
       "namefits@butthedo" + kEllipsisStr + "snt.com"},
      {"widthtootight@nospace.com", kEllipsisStr},
      {"nospaceforusername@l", kEllipsisStr},
      {"little@littlespace.com", "l" + kEllipsisStr + "@l" + kEllipsisStr},
      {"l@llllllllllllllllllllllll.com", "l@lllll" + kEllipsisStr + ".com"},
      {"messed\"up@whyanat\"++@notgoogley.com",
       "messed\"up@whyanat\"++@notgoogley.com"},
      {"messed\"up@whyanat\"++@notgoogley.com",
       "messed\"up@why" + kEllipsisStr + "@notgoogley.com"},
      {"noca_messed\"up@whyanat\"++@notgoogley.ca",
       "noca" + kEllipsisStr + "@no" + kEllipsisStr + "ca"},
      {"at\"@@@@@@@@@...@@.@.@.@@@\"@madness.com",
       "at\"@@@@@@@@@...@@.@." + kEllipsisStr + "@madness.com"},
      // Special case: "m..." takes more than half of the available width; thus
      // the domain must elide to "l..." and not "l...l" as it must allow enough
      // space for the minimal username elision although its half of the
      // available width would normally allow it to elide to "l...l".
      {"mmmmm@llllllllll", "m" + kEllipsisStr + "@l" + kEllipsisStr},
  };

  const gfx::Font font;
  for (size_t i = 0; i < arraysize(testcases); ++i) {
    const string16 expected_output = UTF8ToUTF16(testcases[i].output);
    EXPECT_EQ(expected_output,
              ElideEmail(UTF8ToUTF16(testcases[i].input),
                         font,
                         font.GetStringWidth(expected_output)));
  }
}

TEST(TextEliderTest, ElideEmailMoreSpace) {
  const int test_width_factors[] = {
      100,
      10000,
      1000000,
  };
  const std::string test_emails[] = {
      "a@c",
      "test@email.com",
      "short@verysuperdupperlongdomain.com",
      "supermegalongusername@withasuperlonnnggggdomain.gouv.qc.ca",
  };

  const gfx::Font font;
  for (size_t i = 0; i < arraysize(test_width_factors); ++i) {
    const int test_width = test_width_factors[i] *
                           font.GetAverageCharacterWidth();
    for (size_t j = 0; j < arraysize(test_emails); ++j) {
      // Extra space is available: the email should not be elided.
      const string16 test_email = UTF8ToUTF16(test_emails[j]);
      EXPECT_EQ(test_email, ElideEmail(test_email, font, test_width));
    }
  }
}

// Test eliding of commonplace URLs.
TEST(TextEliderTest, TestGeneralEliding) {
  const std::string kEllipsisStr(kEllipsis);
  Testcase testcases[] = {
    {"http://www.google.com/intl/en/ads/",
     "www.google.com/intl/en/ads/"},
    {"http://www.google.com/intl/en/ads/", "www.google.com/intl/en/ads/"},
    {"http://www.google.com/intl/en/ads/",
     "google.com/intl/" + kEllipsisStr + "/ads/"},
    {"http://www.google.com/intl/en/ads/",
     "google.com/" + kEllipsisStr + "/ads/"},
    {"http://www.google.com/intl/en/ads/", "google.com/" + kEllipsisStr},
    {"http://www.google.com/intl/en/ads/", "goog" + kEllipsisStr},
    {"https://subdomain.foo.com/bar/filename.html",
     "subdomain.foo.com/bar/filename.html"},
    {"https://subdomain.foo.com/bar/filename.html",
     "subdomain.foo.com/" + kEllipsisStr + "/filename.html"},
    {"http://subdomain.foo.com/bar/filename.html",
     kEllipsisStr + "foo.com/" + kEllipsisStr + "/filename.html"},
    {"http://www.google.com/intl/en/ads/?aLongQueryWhichIsNotRequired",
     "www.google.com/intl/en/ads/?aLongQ" + kEllipsisStr},
  };

  RunUrlTest(testcases, arraysize(testcases));
}

// When there is very little space available, the elision code will shorten
// both path AND file name to an ellipsis - ".../...". To avoid this result,
// there is a hack in place that simply treats them as one string in this
// case.
TEST(TextEliderTest, TestTrailingEllipsisSlashEllipsisHack) {
  const std::string kEllipsisStr(kEllipsis);

  // Very little space, would cause double ellipsis.
  gfx::Font font;
  GURL url("http://battersbox.com/directory/foo/peter_paul_and_mary.html");
  int available_width = font.GetStringWidth(
      UTF8ToUTF16("battersbox.com/" + kEllipsisStr + "/" + kEllipsisStr));

  // Create the expected string, after elision. Depending on font size, the
  // directory might become /dir... or /di... or/d... - it never should be
  // shorter than that. (If it is, the font considers d... to be longer
  // than .../... -  that should never happen).
  ASSERT_GT(font.GetStringWidth(UTF8ToUTF16(kEllipsisStr + "/" + kEllipsisStr)),
      font.GetStringWidth(UTF8ToUTF16("d" + kEllipsisStr)));
  GURL long_url("http://battersbox.com/directorynameisreallylongtoforcetrunc");
  string16 expected = ElideUrl(long_url, font, available_width, std::string());
  // Ensure that the expected result still contains part of the directory name.
  ASSERT_GT(expected.length(), std::string("battersbox.com/d").length());
  EXPECT_EQ(expected,
             ElideUrl(url, font, available_width, std::string()));

  // More space available - elide directories, partially elide filename.
  Testcase testcases[] = {
    {"http://battersbox.com/directory/foo/peter_paul_and_mary.html",
     "battersbox.com/" + kEllipsisStr + "/peter" + kEllipsisStr},
  };
  RunUrlTest(testcases, arraysize(testcases));
}

// Test eliding of empty strings, URLs with ports, passwords, queries, etc.
TEST(TextEliderTest, TestMoreEliding) {
  const std::string kEllipsisStr(kEllipsis);
  Testcase testcases[] = {
    {"http://www.google.com/foo?bar", "www.google.com/foo?bar"},
    {"http://xyz.google.com/foo?bar", "xyz.google.com/foo?" + kEllipsisStr},
    {"http://xyz.google.com/foo?bar", "xyz.google.com/foo" + kEllipsisStr},
    {"http://xyz.google.com/foo?bar", "xyz.google.com/fo" + kEllipsisStr},
    {"http://a.b.com/pathname/c?d", "a.b.com/" + kEllipsisStr + "/c?d"},
    {"", ""},
    {"http://foo.bar..example.com...hello/test/filename.html",
     "foo.bar..example.com...hello/" + kEllipsisStr + "/filename.html"},
    {"http://foo.bar../", "foo.bar.."},
    {"http://xn--1lq90i.cn/foo", "\xe5\x8c\x97\xe4\xba\xac.cn/foo"},
    {"http://me:mypass@secrethost.com:99/foo?bar#baz",
     "secrethost.com:99/foo?bar#baz"},
    {"http://me:mypass@ss%xxfdsf.com/foo", "ss%25xxfdsf.com/foo"},
    {"mailto:elgoato@elgoato.com", "mailto:elgoato@elgoato.com"},
    {"javascript:click(0)", "javascript:click(0)"},
    {"https://chess.eecs.berkeley.edu:4430/login/arbitfilename",
     "chess.eecs.berkeley.edu:4430/login/arbitfilename"},
    {"https://chess.eecs.berkeley.edu:4430/login/arbitfilename",
     kEllipsisStr + "berkeley.edu:4430/" + kEllipsisStr + "/arbitfilename"},

    // Unescaping.
    {"http://www/%E4%BD%A0%E5%A5%BD?q=%E4%BD%A0%E5%A5%BD#\xe4\xbd\xa0",
     "www/\xe4\xbd\xa0\xe5\xa5\xbd?q=\xe4\xbd\xa0\xe5\xa5\xbd#\xe4\xbd\xa0"},

    // Invalid unescaping for path. The ref will always be valid UTF-8. We don't
    // bother to do too many edge cases, since these are handled by the escaper
    // unittest.
    {"http://www/%E4%A0%E5%A5%BD?q=%E4%BD%A0%E5%A5%BD#\xe4\xbd\xa0",
     "www/%E4%A0%E5%A5%BD?q=\xe4\xbd\xa0\xe5\xa5\xbd#\xe4\xbd\xa0"},
  };

  RunUrlTest(testcases, arraysize(testcases));
}

// Test eliding of file: URLs.
TEST(TextEliderTest, TestFileURLEliding) {
  const std::string kEllipsisStr(kEllipsis);
  Testcase testcases[] = {
    {"file:///C:/path1/path2/path3/filename",
     "file:///C:/path1/path2/path3/filename"},
    {"file:///C:/path1/path2/path3/filename",
     "C:/path1/path2/path3/filename"},
// GURL parses "file:///C:path" differently on windows than it does on posix.
#if defined(OS_WIN)
    {"file:///C:path1/path2/path3/filename",
     "C:/path1/path2/" + kEllipsisStr + "/filename"},
    {"file:///C:path1/path2/path3/filename",
     "C:/path1/" + kEllipsisStr + "/filename"},
    {"file:///C:path1/path2/path3/filename",
     "C:/" + kEllipsisStr + "/filename"},
#endif
    {"file://filer/foo/bar/file", "filer/foo/bar/file"},
    {"file://filer/foo/bar/file", "filer/foo/" + kEllipsisStr + "/file"},
    {"file://filer/foo/bar/file", "filer/" + kEllipsisStr + "/file"},
  };

  RunUrlTest(testcases, arraysize(testcases));
}

// TODO(ios): Complex eliding is off by one for some of those tests on iOS.
// See crbug.com/154019
#if defined(OS_IOS)
#define MAYBE_TestFilenameEliding DISABLED_TestFilenameEliding
#else
#define MAYBE_TestFilenameEliding TestFilenameEliding
#endif
TEST(TextEliderTest, MAYBE_TestFilenameEliding) {
  const std::string kEllipsisStr(kEllipsis);
  const base::FilePath::StringType kPathSeparator =
      base::FilePath::StringType().append(1, base::FilePath::kSeparators[0]);

  FileTestcase testcases[] = {
    {FILE_PATH_LITERAL(""), ""},
    {FILE_PATH_LITERAL("."), "."},
    {FILE_PATH_LITERAL("filename.exe"), "filename.exe"},
    {FILE_PATH_LITERAL(".longext"), ".longext"},
    {FILE_PATH_LITERAL("pie"), "pie"},
    {FILE_PATH_LITERAL("c:") + kPathSeparator + FILE_PATH_LITERAL("path") +
      kPathSeparator + FILE_PATH_LITERAL("filename.pie"),
      "filename.pie"},
    {FILE_PATH_LITERAL("c:") + kPathSeparator + FILE_PATH_LITERAL("path") +
      kPathSeparator + FILE_PATH_LITERAL("longfilename.pie"),
      "long" + kEllipsisStr + ".pie"},
    {FILE_PATH_LITERAL("http://path.com/filename.pie"), "filename.pie"},
    {FILE_PATH_LITERAL("http://path.com/longfilename.pie"),
      "long" + kEllipsisStr + ".pie"},
    {FILE_PATH_LITERAL("piesmashingtacularpants"), "pie" + kEllipsisStr},
    {FILE_PATH_LITERAL(".piesmashingtacularpants"), ".pie" + kEllipsisStr},
    {FILE_PATH_LITERAL("cheese."), "cheese."},
    {FILE_PATH_LITERAL("file name.longext"),
      "file" + kEllipsisStr + ".longext"},
    {FILE_PATH_LITERAL("fil ename.longext"),
      "fil " + kEllipsisStr + ".longext"},
    {FILE_PATH_LITERAL("filename.longext"),
      "file" + kEllipsisStr + ".longext"},
    {FILE_PATH_LITERAL("filename.middleext.longext"),
      "filename.mid" + kEllipsisStr + ".longext"},
    {FILE_PATH_LITERAL("filename.superduperextremelylongext"),
      "filename.sup" + kEllipsisStr + "emelylongext"},
    {FILE_PATH_LITERAL("filenamereallylongtext.superduperextremelylongext"),
      "filenamereall" + kEllipsisStr + "emelylongext"},
    {FILE_PATH_LITERAL("file.name.really.long.text.superduperextremelylongext"),
      "file.name.re" + kEllipsisStr + "emelylongext"}
  };

  static const gfx::Font font;
  for (size_t i = 0; i < arraysize(testcases); ++i) {
    base::FilePath filepath(testcases[i].input);
    string16 expected = UTF8ToUTF16(testcases[i].output);
    expected = base::i18n::GetDisplayStringInLTRDirectionality(expected);
    EXPECT_EQ(expected, ElideFilename(filepath,
        font,
        font.GetStringWidth(UTF8ToUTF16(testcases[i].output))));
  }
}

TEST(TextEliderTest, ElideTextTruncate) {
  const gfx::Font font;
  const int kTestWidth = font.GetStringWidth(ASCIIToUTF16("Test"));
  struct TestData {
    const char* input;
    int width;
    const char* output;
  } cases[] = {
    { "", 0, "" },
    { "Test", 0, "" },
    { "", kTestWidth, "" },
    { "Tes", kTestWidth, "Tes" },
    { "Test", kTestWidth, "Test" },
    { "Tests", kTestWidth, "Test" },
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    string16 result = ElideText(UTF8ToUTF16(cases[i].input), font,
                                cases[i].width, TRUNCATE_AT_END);
    EXPECT_EQ(cases[i].output, UTF16ToUTF8(result));
  }
}

TEST(TextEliderTest, ElideTextEllipsis) {
  const gfx::Font font;
  const int kTestWidth = font.GetStringWidth(ASCIIToUTF16("Test"));
  const char* kEllipsis = "\xE2\x80\xA6";
  const int kEllipsisWidth = font.GetStringWidth(UTF8ToUTF16(kEllipsis));
  struct TestData {
    const char* input;
    int width;
    const char* output;
  } cases[] = {
    { "", 0, "" },
    { "Test", 0, "" },
    { "Test", kEllipsisWidth, kEllipsis },
    { "", kTestWidth, "" },
    { "Tes", kTestWidth, "Tes" },
    { "Test", kTestWidth, "Test" },
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    string16 result = ElideText(UTF8ToUTF16(cases[i].input), font,
                                cases[i].width, ELIDE_AT_END);
    EXPECT_EQ(cases[i].output, UTF16ToUTF8(result));
  }
}

// Checks that all occurrences of |first_char| are followed by |second_char| and
// all occurrences of |second_char| are preceded by |first_char| in |text|.
static void CheckSurrogatePairs(const string16& text,
                                char16 first_char,
                                char16 second_char) {
  size_t index = text.find_first_of(first_char);
  while (index != string16::npos) {
    EXPECT_LT(index, text.length() - 1);
    EXPECT_EQ(second_char, text[index + 1]);
    index = text.find_first_of(first_char, index + 1);
  }
  index = text.find_first_of(second_char);
  while (index != string16::npos) {
    EXPECT_GT(index, 0U);
    EXPECT_EQ(first_char, text[index - 1]);
    index = text.find_first_of(second_char, index + 1);
  }
}

TEST(TextEliderTest, ElideTextSurrogatePairs) {
  const gfx::Font font;
  // The below is 'MUSICAL SYMBOL G CLEF', which is represented in UTF-16 as
  // two characters forming a surrogate pair 0x0001D11E.
  const std::string kSurrogate = "\xF0\x9D\x84\x9E";
  const string16 kTestString =
      UTF8ToUTF16(kSurrogate + "ab" + kSurrogate + kSurrogate + "cd");
  const int kTestStringWidth = font.GetStringWidth(kTestString);
  const char16 kSurrogateFirstChar = kTestString[0];
  const char16 kSurrogateSecondChar = kTestString[1];
  string16 result;

  // Elide |kTextString| to all possible widths and check that no instance of
  // |kSurrogate| was split in two.
  for (int width = 0; width <= kTestStringWidth; width++) {
    result = ElideText(kTestString, font, width, TRUNCATE_AT_END);
    CheckSurrogatePairs(result, kSurrogateFirstChar, kSurrogateSecondChar);

    result = ElideText(kTestString, font, width, ELIDE_AT_END);
    CheckSurrogatePairs(result, kSurrogateFirstChar, kSurrogateSecondChar);

    result = ElideText(kTestString, font, width, ELIDE_IN_MIDDLE);
    CheckSurrogatePairs(result, kSurrogateFirstChar, kSurrogateSecondChar);
  }
}

TEST(TextEliderTest, ElideTextLongStrings) {
  const string16 kEllipsisStr = UTF8ToUTF16(kEllipsis);
  string16 data_scheme(UTF8ToUTF16("data:text/plain,"));
  size_t data_scheme_length = data_scheme.length();

  string16 ten_a(10, 'a');
  string16 hundred_a(100, 'a');
  string16 thousand_a(1000, 'a');
  string16 ten_thousand_a(10000, 'a');
  string16 hundred_thousand_a(100000, 'a');
  string16 million_a(1000000, 'a');

  size_t number_of_as = 156;
  string16 long_string_end(
      data_scheme + string16(number_of_as, 'a') + kEllipsisStr);
  UTF16Testcase testcases_end[] = {
     {data_scheme + ten_a,              data_scheme + ten_a},
     {data_scheme + hundred_a,          data_scheme + hundred_a},
     {data_scheme + thousand_a,         long_string_end},
     {data_scheme + ten_thousand_a,     long_string_end},
     {data_scheme + hundred_thousand_a, long_string_end},
     {data_scheme + million_a,          long_string_end},
  };

  const gfx::Font font;
  int ellipsis_width = font.GetStringWidth(kEllipsisStr);
  for (size_t i = 0; i < arraysize(testcases_end); ++i) {
    // Compare sizes rather than actual contents because if the test fails,
    // output is rather long.
    EXPECT_EQ(testcases_end[i].output.size(),
              ElideText(testcases_end[i].input, font,
                        font.GetStringWidth(testcases_end[i].output),
                        ELIDE_AT_END).size());
    EXPECT_EQ(kEllipsisStr,
              ElideText(testcases_end[i].input, font, ellipsis_width,
                        ELIDE_AT_END));
  }

  size_t number_of_trailing_as = (data_scheme_length + number_of_as) / 2;
  string16 long_string_middle(data_scheme +
      string16(number_of_as - number_of_trailing_as, 'a') + kEllipsisStr +
      string16(number_of_trailing_as, 'a'));
  UTF16Testcase testcases_middle[] = {
     {data_scheme + ten_a,              data_scheme + ten_a},
     {data_scheme + hundred_a,          data_scheme + hundred_a},
     {data_scheme + thousand_a,         long_string_middle},
     {data_scheme + ten_thousand_a,     long_string_middle},
     {data_scheme + hundred_thousand_a, long_string_middle},
     {data_scheme + million_a,          long_string_middle},
  };

  for (size_t i = 0; i < arraysize(testcases_middle); ++i) {
    // Compare sizes rather than actual contents because if the test fails,
    // output is rather long.
    EXPECT_EQ(testcases_middle[i].output.size(),
              ElideText(testcases_middle[i].input, font,
                        font.GetStringWidth(testcases_middle[i].output),
                        ELIDE_AT_END).size());
    EXPECT_EQ(kEllipsisStr,
              ElideText(testcases_middle[i].input, font, ellipsis_width,
                        ELIDE_AT_END));
  }
}

// Verifies display_url is set correctly.
TEST(TextEliderTest, SortedDisplayURL) {
  SortedDisplayURL d_url(GURL("http://www.google.com"), std::string());
  EXPECT_EQ("www.google.com", UTF16ToASCII(d_url.display_url()));
}

// Verifies DisplayURL::Compare works correctly.
TEST(TextEliderTest, SortedDisplayURLCompare) {
  UErrorCode create_status = U_ZERO_ERROR;
  scoped_ptr<icu::Collator> collator(
      icu::Collator::createInstance(create_status));
  if (!U_SUCCESS(create_status))
    return;

  TestData tests[] = {
    // IDN comparison. Hosts equal, so compares on path.
    { "http://xn--1lq90i.cn/a", "http://xn--1lq90i.cn/b", -1},

    // Because the host and after host match, this compares the full url.
    { "http://www.x/b", "http://x/b", -1 },

    // Because the host and after host match, this compares the full url.
    { "http://www.a:1/b", "http://a:1/b", 1 },

    // The hosts match, so these end up comparing on the after host portion.
    { "http://www.x:0/b", "http://x:1/b", -1 },
    { "http://www.x/a", "http://x/b", -1 },
    { "http://x/b", "http://www.x/a", 1 },

    // Trivial Equality.
    { "http://a/", "http://a/", 0 },

    // Compares just hosts.
    { "http://www.a/", "http://b/", -1 },
  };

  for (size_t i = 0; i < arraysize(tests); ++i) {
    SortedDisplayURL url1(GURL(tests[i].a), std::string());
    SortedDisplayURL url2(GURL(tests[i].b), std::string());
    EXPECT_EQ(tests[i].compare_result, url1.Compare(url2, collator.get()));
    EXPECT_EQ(-tests[i].compare_result, url2.Compare(url1, collator.get()));
  }
}

TEST(TextEliderTest, ElideString) {
  struct TestData {
    const char* input;
    int max_len;
    bool result;
    const char* output;
  } cases[] = {
    { "Hello", 0, true, "" },
    { "", 0, false, "" },
    { "Hello, my name is Tom", 1, true, "H" },
    { "Hello, my name is Tom", 2, true, "He" },
    { "Hello, my name is Tom", 3, true, "H.m" },
    { "Hello, my name is Tom", 4, true, "H..m" },
    { "Hello, my name is Tom", 5, true, "H...m" },
    { "Hello, my name is Tom", 6, true, "He...m" },
    { "Hello, my name is Tom", 7, true, "He...om" },
    { "Hello, my name is Tom", 10, true, "Hell...Tom" },
    { "Hello, my name is Tom", 100, false, "Hello, my name is Tom" }
  };
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    string16 output;
    EXPECT_EQ(cases[i].result,
              ElideString(UTF8ToUTF16(cases[i].input),
                          cases[i].max_len, &output));
    EXPECT_EQ(cases[i].output, UTF16ToUTF8(output));
  }
}

TEST(TextEliderTest, ElideRectangleText) {
  const gfx::Font font;
  const int line_height = font.GetHeight();
  const int test_width = font.GetStringWidth(ASCIIToUTF16("Test"));

  struct TestData {
    const char* input;
    int available_pixel_width;
    int available_pixel_height;
    bool truncated_y;
    const char* output;
  } cases[] = {
    { "", 0, 0, false, NULL },
    { "", 1, 1, false, NULL },
    { "Test", test_width, 0, true, NULL },
    { "Test", test_width, 1, false, "Test" },
    { "Test", test_width, line_height, false, "Test" },
    { "Test Test", test_width, line_height, true, "Test" },
    { "Test Test", test_width, line_height + 1, false, "Test|Test" },
    { "Test Test", test_width, line_height * 2, false, "Test|Test" },
    { "Test Test", test_width, line_height * 3, false, "Test|Test" },
    { "Test Test", test_width * 2, line_height * 2, false, "Test|Test" },
    { "Test Test", test_width * 3, line_height, false, "Test Test" },
    { "Test\nTest", test_width * 3, line_height * 2, false, "Test|Test" },
    { "Te\nst Te", test_width, line_height * 3, false, "Te|st|Te" },
    { "\nTest", test_width, line_height * 2, false, "|Test" },
    { "\nTest", test_width, line_height, true, "" },
    { "\n\nTest", test_width, line_height * 3, false, "||Test" },
    { "\n\nTest", test_width, line_height * 2, true, "|" },
    { "Test\n", 2 * test_width, line_height * 5, false, "Test|" },
    { "Test\n\n", 2 * test_width, line_height * 5, false, "Test||" },
    { "Test\n\n\n", 2 * test_width, line_height * 5, false, "Test|||" },
    { "Test\nTest\n\n", 2 * test_width, line_height * 5, false, "Test|Test||" },
    { "Test\n\nTest\n", 2 * test_width, line_height * 5, false, "Test||Test|" },
    { "Test\n\n\nTest", 2 * test_width, line_height * 5, false, "Test|||Test" },
    { "Te ", test_width, line_height, false, "Te" },
    { "Te  Te Test", test_width, 3 * line_height, false, "Te|Te|Test" },
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    std::vector<string16> lines;
    EXPECT_EQ(cases[i].truncated_y ? INSUFFICIENT_SPACE_VERTICAL : 0,
              ElideRectangleText(UTF8ToUTF16(cases[i].input),
                                 font,
                                 cases[i].available_pixel_width,
                                 cases[i].available_pixel_height,
                                 TRUNCATE_LONG_WORDS,
                                 &lines));
    if (cases[i].output) {
      const std::string result = UTF16ToUTF8(JoinString(lines, '|'));
      EXPECT_EQ(cases[i].output, result) << "Case " << i << " failed!";
    } else {
      EXPECT_TRUE(lines.empty()) << "Case " << i << " failed!";
    }
  }
}

TEST(TextEliderTest, ElideRectangleTextPunctuation) {
  const gfx::Font font;
  const int line_height = font.GetHeight();
  const int test_width = font.GetStringWidth(ASCIIToUTF16("Test"));
  const int test_t_width = font.GetStringWidth(ASCIIToUTF16("Test T"));

  struct TestData {
    const char* input;
    int available_pixel_width;
    int available_pixel_height;
    bool wrap_words;
    bool truncated_x;
    const char* output;
  } cases[] = {
    { "Test T.", test_t_width, line_height * 2, false, false, "Test|T." },
    { "Test T ?", test_t_width, line_height * 2, false, false, "Test|T ?" },
    { "Test. Test", test_width, line_height * 3, false, true, "Test|Test" },
    { "Test. Test", test_width, line_height * 3, true, false, "Test|.|Test" },
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    std::vector<string16> lines;
    const WordWrapBehavior wrap_behavior =
        (cases[i].wrap_words ? WRAP_LONG_WORDS : TRUNCATE_LONG_WORDS);
    EXPECT_EQ(cases[i].truncated_x ? INSUFFICIENT_SPACE_HORIZONTAL : 0,
              ElideRectangleText(UTF8ToUTF16(cases[i].input),
                                 font,
                                 cases[i].available_pixel_width,
                                 cases[i].available_pixel_height,
                                 wrap_behavior,
                                 &lines));
    if (cases[i].output) {
      const std::string result = UTF16ToUTF8(JoinString(lines, '|'));
      EXPECT_EQ(cases[i].output, result) << "Case " << i << " failed!";
    } else {
      EXPECT_TRUE(lines.empty()) << "Case " << i << " failed!";
    }
  }
}

TEST(TextEliderTest, ElideRectangleTextLongWords) {
  const gfx::Font font;
  const int kAvailableHeight = 1000;
  const string16 kElidedTesting = UTF8ToUTF16(std::string("Tes") + kEllipsis);
  const int elided_width = font.GetStringWidth(kElidedTesting);
  const int test_width = font.GetStringWidth(ASCIIToUTF16("Test"));

  struct TestData {
    const char* input;
    int available_pixel_width;
    WordWrapBehavior wrap_behavior;
    bool truncated_x;
    const char* output;
  } cases[] = {
    { "Testing", test_width, IGNORE_LONG_WORDS, false, "Testing" },
    { "X Testing", test_width, IGNORE_LONG_WORDS, false, "X|Testing" },
    { "Test Testing", test_width, IGNORE_LONG_WORDS, false, "Test|Testing" },
    { "Test\nTesting", test_width, IGNORE_LONG_WORDS, false, "Test|Testing" },
    { "Test Tests ", test_width, IGNORE_LONG_WORDS, false, "Test|Tests" },
    { "Test Tests T", test_width, IGNORE_LONG_WORDS, false, "Test|Tests|T" },

    { "Testing", elided_width, ELIDE_LONG_WORDS, true, "Tes..." },
    { "X Testing", elided_width, ELIDE_LONG_WORDS, true, "X|Tes..." },
    { "Test Testing", elided_width, ELIDE_LONG_WORDS, true, "Test|Tes..." },
    { "Test\nTesting", elided_width, ELIDE_LONG_WORDS, true, "Test|Tes..." },

    { "Testing", test_width, TRUNCATE_LONG_WORDS, true, "Test" },
    { "X Testing", test_width, TRUNCATE_LONG_WORDS, true, "X|Test" },
    { "Test Testing", test_width, TRUNCATE_LONG_WORDS, true, "Test|Test" },
    { "Test\nTesting", test_width, TRUNCATE_LONG_WORDS, true, "Test|Test" },
    { "Test Tests ", test_width, TRUNCATE_LONG_WORDS, true, "Test|Test" },
    { "Test Tests T", test_width, TRUNCATE_LONG_WORDS, true, "Test|Test|T" },

    { "Testing", test_width, WRAP_LONG_WORDS, false, "Test|ing" },
    { "X Testing", test_width, WRAP_LONG_WORDS, false, "X|Test|ing" },
    { "Test Testing", test_width, WRAP_LONG_WORDS, false, "Test|Test|ing" },
    { "Test\nTesting", test_width, WRAP_LONG_WORDS, false, "Test|Test|ing" },
    { "Test Tests ", test_width, WRAP_LONG_WORDS, false, "Test|Test|s" },
    { "Test Tests T", test_width, WRAP_LONG_WORDS, false, "Test|Test|s T" },
    { "TestTestTest", test_width, WRAP_LONG_WORDS, false, "Test|Test|Test" },
    { "TestTestTestT", test_width, WRAP_LONG_WORDS, false, "Test|Test|Test|T" },
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    std::vector<string16> lines;
    EXPECT_EQ(cases[i].truncated_x ? INSUFFICIENT_SPACE_HORIZONTAL : 0,
              ElideRectangleText(UTF8ToUTF16(cases[i].input),
                                 font,
                                 cases[i].available_pixel_width,
                                 kAvailableHeight,
                                 cases[i].wrap_behavior,
                                 &lines));
    std::string expected_output(cases[i].output);
    ReplaceSubstringsAfterOffset(&expected_output, 0, "...", kEllipsis);
    const std::string result = UTF16ToUTF8(JoinString(lines, '|'));
    EXPECT_EQ(expected_output, result) << "Case " << i << " failed!";
  }
}

TEST(TextEliderTest, ElideRectangleString) {
  struct TestData {
    const char* input;
    int max_rows;
    int max_cols;
    bool result;
    const char* output;
  } cases[] = {
    { "", 0, 0, false, "" },
    { "", 1, 1, false, "" },
    { "Hi, my name is\nTom", 0, 0,  true,  "..." },
    { "Hi, my name is\nTom", 1, 0,  true,  "\n..." },
    { "Hi, my name is\nTom", 0, 1,  true,  "..." },
    { "Hi, my name is\nTom", 1, 1,  true,  "H\n..." },
    { "Hi, my name is\nTom", 2, 1,  true,  "H\ni\n..." },
    { "Hi, my name is\nTom", 3, 1,  true,  "H\ni\n,\n..." },
    { "Hi, my name is\nTom", 4, 1,  true,  "H\ni\n,\n \n..." },
    { "Hi, my name is\nTom", 5, 1,  true,  "H\ni\n,\n \nm\n..." },
    { "Hi, my name is\nTom", 0, 2,  true,  "..." },
    { "Hi, my name is\nTom", 1, 2,  true,  "Hi\n..." },
    { "Hi, my name is\nTom", 2, 2,  true,  "Hi\n, \n..." },
    { "Hi, my name is\nTom", 3, 2,  true,  "Hi\n, \nmy\n..." },
    { "Hi, my name is\nTom", 4, 2,  true,  "Hi\n, \nmy\n n\n..." },
    { "Hi, my name is\nTom", 5, 2,  true,  "Hi\n, \nmy\n n\nam\n..." },
    { "Hi, my name is\nTom", 0, 3,  true,  "..." },
    { "Hi, my name is\nTom", 1, 3,  true,  "Hi,\n..." },
    { "Hi, my name is\nTom", 2, 3,  true,  "Hi,\n my\n..." },
    { "Hi, my name is\nTom", 3, 3,  true,  "Hi,\n my\n na\n..." },
    { "Hi, my name is\nTom", 4, 3,  true,  "Hi,\n my\n na\nme \n..." },
    { "Hi, my name is\nTom", 5, 3,  true,  "Hi,\n my\n na\nme \nis\n..." },
    { "Hi, my name is\nTom", 1, 4,  true,  "Hi, \n..." },
    { "Hi, my name is\nTom", 2, 4,  true,  "Hi, \nmy n\n..." },
    { "Hi, my name is\nTom", 3, 4,  true,  "Hi, \nmy n\name \n..." },
    { "Hi, my name is\nTom", 4, 4,  true,  "Hi, \nmy n\name \nis\n..." },
    { "Hi, my name is\nTom", 5, 4,  false, "Hi, \nmy n\name \nis\nTom" },
    { "Hi, my name is\nTom", 1, 5,  true,  "Hi, \n..." },
    { "Hi, my name is\nTom", 2, 5,  true,  "Hi, \nmy na\n..." },
    { "Hi, my name is\nTom", 3, 5,  true,  "Hi, \nmy na\nme \n..." },
    { "Hi, my name is\nTom", 4, 5,  true,  "Hi, \nmy na\nme \nis\n..." },
    { "Hi, my name is\nTom", 5, 5,  false, "Hi, \nmy na\nme \nis\nTom" },
    { "Hi, my name is\nTom", 1, 6,  true,  "Hi, \n..." },
    { "Hi, my name is\nTom", 2, 6,  true,  "Hi, \nmy \n..." },
    { "Hi, my name is\nTom", 3, 6,  true,  "Hi, \nmy \nname \n..." },
    { "Hi, my name is\nTom", 4, 6,  true,  "Hi, \nmy \nname \nis\n..." },
    { "Hi, my name is\nTom", 5, 6,  false, "Hi, \nmy \nname \nis\nTom" },
    { "Hi, my name is\nTom", 1, 7,  true,  "Hi, \n..." },
    { "Hi, my name is\nTom", 2, 7,  true,  "Hi, \nmy \n..." },
    { "Hi, my name is\nTom", 3, 7,  true,  "Hi, \nmy \nname \n..." },
    { "Hi, my name is\nTom", 4, 7,  true,  "Hi, \nmy \nname \nis\n..." },
    { "Hi, my name is\nTom", 5, 7,  false, "Hi, \nmy \nname \nis\nTom" },
    { "Hi, my name is\nTom", 1, 8,  true,  "Hi, my \n..." },
    { "Hi, my name is\nTom", 2, 8,  true,  "Hi, my \nname \n..." },
    { "Hi, my name is\nTom", 3, 8,  true,  "Hi, my \nname \nis\n..." },
    { "Hi, my name is\nTom", 4, 8,  false, "Hi, my \nname \nis\nTom" },
    { "Hi, my name is\nTom", 1, 9,  true,  "Hi, my \n..." },
    { "Hi, my name is\nTom", 2, 9,  true,  "Hi, my \nname is\n..." },
    { "Hi, my name is\nTom", 3, 9,  false, "Hi, my \nname is\nTom" },
    { "Hi, my name is\nTom", 1, 10, true,  "Hi, my \n..." },
    { "Hi, my name is\nTom", 2, 10, true,  "Hi, my \nname is\n..." },
    { "Hi, my name is\nTom", 3, 10, false, "Hi, my \nname is\nTom" },
    { "Hi, my name is\nTom", 1, 11, true,  "Hi, my \n..." },
    { "Hi, my name is\nTom", 2, 11, true,  "Hi, my \nname is\n..." },
    { "Hi, my name is\nTom", 3, 11, false, "Hi, my \nname is\nTom" },
    { "Hi, my name is\nTom", 1, 12, true,  "Hi, my \n..." },
    { "Hi, my name is\nTom", 2, 12, true,  "Hi, my \nname is\n..." },
    { "Hi, my name is\nTom", 3, 12, false, "Hi, my \nname is\nTom" },
    { "Hi, my name is\nTom", 1, 13, true,  "Hi, my name \n..." },
    { "Hi, my name is\nTom", 2, 13, true,  "Hi, my name \nis\n..." },
    { "Hi, my name is\nTom", 3, 13, false, "Hi, my name \nis\nTom" },
    { "Hi, my name is\nTom", 1, 20, true,  "Hi, my name is\n..." },
    { "Hi, my name is\nTom", 2, 20, false, "Hi, my name is\nTom" },
    { "Hi, my name is Tom",  1, 40, false, "Hi, my name is Tom" },
  };
  string16 output;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    EXPECT_EQ(cases[i].result,
              ElideRectangleString(UTF8ToUTF16(cases[i].input),
                                   cases[i].max_rows, cases[i].max_cols,
                                   true, &output));
    EXPECT_EQ(cases[i].output, UTF16ToUTF8(output));
  }
}

TEST(TextEliderTest, ElideRectangleStringNotStrict) {
  struct TestData {
    const char* input;
    int max_rows;
    int max_cols;
    bool result;
    const char* output;
  } cases[] = {
    { "", 0, 0, false, "" },
    { "", 1, 1, false, "" },
    { "Hi, my name_is\nDick", 0, 0,  true,  "..." },
    { "Hi, my name_is\nDick", 1, 0,  true,  "\n..." },
    { "Hi, my name_is\nDick", 0, 1,  true,  "..." },
    { "Hi, my name_is\nDick", 1, 1,  true,  "H\n..." },
    { "Hi, my name_is\nDick", 2, 1,  true,  "H\ni\n..." },
    { "Hi, my name_is\nDick", 3, 1,  true,  "H\ni\n,\n..." },
    { "Hi, my name_is\nDick", 4, 1,  true,  "H\ni\n,\n \n..." },
    { "Hi, my name_is\nDick", 5, 1,  true,  "H\ni\n,\n \nm\n..." },
    { "Hi, my name_is\nDick", 0, 2,  true,  "..." },
    { "Hi, my name_is\nDick", 1, 2,  true,  "Hi\n..." },
    { "Hi, my name_is\nDick", 2, 2,  true,  "Hi\n, \n..." },
    { "Hi, my name_is\nDick", 3, 2,  true,  "Hi\n, \nmy\n..." },
    { "Hi, my name_is\nDick", 4, 2,  true,  "Hi\n, \nmy\n n\n..." },
    { "Hi, my name_is\nDick", 5, 2,  true,  "Hi\n, \nmy\n n\nam\n..." },
    { "Hi, my name_is\nDick", 0, 3,  true,  "..." },
    { "Hi, my name_is\nDick", 1, 3,  true,  "Hi,\n..." },
    { "Hi, my name_is\nDick", 2, 3,  true,  "Hi,\n my\n..." },
    { "Hi, my name_is\nDick", 3, 3,  true,  "Hi,\n my\n na\n..." },
    { "Hi, my name_is\nDick", 4, 3,  true,  "Hi,\n my\n na\nme_\n..." },
    { "Hi, my name_is\nDick", 5, 3,  true,  "Hi,\n my\n na\nme_\nis\n..." },
    { "Hi, my name_is\nDick", 1, 4,  true,  "Hi, ..." },
    { "Hi, my name_is\nDick", 2, 4,  true,  "Hi, my n\n..." },
    { "Hi, my name_is\nDick", 3, 4,  true,  "Hi, my n\name_\n..." },
    { "Hi, my name_is\nDick", 4, 4,  true,  "Hi, my n\name_\nis\n..." },
    { "Hi, my name_is\nDick", 5, 4,  false, "Hi, my n\name_\nis\nDick" },
    { "Hi, my name_is\nDick", 1, 5,  true,  "Hi, ..." },
    { "Hi, my name_is\nDick", 2, 5,  true,  "Hi, my na\n..." },
    { "Hi, my name_is\nDick", 3, 5,  true,  "Hi, my na\nme_is\n..." },
    { "Hi, my name_is\nDick", 4, 5,  true,  "Hi, my na\nme_is\n\n..." },
    { "Hi, my name_is\nDick", 5, 5,  false, "Hi, my na\nme_is\n\nDick" },
    { "Hi, my name_is\nDick", 1, 6,  true,  "Hi, ..." },
    { "Hi, my name_is\nDick", 2, 6,  true,  "Hi, my nam\n..." },
    { "Hi, my name_is\nDick", 3, 6,  true,  "Hi, my nam\ne_is\n..." },
    { "Hi, my name_is\nDick", 4, 6,  false, "Hi, my nam\ne_is\nDick" },
    { "Hi, my name_is\nDick", 5, 6,  false, "Hi, my nam\ne_is\nDick" },
    { "Hi, my name_is\nDick", 1, 7,  true,  "Hi, ..." },
    { "Hi, my name_is\nDick", 2, 7,  true,  "Hi, my name\n..." },
    { "Hi, my name_is\nDick", 3, 7,  true,  "Hi, my name\n_is\n..." },
    { "Hi, my name_is\nDick", 4, 7,  false, "Hi, my name\n_is\nDick" },
    { "Hi, my name_is\nDick", 5, 7,  false, "Hi, my name\n_is\nDick" },
    { "Hi, my name_is\nDick", 1, 8,  true,  "Hi, my n\n..." },
    { "Hi, my name_is\nDick", 2, 8,  true,  "Hi, my n\name_is\n..." },
    { "Hi, my name_is\nDick", 3, 8,  false, "Hi, my n\name_is\nDick" },
    { "Hi, my name_is\nDick", 1, 9,  true,  "Hi, my ..." },
    { "Hi, my name_is\nDick", 2, 9,  true,  "Hi, my name_is\n..." },
    { "Hi, my name_is\nDick", 3, 9,  false, "Hi, my name_is\nDick" },
    { "Hi, my name_is\nDick", 1, 10, true,  "Hi, my ..." },
    { "Hi, my name_is\nDick", 2, 10, true,  "Hi, my name_is\n..." },
    { "Hi, my name_is\nDick", 3, 10, false, "Hi, my name_is\nDick" },
    { "Hi, my name_is\nDick", 1, 11, true,  "Hi, my ..." },
    { "Hi, my name_is\nDick", 2, 11, true,  "Hi, my name_is\n..." },
    { "Hi, my name_is\nDick", 3, 11, false, "Hi, my name_is\nDick" },
    { "Hi, my name_is\nDick", 1, 12, true,  "Hi, my ..." },
    { "Hi, my name_is\nDick", 2, 12, true,  "Hi, my name_is\n..." },
    { "Hi, my name_is\nDick", 3, 12, false, "Hi, my name_is\nDick" },
    { "Hi, my name_is\nDick", 1, 13, true,  "Hi, my ..." },
    { "Hi, my name_is\nDick", 2, 13, true,  "Hi, my name_is\n..." },
    { "Hi, my name_is\nDick", 3, 13, false, "Hi, my name_is\nDick" },
    { "Hi, my name_is\nDick", 1, 20, true,  "Hi, my name_is\n..." },
    { "Hi, my name_is\nDick", 2, 20, false, "Hi, my name_is\nDick" },
    { "Hi, my name_is Dick",  1, 40, false, "Hi, my name_is Dick" },
  };
  string16 output;
  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    EXPECT_EQ(cases[i].result,
              ElideRectangleString(UTF8ToUTF16(cases[i].input),
                                   cases[i].max_rows, cases[i].max_cols,
                                   false, &output));
    EXPECT_EQ(cases[i].output, UTF16ToUTF8(output));
  }
}

TEST(TextEliderTest, ElideRectangleWide16) {
  // Two greek words separated by space.
  const string16 str(WideToUTF16(
      L"\x03a0\x03b1\x03b3\x03ba\x03cc\x03c3\x03bc\x03b9"
      L"\x03bf\x03c2\x0020\x0399\x03c3\x03c4\x03cc\x03c2"));
  const string16 out1(WideToUTF16(
      L"\x03a0\x03b1\x03b3\x03ba\n"
      L"\x03cc\x03c3\x03bc\x03b9\n"
      L"..."));
  const string16 out2(WideToUTF16(
      L"\x03a0\x03b1\x03b3\x03ba\x03cc\x03c3\x03bc\x03b9\x03bf\x03c2\x0020\n"
      L"\x0399\x03c3\x03c4\x03cc\x03c2"));
  string16 output;
  EXPECT_TRUE(ElideRectangleString(str, 2, 4, true, &output));
  EXPECT_EQ(out1, output);
  EXPECT_FALSE(ElideRectangleString(str, 2, 12, true, &output));
  EXPECT_EQ(out2, output);
}

TEST(TextEliderTest, ElideRectangleWide32) {
  // Four U+1D49C MATHEMATICAL SCRIPT CAPITAL A followed by space "aaaaa".
  const string16 str(UTF8ToUTF16(
      "\xF0\x9D\x92\x9C\xF0\x9D\x92\x9C\xF0\x9D\x92\x9C\xF0\x9D\x92\x9C"
      " aaaaa"));
  const string16 out(UTF8ToUTF16(
      "\xF0\x9D\x92\x9C\xF0\x9D\x92\x9C\xF0\x9D\x92\x9C\n"
      "\xF0\x9D\x92\x9C \naaa\n..."));
  string16 output;
  EXPECT_TRUE(ElideRectangleString(str, 3, 3, true, &output));
  EXPECT_EQ(out, output);
}

TEST(TextEliderTest, TruncateString) {
  string16 string = ASCIIToUTF16("foooooey    bxxxar baz");

  // Make sure it doesn't modify the string if length > string length.
  EXPECT_EQ(string, TruncateString(string, 100));

  // Test no characters.
  EXPECT_EQ(L"", UTF16ToWide(TruncateString(string, 0)));

  // Test 1 character.
  EXPECT_EQ(L"\x2026", UTF16ToWide(TruncateString(string, 1)));

  // Test adds ... at right spot when there is enough room to break at a
  // word boundary.
  EXPECT_EQ(L"foooooey\x2026", UTF16ToWide(TruncateString(string, 14)));

  // Test adds ... at right spot when there is not enough space in first word.
  EXPECT_EQ(L"f\x2026", UTF16ToWide(TruncateString(string, 2)));

  // Test adds ... at right spot when there is not enough room to break at a
  // word boundary.
  EXPECT_EQ(L"foooooey\x2026", UTF16ToWide(TruncateString(string, 11)));

  // Test completely truncates string if break is on initial whitespace.
  EXPECT_EQ(L"\x2026", UTF16ToWide(TruncateString(ASCIIToUTF16("   "), 2)));
}

}  // namespace ui
