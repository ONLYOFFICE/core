/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include "CellXF.h"
#include "ExtProp.h"
#include <Binary/CFRecord.h>

namespace XLS
{


CellXF::CellXF(size_t& cell_xf_current_id, size_t& style_xf_current_id) :	cell_xf_current_id_(cell_xf_current_id), 
																			style_xf_current_id_(style_xf_current_id)
{
	alc = alcV = 0;
	font_id = 0xFFFF;
}


BiffStructurePtr CellXF::clone()
{
	return BiffStructurePtr(new CellXF(*this));
}

void CellXF::load(CFRecord& record)
{
	m_GlobalWorkbookInfo = record.getGlobalWorkbookInfo();

	if (m_GlobalWorkbookInfo->Version < 0x0600)
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
		fAtrNum = GETBIT(flags2, 10);
		fAtrFnt = GETBIT(flags2, 11);
		fAtrAlc = GETBIT(flags2, 12);
		fAtrBdr = GETBIT(flags2, 13);
		fAtrPat = GETBIT(flags2, 14);
		fAtrProt = GETBIT(flags2, 15);

		fill.fls = static_cast<unsigned char>(GETBITS(flags2, 16, 21));
		
		fill.icvFore = GETBITS(flags2, 0, 6);
		fill.icvBack = GETBITS(flags2, 7, 13);
		
		border.dgTop		= static_cast<unsigned char>(GETBITS(flags3, 0, 2));
		border.dgLeft		= static_cast<unsigned char>(GETBITS(flags3, 3, 5));
		border.dgRight		= static_cast<unsigned char>(GETBITS(flags3, 6, 8));		
		border.dgBottom		= static_cast<unsigned char>(GETBITS(flags2, 22, 24));	
		border.dgDiag		= 0;
		
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
		
		fAtrNum = GETBIT(flags1, 26);
		fAtrFnt = GETBIT(flags1, 27);
		fAtrAlc = GETBIT(flags1, 28);
		fAtrBdr = GETBIT(flags1, 29);
		fAtrPat = GETBIT(flags1, 30);
		fAtrProt = GETBIT(flags1, 31);

		border.dgLeft	= static_cast<unsigned char>(GETBITS(flags2, 0, 3));
		border.dgRight	= static_cast<unsigned char>(GETBITS(flags2, 4, 7));
		border.dgTop	= static_cast<unsigned char>(GETBITS(flags2, 8, 11));
		border.dgBottom = static_cast<unsigned char>(GETBITS(flags2, 12, 15));
		border.dgDiag	= static_cast<unsigned char>(GETBITS(flags3, 21, 24));

		border.icvLeft = 0 != border.dgLeft ? static_cast<unsigned char>(GETBITS(flags2, 16, 22)) : 0;
		border.icvRight = 0 != border.dgRight ? static_cast<unsigned char>(GETBITS(flags2, 23, 29)) : 0;
		border.icvTop = 0 != border.dgTop ? static_cast<unsigned char>(GETBITS(flags3, 0, 6)) : 0;
		border.icvBottom = 0 != border.dgBottom ? static_cast<unsigned char>(GETBITS(flags3, 7, 13)) : 0;
		border.icvDiag = 0 != border.dgDiag ? static_cast<unsigned char>(GETBITS(flags3, 14, 20)) : 0;
		
		border.grbitDiag = static_cast<unsigned char>(GETBITS(flags2, 30, 31));

		fHasXFExt		= GETBIT(flags3, 25);
		fill.fls		= static_cast<unsigned char>(GETBITS(flags3, 26, 31));

		fill.icvFore	= GETBITS(flags4, 0, 6);
		fill.icvBack	= GETBITS(flags4, 7, 13);
		fsxButton		= GETBIT(flags4, 14);
	}
}

