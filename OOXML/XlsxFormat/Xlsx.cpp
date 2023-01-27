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
#include "Xlsx.h"

#include "../DocxFormat/App.h"
#include "../DocxFormat/Core.h"

#include "Workbook/Workbook.h"
#include "SharedStrings/SharedStrings.h"
#include "Styles/Styles.h"
#include "Worksheets/Worksheet.h"
#include "CalcChain/CalcChain.h"
#include "ExternalLinks/ExternalLinks.h"
#include "ExternalLinks/ExternalLinkPath.h"
#include "Pivot/PivotTable.h"
#include "Pivot/PivotCacheDefinition.h"
#include "Pivot/PivotCacheRecords.h"

#include "../../DesktopEditor/common/SystemUtils.h"

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
	IFileContainer::Read(oRels, oFilePath, oFilePath);

	if (!m_pWorkbook) return false;

	CXlsb* xlsb = dynamic_cast<CXlsb*>(this);
	if (xlsb)
	{
		xlsb->PrepareSi();
		xlsb->PrepareTableFormula();
	}

	return true;
}
bool OOX::Spreadsheet::CXlsx::Read(const CPath& oFilePath)
{
	ReadNative(oFilePath);

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
	//добавляем sheet, если нет ни одного
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
	//делаем так чтобы всегда были нулевые стили и первый font всегда имел шрифт и размер
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
	//переносим теги <is> и ячейки с типом str в sharedString если они не перенеслисьпричтении
	for (size_t i = 0; i < m_arWorksheets.size(); ++i)
	{
		PrepareWorksheet(m_arWorksheets[i]);
	}
	//todo парсим даты в формате iso 8601
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
							//меняем значение ячейки
							pCell->m_oValue.Init();
                            pCell->m_oValue->m_sText = std::to_wstring(nIndex);
							//меняем тип ячейки
							pCell->m_oType.Init();
							pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
						}
					}
					else if(SimpleTypes::Spreadsheet::celltypeStr == pCell->m_oType->GetValue() || SimpleTypes::Spreadsheet::celltypeError == pCell->m_oType->GetValue())
					{
						if (pCell->m_oValue.IsInit())
						{
							if(!m_pSharedStrings) CreateSharedStrings();

							//добавляем в SharedStrings
							CSi* pSi = new CSi();
							CText* pText =  new CText();
							pText->m_sText = pCell->m_oValue->ToString();
							pSi->m_arrItems.push_back(pText);

							int nIndex = m_pSharedStrings->AddSi(pSi);
							//меняем значение ячейки
							pCell->m_oValue.Init();
							pCell->m_oValue->m_sText = std::to_wstring(nIndex);
							//меняем тип ячейки
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
						//обычно пишется 1/0, но встречается, что пишут true/false
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
