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

#include "../XlsxFormat/Workbook/Workbook.h"
#include "../XlsxFormat/SharedStrings/SharedStrings.h"
#include "../XlsxFormat/Styles/Styles.h"
#include "../XlsxFormat/Worksheets/Worksheet.h"
#include "../XlsxFormat/CalcChain/CalcChain.h"
#include "../XlsxFormat/ExternalLinks/ExternalLinks.h"
#include "../XlsxFormat/ExternalLinks/ExternalLinkPath.h"
#include "../XlsxFormat/Pivot/PivotTable.h"
#include "../XlsxFormat/Pivot/PivotCacheDefinition.h"
#include "../XlsxFormat/Pivot/PivotCacheRecords.h"

#include "../../MsBinaryFile/XlsFile/Format/Logic/GlobalWorkbookInfo.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/WorkbookStreamObject.h"
#include "../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheReader.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/BinProcessor.h"

#include "../../DesktopEditor/common/SystemUtils.h"

#include "Biff12_unions/CELLTABLE.h"

using namespace XLS;

OOX::Spreadsheet::CXlsb::~CXlsb()
{

}	
void OOX::Spreadsheet::CXlsb::init()
{
	workbook_code_page = XLS::WorkbookStreamObject::DefaultCodePage;
	xls_global_info = boost::shared_ptr<XLS::GlobalWorkbookInfo>(new XLS::GlobalWorkbookInfo(workbook_code_page, nullptr));
	xls_global_info->Version = 0x0800;    
    m_binaryReader = boost::shared_ptr<NSBinPptxRW::CBinaryFileReader>(new NSBinPptxRW::CBinaryFileReader);
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

    delete[] m_pStream;
    //reader.reset();

    return true;
}

