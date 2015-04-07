#pragma once
#include "../../Reader/Records.h"
#include "GroupShapeContainer.h"

class CRecordDrawingContainer : public CRecordsContainer
{
public:
	
	CRecordDrawingContainer()
	{
	}

	~CRecordDrawingContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);

		std::vector<CRecordGroupShapeContainer*> oArrayGroupContainer;
		this->GetRecordsByType(&oArrayGroupContainer, false, false);

		int nCountGroups = oArrayGroupContainer.size();
		for (int i = 0; i < nCountGroups; ++i)
		{
			oArrayGroupContainer[i]->SetGroupRect();
		}
	}
};