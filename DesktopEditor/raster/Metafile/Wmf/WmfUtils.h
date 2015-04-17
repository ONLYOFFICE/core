#ifndef _WMF_UTILS_H
#define _WMF_UTILS_H

#include "WmfTypes.h"
#include <stdio.h>
#include <time.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>

#include "../../../common/File.h"

#define UTF8_TO_U(val) NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)val, strlen(val))
#define U_TO_UTF8(val) NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(val, wcslen(val))

static std::wstring ascii_to_unicode(const char *src)
{
	size_t nSize = mbstowcs(0, src, 0);
	wchar_t* pBuffer = new wchar_t[nSize];
	nSize = mbstowcs(pBuffer, src, nSize);
	std::wstring sRes;
	if (nSize != (size_t)-1)
		sRes = std::wstring(pBuffer, nSize);
	delete[] pBuffer;
	return sRes;
}
static std::string unicode_to_ascii(const wchar_t *src)
{
	size_t nSize = wcstombs(0, src, 0);
	char* pBuffer = new char[nSize];
	nSize = wcstombs(pBuffer, src, nSize);
	std::string sRes;
	if (nSize != (size_t)-1)
		sRes = std::string(pBuffer, nSize);
	delete[] pBuffer;
	return sRes;
}

static char *WmfStrDup(const char *sString)
{
	if ( NULL == sString )
		return NULL;

	char *sCopy = (char*)malloc( strlen(sString) + 1 );
	if ( NULL == sCopy )
		return NULL;

	strcpy( sCopy, sString );

	return sCopy;
}
static TWmfCoord WmfCoord(unsigned short ushX, unsigned short ushY)
{	
	TWmfCoord oPoint;

	oPoint.unX = UShort_2_Long( ushX );
	oPoint.unY = UShort_2_Long( ushY );

	return oPoint;
}

static void PolyPoly_To_PolyLine(TWmfPolyPoly *pPolyPoly, TWmfPolyLine *pPolyLine, unsigned short ushPolyIndex)
{	
	unsigned short ushCount = pPolyPoly->pCount[ushPolyIndex];

	if ( NULL == pPolyPoly->ppPoints ) 
		return;

	if ( ( NULL == pPolyPoly->ppPoints[ushPolyIndex] ) || ( pPolyPoly->pCount[ushPolyIndex] < 3 ) ) 
		return;

	// Избавляемся от лишних точек
	while ( ( pPolyPoly->ppPoints[ushPolyIndex][0].fX == pPolyPoly->ppPoints[ushPolyIndex][ushCount - 1].fX ) && ( pPolyPoly->ppPoints[ushPolyIndex][0].fY == pPolyPoly->ppPoints[ushPolyIndex][ushCount - 1].fY ) )
	{
		ushCount--;
		if ( ushCount < 3 ) 
			break;
	}
	if ( ushCount < 3 ) 
		return;

	// Определяем последний ли полигон
	bool bLast = false;
	if ( ushPolyIndex < ( pPolyPoly->ushPolyCount - 1 ) )
	{	
		if ( ( NULL == pPolyPoly->ppPoints[ushPolyIndex + 1] == 0 ) || ( pPolyPoly->pCount[ushPolyIndex + 1] < 3 ) )
		{	
			bLast = 1;
		}
	}
	else
	{	
		bLast = 1;
	}

	if ( bLast )
	{	
		for ( unsigned short ushPointIndex = 0; ushPointIndex < ushCount; ushPointIndex++ )
		{	
			pPolyLine->pPoints[pPolyLine->ushCount].fX = pPolyPoly->ppPoints[ushPolyIndex][ushPointIndex].fX;
			pPolyLine->pPoints[pPolyLine->ushCount].fY = pPolyPoly->ppPoints[ushPolyIndex][ushPointIndex].fY;
			pPolyLine->ushCount++;
		}
		pPolyLine->pPoints[pPolyLine->ushCount].fX = pPolyPoly->ppPoints[ushPolyIndex][0].fX;
		pPolyLine->pPoints[pPolyLine->ushCount].fY = pPolyPoly->ppPoints[ushPolyIndex][0].fY;
		pPolyLine->ushCount++;

		return;
	}

	// Ищем в данном полигоне точку ближающую к нулевой точке следующего полигона (т.к. этот полигон не последний, то все впорядке)
	unsigned short ushMinIndex = 0;
	double dMinR2;
	for ( unsigned short ushPointIndex = 0; ushPointIndex < ushCount; ushPointIndex++ )
	{	
		double dX2 = (double) pPolyPoly->ppPoints[ushPolyIndex][ushPointIndex].fX - (double) pPolyPoly->ppPoints[ushPolyIndex+1][0].fX;
		dX2 *= dX2;
		double dY2 = (double) pPolyPoly->ppPoints[ushPolyIndex][ushPointIndex].fY - (double) pPolyPoly->ppPoints[ushPolyIndex+1][0].fY;
		dY2 *= dY2;
		double dR2 = dX2 + dY2;
		if ( 0 == ushPointIndex )
		{	
			dMinR2 = dR2;
		}
		else if ( dR2 < dMinR2 )
		{	
			dMinR2 = dR2;
			ushMinIndex = ushPointIndex;
		}
	}

	for ( unsigned short ushPointIndex = 0; ushPointIndex <= ushMinIndex; ushPointIndex++ )
	{	
		pPolyLine->pPoints[pPolyLine->ushCount].fX = pPolyPoly->ppPoints[ushPolyIndex][ushPointIndex].fX;
		pPolyLine->pPoints[pPolyLine->ushCount].fY = pPolyPoly->ppPoints[ushPolyIndex][ushPointIndex].fY;
		pPolyLine->ushCount++;
	}

	PolyPoly_To_PolyLine( pPolyPoly, pPolyLine, (unsigned short)(ushPolyIndex + 1) );

	for ( unsigned short ushPointIndex = ushMinIndex; ushPointIndex < ushCount; ushPointIndex++ )
	{	
		pPolyLine->pPoints[pPolyLine->ushCount].fX = pPolyPoly->ppPoints[ushPolyIndex][ushPointIndex].fX;
		pPolyLine->pPoints[pPolyLine->ushCount].fY = pPolyPoly->ppPoints[ushPolyIndex][ushPointIndex].fY;
		pPolyLine->ushCount++;
	}
	pPolyLine->pPoints[pPolyLine->ushCount].fX = pPolyPoly->ppPoints[ushPolyIndex][0].fX;
	pPolyLine->pPoints[pPolyLine->ushCount].fY = pPolyPoly->ppPoints[ushPolyIndex][0].fY;
	pPolyLine->ushCount++;
}

