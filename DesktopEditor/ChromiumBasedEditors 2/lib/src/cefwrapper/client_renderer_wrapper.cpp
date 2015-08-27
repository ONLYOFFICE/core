#include "./client_renderer.h"

#include <sstream>
#include <string>

#include "include/cef_dom.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"

#include "../../../../common/File.h"
#include "../../../../common/Directory.h"
#include "../../../../graphics/BaseThread.h"
#include "../../src/filedownloader.h"
#include "../../../../raster/ImageFileFormatChecker.h"

namespace asc_client_renderer
{
class CAscEditorNativeV8Handler : public CefV8Handler
{
    enum EditorType
    {
        Document        = 0,
        Presentation    = 1,
        Spreadsheet     = 2
    };

public:
    EditorType  m_etType;
    int         m_nEditorId;
    bool*       sync_command_check;

    NSFile::CFileBinary m_oCurrentFileBinary;
    BYTE*               m_pCurrentFileData;
    int                 m_nCurrentIndex;

    int                 m_nCurrentPrintIndex;
    std::string         m_sVersion;
    std::wstring        m_sAppData;
    std::wstring        m_sFontsData;

    int                 m_nSkipMouseWhellMax;
    int                 m_nSkipMouseWhellCounter;

    CAscEditorNativeV8Handler()
    {
        m_etType = Document;
        m_nEditorId = -1;
        sync_command_check = NULL;

        m_pCurrentFileData  = NULL;
        m_nCurrentIndex     = 0;
        m_nCurrentPrintIndex = 0;
        m_sVersion = "";
        m_sAppData = L"";
        m_sFontsData = L"";

        m_nSkipMouseWhellMax = 2;
        m_nSkipMouseWhellCounter = 0;
    }

    virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) OVERRIDE
    {
        if (name == "Copy")
        {
            CefV8Context::GetCurrentContext()->GetFrame()->Copy();
            return  true;
        }
        else if (name == "Paste")
        {
            CefV8Context::GetCurrentContext()->GetFrame()->Paste();
            return  true;
        }
        else if (name == "Cut")
        {
            CefV8Context::GetCurrentContext()->GetFrame()->Cut();
            return  true;
        }
        else if (name == "SetEditorType")
        {
            CefRefPtr<CefV8Value> val = *arguments.begin();
            if (val->IsValid() && val->IsString())
            {
                CefString editorType = val->GetStringValue();
                std::string strEditorType = editorType.ToString();

                if (strEditorType == "document")
                    m_etType = Document;
                else if (strEditorType == "presentation")
                    m_etType = Presentation;
                else if (strEditorType == "spreadsheet")
                    m_etType = Spreadsheet;
            }

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("EditorType");
            message->GetArgumentList()->SetInt(0, (int)m_etType);
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "SetEditorId")
        {
            CefRefPtr<CefV8Value> val = *arguments.begin();
            if (val->IsValid() && val->IsInt())
            {
                m_nEditorId = val->GetIntValue();
            }
            return true;
        }
        else if (name == "LoadJS")
        {
            if (m_sVersion.empty())
            {
                CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();

                CefRefPtr<CefV8Value> retval;
                CefRefPtr<CefV8Exception> exception;

                bool bIsVersion = browser->GetMainFrame()->GetV8Context()->Eval("window.DocsAPI.DocEditor.version();", retval, exception);
                if (bIsVersion)
                {
                    if (retval->IsString())
                        m_sVersion = retval->GetStringValue().ToString();
                }

                if (m_sVersion.empty())
                    m_sVersion = "undefined";

                CefRefPtr<CefV8Value> retval2;
                CefRefPtr<CefV8Exception> exception2;

                bool bIsAppData = CefV8Context::GetCurrentContext()->Eval("window[\"AscDesktopEditor_AppData\"]();", retval2, exception2);
                if (bIsAppData)
                {
                    if (retval2->IsString())
                    {
                        std::string sAppDataA = retval2->GetStringValue().ToString();
                        m_sAppData = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sAppDataA.c_str(), (LONG)sAppDataA.length());
                    }
                }
                retval = NULL;
                exception2 = NULL;
                bool bIsFontsData = CefV8Context::GetCurrentContext()->Eval("window[\"AscDesktopEditor_FontsData\"]();", retval2, exception2);
                if (bIsAppData)
                {
                    if (retval2->IsString())
                    {
                        std::string sFontsDataA = retval2->GetStringValue().ToString();
                        m_sFontsData = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sFontsDataA.c_str(), (LONG)sFontsDataA.length());
                    }
                }

                /*
                FILE* f = fopen("D:\\editor_version.txt", "a+");
                fprintf(f, m_sVersion.c_str());
                fprintf(f, "\n");
                fclose(f);
                */
            }

