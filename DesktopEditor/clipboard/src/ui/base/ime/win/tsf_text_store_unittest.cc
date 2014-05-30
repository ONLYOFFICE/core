// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ime/win/tsf_text_store.h"

#include "base/win/scoped_com_initializer.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/gfx/rect.h"

using testing::_;
using testing::Invoke;
using testing::Return;

namespace ui {

namespace {

class MockTextInputClient : public TextInputClient {
 public:
  ~MockTextInputClient() {}
  MOCK_METHOD1(SetCompositionText, void(const ui::CompositionText&));
  MOCK_METHOD0(ConfirmCompositionText, void());
  MOCK_METHOD0(ClearCompositionText, void());
  MOCK_METHOD1(InsertText, void(const string16&));
  MOCK_METHOD2(InsertChar, void(char16, int));
  MOCK_CONST_METHOD0(GetTextInputType, ui::TextInputType());
  MOCK_CONST_METHOD0(CanComposeInline, bool());
  MOCK_METHOD0(GetCaretBounds, gfx::Rect());
  MOCK_METHOD2(GetCompositionCharacterBounds, bool(uint32, gfx::Rect*));
  MOCK_METHOD0(HasCompositionText, bool());
  MOCK_METHOD1(GetTextRange, bool(ui::Range*));
  MOCK_METHOD1(GetCompositionTextRange, bool(ui::Range*));
  MOCK_METHOD1(GetSelectionRange, bool(ui::Range*));
  MOCK_METHOD1(SetSelectionRange, bool(const ui::Range&));
  MOCK_METHOD1(DeleteRange, bool(const ui::Range&));
  MOCK_METHOD2(GetTextFromRange, bool(const ui::Range&, string16*));
  MOCK_METHOD0(OnInputMethodChanged, void());
  MOCK_METHOD1(ChangeTextDirectionAndLayoutAlignment,
               bool(base::i18n::TextDirection));
  MOCK_METHOD2(ExtendSelectionAndDelete, void(size_t, size_t));
  MOCK_METHOD1(EnsureCaretInRect, void(const gfx::Rect&));
};

class MockStoreACPSink : public ITextStoreACPSink {
 public:
  MockStoreACPSink() : ref_count_(0) {
  }

  // IUnknown
  virtual ULONG STDMETHODCALLTYPE AddRef() OVERRIDE {
    return InterlockedIncrement(&ref_count_);
  }
  virtual ULONG STDMETHODCALLTYPE Release() OVERRIDE {
    const LONG count = InterlockedDecrement(&ref_count_);
    if (!count) {
      delete this;
      return 0;
    }
    return static_cast<ULONG>(count);
  }
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(
      REFIID iid, void** report) OVERRIDE {
    if (iid == IID_IUnknown || iid == IID_ITextStoreACPSink) {
      *report = static_cast<ITextStoreACPSink*>(this);
    } else {
      *report = NULL;
      return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
  }

  // ITextStoreACPSink
  MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, OnTextChange,
                             HRESULT(DWORD, const TS_TEXTCHANGE*));
  MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, OnSelectionChange,
                             HRESULT());
  MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, OnLayoutChange,
                             HRESULT(TsLayoutCode, TsViewCookie));
  MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, OnStatusChange,
                             HRESULT(DWORD));
  MOCK_METHOD4_WITH_CALLTYPE(STDMETHODCALLTYPE, OnAttrsChange,
                             HRESULT(LONG, LONG, ULONG, const TS_ATTRID*));
  MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, OnLockGranted,
                             HRESULT(DWORD));
  MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, OnStartEditTransaction,
                             HRESULT());
  MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, OnEndEditTransaction,
                             HRESULT());

 private:
  ~MockStoreACPSink() {
  }

  volatile LONG ref_count_;
};

}  // namespace

class TSFTextStoreTest : public testing::Test {
 protected:
  virtual void SetUp() OVERRIDE {
    text_store_ = new TSFTextStore();
    text_store_->AddRef();
    sink_ = new MockStoreACPSink();
    sink_->AddRef();
    EXPECT_EQ(S_OK, text_store_->AdviseSink(IID_ITextStoreACPSink,
                                            sink_, TS_AS_ALL_SINKS));
    text_store_->SetFocusedTextInputClient(0, &text_input_client_);
  }

  virtual void TearDown() OVERRIDE {
    EXPECT_EQ(S_OK, text_store_->UnadviseSink(sink_));
    sink_->Release();
    text_store_->Release();
  }

  base::win::ScopedCOMInitializer com_initializer_;
  MockTextInputClient text_input_client_;
  TSFTextStore* text_store_;
  MockStoreACPSink* sink_;
};

class TSFTextStoreTestCallback {
 public:
  explicit TSFTextStoreTestCallback(TSFTextStore* text_store)
      : text_store_(text_store) {
    CHECK(text_store_);
  }
  virtual ~TSFTextStoreTestCallback() {}

 protected:
  // Accessors to the internal state of TSFTextStore.
  bool* edit_flag() { return &text_store_->edit_flag_; }
  string16* string_buffer() { return &text_store_->string_buffer_; }
  size_t* committed_size() { return &text_store_->committed_size_; }
  Range* selection() { return &text_store_->selection_; }
  CompositionUnderlines* composition_undelines() {
    return &text_store_->composition_undelines_;
  }

  void SetInternalState(const string16& new_string_buffer,
                        LONG new_committed_size, LONG new_selection_start,
                        LONG new_selection_end) {
    ASSERT_LE(0, new_committed_size);
    ASSERT_LE(new_committed_size, new_selection_start);
    ASSERT_LE(new_selection_start, new_selection_end);
    ASSERT_LE(new_selection_end, static_cast<LONG>(new_string_buffer.size()));
    *string_buffer() = new_string_buffer;
    *committed_size() = new_committed_size;
    selection()->set_start(new_selection_start);
    selection()->set_end(new_selection_end);
  }

  bool HasReadLock() const { return text_store_->HasReadLock(); }
  bool HasReadWriteLock() const { return text_store_->HasReadWriteLock(); }

