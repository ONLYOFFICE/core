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

#include "../Xlsx.h"

#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CDoubleFeatureProperty : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDoubleFeatureProperty)
			CDoubleFeatureProperty(OOX::Document* pMain = NULL) {}
			virtual ~CDoubleFeatureProperty() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			nullable_string m_oK;
			nullable_double m_oContent;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CBoolFeatureProperty : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CBoolFeatureProperty)
			CBoolFeatureProperty(OOX::Document* pMain = NULL) {}
			virtual ~CBoolFeatureProperty() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_string m_oK;
			nullable_bool m_oContent;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CStringFeatureProperty : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CStringFeatureProperty)
			CStringFeatureProperty(OOX::Document* pMain = NULL) {}
			virtual ~CStringFeatureProperty() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_string m_oK;
			nullable_string m_oContent;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CIntFeatureProperty : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CIntFeatureProperty)
			CIntFeatureProperty(OOX::Document* pMain = NULL) {}
			virtual ~CIntFeatureProperty(){}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_string m_oK;
			nullable_int m_oContent;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CBagFeatureProperty : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CBagFeatureProperty)
			CBagFeatureProperty(OOX::Document* pMain = NULL) {}
			virtual ~CBagFeatureProperty() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_string m_oK;
			nullable_uint m_oContent;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CArrayFeatureProperty : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CArrayFeatureProperty)
			CArrayFeatureProperty(OOX::Document* pMain = NULL) {}
			virtual ~CArrayFeatureProperty() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_string m_oK;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CFeaturePropertyBag : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CFeaturePropertyBag)
			CFeaturePropertyBag(OOX::Document* pMain = NULL) {}
			virtual ~CFeaturePropertyBag() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable_string m_oType;
			nullable_string m_oExtRef;
			nullable_uint m_oBagExtId;
			nullable_string m_oAtt;
		};
 //------------------------------------------------------------------------------------------------------------------------
		class CFeaturePropertyBags : public WritingElementWithChilds<CFeaturePropertyBag>
		{ 
		public:
			WritingElement_AdditionMethods(CFeaturePropertyBags)
			CFeaturePropertyBags(OOX::Document* pMain = NULL);
			virtual ~CFeaturePropertyBags();

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		};
//------------------------------------------------------------------------------------------------------------------------		
		class CFeaturePropertyBagFile : public OOX::File
		{
		public:
			CFeaturePropertyBagFile(OOX::Document* pMain);
			CFeaturePropertyBagFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CFeaturePropertyBagFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			nullable<CFeaturePropertyBags> m_oPropertyBags;
		private:
			CPath m_oReadPath;
		};
	} //Spreadsheet
} // namespace OOX

