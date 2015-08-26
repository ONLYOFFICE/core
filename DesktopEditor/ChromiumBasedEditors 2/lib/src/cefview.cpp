#include "../include/applicationmanager.h"

#include "include/cef_browser.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "cefclient/browser/client_handler.h"
#include "cefclient/common/client_switches.h"
#include "cefclient/renderer/client_renderer.h"

#include "../../common/File.h"
#include "../../graphics/Timer.h"
#include "../../graphics/TemporaryCS.h"
#include "../../graphics/GraphicsRenderer.h"
#include "../../fontengine/ApplicationFonts.h"

#include "../src/cookiesworker.h"

#include "filedownloader.h"

#include "include/cef_menu_model.h"

#include "include/wrapper/cef_stream_resource_handler.h"
#include "include/wrapper/cef_byte_read_handler.h"

#ifdef WIN32
LRESULT CALLBACK MyMouseHook(int nCode, WPARAM wp, LPARAM lp)
{
    MOUSEHOOKSTRUCT *pmh = (MOUSEHOOKSTRUCT *) lp;

    HWND h1 = GetParent(pmh->hwnd); // cefbrowser
    HWND h2 = GetParent(h1);        // cefview

    if (nCode == HC_ACTION && wp == WM_MBUTTONUP)
    {
        SendMessageA(h2, WM_USER + 1, 0, 0);
    }

    return CallNextHookEx(NULL, nCode, wp, lp);
}
#endif

class CPagePrintData
{
public:
    std::string Base64;
    double      Width;
    double      Height;

    CPagePrintData()
    {
        Width   = 0;
        Height  = 0;
    }
};

class CPrintData
{
public:
    CArray<CPagePrintData> m_arPages;
    int m_nCurrentPage;
    std::wstring m_sFrameUrl;
    std::wstring m_sDocumentUrl;
    std::wstring m_sThemesUrl;

    std::map<std::wstring, std::wstring> m_mapImages;

    std::wstring m_sDocumentImagesPath;
    std::wstring m_sPresentationThemesPath;

    CFontManager* m_pFontManager;
    CImageFilesCache* m_pCache;

    CApplicationFonts* m_pApplicationFonts;
    AscEditorType m_eEditorType;

public:
    enum CommandType
    {
        ctPenXML						= 0,
        ctPenColor						= 1,
        ctPenAlpha						= 2,
        ctPenSize						= 3,
        ctPenDashStyle					= 4,
        ctPenLineStartCap				= 5,
        ctPenLineEndCap				    = 6,
        ctPenLineJoin					= 7,
        ctPenDashPatern				    = 8,
        ctPenDashPatternCount			= 9,
        ctPenDashOffset				    = 10,
        ctPenAlign						= 11,
        ctPenMiterLimit				    = 12,

        // brush
        ctBrushXML						= 20,
        ctBrushType					    = 21,
        ctBrushColor1					= 22,
        ctBrushColor2					= 23,
        ctBrushAlpha1					= 24,
        ctBrushAlpha2					= 25,
        ctBrushTexturePath				= 26,
        ctBrushTextureAlpha			    = 27,
        ctBrushTextureMode				= 28,
        ctBrushRectable				    = 29,
        ctBrushRectableEnabled 		    = 30,
        ctBrushGradient                 = 31,

        // font
        ctFontXML						= 40,
        ctFontName						= 41,
        ctFontSize						= 42,
        ctFontStyle					    = 43,
        ctFontPath						= 44,
        ctFontGID						= 45,
        ctFontCharSpace				    = 46,

        // shadow
        ctShadowXML					    = 50,
        ctShadowVisible				    = 51,
        ctShadowDistanceX				= 52,
        ctShadowDistanceY				= 53,
        ctShadowBlurSize				= 54,
        ctShadowColor					= 55,
        ctShadowAlpha					= 56,

        // edge
        ctEdgeXML						= 70,
        ctEdgeVisible					= 71,
        ctEdgeDistance					= 72,
        ctEdgeColor					    = 73,
        ctEdgeAlpha					    = 74,

        // text
        ctDrawText						= 80,
        ctDrawTextEx					= 81,

        // pathcommands
        ctPathCommandMoveTo			    = 91,
        ctPathCommandLineTo			    = 92,
        ctPathCommandLinesTo			= 93,
        ctPathCommandCurveTo			= 94,
        ctPathCommandCurvesTo			= 95,
        ctPathCommandArcTo		        = 96,
        ctPathCommandClose				= 97,
        ctPathCommandEnd				= 98,
        ctDrawPath						= 99,
        ctPathCommandStart				= 100,
        ctPathCommandGetCurrentPoint	= 101,
        ctPathCommandText				= 102,
        ctPathCommandTextEx			    = 103,

        // image
        ctDrawImage					    = 110,
        ctDrawImageFromFile			    = 111,

        ctSetParams					    = 120,

        ctBeginCommand					= 121,
        ctEndCommand					= 122,

        ctSetTransform					= 130,
        ctResetTransform				= 131,

        ctClipMode						= 140,

        ctCommandLong1					= 150,
        ctCommandDouble1				= 151,
        ctCommandString1				= 152,
        ctCommandLong2					= 153,
        ctCommandDouble2				= 154,
        ctCommandString2				= 155,

        ctPageWidth                     = 200,
        ctPageHeight                    = 201,

        ctPageStart                     = 202,
        ctPageEnd                       = 203,

        // gradients

        ctGradientFill					= 220,
        ctGradientFillXML				= 221,

        ctGradientStroke				= 222,
        ctGradientStrokeXML				= 223,

        ctError						    = 255
    };

public:
    CPrintData()
    {
        m_pFontManager = NULL;
        m_pCache = NULL;

        m_nCurrentPage = -1;
        m_eEditorType = etDocument;
    }
    ~CPrintData()
    {
        RELEASEINTERFACE(m_pFontManager);
        RELEASEINTERFACE(m_pCache);
    }

    void Print_Start()
    {
        m_pFontManager = m_pApplicationFonts->GenerateFontManager();
        CFontsCache* pFontsCache = new CFontsCache();
        pFontsCache->SetStreams(m_pApplicationFonts->GetStreams());
        m_pFontManager->SetOwnerCache(pFontsCache);

        m_pCache = new CImageFilesCache(m_pApplicationFonts);

        m_nCurrentPage = -1;
    }

    void Print_End()
    {
        RELEASEINTERFACE(m_pFontManager);
        RELEASEINTERFACE(m_pCache);

        m_pApplicationFonts->GetStreams()->Clear();

        for (std::map<std::wstring, std::wstring>::iterator iter = m_mapImages.begin(); iter != m_mapImages.end(); iter++)
        {
            NSFile::CFileBinary::Remove(iter->second);
        }
        m_mapImages.clear();

        m_nCurrentPage = -1;

        m_eEditorType = etDocument;
    }

    void Print(NSEditorApi::CAscPrinterContextBase* pContext, const CAscPrintSettings& settings, const int& nPageIndex);
    void DrawOnRenderer(CGraphicsRenderer* pRenderer, int nPageIndex);
    void TestSaveToRasterFile(std::wstring sFile, int nWidth, int nHeight, int nPageIndex);
    std::wstring GetImagePath(const std::wstring& sPath);
    void FitToPage(float fSourceWidth, float  fSourceHeight, float  fTargetWidth, float fTargetHeight, float& fResX, float& fResY, float& fResWidth, float& fResHeight);

    std::wstring DownloadImage(const std::wstring& strFile)
    {
        CFileDownloader oDownloader(strFile, false);
        oDownloader.Start( 0 );
        while ( oDownloader.IsRunned() )
        {
            ::Sleep( 10 );
        }
        std::wstring strFileName;
        if ( oDownloader.IsFileDownloaded() )
        {
            strFileName = oDownloader.GetFilePath();
        }
        else
        {
            strFileName = oDownloader.GetFilePath();
            NSFile::CFileBinary::Remove(strFileName);
            strFileName = L"";
        }
        return strFileName;
    }

    void CalculateImagePaths()
    {
        m_sDocumentImagesPath = L"";
        if (CFileDownloader::IsNeedDownload(m_sFrameUrl) && !CFileDownloader::IsNeedDownload(m_sDocumentUrl))
        {
            if (0 == m_sDocumentUrl.find(wchar_t('/')))
            {
                // нужно брать корень сайта
                int nPos = m_sFrameUrl.find(L"//");
                if (nPos != std::wstring::npos)
                {
                    nPos = m_sFrameUrl.find(wchar_t('/'), nPos + 3);
                    if (nPos != std::wstring::npos)
                    {
                        m_sDocumentImagesPath = m_sFrameUrl.substr(0, nPos);
                        m_sDocumentImagesPath += m_sDocumentUrl;
                    }
                }
                if (m_sDocumentImagesPath.empty())
                {
                    m_sDocumentImagesPath = m_sFrameUrl;
                    m_sDocumentImagesPath += (L"/" + m_sDocumentUrl);
                }
            }
            else
            {
                // брать место урла
                int nPos = m_sFrameUrl.find_last_of(wchar_t('/'));
                if (std::wstring::npos != nPos)
                {
                    m_sDocumentImagesPath = m_sFrameUrl.substr(0, nPos + 1);
                }
                else
                {
                    m_sDocumentImagesPath = m_sFrameUrl;
                }
                m_sDocumentImagesPath += (L"/" + m_sDocumentUrl);
            }
        }
        else
        {
            m_sDocumentImagesPath = m_sDocumentUrl;
        }

        m_sPresentationThemesPath = L"";
        if (CFileDownloader::IsNeedDownload(m_sFrameUrl) && !CFileDownloader::IsNeedDownload(m_sThemesUrl))
        {
            if (0 == m_sThemesUrl.find(wchar_t('/')))
            {
                // нужно брать корень сайта
                int nPos = m_sFrameUrl.find(L"//");
                if (nPos != std::wstring::npos)
                {
                    nPos = m_sFrameUrl.find(wchar_t('/'), nPos + 3);
                    if (nPos != std::wstring::npos)
                    {
                        m_sPresentationThemesPath = m_sFrameUrl.substr(0, nPos);
                        //m_sPresentationThemesPath += m_sThemesUrl;
                    }
                }
                if (m_sPresentationThemesPath.empty())
                {
                    m_sPresentationThemesPath = m_sFrameUrl;
                    //m_sPresentationThemesPath += (L"/" + m_sThemesUrl);
                }
            }
            else
            {
                // брать место урла
                int nPos = m_sFrameUrl.find_last_of(wchar_t('/'));
                if (std::wstring::npos != nPos)
                {
                    m_sPresentationThemesPath = m_sFrameUrl.substr(0, nPos + 1);
                }
                else
                {
                    m_sPresentationThemesPath = m_sFrameUrl;
                }
                //m_sPresentationThemesPath += (L"/" + m_sThemesUrl);
            }
        }
        else
        {
            m_sPresentationThemesPath = m_sThemesUrl;
        }
    }
};

class CCefBinaryFileReaderCounter : public CefBase
{
public:
    BYTE* data;

    CCefBinaryFileReaderCounter(BYTE* _data = NULL)
    {
        this->AddRef();
        data = _data;
    }

    virtual ~CCefBinaryFileReaderCounter()
    {
        RELEASEARRAYOBJECTS(data);
    }

public:
    IMPLEMENT_REFCOUNTING(CCefBinaryFileReaderCounter);
};

class CCefView_Private : public NSEditorApi::IMenuEventDataBase
{
public:
    CefRefPtr<CAscClientHandler>        m_handler;
    CAscApplicationManager*             m_pManager;
    CCefViewWidgetImpl*                 m_pWidgetImpl;

    std::map<std::string, std::string>  m_cookies;

    int m_nParentId;
    bool m_bIsModified;

    bool m_bIsNativeSave;

