#ifndef _BUILD_IMAGE_H_
#define _BUILD_IMAGE_H_

#include "AggPlusEnums.h"
#include <string>
#include "IRenderer.h"

namespace Aggplus
{

class CImage : public IGrObject
{
public:
	friend class CGraphics;
	friend class CBrushTexture;

public:
	CImage();
	CImage(const std::wstring& filename);
	virtual ~CImage();

	DWORD GetWidth() const;
	DWORD GetHeight() const;
	long GetStride() const;
	BYTE* GetData() const;
	 
	Status GetLastStatus() const;

	void Create(const std::wstring& filename);
	void Create(BYTE* pImgData, const DWORD& dwWidth, const DWORD& dwHeight, const long& nStride);
	bool SaveFile(const std::wstring& strFileName, unsigned __int32 nFileType);
	void Destroy();

public:	
	Status m_Status;
	
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	long m_nStride;

	BYTE* m_pImgData;
	bool m_bExternalBuffer;
};

//---------------------------------------------------------------------------
// Information about image pixel data
//---------------------------------------------------------------------------

class CBitmapData
{
public:
	UINT Width;
	UINT Height;
	LONG Stride;
    Aggplus::PixelFormat PixelFormat;
	void* Scan0;
	UINT* Reserved;
};

class CBitmap : public CImage
{
public:
	CBitmap(LONG width, LONG height, PixelFormat format = PixelFormat32bppARGB);	
	CBitmap(LONG width, LONG height, LONG stride, PixelFormat format, BYTE* scan0);	 
	CBitmap(const std::wstring& filename);
	
	virtual ~CBitmap();

	void LockBits(const RectF* rect, PixelFormat format, CBitmapData* lockedBitmapData);
};

}

#endif // _BUILD_IMAGE_H_