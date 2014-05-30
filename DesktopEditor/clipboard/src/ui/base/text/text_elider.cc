// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// This file implements utility functions for eliding and formatting UI text.
//
// Note that several of the functions declared in text_elider.h are implemented
// in this file using helper classes in an unnamed namespace.

#include "ui/base/text/text_elider.h"

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/i18n/break_iterator.h"
#include "base/i18n/char_iterator.h"
#include "base/i18n/rtl.h"
#include "base/memory/scoped_ptr.h"
#include "base/string_util.h"
#include "base/strings/string_split.h"
#include "base/strings/sys_string_conversions.h"
#include "base/utf_string_conversions.h"
#include "googleurl/src/gurl.h"
#include "net/base/escape.h"
#include "net/base/net_util.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "third_party/icu/public/common/unicode/rbbi.h"
#include "third_party/icu/public/common/unicode/uloc.h"
#include "ui/gfx/font.h"

namespace ui {

// U+2026 in utf8
const char kEllipsis[] = "\xE2\x80\xA6";
const char16 kForwardSlash = '/';

namespace {

// Helper class to split + elide text, while respecting UTF16 surrogate pairs.
class StringSlicer {
 public:
  StringSlicer(const string16& text,
               const string16& ellipsis,
               bool elide_in_middle)
      : text_(text),
        ellipsis_(ellipsis),
        elide_in_middle_(elide_in_middle) {
  }

  // Cuts |text_| to be |length| characters long. If |elide_in_middle_| is true,
  // the middle of the string is removed to leave equal-length pieces from the
  // beginning and end of the string; otherwise, the end of the string is
  // removed and only the beginning remains. If |insert_ellipsis| is true,
  // then an ellipsis character will be inserted at the cut point.
  string16 CutString(size_t length, bool insert_ellipsis) {
    const string16 ellipsis_text = insert_ellipsis ? ellipsis_ : string16();

    if (!elide_in_middle_)
      return text_.substr(0, FindValidBoundaryBefore(length)) + ellipsis_text;

    // We put the extra character, if any, before the cut.
    const size_t half_length = length / 2;
    const size_t prefix_length = FindValidBoundaryBefore(length - half_length);
    const size_t suffix_start_guess = text_.length() - half_length;
    const size_t suffix_start = FindValidBoundaryAfter(suffix_start_guess);
    const size_t suffix_length =
        half_length - (suffix_start_guess - suffix_start);
    return text_.substr(0, prefix_length) + ellipsis_text +
           text_.substr(suffix_start, suffix_length);
  }

 private:
  // Returns a valid cut boundary at or before |index|.
  size_t FindValidBoundaryBefore(size_t index) const {
    DCHECK_LE(index, text_.length());
    if (index != text_.length())
      U16_SET_CP_START(text_.data(), 0, index);
    return index;
  }

  // Returns a valid cut boundary at or after |index|.
  size_t FindValidBoundaryAfter(size_t index) const {
    DCHECK_LE(index, text_.length());
    if (index != text_.length())
      U16_SET_CP_LIMIT(text_.data(), 0, index, text_.length());
    return index;
  }

  // The text to be sliced.
  const string16& text_;

  // Ellipsis string to use.
  const string16& ellipsis_;

  // If true, the middle of the string will be elided.
  bool elide_in_middle_;