  void GetSelectionTest(LONG expected_acp_start, LONG expected_acp_end) {
    TS_SELECTION_ACP selection;
    ULONG fetched;
    EXPECT_EQ(S_OK, text_store_->GetSelection(0, 1, &selection, &fetched));
    EXPECT_EQ(1, fetched);
    EXPECT_EQ(expected_acp_start, selection.acpStart);
    EXPECT_EQ(expected_acp_end, selection.acpEnd);
  }

  void SetSelectionTest(LONG acp_start, LONG acp_end, HRESULT expected_result) {
    TS_SELECTION_ACP selection;
    selection.acpStart = acp_start;
    selection.acpEnd = acp_end;
    selection.style.ase = TS_AE_NONE;
    selection.style.fInterimChar = 0;
    EXPECT_EQ(expected_result, text_store_->SetSelection(1, &selection));
    if (expected_result == S_OK) {
      GetSelectionTest(acp_start, acp_end);
    }
  }

  void SetTextTest(LONG acp_start, LONG acp_end,
                   const string16& text, HRESULT error_code) {
    TS_TEXTCHANGE change;
    ASSERT_EQ(error_code,
              text_store_->SetText(0, acp_start, acp_end,
                                   text.c_str(), text.size(), &change));
    if (error_code == S_OK) {
      EXPECT_EQ(acp_start, change.acpStart);
      EXPECT_EQ(acp_end, change.acpOldEnd);
      EXPECT_EQ(acp_start + text.size(), change.acpNewEnd);
    }
  }

  void GetTextTest(LONG acp_start, LONG acp_end,
                   const string16& expected_string,
                   LONG expected_next_acp) {
    wchar_t buffer[1024];
    ULONG text_buffer_copied;
    TS_RUNINFO run_info;
    ULONG run_info_buffer_copied;
    LONG next_acp;
    ASSERT_EQ(S_OK,
              text_store_->GetText(acp_start, acp_end, buffer, 1024,
                                   &text_buffer_copied,
                                   &run_info, 1, &run_info_buffer_copied,
                                   &next_acp));
    ASSERT_EQ(expected_string.size(), text_buffer_copied);
    EXPECT_EQ(expected_string, string16(buffer, buffer + text_buffer_copied));
    EXPECT_EQ(1, run_info_buffer_copied);
    EXPECT_EQ(expected_string.size(), run_info.uCount);
    EXPECT_EQ(TS_RT_PLAIN, run_info.type);
    EXPECT_EQ(expected_next_acp, next_acp);
  }

  void GetTextErrorTest(LONG acp_start, LONG acp_end, HRESULT error_code) {
    wchar_t buffer[1024];
    ULONG text_buffer_copied;
    TS_RUNINFO run_info;
    ULONG run_info_buffer_copied;
    LONG next_acp;
    EXPECT_EQ(error_code,
              text_store_->GetText(acp_start, acp_end, buffer, 1024,
                                   &text_buffer_copied,
                                   &run_info, 1, &run_info_buffer_copied,
                                   &next_acp));
  }

  void InsertTextAtSelectionTest(const wchar_t* buffer, ULONG buffer_size,
                                 LONG expected_start, LONG expected_end,
                                 LONG expected_change_start,
                                 LONG expected_change_old_end,
                                 LONG expected_change_new_end) {
    LONG start, end;
    TS_TEXTCHANGE change;
    EXPECT_EQ(S_OK,
              text_store_->InsertTextAtSelection(0, buffer, buffer_size,
                                                 &start, &end, &change));
    EXPECT_EQ(expected_start, start);
    EXPECT_EQ(expected_end, end);
    EXPECT_EQ(expected_change_start, change.acpStart);
    EXPECT_EQ(expected_change_old_end, change.acpOldEnd);
    EXPECT_EQ(expected_change_new_end, change.acpNewEnd);
  }

  void InsertTextAtSelectionQueryOnlyTest(const wchar_t* buffer,
                                          ULONG buffer_size,
                                          LONG expected_start,
                                          LONG expected_end) {
    LONG start, end;
    EXPECT_EQ(S_OK,
              text_store_->InsertTextAtSelection(TS_IAS_QUERYONLY, buffer,
                                                 buffer_size, &start, &end,
                                                 NULL));
    EXPECT_EQ(expected_start, start);
    EXPECT_EQ(expected_end, end);
  }

  void GetTextExtTest(TsViewCookie view_cookie, LONG acp_start, LONG acp_end,
                      LONG expected_left, LONG expected_top,
                      LONG expected_right, LONG expected_bottom) {
    RECT rect;
    BOOL clipped;
    EXPECT_EQ(S_OK, text_store_->GetTextExt(view_cookie, acp_start, acp_end,
                                            &rect, &clipped));
    EXPECT_EQ(expected_left, rect.left);
    EXPECT_EQ(expected_top, rect.top);
    EXPECT_EQ(expected_right, rect.right);
    EXPECT_EQ(expected_bottom, rect.bottom);
    EXPECT_EQ(FALSE, clipped);
  }

  void GetTextExtNoLayoutTest(TsViewCookie view_cookie, LONG acp_start,
                              LONG acp_end) {
    RECT rect;
    BOOL clipped;
    EXPECT_EQ(TS_E_NOLAYOUT,
              text_store_->GetTextExt(view_cookie, acp_start, acp_end,
                                      &rect, &clipped));
  }

  TSFTextStore* text_store_;
};

TEST_F(TSFTextStoreTest, GetStatusTest) {
  TS_STATUS status;
  EXPECT_EQ(S_OK, text_store_->GetStatus(&status));
  EXPECT_EQ(0, status.dwDynamicFlags);
  EXPECT_EQ(TS_SS_TRANSITORY | TS_SS_NOHIDDENTEXT, status.dwStaticFlags);
}


class SyncRequestLockTestCallback : public TSFTextStoreTestCallback {
 public:
  explicit SyncRequestLockTestCallback(TSFTextStore* text_store)
      : TSFTextStoreTestCallback(text_store) {
  }

  HRESULT LockGranted1(DWORD flags) {
    EXPECT_TRUE(HasReadLock());
    EXPECT_FALSE(HasReadWriteLock());
    return S_OK;
  }

