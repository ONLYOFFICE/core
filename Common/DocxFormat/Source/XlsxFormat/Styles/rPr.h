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
#ifndef OOX_RPR_FILE_INCLUDE_H_
#define OOX_RPR_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace NSBinPptxRW
{
	class CBinaryFileReader;
	class CXlsbBinaryWriter;
}

namespace OOX
{
	namespace Spreadsheet
	{
		class CRgbColor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRgbColor)
			CRgbColor()
			{
			}
			virtual ~CRgbColor()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
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
				return et_x_RgbColor;
			}
		private:
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("rgb"),      m_oRgb )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CHexColor> m_oRgb;
		};
		
		class CIndexedColors : public WritingElementWithChilds<CRgbColor>
		{
		public:
			WritingElement_AdditionConstructors(CIndexedColors)
			CIndexedColors()
			{
			}
			virtual ~CIndexedColors()
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

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				int index = 0;
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("rgbColor") == sName )
					{
						CRgbColor* color = new CRgbColor( oReader );
						mapIndexedColors.insert(std::make_pair(index++, color));
						m_arrItems.push_back( color );
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_IndexedColors;
			}
			static bool GetDefaultRGBAByIndex(int index, unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA)
			{
				unA = 255;
				switch(index)
				{
				case 0 : unR = 0x00; unG = 0x00; unB = 0x00; break;
				case 1 : unR = 0xFF; unG = 0xFF; unB = 0xFF; break;
				case 2 : unR = 0xFF; unG = 0x00; unB = 0x00; break;
				case 3 : unR = 0x00; unG = 0xFF; unB = 0x00; break;
				case 4 : unR = 0x00; unG = 0x00; unB = 0xFF; break;

				case 5 : unR = 0xFF; unG = 0xFF; unB = 0x00; break;
				case 6 : unR = 0xFF; unG = 0x00; unB = 0xFF; break;
				case 7 : unR = 0x00; unG = 0xFF; unB = 0xFF; break;
				case 8 : unR = 0x00; unG = 0x00; unB = 0x00; break;
				case 9 : unR = 0xFF; unG = 0xFF; unB = 0xFF; break;

				case 10: unR = 0xFF; unG = 0x00; unB = 0x00; break;
				case 11: unR = 0x00; unG = 0xFF; unB = 0x00; break;
				case 12: unR = 0x00; unG = 0x00; unB = 0xFF; break;
				case 13: unR = 0xFF; unG = 0xFF; unB = 0x00; break;
				case 14: unR = 0xFF; unG = 0x00; unB = 0xFF; break;

				case 15: unR = 0x00; unG = 0xFF; unB = 0xFF; break;
				case 16: unR = 0x80; unG = 0x00; unB = 0x00; break;
				case 17: unR = 0x00; unG = 0x80; unB = 0x00; break;
				case 18: unR = 0x00; unG = 0x00; unB = 0x80; break;
				case 19: unR = 0x80; unG = 0x80; unB = 0x00; break;

				case 20: unR = 0x80; unG = 0x00; unB = 0x80; break;
				case 21: unR = 0x00; unG = 0x80; unB = 0x80; break;
				case 22: unR = 0xC0; unG = 0xC0; unB = 0xC0; break;
				case 23: unR = 0x80; unG = 0x80; unB = 0x80; break;
				case 24: unR = 0x99; unG = 0x99; unB = 0xFF; break;

				case 25: unR = 0x99; unG = 0x33; unB = 0x66; break;
				case 26: unR = 0xFF; unG = 0xFF; unB = 0xCC; break;
				case 27: unR = 0xCC; unG = 0xFF; unB = 0xFF; break;
				case 28: unR = 0x66; unG = 0x00; unB = 0x66; break;
				case 29: unR = 0xFF; unG = 0x80; unB = 0x80; break;

				case 30: unR = 0x00; unG = 0x66; unB = 0xCC; break;
				case 31: unR = 0xCC; unG = 0xCC; unB = 0xFF; break;
				case 32: unR = 0x00; unG = 0x00; unB = 0x80; break;
				case 33: unR = 0xFF; unG = 0x00; unB = 0xFF; break;
				case 34: unR = 0xFF; unG = 0xFF; unB = 0x00; break;

				case 35: unR = 0x00; unG = 0xFF; unB = 0xFF; break;
				case 36: unR = 0x80; unG = 0x00; unB = 0x80; break;
				case 37: unR = 0x80; unG = 0x00; unB = 0x00; break;
				case 38: unR = 0x00; unG = 0x80; unB = 0x80; break;
				case 39: unR = 0x00; unG = 0x00; unB = 0xFF; break;

				case 40: unR = 0x00; unG = 0xCC; unB = 0xFF; break;
				case 41: unR = 0xCC; unG = 0xFF; unB = 0xFF; break;
				case 42: unR = 0xCC; unG = 0xFF; unB = 0xCC; break;
				case 43: unR = 0xFF; unG = 0xFF; unB = 0x99; break;
				case 44: unR = 0x99; unG = 0xCC; unB = 0xFF; break;

				case 45: unR = 0xFF; unG = 0x99; unB = 0xCC; break;
				case 46: unR = 0xCC; unG = 0x99; unB = 0xFF; break;
				case 47: unR = 0xFF; unG = 0xCC; unB = 0x99; break;
				case 48: unR = 0x33; unG = 0x66; unB = 0xFF; break;
				case 49: unR = 0x33; unG = 0xCC; unB = 0xCC; break;

				case 50: unR = 0x99; unG = 0xCC; unB = 0x00; break;
				case 51: unR = 0xFF; unG = 0xCC; unB = 0x00; break;
				case 52: unR = 0xFF; unG = 0x99; unB = 0x00; break;
				case 53: unR = 0xFF; unG = 0x66; unB = 0x00; break;
				case 54: unR = 0x66; unG = 0x66; unB = 0x99; break;

				case 55: unR = 0x96; unG = 0x96; unB = 0x96; break;
				case 56: unR = 0x00; unG = 0x33; unB = 0x66; break;
				case 57: unR = 0x33; unG = 0x99; unB = 0x66; break;
				case 58: unR = 0x00; unG = 0x33; unB = 0x00; break;
				case 59: unR = 0x33; unG = 0x33; unB = 0x00; break;

				case 60: unR = 0x99; unG = 0x33; unB = 0x00; break;
				case 61: unR = 0x99; unG = 0x33; unB = 0x66; break;
				case 62: unR = 0x33; unG = 0x33; unB = 0x99; break;
				case 63: unR = 0x33; unG = 0x33; unB = 0x33; break;
				case 64: unR = 0x00; unG = 0x00; unB = 0x00; break;

				case 65: unR = 0xFF; unG = 0xFF; unB = 0xFF; break;
				default: return false;
				}
				return true;
			}
			static int GetDefaultIndexByRGBA( unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA)
			{
				if(255 != unA)
					return -1;
				int nIndex = -1;
				if(unR == 0x00 && unG == 0x00 && unB == 0x00)
					nIndex = 64;
				else if(unR == 0xFF && unG == 0xFF && unB == 0xFF)
					nIndex = 65;
				else if(unR == 0x00 && unG == 0x00 && unB == 0x00)
					nIndex = 0;
				else if(unR == 0xFF && unG == 0xFF && unB == 0xFF)
					nIndex = 1;
				else if(unR == 0xFF && unG == 0x00 && unB == 0x00)
					nIndex = 2;
				else if(unR == 0x00 && unG == 0xFF && unB == 0x00)
					nIndex = 3;
				else if(unR == 0x00 && unG == 0x00 && unB == 0xFF)
					nIndex = 4;
				else if(unR == 0xFF && unG == 0xFF && unB == 0x00)
					nIndex = 5;
				else if(unR == 0xFF && unG == 0x00 && unB == 0xFF)
					nIndex = 6;
				else if(unR == 0x00 && unG == 0xFF && unB == 0xFF)
					nIndex = 7;
				else if(unR == 0x00 && unG == 0x00 && unB == 0x00)
					nIndex = 8;
				else if(unR == 0xFF && unG == 0xFF && unB == 0xFF)
					nIndex = 9;
				else if(unR == 0xFF && unG == 0x00 && unB == 0x00)
					nIndex = 10;
				else if(unR == 0x00 && unG == 0xFF && unB == 0x00)
					nIndex = 11;
				else if(unR == 0x00 && unG == 0x00 && unB == 0xFF)
					nIndex = 12;
				else if(unR == 0xFF && unG == 0xFF && unB == 0x00)
					nIndex = 13;
				else if(unR == 0xFF && unG == 0x00 && unB == 0xFF)
					nIndex = 14;
				else if(unR == 0x00 && unG == 0xFF && unB == 0xFF)
					nIndex = 15;
				else if(unR == 0x80 && unG == 0x00 && unB == 0x00)
					nIndex = 16;
				else if(unR == 0x00 && unG == 0x80 && unB == 0x00)
					nIndex = 17;
				else if(unR == 0x00 && unG == 0x00 && unB == 0x80)
					nIndex = 18;
				else if(unR == 0x80 && unG == 0x80 && unB == 0x00)
					nIndex = 19;
				else if(unR == 0x80 && unG == 0x00 && unB == 0x80)
					nIndex = 20;
				else if(unR == 0x00 && unG == 0x80 && unB == 0x80)
					nIndex = 21;
				else if(unR == 0xC0 && unG == 0xC0 && unB == 0xC0)
					nIndex = 22;
				else if(unR == 0x80 && unG == 0x80 && unB == 0x80)
					nIndex = 23;
				else if(unR == 0x99 && unG == 0x99 && unB == 0xFF)
					nIndex = 24;
				else if(unR == 0x99 && unG == 0x33 && unB == 0x66)
					nIndex = 25;
				else if(unR == 0xFF && unG == 0xFF && unB == 0xCC)
					nIndex = 26;
				else if(unR == 0xCC && unG == 0xFF && unB == 0xFF)
					nIndex = 27;
				else if(unR == 0x66 && unG == 0x00 && unB == 0x66)
					nIndex = 28;
				else if(unR == 0xFF && unG == 0x80 && unB == 0x80)
					nIndex = 29;
				else if(unR == 0x00 && unG == 0x66 && unB == 0xCC)
					nIndex = 30;
				else if(unR == 0xCC && unG == 0xCC && unB == 0xFF)
					nIndex = 31;
				else if(unR == 0x00 && unG == 0x00 && unB == 0x80)
					nIndex = 32;
				else if(unR == 0xFF && unG == 0x00 && unB == 0xFF)
					nIndex = 33;
				else if(unR == 0xFF && unG == 0xFF && unB == 0x00)
					nIndex = 34;
				else if(unR == 0x00 && unG == 0xFF && unB == 0xFF)
					nIndex = 35;
				else if(unR == 0x80 && unG == 0x00 && unB == 0x80)
					nIndex = 36;
				else if(unR == 0x80 && unG == 0x00 && unB == 0x00)
					nIndex = 37;
				else if(unR == 0x00 && unG == 0x80 && unB == 0x80)
					nIndex = 38;
				else if(unR == 0x00 && unG == 0x00 && unB == 0xFF)
					nIndex = 39;
				else if(unR == 0x00 && unG == 0xCC && unB == 0xFF)
					nIndex = 40;
				else if(unR == 0xCC && unG == 0xFF && unB == 0xFF)
					nIndex = 41;
				else if(unR == 0xCC && unG == 0xFF && unB == 0xCC)
					nIndex = 42;
				else if(unR == 0xFF && unG == 0xFF && unB == 0x99)
					nIndex = 43;
				else if(unR == 0x99 && unG == 0xCC && unB == 0xFF)
					nIndex = 44;
				else if(unR == 0xFF && unG == 0x99 && unB == 0xCC)
					nIndex = 45;
				else if(unR == 0xCC && unG == 0x99 && unB == 0xFF)
					nIndex = 46;
				else if(unR == 0xFF && unG == 0xCC && unB == 0x99)
					nIndex = 47;
				else if(unR == 0x33 && unG == 0x66 && unB == 0xFF)
					nIndex = 48;
				else if(unR == 0x33 && unG == 0xCC && unB == 0xCC)
					nIndex = 49;
				else if(unR == 0x99 && unG == 0xCC && unB == 0x00)
					nIndex = 50;
				else if(unR == 0xFF && unG == 0xCC && unB == 0x00)
					nIndex = 51;
				else if(unR == 0xFF && unG == 0x99 && unB == 0x00)
					nIndex = 52;
				else if(unR == 0xFF && unG == 0x66 && unB == 0x00)
					nIndex = 53;
				else if(unR == 0x66 && unG == 0x66 && unB == 0x99)
					nIndex = 54;
				else if(unR == 0x96 && unG == 0x96 && unB == 0x96)
					nIndex = 55;
				else if(unR == 0x00 && unG == 0x33 && unB == 0x66)
					nIndex = 56;
				else if(unR == 0x33 && unG == 0x99 && unB == 0x66)
					nIndex = 57;
				else if(unR == 0x00 && unG == 0x33 && unB == 0x00)
					nIndex = 58;
				else if(unR == 0x33 && unG == 0x33 && unB == 0x00)
					nIndex = 59;
				else if(unR == 0x99 && unG == 0x33 && unB == 0x00)
					nIndex = 60;
				else if(unR == 0x99 && unG == 0x33 && unB == 0x66)
					nIndex = 61;
				else if(unR == 0x33 && unG == 0x33 && unB == 0x99)
					nIndex = 62;
				else if(unR == 0x33 && unG == 0x33 && unB == 0x33)
					nIndex = 63;
				return nIndex;
			}
			std::map<int, CRgbColor*> mapIndexedColors;
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CColor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColor)
			CColor()
			{
			}
			virtual ~CColor()
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
				// вызовем по default
				toXML2(writer, _T("color"));
			}
			void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
			{
				writer.WriteString(_T("<"));
				writer.WriteString(sName);
				WritingStringNullableAttrBool(L"auto", m_oAuto);
				WritingStringNullableAttrInt(L"indexed", m_oIndexed, m_oIndexed->GetValue());
				if(m_oRgb.IsInit() && !m_oIndexed.IsInit())
				{
					int nIndex = OOX::Spreadsheet::CIndexedColors::GetDefaultIndexByRGBA(m_oRgb->Get_R(), m_oRgb->Get_G(), m_oRgb->Get_B(), m_oRgb->Get_A());
					if(-1 == nIndex)
					{
						WritingStringAttrString(L"rgb", m_oRgb->ToString());
					}
					else
					{
						WritingStringAttrInt(L"indexed", nIndex);
					}
				}
				WritingStringNullableAttrInt(L"theme", m_oThemeColor, m_oThemeColor->GetValue());
				WritingStringNullableAttrDouble(L"tint", m_oTint, m_oTint->GetValue());

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
				return et_x_Color;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

                    WritingElement_ReadAttributes_Read_if     ( oReader, _T("auto"),    m_oAuto )
                    WritingElement_ReadAttributes_Read_if     ( oReader, _T("indexed"), m_oIndexed )
                    WritingElement_ReadAttributes_Read_if     ( oReader, _T("rgb"),     m_oRgb )
                    WritingElement_ReadAttributes_Read_if     ( oReader, _T("theme"),   m_oThemeColor )
                    WritingElement_ReadAttributes_Read_if     ( oReader, _T("tint"),    m_oTint )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<>>						m_oAuto;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oIndexed;
			nullable<SimpleTypes::Spreadsheet::CHexColor>		m_oRgb;
			nullable<SimpleTypes::Spreadsheet::CThemeColor<>>	m_oThemeColor;
			nullable<SimpleTypes::CDouble>						m_oTint;
		};

		class CMruColors : public WritingElementWithChilds<CColor>
		{
		public:
			WritingElement_AdditionConstructors(CMruColors)
			CMruColors()
			{
			}
			virtual ~CMruColors()
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

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("color") == sName )
						m_arrItems.push_back( new CColor( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_MruColors;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
		class CCharset
		{
		public:
			WritingElement_AdditionConstructors(CCharset)
			CCharset()
			{
			}
			virtual ~CCharset()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oCharset )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CFontCharset<>>	m_oCharset;
		};
		class CVerticalAlign
		{
		public:
			WritingElement_AdditionConstructors(CVerticalAlign)
			CVerticalAlign()
			{
			}
			virtual ~CVerticalAlign()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVerticalAlign )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CVerticalAlignRun<>>	m_oVerticalAlign;
		};
		class CFontFamily
		{
		public:
			WritingElement_AdditionConstructors(CFontFamily)
			CFontFamily()
			{
			}
			virtual ~CFontFamily()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oFontFamily )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CFontFamily<>>	m_oFontFamily;
		};
		class CFontScheme
		{
		public:
			WritingElement_AdditionConstructors(CFontScheme)
			CFontScheme()
			{
			}
			virtual ~CFontScheme()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oFontScheme )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CFontScheme<>>	m_oFontScheme;
		};
		class CUnderline
		{
		public:
			WritingElement_AdditionConstructors(CUnderline)
			CUnderline()
			{
			}
			virtual ~CUnderline()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oUnderline )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CUnderline<>>	m_oUnderline;
		};
		//необработано:
		class CRPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CRPr)
			CRPr()
			{
			}
			virtual ~CRPr()
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
				writer.WriteString(_T("<rPr>"));
				if(m_oBold.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oBold->m_oVal.GetValue())
						writer.WriteString(_T("<b/>"));
					else
						writer.WriteString(_T("<b val=\"false\"/>"));
				}
				if(m_oItalic.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oItalic->m_oVal.GetValue())
						writer.WriteString(_T("<i/>"));
					else
						writer.WriteString(_T("<i val=\"false\"/>"));
				}
				if(m_oStrike.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oStrike->m_oVal.GetValue())
						writer.WriteString(_T("<strike/>"));
					else
						writer.WriteString(_T("<strike val=\"false\"/>"));
				}
				if(m_oUnderline.IsInit() && m_oUnderline->m_oUnderline.IsInit())
				{
					if( SimpleTypes::underlineSingle != m_oUnderline->m_oUnderline->GetValue())
					{
						WritingStringValAttrString(L"u", m_oUnderline->m_oUnderline->ToString());
					}
					else
					{
						writer.WriteString(L"<u/>");
					}
				}
				if(m_oOutline.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oOutline->m_oVal.GetValue())
						writer.WriteString(_T("<outline/>"));
					else
						writer.WriteString(_T("<outline val=\"false\"/>"));
				}
				if(m_oShadow.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oShadow->m_oVal.GetValue())
						writer.WriteString(_T("<shadow/>"));
					else
						writer.WriteString(_T("<shadow val=\"false\"/>"));
				}
				if(m_oCondense.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oCondense->m_oVal.GetValue())
						writer.WriteString(_T("<condense/>"));
					else
						writer.WriteString(_T("<condense val=\"false\"/>"));
				}
				if(m_oExtend.IsInit())
				{
					if(SimpleTypes::onoffTrue == m_oExtend->m_oVal.GetValue())
						writer.WriteString(_T("<extend/>"));
					else
						writer.WriteString(_T("<extend val=\"false\"/>"));
				}
				if(m_oVertAlign.IsInit() && m_oVertAlign->m_oVerticalAlign.IsInit())
				{
					std::wstring sVerticalAlign = m_oVertAlign->m_oVerticalAlign->ToString();
					writer.WriteString(L"<vertAlign val=\"");
					writer.WriteString(sVerticalAlign);
					writer.WriteString(L"\"/>");
				}
				if(m_oSz.IsInit() && m_oSz->m_oVal.IsInit())
				{
					WritingStringValAttrDouble(L"sz", m_oSz->m_oVal->GetValue());
				}
				if(m_oColor.IsInit())
					m_oColor->toXML2(writer, _T("color"));

				if(m_oRFont.IsInit() && m_oRFont->m_sVal.IsInit())
				{
					WritingStringValAttrEncodeXmlString(L"rFont", m_oRFont->m_sVal.get());
				}
				if(m_oFamily.IsInit() && m_oFamily->m_oFontFamily.IsInit())
				{
					WritingStringValAttrString(L"family", m_oFamily->m_oFontFamily->ToString());
				}
				if(m_oCharset.IsInit() && m_oCharset->m_oCharset.IsInit())
				{
					WritingStringValAttrString(L"charset", m_oCharset->m_oCharset->ToString());
				}
				if(m_oScheme.IsInit() && m_oScheme->m_oFontScheme.IsInit())
				{
					WritingStringValAttrString(L"scheme", m_oScheme->m_oFontScheme->ToString());
				}
				writer.WriteString(_T("</rPr>"));
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

					if ( _T("b") == sName )
						m_oBold = oReader;
					else if ( _T("charset") == sName )
						m_oCharset = oReader;
					else if ( _T("color") == sName )
						m_oColor = oReader;
					else if ( _T("condense") == sName )
						m_oCondense = oReader;
					else if ( _T("extend") == sName )
						m_oExtend = oReader;
					else if ( _T("family") == sName )
						m_oFamily = oReader;
					else if ( _T("i") == sName )
						m_oItalic = oReader;
					else if ( _T("outline") == sName )
						m_oOutline = oReader;
					else if ( _T("rFont") == sName || _T("name") == sName)
						m_oRFont = oReader;
					else if ( _T("scheme") == sName )
						m_oScheme = oReader;
					else if ( _T("shadow") == sName )
						m_oShadow = oReader;
					else if ( _T("strike") == sName )
						m_oStrike = oReader;
					else if ( _T("sz") == sName )
						m_oSz = oReader;
					else if ( _T("u") == sName )
						m_oUnderline = oReader;
					else if ( _T("vertAlign") == sName )
						m_oVertAlign = oReader;

				}
			}

			void fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType);
			void toXLSB (NSBinPptxRW::CXlsbBinaryWriter& oStream) const;
			_UINT32 getXLSBSize() const;

			virtual EElementType getType () const
			{
				return et_x_rPr;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oBold;
            nullable<CCharset>                                                      m_oCharset;
            nullable<CColor>                                                        m_oColor;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oCondense;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oExtend;
            nullable<CFontFamily >                                                  m_oFamily;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oItalic;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oOutline;
            nullable<ComplexTypes::Spreadsheet::String >                            m_oRFont;
            nullable<CFontScheme>                                                   m_oScheme;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oShadow;
			nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oStrike;
			nullable<ComplexTypes::Spreadsheet::CDouble>							m_oSz;
            nullable<CUnderline>                                                    m_oUnderline;
            nullable<CVerticalAlign>                                                m_oVertAlign;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_RPR_FILE_INCLUDE_H_