  DISALLOW_COPY_AND_ASSIGN(StringSlicer);
};

// Build a path from the first |num_components| elements in |path_elements|.
// Prepends |path_prefix|, appends |filename|, inserts ellipsis if appropriate.
string16 BuildPathFromComponents(const string16& path_prefix,
                                 const std::vector<string16>& path_elements,
                                 const string16& filename,
                                 size_t num_components) {
  const string16 kEllipsisAndSlash = UTF8ToUTF16(kEllipsis) + kForwardSlash;

  // Add the initial elements of the path.
  string16 path = path_prefix;

  // Build path from first |num_components| elements.
  for (size_t j = 0; j < num_components; ++j)
    path += path_elements[j] + kForwardSlash;

  // Add |filename|, ellipsis if necessary.
  if (num_components != (path_elements.size() - 1))
    path += kEllipsisAndSlash;
  path += filename;

  return path;
}

// Takes a prefix (Domain, or Domain+subdomain) and a collection of path
// components and elides if possible. Returns a string containing the longest
// possible elided path, or an empty string if elision is not possible.
string16 ElideComponentizedPath(const string16& url_path_prefix,
                                const std::vector<string16>& url_path_elements,
                                const string16& url_filename,
                                const string16& url_query,
                                const gfx::Font& font,
                                int available_pixel_width) {
  const size_t url_path_number_of_elements = url_path_elements.size();

  const string16 kEllipsisAndSlash = UTF8ToUTF16(kEllipsis) + kForwardSlash;

  CHECK(url_path_number_of_elements);
  for (size_t i = url_path_number_of_elements - 1; i > 0; --i) {
    string16 elided_path = BuildPathFromComponents(url_path_prefix,
        url_path_elements, url_filename, i);
    if (available_pixel_width >= font.GetStringWidth(elided_path))
      return ElideText(elided_path + url_query,
                       font, available_pixel_width, ELIDE_AT_END);
  }

  return string16();
}

}  // namespace

string16 ElideEmail(const string16& email,
                    const gfx::Font& font,
                    int available_pixel_width) {
  if (font.GetStringWidth(email) <= available_pixel_width)
    return email;

  // Split the email into its local-part (username) and domain-part. The email
  // spec technically allows for @ symbols in the local-part (username) of the
  // email under some special requirements. It is guaranteed that there is no @
  // symbol in the domain part of the email however so splitting at the last @
  // symbol is safe.
  const size_t split_index = email.find_last_of('@');
  DCHECK_NE(split_index, string16::npos);
  string16 username = email.substr(0, split_index);
  string16 domain = email.substr(split_index + 1);
  DCHECK(!username.empty());
  DCHECK(!domain.empty());

  const string16 kEllipsisUTF16 = UTF8ToUTF16(kEllipsis);

  // Subtract the @ symbol from the available width as it is mandatory.
  const string16 kAtSignUTF16 = ASCIIToUTF16("@");
  available_pixel_width -= font.GetStringWidth(kAtSignUTF16);

  // Check whether eliding the domain is necessary: if eliding the username
  // is sufficient, the domain will not be elided.
  const int full_username_width = font.GetStringWidth(username);
  const int available_domain_width =
      available_pixel_width -
      std::min(full_username_width,
               font.GetStringWidth(username.substr(0, 1) + kEllipsisUTF16));
  if (font.GetStringWidth(domain) > available_domain_width) {
    // Elide the domain so that it only takes half of the available width.
    // Should the username not need all the width available in its half, the
    // domain will occupy the leftover width.
    // If |desired_domain_width| is greater than |available_domain_width|: the
    // minimal username elision allowed by the specifications will not fit; thus
    // |desired_domain_width| must be <= |available_domain_width| at all cost.
    const int desired_domain_width =
        std::min(available_domain_width,
                 std::max(available_pixel_width - full_username_width,
                          available_pixel_width / 2));
    domain = ElideText(domain, font, desired_domain_width, ELIDE_IN_MIDDLE);
    // Failing to elide the domain such that at least one character remains
    // (other than the ellipsis itself) remains: return a single ellipsis.
    if (domain.length() <= 1U)
      return kEllipsisUTF16;
  }

  // Fit the username in the remaining width (at this point the elided username
  // is guaranteed to fit with at least one character remaining given all the
  // precautions taken earlier).
  username = ElideText(username,
                       font,
                       available_pixel_width - font.GetStringWidth(domain),
                       ELIDE_AT_END);

  return username + kAtSignUTF16 + domain;
}

// TODO(pkasting): http://crbug.com/77883 This whole function gets
// kerning/ligatures/etc. issues potentially wrong by assuming that the width of
// a rendered string is always the sum of the widths of its substrings.  Also I
// suspect it could be made simpler.
string16 ElideUrl(const GURL& url,
                  const gfx::Font& font,
                  int available_pixel_width,
                  const std::string& languages) {
  // Get a formatted string and corresponding parsing of the url.
  url_parse::Parsed parsed;
  const string16 url_string =
      net::FormatUrl(url, languages, net::kFormatUrlOmitAll,
                     net::UnescapeRule::SPACES, &parsed, NULL, NULL);
  if (available_pixel_width <= 0)
    return url_string;

  // If non-standard, return plain eliding.
  if (!url.IsStandard())
    return ElideText(url_string, font, available_pixel_width, ELIDE_AT_END);

  // Now start eliding url_string to fit within available pixel width.
  // Fist pass - check to see whether entire url_string fits.
  const int pixel_width_url_string = font.GetStringWidth(url_string);
  if (available_pixel_width >= pixel_width_url_string)
    return url_string;

  // Get the path substring, including query and reference.
  const size_t path_start_index = parsed.path.begin;
  const size_t path_len = parsed.path.len;
  string16 url_path_query_etc = url_string.substr(path_start_index);
  string16 url_path = url_string.substr(path_start_index, path_len);

  // Return general elided text if url minus the query fits.
  const string16 url_minus_query =
      url_string.substr(0, path_start_index + path_len);
  if (available_pixel_width >= font.GetStringWidth(url_minus_query))
    return ElideText(url_string, font, available_pixel_width, ELIDE_AT_END);

  // Get Host.
  string16 url_host = UTF8ToUTF16(url.host());

  // Get domain and registry information from the URL.
  string16 url_domain = UTF8ToUTF16(
      net::RegistryControlledDomainService::GetDomainAndRegistry(url));
  if (url_domain.empty())
    url_domain = url_host;

  // Add port if required.
  if (!url.port().empty()) {
    url_host += UTF8ToUTF16(":" + url.port());
    url_domain += UTF8ToUTF16(":" + url.port());
  }

  // Get sub domain.
  string16 url_subdomain;
  const size_t domain_start_index = url_host.find(url_domain);
  if (domain_start_index != string16::npos)
    url_subdomain = url_host.substr(0, domain_start_index);
  const string16 kWwwPrefix = UTF8ToUTF16("www.");
  if ((url_subdomain == kWwwPrefix || url_subdomain.empty() ||
      url.SchemeIsFile())) {
    url_subdomain.clear();
  }

  // If this is a file type, the path is now defined as everything after ":".
  // For example, "C:/aa/aa/bb", the path is "/aa/bb/cc". Interesting, the
  // domain is now C: - this is a nice hack for eliding to work pleasantly.
  if (url.SchemeIsFile()) {
    // Split the path string using ":"
    std::vector<string16> file_path_split;
    base::SplitString(url_path, ':', &file_path_split);
    if (file_path_split.size() > 1) {  // File is of type "file:///C:/.."
      url_host.clear();
      url_domain.clear();
      url_subdomain.clear();

      const string16 kColon = UTF8ToUTF16(":");
      url_host = url_domain = file_path_split.at(0).substr(1) + kColon;
      url_path_query_etc = url_path = file_path_split.at(1);
    }
  }

  // Second Pass - remove scheme - the rest fits.
  const int pixel_width_url_host = font.GetStringWidth(url_host);
  const int pixel_width_url_path = font.GetStringWidth(url_path_query_etc);
  if (available_pixel_width >=
      pixel_width_url_host + pixel_width_url_path)
    return url_host + url_path_query_etc;

  // Third Pass: Subdomain, domain and entire path fits.
  const int pixel_width_url_domain = font.GetStringWidth(url_domain);
  const int pixel_width_url_subdomain = font.GetStringWidth(url_subdomain);
  if (available_pixel_width >=
      pixel_width_url_subdomain + pixel_width_url_domain +
      pixel_width_url_path)
    return url_subdomain + url_domain + url_path_query_etc;

  // Query element.
  string16 url_query;
  const int kPixelWidthDotsTrailer =
      font.GetStringWidth(UTF8ToUTF16(kEllipsis));
  if (parsed.query.is_nonempty()) {
    url_query = UTF8ToUTF16("?") + url_string.substr(parsed.query.begin);
    if (available_pixel_width >= (pixel_width_url_subdomain +
        pixel_width_url_domain + pixel_width_url_path -
        font.GetStringWidth(url_query))) {
      return ElideText(url_subdomain + url_domain + url_path_query_etc,
                       font, available_pixel_width, ELIDE_AT_END);
    }
  }

  // Parse url_path using '/'.
  std::vector<string16> url_path_elements;
  base::SplitString(url_path, kForwardSlash, &url_path_elements);

  // Get filename - note that for a path ending with /
  // such as www.google.com/intl/ads/, the file name is ads/.
  size_t url_path_number_of_elements = url_path_elements.size();
  DCHECK(url_path_number_of_elements != 0);
  string16 url_filename;
  if ((url_path_elements.at(url_path_number_of_elements - 1)).length() > 0) {
    url_filename = *(url_path_elements.end() - 1);
  } else if (url_path_number_of_elements > 1) {  // Path ends with a '/'.
    url_filename = url_path_elements.at(url_path_number_of_elements - 2) +
        kForwardSlash;
    url_path_number_of_elements--;
  }
  DCHECK(url_path_number_of_elements != 0);

  const size_t kMaxNumberOfUrlPathElementsAllowed = 1024;
  if (url_path_number_of_elements <= 1 ||
      url_path_number_of_elements > kMaxNumberOfUrlPathElementsAllowed) {
    // No path to elide, or too long of a path (could overflow in loop below)
    // Just elide this as a text string.
    return ElideText(url_subdomain + url_domain + url_path_query_etc, font,
                     available_pixel_width, ELIDE_AT_END);
  }

  // Start eliding the path and replacing elements by ".../".
  const string16 kEllipsisAndSlash = UTF8ToUTF16(kEllipsis) + kForwardSlash;
  const int pixel_width_ellipsis_slash = font.GetStringWidth(kEllipsisAndSlash);

  // Check with both subdomain and domain.
  string16 elided_path =
      ElideComponentizedPath(url_subdomain + url_domain, url_path_elements,
                             url_filename, url_query, font,
                             available_pixel_width);
  if (!elided_path.empty())
    return elided_path;

  // Check with only domain.
  // If a subdomain is present, add an ellipsis before domain.
  // This is added only if the subdomain pixel width is larger than
  // the pixel width of kEllipsis. Otherwise, subdomain remains,
  // which means that this case has been resolved earlier.
  string16 url_elided_domain = url_subdomain + url_domain;
  if (pixel_width_url_subdomain > kPixelWidthDotsTrailer) {
    if (!url_subdomain.empty()) {
      url_elided_domain = kEllipsisAndSlash[0] + url_domain;
    } else {
      url_elided_domain = url_domain;
    }

    elided_path = ElideComponentizedPath(url_elided_domain, url_path_elements,
                                         url_filename, url_query, font,
                                         available_pixel_width);

    if (!elided_path.empty())
      return elided_path;
  }

  // Return elided domain/.../filename anyway.
  string16 final_elided_url_string(url_elided_domain);
  const int url_elided_domain_width = font.GetStringWidth(url_elided_domain);

  // A hack to prevent trailing ".../...".
  if ((available_pixel_width - url_elided_domain_width) >
      pixel_width_ellipsis_slash + kPixelWidthDotsTrailer +
      font.GetStringWidth(ASCIIToUTF16("UV"))) {
    final_elided_url_string += BuildPathFromComponents(string16(),
        url_path_elements, url_filename, 1);
  } else {
    final_elided_url_string += url_path;
  }

  return ElideText(final_elided_url_string, font, available_pixel_width,
                   ELIDE_AT_END);
}

string16 ElideFilename(const base::FilePath& filename,
                       const gfx::Font& font,
                       int available_pixel_width) {
#if defined(OS_WIN)
  string16 filename_utf16 = filename.value();
  string16 extension = filename.Extension();
  string16 rootname = filename.BaseName().RemoveExtension().value();
#elif defined(OS_POSIX)
  string16 filename_utf16 = WideToUTF16(base::SysNativeMBToWide(
      filename.value()));
  string16 extension = WideToUTF16(base::SysNativeMBToWide(
      filename.Extension()));
  string16 rootname = WideToUTF16(base::SysNativeMBToWide(
      filename.BaseName().RemoveExtension().value()));
#endif

  const int full_width = font.GetStringWidth(filename_utf16);
  if (full_width <= available_pixel_width)
    return base::i18n::GetDisplayStringInLTRDirectionality(filename_utf16);

  if (rootname.empty() || extension.empty()) {
    const string16 elided_name = ElideText(filename_utf16, font,
                                           available_pixel_width, ELIDE_AT_END);
    return base::i18n::GetDisplayStringInLTRDirectionality(elided_name);
  }

  const int ext_width = font.GetStringWidth(extension);
  const int root_width = font.GetStringWidth(rootname);

  // We may have trimmed the path.
  if (root_width + ext_width <= available_pixel_width) {
    const string16 elided_name = rootname + extension;
    return base::i18n::GetDisplayStringInLTRDirectionality(elided_name);
  }

  if (ext_width >= available_pixel_width) {
    const string16 elided_name = ElideText(rootname + extension, font,
                                           available_pixel_width,
                                           ELIDE_IN_MIDDLE);
    return base::i18n::GetDisplayStringInLTRDirectionality(elided_name);
  }

  int available_root_width = available_pixel_width - ext_width;
  string16 elided_name =
      ElideText(rootname, font, available_root_width, ELIDE_AT_END);
  elided_name += extension;
  return base::i18n::GetDisplayStringInLTRDirectionality(elided_name);
}

string16 ElideText(const string16& text,
                   const gfx::Font& font,
                   int available_pixel_width,
                   ElideBehavior elide_behavior) {
  if (text.empty())
    return text;

  const string16 kEllipsisUTF16 = UTF8ToUTF16(kEllipsis);

  const int current_text_pixel_width = font.GetStringWidth(text);
  const bool elide_in_middle = (elide_behavior == ELIDE_IN_MIDDLE);
  const bool insert_ellipsis = (elide_behavior != TRUNCATE_AT_END);

  StringSlicer slicer(text, kEllipsisUTF16, elide_in_middle);

  // Pango will return 0 width for absurdly long strings. Cut the string in
  // half and try again.
  // This is caused by an int overflow in Pango (specifically, in
  // pango_glyph_string_extents_range). It's actually more subtle than just
  // returning 0, since on super absurdly long strings, the int can wrap and
  // return positive numbers again. Detecting that is probably not worth it
  // (eliding way too much from a ridiculous string is probably still
  // ridiculous), but we should check other widths for bogus values as well.
  if (current_text_pixel_width <= 0 && !text.empty()) {
    const string16 cut = slicer.CutString(text.length() / 2, false);
    return ElideText(cut, font, available_pixel_width, elide_behavior);
  }

  if (current_text_pixel_width <= available_pixel_width)
    return text;

  if (insert_ellipsis &&
      font.GetStringWidth(kEllipsisUTF16) > available_pixel_width)
    return string16();

  // Use binary search to compute the elided text.
  size_t lo = 0;
  size_t hi = text.length() - 1;
  size_t guess;
  for (guess = (lo + hi) / 2; lo <= hi; guess = (lo + hi) / 2) {
    // We check the length of the whole desired string at once to ensure we
    // handle kerning/ligatures/etc. correctly.
    const string16 cut = slicer.CutString(guess, insert_ellipsis);
    const int guess_length = font.GetStringWidth(cut);
    // Check again that we didn't hit a Pango width overflow. If so, cut the
    // current string in half and start over.
    if (guess_length <= 0) {
      return ElideText(slicer.CutString(guess / 2, false),
                       font, available_pixel_width, elide_behavior);
    }
    if (guess_length > available_pixel_width)
      hi = guess - 1;
    else
      lo = guess + 1;
  }

  return slicer.CutString(guess, insert_ellipsis);
}

SortedDisplayURL::SortedDisplayURL(const GURL& url,
                                   const std::string& languages) {
  net::AppendFormattedHost(url, languages, &sort_host_);
  string16 host_minus_www = net::StripWWW(sort_host_);
  url_parse::Parsed parsed;
  display_url_ =
      net::FormatUrl(url, languages, net::kFormatUrlOmitAll,
                     net::UnescapeRule::SPACES, &parsed, &prefix_end_, NULL);
  if (sort_host_.length() > host_minus_www.length()) {
    prefix_end_ += sort_host_.length() - host_minus_www.length();
    sort_host_.swap(host_minus_www);
  }
}

SortedDisplayURL::SortedDisplayURL() : prefix_end_(0) {
}

SortedDisplayURL::~SortedDisplayURL() {
}

int SortedDisplayURL::Compare(const SortedDisplayURL& other,
                              icu::Collator* collator) const {
  // Compare on hosts first. The host won't contain 'www.'.
  UErrorCode compare_status = U_ZERO_ERROR;
  UCollationResult host_compare_result = collator->compare(
      static_cast<const UChar*>(sort_host_.c_str()),
      static_cast<int>(sort_host_.length()),
      static_cast<const UChar*>(other.sort_host_.c_str()),
      static_cast<int>(other.sort_host_.length()),
      compare_status);
  DCHECK(U_SUCCESS(compare_status));
  if (host_compare_result != 0)
    return host_compare_result;

  // Hosts match, compare on the portion of the url after the host.
  string16 path = this->AfterHost();
  string16 o_path = other.AfterHost();
  compare_status = U_ZERO_ERROR;
  UCollationResult path_compare_result = collator->compare(
      static_cast<const UChar*>(path.c_str()),
      static_cast<int>(path.length()),
      static_cast<const UChar*>(o_path.c_str()),
      static_cast<int>(o_path.length()),
      compare_status);
  DCHECK(U_SUCCESS(compare_status));
  if (path_compare_result != 0)
    return path_compare_result;

  // Hosts and paths match, compare on the complete url. This'll push the www.
  // ones to the end.
  compare_status = U_ZERO_ERROR;
  UCollationResult display_url_compare_result = collator->compare(
      static_cast<const UChar*>(display_url_.c_str()),
      static_cast<int>(display_url_.length()),
      static_cast<const UChar*>(other.display_url_.c_str()),
      static_cast<int>(other.display_url_.length()),
      compare_status);
  DCHECK(U_SUCCESS(compare_status));
  return display_url_compare_result;
}

string16 SortedDisplayURL::AfterHost() const {
  const size_t slash_index = display_url_.find(sort_host_, prefix_end_);
  if (slash_index == string16::npos) {
    NOTREACHED();
    return string16();
  }
  return display_url_.substr(slash_index + sort_host_.length());
}

bool ElideString(const string16& input, int max_len, string16* output) {
  DCHECK_GE(max_len, 0);
  if (static_cast<int>(input.length()) <= max_len) {
    output->assign(input);
    return false;
  }

  switch (max_len) {
    case 0:
      output->clear();
      break;
    case 1:
      output->assign(input.substr(0, 1));
      break;
    case 2:
      output->assign(input.substr(0, 2));
      break;
    case 3:
      output->assign(input.substr(0, 1) + ASCIIToUTF16(".") +
                     input.substr(input.length() - 1));
      break;
    case 4:
      output->assign(input.substr(0, 1) + ASCIIToUTF16("..") +
                     input.substr(input.length() - 1));
      break;
    default: {
      int rstr_len = (max_len - 3) / 2;
      int lstr_len = rstr_len + ((max_len - 3) % 2);
      output->assign(input.substr(0, lstr_len) + ASCIIToUTF16("...") +
                     input.substr(input.length() - rstr_len));
      break;
    }
  }

  return true;
}

}  // namespace ui

