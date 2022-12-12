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

#include "File.h"
#include "IFileContainer.h"
#include "FileTypes.h"
#include "WritingElement.h"
#include "../Common/SimpleTypes_Shared.h"
#include "Document.h"

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

	// Attributes
		SimpleTypes::CGuid   m_oItemID;

	// Childs
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
