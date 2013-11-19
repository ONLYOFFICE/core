#include "stdafx.h"
#include "OfficeArtFSP.h"
#include <XLS_bin/CFRecord.h>

namespace ODRAW
{;


OfficeArtFSP::OfficeArtFSP()
:	OfficeArtRecord(0x02, FSP)
{
}


XLS::BiffStructurePtr OfficeArtFSP::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFSP(*this));
}


void OfficeArtFSP::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"shape_id", shape_id);
	xml_tag->setAttribute(L"spid", spid);

	xml_tag->setAttribute(L"fGroup", fGroup);
	xml_tag->setAttribute(L"fChild", fChild);
	xml_tag->setAttribute(L"fPatriarch", fPatriarch);
	xml_tag->setAttribute(L"fDeleted", fDeleted);
	xml_tag->setAttribute(L"fOleShape", fOleShape);
	xml_tag->setAttribute(L"fHaveMaster", fHaveMaster);
	xml_tag->setAttribute(L"fFlipH", fFlipH);
	xml_tag->setAttribute(L"fFlipV", fFlipV);
	xml_tag->setAttribute(L"fConnector", fConnector);
	xml_tag->setAttribute(L"fHaveAnchor", fHaveAnchor);
	xml_tag->setAttribute(L"fBackground", fBackground);
	xml_tag->setAttribute(L"fHaveSpt", fHaveSpt);
}


void OfficeArtFSP::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	shape_id = getStructAttribute(xml_tag, L"shape_id");
	spid = getStructAttribute(xml_tag, L"spid");

	fGroup = getStructAttribute(xml_tag, L"fGroup");
	fChild = getStructAttribute(xml_tag, L"fChild");
	fPatriarch = getStructAttribute(xml_tag, L"fPatriarch");
	fDeleted = getStructAttribute(xml_tag, L"fDeleted");
	fOleShape = getStructAttribute(xml_tag, L"fOleShape");
	fHaveMaster = getStructAttribute(xml_tag, L"fHaveMaster");
	fFlipH = getStructAttribute(xml_tag, L"fFlipH");
	fFlipV = getStructAttribute(xml_tag, L"fFlipV");
	fConnector = getStructAttribute(xml_tag, L"fConnector");
	fHaveAnchor = getStructAttribute(xml_tag, L"fHaveAnchor");
	fBackground = getStructAttribute(xml_tag, L"fBackground");
	fHaveSpt = getStructAttribute(xml_tag, L"fHaveSpt");
}


void OfficeArtFSP::storeFields(XLS::CFRecord& record)
{
	DWORD flags = 0;
	SETBIT(flags, 0, fGroup);
	SETBIT(flags, 1, fChild);
	SETBIT(flags, 2, fPatriarch);
	SETBIT(flags, 3, fDeleted);
	SETBIT(flags, 4, fOleShape);
	SETBIT(flags, 5, fHaveMaster);
	SETBIT(flags, 6, fFlipH);
	SETBIT(flags, 7, fFlipV);
	SETBIT(flags, 8, fConnector);
	SETBIT(flags, 9, fHaveAnchor);
	SETBIT(flags, 10, fBackground);
	SETBIT(flags, 11, fHaveSpt);

	record << spid << flags;
}


void OfficeArtFSP::loadFields(XLS::CFRecord& record)
{
	DWORD flags;
	record >> spid >> flags;

	fGroup = GETBIT(flags, 0);
	fChild = GETBIT(flags, 1);
	fPatriarch = GETBIT(flags, 2);
	fDeleted = GETBIT(flags, 3);
	fOleShape = GETBIT(flags, 4);
	fHaveMaster = GETBIT(flags, 5);
	fFlipH = GETBIT(flags, 6);
	fFlipV = GETBIT(flags, 7);
	fConnector = GETBIT(flags, 8);
	fHaveAnchor = GETBIT(flags, 9);
	fBackground = GETBIT(flags, 10);
	fHaveSpt = GETBIT(flags, 11);

	shape_id = rh_own.recInstance;
}


const WORD OfficeArtFSP::GetInstanceToStore()
{
	return shape_id;
}


} // namespace XLS
