#include "stdafx.h"

#include <stddef.h>
#include <string.h>
#include "MemoryUtils.h"
#include "Object.h"
#include "XRef.h"
#include "Dict.h"

//------------------------------------------------------------------------
// Dict
//------------------------------------------------------------------------

Dict::Dict(XRef *pXref)
{
	m_pXref = pXref;
	m_arrEntries = NULL;
	m_nEntriesSize = m_nEntriesCount = 0;
	m_nRef = 1;
}

Dict::~Dict() 
{
	for (int nIndex = 0; nIndex < m_nEntriesCount; ++nIndex ) 
	{
		MemUtilsFree( m_arrEntries[ nIndex ].sKey);
		m_arrEntries[ nIndex ].oValue.Free();
	}
	MemUtilsFree(m_arrEntries);
}

void Dict::AddItem(char *sKey, Object *pValue) 
{
	if ( m_nEntriesCount == m_nEntriesSize ) 
	{
		if ( 0 == m_nEntriesCount ) 
		{
			m_nEntriesSize = 8;
		} 
		else 
		{
			m_nEntriesSize *= 2;
		}
		m_arrEntries = (DictEntry *)MemUtilsReallocArray(m_arrEntries, m_nEntriesSize, sizeof(DictEntry));
	}
	m_arrEntries[m_nEntriesCount].sKey = sKey;
	m_arrEntries[m_nEntriesCount].oValue = *pValue;
	++m_nEntriesCount;
}

inline DictEntry *Dict::Find(char *sKey) 
{
	for (int nIndex = 0; nIndex < m_nEntriesCount; ++nIndex ) 
	{
		if ( !strcmp( sKey, m_arrEntries[ nIndex ].sKey ) )
			return &m_arrEntries[ nIndex ];
	}
	return NULL;
}

BOOL Dict::CheckType(char *sType) 
{
	DictEntry *pEntry = NULL;

	return ( pEntry = Find("Type") ) && pEntry->oValue.IsName( sType );
}

Object *Dict::Search(char *sKey, Object *pObj) 
{
	DictEntry *pEntry = NULL;

	return ( pEntry = Find( sKey ) ) ? pEntry->oValue.Fetch( m_pXref, pObj ) : pObj->InitNull();
}

Object *Dict::SearchAndCopy(char *sKey, Object *pObj) 
{
	DictEntry *pEntry = NULL;

	return ( pEntry = Find( sKey ) ) ? pEntry->oValue.Copy( pObj ) : pObj->InitNull();
}

char *Dict::GetKey(int nIndex) 
{
	return m_arrEntries[ nIndex ].sKey;
}

Object *Dict::GetValue(int nIndex, Object *pObj) 
{
	return m_arrEntries[ nIndex ].oValue.Fetch( m_pXref, pObj);
}

Object *Dict::GetValueCopy(int nIndex, Object *pObj) 
{
	return m_arrEntries[ nIndex ].oValue.Copy( pObj );
}
