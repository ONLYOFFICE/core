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
#pragma once

#include "XlsxFlat.h"

#include "Workbook/Workbook.h"
#include "SharedStrings/SharedStrings.h"
#include "Styles/Styles.h"
#include "Worksheets/Worksheet.h"
#include "Workbook/DefinedNames.h"


namespace OOX
{
namespace Spreadsheet
{
	CXlsxFlat::CXlsxFlat() : File(dynamic_cast<Document*>(this))
	{
		m_nLastReadRow = 0;
		m_nLastReadCol = -1;
		
		m_maxDigitSize = std::make_pair(0, 0);
	}
	CXlsxFlat::CXlsxFlat(const CPath& oFilePath) : File(this)
	{
		m_nLastReadRow = 0;
		m_nLastReadCol = -1;
		
		m_maxDigitSize = std::make_pair(0, 0);
		
		read( oFilePath );
	}
	CXlsxFlat::~CXlsxFlat()
	{
		m_pWorkbook.reset();
		m_pStyles.reset();
        m_pSharedStrings.reset();
		
		for (size_t i = 0; i < m_arWorksheets.size(); ++i)
		{
			if (m_arWorksheets[i])
			{
				delete m_arWorksheets[i];
				m_arWorksheets[i] = NULL; 
			}
		}
		m_arWorksheets.clear();
	}

	void CXlsxFlat::read(const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		fromXML(oReader);
	}
	void CXlsxFlat::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml = toXML();

		NSFile::CFileBinary file;
		file.CreateFileW(oFilePath.GetPath());
		file.WriteStringUTF8(sXml);
		file.CloseFile();
	}
	const OOX::FileType CXlsxFlat::type() const
	{
		return OOX::Spreadsheet::FileTypes::SpreadsheetFlat;
	}
	const CPath CXlsxFlat::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CXlsxFlat::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CXlsxFlat::fromXML(XmlUtils::CXmlNode& oNode)
	{
	}
	std::wstring CXlsxFlat::toXML() const
	{
		std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";

		return sXml;
	}
	void CXlsxFlat::toXML(NSStringUtils::CStringBuilder& writer) const
	{
	}
	EElementType CXlsxFlat::getType() const
	{
		return et_x_SpreadsheetFlat;
	}
	void CXlsxFlat::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if ( oReader.IsEmptyNode() )
			return;

		if (false == m_pWorkbook.IsInit())
		{
			m_pWorkbook = new CWorkbook(dynamic_cast<OOX::Document*>(this));
		}
		if (false == m_pSharedStrings.IsInit())
		{
			m_pSharedStrings = new CSharedStrings(dynamic_cast<OOX::Document*>(this));
		}
		int nStylesDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nStylesDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"ExcelWorkbook" == sName )
			{
				m_pWorkbook->fromXML(oReader);
			}
			else if ( L"Worksheet" == sName )
			{
				CWorksheet *pWorksheet = new CWorksheet(dynamic_cast<OOX::Document*>(this));
				m_arWorksheets.push_back(pWorksheet);
				
				pWorksheet->fromXML(oReader);				
			}
			else if ( L"Styles" == sName )
			{
				m_pStyles = new CStyles(dynamic_cast<OOX::Document*>(this));
				m_pStyles->fromXML(oReader);
			}
			else if ( L"Names" == sName )
			{
				m_pWorkbook->m_oDefinedNames = new CDefinedNames(dynamic_cast<OOX::Document*>(this));
				m_pWorkbook->m_oDefinedNames->fromXML(oReader);
			}
		}
	}
	std::pair<double, double> CXlsxFlat::getMaxDigitSize()
	{
		if (m_maxDigitSize.first <= 0.1)
		{
			COfficeFontPicker* pFontPicker = new COfficeFontPicker();
			pFontPicker->Init(m_strFontDirectory);
			NSFonts::IFontManager* pFontManager = pFontPicker->get_FontManager();

			std::wstring fontName = L"Arial";
			double fontSize = 10;
			int fontStyle = 0;
			//-----------------------------------------------------------
			if ((m_pStyles.IsInit()) && (m_pStyles->m_oFonts.IsInit()) && (false == m_pStyles->m_oFonts->m_arrItems.empty()))
			{
				fontSize = m_pStyles->m_oFonts->m_arrItems[0]->m_oSz->m_oVal->GetValue();
				fontName = *m_pStyles->m_oFonts->m_arrItems[0]->m_oRFont->m_sVal;
			}
			//-----------------------------------------------------------
			int hr = FALSE;

			double dpi = 96;
			if (FALSE == (hr = pFontManager->LoadFontByName(fontName, fontSize, fontStyle, dpi, dpi)))
			{
				if (FALSE == (hr = pFontManager->LoadFontByName(L"Arial", fontSize, fontStyle, dpi, dpi)))
				{
					delete pFontPicker;
					m_maxDigitSize = std::pair<double, double>(7, 8);
					return m_maxDigitSize;
				}
			}

			double maxWidth = 0;
			double maxHeight = 0;

			double minWidth = 0xffffffff;
			double minHeight = 0xfffffff;

			// for (int i = 0; i <= 9; ++i)
			//{
				//if (FALSE == (hr = pFontManager->LoadString2( std::to_wstring(i), 0, 0)))
				//	return std::pair<float, float>(7,8);
				//TBBox box;
				//try
				//{
				//	box = pFontManager->MeasureString();
				//}
			//}
			if (FALSE == (hr = pFontManager->LoadString2(L"0123456789", 0, 0)))//
			{
				m_maxDigitSize = std::pair<double, double>(7., 8.);
				return m_maxDigitSize;
			}

			TBBox box;
			try
			{
				box = pFontManager->MeasureString();
			}
			catch (...)
			{
				m_maxDigitSize = std::pair<double, double>(7., 8.);
				return m_maxDigitSize;
			}

			if (box.fMaxX < -0xffff + 1 || box.fMaxY < -0xffff + 1 ||
				box.fMinX > 0xffff - 1 || box.fMinY > 0xffff - 1)
			{
				m_maxDigitSize = std::pair<double, double>(7., 8.);
				return m_maxDigitSize;
			}

			if (box.fMaxX - box.fMinX > maxWidth)   maxWidth = box.fMaxX - box.fMinX;
			if (box.fMaxY - box.fMinY > maxHeight)  maxHeight = box.fMaxY - box.fMinY;

			if (box.fMaxX - box.fMinX < minWidth)   minWidth = box.fMaxX - box.fMinX;
			if (box.fMaxY - box.fMinY < minHeight)  minHeight = box.fMaxY - box.fMinY;

			double width = (minWidth + 2 * maxWidth) / 10. / 3.;

			if (width > 0.01 && maxHeight > 0.01)
				m_maxDigitSize = std::pair<double, double>(width, maxHeight);
			else
				m_maxDigitSize = std::pair<double, double>(7., 8.);
			
			delete pFontPicker;
		}
		return m_maxDigitSize;
	}

}

}
