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

#include "File.h"
#include "IFileContainer.h"
#include "WritingElement.h"
#include "../Common/SimpleTypes_Shared.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CCustomXML 22.5
	//--------------------------------------------------------------------------------	
	class CCustomXMLProps : public OOX::FileGlobalEnumerated, public WritingElement
	{
	public:

		class CShemaRef : public WritingElement
		{
		public:
			CShemaRef();
			CShemaRef(const XmlUtils::CXmlNode& oNode);
			virtual ~CShemaRef();

			const CShemaRef& operator =(const XmlUtils::CXmlNode& oNode);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual std::wstring toXML() const;

			virtual EElementType getType() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			std::wstring m_sUri;
		};

		//----------------------------------------------------------------------

		class CShemaRefs : public WritingElementWithChilds<CShemaRef>
		{
		public:
			CShemaRefs();
			CShemaRefs(const XmlUtils::CXmlNode& oNode);
			virtual ~CShemaRefs();

			const CShemaRefs& operator =(const XmlUtils::CXmlNode& oNode);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		//----------------------------------------------------------------------

		CCustomXMLProps(OOX::Document *pMain);
		CCustomXMLProps(OOX::Document *pMain, const OOX::CPath& oFilePath);
		virtual ~CCustomXMLProps();

		const CCustomXMLProps& operator =(const XmlUtils::CXmlNode& oNode);

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual std::wstring toXML() const;

		virtual void read(const CPath& oFilePath);
		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual EElementType getType() const;
		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		SimpleTypes::CGuid   m_oItemID;

		nullable<CShemaRefs> m_oShemaRefs;
//------------- for write from binary
		std::wstring m_oCustomXmlContent;
		std::string m_oCustomXmlContentA;
	};

	class CCustomXML : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
	{
	public:
		CCustomXML(OOX::Document *pMain, bool bDocument = true);
		CCustomXML(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath);
		virtual ~CCustomXML();

		virtual void read(const CPath& oPath);
		virtual void read(const CPath& oRootPath, const CPath& oFilePath);

		virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

		virtual const OOX::FileType type() const;

		virtual const CPath DefaultDirectory() const;
		virtual const CPath DefaultFileName() const;

		std::wstring GetSchemaUrl();

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter);
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

		std::string m_sXmlA;

		bool m_bUsed = false;
		bool m_bDocument = false;
	};

} // namespace OOX
