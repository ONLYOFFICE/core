// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/accessibility/native_view_accessibility_win.h"

#include <UIAutomationClient.h>
#include <oleacc.h>

#include <set>
#include <vector>

#include "base/memory/singleton.h"
#include "base/win/windows_version.h"
#include "third_party/iaccessible2/ia2_api_all.h"
#include "ui/base/accessibility/accessible_text_utils.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/win/accessibility_ids_win.h"
#include "ui/base/win/accessibility_misc_utils.h"
#include "ui/base/win/atl_module.h"
#include "ui/views/controls/button/custom_button.h"
#include "ui/views/controls/webview/webview.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/focus/view_storage.h"
#include "ui/views/win/hwnd_util.h"

using ui::AccessibilityTypes;

namespace views {
namespace {

class AccessibleWebViewRegistry {
 public:
  static AccessibleWebViewRegistry* GetInstance();

  void RegisterWebView(AccessibleWebView* web_view);

  void UnregisterWebView(AccessibleWebView* web_view);

  // Given the view that received the request for the accessible
  // id in |top_view|, and the child id requested, return the native
  // accessible object with that child id from one of the WebViews in
  // |top_view|'s view hierarchy, if any.
  IAccessible* GetAccessibleFromWebView(View* top_view, long child_id);

 private:
  friend struct DefaultSingletonTraits<AccessibleWebViewRegistry>;
  AccessibleWebViewRegistry();
  ~AccessibleWebViewRegistry() {}

  // Set of all web views. We check whether each one is contained in a
  // top view dynamically rather than keeping track of a map.
  std::set<AccessibleWebView*> web_views_;

  // The most recent top view used in a call to GetAccessibleFromWebView.
  View* last_top_view_;

  // The most recent web view where an accessible object was found,
  // corresponding to |last_top_view_|.
  AccessibleWebView* last_web_view_;

