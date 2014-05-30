// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/x/selection_utils.h"

#include <set>

#include "base/i18n/icu_string_conversions.h"
#include "base/logging.h"
#include "base/utf_string_conversions.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/x/x11_atom_cache.h"

namespace ui {

const char kMimeTypeMozillaURL[] = "text/x-moz-url";
const char kString[] = "STRING";
const char kText[] = "TEXT";
const char kUtf8String[] = "UTF8_STRING";

const char* kSelectionDataAtoms[] = {
  Clipboard::kMimeTypeHTML,
  kString,
  kText,
  kUtf8String,
  NULL
};

std::vector< ::Atom> GetTextAtomsFrom(const X11AtomCache* atom_cache) {
  std::vector< ::Atom> atoms;
  atoms.push_back(atom_cache->GetAtom(kString));
  atoms.push_back(atom_cache->GetAtom(kText));
  atoms.push_back(atom_cache->GetAtom(kUtf8String));
  return atoms;
}

std::vector< ::Atom> GetURLAtomsFrom(const X11AtomCache* atom_cache) {
  std::vector< ::Atom> atoms;
  atoms.push_back(atom_cache->GetAtom(Clipboard::kMimeTypeURIList));
  atoms.push_back(atom_cache->GetAtom(kMimeTypeMozillaURL));
  return atoms;
}

void GetAtomIntersection(const std::vector< ::Atom>& one,
                         const std::vector< ::Atom>& two,
                         std::vector< ::Atom>* output) {
  for (std::vector< ::Atom>::const_iterator it = one.begin(); it != one.end();
       ++it) {
    for (std::vector< ::Atom>::const_iterator jt = two.begin(); jt != two.end();
         ++jt) {
      if (*it == *jt) {
        output->push_back(*it);
        break;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

SelectionFormatMap::SelectionFormatMap() {}

SelectionFormatMap::~SelectionFormatMap() {
  // WriteText() inserts the same pointer multiple times for different
  // representations; we need to dedupe it.
  std::set<char*> to_delete;
  for (InternalMap::iterator it = data_.begin(); it != data_.end(); ++it)
    to_delete.insert(it->second.first);

  for (std::set<char*>::iterator it = to_delete.begin(); it != to_delete.end();
       ++it) {
    delete [] *it;
  }
}

void SelectionFormatMap::Insert(::Atom atom, char* data, size_t size) {
  DCHECK(data_.find(atom) == data_.end());
  data_.insert(std::make_pair(atom, std::make_pair(data, size)));
}

///////////////////////////////////////////////////////////////////////////////

SelectionData::SelectionData(Display* x_display)
    : type_(None),
      data_(NULL),
      size_(0),
      owned_(false),
      atom_cache_(x_display, kSelectionDataAtoms) {
}

SelectionData::~SelectionData() {
  if (owned_)
    XFree(data_);
}

void SelectionData::Set(::Atom type, char* data, size_t size, bool owned) {
  if (owned_)
    XFree(data_);

  type_ = type;
  data_ = data;
  size_ = size;
  owned_ = owned;
}

std::string SelectionData::GetText() const {
  if (type_ == atom_cache_.GetAtom(kUtf8String) ||
      type_ == atom_cache_.GetAtom(kText)) {
    return std::string(data_, size_);
  } else if (type_ == atom_cache_.GetAtom(kString)) {
    std::string result;
    base::ConvertToUtf8AndNormalize(std::string(data_, size_),
                                    base::kCodepageLatin1,
                                    &result);
    return result;
  } else {
    // BTW, I looked at COMPOUND_TEXT, and there's no way we're going to
    // support that. Yuck.
    NOTREACHED();
    return std::string();
  }
}

string16 SelectionData::GetHtml() const {
  string16 markup;

  if (type_ == atom_cache_.GetAtom(Clipboard::kMimeTypeHTML)) {
    // If the data starts with 0xFEFF, i.e., Byte Order Mark, assume it is
    // UTF-16, otherwise assume UTF-8.
    if (size_ >= 2 &&
        reinterpret_cast<const uint16_t*>(data_)[0] == 0xFEFF) {
      markup.assign(reinterpret_cast<const uint16_t*>(data_) + 1,
                    (size_ / 2) - 1);
    } else {
      UTF8ToUTF16(reinterpret_cast<const char*>(data_), size_, &markup);
    }

    // If there is a terminating NULL, drop it.
    if (!markup.empty() && markup.at(markup.length() - 1) == '\0')
      markup.resize(markup.length() - 1);

    return markup;
  } else {
    NOTREACHED();
    return markup;
  }
}

void SelectionData::AssignTo(std::string* result) const {
  result->assign(data_, size_);
}

void SelectionData::AssignTo(string16* result) const {
  result->assign(reinterpret_cast<base::char16*>(data_), size_ / 2);
}

}  // namespace ui