    bool m_bIsCancelDownload;
    DWORD m_dwTimeMouseWheelUp;

    CPrintData m_oPrintData;

    bool m_bIsMouseHook;

    std::wstring m_strUrl;

    CefRefPtr<CefBeforeDownloadCallback> m_before_callback;

public:
    CCefView_Private()
    {
        m_pManager = NULL;
        m_pWidgetImpl = NULL;
        m_nParentId = -1;
        m_bIsModified = false;
        m_bIsNativeSave = false;
        m_bIsCancelDownload = false;
        m_dwTimeMouseWheelUp = (DWORD)-1;

        m_bIsMouseHook = false;
        m_strUrl = L"";

        m_before_callback = NULL;
    }

    ~CCefView_Private()
    {
    }

    void CloseBrowser(bool _force_close);
};

class CAscClientHandler : public client::ClientHandler, public CCookieFoundCallback, public client::ClientHandler::Delegate
{
public:
    CCefView* m_pParent;
    bool m_bIsLoaded;

    bool m_bIsEditorTypeSet;
    int m_nBeforeBrowserCounter;

    CefRefPtr<CefBrowser> browser_;
    int browser_id_;

    enum client_menu_ids
    {
        CLIENT_ID_SHOW_DEVTOOLS   = MENU_ID_USER_FIRST,
        CLIENT_ID_CLOSE_DEVTOOLS,
        CLIENT_ID_INSPECT_ELEMENT
    };

public:
    CAscClientHandler() : client::ClientHandler(this, false, "https://onlyoffice.com/")
    {
        m_pParent = NULL;
        m_bIsLoaded = false;

        m_bIsEditorTypeSet = false;
        m_nBeforeBrowserCounter = 0;

        browser_id_ = 0;
    }

    virtual ~CAscClientHandler()
    {
    }

    CefRefPtr<CefBrowser> GetBrowser() const
    {
        return browser_;
    }

    int GetBrowserId() const
    {
        return browser_id_;
    }

    bool CheckPopup(std::wstring sUrl, bool bIsBeforeBrowse = false)
    {
        NSEditorApi::CAscMenuEventListener* pListener = NULL;
        if (NULL != m_pParent && NULL != m_pParent->GetAppManager())
            pListener = m_pParent->GetAppManager()->GetEventListener();

        bool bIsEditor      = (sUrl.find(L"files/doceditor.aspx?fileid") == std::wstring::npos) ? false : true;

        bool bIsDownload    = false;
        if (sUrl.find(L"filehandler.ashx?action=download") != std::wstring::npos)
            bIsDownload     = true;
        else if (sUrl.find(L"filehandler.ashx?action=view") != std::wstring::npos)
            bIsDownload     = true;

        if (!bIsBeforeBrowse && !bIsEditor && !bIsDownload)
        {
            if (m_pParent && m_pParent->GetType() == cvwtEditor || m_pParent->GetType() == cvwtSimple)
            {
                NSEditorApi::CAscOnOpenExternalLink* pData = new NSEditorApi::CAscOnOpenExternalLink();
                pData->put_Id(m_pParent->GetId());
                pData->put_Url(sUrl);

                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_ONOPENLINK;
                pEvent->m_pData = pData;

                pListener->OnEvent(pEvent);
            }
            return true;
        }

        if (bIsDownload)
        {
            if (NULL != m_pParent && NULL != pListener)
            {
                NSEditorApi::CAscDownloadFileInfo* pData = new NSEditorApi::CAscDownloadFileInfo();
                pData->put_Url(sUrl);
                pData->put_FilePath(L"");
                pData->put_Percent(0);
                pData->put_IsComplete(false);
                pData->put_Id(m_pParent->GetId());

                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD_START;
                pEvent->m_pData = pData;

                pListener->OnEvent(pEvent);
            }

            return true;
        }

        if (bIsEditor)
        {
            sUrl += L"&placement=desktop";

            if (NULL != pListener)
            {
                NSEditorApi::CAscCreateTab* pData = new NSEditorApi::CAscCreateTab();
                pData->put_Url(sUrl);

#if 1
                if (m_pParent && m_pParent->m_pInternal->m_dwTimeMouseWheelUp != ((DWORD)-1))
                {
                    DWORD dwDelta = NSTimers::GetTickCount() - m_pParent->m_pInternal->m_dwTimeMouseWheelUp;
                    if (dwDelta < 500)
                        pData->put_Active(false);
                }
#endif

                if (true)
                {
                    CCefView* pEqual = m_pParent->GetAppManager()->GetViewByUrl(sUrl);
                    if (NULL != pEqual)
                        pData->put_IdEqual(pEqual->GetId());
                }

                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_CREATETAB;
                pEvent->m_pData = pData;

                pListener->OnEvent(pEvent);
            }

            return true;
        }

        return false;
    }

    virtual bool OnBeforePopup(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& target_url,
        const CefString& target_frame_name,
        CefLifeSpanHandler::WindowOpenDisposition target_disposition,
        bool user_gesture,
        const CefPopupFeatures& popupFeatures,
        CefWindowInfo& windowInfo,
        CefRefPtr<CefClient>& client,
        CefBrowserSettings& settings,
        bool* no_javascript_access) OVERRIDE
    {
        CEF_REQUIRE_IO_THREAD();

        std::wstring sUrl = target_url.ToWString();

        CheckPopup(sUrl);

        return true;
    }

    virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefRequest> request,
                                bool is_redirect)
    {
        m_nBeforeBrowserCounter++;

        std::wstring sUrl = request->GetURL().ToWString();
        CefViewWrapperType eType = this->m_pParent->GetType();

        if (m_pParent && m_pParent->GetType() == cvwtSimple)
        {
            bool bHandled = CheckPopup(sUrl, true);
            if (bHandled)
                return true;

#if 0
            // блокируем, если не первый раз
            if (m_nBeforeBrowserCounter > 1)
                return true;
#else
            // если делать, как выше, то множественные логины отрубятся.
            // и дебаггер тоже
            if (sUrl.find(L"files/#") != std::wstring::npos)
                return true;
#endif
        }

        bool ret = client::ClientHandler::OnBeforeBrowse(browser, frame, request, is_redirect);
        if (NULL != m_pParent)
        {            
            m_pParent->resizeEvent();
            m_pParent->focus();

            if (!m_bIsLoaded)
            {
                if (m_pParent->GetType() == cvwtEditor)
                {
                    std::string sName = frame->GetName().ToString();
                    if (sName == "frameEditor")
                    {
                        m_pParent->GetWidgetImpl()->child_loaded();
                        m_bIsLoaded = true;

#if 0
                        std::wstring sCode = L"if (window[\"AscDesktopEditor\"]) { window[\"AscDesktopEditor\"][\"Set_App_Data\"](\"";
                        sCode += m_pParent->GetAppManager()->m_oSettings.app_data_path;
                        sCode += L"\"); }";

                        CefString sCodeCef;
                        sCodeCef.FromWString(sCode);

                        CefRefPtr<CefFrame> _frame = browser->GetFrame("frameEditor");
                        if (_frame)
                        {
                            _frame->ExecuteJavaScript(sCodeCef, frame->GetURL(), 0);
                        }
#endif
                    }
                }
                else
                {
                    m_pParent->GetWidgetImpl()->child_loaded();
                    m_bIsLoaded = true;
                }
            }
        }
        return ret;
    }

    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message)
    {
        CEF_REQUIRE_UI_THREAD();

        NSEditorApi::CAscMenuEventListener* pListener = NULL;
        if (NULL != m_pParent && NULL != m_pParent->GetAppManager())
            pListener = m_pParent->GetAppManager()->GetEventListener();

        // Check for messages from the client renderer.
        std::string message_name = message->GetName();
        if (message_name == "EditorType" && !m_bIsEditorTypeSet)
        {
            m_bIsEditorTypeSet = true;

            int nType = message->GetArgumentList()->GetInt(0);

            if (NULL != pListener)
            {
                NSEditorApi::CAscTabEditorType* pData = new NSEditorApi::CAscTabEditorType();
                pData->put_Id(m_pParent->GetId());
                pData->put_Type(nType);

                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_TABEDITORTYPE;
                pEvent->m_pData = pData;

                pListener->OnEvent(pEvent);
            }

            return true;
        }
        else if (message_name == "spell_check_task")
        {
            if (m_pParent && m_pParent->GetAppManager())
            {
                m_pParent->GetAppManager()->SpellCheck(message->GetArgumentList()->GetInt(0),
                                                       message->GetArgumentList()->GetString(1).ToString(),
                                                       message->GetArgumentList()->GetInt(2));
            }

            return true;
        }
        else if (message_name == "create_editor_api")
        {
            if (m_pParent)
            {
                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_CONTROL_ID;
                m_pParent->Apply(pEvent);

                CAscApplicationManager* pManager = m_pParent->GetAppManager();
                if (pManager)
                    pManager->SendKeyboardAttack();

                NSEditorApi::CAscMenuEvent* pEvent2 = new NSEditorApi::CAscMenuEvent();
                pEvent2->m_nType = ASC_MENU_EVENT_TYPE_CEF_SYNC_COMMAND;
                m_pParent->Apply(pEvent2);
            }

            return true;
        }
        else if (message_name == "is_cookie_present")
        {
            CCefCookieVisitor* pVisitor = new CCefCookieVisitor();
            pVisitor->m_pCallback       = this;
            pVisitor->m_sDomain         = message->GetArgumentList()->GetString(0).ToString();
            pVisitor->m_sCookieSearch   = message->GetArgumentList()->GetString(1).ToString();

            pVisitor->CheckCookiePresent(CefCookieManager::GetGlobalManager(NULL));

            return true;
        }
        else if (message_name == "set_cookie")
        {
            CCefCookieSetter* pVisitor = new CCefCookieSetter();
            pVisitor->m_pCallback       = this;

            pVisitor->m_sUrl            = message->GetArgumentList()->GetString(0).ToString();
            pVisitor->m_sDomain         = message->GetArgumentList()->GetString(1).ToString();
            pVisitor->m_sPath           = message->GetArgumentList()->GetString(2).ToString();
            pVisitor->m_sCookieKey      = message->GetArgumentList()->GetString(3).ToString();
            pVisitor->m_sCookieValue    = message->GetArgumentList()->GetString(4).ToString();

            pVisitor->SetCookie(CefCookieManager::GetGlobalManager(NULL));

            return true;
        }
        else if (message_name == "onDocumentModifiedChanged")
        {
            if (m_pParent && m_pParent->GetAppManager()->GetEventListener())
            {
                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_MODIFY_CHANGED;

                bool bValue = message->GetArgumentList()->GetBool(0);

                NSEditorApi::CAscDocumentModifyChanged* pData = new NSEditorApi::CAscDocumentModifyChanged();
                pData->put_Id(m_pParent->GetId());
                pData->put_Changed(bValue);
                pEvent->m_pData = pData;

                m_pParent->SetModified(bValue);
                m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);
            }
            return true;
        }
        else if (message_name == "set_document_name")
        {
            if (m_pParent && m_pParent->GetAppManager()->GetEventListener())
            {
                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_DOCUMENT_NAME;

                NSEditorApi::CAscDocumentName* pData = new NSEditorApi::CAscDocumentName();
                pData->put_Id(m_pParent->GetId());
                pData->put_Name(message->GetArgumentList()->GetString(0).ToWString());

                pEvent->m_pData = pData;

                m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);
            }
            return true;
        }
        else if (message_name == "on_document_save")
        {
            if (m_pParent && m_pParent->GetAppManager()->GetEventListener() && m_pParent->m_pInternal->m_bIsNativeSave)
            {
                m_pParent->m_pInternal->m_bIsNativeSave = false;

                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_ONSAVE;

                NSEditorApi::CAscTypeId* pData = new NSEditorApi::CAscTypeId();
                pData->put_Id(m_pParent->GetId());

                pEvent->m_pData = pData;

                m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);
            }
            return true;
        }
        else if (message_name == "js_message")
        {
            if (m_pParent && m_pParent->GetAppManager()->GetEventListener())
            {
                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_JS_MESSAGE;

                NSEditorApi::CAscJSMessage* pData = new NSEditorApi::CAscJSMessage();
                pData->put_Id(m_pParent->GetId());
                pData->put_Name(message->GetArgumentList()->GetString(0).ToWString());
                pData->put_Value(message->GetArgumentList()->GetString(1).ToWString());

                pEvent->m_pData = pData;

                m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);
            }
            return true;
        }
        else if (message_name == "print_start")
        {
            if (m_pParent)
            {
                m_pParent->m_pInternal->m_oPrintData.m_sDocumentUrl = message->GetArgumentList()->GetString(0).ToWString();
                m_pParent->m_pInternal->m_oPrintData.m_sFrameUrl = message->GetArgumentList()->GetString(2).ToWString();
                m_pParent->m_pInternal->m_oPrintData.m_arPages.SetCount(message->GetArgumentList()->GetInt(1));
                m_pParent->m_pInternal->m_oPrintData.m_sThemesUrl = message->GetArgumentList()->GetString(3).ToWString();
                m_pParent->m_pInternal->m_oPrintData.m_nCurrentPage = message->GetArgumentList()->GetInt(4);

                m_pParent->m_pInternal->m_oPrintData.CalculateImagePaths();

                if (m_pParent->GetAppManager()->GetEventListener())
                {
                    NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                    pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_ONBEFORE_PRINT_START;

                    NSEditorApi::CAscTypeId* pData = new NSEditorApi::CAscTypeId();
                    pData->put_Id(m_pParent->GetId());
                    pEvent->m_pData = pData;

                    m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);
                }
            }
            return true;
        }
        else if (message_name == "print_page")
        {
            if (m_pParent)
            {
                int nIndex = message->GetArgumentList()->GetInt(1);
                int nProgress = 100 * (nIndex + 1) / m_pParent->m_pInternal->m_oPrintData.m_arPages.GetCount();

                if (nProgress >= 0 && nProgress <= 100)
                {
                    m_pParent->m_pInternal->m_oPrintData.m_arPages[nIndex].Base64 = message->GetArgumentList()->GetString(0).ToString();
                    m_pParent->m_pInternal->m_oPrintData.m_arPages[nIndex].Width  = message->GetArgumentList()->GetDouble(2);
                    m_pParent->m_pInternal->m_oPrintData.m_arPages[nIndex].Height = message->GetArgumentList()->GetDouble(3);
                }

                if (m_pParent->GetAppManager()->GetEventListener())
                {
                    NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                    pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_ONBEFORE_PRINT_PROGRESS;

                    NSEditorApi::CAscPrintProgress* pData = new NSEditorApi::CAscPrintProgress();
                    pData->put_Id(m_pParent->GetId());
                    pData->put_Progress(nProgress);

                    pEvent->m_pData = pData;

                    m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);
                }
            }
            return true;
        }
        else if (message_name == "print_end")
        {
            if (m_pParent)
            {
                if (m_pParent->GetAppManager()->GetEventListener())
                {
                    NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                    pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_ONBEFORE_PRINT_END;

                    NSEditorApi::CAscPrintEnd* pData = new NSEditorApi::CAscPrintEnd();
                    pData->put_Id(m_pParent->GetId());
                    pData->put_PagesCount(m_pParent->m_pInternal->m_oPrintData.m_arPages.GetCount());
                    pData->put_CurrentPage(m_pParent->m_pInternal->m_oPrintData.m_nCurrentPage);
                    pEvent->m_pData = pData;

                    m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);

#if 0
                    // TEST
                    m_pParent->m_pInternal->m_oPrintData.Print_Start();

                    for (int i = 0; i < m_pParent->m_pInternal->m_oPrintData.m_arPages.GetCount(); ++i)
                        m_pParent->m_pInternal->m_oPrintData.TestSaveToRasterFile(L"D:\\test_page" + std::to_wstring(i) + L".png", 793, 1122, i);

                    m_pParent->m_pInternal->m_oPrintData.Print_End();
#endif
                }
            }
            return true;
        }
        else if (message_name == "print")
        {
            if (m_pParent)
            {
                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_PRINT_START;
                m_pParent->Apply(pEvent);
            }
            return true;
        }
        else if (message_name == "load_js")
        {
            if (m_pParent)
            {
                NSEditorApi::CAscEditorScript* pData = new NSEditorApi::CAscEditorScript();
                pData->put_Url(message->GetArgumentList()->GetString(0).ToWString());
                pData->put_Destination(message->GetArgumentList()->GetString(1).ToWString());
                pData->put_Id(m_pParent->GetId());
                pData->put_FrameId(message->GetArgumentList()->GetInt(2));

                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_SCRIPT_EDITOR_VERSION;
                pEvent->m_pData = pData;

                m_pParent->GetAppManager()->Apply(pEvent);
            }
            return true;
        }
        else if (message_name == "onfullscreenenter")
        {
            if (m_pParent && m_pParent->GetAppManager() && m_pParent->GetAppManager()->GetEventListener())
            {
                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent(ASC_MENU_EVENT_TYPE_CEF_ONFULLSCREENENTER);
                m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);
            }

            return true;
        }
        else if (message_name == "onfullscreenleave")
        {
            if (m_pParent && m_pParent->GetAppManager() && m_pParent->GetAppManager()->GetEventListener())
            {
                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent(ASC_MENU_EVENT_TYPE_CEF_ONFULLSCREENLEAVE);
                m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);
            }

            return true;
        }

        if (message_router_->OnProcessMessageReceived(browser, source_process, message))
        {
            return true;
        }

        return false;
    }

    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE
    {
        client::ClientHandler::OnAfterCreated(browser);
        if (!GetBrowser())
        {
            // We need to keep the main child window, but not popup windows
            browser_ = browser;
            browser_id_ = browser->GetIdentifier();
        }
    }

    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

        if (--browser_count_ == 0)
        {
            // Remove and delete message router handlers.
            MessageHandlerSet::const_iterator it =
                message_handler_set_.begin();
            for (; it != message_handler_set_.end(); ++it)
            {
                message_router_->RemoveHandler(*(it));
                delete *(it);
            }
            message_handler_set_.clear();
            message_router_ = NULL;
        }

        NotifyBrowserClosed(browser);
    }

    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

        NotifyBrowserClosing(browser);

        if (GetBrowserId() == browser->GetIdentifier())
        {
            SetParent(browser->GetHost()->GetWindowHandle(), NULL);
        }

        // Allow the close. For windowed browsers this will result in the OS close
        // event being sent.
        return false;
    }

    virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

        int _remove_commands[] = {MENU_ID_BACK,
                                  MENU_ID_FORWARD,
                                  MENU_ID_RELOAD,
                                  MENU_ID_RELOAD_NOCACHE,
                                  MENU_ID_STOPLOAD,

                                  // Editing.
                                  MENU_ID_UNDO,
                                  MENU_ID_REDO,
                                  //MENU_ID_CUT,
                                  //MENU_ID_COPY,
                                  //MENU_ID_PASTE,
                                  //MENU_ID_DELETE,
                                  //MENU_ID_SELECT_ALL,

                                  // Miscellaneous.
                                  MENU_ID_FIND,
                                  MENU_ID_PRINT,
                                  MENU_ID_VIEW_SOURCE,

                                  // Spell checking word correction suggestions.
                                  MENU_ID_SPELLCHECK_SUGGESTION_0,
                                  MENU_ID_SPELLCHECK_SUGGESTION_1,
                                  MENU_ID_SPELLCHECK_SUGGESTION_2,
                                  MENU_ID_SPELLCHECK_SUGGESTION_3,
                                  MENU_ID_SPELLCHECK_SUGGESTION_4,
                                  MENU_ID_SPELLCHECK_SUGGESTION_LAST,
                                  MENU_ID_NO_SPELLING_SUGGESTIONS,
                                  MENU_ID_ADD_TO_DICTIONARY
                                 };

        int _remove_count = 23 - 5;
        for (int i = 0; i < _remove_count; ++i)
        {
            if (-1 != model->GetIndexOf(_remove_commands[i]))
            {
                model->Remove(_remove_commands[i]);
            }
        }

        bool bIsExist = false;
        int nNaturalCount = model->GetCount();
        for (int i = 0; i < nNaturalCount; ++i)
        {
             CefMenuModel::MenuItemType type = model->GetTypeAt(i);
             if (type != MENUITEMTYPE_SEPARATOR)
             {
                 bIsExist = true;
                 break;
             }
        }

        if (bIsExist)
        {
            // удаляем все первые и последние сепараторы
            int nNaturalCount2 = model->GetCount();
            for (int i = 0; i < nNaturalCount2; ++i)
            {
                 CefMenuModel::MenuItemType type = model->GetTypeAt(i);
                 if (type != MENUITEMTYPE_SEPARATOR)
                 {
                     break;
                 }
                 model->RemoveAt(i);
                 --nNaturalCount2;
            }

            for (int i = nNaturalCount2 - 1; i >= 0; --i)
            {
                 CefMenuModel::MenuItemType type = model->GetTypeAt(i);
                 if (type != MENUITEMTYPE_SEPARATOR)
                 {
                     break;
                 }
                 model->RemoveAt(i);
                 --nNaturalCount2;
            }
        }

        if (!bIsExist)
            model->Clear();

        if (m_pParent && m_pParent->GetAppManager()->GetDebugInfoSupport())
        {
            if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0)
            {
                // Add a separator if the menu already has items.
                if (model->GetCount() > 0)
                    model->AddSeparator();

                // Add DevTools items to all context menus.
                model->AddItem(CLIENT_ID_SHOW_DEVTOOLS, "&Show DevTools");
                model->AddItem(CLIENT_ID_CLOSE_DEVTOOLS, "Close DevTools");
                model->AddSeparator();
                model->AddItem(CLIENT_ID_INSPECT_ELEMENT, "Inspect Element");
            }
        }
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
        if (event.type == KEYEVENT_RAWKEYDOWN && m_pParent && m_pParent->GetAppManager() && m_pParent->GetAppManager()->GetEventListener())
        {
            NSEditorApi::CAscKeyboardDown* pData = new NSEditorApi::CAscKeyboardDown();
            pData->put_KeyCode(event.windows_key_code);
            int nMods = event.modifiers;

            if ((nMods & EVENTFLAG_CONTROL_DOWN) != 0)
                pData->put_IsCtrl(true);
            if ((nMods & EVENTFLAG_SHIFT_DOWN) != 0)
                pData->put_IsShift(true);
            if ((nMods & EVENTFLAG_ALT_DOWN) != 0)
                pData->put_IsAlt(true);

            NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent(ASC_MENU_EVENT_TYPE_CEF_ONKEYBOARDDOWN);
            pEvent->m_pData = pData;
            m_pParent->GetAppManager()->GetEventListener()->OnEvent(pEvent);
        }

        return false;
    }

    virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request) OVERRIDE
    {
        std::wstring url = request->GetURL().ToWString();
        if (url.find(L"require.js") != std::wstring::npos)
        {
            const std::wstring& sAppDataPath = m_pParent->GetAppManager()->m_oSettings.app_data_path;
            std::string sAppData = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sAppDataPath.c_str(),
                                                                                     (LONG)sAppDataPath.length());

            const std::wstring& sFontsDataPath = m_pParent->GetAppManager()->m_oSettings.fonts_cache_info_path;
            std::string sFontsData = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sFontsDataPath.c_str(),
                                                                                     (LONG)sFontsDataPath.length());

            sAppData = "window[\"AscDesktopEditor_AppData\"] = function(){return \"" + sAppData + "\";}\n";
            sAppData += ("window[\"AscDesktopEditor_FontsData\"] = function(){return \"" + sFontsData + "\";}\n");
            return GetLocalFileRequest(sAppDataPath + L"/webdata/cloud/require.js", sAppData);
        }

        if (true)
        {
            int nPos = url.find(L"apps/documenteditor/main/");

            if (nPos != std::wstring::npos)
            {
                const std::wstring& sAppDataPath = m_pParent->GetAppManager()->m_oSettings.app_data_path;

                std::wstring urlFind = url.substr(nPos);
                urlFind = sAppDataPath + L"/webdata/cloud/" + urlFind;

                return GetLocalFileRequest(urlFind);
            }
        }
        if (true)
        {
            int nPos = url.find(L"apps/presentationeditor/main/");

            if (nPos != std::wstring::npos)
            {
                const std::wstring& sAppDataPath = m_pParent->GetAppManager()->m_oSettings.app_data_path;

                std::wstring urlFind = url.substr(nPos);
                urlFind = sAppDataPath + L"/webdata/cloud/" + urlFind;

                return GetLocalFileRequest(urlFind);
            }
        }
        if (true)
        {
            int nPos = url.find(L"apps/spreadsheeteditor/main/");

            if (nPos != std::wstring::npos)
            {
                const std::wstring& sAppDataPath = m_pParent->GetAppManager()->m_oSettings.app_data_path;

                std::wstring urlFind = url.substr(nPos);
                urlFind = sAppDataPath + L"/webdata/cloud/" + urlFind;

                return GetLocalFileRequest(urlFind);
            }
        }

