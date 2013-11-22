#pragma once

#include <atlcoll.h>

namespace ArrayUtils
{
	// Construct elements
	template<class T> void ConstructElements(T* pData, int nSize)
	{
		ATLASSERT(!nSize || nSize > 0 && pData);

		// first do bit-wise zero initialization
		if (nSize)
			ZeroMemory(pData, nSize * sizeof(T));

		// then call the constructor(s)
		for (; nSize--; pData++)
			::new((void*)pData) T;
	}

	// Destruct elements
	template<class T> void DestructElements(T* pData, int nSize)
	{
		ATLASSERT(!nSize || nSize > 0 && pData);

		// call the destructor(s)
		for (; nSize--; pData++)
			pData->~T();
	}

	// Swap elements
	template<class T> void SwapElements(T& tL, T& tR)
	{
		T tTemp = tL; tL = tR; tR = tTemp;
	}

	// Sort elements in ascending order
	template<class T> void SortElements(T* pData, int nSize)
	{
		ATLASSERT(!nSize || nSize > 0 && pData);

		// handle 0, 1 and 2 elements
		if (nSize <= 1)
			return;
		if (nSize == 2)
		{
			if (pData[0] > pData[1])
				SwapElements<T>(pData[0], pData[1]);
			return;
		}

		T tTemp;

		// arrange elements as tree with greater elements appearing first
		int nIndex = (nSize >> 1) - 1, nCurr = 0, nNext = 0;
		int nLast = nSize - 1;
		int nHalf = nSize >> 1;
		do
		{
			// save element at start of chain
			tTemp = pData[nIndex];

			nCurr = nIndex;
			while (nCurr < nHalf)
			{
				nNext = (nCurr << 1) + 1;
				if (nNext < nLast && pData[nNext + 1] > pData[nNext])
					nNext++;
				if (tTemp >= pData[nNext])
					break;

				// promote element in chain
				pData[nCurr] = pData[nNext];
				nCurr = nNext;
			}

			// restore element at end of chain
			pData[nCurr] = tTemp;
		}
		while (nIndex--);

		// sequentially reduce tree size by removing maximum element and rebalancing
		nIndex = nSize;
		while (--nIndex)
		{
			// save element at start of chain
			tTemp = pData[nIndex];
			pData[nIndex] = pData[0];

			nCurr = 0;
			nLast = nIndex - 1;
			nHalf = nIndex >> 1;
			while (nCurr < nHalf)
			{
				nNext = (nCurr << 1) + 1;
				if (nNext < nLast && pData[nNext + 1] > pData[nNext])
					nNext++;
				if (tTemp >= pData[nNext])
					break;

				// promote element in chain
				pData[nCurr] = pData[nNext];
				nCurr = nNext;
			}

			// restore element at end of chain
			pData[nCurr] = tTemp;
		}
	}

	// Sort elements in order determined by callback comparison
	template<class T> void SortElements(T* pData, int nSize, int (*pfn)(const T& tElement1, const T& tElement2, void* pUser), void* pUser)
	{
		ATLASSERT(!nSize || nSize > 0 && pData);

		// handle 0, 1 and 2 elements
		if (nSize <= 1)
			return;
		if (nSize == 2)
		{
			if (pfn(pData[0], pData[1], pUser) > 0)
				SwapElements<T>(pData[0], pData[1]);
			return;
		}

		T tTemp;

		// arrange elements as tree with greater elements appearing first
		int nIndex = (nSize >> 1) - 1, nCurr = 0, nNext = 0;
		int nLast = nSize - 1;
		int nHalf = nSize >> 1;
		do
		{
			// save element at start of chain
			tTemp = pData[nIndex];

			nCurr = nIndex;
			while (nCurr < nHalf)
			{
				nNext = (nCurr << 1) + 1;
				if (nNext < nLast && pfn(pData[nNext + 1], pData[nNext], pUser) > 0)
					nNext++;
				if (pfn(tTemp, pData[nNext], pUser) >= 0)
					break;

				// promote element in chain
				pData[nCurr] = pData[nNext];
				nCurr = nNext;
			}

			// restore element at end of chain
			pData[nCurr] = tTemp;
		}
		while (nIndex--);

		// sequentially reduce tree size by removing maximum element and rebalancing
		nIndex = nSize;
		while (--nIndex)
		{
			// save element at start of chain
			tTemp = pData[nIndex];
			pData[nIndex] = pData[0];

			nCurr = 0;
			nLast = nIndex - 1;
			nHalf = nIndex >> 1;
			while (nCurr < nHalf)
			{
				nNext = (nCurr << 1) + 1;
				if (nNext < nLast && pfn(pData[nNext + 1], pData[nNext], pUser) > 0)
					nNext++;
				if (pfn(tTemp, pData[nNext], pUser) >= 0)
					break;

				// promote element in chain
				pData[nCurr] = pData[nNext];
				nCurr = nNext;
			}

			// restore element at end of chain
			pData[nCurr] = tTemp;
		}
	}

