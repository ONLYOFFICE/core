
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
	
	unsigned short flags;
	record >> flags >> colL >> dxL >> rwT >> dyT >> colR >> dxR >> rwB >> dyB;
	
	fMove = GETBIT(flags, 0);
	fSize = GETBIT(flags, 1);
}

void OfficeArtClientAnchorSheet::calculate()
{
	XLS::GlobalWorkbookInfo::_sheet_size_info zero;
	XLS::GlobalWorkbookInfo::_sheet_size_info & sheet_info = global_info->current_sheet >=0 ? 
										global_info->sheet_size_info[global_info->current_sheet - 1] : zero;

//----------------------------------------------------------------------------------------------------
	double kfCol	= 17640 / 256.;
	double kfRow	= ( 360000 * 2.54 / 72) / 256. ;

	if (sheet_info.customColumnsWidth.find(colL) != sheet_info.customColumnsWidth.end())
	{
		_dxL = dxL * kfCol * sheet_info.customColumnsWidth[colL];	
	}
	else 
		_dxL = dxL * kfCol * sheet_info.defaultColumnWidth;	

	if (sheet_info.customColumnsWidth.find(colR) != sheet_info.customColumnsWidth.end())
	{
		_dxR = dxR * kfCol * sheet_info.customColumnsWidth[colR];	
	}
	else 
		_dxR = dxR * kfCol * sheet_info.defaultColumnWidth;	

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


