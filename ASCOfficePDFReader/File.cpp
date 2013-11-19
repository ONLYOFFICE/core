#include "stdafx.h"

#include "File.h"

//------------------------------------------------------------------------
// CDir and CDirEntry
//------------------------------------------------------------------------

CDirEntry::CDirEntry(char *sDirPath, char *sName, BOOL bCheckStat) 
{
	StringExt *seTemp;

	m_seName = new StringExt( sName );
	m_bDir = FALSE;
	if ( bCheckStat ) 
	{
		seTemp = new StringExt( sDirPath );
		AppendToPath( seTemp, sName);
		int nFileAttr = GetFileAttributesA( seTemp->GetBuffer() );
		m_bDir = ( nFileAttr != 0xFFFFFFFF && ( nFileAttr & FILE_ATTRIBUTE_DIRECTORY) );
		delete seTemp;
	}
}

CDirEntry::~CDirEntry() 
{
	delete m_seName;
}

CDir::CDir(char *sName, BOOL bCheckStat) 
{
	m_sePath = new StringExt( sName );
	m_bCheckStat = bCheckStat;
	StringExt *seTemp = m_sePath->Copy();
	seTemp->Append("/*.*");
	m_hHandle = FindFirstFileA( seTemp->GetBuffer(), &m_FileFindData);
	delete seTemp;
}

CDir::~CDir() 
{

	delete m_sePath;
	if ( m_hHandle ) 
	{
		FindClose(m_hHandle);
		m_hHandle = NULL;
	}
}

CDirEntry *CDir::GetNextEntry() 
{
	CDirEntry *pEntry;

	if ( m_hHandle ) 
	{
		pEntry = new CDirEntry( m_sePath->GetBuffer(), m_FileFindData.cFileName, m_bCheckStat );
		if ( m_hHandle  && !FindNextFileA( m_hHandle, &m_FileFindData) ) 
		{
			FindClose(m_hHandle);
			m_hHandle = NULL;
		}
	} 
	else
		pEntry = NULL;

	return pEntry;
}

void CDir::Rewind() 
{
	if ( m_hHandle )
		FindClose( m_hHandle );
	StringExt *seTemp = m_sePath->Copy();
	seTemp->Append("/*.*");
	m_hHandle = FindFirstFileA( seTemp->GetBuffer(), &m_FileFindData);
	delete seTemp;
}
