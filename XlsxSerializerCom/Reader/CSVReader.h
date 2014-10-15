#ifndef CSV_READER
#define CSV_READER

#include <stack>

#ifdef _WIN32
#include <atlbase.h>
#include <atlstr.h>
#else
#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"

namespace CSVReader
{
	void AddCell(CString &sText, INT nStartCell, std::stack<INT> &oDeleteChars, OOX::Spreadsheet::CRow &oRow, INT nRow, INT nCol, BOOL bIsWrap);
    void ReadFromCsvToXlsx(CString &sFileName, OOX::Spreadsheet::CXlsx &oXlsx, UINT nCodePage, const WCHAR wcDelimiter);
}

#endif //CSV_READER
