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
#ifndef OOX_OLEOBJECTS_FILE_INCLUDE_H_
#define OOX_OLEOBJECTS_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "../Drawing/FromTo.h"
#include "../../XlsbFormat/Biff12_unions/OLEOBJECTS.h"
#include "../../XlsbFormat/Biff12_records/OleObject.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CExtAnchor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExtAnchor)
			CExtAnchor();
			virtual ~CExtAnchor();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_bool m_oMoveWithCells;
			nullable_bool m_oSizeWithCells;
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oZOrder;

			nullable<CFromTo> m_oFrom;
			nullable<CFromTo> m_oTo;
		};

		class COleObjectPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COleObjectPr)
			COleObjectPr();
			virtual ~COleObjectPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring>				m_oAltText;
			nullable<SimpleTypes::COnOff>		m_oAutoFill;
			nullable<SimpleTypes::COnOff>		m_oAutoLine;
			nullable<SimpleTypes::COnOff>		m_oAutoPict;
			nullable<SimpleTypes::COnOff>		m_oDde;
			nullable<SimpleTypes::COnOff>		m_oDefaultSize;
			nullable<SimpleTypes::COnOff>		m_oDisabled;
			nullable<SimpleTypes::CRelationshipId> m_oRid;
			nullable<SimpleTypes::COnOff>		m_oLocked;
			nullable<std::wstring>				m_oMacro;
			nullable<SimpleTypes::COnOff>		m_oPrint;
			nullable<SimpleTypes::COnOff>		m_oUiObject;

			nullable<CExtAnchor>			m_oAnchor;
		};

		class COleObject : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COleObject)
			WritingElement_XlsbConstructors(COleObject)
			COleObject();
			virtual ~COleObject();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, bool bObjectPr) const;

			void toXMLPptx(NSStringUtils::CStringBuilder& writer, std::wstring qqq);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<std::wstring >								m_oProgId;
			nullable<SimpleTypes::Spreadsheet::ST_DvAspect>		m_oDvAspect;
			nullable<std::wstring >								m_oLink;
			nullable<SimpleTypes::Spreadsheet::ST_OleUpdate>	m_oOleUpdate;
			nullable<SimpleTypes::COnOff>						m_oAutoLoad;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oShapeId;
			nullable<SimpleTypes::CRelationshipId>				m_oRid;
			nullable<COleObjectPr>								m_oObjectPr;
	//internal
			smart_ptr<OOX::OleObject>							m_OleObjectFile;
			nullable<SimpleTypes::CRelationshipId>				m_oRidImg;
		};

		class COleObjects : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COleObjects)
			WritingElement_XlsbConstructors(COleObjects)
			COleObjects();
			virtual ~COleObjects();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            boost::unordered_map<unsigned int, COleObject*> m_mapOleObjects;
		};

	} //Spreadsheet
} // namespace OOX

#endif // OOX_OLEOBJECTS_FILE_INCLUDE_H_
