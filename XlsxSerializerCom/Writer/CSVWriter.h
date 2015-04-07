#ifndef CSV_WRITER
#define CSV_WRITER

#define CP_UTF16		1200
#define CP_unicodeFFFE	1201

#if defined(_WIN32) || defined (_WIN64)
	#include <atlbase.h>
	#include <atlstr.h>
#else
	#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../DesktopEditor/common/File.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"

namespace CSVWriter
{
	void WriteFile(NSFile::CFileBinary *pFile, WCHAR **pWriteBuffer, INT &nCurrentIndex, CString &sWriteString, UINT &nCodePage, BOOL bIsEnd = FALSE);
    void WriteFromXlsxToCsv(CString &sFileDst, OOX::Spreadsheet::CXlsx &oXlsx, UINT nCodePage, const WCHAR wcDelimiter, BOOL bJSON);
}

#endif //CSV_WRITER
