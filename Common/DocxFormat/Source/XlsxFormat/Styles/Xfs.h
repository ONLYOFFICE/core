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

#include "../CommonInclude.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CAligment : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAligment)
            WritingElement_XlsbConstructors(CAligment)
			CAligment()
			{
			}
			virtual ~CAligment()
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
				toXMLWithNS(writer, L"", L"alignment", L"");
			}
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				writer.StartNodeWithNS(node_ns, node_name);
				writer.StartAttributes();
					WritingStringNullableAttrString(L"horizontal", m_oHorizontal, m_oHorizontal->ToString());
					WritingStringNullableAttrInt(L"indent", m_oIndent, *m_oIndent);
					WritingStringNullableAttrBool(L"justifyLastLine", m_oJustifyLastLine);
					WritingStringNullableAttrInt(L"readingOrder", m_oReadingOrder, *m_oReadingOrder);
					WritingStringNullableAttrInt(L"relativeIndent", m_oRelativeIndent, *m_oRelativeIndent);
					WritingStringNullableAttrBool(L"shrinkToFit", m_oShrinkToFit);
					WritingStringNullableAttrInt(L"textRotation", m_oTextRotation, *m_oTextRotation);
					WritingStringNullableAttrString(L"vertical", m_oVertical, m_oVertical->ToString());
					WritingStringNullableAttrBool(L"wrapText", m_oWrapText);
				writer.EndAttributesAndNode();
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_Aligment;
			}
			bool IsEmpty()
			{
				return !(m_oHorizontal.IsInit() || m_oIndent.IsInit() || m_oJustifyLastLine.IsInit() || m_oReadingOrder.IsInit() || m_oRelativeIndent.IsInit() ||
					m_oShrinkToFit.IsInit() || m_oTextRotation.IsInit() || (m_oVertical.IsInit() && SimpleTypes::Spreadsheet::verticalalignmentBottom != m_oVertical->GetValue()) || m_oWrapText.IsInit());
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				nullable_string readingOrder;
				nullable_double rotate;

				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if( oReader, _T("horizontal"),			m_oHorizontal )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("indent"),			m_oIndent )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("justifyLastLine"),	m_oJustifyLastLine )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("readingOrder"),	m_oReadingOrder )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("relativeIndent"),	m_oRelativeIndent )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("shrinkToFit"),		m_oShrinkToFit )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("textRotation"),	m_oTextRotation )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("vertical"),		m_oVertical )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("wrapText"),		m_oWrapText )
			// 2003
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:Horizontal"),	m_oHorizontal)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:Vertical"),		m_oVertical)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:WrapText"),		m_oWrapText)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:Indent"),		m_oIndent)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:ReadingOrder"),	readingOrder)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:Rotate"),		rotate)
					WritingElement_ReadAttributes_Read_else_if(oReader, _T("ss:ShrinkToFit"),	m_oShrinkToFit)
				WritingElement_ReadAttributes_End( oReader )

				if (readingOrder.IsInit())
				{
					if (*readingOrder == L"Context")			m_oReadingOrder = (unsigned int)0;
					else if (*readingOrder == L"LeftToRight")	m_oReadingOrder = (unsigned int)1;
					else if (*readingOrder == L"RightToLeft")	m_oReadingOrder = (unsigned int)2;
				}
				if (rotate.IsInit())
				{
					if (*rotate >= 0)
						m_oTextRotation = (unsigned int)(*rotate);
					else
						m_oTextRotation = (unsigned int)(90 - *rotate);
				}
			}

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptrBiff = dynamic_cast<XLS::BiffRecord*>(obj.get());
                if(ptrBiff->getTypeId() == XLSB::rt_XF)
                {
                    auto ptr = static_cast<XLS::XF*>(obj.get());
                    if(ptr != nullptr)
                    {
                        m_oIndent              = ptr->cIndent;
                        m_oJustifyLastLine     = ptr->fJustLast;
                        m_oReadingOrder        = ptr->iReadOrder;
                        m_oRelativeIndent      = ptr->iReadOrder;
                        m_oShrinkToFit         = ptr->fShrinkToFit;
                        m_oTextRotation        = ptr->trot;
                        m_oWrapText            = ptr->fWrap;

                        switch(ptr->alc)
                        {
                            case 0:
                                m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentGeneral; break;
                            case 1:
                                m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentLeft; break;
                            case 2:
                                m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentCenter; break;
                            case 3:
                                m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentRight; break;
                            case 4:
                                m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentFill; break;
                            case 5:
                                m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentJustify; break;
                            case 6:
                                m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentCenterContinuous; break;
                            case 7:
                                m_oHorizontal = SimpleTypes::Spreadsheet::EHorizontalAlignment::horizontalalignmentDistributed; break;
                        }

                        switch(ptr->alcV)
                        {
                            case 0:
                                m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentTop; break;
                            case 1:
                                m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentCenter; break;
                            case 2:
                                m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentBottom; break;
                            case 3:
                                m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentJustify; break;
                            case 4:
                                m_oVertical = SimpleTypes::Spreadsheet::EVerticalAlignment::verticalalignmentDistributed; break;
                        }
                    }
                }
            }

		public:
			nullable<SimpleTypes::Spreadsheet::CHorizontalAlignment<>>		m_oHorizontal;
			nullable_uint													m_oIndent;
			nullable<SimpleTypes::COnOff<>>									m_oJustifyLastLine;
			nullable_uint													m_oReadingOrder; //todooo to simple
			nullable_int													m_oRelativeIndent;
			nullable<SimpleTypes::COnOff<>>									m_oShrinkToFit;
			nullable_uint													m_oTextRotation;
			nullable<SimpleTypes::Spreadsheet::CVerticalAlignment<>>		m_oVertical;
			nullable<SimpleTypes::COnOff<>>									m_oWrapText;
		};
		class CProtection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CProtection)
            WritingElement_XlsbConstructors(CProtection)
			CProtection()
			{
			}
			virtual ~CProtection()
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
				toXMLWithNS(writer, L"", L"protection", L"");
			}
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				writer.WriteString(L"<" + node_ns + node_name);
				WritingStringNullableAttrBool(L"hidden", m_oHidden);
				WritingStringNullableAttrBool(L"locked", m_oLocked);
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);
            }

			virtual EElementType getType () const
			{
				return et_x_Protection;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("hidden"), m_oHidden )
					WritingElement_ReadAttributes_Read_else_if ( oReader, _T("locked"), m_oLocked )
				WritingElement_ReadAttributes_End( oReader )
			}

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::XF*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oHidden     = ptr->fHidden;
                    m_oLocked     = ptr->fLocked;
                }
            }

		public:
			nullable<SimpleTypes::COnOff<>> m_oHidden;
			nullable<SimpleTypes::COnOff<>> m_oLocked;
		};
		//нереализован:
		//<extLst>
		class CXfs : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CXfs)
            WritingElement_XlsbConstructors(CXfs)
				CXfs()
			{
			}
			virtual ~CXfs()
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
				writer.WriteString(_T("<xf"));
				WritingStringNullableAttrInt(L"fontId", m_oFontId, m_oFontId->GetValue());
				WritingStringNullableAttrInt(L"fillId", m_oFillId, m_oFillId->GetValue());
				WritingStringNullableAttrInt(L"borderId", m_oBorderId, m_oBorderId->GetValue());
				WritingStringNullableAttrInt(L"numFmtId", m_oNumFmtId, m_oNumFmtId->GetValue());
				WritingStringNullableAttrInt(L"xfId", m_oXfId, m_oXfId->GetValue());
				WritingStringNullableAttrBool(L"applyNumberFormat", m_oApplyNumberFormat);
				WritingStringNullableAttrBool(L"applyFont", m_oApplyFont);
				WritingStringNullableAttrBool(L"applyFill", m_oApplyFill);
				WritingStringNullableAttrBool(L"applyBorder", m_oApplyBorder);
				WritingStringNullableAttrBool(L"applyAlignment", m_oApplyAlignment);
				WritingStringNullableAttrBool(L"applyProtection", m_oApplyProtection);
				WritingStringNullableAttrBool(L"quotePrefix", m_oQuotePrefix);
				WritingStringNullableAttrBool(L"pivotButton", m_oPivotButton);
				if (m_oAligment.IsInit() || m_oProtection.IsInit())
				{
					writer.WriteString(_T(">"));

					if (m_oAligment.IsInit())m_oAligment->toXML(writer);
					if (m_oProtection.IsInit())m_oProtection->toXML(writer);

					writer.WriteString(_T("</xf>"));
				}
				else
					writer.WriteString(_T("/>"));
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

					if ( _T("alignment") == sName )
						m_oAligment = oReader;
					else if( _T("protection") == sName )
						m_oProtection = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Xfs;
			}

            void fromBin(XLS::BaseObjectPtr& obj)
            {
                ReadAttributes(obj);

                m_oAligment     = obj;
                m_oProtection   = obj;
            }

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyAlignment"),      m_oApplyAlignment )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyBorder"),      m_oApplyBorder )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyFill"),      m_oApplyFill )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyFont"),      m_oApplyFont )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyNumberFormat"),      m_oApplyNumberFormat )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("applyProtection"),      m_oApplyProtection )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("borderId"),      m_oBorderId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("fillId"),      m_oFillId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("fontId"),      m_oFontId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("numFmtId"),      m_oNumFmtId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("pivotButton"),      m_oPivotButton )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("quotePrefix"),      m_oQuotePrefix )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("xfId"),      m_oXfId )
				WritingElement_ReadAttributes_End( oReader )
			}

            void ReadAttributes(XLS::BaseObjectPtr& obj)
            {
                auto ptr = static_cast<XLSB::XF*>(obj.get());
                if(ptr != nullptr)
                {
                    m_oBorderId     = ptr->ixBorder;
                    m_oFillId       = ptr->iFill;
                    m_oFontId       = ptr->font_index;
                    m_oNumFmtId     = ptr->ifmt;
                    m_oPivotButton  = ptr->fsxButton;
                    m_oQuotePrefix  = ptr->f123Prefix;

                    if(ptr->ixfParent != 0xFFFF)
                        m_oXfId     = ptr->ixfParent;

                    m_oApplyAlignment       = ptr->fAtrAlc;
                    m_oApplyBorder          = ptr->fAtrBdr;
                    m_oApplyFill            = ptr->fAtrPat;
                    m_oApplyFont            = ptr->fAtrFnt;
                    m_oApplyNumberFormat    = ptr->fAtrNum;
                    m_oApplyProtection      = ptr->fAtrProt;
                }
            }

		public:
			nullable<SimpleTypes::COnOff<>>					m_oApplyAlignment;
			nullable<SimpleTypes::COnOff<>>					m_oApplyBorder;
			nullable<SimpleTypes::COnOff<>>					m_oApplyFill;
			nullable<SimpleTypes::COnOff<>>					m_oApplyFont;
			nullable<SimpleTypes::COnOff<>>					m_oApplyNumberFormat;
			nullable<SimpleTypes::COnOff<>>					m_oApplyProtection;

			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oBorderId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oFillId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oFontId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oNumFmtId;
			nullable<SimpleTypes::COnOff<>>					m_oPivotButton;
			nullable<SimpleTypes::COnOff<>>					m_oQuotePrefix;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oXfId;

			nullable<CAligment>								m_oAligment;
			nullable<CProtection>							m_oProtection;
			
		};

		class CCellXfs  : public WritingElementWithChilds<CXfs>
		{
		public:
			WritingElement_AdditionConstructors(CCellXfs)
            WritingElement_XlsbVectorConstructors(CCellXfs)
			CCellXfs()
			{
			}
			virtual ~CCellXfs()
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
				writer.WriteString(_T("<cellXfs"));
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				writer.WriteString(_T(">"));
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				writer.WriteString(_T("</cellXfs>"));
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

					if ( _T("xf") == sName )
						m_arrItems.push_back( new CXfs( oReader ));
				}
			}

            void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
            {
                ReadAttributes(obj);

                for(auto &xfs : obj)
                {
                    CXfs *pXfs = new CXfs(xfs);
                    m_arrItems.push_back(pXfs);
                }
            }

			virtual EElementType getType () const
			{
				return et_x_CellXfs;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
            {
                m_oCount = (_UINT32)obj.size();
            }
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
		class CCellStyleXfs  : public WritingElementWithChilds<CXfs>
		{
		public:
			WritingElement_AdditionConstructors(CCellStyleXfs)
            WritingElement_XlsbVectorConstructors(CCellStyleXfs)
			CCellStyleXfs()
			{
			}
			virtual ~CCellStyleXfs()
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
				writer.WriteString(_T("<cellStyleXfs"));
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				writer.WriteString(_T(">"));
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				
				writer.WriteString(_T("</cellStyleXfs>"));
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

					if ( _T("xf") == sName )
						m_arrItems.push_back( new CXfs( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_CellStyleXfs;
			}

            void fromBin(std::vector<XLS::BaseObjectPtr>& obj)
            {
                ReadAttributes(obj);

                for(auto &xfs : obj)
                {
                    CXfs *pXfs = new CXfs(xfs);
                    m_arrItems.push_back(pXfs);
                }
            }

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}
            void ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
            {
                m_oCount = (_UINT32)obj.size();
            }
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
	} //Spreadsheet
} // namespace OOX

