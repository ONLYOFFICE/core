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

#include "../Drawing/Drawing.h"
#include "../../Common/SimpleTypes_Word.h"

#include "VmlOfficeDrawing.h"
#include "Vml.h"

namespace OOX
{
	namespace Logic
	{
		class CBinData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBinData)
			CBinData(OOX::Document *pMain = NULL) : WritingElement(pMain) {}
			virtual ~CBinData() {}

            virtual void fromXML(XmlUtils::CXmlNode &oNode)
			{
			}

            virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sData = oReader.GetText2A();
			}

            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:binData ";

				ComplexTypes_WriteAttribute2( L"w:name=\"", m_sName );

				sResult += L">";

				if (m_sData.IsInit())
				{
				}
				sResult += L"</w:binData>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_binData;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( L"w:name" == wsName )
						m_sName = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

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
			WritingElement_AdditionConstructors(CControl)
			CControl(OOX::Document *pMain = NULL) : WritingElement(pMain) {}
			virtual ~CControl() {}

			virtual void fromXML(XmlUtils::CXmlNode &oNode)
			{
				XmlMacroReadAttributeBase( oNode, L"r:id",      m_rId  );
				XmlMacroReadAttributeBase( oNode, L"w:name",    m_sName );
				XmlMacroReadAttributeBase( oNode, L"w:shapeid", m_sShapeId );

				if (false == m_rId.IsInit())
				{
					XmlMacroReadAttributeBase( oNode, L"relationships:id", m_rId );
				}
			}

            virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}

            virtual std::wstring toXML() const
			{
                std::wstring sResult = L"<w:control";

				ComplexTypes_WriteAttribute ( L" r:id=\"",      m_rId );
				ComplexTypes_WriteAttribute2( L" w:name=\"",    m_sName );
				ComplexTypes_WriteAttribute2( L" w:shapeid=\"", m_sShapeId );

				sResult += L"/>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_control;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"r:id",				m_rId )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"relationships:id",	m_rId )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:name",				m_sName )
					WritingElement_ReadAttributes_Read_else_if( oReader, L"w:shapeid",			m_sShapeId )
				WritingElement_ReadAttributes_End( oReader )
			}

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
			WritingElement_AdditionConstructors(CPicture)
			CPicture(OOX::Document *pMain = NULL) : WritingElementWithChilds<>(pMain) {}
			virtual ~CPicture() 
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode &oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromStringXML(const std::wstring & xml_string);

			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_w_pict;
			}

            nullable<std::wstring>          m_sXml;

			nullable<OOX::Logic::CControl>	m_oControl;
			nullable<OOX::Logic::CBinData>	m_oBinData;

			nullable<OOX::Vml::CShapeType>	m_oShapeType;
			nullable<OOX::WritingElement>	m_oShapeElement;
			
			// TO DO: Добавить класс, читающий movie
		};


		//--------------------------------------------------------------------------------
		// CObject 17.3.3.19 (Part 1)
		//--------------------------------------------------------------------------------	
		class CObject : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CObject)
			CObject(OOX::Document *pMain = NULL) : WritingElementWithChilds<>(pMain) {}
			virtual ~CObject() 
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_w_object;
			}

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

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, L"w:dxaOrig", m_oDxaOrig )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"w:dyaOrig", m_oDyaOrig )
				WritingElement_ReadAttributes_End( oReader )
			}
		};



	} // namespace Logic
} // namespace OOX