            //std::wstring strAppPath = NSFile::GetProcessDirectory();
            std::wstring strAppPath = m_sAppData + L"/webdata/cloud";
            std::wstring strAppPathEditors = strAppPath + L"/" + NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)m_sVersion.c_str(), (LONG)m_sVersion.length());
            if (!NSDirectory::Exists(strAppPathEditors))
            {
                NSDirectory::CreateDirectory(strAppPathEditors);
                NSDirectory::CreateDirectory(strAppPathEditors + L"/word");
                NSDirectory::CreateDirectory(strAppPathEditors + L"/excel");
                NSDirectory::CreateDirectory(strAppPathEditors + L"/powerpoint");
            }

            CefRefPtr<CefV8Value> val = *arguments.begin();
            if (val->IsValid() && val->IsString())
            {
                CefString scriptUrl = val->GetStringValue();
                std::wstring strUrl = scriptUrl.ToWString();

                // 0 - грузить из облака
                // 1 - загружен и исполнен
                // 2 - ждать ответа
                int nResult = 0;

                std::wstring strPath = L"";
                //if (strUrl.find("api/documents/api.js") != std::string::npos)
                //    strPath = strAppPath + L"/api.js";
                if (strUrl.find(L"sdk/Common/AllFonts.js") != std::wstring::npos)
                {
                    strPath = m_sFontsData + L"/AllFonts.js";
                    nResult = 2;
                }
                else if (strUrl.find(L"vendor/xregexp/xregexp-all-min.js") != std::wstring::npos)
                {
                    strPath = strAppPath + L"/xregexp-all-min.js";
                    nResult = 1;
                }
                else if (strUrl.find(L"vendor/sockjs/sockjs.min.js") != std::wstring::npos)
                {
                    strPath = strAppPath + L"/sockjs.min.js";
                    nResult = 1;
                }
                else if (strUrl.find(L"sdk-all.js") != std::wstring::npos)
                {
                    if (m_etType == Document)
                        strPath = strAppPathEditors + L"/word/sdk-all.js";
                    else if (m_etType == Presentation)
                        strPath = strAppPathEditors + L"/powerpoint/sdk-all.js";
                    else if (m_etType == Spreadsheet)
                        strPath = strAppPathEditors + L"/excel/sdk-all.js";

#if 0
                    retval = CefV8Value::CreateInt(0);
                    return true;
#endif

                    nResult = 2;
                }
                else if (strUrl.find(L"app.js") != std::wstring::npos)
                {
                    // сначала определим тип редактора
                    if (strUrl.find(L"documenteditor") != std::wstring::npos)
                        m_etType = Document;
                    else if (strUrl.find(L"presentationeditor") != std::wstring::npos)
                        m_etType = Presentation;
                    else if (strUrl.find(L"spreadsheeteditor") != std::wstring::npos)
                        m_etType = Spreadsheet;

                    CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
                    CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("EditorType");
                    message->GetArgumentList()->SetInt(0, (int)m_etType);
                    browser->SendProcessMessage(PID_BROWSER, message);

                    if (m_etType == Document)
                        strPath = strAppPathEditors + L"/word/app.js";
                    else if (m_etType == Presentation)
                        strPath = strAppPathEditors + L"/powerpoint/app.js";
                    else if (m_etType == Spreadsheet)
                        strPath = strAppPathEditors + L"/excel/app.js";

#if 0
                    retval = CefV8Value::CreateInt(0);
                    return true;
#endif

                    nResult = 2;
                }

                if (strPath != L"" && nResult != 0)
                {
                    if (nResult == 1)
                    {
                        NSFile::CFileBinary oFile;
                        if (oFile.OpenFile(strPath))
                        {
    #if 0
                            FILE* f = fopen("D:\\log_local_url.txt", "a+");
                            fprintf(f, "url: %s\n", strUrl.c_str());
                            fclose(f);
    #endif

                            int nSize = (int)oFile.GetFileSize();
                            BYTE* scriptData = new BYTE[nSize];
                            DWORD dwReadSize = 0;
                            oFile.ReadFile(scriptData, (DWORD)nSize, dwReadSize);

                            std::string strUTF8((char*)scriptData, (LONG)nSize);

                            delete [] scriptData;

                            CefV8Context::GetCurrentContext()->GetFrame()->ExecuteJavaScript(strUTF8, "", 0);
                            retval = CefV8Value::CreateInt(1);
                        }
                        else
                        {
                            retval = CefV8Value::CreateInt(0);
                        }
                    }
                    else
                    {
                        CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
                        CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("load_js");
                        int64 frameId = CefV8Context::GetCurrentContext()->GetFrame()->GetIdentifier();
                        message->GetArgumentList()->SetString(0, GetFullUrl(strUrl, CefV8Context::GetCurrentContext()->GetFrame()->GetURL().ToWString()));
                        message->GetArgumentList()->SetString(1, strPath);
                        message->GetArgumentList()->SetInt(2, (int)frameId);
                        browser->SendProcessMessage(PID_BROWSER, message);

                        retval = CefV8Value::CreateInt(2);
                    }
                }
                else
                {
                    retval = CefV8Value::CreateInt(0);
                }
            }
            else
            {
                retval = CefV8Value::CreateInt(0);
            }
            return true;
        }
        else if (name == "LoadFontBase64")
        {
            CefRefPtr<CefV8Value> val = *arguments.begin();
            CefString fileUrl = val->GetStringValue();
            std::wstring strUrl = fileUrl.ToWString();

            NSFile::CFileBinary oFile;
            oFile.OpenFile(strUrl);

            int nSize1 = oFile.GetFileSize();
            DWORD dwSize = 0;
            BYTE* pFontData = new BYTE[nSize1];
            oFile.ReadFile(pFontData, (DWORD)nSize1, dwSize);
            oFile.CloseFile();

            char* pDataDst = NULL;
            int nDataDst = 0;
            NSFile::CBase64Converter::Encode(pFontData, nSize1, pDataDst, nDataDst, NSBase64::B64_BASE64_FLAG_NOCRLF);

            std::string sFontBase64(pDataDst, nDataDst);
            RELEASEARRAYOBJECTS(pDataDst);

            int pos1 = strUrl.find_last_of(L"\\");
            int pos2 = strUrl.find_last_of(L"/");

            int nMax = (pos1 > pos2) ? pos1 : pos2;

            //std::wstring sName = strUrl.substr(nMax + 1);
            std::string sName = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(strUrl.c_str(), strUrl.length());
            sName = "window[\"" + sName + "\"] = \"" + std::to_string(nSize1) + ";" + sFontBase64 + "\";";

            CefString sJS;
            sJS.FromString(sName);

            CefV8Context::GetCurrentContext()->GetFrame()->ExecuteJavaScript(sJS, "", 0);

            return true;
        }
        else if (name == "OpenBinaryFile")
        {
            CefRefPtr<CefV8Value> val = *arguments.begin();
            CefString fileUrl = val->GetStringValue();
            std::wstring strUrl = fileUrl.ToWString();

            m_oCurrentFileBinary.CloseFile();
            m_oCurrentFileBinary.OpenFile(strUrl);

            uint32 nSize = (uint32)m_oCurrentFileBinary.GetFileSize();
            m_nCurrentIndex = 0;

            retval = CefV8Value::CreateUInt(nSize);

            m_pCurrentFileData = new BYTE[nSize];

            return true;
        }
        else if (name == "CloseBinaryFile")
        {
            m_oCurrentFileBinary.CloseFile();
            RELEASEARRAYOBJECTS(m_pCurrentFileData);
            m_nCurrentIndex = 0;

            return true;
        }
        else if (name == "GetBinaryFileData")
        {
            retval = CefV8Value::CreateInt(m_pCurrentFileData[m_nCurrentIndex++]);

            return true;
        }
        else if (name == "getFontsSprite")
        {
            bool bIsRetina = false;
            if (arguments.size() > 0)
            {
                CefRefPtr<CefV8Value> val = *arguments.begin();
                bIsRetina = val->GetBoolValue();
            }

            std::wstring strUrl = (false == bIsRetina) ?
                        (m_sFontsData + L"/fonts_thumbnail.png") :
                        (m_sFontsData + L"/fonts_thumbnail@2x.png");

            NSFile::CFileBinary oFile;
            oFile.OpenFile(strUrl);

            int nSize1 = oFile.GetFileSize();
            DWORD dwSize = 0;
            BYTE* pFontData = new BYTE[nSize1];
            oFile.ReadFile(pFontData, (DWORD)nSize1, dwSize);
            oFile.CloseFile();

            char* pDataDst = NULL;
            int nDataDst = 0;
            NSFile::CBase64Converter::Encode(pFontData, nSize1, pDataDst, nDataDst);

            std::string sFontBase64(pDataDst, nDataDst);
            RELEASEARRAYOBJECTS(pDataDst);

            sFontBase64 = "data:image/jpeg;base64," + sFontBase64;

            retval = CefV8Value::CreateString(sFontBase64.c_str());
            return true;
        }
        else if (name == "SpellCheck")
        {
            CefRefPtr<CefV8Value> val = *arguments.begin();
            CefString sTask = val->GetStringValue();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            int64 frameId = CefV8Context::GetCurrentContext()->GetFrame()->GetIdentifier();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("spell_check_task");
            message->GetArgumentList()->SetInt(0, (int)m_nEditorId);
            message->GetArgumentList()->SetString(1, sTask);
            message->GetArgumentList()->SetInt(2, (int)frameId);
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "CreateEditorApi")
        {
            volatile bool* pChecker = this->sync_command_check;
            *pChecker = true;

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("create_editor_api");
            browser->SendProcessMessage(PID_BROWSER, message);
            return true;
        }
        else if (name == "ConsoleLog")
        {
#if 0
            CefRefPtr<CefV8Value> val = *arguments.begin();
            CefString sTask = val->GetStringValue();

            FILE* ff = fopen("D:\\cef_console.log", "a+");
            fprintf(ff, sTask.ToString().c_str());
            fprintf(ff, "\n");
            fclose(ff);
#endif
            return true;
        }
        else if (name == "GetFrameContent")
        {
            CefRefPtr<CefV8Value> val = *arguments.begin();
            CefString sFrame = val->GetStringValue();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefFrame> _frame = browser->GetFrame(sFrame);
            if (_frame)
            {
                std::string sName = _frame->GetName().ToString();
                std::string sCode = "window[\"AscDesktopEditor\"][\"SetFrameContent\"](document.body.firstChild ? JSON.stringify(document.body.firstChild.innerHTML) : \"\", \"" +
                        sName + "\");";
                //std::string sCode = "alert(document.body.innerHTML);";
                _frame->ExecuteJavaScript(sCode, _frame->GetURL(), 0);
            }

            return true;
        }
        else if (name == "SetFrameContent")
        {
            std::vector<CefRefPtr<CefV8Value> >::const_iterator iter = arguments.begin();

            std::string sFrame =  (*iter)->GetStringValue(); ++iter;
            std::string sFrameName =  (*iter)->GetStringValue();

            if (sFrame.find("\"") != 0)
            {
                sFrame = ("\"" + sFrame + "\"");
            }

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefFrame> _frame = browser->GetMainFrame();
            if (_frame)
            {
                std::string sCode = "if (window[\"onchildframemessage\"]) { window[\"onchildframemessage\"](" +
                        sFrame + ", \"" + sFrameName + "\"); }";
                _frame->ExecuteJavaScript(sCode, _frame->GetURL(), 0);
            }

            return true;
        }
        else if (name == "setCookie")
        {
            if (arguments.size() != 5)
                return true;

            std::vector<CefRefPtr<CefV8Value> >::const_iterator iter = arguments.begin();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("set_cookie");
            message->GetArgumentList()->SetString(0, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(1, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(2, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(3, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(4, (*iter)->GetStringValue()); ++iter;
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "setAuth")
        {
            if (arguments.size() != 4)
                return true;

            std::vector<CefRefPtr<CefV8Value> >::const_iterator iter = arguments.begin();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("set_cookie");
            message->GetArgumentList()->SetString(0, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(1, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(2, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(3, "asc_auth_key");
            message->GetArgumentList()->SetString(4, (*iter)->GetStringValue()); ++iter;
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "getCookiePresent")
        {
            if (arguments.size() != 2)
                return true;

            std::vector<CefRefPtr<CefV8Value> >::const_iterator iter = arguments.begin();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("is_cookie_present");
            message->GetArgumentList()->SetString(0, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(1, (*iter)->GetStringValue()); ++iter;
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "getAuth")
        {
            if (arguments.size() != 1)
                return true;

            std::vector<CefRefPtr<CefV8Value> >::const_iterator iter = arguments.begin();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("is_cookie_present");
            message->GetArgumentList()->SetString(0, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(1, "asc_auth_key");
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "onDocumentModifiedChanged")
        {
            if (arguments.size() != 1)
                return true;

            CefRefPtr<CefV8Value> val = *arguments.begin();
            bool bValue = val->GetBoolValue();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("onDocumentModifiedChanged");
            message->GetArgumentList()->SetBool(0, bValue);
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "SetDocumentName")
        {
            CefRefPtr<CefV8Value> val = *arguments.begin();
            CefString sName = val->GetStringValue();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("set_document_name");
            message->GetArgumentList()->SetString(0, sName);
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "OnSave")
        {
            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("on_document_save");
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "js_message")
        {
            if (arguments.size() != 2)
                return true;

            std::vector<CefRefPtr<CefV8Value> >::const_iterator iter = arguments.begin();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("js_message");
            message->GetArgumentList()->SetString(0, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetString(1, (*iter)->GetStringValue()); ++iter;
            browser->SendProcessMessage(PID_BROWSER, message);

            return true;
        }
        else if (name == "Print_Start")
        {
            if (arguments.size() != 4)
                return true;

            std::vector<CefRefPtr<CefV8Value> >::const_iterator iter = arguments.begin();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("print_start");
            message->GetArgumentList()->SetString(0, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetInt(1, (*iter)->GetIntValue()); ++iter;
            message->GetArgumentList()->SetString(2, browser->GetFocusedFrame()->GetURL());
            message->GetArgumentList()->SetString(3, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetInt(4, (*iter)->GetIntValue());
            browser->SendProcessMessage(PID_BROWSER, message);

            m_nCurrentPrintIndex = 0;
            return true;
        }
        else if (name == "Print_Page")
        {
            if (arguments.size() != 3)
                return true;

            std::vector<CefRefPtr<CefV8Value> >::const_iterator iter = arguments.begin();

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("print_page");
            message->GetArgumentList()->SetString(0, (*iter)->GetStringValue()); ++iter;
            message->GetArgumentList()->SetInt(1, m_nCurrentPrintIndex);
            message->GetArgumentList()->SetDouble(2, (*iter)->GetDoubleValue()); ++iter;
            message->GetArgumentList()->SetDouble(3, (*iter)->GetDoubleValue()); ++iter;
            browser->SendProcessMessage(PID_BROWSER, message);

            m_nCurrentPrintIndex++;
            return true;
        }
        else if (name == "Print_End")
        {
            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("print_end");
            browser->SendProcessMessage(PID_BROWSER, message);

            m_nCurrentPrintIndex = 0;
            return true;
        }
        else if (name == "Print")
        {
            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("print");
            browser->SendProcessMessage(PID_BROWSER, message);
            return true;
        }
        else if (name == "Set_App_Data")
        {
            CefRefPtr<CefV8Value> val = *arguments.begin();
            m_sAppData = val->GetStringValue().ToWString();

            return true;
        }
        else if (name == "IsSupportNativePrint")
        {
            retval = CefV8Value::CreateBool(false);
            return true;
        }
        else if (name == "CheckNeedWheel")
        {
#ifdef WIN32
            m_nSkipMouseWhellCounter++;
            if (m_nSkipMouseWhellCounter == m_nSkipMouseWhellMax)
            {
                m_nSkipMouseWhellCounter = 0;
                retval = CefV8Value::CreateBool(false);
            }
            else
            {
                retval = CefV8Value::CreateBool(true);
            }
#else
            retval = CefV8Value::CreateBool(true);
#endif
            return true;
        }
        else if (name == "GetImageBase64")
        {
            if (arguments.size() != 1)
            {
                retval = CefV8Value::CreateString("");
                return true;
            }

            CefRefPtr<CefV8Value> val = *arguments.begin();
            std::wstring sFileUrl = val->GetStringValue().ToWString();

            if (sFileUrl.find(L"file://") == 0)
            {
                if (NSFile::CFileBinary::Exists(sFileUrl.substr(7)))
                    sFileUrl = sFileUrl.substr(7);
                else if (NSFile::CFileBinary::Exists(sFileUrl.substr(8)))
                    sFileUrl = sFileUrl.substr(8);
            }

            std::string sHeader = "";

            NSFile::CFileBinary oFileBinary;
            if (!oFileBinary.OpenFile(sFileUrl))
            {
                retval = CefV8Value::CreateString("");
                return true;
            }

            int nDetectSize = 50;
            if (nDetectSize > (int)oFileBinary.GetFileSize())
            {
                retval = CefV8Value::CreateString("");
                return true;
            }

            BYTE* pData = new BYTE[nDetectSize];
            memset(pData, 0, nDetectSize);
            DWORD dwRead = 0;
            oFileBinary.ReadFile(pData, (DWORD)nDetectSize, dwRead);
            oFileBinary.CloseFile();

            CImageFileFormatChecker _checker;

            if (_checker.isBmpFile(pData, nDetectSize))
                sHeader = "data:image/bmp;base64,";
            else if (_checker.isJpgFile(pData, nDetectSize))
                sHeader = "data:image/jpeg;base64,";
            else if (_checker.isPngFile(pData, nDetectSize))
                sHeader = "data:image/png;base64,";
            else if (_checker.isGifFile(pData, nDetectSize))
                sHeader = "data:image/gif;base64,";
            else if (_checker.isTiffFile(pData, nDetectSize))
                sHeader = "data:image/tiff;base64,";

            RELEASEARRAYOBJECTS(pData);

            if (sHeader.empty())
            {
                retval = CefV8Value::CreateString("");
                return true;
            }

            NSFile::CFileBinary oFile;
            oFile.OpenFile(sFileUrl);

            int nSize1 = oFile.GetFileSize();
            DWORD dwSize = 0;
            BYTE* pFontData = new BYTE[nSize1];
            oFile.ReadFile(pFontData, (DWORD)nSize1, dwSize);
            oFile.CloseFile();

            char* pDataDst = NULL;
            int nDataDst = 0;
            NSFile::CBase64Converter::Encode(pFontData, nSize1, pDataDst, nDataDst);

            std::string sFontBase64(pDataDst, nDataDst);
            RELEASEARRAYOBJECTS(pDataDst);

            sFontBase64 = sHeader + sFontBase64;

            retval = CefV8Value::CreateString(sFontBase64.c_str());
            return true;
        }
        else if (name == "SetFullscreen")
        {
            bool bIsFullScreen = false;
            if (arguments.size() > 0)
            {
                CefRefPtr<CefV8Value> val = *arguments.begin();
                bIsFullScreen = val->GetBoolValue();
            }

            CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();

            if (bIsFullScreen)
            {
                CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("onfullscreenenter");
                browser->SendProcessMessage(PID_BROWSER, message);
            }
            else
            {
                CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("onfullscreenleave");
                browser->SendProcessMessage(PID_BROWSER, message);
            }

            return true;
        }
        // Function does not exist.
        return false;
    }

    std::wstring GetFullUrl(const std::wstring& sUrl, const std::wstring& sBaseUrl)
    {
        std::wstring sUrlSrc = L"";
        if (CFileDownloader::IsNeedDownload(sUrl))
        {
            sUrlSrc = sUrl;
        }
        else
        {
            if (0 == sUrl.find(wchar_t('/')))
            {
                // нужно брать корень сайта
                int nPos = sBaseUrl.find(L"//");
                if (nPos != std::wstring::npos)
                {
                    nPos = sBaseUrl.find(wchar_t('/'), nPos + 3);
                    if (nPos != std::wstring::npos)
                    {
                        sUrlSrc = sBaseUrl.substr(0, nPos);
                        sUrlSrc += sUrl;
                    }
                }
                if (sUrlSrc.empty())
                {
                    sUrlSrc = sBaseUrl;
                    sUrlSrc += (L"/" + sUrl);
                }
            }
            else
            {
                // брать место урла
                int nPos = sBaseUrl.find_last_of(wchar_t('/'));
                if (std::wstring::npos != nPos)
                {
                    sUrlSrc = sBaseUrl.substr(0, nPos);
                }
                else
                {
                    sUrlSrc = sBaseUrl;
                }
                sUrlSrc += (L"/" + sUrl);
            }
        }
        return sUrlSrc;
    }

    bool DownloadFile(const std::wstring& sUrl, const std::wstring& sDst, const std::wstring& sBaseUrl)
    {
        std::wstring sUrlSrc = GetFullUrl(sUrl, sBaseUrl);

        CFileDownloader oDownloader(sUrlSrc, false);
        oDownloader.SetFilePath(sDst);
        oDownloader.Start( 0 );
        while ( oDownloader.IsRunned() )
        {
            NSThreads::Sleep(10);
        }
        return oDownloader.IsFileDownloaded();
    }

    // Provide the reference counting implementation for this class.
    IMPLEMENT_REFCOUNTING(CAscEditorNativeV8Handler);
};

class ClientRenderDelegate : public client::ClientAppRenderer::Delegate {
 public:
  ClientRenderDelegate()
    : last_node_is_editable_(false)
  {
    sync_command_check = false;
  }

  virtual void OnWebKitInitialized(CefRefPtr<client::ClientAppRenderer> app) OVERRIDE {
    // Create the renderer-side router for query handling.
    CefMessageRouterConfig config;
    message_router_ = CefMessageRouterRendererSide::Create(config);
  }

  virtual void OnContextCreated(CefRefPtr<client::ClientAppRenderer> app,
                                CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefV8Context> context) OVERRIDE {
    message_router_->OnContextCreated(browser,  frame, context);

    // add AscEditorNative
    CefRefPtr<CefV8Value> object = context->GetGlobal();

    CefRefPtr<CefV8Value> objNative = CefV8Value::CreateObject(NULL);
    CAscEditorNativeV8Handler* pNativeHandlerWrapper = new CAscEditorNativeV8Handler();
    pNativeHandlerWrapper->sync_command_check = &sync_command_check;

    CefRefPtr<CefV8Handler> _nativeHandler = pNativeHandlerWrapper;

    CefRefPtr<CefV8Value> _nativeFunctionCopy = CefV8Value::CreateFunction("Copy", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunctionPaste = CefV8Value::CreateFunction("Paste", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunctionCut = CefV8Value::CreateFunction("Cut", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunctionLoadJS = CefV8Value::CreateFunction("LoadJS", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunctionEditorType = CefV8Value::CreateFunction("SetEditorType", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction11 = CefV8Value::CreateFunction("LoadFontBase64", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction22 = CefV8Value::CreateFunction("getFontsSprite", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction33 = CefV8Value::CreateFunction("SetEditorId", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction44 = CefV8Value::CreateFunction("SpellCheck", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction55 = CefV8Value::CreateFunction("CreateEditorApi", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction66 = CefV8Value::CreateFunction("ConsoleLog", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction77 = CefV8Value::CreateFunction("GetFrameContent", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction88 = CefV8Value::CreateFunction("SetFrameContent", _nativeHandler);

    CefRefPtr<CefV8Value> _nativeFunction111 = CefV8Value::CreateFunction("setCookie", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction112 = CefV8Value::CreateFunction("setAuth", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction113 = CefV8Value::CreateFunction("getCookiePresent", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction114 = CefV8Value::CreateFunction("getAuth", _nativeHandler);

    CefRefPtr<CefV8Value> _nativeFunction222 = CefV8Value::CreateFunction("onDocumentModifiedChanged", _nativeHandler);

    CefRefPtr<CefV8Value> _nativeFunction301 = CefV8Value::CreateFunction("OpenBinaryFile", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction302 = CefV8Value::CreateFunction("CloseBinaryFile", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction303 = CefV8Value::CreateFunction("GetBinaryFileData", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction304 = CefV8Value::CreateFunction("GetImageBase64", _nativeHandler);

    CefRefPtr<CefV8Value> _nativeFunction401 = CefV8Value::CreateFunction("SetDocumentName", _nativeHandler);

    CefRefPtr<CefV8Value> _nativeFunction501 = CefV8Value::CreateFunction("OnSave", _nativeHandler);

    CefRefPtr<CefV8Value> _nativeFunction601 = CefV8Value::CreateFunction("js_message", _nativeHandler);

    CefRefPtr<CefV8Value> _nativeFunction602 = CefV8Value::CreateFunction("CheckNeedWheel", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction603 = CefV8Value::CreateFunction("SetFullscreen", _nativeHandler);

    CefRefPtr<CefV8Value> _nativeFunction701 = CefV8Value::CreateFunction("Print_Start", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction702 = CefV8Value::CreateFunction("Print_Page", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction703 = CefV8Value::CreateFunction("Print_End", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction704 = CefV8Value::CreateFunction("Print", _nativeHandler);
    CefRefPtr<CefV8Value> _nativeFunction705 = CefV8Value::CreateFunction("IsSupportNativePrint", _nativeHandler);

    CefRefPtr<CefV8Value> _nativeFunction801 = CefV8Value::CreateFunction("Set_App_Data", _nativeHandler);

    objNative->SetValue("Copy", _nativeFunctionCopy, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("Paste", _nativeFunctionPaste, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("Cut", _nativeFunctionCut, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("LoadJS", _nativeFunctionLoadJS, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("SetEditorType", _nativeFunctionEditorType, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("LoadFontBase64", _nativeFunction11, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("getFontsSprite", _nativeFunction22, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("SetEditorId", _nativeFunction33, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("SpellCheck", _nativeFunction44, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("CreateEditorApi", _nativeFunction55, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("ConsoleLog", _nativeFunction66, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("GetFrameContent", _nativeFunction77, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("SetFrameContent", _nativeFunction88, V8_PROPERTY_ATTRIBUTE_NONE);

    objNative->SetValue("setCookie", _nativeFunction111, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("setAuth", _nativeFunction112, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("getCookiePresent", _nativeFunction113, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("getAuth", _nativeFunction114, V8_PROPERTY_ATTRIBUTE_NONE);

    objNative->SetValue("onDocumentModifiedChanged", _nativeFunction222, V8_PROPERTY_ATTRIBUTE_NONE);

    objNative->SetValue("OpenBinaryFile", _nativeFunction301, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("CloseBinaryFile", _nativeFunction302, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("GetBinaryFileData", _nativeFunction303, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("GetImageBase64", _nativeFunction304, V8_PROPERTY_ATTRIBUTE_NONE);

    objNative->SetValue("SetDocumentName", _nativeFunction401, V8_PROPERTY_ATTRIBUTE_NONE);

    objNative->SetValue("OnSave", _nativeFunction501, V8_PROPERTY_ATTRIBUTE_NONE);

    objNative->SetValue("js_message", _nativeFunction601, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("CheckNeedWheel", _nativeFunction602, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("SetFullscreen", _nativeFunction603, V8_PROPERTY_ATTRIBUTE_NONE);

    objNative->SetValue("Print_Start", _nativeFunction701, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("Print_Page", _nativeFunction702, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("Print_End", _nativeFunction703, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("Print", _nativeFunction704, V8_PROPERTY_ATTRIBUTE_NONE);
    objNative->SetValue("IsSupportNativePrint", _nativeFunction705, V8_PROPERTY_ATTRIBUTE_NONE);

    objNative->SetValue("Set_App_Data", _nativeFunction801, V8_PROPERTY_ATTRIBUTE_NONE);

    object->SetValue("AscDesktopEditor", objNative, V8_PROPERTY_ATTRIBUTE_NONE);
  }

  virtual void OnContextReleased(CefRefPtr<client::ClientAppRenderer> app,
                                 CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context) OVERRIDE {
    message_router_->OnContextReleased(browser,  frame, context);
  }

  virtual void OnFocusedNodeChanged(CefRefPtr<client::ClientAppRenderer> app,
                                    CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefDOMNode> node) OVERRIDE {
    bool is_editable = (node.get() && node->IsEditable());
    if (is_editable != last_node_is_editable_)
    {
      // Notify the browser of the change in focused element type.
      last_node_is_editable_ = is_editable;
#if 0
      CefRefPtr<CefProcessMessage> message =
          CefProcessMessage::Create(kFocusedNodeChangedMessage);
      message->GetArgumentList()->SetBool(0, is_editable);
      browser->SendProcessMessage(PID_BROWSER, message);
#endif
    }
  }

  virtual bool OnProcessMessageReceived(
      CefRefPtr<client::ClientAppRenderer> app,
      CefRefPtr<CefBrowser> browser,
      CefProcessId source_process,
      CefRefPtr<CefProcessMessage> message) OVERRIDE
{

    std::string sMessageName = message->GetName().ToString();

    if (sMessageName == "keyboard_layout")
    {
        int nKeyboardLayout = message->GetArgumentList()->GetInt(0);
        std::string sLayout = std::to_string(nKeyboardLayout);
        std::string sCode = "window[\"asc_current_keyboard_layout\"] = " + sLayout + ";";

        std::vector<int64> ids;
        browser->GetFrameIdentifiers(ids);

        for (std::vector<int64>::iterator i = ids.begin(); i != ids.end(); i++)
        {
            CefRefPtr<CefFrame> _frame = browser->GetFrame(*i);
            _frame->ExecuteJavaScript(sCode, _frame->GetURL(), 0);
        }

        return true;
    }
    else if (sMessageName == "cef_control_id")
    {
        CefRefPtr<CefFrame> _frame = browser->GetFrame("frameEditor");
        if (_frame)
        {
            int nControlId = message->GetArgumentList()->GetInt(0);
            std::string sControlId = std::to_string(nControlId);
            std::string sCode = "window[\"AscDesktopEditor\"][\"SetEditorId\"](" + sControlId + ");";

            _frame->ExecuteJavaScript(sCode, _frame->GetURL(), 0);
        }

        return true;
    }
    else if (sMessageName == "spell_check_response")
    {
        int64 nFrameId = (int64)message->GetArgumentList()->GetInt(1);
        CefRefPtr<CefFrame> _frame = browser->GetFrame(nFrameId);
        if (_frame)
        {
            std::string sCode = "window[\"asc_nativeOnSpellCheck\"](" + message->GetArgumentList()->GetString(0).ToString() + ");";
            _frame->ExecuteJavaScript(sCode, _frame->GetURL(), 0);
        }

        return true;
    }
    else if (sMessageName == "sync_command_end")
    {
        sync_command_check = false;
        return true;
    }
    else if (sMessageName == "on_is_cookie_present")
    {
        CefRefPtr<CefFrame> _frame = browser->GetMainFrame();
        bool bIsPresent = message->GetArgumentList()->GetBool(0);
        std::string sValue = message->GetArgumentList()->GetString(1).ToString();

        std::string sCode = bIsPresent ? ("if (window[\"on_is_cookie_present\"]) { window[\"on_is_cookie_present\"](true, \"" + sValue + "\"); }") :
                                 "if (window[\"on_is_cookie_present\"]) { window[\"on_is_cookie_present\"](false, undefined); }";
        _frame->ExecuteJavaScript(sCode, _frame->GetURL(), 0);

        return true;
    }
    else if (sMessageName == "on_set_cookie")
    {
        CefRefPtr<CefFrame> _frame = browser->GetMainFrame();

        std::string sCode = "if (window[\"on_set_cookie\"]) { window[\"on_set_cookie\"](); }";
        _frame->ExecuteJavaScript(sCode, _frame->GetURL(), 0);

        return true;
    }
    else if (sMessageName == "document_save")
    {
        CefRefPtr<CefFrame> _frame = browser->GetFrame("frameEditor");

        if (_frame)
        {
            std::string sCode = "if (window[\"AscDesktopEditor_Save\"]) { window[\"AscDesktopEditor_Save\"](); }";
            _frame->ExecuteJavaScript(sCode, _frame->GetURL(), 0);
        }
        return true;
    }
    else if (sMessageName == "print")
    {
        CefRefPtr<CefFrame> _frame = browser->GetFrame("frameEditor");

        if (_frame)
        {
            std::string sCode = "if (window[\"Asc\"] && window[\"Asc\"][\"editor\"]) { window[\"Asc\"][\"editor\"][\"asc_nativePrint\"](undefined, undefined); }";
            sCode += "else if (window[\"editor\"]) { window[\"editor\"][\"asc_nativePrint\"](undefined, undefined); }";
            _frame->ExecuteJavaScript(sCode, _frame->GetURL(), 0);
        }
        return true;
    }
    else if (sMessageName == "on_load_js")
    {
        int64 frameId = (int64)message->GetArgumentList()->GetInt(2);
        CefRefPtr<CefFrame> _frame = browser->GetFrame(frameId);

        if (_frame)
        {
            std::wstring sFilePath = message->GetArgumentList()->GetString(0).ToWString();
            NSFile::CFileBinary oFile;
            if (oFile.OpenFile(sFilePath))
            {
                int nSize = (int)oFile.GetFileSize();
                BYTE* scriptData = new BYTE[nSize];
                DWORD dwReadSize = 0;
                oFile.ReadFile(scriptData, (DWORD)nSize, dwReadSize);

                std::string strUTF8((char*)scriptData, nSize);

                delete [] scriptData;
                scriptData = NULL;

                _frame->ExecuteJavaScript(strUTF8, _frame->GetURL(), 0);

                _frame->ExecuteJavaScript("window[\"asc_desktop_on_load_js\"]();", _frame->GetURL(), 0);
            }
            else
            {
                // все равно посылаем - пусть лучше ошибка в консоль, чем подвисание в requirejs
                _frame->ExecuteJavaScript("window[\"asc_desktop_on_load_js\"]();", _frame->GetURL(), 0);
            }
        }
        return true;
    }

    return message_router_->OnProcessMessageReceived(
        browser, source_process, message);
}

 private:
  bool last_node_is_editable_;
  bool sync_command_check;

  // Handles the renderer side of query routing.
  CefRefPtr<CefMessageRouterRendererSide> message_router_;

  IMPLEMENT_REFCOUNTING(ClientRenderDelegate);
};

void CreateRenderDelegates(client::ClientAppRenderer::DelegateSet& delegates) {
  delegates.insert(new ClientRenderDelegate);
}

}  // namespace client_renderer