  DISALLOW_COPY_AND_ASSIGN(AccessibleWebViewRegistry);
};

AccessibleWebViewRegistry::AccessibleWebViewRegistry()
    : last_top_view_(NULL),
      last_web_view_(NULL) {
}

AccessibleWebViewRegistry* AccessibleWebViewRegistry::GetInstance() {
  return Singleton<AccessibleWebViewRegistry>::get();
}

void AccessibleWebViewRegistry::RegisterWebView(AccessibleWebView* web_view) {
  DCHECK(web_views_.find(web_view) == web_views_.end());
  web_views_.insert(web_view);
}

void AccessibleWebViewRegistry::UnregisterWebView(AccessibleWebView* web_view) {
  DCHECK(web_views_.find(web_view) != web_views_.end());
  web_views_.erase(web_view);
  if (last_web_view_ == web_view) {
    last_top_view_ = NULL;
    last_web_view_ = NULL;
  }
}

IAccessible* AccessibleWebViewRegistry::GetAccessibleFromWebView(
    View* top_view, long child_id) {
  // This function gets called frequently, so try to avoid searching all
  // of the web views if the notification is on the same web view that
  // sent the last one.
  if (last_top_view_ == top_view) {
    IAccessible* accessible =
        last_web_view_->AccessibleObjectFromChildId(child_id);
    if (accessible)
      return accessible;
  }

  // Search all web views. For each one, first ensure it's a descendant
  // of this view where the event was posted - and if so, see if it owns
  // an accessible object with that child id. If so, save the view to speed
  // up the next notification.
  for (std::set<AccessibleWebView*>::iterator iter = web_views_.begin();
       iter != web_views_.end(); ++iter) {
    AccessibleWebView* web_view = *iter;
    if (!top_view->Contains(web_view->AsView()))
      continue;
    IAccessible* accessible = web_view->AccessibleObjectFromChildId(child_id);
    if (accessible) {
      last_top_view_ = top_view;
      last_web_view_ = web_view;
      return accessible;
    }
  }
  return NULL;
}

}  // anonymous namespace

// static
long NativeViewAccessibilityWin::next_unique_id_ = 1;
int NativeViewAccessibilityWin::view_storage_ids_[kMaxViewStorageIds] = {0};
int NativeViewAccessibilityWin::next_view_storage_id_index_ = 0;

// static
NativeViewAccessibility* NativeViewAccessibility::Create(View* view) {
  // Make sure ATL is initialized in this module.
  ui::win::CreateATLModuleIfNeeded();

  CComObject<NativeViewAccessibilityWin>* instance = NULL;
  HRESULT hr = CComObject<NativeViewAccessibilityWin>::CreateInstance(
      &instance);
  DCHECK(SUCCEEDED(hr));
  instance->set_view(view);
  instance->AddRef();
  return instance;
}

NativeViewAccessibilityWin::NativeViewAccessibilityWin()
    : view_(NULL),
      unique_id_(next_unique_id_++) {
}

NativeViewAccessibilityWin::~NativeViewAccessibilityWin() {
}

void NativeViewAccessibilityWin::NotifyAccessibilityEvent(
    ui::AccessibilityTypes::Event event_type) {
  if (!view_)
    return;

  ViewStorage* view_storage = ViewStorage::GetInstance();
  HWND hwnd = HWNDForView(view_);
  int view_storage_id = view_storage_ids_[next_view_storage_id_index_];
  if (view_storage_id == 0) {
    view_storage_id = view_storage->CreateStorageID();
    view_storage_ids_[next_view_storage_id_index_] = view_storage_id;
  } else {
    view_storage->RemoveView(view_storage_id);
  }
  view_storage->StoreView(view_storage_id, view_);

  // Positive child ids are used for enumerating direct children,
  // negative child ids can be used as unique ids to refer to a specific
  // descendants.  Make index into view_storage_ids_ into a negative child id.
  int child_id =
      base::win::kFirstViewsAccessibilityId - next_view_storage_id_index_;
  ::NotifyWinEvent(MSAAEvent(event_type), hwnd, OBJID_CLIENT, child_id);
  next_view_storage_id_index_ =
      (next_view_storage_id_index_ + 1) % kMaxViewStorageIds;
}

gfx::NativeViewAccessible NativeViewAccessibilityWin::GetNativeObject() {
  return this;
}

void NativeViewAccessibilityWin::Destroy() {
  view_ = NULL;
  Release();
}

// TODO(ctguil): Handle case where child View is not contained by parent.
STDMETHODIMP NativeViewAccessibilityWin::accHitTest(
    LONG x_left, LONG y_top, VARIANT* child) {
  if (!child)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  gfx::Point point(x_left, y_top);
  View::ConvertPointToTarget(NULL, view_, &point);

  if (!view_->HitTestPoint(point)) {
    // If containing parent is not hit, return with failure.
    child->vt = VT_EMPTY;
    return S_FALSE;
  }

  View* view = view_->GetEventHandlerForPoint(point);
  if (view == view_) {
    // No child hit, return parent id.
    child->vt = VT_I4;
    child->lVal = CHILDID_SELF;
  } else {
    child->vt = VT_DISPATCH;
    child->pdispVal = view->GetNativeViewAccessible();
    child->pdispVal->AddRef();
  }
  return S_OK;
}

HRESULT NativeViewAccessibilityWin::accDoDefaultAction(VARIANT var_id) {
  if (!IsValidId(var_id))
    return E_INVALIDARG;

  // The object does not support the method. This value is returned for
  // controls that do not perform actions, such as edit fields.
  return DISP_E_MEMBERNOTFOUND;
}

STDMETHODIMP NativeViewAccessibilityWin::accLocation(
    LONG* x_left, LONG* y_top, LONG* width, LONG* height, VARIANT var_id) {
  if (!IsValidId(var_id) || !x_left || !y_top || !width || !height)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  if (!view_->bounds().IsEmpty()) {
    *width  = view_->width();
    *height = view_->height();
    gfx::Point topleft(view_->bounds().origin());
    View::ConvertPointToScreen(
        view_->parent() ? view_->parent() : view_, &topleft);
    *x_left = topleft.x();
    *y_top  = topleft.y();
  } else {
    return E_FAIL;
  }
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::accNavigate(
    LONG nav_dir, VARIANT start, VARIANT* end) {
  if (start.vt != VT_I4 || !end)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  switch (nav_dir) {
    case NAVDIR_FIRSTCHILD:
    case NAVDIR_LASTCHILD: {
      if (start.lVal != CHILDID_SELF) {
        // Start of navigation must be on the View itself.
        return E_INVALIDARG;
      } else if (!view_->has_children()) {
        // No children found.
        return S_FALSE;
      }

      // Set child_id based on first or last child.
      int child_id = 0;
      if (nav_dir == NAVDIR_LASTCHILD)
        child_id = view_->child_count() - 1;

      View* child = view_->child_at(child_id);
      end->vt = VT_DISPATCH;
      end->pdispVal = child->GetNativeViewAccessible();
      end->pdispVal->AddRef();
      return S_OK;
    }
    case NAVDIR_LEFT:
    case NAVDIR_UP:
    case NAVDIR_PREVIOUS:
    case NAVDIR_RIGHT:
    case NAVDIR_DOWN:
    case NAVDIR_NEXT: {
      // Retrieve parent to access view index and perform bounds checking.
      View* parent = view_->parent();
      if (!parent) {
        return E_FAIL;
      }

      if (start.lVal == CHILDID_SELF) {
        int view_index = parent->GetIndexOf(view_);
        // Check navigation bounds, adjusting for View child indexing (MSAA
        // child indexing starts with 1, whereas View indexing starts with 0).
        if (!IsValidNav(nav_dir, view_index, -1,
                        parent->child_count() - 1)) {
          // Navigation attempted to go out-of-bounds.
          end->vt = VT_EMPTY;
          return S_FALSE;
        } else {
          if (IsNavDirNext(nav_dir)) {
            view_index += 1;
          } else {
            view_index -=1;
          }
        }

        View* child = parent->child_at(view_index);
        end->pdispVal = child->GetNativeViewAccessible();
        end->vt = VT_DISPATCH;
        end->pdispVal->AddRef();
        return S_OK;
      } else {
        // Check navigation bounds, adjusting for MSAA child indexing (MSAA
        // child indexing starts with 1, whereas View indexing starts with 0).
        if (!IsValidNav(nav_dir, start.lVal, 0, parent->child_count() + 1)) {
            // Navigation attempted to go out-of-bounds.
            end->vt = VT_EMPTY;
            return S_FALSE;
          } else {
            if (IsNavDirNext(nav_dir)) {
              start.lVal += 1;
            } else {
              start.lVal -= 1;
            }
        }

        HRESULT result = this->get_accChild(start, &end->pdispVal);
        if (result == S_FALSE) {
          // Child is a leaf.
          end->vt = VT_I4;
          end->lVal = start.lVal;
        } else if (result == E_INVALIDARG) {
          return E_INVALIDARG;
        } else {
          // Child is not a leaf.
          end->vt = VT_DISPATCH;
        }
      }
      break;
    }
    default:
      return E_INVALIDARG;
  }
  // Navigation performed correctly. Global return for this function, if no
  // error triggered an escape earlier.
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accChild(VARIANT var_child,
                                                      IDispatch** disp_child) {
  if (var_child.vt != VT_I4 || !disp_child)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  LONG child_id = V_I4(&var_child);

  if (child_id == CHILDID_SELF) {
    // Remain with the same dispatch.
    return S_OK;
  }

  View* child_view = NULL;
  if (child_id > 0) {
    // Positive child ids are a 1-based child index, used by clients
    // that want to enumerate all immediate children.
    int child_id_as_index = child_id - 1;
    if (child_id_as_index < view_->child_count())
      child_view = view_->child_at(child_id_as_index);
  } else {
    // Negative child ids can be used to map to any descendant;
    // we map child ids to a view storage id that can refer to a
    // specific view (if that view still exists).
    int view_storage_id_index =
        base::win::kFirstViewsAccessibilityId - child_id;
    if (view_storage_id_index >= 0 &&
        view_storage_id_index < kMaxViewStorageIds) {
      int view_storage_id = view_storage_ids_[view_storage_id_index];
      ViewStorage* view_storage = ViewStorage::GetInstance();
      child_view = view_storage->RetrieveView(view_storage_id);
    } else {
      *disp_child = AccessibleWebViewRegistry::GetInstance()->
          GetAccessibleFromWebView(view_, child_id);
      if (*disp_child) {
        (*disp_child)->AddRef();
        return S_OK;
      }
    }
  }

  if (!child_view) {
    // No child found.
    *disp_child = NULL;
    return E_FAIL;
  }

  *disp_child = child_view->GetNativeViewAccessible();
  (*disp_child)->AddRef();
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accChildCount(LONG* child_count) {
  if (!child_count || !view_)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  *child_count = view_->child_count();
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accDefaultAction(
    VARIANT var_id, BSTR* def_action) {
  if (!IsValidId(var_id) || !def_action)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  string16 temp_action = state.default_action;

  if (!temp_action.empty()) {
    *def_action = SysAllocString(temp_action.c_str());
  } else {
    return S_FALSE;
  }

  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accDescription(
    VARIANT var_id, BSTR* desc) {
  if (!IsValidId(var_id) || !desc)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  string16 temp_desc;

  view_->GetTooltipText(gfx::Point(), &temp_desc);
  if (!temp_desc.empty()) {
    *desc = SysAllocString(temp_desc.c_str());
  } else {
    return S_FALSE;
  }

  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accFocus(VARIANT* focus_child) {
  if (!focus_child)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  FocusManager* focus_manager = view_->GetFocusManager();
  View* focus = focus_manager ? focus_manager->GetFocusedView() : NULL;
  if (focus == view_) {
    // This view has focus.
    focus_child->vt = VT_I4;
    focus_child->lVal = CHILDID_SELF;
  } else if (focus && view_->Contains(focus)) {
    // Return the child object that has the keyboard focus.
    focus_child->vt = VT_DISPATCH;
    focus_child->pdispVal = focus->GetNativeViewAccessible();
    focus_child->pdispVal->AddRef();
    return S_OK;
  } else {
    // Neither this object nor any of its children has the keyboard focus.
    focus_child->vt = VT_EMPTY;
  }
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accKeyboardShortcut(
    VARIANT var_id, BSTR* acc_key) {
  if (!IsValidId(var_id) || !acc_key)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  string16 temp_key = state.keyboard_shortcut;

  if (!temp_key.empty()) {
    *acc_key = SysAllocString(temp_key.c_str());
  } else {
    return S_FALSE;
  }

  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accName(
    VARIANT var_id, BSTR* name) {
  if (!IsValidId(var_id) || !name)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  // Retrieve the current view's name.
  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  string16 temp_name = state.name;
  if (!temp_name.empty()) {
    // Return name retrieved.
    *name = SysAllocString(temp_name.c_str());
  } else {
    // If view has no name, return S_FALSE.
    return S_FALSE;
  }

  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accParent(
    IDispatch** disp_parent) {
  if (!disp_parent)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  *disp_parent = NULL;
  View* parent_view = view_->parent();

  if (!parent_view) {
    HWND hwnd = HWNDForView(view_);
    if (!hwnd)
      return S_FALSE;

    return ::AccessibleObjectFromWindow(
        hwnd, OBJID_WINDOW, IID_IAccessible,
        reinterpret_cast<void**>(disp_parent));
  }

  *disp_parent = parent_view->GetNativeViewAccessible();
  (*disp_parent)->AddRef();
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accRole(
    VARIANT var_id, VARIANT* role) {
  if (!IsValidId(var_id) || !role)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  role->vt = VT_I4;
  role->lVal = MSAARole(state.role);
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accState(
    VARIANT var_id, VARIANT* state) {
  // This returns MSAA states. See also the IAccessible2 interface
  // get_states().

  if (!IsValidId(var_id) || !state)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  state->vt = VT_I4;

  // Retrieve all currently applicable states of the parent.
  SetState(state, view_);

  // Make sure that state is not empty, and has the proper type.
  if (state->vt == VT_EMPTY)
    return E_FAIL;

  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accValue(
    VARIANT var_id, BSTR* value) {
  if (!IsValidId(var_id) || !value)
    return E_INVALIDARG;

  if (!view_)
    return E_FAIL;

  // Retrieve the current view's value.
  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  string16 temp_value = state.value;

  if (!temp_value.empty()) {
    // Return value retrieved.
    *value = SysAllocString(temp_value.c_str());
  } else {
    // If view has no value, fall back into the default implementation.
    *value = NULL;
    return E_NOTIMPL;
  }

  return S_OK;
}

// IAccessible functions not supported.

STDMETHODIMP NativeViewAccessibilityWin::get_accSelection(VARIANT* selected) {
  if (selected)
    selected->vt = VT_EMPTY;
  return E_NOTIMPL;
}

STDMETHODIMP NativeViewAccessibilityWin::accSelect(
    LONG flagsSelect, VARIANT var_id) {
  return E_NOTIMPL;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accHelp(
    VARIANT var_id, BSTR* help) {
  if (help)
    *help = NULL;
  return E_NOTIMPL;
}

STDMETHODIMP NativeViewAccessibilityWin::get_accHelpTopic(
    BSTR* help_file, VARIANT var_id, LONG* topic_id) {
  if (help_file) {
    *help_file = NULL;
  }
  if (topic_id) {
    *topic_id = static_cast<LONG>(-1);
  }
  return E_NOTIMPL;
}

STDMETHODIMP NativeViewAccessibilityWin::put_accName(
    VARIANT var_id, BSTR put_name) {
  // Deprecated.
  return E_NOTIMPL;
}

STDMETHODIMP NativeViewAccessibilityWin::put_accValue(
    VARIANT var_id, BSTR put_val) {
  // Deprecated.
  return E_NOTIMPL;
}

//
// IAccessible2
//

STDMETHODIMP NativeViewAccessibilityWin::role(LONG* role) {
  if (!view_)
    return E_FAIL;

  if (!role)
    return E_INVALIDARG;

  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  *role = MSAARole(state.role);
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_states(AccessibleStates* states) {
  // This returns IAccessible2 states, which supplement MSAA states.
  // See also the MSAA interface get_accState.

  if (!view_)
    return E_FAIL;

  if (!states)
    return E_INVALIDARG;

  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);

  // There are only a couple of states we need to support
  // in IAccessible2. If any more are added, we may want to
  // add a helper function like MSAAState.
  *states = IA2_STATE_OPAQUE;
  if (state.state & AccessibilityTypes::STATE_EDITABLE)
    *states |= IA2_STATE_EDITABLE;

  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_uniqueID(LONG* unique_id) {
  if (!view_)
    return E_FAIL;

  if (!unique_id)
    return E_INVALIDARG;

  *unique_id = unique_id_;
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_windowHandle(HWND* window_handle) {
  if (!view_)
    return E_FAIL;

  if (!window_handle)
    return E_INVALIDARG;

  *window_handle = HWNDForView(view_);
  return *window_handle ? S_OK : S_FALSE;
}

//
// IAccessibleText
//

STDMETHODIMP NativeViewAccessibilityWin::get_nCharacters(LONG* n_characters) {
  if (!view_)
    return E_FAIL;

  if (!n_characters)
    return E_INVALIDARG;

  string16 text = TextForIAccessibleText();
  *n_characters = static_cast<LONG>(text.size());
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_caretOffset(LONG* offset) {
  if (!view_)
    return E_FAIL;

  if (!offset)
    return E_INVALIDARG;

  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  *offset = static_cast<LONG>(state.selection_end);
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_nSelections(LONG* n_selections) {
  if (!view_)
    return E_FAIL;

  if (!n_selections)
    return E_INVALIDARG;

  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  if (state.selection_start != state.selection_end)
    *n_selections = 1;
  else
    *n_selections = 0;
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_selection(LONG selection_index,
                                                      LONG* start_offset,
                                                      LONG* end_offset) {
  if (!view_)
    return E_FAIL;

  if (!start_offset || !end_offset || selection_index != 0)
    return E_INVALIDARG;

  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  *start_offset = static_cast<LONG>(state.selection_start);
  *end_offset = static_cast<LONG>(state.selection_end);
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_text(LONG start_offset,
                                                  LONG end_offset,
                                                  BSTR* text) {
  if (!view_)
    return E_FAIL;

  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  string16 text_str = TextForIAccessibleText();
  LONG len = static_cast<LONG>(text_str.size());

  if (start_offset == IA2_TEXT_OFFSET_LENGTH) {
    start_offset = len;
  } else if (start_offset == IA2_TEXT_OFFSET_CARET) {
    start_offset = static_cast<LONG>(state.selection_end);
  }
  if (end_offset == IA2_TEXT_OFFSET_LENGTH) {
    end_offset = static_cast<LONG>(text_str.size());
  } else if (end_offset == IA2_TEXT_OFFSET_CARET) {
    end_offset = static_cast<LONG>(state.selection_end);
  }

  // The spec allows the arguments to be reversed.
  if (start_offset > end_offset) {
    LONG tmp = start_offset;
    start_offset = end_offset;
    end_offset = tmp;
  }

  // The spec does not allow the start or end offsets to be out or range;
  // we must return an error if so.
  if (start_offset < 0)
    return E_INVALIDARG;
  if (end_offset > len)
    return E_INVALIDARG;

  string16 substr = text_str.substr(start_offset, end_offset - start_offset);
  if (substr.empty())
    return S_FALSE;

  *text = SysAllocString(substr.c_str());
  DCHECK(*text);
  return S_OK;
}

STDMETHODIMP NativeViewAccessibilityWin::get_textAtOffset(
    LONG offset,
    enum IA2TextBoundaryType boundary_type,
    LONG* start_offset, LONG* end_offset,
    BSTR* text) {
  if (!start_offset || !end_offset || !text)
    return E_INVALIDARG;

  // The IAccessible2 spec says we don't have to implement the "sentence"
  // boundary type, we can just let the screenreader handle it.
  if (boundary_type == IA2_TEXT_BOUNDARY_SENTENCE) {
    *start_offset = 0;
    *end_offset = 0;
    *text = NULL;
    return S_FALSE;
  }

  const string16& text_str = TextForIAccessibleText();

  *start_offset = FindBoundary(
      text_str, boundary_type, offset, ui::BACKWARDS_DIRECTION);
  *end_offset = FindBoundary(
      text_str, boundary_type, offset, ui::FORWARDS_DIRECTION);
  return get_text(*start_offset, *end_offset, text);
}

STDMETHODIMP NativeViewAccessibilityWin::get_textBeforeOffset(
    LONG offset,
    enum IA2TextBoundaryType boundary_type,
    LONG* start_offset, LONG* end_offset,
    BSTR* text) {
  if (!start_offset || !end_offset || !text)
    return E_INVALIDARG;

  // The IAccessible2 spec says we don't have to implement the "sentence"
  // boundary type, we can just let the screenreader handle it.
  if (boundary_type == IA2_TEXT_BOUNDARY_SENTENCE) {
    *start_offset = 0;
    *end_offset = 0;
    *text = NULL;
    return S_FALSE;
  }

  const string16& text_str = TextForIAccessibleText();

  *start_offset = FindBoundary(
      text_str, boundary_type, offset, ui::BACKWARDS_DIRECTION);
  *end_offset = offset;
  return get_text(*start_offset, *end_offset, text);
}

STDMETHODIMP NativeViewAccessibilityWin::get_textAfterOffset(
    LONG offset,
    enum IA2TextBoundaryType boundary_type,
    LONG* start_offset, LONG* end_offset,
    BSTR* text) {
  if (!start_offset || !end_offset || !text)
    return E_INVALIDARG;

  // The IAccessible2 spec says we don't have to implement the "sentence"
  // boundary type, we can just let the screenreader handle it.
  if (boundary_type == IA2_TEXT_BOUNDARY_SENTENCE) {
    *start_offset = 0;
    *end_offset = 0;
    *text = NULL;
    return S_FALSE;
  }

  const string16& text_str = TextForIAccessibleText();

  *start_offset = offset;
  *end_offset = FindBoundary(
      text_str, boundary_type, offset, ui::FORWARDS_DIRECTION);
  return get_text(*start_offset, *end_offset, text);
}

STDMETHODIMP NativeViewAccessibilityWin::get_offsetAtPoint(
    LONG x, LONG y, enum IA2CoordinateType coord_type, LONG* offset) {
  if (!view_)
    return E_FAIL;

  if (!offset)
    return E_INVALIDARG;

  // We don't support this method, but we have to return something
  // rather than E_NOTIMPL or screen readers will complain.
  *offset = 0;
  return S_OK;
}

//
// IServiceProvider methods.
//

STDMETHODIMP NativeViewAccessibilityWin::QueryService(
    REFGUID guidService, REFIID riid, void** object) {
  if (!view_)
    return E_FAIL;

  if (guidService == IID_IAccessible ||
      guidService == IID_IAccessible2 ||
      guidService == IID_IAccessibleText)  {
    return QueryInterface(riid, object);
  }

  // We only support the IAccessibleEx interface on Windows 8 and above. This
  // is needed for the On screen Keyboard to show up in metro mode, when the
  // user taps an editable region in the window.
  // All methods in the IAccessibleEx interface are unimplemented.
  if (riid == IID_IAccessibleEx &&
      base::win::GetVersion() >= base::win::VERSION_WIN8) {
    return QueryInterface(riid, object);
  }

  *object = NULL;
  return E_FAIL;
}

STDMETHODIMP NativeViewAccessibilityWin::GetPatternProvider(
    PATTERNID id, IUnknown** provider) {
  DVLOG(1) << "In Function: "
           << __FUNCTION__
           << " for pattern id: "
           << id;
  if (id == UIA_ValuePatternId || id == UIA_TextPatternId) {
    ui::AccessibleViewState state;
    view_->GetAccessibleState(&state);
    long role = MSAARole(state.role);

    if (role == ROLE_SYSTEM_TEXT) {
      DVLOG(1) << "Returning UIA text provider";
      base::win::UIATextProvider::CreateTextProvider(true, provider);
      return S_OK;
    }
  }
  return E_NOTIMPL;
}

STDMETHODIMP NativeViewAccessibilityWin::GetPropertyValue(PROPERTYID id,
                                                          VARIANT* ret) {
  DVLOG(1) << "In Function: "
           << __FUNCTION__
           << " for property id: "
           << id;
  if (id == UIA_ControlTypePropertyId) {
    ui::AccessibleViewState state;
    view_->GetAccessibleState(&state);
    long role = MSAARole(state.role);
    if (role == ROLE_SYSTEM_TEXT) {
      V_VT(ret) = VT_I4;
      ret->lVal = UIA_EditControlTypeId;
      DVLOG(1) << "Returning Edit control type";
    } else {
      DVLOG(1) << "Returning empty control type";
      V_VT(ret) = VT_EMPTY;
    }
  } else {
    V_VT(ret) = VT_EMPTY;
  }
  return S_OK;
}

//
// Static methods.
//

void NativeViewAccessibility::RegisterWebView(AccessibleWebView* web_view) {
  AccessibleWebViewRegistry::GetInstance()->RegisterWebView(web_view);
}

void NativeViewAccessibility::UnregisterWebView(AccessibleWebView* web_view) {
  AccessibleWebViewRegistry::GetInstance()->UnregisterWebView(web_view);
}

int32 NativeViewAccessibilityWin::MSAAEvent(AccessibilityTypes::Event event) {
  switch (event) {
    case AccessibilityTypes::EVENT_ALERT:
      return EVENT_SYSTEM_ALERT;
    case AccessibilityTypes::EVENT_FOCUS:
      return EVENT_OBJECT_FOCUS;
    case AccessibilityTypes::EVENT_MENUSTART:
      return EVENT_SYSTEM_MENUSTART;
    case AccessibilityTypes::EVENT_MENUEND:
      return EVENT_SYSTEM_MENUEND;
    case AccessibilityTypes::EVENT_MENUPOPUPSTART:
      return EVENT_SYSTEM_MENUPOPUPSTART;
    case AccessibilityTypes::EVENT_MENUPOPUPEND:
      return EVENT_SYSTEM_MENUPOPUPEND;
    case AccessibilityTypes::EVENT_NAME_CHANGED:
      return EVENT_OBJECT_NAMECHANGE;
    case AccessibilityTypes::EVENT_TEXT_CHANGED:
      return EVENT_OBJECT_VALUECHANGE;
    case AccessibilityTypes::EVENT_SELECTION_CHANGED:
      return IA2_EVENT_TEXT_CARET_MOVED;
    case AccessibilityTypes::EVENT_VALUE_CHANGED:
      return EVENT_OBJECT_VALUECHANGE;
    default:
      // Not supported or invalid event.
      NOTREACHED();
      return -1;
  }
}

int32 NativeViewAccessibilityWin::MSAARole(AccessibilityTypes::Role role) {
  switch (role) {
    case AccessibilityTypes::ROLE_ALERT:
      return ROLE_SYSTEM_ALERT;
    case AccessibilityTypes::ROLE_APPLICATION:
      return ROLE_SYSTEM_APPLICATION;
    case AccessibilityTypes::ROLE_BUTTONDROPDOWN:
      return ROLE_SYSTEM_BUTTONDROPDOWN;
    case AccessibilityTypes::ROLE_BUTTONMENU:
      return ROLE_SYSTEM_BUTTONMENU;
    case AccessibilityTypes::ROLE_CHECKBUTTON:
      return ROLE_SYSTEM_CHECKBUTTON;
    case AccessibilityTypes::ROLE_COMBOBOX:
      return ROLE_SYSTEM_COMBOBOX;
    case AccessibilityTypes::ROLE_DIALOG:
      return ROLE_SYSTEM_DIALOG;
    case AccessibilityTypes::ROLE_GRAPHIC:
      return ROLE_SYSTEM_GRAPHIC;
    case AccessibilityTypes::ROLE_GROUPING:
      return ROLE_SYSTEM_GROUPING;
    case AccessibilityTypes::ROLE_LINK:
      return ROLE_SYSTEM_LINK;
    case AccessibilityTypes::ROLE_LOCATION_BAR:
      return ROLE_SYSTEM_GROUPING;
    case AccessibilityTypes::ROLE_MENUBAR:
      return ROLE_SYSTEM_MENUBAR;
    case AccessibilityTypes::ROLE_MENUITEM:
      return ROLE_SYSTEM_MENUITEM;
    case AccessibilityTypes::ROLE_MENUPOPUP:
      return ROLE_SYSTEM_MENUPOPUP;
    case AccessibilityTypes::ROLE_OUTLINE:
      return ROLE_SYSTEM_OUTLINE;
    case AccessibilityTypes::ROLE_OUTLINEITEM:
      return ROLE_SYSTEM_OUTLINEITEM;
    case AccessibilityTypes::ROLE_PAGETAB:
      return ROLE_SYSTEM_PAGETAB;
    case AccessibilityTypes::ROLE_PAGETABLIST:
      return ROLE_SYSTEM_PAGETABLIST;
    case AccessibilityTypes::ROLE_PANE:
      return ROLE_SYSTEM_PANE;
    case AccessibilityTypes::ROLE_PROGRESSBAR:
      return ROLE_SYSTEM_PROGRESSBAR;
    case AccessibilityTypes::ROLE_PUSHBUTTON:
      return ROLE_SYSTEM_PUSHBUTTON;
    case AccessibilityTypes::ROLE_RADIOBUTTON:
      return ROLE_SYSTEM_RADIOBUTTON;
    case AccessibilityTypes::ROLE_SCROLLBAR:
      return ROLE_SYSTEM_SCROLLBAR;
    case AccessibilityTypes::ROLE_SEPARATOR:
      return ROLE_SYSTEM_SEPARATOR;
    case AccessibilityTypes::ROLE_SLIDER:
      return ROLE_SYSTEM_SLIDER;
    case AccessibilityTypes::ROLE_STATICTEXT:
      return ROLE_SYSTEM_STATICTEXT;
    case AccessibilityTypes::ROLE_TEXT:
      return ROLE_SYSTEM_TEXT;
    case AccessibilityTypes::ROLE_TITLEBAR:
      return ROLE_SYSTEM_TITLEBAR;
    case AccessibilityTypes::ROLE_TOOLBAR:
      return ROLE_SYSTEM_TOOLBAR;
    case AccessibilityTypes::ROLE_WINDOW:
      return ROLE_SYSTEM_WINDOW;
    case AccessibilityTypes::ROLE_CLIENT:
    default:
      // This is the default role for MSAA.
      return ROLE_SYSTEM_CLIENT;
  }
}

int32 NativeViewAccessibilityWin::MSAAState(AccessibilityTypes::State state) {
  // This maps MSAA states for get_accState(). See also the IAccessible2
  // interface get_states().

  int32 msaa_state = 0;
  if (state & AccessibilityTypes::STATE_CHECKED)
    msaa_state |= STATE_SYSTEM_CHECKED;
  if (state & AccessibilityTypes::STATE_COLLAPSED)
    msaa_state |= STATE_SYSTEM_COLLAPSED;
  if (state & AccessibilityTypes::STATE_DEFAULT)
    msaa_state |= STATE_SYSTEM_DEFAULT;
  if (state & AccessibilityTypes::STATE_EXPANDED)
    msaa_state |= STATE_SYSTEM_EXPANDED;
  if (state & AccessibilityTypes::STATE_HASPOPUP)
    msaa_state |= STATE_SYSTEM_HASPOPUP;
  if (state & AccessibilityTypes::STATE_HOTTRACKED)
    msaa_state |= STATE_SYSTEM_HOTTRACKED;
  if (state & AccessibilityTypes::STATE_INVISIBLE)
    msaa_state |= STATE_SYSTEM_INVISIBLE;
  if (state & AccessibilityTypes::STATE_LINKED)
    msaa_state |= STATE_SYSTEM_LINKED;
  if (state & AccessibilityTypes::STATE_OFFSCREEN)
    msaa_state |= STATE_SYSTEM_OFFSCREEN;
  if (state & AccessibilityTypes::STATE_PRESSED)
    msaa_state |= STATE_SYSTEM_PRESSED;
  if (state & AccessibilityTypes::STATE_PROTECTED)
    msaa_state |= STATE_SYSTEM_PROTECTED;
  if (state & AccessibilityTypes::STATE_READONLY)
    msaa_state |= STATE_SYSTEM_READONLY;
  if (state & AccessibilityTypes::STATE_SELECTED)
    msaa_state |= STATE_SYSTEM_SELECTED;
  if (state & AccessibilityTypes::STATE_FOCUSED)
    msaa_state |= STATE_SYSTEM_FOCUSED;
  if (state & AccessibilityTypes::STATE_UNAVAILABLE)
    msaa_state |= STATE_SYSTEM_UNAVAILABLE;
  return msaa_state;
}

//
// Private methods.
//

bool NativeViewAccessibilityWin::IsNavDirNext(int nav_dir) const {
  return (nav_dir == NAVDIR_RIGHT ||
          nav_dir == NAVDIR_DOWN ||
          nav_dir == NAVDIR_NEXT);
}

bool NativeViewAccessibilityWin::IsValidNav(
    int nav_dir, int start_id, int lower_bound, int upper_bound) const {
  if (IsNavDirNext(nav_dir)) {
    if ((start_id + 1) > upper_bound) {
      return false;
    }
  } else {
    if ((start_id - 1) <= lower_bound) {
      return false;
    }
  }
  return true;
}

bool NativeViewAccessibilityWin::IsValidId(const VARIANT& child) const {
  // View accessibility returns an IAccessible for each view so we only support
  // the CHILDID_SELF id.
  return (VT_I4 == child.vt) && (CHILDID_SELF == child.lVal);
}

void NativeViewAccessibilityWin::SetState(
    VARIANT* msaa_state, View* view) {
  // Ensure the output param is initialized to zero.
  msaa_state->lVal = 0;

  // Default state; all views can have accessibility focus.
  msaa_state->lVal |= STATE_SYSTEM_FOCUSABLE;

  if (!view)
    return;

  if (!view->enabled())
    msaa_state->lVal |= STATE_SYSTEM_UNAVAILABLE;
  if (!view->visible())
    msaa_state->lVal |= STATE_SYSTEM_INVISIBLE;
  if (view->GetClassName() == CustomButton::kViewClassName) {
    CustomButton* button = static_cast<CustomButton*>(view);
    if (button->IsHotTracked())
      msaa_state->lVal |= STATE_SYSTEM_HOTTRACKED;
  }
  if (view->HasFocus())
    msaa_state->lVal |= STATE_SYSTEM_FOCUSED;

  // Add on any view-specific states.
  ui::AccessibleViewState view_state;
  view->GetAccessibleState(&view_state);
  msaa_state->lVal |= MSAAState(view_state.state);
}

string16 NativeViewAccessibilityWin::TextForIAccessibleText() {
  ui::AccessibleViewState state;
  view_->GetAccessibleState(&state);
  if (state.role == AccessibilityTypes::ROLE_TEXT)
    return state.value;
  else
    return state.name;
}

void NativeViewAccessibilityWin::HandleSpecialTextOffset(
    const string16& text, LONG* offset) {
  if (*offset == IA2_TEXT_OFFSET_LENGTH) {
    *offset = static_cast<LONG>(text.size());
  } else if (*offset == IA2_TEXT_OFFSET_CARET) {
    get_caretOffset(offset);
  }
}

ui::TextBoundaryType NativeViewAccessibilityWin::IA2TextBoundaryToTextBoundary(
    IA2TextBoundaryType ia2_boundary) {
  switch(ia2_boundary) {
    case IA2_TEXT_BOUNDARY_CHAR: return ui::CHAR_BOUNDARY;
    case IA2_TEXT_BOUNDARY_WORD: return ui::WORD_BOUNDARY;
    case IA2_TEXT_BOUNDARY_LINE: return ui::LINE_BOUNDARY;
    case IA2_TEXT_BOUNDARY_SENTENCE: return ui::SENTENCE_BOUNDARY;
    case IA2_TEXT_BOUNDARY_PARAGRAPH: return ui::PARAGRAPH_BOUNDARY;
    case IA2_TEXT_BOUNDARY_ALL: return ui::ALL_BOUNDARY;
    default:
      NOTREACHED();
      return ui::CHAR_BOUNDARY;
  }
}

LONG NativeViewAccessibilityWin::FindBoundary(
    const string16& text,
    IA2TextBoundaryType ia2_boundary,
    LONG start_offset,
    ui::TextBoundaryDirection direction) {
  HandleSpecialTextOffset(text, &start_offset);
  ui::TextBoundaryType boundary = IA2TextBoundaryToTextBoundary(ia2_boundary);
  std::vector<int32> line_breaks;
  return ui::FindAccessibleTextBoundary(
      text, line_breaks, boundary, start_offset, direction);
}

}  // namespace views
