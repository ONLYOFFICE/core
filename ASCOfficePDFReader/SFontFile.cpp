#include "stdafx.h"

#include <stdio.h>
#include "StringExt.h"
#include "SFontFile.h"
#include "SFontFileID.h"

//-------------------------------------------------------------------------------------------------------------------------------
// SFontFile
//-------------------------------------------------------------------------------------------------------------------------------

SFontFile::SFontFile(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile) 
{
	m_pFontFileID = pFontFileID;
	m_wsFileName  = CStringW( wsFileName );
	m_bDeleteFile = bDeleteFile;
	m_nRef        = 0;
}

SFontFile::~SFontFile() 
{
	if ( m_bDeleteFile ) 
	{
		_wunlink( m_wsFileName.GetBuffer() );
	}

	if ( m_pFontFileID )
		delete m_pFontFileID;
}

void SFontFile::AddRef() 
{
	++m_nRef;
}

void SFontFile::Release() 
{
	if ( !--m_nRef ) 
	{
		delete this;
	}
}
