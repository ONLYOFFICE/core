#pragma once

#include "ASCStreamReader.h"

#include "../../../common/File.h"

class WinStreamReader : public SVMCore::SvStream
{
private: 

	NSFile::CFileBinary	m_oFileStream;
	long				m_nSize;
public: 
	WinStreamReader()
	{
		m_nSize = 0;
	}
	~WinStreamReader()
	{
		Close();
	}
	bool Open( std::wstring sFilename )
	{
		Close();

		bool bRes = m_oFileStream.OpenFile(sFilename);
		if( false == bRes )
			return false;

		m_nSize		= m_oFileStream.GetFileSize();

		return true;
	}
	void Close( )
	{
		m_oFileStream.CloseFile();
	}
	int Read( void* pData, int nSize )
	{
		if( NULL == pData )
			return 0;

		DWORD nSizeRead = 0;
		m_oFileStream.ReadFile((BYTE*)pData, nSize, nSizeRead);
		return nSizeRead;

	}
	void SetBuffer( char* pcBuffer, SVMCore::sal_uInt32 nSize )
	{
	}
	SVMCore::sal_Size Seek( SVMCore::sal_Size nFilePos )
	{
		if( STREAM_SEEK_TO_END == nFilePos )
			m_oFileStream.SeekFile( m_nSize - 1 );
		else
			m_oFileStream.SeekFile ( nFilePos );
		return m_oFileStream.GetFilePosition();
	}
	SVMCore::sal_Size Tell()
	{
		return m_oFileStream.GetFilePosition();
	}
	SVMCore::SvStream* CreateInstance()
	{
		return new WinStreamReader();
	}
	bool IsEof()
	{
		return m_nSize - 1 == m_oFileStream.GetFilePosition();
	}
};