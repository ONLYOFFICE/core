#ifndef _PDF_WRITER_ONLINEOFFICEBINTOPDF_H
#define _PDF_WRITER_ONLINEOFFICEBINTOPDF_H

#include <string>
#include "PdfRenderer.h"

namespace NSOnlineOfficeBinToPdf
{
	bool ConvertBinToPdf(CPdfRenderer* pPdf, const std::wstring& wsSrcFile, const std::wstring& wsDstFile, bool bBinary);		
};

#endif // _PDF_WRITER_ONLINEOFFICEBINTOPDF_H

