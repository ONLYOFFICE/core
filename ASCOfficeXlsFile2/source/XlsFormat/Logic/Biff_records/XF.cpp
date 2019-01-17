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

#include "XF.h"
#include "../Biff_structures/XFProp.h"
#include "../Biff_structures/ExtProp.h"
#include "../Biff_structures/XFPropColor.h"
#include "../Biff_structures/BitMarkedStructs.h"
#include "../Biff_structures/XFPropBorder.h"
#include "../Biff_structures/FontIndex.h"
#include "Font.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Theme.h"

namespace XLS
{

	XF::XF (size_t& cell_xf_current_id, size_t& style_xf_current_id) :
		cell_xf_current_id_(cell_xf_current_id), style_xf_current_id_(style_xf_current_id)
	{
		alc = alcV = 0;

		border_x_id = -1;
		fill_x_id	= -1;
		font_x_id	= -1;
		
		fill.fls	= 0;

		ixfParent = 0;
		font_index = 0xffff;
		
		fAtrNum = false;
		fAtrFnt = false;
		fAtrAlc = false;
		fAtrBdr = false;
		fAtrPat = false;
		fAtrProt = false;
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
	global_info = record.getGlobalWorkbookInfo();
	
	unsigned short	flags;
	FontIndex		ifnt;

	record >> ifnt >> ifmt >> flags;

	font_index = ifnt.getValue();
	
	if (font_index >= 0 && font_index < global_info->m_arFonts.size())
	{
		Font* pFont = dynamic_cast<Font*>(global_info->m_arFonts[font_index].get());
		if (pFont)
		{
			pFont->set(font);
		}
	}

	fLocked		= GETBIT(flags, 0);
	fHidden		= GETBIT(flags, 1);
	fStyle		= GETBIT(flags, 2);
	f123Prefix	= GETBIT(flags, 3);
	ixfParent	= GETBITS(flags, 4, 15);
	
	if (global_info->Version < 0x0600)
	{
		_UINT16 flags1;
		_UINT32 flags2;
		_UINT32 flags3;

		record >> flags1 >> flags2 >> flags3;
		
		alc					= GETBITS(flags1, 0, 2);
		fWrap				= GETBIT (flags1, 3);
		alcV				= GETBITS(flags1, 4, 6);
		fJustLast			= GETBIT (flags1, 7);
		trot				= 0;//static_cast<unsigned char>(GETBITS(flags1, 8, 15));	
		cIndent				= 0;//static_cast<unsigned char>(GETBITS(flags1, 16, 19));
		fShrinkToFit		= 0;//GETBIT(flags1, 20);
		iReadOrder			= 0;//static_cast<unsigned char>(GETBITS(flags1, 22, 23));
			
		char orient			= static_cast<unsigned char>(GETBITS(flags1, 8, 10));

		switch(orient)
		{
			case 0:	trot = 0;		break;		// Text orientation: not rotated.
			case 1: trot = 0xff;	break;		// Text orientation: vertically stacked.
			case 2: trot = 90;		break;		// Text orientation: 90 deg counterclockwise.
			case 3: trot = 270;		break;		// Text orientation: 90 deg clockwise.
		}
		if(!fStyle)
		{
			fAtrNum = GETBIT(flags2, 10);
			fAtrFnt = GETBIT(flags2, 11);
			fAtrAlc = GETBIT(flags2, 12);
			fAtrBdr = GETBIT(flags2, 13);
			fAtrPat = GETBIT(flags2, 14);
			fAtrProt = GETBIT(flags2, 15);
		}

		fill.fls = static_cast<unsigned char>(GETBITS(flags2, 16, 21));
		
		fill.icvFore = GETBITS(flags2, 0, 6);
		fill.icvBack = GETBITS(flags2, 7, 13);
		
		border.dgTop		= static_cast<unsigned char>(GETBITS(flags3, 0, 2));
		border.dgLeft		= static_cast<unsigned char>(GETBITS(flags3, 3, 5));
		border.dgRight		= static_cast<unsigned char>(GETBITS(flags3, 6, 8));		
		border.dgBottom		= static_cast<unsigned char>(GETBITS(flags2, 22, 24));	
		border.dgDiag		= 0;
		border.grbitDiag	= 0;
		
		border.icvTop		= (0 != border.dgTop)	? static_cast<unsigned char>(GETBITS(flags3,  9, 15))	: 0;
		border.icvLeft		= (0 != border.dgLeft)	? static_cast<unsigned char>(GETBITS(flags3, 16, 22))	: 0;
		border.icvRight		= (0 != border.dgRight)	? static_cast<unsigned char>(GETBITS(flags3, 23, 29))	: 0;
		border.icvBottom	= (0 != border.dgBottom)? static_cast<unsigned char>(GETBITS(flags2, 25, 31))	: 0;
		border.icvDiag		= 0;
	}
	else
	{
		_UINT32 flags1;
		_UINT32 flags2;
		_UINT32 flags3;
		_UINT16 flags4;

		record >> flags1 >> flags2 >> flags3 >> flags4;
		
		alc = static_cast<unsigned char>(GETBITS(flags1, 0, 2));
		fWrap = GETBIT(flags1, 3);
		alcV = static_cast<unsigned char>(GETBITS(flags1, 4, 6));
		fJustLast = GETBIT(flags1, 7);
		trot = static_cast<unsigned char>(GETBITS(flags1, 8, 15));
		cIndent = static_cast<unsigned char>(GETBITS(flags1, 16, 19));
		fShrinkToFit = GETBIT(flags1, 20);
		iReadOrder = static_cast<unsigned char>(GETBITS(flags1, 22, 23));
		
		if(!fStyle)
		{
			fAtrNum = GETBIT(flags1, 26);
			fAtrFnt = GETBIT(flags1, 27);
			fAtrAlc = GETBIT(flags1, 28);
			fAtrBdr = GETBIT(flags1, 29);
			fAtrPat = GETBIT(flags1, 30);
			fAtrProt = GETBIT(flags1, 31);
		}

		border.dgLeft	= static_cast<unsigned char>(GETBITS(flags2, 0, 3));
		border.dgRight	= static_cast<unsigned char>(GETBITS(flags2, 4, 7));
		border.dgTop	= static_cast<unsigned char>(GETBITS(flags2, 8, 11));
		border.dgBottom = static_cast<unsigned char>(GETBITS(flags2, 12, 15));
		border.dgDiag	= static_cast<unsigned char>(GETBITS(flags3, 21, 24));

		border.icvLeft		= 0 != border.dgLeft	? static_cast<unsigned char>(GETBITS(flags2, 16, 22))	: 0;
		border.icvRight		= 0 != border.dgRight	? static_cast<unsigned char>(GETBITS(flags2, 23, 29))	: 0;
		border.icvTop		= 0 != border.dgTop		? static_cast<unsigned char>(GETBITS(flags3, 0, 6))		: 0;
		border.icvBottom	= 0 != border.dgBottom	? static_cast<unsigned char>(GETBITS(flags3, 7, 13))	: 0;
		border.icvDiag		= 0 != border.dgDiag	? static_cast<unsigned char>(GETBITS(flags3, 14, 20))	: 0;
		
		border.grbitDiag = static_cast<unsigned char>(GETBITS(flags2, 30, 31));

		if(!fStyle)
		{
			fHasXFExt		= GETBIT(flags3, 25);
		}
		fill.fls		= static_cast<unsigned char>(GETBITS(flags3, 26, 31));

		fill.icvFore	= GETBITS(flags4, 0, 6);
		fill.icvBack	= GETBITS(flags4, 7, 13);
		
		if(!fStyle)
		{
			fsxButton		= GETBIT(flags4, 14);
		}
	}
}
void XF::Update(ExtProp* ext_prop)
{
	if (!ext_prop) return;

	FillInfoExt color_out;

	{
		color_out.enabled		= true;
		color_out.icv			= ext_prop->extPropData.color.icv;
		color_out.xclrType		= ext_prop->extPropData.color.xclrType;
		color_out.nTintShade	= ext_prop->extPropData.color.nTintShade;
		color_out.xclrValue		= ext_prop->extPropData.color.xclrValue;
	}
	switch(ext_prop->extType)
	{
		case 0x0004:
		{
			fill.foreFillInfo_ = color_out;
		}break;
		case 0x0005:
		{
			fill.backFillInfo_ = color_out;
		}break;
		case 0x0007:
		{
			border.topFillInfo_ = color_out;
		}break;
		case 0x0008:
		{
			border.bottomFillInfo_ = color_out;
		}break;
		case 0x0009:
		{
			border.leftFillInfo_ = color_out;
		}break;
		case 0x000A:
		{
			border.rightFillInfo_ = color_out;
		}break;
		//case 0x000B:	//diag color
		//case 0x000C:	//diag color
		case 0x000D:
		{
			font.color = color_out;
		}break;
		//case 0x0006:
		//	extPropData.gradient_fill.toXML(own_tag);
		//	break;
		case 0x000E:
		{
			font.scheme = ext_prop->extPropData.font_scheme;
			
			if (global_info->m_pTheme && font.scheme == 0x01)
			{
				font.name = global_info->m_pTheme->themeElements.fontScheme.majorFont.latin.typeface;
			}
			else if (global_info->m_pTheme && font.scheme == 0x02)
			{
				font.name = global_info->m_pTheme->themeElements.fontScheme.minorFont.latin.typeface;
			}
		}break;
		case 0x000F:
		{
			cIndent = ext_prop->extPropData.indent_level;
		}break;
	}
}
void XF::Update(XFProp* xfProp)
{
	if (!xfProp) return;

	XFPropColor* color = dynamic_cast<XFPropColor*>(xfProp->xfPropDataBlob.get());
	if (color)
	{
		FillInfoExt color_out;
		
		color_out.enabled		= true;
		color_out.icv			= color->icv;
		color_out.xclrType		= color->xclrType;
		color_out.nTintShade	= color->nTintShade;

		if (color->xclrType == 2)color_out.xclrValue = color->dwRgba.ToRGBA();
		else					 color_out.xclrValue = color->icv; // 1 - icv, 3 - theme

		switch(xfProp->xfPropType)
		{
			case 0x0001:	fill.foreFillInfo_	= color_out; break;
			case 0x0002:	fill.backFillInfo_	= color_out; break;
			case 0x0005:	font.color			= color_out; break;
		}
		return;
	}
	BIFF_BYTE* byte_ = dynamic_cast<BIFF_BYTE*>(xfProp->xfPropDataBlob.get());

	if (byte_)
	{
		switch(xfProp->xfPropType)
		{
			case 0x0025:
			{
				font.scheme = *byte_->value(); //from Theme !!!!

				if (global_info->m_pTheme && font.scheme == 0x01)
				{
					font.name = global_info->m_pTheme->themeElements.fontScheme.majorFont.latin.typeface;
				}
				else if (global_info->m_pTheme && font.scheme == 0x02)
				{
					font.name = global_info->m_pTheme->themeElements.fontScheme.minorFont.latin.typeface;
				}
			}break;
			case 0x001C:	font.fItalic	= *byte_->value(); break;
			case 0x001D:	font.fStrikeOut	= *byte_->value(); break;
			case 0x001E:	font.fOutline	= *byte_->value(); break;
			case 0x001F:	font.fShadow	= *byte_->value(); break;
			case 0x0022:	font.bCharSet	= *byte_->value(); break;
		}	
		return;
	}
	BIFF_WORD* word = dynamic_cast<BIFF_WORD*>(xfProp->xfPropDataBlob.get());

	if (word)
	{
		switch(xfProp->xfPropType)
		{
			case 0x0019:	font.bls	= *word->value(); break;
			case 0x001A:	font.uls	= *word->value(); break;		
			case 0x001B:	font.sss	= *word->value(); break;
		}
		return;
	}
	BIFF_DWORD* dword = dynamic_cast<BIFF_DWORD*>(xfProp->xfPropDataBlob.get());

	if (dword)
	{
		switch(xfProp->xfPropType)
		{
			case 0x0024:	font.dyHeight = *dword->value(); break;
		}
		return;	
	}
	XFPropBorder *brdr = dynamic_cast<XFPropBorder*>(xfProp->xfPropDataBlob.get());
	if (brdr)
	{
		FillInfoExt color_out;
		
		color_out.enabled		= true;
		color_out.icv			= brdr->color.icv;
		color_out.xclrType		= brdr->color.xclrType;
		color_out.nTintShade	= brdr->color.nTintShade;

		if (brdr->color.xclrType == 3)	color_out.xclrValue = brdr->color.icv;
		else							color_out.xclrValue = brdr->color.dwRgba.ToRGBA();

		switch(xfProp->xfPropType)
		{
			case 0x0006:	border.topFillInfo_		= color_out; break;
			case 0x0007:	border.bottomFillInfo_	= color_out; break;
			case 0x0008:	border.leftFillInfo_	= color_out; break;
			case 0x0009:	border.rightFillInfo_	= color_out; break;
				//horiz, vert, diag
		}	
		return;
	}
}
void XF::RegisterFillBorderFont()
{
    for (size_t i = 0; i < ext_props.size(); i++ ) //xls ext styles
	{
		ExtProp* ext_prop = dynamic_cast<ExtProp*>(ext_props[i].get());
		Update (ext_prop);
	}
    for (size_t i = 0; i < xf_props.size(); i++ ) //xlsx ext styles 
	{
		XFProp* xf_prop = dynamic_cast<XFProp*>(xf_props[i].get());
		Update (xf_prop);
	}
	border_x_id	= global_info->RegisterBorderId(border);
	fill_x_id	= global_info->RegisterFillId(fill);
	font_x_id	= global_info->RegisterFontId(font);
}

static const std::wstring HorAlign[8] = {L"general", L"left", L"center", L"right", L"fill", L"justify", L"centerContinuous", L"distributed"};
static const std::wstring VerAlign[5]  = {L"top", L"center", L"bottom", L"justify", L"distributed"};

int XF::serialize(std::wostream & stream)
{
     CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"xf")
		{	
			CP_XML_ATTR(L"fontId", font_x_id);
				
			CP_XML_ATTR(L"numFmtId"	, ifmt);

			if(fStyle)
			{
				CP_XML_ATTR(L"borderId"	, border_x_id);
				CP_XML_ATTR(L"fillId"	, fill_x_id);
			
				if ((alc >= 0 && alc < 8 ) || (alcV >= 0 && alcV < 5) || (fShrinkToFit) || (cIndent > 0) || (trot > 0 && trot < 0xff) || (fWrap))
				{
					CP_XML_NODE(L"alignment")
					{
						if (alc  >=0	&& alc  <8)	CP_XML_ATTR(L"horizontal"	, HorAlign[alc]);
						if (alcV >=0	&& alcV <5)	CP_XML_ATTR(L"vertical"		, VerAlign[alcV] );

						CP_XML_ATTR(L"shrinkToFit"	, fShrinkToFit);
						if (cIndent > 0) CP_XML_ATTR(L"indent", cIndent);
						CP_XML_ATTR(L"wrapText"		, fWrap);
						
						if (trot > 0 && trot < 0xff) CP_XML_ATTR(L"textRotation"	, trot);
					}
				}		
			}
			else
			{
				CP_XML_ATTR(L"xfId"		, ixfParent );
				CP_XML_ATTR(L"borderId"	, border_x_id);
				CP_XML_ATTR(L"fillId"	, fill_x_id);

				CP_XML_ATTR(L"applyFill"		, fAtrPat);
				CP_XML_ATTR(L"applyBorder"		, fAtrBdr);
				CP_XML_ATTR(L"applyNumberFormat", fAtrNum);
				CP_XML_ATTR(L"applyFont"		, fAtrFnt);
				CP_XML_ATTR(L"applyAlignment"	, fAtrAlc);

				if ((alc >= 0 && alc < 8) || (alcV >= 0 && alcV < 5) || (fShrinkToFit) || 
					(cIndent > 0) || (trot > 0 && trot < 0xff) || (fWrap))
				{
					CP_XML_NODE(L"alignment")
					{
						if (alc  >= 0	&& alc  < 8)	CP_XML_ATTR(L"horizontal"	, HorAlign[alc]);
						if (alcV >= 0	&& alcV < 5)	CP_XML_ATTR(L"vertical"		, VerAlign[alcV] );
						
						CP_XML_ATTR(L"shrinkToFit"	, fShrinkToFit);
						if (cIndent > 0) CP_XML_ATTR(L"indent", cIndent);
						CP_XML_ATTR(L"wrapText"		, fWrap);
					
						if (trot > 0 && trot < 0xff) CP_XML_ATTR(L"textRotation", trot);
					}
				}
			}
		}
	 }
	return 0;
}


} // namespace XLS