#if 0

        // TEST
        std::wstring sPathVersion = m_pParent->GetAppManager()->m_oSettings.app_data_path + L"/webdata/cloud/3.0b.1439/word/";
        if (true)
        {
            int nPos = url.find(L"sdk-all.js");

            if (nPos != std::wstring::npos)
            {
                std::wstring urlFind = sPathVersion + L"sdk-all.js";
                return GetLocalFileRequest(urlFind);
            }
        }
        if (true)
        {
            int nPos = url.find(L"app.js");

            if (nPos != std::wstring::npos)
            {
                std::wstring urlFind = sPathVersion + L"app.js";
                return GetLocalFileRequest(urlFind);
            }
        }

#endif

        if (false)
        {
            int nPos = url.find(L"OfficeWeb/sdk/Fonts/");

            if (nPos != std::wstring::npos)
            {
                std::wstring urlFind = url.substr(nPos + 20); // 20 = len(OfficeWeb/sdk/Fonts/)
                //CefRequest::HeaderMap _map;
                //request->GetHeaderMap(_map);
                //_map.insert(std::pair<CefString, CefString>("Access-Control-Allow-Origin", "*"));
                //_map.insert(std::pair<CefString, CefString>("Access-Control-Allow-Methods", "POST, GET, OPTIONS, HEAD"));
                //request->SetHeaderMap(_map);
                return GetLocalFileRequest(urlFind);
            }
        }

        return NULL;
    }

    CefRefPtr<CefResourceHandler> GetLocalFileRequest(const std::wstring& strFileName, const std::string& sHeaderScript = "")
    {
        NSFile::CFileBinary oFileBinary;
        if (!oFileBinary.OpenFile(strFileName))
            return NULL;

        DWORD dwSize = (DWORD)oFileBinary.GetFileSize();
        DWORD dwOffset = 0;

        if (!sHeaderScript.empty())
        {
            dwOffset = (DWORD)sHeaderScript.length();
        }

        LPBYTE pBytes = new BYTE[dwOffset + dwSize];
        if (dwOffset != 0)
        {
            memcpy(pBytes, sHeaderScript.c_str(), dwOffset);
        }

        DWORD dwSize2 = 0;
        oFileBinary.ReadFile(pBytes + dwOffset, dwSize, dwSize2);
        oFileBinary.CloseFile();

        if (dwSize2 != dwSize)
        {
            RELEASEARRAYOBJECTS(pBytes);
            return NULL;
        }

        std::string mime_type = GetMimeType(strFileName);
        if (mime_type.empty())
        {
            RELEASEARRAYOBJECTS(pBytes);
            return NULL;
        }

        CCefBinaryFileReaderCounter* pCounter = new CCefBinaryFileReaderCounter(pBytes);
        CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForHandler(new CefByteReadHandler(pBytes, dwOffset + dwSize, pCounter));
        if (stream.get())
        {
#if 0
            FILE* f = fopen("D:/request_test.log", "a+");
            fprintf(f, mime_type.c_str());
            fprintf(f, ": ");
            std::string sFileA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(strFileName.c_str(), (LONG)strFileName.length());
            fprintf(f, sFileA.c_str());
            fprintf(f, "\n");
            fclose(f);
#endif

            return new CefStreamResourceHandler(mime_type, stream);
        }

        pCounter->data = NULL;
        RELEASEARRAYOBJECTS(pBytes);
        return NULL;
    }

    std::string GetMimeType(const std::wstring& sFile)
    {
        int nPos = sFile.find_last_of(wchar_t('.'));
        if (nPos == std::wstring::npos)
            return "";

        std::wstring sExt = sFile.substr(nPos + 1);
        std::transform(sExt.begin(), sExt.end(), sExt.begin(), ::tolower);
        if (sExt == L"js")
            return "application/javascript";
        else if (sExt == L"css")
            return "text/css";
        else if (sExt == L"png")
            return "image/png";
        else if (sExt == L"jpg" || sExt == L"jpeg")
            return "image/jpeg";
        else if (sExt == L"gif")
            return "image/gif";
        else if (sExt == L"tiff")
            return "image/tiff";
        else if (sExt == L"ico")
            return "image/x-icon";
        else if (sExt == L"htm" || sExt == L"html")
            return "text/html";
        else if (sExt == L"json")
            return "application/json";
        //else if (sExt == L"ttf")
        //    return "application/octet-stream";
        //else if (sExt == L"otf")
        //    return "application/octet-stream";

        return "";
    }

    virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                           TerminationStatus status) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

        message_router_->OnRenderProcessTerminated(browser);

        // Load the startup URL if that's not the website that we terminated on.
        CefRefPtr<CefFrame> frame = browser->GetMainFrame();
        std::string url = frame->GetURL();
        std::transform(url.begin(), url.end(), url.begin(), tolower);

        frame->LoadURL("ascdesktop://crash.html");
    }

    virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefDownloadItem> download_item,
        const CefString& suggested_name,
        CefRefPtr<CefBeforeDownloadCallback> callback)
    {
        CEF_REQUIRE_UI_THREAD();

        m_pParent->m_pInternal->m_before_callback = callback;
        callback->AddRef();

        NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent(ASC_MENU_EVENT_TYPE_CEF_SAVEFILEDIALOG);
        NSEditorApi::CAscSaveDialog* pData = new NSEditorApi::CAscSaveDialog();
        pData->put_Id(m_pParent->GetId());
        pData->put_FilePath(suggested_name.ToWString());
        pEvent->m_pData = pData;

        m_pParent->GetAppManager()->Apply(pEvent);

        /*
        std::wstring sPath = m_pParent->GetAppManager()->GetDialogResult();
        if (!sPath.empty())
        {
            CefString cefPath;
            cefPath.FromWString(sPath);
            callback->Continue(cefPath, false);
        }
        */
    }

    void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefDownloadItem> download_item,
                                          CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

        if (NULL == m_pParent)
          return;

        NSEditorApi::CAscDownloadFileInfo* pData = new NSEditorApi::CAscDownloadFileInfo();
        pData->put_Url(download_item->GetURL().ToWString());
        pData->put_FilePath(download_item->GetFullPath().ToWString());
        pData->put_Percent(download_item->GetPercentComplete());
        pData->put_IsComplete(false);
        pData->put_Id(m_pParent->GetId());
        pData->put_Speed(download_item->GetCurrentSpeed() / 1024.0);

        long long l1 = download_item->GetReceivedBytes();
        long long l2 = download_item->GetTotalBytes();

        NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
        pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD;
        pEvent->m_pData = pData;

        if (m_pParent->m_pInternal->m_bIsCancelDownload)
        {
            pData->put_IsCanceled(true);
            pData->put_IsComplete(true);

            if (callback)
                callback->Cancel();
        }

        if (download_item->IsComplete())
        {
            //SetLastDownloadFile(download_item->GetFullPath());
            //SendNotification(NOTIFY_DOWNLOAD_COMPLETE);

            if (NULL != m_pParent)
            {
                pData->put_IsComplete(true);
            }
        }

        m_pParent->GetAppManager()->Apply(pEvent);
    }

    bool OnDragEnter(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefDragData> dragData,
                     CefDragHandler::DragOperationsMask mask) OVERRIDE
    {
        CEF_REQUIRE_UI_THREAD();

#if 0
        bool bIsFile = dragData->IsFile();
        std::wstring strFileName = dragData->GetFileName();
        std::wstring strFile2 = dragData->GetLinkURL();
#endif

        // разруливаем на стороне js
        return false;
    }

    virtual void OnFoundCookie(bool bIsPresent, std::string sValue);
    virtual void OnSetCookie();
    virtual void OnDeleteCookie(bool bIsPresent);

    // ClientHandlerDelegate
    virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE {}

    // Called when the browser is closing.
    virtual void OnBrowserClosing(CefRefPtr<CefBrowser> browser) OVERRIDE {}

    // Called when the browser has been closed.
    virtual void OnBrowserClosed(CefRefPtr<CefBrowser> browser) OVERRIDE {}

    // Set the window URL address.
    virtual void OnSetAddress(const std::string& url) OVERRIDE {}

    // Set the window title.
    virtual void OnSetTitle(const std::string& title) OVERRIDE {}

    // Set fullscreen mode.
    virtual void OnSetFullscreen(bool fullscreen) OVERRIDE {}

    // Set the loading state.
    virtual void OnSetLoadingState(bool isLoading,
                                     bool canGoBack,
                                     bool canGoForward) OVERRIDE {}

    // Set the draggable regions.
    virtual void OnSetDraggableRegions(const std::vector<CefDraggableRegion>& regions) OVERRIDE {}

public:
    IMPLEMENT_REFCOUNTING(CAscClientHandler);
};

void CCefView_Private::CloseBrowser(bool _force_close)
{
    if (!CefCurrentlyOn(TID_UI))
    {
        CefPostTask(TID_UI,
            base::Bind(&CCefView_Private::CloseBrowser, this, _force_close));
        return;
    }

    if (m_handler && m_handler->GetBrowser() && m_handler->GetBrowser()->GetHost())
        m_handler->GetBrowser()->GetHost()->CloseBrowser(_force_close);
}

void CAscClientHandler::OnFoundCookie(bool bIsPresent, std::string sValue)
{
    if (!m_pParent || !m_pParent->m_pInternal || !m_pParent->m_pInternal->m_handler || !m_pParent->m_pInternal->m_handler->GetBrowser())
        return;

    CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("on_is_cookie_present");
    message->GetArgumentList()->SetBool(0, bIsPresent);
    message->GetArgumentList()->SetString(1, sValue);
    m_pParent->m_pInternal->m_handler->GetBrowser()->SendProcessMessage(PID_RENDERER, message);
}
void CAscClientHandler::OnSetCookie()
{
    if (!m_pParent || !m_pParent->m_pInternal || !m_pParent->m_pInternal->m_handler || !m_pParent->m_pInternal->m_handler->GetBrowser())
        return;

    CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("on_set_cookie");
    m_pParent->m_pInternal->m_handler->GetBrowser()->SendProcessMessage(PID_RENDERER, message);
}

void CAscClientHandler::OnDeleteCookie(bool bIsPresent)
{
    // not used
}

