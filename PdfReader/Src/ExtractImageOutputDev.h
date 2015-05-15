#ifndef _PDF_READER_EXTRACT_IMAGE_OUTPUTDEV_H
#define _PDF_READER_EXTRACT_IMAGE_OUTPUTDEV_H

#include <stdio.h>
#include "OutputDevice.h"

namespace PdfReader
{
	class GrState;

	//-------------------------------------------------------------------------------------------------------------------------------
	// ExtractImageOutputDev
	//-------------------------------------------------------------------------------------------------------------------------------

	class ExtractImageOutputDev : public OutputDev
	{
	public:

		ExtractImageOutputDev(GlobalParams *pGlobalParams, char *sFilePrefix, bool bDumpJPEG, bool bCountImages = false);

		virtual ~ExtractImageOutputDev();

		virtual bool CheckValidate()
		{
			return m_bValid;
		}

		virtual bool InterpretType3Chars()
		{
			return false;
		}

		virtual bool NeedNonText()
		{
			return true;
		}


		// Информация об устройстве.
		virtual bool UpSideDown()
		{
			return false;
		}

		virtual bool UseDrawChar()
		{
			return false;
		}

		virtual void DrawImageMask(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, bool bInvert, bool bInlineImage);
		virtual void DrawImage(GrState *pGState, Object *pRef, Stream *pStream, int nWidth, int nHeight, GrImageColorMap *pColorMap, int *pnMaskColors, bool bInlineImage);

		// 

		int GetImagesCount()
		{
			return m_nImageCount;
		}

	private:

		char *m_sFilePrefix;  // 
		char *m_sFileName;    // 
		bool  m_bDumpJPEG;    // 
		int   m_nImageCount;  // Текущее количество изображений
		bool  m_bCountImages; // Считаем картинки

		bool  m_bValid;
	};
}

#endif // _PDF_READER_EXTRACT_IMAGE_OUTPUTDEV_H
