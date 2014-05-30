// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <msctf.h>

#include <map>

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/message_loop.h"
#include "base/threading/thread_local_storage.h"
#include "base/win/scoped_comptr.h"
#include "base/win/scoped_variant.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/base/ime/win/tsf_bridge.h"
#include "ui/base/ime/win/tsf_text_store.h"

namespace ui {

namespace {

// We use thread local storage for TSFBridge lifespan management.
base::ThreadLocalStorage::StaticSlot tls_tsf_bridge = TLS_INITIALIZER;


// TsfBridgeDelegate -----------------------------------------------------------

// A TLS implementation of TSFBridge.
class TSFBridgeDelegate : public TSFBridge {
 public:
  TSFBridgeDelegate();
  virtual ~TSFBridgeDelegate();

  bool Initialize();

  // TsfBridge:
  virtual void Shutdown() OVERRIDE;
  virtual void OnTextInputTypeChanged(TextInputClient* client) OVERRIDE;
  virtual bool CancelComposition() OVERRIDE;
  virtual void SetFocusedClient(HWND focused_window,
                                TextInputClient* client) OVERRIDE;
  virtual void RemoveFocusedClient(TextInputClient* client) OVERRIDE;
  virtual base::win::ScopedComPtr<ITfThreadMgr> GetThreadManager() OVERRIDE;
  virtual TextInputClient* GetFocusedTextInputClient() const OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<TSFBridgeDelegate>;

  // Returns true if |tsf_document_map_| is successfully initialized. This
  // method should be called from and only from Initialize().
  bool InitializeDocumentMapInternal();

  // Returns true if |context| is successfully updated to be a disabled
  // context, where an IME should be deactivated. This is suitable for some
  // special input context such as password fields.
  bool InitializeDisabledContext(ITfContext* context);

  // Returns true if a TSF document manager and a TSF context is successfully
  // created with associating with given |text_store|. The returned
  // |source_cookie| indicates the binding between |text_store| and |context|.
  // You can pass NULL to |text_store| and |source_cookie| when text store is
  // not necessary.
  bool CreateDocumentManager(TSFTextStore* text_store,
                             ITfDocumentMgr** document_manager,
                             ITfContext** context,
                             DWORD* source_cookie);

  // Returns true if |document_manager| is the focused document manager.
  bool IsFocused(ITfDocumentMgr* document_manager);

  // Returns true if already initialized.
  bool IsInitialized();

  // Returns an instance of ITfDocumentMgr that is associated with the
  // current TextInputType of |client_|.
  base::win::ScopedComPtr<ITfDocumentMgr> GetAssociatedDocumentManager();

  // An ITfThreadMgr object to be used in focus and document management.
  base::win::ScopedComPtr<ITfThreadMgr> thread_manager_;

  // A triple of document manager, text store and binding cookie between
  // a context owned by the document manager and the text store. This is a
  // minimum working set of an editable document in TSF.
  struct TSFDocument {
   public:
    TSFDocument() : cookie(TF_INVALID_COOKIE) {}
    TSFDocument(const TSFDocument& src)
        : document_manager(src.document_manager),
          cookie(src.cookie) {}
    base::win::ScopedComPtr<ITfDocumentMgr> document_manager;
    scoped_refptr<TSFTextStore> text_store;
    DWORD cookie;
  };

  // A map from TextInputType to an editable document for TSF. We use multiple
  // TSF documents that have different InputScopes and TSF attributes based on
  // the TextInputType associated with the target document. For a TextInputType
  // that is not coverted by this map, a default document, e.g. the document
  // for TEXT_INPUT_TYPE_TEXT, should be used.
  // Note that some IMEs don't change their state unless the document focus is
  // changed. This is why we use multiple documents instead of changing TSF
  // metadata of a single document on the fly.
  typedef std::map<TextInputType, TSFDocument> TSFDocumentMap;
  TSFDocumentMap tsf_document_map_;

  // An identifier of TSF client.
  TfClientId client_id_;

  // Current focused text input client. Do not free |client_|.
  TextInputClient* client_;

