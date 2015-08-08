
#include "SimpleOfficeArtContainers.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;

void OfficeArtFConnectorRule::loadFields(XLS::CFRecord& record)
{
	record >> ruid >> spidA >> spidB >> spidC >> cptiA >> cptiB;
}

void OfficeArtDggContainer::loadFields(XLS::CFRecord& record)
{
	OfficeArtContainer::loadFields(record);
	
	for (long i = 0 ; i < child_records.size(); i++)
	{
		switch(child_records[i]->rh_own.recType)
		{
		case ODRAW::OfficeArtRecord::BStoreContainer:
			{
				m_OfficeArtBStoreContainer = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--; 
			}break;
		case ODRAW::OfficeArtRecord::ColorMRUContainer:
			{
				m_OfficeArtColorMRUContainer = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--; 
			}break;
		case ODRAW::OfficeArtRecord::SplitMenuColorContainer:
			{
				m_OfficeArtSplitMenuColorContainer = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--; 
			}break;
		case ODRAW::OfficeArtRecord::FDGGBlock:
			{
				m_OfficeArtFDGGBlock = child_records[i];
				child_records.erase(child_records.begin() + i,child_records.begin() + i + 1); i--; 
			}break;			
		//case ODRAW::OfficeArtRecord::SpgrContainerFileBlock:
		//	{
		//		m_OfficeArtSpgrContainerFileBlock = OfficeArtContainerPtr(art_container);
		//		child_records.erase(child_records.begin() + i,child_records.begin() + i + 1);
		//	}break;		

		}
	}
}

void OfficeArtSpgrContainer::loadFields(XLS::CFRecord& record)
{
	OfficeArtContainer::loadFields(record);

	//for (long i = 0 ; i < child_records.size(); i++)
	//{
	//	switch(child_records[i]->rh_own.recType)
	//	{
	//	case ODRAW::OfficeArtRecord::SpgrContainerFileBlock:
	//		{
	//			m_OfficeArtSpgrContainerFileBlock.push_back(child_records[i]);
	//			child_records.erase(child_records.begin() + i,child_records.begin() + i + 1);
	//		}break;
	//	}
	//}

}

} // namespace ODRAW
