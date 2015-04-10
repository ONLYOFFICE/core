#ifndef _WMF_MEMORY_H
#define _WMF_MEMORY_H

#include <stdio.h>


#ifdef _DEBUG
//#define DebugCheckMemory
//#include <vector>
//#include <iterator>
#endif

class CWmfMemoryManager
{

public:

	CWmfMemoryManager()
	{
#ifdef DebugCheckMemory
		m_arrDescription.clear();
		m_arrMemoryList.clear();
#endif

	}
	~CWmfMemoryManager()
	{
#ifdef DebugCheckMemory
		m_arrDescription.clear();
		m_arrMemoryList.clear();
#endif
	}

	void *Malloc(size_t unSize, std::wstring sDescription)
	{
		void *pAddress = malloc(unSize);
		if (NULL == pAddress)
			return NULL;

#ifdef DebugCheckMemory
		Add(pAddress, sDescription);
#endif

		return pAddress;
	}
	void *Realloc(void *pMemory, size_t unSize)
	{
#ifdef DebugCheckMemory
		std::wstring sDescription = Remove(pMemory);
#endif

		void *pAddress = realloc(pMemory, unSize);
		if (NULL == pAddress)
			return NULL;

#ifdef DebugCheckMemory
		Add(pAddress, sDescription);
#endif

		return pAddress;
	}
	void Free(void *pMemory)
	{
		if (pMemory)
		{
			free(pMemory);
		}

#ifdef DebugCheckMemory
		Remove(pMemory);
#endif
	}

private:

#ifdef DebugCheckMemory
	void Add(void *pAddress, std::wstring sDescription)
	{
		m_arrDescription.push_back(sDescription);
		m_arrMemoryList.push_back(pAddress);
	}
	std::wstring Remove(void *pAddress)
	{
		int nIndex = 0;
		for (nIndex = 0; nIndex < m_arrMemoryList.size(); nIndex++)
		{
			if (pAddress == m_arrMemoryList.at(nIndex))
				break;
		}

		if (nIndex >= m_arrMemoryList.size())
			return L"";
		
		m_arrMemoryList.erase(m_arrMemoryList.begin() + nIndex);
		std::wstring sResult = m_arrDescription.at(nIndex);
		m_arrDescription.erase(m_arrDescription.begin() + nIndex);
		return sResult;
	}

#endif

private:

#ifdef DebugCheckMemory
	std::vector<void *>       m_arrMemoryList;
	std::vector<std::wstring> m_arrDescription;
#endif
};

#endif /* _WMF_MEMORY_H */