  HRESULT LockGranted2(DWORD flags) {
    EXPECT_TRUE(HasReadLock());
    EXPECT_TRUE(HasReadWriteLock());
    return S_OK;
  }

  HRESULT LockGranted3(DWORD flags) {
    EXPECT_TRUE(HasReadLock());
    EXPECT_FALSE(HasReadWriteLock());
    HRESULT result;
    EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ | TS_LF_SYNC, &result));
    EXPECT_EQ(TS_E_SYNCHRONOUS, result);
    return S_OK;
  }

  HRESULT LockGranted4(DWORD flags) {
    EXPECT_TRUE(HasReadLock());
    EXPECT_FALSE(HasReadWriteLock());
    HRESULT result;
    EXPECT_EQ(S_OK,
              text_store_->RequestLock(TS_LF_READWRITE | TS_LF_SYNC, &result));
    EXPECT_EQ(TS_E_SYNCHRONOUS, result);
    return S_OK;
  }

  HRESULT LockGranted5(DWORD flags) {
    EXPECT_TRUE(HasReadLock());
    EXPECT_TRUE(HasReadWriteLock());
    HRESULT result;
    EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ | TS_LF_SYNC, &result));
    EXPECT_EQ(TS_E_SYNCHRONOUS, result);
    return S_OK;
  }

  HRESULT LockGranted6(DWORD flags) {
    EXPECT_TRUE(HasReadLock());
    EXPECT_TRUE(HasReadWriteLock());
    HRESULT result;
    EXPECT_EQ(S_OK,
              text_store_->RequestLock(TS_LF_READWRITE | TS_LF_SYNC, &result));
    EXPECT_EQ(TS_E_SYNCHRONOUS, result);
    return S_OK;
  }
};

TEST_F(TSFTextStoreTest, SynchronousRequestLockTest) {
  SyncRequestLockTestCallback callback(text_store_);
  EXPECT_CALL(*sink_, OnLockGranted(_))
      .WillOnce(Invoke(&callback, &SyncRequestLockTestCallback::LockGranted1))
      .WillOnce(Invoke(&callback, &SyncRequestLockTestCallback::LockGranted2))
      .WillOnce(Invoke(&callback, &SyncRequestLockTestCallback::LockGranted3))
      .WillOnce(Invoke(&callback, &SyncRequestLockTestCallback::LockGranted4))
      .WillOnce(Invoke(&callback, &SyncRequestLockTestCallback::LockGranted5))
      .WillOnce(Invoke(&callback, &SyncRequestLockTestCallback::LockGranted6));

  HRESULT result;
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ | TS_LF_SYNC, &result));
  EXPECT_EQ(S_OK, result);
  EXPECT_EQ(S_OK,
            text_store_->RequestLock(TS_LF_READWRITE | TS_LF_SYNC, &result));
  EXPECT_EQ(S_OK, result);

  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ | TS_LF_SYNC, &result));
  EXPECT_EQ(S_OK, result);
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ | TS_LF_SYNC, &result));
  EXPECT_EQ(S_OK, result);

  EXPECT_EQ(S_OK,
            text_store_->RequestLock(TS_LF_READWRITE | TS_LF_SYNC, &result));
  EXPECT_EQ(S_OK, result);
  EXPECT_EQ(S_OK,
            text_store_->RequestLock(TS_LF_READWRITE | TS_LF_SYNC, &result));
  EXPECT_EQ(S_OK, result);
}

class AsyncRequestLockTestCallback : public TSFTextStoreTestCallback {
 public:
  explicit AsyncRequestLockTestCallback(TSFTextStore* text_store)
      : TSFTextStoreTestCallback(text_store),
        state_(0) {
  }

  HRESULT LockGranted1(DWORD flags) {
    EXPECT_EQ(0, state_);
    state_ = 1;
    EXPECT_TRUE(HasReadLock());
    EXPECT_FALSE(HasReadWriteLock());
    HRESULT result;
    EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ, &result));
    EXPECT_EQ(TS_S_ASYNC, result);
    EXPECT_EQ(1, state_);
    state_ = 2;
    return S_OK;
  }

  HRESULT LockGranted2(DWORD flags) {
    EXPECT_EQ(2, state_);
    EXPECT_TRUE(HasReadLock());
    EXPECT_FALSE(HasReadWriteLock());
    HRESULT result;
    EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
    EXPECT_EQ(TS_S_ASYNC, result);
    EXPECT_EQ(2, state_);
    state_ = 3;
    return S_OK;
  }

  HRESULT LockGranted3(DWORD flags) {
    EXPECT_EQ(3, state_);
    EXPECT_TRUE(HasReadLock());
    EXPECT_TRUE(HasReadWriteLock());
    HRESULT result;
    EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
    EXPECT_EQ(TS_S_ASYNC, result);
    EXPECT_EQ(3, state_);
    state_ = 4;
    return S_OK;
  }

  HRESULT LockGranted4(DWORD flags) {
    EXPECT_EQ(4, state_);
    EXPECT_TRUE(HasReadLock());
    EXPECT_TRUE(HasReadWriteLock());
    HRESULT result;
    EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ, &result));
    EXPECT_EQ(TS_S_ASYNC, result);
    EXPECT_EQ(4, state_);
    state_ = 5;
    return S_OK;
  }

  HRESULT LockGranted5(DWORD flags) {
    EXPECT_EQ(5, state_);
    EXPECT_TRUE(HasReadLock());
    EXPECT_FALSE(HasReadWriteLock());
    state_ = 6;
    return S_OK;
  }

 private:
  int state_;
};

TEST_F(TSFTextStoreTest, AsynchronousRequestLockTest) {
  AsyncRequestLockTestCallback callback(text_store_);
  EXPECT_CALL(*sink_, OnLockGranted(_))
      .WillOnce(Invoke(&callback, &AsyncRequestLockTestCallback::LockGranted1))
      .WillOnce(Invoke(&callback, &AsyncRequestLockTestCallback::LockGranted2))
      .WillOnce(Invoke(&callback, &AsyncRequestLockTestCallback::LockGranted3))
      .WillOnce(Invoke(&callback, &AsyncRequestLockTestCallback::LockGranted4))
      .WillOnce(Invoke(&callback, &AsyncRequestLockTestCallback::LockGranted5));

  HRESULT result;
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ, &result));
  EXPECT_EQ(S_OK, result);
}