void CellXF::Update(ExtProp* ext_prop)
{
	if (!ext_prop) return;

	switch(ext_prop->extType)
	{
		case 0x0004:
		{
			fill.foreFillInfo_.enabled		= true;
			fill.foreFillInfo_.icv			= ext_prop->extPropData.color.icv;
			fill.foreFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
			fill.foreFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
			fill.foreFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;
		}break;
		case 0x0005:
		{
			fill.backFillInfo_.enabled		= true;
			fill.backFillInfo_.icv			= ext_prop->extPropData.color.icv;
			fill.backFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
			fill.backFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
			fill.backFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;
		}break;
		case 0x0007:
		{
			border.topFillInfo_.enabled		= true;
			border.topFillInfo_.icv			= ext_prop->extPropData.color.icv;
			border.topFillInfo_.xclrType	= ext_prop->extPropData.color.xclrType;
			border.topFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
			border.topFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;			
		}break;
		case 0x0008:
		{
			border.bottomFillInfo_.enabled		= true;
			border.bottomFillInfo_.icv			= ext_prop->extPropData.color.icv;
			border.bottomFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
			border.bottomFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
			border.bottomFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;			
		}break;
		case 0x0009:
		{
			border.leftFillInfo_.enabled		= true;
			border.leftFillInfo_.icv			= ext_prop->extPropData.color.icv;
			border.leftFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
			border.leftFillInfo_.nTintShade		= ext_prop->extPropData.color.nTintShade;
			border.leftFillInfo_.xclrValue		= ext_prop->extPropData.color.xclrValue;			
		}break;
		case 0x000A:
		{
			border.rightFillInfo_.enabled		= true;
			border.rightFillInfo_.icv			= ext_prop->extPropData.color.icv;
			border.rightFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
			border.rightFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
			border.rightFillInfo_.xclrValue		= ext_prop->extPropData.color.xclrValue;			
		}break;
		//case 0x000B:	//diag color
		//case 0x000C:	//diag color
		case 0x000D:
		{
			font_color.enabled		= true;
			font_color.icv			= ext_prop->extPropData.color.icv;
			font_color.xclrType		= ext_prop->extPropData.color.xclrType;
			font_color.nTintShade	= ext_prop->extPropData.color.nTintShade;
			font_color.xclrValue	= ext_prop->extPropData.color.xclrValue;			
		}break;
		//case 0x0006:
		//	extPropData.gradient_fill.toXML(own_tag);
		//	break;
		case 0x000E:
		{
			font_id		= ext_prop->extPropData.font_scheme;
		}break;
		case 0x000F:
		{
			cIndent		= ext_prop->extPropData.indent_level;
		}break;
	}
}
void CellXF::Update(XFProps* xfProps)
{
	if (!xfProps) return;
//
//	std::wstringstream strm;
//	xfProps->serialize_fill(strm);
//	fill.ext = strm.str();
}
void CellXF::RegisterFillBorder()
{
    for (size_t i = 0; i < ext_props.size(); i++ )
	{
		ExtProp* ext_prop = dynamic_cast<ExtProp*>(ext_props[i].get());

		switch(ext_prop->extType)
		{
			case 0x0004:
			{
				fill.foreFillInfo_.enabled		= true;
				fill.foreFillInfo_.icv			= ext_prop->extPropData.color.icv;
				fill.foreFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
				fill.foreFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
				fill.foreFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;
			}break;
			case 0x0005:
			{
				fill.backFillInfo_.enabled		= true;
				fill.backFillInfo_.icv			= ext_prop->extPropData.color.icv;
				fill.backFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
				fill.backFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
				fill.backFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;
			}break;
			case 0x0007:
			{
				border.topFillInfo_.enabled		= true;
				border.topFillInfo_.icv			= ext_prop->extPropData.color.icv;
				border.topFillInfo_.xclrType	= ext_prop->extPropData.color.xclrType;
				border.topFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
				border.topFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;			
			}break;
			case 0x0008:
			{
				border.bottomFillInfo_.enabled		= true;
				border.bottomFillInfo_.icv			= ext_prop->extPropData.color.icv;
				border.bottomFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
				border.bottomFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
				border.bottomFillInfo_.xclrValue	= ext_prop->extPropData.color.xclrValue;			
			}break;
			case 0x0009:
			{
				border.leftFillInfo_.enabled		= true;
				border.leftFillInfo_.icv			= ext_prop->extPropData.color.icv;
				border.leftFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
				border.leftFillInfo_.nTintShade		= ext_prop->extPropData.color.nTintShade;
				border.leftFillInfo_.xclrValue		= ext_prop->extPropData.color.xclrValue;			
			}break;
			case 0x000A:
			{
				border.rightFillInfo_.enabled		= true;
				border.rightFillInfo_.icv			= ext_prop->extPropData.color.icv;
				border.rightFillInfo_.xclrType		= ext_prop->extPropData.color.xclrType;
				border.rightFillInfo_.nTintShade	= ext_prop->extPropData.color.nTintShade;
				border.rightFillInfo_.xclrValue		= ext_prop->extPropData.color.xclrValue;			
			}break;
			//case 0x000B:	//diag color
			//case 0x000C:	//diag color
			case 0x000D:
				font_color.enabled		= true;
				font_color.icv			= ext_prop->extPropData.color.icv;
				font_color.xclrType		= ext_prop->extPropData.color.xclrType;
				font_color.nTintShade	= ext_prop->extPropData.color.nTintShade;
				font_color.xclrValue	= ext_prop->extPropData.color.xclrValue;			
				break;
			//case 0x0006:
			//	extPropData.gradient_fill.toXML(own_tag);
			//	break;
			case 0x000E:
				font_id		= ext_prop->extPropData.font_scheme;
				break;
			case 0x000F:
				cIndent		= ext_prop->extPropData.indent_level;
				break;
		}
	}	

	border_x_id	= m_GlobalWorkbookInfo->RegisterBorderId(border);
	fill_x_id	= m_GlobalWorkbookInfo->RegisterFillId(fill);

	m_GlobalWorkbookInfo->RegisterFontColorId(font_id, font_color);
}



} // namespace XLS

