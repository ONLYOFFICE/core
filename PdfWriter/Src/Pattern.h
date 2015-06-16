#ifndef _PDF_WRITER_SRC_PATTERN_H
#define _PDF_WRITER_SRC_PATTERN_H

#include "Objects.h"

namespace PdfWriter
{
	class CImageDict;
	//----------------------------------------------------------------------------------------
	// CImageTilePattern
	//----------------------------------------------------------------------------------------
	class CImageTilePattern : public CDictObject
	{
	public:
		CImageTilePattern(CXref* pXref, double dW, double dH, CImageDict* pImageDict, EImageTilePatternType eType = imagetilepatterntype_Default);
	};
}

#endif // _PDF_WRITER_SRC_PATTERN_H