namespace {

// Internal class used to track progress of a rectangular string elide
// operation.  Exists so the top-level ElideRectangleString() function
// can be broken into smaller methods sharing this state.
class RectangleString {
 public:
  RectangleString(size_t max_rows, size_t max_cols,
                  bool strict, string16 *output)
      : max_rows_(max_rows),
        max_cols_(max_cols),
        current_row_(0),
        current_col_(0),
        strict_(strict),
        suppressed_(false),
        output_(output) {}

  // Perform deferred initializations following creation.  Must be called
  // before any input can be added via AddString().
  void Init() { output_->clear(); }

  // Add an input string, reformatting to fit the desired dimensions.
  // AddString() may be called multiple times to concatenate together
  // multiple strings into the region (the current caller doesn't do
  // this, however).
  void AddString(const string16& input);

  // Perform any deferred output processing.  Must be called after the
  // last AddString() call has occurred.
  bool Finalize();

 private:
  // Add a line to the rectangular region at the current position,
  // either by itself or by breaking it into words.
  void AddLine(const string16& line);

  // Add a word to the rectangular region at the current position,
  // either by itself or by breaking it into characters.
  void AddWord(const string16& word);

  // Add text to the output string if the rectangular boundaries
  // have not been exceeded, advancing the current position.
  void Append(const string16& string);

