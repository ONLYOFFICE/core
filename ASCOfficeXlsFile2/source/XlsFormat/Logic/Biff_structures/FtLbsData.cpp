#include "precompiled_xls.h"
#include "FtLbsData.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


FtLbsData::FtLbsData()
:	fmla(false)
{
}

BiffStructurePtr FtLbsData::clone()
{
	return BiffStructurePtr(new FtLbsData(*this));
}


//void FtLbsData::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	fmla.toXML(own_tag);
//
//	own_tag->setAttribute(L"cLines", cLines);
//	own_tag->setAttribute(L"iSel", iSel);
//
//	own_tag->setAttribute(L"fUseCB", fUseCB);
//	own_tag->setAttribute(L"fValidPlex", fValidPlex);
//	own_tag->setAttribute(L"fValidIds", fValidIds);
//	own_tag->setAttribute(L"fNo3d", fNo3d);
//	own_tag->setAttribute(L"wListSelType", wListSelType);
//	own_tag->setAttribute(L"lct", lct);
//
//	own_tag->setAttribute(L"idEdit", idEdit);
//
//	if(dropData)
//	{
//		dropData->toXML(own_tag);
//	}
//
//	if(fValidPlex)
//	{
//		for (std::vector<XLUnicodeString>::iterator it = rgLines.begin(), itEnd = rgLines.end(); it != itEnd; ++it)
//		{
//			MSXML2::IXMLDOMElementPtr str_tag = XMLSTUFF::createElement(L"rgLine", own_tag);
//			it->toXML(str_tag, L"str");
//		}
//	}
//
//	if(0 != wListSelType)
//	{
//		for (std::vector<Boolean<unsigned char>>::iterator it = bsels.begin(), itEnd = bsels.end(); it != itEnd; ++it)
//		{
//			MSXML2::IXMLDOMElementPtr bsel_tag = XMLSTUFF::createElement(L"bsel", own_tag);
//			bsel_tag->setAttribute(L"val", *it);
//		}
//	}
//
//}
//
//
//void FtLbsData::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	fmla.fromXML(own_tag);
//
//	cLines = getStructAttribute(own_tag, L"cLines");
//	iSel = getStructAttribute(own_tag, L"iSel");
//	
//	fUseCB = getStructAttribute(own_tag, L"fUseCB");
//	fValidPlex = getStructAttribute(own_tag, L"fValidPlex");
//	fValidIds = getStructAttribute(own_tag, L"fValidIds");
//	fNo3d = getStructAttribute(own_tag, L"fNo3d");
//	wListSelType = getStructAttribute(own_tag, L"wListSelType");
//	lct = getStructAttribute(own_tag, L"lct");
//
//	idEdit = getStructAttribute(own_tag, L"idEdit");
//
//	LbsDropData data;
//	if(data.fromXML(own_tag))
//	{
//		dropData.reset(new LbsDropData(data));
//	}
//
//	if(fValidPlex)
//	{
//		MSXML2::IXMLDOMNodeListPtr nodes = own_tag->selectNodes(L"rgLine");
//		MSXML2::IXMLDOMElementPtr str_tag;
//		for(size_t counter = 1; (str_tag = nodes->nextNode()); ++counter)
//		{
//			XLUnicodeString str;
//			str.fromXML(str_tag, L"str");
//			rgLines.push_back(str);
//		}
//	}
//	if(0 != wListSelType)
//	{
//		MSXML2::IXMLDOMNodeListPtr nodes = own_tag->selectNodes(L"bsel");
//		MSXML2::IXMLDOMElementPtr str_tag;
//		for(size_t counter = 1; (str_tag = nodes->nextNode()); ++counter)
//		{
//			Boolean<unsigned char> bsel = static_cast<bool>(getStructAttribute(own_tag, L"val"));
//			bsels.push_back(bsel);
//		}
//	}
//
//}
//
//
void FtLbsData::store(CFRecord& record, const unsigned short ot)
{
	unsigned short ft = 0x0013; // reserved
	record << ft;
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cbFmla*/);
	size_t start_ptr = record.getDataSize();

	fmla.store(record);

	unsigned short flags = 0;
	SETBIT(flags, 0, fUseCB);
	SETBIT(flags, 1, fValidPlex);
	SETBIT(flags, 2, fValidIds);
	SETBIT(flags, 3, fNo3d);
	SETBITS(flags, 4, 5, wListSelType);
	SETBITS(flags, 8, 15, lct);

	record << cLines << iSel << flags << idEdit;
	if(dropData)
	{
		dropData->store(record);
	}

	if(fValidPlex)
	{
		record << rgLines;
	}

	if (0 != wListSelType)
	{
		record << bsels;
	}

	size_t data_size = record.getDataSize() - start_ptr;
	record.registerDelayedDataSource(data_size, rt_Obj);
}


void FtLbsData::load(CFRecord& record, const unsigned short ot)
{
	record.skipNunBytes(4); // reserved

	fmla.load(record);

	unsigned short flags;
	record >> cLines >> iSel >> flags >> idEdit;

	fUseCB = GETBIT(flags, 0);
	fValidPlex = GETBIT(flags, 1);
	fValidIds = GETBIT(flags, 2);
	fNo3d = GETBIT(flags, 3);
	wListSelType = GETBITS(flags, 4, 5);
	lct = GETBITS(flags, 8, 15);

	if(0x0014 == ot)
	{
		dropData.reset(new LbsDropData);
		dropData->load(record);
	}
	if(fValidPlex)
	{
		for(int i = 0; i < cLines; ++i)
		{
			XLUnicodeString str;
			record >> str;
			rgLines.push_back(str);
		}
	}

	if (0 != wListSelType)
	{
		for(int i = 0; i < cLines; ++i)
		{
			Boolean<unsigned char> bsel;
			record >> bsel;
			bsels.push_back(bsel);
		}
	}




}


} // namespace XLS

