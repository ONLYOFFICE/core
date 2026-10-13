/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "Xlsx.h"

#include "../DocxFormat/App.h"
#include "../DocxFormat/Core.h"
#include "../DocxFormat/FileTypes.h"

#include "Workbook/Workbook.h"
#include "SharedStrings/SharedStrings.h"
#include "Styles/Styles.h"
#include "Worksheets/Worksheet.h"
#include "CalcChain/CalcChain.h"
#include "Drawing/WpsCellImages.h"
#include "Drawing/FromTo.h"
#include "../PPTXFormat/Logic/Pic.h"
#include "../DocxFormat/Media/Image.h"
#include "ExternalLinks/ExternalLinks.h"
#include "ExternalLinks/ExternalLinkPath.h"
#include "Pivot/PivotTable.h"
#include "Pivot/PivotCacheDefinition.h"
#include "Pivot/PivotCacheRecords.h"

#include "Styles/Borders.h"
#include "Styles/CellStyles.h"
#include "Styles/Xfs.h"
#include "Styles/Colors.h"
#include "Styles/dxf.h"
#include "Styles/Fills.h"
#include "Styles/Fonts.h"
#include "Styles/NumFmts.h"
#include "Styles/TableStyles.h"

#include "ComplexTypes_Spreadsheet.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../Binary/XlsbFormat/FileTypes_SpreadsheetBin.h"

#include "../../MsBinaryFile/XlsFile/Converter/xls_writer.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/WorkbookStreamObject.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/GlobalsSubstream.h"
#include "../../MsBinaryFile/XlsFile/Format/Logic/Biff_unions/PIVOTCACHE.h"

#include <iomanip>
#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>

OOX::Spreadsheet::CXlsx::CXlsx() : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
{
	init();
}
OOX::Spreadsheet::CXlsx::CXlsx(const CPath& oFilePath) : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
{
	init();

	Read( oFilePath );
}
OOX::Spreadsheet::CXlsx::~CXlsx()
{
    if(bDeleteWorkbook)			RELEASEOBJECT(m_pWorkbook);
    if(bDeleteSharedStrings)	RELEASEOBJECT(m_pSharedStrings);
    if(bDeleteStyles)			RELEASEOBJECT(m_pStyles);
    if(bDeleteCalcChain)		RELEASEOBJECT(m_pCalcChain);
	
	for (size_t i = 0; i < m_arWorksheets.size(); ++i)
    {
        if (bDeleteWorksheets && m_arWorksheets[i])
		{
            delete m_arWorksheets[i];
		}
    }
	m_arWorksheets.clear();
	m_mapWorksheets.clear();
}	
void OOX::Spreadsheet::CXlsx::init()
{
    m_bSpreadsheets		= true;

    m_pApp				= NULL;
    m_pCore				= NULL;

    m_pWorkbook			= NULL;
    m_pSharedStrings	= NULL;
    m_pStyles			= NULL;
    m_pCalcChain		= NULL;
    m_pVbaProject		= NULL;
    m_pJsaProject		= NULL;
    m_pWorkbookComments = NULL;
    m_pXlsbWriter       = NULL;
    m_nLastReadRow      = 0;
    m_nLastReadCol      = -1;
    m_bNeedCalcChain    = true;

    bDeleteWorkbook			= false;
    bDeleteSharedStrings	= false;
    bDeleteStyles			= false;
    bDeleteCalcChain		= false;
    bDeleteWorksheets		= false;
    bDeleteVbaProject		= false;
    bDeleteJsaProject		= false;
}
bool OOX::Spreadsheet::CXlsx::ReadNative(const CPath& oFilePath)
{
	m_sDocumentPath = oFilePath.GetPath();

	OOX::CRels oRels(oFilePath / FILE_SEPARATOR_STR);
	// Desktop's no-base64 path normally streams cells straight to the binary
	// writer. These workbooks need the in-memory path so photos can replace the
	// formulas before serialization; ordinary workbooks retain streaming.
	NSBinPptxRW::CXlsbBinaryWriter* streamingWriter = m_pXlsbWriter;
	if (streamingWriter)
	{
		for (auto relation : oRels.m_arRelations)
		{
			if (!relation || relation->IsExternal() || !(relation->Type() == OOX::Spreadsheet::FileTypes::Workbook))
				continue;
			OOX::CRels workbookRels(oFilePath / relation->Filename());
			for (auto workbookRel : workbookRels.m_arRelations)
				if (workbookRel && !workbookRel->IsExternal() && workbookRel->Type() == WpsCellImages::RelationshipType)
					m_pXlsbWriter = NULL;
		}
	}
	IFileContainer::Read(oRels, oFilePath, oFilePath);

	if (m_pWorkbook)
		ImportWpsCellImages(oFilePath);
	m_pXlsbWriter = streamingWriter;
	if (!m_pWorkbook) return false;

	CXlsb* xlsb = dynamic_cast<CXlsb*>(this);
	if (xlsb)
	{
		xlsb->PrepareSi();
		xlsb->PrepareTableFormula();
	}

	return true;
}