  // Set the current position to the beginning of the next line.  If
  // |output| is true, add a newline to the output string if the rectangular
  // boundaries have not been exceeded.  If |output| is false, we assume
  // some other mechanism will (likely) do similar breaking after the fact.
  void NewLine(bool output);

  // Maximum number of rows allowed in the output string.
  size_t max_rows_;

  // Maximum number of characters allowed in the output string.
  size_t max_cols_;

  // Current row position, always incremented and may exceed max_rows_
  // when the input can not fit in the region.  We stop appending to
  // the output string, however, when this condition occurs.  In the
  // future, we may want to expose this value to allow the caller to
  // determine how many rows would actually be required to hold the
  // formatted string.
  size_t current_row_;

  // Current character position, should never exceed max_cols_.
  size_t current_col_;

  // True when we do whitespace to newline conversions ourselves.
  bool strict_;

  // True when some of the input has been truncated.
  bool suppressed_;

  // String onto which the output is accumulated.
  string16* output_;

  DISALLOW_COPY_AND_ASSIGN(RectangleString);
};

void RectangleString::AddString(const string16& input) {
  base::i18n::BreakIterator lines(input,
                                  base::i18n::BreakIterator::BREAK_NEWLINE);
  if (lines.Init()) {
    while (lines.Advance())
      AddLine(lines.GetString());
  } else {
    NOTREACHED() << "BreakIterator (lines) init failed";
  }
}

bool RectangleString::Finalize() {
  if (suppressed_) {
    output_->append(ASCIIToUTF16("..."));
    return true;
  }
  return false;
}

void RectangleString::AddLine(const string16& line) {
  if (line.length() < max_cols_) {
    Append(line);
  } else {
    base::i18n::BreakIterator words(line,
                                    base::i18n::BreakIterator::BREAK_SPACE);
    if (words.Init()) {
      while (words.Advance())
        AddWord(words.GetString());
    } else {
      NOTREACHED() << "BreakIterator (words) init failed";
    }
  }
  // Account for naturally-occuring newlines.
  ++current_row_;
  current_col_ = 0;
}

void RectangleString::AddWord(const string16& word) {
  if (word.length() < max_cols_) {
    // Word can be made to fit, no need to fragment it.
    if (current_col_ + word.length() >= max_cols_)
      NewLine(strict_);
    Append(word);
  } else {
    // Word is so big that it must be fragmented.
    int array_start = 0;
    int char_start = 0;
    base::i18n::UTF16CharIterator chars(&word);
    while (!chars.end()) {
      // When boundary is hit, add as much as will fit on this line.
      if (current_col_ + (chars.char_pos() - char_start) >= max_cols_) {
        Append(word.substr(array_start, chars.array_pos() - array_start));
        NewLine(true);
        array_start = chars.array_pos();
        char_start = chars.char_pos();
      }
      chars.Advance();
    }
    // Add the last remaining fragment, if any.
    if (array_start != chars.array_pos())
      Append(word.substr(array_start, chars.array_pos() - array_start));
  }
}

void RectangleString::Append(const string16& string) {
  if (current_row_ < max_rows_)
    output_->append(string);
  else
    suppressed_ = true;
  current_col_ += string.length();
}

void RectangleString::NewLine(bool output) {
  if (current_row_ < max_rows_) {
    if (output)
      output_->append(ASCIIToUTF16("\n"));
  } else {
    suppressed_ = true;
  }
  ++current_row_;
  current_col_ = 0;
}

// Internal class used to track progress of a rectangular text elide
// operation.  Exists so the top-level ElideRectangleText() function
// can be broken into smaller methods sharing this state.
class RectangleText {
 public:
  RectangleText(const gfx::Font& font,
                int available_pixel_width,
                int available_pixel_height,
                ui::WordWrapBehavior wrap_behavior,
                std::vector<string16>* lines)
      : font_(font),
        line_height_(font.GetHeight()),
        available_pixel_width_(available_pixel_width),
        available_pixel_height_(available_pixel_height),
        wrap_behavior_(wrap_behavior),
        current_width_(0),
        current_height_(0),
        last_line_ended_in_lf_(false),
        lines_(lines),
        insufficient_width_(false),
        insufficient_height_(false) {}

