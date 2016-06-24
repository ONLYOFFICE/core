/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#ifndef OOX_CELLSTYLES_FILE_INCLUDE_H_
#define OOX_CELLSTYLES_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//нереализован:
		//<extLst>
		class CCellStyle : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCellStyle)
			CCellStyle()
			{
			}
			virtual ~CCellStyle()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<cellStyle"));
				if(m_oName.IsInit())
				{
					//CString sVal;sVal.Format(_T(" name=\"%ls\""), XmlUtils::EncodeXmlString(m_oName.get()));
                    
                    CString sVal(_T(" name=\""));
                    sVal += XmlUtils::EncodeXmlString(m_oName.get());
                    sVal += CString(_T("\""));
                    
					writer.WriteString(sVal);
				}
				if(m_oXfId.IsInit())
				{
					CString sVal;sVal.Format(_T(" xfId=\"%d\""), m_oXfId->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oBuiltinId.IsInit())
				{
					CString sVal;sVal.Format(_T(" builtinId=\"%d\""), m_oBuiltinId->GetValue());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_CellStyle;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("builtinId"),      m_oBuiltinId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("customBuiltin"),      m_oCustomBuiltin )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("hidden"),      m_oHidden )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("iLevel"),      m_oILevel )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),      m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("xfId"),      m_oXfId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oBuiltinId;
			nullable<SimpleTypes::COnOff<>>					m_oCustomBuiltin;
			nullable<SimpleTypes::COnOff<>>					m_oHidden;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oILevel;
			nullable<CString>								m_oName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oXfId;
		};

		class CCellStyles : public WritingElementWithChilds<CCellStyle>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CCellStyles)
			CCellStyles()
			{
			}
			virtual ~CCellStyles()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<cellStyles"));
				if(m_oCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
					writer.WriteString(sVal);
				}
				writer.WriteString(_T(">"));
				for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
					m_arrItems[i]->toXML(writer);
				writer.WriteString(_T("</cellStyles>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("cellStyle") == sName )
						m_arrItems.push_back( new CCellStyle( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_CellStyles;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_ReadSingle ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_CELLSTYLES_FILE_INCLUDE_H_