#pragma once
#include "ASCStreamReader.h"

#include <windows.h>
#include <atlbase.h>
#include <atlstr.h>

#include "MappingFile.h"
#include "StreamUtils.h"
#include "ATLDefine.h"

using namespace MemoryMapping;
using namespace Streams;

class WinStreamReader : public SVMCore::SvStream
{
private: CMappingFile m_oMapingFile;
private: CBufferedStream m_oFileStream;
private: CBuffer* m_poBuffer;
private: long m_nSize;
public: WinStreamReader()
		{
			m_poBuffer = NULL;
		}
public: ~WinStreamReader()
		{
			Close();
		}
public: bool Open( std::wstring sFilename )
		{
			Close();

			BOOL bRes = m_oMapingFile.Open( CString( sFilename.c_str() ) );
			if( FALSE == bRes )
				return false;

			m_nSize = m_oMapingFile.GetSize();
			m_poBuffer = new CBuffer();
			m_oFileStream.SetBuffer( m_poBuffer );
			bRes = m_oFileStream.Create( m_oMapingFile.GetData(), m_nSize );
			if( FALSE == bRes )
				return false;
			//в FileStream происходит копирование
			m_oMapingFile.Close();
			return true;
		}
public: void Close( )
		{
			RELEASEOBJECT( m_poBuffer );
		}
public: int Read( void* pData, int nSize )
		{
			if( NULL == pData )
				return 0;
			BYTE* pPointer = m_oFileStream.ReadPointer( nSize );
			if( NULL == pPointer )
			{
				long nBufSize = m_nSize;
				long nPos = m_oFileStream.GetPosition();
				if( nSize + nPos >= nBufSize )
					nSize = nBufSize - nPos - 1;
				pPointer = m_oFileStream.ReadPointer( nSize );
			}
			if( NULL != pPointer )
				memcpy( pData, pPointer, nSize );
			return nSize;
		}
public: SVMCore::sal_Size Seek( SVMCore::sal_Size nFilePos )
		{
			if( STREAM_SEEK_TO_END == nFilePos )
				m_oFileStream.Seek( m_nSize - 1 );
			else
				m_oFileStream.Seek( nFilePos );
			return m_oFileStream.GetPosition();
		}
public: SVMCore::sal_Size Tell()
		{
			return m_oFileStream.GetPosition();
		}
public: SVMCore::SvStream* CreateInstance()
		{
			return new WinStreamReader();
		}
public: void SetBuffer( char* pcBuffer, SVMCore::sal_uInt32 nSize )
		{
			Close( );
			m_nSize = nSize;
			m_poBuffer = new CBuffer();
			m_oFileStream.SetBuffer( m_poBuffer );
			m_oFileStream.Create( (BYTE*)pcBuffer, nSize );
		}
public: bool IsEof()
		{
			return m_nSize - 1 == m_oFileStream.GetPosition();
		}
};