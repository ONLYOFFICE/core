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
#include "Format.h"
#include "../../../../../OOXML/PPTXFormat/Theme.h"

namespace XLS
{
	XF_BIFF2::XF_BIFF2(size_t& cell_xf_current_id, size_t& style_xf_current_id) :
											XF(cell_xf_current_id, style_xf_current_id)
	{}
	XF_BIFF2::~XF_BIFF2()
	{}

	BaseObjectPtr XF_BIFF2::clone()
	{
		return BaseObjectPtr(new XF_BIFF2(*this));
	}
	void XF_BIFF2::readFields(CFRecord& record)
	{
		global_info = record.getGlobalWorkbookInfo();

		unsigned char ifmt_index, ifnt, notUsing, flags1, flags2;
		record >> ifnt >> notUsing >> flags1 >> flags2;
		font_index = ifnt < 4 ? ifnt : ifnt - 1;

		ifmt_index = GETBITS(flags1, 0, 5);
		fLocked = GETBIT(flags1, 6);
		fHidden = GETBIT(flags1, 7);

		alc = GETBITS(flags2, 0, 2);

		border.dgLeft = GETBIT(flags2, 3) ? 1 : 0;
		border.dgRight = GETBIT(flags2, 4) ? 1 : 0;
		border.dgTop = GETBIT(flags2, 5) ? 1 : 0;
		border.dgBottom = GETBIT(flags2, 6) ? 1 : 0;

		fill.fls = GETBIT(flags2, 7) ? 1 : 0;
//---------------------------------------------------------------------------------------------------	
		if (ifmt_index < global_info->m_arNumFormats.size() && ifmt_index != 0xff)
		{
			Format* fmt = dynamic_cast<Format*>(global_info->m_arNumFormats[ifmt_index].get());
			if (fmt)
				ifmt = fmt->ifmt;
		}

		global_info->mapUsedFormatCode.insert(std::make_pair(ifmt, true));

		if (font_index >= 0 && font_index < global_info->m_arFonts.size())
		{
			Font* pFont = dynamic_cast<Font*>(global_info->m_arFonts[font_index].get());
			if (pFont)
			{
				pFont->set(font);
			}
		}
	}
//-------------------------------------------------------------------------------------------------
	XF_BIFF3::XF_BIFF3(size_t& cell_xf_current_id, size_t& style_xf_current_id) :
											XF(cell_xf_current_id, style_xf_current_id)
	{}
	XF_BIFF3::~XF_BIFF3()
	{}

	BaseObjectPtr XF_BIFF3::clone()
	{
		return BaseObjectPtr(new XF_BIFF3(*this));
	}
	void XF_BIFF3::readFields(CFRecord& record)
	{
		global_info = record.getGlobalWorkbookInfo();

		unsigned char ifmt_index, ifnt, flags1, flags2;
		unsigned short	flags3, flags4;
		_UINT32 flags5;

		record >> ifnt >> ifmt_index >> flags1 >> flags2 >> flags3 >> flags4 >> flags5;
		font_index = ifnt < 4 ? ifnt : ifnt - 1;

		fLocked = GETBIT(flags1, 0);
		fHidden = GETBIT(flags1, 1);
		fStyle = GETBIT(flags1, 2);

		fAtrNum = GETBIT(flags2, 2);
		fAtrFnt = GETBIT(flags2, 3);
		fAtrAlc = GETBIT(flags2, 4);
		fAtrBdr = GETBIT(flags2, 5);
		fAtrPat = GETBIT(flags2, 6);
		fAtrProt = GETBIT(flags2, 7);

		alc = GETBITS(flags3, 0, 2);
		fWrap = GETBIT(flags3, 3);
		ixfParent = GETBITS(flags3, 4, 15);

		fill.fls = static_cast<unsigned char>(GETBITS(flags4, 0, 5));
		fill.icvFore = GETBITS(flags4, 0, 6);
		fill.icvBack = GETBITS(flags4, 7, 13);

		border.dgTop = static_cast<unsigned char>(GETBITS(flags5, 0, 2));
		border.icvTop = (0 != border.dgTop) ? static_cast<unsigned char>(GETBITS(flags5, 3, 7)) : 0;
		border.dgLeft = static_cast<unsigned char>(GETBITS(flags5, 8, 10));
		border.icvLeft = (0 != border.dgLeft) ? static_cast<unsigned char>(GETBITS(flags5, 11, 15)) : 0;
		border.dgBottom = static_cast<unsigned char>(GETBITS(flags5, 16, 18));
		border.icvBottom = (0 != border.dgBottom) ? static_cast<unsigned char>(GETBITS(flags5, 19, 23)) : 0;
		border.dgRight = static_cast<unsigned char>(GETBITS(flags5, 24, 26));
		border.icvRight = (0 != border.dgRight) ? static_cast<unsigned char>(GETBITS(flags5, 27, 31)) : 0;
//---------------------------------------------------------------------------------------------------	
		if (ifmt_index < global_info->m_arNumFormats.size() && ifmt_index != 0xff)
		{
			Format* fmt = dynamic_cast<Format*>(global_info->m_arNumFormats[ifmt_index].get());
			if (fmt)
				ifmt = fmt->ifmt;
		}

		global_info->mapUsedFormatCode.insert(std::make_pair(ifmt, true));

		if (font_index >= 0 && font_index < global_info->m_arFonts.size())
		{
			Font* pFont = dynamic_cast<Font*>(global_info->m_arFonts[font_index].get());
			if (pFont)
			{
				pFont->set(font);
			}
		}
	}
//-------------------------------------------------------------------------------------------------
	XF_BIFF4::XF_BIFF4(size_t& cell_xf_current_id, size_t& style_xf_current_id) :
												XF(cell_xf_current_id, style_xf_current_id)
	{}
	XF_BIFF4::~XF_BIFF4()
	{}