  // Perform deferred initializions following creation.  Must be called
  // before any input can be added via AddString().
  void Init() { lines_->clear(); }

  // Add an input string, reformatting to fit the desired dimensions.
  // AddString() may be called multiple times to concatenate together
  // multiple strings into the region (the current caller doesn't do
  // this, however).
  void AddString(const string16& input);

  // Perform any deferred output processing.  Must be called after the last
  // AddString() call has occured. Returns a combination of
  // |ReformattingResultFlags| indicating whether the given width or height was
  // insufficient, leading to elision or truncation.
  int Finalize();

 private:
  // Add a line to the rectangular region at the current position,
  // either by itself or by breaking it into words.
  void AddLine(const string16& line);

  // Wrap the specified word across multiple lines.
  int WrapWord(const string16& word);

  // Add a long word - wrapping, eliding or truncating per the wrap behavior.
  int AddWordOverflow(const string16& word);

  // Add a word to the rectangluar region at the current position.
  int AddWord(const string16& word);

  // Append the specified |text| to the current output line, incrementing the
  // running width by the specified amount. This is an optimization over
  // |AddToCurrentLine()| when |text_width| is already known.
  void AddToCurrentLineWithWidth(const string16& text, int text_width);

  // Append the specified |text| to the current output line.
  void AddToCurrentLine(const string16& text);