	// Scan for element in array sorted in ascending order
	template<class S, class T> int BinaryScanForElement(S& tStorage, int nStart, int nCount, const T& tElement, BOOL bLeftmost = FALSE, BOOL bDontFail = FALSE)
	{
		ATLASSERT(nCount >= 0);

		// locate element using binary scan
		int nLow = nStart;
		int nHgh = nStart + nCount - 1;
		while (nLow <= nHgh)
		{
			int nMed = nLow + ((nHgh - nLow) >> 1);
			if (tStorage[nMed] < tElement)
				nLow = nMed + 1;
			else if (tStorage[nMed] > tElement)
				nHgh = nMed - 1;
			else
			{
				if (bLeftmost)
				{
					nHgh = nMed;

					// continue scan to locate leftmost element
					while (nLow != nHgh)
					{
						nMed = nLow + ((nHgh - nLow) >> 1);
						if (tStorage[nMed] < tElement)
							nLow = nMed + 1;
						else
							nHgh = nMed;
					}

					nMed = nHgh;
				}

				return nMed;
			}
		}
		return bDontFail? nLow: -1;
	}

	// Scan for element in array sorted in order determined by callback comparison
	template<class S, class T> int BinaryScanForElement(S& tStorage, int nStart, int nCount, const T& tElement, int (*pfn)(const T& tElement1, const T& tElement2, void* pUser), void* pUser, BOOL bLeftmost = FALSE, BOOL bDontFail = FALSE)
	{
		ATLASSERT(nCount >= 0);

		// locate element using binary scan
		int nLow = nStart;
		int nHgh = nStart + nCount - 1;
		while (nLow <= nHgh)
		{
			int nMed = nLow + ((nHgh - nLow) >> 1);
			if (pfn(tStorage[nMed], tElement, pUser) < 0)
				nLow = nMed + 1;
			else if (pfn(tStorage[nMed], tElement, pUser) > 0)
				nHgh = nMed - 1;
			else
			{
				if (bLeftmost)
				{
					nHgh = nMed;

					// continue scan to locate leftmost element
					while (nLow != nHgh)
					{
						nMed = nLow + ((nHgh - nLow) >> 1);
						if (pfn(tStorage[nMed], tElement, pUser) < 0)
							nLow = nMed + 1;
						else
							nHgh = nMed;
					}

					nMed = nHgh;
				}

				return nMed;
			}
		}
		return bDontFail? nLow: -1;
	}

	// Remove equal elements from sorted array
	template<class T> int RemoveEqualElements(T* pData, int nSize)
	{
		ATLASSERT(!nSize || nSize > 0 && pData);

		if (nSize < 2)
			return nSize;
		int nUsed = 1;

		// traverse an array
		for (int nIndex = 1; nIndex < nSize; nIndex++)
		{
			if (!(pData[nUsed - 1] == pData[nIndex]))
				if (nUsed++ != nIndex)
					pData[nUsed - 1] = pData[nIndex];
		}

		return nUsed;
	}

	// Remove equal elements from array sorted in order determined by callback comparison
	template<class T> int RemoveEqualElements(T* pData, int nSize, int (*pfn)(const T& tElement1, const T& tElement2, void* pUser), void* pUser)
	{
		ATLASSERT(!nSize || nSize > 0 && pData);

		if (nSize < 2)
			return nSize;
		int nUsed = 1;

		// traverse an array
		for (int nIndex = 1; nIndex < nSize; nIndex++)
		{
			if (pfn(pData[nUsed-1], pData[nIndex], pUser))
				if (nUsed++ != nIndex)
					pData[nUsed - 1] = pData[nIndex];
		}

		// delete remaining elements
		DestructElements<T>(pData + nUsed, nSize - nUsed);

		return nUsed;
	}

