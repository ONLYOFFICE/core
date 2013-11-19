#include "stdafx.h"
#include "HyperlinkMoniker.h"
#include <XLS_bin/CFRecord.h>
#include <XLS_logic/Biff_structures/URLMoniker.h>
#include <XLS_logic/Biff_structures/FileMoniker.h>
#include <XLS_logic/Biff_structures/CompositeMoniker.h>
#include <XLS_logic/Biff_structures/AntiMoniker.h>
#include <XLS_logic/Biff_structures/ItemMoniker.h>
#include <Exception/WrongBiffRecord.h>

namespace OSHARED
{;

static const CLSID URLMoniker_CLSID =		{0x79EAC9E0, 0xBAF9, 0x11CE, {0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B}};
static const CLSID FileMoniker_CLSID =		{0x00000303, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const CLSID CompositeMoniker_CLSID =	{0x00000309, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const CLSID AntiMoniker_CLSID =		{0x00000305, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const CLSID ItemMoniker_CLSID =		{0x00000304, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};


XLS::BiffStructurePtr HyperlinkMoniker::clone()
{
	return XLS::BiffStructurePtr(new HyperlinkMoniker(*this));
}


void HyperlinkMoniker::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	data->toXML(parent);
}


const bool HyperlinkMoniker::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
	data.reset(new URLMoniker);
	data->fromXML(parent, 1, true);
	return true;
}


void HyperlinkMoniker::store(XLS::CFRecord& record)
{
	CLSID clsid = URLMoniker_CLSID;
	record << clsid;
	record << *data;
}


void HyperlinkMoniker::load(XLS::CFRecord& record)
{
	CLSID clsid;
	record >> clsid;
	monikerClsid = STR::guid2bstr(clsid);

	if(URLMoniker_CLSID == clsid)
	{
		data.reset(new URLMoniker);
	}

	else if (FileMoniker_CLSID == clsid)
	{
		data.reset(new FileMoniker);
	} 
	else if (CompositeMoniker_CLSID == clsid)
	{
		data.reset(new CompositeMoniker);
	} 
	else if (AntiMoniker_CLSID == clsid)
	{
		data.reset(new AntiMoniker);
	} 
	else if (ItemMoniker_CLSID == clsid)
	{
		data.reset(new ItemMoniker);
	}
	else
	{
		throw EXCEPT::RT::WrongBiffRecord("Unsupported type of HyperlinkMoniker.", record.getTypeString());
	}

	record >> *data;
}


} // namespace OSHARED

