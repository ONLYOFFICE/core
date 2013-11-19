#pragma once

#include <atlenc.h>
#include <atlcoll.h>
#include "CHtmlBitmap.h"
#include "CHtmlCssFile.h"
#include "CHtmlUtility.h"
#include "version.h"


#define MHT_FROM         "From: <Saved by AVSDocumentConverter>\n"
#define MHT_MIME_VERSION "MIME-Version: 1.0\n"
#define MHT_CONTENT_TYPE "Content-Type: multipart/related;\n	type=\"text/html\";\n	boundary=\"----=_NextPart_000_00\"\n"
#define MHT_BOUNDARY     "------=_NextPart_000_00\n"
#define MHT_XMILE_OLE    "X-MileOLE: Produced by AVSHTMLFile.dll " STRVER
#define MHT_MESSAGE      "\n\nThis is a multi-part message in MIME format.\n\n"

class CHtmlExport
{
	CSimpleArray<CHtmlBitmap*> m_arrBitmaps;
	CSimpleArray<CHtmlCss*> m_arrCss;
	CSimpleArray<CString> m_arrStrings;
	CAtlMap<CString, int> m_mapImageName;
	CAtlMap<CString, int> m_mapCssName;
public:

	CHtmlExport()
	{
	}

	~CHtmlExport()
	{
		Clear();
	}

	int GetBitmapCount()
	{
		return m_arrBitmaps.GetSize();
	}
	int GetStringCount()
	{
		return m_arrStrings.GetSize();
	}

	void AddBitmap(CString strName, CString strPath)
	{
		if (NULL != m_mapImageName.Lookup(strName))
			return;

		CStringW sTemp;
		sTemp = strPath;
		Gdiplus::Bitmap oBitmap(sTemp);

		CHtmlBitmap *pHtmlBitmap = new CHtmlBitmap();

		pHtmlBitmap->Create(&oBitmap, strName, strPath);

		m_arrBitmaps.Add( pHtmlBitmap );

		m_mapImageName.SetAt(strName,1);
	}

	void AddStyleSheet(CString strName, CString strPath)
	{
		if (NULL != m_mapCssName.Lookup(strName))
			return;

		CHtmlCss *pHtmlCss = new CHtmlCss();
		pHtmlCss->Create(strName, strPath);

		m_arrCss.Add( pHtmlCss );

		m_mapCssName.SetAt(strName,1);
	}

	void AddString(CString strText)
	{
		m_arrStrings.Add(strText);
	}

	void Clear()
	{
		if ( m_arrBitmaps.GetSize() > 0 )
		{
			for ( int nIndex = 0; nIndex < m_arrBitmaps.GetSize(); nIndex++ )
			{
				CHtmlBitmap *pBitmap = m_arrBitmaps.GetData()[nIndex];
				delete pBitmap;
			}
			m_arrBitmaps.RemoveAll();
		}
		m_arrStrings.RemoveAll();
	}

	BOOL Build(CString strPath)
	{
		FILE* pFile = _wfopen( strPath, _T("w") );

		if (!pFile)
			return FALSE;

		// Пишем заголовок MHT
		fprintf( pFile, MHT_FROM );
		fprintf( pFile, MHT_MIME_VERSION  );
		fprintf( pFile, MHT_CONTENT_TYPE );		
		fprintf( pFile, MHT_XMILE_OLE );
		fprintf( pFile, MHT_MESSAGE );

		// Пишем заголовок к основному HTML-файлу
		fprintf( pFile, MHT_BOUNDARY  );
		fprintf( pFile, "Content-Type: text/html;\n	charset=\"utf-8\"\n");
		fprintf( pFile, "Content-Location: file:///C:/OriginHtml.html\n");
		fprintf( pFile, "Content-Transfer-Encoding: 8bit\n");
		fprintf( pFile, "\n");
		for (int nIndex = 0; nIndex < m_arrStrings.GetSize(); ++nIndex )
		{
			CStringA sLine = WCharToCharUTF8( m_arrStrings[nIndex] );
			fprintf( pFile, "%s\n", sLine.GetBuffer() );
		}

		// Записываем изображения
		for (int nIndex = 0; nIndex < m_arrBitmaps.GetSize(); ++nIndex )
		{
			std::wstring wsName = m_arrBitmaps[nIndex]->GetName();
			std::wstring wsPath = m_arrBitmaps[nIndex]->GetPath();
			std::wstring wsExt  = wsName.substr( wsName.find_last_of(L".") + 1 );

			fprintf( pFile, "\n" );
			fprintf( pFile, MHT_BOUNDARY );
			fprintf( pFile, "Content-Type: image/%s;\n", WCharToCharUTF8( wsExt.c_str() ).GetBuffer() );
			fprintf( pFile, "Content-Location: file:///C:/%s\n", WCharToCharUTF8( wsName.c_str() ).GetBuffer() );
			fprintf( pFile, "Content-Transfer-Encoding: base64\n" );		
			fprintf( pFile, "\n" );

			::fwrite( m_arrBitmaps[nIndex]->GetData2(), 1, m_arrBitmaps[nIndex]->GetSize2(), pFile );			
		}

		// Записываем стили
		for (int nIndex = 0; nIndex < m_arrCss.GetSize(); ++nIndex )
		{
			std::wstring wsName = m_arrCss[nIndex]->GetName();
			std::wstring wsPath = m_arrCss[nIndex]->GetPath();

			fprintf( pFile, "\n" );
			fprintf( pFile, MHT_BOUNDARY );
			fprintf( pFile, "Content-Type: text/css;\n" );
			fprintf( pFile, "Content-Location: file:///C:/%s\n", WCharToCharUTF8( wsName.c_str() ).GetBuffer() );
			fprintf( pFile, "Content-Transfer-Encoding: 8bit\n" );		
			fprintf( pFile, "\n" );

			::fwrite( m_arrCss[nIndex]->GetData2(), 1, m_arrCss[nIndex]->GetSize2(), pFile );			
		}

		fprintf( pFile, "\n");
		fprintf( pFile, MHT_BOUNDARY );

		fclose( pFile );

		return TRUE;
	}

private:

	CStringA WCharToCharUTF8(CStringW wsString)
	{
		char *sString = NULL;
		int nSize = WideCharToMultiByte( CP_UTF8, 0, wsString.GetBuffer(), wsString.GetLength(), sString, 0, NULL, NULL );
		sString = new char[nSize + 1];
		if ( !sString )
			return "";

		::memset( sString, 0x00, nSize + 1 );

		int nRes = WideCharToMultiByte( CP_UTF8, 0, wsString.GetBuffer(), wsString.GetLength(), sString, nSize, NULL, NULL );
		if ( nSize != nRes )
		{
			delete []sString;
			return "";
		}

		CStringA sResult( sString );
		sResult = sResult.Mid( 0, nSize );
		delete []sString;

		return sResult;
	}

	CStringA WCharToChar(CStringW wsString)
	{
		char *sString = NULL;
		int nSize = WideCharToMultiByte( CP_ACP, 0, wsString.GetBuffer(), wsString.GetLength(), sString, 0, NULL, NULL );
		sString = new char[nSize + 1];
		if ( !sString )
			return "";

		::memset( sString, 0x00, nSize + 1 );

		int nRes = WideCharToMultiByte( CP_ACP, 0, wsString.GetBuffer(), wsString.GetLength(), sString, nSize, NULL, NULL );
		if ( nSize != nRes )
		{
			delete []sString;
			return "";
		}

		CStringA sResult( sString );
		sResult = sResult.Mid( 0, nSize );
		delete []sString;

		return sResult;
	}

};