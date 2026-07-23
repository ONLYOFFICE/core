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

#include "../../Base/Nullable.h"
#include "../WritingElement.h"

namespace ComplexTypes
{
	namespace Word
	{
		class String;
		class CDocPartBehavior;
		class CDocPartGallery;
	}
}

namespace OOX
{
	namespace Logic
	{
		class CDocPartTypes : public WritingElementWithChilds<ComplexTypes::Word::String>
		{
		public:
			WritingElement_AdditionMethods(CDocPartTypes)

			CDocPartTypes(OOX::Document *pMain = NULL);
			virtual ~CDocPartTypes();

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
			
			nullable_bool m_oAll;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

		class CDocPartBehaviors : public WritingElementWithChilds<ComplexTypes::Word::CDocPartBehavior>
		{
		public:
			WritingElement_AdditionMethods(CDocPartBehaviors)

			CDocPartBehaviors(OOX::Document *pMain = NULL);
			virtual ~CDocPartBehaviors();

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;
		};

		class CDocPartCategory : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDocPartCategory)

			CDocPartCategory(OOX::Document *pMain = NULL);
			virtual ~CDocPartCategory();

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable<ComplexTypes::Word::String>			m_oName;
			nullable<ComplexTypes::Word::CDocPartGallery>	m_oGallery;
		};

		class CDocPartPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDocPartPr)

			CDocPartPr(OOX::Document *pMain = NULL);
			virtual ~CDocPartPr();

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable<ComplexTypes::Word::String>	m_oName;
			nullable<ComplexTypes::Word::String>	m_oDescription;
			nullable<ComplexTypes::Word::String>	m_oGuid; // guid !!!! ????
			nullable<ComplexTypes::Word::String>	m_oStyle;
			nullable<CDocPartCategory>				m_oCategory;
			nullable<CDocPartTypes>					m_oTypes;
			nullable<CDocPartBehaviors>				m_oBehaviors;
		};

		class CDocPartBody : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CDocPartBody)

			CDocPartBody(OOX::Document *pMain = NULL);

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

		class CDocPart : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CDocPart)

			CDocPart(OOX::Document *pMain = NULL);
			virtual ~CDocPart();

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const;

			nullable<CDocPartPr>	m_oDocPartPr;
			nullable<CDocPartBody>	m_oDocPartBody;
		};

		class CDocParts : public WritingElementWithChilds<CDocPart>
		{
		public:
			WritingElement_AdditionMethods(CDocParts)

			CDocParts(OOX::Document *pMain = NULL);
			virtual ~CDocParts();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
		};

	} // namespace Logic
} // namespace OOX

