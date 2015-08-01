
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
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(dxL * 256);
			}
			CP_XML_NODE(L"xdr:row")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(rwT); 
			}
			CP_XML_NODE(L"xdr:rowOff")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(dyT * 256);
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
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(dxR * 256);
			}
			CP_XML_NODE(L"xdr:row")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(rwB);
			}
			CP_XML_NODE(L"xdr:rowOff")
			{
				CP_XML_STREAM() << boost::lexical_cast<std::wstring>(dyB * 256); 
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
	unsigned short flags;
	record >> flags >> colL >> dxL >> rwT >> dyT >> colR >> dxR >> rwB >> dyB;
	fMove = GETBIT(flags, 0);
	fSize = GETBIT(flags, 1);

	_dxL = dxL * 595;
	_dyT = dyT * 744;
	_dxR = dxR * 595;
	_dyB = dyB * 744;
}


} // namespace XLS
