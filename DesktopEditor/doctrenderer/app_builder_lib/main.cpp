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

#include "./../common_deploy.h"
#include "../docbuilder.h"
#include "../../common/File.h"

using namespace NSDoctRenderer;
int main(int argc, char *argv[])
{
    if (argc <= 0)
        return 0;

    std::wstring sProcessDirectory = NSFile::GetProcessDirectory();
    // для дебага БЕЗ x2t - подкидываем билдер в папку builder к тестовому примеру
    std::wstring sWorkDirectory = sProcessDirectory + L"/builder";

    CDocBuilder oBuilder;
    oBuilder.SetProperty("--work-directory", sWorkDirectory.c_str());

    oBuilder.CreateFile(OFFICESTUDIO_FILE_DOCUMENT_DOCX);

    if (true)
    {
        CContext oContext = oBuilder.GetContext();
        CContextScope oScope = oContext.CreateScope();

        CValue oGlobal = oContext.GetGlobal();

        CValue oApi = oGlobal[L"Api"];
        CValue oDocument = oApi.Call(L"GetDocument");
        CValue oParagraph = oApi.Call(L"CreateParagraph");
        oParagraph.Call(L"AddText", "Hello, world!");
        CValue oContent = oContext.CreateArray(1);
        oContent.Set(0, oParagraph);
        oDocument.Call(L"InsertContent", oContent);
    }

    std::wstring sDstPath = sProcessDirectory + L"/result.docx";
    oBuilder.SaveFile(OFFICESTUDIO_FILE_DOCUMENT_DOCX, sDstPath.c_str());
    oBuilder.CloseFile();

    NSDoctRenderer::CDocBuilder::Dispose();
    return 0;
}
