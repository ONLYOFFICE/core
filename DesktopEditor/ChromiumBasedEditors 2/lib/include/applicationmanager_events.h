#ifndef APPLICATION_MANAGER_EVENTS_H
#define APPLICATION_MANAGER_EVENTS_H

#include "../../../Word_Api/Editor_Api.h"

#define ASC_MENU_EVENT_TYPE_CEF_CREATETAB       1001
#define ASC_MENU_EVENT_TYPE_CEF_TABEDITORTYPE   1002
#define ASC_MENU_EVENT_TYPE_CEF_SPELLCHECK      1003
#define ASC_MENU_EVENT_TYPE_CEF_KEYBOARDLAYOUT  1004
#define ASC_MENU_EVENT_TYPE_CEF_CONTROL_ID      1005
#define ASC_MENU_EVENT_TYPE_CEF_SYNC_COMMAND    1006
#define ASC_MENU_EVENT_TYPE_CEF_CLOSE           1007
#define ASC_MENU_EVENT_TYPE_CEF_DESTROY         1008
#define ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD        1009
#define ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD_START  1010
#define ASC_MENU_EVENT_TYPE_CEF_DOWNLOAD_END    1011
#define ASC_MENU_EVENT_TYPE_CEF_MODIFY_CHANGED  1012

#define ASC_MENU_EVENT_TYPE_CEF_DOCUMENT_NAME   1013
#define ASC_MENU_EVENT_TYPE_CEF_ONLOGOUT        1014

#define ASC_MENU_EVENT_TYPE_CEF_ONOPENLINK      1015

#define ASC_MENU_EVENT_TYPE_CEF_ONCLOSE         2001
#define ASC_MENU_EVENT_TYPE_CEF_ONBEFORECLOSE   2002

#define ASC_MENU_EVENT_TYPE_CEF_SAVE            3001
#define ASC_MENU_EVENT_TYPE_CEF_ONSAVE          3002

#define ASC_MENU_EVENT_TYPE_CEF_ONKEYBOARDDOWN      3100
#define ASC_MENU_EVENT_TYPE_CEF_ONFULLSCREENENTER   3101
#define ASC_MENU_EVENT_TYPE_CEF_ONFULLSCREENLEAVE   3102

#define ASC_MENU_EVENT_TYPE_CEF_JS_MESSAGE      4001

#define ASC_MENU_EVENT_TYPE_CEF_ONBEFORE_PRINT_START        5001
#define ASC_MENU_EVENT_TYPE_CEF_ONBEFORE_PRINT_PROGRESS     5002
#define ASC_MENU_EVENT_TYPE_CEF_ONBEFORE_PRINT_END          5003

#define ASC_MENU_EVENT_TYPE_CEF_PRINT_START                 5011
#define ASC_MENU_EVENT_TYPE_CEF_PRINT_PAGE                  5012
#define ASC_MENU_EVENT_TYPE_CEF_PRINT_END                   5013

#define ASC_MENU_EVENT_TYPE_CEF_SCRIPT_EDITOR_VERSION       6001

#define ASC_MENU_EVENT_TYPE_CEF_SAVEFILEDIALOG              7001

#define ASC_MENU_EVENT_TYPE_WINDOWS_MESSAGE_USER_COUNT      10

namespace NSEditorApi
{
    class CAscCreateTab : public IMenuEventDataBase
    {
    private:
        std::wstring m_sUrl;
        bool m_bActive;
        int m_nIdEqual;

    public:
        CAscCreateTab()
        {
            m_bActive = true;
            m_nIdEqual = -1;
        }
        virtual ~CAscCreateTab()
        {
        }

        LINK_PROPERTY_STRING(Url)
        LINK_PROPERTY_BOOL(Active)
        LINK_PROPERTY_INT(IdEqual)
    };

    class CAscDownloadFileInfo : public IMenuEventDataBase
    {
    private:
        std::wstring    m_sUrl;
        std::wstring    m_sFilePath;

