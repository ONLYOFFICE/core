/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _BUILD_APPLICATIONFONTSWORKER_H_
#define _BUILD_APPLICATIONFONTSWORKER_H_

#include <string>
#include <vector>
#include "../graphics/pro/Fonts.h"

#define ONLYOFFICE_FONTS_VERSION     10
#define ONLYOFFICE_ALL_FONTS_VERSION 2

class CApplicationFontsWorkerBreaker
{
public:
    virtual bool IsFontsWorkerRunned() { return true; }
};

class CApplicationFontsWorker_private;
class GRAPHICS_DECL CApplicationFontsWorker
{
public:
    // использовать ли системные шрифты
    bool                        m_bIsUseSystemFonts;
    // дополнительные папки с шрифтами
    std::vector<std::wstring>   m_arAdditionalFolders;

    // рабоча директория (сюда скидываем все артефакты)
    std::wstring                m_sDirectory;

    // поддерживать ли opentype шрифты
    bool                        m_bIsUseOpenType;

    // поддерживать ли все версии AllFonts.js
    bool                        m_bIsUseAllVersions;

    // нужны ли табнейлы
    bool                        m_bIsNeedThumbnails;
    // какой-то табнейл уже есть - нудно ли все равно удалить и перегенерировать?
    bool                        m_bIsRemoveOldThumbnails;
    // генерим табнейлы отдельно
    bool                        m_bSeparateThumbnails;
    // какие масштабы нужны для табнейлов
    std::vector<double>         m_arThumbnailsScales;
    bool                        m_bIsGenerateThumbnailsEA;

    // WEB версия ----
    std::wstring                m_sThumbnailsDirectory;

    std::wstring                m_sAllFontsJSPath;
    std::wstring                m_sWebAllFontsJSPath;

    std::wstring                m_sWebFontsDirectory;
    // ---------------

    bool                        m_bIsCleanDirectory;

private:
    CApplicationFontsWorker_private* m_pInternal;

public:
    CApplicationFontsWorker();
    ~CApplicationFontsWorker();
    
    NSFonts::IApplicationFonts* Check();
    void CheckThumbnails();

    void SetBreaker(CApplicationFontsWorkerBreaker* pChecker);

    std::string GetAllFonts();

    static std::vector<std::wstring> GetFontNames(NSFonts::IApplicationFonts* pFonts);
    static std::vector<std::wstring> GetFontNamesWithExcludes(NSFonts::IApplicationFonts* pFonts, std::vector<std::wstring> arExcludes);
};

#endif // _BUILD_APPLICATIONFONTSWORKER_H_
