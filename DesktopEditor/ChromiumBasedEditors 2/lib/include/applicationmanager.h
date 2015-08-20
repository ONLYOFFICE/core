#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

#include <string>
#include <vector>
#include "../../../common/File.h"
#include "./applicationmanager_events.h"

#include "./keyboardchecker.h"
#include "./spellchecker.h"
#include "./cefview.h"
#include "./cefapplication.h"

namespace NSCommon
{
    static void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
    {
        size_t posn = 0;
        while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }
}

class CAscApplicationSettings
{
public:
    std::wstring                    spell_dictionaries_path;
    std::wstring                    cache_path;
    std::wstring                    cookie_path;

    bool                            use_system_fonts;
    std::vector<std::wstring>       additional_fonts_folder;
    std::wstring                    fonts_cache_info_path;

    std::wstring                    app_data_path;

public:
    CAscApplicationSettings()
    {
        std::wstring sApplicationPath   = NSFile::GetProcessDirectory();
        NSCommon::string_replace(sApplicationPath, L"\\", L"/");

        app_data_path                   = sApplicationPath;

        spell_dictionaries_path         = sApplicationPath + L"/Dictionaries";
        cache_path                      = sApplicationPath + L"/data/cache";
        cookie_path                     = sApplicationPath + L"/data";

        use_system_fonts                = true;
        fonts_cache_info_path           = app_data_path + L"/webdata/cloud/fonts";
    }

    void SetUserDataPath(std::wstring sPath)
    {
        app_data_path                   = sPath;
        NSCommon::string_replace(app_data_path, L"\\", L"/");

        cache_path                      = app_data_path + L"/data/cache";
        cookie_path                     = app_data_path + L"/data";

        use_system_fonts                = true;
        fonts_cache_info_path           = app_data_path + L"/webdata/cloud/fonts";
    }
};

class CAscPrintSettings
{
public:
    enum PrintMode
    {
        pmFit       = 0,    // вписывать по аспекту
        pmStretch   = 1,    // растягивать
        pm100       = 2     // печатать как есть
    };

public:
    PrintMode   Mode;
    int         WidthPix;
    int         HeightPix;
    bool        ZoomEnable;
    bool        RotateEnable;
    bool        PrintableArea;

public:

    CAscPrintSettings()
    {
        Mode = pm100;
        WidthPix = -1;
        HeightPix = -1;
        ZoomEnable = false;
        RotateEnable = true;
        PrintableArea = false;
    }
};

class CAscApplicationManager_Private;
class CApplicationFonts;
class Q_DECL_EXPORT CAscApplicationManager
{
public:
    CAscApplicationManager();
    ~CAscApplicationManager();

public:
    CAscApplicationSettings m_oSettings;
    CAscPrintSettings m_oPrintSettings;

public:

    void StartSpellChecker();
    void StopSpellChecker();
    void SpellCheck(const int& nEditorId, const std::string& sTask, int nId);

    void CheckKeyboard();
    void SendKeyboardAttack();

    void CheckFonts(bool bAsync = true);

    void SetEventListener(NSEditorApi::CAscMenuEventListener* );
    NSEditorApi::CAscMenuEventListener* GetEventListener();

    void Apply(NSEditorApi::CAscMenuEvent* );
    NSEditorApi::CAscMenuEvent* ApplySync(NSEditorApi::CAscMenuEvent* );

    CCefView* CreateCefView(CCefViewWidgetImpl* parent);
    CCefViewEditor* CreateCefEditor(CCefViewWidgetImpl* parent);

    CCefView* GetViewById(int nId);
    CCefView* GetViewByUrl(const std::wstring& url);

    void Logout(std::wstring strUrl);
    void CancelDownload(int nId);

    void DestroyCefView(int nId);

    void LockCS(int nId);
    void UnlockCS(int nId);

    bool IsInitFonts();

    CApplicationCEF* GetApplication();
    void SetApplication(CApplicationCEF* );

    void SetDebugInfoSupport(bool bIsSupport);
    bool GetDebugInfoSupport();

    void CloseApplication();
    CApplicationFonts* GetApplicationFonts();

    virtual void StartSaveDialog(const std::wstring& sName);
    virtual void EndSaveDialog(const std::wstring& sPath);

    virtual int GetPlatformKeyboardLayout();

protected:
    CAscApplicationManager_Private* m_pInternal;    
};

#endif // APPLICATION_MANAGER_H