        int             m_nId;

        double          m_dSpeed;       // Kb/sec
        int             m_nPercent;
        bool            m_bIsComplete;
        bool            m_bIsCanceled;

    public:
        CAscDownloadFileInfo()
        {
            m_nId           = -1;

            m_dSpeed        = 0;
            m_nPercent      = 0;
            m_bIsComplete   = false;
            m_bIsCanceled     = false;
        }
        virtual ~CAscDownloadFileInfo()
        {
        }

        LINK_PROPERTY_STRING(Url)
        LINK_PROPERTY_STRING(FilePath)
        LINK_PROPERTY_INT(Percent)
        LINK_PROPERTY_BOOL(IsComplete)
        LINK_PROPERTY_BOOL(IsCanceled)
        LINK_PROPERTY_INT(Id)
        LINK_PROPERTY_DOUBLE(Speed)
    };

    class CAscOnOpenExternalLink : public IMenuEventDataBase
    {
    private:
        std::wstring    m_sUrl;
        int             m_nId;

    public:
        CAscOnOpenExternalLink()
        {
            m_nId           = -1;
        }
        virtual ~CAscOnOpenExternalLink()
        {
        }

        LINK_PROPERTY_STRING(Url)
        LINK_PROPERTY_INT(Id)
    };

    class CAscTabEditorType : public IMenuEventDataBase
    {
    private:
        int m_nId;
        int m_nType;

    public:
        CAscTabEditorType()
        {
        }
        virtual ~CAscTabEditorType()
        {
        }

        LINK_PROPERTY_INT(Id)
        LINK_PROPERTY_INT(Type)
    };

    class CAscSpellCheckType : public IMenuEventDataBase
    {
    private:
        std::string m_sResult;
        int         m_nEditorId;
        int         m_nFrameId;

    public:
        CAscSpellCheckType()
        {
            m_nEditorId = 0;
            m_nFrameId = 0;
        }
        virtual ~CAscSpellCheckType()
        {
        }

        LINK_PROPERTY_STRINGA(Result)
        LINK_PROPERTY_INT(EditorId)
        LINK_PROPERTY_INT(FrameId)
    };

    class CAscKeyboardLayout : public IMenuEventDataBase
    {
    private:
        int         m_nLanguage;

    public:
        CAscKeyboardLayout()
        {
        }
        virtual ~CAscKeyboardLayout()
        {
        }

        LINK_PROPERTY_INT(Language)
    };

    class CAscTypeId : public IMenuEventDataBase
    {
    private:
        int m_nId;

    public:
        CAscTypeId()
        {
        }
        virtual ~CAscTypeId()
        {
        }

        LINK_PROPERTY_INT(Id)
    };

    class CAscDocumentModifyChanged : public IMenuEventDataBase
    {
    private:
        int m_nId;
        bool m_bChanged;

    public:
        CAscDocumentModifyChanged()
        {
        }
        virtual ~CAscDocumentModifyChanged()
        {
        }

        LINK_PROPERTY_INT(Id)
        LINK_PROPERTY_BOOL(Changed)
    };

    class CAscDocumentName : public IMenuEventDataBase
    {
    private:
        std::wstring m_sName;
        int m_nId;

    public:
        CAscDocumentName()
        {
        }
        virtual ~CAscDocumentName()
        {
        }

        LINK_PROPERTY_STRING(Name)
        LINK_PROPERTY_INT(Id)
    };

    class CAscJSMessage : public IMenuEventDataBase
    {
    private:
        std::wstring m_sName;
        std::wstring m_sValue;
        int m_nId;

    public:
        CAscJSMessage()
        {
        }
        virtual ~CAscJSMessage()
        {
        }

        LINK_PROPERTY_STRING(Name)
        LINK_PROPERTY_STRING(Value)
        LINK_PROPERTY_INT(Id)
    };

