#pragma once
#include "..\..\Reader\Records.h"
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

	void SetUpEmptyInfos(CAtlArray<BOOL>* pArray)
	{
		if (NULL == pArray)
			return;

		pArray->RemoveAll();
		for (int nIndex = 0; nIndex < m_arRecords.GetSize(); ++nIndex)
		{
			CRecordBlipStoreEntry* pEntry = dynamic_cast<CRecordBlipStoreEntry*>(m_arRecords[nIndex]);
			if (NULL != pEntry)
			{
				pArray->Add(0 == pEntry->m_oHeader.RecInstance);
			}
		}
	}
};