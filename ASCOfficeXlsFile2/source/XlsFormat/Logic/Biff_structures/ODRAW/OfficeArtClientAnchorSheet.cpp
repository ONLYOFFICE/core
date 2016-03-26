
#include "OfficeArtClientAnchorSheet.h"
#include <Binary/CFRecord.h>

#include <boost/lexical_cast.hpp>
#include <simple_xml_writer.h>

namespace ODRAW
{


OfficeArtClientAnchorSheet::OfficeArtClientAnchorSheet()
:	OfficeArtRecord(0x00, ClientAnchor)
{
	_x = _y = _cx = _cy = 0;
}


XLS::BiffStructurePtr OfficeArtClientAnchorSheet::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtClientAnchorSheet(*this));
}



void OfficeArtClientAnchorSheet::storeFields(XLS::CFRecord& record)
{
	unsigned short flags = 0;
	
	SETBIT(flags, 0, fMove);
	SETBIT(flags, 1, fSize);
	
	record << flags << colL << dxL << rwT << dyT << colR << dxR << rwB << dyB;
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
	XLS::GlobalWorkbookInfo::_sheet_size_info zero;
	XLS::GlobalWorkbookInfo::_sheet_size_info & sheet_info = global_info->current_sheet >=0 ? 
										global_info->sheet_size_info[global_info->current_sheet - 1] : zero;

//----------------------------------------------------------------------------------------------------
	//1 inch	=	72 point
	//1 emu		=	360000 * 2.54 inch

	double kfCol	=  360000 / 72.;

	double kfRow	= ( 360000 * 2.54 / 72) / 256. ;

	double Digit_Width = 8.43;
	double column_width = 0;

	if (sheet_info.customColumnsWidth.find(colL) != sheet_info.customColumnsWidth.end())
	{
		column_width = sheet_info.customColumnsWidth[colL] / 1024.;	
	}
	else 
		column_width = sheet_info.defaultColumnWidth / 1024.;	

	//double width = ((int)((column_width * Maximum_Digit_Width + 5 ) / Maximum_Digit_Width * 256. )) / 256.; //px
	double width = (double)(((256. * column_width + (int)(128. / Digit_Width)) / 256.) * Digit_Width) * 72 / 96.;
	
	_dxL = dxL * kfCol * width;

	if (sheet_info.customColumnsWidth.find(colR) != sheet_info.customColumnsWidth.end())
	{
		column_width = sheet_info.customColumnsWidth[colR];		
	}
	else 
		column_width = sheet_info.defaultColumnWidth;

	width = (double)(((256. * column_width + (int)(128. / Digit_Width)) / 256.) * Digit_Width) * 72 / 96.;
	
	_dxR = dxR * kfCol * width;
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

//----------------------------------------------------------------------------------------------------
	for (int i = 0 ; i < colL; i++)
	{
		if (sheet_info.customColumnsWidth.find(i) != sheet_info.customColumnsWidth.end())
			_x +=  256 * kfCol * sheet_info.customColumnsWidth[i];	
		else 
			_x +=  256 * kfCol * sheet_info.defaultColumnWidth;
	}
	_x += _dxL;

	for (int i = colL ; i < colR; i++)
	{
		if (sheet_info.customColumnsWidth.find(i) != sheet_info.customColumnsWidth.end())
			_cx += 256 * kfCol * sheet_info.customColumnsWidth[i];	
		else 
			_cx += 256 * kfCol * sheet_info.defaultColumnWidth;
	}
	_cx += _dxR;

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

	for (int i = rwT ; i < rwB; i++)
	{
		if (sheet_info.customRowsHeight.find(i) != sheet_info.customRowsHeight.end())
		{
			_cy += 256 * kfRow * sheet_info.customRowsHeight[i];	
		}
		else 
			_cy += 256 * kfRow * sheet_info.defaultRowHeight;	
	}
	_cy += _dyT;
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