  DISALLOW_COPY_AND_ASSIGN(TSFBridgeDelegate);
};

TSFBridgeDelegate::TSFBridgeDelegate()
    : client_id_(TF_CLIENTID_NULL),
      client_(NULL) {
}

TSFBridgeDelegate::~TSFBridgeDelegate() {
}

bool TSFBridgeDelegate::Initialize() {
  DCHECK_EQ(base::MessageLoop::TYPE_UI, base::MessageLoop::current()->type());
  if (client_id_ != TF_CLIENTID_NULL) {
    DVLOG(1) << "Already initialized.";
    return false;
  }

  if (FAILED(thread_manager_.CreateInstance(CLSID_TF_ThreadMgr))) {
    DVLOG(1) << "Failed to create ThreadManager instance.";
    return false;
  }

  if (FAILED(thread_manager_->Activate(&client_id_))) {
    DVLOG(1) << "Failed to activate Thread Manager.";
    return false;
  }

  if (!InitializeDocumentMapInternal())
    return false;

  // Japanese IME expects the default value of this compartment is
  // TF_SENTENCEMODE_PHRASEPREDICT like IMM32 implementation. This value is
  // managed per thread, so that it is enough to set this value at once. This
  // value does not affect other language's IME behaviors.
  base::win::ScopedComPtr<ITfCompartmentMgr> thread_compartment_manager;
  if (FAILED(thread_compartment_manager.QueryFrom(thread_manager_))) {
    DVLOG(1) << "Failed to get ITfCompartmentMgr.";
    return false;
  }

  base::win::ScopedComPtr<ITfCompartment> sentence_compartment;
  if (FAILED(thread_compartment_manager->GetCompartment(
      GUID_COMPARTMENT_KEYBOARD_INPUTMODE_SENTENCE,
      sentence_compartment.Receive()))) {
    DVLOG(1) << "Failed to get sentence compartment.";
    return false;
  }

  base::win::ScopedVariant sentence_variant;
  sentence_variant.Set(TF_SENTENCEMODE_PHRASEPREDICT);
  if (FAILED(sentence_compartment->SetValue(client_id_, &sentence_variant))) {
    DVLOG(1) << "Failed to change the sentence mode.";
    return false;
  }

  return true;
}

void TSFBridgeDelegate::Shutdown() {
  DCHECK_EQ(base::MessageLoop::TYPE_UI, base::MessageLoop::current()->type());
  if (!IsInitialized())
    return;
  for (TSFDocumentMap::iterator it = tsf_document_map_.begin();
       it != tsf_document_map_.end(); ++it) {
    base::win::ScopedComPtr<ITfContext> context;
    base::win::ScopedComPtr<ITfSource> source;
    if (it->second.cookie != TF_INVALID_COOKIE &&
        SUCCEEDED(it->second.document_manager->GetBase(context.Receive())) &&
        SUCCEEDED(source.QueryFrom(context))) {
      source->UnadviseSink(it->second.cookie);
    }
  }
  tsf_document_map_.clear();

  client_id_ = TF_CLIENTID_NULL;
}

void TSFBridgeDelegate::OnTextInputTypeChanged(TextInputClient* client) {
  DCHECK_EQ(base::MessageLoop::TYPE_UI, base::MessageLoop::current()->type());
  DCHECK(IsInitialized());

  if (client != client_) {
    // Called from not focusing client. Do nothing.
    return;
  }

  thread_manager_->SetFocus(GetAssociatedDocumentManager().get());
}

bool TSFBridgeDelegate::CancelComposition() {
  DCHECK_EQ(base::MessageLoop::TYPE_UI, base::MessageLoop::current()->type());
  DCHECK(IsInitialized());

  base::win::ScopedComPtr<ITfDocumentMgr> focused_document_manager;
  for (TSFDocumentMap::iterator it = tsf_document_map_.begin();
       it != tsf_document_map_.end(); ++it) {
    if (IsFocused(it->second.document_manager.get())) {
      focused_document_manager = it->second.document_manager.get();
      break;
    }
  }

  if (focused_document_manager.get() == NULL)
    return false;

  base::win::ScopedComPtr<ITfContext> context;
  // We should use ITfDocumentMgr::GetBase instead of ITfDocumentMgr::GetTop,
  // which may return a temporal context created by an IME for its modal UI
  // handling, to obtain a context against which on-going composition is
  // canceled. This is because ITfDocumentMgr::GetBase always returns the
  // context that is created by us and owns the on-going composition.
  // See http://crbug.com/169664 for details.
  if (FAILED(focused_document_manager->GetBase(context.Receive()))) {
    DVLOG(1) << "Failed to get top context.";
    return false;
  }

  base::win::ScopedComPtr<ITfContextOwnerCompositionServices> owner;
  if (FAILED(owner.QueryFrom(context))) {
    DVLOG(1) << "Failed to get ITfContextOwnerCompositionService.";
    return false;
  }
  // Cancel all compositions.
  owner->TerminateComposition(NULL);
  return true;
}

void TSFBridgeDelegate::SetFocusedClient(HWND focused_window,
                                         TextInputClient* client) {
  DCHECK_EQ(base::MessageLoop::TYPE_UI, base::MessageLoop::current()->type());
  DCHECK(client);
  DCHECK(IsInitialized());
  client_ = client;

  for (TSFDocumentMap::iterator it = tsf_document_map_.begin();
       it != tsf_document_map_.end(); ++it) {
    if (it->second.text_store.get() == NULL)
      continue;
    it->second.text_store->SetFocusedTextInputClient(focused_window,
                                                     client);
  }

  // Synchronize text input type state.
  OnTextInputTypeChanged(client);
}

void TSFBridgeDelegate::RemoveFocusedClient(TextInputClient* client) {
  DCHECK_EQ(base::MessageLoop::TYPE_UI, base::MessageLoop::current()->type());
  DCHECK(IsInitialized());
  if (client_ != client)
    return;
  client_ = NULL;
  for (TSFDocumentMap::iterator it = tsf_document_map_.begin();
        it != tsf_document_map_.end(); ++it) {
    if (it->second.text_store.get() == NULL)
      continue;
    it->second.text_store->SetFocusedTextInputClient(NULL, NULL);
  }
}

TextInputClient* TSFBridgeDelegate::GetFocusedTextInputClient() const {
  return client_;
}

base::win::ScopedComPtr<ITfThreadMgr> TSFBridgeDelegate::GetThreadManager() {
  DCHECK_EQ(base::MessageLoop::TYPE_UI, base::MessageLoop::current()->type());
  DCHECK(IsInitialized());
  return thread_manager_;
}

bool TSFBridgeDelegate::CreateDocumentManager(TSFTextStore* text_store,
                                              ITfDocumentMgr** document_manager,
                                              ITfContext** context,
                                              DWORD* source_cookie) {
  if (FAILED(thread_manager_->CreateDocumentMgr(document_manager))) {
    DVLOG(1) << "Failed to create Document Manager.";
    return false;
  }

  DWORD edit_cookie = TF_INVALID_EDIT_COOKIE;
  if (FAILED((*document_manager)->CreateContext(
      client_id_,
      0,
      static_cast<ITextStoreACP*>(text_store),
      context,
      &edit_cookie))) {
    DVLOG(1) << "Failed to create Context.";
    return false;
  }

  if (FAILED((*document_manager)->Push(*context))) {
    DVLOG(1) << "Failed to push context.";
    return false;
  }

  if (!text_store || !source_cookie)
    return true;

  base::win::ScopedComPtr<ITfSource> source;
  if (FAILED(source.QueryFrom(*context))) {
    DVLOG(1) << "Failed to get source.";
    return false;
  }

  if (FAILED(source->AdviseSink(IID_ITfTextEditSink,
                                static_cast<ITfTextEditSink*>(text_store),
                                source_cookie))) {
    DVLOG(1) << "AdviseSink failed.";
    return false;
  }

  if (*source_cookie == TF_INVALID_COOKIE) {
    DVLOG(1) << "The result of cookie is invalid.";
    return false;
  }
  return true;
}

bool TSFBridgeDelegate::InitializeDocumentMapInternal() {
  const TextInputType kTextInputTypes[] = {
    TEXT_INPUT_TYPE_NONE,
    TEXT_INPUT_TYPE_TEXT,
    TEXT_INPUT_TYPE_PASSWORD,
    TEXT_INPUT_TYPE_SEARCH,
    TEXT_INPUT_TYPE_EMAIL,
    TEXT_INPUT_TYPE_NUMBER,
    TEXT_INPUT_TYPE_TELEPHONE,
    TEXT_INPUT_TYPE_URL,
  };
  for (size_t i = 0; i < arraysize(kTextInputTypes); ++i) {
    const TextInputType input_type = kTextInputTypes[i];
    base::win::ScopedComPtr<ITfContext> context;
    base::win::ScopedComPtr<ITfDocumentMgr> document_manager;
    DWORD cookie = TF_INVALID_COOKIE;
    const bool use_null_text_store = (input_type == TEXT_INPUT_TYPE_NONE);
    DWORD* cookie_ptr = use_null_text_store ? NULL : &cookie;
    scoped_refptr<TSFTextStore> text_store =
        use_null_text_store ? NULL : new TSFTextStore();
    if (!CreateDocumentManager(text_store,
                               document_manager.Receive(),
                               context.Receive(),
                               cookie_ptr))
      return false;
    const bool use_disabled_context =
        (input_type == TEXT_INPUT_TYPE_PASSWORD ||
         input_type == TEXT_INPUT_TYPE_NONE);
    if (use_disabled_context && !InitializeDisabledContext(context))
      return false;
    tsf_document_map_[input_type].text_store = text_store;
    tsf_document_map_[input_type].document_manager = document_manager;
    tsf_document_map_[input_type].cookie = cookie;
  }
  return true;
}

bool TSFBridgeDelegate::InitializeDisabledContext(ITfContext* context) {
  base::win::ScopedComPtr<ITfCompartmentMgr> compartment_mgr;
  if (FAILED(compartment_mgr.QueryFrom(context))) {
    DVLOG(1) << "Failed to get CompartmentMgr.";
    return false;
  }

  base::win::ScopedComPtr<ITfCompartment> disabled_compartment;
  if (FAILED(compartment_mgr->GetCompartment(
      GUID_COMPARTMENT_KEYBOARD_DISABLED,
      disabled_compartment.Receive()))) {
    DVLOG(1) << "Failed to get keyboard disabled compartment.";
    return false;
  }

  base::win::ScopedVariant variant;
  variant.Set(1);
  if (FAILED(disabled_compartment->SetValue(client_id_, &variant))) {
    DVLOG(1) << "Failed to disable the DocumentMgr.";
    return false;
  }

  base::win::ScopedComPtr<ITfCompartment> empty_context;
  if (FAILED(compartment_mgr->GetCompartment(GUID_COMPARTMENT_EMPTYCONTEXT,
                                             empty_context.Receive()))) {
    DVLOG(1) << "Failed to get empty context compartment.";
    return false;
  }
  base::win::ScopedVariant empty_context_variant;
  empty_context_variant.Set(static_cast<int32>(1));
  if (FAILED(empty_context->SetValue(client_id_, &empty_context_variant))) {
    DVLOG(1) << "Failed to set empty context.";
    return false;
  }

  return true;
}

bool TSFBridgeDelegate::IsFocused(ITfDocumentMgr* document_manager) {
  base::win::ScopedComPtr<ITfDocumentMgr> focused_document_manager;
  if (FAILED(thread_manager_->GetFocus(focused_document_manager.Receive())))
    return false;
  return focused_document_manager.IsSameObject(document_manager);
}

bool TSFBridgeDelegate::IsInitialized() {
  return client_id_ != TF_CLIENTID_NULL;
}

base::win::ScopedComPtr<ITfDocumentMgr>
TSFBridgeDelegate::GetAssociatedDocumentManager() {
  TSFDocumentMap::const_iterator it =
      tsf_document_map_.find(client_->GetTextInputType());
  if (it == tsf_document_map_.end())
    return tsf_document_map_[TEXT_INPUT_TYPE_TEXT].document_manager;
  return it->second.document_manager;
}

}  // namespace


// TsfBridge  -----------------------------------------------------------------

TSFBridge::TSFBridge() {
}

TSFBridge::~TSFBridge() {
}

// static
bool TSFBridge::Initialize() {
  if (base::MessageLoop::current()->type() != base::MessageLoop::TYPE_UI) {
    DVLOG(1) << "Do not use TSFBridge without UI thread.";
    return false;
  }
  if (!tls_tsf_bridge.initialized()) {
    tls_tsf_bridge.Initialize(TSFBridge::Finalize);
    TSFBridgeDelegate* delegate = new TSFBridgeDelegate();
    tls_tsf_bridge.Set(delegate);
    return delegate->Initialize();
  }
  return true;
}

// static
TSFBridge* TSFBridge::ReplaceForTesting(TSFBridge* bridge) {
  if (base::MessageLoop::current()->type() != base::MessageLoop::TYPE_UI) {
    DVLOG(1) << "Do not use TSFBridge without UI thread.";
    return NULL;
  }
  TSFBridge* old_bridge = TSFBridge::GetInstance();
  tls_tsf_bridge.Set(bridge);
  return old_bridge;
}

// static
TSFBridge* TSFBridge::GetInstance() {
  if (base::MessageLoop::current()->type() != base::MessageLoop::TYPE_UI) {
    DVLOG(1) << "Do not use TSFBridge without UI thread.";
    return NULL;
  }
  TSFBridgeDelegate* delegate =
      static_cast<TSFBridgeDelegate*>(tls_tsf_bridge.Get());
  DCHECK(delegate) << "Do no call GetInstance before TSFBridge::Initialize.";
  return delegate;
}

// static
void TSFBridge::Finalize(void* data) {
  TSFBridgeDelegate* delegate = static_cast<TSFBridgeDelegate*>(data);
  delete delegate;
}

}  // namespace ui
