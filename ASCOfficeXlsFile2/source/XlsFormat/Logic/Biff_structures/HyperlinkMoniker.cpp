

#include "HyperlinkMoniker.h"
#include <Binary/CFRecord.h>

#include <Logic/Biff_structures/URLMoniker.h>
#include <Logic/Biff_structures/FileMoniker.h>
#include <Logic/Biff_structures/CompositeMoniker.h>
#include <Logic/Biff_structures/AntiMoniker.h>
#include <Logic/Biff_structures/ItemMoniker.h>
//#include <Exception/WrongBiffRecord.h>

bool operator ==(const _GUID_ & rguid1,const _GUID_ &rguid2)
{
   return (
      ((_UINT32 *) &rguid1)[0] == ((_UINT32 *) &rguid2)[0] &&
      ((_UINT32 *) &rguid1)[1] == ((_UINT32 *) &rguid2)[1] &&
      ((_UINT32 *) &rguid1)[2] == ((_UINT32 *) &rguid2)[2] &&
      ((_UINT32 *) &rguid1)[3] == ((_UINT32 *) &rguid2)[3]);

}

namespace OSHARED
{;

static const _GUID_ URLMoniker_CLSID		=	{0x79EAC9E0, 0xBAF9, 0x11CE, {0x8C, 0x82, 0x00, 0xAA, 0x00, 0x4B, 0xA9, 0x0B}};
static const _GUID_ FileMoniker_CLSID		=	{0x00000303, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const _GUID_ CompositeMoniker_CLSID	=	{0x00000309, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const _GUID_ AntiMoniker_CLSID		=	{0x00000305, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
static const _GUID_ ItemMoniker_CLSID		=	{0x00000304, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};


XLS::BiffStructurePtr HyperlinkMoniker::clone()
{
	return XLS::BiffStructurePtr(new HyperlinkMoniker(*this));
}


//void HyperlinkMoniker::toXML(BiffStructurePtr & parent)
//{
//	data->toXML(parent);
//}


//const bool HyperlinkMoniker::fromXML(BiffStructurePtr & parent)
//{
//	data.reset(new URLMoniker);
//	data->fromXML(parent, 1, true);
//	return true;
//}


void HyperlinkMoniker::store(XLS::CFRecord& record)
{
	_GUID_ clsid = URLMoniker_CLSID;
	record << clsid;
	record << *data;
}


void HyperlinkMoniker::load(XLS::CFRecord& record)
{
	_GUID_ clsid;
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
		throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of HyperlinkMoniker.", record.getTypeString());
	}

	record >> *data;
}


} // namespace OSHARED