CCefView::CCefView(CCefViewWidgetImpl* parent, int nId)
{
    //setAttribute(Qt::WA_NativeWindow);
    //setAttribute(Qt::WA_DontCreateNativeAncestors);
    //RegisterDragDrop((HWND)this->winId(), this);

    //setAcceptDrops(true);
    m_pInternal = new CCefView_Private();
    m_pInternal->m_pWidgetImpl = parent;
    m_nId = nId;
    m_eWrapperType = cvwtSimple;
}

CCefView::~CCefView()
{
    RELEASEOBJECT(m_pInternal);
}

void CCefView::load(const std::wstring& url)
{
    if (!m_pInternal || !m_pInternal->m_pManager)
        return;

    m_pInternal->m_strUrl = url;
    m_pInternal->m_oPrintData.m_pApplicationFonts = GetAppManager()->GetApplicationFonts();

    if (m_pInternal->m_handler)
    {
        CefRefPtr<CefFrame> _frame;
        if (m_pInternal->m_handler->GetBrowser())
            _frame = m_pInternal->m_handler->GetBrowser()->GetMainFrame();

        if (_frame)
        {
            CefString sUrl;
            sUrl.FromWString(url);
            _frame->LoadURL(sUrl);
        }
        return;
    }

    CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
    manager->SetStoragePath(m_pInternal->m_pManager->m_oSettings.cookie_path, true, NULL);

    //m_pInternal->SetCookie(manager);

#if 0
    CCefCookieVisitor* pVisitor = new CCefCookieVisitor();
    pVisitor->m_sDomain         = "";
    pVisitor->m_sCookieSearch   = "";
    pVisitor->CheckCookiePresent(CefCookieManager::GetGlobalManager());
#endif

    // Create the single static handler class instance
    CAscClientHandler* pClientHandler = new CAscClientHandler();
    pClientHandler->m_pParent = this;
    m_pInternal->m_handler = pClientHandler;

    ClientWindowHandle hWnd = (ClientWindowHandle)m_pInternal->m_pWidgetImpl->parent_wid();
    //m_pInternal->m_handler->SetMainWindowHandle(hWnd);

    CefWindowInfo info;
    CefBrowserSettings _settings;
    _settings.file_access_from_file_urls = STATE_ENABLED;
    _settings.universal_access_from_file_urls = STATE_ENABLED;
    _settings.javascript_access_clipboard = STATE_ENABLED;

    _settings.plugins = STATE_DISABLED;
    _settings.java = STATE_DISABLED;

    if (m_eWrapperType == cvwtEditor)
    {
        _settings.background_color = 0xFFF4F4F4;
    }
    else
    {
        _settings.background_color = 0xFFFFFFFF;
    }

    // Initialize window info to the defaults for a child window.

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = m_pInternal->m_pWidgetImpl->parent_width() - 1;
    rect.bottom = m_pInternal->m_pWidgetImpl->parent_height() - 1;
    info.SetAsChild(hWnd, rect);

    CefString sUrl = url;

#if 0
    CefString sUrl;
    sUrl.FromString(NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(url.c_str(), url.length()));

    std::wstring ss = sUrl.ToWString();

    f = fopen("d:\\1111111.txt", "a+");
    std::string sss = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(ss.c_str(), ss.length());
    fprintf(f, sss.c_str());
    fprintf(f, "\n");
    fclose(f);

    f = fopen("d:\\1111111.txt", "a+");
    fprintf(f, "2\n");
    fclose(f);
#endif

    // Creat the new child browser window
    CefBrowserHost::CreateBrowser(info, m_pInternal->m_handler.get(), sUrl, _settings, NULL);

    focus();
}

std::wstring CCefView::GetUrl()
{
    if (!m_pInternal)
        return L"";

    return m_pInternal->m_strUrl;
}

void CCefView::focus(bool value)
{
    if (!m_pInternal || !m_pInternal->m_handler.get())
        return;

    CefRefPtr<CefBrowser> browser = m_pInternal->m_handler->GetBrowser();
    if (browser)
    {
        // Give focus to the browser.
        browser->GetHost()->SetFocus(value);

        if (!m_pInternal->m_bIsMouseHook)
        {
            if (this->GetType() == cvwtSimple)
            {
#ifdef WIN32
                DWORD threadId = GetWindowThreadProcessId(browser->GetHost()->GetWindowHandle(), NULL);
                HHOOK hook = SetWindowsHookEx(WH_MOUSE, MyMouseHook, NULL, threadId);
#endif
            }

            m_pInternal->m_bIsMouseHook = true;
        }
    }
}

bool CCefView::nativeEvent(const char* data, const int& datalen, void *message, long *result)
{
#ifdef WIN32
    *result = 0;
    MSG *msg = static_cast<MSG *>(message);

    if (msg->message == WM_ERASEBKGND)
    {
        if (m_pInternal->m_handler && m_pInternal->m_handler->GetBrowser() && m_pInternal->m_handler->GetBrowser()->GetHost())
        {
            CefWindowHandle hwnd = m_pInternal->m_handler->GetBrowser()->GetHost()->GetWindowHandle();
            if (hwnd)
            {
                // Dont erase the background if the browser window has been loaded
                // (this avoids flashing)
                return true;
            }
        }
    }
    else if (msg->message == WM_MOVE || msg->message == WM_MOVING)
    {
        if (m_pInternal->m_handler && m_pInternal->m_handler->GetBrowser() && m_pInternal->m_handler->GetBrowser()->GetHost())
        {
            m_pInternal->m_handler->GetBrowser()->GetHost()->NotifyMoveOrResizeStarted();
        }
        return true;
    }
    else if (msg->message == (WM_USER + 1))
    {
        if (this->GetType() == cvwtSimple)
        {
            m_pInternal->m_dwTimeMouseWheelUp = NSTimers::GetTickCount();
        }
        return true;
    }
#endif

    return false;
}

void CCefView::resizeEvent(int width, int height)
{
    if (!m_pInternal->m_handler || !m_pInternal->m_handler->GetBrowser() || !m_pInternal->m_handler->GetBrowser()->GetHost())
        return;

    CefWindowHandle hwnd = m_pInternal->m_handler->GetBrowser()->GetHost()->GetWindowHandle();

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = (0 == width) ? (m_pInternal->m_pWidgetImpl->parent_width() - 1) : width;
    rect.bottom = (0 == height) ? (m_pInternal->m_pWidgetImpl->parent_height() - 1) : height;

#if 0
    SetWindowPos(hwnd, NULL, rect.left, rect.top, rect.right - rect.left + 1,
                 rect.bottom - rect.top + 1, SWP_NOZORDER);
#else
    HDWP hdwp = BeginDeferWindowPos(1);
    hdwp = DeferWindowPos(hdwp, hwnd, NULL,
        rect.left, rect.top, rect.right - rect.left + 1,
        rect.bottom - rect.top + 1, SWP_NOZORDER);
    EndDeferWindowPos(hdwp);
#endif

    focus();
}

void CCefView::moveEvent()
{
#if defined(_LINUX) && !defined(_MAC)
    if (m_pInternal && m_pInternal->m_handler && m_pInternal->m_handler->GetBrowser() && m_pInternal->m_handler->GetBrowser()->GetHost())
    {
        m_pInternal->m_handler->GetBrowser()->GetHost()->NotifyMoveOrResizeStarted();
    }
#endif
}

