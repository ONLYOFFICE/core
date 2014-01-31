#ifndef _GRAPHICS_ARRAY_H
#define _GRAPHICS_ARRAY_H

#include "Types.h"

template <class T>
class CArray
{
public:
	// Construction/destruction
	CArray() : m_aT(NULL), m_nSize(0), m_nAllocSize(0)
	{ }

	~CArray()
	{
		if (m_aT)
			delete []m_aT; 
	}

	CArray(const CArray<T>& src) : m_aT(NULL), m_nSize(0), m_nAllocSize(0)
	{
		int nSrcSize = src.GetCount();
        if (0 != nSrcSize)
        {
			m_aT = new T[nSrcSize];
			if (m_aT != NULL)
			{
				m_nAllocSize = nSrcSize;
				for (int i = 0; i < nSrcSize; i++)
					Add(src[i]);
			}
		}
	}
	CArray<T>& operator=(const CArray<T>& src)
	{
		int nSrcSize = src.GetCount();
		
		RemoveAll();
		m_aT = new T[nSrcSize];
		if (m_aT != NULL)
			m_nAllocSize = nSrcSize;
		
		for (int i = 0; i < nSrcSize; i++)
			Add(src[i]);

		return *this;
	}

	// Operations
	int GetSize() const
	{
		return m_nSize;
	}
	int GetCount() const
	{
		return m_nSize;
	}

	BOOL SetCount(int nAllocSize)
	{
		RemoveAll();
		if (nAllocSize != m_nAllocSize)
		{
			int nNewAllocSize = nAllocSize;
	  
			m_aT = new T[nNewAllocSize];

			if (NULL == m_aT)
				return FALSE;
		}

		return TRUE;
	}

	BOOL Add()
	{
		if (m_nSize == m_nAllocSize)
		{
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
	  
			T* newT = new T[nNewAllocSize];

			if (NULL == newT)
				return FALSE;

			m_nAllocSize = nNewAllocSize;

			if (m_nSize != 0)
			{
				for (int i = 0; i < m_nSize; ++i)
					newT[i] = m_aT[i];
			}
			
			RELEASEARRAYOBJECTS(m_aT);
			m_aT = newT;						
		}

		return TRUE;
	}

	BOOL Add(const T& t)
	{
		if (m_nSize == m_nAllocSize)
		{
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
	  
			T* newT = new T[nNewAllocSize];

			if (NULL == newT)
				return FALSE;

			m_nAllocSize = nNewAllocSize;

			if (m_nSize != 0)
			{
				for (int i = 0; i < m_nSize; ++i)
					newT[i] = m_aT[i];
			}
			
			RELEASEARRAYOBJECTS(m_aT);
			m_aT = newT;						
		}

		m_aT[m_nSize++] = t;		
		return TRUE;
	}
	BOOL RemoveAt(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_nSize)
			return FALSE;
		m_aT[nIndex].~T();
		if (nIndex != (m_nSize - 1))
			memmove_s((void*)(m_aT + nIndex), (m_nSize - nIndex) * sizeof(T), (void*)(m_aT + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(T));
		m_nSize--;
		return TRUE;
	}

	BOOL RemoveAt(int nIndex, int nCount)
	{
		if (nIndex < 0 || nIndex >= m_nSize || nCount < 1)
			return FALSE;

		if ((nIndex + nCount) > m_nSize)
			nCount = m_nSize - nIndex;

		for (int i = 0; i < nCount; ++i)
			m_aT[nIndex + i].~T();

		if ((nIndex + nCount) != m_nSize)
			memmove_s((void*)(m_aT + nIndex), (m_nSize - nIndex - nCount + 1) * sizeof(T), (void*)(m_aT + nIndex + nCount), (m_nSize - (nIndex + nCount)) * sizeof(T));
		m_nSize--;
		return TRUE;
	}

	void RemoveAll()
	{
		if (m_aT != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
				m_aT[i].~T();
			
			RELEASEARRAYOBJECTS(m_aT);
		}
		m_nSize = 0;
		m_nAllocSize = 0;
    }   
	const T& operator[] (int nIndex) const
	{
		if (nIndex < 0 || nIndex >= m_nSize)
		{
			return m_aT[0]; // exeption
		}
		return m_aT[nIndex];
	}
	T& operator[] (int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_nSize)
		{
			return m_aT[0]; // exeption
		}
		return m_aT[nIndex];
	}
	T* GetData() const
	{
		return m_aT;
	}

private:

	T*	m_aT;
	int m_nSize;
	int m_nAllocSize;
};


#endif // !defined(_GRAPHICS_ARRAY_H)
