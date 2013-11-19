#ifndef _EXTRACT_IMAGE_OUTPUTDEV_H
#define _EXTRACT_IMAGE_OUTPUTDEV_H

#include <stdio.h>
#include "OutputDevice.h"

class GrState;

//-------------------------------------------------------------------------------------------------------------------------------
// ExtractImageOutputDev
//-------------------------------------------------------------------------------------------------------------------------------

class ExtractImageOutputDev: public OutputDev 
{
public:

	ExtractImageOutputDev(GlobalParams *pGlobalParams, char *sFilePrefix, BOOL bDumpJPEG, BOOL bCountImages = FALSE);

	virtual ~ExtractImageOutputDev();

	virtual BOOL CheckValidate() 
	{ 
		return m_bValid; 
	}

	virtual BOOL InterpretType3Chars() 
	{ 
		return FALSE; 
	}

	virtual BOOL NeedNonText() 
	{ 
		return TRUE; 
	}


	// Информация об устройстве.
	virtual BOOL UpSideDown() 
	{ 
		return FALSE; 
	}

	virtual BOOL UseDrawChar() 
	{ 
		return FALSE; 
	}

	virtual void DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, BOOL bInvert, BOOL bInlineImage);
	virtual void DrawImage    (GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pnMaskColors, BOOL bInlineImage);

	// 

	int ExtractImageOutputDev::GetImagesCount()
	{
		return m_nImageCount;
	}

private:

	char *m_sFilePrefix;  // 
	char *m_sFileName;    // 
	BOOL  m_bDumpJPEG;    // 
	int   m_nImageCount;  // Текущее количество изображений
	BOOL  m_bCountImages; // Считаем картинки

	BOOL  m_bValid;
};

#endif /* _EXTRACT_IMAGE_OUTPUTDEV_H */
