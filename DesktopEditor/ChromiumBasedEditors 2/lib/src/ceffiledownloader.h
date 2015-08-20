#ifndef CEF_FILE_DOWNLOADER_H
#define CEF_FILE_DOWNLOADER_H

#include "include/wrapper/cef_helpers.h"
#include "cefclient/client_handler.h"
#include "cefclient/client_handler.h"

class CAscDownloadHandler : public client::ClientHandler
{
public:
    std::wstring m_sUrl;
    std::wstring m_sDestination;

    int m_nProgress;
    bool m_bIsComplete;
    bool m_bIsCancelled;

public:
    CAscDownloadHandler()
    {
        m_nProgress = 0;
        m_bIsComplete = false;
        m_bIsCancelled = false;
    }

    virtual ~CAscDownloadHandler()
    {
    }

    virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      const CefString& target_url,
                                      const CefString& target_frame_name,
                                      const CefPopupFeatures& popupFeatures,
                                      CefWindowInfo& windowInfo,
                                      CefRefPtr<CefClient>& client,
                                      CefBrowserSettings& settings,
                                      bool* no_javascript_access)
    {
        CEF_REQUIRE_IO_THREAD();
        return true;
    }

    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

        if (!message_router_) {
          // Create the browser-side router for query handling.
          CefMessageRouterConfig config;
          message_router_ = CefMessageRouterBrowserSide::Create(config);

          // Register handlers with the router.
          //CreateMessageHandlers(message_handler_set_);
          MessageHandlerSet::const_iterator it = message_handler_set_.begin();
          for (; it != message_handler_set_.end(); ++it)
            message_router_->AddHandler(*(it), false);
        }

        // Disable mouse cursor change if requested via the command-line flag.
        if (mouse_cursor_change_disabled_)
          browser->GetHost()->SetMouseCursorChangeDisabled(true);

        if (!GetBrowser())   {
          base::AutoLock lock_scope(lock_);
          // We need to keep the main child window, but not popup windows
          browser_ = browser;
          browser_id_ = browser->GetIdentifier();
        } else if (browser->IsPopup()) {
          // Add to the list of popup browsers.
          popup_browsers_.push_back(browser);

          // Give focus to the popup browser. Perform asynchronously because the
          // parent window may attempt to keep focus after launching the popup.
          CefPostTask(TID_UI,
              base::Bind(&CefBrowserHost::SetFocus, browser->GetHost().get(), true));
        }

        browser_count_++;
    }

    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

        message_router_->OnBeforeClose(browser);

        if (GetBrowserId() == browser->GetIdentifier()) {
          {
            base::AutoLock lock_scope(lock_);
            // Free the browser pointer so that the browser can be destroyed
            browser_ = NULL;
          }

          if (osr_handler_.get()) {
            osr_handler_->OnBeforeClose(browser);
            osr_handler_ = NULL;
          }
        } else if (browser->IsPopup()) {
          // Remove from the browser popup list.
          BrowserList::iterator bit = popup_browsers_.begin();
          for (; bit != popup_browsers_.end(); ++bit) {
            if ((*bit)->IsSame(browser)) {
              popup_browsers_.erase(bit);
              break;
            }
          }
        }

        if (--browser_count_ == 0) {
          // All browser windows have closed.
          // Remove and delete message router handlers.
          MessageHandlerSet::const_iterator it = message_handler_set_.begin();
          for (; it != message_handler_set_.end(); ++it) {
            message_router_->RemoveHandler(*(it));
            delete *(it);
          }
          message_handler_set_.clear();
          message_router_ = NULL;

          // Quit the application message loop.
          //client::MainMessageLoop::Get()->Quit();
        }
    }

    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

        // Closing the main window requires special handling. See the DoClose()
        // documentation in the CEF header for a detailed destription of this
        // process.
        if (GetBrowserId() == browser->GetIdentifier())
        {
            base::AutoLock lock_scope(lock_);
            // Set a flag to indicate that the window close should be allowed.
            is_closing_ = true;
        }

        if (GetBrowserId() == browser->GetIdentifier())
        {
            SetParent(browser->GetHost()->GetWindowHandle(), NULL);
        }

        // Allow the close. For windowed browsers this will result in the OS close
        // event being sent.
        return false;
    }

    virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                  const CefString& message,
                                  const CefString& source,
                                  int line) OVERRIDE
    {
        return false;
    }

    virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                               const CefKeyEvent& event,
                               CefEventHandle os_event,
                               bool* is_keyboard_shortcut) OVERRIDE
    {
        return false;
    }

    virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefDownloadItem> download_item,
        const CefString& suggested_name,
        CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();
        CefString sDst;
        sDst.FromWString(m_sDestination);
        callback->Continue(sDst, false);
    }

    virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefDownloadItem> download_item,
                                          CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

        m_nProgress = download_item->GetPercentComplete();
        if (download_item->IsComplete())
        {
            m_bIsComplete = true;
        }
        if (download_item->IsCanceled())
        {
            m_bIsCancelled = true;
            m_bIsComplete = true;
        }
    }
};

#endif