void CCefView::Apply(NSEditorApi::CAscMenuEvent* pEvent)
{
    if (NULL == pEvent)
        return;

    if (!m_pInternal)
        return;

    CefRefPtr<CefBrowser> browser;
    if (m_pInternal->m_handler.get())
    {
        browser = m_pInternal->m_handler->GetBrowser();
    }
    if (!browser)
    {
        RELEASEINTERFACE(pEvent);
        return;
    }

    switch (pEvent->m_nType)
    {
        case ASC_MENU_EVENT_TYPE_CEF_KEYBOARDLAYOUT:
        {
            NSEditorApi::CAscKeyboardLayout* pData = (NSEditorApi::CAscKeyboardLayout*)pEvent->m_pData;

            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("keyboard_layout");
            message->GetArgumentList()->SetInt(0, pData->get_Language());
            browser->SendProcessMessage(PID_RENDERER, message);

            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_SPELLCHECK:
        {
            NSEditorApi::CAscSpellCheckType* pData = (NSEditorApi::CAscSpellCheckType*)pEvent->m_pData;

            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("spell_check_response");
            message->GetArgumentList()->SetString(0, pData->get_Result());
            message->GetArgumentList()->SetInt(1, pData->get_FrameId());
            browser->SendProcessMessage(PID_RENDERER, message);
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_CONTROL_ID:
        {
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("cef_control_id");
            message->GetArgumentList()->SetInt(0, m_nId);
            browser->SendProcessMessage(PID_RENDERER, message);
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_SYNC_COMMAND:
        {
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("sync_command_end");
            browser->SendProcessMessage(PID_RENDERER, message);
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_CLOSE:
        {
            if (!this->GetModified())
            {
                // здесь потом делать запрос на то, сохранен ли файл. пока просто считаем что все сохранено
                NSEditorApi::CAscMenuEvent* pData = new NSEditorApi::CAscMenuEvent();
                pData->m_nType = ASC_MENU_EVENT_TYPE_CEF_ONCLOSE;

                NSEditorApi::CAscTypeId* pDataId = new NSEditorApi::CAscTypeId();
                pDataId->put_Id(m_nId);
                pData->m_pData = pDataId;

                if (NULL != this->GetAppManager()->GetEventListener())
                    this->GetAppManager()->GetEventListener()->OnEvent(pData);
            }
            else
            {
                // здесь потом делать запрос на то, сохранен ли файл. пока просто считаем что все сохранено
                NSEditorApi::CAscMenuEvent* pData = new NSEditorApi::CAscMenuEvent();
                pData->m_nType = ASC_MENU_EVENT_TYPE_CEF_ONBEFORECLOSE;

                NSEditorApi::CAscTypeId* pDataId = new NSEditorApi::CAscTypeId();
                pDataId->put_Id(m_nId);
                pData->m_pData = pDataId;

                if (NULL != this->GetAppManager()->GetEventListener())
                    this->GetAppManager()->GetEventListener()->OnEvent(pData);
            }

            RELEASEINTERFACE(pEvent);
            return;

            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_DESTROY:
        {
            if (m_pInternal && m_pInternal->m_handler && m_pInternal->m_handler->GetBrowser())
            {
                m_pInternal->CloseBrowser(true);
            }
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_SAVE:
        {
            m_pInternal->m_bIsNativeSave = true;

            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("document_save");
            browser->SendProcessMessage(PID_RENDERER, message);
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_PRINT_START:
        {
            if (m_pInternal)
            {
                if (this->GetType() == cvwtEditor)
                {
                    m_pInternal->m_oPrintData.m_eEditorType = ((CCefViewEditor*)this)->GetEditorType();
                }

                m_pInternal->m_oPrintData.Print_Start();
            }

            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("print");
            browser->SendProcessMessage(PID_RENDERER, message);

            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_PRINT_END:
        {
            if (m_pInternal)
            {
                m_pInternal->m_oPrintData.Print_End();
            }
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_PRINT_PAGE:
        {
            if (m_pInternal)
            {
                NSEditorApi::CAscPrintPage* pData = (NSEditorApi::CAscPrintPage*)pEvent->m_pData;
                m_pInternal->m_oPrintData.Print(pData->get_Context(), GetAppManager()->m_oPrintSettings, pData->get_Page());
            }
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_SCRIPT_EDITOR_VERSION:
        {
            NSEditorApi::CAscEditorScript* pData = (NSEditorApi::CAscEditorScript*)pEvent->m_pData;

            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("on_load_js");
            message->GetArgumentList()->SetString(0, pData->get_Destination());
            message->GetArgumentList()->SetString(1, pData->get_Url());
            message->GetArgumentList()->SetInt(2, pData->get_FrameId());
            browser->SendProcessMessage(PID_RENDERER, message);
            break;
        }
        case ASC_MENU_EVENT_TYPE_CEF_SAVEFILEDIALOG:
        {
            NSEditorApi::CAscSaveDialog* pData = (NSEditorApi::CAscSaveDialog*)pEvent->m_pData;

            std::wstring strPath = pData->get_FilePath();
            if (strPath.empty())
            {
                // нужно послать отмену.
                NSEditorApi::CAscDownloadFileInfo* pData = new NSEditorApi::CAscDownloadFileInfo();
                pData->put_Url(L"");
                pData->put_FilePath(L"");
                pData->put_Percent(0);
                pData->put_IsComplete(false);
                pData->put_Id(this->GetId());
                pData->put_Speed(0);
                pData->put_IsCanceled(true);

                NSEditorApi::CAscMenuEvent* pEvent = new NSEditorApi::CAscMenuEvent();
                pEvent->m_nType = ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD;
                pEvent->m_pData = pData;

                if (GetAppManager()->GetEventListener())
                    GetAppManager()->GetEventListener()->OnEvent(pEvent);
            }
            else
            {
                CefString sPath;
                sPath.FromWString(pData->get_FilePath());
                m_pInternal->m_before_callback->Continue(sPath, false);
            }

            m_pInternal->m_before_callback->Release();
            m_pInternal->m_before_callback = NULL;

            break;
        }
        default:
            break;
    }

    RELEASEINTERFACE(pEvent);
}

NSEditorApi::CAscMenuEvent* CCefView::ApplySync(NSEditorApi::CAscMenuEvent* pEvent)
{
    return NULL;
}

#if 0
void CCefView::dragEnterEvent(QDragEnterEvent *)
{
    OutputDebugStringA( "dragEnterEvent" );
}

void CCefView::dragMoveEvent(QDragMoveEvent *)
{
    OutputDebugStringA( "dragMoveEvent" );
}

void CCefView::dragLeaveEvent(QDragLeaveEvent *)
{
    OutputDebugStringA( "dragLeaveEvent" );
}

void CCefView::dropEvent(QDropEvent *)
{
    OutputDebugStringA( "dropEvent" );
}
#endif

CAscApplicationManager* CCefView::GetAppManager()
{
    return m_pInternal->m_pManager;
}

void CCefView::SetAppManager(CAscApplicationManager* pManager)
{
    if (!m_pInternal)
        return;
    m_pInternal->m_pManager = pManager;
}

CCefViewWidgetImpl* CCefView::GetWidgetImpl()
{
    return m_pInternal->m_pWidgetImpl;
}

int CCefView::GetId()
{
    return m_nId;
}

CefViewWrapperType CCefView::GetType()
{
    return m_eWrapperType;
}

void CCefView::SetParentCef(int nId)
{
    m_pInternal->m_nParentId = nId;
}
int CCefView::GetParentCef()
{
    return m_pInternal->m_nParentId;
}

void CCefView::SetModified(bool bIsModified)
{
    m_pInternal->m_bIsModified = bIsModified;
}

bool CCefView::GetModified()
{
    return m_pInternal->m_bIsModified;
}

/////////////////////////////////////////////////////////////
CCefViewEditor::CCefViewEditor(CCefViewWidgetImpl* parent, int nId) : CCefView(parent, nId)
{
    m_eType = etUndefined;
    m_eWrapperType = cvwtEditor;
}

CCefViewEditor::~CCefViewEditor()
{
}

void CCefViewEditor::SetEditorType(AscEditorType eType)
{
    m_eType = eType;
}

AscEditorType CCefViewEditor::GetEditorType()
{
    return m_eType;
}

////////////////////////////////////////////////////////////
void CAscApplicationManager::CancelDownload(int nId)
{
    CCefView* pView = this->GetViewById(nId);
    if (NULL != pView)
        pView->m_pInternal->m_bIsCancelDownload = true;
}

////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
namespace NSCommonReader
{
    inline int32_t ReadInt(BYTE*& pData, int& nOffset)
    {
#ifdef _ARM_ALIGN_
        int32_t ret = 0;
        memcpy(&ret, pData, sizeof(int32_t));
        pData   += 4;
        nOffset += 4;
        return ret;
#else
        int32_t ret = *((int32_t*)pData);
        pData   += 4;
        nOffset += 4;
        return ret;
#endif
    }

    inline USHORT ReadUSHORT(BYTE*& pData, int& nOffset)
    {
#ifdef _ARM_ALIGN_
        USHORT ret = 0;
        memcpy(&ret, pData, sizeof(USHORT));
        pData   += 2;
        nOffset += 2;
        return ret;
#else
        USHORT ret = *((USHORT*)pData);
        pData   += 2;
        nOffset += 2;
        return ret;
#endif
    }

    inline std::wstring ReadString16(BYTE*& pData, int& nOffset, int nLen)
    {
        std::wstring wsTempString;
#ifdef _ARM_ALIGN_

        if (sizeof(wchar_t) == 4)
        {
#if !defined(_WIN32) && !defined(_WIN64)
            int len = nLen / 2;
            UTF16* buf = new UTF16[len];
            memcpy(buf, pData, nLen);
            wsTempString = stringUtf16ToWString(buf, len);
            RELEASEARRAYOBJECTS(buf);
#endif
        }
        else
        {
            int len = nLen / 2;
            wchar_t* buf = new wchar_t[len];
            memcpy(buf, pData, nLen);
            wsTempString = std::wstring(buf, len);
            RELEASEARRAYOBJECTS(buf);
        }

#else

        if (sizeof(wchar_t) == 4)
        {
#if !defined(_WIN32) && !defined(_WIN64)
            NSFile::CStringUtf16 oData;
            oData = pData;
            oData.Length = nLen;

            wsTempString = NSFile::CUtf8Converter::GetWStringFromUTF16(oData);
            oData.Data = NULL;
#endif
        }
        else
        {
            wsTempString = std::wstring((wchar_t*)pData, nLen/2);
        }

#endif
        pData += nLen;
        nOffset += nLen;
        return wsTempString;
    }
}

void CPrintData::DrawOnRenderer(CGraphicsRenderer* pRenderer, int nPageIndex)
{
    CommandType eCommand = ctError;

    bool bIsPathOpened = false;
    int curindex = 0;

    int32_t* m = NULL;
    USHORT* ms = NULL;
    int _sLen = 0;
    int32_t gradientType = 0;

    double m1 = 0;
    double m2 = 0;
    double m3 = 0;
    double m4 = 0;
    double m5 = 0;
    double m6 = 0;

    BYTE* dstArray = NULL;
    int len = 0;
    NSFile::CBase64Converter::Decode(m_arPages[nPageIndex].Base64.c_str(), m_arPages[nPageIndex].Base64.length(), dstArray, len);

    BYTE* p = dstArray;
    {
        BYTE* current = p;
        while (curindex < len)
        {
            eCommand = (CommandType)(*current);
            current++;
            curindex++;
            switch (eCommand)
            {
                case ctPageWidth:                    
                    pRenderer->put_Width(NSCommonReader::ReadInt(current, curindex) / 100000.0);
                    break;
                case ctPageHeight:                    
                    pRenderer->put_Height(NSCommonReader::ReadInt(current, curindex) / 100000.0);
                    break;
                case ctPageStart:
                    pRenderer->BeginCommand(1);
                    break;
                case ctPageEnd:
                    if (bIsPathOpened)
                    {
                        pRenderer->PathCommandEnd();
                        pRenderer->EndCommand(4);
                    }
                    bIsPathOpened = false;

                    pRenderer->EndCommand(1);
                    break;
                case ctPenColor:
                    pRenderer->put_PenColor(NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctPenAlpha:
                    pRenderer->put_PenAlpha(*current);
                    current++;
                    curindex++;
                    break;
                case ctPenSize:
                    pRenderer->put_PenSize(NSCommonReader::ReadInt(current, curindex) / 100000.0);
                    break;
                case ctPenLineJoin:
                    pRenderer->put_PenLineJoin(*current);
                    current++;
                    curindex++;
                    break;
                case ctBrushType:
                    pRenderer->put_BrushType(NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctBrushColor1:
                    pRenderer->put_BrushColor1(NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctBrushAlpha1:
                    pRenderer->put_BrushAlpha1(*current);
                    current++;
                    curindex++;
                    break;
                case ctBrushColor2:
                    pRenderer->put_BrushColor1(NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctBrushAlpha2:
                    pRenderer->put_BrushAlpha2(*current);
                    current++;
                    curindex++;
                    break;
                case ctBrushRectable:
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;

#if 0
                    pRenderer->BrushRect(0, m1, m2, m3, m4);
#endif
                    break;
                case ctBrushRectableEnabled:
                {
#if 0
                    VARIANT bEn;
                    bEn.vt = VT_BOOL;
                    bEn.boolVal = (1 == *current) ? VARIANT_TRUE : VARIANT_FALSE;

                    SetAdditionalParam(L"BrushFillBoundsEnable", bEn);
#endif

                    current += 1;
                    curindex += 1;
                    break;
                }
                case ctBrushTexturePath:
                {
                    _sLen = 2 * NSCommonReader::ReadUSHORT(current, curindex);

                    std::wstring wsTempStringSrc = NSCommonReader::ReadString16(current, curindex, _sLen);
                    std::wstring wsTempString = GetImagePath(wsTempStringSrc);

                    pRenderer->put_BrushTexturePath(wsTempString);
                    break;
                }
                case ctBrushGradient:
                {
                    current++;
                    curindex++;

                    while (true)
                    {
                        BYTE _command = *current;
                        current++;
                        curindex++;

                        if (251 == _command)
                            break;

                        switch (_command)
                        {
                            case 0:
                            {
                                current += 5;
                                curindex += 5;

                                double x0 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double y0 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double x1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double y1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;

                                double dAngle = 0;
                                if (fabs(x1 - x0) >= FLT_EPSILON || fabs(y1 - y0) >= FLT_EPSILON)
                                    dAngle = 180 * atan2(y1 - y0, x1 - x0) / agg::pi;

                                pRenderer->put_BrushType(c_BrushTypePathGradient1);
                                pRenderer->put_BrushLinearAngle(dAngle);

                                break;
                            }
                            case 1:
                            {
                                current++;
                                curindex++;

                                double d1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d5 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                                double d6 = NSCommonReader::ReadInt(current, curindex) / 100000.0;

                                // TODO: realize
                                pRenderer->put_BrushType(c_BrushTypePathGradient2);
                                break;
                            }
                            case 2:
                            {
                                int nCountColors = NSCommonReader::ReadInt(current, curindex);

                                if (nCountColors <= 0)
                                {
                                    pRenderer->put_BrushGradientColors(NULL, NULL, 0);
                                }
                                else
                                {
                                    LONG* pColors		= new LONG[nCountColors];
                                    double* pPositions	= new double[nCountColors];
                                    for (LONG i = 0; i < nCountColors; ++i)
                                    {
                                        pPositions[i] = NSCommonReader::ReadInt(current, curindex) / 100000.0;

                                        BYTE _b = *current++;
                                        BYTE _g = *current++;
                                        BYTE _r = *current++;
                                        BYTE _a = *current++;

                                        curindex += 4;

                                        pColors[i] = ((_b << 24) & 0xFF000000) | ((_g << 16) & 0xFF0000) | ((_r << 8) & 0xFF00) | _a;
                                    }

                                    pRenderer->put_BrushGradientColors(pColors, pPositions, nCountColors);

                                    RELEASEARRAYOBJECTS(pColors);
                                    RELEASEARRAYOBJECTS(pPositions);
                                }
                                break;
                            }
                            default:
                                break;
                        };
                    }

                    break;
                }
                case ctBrushTextureMode:
                {
                    LONG mode = (LONG)(*current);
                    pRenderer->put_BrushTextureMode(mode);

                    current += 1;
                    curindex += 1;
                    break;
                }
                case ctBrushTextureAlpha:
                {
                    LONG txalpha = (LONG)(*current);
                    pRenderer->put_BrushTextureAlpha(txalpha);

                    current += 1;
                    curindex += 1;

                    break;
                }                    
                case ctSetTransform:
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m5 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m6 = NSCommonReader::ReadInt(current, curindex) / 100000.0;

                    pRenderer->SetTransform(m1, m2, m3, m4, m5, m6);
                    break;
                case ctPathCommandStart:
                    if (bIsPathOpened)
                    {
                        pRenderer->PathCommandEnd();
                        pRenderer->EndCommand(4);
                        pRenderer->BeginCommand(4);
                        pRenderer->PathCommandStart();
                    }                    
                    else
                    {
                        pRenderer->BeginCommand(4);
                        pRenderer->PathCommandStart();
                    }
                    bIsPathOpened = true;
                    break;
                case ctPathCommandMoveTo:
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    pRenderer->PathCommandMoveTo(m1, m2);
                    break;
                case ctPathCommandLineTo:
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    pRenderer->PathCommandLineTo(m1, m2);
                    break;
                case ctPathCommandCurveTo:
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m5 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m6 = NSCommonReader::ReadInt(current, curindex) / 100000.0;

                    pRenderer->PathCommandCurveTo(m1, m2, m3, m4, m5, m6);
                    break;
                case ctPathCommandClose:
                    pRenderer->PathCommandClose();
                    break;
                case ctPathCommandEnd:
                    if (bIsPathOpened)
                    {
                        pRenderer->PathCommandEnd();
                        pRenderer->EndCommand(4);
                        bIsPathOpened = false;
                    }
                    break;
                case ctDrawPath:
                    pRenderer->DrawPath(NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctDrawImageFromFile:
                {
                    _sLen = NSCommonReader::ReadInt(current, curindex);
                    std::wstring wsTempStringSrc = NSCommonReader::ReadString16(current, curindex, _sLen);
                    std::wstring wsTempString = GetImagePath(wsTempStringSrc);

                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m3 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m4 = NSCommonReader::ReadInt(current, curindex) / 100000.0;

                    try
                    {
                        pRenderer->DrawImageFromFile(wsTempString, m1, m2, m3, m4);
                    }
                    catch (...)
                    {
                    }

                    break;
                }
                case ctFontName:
                {
                    _sLen = 2 * (int)NSCommonReader::ReadUSHORT(current, curindex);
                    std::wstring wsTempString = NSCommonReader::ReadString16(current, curindex, _sLen);

                    pRenderer->put_FontName(wsTempString);
                    break;
                }
                case ctFontSize:
                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    pRenderer->put_FontSize(m1);
                    break;
                case ctFontStyle:
                    pRenderer->put_FontStyle (NSCommonReader::ReadInt(current, curindex));
                    break;
                case ctDrawText:
                {
                    _sLen = 2 * (int)NSCommonReader::ReadUSHORT(current, curindex);

                    std::wstring wsTempString = NSCommonReader::ReadString16(current, curindex, _sLen);

                    m1 = NSCommonReader::ReadInt(current, curindex) / 100000.0;
                    m2 = NSCommonReader::ReadInt(current, curindex) / 100000.0;

                    pRenderer->CommandDrawText(wsTempString, m1, m2, 0, 0);
                    break;
                }
                case ctBeginCommand:
                    if (bIsPathOpened)
                    {
                        pRenderer->PathCommandEnd();
                        pRenderer->EndCommand(4);
                        bIsPathOpened = false;
                    }
                    pRenderer->BeginCommand((DWORD)(NSCommonReader::ReadInt(current, curindex)));
                    break;
                case ctEndCommand:
                    
                    pRenderer->EndCommand((DWORD)(NSCommonReader::ReadInt(current, curindex)));
					
                    if (bIsPathOpened)
                    {
                        pRenderer->PathCommandEnd();
                        pRenderer->EndCommand(4);
                        bIsPathOpened = false;
                    }

                    break;
                default:
                    break;
            }; // switch (eCommand)
        } // while (curindex < len)
    }

    RELEASEARRAYOBJECTS(dstArray);
}

std::wstring CPrintData::GetImagePath(const std::wstring& sPath)
{
    // 1) смотрим в мапе
    std::map<std::wstring, std::wstring>::iterator iFind = m_mapImages.find(sPath);
    if (iFind != m_mapImages.end())
        return iFind->second;

    // 2) в мапе нет. смотрим - может путь правильный совсем
    if (NSFile::CFileBinary::Exists(sPath))
    {
        m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sPath));
        return sPath;
    }

    // 3) смотрим, может это прямая ссылка
    if (CFileDownloader::IsNeedDownload(sPath))
    {
        std::wstring sFileDownload = this->DownloadImage(sPath);
        m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sFileDownload));
        return sFileDownload;
    }

    // 4) может это файл файла?
    if (0 == sPath.find(L"media/image"))
    {
        std::wstring sExt = L"";
        int nPos = sPath.find_last_of(wchar_t('.'));
        if (std::wstring::npos != nPos)
        {
            sExt = sPath.substr(nPos + 1);
        }

        std::wstring sUrl = m_sDocumentImagesPath + sPath;
        std::wstring sUrl2 = L"";
        if (sExt == L"svg")
        {
            sUrl = m_sDocumentImagesPath + sPath.substr(0, nPos) + L".emf";
            sUrl2 = m_sDocumentImagesPath + sPath.substr(0, nPos) + L".wmf";
        }

        if (CFileDownloader::IsNeedDownload(m_sDocumentImagesPath))
        {
            std::wstring s1 = this->DownloadImage(sUrl);
            if (!s1.empty())
            {
                m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, s1));
                return s1;
            }
            if (!sUrl2.empty())
            {
                std::wstring s2 = this->DownloadImage(sUrl2);
                if (!s2.empty())
                {
                    m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, s2));
                    return s2;
                }
            }
            m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sPath));
            return sPath;
        }
        else
        {
            if (NSFile::CFileBinary::Exists(sUrl))
            {
                m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sUrl));
                return sUrl;
            }
            else if (NSFile::CFileBinary::Exists(sUrl2))
            {
                m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sUrl2));
                return sUrl2;
            }
            else
            {
                m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sPath));
                return sPath;
            }
        }
    }

    // 5) может это файл темы?
    if (!m_sThemesUrl.empty() && (0 == sPath.find(m_sThemesUrl)))
    {
        std::wstring sExt = L"";
        int nPos = sPath.find_last_of(wchar_t('.'));
        if (std::wstring::npos != nPos)
        {
            sExt = sPath.substr(nPos + 1);
        }

        std::wstring sUrl = m_sPresentationThemesPath + sPath;
        std::wstring sUrl2 = L"";
        if (sExt == L"svg")
        {
            sUrl = m_sPresentationThemesPath + sPath.substr(0, nPos) + L".emf";
            sUrl2 = m_sPresentationThemesPath + sPath.substr(0, nPos) + L".wmf";
        }

        if (CFileDownloader::IsNeedDownload(m_sPresentationThemesPath))
        {
            std::wstring s1 = this->DownloadImage(sUrl);
            if (!s1.empty())
            {
                m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, s1));
                return s1;
            }
            if (!sUrl2.empty())
            {
                std::wstring s2 = this->DownloadImage(sUrl2);
                if (!s2.empty())
                {
                    m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, s2));
                    return s2;
                }
            }
            m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sPath));
            return sPath;
        }
        else
        {
            if (NSFile::CFileBinary::Exists(sUrl))
            {
                m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sUrl));
                return sUrl;
            }
            else if (NSFile::CFileBinary::Exists(sUrl2))
            {
                m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sUrl2));
                return sUrl2;
            }
            else
            {
                m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sPath));
                return sPath;
            }
        }
    }

    // 6) base64?
    if (0 == sPath.find(L"data:"))
    {
        int nPos = sPath.find(wchar_t(','));

        if (nPos != std::wstring::npos)
        {
            int nLenBase64 = sPath.length() - nPos - 1;
            const wchar_t* pSrc = sPath.c_str() + nPos + 1;

            char* pData = new char[nLenBase64];
            for (int i = 0; i < nLenBase64; ++i)
                pData[i] = (char)(pSrc[i]);

            int nLenDecode = 0;
            BYTE* pDstData = NULL;

            bool bRes = NSFile::CBase64Converter::Decode(pData, nLenBase64, pDstData, nLenDecode);

            std::wstring sTmpFile = sPath;
            if (bRes)
            {
                sTmpFile = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPathW(), L"Image64");
                NSFile::CFileBinary oFile;
                if (oFile.CreateFileW(sTmpFile))
                {
                    oFile.WriteFile(pDstData, nLenDecode);
                    oFile.CloseFile();
                }
            }

            RELEASEARRAYOBJECTS(pData);
            RELEASEARRAYOBJECTS(pDstData);

            m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sTmpFile));
            return sTmpFile;
        }
    }

    // ошибка
    m_mapImages.insert(std::pair<std::wstring, std::wstring>(sPath, sPath));
    return sPath;
}

