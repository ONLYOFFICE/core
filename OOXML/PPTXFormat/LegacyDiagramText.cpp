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

#include "LegacyDiagramText.h"

namespace PPTX
{
	LegacyDiagramText::LegacyDiagramText(OOX::Document *pMain) : OOX::File(pMain)
	{
		m_Data		= 0;
		m_DataSize	= 0;
	}
	LegacyDiagramText::LegacyDiagramText(OOX::Document *pMain, const OOX::CPath& filename) : OOX::File(pMain)
	{
		m_Data		= 0;
		m_DataSize	= 0;

		read(filename);
	}
	LegacyDiagramText::~LegacyDiagramText()
	{
		if (m_Data)delete []m_Data;
		m_Data = NULL;
		m_DataSize =0;
	}
	void LegacyDiagramText::read(const OOX::CPath& filename)
	{
		if (m_Data)delete []m_Data;
		m_Data = NULL;
		m_DataSize =0;

		NSFile::CFileBinary file;
		if (true == file.OpenFile(filename.GetPath()))
		{
			m_DataSize = (int)file.GetFileSize();
			m_Data = new BYTE[m_DataSize];

			if (m_Data)
			{
				file.ReadFile(m_Data, m_DataSize);
			}

			file.CloseFile();
		}

		Parse();
		// todooo .. разбить строку и создать структуру <a:p>...</a:p><a:p>...</a:p>...<a:p>...</a:p>
		// щас используется 'нулевой' вариант
		// незабыть поменять в doc_LoadShape (ASCOfficeDrawingConvert.cpp)
	}
	void LegacyDiagramText::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		//старье на запись не поддерживаем
		content.Registration(type().OverrideType(), directory, filename);
	}
	LegacyDiagramText& LegacyDiagramText::operator=(const LegacyDiagramText& oSrc)
	{
		m_Text = oSrc.m_Text;
		return *this;
	}
	const OOX::FileType LegacyDiagramText::type() const
	{
		return OOX::FileTypes::LegacyDiagramText;
	}
	const OOX::CPath LegacyDiagramText::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath LegacyDiagramText::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	void LegacyDiagramText::Parse()
	{
		_UINT32 typeText =0;
		int pos = 8; //skip first main header
		while (pos < m_DataSize - 8)
		{
			SRecordHeader rec = *(SRecordHeader*)(m_Data + pos);
			pos += 8;
			switch (rec.RecType)
			{
				case 0x0F9F:	//TextHeaderAtom
				{
					typeText = *(_UINT32*)(m_Data + pos);

//Tx_TYPE_TITLE			0x00000000 Title placeholder shape text.
//Tx_TYPE_BODY			0x00000001 Body placeholder shape text.
//Tx_TYPE_NOTES			0x00000002 Notes placeholder shape text.
//Tx_TYPE_OTHER			0x00000004 Any other text.
//Tx_TYPE_CENTERBODY	0x00000005 Center body placeholder shape text.
//Tx_TYPE_CENTERTITLE	0x00000006 Center title placeholder shape text.
//Tx_TYPE_HALFBODY		0x00000007 Half-sized body placeholder shape text.
//Tx_TYPE_QUARTERBODY	0x00000008 Quarter-sized body placeholder shape text.
				}break;
				case 0x0FA0:	//TextCharsAtom
				{
					int lSize = rec.RecLen /2;
					if (sizeof(wchar_t) == 4)
					{
						wchar_t * sBuffer = new wchar_t[lSize + 1];
						memset(sBuffer, 0, (lSize + 1)* sizeof(wchar_t));

						UTF16* pStrUtf16 = (UTF16 *) (m_Data+pos);
						UTF32 *pStrUtf32 = (UTF32 *) sBuffer;

						// this values will be modificated
						const UTF16 *pStrUtf16_Conv = pStrUtf16;
						UTF32 *pStrUtf32_Conv = pStrUtf32;

						ConversionResult eUnicodeConversionResult =
						ConvertUTF16toUTF32 (&pStrUtf16_Conv
											, &pStrUtf16[lSize]
											, &pStrUtf32_Conv
											, &pStrUtf32 [lSize]
											, strictConversion);

						if (conversionOK == eUnicodeConversionResult)
						{
							m_Text = std::wstring((WCHAR*)sBuffer, lSize);

						}
						delete []sBuffer;
					}
					else
					{
						m_Text = std::wstring((wchar_t*)(m_Data+pos),  lSize);
					}
				}break;
				case 0x0FA8:	//TextBytesAtom
				{
					std::string		strAnsi		= std::string((char*)m_Data+pos, rec.RecLen);
					std::wstring	strUnicode(strAnsi.begin(), strAnsi.end());
					m_Text = strUnicode;
				}break;
				case 0x0FA1:	//StyleTextPropAtom
				{
					//rgTextPFRun (variable)
					//rgTextCFRun (variable)

					ParseTextProps(m_Data + pos, rec.RecLen, (int)m_Text.length());
				}break;
				case 0x0FAA:	//TextSpecialInfoAtom
				{
					//TextSIRun
					_UINT32 size = *(_UINT32*)(m_Data + pos);
					if (size > 0)
					{
						//TextSIExceptions ... variable
						//lang, bidi, spelling
					}
				}break;
			}
			pos += rec.RecLen;

		}
	}

	void LegacyDiagramText::ParseTextProps(BYTE* Data, int size, int count_text)
	{
		int pos = 0;
		int count_all = 0;

		while (count_all < count_text + 1)
		{
			PPTX::Logic::TextParagraphPr elm;
			m_arrPPr.push_back(elm);

			int count = 0;

			pos += ParseParaProps(m_arrPPr.back(), Data + pos, size - pos, count);
			count_all += count;
		}

		count_all = 0;
		while (count_all < count_text + 1)
		{
			PPTX::Logic::RunProperties elm;
			m_arrRPr.push_back(elm);

			int count = 0;
			pos += ParseRunProps(m_arrRPr.back(), Data + pos, size - pos, count);
			count_all += count;
		}
	}

	int LegacyDiagramText::ParseParaProps(PPTX::Logic::TextParagraphPr& props, BYTE* Data, int size, int  & count)
	{
		int pos=0;
		count = *(_UINT32*)(Data + pos); pos += 4;

		int m_lLevel	= *(_UINT16*)(Data + pos); pos += 2;

		if (m_lLevel > 0x0004)
			m_lLevel = 0x0004;


		DWORD dwFlags = *(_UINT32*)(Data + pos); pos += 4;
		BYTE flag1 = (BYTE)(dwFlags);
		BYTE flag2 = (BYTE)(dwFlags >> 8);
		BYTE flag3 = (BYTE)(dwFlags >> 16);
		BYTE flag4 = (BYTE)(dwFlags >> 24);

		bool hasBullet_				= (0x01 == (0x01 & flag1));
		bool bulletHasFont_			= (0x02 == (0x02 & flag1));
		bool bulletHasColor_		= (0x04 == (0x04 & flag1));
		bool bulletHasSize_			= (0x08 == (0x08 & flag1));

		bool bulletFontRef_			= (0x10 == (0x10 & flag1));
		bool bulletColor_			= (0x20 == (0x20 & flag1));
		bool bulletSize_			= (0x40 == (0x40 & flag1));
		bool bulletChar_			= (0x80 == (0x80 & flag1));

		bool leftMargin_			= (0x01 == (0x01 & flag2));
		// reserved
		bool indent_				= (0x04 == (0x04 & flag2));
		bool textAlignment_			= (0x08 == (0x08 & flag2));
		bool lineSpacing_			= (0x10 == (0x10 & flag2));
		bool spaceBefore_			= (0x20 == (0x20 & flag2));
		bool spaceAfter_			= (0x40 == (0x40 & flag2));
		bool defaultTabSize_		= (0x80 == (0x80 & flag2));

		bool fontAlign_				= (0x01 == (0x01 & flag3));
		bool charWrap_				= (0x02 == (0x02 & flag3));
		bool wordWrap_				= (0x04 == (0x04 & flag3));
		bool overflow_				= (0x08 == (0x08 & flag3));
		bool tabStops_				= (0x10 == (0x10 & flag3));
		bool textDirection_			= (0x20 == (0x20 & flag3));
		//reserved
		bool bulletBlip_			= (0x80 == (0x80 & flag3));

		bool bulletScheme_			= (0x01 == (0x01 & flag4));
		bool bulletHasScheme_		= (0x02 == (0x02 & flag4));

		bool Bulleted = false;
		if (hasBullet_ || bulletHasFont_ || bulletHasColor_ || bulletHasSize_)
		{
			WORD bulletFlag		= *(_UINT16*)(Data + pos); pos += 2;
			if (bulletFlag & 0x0F)
				Bulleted	=  (0x01 == (bulletFlag & 0x01));
			else
				Bulleted	= false;
		}
//todooo протестировано тока align
		if (bulletChar_)
		{
			Logic::BuChar *buChar = new Logic::BuChar();
			buChar->Char	= (WCHAR)(*(_UINT16*)(Data + pos)); pos += 2;
			props.ParagraphBullet.m_Bullet = buChar;
		}
		if (bulletFontRef_)
		{
			_UINT16 buTypeface	= *(_UINT16*)(Data + pos); pos += 2;
		}
		if (bulletSize_)
		{
			Logic::BuSzPts * sz = new Logic::BuSzPts();
			sz->val				= *(_UINT16*)(Data + pos); pos += 2;
			props.buSize.m_Size = sz;
		}
		if (bulletColor_)
		{
			Logic::BuClr *bu_color = new Logic::BuClr();

			BYTE R				= *(Data + pos); pos += 1;
			BYTE G				= *(Data + pos); pos += 1;
			BYTE B				= *(Data + pos); pos += 1;
			BYTE Index			= *(Data + pos); pos += 1;
			//buColor

			if (Index < 10)
			{
				//ODRAW::CorrectColorPPT(Index);
			}
			else
			{

				Logic::SrgbClr *color = new Logic::SrgbClr();

				color->SetRGB(R, G, B);
				bu_color->Color.Color = color;
			}
			props.buColor.m_Color = bu_color;
		}
		if (textAlignment_)
		{
			int ppt_align = *(_UINT16*)(Data + pos); pos += 2;
			int pptx_align = 0;
			switch(ppt_align)
			{
				case 0: pptx_align = 4; break;//l
				case 1: pptx_align = 0; break;//ctr
				case 2: pptx_align = 5; break;//r
				case 3: pptx_align = 2; break;//just
				case 4: pptx_align = 1; break;//dist
				case 5: pptx_align = 6; break;//thaiDist
				case 6: pptx_align = 3; break;//justLow
			}
			props.algn = pptx_align;
		}
		if (lineSpacing_)
		{
			PPTX::Logic::TextSpacing s;
			s.spcPts = *(_INT16*)(Data + pos); pos += 2;
			props.lnSpc	= s;
		}

		if (spaceBefore_)
		{
			PPTX::Logic::TextSpacing s;
			s.spcPts = *(_INT16*)(Data + pos); pos += 2;
			props.spcBef= s;
		}

		if (spaceAfter_)
		{
			PPTX::Logic::TextSpacing s;
			s.spcPts = *(_INT16*)(Data + pos); pos += 2;
			props.spcAft = s;
		}

		if (leftMargin_)
		{
			props.marL			= *(_INT16*)(Data + pos); pos += 2;
		}

		if (indent_)
		{
			props.indent				= *(_INT16*)(Data + pos); pos += 2;
		}

		if (defaultTabSize_)
		{
			props.defTabSz		= *(_UINT16*)(Data + pos); pos += 2;
		}

		if (tabStops_)
		{
			WORD tabStopsCount = *(_UINT16*)(Data + pos); pos += 2;
			props.tabLst.clear();

			if (tabStopsCount > 10)
				tabStopsCount = 10;

			for (int i = 0; i < (int)tabStopsCount; ++i)
			{
				PPTX::Logic::Tab tab;
				tab.pos = *(_UINT32*)(Data + pos); pos += 4;
				props.tabLst.push_back(tab);
			}

			if (0 < props.tabLst.size())
				props.defTabSz = props.tabLst[0].pos;
		}

		if (fontAlign_)
		{
			props.fontAlgn	= (BYTE)(*(_UINT16*)(Data + pos)); pos += 2;
		}

		if (charWrap_ || wordWrap_ || overflow_)
		{
			int wrapFlags	= *(_UINT16*)(Data + pos); pos += 2;
		}

		if (textDirection_)
		{
			int textDirection = *(_UINT16*)(Data + pos); pos += 2;
		}
		return pos;
	}

	int LegacyDiagramText::ParseRunProps(PPTX::Logic::RunProperties& props, BYTE* Data, int size, int  & count)
	{
		int pos = 0;
		count	= *(_UINT32*)(Data + pos); pos += 4;

		DWORD dwFlags = *(_UINT32*)(Data + pos); pos += 4;
		BYTE flag1 = (BYTE)(dwFlags);
		BYTE flag2 = (BYTE)(dwFlags >> 8);
		BYTE flag3 = (BYTE)(dwFlags >> 16);
		BYTE flag4 = (BYTE)(dwFlags >> 24);

		bool hasBold				= (0x01 == (0x01 & flag1));
		bool hasItalic				= (0x02 == (0x02 & flag1));
		bool hasUnderline			= (0x04 == (0x04 & flag1));
		// unused
		bool hasShadow				= (0x10 == (0x10 & flag1));
		bool hasFehint				= (0x20 == (0x20 & flag1));
		// unused
		bool hasKimi				= (0x80 == (0x80 & flag1));

		// unused
		bool hasEmboss				= (0x02 == (0x02 & flag2));
		// unused
		BYTE hasStyle				= ((0x3C & flag2) >> 2);
		// unused

		bool typeface_				= (0x01 == (0x01 & flag3));
		bool size_					= (0x02 == (0x02 & flag3));
		bool color_					= (0x04 == (0x04 & flag3));
		bool BaseLineOffset_		= (0x08 == (0x08 & flag3));
		bool EAFontRef_				= (0x20 == (0x20 & flag3));
		bool AnsiFontRef_			= (0x40 == (0x40 & flag3));
		bool SymbolFontRef_			= (0x80 == (0x80 & flag3));

		bool hasNewEATypeface		= (0x01 == (0x01 & flag4));
		bool hasCsTypeface			= (0x02 == (0x02 & flag4));
		bool hasPp11ext				= (0x04 == (0x04 & flag4));

		bool bIsFontStylePresent = (hasBold || hasItalic || hasUnderline || hasShadow ||
			hasFehint || hasKimi || hasEmboss || hasStyle != 0);

//todooo протестировано тока sz, color
		if (bIsFontStylePresent)
		{
			WORD fontStyle = *(_UINT16*)(Data + pos); pos += 2;

			if (0x01 == (0x01 & fontStyle))
				props.b = true;
			if (0x02 == (0x02 & fontStyle))
				props.i = true;
			if (0x04 == (0x04 & fontStyle))
				props.u = Limit::TextUnderline();
			if (0x10 == (0x10 & fontStyle))
				props.u = Limit::TextUnderline();
		}

		_UINT32 Typeface;
		_UINT16 EAFontRef, AnsiFontRef, SymbolFontRef;

		if (typeface_)
		{
			Typeface = *(_UINT16*)(Data + pos); pos += 2;
		}

		if (EAFontRef_)
		{
			EAFontRef = *(_UINT16*)(Data + pos); pos += 2;
		}

		if (AnsiFontRef_)
		{
			AnsiFontRef = *(_UINT16*)(Data + pos); pos += 2;
		}

		if (SymbolFontRef_)
		{
			SymbolFontRef = *(_UINT16*)(Data + pos); pos += 2;
		}
		if (size_)
		{
			props.sz = *(_UINT16*)(Data + pos) * 100 ; pos += 2;
		}

		if (color_)
		{
			Logic::SolidFill *fill = new Logic::SolidFill();

			BYTE R				= *(Data + pos); pos += 1;
			BYTE G				= *(Data + pos); pos += 1;
			BYTE B				= *(Data + pos); pos += 1;
			BYTE Index			= *(Data + pos); pos += 1;

			if (Index < 10)
			{
				//ODRAW::CorrectColorPPT(Index);
			}
			else
			{
				Logic::SrgbClr *color = new Logic::SrgbClr();

				color->SetRGB(R, G, B);
				fill->Color.Color = color;
			}

			props.Fill.Fill = fill;
			props.Fill.m_type = Logic::UniFill::solidFill;
		}

		if (BaseLineOffset_)
		{
			int BaseLineOffset = static_cast<int>((double)(*(_UINT16*)(Data + pos))); pos += 2;
		}

		// или два последних наоборот????

		return pos;
	}

} // namespace PPTX