class RequestLockTextChangeTestCallback : public TSFTextStoreTestCallback {
 public:
  explicit RequestLockTextChangeTestCallback(TSFTextStore* text_store)
      : TSFTextStoreTestCallback(text_store),
        state_(0) {
  }

  HRESULT LockGranted1(DWORD flags) {
    EXPECT_EQ(0, state_);
    state_ = 1;
    EXPECT_TRUE(HasReadLock());
    EXPECT_TRUE(HasReadWriteLock());

    *edit_flag() = true;
    SetInternalState(L"012345", 6, 6, 6);
    composition_undelines()->clear();

    state_ = 2;
    return S_OK;
  }

  void InsertText(const string16& text) {
    EXPECT_EQ(2, state_);
    EXPECT_EQ(L"012345", text);
    state_ = 3;
  }

  void SetCompositionText(const ui::CompositionText& composition) {
    EXPECT_EQ(3, state_);
    EXPECT_EQ(L"", composition.text);
    EXPECT_EQ(0, composition.selection.start());
    EXPECT_EQ(0, composition.selection.end());
    EXPECT_EQ(0, composition.underlines.size());
    state_ = 4;
  }

  HRESULT OnTextChange(DWORD flags, const TS_TEXTCHANGE* change) {
    EXPECT_EQ(4, state_);
    HRESULT result;
    state_ = 5;
    EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
    EXPECT_EQ(S_OK, result);
    EXPECT_EQ(6, state_);
    state_ = 7;
    return S_OK;
  }

  HRESULT LockGranted2(DWORD flags) {
    EXPECT_EQ(5, state_);
    EXPECT_TRUE(HasReadLock());
    EXPECT_TRUE(HasReadWriteLock());
    state_ = 6;
    return S_OK;
  }

 private:
  int state_;
};

TEST_F(TSFTextStoreTest, RequestLockOnTextChangeTest) {
  RequestLockTextChangeTestCallback callback(text_store_);
  EXPECT_CALL(*sink_, OnLockGranted(_))
      .WillOnce(Invoke(&callback,
                       &RequestLockTextChangeTestCallback::LockGranted1))
      .WillOnce(Invoke(&callback,
                       &RequestLockTextChangeTestCallback::LockGranted2));

  EXPECT_CALL(*sink_, OnSelectionChange())
      .WillOnce(Return(S_OK));
  EXPECT_CALL(*sink_, OnLayoutChange(_, _))
      .WillOnce(Return(S_OK));
  EXPECT_CALL(*sink_, OnTextChange(_, _))
      .WillOnce(Invoke(&callback,
                       &RequestLockTextChangeTestCallback::OnTextChange));
  EXPECT_CALL(text_input_client_, InsertText(_))
      .WillOnce(Invoke(&callback,
                       &RequestLockTextChangeTestCallback::InsertText));
  EXPECT_CALL(text_input_client_, SetCompositionText(_))
      .WillOnce(Invoke(&callback,
                       &RequestLockTextChangeTestCallback::SetCompositionText));

  HRESULT result;
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
  EXPECT_EQ(S_OK, result);
}

class SelectionTestCallback : public TSFTextStoreTestCallback {
 public:
  explicit SelectionTestCallback(TSFTextStore* text_store)
      : TSFTextStoreTestCallback(text_store) {
  }

  HRESULT ReadLockGranted(DWORD flags) {
    SetInternalState(L"", 0, 0, 0);

    GetSelectionTest(0, 0);
    SetSelectionTest(0, 0, TF_E_NOLOCK);

    SetInternalState(L"012345", 0, 0, 3);

    GetSelectionTest(0, 3);
    SetSelectionTest(0, 0, TF_E_NOLOCK);

    return S_OK;
  }

  HRESULT ReadWriteLockGranted(DWORD flags) {
    SetInternalState(L"", 0, 0, 0);

    SetSelectionTest(0, 0, S_OK);
    GetSelectionTest(0, 0);
    SetSelectionTest(0, 1, TF_E_INVALIDPOS);
    SetSelectionTest(1, 0, TF_E_INVALIDPOS);
    SetSelectionTest(1, 1, TF_E_INVALIDPOS);

    SetInternalState(L"0123456", 3, 3, 3);

    SetSelectionTest(0, 0, TF_E_INVALIDPOS);
    SetSelectionTest(0, 1, TF_E_INVALIDPOS);
    SetSelectionTest(0, 3, TF_E_INVALIDPOS);
    SetSelectionTest(0, 6, TF_E_INVALIDPOS);
    SetSelectionTest(0, 7, TF_E_INVALIDPOS);
    SetSelectionTest(0, 8, TF_E_INVALIDPOS);

    SetSelectionTest(1, 0, TF_E_INVALIDPOS);
    SetSelectionTest(1, 1, TF_E_INVALIDPOS);
    SetSelectionTest(1, 3, TF_E_INVALIDPOS);
    SetSelectionTest(1, 6, TF_E_INVALIDPOS);
    SetSelectionTest(1, 7, TF_E_INVALIDPOS);
    SetSelectionTest(1, 8, TF_E_INVALIDPOS);

    SetSelectionTest(3, 0, TF_E_INVALIDPOS);
    SetSelectionTest(3, 1, TF_E_INVALIDPOS);
    SetSelectionTest(3, 3, S_OK);
    SetSelectionTest(3, 6, S_OK);
    SetSelectionTest(3, 7, S_OK);
    SetSelectionTest(3, 8, TF_E_INVALIDPOS);

    SetSelectionTest(6, 0, TF_E_INVALIDPOS);
    SetSelectionTest(6, 1, TF_E_INVALIDPOS);
    SetSelectionTest(6, 3, TF_E_INVALIDPOS);
    SetSelectionTest(6, 6, S_OK);
    SetSelectionTest(6, 7, S_OK);
    SetSelectionTest(6, 8, TF_E_INVALIDPOS);

    SetSelectionTest(7, 0, TF_E_INVALIDPOS);
    SetSelectionTest(7, 1, TF_E_INVALIDPOS);
    SetSelectionTest(7, 3, TF_E_INVALIDPOS);
    SetSelectionTest(7, 6, TF_E_INVALIDPOS);
    SetSelectionTest(7, 7, S_OK);
    SetSelectionTest(7, 8, TF_E_INVALIDPOS);

    SetSelectionTest(8, 0, TF_E_INVALIDPOS);
    SetSelectionTest(8, 1, TF_E_INVALIDPOS);
    SetSelectionTest(8, 3, TF_E_INVALIDPOS);
    SetSelectionTest(8, 6, TF_E_INVALIDPOS);
    SetSelectionTest(8, 7, TF_E_INVALIDPOS);
    SetSelectionTest(8, 8, TF_E_INVALIDPOS);

    return S_OK;
  }
};

