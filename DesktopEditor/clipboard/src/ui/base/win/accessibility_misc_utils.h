// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef UI_BASE_WIN_ACCESSIBILITY_MISC_UTILS_H_
#define UI_BASE_WIN_ACCESSIBILITY_MISC_UTILS_H_

#include <atlbase.h>
#include <atlcom.h>
#include <UIAutomationCore.h>

#include "base/compiler_specific.h"
#include "ui/base/ui_export.h"

namespace base {
namespace win {

  // UIA Text provider implementation for edit controls.
class UI_EXPORT UIATextProvider
    : public NON_EXPORTED_BASE(CComObjectRootEx<CComMultiThreadModel>),
      public IValueProvider,
      public ITextProvider {
 public:
  BEGIN_COM_MAP(UIATextProvider)
    COM_INTERFACE_ENTRY2(IUnknown, ITextProvider)
    COM_INTERFACE_ENTRY(IValueProvider)
    COM_INTERFACE_ENTRY(ITextProvider)
  END_COM_MAP()

  UIATextProvider();

  // Creates an instance of the UIATextProvider class.
  // Returns true on success
  static bool CreateTextProvider(bool editable, IUnknown** provider);

  void set_editable(bool editable) {
    editable_ = editable;
  }

  //
  // IValueProvider methods.
  //
  STDMETHOD(get_IsReadOnly)(BOOL* read_only);

  //
  // IValueProvider methods not implemented.
  //
  STDMETHOD(SetValue)(const wchar_t* val) {
    return E_NOTIMPL;
  }

  STDMETHOD(get_Value)(BSTR* value) {
    return E_NOTIMPL;
  }

  //
  // ITextProvider methods.
  //
  STDMETHOD(GetSelection)(SAFEARRAY** ret) {
    return E_NOTIMPL;
  }

  STDMETHOD(GetVisibleRanges)(SAFEARRAY** ret) {
    return E_NOTIMPL;
  }

  STDMETHOD(RangeFromChild)(IRawElementProviderSimple* child,
                            ITextRangeProvider** ret) {
    return E_NOTIMPL;
  }

  STDMETHOD(RangeFromPoint)(struct UiaPoint point,
                            ITextRangeProvider** ret) {
    return E_NOTIMPL;
  }

  STDMETHOD(get_DocumentRange)(ITextRangeProvider** ret) {
    return E_NOTIMPL;
  }

  STDMETHOD(get_SupportedTextSelection)(enum SupportedTextSelection* ret) {
    return E_NOTIMPL;
  }

 private:
  bool editable_;
};

}  // win
}  // base

#endif  // UI_BASE_WIN_ACCESSIBILITY_MISC_UTILS_H_
