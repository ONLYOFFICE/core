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

    void SetUpPicturesInfos(std::vector<int>* pArray)
	{
		if (NULL == pArray)
			return;

		pArray->clear();

		std::map<DWORD, int> image_map;
		for (int nIndex = 0; nIndex < m_arRecords.size(); ++nIndex)
		{
			CRecordBlipStoreEntry* pEntry = dynamic_cast<CRecordBlipStoreEntry*>(m_arRecords[nIndex]);
			if (NULL != pEntry)
			{
				image_map.insert(std::pair<DWORD, int>(pEntry->m_nFoDelay, pArray->size()));

				int offset = pEntry->m_nFoDelay;
				if (0 == pEntry->m_oHeader.RecInstance)	offset = -1;

				if (pEntry->m_oHeader.RecType == 0xf007)
				{
					//inside here?
				}

				pArray->push_back(offset);
			}
		}
	}
};
