
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

int OfficeArtClientAnchorSheet::serialize(std::wostream &stream)
{
//-------------------------------------------------
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"xdr:from")
		{ 
			CP_XML_NODE(L"xdr:col")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(colL);
			}
			CP_XML_NODE(L"xdr:colOff")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(_dxL );
			}
			CP_XML_NODE(L"xdr:row")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(rwT); 
			}
			CP_XML_NODE(L"xdr:rowOff")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(_dyT);
			}
		}
		CP_XML_NODE(L"xdr:to")
		{  		
			CP_XML_NODE(L"xdr:col")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(colR); 
			}
			CP_XML_NODE(L"xdr:colOff")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(_dxR );
			}
			CP_XML_NODE(L"xdr:row")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(rwB);
			}
			CP_XML_NODE(L"xdr:rowOff")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(_dyB); 
			}
		}
	}
	return 0;
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
	XLS::GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	
	unsigned short flags;
	record >> flags >> colL >> dxL >> rwT >> dyT >> colR >> dxR >> rwB >> dyB;
	
	fMove = GETBIT(flags, 0);
	fSize = GETBIT(flags, 1);
//----------------------------------------------------------------------------------------------------
	double kfCol	= 17640 / 256.;
	double kfRow	= ( 360000 * 2.54 / 72) / 256. ;

	if (global_info->customColumnsWidth.find(colL) != global_info->customColumnsWidth.end())
	{
		_dxL = dxL * kfCol * global_info->customColumnsWidth[colL];	
	}
	else 
		_dxL = dxL * kfCol * global_info->defaultColumnWidth;	

	if (global_info->customColumnsWidth.find(colR) != global_info->customColumnsWidth.end())
	{
		_dxR = dxR * kfCol * global_info->customColumnsWidth[colR];	
	}
	else 
		_dxR = dxR * kfCol * global_info->defaultColumnWidth;	

//---------------------------------------------------------------------------------------------------
	if (global_info->customRowsHeight.find(rwT) != global_info->customRowsHeight.end())
	{
		_dyT = dyT * kfRow * global_info->customRowsHeight[rwT];	
	}
	else 
		_dyT = dyT * kfRow * global_info->defaultRowHeight;	

	if (global_info->customRowsHeight.find(rwB) != global_info->customRowsHeight.end())
	{
		_dyB = dyB * kfRow * global_info->customRowsHeight[rwB];	
	}
	else 
		_dyB = dyB * kfRow * global_info->defaultRowHeight;	
//----------------------------------------------------------------------------------------------------
	column	= (std::min)(colL, colR);
	row		= (std::min)(rwT, rwB);

//----------------------------------------------------------------------------------------------------
	for (int i = 1 ; i < colL; i++)
	{
		if (global_info->customColumnsWidth.find(i) != global_info->customColumnsWidth.end())
			_x +=  256 * kfCol * global_info->customColumnsWidth[i];	
		else 
			_x +=  256 * kfCol * global_info->defaultColumnWidth;
	}
	_x += _dxL;

	for (int i = colL ; i < colR; i++)
	{
		if (global_info->customColumnsWidth.find(i) != global_info->customColumnsWidth.end())
			_cx += 256 * kfCol * global_info->customColumnsWidth[i];	
		else 
			_cx += 256 * kfCol * global_info->defaultColumnWidth;
	}
	_cx += _dxR;

	for (int i = 1 ; i < rwT; i++)
	{
		if (global_info->customRowsHeight.find(i) != global_info->customRowsHeight.end())
		{
			_y += 256 * kfRow * global_info->customRowsHeight[i];	
		}
		else 
			_y += 256 * kfRow * global_info->defaultRowHeight;	
	}
	_y += _dyT;

	for (int i = rwT ; i < rwB; i++)
	{
		if (global_info->customRowsHeight.find(i) != global_info->customRowsHeight.end())
		{
			_cy += 256 * kfRow * global_info->customRowsHeight[i];	
		}
		else 
			_cy += 256 * kfRow * global_info->defaultRowHeight;	
	}
	_cy += _dyT;
}



//-------------------------------------------------------------------------------------


OfficeArtChildAnchor::OfficeArtChildAnchor()
:	OfficeArtRecord(0x00, ChildAnchor)
{
	_x = _y = _cx = _cy = 0;
}


XLS::BiffStructurePtr OfficeArtChildAnchor::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtChildAnchor(*this));
}

int OfficeArtChildAnchor::serialize(std::wostream &stream)
{
	CP_XML_WRITER(stream)    
	{
		CP_XML_NODE(L"a:xfrm")
		{ 
			CP_XML_NODE(L"a:off")
			{
				CP_XML_ATTR(L"x", _x);
				CP_XML_ATTR(L"y", _y);
			}
			CP_XML_NODE(L"xdr:colOff")
			{
				CP_XML_ATTR(L"cx", _cx);
				CP_XML_ATTR(L"cy", _cy);
			}
		}
	}
	return 0;
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


