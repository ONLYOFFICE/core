/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _GRAPHICS_ARRAY_H
#define _GRAPHICS_ARRAY_H

#include "Types.h"
#include <string.h>

#if 0 //__APPLE__

#include <vector>

template <class T>
class CArray
{
public:
    // Construction/destruction
    CArray()
    { }
    
    ~CArray()
    { }
    
    CArray(const CArray<T>& src)
    {
        int nSrcSize = src.GetCount();
        if (0 != nSrcSize)
        {            
            for (int i = 0; i < nSrcSize; i++)
                Add(src[i]);
        }
    }
    CArray<T>& operator=(const CArray<T>& src)
    {
        int nSrcSize = src.GetCount();
        
        RemoveAll();
        
        for (int i = 0; i < nSrcSize; i++)
            Add(src[i]);
        
        return *this;
    }
    
    // Operations
    int GetSize() const
    {
        return (int)m_aT.size();
    }
    int GetCount() const
    {
        return (int)m_aT.size();
    }
    
    INT SetCount(int nAllocSize)
    {
        RemoveAll();
        
        for (int i = 0; i < nAllocSize; i++)
            Add();
        
        return TRUE;
    }
    
    INT Add()
    {
        T element;
        m_aT.push_back(element);

        return TRUE;
    }
    
    INT Add(const T& t)
    {
        m_aT.push_back(t);

        return TRUE;
    }
    INT RemoveAt(int nIndex)
    {
        m_aT.erase(m_aT.begin() + nIndex);
        return TRUE;
    }
    
    INT RemoveAt(int nIndex, int nCount)
    {
        m_aT.erase(m_aT.begin() + nIndex, m_aT.begin() + nIndex + nCount);
        return TRUE;
    }
    
    void RemoveAll()
    {
        m_aT.clear();
    }
    const T& operator[] (int nIndex) const
    {
        if (nIndex < 0 || nIndex >= (int)m_aT.size())
        {
            return m_aT[0]; // exeption
        }
        return m_aT[nIndex];
    }
    T& operator[] (int nIndex)
    {
        if (nIndex < 0 || nIndex >= (int)m_aT.size())
        {
            return m_aT[0]; // exeption
        }
        return m_aT[nIndex];
    }
    
private:
    
    std::vector<T> m_aT;
};

#else
#include <stdlib.h>

#if defined(_IOS) || defined(__ANDROID__)
#include <new>
#endif

template <class T>
class CArray
{
public:
	// Construction/destruction
	CArray() : m_aT(NULL), m_nSize(0), m_nAllocSize(0)
	{ }

	~CArray()
	{
		RemoveAll();
		if (m_aT)
			::free(m_aT);
	}

	CArray(const CArray<T>& src) : m_aT(NULL), m_nSize(0), m_nAllocSize(0)
	{
		int nSrcSize = src.GetCount();
		if (0 != nSrcSize)
		{
			m_aT = (T*)::calloc(nSrcSize, sizeof(T));
			if (m_aT != NULL)
			{
				m_nAllocSize = nSrcSize;
				for (int i = 0; i < nSrcSize; i++)
					::new(m_aT + i) T(src.m_aT[i]);

				m_nSize = nSrcSize;
			}
		}
	}
	CArray<T>& operator=(const CArray<T>& src)
	{
		RemoveAll();
		int nSrcSize = src.GetCount();

		if (0 != nSrcSize)
		{
			m_aT = (T*)::calloc(nSrcSize, sizeof(T));
			if (m_aT != NULL)
			{
				m_nAllocSize = nSrcSize;
				for (int i = 0; i < nSrcSize; i++)
					::new(m_aT + i) T(src.m_aT[i]);

				m_nSize = nSrcSize;
			}
		}
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

	INT SetCount(int nAllocSize)
	{
		RemoveAll();
		if (nAllocSize != m_nAllocSize)
		{
			int nNewAllocSize = nAllocSize;

			m_aT = (T*)::calloc(nNewAllocSize, sizeof(T));
            for (int i = 0; i < nNewAllocSize; i++)
                ::new(m_aT + i) T;

			if (NULL == m_aT)
				return FALSE;

			m_nAllocSize = nNewAllocSize;
			m_nSize = nNewAllocSize;
		}

		return TRUE;
	}

	INT Add()
	{
		if (m_nSize == m_nAllocSize)
		{
            int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);

            T* newT = (T*)::calloc(nNewAllocSize, sizeof(T));
            //T* newT = (T*)malloc(nNewAllocSize * sizeof(T));

			if (NULL == newT)
				return FALSE;

			m_nAllocSize = nNewAllocSize;

			// без операторов копирования и деструкторов
			MoveElements(newT, m_aT, m_nSize);
			if (NULL != m_aT)
				::free(m_aT);

			m_aT = newT;
		}

        ::new(m_aT + m_nSize) T;
		m_nSize++;
		return TRUE;
	}

	INT Add(const T& t)
	{
		if (FALSE == Add())
			return FALSE;
		m_aT[m_nSize - 1] = t;
		return TRUE;
	}
	INT RemoveAt(int nIndex, int nCount = 1)
	{
		if (nIndex < 0 || nIndex >= m_nSize || nCount < 1)
			return FALSE;

		if ((nIndex + nCount) > m_nSize)
			nCount = m_nSize - nIndex;

		// вызываем деструкторы
		for (int i = 0; i < nCount; ++i)
			m_aT[nIndex + i].~T();

		int nNewCount = nIndex + nCount;
		int nMoveCount = m_nSize - nNewCount;

		if (nNewCount != m_nSize)
		{
			MoveElements(m_aT + nIndex, m_aT + nNewCount, nMoveCount);
		}
		m_nSize -= nCount;
		return TRUE;
	}

	void RemoveAll()
	{
		if (m_aT != NULL)
		{
			// вызываем деструкторы
			for (int i = 0; i < m_nSize; i++)
				m_aT[i].~T();

			::free(m_aT);
			m_aT = NULL;
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

private:
	void MoveElements(T* dst, T* src, int nCount)
	{
		if (0 >= nCount)
			return;
#ifdef WIN32
		memmove_s((void*)dst, nCount * sizeof(T), (void*)src, nCount * sizeof(T));
#else
		memmove((void*)dst, (void*)src, nCount * sizeof(T));
#endif
	}
};

#endif


#endif // !defined(_GRAPHICS_ARRAY_H)
