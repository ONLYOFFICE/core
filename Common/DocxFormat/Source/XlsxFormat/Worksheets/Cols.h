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
#ifndef OOX_COLS_FILE_INCLUDE_H_
#define OOX_COLS_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "../../XlsbFormat/Biff12_unions/COLINFOS.h"

namespace OOX
{
	namespace Spreadsheet
	{
		//необработано:
		//<extLst>
		class CCol : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCol)
				CCol(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CCol()
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
				writer.WriteString(_T("<col"));
				WritingStringNullableAttrBool(L"bestFit", m_oBestFit);
				WritingStringNullableAttrBool(L"collapsed", m_oCollapsed);
				WritingStringNullableAttrBool(L"customWidth", m_oCustomWidth);
				WritingStringNullableAttrBool(L"hidden", m_oHidden);
				WritingStringNullableAttrInt(L"min", m_oMin, m_oMin->GetValue());
				WritingStringNullableAttrInt(L"max", m_oMax, m_oMax->GetValue());
				WritingStringNullableAttrInt(L"outlineLevel", m_oOutlineLevel, m_oOutlineLevel->GetValue());
				WritingStringNullableAttrBool(L"phonetic", m_oPhonetic);
				WritingStringNullableAttrInt(L"style", m_oStyle, m_oStyle->GetValue());
				WritingStringNullableAttrDouble(L"width", m_oWidth, m_oWidth->GetValue());
				writer.WriteString(_T("/>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			virtual EElementType getType() const
			{
				return et_x_Col;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader); // -> Worksheet.cpp

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::ColInfo*>(obj.get());
                m_oBestFit                  = ptr->fBestFit;
                m_oCollapsed                = ptr->fCollapsed;
                m_oCustomWidth              = ptr->fUserSet;
                m_oHidden                   = ptr->fHidden;
                m_oMax                      = ptr->colLast + 1;
                m_oMin                      = ptr->colFirst + 1;
                m_oOutlineLevel             = ptr->iOutLevel;
                m_oPhonetic                 = ptr->fPhonetic;
                m_oStyle                    = ptr->ixfeXLSB;

                if (ptr->coldx > 0)
                {
                        m_oWidth            = (double)ptr->coldx / 256.;
                }

            }

		public:
				nullable<SimpleTypes::COnOff<>>					m_oBestFit;
				nullable<SimpleTypes::COnOff<>>					m_oCollapsed;
				nullable<SimpleTypes::COnOff<>>					m_oCustomWidth;
				nullable<SimpleTypes::COnOff<>>					m_oHidden;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMax;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMin;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevel;
				nullable<SimpleTypes::COnOff<>>					m_oPhonetic;
				nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oStyle;
				nullable<SimpleTypes::CDouble>					m_oWidth;
		};

		class CCols  : public WritingElementWithChilds<CCol>
		{
		public:
			WritingElement_AdditionConstructors(CCols)
            WritingElement_XlsbVectorConstructors(CCols)
			CCols(OOX::Document *pMain = NULL) : WritingElementWithChilds<CCol>(pMain)
			{
			}
			virtual ~CCols()
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
				if(m_arrItems.empty()) return;

				writer.WriteString(_T("<cols>"));
					
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
                    if ( m_arrItems[i] )
					{
                        m_arrItems[i]->toXML(writer);
					}
				}
				
				writer.WriteString(_T("</cols>"));
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("col") == sName )
					{
						CCol *pCol = new CCol(m_pMainDocument);
						pCol->fromXML(oReader);

						m_arrItems.push_back(pCol);
					}
				}
			}
            void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
            {
                //ReadAttributes(obj);

                if (obj.empty())
                    return;

                for(auto &COLINFOS : obj)
                {
                    auto ptr = static_cast<XLSB::COLINFOS*>(COLINFOS.get());
                    for(auto &col : ptr->m_arBrtColInfo)
                    {
                        CCol *pCol = new CCol(m_pMainDocument);
                        pCol->fromBin(col);

                        m_arrItems.push_back(pCol);
                    }
                }
            }

			virtual EElementType getType () const
			{
				return et_x_Cols;
			}
		
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_COLS_FILE_INCLUDE_H_
