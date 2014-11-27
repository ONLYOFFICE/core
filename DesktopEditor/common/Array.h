#ifndef _GRAPHICS_ARRAY_H
#define _GRAPHICS_ARRAY_H

#include "Types.h"
#include <string.h>
#ifdef _LINUX
#include "string.h"
#endif

#if 1 //__APPLE__

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


// NOTE: при определенных условиях ловим исключения в деструкторе класса    (T)CArray()::~CArray()

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
    
    INT SetCount(int nAllocSize)
    {
        RemoveAll();
        if (nAllocSize != m_nAllocSize)
        {
            int nNewAllocSize = nAllocSize;
            
            m_aT = new T[nNewAllocSize];
            
            if (NULL == m_aT)
                return FALSE;
            
            m_nSize = nAllocSize;
        }
        
        return TRUE;
    }
    
    INT Add()
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
        
        m_nSize++;
        return TRUE;
    }
    
    INT Add(const T& t)
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
    INT RemoveAt(int nIndex)
    {
        if (nIndex < 0 || nIndex >= m_nSize)
            return FALSE;
        m_aT[nIndex].~T();
        if (nIndex != (m_nSize - 1))
        {
#ifdef WIN32
            memmove_s((void*)(m_aT + nIndex), (m_nSize - nIndex) * sizeof(T), (void*)(m_aT + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(T));
#else
            memmove((void*)(m_aT + nIndex), (void*)(m_aT + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(T));
#endif
        }
        m_nSize--;
        return TRUE;
    }
    
    INT RemoveAt(int nIndex, int nCount)
    {
        if (nIndex < 0 || nIndex >= m_nSize || nCount < 1)
            return FALSE;
        
        if ((nIndex + nCount) > m_nSize)
            nCount = m_nSize - nIndex;
        
        for (int i = 0; i < nCount; ++i)
            m_aT[nIndex + i].~T();
        
        if ((nIndex + nCount) != m_nSize)
        {
#ifdef WIN32
            memmove_s((void*)(m_aT + nIndex), (m_nSize - nIndex - nCount + 1) * sizeof(T), (void*)(m_aT + nIndex + nCount), (m_nSize - (nIndex + nCount)) * sizeof(T));
#else
            memmove((void*)(m_aT + nIndex), (void*)(m_aT + nIndex + nCount), (m_nSize - (nIndex + nCount)) * sizeof(T));
#endif
        }
        m_nSize -= nCount;
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

#endif


#endif // !defined(_GRAPHICS_ARRAY_H)