TEST_F(TSFTextStoreTest, SetGetSelectionTest) {
  SelectionTestCallback callback(text_store_);
  EXPECT_CALL(*sink_, OnLockGranted(_))
      .WillOnce(Invoke(&callback, &SelectionTestCallback::ReadLockGranted))
      .WillOnce(Invoke(&callback,
                       &SelectionTestCallback::ReadWriteLockGranted));

  TS_SELECTION_ACP selection_buffer;
  ULONG fetched_count;
  EXPECT_EQ(TS_E_NOLOCK,
            text_store_->GetSelection(0, 1, &selection_buffer,
                                      &fetched_count));

  HRESULT result;
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ, &result));
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
}


class SetGetTextTestCallback : public TSFTextStoreTestCallback {
 public:
  explicit SetGetTextTestCallback(TSFTextStore* text_store)
      : TSFTextStoreTestCallback(text_store) {
  }

  HRESULT ReadLockGranted(DWORD flags) {
    SetTextTest(0, 0, L"", TF_E_NOLOCK);

    GetTextTest(0, -1, L"", 0);
    GetTextTest(0, 0, L"", 0);
    GetTextErrorTest(0, 1, TF_E_INVALIDPOS);

    SetInternalState(L"0123456", 3, 3, 3);

    GetTextErrorTest(-1, -1, TF_E_INVALIDPOS);
    GetTextErrorTest(-1, 0, TF_E_INVALIDPOS);
    GetTextErrorTest(-1, 1, TF_E_INVALIDPOS);
    GetTextErrorTest(-1, 3, TF_E_INVALIDPOS);
    GetTextErrorTest(-1, 6, TF_E_INVALIDPOS);
    GetTextErrorTest(-1, 7, TF_E_INVALIDPOS);
    GetTextErrorTest(-1, 8, TF_E_INVALIDPOS);

    GetTextTest(0, -1, L"0123456", 7);
    GetTextTest(0, 0, L"", 0);
    GetTextTest(0, 1, L"0", 1);
    GetTextTest(0, 3, L"012", 3);
    GetTextTest(0, 6, L"012345", 6);
    GetTextTest(0, 7, L"0123456", 7);
    GetTextErrorTest(0, 8, TF_E_INVALIDPOS);

    GetTextTest(1, -1, L"123456", 7);
    GetTextErrorTest(1, 0, TF_E_INVALIDPOS);
    GetTextTest(1, 1, L"", 1);
    GetTextTest(1, 3, L"12", 3);
    GetTextTest(1, 6, L"12345", 6);
    GetTextTest(1, 7, L"123456", 7);
    GetTextErrorTest(1, 8, TF_E_INVALIDPOS);

    GetTextTest(3, -1, L"3456", 7);
    GetTextErrorTest(3, 0, TF_E_INVALIDPOS);
    GetTextErrorTest(3, 1, TF_E_INVALIDPOS);
    GetTextTest(3, 3, L"", 3);
    GetTextTest(3, 6, L"345", 6);
    GetTextTest(3, 7, L"3456", 7);
    GetTextErrorTest(3, 8, TF_E_INVALIDPOS);

    GetTextTest(6, -1, L"6", 7);
    GetTextErrorTest(6, 0, TF_E_INVALIDPOS);
    GetTextErrorTest(6, 1, TF_E_INVALIDPOS);
    GetTextErrorTest(6, 3, TF_E_INVALIDPOS);
    GetTextTest(6, 6, L"", 6);
    GetTextTest(6, 7, L"6", 7);
    GetTextErrorTest(6, 8, TF_E_INVALIDPOS);

    GetTextTest(7, -1, L"", 7);
    GetTextErrorTest(7, 0, TF_E_INVALIDPOS);
    GetTextErrorTest(7, 1, TF_E_INVALIDPOS);
    GetTextErrorTest(7, 3, TF_E_INVALIDPOS);
    GetTextErrorTest(7, 6, TF_E_INVALIDPOS);
    GetTextTest(7, 7, L"", 7);
    GetTextErrorTest(7, 8, TF_E_INVALIDPOS);

    GetTextErrorTest(8, -1, TF_E_INVALIDPOS);
    GetTextErrorTest(8, 0, TF_E_INVALIDPOS);
    GetTextErrorTest(8, 1, TF_E_INVALIDPOS);
    GetTextErrorTest(8, 3, TF_E_INVALIDPOS);
    GetTextErrorTest(8, 6, TF_E_INVALIDPOS);
    GetTextErrorTest(8, 7, TF_E_INVALIDPOS);
    GetTextErrorTest(8, 8, TF_E_INVALIDPOS);

    return S_OK;
  }

