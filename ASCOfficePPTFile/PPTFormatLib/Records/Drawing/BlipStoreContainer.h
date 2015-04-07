#pragma once
#include "../../Reader/Records.h"
#include "BlipStoreEntry.h"

class CRecordBlipStoreContainer : public CRecordsContainer
{
public:
	
	CRecordBlipStoreContainer()
	{
	}

	~CRecordBlipStoreContainer()
	{
	}

	void SetUpEmptyInfos(std::vector<BOOL>* pArray)
	{
		if (NULL == pArray)
			return;

		pArray->clear();
		for (int nIndex = 0; nIndex < m_arRecords.size(); ++nIndex)
		{
			CRecordBlipStoreEntry* pEntry = dynamic_cast<CRecordBlipStoreEntry*>(m_arRecords[nIndex]);
			if (NULL != pEntry)
			{
				pArray->push_back(0 == pEntry->m_oHeader.RecInstance);
			}
		}
	}
};