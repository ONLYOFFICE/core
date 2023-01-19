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

#include "AXS.h"
#include "GELFRAME.h"
#include "SHAPEPROPS.h"
#include "../Biff_records/IFmtRecord.h"
#include "../Biff_records/Tick.h"
#include "../Biff_records/FontX.h"
#include "../Biff_records/AxisLine.h"
#include "../Biff_records/LineFormat.h"
#include "../Biff_records/AreaFormat.h"
#include "../Biff_records/TextPropsStream.h"
#include "../Biff_records/ContinueFrt12.h"
#include "../Biff_records/Font.h"
#include "../Biff_records/StartObject.h"
#include "../Biff_records/EndObject.h"
namespace XLS
{


AXS::AXS()
{
}


AXS::~AXS()
{
}


//  (AxisLine LineFormat)
class Parenthesis_AXS_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_AXS_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_AXS_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<AxisLine>())
		{
			return false;
		}
		proc.mandatory<LineFormat>();

		if (proc.optional<StartObject>())
		{
			proc.optional<EndObject>();
		}
		return true;
	};
};



BaseObjectPtr AXS::clone()
{
	return BaseObjectPtr(new AXS(*this));
}


/*
AXS = [IFmtRecord] [Tick] [FontX] *4(AxisLine LineFormat) [AreaFormat] [GELFRAME] *4SHAPEPROPS 
		[TextPropsStream *ContinueFrt12]
*/
const bool AXS::loadContent(BinProcessor& proc)
{
	pGlobalWorkbookInfoPtr = proc.getGlobalWorkbookInfo();

	int count = 0;

	if (proc.optional<IFmtRecord>())
	{
		m_IFmtRecord = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<Tick>())
	{
		m_Tick = elements_.back();
		elements_.pop_back();
	}

	if (proc.optional<FontX>())
	{
		m_FontX = elements_.back();
		elements_.pop_back();
	}
	
	count = proc.repeated<Parenthesis_AXS_1>(0, 4);
	while(count > 0)
	{
		_axis_line_format a;
		a.axisLine		= elements_.front();		elements_.pop_front();
		a.lineFormat	= elements_.front();		elements_.pop_front();
		
		m_AxisLine_Format.push_back( a);
		count--;
	}

	if (proc.optional<AreaFormat>())
	{
		m_AreaFormat = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.optional<GELFRAME>())
	{
		m_GELFRAME = elements_.back();
		elements_.pop_back();
	}
	
	count = proc.repeated<SHAPEPROPS>(0, 4);

	if(proc.optional<TextPropsStream>())
	{
		count = proc.repeated<ContinueFrt12>(0, 0);
	}
	
	return true;
}

int AXS::serialize(std::wostream & _stream)
{
	BaseObjectPtr axis_line_format;
	Tick * tick = dynamic_cast<Tick*>(m_Tick.get());

	CP_XML_WRITER(_stream)    
	{
		for (size_t i = 0 ; i < m_AxisLine_Format.size(); i++)
		{
			std::wstring grid;
			
			AxisLine *axis_line = dynamic_cast<AxisLine*>(m_AxisLine_Format[i].axisLine.get());
			if (axis_line)
			{
				if (axis_line->id == (_UINT16)0) axis_line_format = m_AxisLine_Format[i].lineFormat;
				if (axis_line->id == (_UINT16)1) grid = L"c:majorGridlines";
				if (axis_line->id == (_UINT16)2) grid = L"c:minorGridlines";
				if (axis_line->id == (_UINT16)3) {}//wall || floor todooo
			}
			
			if (!grid.empty() && m_AxisLine_Format[i].lineFormat)
			{
				CP_XML_NODE(grid)
				{
					CP_XML_NODE(L"c:spPr")
					{			
						m_AxisLine_Format[i].lineFormat->serialize(CP_XML_STREAM());
					}
				}
			}
				
		}

		if (tick)
		{
			tick->serialize(_stream);
		}
		if (axis_line_format)
		{
			CP_XML_NODE(L"c:spPr")
			{
				axis_line_format->serialize(CP_XML_STREAM());
			}
		}
		if ((m_FontX) || ((tick) && (!tick->fAutoRot)))
		{
			CP_XML_NODE(L"c:txPr")
			{
				bool rtl = false;
				CP_XML_NODE(L"a:bodyPr")
				{
					if (tick)
					{
						if (tick->fAutoRot == false)
						{
							if (tick->trot == (_UINT16)0xffff)	CP_XML_ATTR(L"vert", L"vert");
							else
							{
								if (tick->trot > 90)	
								{
									CP_XML_ATTR(L"rot", (tick->trot - 90)	* 60000);	
									switch(tick->rot)
									{
										case 0: CP_XML_ATTR(L"vert", L"horz");			break;
										case 1: CP_XML_ATTR(L"vert", L"wordArtVert");	break;
										case 2: CP_XML_ATTR(L"vert", L"vert270");		break;
										case 3: CP_XML_ATTR(L"vert", L"vert");			break;
									}	
								}
								else
								{
									CP_XML_ATTR(L"rot",	-tick->trot			* 60000);
									CP_XML_ATTR(L"vert", L"horz");		//(1-333.xls) .. todoooo может тут нужно проверять гоизонт ось или нет
								}
								

							}
						}
						if (tick->iReadingOrder == (unsigned char)2) rtl = true;
					}
				}
			
				CP_XML_NODE(L"a:p")
				{
					CP_XML_NODE(L"a:pPr")
					{
						FontX * font = dynamic_cast<FontX*>(m_FontX.get());
						
						_CP_OPT(_UINT32) color;
						if (tick) color = ( tick->rgb.red << 16) + (tick->rgb.green << 8) + tick->rgb.blue;
						
                        int iFont = font ? (_UINT16)font->iFont : 0;

						serialize_rPr (CP_XML_STREAM(), iFont, color, rtl, true);
					}
					CP_XML_NODE(L"a:endParaRPr");
				}
			}
		}
	}
	return 0;
}

int AXS::serialize_rPr (std::wostream & _stream, int iFmt, _CP_OPT(unsigned int) color, bool rtl, bool defRPr)
{
	if (!pGlobalWorkbookInfoPtr) return 0;

	Font * font = NULL;
	int sz = pGlobalWorkbookInfoPtr->m_arFonts.size();
	
	if (iFmt > 0 && iFmt <= sz)
		font = dynamic_cast<Font*>(pGlobalWorkbookInfoPtr->m_arFonts.at(iFmt-1).get());

	if (font)
	{
		FillInfoExt cFont;
		if (color)
		{
			cFont = font->color_ext;
			font->color_ext.enabled = true;
			font->color_ext.xclrValue = *color;
			font->color_ext.xclrType = 2; //rgb
		}
		font->serialize_rPr(_stream, rtl, defRPr);

		if (color)font->color_ext = cFont;
	}
	else if (color)
	{
		CP_XML_WRITER(_stream)    
		{
			CP_XML_NODE(L"a:defRPr")
			{
				CP_XML_NODE(L"a:solidFill")
				{
					CP_XML_NODE(L"a:srgbClr")
					{
						CP_XML_ATTR(L"val", STR::toRGB(*color)); 
					}				
				}
			}
		}
	}
	
	return 0;
}
} // namespace XLS

