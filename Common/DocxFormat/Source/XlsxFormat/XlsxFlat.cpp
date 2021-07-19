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
	}
	CXlsxFlat::CXlsxFlat(const CPath& oFilePath) : File(this)
	{
		m_nLastReadRow = 0;
		m_nLastReadCol = -1;
		
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
			std::wstring sName = oReader.GetName();

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
}

}