// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/webview/webview.h"

#include "content/public/browser/browser_context.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/notification_source.h"
#include "content/public/browser/notification_types.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/render_widget_host_view.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_view.h"
#include "ipc/ipc_message.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/events/event.h"
#include "ui/views/accessibility/native_view_accessibility.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/views_delegate.h"

namespace views {

// static
const char WebView::kViewClassName[] =
    "ui/views/WebView";

////////////////////////////////////////////////////////////////////////////////
// WebView, public:

WebView::WebView(content::BrowserContext* browser_context)
    : wcv_holder_(new NativeViewHost),
      web_contents_(NULL),
      browser_context_(browser_context),
      allow_accelerators_(false) {
  AddChildView(wcv_holder_);
  NativeViewAccessibility::RegisterWebView(this);
}

WebView::~WebView() {
  NativeViewAccessibility::UnregisterWebView(this);
}

content::WebContents* WebView::GetWebContents() {
  CreateWebContentsWithSiteInstance(NULL);
  return web_contents_;
}

void WebView::CreateWebContentsWithSiteInstance(
    content::SiteInstance* site_instance) {
  if (!web_contents_) {
    wc_owner_.reset(CreateWebContents(browser_context_, site_instance));
    web_contents_ = wc_owner_.get();
    web_contents_->SetDelegate(this);
    AttachWebContents();
  }
}

void WebView::SetWebContents(content::WebContents* web_contents) {
  if (web_contents == web_contents_)
    return;
  DetachWebContents();
  wc_owner_.reset();
  web_contents_ = web_contents;
  AttachWebContents();
}

void WebView::LoadInitialURL(const GURL& url) {
  GetWebContents()->GetController().LoadURL(
      url, content::Referrer(), content::PAGE_TRANSITION_AUTO_TOPLEVEL,
      std::string());
}

void WebView::SetFastResize(bool fast_resize) {
  wcv_holder_->set_fast_resize(fast_resize);
}

void WebView::OnWebContentsFocused(content::WebContents* web_contents) {
  FocusManager* focus_manager = GetFocusManager();
  if (focus_manager)
    focus_manager->SetFocusedView(this);
}

void WebView::SetPreferredSize(const gfx::Size& preferred_size) {
  preferred_size_ = preferred_size;
  PreferredSizeChanged();
}

////////////////////////////////////////////////////////////////////////////////
// WebView, View overrides:

std::string WebView::GetClassName() const {
  return kViewClassName;
}

void WebView::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  wcv_holder_->SetSize(bounds().size());
}

void WebView::ViewHierarchyChanged(bool is_add, View* parent, View* child) {
  if (is_add)
    AttachWebContents();
}

bool WebView::SkipDefaultKeyEventProcessing(const ui::KeyEvent& event) {
  if (allow_accelerators_)
    return FocusManager::IsTabTraversalKeyEvent(event);

  // Don't look-up accelerators or tab-traversal if we are showing a non-crashed
  // TabContents.
  // We'll first give the page a chance to process the key events.  If it does
  // not process them, they'll be returned to us and we'll treat them as
  // accelerators then.
  return web_contents_ && !web_contents_->IsCrashed();
}

bool WebView::IsFocusable() const {
  // We need to be focusable when our contents is not a view hierarchy, as
  // clicking on the contents needs to focus us.
  return !!web_contents_;
}

void WebView::OnFocus() {
  if (web_contents_)
    web_contents_->GetView()->Focus();
}

void WebView::AboutToRequestFocusFromTabTraversal(bool reverse) {
  if (web_contents_)
    web_contents_->FocusThroughTabTraversal(reverse);
}

void WebView::GetAccessibleState(ui::AccessibleViewState* state) {
  state->role = ui::AccessibilityTypes::ROLE_GROUPING;
}

gfx::NativeViewAccessible WebView::GetNativeViewAccessible() {
  if (web_contents_) {
    content::RenderWidgetHostView* host_view =
        web_contents_->GetRenderWidgetHostView();
    if (host_view)
      return host_view->GetNativeViewAccessible();
  }
  return View::GetNativeViewAccessible();
}

gfx::Size WebView::GetPreferredSize() {
  if (preferred_size_ == gfx::Size())
    return View::GetPreferredSize();
  else
    return preferred_size_;
}

////////////////////////////////////////////////////////////////////////////////
// WebView, content::NotificationObserver implementation:

void WebView::Observe(int type,
                      const content::NotificationSource& source,
                      const content::NotificationDetails& details) {
  if (type == content::NOTIFICATION_RENDER_VIEW_HOST_CHANGED) {
    std::pair<content::RenderViewHost*, content::RenderViewHost*>*
        switched_details =
            content::Details<std::pair<content::RenderViewHost*,
                                       content::RenderViewHost*> >(
                details).ptr();
    RenderViewHostChanged(switched_details->first,
                          switched_details->second);
  } else if (type == content::NOTIFICATION_WEB_CONTENTS_DESTROYED) {
    WebContentsDestroyed(content::Source<content::WebContents>(source).ptr());
  } else {
    NOTREACHED();
  }
}

////////////////////////////////////////////////////////////////////////////////
// WebView, content::WebContentsDelegate implementation:

void WebView::WebContentsFocused(content::WebContents* web_contents) {
  DCHECK(wc_owner_.get());
  // The WebView is only the delegate of WebContentses it creates itself.
  OnWebContentsFocused(web_contents_);
}

////////////////////////////////////////////////////////////////////////////////
// WebView, AccessibleWebView implementation:

gfx::NativeViewAccessible WebView::AccessibleObjectFromChildId(long child_id) {
#if defined(OS_WIN) && defined(USE_AURA)
  if (!web_contents_)
    return NULL;
  content::RenderWidgetHostView* host_view =
      web_contents_->GetRenderWidgetHostView();
  if (host_view)
    return host_view->AccessibleObjectFromChildId(child_id);
  return NULL;
#else
  NOTIMPLEMENTED();
  return NULL;
#endif
}

View* WebView::AsView() {
  return this;
}

////////////////////////////////////////////////////////////////////////////////
// WebView, private:

void WebView::AttachWebContents() {
  // Prevents attachment if the WebView isn't already in a Widget, or it's
  // already attached.
  if (!GetWidget() || !web_contents_ ||
      wcv_holder_->native_view() == web_contents_->GetView()->GetNativeView()) {
    return;
  }

  if (web_contents_) {
    wcv_holder_->Attach(web_contents_->GetView()->GetNativeView());

    // The WebContentsView will not be focused automatically when it is
    // attached, so we need to pass on focus to it if the FocusManager thinks
    // the WebView is focused. Note that not every Widget has a focus manager.
    FocusManager* focus_manager = GetFocusManager();
    if (focus_manager && focus_manager->GetFocusedView() == this)
      web_contents_->GetView()->Focus();

    registrar_.Add(
        this,
        content::NOTIFICATION_RENDER_VIEW_HOST_CHANGED,
        content::Source<content::NavigationController>(
            &web_contents_->GetController()));
    registrar_.Add(
        this,
        content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
        content::Source<content::WebContents>(web_contents_));
  }

#if defined(OS_WIN) && defined(USE_AURA)
  web_contents_->SetParentNativeViewAccessible(
      parent()->GetNativeViewAccessible());
#endif
}

void WebView::DetachWebContents() {
  if (web_contents_) {
    wcv_holder_->Detach();
#if defined(OS_WIN) && !defined(USE_AURA)
    // TODO(beng): This should either not be necessary, or be done implicitly by
    // NativeViewHostWin on Detach(). As it stands, this is needed so that the
    // view of the detached contents knows to tell the renderer it's been
    // hidden.
    //
    // Moving this out of here would also mean we wouldn't be potentially
    // calling member functions on a half-destroyed WebContents.
    ShowWindow(web_contents_->GetView()->GetNativeView(), SW_HIDE);
#elif defined(OS_WIN) && defined(USE_AURA)
    web_contents_->SetParentNativeViewAccessible(NULL);
#endif
  }
  registrar_.RemoveAll();
}

void WebView::RenderViewHostChanged(content::RenderViewHost* old_host,
                                    content::RenderViewHost* new_host) {
  if (GetFocusManager()->GetFocusedView() == this)
    web_contents_->GetView()->Focus();
}

void WebView::WebContentsDestroyed(content::WebContents* web_contents) {
  DCHECK(web_contents == web_contents_);
  SetWebContents(NULL);
}

content::WebContents* WebView::CreateWebContents(
      content::BrowserContext* browser_context,
      content::SiteInstance* site_instance) {
  content::WebContents* contents = NULL;
  if (ViewsDelegate::views_delegate) {
    contents = ViewsDelegate::views_delegate->CreateWebContents(
        browser_context, site_instance);
  }

  if (!contents) {
    content::WebContents::CreateParams create_params(
        browser_context, site_instance);
    return content::WebContents::Create(create_params);
  }

  return contents;
}

}  // namespace views