  // Set the current position to the beginning of the next line.
  bool NewLine();

  // The font used for measuring text width.
  const gfx::Font& font_;

  // The height of each line of text.
  const int line_height_;

  // The number of pixels of available width in the rectangle.
  const int available_pixel_width_;

  // The number of pixels of available height in the rectangle.
  const int available_pixel_height_;

  // The wrap behavior for words that are too long to fit on a single line.
  const ui::WordWrapBehavior wrap_behavior_;

  // The current running width.
  int current_width_;

  // The current running height.
  int current_height_;

  // The current line of text.
  string16 current_line_;

  // Indicates whether the last line ended with \n.
  bool last_line_ended_in_lf_;

  // The output vector of lines.
  std::vector<string16>* lines_;

  // Indicates whether a word was so long that it had to be truncated or elided
  // to fit the available width.
  bool insufficient_width_;

  // Indicates whether there were too many lines for the available height.
  bool insufficient_height_;

  DISALLOW_COPY_AND_ASSIGN(RectangleText);
};

void RectangleText::AddString(const string16& input) {
  base::i18n::BreakIterator lines(input,
                                  base::i18n::BreakIterator::BREAK_NEWLINE);
  if (lines.Init()) {
    while (!insufficient_height_ && lines.Advance()) {
      string16 line = lines.GetString();
      // The BREAK_NEWLINE iterator will keep the trailing newline character,
      // except in the case of the last line, which may not have one.  Remove
      // the newline character, if it exists.
      last_line_ended_in_lf_ = !line.empty() && line[line.length() - 1] == '\n';
      if (last_line_ended_in_lf_)
        line.resize(line.length() - 1);
      AddLine(line);
    }
  } else {
    NOTREACHED() << "BreakIterator (lines) init failed";
  }
}

int RectangleText::Finalize() {
  // Remove trailing whitespace from the last line or remove the last line
  // completely, if it's just whitespace.
  if (!insufficient_height_ && !lines_->empty()) {
    TrimWhitespace(lines_->back(), TRIM_TRAILING, &lines_->back());
    if (lines_->back().empty() && !last_line_ended_in_lf_)
      lines_->pop_back();
  }
  if (last_line_ended_in_lf_)
    lines_->push_back(string16());
  return (insufficient_width_ ? ui::INSUFFICIENT_SPACE_HORIZONTAL : 0) |
         (insufficient_height_ ? ui::INSUFFICIENT_SPACE_VERTICAL : 0);
}

void RectangleText::AddLine(const string16& line) {
  const int line_width = font_.GetStringWidth(line);
  if (line_width <= available_pixel_width_) {
    AddToCurrentLineWithWidth(line, line_width);
  } else {
    // Iterate over positions that are valid to break the line at. In general,
    // these are word boundaries but after any punctuation following the word.
    base::i18n::BreakIterator words(line,
                                    base::i18n::BreakIterator::BREAK_LINE);
    if (words.Init()) {
      while (words.Advance()) {
        const bool truncate = !current_line_.empty();
        const string16& word = words.GetString();
        const int lines_added = AddWord(word);
        if (lines_added) {
          if (truncate) {
            // Trim trailing whitespace from the line that was added.
            const int line = lines_->size() - lines_added;
            TrimWhitespace(lines_->at(line), TRIM_TRAILING, &lines_->at(line));
          }
          if (ContainsOnlyWhitespace(word)) {
            // Skip the first space if the previous line was carried over.
            current_width_ = 0;
            current_line_.clear();
          }
        }
      }
    } else {
      NOTREACHED() << "BreakIterator (words) init failed";
    }
  }
  // Account for naturally-occuring newlines.
  NewLine();
}

int RectangleText::WrapWord(const string16& word) {
  // Word is so wide that it must be fragmented.
  string16 text = word;
  int lines_added = 0;
  bool first_fragment = true;
  while (!insufficient_height_ && !text.empty()) {
    string16 fragment =
        ui::ElideText(text, font_, available_pixel_width_, ui::TRUNCATE_AT_END);
    // At least one character has to be added at every line, even if the
    // available space is too small.
    if(fragment.empty())
      fragment = text.substr(0, 1);
    if (!first_fragment && NewLine())
      lines_added++;
    AddToCurrentLine(fragment);
    text = text.substr(fragment.length());
    first_fragment = false;
  }
  return lines_added;
}

int RectangleText::AddWordOverflow(const string16& word) {
  int lines_added = 0;

  // Unless this is the very first word, put it on a new line.
  if (!current_line_.empty()) {
    if (!NewLine())
      return 0;
    lines_added++;
  }

  if (wrap_behavior_ == ui::IGNORE_LONG_WORDS) {
    current_line_ = word;
    current_width_ = available_pixel_width_;
  } else if (wrap_behavior_ == ui::WRAP_LONG_WORDS) {
    lines_added += WrapWord(word);
  } else {
    const ui::ElideBehavior elide_behavior =
        (wrap_behavior_ == ui::ELIDE_LONG_WORDS ? ui::ELIDE_AT_END :
                                                  ui::TRUNCATE_AT_END);
    const string16 elided_word =
        ui::ElideText(word, font_, available_pixel_width_, elide_behavior);
    AddToCurrentLine(elided_word);
    insufficient_width_ = true;
  }

  return lines_added;
}

int RectangleText::AddWord(const string16& word) {
  int lines_added = 0;
  string16 trimmed;
  TrimWhitespace(word, TRIM_TRAILING, &trimmed);
  const int trimmed_width = font_.GetStringWidth(trimmed);
  if (trimmed_width <= available_pixel_width_) {
    // Word can be made to fit, no need to fragment it.
    if ((current_width_ + trimmed_width > available_pixel_width_) && NewLine())
      lines_added++;
    // Append the non-trimmed word, in case more words are added after.
    AddToCurrentLine(word);
  } else {
    lines_added = AddWordOverflow(wrap_behavior_ == ui::IGNORE_LONG_WORDS ?
                                  trimmed : word);
  }
  return lines_added;
}

void RectangleText::AddToCurrentLine(const string16& text) {
  AddToCurrentLineWithWidth(text, font_.GetStringWidth(text));
}

void RectangleText::AddToCurrentLineWithWidth(const string16& text,
                                              int text_width) {
  if (current_height_ >= available_pixel_height_) {
    insufficient_height_ = true;
    return;
  }
  current_line_.append(text);
  current_width_ += text_width;
}

bool RectangleText::NewLine() {
  bool line_added = false;
  if (current_height_ < available_pixel_height_) {
    lines_->push_back(current_line_);
    current_line_.clear();
    line_added = true;
  } else {
    insufficient_height_ = true;
  }
  current_height_ += line_height_;
  current_width_ = 0;
  return line_added;
}

}  // namespace