  HRESULT ReadWriteLockGranted(DWORD flags) {
    SetInternalState(L"", 0, 0, 0);
    SetTextTest(0, 0, L"", S_OK);

    SetInternalState(L"", 0, 0, 0);
    SetTextTest(0, 1, L"", TS_E_INVALIDPOS);

    SetInternalState(L"0123456", 3, 3, 3);

    SetTextTest(0, 0, L"", TS_E_INVALIDPOS);
    SetTextTest(0, 1, L"", TS_E_INVALIDPOS);
    SetTextTest(0, 3, L"", TS_E_INVALIDPOS);
    SetTextTest(0, 6, L"", TS_E_INVALIDPOS);
    SetTextTest(0, 7, L"", TS_E_INVALIDPOS);
    SetTextTest(0, 8, L"", TS_E_INVALIDPOS);

    SetTextTest(1, 0, L"", TS_E_INVALIDPOS);
    SetTextTest(1, 1, L"", TS_E_INVALIDPOS);
    SetTextTest(1, 3, L"", TS_E_INVALIDPOS);
    SetTextTest(1, 6, L"", TS_E_INVALIDPOS);
    SetTextTest(1, 7, L"", TS_E_INVALIDPOS);
    SetTextTest(1, 8, L"", TS_E_INVALIDPOS);

    SetTextTest(3, 0, L"", TS_E_INVALIDPOS);
    SetTextTest(3, 1, L"", TS_E_INVALIDPOS);

    SetTextTest(3, 3, L"", S_OK);
    GetTextTest(0, -1, L"0123456", 7);
    GetSelectionTest(3, 3);
    SetInternalState(L"0123456", 3, 3, 3);

    SetTextTest(3, 6, L"", S_OK);
    GetTextTest(0, -1, L"0126", 4);
    GetSelectionTest(3, 3);
    SetInternalState(L"0123456", 3, 3, 3);

    SetTextTest(3, 7, L"", S_OK);
    GetTextTest(0, -1, L"012", 3);
    GetSelectionTest(3, 3);
    SetInternalState(L"0123456", 3, 3, 3);

    SetTextTest(3, 8, L"", TS_E_INVALIDPOS);

    SetTextTest(6, 0, L"", TS_E_INVALIDPOS);
    SetTextTest(6, 1, L"", TS_E_INVALIDPOS);
    SetTextTest(6, 3, L"", TS_E_INVALIDPOS);

    SetTextTest(6, 6, L"", S_OK);
    GetTextTest(0, -1, L"0123456", 7);
    GetSelectionTest(6, 6);
    SetInternalState(L"0123456", 3, 3, 3);

    SetTextTest(6, 7, L"", S_OK);
    GetTextTest(0, -1, L"012345", 6);
    GetSelectionTest(6, 6);
    SetInternalState(L"0123456", 3, 3, 3);

    SetTextTest(6, 8, L"", TS_E_INVALIDPOS);

    SetTextTest(7, 0, L"", TS_E_INVALIDPOS);
    SetTextTest(7, 1, L"", TS_E_INVALIDPOS);
    SetTextTest(7, 3, L"", TS_E_INVALIDPOS);
    SetTextTest(7, 6, L"", TS_E_INVALIDPOS);

    SetTextTest(7, 7, L"", S_OK);
    GetTextTest(0, -1, L"0123456", 7);
    GetSelectionTest(7, 7);
    SetInternalState(L"0123456", 3, 3, 3);

    SetTextTest(7, 8, L"", TS_E_INVALIDPOS);

    SetInternalState(L"0123456", 3, 3, 3);
    SetTextTest(3, 3, L"abc", S_OK);
    GetTextTest(0, -1, L"012abc3456", 10);
    GetSelectionTest(3, 6);

    SetInternalState(L"0123456", 3, 3, 3);
    SetTextTest(3, 6, L"abc", S_OK);
    GetTextTest(0, -1, L"012abc6", 7);
    GetSelectionTest(3, 6);

    SetInternalState(L"0123456", 3, 3, 3);
    SetTextTest(3, 7, L"abc", S_OK);
    GetTextTest(0, -1, L"012abc", 6);
    GetSelectionTest(3, 6);

    SetInternalState(L"0123456", 3, 3, 3);
    SetTextTest(6, 6, L"abc", S_OK);
    GetTextTest(0, -1, L"012345abc6", 10);
    GetSelectionTest(6, 9);

    SetInternalState(L"0123456", 3, 3, 3);
    SetTextTest(6, 7, L"abc", S_OK);
    GetTextTest(0, -1, L"012345abc", 9);
    GetSelectionTest(6, 9);

    SetInternalState(L"0123456", 3, 3, 3);
    SetTextTest(7, 7, L"abc", S_OK);
    GetTextTest(0, -1, L"0123456abc", 10);
    GetSelectionTest(7, 10);

    return S_OK;
  }
};

TEST_F(TSFTextStoreTest, SetGetTextTest) {
  SetGetTextTestCallback callback(text_store_);
  EXPECT_CALL(*sink_, OnLockGranted(_))
      .WillOnce(Invoke(&callback, &SetGetTextTestCallback::ReadLockGranted))
      .WillOnce(Invoke(&callback,
                       &SetGetTextTestCallback::ReadWriteLockGranted));

  wchar_t buffer[1024];
  ULONG text_buffer_copied;
  TS_RUNINFO run_info;
  ULONG run_info_buffer_copied;
  LONG next_acp;
  EXPECT_EQ(TF_E_NOLOCK,
            text_store_->GetText(0, -1, buffer, 1024, &text_buffer_copied,
                                 &run_info, 1, &run_info_buffer_copied,
                                 &next_acp));
  TS_TEXTCHANGE change;
  EXPECT_EQ(TF_E_NOLOCK, text_store_->SetText(0, 0, 0, L"abc", 3, &change));

  HRESULT result;
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ, &result));
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
}

class InsertTextAtSelectionTestCallback : public TSFTextStoreTestCallback {
 public:
  explicit InsertTextAtSelectionTestCallback(TSFTextStore* text_store)
      : TSFTextStoreTestCallback(text_store) {
  }

