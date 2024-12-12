/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "Cols.h"
#include "../XlsxFlat.h"

#include "../../XlsbFormat/Biff12_unions/COLINFOS.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheWriter.h"

namespace OOX
{
	namespace Spreadsheet
	{
			CCol::CCol(OOX::Document *pMain) : WritingElement(pMain)
			{
			}
			CCol::~CCol()
			{
			}
			void CCol::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CCol::toXML() const
			{
				return _T("");
			}
			void CCol::toXML(NSStringUtils::CStringBuilder& writer) const
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
			void CCol::fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes(oReader);

				if (!oReader.IsEmptyNode())
					oReader.ReadTillEnd();
			}
			void CCol::fromBin(XLS::BaseObjectPtr& obj)
			{
				ReadAttributes(obj);
			}
			XLS::BaseObjectPtr CCol::toBin()
			{
				auto castedPtr = new XLSB::ColInfo;
				XLS::BaseObjectPtr ptr(castedPtr);
				if(m_oBestFit.IsInit())
					castedPtr->fBestFit = m_oBestFit->ToBool();
				else
					castedPtr->fBestFit = false;
				if(m_oCollapsed.IsInit())
					castedPtr->fCollapsed = m_oCollapsed->ToBool();
				else
					castedPtr->fCollapsed = false;
				if(m_oCustomWidth.IsInit())
					castedPtr->fUserSet = m_oCustomWidth->ToBool();
				else
					castedPtr->fUserSet = false;
				if(m_oHidden.IsInit())
					castedPtr->fHidden = m_oHidden->ToBool();
				else
					castedPtr->fHidden = false;
				if(m_oMax.IsInit())
					castedPtr->colLast = m_oMax->m_eValue - 1;
				else
                    castedPtr->colLast = 16383;
				if(m_oMin.IsInit())
					castedPtr->colFirst  = m_oMin->m_eValue - 1;
				else
					castedPtr->colFirst = 0;
				if(m_oOutlineLevel.IsInit())
					castedPtr->iOutLevel =  m_oOutlineLevel->m_eValue;
				else
					castedPtr->iOutLevel = false;
				if(m_oPhonetic.IsInit())
					castedPtr->fPhonetic = m_oPhonetic->ToBool();
				else
					castedPtr->fPhonetic = false;
				if(m_oStyle.IsInit())
					castedPtr->ixfeXLSB = m_oStyle->m_eValue;
                else
                    castedPtr->ixfeXLSB = 0;

				if (m_oWidth.IsInit())
				{
					if(m_oWidth->GetValue() > 0)
						castedPtr->coldx           = m_oWidth->GetValue() * 256;
				}
				else
					castedPtr->coldx = 2304; ///standart col width(9) * 256
				return ptr;
			}
            void CCol::toBin(XLS::StreamCacheWriterPtr& writer)
            {
                auto record = writer->getNextRecord(XLSB::rt_ColInfo);
                {
                    _UINT32 colNum = 0;
                    if(m_oMin.IsInit())
                        colNum  = m_oMin->m_eValue - 1;
                    *record << colNum;
                    if(m_oMax.IsInit())
                        colNum = m_oMax->m_eValue - 1;
                    else
                        colNum = 16383;
                    *record << colNum;
                }
                {
                    _UINT32 coldx = 2304; ///standart col width(9) * 256
                    if (m_oWidth.IsInit())
                        if(m_oWidth->GetValue() > 0)
                            coldx = m_oWidth->GetValue() * 256;
                    *record << coldx;
                }
                {
                    _UINT32 ixfe = 0;
                    if(m_oStyle.IsInit())
                        ixfe = m_oStyle->m_eValue;
                    *record << ixfe;
                }
                {
                    _UINT16		flags = 0;
                    if(m_oHidden.IsInit())
                        SETBIT(flags, 0, m_oHidden->ToBool())
                    if(m_oCustomWidth.IsInit())
                        SETBIT(flags, 1, m_oCustomWidth->ToBool())
                    if(m_oBestFit.IsInit())
                        SETBIT(flags, 2, m_oBestFit->ToBool())
                    if(m_oPhonetic.IsInit())
                        SETBIT(flags, 3, m_oPhonetic->ToBool())
                    if(m_oOutlineLevel.IsInit())
                        SETBITS(flags, 8, 10, m_oOutlineLevel->m_eValue)
                    if(m_oCollapsed.IsInit())
                        SETBIT(flags, 12, m_oCollapsed->ToBool())
                    *record << flags;
                }
                writer->storeNextRecord(record);
            }
			EElementType CCol::getType() const
			{
				return et_x_Col;
			}
			void CCol::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				CXlsxFlat* xlsx_flat = dynamic_cast<CXlsxFlat*>(m_pMainDocument);

