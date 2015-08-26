
#include "OfficeArtDgContainer.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{

OfficeArtDgContainer::OfficeArtDgContainer(const OfficeArtClientAnchorType anchor_type)
:	OfficeArtContainer(0x0F, DgContainer, anchor_type)
{
}


XLS::BiffStructurePtr OfficeArtDgContainer::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtDgContainer(*this));
}


const bool OfficeArtDgContainer::CheckIfContainerStartFound(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh;
	if(!record.checkFitReadSafe(rh.size())) return false;
	record >> rh;

	return 0xF002 == rh.recType;
}


const bool OfficeArtDgContainer::CheckIfContainerSizeOK(XLS::CFRecord& record)
{
	OfficeArtRecordHeader rh;
	if(!record.checkFitReadSafe(rh.size())) return false;
	record >> rh;

	bool res = record.getDataSize() == rh.recLen + rh.size();

	return 0xF002 == rh.recType && res;
}

void OfficeArtDgContainer::loadFields(XLS::CFRecord& record)
{
	try
	{
		OfficeArtContainer::loadFields(record);
	}catch(...)
	{
	}
	
    for (int i = 0 ; i < child_records.size(); i++)
	{
		switch(child_records[i]->rh_own.recType)
		{
		case ODRAW::OfficeArtRecord::DgContainer:
			{
				OfficeArtDgContainer * dg = dynamic_cast<OfficeArtDgContainer *>(child_records[i].get());
				if (dg)
				{
					for (int i = 0 ; i < dg->child_records.size(); i++)
					{
						child_records.push_back(dg->child_records[i]);
					}
					dg->child_records.clear();

					if (dg->m_OfficeArtSpgrContainer)	m_OfficeArtSpgrContainer	= dg->m_OfficeArtSpgrContainer;
					if (dg->m_OfficeArtFDG)				m_OfficeArtFDG				= dg->m_OfficeArtFDG;
					if (dg->m_OfficeArtSpContainer.size() > 0)	
						m_OfficeArtSpContainer.insert(m_OfficeArtSpContainer.end(), dg->m_OfficeArtSpContainer.begin(), dg->m_OfficeArtSpContainer.end());

				}
				child_records.erase(child_records.begin() + i, child_records.begin() + i + 1); i--;
			}break;
		case ODRAW::OfficeArtRecord::FDG:
			{
				m_OfficeArtFDG = child_records[i];
				child_records.erase(child_records.begin() + i, child_records.begin() + i + 1); i--;
			}break;
		case ODRAW::OfficeArtRecord::FRITContainer:
			{
				m_OfficeArtFRITContainer = child_records[i];
				child_records.erase(child_records.begin() + i, child_records.begin() + i + 1); i--;
			}break;
		case ODRAW::OfficeArtRecord::SpgrContainer:
			{
				m_OfficeArtSpgrContainer = child_records[i];
				child_records.erase(child_records.begin() + i, child_records.begin() + i + 1); i--;
			}break;
		case ODRAW::OfficeArtRecord::SpContainer:
			{
				m_OfficeArtSpContainer.push_back(child_records[i]);
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

} // namespace ODRAW
