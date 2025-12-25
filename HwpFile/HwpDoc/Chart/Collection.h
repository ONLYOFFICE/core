#ifndef COLLECTION_H
#define COLLECTION_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
template<typename T>
class CCollection : public IChartObject
{
	std::vector<const T*> m_arItems;
public:
	CCollection() = default;
	~CCollection()
	{
		for (const T* pItem : m_arItems)
			delete pItem;
	}

	bool AddItem(const T* pItem)
	{
		if(nullptr == pItem)
			return false;

		m_arItems.push_back(pItem);
		return true;
	}

	virtual bool Read(CHWPStream& oStream)
	{
		long lCount;

		if (!oStream.ReadLong(lCount))
			return false;

		for (long lIndex = 0; lIndex < lCount; ++lIndex)
		{
			T* pItem = new T();

			if (nullptr == pItem)
				continue;

			if (!pItem->Read(oStream) || !AddItem(pItem))
			{
				delete pItem;
				return false;
			}

			return true;
		}
	}
};
}}
#endif // COLLECTION_H
