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
#include "Xlsb.h"
#include "../XlsxFormat/SharedStrings/SharedStrings.h"
#include "../XlsxFormat/Styles/Styles.h"
#include "../XlsxFormat/Worksheets/Worksheet.h"
#include "../XlsxFormat/Comments/Comments.h"

#include "../../../../DesktopEditor/common/SystemUtils.h"

OOX::Spreadsheet::CXlsb::~CXlsb()
{

}	

bool OOX::Spreadsheet::CXlsb::ReadBin(const CPath& oFilePath, XLS::BaseObject* objStream)
{
    NSFile::CFileBinary oFile;
    if (oFile.OpenFile(oFilePath.GetPath()) == false)
        return false;

    auto m_lStreamLen = (LONG)oFile.GetFileSize();
    auto m_pStream = new BYTE[m_lStreamLen];
    DWORD dwRead = 0;
    oFile.ReadFile(m_pStream, (DWORD)m_lStreamLen, dwRead);
    oFile.CloseFile();

    m_binaryReader->Init(m_pStream, 0, dwRead);

    XLS::StreamCacheReaderPtr reader(new XLS::BinaryStreamCacheReader(m_binaryReader, xls_global_info));
    XLS::BinReaderProcessor proc(reader, objStream, true);
    proc.mandatory(*objStream);
    return true;
}

void OOX::Spreadsheet::CXlsb::PrepareSi()
{
    if(m_pStyles && m_pStyles->m_oFonts.IsInit())
    {
        auto lambdaSi = [&](OOX::Spreadsheet::CSi* si) {
            for(size_t i = 0, length = si->m_arrItems.size(); i < length; ++i)
            {
                OOX::Spreadsheet::WritingElement* we = si->m_arrItems[i];
                if(OOX::et_x_r == we->getType())
                {
                    OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(we);
                    if(pRun->m_oRPr.IsInit() && pRun->m_oRPr->m_nFontIndex.IsInit())
                    {
                        CFont* font = nullptr;
                        auto findFont = m_pStyles->m_oFonts->m_mapFonts.find(pRun->m_oRPr->m_nFontIndex->GetValue());
                        if(findFont != m_pStyles->m_oFonts->m_mapFonts.end())
                            font = findFont->second;

                        if(font != nullptr)
                            pRun->m_oRPr->fromFont(font);
                    }
                }
            }
        };
        if(m_pSharedStrings)
        {
            for(auto &si : m_pSharedStrings->m_arrItems)
            {
                lambdaSi(si);
            }
        }

        /*if(!m_arWorksheets.empty())
        {
            for(auto &wsheet : m_arWorksheets)
            {
                for(auto &comment : wsheet->m_mapComments)
                {
                    auto si = comment.second->m_oText;
                    lambdaSi(si.GetPointer());
                }
            }
        }*/
    }
}