    class CAscPrintProgress : public IMenuEventDataBase
    {
    private:
        int m_nId;
        int m_nProgress;

    public:
        CAscPrintProgress()
        {
        }
        virtual ~CAscPrintProgress()
        {
        }

        LINK_PROPERTY_INT(Id)
        LINK_PROPERTY_INT(Progress)
    };

    class CAscPrintEnd : public IMenuEventDataBase
    {
    private:
        int m_nId;
        int m_nPagesCount;
        int m_nCurrentPage;

    public:
        CAscPrintEnd()
        {
        }
        virtual ~CAscPrintEnd()
        {
        }

        LINK_PROPERTY_INT(Id)
        LINK_PROPERTY_INT(PagesCount)
        LINK_PROPERTY_INT(CurrentPage)
    };

    class CAscPrinterContextBase : public IMenuEventDataBase
    {
    public:
        CAscPrinterContextBase() : IMenuEventDataBase() {}
        virtual ~CAscPrinterContextBase() {}

        virtual void GetLogicalDPI(int& nDpiX, int& nDpiY) = 0;
        virtual void GetPhysicalRect(int& nX, int& nY, int& nW, int& nH) = 0;
        virtual void GetPrintAreaSize(int& nW, int& nH) = 0;
        virtual void BitBlt(unsigned char* pBGRA, const int& nRasterX, const int& nRasterY, const int& nRasterW, const int& nRasterH,
                            const double& x, const double& y, const double& w, const double& h, const double& dAngle) = 0;
    };

    class CAscPrintPage : public IMenuEventDataBase
    {
    private:
        CAscPrinterContextBase* m_pContext;
        int                     m_nPage;

    public:
        CAscPrintPage()
        {
            m_pContext = NULL;
            m_nPage = 0;
        }
        virtual ~CAscPrintPage()
        {
            if (NULL != m_pContext)
                m_pContext->Release();
        }

        LINK_PROPERTY_INT(Page)

        inline CAscPrinterContextBase* get_Context()
        {
            return m_pContext;
        }
        inline void put_Context(CAscPrinterContextBase* pContext)
        {
            m_pContext = pContext;
        }
    };

    class CAscEditorScript : public IMenuEventDataBase
    {
    private:
        std::wstring m_sUrl;
        std::wstring m_sDestination;
        int m_nId;
        int m_nFrameId;

    public:
        CAscEditorScript()
        {
            m_nId = -1;
            m_nFrameId = -1;
        }
        virtual ~CAscEditorScript()
        {
        }

        LINK_PROPERTY_INT(Id)
        LINK_PROPERTY_STRING(Url)
        LINK_PROPERTY_STRING(Destination)
        LINK_PROPERTY_INT(FrameId)
    };
}

namespace NSEditorApi
{
    class CAscSaveDialog : public IMenuEventDataBase
    {
    private:
        std::wstring    m_sFilePath;
        int             m_nId;

    public:
        CAscSaveDialog()
        {
            m_nId           = -1;
        }
        virtual ~CAscSaveDialog()
        {
        }

        LINK_PROPERTY_STRING(FilePath)
        LINK_PROPERTY_INT(Id)
    };
}

namespace NSEditorApi
{
    class CAscKeyboardDown : public IMenuEventDataBase
    {
    private:
        int m_nKeyCode;
        bool m_bIsCtrl;
        bool m_bIsShift;
        bool m_bIsAlt;

    public:
        CAscKeyboardDown()
        {
            m_nKeyCode  = -1;
            m_bIsCtrl   = false;
            m_bIsShift  = false;
            m_bIsAlt    = false;
        }
        virtual ~CAscKeyboardDown()
        {
        }

        LINK_PROPERTY_INT(KeyCode)
        LINK_PROPERTY_BOOL(IsCtrl)
        LINK_PROPERTY_BOOL(IsShift)
        LINK_PROPERTY_BOOL(IsAlt)
    };
}

#endif // APPLICATION_MANAGER_EVENTS_H
