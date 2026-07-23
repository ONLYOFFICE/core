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
#pragma once

#include "IFileContainer.h"
#include "../Common/SimpleTypes_Word.h"
#include "../../DesktopEditor/common/File.h"

namespace OOX
{
	class CHdrFtr;
	class CDocument;
	class CFontTable;
	class CNumbering;
	class CStyles;
	class CEndnotes;
	class CFootnotes;
	class CComments;
	class CSettings;
	class CApp;
	class CCore;

	namespace Logic
	{
		class CBgPict;
		class CDocSuppData;
	}

	class CDocxFlat : public Document, public File, public WritingElement
	{
	public:
		CDocxFlat();
		CDocxFlat(const CPath& oFilePath);
		virtual ~CDocxFlat();

		virtual void read(const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

		OOX::CHdrFtr *GetHeaderOrFooter(const OOX::RId& rId) const;
		void ReadDocumentProperties(XmlUtils::CXmlLiteReader& oReader);

//-----------------------------------------------------------------------
		nullable<SimpleTypes::CXmlSpace> m_oSpace;

		nullable<CDocument>				m_pDocument;
		nullable<CStyles>				m_pStyles;
		nullable<CFontTable>			m_pFontTable;
		nullable<CNumbering>			m_pNumbering;
		nullable<CSettings>				m_pSettings;
		nullable<Logic::CBgPict>		m_pBgPict;
		nullable<Logic::CDocSuppData>	m_oDocSuppData;
		nullable<Logic::CDocSuppData>	m_oDocOleData;

		nullable<CComments>				m_pComments;
		nullable<CFootnotes>			m_pFootnotes;
		nullable<CEndnotes>				m_pEndnotes;
		nullable<CApp>					m_pApp;
		nullable<CCore>					m_pCore;
//-----------------------------------------------------------
		nullable_string					m_sCompatibilityMode;

		std::map<std::wstring, NSCommon::smart_ptr<OOX::File>> m_mapImages;
		std::map<std::wstring, NSCommon::smart_ptr<OOX::File>> m_mapOleData;

		OOX::IFileContainer *m_currentContainer = NULL;
	private:
		void ParsingOleData();
		void ParsingSuppData();
	};


}
