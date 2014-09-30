#include "CommonInclude.h"
#include "Common.h"

#ifndef _gcvt
#include "stdlib.h"
#define _CVTBUFSIZE  128
#define _gcvt gcvt
#endif

#define DBL_MAX 15
#define DBL_MAXDIG10 17

CString OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(double dVal)
{
	//согласно http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2005.pdf
	//DBL_MAX = 15 digits, DBL_MAXDIG10 = 17 digits.
	char buffer[_CVTBUFSIZE];
	_gcvt( dVal, DBL_MAXDIG10, buffer );
	CString sRes(buffer);
	int nLength = sRes.GetLength();
	if(nLength > 0 && '.' == sRes[nLength - 1])
		sRes = sRes.Left(nLength - 1);
	return sRes;
}
