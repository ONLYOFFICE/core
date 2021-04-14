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

#include "DocxFlat.h"

#include "HeaderFooter.h"
#include "Document.h"
#include "FontTable.h"
#include "Numbering.h"
#include "Styles.h"
#include "Settings/Settings.h"

namespace OOX
{

	CDocxFlat::CDocxFlat() : File(dynamic_cast<Document*>(this)),	m_oComments(dynamic_cast<Document*>(this)),
																	m_oEndnotes(dynamic_cast<Document*>(this)), 
																	m_oFootnotes(dynamic_cast<Document*>(this))
	{
	}
	CDocxFlat::CDocxFlat(const CPath& oFilePath) : File(this),	m_oComments(dynamic_cast<Document*>(this)), 
																m_oEndnotes(dynamic_cast<Document*>(this)), 
																m_oFootnotes(dynamic_cast<Document*>(this))
	{
		read( oFilePath );
	}
	CDocxFlat::~CDocxFlat()
	{
		for (std::map<std::wstring, OOX::CHdrFtr*>::iterator it = m_mapHeadersFooters.begin(); it != m_mapHeadersFooters.end(); ++it)
		{
			if (it->second) delete it->second; it->second = NULL;
		}
		m_mapHeadersFooters.clear();
	}
	void CDocxFlat::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if	( oReader, L"xml:space", m_oSpace )
		WritingElement_ReadAttributes_End( oReader )
	}
	void CDocxFlat::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode(nDepth) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"w:body" == sName )
			{
				m_pDocument = new CDocument(dynamic_cast<Document*>(this));
				m_pDocument->fromXML(oReader);
			}
			else if ( L"w:fonts" == sName )
				m_pFontTable = oReader;
			else if ( L"w:lists" == sName )
				m_pNumbering = oReader;
			else if ( L"w:styles" == sName )
				m_pStyles = oReader;
			else if ( L"w:docPr" == sName )
				m_pSettings = oReader;
			else if ( L"w:bgPict" == sName )
				m_oBgPict = oReader;
		}
	}
}
