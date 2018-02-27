/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "XF.h"

namespace XLS
{

XF::XF(size_t& cell_xf_current_id, size_t& style_xf_current_id) :	cell (cell_xf_current_id, style_xf_current_id),
																	style(cell_xf_current_id, style_xf_current_id)
{
	ixfParent = 0;
}


XF::~XF()
{
}


BaseObjectPtr XF::clone()
{
	return BaseObjectPtr(new XF(*this));
}

void XF::readFields(CFRecord& record)
{
	unsigned short flags;
	
	record >> ifnt >> ifmt >> flags;
	
	fLocked		= GETBIT(flags, 0);
	fHidden		= GETBIT(flags, 1);
	fStyle		= GETBIT(flags, 2);
	f123Prefix	= GETBIT(flags, 3);
	ixfParent	= GETBITS(flags, 4, 15);
	
	if(fStyle)
	{
		style.load(record);
	}
	else
	{
		cell.load(record);
	}
}
static const std::wstring HorAlign[8] = {L"general", L"left", L"center", L"right", L"fill", L"justify", L"centerContinuous", L"distributed"};
static const std::wstring VerAlign[5]  = {L"top", L"center", L"bottom", L"justify", L"distributed"};

int XF::serialize(std::wostream & stream)
{
     CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"xf")
		{	
			if (ifnt.value())			CP_XML_ATTR(L"fontId"	, ifnt.getValue());
			
			CP_XML_ATTR(L"numFmtId"	, ifmt);

			if(fStyle)
			{
				CP_XML_ATTR(L"borderId"	, style.border_x_id);
				CP_XML_ATTR(L"fillId"	, style.fill_x_id);
			
				//CP_XML_ATTR(L"applyFill"			, style.fAtrPat);
				//CP_XML_ATTR(L"applyBorder"		, style.fAtrBdr);
				//CP_XML_ATTR(L"applyNumberFormat"	, style.fAtrNum);

				if ((style.alc  >=0	&& style.alc  <8) || (style.alcV >=0 && style.alcV <5) || (style.fShrinkToFit) || (cell.cIndent > 0) || (style.trot > 0 && style.trot < 0xff) || (style.fWrap))
				{
					//CP_XML_ATTR(L"applyAlignment", style.fAtrAlc);
					CP_XML_NODE(L"alignment")
					{
						if (style.alc  >=0	&& style.alc  <8)	CP_XML_ATTR(L"horizontal"	, HorAlign[style.alc]);
						if (style.alcV >=0	&& style.alcV <5)	CP_XML_ATTR(L"vertical"		, VerAlign[style.alcV] );

						CP_XML_ATTR(L"shrinkToFit"	, style.fShrinkToFit);
						if (cell.cIndent > 0) CP_XML_ATTR(L"indent", style.cIndent);
						CP_XML_ATTR(L"wrapText"		, style.fWrap);
						
						if (style.trot > 0 && style.trot < 0xff) CP_XML_ATTR(L"textRotation"	, style.trot);
					}
				}		
			}
			else
			{
				CP_XML_ATTR(L"xfId"		, ixfParent );
				CP_XML_ATTR(L"borderId"	, cell.border_x_id);
				CP_XML_ATTR(L"fillId"	, cell.fill_x_id);

				CP_XML_ATTR(L"applyFill"		, cell.fAtrPat);
				CP_XML_ATTR(L"applyBorder"		, cell.fAtrBdr);
				CP_XML_ATTR(L"applyNumberFormat", cell.fAtrNum);
				CP_XML_ATTR(L"applyFont"		, cell.fAtrFnt);
				CP_XML_ATTR(L"applyAlignment"	, cell.fAtrAlc);

				if ((cell.alc  >=0	&& cell.alc  <8) || (cell.alcV >=0 && cell.alcV <5) || (cell.fShrinkToFit) || (cell.cIndent > 0) || (cell.trot > 0 && cell.trot < 0xff) || (cell.fWrap))
				{
					CP_XML_NODE(L"alignment")
					{
						if (cell.alc  >=0	&& cell.alc  <8)	CP_XML_ATTR(L"horizontal"	, HorAlign[cell.alc]);
						if (cell.alcV >=0	&& cell.alcV <5)	CP_XML_ATTR(L"vertical"		, VerAlign[cell.alcV] );
						
						CP_XML_ATTR(L"shrinkToFit"	, cell.fShrinkToFit);
						if (cell.cIndent > 0) CP_XML_ATTR(L"indent", cell.cIndent);
						CP_XML_ATTR(L"wrapText"		, cell.fWrap);
					
						if (cell.trot > 0 && cell.trot < 0xff) CP_XML_ATTR(L"textRotation"	, cell.trot);
					}
				}
			}
		}
	 }
	return 0;
}


} // namespace XLS

