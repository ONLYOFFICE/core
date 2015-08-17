#pragma once

#include "OfficeArtContainer.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtDgContainer : public OfficeArtContainer
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtDgContainer)
public:
	XLS::BiffStructurePtr clone();
	OfficeArtDgContainer(const OfficeArtClientAnchorType anchor_type);

	static const bool CheckIfContainerStartFound(XLS::CFRecord& record);
	static const bool CheckIfContainerSizeOK(XLS::CFRecord& record);

	void loadFields(XLS::CFRecord& record);
	
	OfficeArtRecordPtr					m_OfficeArtFDG;
	OfficeArtRecordPtr					m_OfficeArtFRITContainer;
	OfficeArtRecordPtr					m_OfficeArtSpgrContainer;
	std::vector<OfficeArtRecordPtr>		m_OfficeArtSpContainer; 
	OfficeArtRecordPtr					m_OfficeArtSpgrContainerFileBlock;
};

typedef boost::shared_ptr<OfficeArtDgContainer> OfficeArtDgContainerPtr;


} // namespace ODRAW
