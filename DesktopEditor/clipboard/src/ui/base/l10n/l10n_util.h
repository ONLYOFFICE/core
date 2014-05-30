// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file contains utility functions for dealing with localized
// content.

#ifndef UI_BASE_L10N_L10N_UTIL_H_
#define UI_BASE_L10N_L10N_UTIL_H_

#include <string>
#include <vector>

#include "base/string16.h"
#include "base/string_util.h"
#include "ui/base/ui_export.h"

#if defined(OS_MACOSX)
#include "ui/base/l10n/l10n_util_mac.h"
#endif  // OS_MACOSX

namespace l10n_util {

// This method is responsible for determining the locale as defined below. In
// nearly all cases you shouldn't call this, rather use GetApplicationLocale
// defined on browser_process.
//
// Returns the locale used by the Application.  First we use the value from the
// command line (--lang), second we try the value in the prefs file (passed in
// as |pref_locale|), finally, we fall back on the system locale. We only return
// a value if there's a corresponding resource DLL for the locale.  Otherwise,
// we fall back to en-us.
UI_EXPORT std::string GetApplicationLocale(const std::string& pref_locale);

// Given a locale code, return true if the OS is capable of supporting it.
// For instance, Oriya is not well supported on Windows XP and we return
// false for "or".
bool IsLocaleSupportedByOS(const std::string& locale);

// This method returns the display name of the locale code in |display_locale|.

// For example, for |locale| = "fr" and |display_locale| = "en",
// it returns "French". To get the display name of
// |locale| in the UI language of Chrome, |display_locale| can be
// set to the return value of g_browser_process->GetApplicationLocale()
// in the UI thread.
// If |is_for_ui| is true, U+200F is appended so that it can be
// rendered properly in a RTL Chrome.
UI_EXPORT string16 GetDisplayNameForLocale(const std::string& locale,
                                           const std::string& display_locale,
                                           bool is_for_ui);

// Returns the display name of the |country_code| in |display_locale|.
UI_EXPORT string16 GetDisplayNameForCountry(const std::string& country_code,
                                            const std::string& display_locale);

// Converts all - into _, to be consistent with ICU and file system names.
UI_EXPORT std::string NormalizeLocale(const std::string& locale);

// Produce a vector of parent locales for given locale.
// It includes the current locale in the result.
// sr_Cyrl_RS generates sr_Cyrl_RS, sr_Cyrl and sr.
UI_EXPORT void GetParentLocales(const std::string& current_locale,
                                std::vector<std::string>* parent_locales);

// Checks if a string is plausibly a syntactically-valid locale string,
// for cases where we want the valid input to be a locale string such as
// 'en', 'pt-BR', 'fil', 'es-419', 'zh-Hans-CN', 'i-klingon' or
// 'de_DE@collation=phonebook', but we don't want to limit it to
// locales that Chrome actually knows about, so 'xx-YY' should be
// accepted, but 'z', 'German', 'en-$1', or 'abcd-1234' should not.
// Case-insensitive. Based on BCP 47, see:
//   http://unicode.org/reports/tr35/#Unicode_Language_and_Locale_Identifiers
UI_EXPORT bool IsValidLocaleSyntax(const std::string& locale);

//
// Mac Note: See l10n_util_mac.h for some NSString versions and other support.
//

// Pulls resource string from the string bundle and returns it.
UI_EXPORT std::string GetStringUTF8(int message_id);
UI_EXPORT string16 GetStringUTF16(int message_id);

// Get a resource string and replace $1-$2-$3 with |a| and |b|
// respectively.  Additionally, $$ is replaced by $.
UI_EXPORT string16 GetStringFUTF16(int message_id,
                                   const string16& a);
UI_EXPORT string16 GetStringFUTF16(int message_id,
                                   const string16& a,
                                   const string16& b);
UI_EXPORT string16 GetStringFUTF16(int message_id,
                                   const string16& a,
                                   const string16& b,
                                   const string16& c);
UI_EXPORT string16 GetStringFUTF16(int message_id,
                                   const string16& a,
                                   const string16& b,
                                   const string16& c,
                                   const string16& d);
UI_EXPORT string16 GetStringFUTF16(int message_id,
                                   const string16& a,
                                   const string16& b,
                                   const string16& c,
                                   const string16& d,
                                   const string16& e);
UI_EXPORT std::string GetStringFUTF8(int message_id,
                                     const string16& a);
UI_EXPORT std::string GetStringFUTF8(int message_id,
                                     const string16& a,
                                     const string16& b);
UI_EXPORT std::string GetStringFUTF8(int message_id,
                                     const string16& a,
                                     const string16& b,
                                     const string16& c);
UI_EXPORT std::string GetStringFUTF8(int message_id,
                                     const string16& a,
                                     const string16& b,
                                     const string16& c,
                                     const string16& d);

// Variants that return the offset(s) of the replaced parameters. The
// vector based version returns offsets ordered by parameter. For example if
// invoked with a and b offsets[0] gives the offset for a and offsets[1] the
// offset of b regardless of where the parameters end up in the string.
UI_EXPORT string16 GetStringFUTF16(int message_id,
                                   const string16& a,
                                   size_t* offset);
UI_EXPORT string16 GetStringFUTF16(int message_id,
                                   const string16& a,
                                   const string16& b,
                                   std::vector<size_t>* offsets);

// Convenience functions to get a string with a single number as a parameter.
UI_EXPORT string16 GetStringFUTF16Int(int message_id, int a);
string16 GetStringFUTF16Int(int message_id, int64 a);

// In place sorting of string16 strings using collation rules for |locale|.
UI_EXPORT void SortStrings16(const std::string& locale,
                             std::vector<string16>* strings);

// Returns a vector of available locale codes. E.g., a vector containing
// en-US, es, fr, fi, pt-PT, pt-BR, etc.
UI_EXPORT const std::vector<std::string>& GetAvailableLocales();

// Returns a vector of locale codes usable for accept-languages.
UI_EXPORT void GetAcceptLanguagesForLocale(
    const std::string& display_locale,
    std::vector<std::string>* locale_codes);

// Returns the preferred size of the contents view of a window based on
// designer given constraints which might dependent on the language used.
UI_EXPORT int GetLocalizedContentsWidthInPixels(int pixel_resource_id);

}  // namespace l10n_util

#endif  // UI_BASE_L10N_L10N_UTIL_H_
