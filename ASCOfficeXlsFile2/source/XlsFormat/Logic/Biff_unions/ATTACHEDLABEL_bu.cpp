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

#include "ATTACHEDLABEL.h"
#include <Logic/Biff_records/Text.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/Pos.h>
#include <Logic/Biff_records/FontX.h>
#include <Logic/Biff_records/AlRuns.h>
#include <Logic/Biff_records/ObjectLink.h>
#include <Logic/Biff_records/DataLabExtContents.h>
#include <Logic/Biff_records/CrtLayout12.h>
#include <Logic/Biff_records/End.h>
#include <Logic/Biff_records/SeriesText.h>
#include <Logic/Biff_records/Font.h>

#include <Logic/Biff_unions/AI.h>
#include <Logic/Biff_unions/FRAME.h>
#include <Logic/Biff_unions/TEXTPROPS.h>
#include <Logic/Biff_unions/CRTMLFRT.h>

#include <utils.h>

namespace XLS
{


ATTACHEDLABEL::ATTACHEDLABEL()
{
	m_iLinkObject = -1;
}


ATTACHEDLABEL::~ATTACHEDLABEL()
{
}


BaseObjectPtr ATTACHEDLABEL::clone()
{
	return BaseObjectPtr(new ATTACHEDLABEL(*this));
}


/*
ATTACHEDLABEL = Text Begin Pos [FontX] [AlRuns] AI [FRAME] [ObjectLink] [DataLabExtContents] 
				[CrtLayout12] [TEXTPROPS] [CRTMLFRT] End
*/
const bool ATTACHEDLABEL::loadContent(BinProcessor& proc)
{
	pGlobalWorkbookInfoPtr = proc.getGlobalWorkbookInfo();

	if(!proc.mandatory<Text>())
	{
		return false;
	}
	m_TextProperties = elements_.back();
	elements_.pop_back();
	
	proc.mandatory<Begin>();	elements_.pop_back();

	if (proc.optional<Pos>())
	{
		m_Pos = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<FontX>())
	{
		m_FontX = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<AlRuns>())
	{
		m_AlRuns = elements_.back();
		elements_.pop_back();
	}

	if (proc.mandatory<AI>())
	{
		m_AI = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<FRAME>())
	{
		m_FRAME = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<ObjectLink>())
	{
		m_ObjectLink = elements_.back();
		elements_.pop_back();

		ObjectLink *o_l = dynamic_cast<ObjectLink*>(m_ObjectLink.get());
		
		m_iLinkObject = o_l->wLinkObj;

		Pos * pos = dynamic_cast<Pos*>(m_Pos.get());
		if (pos)
			pos->m_iLinkObject = m_iLinkObject;
	}
	
	if (proc.optional<DataLabExtContents>())
	{
		m_DataLabExtContents = elements_.back();
		elements_.pop_back();
	}
	
	proc.optional<CrtLayout12>();
	if (proc.optional<TEXTPROPS>())
	{
		m_TEXTPROPS = elements_.back();
		elements_.pop_back();
	}
	
	proc.optional<CRTMLFRT>();
	proc.mandatory<End>();			elements_.pop_back();

	return true;
}

int ATTACHEDLABEL::serialize_txPr(std::wostream & _stream)
{
	FontX *font			= dynamic_cast<FontX*>(m_FontX.get());
	Text * textProps	= dynamic_cast<Text*>(m_TextProperties.get());

	if (font == NULL) return 0;

	bool rtl = false;
	if((textProps) && (textProps->iReadingOrder == (unsigned char)2)) rtl = true;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:txPr")
		{
			CP_XML_NODE(L"a:bodyPr")
			{
				if (textProps)
				{
					if (textProps->trot == (_UINT16)0xff)	
					{
						CP_XML_ATTR(L"rot", 0);
						CP_XML_ATTR(L"vert", L"wordArtVert");	
					}
					else
					{
						if (textProps->trot > 90)	CP_XML_ATTR(L"rot", (textProps->trot - 90)	* 60000);						
						else						CP_XML_ATTR(L"rot",	-textProps->trot		* 60000);
						CP_XML_ATTR(L"vert", L"horz");	
					}
				}
			}
			CP_XML_NODE(L"a:lstStyle");
			CP_XML_NODE(L"a:p")
			{
				CP_XML_NODE(L"a:pPr")
				{
					serialize_rPr(CP_XML_STREAM(),font->iFont, rtl, true);
				}
				CP_XML_NODE(L"a:endParaRPr");
			}
		}
	}
	return 0;
}

int ATTACHEDLABEL::serialize(std::wostream & _stream)
{
	return serialize(_stream, true);
}

