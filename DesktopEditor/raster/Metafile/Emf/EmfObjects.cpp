#include "EmfObjects.h"
#include "../../../raster/ImageFileFormatChecker.h"
#include "../../../graphics/Image.h"
#include "../Common.h"

namespace MetaFile
{
	CEmfLogBrushEx::CEmfLogBrushEx() : Color(255, 255, 255)
	{
		BrushStyle     = BS_SOLID;
		BrushHatch     = HS_HORIZONTAL;
		DibPatternPath = L"";
		BrushAlpha     = 255;
		DibBuffer      = NULL;
		DibWidth       = 0;
		DibHeigth      = 0;
	}
	CEmfLogBrushEx::~CEmfLogBrushEx()
	{
		if (BS_DIBPATTERN == BrushStyle && L"" != DibPatternPath)
        {
            NSFile::CFileBinary::Remove(DibPatternPath);
        }

		if (DibBuffer)
			delete[] DibBuffer;
	}
	void CEmfLogBrushEx::SetDibPattern(unsigned char* pBuffer, unsigned int ulWidth, unsigned int ulHeight)
	{
		DibBuffer = pBuffer;
		DibWidth  = ulWidth;
		DibHeigth = ulHeight;		

		if (ulWidth <= 0 || ulHeight <= 0)
			return;

		unsigned int ulBufferSize = 4 * ulWidth * ulHeight;
		Aggplus::CImage oImage;
		BYTE* pBufferPtr = new BYTE[ulBufferSize];
		oImage.Create(pBufferPtr, ulWidth, ulHeight, 4 * ulWidth);

		// ѕишем данные в pBufferPtr
		for (unsigned int ulIndex = 0; ulIndex < ulBufferSize; ulIndex += 4)
		{
			pBufferPtr[0] = (unsigned char)pBuffer[ulIndex + 0];
			pBufferPtr[1] = (unsigned char)pBuffer[ulIndex + 1];
			pBufferPtr[2] = (unsigned char)pBuffer[ulIndex + 2];
			pBufferPtr[3] = (unsigned char)pBuffer[ulIndex + 3];
			pBufferPtr += 4;
		}

		FILE *pTempFile = NULL;
		std::wstring wsTempFileName;
		if (!WmfOpenTempFile(&wsTempFileName, &pTempFile, L"wb", L".emf0", NULL))
			return;

		::fclose(pTempFile);

		oImage.SaveFile(wsTempFileName, _CXIMAGE_FORMAT_PNG);

		BrushStyle     = BS_DIBPATTERN;
		DibPatternPath = wsTempFileName;
	}
	int  CEmfLogBrushEx::GetColor()
	{
		return METAFILE_RGBA(Color.r, Color.g, Color.b);
	}
	int  CEmfLogPen::GetColor()
	{
		return METAFILE_RGBA(Color.r, Color.g, Color.b);
	}
}