	template<class T> class CArray : public CAtlArray<T>
	{
	public:
		// Sorting
		void Sort()
			{ SortElements(GetData(), (int)GetCount()); }
		void Sort(int (*pfn)(const T& tElement1, const T& tElement2, void *pUser), void *pUser)
			{ SortElements(GetData(), (int)GetCount(), pfn, pUser); }
		void SortPart(int nStart, int nSize)
			{ ATLASSERT(nStart >= 0 && nSize >= 0 && nStart + nSize <= (int)GetCount()); SortElements(GetData() + nStart, nSize); }
		void SortPart(int nStart, int nSize, int (*pfn)(const T& tElement1, const T& tElement2, void* pUser), void* pUser)
			{ ATLASSERT(nStart >= 0 && nSize >= 0 && nStart + nSize <= (int)GetCount()); SortElements(GetData() + nStart, nSize, pfn, pUser); }

		// Searching
		int BinaryScan(const T& tElement, BOOL bLeftmost = FALSE, BOOL bDontFail = FALSE) const
			{ return BinaryScanForElement(*this, 0, (int)GetCount(), tElement, bLeftmost, bDontFail); }
		int BinaryScan(const T& tElement, int (*pfn)(const T& tElement1, const T& tElement2, void* pUser), void* pUser, BOOL bLeftmost = FALSE, BOOL bDontFail = FALSE) const
			{ return BinaryScanForElement(*this, 0, (int)GetCount(), tElement, pfn, pUser, bLeftmost, bDontFail); }

		// Removals
		void RemoveEqual()
			{ SetCount(RemoveEqualElements(GetData(), (int)GetCount())); }
		void RemoveEqual(int (*pfn)(const T& tElement1, const T& tElement2, void* pUser), void* pUser)
			{ SetCount(RemoveEqualElements(GetData(), (int)GetCount(), pfn, pUser)); }
	
		// Wrappers
		BOOL CopyToVariant(int nVariantType, VARIANT *pVariant);
	};

	// Copy array to variant
	template<class T> BOOL CArray<T>::CopyToVariant(int nVariantType, VARIANT *pVariant)
	{
		// remove any previous data from variant
		if (FAILED(VariantClear(pVariant)))
			return FALSE;

		// marshal empty array as VT_EMPTY
		if (0 == (int)GetCount())
			return TRUE;

		BOOL bFailed = TRUE;

		// marshal array with data as VT_ARRAY|xxx
		pVariant->parray = SafeArrayCreateVector(nVariantType, 0, (ULONG)GetSize());
		if (NULL != pVariant->parray)
		{
			pVariant->vt = VT_ARRAY | nVariantType;

    		T* pData = 0;

			// acquire raw pointer to array data
			if (SUCCEEDED(SafeArrayAccessData(pVariant->parray, (void**)&pData)))
			{
				CopyMemory(pData, GetData(), GetSize()*sizeof(T));

				// release raw pointer to array data
				if (SUCCEEDED(SafeArrayUnaccessData(pVariant->parray)))
					bFailed = FALSE;
			}
		}
		if (bFailed)
		{
			VariantClear(pVariant);
			return FALSE;
		}

		return TRUE;
	}
}

/*
// Example used
struct Point
{
	double m_dX;
	double m_dY;
};

// Compare points from A to Z
int ComparePointsAZ(const Point& dp1, const Point& dp2, void *pUser)
{
	return	(dp1.m_dX > dp2.m_dX)?  1 : 
			(dp1.m_dX < dp2.m_dX)? -1 :
			(dp1.m_dY > dp2.m_dY)?  1 : 
			(dp1.m_dY < dp2.m_dY)? -1 : 0;
}

// Compare index points from A to Z
int CompareIndexPointsAZ(const int& idx1, const int& idx2, void *pUser)
{
	ATLASSERT(NULL != pUser);
	const Point* pPoints = (const Point*)pUser;

	return	(pPoints[idx1].m_dX > pPoints[idx2].m_dX)?  1 : 
			(pPoints[idx1].m_dX < pPoints[idx2].m_dX)? -1 :
			(pPoints[idx1].m_dY > pPoints[idx2].m_dY)?  1 : 
			(pPoints[idx1].m_dY < pPoints[idx2].m_dY)? -1 : 0;
}

// array sort 
ArrayUtils::CArray<Point> arPoints;
arPoints.Sort(ComparePointsAZ, NULL);
arPoints.RemoveEqual(ComparePointsAZ, NULL);

// or index array sort
ArrayUtils::CArray<int> arIndexPoints; // 0,1,2,3,....
arIndexPoints.Sort(CompareIndexPointsAZ, arPoints.GetData());
arIndexPoints.RemoveEqual(CompareIndexPointsAZ, arPoints.GetData());
*/
