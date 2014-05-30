// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ime/win/tsf_input_scope.h"

#include <InputScope.h>

#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/win/windows_version.h"

namespace ui {
namespace tsf_inputscope {
namespace {

class TSFInputScope : public ITfInputScope {
 public:
  explicit TSFInputScope(InputScope input_scope)
      : input_scope_(input_scope),
        ref_count_(0) {}

  // ITfInputScope:
  STDMETHOD_(ULONG, AddRef)() OVERRIDE {
    return InterlockedIncrement(&ref_count_);
  }

  STDMETHOD_(ULONG, Release)() OVERRIDE {
    const LONG count = InterlockedDecrement(&ref_count_);
    if (!count) {
      delete this;
      return 0;
    }
    return static_cast<ULONG>(count);
  }

  STDMETHOD(QueryInterface)(REFIID iid, void** result) OVERRIDE {
    if (!result)
      return E_INVALIDARG;
    if (iid == IID_IUnknown || iid == IID_ITfInputScope) {
      *result = static_cast<ITfInputScope*>(this);
    } else {
      *result = NULL;
      return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
  }

  STDMETHOD(GetInputScopes)(InputScope** input_scopes, UINT* count) OVERRIDE {
    if (!count || !input_scopes)
      return E_INVALIDARG;
    *input_scopes = static_cast<InputScope*>(CoTaskMemAlloc(
        sizeof(InputScope)));
    if (!input_scopes) {
      *count = 0;
      return E_OUTOFMEMORY;
    }
    (*input_scopes)[0] = input_scope_;
    *count = 1;
    return S_OK;
  }

  STDMETHOD(GetPhrase)(BSTR** phrases, UINT* count) OVERRIDE {
    return E_NOTIMPL;
  }

  STDMETHOD(GetRegularExpression)(BSTR* regexp) OVERRIDE {
    return E_NOTIMPL;
  }

  STDMETHOD(GetSRGS)(BSTR* srgs) OVERRIDE {
    return E_NOTIMPL;
  }

  STDMETHOD(GetXML)(BSTR* xml) OVERRIDE {
    return E_NOTIMPL;
  }

 private:
  // The corresponding text input type.
  InputScope input_scope_;

  // The refrence count of this instance.
  volatile LONG ref_count_;

  DISALLOW_COPY_AND_ASSIGN(TSFInputScope);
};

typedef HRESULT (WINAPI *SetInputScopeFunc)(HWND window_handle,
                                            InputScope input_scope);

SetInputScopeFunc g_set_input_scope = NULL;
bool g_get_proc_done = false;

SetInputScopeFunc GetSetInputScope() {
  // Thread safety is not required.
  if (!g_get_proc_done) {
    g_get_proc_done = true;

    // For stability reasons, we do not support Windows XP.
    if (base::win::GetVersion() < base::win::VERSION_VISTA)
      return NULL;

    HMODULE module = NULL;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_PIN, L"msctf.dll",
        &module)) {
      return NULL;
    }
    g_set_input_scope = reinterpret_cast<SetInputScopeFunc>(
        GetProcAddress(module, "SetInputScope"));
  }
  return g_set_input_scope;
}

InputScope GetInputScopeType(TextInputType text_input_type) {
  // Following mapping is based in IE10 on Windows 8.
  switch (text_input_type) {
    case TEXT_INPUT_TYPE_PASSWORD:
      return IS_PASSWORD;
    case TEXT_INPUT_TYPE_SEARCH:
      return IS_SEARCH;
    case TEXT_INPUT_TYPE_EMAIL:
      return IS_EMAIL_SMTPEMAILADDRESS;
    case TEXT_INPUT_TYPE_NUMBER:
      return IS_NUMBER;
    case TEXT_INPUT_TYPE_TELEPHONE:
      return IS_TELEPHONE_FULLTELEPHONENUMBER;
    case TEXT_INPUT_TYPE_URL:
      return IS_URL;
    default:
      return IS_DEFAULT;
  }
}

}  // namespace

ITfInputScope* CreateInputScope(TextInputType text_input_type) {
  return new TSFInputScope(GetInputScopeType(text_input_type));
}

void SetInputScopeForTsfUnawareWindow(HWND window_handle,
                                      TextInputType text_input_type) {
  SetInputScopeFunc set_input_scope = GetSetInputScope();
  if (set_input_scope)
    set_input_scope(window_handle, GetInputScopeType(text_input_type));
}

}  // namespace tsf_inputscope
}  // namespace ui