XLS::GlobalWorkbookInfo* OOX::Spreadsheet::CXlsb::GetGlobalinfo()
{
    return xls_global_info.get();
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
//отложенный парсинг SheetData
void OOX::Spreadsheet::CXlsb::ReadSheetData(bool isWriteSheetToXlsx)
{
    for(auto &worksheet : m_arWorksheets)
    {
        auto dataPosition = m_mapSheetNameSheetData.find(worksheet->GetReadPath().GetPath())->second;

        NSFile::CFileBinary oFile;
        if (oFile.OpenFile(worksheet->GetReadPath().GetPath()) == false)
            continue;

        auto m_lStreamLen = (LONG)oFile.GetFileSize();
        auto m_pStream = new BYTE[m_lStreamLen];
        DWORD dwRead = 0;
        oFile.ReadFile(m_pStream, (DWORD)m_lStreamLen, dwRead);
        oFile.CloseFile();

        m_binaryReader->Init(m_pStream, 0, dwRead);

        XLS::BaseObjectPtr cell_table_temlate = XLS::BaseObjectPtr(new XLSB::CELLTABLE());

        XLS::StreamCacheReaderPtr reader(new XLS::BinaryStreamCacheReader(m_binaryReader, xls_global_info));
        XLS::BinReaderProcessor proc(reader, cell_table_temlate.get(), true);

        proc.SetRecordPosition(dataPosition);

        proc.mandatory(*cell_table_temlate.get());
        delete[] m_pStream;

        //auto base = boost::static_pointer_cast<BaseObject>(cell_table_temlate);
        worksheet->m_oSheetData->fromBin(cell_table_temlate);

        //для оптимизации по памяти сразу записываем в файл все листы
        if(isWriteSheetToXlsx)
        {
            WriteSheet(worksheet);
        }

        //cell_table_temlate.reset();
        //reader.reset();
    }
}
void OOX::Spreadsheet::CXlsb::SetPropForWriteSheet(const std::wstring &sPath, OOX::CContentTypes& oContentTypes)
{
    m_sPath          = sPath + L"/xl";
    m_oContentTypes = oContentTypes;
}
void OOX::Spreadsheet::CXlsb::WriteSheet(CWorksheet* worksheet)
{
    OOX::CPath oDefDir = worksheet->DefaultDirectory();
    OOX::CPath oName   = worksheet->DefaultFileName();

    if (false == worksheet->m_sOutputFilename.empty())
        oName.SetName(worksheet->m_sOutputFilename, false);

    OOX::CSystemUtility::CreateDirectories(m_sPath);
    OOX::CSystemUtility::CreateDirectories( m_sPath / oDefDir );
    worksheet->write( m_sPath / oDefDir / oName, L"/xl/" + oDefDir, m_oContentTypes );
    worksheet->m_oSheetData.reset();
}
void OOX::Spreadsheet::CXlsb::PrepareTableFormula()
{
    for(auto &worksheet : m_arWorksheets)
    {
        auto lambdaFormula = [&](std::wstring& formula) {
            auto str = STR::guidFromStr(formula);
            if(!str.empty())
            {
                auto guidTableIndex = this->xls_global_info->mapTableGuidsIndex.find(str);
                if (guidTableIndex != this->xls_global_info->mapTableGuidsIndex.end())
                {
                    auto tableIndex = this->xls_global_info->mapTableNames.find(guidTableIndex->second);
                    if (tableIndex != this->xls_global_info->mapTableNames.end())
                    {
                        auto tableName = tableIndex->second;
                        formula.replace(formula.find(str), str.size(), tableName);
                    }
                }
            }
        };

        if(worksheet->m_oTableParts.IsInit())
        {
            for(size_t i = 0, length = worksheet->m_oTableParts->m_arrItems.size(); i < length; ++i)
            {
                auto &oTablePart = worksheet->m_oTableParts->m_arrItems[i];
                if(oTablePart->m_oRId.IsInit())
                {
                    smart_ptr<OOX::File> pFile = worksheet->Find(OOX::RId(oTablePart->m_oRId->GetValue()));
                    if (pFile.IsInit() && OOX::Spreadsheet::FileTypes::Table == pFile->type())
                    {
                        OOX::Spreadsheet::CTableFile* pTableFile = static_cast<OOX::Spreadsheet::CTableFile*>(pFile.GetPointer());
                        if(pTableFile && pTableFile->m_oTable.IsInit())
                        {
                            OOX::Spreadsheet::CTable* pTable = pTableFile->m_oTable.GetPointer();
                            if(pTable->m_oTableColumns.IsInit())
                            {
                                OOX::Spreadsheet::CTableColumns* oTableColumns = pTable->m_oTableColumns.GetPointer();

                                for(size_t i = 0, length = oTableColumns->m_arrItems.size(); i < length; ++i)
                                {
                                    auto& oTableColumn = oTableColumns->m_arrItems[i];                                   

                                    if(oTableColumn->m_oCalculatedColumnFormula.IsInit())
                                    {                                       
                                       lambdaFormula(oTableColumn->m_oCalculatedColumnFormula.get());
                                    }
                                    if(oTableColumn->m_oTotalsRowFormula.IsInit())
                                    {
                                        lambdaFormula(oTableColumn->m_oTotalsRowFormula.get());
                                    }
                                }
                            }
                        }
                    }
                }
            }

           /*if(worksheet->m_oSheetData.IsInit())
            {
                for(size_t i = 0, length = worksheet->m_oSheetData->m_arrItems.size(); i < length; ++i)
                {
                    OOX::Spreadsheet::CRow* pRow = worksheet->m_oSheetData->m_arrItems[i];
                    if(pRow)
                    {
                        for(size_t i = 0, length = pRow->m_arrItems.size(); i < length; ++i)
                        {
                            OOX::Spreadsheet::CCell* oCell = pRow->m_arrItems[i];
                            if(oCell->m_oFormula.IsInit())
                            {
                                lambdaFormula(oCell->m_oFormula.GetPointer()->m_sText);
                            }
                        }
                    }
                }

            }*/
        }
    }
}






