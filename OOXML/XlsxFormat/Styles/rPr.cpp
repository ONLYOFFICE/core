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
#include "rPr.h"

#include "../../Binary/Presentation/BinaryFileReaderWriter.h"
#include "Fonts.h"

#include "../../XlsbFormat/Biff12_records/Color.h"
#include "../../XlsbFormat/Biff12_records/IndexedColor.h"
#include "../../XlsbFormat/Biff12_records/MRUColor.h"
namespace OOX
{
	namespace Spreadsheet
	{
		CRgbColor::CRgbColor()
		{
		}
		CRgbColor::~CRgbColor()
		{
		}
		void CRgbColor::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CRgbColor::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CRgbColor::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::IndexedColor*>(obj.get());
			if(ptr != nullptr)
			{
				 m_oRgb = SimpleTypes::Spreadsheet::CHexColor(ptr->bRed, ptr->bGreen, ptr->bBlue);
			}
		}
		EElementType CRgbColor::getType () const
		{
			return et_x_RgbColor;
		}
		std::wstring CRgbColor::toXML() const
		{
			return _T("");
		}
		void CRgbColor::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CRgbColor::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("rgb"),      m_oRgb )
			WritingElement_ReadAttributes_End( oReader )
		}

		CIndexedColors::CIndexedColors()
		{
		}
		CIndexedColors::~CIndexedColors()
		{
		}
		void CIndexedColors::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CIndexedColors::toXML() const
		{
			return _T("");
		}
		void CIndexedColors::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CIndexedColors::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		void CIndexedColors::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			int index = 0;

			for(auto &indexedColor : obj)
			{
				CRgbColor *pRgbColor = new CRgbColor(indexedColor);
				mapIndexedColors.insert(std::make_pair(index++, pRgbColor));
				m_arrItems.push_back(pRgbColor);
			}
		}
		EElementType CIndexedColors::getType () const
		{
			return et_x_IndexedColors;
		}
		bool CIndexedColors::GetDefaultRGBAByIndex(int index, unsigned char& unR, unsigned char& unG, unsigned char& unB, unsigned char& unA)
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
		int CIndexedColors::GetDefaultIndexByRGBA( unsigned char unR, unsigned char unG, unsigned char unB, unsigned char unA)
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
		void CIndexedColors::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		CColor::CColor(OOX::Document *pMain) :  WritingElement(pMain){}
		CColor::~CColor()
		{
		}
		void CColor::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CColor::toXML() const
		{
			return _T("");
		}
		void CColor::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			// вызовем по default
			toXML2(writer, _T("color"));
		}
		void CColor::toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
		{
			toXMLWithNS(writer, L"", sName, L"");
		}
		void CColor::toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
		{
			writer.StartNodeWithNS(node_ns, node_name);
			writer.StartAttributes();
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

			writer.EndAttributesAndNode();
		}
		void CColor::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CColor::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		void CColor::fromBin(XLS::BaseObject* obj)
		{
			ReadAttributes(obj);
		}
		EElementType CColor::getType () const
		{
			return et_x_Color;
		}
		void CColor::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("auto"),    m_oAuto )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("indexed"), m_oIndexed )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("rgb"),     m_oRgb )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("theme"),   m_oThemeColor )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("tint"),    m_oTint )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CColor::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::Color*>(obj.get());

			if(ptr != nullptr)
			{
				switch(ptr->xColorType)
				{
					case 0: m_oAuto     = true;		break;
					case 1: m_oIndexed  = ptr->index;	break;
					case 3: m_oThemeColor = (SimpleTypes::Spreadsheet::EThemeColor)ptr->index; break;
						/*switch (ptr->index)
						{
							case 0:
								m_oThemeColor = SimpleTypes::Spreadsheet::EThemeColor::themecolorDark1;
								break;
							case 1:
								m_oThemeColor = SimpleTypes::Spreadsheet::EThemeColor::themecolorLight1;
								break;
							case 2:
								m_oThemeColor = SimpleTypes::Spreadsheet::EThemeColor::themecolorDark2;
								break;
							case 3:
								m_oThemeColor = SimpleTypes::Spreadsheet::EThemeColor::themecolorLight2;
								break;
							default:
								m_oThemeColor = (SimpleTypes::Spreadsheet::EThemeColor)ptr->index;
						}
						break;*/
					default:
						m_oRgb = SimpleTypes::Spreadsheet::CHexColor(ptr->bRed, ptr->bGreen, ptr->bBlue, ptr->bAlpha);
						break;
				}
				if (ptr->nTintAndShade != 0)
				{
					 m_oTint     = ptr->nTintAndShade/32767.0;
				}
			}
		}
		void CColor::ReadAttributes(XLS::BaseObject* obj)
		{
			auto ptr = static_cast<XLSB::Color*>(obj);

			if(ptr != nullptr)
			{
				switch(ptr->xColorType)
				{
					case 0: m_oAuto     = true;		break;
					case 1: m_oIndexed  = ptr->index;	break;
					case 3: m_oThemeColor = (SimpleTypes::Spreadsheet::EThemeColor)ptr->index; break;
						/*switch (ptr->index)
						{
							case 0:
								m_oThemeColor = SimpleTypes::Spreadsheet::EThemeColor::themecolorDark1;
								break;
							case 1:
								m_oThemeColor = SimpleTypes::Spreadsheet::EThemeColor::themecolorLight1;
								break;
							case 2:
								m_oThemeColor = SimpleTypes::Spreadsheet::EThemeColor::themecolorDark2;
								break;
							case 3:
								m_oThemeColor = SimpleTypes::Spreadsheet::EThemeColor::themecolorLight2;
								break;
							default:
								m_oThemeColor = (SimpleTypes::Spreadsheet::EThemeColor)ptr->index;
						}
						break;*/
					default:
						m_oRgb = SimpleTypes::Spreadsheet::CHexColor(ptr->bRed, ptr->bGreen, ptr->bBlue, ptr->bAlpha);
						break;
				}
				if (ptr->nTintAndShade != 0)
				{
					 m_oTint     = ptr->nTintAndShade/32767.0;
				}
			}
		}

		CMruColors::CMruColors()
		{
		}
		CMruColors::~CMruColors()
		{
		}
		void CMruColors::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CMruColors::toXML() const
		{
			return _T("");
		}
		void CMruColors::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CMruColors::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		void CMruColors::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			for(auto &MRUColor : obj)
			{
				auto color = new CColor();
				color->fromBin(dynamic_cast<XLS::BaseObject*>(&(static_cast<XLSB::MRUColor*>(MRUColor.get())->colorMRU)));
				m_arrItems.push_back(color);
			}
		}
		EElementType CMruColors::getType () const
		{
			return et_x_MruColors;
		}
		void CMruColors::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		CCharset::CCharset(OOX::Document *pMain) :  WritingElement(pMain){}
		CCharset::~CCharset()
		{
		}
		void CCharset::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CCharset::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCharset::toXML() const
		{
			return _T("");
		}
		void CCharset::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CCharset::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oCharset )
			WritingElement_ReadAttributes_End( oReader )
		}

		CVerticalAlign::CVerticalAlign(OOX::Document *pMain) :  WritingElement(pMain){}
		CVerticalAlign::~CVerticalAlign()
		{
		}
		void CVerticalAlign::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CVerticalAlign::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CVerticalAlign::toXML() const
		{
			return _T("");
		}
		void CVerticalAlign::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CVerticalAlign::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oVerticalAlign )
			WritingElement_ReadAttributes_End( oReader )
		}

		CFontFamily::CFontFamily(OOX::Document *pMain) :  WritingElement(pMain){}
		CFontFamily::~CFontFamily()
		{
		}
		void CFontFamily::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CFontFamily::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CFontFamily::toXML() const
		{
			return _T("");
		}
		void CFontFamily::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CFontFamily::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"),      m_oFontFamily )
			WritingElement_ReadAttributes_End( oReader )
		}

		CFontScheme::CFontScheme()
		{
		}
		CFontScheme::~CFontScheme()
		{
		}
		void CFontScheme::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CFontScheme::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CFontScheme::toXML() const
		{
			return _T("");
		}
		void CFontScheme::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CFontScheme::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, _T("val"), m_oFontScheme )
			WritingElement_ReadAttributes_End( oReader )
		}

		CUnderline::CUnderline(OOX::Document *pMain) :  WritingElement(pMain){}
		CUnderline::~CUnderline()
		{
		}
		void CUnderline::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void CUnderline::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CUnderline::toXML() const
		{
			return _T("");
		}
		void CUnderline::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CUnderline::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"), m_oUnderline )
			WritingElement_ReadAttributes_End( oReader )
		}

		CRPr::CRPr(OOX::Document *pMain) :  WritingElement(pMain){}
		CRPr::~CRPr()
		{
		}
		void CRPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CRPr::toXML() const
		{
			return _T("");
		}
		void CRPr::toXML(NSStringUtils::CStringBuilder& writer) const
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
				//todo more complex solution
				//if name more then 31 chars Excel wants to recover xlsx
				if (m_oRFont->m_sVal->length() <= 31)
				{
					WritingStringValAttrEncodeXmlString(L"rFont", m_oRFont->m_sVal.get());
				}
				else
				{
					WritingStringValAttrEncodeXmlString(L"rFont", m_oRFont->m_sVal->substr(0, 31));
				}
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
		void CRPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		EElementType CRPr::getType () const
		{
			return et_x_rPr;
		}
		void CRPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
		void CRPr::fromXLSB (NSBinPptxRW::CBinaryFileReader& oStream, _UINT16 nType)
		{
			//LONG nEnd = oStream.XlsbReadRecordLength() + oStream.GetPos();

			_UINT16 dyHeight = oStream.GetUShort();
			if(dyHeight >= 0x0014)
			{
				m_oSz.Init();
				m_oSz->m_oVal.Init();
				m_oSz->m_oVal->SetValue(dyHeight / 20);
			}
			_UINT16 grbit = oStream.GetUShort();
			if(0 != (grbit & 0x2))
			{
				m_oItalic.Init();
				m_oItalic->FromBool(true);
			}
			if(0 != (grbit & 0x8))
			{
				m_oStrike.Init();
				m_oStrike->FromBool(true);
			}
			if(0 != (grbit & 0x10))
			{
				m_oOutline.Init();
				m_oOutline->FromBool(true);
			}
			if(0 != (grbit & 0x20))
			{
				m_oShadow.Init();
				m_oShadow->FromBool(true);
			}
			if(0 != (grbit & 0x40))
			{
				m_oCondense.Init();
				m_oCondense->FromBool(true);
			}
			if(0 != (grbit & 0x80))
			{
				m_oExtend.Init();
				m_oExtend->FromBool(true);
			}
			_UINT16 bls = oStream.GetUShort();
			if(0x02BC == bls)
			{
				m_oBold.Init();
				m_oBold->FromBool(true);
			}
			_UINT16 sss = oStream.GetUShort();
			if(sss > 0)
			{
				m_oVertAlign.Init();
				m_oVertAlign->m_oVerticalAlign.Init();
				switch(sss)
				{
					case 0x0001:
						m_oVertAlign->m_oVerticalAlign->SetValue(SimpleTypes::verticalalignrunSuperscript);
						break;
					case 0x0002:
						m_oVertAlign->m_oVerticalAlign->SetValue(SimpleTypes::verticalalignrunSubscript);
						break;
				}
			}
			_UINT16 uls = oStream.GetUChar();
			if(uls > 0)
			{
				m_oUnderline.Init();
				m_oUnderline->m_oUnderline.Init();
				switch(uls)
				{
					case 0x01:
						m_oUnderline->m_oUnderline->SetValue(SimpleTypes::Spreadsheet::underlineSingle);
						break;
					case 0x02:
						m_oUnderline->m_oUnderline->SetValue(SimpleTypes::Spreadsheet::underlineDouble);
						break;
					case 0x21:
						m_oUnderline->m_oUnderline->SetValue(SimpleTypes::Spreadsheet::underlineSingleAccounting);
						break;
					case 0x22:
						m_oUnderline->m_oUnderline->SetValue(SimpleTypes::Spreadsheet::underlineDoubleAccounting);
						break;
				}
			}
			_UINT16 bFamily = oStream.GetUChar();
			if(bFamily > 0)
			{
				m_oFamily.Init();
				m_oFamily->m_oFontFamily.Init();
				m_oFamily->m_oFontFamily->SetValue((SimpleTypes::Spreadsheet::EFontFamily)bFamily);
			}
			_UINT16 bCharSet = oStream.GetUChar();
			if(bCharSet > 0)
			{
				m_oCharset.Init();
				m_oCharset->m_oCharset.Init();
				m_oCharset->m_oCharset->SetValue((SimpleTypes::Spreadsheet::EFontCharset)bCharSet);
			}
			oStream.Skip(1);

			BYTE xColorType = oStream.GetUChar();
			BYTE index = oStream.GetUChar();
			_INT16 nTintAndShade = oStream.GetShort();
			_UINT32 rgba = oStream.GetULong();
			m_oColor.Init();
			if(0 != (xColorType & 0x1))
			{
				m_oColor->m_oRgb.Init();
				m_oColor->m_oRgb->Set_R((unsigned char)(rgba & 0xFF));
				m_oColor->m_oRgb->Set_G((unsigned char)((rgba & 0xFF00)>>8));
				m_oColor->m_oRgb->Set_B((unsigned char)((rgba & 0xFF0000)>>16));
				m_oColor->m_oRgb->Set_A((unsigned char)((rgba & 0xFF000000)>>24));
			}
			xColorType &= 0xFE;
			if(0x2 == xColorType)
			{
				m_oColor->m_oIndexed.Init();
				m_oColor->m_oIndexed->SetValue(index);
			}
			else if(0x6 == xColorType)
			{
				m_oColor->m_oThemeColor.Init();
				SimpleTypes::Spreadsheet::EThemeColor eColor = SimpleTypes::Spreadsheet::themecolorDark1;
				switch(index)
				{
					case 0x01:
						eColor = SimpleTypes::Spreadsheet::themecolorLight1;
						break;
					case 0x00:
						eColor = SimpleTypes::Spreadsheet::themecolorDark1;
						break;
					case 0x03:
						eColor = SimpleTypes::Spreadsheet::themecolorLight2;
						break;
					case 0x02:
						eColor = SimpleTypes::Spreadsheet::themecolorDark2;
						break;
					case 0x04:
						eColor = SimpleTypes::Spreadsheet::themecolorAccent1;
						break;
					case 0x05:
						eColor = SimpleTypes::Spreadsheet::themecolorAccent2;
						break;
					case 0x06:
						eColor = SimpleTypes::Spreadsheet::themecolorAccent3;
						break;
					case 0x07:
						eColor = SimpleTypes::Spreadsheet::themecolorAccent4;
						break;
					case 0x08:
						eColor = SimpleTypes::Spreadsheet::themecolorAccent5;
						break;
					case 0x09:
						eColor = SimpleTypes::Spreadsheet::themecolorAccent6;
						break;
					case 0x0A:
						eColor = SimpleTypes::Spreadsheet::themecolorHyperlink;
						break;
					case 0x0B:
						eColor = SimpleTypes::Spreadsheet::themecolorFollowedHyperlink;
						break;
				}
				m_oColor->m_oThemeColor->SetValue(eColor);
			}
			if(0 != nTintAndShade)
			{
				m_oColor->m_oTint.Init();
				m_oColor->m_oTint->SetValue(((double)nTintAndShade) / 0x7FFF);
			}
			if(!m_oColor->m_oThemeColor.IsInit() && !m_oColor->m_oRgb.IsInit() && !m_oColor->m_oIndexed.IsInit())
			{
				m_oColor->m_oAuto.Init();
				m_oColor->m_oAuto->FromBool(true);
			}

			_UINT16 bFontScheme = oStream.GetUChar();
			if(bFontScheme > 0)
			{
				m_oScheme.Init();
				m_oScheme->m_oFontScheme.Init();
				switch(bFontScheme)
				{
					case 0x01:
						m_oScheme->m_oFontScheme->SetValue(SimpleTypes::Spreadsheet::fontschemeMajor);
						break;
					case 0x02:
						m_oScheme->m_oFontScheme->SetValue(SimpleTypes::Spreadsheet::fontschemeMinor);
						break;
				}
			}
			m_oRFont.Init();
			m_oRFont->m_sVal = oStream.GetString2();

			//oStream.Seek(nEnd);
		}
		void CRPr::toXLSB (NSBinPptxRW::CXlsbBinaryWriter& oStream) const
		{
			//oStream.XlsbStartRecord();

			_UINT16 dyHeight = 0;
			if(m_oSz.IsInit() && m_oSz->m_oVal.IsInit())
			{
				dyHeight = (_UINT16)((m_oSz->m_oVal->GetValue() * 20)) & 0x1FFF;
			}
			oStream.WriteUSHORT(dyHeight);
			_UINT16 grbit = 0;
			if(m_oItalic.IsInit() && m_oItalic->ToBool())
			{
				grbit |= 0x2;
			}
			if(m_oStrike.IsInit() && m_oStrike->ToBool())
			{
				grbit |= 0x8;
			}
			if(m_oOutline.IsInit() && m_oOutline->ToBool())
			{
				grbit |= 0x10;
			}
			if(m_oShadow.IsInit() && m_oShadow->ToBool())
			{
				grbit |= 0x20;
			}
			if(m_oCondense.IsInit() && m_oCondense->ToBool())
			{
				grbit |= 0x40;
			}
			if(m_oExtend.IsInit() && m_oExtend->ToBool())
			{
				grbit |= 0x80;
			}
			oStream.WriteUSHORT(grbit);
			_UINT16 bls = m_oBold.IsInit() && m_oBold->ToBool() ? 0x02BC : 0x0190;
			oStream.WriteUSHORT(bls);
			_UINT16 sss = 0;
			if(m_oVertAlign.IsInit() && m_oVertAlign->m_oVerticalAlign.IsInit())
			{
				if(SimpleTypes::verticalalignrunSuperscript == m_oVertAlign->m_oVerticalAlign->GetValue())
				{
					sss = 0x0001;
				}
				else if(SimpleTypes::verticalalignrunSubscript == m_oVertAlign->m_oVerticalAlign->GetValue())
				{
					sss = 0x0002;
				}
			}
			oStream.WriteUSHORT(sss);
			BYTE uls = 0;
			if(m_oUnderline.IsInit())
			{
				SimpleTypes::Spreadsheet::EUnderline eType = SimpleTypes::Spreadsheet::underlineSingle;
				if(m_oUnderline->m_oUnderline.IsInit())
					eType = m_oUnderline->m_oUnderline->GetValue();

				switch(eType)
				{
					case SimpleTypes::Spreadsheet::underlineSingle:
						uls = 0x01;
						break;
					case SimpleTypes::Spreadsheet::underlineDouble:
						uls = 0x02;
						break;
					case SimpleTypes::Spreadsheet::underlineSingleAccounting:
						uls = 0x21;
						break;
					case SimpleTypes::Spreadsheet::underlineDoubleAccounting:
						uls = 0x22;
						break;
				}
			}
			oStream.WriteBYTE(uls);
			BYTE bFamily = 0;
			if(m_oFamily.IsInit() && m_oFamily->m_oFontFamily.IsInit())
			{
				bFamily = m_oFamily->m_oFontFamily->GetValue();
			}
			oStream.WriteBYTE(bFamily);
			BYTE bCharSet = 0;
			if(m_oCharset.IsInit() && m_oCharset->m_oCharset.IsInit())
			{
				bCharSet = m_oCharset->m_oCharset->GetValue();
			}
			oStream.WriteBYTE(bCharSet);
			oStream.WriteBYTE(0);
			BYTE xColorType = 0;
			_UINT32 rgba = 0;
			BYTE index = 0;
			_INT16 nTintAndShade = 0;
			if(m_oColor.IsInit())
			{
				if(m_oColor->m_oRgb.IsInit())
				{
					xColorType |= 0x1;
					xColorType |= 0x4;
					rgba = m_oColor->m_oRgb->Get_R() | (m_oColor->m_oRgb->Get_G() << 8) | (m_oColor->m_oRgb->Get_B() << 16) | (m_oColor->m_oRgb->Get_A() << 24);
				}
				else if(m_oColor->m_oIndexed.IsInit())
				{
					BYTE ucA;
					BYTE ucR;
					BYTE ucG;
					BYTE ucB;
					int index = m_oColor->m_oIndexed->GetValue();
					//64 is auto color
					if(64 != index && OOX::Spreadsheet::CIndexedColors::GetDefaultRGBAByIndex(index, ucR, ucG, ucB, ucA))
					{
						xColorType |= 0x1;
						xColorType |= 0x4;
						rgba = ucR | (ucG << 8) | (ucB << 16) | (ucA << 24);
					}
				}
				else if(m_oColor->m_oThemeColor.IsInit())
				{
					xColorType |= 0x6;
					switch(m_oColor->m_oThemeColor->GetValue())
					{
						case SimpleTypes::Spreadsheet::themecolorLight1:
							index = 0x01;
							break;
						case SimpleTypes::Spreadsheet::themecolorDark1:
							index = 0x00;
							break;
						case SimpleTypes::Spreadsheet::themecolorLight2:
							index = 0x03;
							break;
						case SimpleTypes::Spreadsheet::themecolorDark2:
							index = 0x02;
							break;
						case SimpleTypes::Spreadsheet::themecolorAccent1:
							index = 0x04;
							break;
						case SimpleTypes::Spreadsheet::themecolorAccent2:
							index = 0x05;
							break;
						case SimpleTypes::Spreadsheet::themecolorAccent3:
							index = 0x06;
							break;
						case SimpleTypes::Spreadsheet::themecolorAccent4:
							index = 0x07;
							break;
						case SimpleTypes::Spreadsheet::themecolorAccent5:
							index = 0x08;
							break;
						case SimpleTypes::Spreadsheet::themecolorAccent6:
							index = 0x09;
							break;
						case SimpleTypes::Spreadsheet::themecolorHyperlink:
							index = 0x0A;
							break;
						case SimpleTypes::Spreadsheet::themecolorFollowedHyperlink:
							index = 0x0B;
							break;
					}
				}
				if(m_oColor->m_oTint.IsInit())
				{
					nTintAndShade = _INT16(0x7FFF * m_oColor->m_oTint->GetValue());
				}
			}
			oStream.WriteBYTE(xColorType);
			oStream.WriteBYTE(index);
			oStream.WriteSHORT(nTintAndShade);
			oStream.WriteULONG(rgba);
			BYTE bFontScheme = 0;
			if(m_oScheme.IsInit() && m_oScheme->m_oFontScheme.IsInit())
			{
				switch(m_oScheme->m_oFontScheme->GetValue())
				{
					case SimpleTypes::Spreadsheet::fontschemeNone:
						bFontScheme = 0x00;
						break;
					case SimpleTypes::Spreadsheet::fontschemeMajor:
						bFontScheme = 0x01;
						break;
					case SimpleTypes::Spreadsheet::fontschemeMinor:
						bFontScheme = 0x02;
						break;
				}
			}
			oStream.WriteBYTE(bFontScheme);
			oStream.WriteString(m_oRFont.IsInit() && m_oRFont->m_sVal.IsInit() ? m_oRFont->m_sVal.get() : L"");

			//oStream.XlsbEndRecord(XLSB::rt_FONT);
		}
		_UINT32 CRPr::getXLSBSize() const
		{
			_UINT32 nLen = 2 + 2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1 + 2 + 4 + 1;
			nLen += 4 + 2 * (m_oRFont.IsInit() && m_oRFont->m_sVal.IsInit() ? m_oRFont->m_sVal->length() : 0);
			return nLen;
		}
		void CRPr::fromFont(CFont* font)
		{
			m_oBold           = font->m_oBold;
			m_oCharset        = font->m_oCharset;
			m_oColor          = font->m_oColor;
			m_oCondense       = font->m_oCondense;
			m_oExtend         = font->m_oExtend;
			m_oFamily         = font->m_oFamily;
			m_oItalic         = font->m_oItalic;
			m_oOutline        = font->m_oOutline;
			m_oRFont          = font->m_oRFont;
			m_oScheme         = font->m_oScheme;
			m_oShadow         = font->m_oShadow;
			m_oStrike         = font->m_oStrike;
			m_oSz             = font->m_oSz;
			m_oUnderline      = font->m_oUnderline;
			m_oVertAlign      = font->m_oVertAlign;

		}

	} //Spreadsheet
} // OOX