void CPrintData::Print(NSEditorApi::CAscPrinterContextBase* pContext, const CAscPrintSettings& settingsConst, const int& nPageIndex)
{
    if (nPageIndex < 0 || nPageIndex >= m_arPages.GetCount())
        return;

    CAscPrintSettings settings = settingsConst;

    if (m_eEditorType == etPresentation)
    {
        settings.Mode = CAscPrintSettings::pmFit;
        settings.ZoomEnable = true;
    }

    double dLeftPix;
    double dTopPix;
    double dWidthPix;
    double dHeightPix;
    double dAngle;
    double fPrintWidthMM;
    double fPrintHeightMM;

    double fPageWidth = m_arPages[nPageIndex].Width;
    double fPageHeight = m_arPages[nPageIndex].Height;

    double ONE_INCH = 2.54;
    double M_PI_2 = agg::pi / 2;

    int nPrintDpiX;
    int nPrintDpiY;
    int nPrintOffsetX;
    int nPrintOffsetY;
    int nPrintWidthPix; // всей страницы
    int nPrintHeightPix;
    int nPrintPageWidthPix; // только области печати
    int nPrintPageHeightPix;

    pContext->GetLogicalDPI(nPrintDpiX, nPrintDpiY);
    pContext->GetPhysicalRect(nPrintOffsetX, nPrintOffsetY, nPrintWidthPix, nPrintHeightPix);
    pContext->GetPrintAreaSize(nPrintPageWidthPix, nPrintPageHeightPix);

    if( -1 != settings.WidthPix && -1 != settings.HeightPix )
    {
        nPrintWidthPix      = settings.WidthPix;
        nPrintHeightPix     = settings.HeightPix;
        nPrintPageWidthPix  = settings.WidthPix;
        nPrintPageHeightPix = settings.HeightPix;
    }

    if (settings.PrintableArea)
    {
        // печатать нужно только в области печати
        // приравниваем высоту страницы к высоте области печати
        nPrintWidthPix  = nPrintPageWidthPix;
        nPrintHeightPix = nPrintPageHeightPix;
        // обнуляем поправки на непечатаемую область
        nPrintOffsetX = 0;
        nPrintOffsetY = 0;
    }

    // подсчитываем размеры страницы в милиметрах
    fPrintWidthMM   = 10 * ONE_INCH * nPrintWidthPix / nPrintDpiX;
    fPrintHeightMM  = 10 * ONE_INCH * nPrintHeightPix / nPrintDpiX;

    if (CAscPrintSettings::pm100  == settings.Mode)
    {
        dWidthPix   = nPrintDpiX * fPageWidth / ( 10 * ONE_INCH );
        dHeightPix  = nPrintDpiX * fPageHeight / ( 10 * ONE_INCH );
        if (true == settings.RotateEnable && ( nPrintWidthPix < dWidthPix || nPrintHeightPix < dHeightPix))
        {
            if (nPrintWidthPix < dHeightPix || nPrintHeightPix < dWidthPix)
            {
                // выбираем лучший вариант по площади
                double dWidth1  = nPrintWidthPix < dWidthPix ? nPrintWidthPix : dWidthPix;
                double dHeight1 = nPrintHeightPix < dHeightPix ? nPrintHeightPix : dHeightPix;

                double dWidth2  = nPrintWidthPix < dHeightPix ? nPrintWidthPix : dHeightPix;
                double dHeight2 = nPrintHeightPix < dWidthPix ? nPrintHeightPix : dWidthPix;

                if (dWidth1 * dHeight1 >= dWidth2 * dHeight2)
                {
                    dLeftPix = 0;
                    dTopPix = 0;
                }
                else
                {
                    dLeftPix = nPrintWidthPix - ( dHeightPix + dWidthPix ) / 2;
                    dTopPix = dWidthPix / 2 - dHeightPix / 2;
                    dAngle = M_PI_2;    // 90
                }
            }
            else
            {
                //если не вписывается, но вписывается повернутое
                dLeftPix    = nPrintWidthPix - (dHeightPix + dWidthPix ) / 2;
                dTopPix     = nPrintHeightPix / 2 - dHeightPix / 2;
                dAngle      = M_PI_2;   //90
            }
        }
        else
        {
            if (dWidthPix < nPrintWidthPix) //если размеры позволяют, то распологаем по центру
                dLeftPix = nPrintWidthPix / 2 - dWidthPix / 2;
            else
                dLeftPix = 0;
            dTopPix = 0;
        }
    }
    else if (CAscPrintSettings::pmStretch  == settings.Mode)
    {
        if (settings.RotateEnable && (fPageWidth / fPageHeight - 1) * (fPrintWidthMM / fPrintHeightMM - 1) < 0)
        {
            // переворачиваем
            dWidthPix   = nPrintHeightPix;
            dHeightPix  = nPrintWidthPix;
            dLeftPix    = nPrintWidthPix / 2 - dWidthPix / 2;
            dTopPix     = nPrintHeightPix / 2 - dHeightPix / 2;
            dAngle      = M_PI_2;   // 90
        }
        else
        {
            dWidthPix = nPrintWidthPix;
            dHeightPix = nPrintHeightPix;
            dLeftPix = 0;
            dTopPix = 0;
        }
    }
    else
    {
        if (settings.ZoomEnable && settings.RotateEnable)
        {
            bool bRotate = false;
            if ((fPageWidth / fPageHeight - 1) * ( fPrintWidthMM / fPrintHeightMM - 1) < 0)
            {
                // переворачиваем
                double dTemp    = fPrintWidthMM;
                fPrintWidthMM   = fPrintHeightMM;
                fPrintHeightMM  = dTemp;
                dAngle          = M_PI_2;   // 90
                bRotate         = true;
            }
            float fFitX = 0;
            float fFitY = 0;
            float fFitWidth = 0;
            float fFitHeight = 0;
            FitToPage(fPageWidth, fPageHeight, fPrintWidthMM, fPrintHeightMM, fFitX, fFitY, fFitWidth, fFitHeight);

            dWidthPix = nPrintDpiX * fFitWidth / (10 * ONE_INCH);
            dHeightPix = nPrintDpiY * fFitHeight / (10 * ONE_INCH);
            if (true == bRotate)
            {
                dLeftPix    = nPrintWidthPix / 2 - dWidthPix / 2;
                dTopPix     = nPrintHeightPix / 2 - dHeightPix / 2;
            }
            else
            {
                dLeftPix    = nPrintDpiX * fFitX / (10 * ONE_INCH);
                dTopPix     = nPrintDpiY * fFitY / (10 * ONE_INCH);
            }
        }
        else if (settings.ZoomEnable)
        {
            float fFitX = 0;
            float fFitY = 0;
            float fFitWidth = 0;
            float fFitHeight = 0;
            FitToPage(fPageWidth, fPageHeight, fPrintWidthMM, fPrintHeightMM, fFitX, fFitY, fFitWidth, fFitHeight);
            dWidthPix   = nPrintDpiX * fFitWidth / (10 * ONE_INCH);
            dHeightPix  = nPrintDpiY * fFitHeight / (10 * ONE_INCH);
            dLeftPix    = nPrintDpiX * fFitX / (10 * ONE_INCH);
            dTopPix     = nPrintDpiY * fFitY / (10 * ONE_INCH);
            dAngle      = 0;
        }
        else if (settings.RotateEnable)
        {
            // проверяем выходит ли картинка за габариты
            if (fPrintWidthMM < fPageWidth || fPrintHeightMM < fPageHeight)
            {
                // проверяем выходит ли повернутая картинка за габариты
                if (fPrintHeightMM < fPageWidth || fPrintWidthMM < fPageHeight)
                {
                    // выбираем, где больше площадь у повернутого или нет
                    float fFitX1 = 0;
                    float fFitY1 = 0;
                    float fFitWidth1 = 0;
                    float fFitHeight1 = 0;
                    FitToPage( fPageWidth, fPageHeight, fPrintWidthMM, fPrintHeightMM, fFitX1, fFitY1, fFitWidth1, fFitHeight1 );

                    float fFitX2 = 0;
                    float fFitY2 = 0;
                    float fFitWidth2 = 0;
                    float fFitHeight2 = 0;
                    FitToPage( fPageWidth, fPageHeight, fPrintHeightMM, fPrintWidthMM, fFitX2, fFitY2, fFitWidth2, fFitHeight2 );
                    if (fFitWidth1 * fFitHeight1 < fFitWidth2 * fFitHeight2)
                    {
                        // поворачиваем
                        dAngle      = M_PI_2;   // 90
                        dWidthPix   = nPrintDpiX * fFitWidth2 / (10 * ONE_INCH);
                        dHeightPix  = nPrintDpiY * fFitHeight2 / (10 * ONE_INCH);
                        dLeftPix    = nPrintWidthPix / 2 - dWidthPix / 2;
                        dTopPix     = nPrintHeightPix / 2 - dHeightPix / 2;
                    }
                    else
                    {
                        dAngle      = 0;
                        dWidthPix   = nPrintDpiX * fFitWidth1 / (10 * ONE_INCH);
                        dHeightPix  = nPrintDpiY * fFitHeight1 / (10 * ONE_INCH);
                        dLeftPix    = nPrintDpiX * fFitX1 / (10 * ONE_INCH);
                        dTopPix     = nPrintDpiY * fFitY1 / (10 * ONE_INCH);
                    }
                }
                else
                {
                    // поворачиваем
                    dWidthPix   = nPrintDpiX * fPageWidth / (10 * ONE_INCH);
                    dHeightPix  = nPrintDpiY * fPageHeight / (10 * ONE_INCH);
                    dLeftPix    = nPrintWidthPix - (dHeightPix + dWidthPix) / 2;
                    dTopPix     = nPrintHeightPix / 2 - dHeightPix / 2;
                    dAngle      = M_PI_2;   // 90
                }
            }
            else
            {
                dWidthPix = nPrintDpiX * fPageWidth / ( 10 * ONE_INCH );
                dHeightPix = nPrintDpiY * fPageHeight / ( 10 * ONE_INCH );
                dLeftPix = nPrintWidthPix / 2 - dWidthPix / 2; // по центру по горизонтали
                dTopPix = 0; // сверху по вертикали
                dAngle = 0;
            }
        }
        else
        {
            // проверяем выходит ли картинка за габариты
            if (fPrintWidthMM < fPageWidth || fPrintHeightMM < fPageHeight)
            {
                float fFitX = 0;
                float fFitY = 0;
                float fFitWidth = 0;
                float fFitHeight = 0;
                FitToPage(fPageWidth, fPageHeight, fPrintWidthMM, fPrintHeightMM, fFitX, fFitY, fFitWidth, fFitHeight);
                dWidthPix   = nPrintDpiX * fFitWidth / (10 * ONE_INCH);
                dHeightPix  = nPrintDpiY * fFitHeight / (10 * ONE_INCH);
                dLeftPix    = nPrintDpiX * fFitX / (10 * ONE_INCH);
                dTopPix     = nPrintDpiY * fFitY / (10 * ONE_INCH);
            }
            else
            {
                dWidthPix   = nPrintDpiX * fPageWidth / (10 * ONE_INCH);
                dHeightPix  = nPrintDpiY * fPageHeight / (10 * ONE_INCH);
                dLeftPix    = nPrintWidthPix / 2 - dWidthPix / 2; // по центру по горизонтали
                dTopPix     = 0; // сверху по вертикали
            }
        }
    }
    dLeftPix -= nPrintOffsetX;
    dTopPix -= nPrintOffsetY;

    CBgraFrame oFrame;
    int nRasterW = (int)(dWidthPix + 0.5);
    int nRasterH = (int)(dHeightPix + 0.5);
    oFrame.put_Width(nRasterW);
    oFrame.put_Height(nRasterH);
    oFrame.put_Stride(4 * nRasterW);

    BYTE* pDataRaster = new BYTE[4 * nRasterW * nRasterH];
    memset(pDataRaster, 0xFF, 4 * nRasterW * nRasterH);
    oFrame.put_Data(pDataRaster);

    CGraphicsRenderer oRenderer;
    oRenderer.SetFontManager(m_pFontManager);
    oRenderer.SetImageCache(m_pCache);

    oRenderer.CreateFromBgraFrame(&oFrame);
    oRenderer.SetSwapRGB(false);

    oRenderer.SetTileImageDpi(96.0);
    this->DrawOnRenderer(&oRenderer, nPageIndex);

#if 0
    oFrame.SaveFile(L"D:\\ttttt.png", 4);
#endif

    pContext->BitBlt(oFrame.get_Data(), 0, 0, nRasterW, nRasterH,
                     dLeftPix, dTopPix, dWidthPix, dHeightPix, dAngle);
}