void OOX::Spreadsheet::CXlsx::ImportWpsCellImages(const CPath& oRootPath)
{
	if (!m_pWorkbook->m_pCurRels.IsInit() || m_pXlsbWriter)
		return;
	std::wstring packageRoot = oRootPath.GetPath();
	if (!packageRoot.empty() && packageRoot.back() != FILE_SEPARATOR_CHAR)
		packageRoot += FILE_SEPARATOR_STR;

	// Resolve the part through the workbook relationships, not its usual name.
	for (auto relation : m_pWorkbook->m_pCurRels->m_arRelations)
	{
		if (!relation || relation->IsExternal() ||
			relation->Type() != WpsCellImages::RelationshipType)
			continue;
		CPath target = relation->Filename();
		const CPath part = target.GetIsRoot() ? oRootPath / target :
			m_pWorkbook->GetReadPath().GetDirectory() / target;
		if (part.GetPath().compare(0, packageRoot.size(), packageRoot) != 0)
			continue;
		XmlUtils::CXmlNode root;
		if (!root.FromXmlFile(part.GetPath()) || XmlUtils::GetNameNoNS(root.GetName()) != L"cellImages")
			continue;

		// Only embedded package images are imported; never fetch external links.
		OOX::CRels imageRels(part);
		std::map<std::wstring, XmlUtils::CXmlNode> pictures;
		for (auto imageNode : root.ReadNodesNoNS(L"cellImage"))
		{
			auto pic = imageNode.ReadNodeNoNS(L"pic");
			auto nv = pic.ReadNodeNoNS(L"nvPicPr").ReadNodeNoNS(L"cNvPr");
			const std::wstring id = nv.GetAttribute(L"name");
			if (pic.IsValid() && !id.empty())
				pictures.emplace(id, pic);
		}
		if (pictures.empty())
			continue;

		for (auto sheet : m_arWorksheets)
		{
			if (!sheet || !sheet->m_oSheetData.IsInit())
				continue;
			smart_ptr<OOX::File> drawingFile;
			if (sheet->m_oDrawing.IsInit() && sheet->m_oDrawing->m_oId.IsInit())
				drawingFile = sheet->Find(OOX::RId(sheet->m_oDrawing->m_oId->GetValue()));
			auto drawing = drawingFile.smart_dynamic_cast<OOX::Spreadsheet::CDrawing>();
			// Do not replace an existing drawing relationship we could not read.
			if (sheet->m_oDrawing.IsInit() && !drawing.IsInit())
				continue;
			int nextId = 1;
			if (drawing.IsInit())
			{
				// Include all kinds of shapes, not just pictures, in the ID scan.
				XmlUtils::CXmlNode existing;
				if (existing.FromXmlFile(drawing->GetReadPath().GetPath()))
				{
					std::vector<XmlUtils::CXmlNode> pending(1, existing);
					while (!pending.empty())
					{
						auto node = pending.back(); pending.pop_back();
						if (XmlUtils::GetNameNoNS(node.GetName()) == L"cNvPr")
						{
							const int existingId = node.GetAttributeInt(L"id");
							nextId = std::max(nextId, existingId < std::numeric_limits<int>::max() ? existingId + 1 : existingId);
						}
						std::vector<XmlUtils::CXmlNode> children;
						node.GetChilds(children);
						pending.insert(pending.end(), children.begin(), children.end());
					}
				}
				for (auto anchor : drawing->m_arrItems)
					if (anchor && anchor->m_oElement.IsInit() && anchor->m_oElement->is<PPTX::Logic::Pic>())
					{
						const int existingId = anchor->m_oElement->as<PPTX::Logic::Pic>().nvPicPr.cNvPr.id;
						nextId = std::max(nextId, existingId < std::numeric_limits<int>::max() ? existingId + 1 : existingId);
					}
			}
			for (auto row : sheet->m_oSheetData->m_arrItems)
			{
				if (!row) continue;
				for (auto cell : row->m_arrItems)
				{
					std::wstring id;
					if (nextId == std::numeric_limits<int>::max() || !cell || !cell->m_oFormula.IsInit() ||
						!WpsCellImages::GetImageId(cell->m_oFormula->m_sText, id))
						continue;
					auto found = pictures.find(id);
					if (found == pictures.end()) continue;
					std::unique_ptr<PPTX::Logic::Pic> pic(new PPTX::Logic::Pic(found->second));
					if (!pic->blipFill.blip.IsInit() || !pic->blipFill.blip->embed.IsInit())
						continue;
					OOX::Rels::CRelationShip* imageRel = NULL;
					imageRels.GetRel(*pic->blipFill.blip->embed, &imageRel);
					if (!imageRel || imageRel->IsExternal() || !(imageRel->Type() == OOX::FileTypes::Image))
						continue;
					CPath imageTarget = imageRel->Filename();
					const CPath imagePath = imageTarget.GetIsRoot() ? oRootPath / imageTarget :
						part.GetDirectory() / imageTarget;
					if (imagePath.GetPath().compare(0, packageRoot.size(), packageRoot) != 0 ||
						!NSFile::CFileBinary::Exists(imagePath.GetPath())) continue;
					int r = 0, c = 0;
					if (cell->m_oRef.IsInit())
					{
						if (!CCell::parseRefA(cell->m_oRef->c_str(), r, c)) continue;
						--r; --c;
					}
					else if (!cell->getRowCol(r, c)) continue;
					if (r < 0 || r >= 1048576 || c < 0 || c >= 16384) continue;

					double columnWidth = 8.43, rowHeight = 15.;
					if (sheet->m_oSheetFormatPr.IsInit())
					{
						if (sheet->m_oSheetFormatPr->m_oDefaultColWidth.IsInit())
							columnWidth = *sheet->m_oSheetFormatPr->m_oDefaultColWidth;
						if (sheet->m_oSheetFormatPr->m_oDefaultRowHeight.IsInit())
							rowHeight = *sheet->m_oSheetFormatPr->m_oDefaultRowHeight;
					}
					if (row->m_oHt.IsInit()) rowHeight = row->m_oHt->GetValue();
					if (sheet->m_oCols.IsInit())
						for (auto col : sheet->m_oCols->m_arrItems)
							if (col && col->m_oMin.IsInit() && col->m_oMax.IsInit() && col->m_oWidth.IsInit() &&
								col->m_oMin->GetValue() <= c+1 && col->m_oMax->GetValue() >= c+1)
								columnWidth = col->m_oWidth->GetValue();
					// Fit the picture using its stored aspect ratio. Positions in
					// the WPS part describe its source size, not the target cell.
					double width = std::floor(columnWidth * 7. + 5.) * 9525.;
					double height = rowHeight * 12700.;
					if (!std::isfinite(width) || !std::isfinite(height) || width <= 0 || height <= 0 ||
						width > std::numeric_limits<int>::max() || height > std::numeric_limits<int>::max())
						continue;
					double pictureWidth = width, pictureHeight = height;
					if (pic->spPr.xfrm.IsInit() && pic->spPr.xfrm->extX.IsInit() && pic->spPr.xfrm->extY.IsInit() &&
						*pic->spPr.xfrm->extX > 0 && *pic->spPr.xfrm->extY > 0)
					{
						const double scale = std::min(width / *pic->spPr.xfrm->extX, height / *pic->spPr.xfrm->extY);
						pictureWidth = *pic->spPr.xfrm->extX * scale;
						pictureHeight = *pic->spPr.xfrm->extY * scale;
					}
					if (!drawing.IsInit())
					{
						drawingFile = smart_ptr<OOX::File>(new CDrawing(this));
						drawing = drawingFile.smart_dynamic_cast<CDrawing>();
						sheet->m_oDrawing.Init();
						sheet->m_oDrawing->m_oId.Init();
						sheet->m_oDrawing->m_oId->SetValue(sheet->Add(drawingFile).get());
					}
					smart_ptr<OOX::File> image(new OOX::Image(this, imagePath));
					pic->blipFill.blip->embed = drawing->Add(image);
					pic->nvPicPr.cNvPr.id = nextId++;
					if (!pic->spPr.xfrm.IsInit()) pic->spPr.xfrm.Init();
					pic->spPr.xfrm->offX = 0; pic->spPr.xfrm->offY = 0;
					pic->spPr.xfrm->extX = static_cast<int>(pictureWidth);
					pic->spPr.xfrm->extY = static_cast<int>(pictureHeight);
					std::unique_ptr<CCellAnchor> anchor(new CCellAnchor(SimpleTypes::Spreadsheet::cellanchorOneCell));
					anchor->m_oFrom.Init();
					anchor->m_oFrom->m_oCol.Init(); anchor->m_oFrom->m_oCol->SetValue(c);
					anchor->m_oFrom->m_oRow.Init(); anchor->m_oFrom->m_oRow->SetValue(r);
					anchor->m_oFrom->m_oColOff.Init(); anchor->m_oFrom->m_oColOff->SetValue((width-pictureWidth)/2.);
					anchor->m_oFrom->m_oRowOff.Init(); anchor->m_oFrom->m_oRowOff->SetValue((height-pictureHeight)/2.);
					anchor->m_oExt.Init();
					anchor->m_oExt->m_oCx.Init(); anchor->m_oExt->m_oCx->SetValue(pictureWidth);
					anchor->m_oExt->m_oCy.Init(); anchor->m_oExt->m_oCy->SetValue(pictureHeight);
					anchor->m_oElement.Init();
					anchor->m_oElement->InitElem(pic.release());
					drawing->m_arrItems.push_back(anchor.release());
					// Clear only a successfully materialized photo. Unresolved IDs
					// keep their formula and cached value for diagnostics/recovery.
					cell->m_oFormula.reset(); cell->m_oValue.reset(); cell->m_oType.reset();
					cell->m_oCacheValue.reset();
				}
			}
		}
	}
}
bool OOX::Spreadsheet::CXlsx::Read(const CPath& oFilePath)
{
	_CP_LOG << L"start read and parsing: " << oFilePath.GetFilename() << std::endl;
	ReadNative(oFilePath);

	_CP_LOG << L"end read ... " << std::endl;

	if (!m_pWorkbook) return false;

	for (size_t i = 0; i < m_arWorksheets.size(); ++i)
	{
		if (m_arWorksheets[i])
			m_arWorksheets[i]->PrepareAfterRead();
	}
	return true;
}
bool OOX::Spreadsheet::CXlsx::WriteNative(const CPath& oDirPath, OOX::CContentTypes &oContentTypes)
{
    if (NULL == m_pWorkbook)
		return false;

	IFileContainer::Write(oDirPath / L"", OOX::CPath(_T("")), oContentTypes);

	oContentTypes.Write(oDirPath);
	return true;
}
bool OOX::Spreadsheet::CXlsx::Write(const CPath& oDirPath, OOX::CContentTypes &oContentTypes)
{
    PrepareToWrite();
  
	if(NULL == m_pWorkbook ||  m_arWorksheets.empty ())
        return false;

//CApp
	if(NULL == m_pApp)
	{
		OOX::CApp* pApp = new OOX::CApp(this);
		pApp->SetDefaults();
		smart_ptr<OOX::File> pAppFile(pApp);
		Add(pAppFile);
	}

//CCore
	if(NULL == m_pCore)
	{
		OOX::CCore* pCore = new OOX::CCore(this);
		pCore->SetDefaults();
		smart_ptr<OOX::File> pCoreFile(pCore);
		Add(pCoreFile);
	}

//xl
    CPath oXlPath = oDirPath / m_pWorkbook->DefaultDirectory();
    WriteWorkbook(oXlPath);

	IFileContainer::Write(oDirPath / L"" , OOX::CPath(_T("")), oContentTypes);

    oContentTypes.Write(oDirPath);
    return true;
}
bool OOX::Spreadsheet::CXlsx::WriteXLS(const CPath& oFilePath)
{
	PrepareToWrite();
	if(NULL == m_pWorkbook ||  m_arWorksheets.empty ())
		return false;
	auto workbookStream = new XLS::WorkbookStreamObject;
	auto workbookPtr = XLS::BaseObjectPtr(workbookStream);
	XlsWriter writer;
	writer.globalInfoPtr.reset(new XLS::GlobalWorkbookInfo(XLS::WorkbookStreamObject::DefaultCodePage, nullptr));
	// add pivot cache ids to global inf and fix 0 ids
	if(m_pWorkbook->m_oPivotCaches.IsInit() && !m_pWorkbook->m_oPivotCaches->m_arrItems.empty())
	{
		auto cacheNum = 0;
		for(auto cacheHeader : m_pWorkbook->m_oPivotCaches->m_arrItems)
		{
			if(!cacheHeader->m_oCacheId.IsInit())
				continue;
			cacheHeader->m_oCacheId = cacheHeader->m_oCacheId->GetValue() + 1;
			writer.globalInfoPtr->mapPivotCacheIndex.emplace(cacheHeader->m_oCacheId->GetValue(), cacheNum);
			cacheNum++;
		}
	}
	workbookStream->m_GlobalsSubstream = m_pWorkbook->toXLS();
	auto CastedGlobalsStram = static_cast<XLS::GlobalsSubstream*>(workbookStream->m_GlobalsSubstream.get());
	CastedGlobalsStram->global_info_ = writer.globalInfoPtr;
	for(auto i : m_arWorksheets)
		workbookStream->m_arWorksheetSubstream.push_back(i->toXLS());
	CastedGlobalsStram->m_arSUPBOOK.push_back(m_pWorkbook->WriteXtiRefsXLS());
	if(m_pSharedStrings != nullptr)
		m_pSharedStrings->toXLS(workbookStream->m_GlobalsSubstream);
	auto themesPtr = m_pWorkbook->Find(OOX::FileTypes::Theme);
	if(!(themesPtr->type() == OOX::FileTypes::Unknown))
	{	auto counter = 0;
		auto theme = static_cast<PPTX::Theme*>(themesPtr.GetPointer());
		for(auto themeColor : theme->themeElements.clrScheme.Scheme)
		{
			if(themeColor.first == L"lt1")
				counter = 0;
			else if(themeColor.first == L"dk1")
				counter = 1;
			else if(themeColor.first == L"lt2")
				counter = 2;
			else if(themeColor.first == L"dk2")
				counter = 3;
			else if(themeColor.first == L"accent1")
				counter = 4;
			else if(themeColor.first == L"accent2")
				counter = 5;
			else if(themeColor.first == L"accent3")
				counter = 6;
			else if(themeColor.first == L"accent4")
				counter = 7;
			else if(themeColor.first == L"accent5")
				counter = 8;
			else if(themeColor.first == L"accent6")
				counter = 9;
			else if(themeColor.first == L"hlink")
				counter = 10;
			else if(themeColor.first == L"folHlink")
				counter = 11;
			else continue;
			auto tempcolor = themeColor.second;
			std::wstringstream sStream;
			sStream << std::hex  << std::setw(6) << std::setfill(L'0')<< tempcolor.Color->GetARGB(0);
			writer.globalInfoPtr->RegisterPaletteColor(counter, sStream.str());
		}
	}
	if(m_pStyles != nullptr)
		m_pStyles->toXLS(workbookStream->m_GlobalsSubstream);

	writer.Open(oFilePath.GetPath());
	writer.WriteWorkbook(workbookPtr);

	if(m_pWorkbook->m_oPivotCaches.IsInit() && !m_pWorkbook->m_oPivotCaches->m_arrItems.empty())
	{
		for(auto cacheHeader : m_pWorkbook->m_oPivotCaches->m_arrItems)
		{
			if(!cacheHeader->m_oCacheId.IsInit() || !cacheHeader->m_oRid.IsInit() ||  !m_pWorkbook->IsExist(cacheHeader->m_oRid->GetValue()))
				continue;
			auto cacheFilePtr = m_pWorkbook->Find(cacheHeader->m_oRid->GetValue());
			auto CachePtr = static_cast<CPivotCacheDefinitionFile*>(cacheFilePtr.GetPointer());
			auto XLSBinCache = CachePtr->m_oPivotCashDefinition->toXLS(cacheHeader->m_oCacheId->GetValue());
			auto castedCache = static_cast<XLS::PIVOTCACHE*>(XLSBinCache.get());
			auto cacheRecordsPtr = CachePtr->Find(OOX::SpreadsheetBin::FileTypes::PivotCacheRecordsBin);
			if(!(cacheRecordsPtr->type() == OOX::FileTypes::Unknown))
			{
				auto castedRecords = static_cast<CPivotCacheRecordsFile*>(cacheRecordsPtr.GetPointer());
				if(!castedRecords->m_oPivotCacheRecords.IsInit() && castedRecords->m_pData!= nullptr)
				{
					castedRecords->m_oPivotCacheRecords.Init();
					XmlUtils::CXmlLiteReader reader;
					reader.FromStringA((char*)castedRecords->m_pData, castedRecords->m_nDataLength);
					reader.ReadNextNode();
					castedRecords->m_oPivotCacheRecords->fromXML(reader);
				}
				if(castedRecords->m_oPivotCacheRecords.IsInit())
				for(auto CacheRecord : castedRecords->m_oPivotCacheRecords->m_arrItems)
				{
					castedCache->m_arDBB.push_back(CacheRecord->toXLS());
				}
			}


			if(CachePtr->m_oPivotCashDefinition.IsInit())
				writer.WritePivotCache(XLSBinCache, cacheHeader->m_oCacheId->GetValue());
		}
	}
	return true;
}
bool OOX::Spreadsheet::CXlsx::WriteWorkbook(const CPath& oDirPath)
{
//Theme
	if (m_pTheme.IsInit())
	{
        smart_ptr<OOX::File> pThemeFile = m_pTheme.smart_dynamic_cast<OOX::File>();
		m_pWorkbook->Add(pThemeFile);
	}
//SharedStrings
    if(m_pSharedStrings && m_pSharedStrings->m_nCount > 0)
    {
        smart_ptr<OOX::File> pSharedStringsFile(m_pSharedStrings);
        bDeleteSharedStrings = false;
        m_pWorkbook->Add(pSharedStringsFile);
    }
//Styles
    if(m_pStyles)
    {
        smart_ptr<OOX::File> pStylesFile(m_pStyles);
        bDeleteStyles = false;
        m_pWorkbook->Add(pStylesFile);
    }

//Workbook
    smart_ptr<OOX::File> pWorkbookFile(m_pWorkbook);
    bDeleteWorkbook = false;
    Add(pWorkbookFile);
    return true;
}
void OOX::Spreadsheet::CXlsx::PrepareToWrite()
{
    if( m_pWorkbook)	m_pWorkbook->PrepareToWrite();
    if( m_pStyles)		m_pStyles->PrepareToWrite();

    for (size_t i = 0; i < m_arWorksheets.size(); ++i)
    {
        if (m_arWorksheets[i])
			m_arWorksheets[i]->PrepareToWrite();
    }
}