	BaseObjectPtr XF_BIFF4::clone()
	{
		return BaseObjectPtr(new XF_BIFF4(*this));
	}
//-------------------------------------------------------------------------------------------------
	XF::XF (size_t& cell_xf_current_id, size_t& style_xf_current_id) :
		cell_xf_current_id_(cell_xf_current_id), style_xf_current_id_(style_xf_current_id)
	{
		border_x_id = -1;
		fill_x_id	= -1;
		font_x_id	= -1;
		
		fill.fls	= 0;

		ixfParent = 0;
		font_index = 0xffff;
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

	unsigned char ifmt_index = 0xff;
	char orient = 0xff;

	if (global_info->Version == 0x0400)
	{
		unsigned short	flags1, flags4;
		unsigned char ifnt, flags2, flags3;
		_UINT32 flags5;

		record >> ifnt >> ifmt_index >> flags1 >> flags2 >> flags3 >> flags4 >> flags5;
		font_index = ifnt < 4 ? ifnt : ifnt - 1;
		
		fLocked = GETBIT(flags1, 0);
		fHidden = GETBIT(flags1, 1);
		fStyle = GETBIT(flags1, 2);
		ixfParent = GETBITS(flags1, 4, 15);
		
		alc = GETBITS(flags2, 0, 2);
		fWrap = GETBIT(flags2, 3);
		alcV = GETBITS(flags2, 4, 5);
		orient = static_cast<unsigned char>(GETBITS(flags2, 6, 7));
		
		fAtrNum = GETBIT(flags3, 2);
		fAtrFnt = GETBIT(flags3, 3);
		fAtrAlc = GETBIT(flags3, 4);
		fAtrBdr = GETBIT(flags3, 5);
		fAtrPat = GETBIT(flags3, 6);
		fAtrProt = GETBIT(flags3, 7);

		fill.fls = static_cast<unsigned char>(GETBITS(flags4, 0, 5));

		fill.icvFore = GETBITS(flags4, 0, 6);
		fill.icvBack = GETBITS(flags4, 7, 13);
		
		border.dgTop = static_cast<unsigned char>(GETBITS(flags5, 0, 2));
		border.icvTop = (0 != border.dgTop) ? static_cast<unsigned char>(GETBITS(flags5, 3, 7)) : 0;
		border.dgLeft = static_cast<unsigned char>(GETBITS(flags5, 8, 10));
		border.icvLeft = (0 != border.dgLeft) ? static_cast<unsigned char>(GETBITS(flags5, 11, 15)) : 0;
		border.dgBottom = static_cast<unsigned char>(GETBITS(flags5, 16, 18));
		border.icvBottom = (0 != border.dgBottom) ? static_cast<unsigned char>(GETBITS(flags5, 19, 23)) : 0;
		border.dgRight = static_cast<unsigned char>(GETBITS(flags5, 24, 26));
		border.icvRight = (0 != border.dgRight) ? static_cast<unsigned char>(GETBITS(flags5, 27, 31)) : 0;
	}
	else if (global_info->Version == 0x0500)
	{
		unsigned short	flags1;
		unsigned char	flags2, flags3;
		_UINT32 flags4, flags5;

		FontIndex ifnt;
		record >> ifnt >> ifmt >> flags1 >> flags2 >> flags3 >> flags4 >> flags5;
		font_index = ifnt.getValue();

		fLocked = GETBIT(flags1, 0);
		fHidden = GETBIT(flags1, 1);
		fStyle = GETBIT(flags1, 2);
		ixfParent = GETBITS(flags1, 4, 15);

		alc = GETBITS(flags2, 0, 2);
		fWrap = GETBIT(flags2, 3);
		alcV = GETBITS(flags2, 4, 6);

		orient = static_cast<unsigned char>(GETBITS(flags3, 0, 1));
		fAtrNum = GETBIT(flags3, 2);
		fAtrFnt = GETBIT(flags3, 3);
		fAtrAlc = GETBIT(flags3, 4);
		fAtrBdr = GETBIT(flags3, 5);
		fAtrPat = GETBIT(flags3, 6);
		fAtrProt = GETBIT(flags3, 7);

		fill.fls = static_cast<unsigned char>(GETBITS(flags4, 16, 21));

		fill.icvFore = GETBITS(flags4, 0, 6);
		fill.icvBack = GETBITS(flags4, 7, 13);

		border.dgTop = static_cast<unsigned char>(GETBITS(flags5, 0, 2));
		border.dgLeft = static_cast<unsigned char>(GETBITS(flags5, 3, 5));
		border.dgRight = static_cast<unsigned char>(GETBITS(flags5, 6, 8));
		border.dgBottom = static_cast<unsigned char>(GETBITS(flags4, 22, 24));

		border.icvTop = (0 != border.dgTop) ? static_cast<unsigned char>(GETBITS(flags5, 9, 15)) : 0;
		border.icvLeft = (0 != border.dgLeft) ? static_cast<unsigned char>(GETBITS(flags5, 16, 22)) : 0;
		border.icvRight = (0 != border.dgRight) ? static_cast<unsigned char>(GETBITS(flags5, 23, 29)) : 0;
		border.icvBottom = (0 != border.dgBottom) ? static_cast<unsigned char>(GETBITS(flags4, 25, 31)) : 0;
	}
	else if (global_info->Version == 0x0600)
	{
        unsigned short	flags1, flags5;
		_UINT32 flags2, flags3, flags4;

		FontIndex ifnt;
		record >> ifnt >> ifmt >> flags1 >> flags2 >> flags3 >> flags4 >> flags5;
		font_index = ifnt.getValue();

        fLocked		= GETBIT(flags1, 0);
        fHidden		= GETBIT(flags1, 1);
        fStyle		= GETBIT(flags1, 2);
        f123Prefix	= GETBIT(flags1, 3);
        ixfParent	= GETBITS(flags1, 4, 15);

		alc = static_cast<unsigned char>(GETBITS(flags2, 0, 2));
		fWrap = GETBIT(flags2, 3);
		alcV = static_cast<unsigned char>(GETBITS(flags2, 4, 6));
		fJustLast = GETBIT(flags2, 7);

		trot = static_cast<unsigned char>(GETBITS(flags2, 8, 15));
		cIndent = static_cast<unsigned char>(GETBITS(flags2, 16, 19));
		fShrinkToFit = GETBIT(flags2, 20);
		iReadOrder = static_cast<unsigned char>(GETBITS(flags2, 22, 23));

		if (!fStyle)
		{
			fAtrNum = GETBIT(flags2, 26);
			fAtrFnt = GETBIT(flags2, 27);
			fAtrAlc = GETBIT(flags2, 28);
			fAtrBdr = GETBIT(flags2, 29);
			fAtrPat = GETBIT(flags2, 30);
			fAtrProt = GETBIT(flags2, 31);
		}

		border.dgLeft = static_cast<unsigned char>(GETBITS(flags3, 0, 3));
		border.dgRight = static_cast<unsigned char>(GETBITS(flags3, 4, 7));
		border.dgTop = static_cast<unsigned char>(GETBITS(flags3, 8, 11));
		border.dgBottom = static_cast<unsigned char>(GETBITS(flags3, 12, 15));
		border.dgDiag = static_cast<unsigned char>(GETBITS(flags4, 21, 24));

		border.icvLeft = 0 != border.dgLeft ? static_cast<unsigned char>(GETBITS(flags3, 16, 22)) : 0;
		border.icvRight = 0 != border.dgRight ? static_cast<unsigned char>(GETBITS(flags3, 23, 29)) : 0;
		border.icvTop = 0 != border.dgTop ? static_cast<unsigned char>(GETBITS(flags4, 0, 6)) : 0;
		border.icvBottom = 0 != border.dgBottom ? static_cast<unsigned char>(GETBITS(flags4, 7, 13)) : 0;
		border.icvDiag = 0 != border.dgDiag ? static_cast<unsigned char>(GETBITS(flags4, 14, 20)) : 0;

		border.grbitDiag = static_cast<unsigned char>(GETBITS(flags3, 30, 31));

		if (!fStyle)
		{
			fHasXFExt = GETBIT(flags4, 25);
		}
		fill.fls = static_cast<unsigned char>(GETBITS(flags4, 26, 31));

		fill.icvFore = GETBITS(flags5, 0, 6);
		fill.icvBack = GETBITS(flags5, 7, 13);

		if (!fStyle)
		{
			fsxButton = GETBIT(flags5, 14);
		}
    }
    else
    {
        _UINT32 flags;
        unsigned char	trot_;
        record >> ixfParent >> ifmt >> font_index >> iFill >> ixBorder >> trot_ >> cIndent >> flags;

		global_info->mapUsedFormatCode.insert(std::make_pair(ifmt, true));
		
		trot = trot_;

        alc             = GETBITS(flags, 0, 2);
        alcV            = GETBITS(flags, 3, 5);
        fWrap           = GETBIT(flags, 6);
        fJustLast       = GETBIT(flags, 7);
        fShrinkToFit    = GETBIT(flags, 8);
        fMergeCell      = GETBIT(flags, 9);
        iReadOrder      = GETBITS(flags, 10, 11);
        fLocked         = GETBIT(flags, 12);
        fHidden         = GETBIT(flags, 13);
        fsxButton       = GETBIT(flags, 14);
        f123Prefix      = GETBIT(flags, 15);
        xfGrbitAtr      = GETBITS(flags, 16, 21);

        fAtrNum         = !GETBIT(xfGrbitAtr, 0);
        fAtrFnt         = !GETBIT(xfGrbitAtr, 1);
        fAtrAlc         = !GETBIT(xfGrbitAtr, 2);
        fAtrBdr         = !GETBIT(xfGrbitAtr, 3);
        fAtrPat         = !GETBIT(xfGrbitAtr, 4);
        fAtrProt        = !GETBIT(xfGrbitAtr, 5);
    }
//---------------------------------------------------------------------------------------------------	
	switch (orient)
	{
		case 0:	trot = 0;		break;		// Text orientation: not rotated.
		case 1: trot = 0xff;	break;		// Text orientation: vertically stacked.
		case 2: trot = 90;		break;		// Text orientation: 90 deg counterclockwise.
		case 3: trot = 270;		break;		// Text orientation: 90 deg clockwise.
	}
	if (ifmt_index < global_info->m_arNumFormats.size() && ifmt_index != 0xff)
	{
		Format* fmt = dynamic_cast<Format*>(global_info->m_arNumFormats[ifmt_index].get());
		if (fmt)
			ifmt = fmt->ifmt;
	}

	global_info->mapUsedFormatCode.insert(std::make_pair(ifmt, true));

	if (font_index >= 0 && font_index < global_info->m_arFonts.size())
	{
		Font* pFont = dynamic_cast<Font*>(global_info->m_arFonts[font_index].get());
		if (pFont)
		{
			pFont->set(font);
		}
	}
}
void XF::Update(ExtProp* ext_prop)
{
	if (!ext_prop) return;

	FillInfoExt color_out;

	{
		color_out.enabled		= true;
		color_out.icv			= ext_prop->extPropData.color.xclrValue;
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
			case 0x001C:	font.fItalic	= (*byte_->value() != 0); break;
			case 0x001D:	font.fStrikeOut	= (*byte_->value() != 0); break;
			case 0x001E:	font.fOutline	= (*byte_->value() != 0); break;
			case 0x001F:	font.fShadow	= (*byte_->value() != 0); break;
			case 0x0022:	font.bCharSet	= (*byte_->value() != 0); break;
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

						CP_XML_ATTR(L"shrinkToFit", fShrinkToFit);
						if (cIndent > 0) CP_XML_ATTR(L"indent", cIndent);
						CP_XML_ATTR(L"wrapText", fWrap);
						
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
				
				if (fAtrProt)
					CP_XML_ATTR(L"applyProtection", fAtrProt);


				if ((alc >= 0 && alc < 8) || (alcV >= 0 && alcV < 5) || (fShrinkToFit) || 
					(cIndent > 0) || (trot > 0 && trot < 0xff) || (fWrap))
				{
					CP_XML_NODE(L"alignment")
					{
						if (alc  >= 0	&& alc  < 8)	CP_XML_ATTR(L"horizontal"	, HorAlign[alc]);
						if (alcV >= 0	&& alcV < 5)	CP_XML_ATTR(L"vertical"		, VerAlign[alcV] );
						
						CP_XML_ATTR(L"shrinkToFit", fShrinkToFit);
						if (cIndent > 0) CP_XML_ATTR(L"indent", cIndent);
						CP_XML_ATTR(L"wrapText", fWrap);
					
						if (trot > 0 && trot < 0xff) CP_XML_ATTR(L"textRotation", trot);
					}
				}
				if (fAtrProt && !fLocked)
				{
					CP_XML_NODE(L"protection")
					{
						CP_XML_ATTR(L"locked", false);
					}
				}
			}
		}
	 }
	return 0;
}


} // namespace XLS

