
#include "OfficeArtClientAnchorSheet.h"
#include <Binary/CFRecord.h>

#include <boost/lexical_cast.hpp>
#include <simple_xml_writer.h>

namespace ODRAW
{;


OfficeArtClientAnchorSheet::OfficeArtClientAnchorSheet()
:	OfficeArtRecord(0x00, ClientAnchor)
{

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
}


} // namespace XLS
