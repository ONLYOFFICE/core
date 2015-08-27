#include "../../common/File.h"
#include "../include/applicationmanager.h"

#include "cefwrapper/client_app.h"
#include "cefwrapper/client_scheme.h"

#include "cefclient/browser/main_context_impl.h"

#ifdef WIN32
#include "cefclient/browser/main_message_loop_multithreaded_win.h"
#endif
#include "cefclient/browser/main_message_loop_std.h"

#if defined(_LINUX) && !defined(_MAC)

#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>

#include <X11/Xlib.h>

#undef Status   // Definition conflicts with cef_urlrequest.h
#undef Success  // Definition conflicts with cef_message_router.h

#include "include/wrapper/cef_helpers.h"
#endif

class CApplicationCEF_Private
{
public:
    CefRefPtr<client::ClientApp> m_app;
    scoped_ptr<client::MainContextImpl> context;
    scoped_ptr<client::MainMessageLoop> message_loop;

#if defined(_LINUX) && !defined(_MAC)
    CefScopedArgArray* argc_copy;
#endif

    CApplicationCEF_Private()
    {
#if defined(_LINUX) && !defined(_MAC)
        argc_copy = NULL;
#endif
    }
    ~CApplicationCEF_Private()
    {
        message_loop.reset();
        context.reset();

#if defined(_LINUX) && !defined(_MAC)
        RELEASEOBJECT(argc_copy);
#endif
    }
};

CApplicationCEF::CApplicationCEF()
{
    m_pInternal = new CApplicationCEF_Private();
}

void CApplicationCEF::Init_CEF(CAscApplicationManager* pManager, int argc, char* argv[])
{
    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();

#if defined(_LINUX) && !defined(_MAC)
    // Create a copy of |argv| on Linux because Chromium mangles the value
    // internally (see issue #620).
    m_pInternal->argc_copy = new CefScopedArgArray(argc, argv);
    char** argv_copy = m_pInternal->argc_copy->array();
#endif

    void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
    // Manage the life span of the sandbox information object. This is necessary
    // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
    CefScopedSandboxInfo scoped_sandbox;
    sandbox_info = scoped_sandbox.sandbox_info();
#endif

#ifdef WIN32
    CefMainArgs main_args((HINSTANCE)GetModuleHandle(NULL));

    // Parse command-line arguments.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW());

    // Create a ClientApp of the correct type.
    client::ClientApp::ProcessType process_type = client::ClientApp::GetProcessType(command_line);
    if (process_type == client::ClientApp::BrowserProcess)
        m_pInternal->m_app = new CAscClientAppBrowser();
    else if (process_type == client::ClientApp::RendererProcess ||
             process_type == client::ClientApp::ZygoteProcess)
        m_pInternal->m_app = new CAscClientAppRenderer();
    else if (process_type == client::ClientApp::OtherProcess)
        m_pInternal->m_app = new CAscClientAppOther();
#endif

#if defined(_LINUX) && !defined(_MAC)
    CefMainArgs main_args(argc, argv);

    // Parse command-line arguments.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);

    // Create a ClientApp of the correct type.
    client::ClientApp::ProcessType process_type = client::ClientApp::GetProcessType(command_line);
    if (process_type == client::ClientApp::BrowserProcess)
        m_pInternal->m_app = new CAscClientAppBrowser();
    else if (process_type == client::ClientApp::RendererProcess ||
             process_type == client::ClientApp::ZygoteProcess)
        m_pInternal->m_app = new CAscClientAppRenderer();
    else if (process_type == client::ClientApp::OtherProcess)
        m_pInternal->m_app = new CAscClientAppOther();
#endif

#ifdef _MAC
    CefMainArgs main_args(argc, argv);

    // Parse command-line arguments.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);

    // Create a ClientApp of the correct type.
    client::ClientApp::ProcessType process_type = client::ClientApp::GetProcessType(command_line);
    if (process_type == client::ClientApp::BrowserProcess)
        m_pInternal->m_app = new CAscClientAppBrowser();
    else if (process_type == client::ClientApp::RendererProcess ||
             process_type == client::ClientApp::ZygoteProcess)
        m_pInternal->m_app = new CAscClientAppRenderer();
    else if (process_type == client::ClientApp::OtherProcess)
        m_pInternal->m_app = new CAscClientAppOther();
#endif

#if 1
    // Execute the secondary process, if any.
    int exit_code = CefExecuteProcess(main_args, m_pInternal->m_app.get(), sandbox_info);
    if (exit_code >= 0)
    {
        // ERROR!!!
    }
#endif

    CefSettings settings;

#if !defined(CEF_USE_SANDBOX)
    settings.no_sandbox = true;
#endif

#if 0
    settings.single_process = 1;
#endif

    // Populate the settings based on command line arguments.
    //AppGetSettings(settings);
    m_pInternal->context.reset(new client::MainContextImpl(command_line, true));
    m_pInternal->context->PopulateSettings(&settings);

#ifdef WIN32
    settings.multi_threaded_message_loop = 1;
    //settings.windowless_rendering_enabled = 1;
#endif

#ifdef WIN32
    if (settings.multi_threaded_message_loop)
        m_pInternal->message_loop.reset(new client::MainMessageLoopMultithreadedWin);
    else
        m_pInternal->message_loop.reset(new client::MainMessageLoopStd);
#else
    m_pInternal->message_loop.reset(new client::MainMessageLoopStd);
#endif

    // ASC command line props
#ifdef WIN32
    std::string sCommandLine = GetCommandLineA();
    if (sCommandLine.find("--ascdesktop-support-debug-info") != std::string::npos)
        pManager->SetDebugInfoSupport(true);
#else
    for (int i = 0; i < argc; ++i)
    {
        std::string sCommandLine(argv[i]);
        if (sCommandLine.find("--ascdesktop-support-debug-info") != std::string::npos)
        {
            pManager->SetDebugInfoSupport(true);
            break;
        }
    }
#endif

    std::wstring sCachePath = pManager->m_oSettings.cache_path;

    cef_string_t _cache;
    memset(&_cache, 0, sizeof(_cache));
    cef_string_from_wide(sCachePath.c_str(), sCachePath.length(), &_cache);
    settings.cache_path = _cache;

    // Initialize CEF.
    bool bInit = CefInitialize(main_args, settings, m_pInternal->m_app.get(), sandbox_info);
    bool bIsInitScheme = asc_scheme::InitScheme();

#if defined(_LINUX) && !defined(_MAC)
    // The Chromium sandbox requires that there only be a single thread during
    // initialization. Therefore initialize GTK after CEF.
    gtk_init(&argc, &argv_copy);

    // Perform gtkglext initialization required by the OSR example.
    gtk_gl_init(&argc, &argv_copy);
#endif

    pManager->SetApplication(this);
    return;
}

CApplicationCEF::~CApplicationCEF()
{
    Close();
}

void CApplicationCEF::Close()
{
    if (NULL != m_pInternal)
    {
        // Shut down CEF.
        CefShutdown();

        RELEASEOBJECT(m_pInternal);
    }
}