void OOX::Spreadsheet::CXlsx::CreateWorkbook ()
{
	if(bDeleteWorkbook)
		RELEASEOBJECT(m_pWorkbook);
	m_pWorkbook = new CWorkbook(NULL);
	bDeleteWorkbook = true;
}

void OOX::Spreadsheet::CXlsx::CreateSharedStrings ()
{
	if(bDeleteSharedStrings)
		RELEASEOBJECT(m_pSharedStrings);
	m_pSharedStrings = new CSharedStrings(NULL);
	bDeleteSharedStrings = true;
}

void OOX::Spreadsheet::CXlsx::CreateStyles ()
{
	if(bDeleteStyles)
		RELEASEOBJECT(m_pStyles);
	m_pStyles = new CStyles(NULL);
	bDeleteStyles = true;
}
bool OOX::Spreadsheet::CXlsx::hasPivot()
{
	if (!m_pWorkbook) return false;
	
	smart_ptr<OOX::File> filePivotCacheDefinition = m_pWorkbook->Find(FileTypes::PivotCacheDefinition.RelationType());
	return filePivotCacheDefinition.IsInit();
}
PPTX::Theme* OOX::Spreadsheet::CXlsx::GetTheme () const
{
	return (PPTX::Theme  *)(m_pTheme.GetPointer());
}

