/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "../DocxFormat/App.h"
#include "../DocxFormat/Core.h"

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
#include "../XlsxFormat/Styles/Fonts.h"

#include "../../MsBinaryFile/XlsFile/Format/Logic/GlobalWorkbookInfo.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/WorkbookStreamObject.h"
#include "../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheReader.h"
#include "../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheWriter.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/BinProcessor.h"

#include "../Binary/Presentation/BinaryFileReaderWriter.h"

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
	m_binaryWriter = boost::shared_ptr<NSBinPptxRW::CXlsbBinaryWriter>(new NSBinPptxRW::CXlsbBinaryWriter);
	m_bWriteToXlsx = false;
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
XLS::StreamCacheReaderPtr OOX::Spreadsheet::CXlsb::GetFileReader(const CPath& oFilePath, BYTE* &streamBuf)
{
    NSFile::CFileBinary oFile;
    if (oFile.OpenFile(oFilePath.GetPath()) == false)
        return nullptr;

    auto m_lStreamLen = (LONG)oFile.GetFileSize();
    streamBuf = new BYTE[m_lStreamLen];
    DWORD dwRead = 0;
    oFile.ReadFile(streamBuf, (DWORD)m_lStreamLen, dwRead);
    oFile.CloseFile();

    m_binaryReader->Init(streamBuf, 0, dwRead);

    XLS::StreamCacheReaderPtr reader(new XLS::BinaryStreamCacheReader(m_binaryReader, xls_global_info));
    return reader;
}
XLS::StreamCacheWriterPtr OOX::Spreadsheet::CXlsb::GetFileWriter(const CPath& oFilePath)
{
    if (m_binaryWriter->CreateFileW(oFilePath.GetPath()) == false)
        return nullptr;

    XLS::StreamCacheWriterPtr writer(new XLS::BinaryStreamCacheWriter(m_binaryWriter, xls_global_info));
    return writer;
}
bool OOX::Spreadsheet::CXlsb::WriteSreamCache(XLS::StreamCacheWriterPtr writer)
{
    auto writeSucced = m_binaryWriter->WriteFile(m_binaryWriter->GetBuffer(), (static_cast<NSBinPptxRW::CBinaryFileWriter*>(m_binaryWriter.get()))->GetPosition());
    if(writeSucced)
        (static_cast<NSBinPptxRW::CBinaryFileWriter*>(m_binaryWriter.get()))->SetPosition(0);
    m_binaryWriter->CloseFile();
    return writeSucced;
}
bool OOX::Spreadsheet::CXlsb::WriteBin(const CPath& oDirPath, OOX::CContentTypes& oContentTypes)
{
    if (NULL == m_pWorkbook)
        return false;

    m_bWriteToXlsb = true;
    if(!m_oContentTypes.m_mapDefaults.empty() && !m_oContentTypes.m_mapOverrides.empty())
    {
        oContentTypes.Merge(&m_oContentTypes);
    }

    IFileContainer::Write(oDirPath / L"", OOX::CPath(_T("")), oContentTypes);

    oContentTypes.Write(oDirPath);
    return true;
}
bool OOX::Spreadsheet::CXlsb::WriteBin(const CPath& oFilePath, XLS::BaseObject* objStream)
{
	if (m_binaryWriter->CreateFileW(oFilePath.GetPath()) == false)
		return false;

	XLS::StreamCacheWriterPtr writer(new XLS::BinaryStreamCacheWriter(m_binaryWriter, xls_global_info));
	XLS::BinWriterProcessor proc(writer, objStream);
	proc.mandatory(*objStream);

    auto writeSucced = m_binaryWriter->WriteFile(m_binaryWriter->GetBuffer(), (static_cast<NSBinPptxRW::CBinaryFileWriter*>(m_binaryWriter.get()))->GetPosition());
    if(writeSucced)
        (static_cast<NSBinPptxRW::CBinaryFileWriter*>(m_binaryWriter.get()))->SetPosition(0);
	m_binaryWriter->CloseFile();

	return true;
}