void CPrintData::FitToPage(float fSourceWidth, float  fSourceHeight, float  fTargetWidth, float fTargetHeight, float& fResX, float& fResY, float& fResWidth, float& fResHeight)
{
    if (fSourceWidth * fTargetHeight > fTargetWidth * fSourceHeight)
    {
        fResHeight = fTargetWidth * fSourceHeight / fSourceWidth;
        fResWidth = fTargetWidth;

        fResX = 0;
        fResY = fTargetHeight / 2 - fResHeight / 2;
    }
    else
    {
        fResWidth = fTargetHeight * fSourceWidth / fSourceHeight;
        fResHeight = fTargetHeight;
        fResY = 0;
        fResX = fTargetWidth / 2 - fResWidth / 2;
    }
}

void CPrintData::TestSaveToRasterFile(std::wstring sFile, int nWidth, int nHeight, int nPageIndex)
{
    CBgraFrame oFrame;
    int nRasterW = nWidth;
    int nRasterH = nHeight;
    oFrame.put_Width(nRasterW);
    oFrame.put_Height(nRasterH);
    oFrame.put_Stride(-4 * nRasterW);

    BYTE* pDataRaster = new BYTE[4 * nRasterW * nRasterH];
    memset(pDataRaster, 0xFF, 4 * nRasterW * nRasterH);
    oFrame.put_Data(pDataRaster);

    CGraphicsRenderer oRenderer;
    oRenderer.SetFontManager(m_pFontManager);
    oRenderer.SetImageCache(m_pCache);

    oRenderer.CreateFromBgraFrame(&oFrame);
    oRenderer.SetSwapRGB(false);

    this->DrawOnRenderer(&oRenderer, nPageIndex);

    oFrame.SaveFile(sFile, 4);
}
