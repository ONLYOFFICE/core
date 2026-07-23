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

#include "../Drawing/Drawing.h"
#include "./Vml.h"

namespace OOX
{
	//namespace Vml
	//{
	//	class CShape;
	//	class CShapeType;
	//}
	//namespace VmlOffice
	//{
	//	class COLEObject;
	//}

	namespace Logic
	{
		class CBinData : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CBinData)

			CBinData(OOX::Document *pMain = NULL);
			virtual ~CBinData();

			virtual void fromXML(XmlUtils::CXmlNode &oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			std::vector<BYTE> GetBytes();
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
            nullable<std::wstring>		m_sName;
			nullable<std::string>		m_sData;
		};

		//--------------------------------------------------------------------------------
		// CControl 9.2.2.1 (Part 4)
		//--------------------------------------------------------------------------------	
		class CControl : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CControl)

			CControl(OOX::Document *pMain = NULL);
			virtual ~CControl();

			virtual void fromXML(XmlUtils::CXmlNode &oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
            nullable<SimpleTypes::CRelationshipId>      m_rId;
            nullable<std::wstring>                      m_sName;
            nullable<std::wstring>                      m_sShapeId;
		};

		//--------------------------------------------------------------------------------
		// CPicture 9.2.2.2 (Part 4)
		//--------------------------------------------------------------------------------	
		class CPicture : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CPicture)

			CPicture(OOX::Document *pMain = NULL);
			virtual ~CPicture();

			virtual void fromXML(XmlUtils::CXmlNode &oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromStringXML(const std::wstring & xml_string);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

            nullable<std::wstring>          m_sXml;

			nullable<OOX::Logic::CControl>	m_oControl;
			nullable<OOX::Logic::CBinData>	m_oBinData;

			nullable<OOX::Vml::CShapeType> m_oShapeType;
			nullable<OOX::WritingElement> m_oShapeElement;
			nullable<OOX::VmlOffice::COLEObject> m_oOLEObject;
			
			// TO DO: Add class for reading movie
		};

		//--------------------------------------------------------------------------------
		// CObject 17.3.3.19 (Part 1)
		//--------------------------------------------------------------------------------	
		class CObject : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionMethods(CObject)

			CObject(OOX::Document *pMain = NULL);
			virtual ~CObject();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

            nullable<std::wstring>					m_sXml;
//-----------------------------------------------------------------------
			nullable_int							m_oDxaOrig;
			nullable_int							m_oDyaOrig;

			nullable<OOX::Logic::CControl>			m_oControl;
			nullable<OOX::Logic::CDrawing>			m_oDrawing;

			nullable<OOX::Vml::CShapeType>			m_oShapeType;
			nullable<OOX::VmlOffice::COLEObject>	m_oOleObject;
			
			nullable<OOX::Vml::CShape>				m_oShape;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

	} // namespace Logic
} // namespace OOX
