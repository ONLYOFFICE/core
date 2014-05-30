// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ime/win/tsf_event_router.h"

#include <msctf.h>
#include <set>
#include <utility>

#include "base/bind.h"
#include "base/win/scoped_comptr.h"
#include "base/win/metro.h"
#include "ui/base/range/range.h"
#include "ui/base/win/atl_module.h"

namespace ui {


// TSFEventRouter::Delegate  ------------------------------------

// The implementation class of ITfUIElementSink, whose member functions will be
// called back by TSF when the UI element status is changed, for example when
// the candidate window is opened or closed. This class also implements
// ITfTextEditSink, whose member function is called back by TSF when the text
// editting session is finished.
class ATL_NO_VTABLE TSFEventRouter::Delegate
    : public ATL::CComObjectRootEx<CComSingleThreadModel>,
      public ITfUIElementSink,
      public ITfTextEditSink {
 public:
  BEGIN_COM_MAP(Delegate)
    COM_INTERFACE_ENTRY(ITfUIElementSink)
    COM_INTERFACE_ENTRY(ITfTextEditSink)
  END_COM_MAP()

  Delegate();
  ~Delegate();

  // ITfTextEditSink:
  STDMETHOD(OnEndEdit)(ITfContext* context, TfEditCookie read_only_cookie,
                       ITfEditRecord* edit_record) OVERRIDE;

  // ITfUiElementSink:
  STDMETHOD(BeginUIElement)(DWORD element_id, BOOL* is_show) OVERRIDE;
  STDMETHOD(UpdateUIElement)(DWORD element_id) OVERRIDE;
  STDMETHOD(EndUIElement)(DWORD element_id) OVERRIDE;

  // Sets |thread_manager| to be monitored. |thread_manager| can be NULL.
  void SetManager(ITfThreadMgr* thread_manager);

  // Returns true if the IME is composing text.
  bool IsImeComposing();

  // Sets |router| to be forwarded TSF-related events.
  void SetRouter(TSFEventRouter* router);

 private:
  // Returns current composition range. Returns ui::Range::InvalidRange if there
  // is no composition.
  static ui::Range GetCompositionRange(ITfContext* context);

  // Returns true if the given |element_id| represents the candidate window.
  bool IsCandidateWindowInternal(DWORD element_id);

  // A context associated with this class.
  base::win::ScopedComPtr<ITfContext> context_;

  // The ITfSource associated with |context_|.
  base::win::ScopedComPtr<ITfSource> context_source_;

  // The cookie for |context_source_|.
  DWORD context_source_cookie_;

  // A UIElementMgr associated with this class.
  base::win::ScopedComPtr<ITfUIElementMgr> ui_element_manager_;

  // The ITfSouce associated with |ui_element_manager_|.
  base::win::ScopedComPtr<ITfSource> ui_source_;

  // The set of currently opened candidate window ids.
  std::set<DWORD> open_candidate_window_ids_;

  // The cookie for |ui_source_|.
  DWORD ui_source_cookie_;

  TSFEventRouter* router_;
  ui::Range previous_composition_range_;

  DISALLOW_COPY_AND_ASSIGN(Delegate);
};

TSFEventRouter::Delegate::Delegate()
    : context_source_cookie_(TF_INVALID_COOKIE),
      ui_source_cookie_(TF_INVALID_COOKIE),
      router_(NULL),
      previous_composition_range_(ui::Range::InvalidRange()) {
}

TSFEventRouter::Delegate::~Delegate() {}

void TSFEventRouter::Delegate::SetRouter(TSFEventRouter* router) {
  router_ = router;
}

STDMETHODIMP TSFEventRouter::Delegate::OnEndEdit(ITfContext* context,
                                                 TfEditCookie read_only_cookie,
                                                 ITfEditRecord* edit_record) {
  if (!edit_record || !context)
    return E_INVALIDARG;
  if (!router_)
    return S_OK;

  // |edit_record| can be used to obtain updated ranges in terms of text
  // contents and/or text attributes. Here we are interested only in text update
  // so we use TF_GTP_INCL_TEXT and check if there is any range which contains
  // updated text.
  base::win::ScopedComPtr<IEnumTfRanges> ranges;
  if (FAILED(edit_record->GetTextAndPropertyUpdates(TF_GTP_INCL_TEXT, NULL, 0,
                                                    ranges.Receive())))
     return S_OK;  // Don't care about failures.

  ULONG fetched_count = 0;
  base::win::ScopedComPtr<ITfRange> range;
  if (FAILED(ranges->Next(1, range.Receive(), &fetched_count)))
    return S_OK;  // Don't care about failures.

  const ui::Range composition_range = GetCompositionRange(context);

  if (!previous_composition_range_.IsValid() && composition_range.IsValid())
    router_->OnTSFStartComposition();

  // |fetched_count| != 0 means there is at least one range that contains
  // updated text.
  if (fetched_count != 0)
    router_->OnTextUpdated(composition_range);

  if (previous_composition_range_.IsValid() && !composition_range.IsValid())
    router_->OnTSFEndComposition();

  previous_composition_range_ = composition_range;
  return S_OK;
}

STDMETHODIMP TSFEventRouter::Delegate::BeginUIElement(DWORD element_id,
                                                      BOOL* is_show) {
  if (is_show)
    *is_show = TRUE;  // Without this the UI element will not be shown.

  if (!IsCandidateWindowInternal(element_id))
    return S_OK;

  std::pair<std::set<DWORD>::iterator, bool> insert_result =
      open_candidate_window_ids_.insert(element_id);
  // Don't call if |router_| is null or |element_id| is already handled.
  if (router_ && insert_result.second)
    router_->OnCandidateWindowCountChanged(open_candidate_window_ids_.size());

  return S_OK;
}

STDMETHODIMP TSFEventRouter::Delegate::UpdateUIElement(
    DWORD element_id) {
  return S_OK;
}

STDMETHODIMP TSFEventRouter::Delegate::EndUIElement(
    DWORD element_id) {
  if ((open_candidate_window_ids_.erase(element_id) != 0) && router_)
    router_->OnCandidateWindowCountChanged(open_candidate_window_ids_.size());
  return S_OK;
}

void TSFEventRouter::Delegate::SetManager(
    ITfThreadMgr* thread_manager) {
  context_.Release();

  if (context_source_) {
    context_source_->UnadviseSink(context_source_cookie_);
    context_source_.Release();
  }
  context_source_cookie_ = TF_INVALID_COOKIE;

  ui_element_manager_.Release();
  if (ui_source_) {
    ui_source_->UnadviseSink(ui_source_cookie_);
    ui_source_.Release();
  }
  ui_source_cookie_ = TF_INVALID_COOKIE;

  if (!thread_manager)
    return;

  base::win::ScopedComPtr<ITfDocumentMgr> document_manager;
  if (FAILED(thread_manager->GetFocus(document_manager.Receive())) ||
      FAILED(document_manager->GetBase(context_.Receive())) ||
      FAILED(context_source_.QueryFrom(context_)))
    return;
  context_source_->AdviseSink(IID_ITfTextEditSink,
                              static_cast<ITfTextEditSink*>(this),
                              &context_source_cookie_);

  if (FAILED(ui_element_manager_.QueryFrom(thread_manager)) ||
      FAILED(ui_source_.QueryFrom(ui_element_manager_)))
    return;
  ui_source_->AdviseSink(IID_ITfUIElementSink,
                         static_cast<ITfUIElementSink*>(this),
                         &ui_source_cookie_);
}

bool TSFEventRouter::Delegate::IsImeComposing() {
  return context_ && GetCompositionRange(context_).IsValid();
}

// static
ui::Range TSFEventRouter::Delegate::GetCompositionRange(
    ITfContext* context) {
  DCHECK(context);
  base::win::ScopedComPtr<ITfContextComposition> context_composition;
  if (FAILED(context_composition.QueryFrom(context)))
    return ui::Range::InvalidRange();
  base::win::ScopedComPtr<IEnumITfCompositionView> enum_composition_view;
  if (FAILED(context_composition->EnumCompositions(
      enum_composition_view.Receive())))
    return ui::Range::InvalidRange();
  base::win::ScopedComPtr<ITfCompositionView> composition_view;
  if (enum_composition_view->Next(1, composition_view.Receive(),
                                  NULL) != S_OK)
    return ui::Range::InvalidRange();

  base::win::ScopedComPtr<ITfRange> range;
  if (FAILED(composition_view->GetRange(range.Receive())))
    return ui::Range::InvalidRange();

  base::win::ScopedComPtr<ITfRangeACP> range_acp;
  if (FAILED(range_acp.QueryFrom(range)))
    return ui::Range::InvalidRange();

  LONG start = 0;
  LONG length = 0;
  if (FAILED(range_acp->GetExtent(&start, &length)))
    return ui::Range::InvalidRange();

  return ui::Range(start, start + length);
}

bool TSFEventRouter::Delegate::IsCandidateWindowInternal(DWORD element_id) {
  DCHECK(ui_element_manager_.get());
  base::win::ScopedComPtr<ITfUIElement> ui_element;
  if (FAILED(ui_element_manager_->GetUIElement(element_id,
                                               ui_element.Receive())))
    return false;
  base::win::ScopedComPtr<ITfCandidateListUIElement> candidate_list_ui_element;
  return SUCCEEDED(candidate_list_ui_element.QueryFrom(ui_element));
}


// TSFEventRouter  ------------------------------------------------------------

TSFEventRouter::TSFEventRouter(TSFEventRouterObserver* observer)
    : observer_(observer),
      delegate_(NULL) {
  DCHECK(base::win::IsTSFAwareRequired())
      << "Do not use TSFEventRouter without TSF environment.";
  DCHECK(observer_);
  CComObject<Delegate>* delegate;
  ui::win::CreateATLModuleIfNeeded();
  if (SUCCEEDED(CComObject<Delegate>::CreateInstance(&delegate))) {
    delegate->AddRef();
    delegate_.Attach(delegate);
    delegate_->SetRouter(this);
  }
}

TSFEventRouter::~TSFEventRouter() {
  if (delegate_) {
    delegate_->SetManager(NULL);
    delegate_->SetRouter(NULL);
  }
}

bool TSFEventRouter::IsImeComposing() {
  return delegate_->IsImeComposing();
}

void TSFEventRouter::OnCandidateWindowCountChanged(size_t window_count) {
  observer_->OnCandidateWindowCountChanged(window_count);
}

void TSFEventRouter::OnTSFStartComposition() {
  observer_->OnTSFStartComposition();
}

void TSFEventRouter::OnTextUpdated(const ui::Range& composition_range) {
  observer_->OnTextUpdated(composition_range);
}

void TSFEventRouter::OnTSFEndComposition() {
  observer_->OnTSFEndComposition();
}

void TSFEventRouter::SetManager(ITfThreadMgr* thread_manager) {
  delegate_->SetManager(thread_manager);
}

}  // namespace ui
