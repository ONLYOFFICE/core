#ifndef _SBITMAP_H
#define _SBITMAP_H

#include "STypes.h"

//-------------------------------------------------------------------------------------------------------------------------------
// SBitmap
//-------------------------------------------------------------------------------------------------------------------------------

class SBitmap 
{
public:

	SBitmap(int nWidth, int nHeight, int nRowPad, SColorMode eMode, BOOL bAlpha, BOOL bTopDown = TRUE);

	~SBitmap();

	int GetWidth() 
	{ 
		return m_nWidth; 
	}
	int GetHeight() 
	{ 
		return m_nHeight; 
	}
	int GetStride() 
	{ 
		return m_nStride; 
	}
	int GetAlphaStride() 
	{ 
		return m_nWidth; 
	}
	SColorMode GetColorMode() 
	{ 
		return m_eMode; 
	}
	SColorPointer GetData() 
	{ 
		return m_pData; 
	}
	unsigned char *GetAlpha() 
	{ 
		return m_pAlpha; 
	}

	int WritePNMFile(wchar_t *wsFileName);
	int WriteBMPFile(wchar_t *wsFileName);
	int WriteJPGFile(wchar_t *wsFileName, wchar_t *wsTempFolder);

	void GetPixel(int nX, int nY, SColorPointer pPixel);
	unsigned char GetAlpha(int nX, int nY);

private:

	int            m_nWidth;    // Ўирина картинки
	int            m_nHeight;   // ¬ысота картинки

	int            m_nStride;   //  оличество байт между началом одной строки и началом следующей, 
	                            // если значение отрицательное, значит картинка первернута€ (flip)
	SColorMode     m_eMode;     // Color mode
	SColorPointer  m_pData;     // ”казатель на начало данных картинки
	unsigned char *m_pAlpha;   	// ”казатель на начало данных альфа канала(не зависит от flip)

	friend class   SImage;
};

#endif /* _SBITMAP_H */