  HRESULT ReadLockGranted(DWORD flags) {
    const wchar_t kBuffer[] = L"0123456789";

    SetInternalState(L"abcedfg", 0, 0, 0);
    InsertTextAtSelectionQueryOnlyTest(kBuffer, 10, 0, 0);
    GetSelectionTest(0, 0);
    InsertTextAtSelectionQueryOnlyTest(kBuffer, 0, 0, 0);

    SetInternalState(L"abcedfg", 0, 2, 5);
    InsertTextAtSelectionQueryOnlyTest(kBuffer, 10, 2, 5);
    GetSelectionTest(2, 5);
    InsertTextAtSelectionQueryOnlyTest(kBuffer, 0, 2, 5);

    LONG start, end;
    TS_TEXTCHANGE change;
    EXPECT_EQ(TS_E_NOLOCK,
              text_store_->InsertTextAtSelection(0, kBuffer, 10,
                                                 &start, &end, &change));
    return S_OK;
  }

  HRESULT ReadWriteLockGranted(DWORD flags) {
    SetInternalState(L"abcedfg", 0, 0, 0);

    const wchar_t kBuffer[] = L"0123456789";
    InsertTextAtSelectionQueryOnlyTest(kBuffer, 10, 0, 0);
    GetSelectionTest(0, 0);
    InsertTextAtSelectionQueryOnlyTest(kBuffer, 0, 0, 0);

    SetInternalState(L"", 0, 0, 0);
    InsertTextAtSelectionTest(kBuffer, 10, 0, 10, 0, 0, 10);
    GetSelectionTest(0, 10);
    GetTextTest(0, -1, L"0123456789", 10);

    SetInternalState(L"abcedfg", 0, 0, 0);
    InsertTextAtSelectionTest(kBuffer, 10, 0, 10, 0, 0, 10);
    GetSelectionTest(0, 10);
    GetTextTest(0, -1, L"0123456789abcedfg", 17);

    SetInternalState(L"abcedfg", 0, 0, 3);
    InsertTextAtSelectionTest(kBuffer, 0, 0, 0, 0, 3, 0);
    GetSelectionTest(0, 0);
    GetTextTest(0, -1, L"edfg", 4);

    SetInternalState(L"abcedfg", 0, 3, 7);
    InsertTextAtSelectionTest(kBuffer, 10, 3, 13, 3, 7, 13);
    GetSelectionTest(3, 13);
    GetTextTest(0, -1, L"abc0123456789", 13);

    SetInternalState(L"abcedfg", 0, 7, 7);
    InsertTextAtSelectionTest(kBuffer, 10, 7, 17, 7, 7, 17);
    GetSelectionTest(7, 17);
    GetTextTest(0, -1, L"abcedfg0123456789", 17);

    return S_OK;
  }
};

TEST_F(TSFTextStoreTest, InsertTextAtSelectionTest) {
  InsertTextAtSelectionTestCallback callback(text_store_);
  EXPECT_CALL(*sink_, OnLockGranted(_))
      .WillOnce(Invoke(&callback,
                       &InsertTextAtSelectionTestCallback::ReadLockGranted))
      .WillOnce(
          Invoke(&callback,
                 &InsertTextAtSelectionTestCallback::ReadWriteLockGranted));

  HRESULT result;
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ, &result));
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
}

class ScenarioTestCallback : public TSFTextStoreTestCallback {
 public:
  explicit ScenarioTestCallback(TSFTextStore* text_store)
      : TSFTextStoreTestCallback(text_store) {
  }

  HRESULT LockGranted1(DWORD flags) {
    SetSelectionTest(0, 0, S_OK);

    SetTextTest(0, 0, L"abc", S_OK);
    SetTextTest(1, 2, L"xyz", S_OK);

    GetTextTest(0, -1, L"axyzc", 5);

    composition_undelines()->clear();
    CompositionUnderline underline;
    underline.start_offset = 0;
    underline.end_offset = 5;
    underline.color = SK_ColorBLACK;
    underline.thick = false;
    composition_undelines()->push_back(underline);
    *edit_flag() = true;
    *committed_size() = 0;
    return S_OK;
  }

  void SetCompositionText1(const ui::CompositionText& composition) {
    EXPECT_EQ(L"axyzc", composition.text);
    EXPECT_EQ(1, composition.selection.start());
    EXPECT_EQ(4, composition.selection.end());
    ASSERT_EQ(1, composition.underlines.size());
    EXPECT_EQ(SK_ColorBLACK, composition.underlines[0].color);
    EXPECT_EQ(0, composition.underlines[0].start_offset);
    EXPECT_EQ(5, composition.underlines[0].end_offset);
    EXPECT_FALSE(composition.underlines[0].thick);
  }

  HRESULT LockGranted2(DWORD flags) {
    SetTextTest(3, 4, L"ZCP", S_OK);
    GetTextTest(0, -1, L"axyZCPc", 7);

    composition_undelines()->clear();
    CompositionUnderline underline;
    underline.start_offset = 3;
    underline.end_offset = 5;
    underline.color = SK_ColorBLACK;
    underline.thick = true;
    composition_undelines()->push_back(underline);
    underline.start_offset = 5;
    underline.end_offset = 7;
    underline.color = SK_ColorBLACK;
    underline.thick = false;
    composition_undelines()->push_back(underline);

    *edit_flag() = true;
    *committed_size() = 3;

    return S_OK;
  }

  void InsertText2(const string16& text) {
    EXPECT_EQ(L"axy", text);
  }

  void SetCompositionText2(const ui::CompositionText& composition) {
    EXPECT_EQ(L"ZCPc", composition.text);
    EXPECT_EQ(0, composition.selection.start());
    EXPECT_EQ(3, composition.selection.end());
    ASSERT_EQ(2, composition.underlines.size());
    EXPECT_EQ(SK_ColorBLACK, composition.underlines[0].color);
    EXPECT_EQ(0, composition.underlines[0].start_offset);
    EXPECT_EQ(2, composition.underlines[0].end_offset);
    EXPECT_TRUE(composition.underlines[0].thick);
    EXPECT_EQ(SK_ColorBLACK, composition.underlines[1].color);
    EXPECT_EQ(2, composition.underlines[1].start_offset);
    EXPECT_EQ(4, composition.underlines[1].end_offset);
    EXPECT_FALSE(composition.underlines[1].thick);
  }

  HRESULT LockGranted3(DWORD flags) {
    GetTextTest(0, -1, L"axyZCPc", 7);

    composition_undelines()->clear();
    *edit_flag() = true;
    *committed_size() = 7;

    return S_OK;
  }

