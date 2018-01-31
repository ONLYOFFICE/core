﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef OOX_SHEETS_FILE_INCLUDE_H_
#define OOX_SHEETS_FILE_INCLUDE_H_

#include "../CommonInclude.h"


namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CSheet : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSheet)
			CSheet()
			{
			}
			virtual ~CSheet()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<sheet"));
				WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
				WritingStringNullableAttrInt(L"sheetId", m_oSheetId, m_oSheetId->GetValue());
				WritingStringNullableAttrString(L"state", m_oState, m_oState->ToString());
				WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_Sheet;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),	m_oRid )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),	m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("sheetId"),	m_oSheetId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("state"),	m_oState )

					WritingElement_ReadAttributes_End( oReader )
			}

		public:
				nullable<SimpleTypes::CRelationshipId>				m_oRid;
				nullable<std::wstring>								m_oName;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oSheetId;
				nullable<SimpleTypes::Spreadsheet::CVisibleType<>>	m_oState;

		};

		class CSheets  : public WritingElementWithChilds<CSheet>
		{
		public:
			WritingElement_AdditionConstructors(CSheets)
			CSheets()
			{
			}
			virtual ~CSheets()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(_T("<sheets>"));
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				
				writer.WriteString(_T("</sheets>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				
				int index = 0;
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("sheet") == sName )
					{
						CSheet* pSheet = new CSheet( oReader );
						
						m_arrItems.push_back( pSheet );
						mapSheets.insert(std::make_pair(index++, pSheet));
					}

				}
			}

			virtual EElementType getType () const
			{
				return et_x_Sheets;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			std::map<int, OOX::Spreadsheet::CSheet*> mapSheets;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_SHEETS_FILE_INCLUDE_H_
