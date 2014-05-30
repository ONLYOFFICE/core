// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/dragdrop/gtk_dnd_util.h"

#include <string>

#include "base/logging.h"
#include "base/pickle.h"
#include "base/utf_string_conversions.h"
#include "googleurl/src/gurl.h"
#include "ui/base/clipboard/custom_data_helper.h"

static const int kBitsPerByte = 8;

namespace ui {

namespace {

void AddTargetToList(GtkTargetList* targets, int target_code) {
  switch (target_code) {
    case ui::TEXT_PLAIN:
      gtk_target_list_add_text_targets(targets, ui::TEXT_PLAIN);
      break;

    case ui::TEXT_URI_LIST:
      gtk_target_list_add_uri_targets(targets, ui::TEXT_URI_LIST);
      break;

    case ui::TEXT_HTML:
      gtk_target_list_add(
          targets, ui::GetAtomForTarget(ui::TEXT_HTML), 0, ui::TEXT_HTML);
      break;

    case ui::NETSCAPE_URL:
      gtk_target_list_add(targets,
          ui::GetAtomForTarget(ui::NETSCAPE_URL), 0, ui::NETSCAPE_URL);
      break;

    case ui::CHROME_TAB:
    case ui::CHROME_BOOKMARK_ITEM:
    case ui::CHROME_NAMED_URL:
      gtk_target_list_add(targets, ui::GetAtomForTarget(target_code),
                          GTK_TARGET_SAME_APP, target_code);
      break;

    case ui::DIRECT_SAVE_FILE:
      gtk_target_list_add(targets,
          ui::GetAtomForTarget(ui::DIRECT_SAVE_FILE), 0, ui::DIRECT_SAVE_FILE);
      break;

    case ui::CUSTOM_DATA:
      gtk_target_list_add(targets,
          ui::GetAtomForTarget(ui::CUSTOM_DATA), 0, ui::CUSTOM_DATA);
      break;

    default:
      NOTREACHED() << " Unexpected target code: " << target_code;
  }
}

}  // namespace

GdkAtom GetAtomForTarget(int target) {
  switch (target) {
    case CHROME_TAB:
      static const GdkAtom kTabAtom = gdk_atom_intern(
          "application/x-chrome-tab", false);
      return kTabAtom;

    case TEXT_HTML:
      static const GdkAtom kHtmlAtom = gdk_atom_intern(
          "text/html", false);
      return kHtmlAtom;

    case CHROME_BOOKMARK_ITEM:
      static const GdkAtom kBookmarkAtom = gdk_atom_intern(
          "application/x-chrome-bookmark-item", false);
      return kBookmarkAtom;

    case TEXT_PLAIN:
      static const GdkAtom kTextAtom= gdk_atom_intern(
          "text/plain;charset=utf-8", false);
      return kTextAtom;

    case TEXT_URI_LIST:
      static const GdkAtom kUrisAtom = gdk_atom_intern(
          "text/uri-list", false);
      return kUrisAtom;

    case CHROME_NAMED_URL:
      static const GdkAtom kNamedUrl = gdk_atom_intern(
          "application/x-chrome-named-url", false);
      return kNamedUrl;

    case NETSCAPE_URL:
      static const GdkAtom kNetscapeUrl = gdk_atom_intern(
          "_NETSCAPE_URL", false);
      return kNetscapeUrl;

    case TEXT_PLAIN_NO_CHARSET:
      static const GdkAtom kTextNoCharsetAtom = gdk_atom_intern(
          "text/plain", false);
      return kTextNoCharsetAtom;

    case DIRECT_SAVE_FILE:
      static const GdkAtom kXdsAtom = gdk_atom_intern(
          "XdndDirectSave0", false);
      return kXdsAtom;

    case CUSTOM_DATA:
      static const GdkAtom kCustomData = gdk_atom_intern(
          kMimeTypeWebCustomData, false);
      return kCustomData;

    default:
      NOTREACHED();
  }

  return NULL;
}

GtkTargetList* GetTargetListFromCodeMask(int code_mask) {
  GtkTargetList* targets = gtk_target_list_new(NULL, 0);

  for (size_t i = 1; i < INVALID_TARGET; i = i << 1) {
    if (i == CHROME_WEBDROP_FILE_CONTENTS)
      continue;

    if (i & code_mask)
      AddTargetToList(targets, i);
  }

  return targets;
}

void SetSourceTargetListFromCodeMask(GtkWidget* source, int code_mask) {
  GtkTargetList* targets = GetTargetListFromCodeMask(code_mask);
  gtk_drag_source_set_target_list(source, targets);
  gtk_target_list_unref(targets);
}

void SetDestTargetList(GtkWidget* dest, const int* target_codes) {
  GtkTargetList* targets = gtk_target_list_new(NULL, 0);

  for (size_t i = 0; target_codes[i] != -1; ++i) {
    AddTargetToList(targets, target_codes[i]);
  }

  gtk_drag_dest_set_target_list(dest, targets);
  gtk_target_list_unref(targets);
}

void WriteURLWithName(GtkSelectionData* selection_data,
                      const GURL& url,
                      string16 title,
                      int type) {
  if (title.empty()) {
    // We prefer to not have empty titles. Set it to the filename extracted
    // from the URL.
    title = UTF8ToUTF16(url.ExtractFileName());
  }

  switch (type) {
    case TEXT_PLAIN: {
      gtk_selection_data_set_text(selection_data, url.spec().c_str(),
                                  url.spec().length());
      break;
    }
    case TEXT_URI_LIST: {
      gchar* uri_array[2];
      uri_array[0] = strdup(url.spec().c_str());
      uri_array[1] = NULL;
      gtk_selection_data_set_uris(selection_data, uri_array);
      free(uri_array[0]);
      break;
    }
    case CHROME_NAMED_URL: {
      Pickle pickle;
      pickle.WriteString(UTF16ToUTF8(title));
      pickle.WriteString(url.spec());
      gtk_selection_data_set(
          selection_data,
          GetAtomForTarget(ui::CHROME_NAMED_URL),
          kBitsPerByte,
          reinterpret_cast<const guchar*>(pickle.data()),
          pickle.size());
      break;
    }
    case NETSCAPE_URL: {
      // _NETSCAPE_URL format is URL + \n + title.
      std::string utf8_text = url.spec() + "\n" + UTF16ToUTF8(title);
      gtk_selection_data_set(selection_data,
                             gtk_selection_data_get_target(selection_data),
                             kBitsPerByte,
                             reinterpret_cast<const guchar*>(utf8_text.c_str()),
                             utf8_text.length());
      break;
    }

    default: {
      NOTREACHED();
      break;
    }
  }
}

bool ExtractNamedURL(GtkSelectionData* selection_data,
                     GURL* url,
                     string16* title) {
  if (!selection_data || gtk_selection_data_get_length(selection_data) <= 0)
    return false;

  Pickle data(
      reinterpret_cast<const char*>(
          gtk_selection_data_get_data(selection_data)),
      gtk_selection_data_get_length(selection_data));
  PickleIterator iter(data);
  std::string title_utf8, url_utf8;
  if (!data.ReadString(&iter, &title_utf8) ||
      !data.ReadString(&iter, &url_utf8)) {
    return false;
  }

  GURL gurl(url_utf8);
  if (!gurl.is_valid())
    return false;

  *url = gurl;
  *title = UTF8ToUTF16(title_utf8);
  return true;
}

bool ExtractURIList(GtkSelectionData* selection_data, std::vector<GURL>* urls) {
  gchar** uris = gtk_selection_data_get_uris(selection_data);
  if (!uris)
    return false;

  for (size_t i = 0; uris[i] != NULL; ++i) {
    GURL url(uris[i]);
    if (url.is_valid())
      urls->push_back(url);
  }

  g_strfreev(uris);
  return true;
}

bool ExtractNetscapeURL(GtkSelectionData* selection_data,
                        GURL* url,
                        string16* title) {
  if (!selection_data || gtk_selection_data_get_length(selection_data) <= 0)
    return false;

  // Find the first '\n' in the data. It is the separator between the url and
  // the title.
  std::string data(
      reinterpret_cast<const char*>(
          gtk_selection_data_get_data(selection_data)),
      gtk_selection_data_get_length(selection_data));
  std::string::size_type newline = data.find('\n');
  if (newline == std::string::npos)
    return false;

  GURL gurl(data.substr(0, newline));
  if (!gurl.is_valid())
    return false;

  *url = gurl;
  *title = UTF8ToUTF16(data.substr(newline + 1));
  return true;
}

}  // namespace ui