namespace ui {

bool ElideRectangleString(const string16& input, size_t max_rows,
                          size_t max_cols, bool strict, string16* output) {
  RectangleString rect(max_rows, max_cols, strict, output);
  rect.Init();
  rect.AddString(input);
  return rect.Finalize();
}

int ElideRectangleText(const string16& input,
                        const gfx::Font& font,
                        int available_pixel_width,
                        int available_pixel_height,
                        WordWrapBehavior wrap_behavior,
                        std::vector<string16>* lines) {
  RectangleText rect(font,
                     available_pixel_width,
                     available_pixel_height,
                     wrap_behavior,
                     lines);
  rect.Init();
  rect.AddString(input);
  return rect.Finalize();
}

string16 TruncateString(const string16& string, size_t length) {
  if (string.size() <= length)
    // String fits, return it.
    return string;

  if (length == 0)
    // No room for the elide string, return an empty string.
    return string16();

  size_t max = length - 1;

  // Added to the end of strings that are too big.
  static const char16 kElideString[] = { 0x2026, 0 };

  if (max == 0)
    // Just enough room for the elide string.
    return kElideString;

  // Use a line iterator to find the first boundary.
  UErrorCode status = U_ZERO_ERROR;
  scoped_ptr<icu::RuleBasedBreakIterator> bi(
      static_cast<icu::RuleBasedBreakIterator*>(
          icu::RuleBasedBreakIterator::createLineInstance(
              icu::Locale::getDefault(), status)));
  if (U_FAILURE(status))
    return string.substr(0, max) + kElideString;
  bi->setText(string.c_str());
  int32_t index = bi->preceding(static_cast<int32_t>(max));
  if (index == icu::BreakIterator::DONE) {
    index = static_cast<int32_t>(max);
  } else {
    // Found a valid break (may be the beginning of the string). Now use
    // a character iterator to find the previous non-whitespace character.
    icu::StringCharacterIterator char_iterator(string.c_str());
    if (index == 0) {
      // No valid line breaks. Start at the end again. This ensures we break
      // on a valid character boundary.
      index = static_cast<int32_t>(max);
    }
    char_iterator.setIndex(index);
    while (char_iterator.hasPrevious()) {
      char_iterator.previous();
      if (!(u_isspace(char_iterator.current()) ||
            u_charType(char_iterator.current()) == U_CONTROL_CHAR ||
            u_charType(char_iterator.current()) == U_NON_SPACING_MARK)) {
        // Not a whitespace character. Advance the iterator so that we
        // include the current character in the truncated string.
        char_iterator.next();
        break;
      }
    }
    if (char_iterator.hasPrevious()) {
      // Found a valid break point.
      index = char_iterator.getIndex();
    } else {
      // String has leading whitespace, return the elide string.
      return kElideString;
    }
  }
  return string.substr(0, index) + kElideString;
}

}  // namespace ui