int ATTACHEDLABEL::serialize(std::wostream & _stream, bool isPosition)
{
	AI *ai	= dynamic_cast<AI *>( m_AI.get());
	
	if (ai == NULL) return 0;
	SeriesText * seriesText = dynamic_cast<SeriesText *>(ai->m_SeriesText.get());

	AlRuns *allRuns = dynamic_cast<AlRuns *>( m_AlRuns.get());
	int count_runs = allRuns ? allRuns->rgRuns.size() : 0;

	Text * textProps = dynamic_cast<Text*> (m_TextProperties.get());
	bool rtl = false;
	if((textProps) && (textProps->iReadingOrder == (unsigned char)2)) rtl = true;

	FRAME	*FRAME_ = dynamic_cast<FRAME*>	(m_FRAME.get());
	Pos		*Pos_	= dynamic_cast<Pos*>	(m_Pos.get());

	if (FRAME_ && Pos_)
	{
		Pos_->m_Frame = FRAME_->m_Frame;
	}

	CP_XML_WRITER(_stream)    
	{
		if (seriesText)
		{
			CP_XML_NODE(L"c:tx")
			{
				CP_XML_NODE(L"c:rich")
				{
					CP_XML_NODE(L"a:bodyPr")
					{
						if (textProps)
						{

							if (textProps->trot == (_UINT16)0xff)	
							{
								CP_XML_ATTR(L"rot", 0);
								CP_XML_ATTR(L"vert", L"wordArtVert");	
							}
							else
							{
								if (textProps->trot > 90)	CP_XML_ATTR(L"rot", (textProps->trot - 90)	* 60000);						
								else						CP_XML_ATTR(L"rot",	-textProps->trot		* 60000);
								CP_XML_ATTR(L"vert", L"horz");	
							}						
						}
					}
					CP_XML_NODE(L"a:p")
					{
						CP_XML_NODE(L"a:pPr")
						{
							FontX * font = dynamic_cast<FontX*>(m_FontX.get());
							if (font)
							{
								serialize_rPr (CP_XML_STREAM(),font->iFont, rtl, true);
							}
						}
						if (seriesText)//todoooo сделать вариант с DFTTEXT
						{	
							std::wstring & str_ = seriesText->stText.value();

							if (count_runs == 0)
							{
								CP_XML_NODE(L"a:r")
								{
									CP_XML_NODE(L"a:t")
									{
										CP_XML_STREAM() << xml::utils::replace_text_to_xml(str_);
									}
								}
							}
							else
							{
								int str_size = str_.size();
								for (int i = 0 ; i < count_runs; i++)
								{
									FormatRun *run = dynamic_cast<FormatRun*>(allRuns->rgRuns[i].get());
									if (run == NULL) continue;

									int end_string = str_size;

									if ( i < count_runs - 1)
									{
										FormatRun *run_next = dynamic_cast<FormatRun*>(allRuns->rgRuns[i+1].get());
										if (run_next)
											end_string = run_next->ich;
									}

									CP_XML_NODE(L"a:r")
									{
										serialize_rPr(CP_XML_STREAM(), run->ifnt, rtl, false );

										CP_XML_NODE(L"a:t")
										{		
											if (run->ich > str_.length())
											{
												//ошибка
												run->ich = 0;
											}

											std::wstring str_part = str_.substr( run->ich, end_string - run->ich);

											CP_XML_STREAM() << xml::utils::replace_text_to_xml(str_part);
										}
									}
								}
							}
						}
					}
				}	
			}
			if (m_Pos && isPosition)	m_Pos->serialize(_stream);
			if (m_FRAME)				m_FRAME->serialize(_stream);
		}
		else
		{
			if (m_Pos && isPosition)	m_Pos->serialize(_stream);
			if (m_FRAME)				m_FRAME->serialize(_stream);
			serialize_txPr(_stream);
		}
	
	}
	return 0;
}

int ATTACHEDLABEL::serialize_rPr (std::wostream & _stream, int iFmt, bool rtl, bool defRPr)
{
	if (!pGlobalWorkbookInfoPtr)			return 0;
	if (!pGlobalWorkbookInfoPtr->m_arFonts) return 0;

	int sz = pGlobalWorkbookInfoPtr->m_arFonts->size();
	if (iFmt -1 > sz || iFmt < 1) return 0;

	Font * font = dynamic_cast<Font*>(pGlobalWorkbookInfoPtr->m_arFonts->at(iFmt-1).get());

	Text * text_props = dynamic_cast<Text*>(m_TextProperties.get());
	
	_CP_OPT(_UINT16)		color;
	_CP_OPT(FillInfoExt)	color_ext;
	if (font)
	{	
		color		= font->icv;
		color_ext	= font->color_ext;	
		
		if ((text_props) && (!text_props->fAutoColor))
		{

			if (text_props->icvText != 0xff)
			{
				font->icv = text_props->icvText; //biff8
			}
			else
			{
				font->color_ext.enabled		= true;
				font->color_ext.xclrType	= 2;
				font->color_ext.xclrValue	= (text_props->rgbText.red << 16) + 
							(text_props->rgbText.green << 8) + (text_props->rgbText.blue);
			}
		}

		font->serialize_rPr(_stream, rtl, defRPr);
		
		font->icv		= *color;
		font->color_ext = *color_ext;
	}
	
	return 0;
}
} // namespace XLS

