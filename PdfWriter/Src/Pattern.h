#ifndef _PDF_WRITER_SRC_PATTERN_H
#define _PDF_WRITER_SRC_PATTERN_H

#include "Objects.h"

namespace PdfWriter
{
	class CImageDict;
	//----------------------------------------------------------------------------------------
	// CPattern
	//----------------------------------------------------------------------------------------
	class CPattern : public CDictObject
	{
	public:
		CPattern(CXref* pXref);
	};
	//----------------------------------------------------------------------------------------
	// CImageTilePattern
	//----------------------------------------------------------------------------------------
	class CImageTilePattern : public CPattern
	{
	public:
		CImageTilePattern(CXref* pXref, const double& dW, const double& dH, CImageDict* pImageDict, CMatrix* pMatrix = NULL, EImageTilePatternType eType = imagetilepatterntype_Default);
	};
}

#endif // _PDF_WRITER_SRC_PATTERN_H