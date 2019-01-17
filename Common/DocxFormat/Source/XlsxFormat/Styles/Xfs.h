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
#ifndef OOX_XFS_FILE_INCLUDE_H_
#define OOX_XFS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CAligment : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAligment)
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
				writer.WriteString(_T("<alignment"));
				WritingStringNullableAttrString(L"horizontal", m_oHorizontal, m_oHorizontal->ToString());
				WritingStringNullableAttrInt(L"indent", m_oIndent, m_oIndent->GetValue());
				WritingStringNullableAttrBool(L"justifyLastLine", m_oJustifyLastLine);
				WritingStringNullableAttrInt(L"readingOrder", m_oReadingOrder, m_oReadingOrder->GetValue());
				WritingStringNullableAttrInt(L"relativeIndent", m_oRelativeIndent, m_oRelativeIndent->GetValue());
				WritingStringNullableAttrBool(L"shrinkToFit", m_oShrinkToFit);
				WritingStringNullableAttrInt(L"textRotation", m_oTextRotation, m_oTextRotation->GetValue());
				WritingStringNullableAttrString(L"vertical", m_oVertical, m_oVertical->ToString());
				WritingStringNullableAttrBool(L"wrapText", m_oWrapText);
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
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("horizontal"),      m_oHorizontal )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("indent"),      m_oIndent )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("justifyLastLine"),      m_oJustifyLastLine )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("readingOrder"),      m_oReadingOrder )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("relativeIndent"),      m_oRelativeIndent )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("shrinkToFit"),      m_oShrinkToFit )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("textRotation"),      m_oTextRotation )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("vertical"),      m_oVertical )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("wrapText"),      m_oWrapText )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CHorizontalAlignment<>>		m_oHorizontal;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>					m_oIndent;
			nullable<SimpleTypes::COnOff<>>									m_oJustifyLastLine;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>					m_oReadingOrder;
			nullable<SimpleTypes::CDecimalNumber<>>							m_oRelativeIndent;
			nullable<SimpleTypes::COnOff<>>									m_oShrinkToFit;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>					m_oTextRotation;
			nullable<SimpleTypes::Spreadsheet::CVerticalAlignment<>>		m_oVertical;
			nullable<SimpleTypes::COnOff<>>									m_oWrapText;
		};
		class CProtection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CProtection)
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
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_Protection;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("hidden"),      m_oHidden )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("locked"),      m_oLocked )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<>>									m_oHidden;
			nullable<SimpleTypes::COnOff<>>									m_oLocked;
		};
		//нереализован:
		//<extLst>
		class CXfs : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CXfs)
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
				WritingStringNullableAttrBool(L"quotePrefix", m_oQuotePrefix);
				WritingStringNullableAttrBool(L"pivotButton", m_oPivotButton);
				if(m_oAligment.IsInit())
				{
					writer.WriteString(_T(">"));
					m_oAligment->toXML(writer);
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

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
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
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
		class CCellStyleXfs  : public WritingElementWithChilds<CXfs>
		{
		public:
			WritingElement_AdditionConstructors(CCellStyleXfs)
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

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_XFS_FILE_INCLUDE_H_
