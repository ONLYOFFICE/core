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

#include "../../Base/Nullable.h"
#include "../WritingElement.h"
#include "../../Common/ComplexTypes.h"

namespace OOX
{
	namespace Logic
	{
		class CDocPartTypes : public WritingElementWithChilds<ComplexTypes::Word::String>
		{
		public:
			WritingElement_AdditionConstructors(CDocPartTypes)
			CDocPartTypes(OOX::Document *pMain = NULL) : WritingElementWithChilds<ComplexTypes::Word::String>(pMain)
			{
			}
			virtual ~CDocPartTypes() {}

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const
			{
				return et_w_docPartTypes;
			}
			
			nullable_bool m_oAll;
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_ReadSingle(oReader, L"w:all", m_oAll)
				WritingElement_ReadAttributes_End(oReader)
			}
		};
		class CDocPartBehaviors : public WritingElementWithChilds<ComplexTypes::Word::CDocPartBehavior>
		{
		public:
			WritingElement_AdditionConstructors(CDocPartBehaviors)
			CDocPartBehaviors(OOX::Document *pMain = NULL) : WritingElementWithChilds<ComplexTypes::Word::CDocPartBehavior>(pMain)
			{
			}
			virtual ~CDocPartBehaviors() {}

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const
			{
				return et_w_docPartBehaviors;
			}
		};
		class CDocPartCategory : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDocPartCategory)
			CDocPartCategory(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CDocPartCategory() {}

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const
			{
				return et_w_docPartCategory;
			}

			nullable<ComplexTypes::Word::String>			m_oName;
			nullable<ComplexTypes::Word::CDocPartGallery>	m_oGallery;
		};
		class CDocPartPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDocPartPr)
			CDocPartPr(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CDocPartPr() {}

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const
			{
				return et_w_docPartPr;
			}

			nullable<ComplexTypes::Word::String>	m_oName;
			nullable<ComplexTypes::Word::String>	m_oDescription;
			nullable<ComplexTypes::Word::String>	m_oGuid;
			nullable<ComplexTypes::Word::String>	m_oStyle;
			nullable<CDocPartCategory>				m_oCategory;
			nullable<CDocPartTypes>					m_oTypes;
			nullable<CDocPartBehaviors>				m_oBehaviors;
		};
		class CDocPartBody : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CDocPartBody)
			CDocPartBody(OOX::Document *pMain = NULL) : WritingElementWithChilds<>(pMain)
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return et_w_docPartBody;
			}
		};
		class CDocPart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDocPart)
			CDocPart(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CDocPart() {}

			virtual std::wstring toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType() const
			{
				return et_w_docPart;
			}
			nullable<CDocPartPr>	m_oDocPartPr;
			nullable<CDocPartBody>	m_oDocPartBody;
		};
		class CDocParts : public WritingElementWithChilds<CDocPart>
		{
		public:
			WritingElement_AdditionConstructors(CDocParts)
			CDocParts(OOX::Document *pMain = NULL) : WritingElementWithChilds<CDocPart>(pMain)
			{
			}
			virtual ~CDocParts()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_w_docParts;
			}
		};
	} // namespace Logic
} // namespace OOX

