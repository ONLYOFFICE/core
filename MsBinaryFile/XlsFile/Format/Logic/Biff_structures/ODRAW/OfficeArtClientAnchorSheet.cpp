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

#include "OfficeArtClientAnchorSheet.h"

#include <boost/lexical_cast.hpp>

namespace ODRAW
{


OfficeArtClientAnchorSheet::OfficeArtClientAnchorSheet() :	OfficeArtRecord(0x00, ClientAnchor)
{
	_x = _y = _cx = _cy = 0;
}


XLS::BiffStructurePtr OfficeArtClientAnchorSheet::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtClientAnchorSheet(*this));
}

void OfficeArtClientAnchorSheet::loadFields(XLS::CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		unsigned char flags;
		record >> flags;
		fMove = GETBIT(flags, 0);
		fSize = GETBIT(flags, 1);
	}
	else
	{
		unsigned short flags;
		record >> flags;
		
		fMove = GETBIT(flags, 0);
		fSize = GETBIT(flags, 1);
	}
	
	record >> colL >> dxL >> rwT >> dyT >> colR >> dxR >> rwB >> dyB;
}

void OfficeArtClientAnchorSheet::calculate()
{
	global_info->GetDigitFontSizePixels();

	XLS::GlobalWorkbookInfo::_sheet_info zero;
	XLS::GlobalWorkbookInfo::_sheet_info & sheet_info = global_info->current_sheet >= 0 ? 
										global_info->sheets_info[global_info->current_sheet - 1] : zero;

//----------------------------------------------------------------------------------------------------
	//1 inch	=	72 point
	//1 emu		=	360000 * 2.54 inch

	//double kfCol	=  1250.;//360000 / 72. / 4.;
	double kfRow	= ( 360000 * 2.54 / 72) / 256. ;

	double Digit_Width	= global_info->defaultDigitFontSize.first;
	double Digit_Height = global_info->defaultDigitFontSize.second;
	double width = 0, column_width = 0;

	if (sheet_info.customColumnsWidth.find(colL) != sheet_info.customColumnsWidth.end())
	{
			column_width = sheet_info.customColumnsWidth[colL];
	}
	else	column_width = sheet_info.defaultColumnWidth;

	width = ((int)((column_width * Digit_Width + 5) / Digit_Width * 256 )) / 256.;
	width = (int)(((256. * width + ((int)(128. / Digit_Width ))) / 256. ) * Digit_Width );	//in pixels
	
	_dxL = dxL / 1024. * width  * 9525. ; //9525 => pixels to emu

	if (sheet_info.customColumnsWidth.find(colR) != sheet_info.customColumnsWidth.end())
	{
			column_width = sheet_info.customColumnsWidth[colR];
	}
	else	column_width = sheet_info.defaultColumnWidth;

	width = ((int)((column_width * Digit_Width + 5) / Digit_Width * 256 )) / 256.;
	width = (int)(((256. * width + ((int)(128. / Digit_Width ))) / 256. ) * Digit_Width ); //in pixels
	
	_dxR = dxR / 1024. * width * 9525.; 
//---------------------------------------------------------------------------------------------------
	if (sheet_info.customRowsHeight.find(rwT) != sheet_info.customRowsHeight.end())
	{
		_dyT = dyT * kfRow * sheet_info.customRowsHeight[rwT];	
	}
	else 
		_dyT = dyT * kfRow * sheet_info.defaultRowHeight;	

	if (sheet_info.customRowsHeight.find(rwB) != sheet_info.customRowsHeight.end())
	{
		_dyB = dyB * kfRow * sheet_info.customRowsHeight[rwB];	
	}
	else 
		_dyB = dyB * kfRow * sheet_info.defaultRowHeight;	
}
void OfficeArtClientAnchorSheet::calculate_1()
{
	global_info->GetDigitFontSizePixels();

	XLS::GlobalWorkbookInfo::_sheet_info zero;
	XLS::GlobalWorkbookInfo::_sheet_info & sheet_info = global_info->current_sheet >= 0 ? 
										global_info->sheets_info[global_info->current_sheet - 1] : zero;
	
	double kfRow	= ( 360000 * 2.54 / 72) / 256. ;
	double Digit_Width	= global_info->defaultDigitFontSize.first;
	double Digit_Height = global_info->defaultDigitFontSize.second;
	double column_width = 0;

	for (int i = 0 ; i < colL; i++)
	{
		if (sheet_info.customColumnsWidth.find(i) != sheet_info.customColumnsWidth.end())
			column_width +=  sheet_info.customColumnsWidth[i];	
		else 
			column_width +=   sheet_info.defaultColumnWidth;
	}
	_x = ((int)((column_width * Digit_Width + 5) / Digit_Width * 256 )) / 256.;
	_x = (int)(((256. * _x + ((int)(128. / Digit_Width ))) / 256. ) * Digit_Width ); //in pixels

	_x = _x  * 9525. + _dxL;

	column_width = 0;
	for (int i = 0; i < colR; i++)
	{
		if (sheet_info.customColumnsWidth.find(i) != sheet_info.customColumnsWidth.end())
			column_width += sheet_info.customColumnsWidth[i];	
		else 
			column_width += sheet_info.defaultColumnWidth;
	}
	_cx = ((int)((column_width * Digit_Width + 5) / Digit_Width * 256 )) / 256.;
	_cx = (int)(((256. * _cx + ((int)(128. / Digit_Width ))) / 256. ) * Digit_Width ); //in pixels
	
	_cx += _cx * 9525. + _dxR - _x;

	for (int i = 0 ; i < rwT; i++)
	{
		if (sheet_info.customRowsHeight.find(i) != sheet_info.customRowsHeight.end())
		{
			_y += 256 * kfRow * sheet_info.customRowsHeight[i];	
		}
		else 
			_y += 256 * kfRow * sheet_info.defaultRowHeight;	
	}
	_y += _dyT;

	for (int i = 0 ; i < rwB; i++)
	{
		if (sheet_info.customRowsHeight.find(i) != sheet_info.customRowsHeight.end())
		{
			_cy += 256 * kfRow * sheet_info.customRowsHeight[i];	
		}
		else 
			_cy += 256 * kfRow * sheet_info.defaultRowHeight;	
	}
	_cy += _dyT - _y;


}



//-------------------------------------------------------------------------------------


OfficeArtChildAnchor::OfficeArtChildAnchor() :	OfficeArtRecord(0x00, ChildAnchor)
{
	_x = _y = _cx = _cy = 0;
}


XLS::BiffStructurePtr OfficeArtChildAnchor::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtChildAnchor(*this));
}

void OfficeArtChildAnchor::loadFields(XLS::CFRecord& record)
{
	XLS::GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	
	record >> dxL >> dyT >> dxR >>  dyB;
	
//----------------------------------------------------------------------------------------------------
	_x	= (std::min)(dxL, dxR);
	_cx = (std::max)(dxL, dxR) - _x;
	
	_y	= (std::min)(dyT, dyB) ;	
	_cy = (std::max)(dyT, dyB) - _y;	


}

} // namespace XLS


