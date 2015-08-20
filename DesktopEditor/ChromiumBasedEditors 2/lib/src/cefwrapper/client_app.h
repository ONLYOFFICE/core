#ifndef CEF_ASC_CLIENT_APP_H_
#define CEF_ASC_CLIENT_APP_H_

#include "cefclient/common/client_app.h"
#include "cefclient/browser/client_app_browser.h"
#include "cefclient/renderer/client_app_renderer.h"
#include "cefclient/common/client_app_other.h"
#include "client_renderer.h"

class CAscClientAppBrowser : public client::ClientAppBrowser
{
public:
    CAscClientAppBrowser() : client::ClientAppBrowser()
    {
    }

    virtual ~CAscClientAppBrowser()
    {
    }
public:
    virtual void OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) OVERRIDE
    {
        if (process_type.empty())
        {
            command_line->AppendSwitch("--disable-accelerated-2d-canvas");
            command_line->AppendSwitchWithValue("--log-severity", "disable");
            command_line->AppendSwitch("--disable-d3d11");

            //command_line->AppendSwitch("--allow-file-access-from-files");
            //command_line->AppendSwitch("--allow-file-access");

            //command_line->AppendSwitch("--allow-running-insecure-content");

            //command_line->AppendSwitch("--disable-web-security");
        }
    }

public:
    IMPLEMENT_REFCOUNTING(CAscClientAppBrowser);
};

class CAscClientAppOther : public client::ClientAppOther
{
public:
    CAscClientAppOther() : client::ClientAppOther()
    {
    }

    virtual ~CAscClientAppOther()
    {
    }
public:
    virtual void OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) OVERRIDE
    {
        if (process_type.empty())
        {
            command_line->AppendSwitch("--disable-accelerated-2d-canvas");
            command_line->AppendSwitchWithValue("--log-severity", "disable");
            command_line->AppendSwitch("--disable-d3d11");

            //command_line->AppendSwitch("--allow-file-access-from-files");
            //command_line->AppendSwitch("--allow-file-access");

            //command_line->AppendSwitch("--allow-running-insecure-content");

            //command_line->AppendSwitch("--disable-web-security");
        }
    }

public:
    IMPLEMENT_REFCOUNTING(CAscClientAppOther);
};

class CAscClientAppRenderer : public client::ClientAppRenderer
{
public:
    CAscClientAppRenderer() : client::ClientAppRenderer()
    {
    }

    virtual ~CAscClientAppRenderer()
    {
    }
public:
    virtual void OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) OVERRIDE
    {
        if (process_type.empty())
        {
            command_line->AppendSwitch("--disable-accelerated-2d-canvas");
            command_line->AppendSwitchWithValue("--log-severity", "disable");
            command_line->AppendSwitch("--disable-d3d11");

            //command_line->AppendSwitch("--allow-file-access-from-files");
            //command_line->AppendSwitch("--allow-file-access");

            //command_line->AppendSwitch("--allow-running-insecure-content");

            //command_line->AppendSwitch("--disable-web-security");
        }
    }

    virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
                                       OVERRIDE
    {
        asc_client_renderer::CreateRenderDelegates(delegates_);

        DelegateSet::iterator it = delegates_.begin();
        for (; it != delegates_.end(); ++it)
          (*it)->OnRenderThreadCreated(this, extra_info);
    }
};

#endif
