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

#include "../../../../../ASCOfficePPTXFile/Editor/BinaryFileReaderWriter.h"

namespace OOX
{
	namespace Spreadsheet
	{
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
			m_oRFont->m_sVal.Init();
			m_oRFont->m_sVal->append(oStream.GetString2());

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
					if(OOX::Spreadsheet::CIndexedColors::GetDefaultRGBAByIndex(m_oColor->m_oIndexed->GetValue(), ucR, ucG, ucB, ucA))
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

	} //Spreadsheet
} // OOX
