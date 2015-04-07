#ifndef _WMF_MEMORY_H
#define _WMF_MEMORY_H

#include <stdio.h>

#ifdef _DEBUG
//#define DebugCheckMemory
#endif

class CWmfMemoryManager
{

public:

	CWmfMemoryManager()
	{
#ifdef DebugCheckMemory
		m_arrDescription.RemoveAll();
		m_arrMemoryList.RemoveAll();
#endif

	}
	~CWmfMemoryManager()
	{
#ifdef DebugCheckMemory
		m_arrDescription.RemoveAll();
		m_arrMemoryList.RemoveAll();
#endif
	}

	void *Malloc(size_t unSize, CString sDescription )
	{
		void *pAddress = malloc( unSize );
		if ( NULL == pAddress )
			return NULL;

#ifdef DebugCheckMemory
		Add( pAddress, sDescription );
#endif

		return pAddress;
	}
	void *Realloc(void *pMemory, size_t unSize)
	{
#ifdef DebugCheckMemory
		CString sDescription = Remove( pMemory );
#endif

		void *pAddress = realloc( pMemory, unSize );
		if ( NULL == pAddress )
			return NULL;

#ifdef DebugCheckMemory
		Add( pAddress, sDescription );
#endif

		return pAddress;
	}
	void Free   (void *pMemory)
	{

		if ( pMemory )
		{
			free( pMemory );
		}

#ifdef DebugCheckMemory
		Remove( pMemory );
#endif
	}

private:

#ifdef DebugCheckMemory
	void Add(void *pAddress, CString sDescription)
	{
		m_arrDescription.Add( sDescription );
		m_arrMemoryList.Add( pAddress );
	}
	CString Remove(void *pAddress)
	{
		int nIndex = m_arrMemoryList.Find( pAddress );
		if ( -1 == nIndex )
			return _T("");

		m_arrMemoryList.RemoveAt( nIndex );
		CString sResult = m_arrDescription.GetData()[nIndex];
		m_arrDescription.RemoveAt( nIndex );

		return sResult;
	}

#endif

private:

#ifdef DebugCheckMemory
	CSimpleArray<void *>   m_arrMemoryList;
	CSimpleArray<CStringW> m_arrDescription;
#endif
};

#endif /* _WMF_MEMORY_H */