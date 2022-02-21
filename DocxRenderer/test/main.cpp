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

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"

#include "../../PdfReader/PdfReader.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../DocxRenderer.h"
#include "../../Common/OfficeFileFormatChecker.h"

int main(int argc, char *argv[])
{
    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;

    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pFonts = oWorker.Check();

    std::wstring sTempDir = NSFile::GetProcessDirectory() + L"/temp";
    std::wstring sTempDirOut = NSFile::GetProcessDirectory() + L"/temp/output";

    if (!NSDirectory::Exists(sTempDir))
        NSDirectory::CreateDirectory(sTempDir);
    if (!NSDirectory::Exists(sTempDirOut))
        NSDirectory::CreateDirectory(sTempDirOut);

    std::wstring sSourceFile = L"PATH_TO_TEST_FILE";
    std::wstring sDestFile = NSFile::GetProcessDirectory() + L"/output.docx";

    IOfficeDrawingFile* pReader = NULL;

    COfficeFileFormatChecker oChecker;
    if (oChecker.isOfficeFile(sSourceFile))
    {
        switch (oChecker.nFileType)
        {
        case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
            pReader = new PdfReader::CPdfReader(pFonts);
            break;
        case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS:
            pReader = new CXpsFile(pFonts);
            break;
        case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU:
            pReader = new CDjVuFile(pFonts);
            break;
        default:
            break;
        }
    }

    if (!pReader)
    {
        pFonts->Release();
        return 0;
    }

    pReader->SetTempDirectory(sTempDir);
    pReader->LoadFromFile(sSourceFile);

    CDocxRenderer oDocxRenderer(pFonts);

    // проверить все режимы
    NSDocxRenderer::TextAssociationType taType;
    //taType = NSDocxRenderer::TextAssociationTypeBlockChar;
    //taType = NSDocxRenderer::TextAssociationTypeBlockLine;
    //taType = NSDocxRenderer::TextAssociationTypePlainLine;
    taType = NSDocxRenderer::TextAssociationTypePlainParagraph;
    oDocxRenderer.SetTextAssociationType(taType);

    oDocxRenderer.SetTempFolder(sTempDirOut);
    oDocxRenderer.Convert(pReader, sDestFile);

    delete pReader;
    pFonts->Release();
    return 0;
}
