#ifndef CEF_COOKIES_WORKER_H
#define CEF_COOKIES_WORKER_H

#include "include/base/cef_bind.h"
#include "include/cef_cookie.h"
#include "include/wrapper/cef_closure_task.h"

class CCookieFoundCallback
{
public:
    virtual void OnFoundCookie(bool bIsPresent, std::string sValue)     = 0;
    virtual void OnSetCookie()                                          = 0;
    virtual void OnDeleteCookie(bool bIsPresent)                        = 0;
};

class CCefCookieVisitor : public CefCookieVisitor
{
public:
    std::map<std::string, std::string>  m_cookies;

    std::string                         m_sDomain;
    std::string                         m_sCookieSearch;

    CCookieFoundCallback*               m_pCallback;

    bool                                m_bIsDelete;
    bool                                m_bIsDeleted;

    CCefCookieVisitor()
    {
        this->AddRef();
        m_pCallback = NULL;
        m_bIsDelete = false;
        m_bIsDeleted = false;
    }
    ~CCefCookieVisitor()
    {
        if (m_pCallback)
        {
            if (!m_bIsDelete)
            {
                std::map<std::string, std::string>::iterator i = m_cookies.find(m_sCookieSearch);

                if (i != m_cookies.end())
                {
                    m_pCallback->OnFoundCookie(true, i->second);
                }
                else
                {
                    m_pCallback->OnFoundCookie(false, "");
                }
            }
            else
            {
                m_pCallback->OnDeleteCookie(m_bIsDeleted);
            }
        }
    }

    virtual bool Visit(const CefCookie& cookie, int count, int total, bool& deleteCookie)
    {
        std::string sDomain = CefString(&cookie.domain).ToString();

        if (sDomain.find(m_sDomain) != std::string::npos)
        {
            std::string sName = CefString(&cookie.name).ToString();
            std::string sValue = CefString(&cookie.value).ToString();

            m_cookies.insert(std::pair<std::string, std::string>(sName, sValue));

            if (m_bIsDelete)
            {
                m_bIsDeleted = true;
                deleteCookie = true;
            }
        }

        return true;
    }

    void CheckCookiePresent(CefRefPtr<CefCookieManager> manager)
    {
        if (!CefCurrentlyOn(TID_IO))
        {
            CefPostTask(TID_IO,
                base::Bind(&CCefCookieVisitor::CheckCookiePresent, this, manager));
            return;
        }

        manager->VisitAllCookies(this);
        this->Release();
    }

public:
    IMPLEMENT_REFCOUNTING(CCefCookieVisitor);
};

class CCefCookieSetter
{
public:
    std::string                         m_sUrl;
    std::string                         m_sDomain;
    std::string                         m_sPath;
    std::string                         m_sCookieKey;
    std::string                         m_sCookieValue;

    CCookieFoundCallback*               m_pCallback;

    CCefCookieSetter()
    {
        this->AddRef();
    }
    ~CCefCookieSetter()
    {
    }

    void SetCookie(CefRefPtr<CefCookieManager> manager)
    {
        if (!CefCurrentlyOn(TID_IO))
        {
            CefPostTask(TID_IO,
                base::Bind(&CCefCookieSetter::SetCookie, this, manager));
            return;
        }

        CefCookie authorization;

    #if 0
        CefString(&authorization.name).FromString("asc_auth_key");
        CefString(&authorization.value).FromString("mNRRcfOz5/kpQs8+H+ImHZrXrz+ByxHpUdf44jektpNgBCuUCcEHB8CEB6TRJTYCqsN5Ag0UlmS+cyj7G838uRoffSe44N/PW4PYIItmSl7HI8lh3nEmmtYjwRWKFDYi");

        CefString(&authorization.domain).FromString("ascdesktop.teamlab.info");
        CefString(&authorization.path).FromString("/");
    #else
        CefString(&authorization.name).FromString(m_sCookieKey);
        CefString(&authorization.value).FromString(m_sCookieValue);

        CefString(&authorization.domain).FromString(m_sDomain);
        CefString(&authorization.path).FromString(m_sPath);
    #endif

        authorization.httponly = 0;
        authorization.secure = 0;

        authorization.has_expires = true;
        authorization.expires.year = 2200;
        authorization.expires.month = 4;
        authorization.expires.day_of_week = 5;
        authorization.expires.day_of_month = 11;

        bool bIsAddedCookie = manager->SetCookie(m_sUrl, authorization, NULL);

        m_pCallback->OnSetCookie();

        this->Release();
    }

public:
    IMPLEMENT_REFCOUNTING(CCefCookieSetter);
};

#endif // CEF_COOKIES_WORKER_H