  void InsertText3(const string16& text) {
    EXPECT_EQ(L"ZCPc", text);
  }

  void SetCompositionText3(const ui::CompositionText& composition) {
    EXPECT_EQ(L"", composition.text);
    EXPECT_EQ(0, composition.selection.start());
    EXPECT_EQ(0, composition.selection.end());
    EXPECT_EQ(0, composition.underlines.size());
  }
};

TEST_F(TSFTextStoreTest, ScenarioTest) {
  ScenarioTestCallback callback(text_store_);
  EXPECT_CALL(text_input_client_, SetCompositionText(_))
      .WillOnce(Invoke(&callback, &ScenarioTestCallback::SetCompositionText1))
      .WillOnce(Invoke(&callback, &ScenarioTestCallback::SetCompositionText2))
      .WillOnce(Invoke(&callback, &ScenarioTestCallback::SetCompositionText3));

  EXPECT_CALL(text_input_client_, InsertText(_))
      .WillOnce(Invoke(&callback, &ScenarioTestCallback::InsertText2))
      .WillOnce(Invoke(&callback, &ScenarioTestCallback::InsertText3));

  EXPECT_CALL(*sink_, OnLockGranted(_))
      .WillOnce(Invoke(&callback, &ScenarioTestCallback::LockGranted1))
      .WillOnce(Invoke(&callback, &ScenarioTestCallback::LockGranted2))
      .WillOnce(Invoke(&callback, &ScenarioTestCallback::LockGranted3));

  // OnSelectionChange will be called once after LockGranted3().
  EXPECT_CALL(*sink_, OnSelectionChange())
      .WillOnce(Return(S_OK));

  // OnLayoutChange will be called once after LockGranted3().
  EXPECT_CALL(*sink_, OnLayoutChange(_, _))
      .WillOnce(Return(S_OK));

  // OnTextChange will be called once after LockGranted3().
  EXPECT_CALL(*sink_, OnTextChange(_, _))
      .WillOnce(Return(S_OK));

  HRESULT result;
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READWRITE, &result));
}

class GetTextExtTestCallback : public TSFTextStoreTestCallback {
 public:
  explicit GetTextExtTestCallback(TSFTextStore* text_store)
      : TSFTextStoreTestCallback(text_store),
        layout_prepared_character_num_(0) {
  }

  HRESULT LockGranted(DWORD flags) {
    SetInternalState(L"0123456789012", 0, 0, 0);
    layout_prepared_character_num_ = 13;

    TsViewCookie view_cookie;
    EXPECT_EQ(S_OK, text_store_->GetActiveView(&view_cookie));
    GetTextExtTest(view_cookie, 0, 0, 11, 12, 11, 20);
    GetTextExtTest(view_cookie, 0, 1, 11, 12, 20, 20);
    GetTextExtTest(view_cookie, 0, 2, 11, 12, 30, 20);
    GetTextExtTest(view_cookie, 9, 9, 100, 12, 100, 20);
    GetTextExtTest(view_cookie, 9, 10, 101, 12, 110, 20);
    GetTextExtTest(view_cookie, 10, 10, 110, 12, 110, 20);
    GetTextExtTest(view_cookie, 11, 11, 20, 112, 20, 120);
    GetTextExtTest(view_cookie, 11, 12, 21, 112, 30, 120);
    GetTextExtTest(view_cookie, 9, 12, 101, 12, 30, 120);
    GetTextExtTest(view_cookie, 9, 13, 101, 12, 40, 120);
    GetTextExtTest(view_cookie, 0, 13, 11, 12, 40, 120);
    GetTextExtTest(view_cookie, 13, 13, 40, 112, 40, 120);

    layout_prepared_character_num_ = 12;
    GetTextExtNoLayoutTest(view_cookie, 13, 13);

    layout_prepared_character_num_ = 0;
    GetTextExtNoLayoutTest(view_cookie, 0, 0);

    SetInternalState(L"", 0, 0, 0);
    GetTextExtTest(view_cookie, 0, 0, 1, 2, 4, 6);

    // Last character is not availabe due to timing issue of async API.
    // In this case, we will get first character bounds instead of whole text
    // bounds.
    SetInternalState(L"abc", 0, 0, 3);
    layout_prepared_character_num_ = 2;
    GetTextExtTest(view_cookie, 0, 0, 11, 12, 11, 20);

    // TODO(nona, kinaba): Remove following test case after PPAPI supporting
    // GetCompositionCharacterBounds.
    SetInternalState(L"a", 0, 0, 1);
    layout_prepared_character_num_ = 0;
    GetTextExtTest(view_cookie, 0, 1, 1, 2, 4, 6);
    return S_OK;
  }

  bool GetCompositionCharacterBounds(uint32 index, gfx::Rect* rect) {
    if (index >= layout_prepared_character_num_)
      return false;
    rect->set_x((index % 10) * 10 + 11);
    rect->set_y((index / 10) * 100 + 12);
    rect->set_width(9);
    rect->set_height(8);
    return true;
  }

  gfx::Rect GetCaretBounds() {
    return gfx::Rect(1, 2, 3, 4);
  }

 private:
  uint32 layout_prepared_character_num_;
};

TEST_F(TSFTextStoreTest, GetTextExtTest) {
  GetTextExtTestCallback callback(text_store_);
  EXPECT_CALL(text_input_client_, GetCaretBounds())
      .WillRepeatedly(Invoke(&callback,
                             &GetTextExtTestCallback::GetCaretBounds));

  EXPECT_CALL(text_input_client_, GetCompositionCharacterBounds(_, _))
      .WillRepeatedly(
          Invoke(&callback,
                 &GetTextExtTestCallback::GetCompositionCharacterBounds));

  EXPECT_CALL(*sink_, OnLockGranted(_))
      .WillOnce(Invoke(&callback, &GetTextExtTestCallback::LockGranted));

  HRESULT result;
  EXPECT_EQ(S_OK, text_store_->RequestLock(TS_LF_READ, &result));
}

}  // namespace ui