void OOX::Spreadsheet::CXlsx::PrepareWorkbook()
{
	IFileContainer::m_mapEnumeratedGlobal.clear();
	if(!m_pWorkbook)
	{
		m_pWorkbook = new OOX::Spreadsheet::CWorkbook(this);
		m_pWorkbook->m_oWorkbookPr.Init();
		m_pWorkbook->m_oWorkbookPr->m_oDefaultThemeVersion.Init();
		m_pWorkbook->m_oWorkbookPr->m_oDefaultThemeVersion->SetValue(124226);
		m_pWorkbook->m_oBookViews.Init();
		
		OOX::Spreadsheet::CWorkbookView* pWorkbookView = new OOX::Spreadsheet::CWorkbookView();
		
		pWorkbookView->m_oXWindow.Init();
		pWorkbookView->m_oXWindow->SetValue(480);
		pWorkbookView->m_oYWindow.Init();
		pWorkbookView->m_oYWindow->SetValue(120);
		pWorkbookView->m_oWindowWidth.Init();
		pWorkbookView->m_oWindowWidth->SetValue(27795);
		pWorkbookView->m_oWindowHeight.Init();
		pWorkbookView->m_oWindowHeight->SetValue(12585);
		
		m_pWorkbook->m_oBookViews->m_arrItems.push_back(pWorkbookView);
	}
	//add sheet if there isn't one
	if (m_arWorksheets.empty())
	{
		OOX::Spreadsheet::CWorksheet* pWorksheet = new OOX::Spreadsheet::CWorksheet(this);
		
		pWorksheet->m_oDimension.Init();
		pWorksheet->m_oDimension->m_oRef = L"A1";
		pWorksheet->m_oSheetViews.Init();
		
		OOX::Spreadsheet::CSheetView* pSheetView = new OOX::Spreadsheet::CSheetView();
		
		pSheetView->m_oTabSelected.Init();
		pSheetView->m_oTabSelected->FromBool(true);
		pSheetView->m_oWorkbookViewId.Init();
		pSheetView->m_oWorkbookViewId->SetValue(0);
		
		pWorksheet->m_oSheetViews->m_arrItems.push_back(pSheetView);
		pWorksheet->m_oSheetFormatPr.Init();
		pWorksheet->m_oSheetFormatPr->m_oDefaultRowHeight = 15.;
		
		pWorksheet->m_oPageMargins.Init();
		pWorksheet->m_oPageMargins->m_oLeft.Init();
		pWorksheet->m_oPageMargins->m_oLeft->FromInches(0.7);
		pWorksheet->m_oPageMargins->m_oTop.Init();
		pWorksheet->m_oPageMargins->m_oTop->FromInches(0.7);
		pWorksheet->m_oPageMargins->m_oRight.Init();
		pWorksheet->m_oPageMargins->m_oRight->FromInches(0.7);
		pWorksheet->m_oPageMargins->m_oBottom.Init();
		pWorksheet->m_oPageMargins->m_oBottom->FromInches(0.7);
		pWorksheet->m_oPageMargins->m_oHeader.Init();
		pWorksheet->m_oPageMargins->m_oHeader->FromInches(0.3);
		pWorksheet->m_oPageMargins->m_oFooter.Init();
		pWorksheet->m_oPageMargins->m_oFooter->FromInches(0.3);
		m_arWorksheets.push_back(pWorksheet);
		
		smart_ptr<OOX::File> pWorksheetFile(pWorksheet);
		OOX::RId oRId = this->Add(pWorksheetFile);		
		
		m_pWorkbook->m_oSheets.Init();
		
		OOX::Spreadsheet::CSheet* pSheet = new OOX::Spreadsheet::CSheet();
		pSheet->m_oName = L"Sheet1";
		pSheet->m_oSheetId.Init();
		pSheet->m_oSheetId->SetValue(1);
		pSheet->m_oRid.Init();
		pSheet->m_oRid->SetValue(oRId.ToString());
		
		m_pWorkbook->m_oSheets->m_arrItems.push_back(pSheet);
	}
	//ensure that there are always zero styles and the first font always has font name and size
	if( m_pStyles )
	{
		//Fonts
		if(false == m_pStyles->m_oFonts.IsInit())
			m_pStyles->m_oFonts.Init();
		
		if(m_pStyles->m_oFonts->m_arrItems.empty())
			m_pStyles->m_oFonts->AddFont(new OOX::Spreadsheet::CFont());
		
		OOX::Spreadsheet::CFont* pFont = m_pStyles->m_oFonts->m_arrItems.front();
		
		if(false == pFont->m_oRFont.IsInit())
		{
			pFont->m_oRFont.Init();
			pFont->m_oRFont->m_sVal = _T("Calibri");

			if(false == pFont->m_oScheme.IsInit())
			{
				pFont->m_oScheme.Init();
				pFont->m_oScheme->m_oFontScheme.Init();
				pFont->m_oScheme->m_oFontScheme->SetValue(SimpleTypes::Spreadsheet::fontschemeMinor);
			}
		}

		if(false == pFont->m_oSz.IsInit() || false == pFont->m_oSz->m_oVal.IsInit())
		{
			pFont->m_oSz.Init();
			pFont->m_oSz->m_oVal.Init();
			pFont->m_oSz->m_oVal->SetValue(11.0);
		}
		//Fills
		if(false == m_pStyles->m_oFills.IsInit())
			m_pStyles->m_oFills.Init();
		
		if(m_pStyles->m_oFills->m_arrItems.empty())
			m_pStyles->m_oFills->m_arrItems.push_back(new OOX::Spreadsheet::CFill());

		OOX::Spreadsheet::CFill* pFill = m_pStyles->m_oFills->m_arrItems.front();
		if(false == pFill->m_oGradientFill.IsInit())
		{
			if(false == pFill->m_oPatternFill.IsInit())
				pFill->m_oPatternFill.Init();
			if(false == pFill->m_oPatternFill->m_oPatternType.IsInit())
				pFill->m_oPatternFill->m_oPatternType.Init();
			pFill->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeNone);
		}
		if(false == m_pStyles->m_oBorders.IsInit())
			m_pStyles->m_oBorders.Init();
		if(m_pStyles->m_oBorders->m_arrItems.size() == 0)
			m_pStyles->m_oBorders->m_arrItems.push_back(new OOX::Spreadsheet::CBorder());

		//Xfs
		if(false == m_pStyles->m_oCellXfs.IsInit())
			m_pStyles->m_oCellXfs.Init();
		
		if(m_pStyles->m_oCellXfs->m_arrItems.empty())
			m_pStyles->m_oCellXfs->m_arrItems.push_back(new OOX::Spreadsheet::CXfs());

		OOX::Spreadsheet::CXfs* pXfs = m_pStyles->m_oCellXfs->m_arrItems.front();
		if(false == pXfs->m_oBorderId.IsInit())
		{
			pXfs->m_oBorderId.Init();
			pXfs->m_oBorderId->SetValue(0);
		}
		if(false == pXfs->m_oFillId.IsInit())
		{
			pXfs->m_oFillId.Init();
			pXfs->m_oFillId->SetValue(0);
		}
		if(false == pXfs->m_oFontId.IsInit())
		{
			pXfs->m_oFontId.Init();
			pXfs->m_oFontId->SetValue(0);
		}
		if(false == pXfs->m_oNumFmtId.IsInit())
		{
			pXfs->m_oNumFmtId.Init();
			pXfs->m_oNumFmtId->SetValue(0);
		}
	}
	//move <is> tags and cells with str type to sharedString if they weren't moved during reading
	for (size_t i = 0; i < m_arWorksheets.size(); ++i)
	{
		PrepareWorksheet(m_arWorksheets[i]);
	}
	//todo parse dates in iso 8601 format
}
void OOX::Spreadsheet::CXlsx::PrepareWorksheet(CWorksheet* pWorksheet)
{
	if (!pWorksheet) return;
	if (pWorksheet->m_bPrepareForBinaryWriter) return;

	pWorksheet->m_bPrepareForBinaryWriter = true;

	if(pWorksheet->m_oSheetData.IsInit())
	{
        std::vector<OOX::Spreadsheet::CRow*>& aRows = pWorksheet->m_oSheetData->m_arrItems;
		
        for(size_t i = 0; i < aRows.size(); ++i)
		{
            OOX::Spreadsheet::CRow* pRow = aRows[i];
			
            std::vector<OOX::Spreadsheet::CCell*> & aCells = pRow->m_arrItems;
			
            for(size_t j = 0; j < aCells.size(); ++j)
			{
                OOX::Spreadsheet::CCell* pCell = aCells[j];
				if (!pCell)continue;

				if(pCell->m_oType.IsInit())
				{
					if(SimpleTypes::Spreadsheet::celltypeInlineStr == pCell->m_oType->GetValue())
					{
						if(!m_pSharedStrings) CreateSharedStrings();
						
						OOX::Spreadsheet::CSi* pSi = pCell->m_oRichText.GetPointerEmptyNullable();
						if(NULL != pSi)
						{
							int nIndex = m_pSharedStrings->AddSi(pSi);
							//change cell value
							pCell->m_oValue.Init();
                            pCell->m_oValue->m_sText = std::to_wstring(nIndex);
							//change cell type
							pCell->m_oType.Init();
							pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
						}
					}
					else if(SimpleTypes::Spreadsheet::celltypeStr == pCell->m_oType->GetValue() || SimpleTypes::Spreadsheet::celltypeError == pCell->m_oType->GetValue())
					{
						if (pCell->m_oValue.IsInit())
						{
							if(!m_pSharedStrings) CreateSharedStrings();

							//add to SharedStrings
							CSi* pSi = new CSi();
							CText* pText =  new CText();
							pText->m_sText = pCell->m_oValue->ToString();
							pSi->m_arrItems.push_back(pText);

							int nIndex = m_pSharedStrings->AddSi(pSi);
							//change cell value
							pCell->m_oValue.Init();
							pCell->m_oValue->m_sText = std::to_wstring(nIndex);
							//change cell type
							if(SimpleTypes::Spreadsheet::celltypeStr == pCell->m_oType->GetValue())
							{
								pCell->m_oType.Init();
								pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
							}
						}
						else
						{
							pCell->m_oValue.reset();
							pCell->m_oType.reset();
						}
					}
					else if(SimpleTypes::Spreadsheet::celltypeBool == pCell->m_oType->GetValue())
					{
						//usually written as 1/0, but sometimes true/false is encountered
						if(pCell->m_oValue.IsInit())
						{
							SimpleTypes::COnOff oOnOff;
                            std::wstring sVal = pCell->m_oValue->ToString();
                            oOnOff.FromString(sVal.c_str());
							pCell->m_oValue.Init();
							if(oOnOff.ToBool())
								pCell->m_oValue->m_sText = _T("1");
							else
								pCell->m_oValue->m_sText = _T("0");
						}
					}
				}
				
			}
		}
	}
}
