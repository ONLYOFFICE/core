#include "precompiled_xls.h"
#include "OfficeArtBStoreContainer.h"
#include <Binary/CFRecord.h>


namespace ODRAW
{;


OfficeArtBStoreContainer::OfficeArtBStoreContainer()
:	OfficeArtRecord(0, BStoreContainer)
{	
}


XLS::BiffStructurePtr OfficeArtBStoreContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtBStoreContainer(*this));
}

//
//void OfficeArtBStoreContainer::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	std::for_each(rgfb.begin(), rgfb.end(), boost::bind(&OfficeArtBStoreContainerFileBlock::toXML, _1, xml_tag));
//}
//
//
//void OfficeArtBStoreContainer::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}


void OfficeArtBStoreContainer::storeFields(XLS::CFRecord& record)
{	
}


void OfficeArtBStoreContainer::loadFields(XLS::CFRecord& record)
{	
	for (size_t i = 0; i < rh_own.recInstance; i++)
	{
		OfficeArtBStoreContainerFileBlock *fileBlock = new OfficeArtBStoreContainerFileBlock();;		
		record >> *fileBlock;

		if (fileBlock->result && (fileBlock->recType != OfficeArtRecord::BlipDIB))
			rgfb.push_back(fileBlock);
	}	

	// OpenOffice
	while(1)
	{
		OfficeArtRecordHeader rh_child;
		record >> rh_child;	
		record.RollRdPtrBack(rh_child.size());

		if (rh_child.recType == OfficeArtRecord::FBSE)
		{
			OfficeArtBStoreContainerFileBlock *fileBlock = new OfficeArtBStoreContainerFileBlock();;		
			record >> *fileBlock;	

			if (fileBlock->result && (fileBlock->recType != OfficeArtRecord::BlipDIB))
				rgfb.push_back(fileBlock);
		}
		else
			break;
	}
}

const unsigned short OfficeArtBStoreContainer::GetInstanceToStore()
{
	return rgfb.size();
}

} // namespace XLS