				nullable_double ptWidth;
				nullable_bool bAutoFit;
				nullable_string sStyleID;

				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, _T("bestFit"), m_oBestFit)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("collapsed"), m_oCollapsed)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("customWidth"), m_oCustomWidth)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("hidden"), m_oHidden)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("max"), m_oMax)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("min"), m_oMin)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("outlineLevel"), m_oOutlineLevel)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("phonetic"), m_oPhonetic)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("style"), m_oStyle)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("width"), m_oWidth)

					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:Width"), ptWidth)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:AutoFitWidth"), bAutoFit)

					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:StyleID"), sStyleID)
					WritingElement_ReadAttributes_End(oReader)

					if (ptWidth.IsInit() && xlsx_flat)
					{
						m_oWidth.Init();
						double pixDpi = *ptWidth / 72.0 * 96.; if (pixDpi < 5) pixDpi = 7; // ~
						double maxDigitSize = xlsx_flat->getMaxDigitSize().first;

						m_oWidth->SetValue(((int)((pixDpi + 5) / maxDigitSize * 100. + 0.5)) / 100.);

						m_oCustomWidth.Init();
						m_oCustomWidth->FromBool(true);
					}

				if (bAutoFit.IsInit() && (*bAutoFit == false))
				{
				}
				else if (xlsx_flat)
				{
					m_oBestFit.Init();
					m_oBestFit->FromBool(true);

					if (false == ptWidth.IsInit())
					{
						m_oCustomWidth.Init();
						m_oCustomWidth->FromBool(true);

						m_oWidth.Init();
						m_oWidth->SetValue(9);
					}
				}
			}
			void CCol::ReadAttributes(XLS::BaseObjectPtr& obj)
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

			CCols::CCols(OOX::Document *pMain) : WritingElementWithChilds<CCol>(pMain)
			{
			}
			CCols::~CCols()
			{
			}
			void CCols::fromXML(XmlUtils::CXmlNode& node)
			{
			}
			std::wstring CCols::toXML() const
			{
				return _T("");
			}
			void CCols::toXML(NSStringUtils::CStringBuilder& writer) const
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
			void CCols::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			void CCols::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
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
			std::vector<XLS::BaseObjectPtr> CCols::toBin()
			{
				std::vector<XLS::BaseObjectPtr> ptrVector;
				for(auto i:m_arrItems)
				{
					ptrVector.push_back(i->toBin());
				}
				return ptrVector;
			}
            void CCols::toBin(XLS::StreamCacheWriterPtr& writer)
            {
                {
                    auto begin = writer->getNextRecord(XLSB::rt_BeginColInfos);
                    writer->storeNextRecord(begin);
                }
                for(auto i:m_arrItems)
                {
                    i->toBin(writer);
                }
                {
                    auto end = writer->getNextRecord(XLSB::rt_EndColInfos);
                    writer->storeNextRecord(end);
                }
            }
			EElementType CCols::getType () const
			{
				return et_x_Cols;
			}
			void CCols::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
