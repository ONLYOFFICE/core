#include "stdafx.h"

#include "FontFile.h"

//-------------------------------------------------------------------------------------------------------------------------------
// CFontFile
//-------------------------------------------------------------------------------------------------------------------------------

CFontFile::CFontFile(wchar_t *wsFileName, long lIndex) 
{
	m_wsFileName  = CStringW( wsFileName );
	m_lIndex      = lIndex;
	m_nRef        = 0;
}

CFontFile::~CFontFile() 
{
}

void CFontFile::AddRef() 
{
	++m_nRef;
}

void CFontFile::Release() 
{
	if ( !--m_nRef ) 
	{
		delete this;
	}
}