void OOX::Spreadsheet::CXlsb::WriteSheetData()
{
    for(auto &worksheet : m_arWorksheets)
    {

        //для оптимизации по памяти сразу записываем в файл все листы
        if(m_bWriteToXlsb)
        {
            WriteSheet(worksheet);
        }//

        //cell_table_temlate.reset();
        //reader.reset();
    }
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

//подготовка шрифтов в richString для конвертации в xlsb
void OOX::Spreadsheet::CXlsb::PrepareRichStr()
{
    if(m_pStyles && m_pStyles->m_oFonts.IsInit())
    {
        auto lambdaSi = [&](OOX::Spreadsheet::CSi* si) 
        {
            for(size_t i = 0, length = si->m_arrItems.size(); i < length; ++i)
            {
                OOX::Spreadsheet::WritingElement* we = si->m_arrItems[i];
                if(OOX::et_x_r == we->getType())
                {
                    OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(we);
                    if(pRun->m_oRPr.IsInit() && !pRun->m_oRPr->m_nFontIndex.IsInit())
                    {
                        auto font = pRun->m_oRPr->toFont();
                        m_pStyles->m_oFonts->AddFont(font);
                        pRun->m_oRPr->m_nFontIndex.Init();
                        pRun->m_oRPr->m_nFontIndex = m_pStyles->m_oFonts->m_arrItems.size() - 1;

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
    }
}
//отложенный парсинг SheetData
void OOX::Spreadsheet::CXlsb::ReadSheetData()
{
    for(auto &worksheet : m_arWorksheets)
    {
		_UINT32 dataPosition;
		auto dataFindPair = m_mapSheetNameSheetData.find(worksheet->GetReadPath().GetPath());

		if(dataFindPair != m_mapSheetNameSheetData.end())
			dataPosition = dataFindPair->second;
		else
			continue;

        NSFile::CFileBinary oFile;
        if (oFile.OpenFile(worksheet->GetReadPath().GetPath()) == false)
            continue;

        auto m_lStreamLen = (LONG)oFile.GetFileSize();
        if(dataPosition > m_lStreamLen)
            continue;
        auto m_pStream = new BYTE[m_lStreamLen];
        DWORD dwRead = 0;
        oFile.ReadFile(m_pStream, (DWORD)m_lStreamLen, dwRead);
        oFile.CloseFile();

        m_binaryReader->Init(m_pStream, 0, dwRead);

        XLS::BaseObjectPtr cell_table_temlate = XLS::BaseObjectPtr(new XLSB::CELLTABLE());

        XLS::StreamCacheReaderPtr reader(new XLS::BinaryStreamCacheReader(m_binaryReader, xls_global_info));
        //XLS::BinReaderProcessor proc(reader, cell_table_temlate.get(), true);

        reader->SetRecordPosition(dataPosition);

        //proc.mandatory(*cell_table_temlate.get());


        //auto base = boost::static_pointer_cast<BaseObject>(cell_table_temlate);
        worksheet->m_oSheetData->fromBin(reader);
        delete[] m_pStream;
        //для оптимизации по памяти сразу записываем в файл все листы
        if(m_bWriteToXlsx)
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
bool OOX::Spreadsheet::CXlsb::IsWriteToXlsx()
{
	return m_bWriteToXlsx;
}
void OOX::Spreadsheet::CXlsb::WriteToXlsx(bool isXlsx)
{
	m_bWriteToXlsx = isXlsx;
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
    auto lambdaFormula = [&](std::wstring& formula) {
            auto str = STR::guidFromStr(formula);
            while(!str.empty())
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
                str = STR::guidFromStr(formula);
            }
        };
    for(auto &worksheet : m_arWorksheets)
    {
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
    if(m_pWorkbook && m_pWorkbook->m_oDefinedNames.IsInit())
    {
        for(auto defName:m_pWorkbook->m_oDefinedNames->m_arrItems)
        {
            if(defName->m_oRef.IsInit())
            {
                lambdaFormula(defName->m_oRef.get());
            }
        }
    }
}

void OOX::Spreadsheet::CXlsb::LinkTables()
{
    {
        bool tablesExist = false;
        for(auto worksheet:m_arWorksheets)
        {
            if(worksheet->m_oTableParts.IsInit())
            tablesExist = true;
        }
        if(!tablesExist)
            return;
    }
    for(auto xti:XLS::GlobalWorkbookInfo::arXti_External_static)
    {
        if(xti.itabFirst != xti.itabLast)
        {
            continue;
        }
        auto sheetName = xti.link;
        if(!m_pWorkbook || !m_pWorkbook->m_oSheets.IsInit())
            continue;
        OOX::Spreadsheet::CSheet * bundle;
        for(auto i:m_pWorkbook->m_oSheets->m_arrItems)
        {
            if(i->m_oName.IsInit() && i->m_oName.get() == sheetName)
            {
                bundle = i;
            }
        }
        if(!bundle || !bundle->m_oRid.IsInit())
            continue;
        auto FilePtr =  m_pWorkbook->Find(bundle->m_oRid->GetValue());
        if(!FilePtr.IsInit() || !(OOX::Spreadsheet::FileTypes::Worksheet == FilePtr->type()))
            continue;
        auto WorksheetFile = static_cast<OOX::Spreadsheet::CWorksheet*>(FilePtr.GetPointer());
        if(!WorksheetFile->m_oTableParts.IsInit())
            continue;
        for(auto tablePart : WorksheetFile->m_oTableParts->m_arrItems)
        {
            if(tablePart->m_oRId.IsInit())
            {
                auto tableFilePtr = WorksheetFile->Find(tablePart->m_oRId->GetValue());
                if(tableFilePtr.IsInit() && OOX::Spreadsheet::FileTypes::Table == tableFilePtr->type())
                {
                    auto tableFile = static_cast<OOX::Spreadsheet::CTableFile*>(tableFilePtr.GetPointer());
                    if(tableFile->m_oTable.IsInit() && tableFile->m_oTable->m_oId.IsInit())
                    {
                        if(!XLS::GlobalWorkbookInfo::mapXtiTables_static.count(xti.itabFirst))
                        {
                            XLS::GlobalWorkbookInfo::mapXtiTables_static.emplace(xti.itabFirst, std::vector<int>());
                        }
                        XLS::GlobalWorkbookInfo::mapXtiTables_static.at(xti.itabFirst).push_back(tableFile->m_oTable->m_oId->GetValue());

                    }
                }
            }
        }
    }
}







