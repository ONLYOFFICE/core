#pragma once
#include "..\..\Reader\Records.h"
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

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);

		CSimpleArray<CRecordGroupShapeContainer*> oArrayGroupContainer;
		this->GetRecordsByType(&oArrayGroupContainer, false, false);

		int nCountGroups = oArrayGroupContainer.GetSize();
		for (int i = 0; i < nCountGroups; ++i)
		{
			oArrayGroupContainer[i]->SetGroupRect();
		}
	}
};