static BOOL WmfOpenTempFile(std::wstring *pwsName, FILE **ppFile, wchar_t *wsMode, wchar_t *wsExt, wchar_t *wsFolder)
{
	std::wstring wsTemp, wsFileName;
	FILE *pTempFile = NULL;
#if defined(_WIN32) || defined (_WIN64)
	wchar_t *wsTempDir;
	if ((wsTempDir = _wgetenv(L"TEMP")) && (wsFolder == NULL))
	{
		wsTemp = std::wstring(wsTempDir);
#else
	char *wsTempDirA;
	if ( ( wsTempDirA = getenv( "TEMP" ) ) && ( wsFolder == NULL ) )
	{
		std::wstring wsTempDir = UTF8_TO_U( wsTempDirA );
		wsTemp = wsTempDir.c_str();
#endif
		wsTemp.append(L"/");
	}
	else if (wsFolder != NULL)
	{
		wsTemp = std::wstring(wsFolder);
		wsTemp.append(L"/");
	}
	else
	{
		wsTemp = L"";
	}
	wsTemp.append(L"x");
	int nTime = (int)time(NULL);
	for (int nIndex = 0; nIndex < 1000; ++nIndex)
	{
		wsFileName = wsTemp;
		wchar_t buffer[24];
		_itow(nTime + nIndex,buffer,10);
		wsFileName.append(buffer);

		if (wsExt)
		{
			wsFileName.append(wsExt);
		}
#if defined (_WIN32) || defined (_WIN64)
		if (!(pTempFile = _wfopen(wsFileName.c_str(), L"r")))
		{
			if (!(pTempFile = _wfopen(wsFileName.c_str(), wsMode)))
#else
		std::string sFileName = U_TO_UTF8(wsFileName);
		if ( !( pTempFile = fopen( sFileName.c_str(), "r" ) ) )
		{
			std::string sMode = U_TO_UTF8(wsMode);
			if ( !( pTempFile = fopen( sFileName.c_str(), sMode.c_str() ) ) )
#endif
			{
				return FALSE;
			}
			*pwsName = wsFileName;
			*ppFile = pTempFile;
			return TRUE;
		}

		fclose(pTempFile);
	}

	return FALSE;
}


#endif /* _WMF_UTILS_H */
