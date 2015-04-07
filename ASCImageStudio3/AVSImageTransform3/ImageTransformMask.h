#pragma once
	
#include "resource.h"
#include <GdiPlusEx.h>

[object, uuid("92981BE7-106C-4f04-AB7E-912BBFC36710"), dual, pointer_default(unique)]
__interface IImageTransformMask3 : IDispatch
{
	[id(101)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(102)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(201)] HRESULT GetMask([out, satype("BYTE")] SAFEARRAY** Array, [out, retval] VARIANT_BOOL* Success);
	[id(202)] HRESULT GetMaskPtr([in] BYTE* Ptr, [out, retval] VARIANT_BOOL* Success);
	
	[id(301)] HRESULT CreateEmpty([in] long Width, [in] long Height);
	[id(302)] HRESULT CreateThreshold([in, satype("BYTE")] SAFEARRAY** Array, [in] BYTE LeftWhite, [in] BYTE RightWhite);
	[id(303)] HRESULT CreateColorKey([in, satype("BYTE")] SAFEARRAY** Array, [in] long Color, [in] long Threshold);
	[id(304)] HRESULT CreateColorKey2([in, satype("BYTE")] SAFEARRAY** Array, [in] long Color1, [in] long Color2);
	
	[id(401)] HRESULT DrawRectangle([in] double Left, [in] double Top, [in] double Right, [in] double Bottom);
	[id(402)] HRESULT DrawEllipse([in] double Left, [in] double Top, [in] double Right, [in] double Bottom);
	[id(403)] HRESULT DrawPolyline([out, satype("double")] SAFEARRAY** Array); // 1d array (xy)-(xy)-...
	[id(404)] HRESULT DrawPolygon([out, satype("double")] SAFEARRAY** Array);  // 1d array
	
	[id(501)] HRESULT EffectClear();
	[id(502)] HRESULT EffectInvert();
	[id(503)] HRESULT EffectDilate();
	[id(504)] HRESULT EffectErode();
	[id(505)] HRESULT EffectThreshold([in] long LeftWhite, [in] long RightWhite);
};
	
[coclass, uuid("E163201F-3DA3-41e6-BE29-3E024D2884BE"), threading(apartment), vi_progid("AVSImageTransform.ImageTransformMask3"), progid("AVSImageTransform.ImageTransformMask3.1"), version(1.0)]
class ATL_NO_VTABLE ImageTransformMask3 : public IImageTransformMask3
{
public:
	
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		/*
		CComVariant ParamVal = ParamValue;
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"ChromaThreshold1")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			m_nAdvancedChromaThreshold1 = ParamVal.lVal;
		}
		else if (ParamStr == L"ChromaThreshold2")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			m_nAdvancedChromaThreshold2 = ParamVal.lVal;
		}
		else if (ParamStr == L"CropZoom")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			m_nAdvancedCropZoom = ParamVal.lVal;
		}
		*/

		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		/*
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"ChromaThreshold1")
			(*ParamValue).lVal = (LONG)m_nAdvancedChromaThreshold1;
		else if (ParamStr == L"ChromaThreshold2")
			(*ParamValue).lVal = (LONG)m_nAdvancedChromaThreshold2;
		else if (ParamStr == L"CropZoom")
			(*ParamValue).lVal = (LONG)m_nAdvancedCropZoom;
		*/
		return S_OK;
	}
	
	STDMETHOD(GetMask)(SAFEARRAY** Array, VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;

		if (!Array || !Valid())
			return S_OK;

		SAFEARRAYBOUND rgsaBound[3];
			rgsaBound[0].lLbound = 0;
			rgsaBound[0].cElements = 4;
			rgsaBound[1].lLbound = 0;
			rgsaBound[1].cElements = m_nWidth;
			rgsaBound[2].lLbound = 0;
			rgsaBound[2].cElements = m_nHeight;

		*Array = SafeArrayCreate(VT_UI1, 3, rgsaBound);

		if (!(*Array))
			return S_OK;

		CopyFlip((BYTE*)(*Array)->pvData, TRUE);

		*Success = VARIANT_TRUE;

		return S_OK;
	}
	STDMETHOD(GetMaskPtr)(BYTE* Ptr, VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;

		if (!Ptr || !Valid())
			return S_OK;

		SAFEARRAY* Array = (SAFEARRAY*)Ptr;

		if (Array->rgsabound[0].cElements != m_nHeight || Array->rgsabound[1].cElements != m_nWidth)
			return S_OK;

		CopyFlip((BYTE*)(Array->pvData), TRUE);

		*Success = VARIANT_TRUE;

		return S_OK;
	}
	
	STDMETHOD(CreateEmpty)(long Width, long Height)
	{
		Destroy();

		m_nWidth = Width;
		m_nHeight = Height;
		m_pMask = new BYTE[4*Width*Height];

		ZeroMemory(m_pMask, 4*Width*Height);

		m_pBitmap = new Bitmap(m_nWidth, m_nHeight, m_nWidth*4, PixelFormat32bppRGB, m_pMask);

		return S_OK;
	}
	STDMETHOD(CreateThreshold)(SAFEARRAY** Array, BYTE LeftWhite, BYTE RightWhite)
	{
		if (!Array)
			return S_OK;

		if (!CreateFromImage(*Array))
		{
			Destroy();
			return S_OK;
		}

		BYTE* pMap = new BYTE[256]; ZeroMemory(pMap, 256);
		int nIndex, nCount = 4*m_nWidth*m_nHeight;
		BYTE* pPtr = m_pMask;
		BYTE* pSource = m_pImage;
		int nValue;

		for (nIndex = LeftWhite; nIndex <= RightWhite; ++nIndex)
			pMap[nIndex] = 255;

		for (nIndex = 0; nIndex < nCount; ++nIndex, ++pPtr, pSource += 4)
		{
			nValue = Intensity(pSource[2], pSource[1], pSource[0]);
			*pPtr = pMap[nValue];
		}

		delete[] pMap;

		return S_OK;
	}
	STDMETHOD(CreateColorKey)(SAFEARRAY** Array, long Color, long Threshold)
	{
		// TODO:
		return S_OK;
	}
	STDMETHOD(CreateColorKey2)(SAFEARRAY** Array, long Color1, long Color2)
	{
		// TODO:
		return S_OK;
	}
	
	STDMETHOD(DrawRectangle)(double Left, double Top, double Right, double Bottom)
	{
		if (!Valid())
			return S_OK;

		Graphics graphics(m_pBitmap);
		SolidBrush brush(Color::White);

		REAL rLeft = (REAL)min(Left, Right);
		REAL rTop = (REAL)min(Top, Bottom);
		REAL rWidth = (REAL)fabs(Left - Right);
		REAL rHeight = (REAL)fabs(Top - Bottom);

		graphics.FillRectangle(&brush, rLeft, rTop, rWidth, rHeight);

		return S_OK;
	}
	STDMETHOD(DrawEllipse)(double Left, double Top, double Right, double Bottom)
	{
		if (!Valid())
			return S_OK;

		Graphics graphics(m_pBitmap);
		SolidBrush brush(Color::White);

		REAL rLeft = (REAL)min(Left, Right);
		REAL rTop = (REAL)min(Top, Bottom);
		REAL rWidth = (REAL)fabs(Left - Right);
		REAL rHeight = (REAL)fabs(Top - Bottom);

		graphics.FillEllipse(&brush, rLeft, rTop, rWidth, rHeight);

		return S_OK;
	}
	STDMETHOD(DrawPolyline)(SAFEARRAY** Array)
	{
		if (!Valid() || !Array || !*Array)
			return S_OK;

		int nCount = (*Array)->rgsabound[0].cElements;

		if (nCount % 2 || nCount < 4)
			return S_OK;

		nCount /= 2;

		PointF* pPoints = new PointF[nCount];
		double* pDoubles = (double*)((*Array)->pvData);
		
		for (int nIndex = 0; nIndex < nCount; ++nIndex, pDoubles += 2)
		{
			pPoints[nIndex].X = pDoubles[0];
			pPoints[nIndex].Y = pDoubles[1];
		}

		Graphics graphics(m_pBitmap);
		Pen pen(Color::White, 1.0);

		graphics.DrawLines(&pen, pPoints, nCount);
		
		delete[] pPoints;

		return S_OK;
	}
	STDMETHOD(DrawPolygon)(SAFEARRAY** Array)
	{
		if (!Valid() || !Array || !*Array)
			return S_OK;

		int nCount = (*Array)->rgsabound[0].cElements;

		if (nCount % 2 || nCount < 6)
			return S_OK;

		nCount /= 2;

		PointF* pPoints = new PointF[nCount];
		double* pDoubles = (double*)((*Array)->pvData);
		
		for (int nIndex = 0; nIndex < nCount; ++nIndex, pDoubles += 2)
		{
			pPoints[nIndex].X = pDoubles[0];
			pPoints[nIndex].Y = pDoubles[1];
		}

		Graphics graphics(m_pBitmap);
		SolidBrush brush(Color::White);

		graphics.FillPolygon(&brush, pPoints, nCount);

		delete[] pPoints;

		return S_OK;
	}
	
	STDMETHOD(EffectClear)()
	{
		if (!Valid())
			return S_OK;

		ZeroMemory(m_pMask, 4*m_nWidth*m_nHeight);

		return S_OK;
	}
	STDMETHOD(EffectInvert)()
	{
		if (!Valid())
			return S_OK;

		int nIndex, nCount = 4*m_nWidth*m_nHeight;
		BYTE* pPtr = m_pMask;

		for (nIndex = 0; nIndex < nCount; ++nIndex, ++pPtr)
			*pPtr = (255 - *pPtr);

		return S_OK;
	}
	STDMETHOD(EffectDilate)()
	{
		if (!Valid())
			return S_OK;

		int nX, nY;
		int nIndex1 = 0;
		int nIndex2 = 0;
		int nLineSize = 4*m_nWidth;
		BYTE* pCopy = new BYTE[m_nWidth*m_nHeight];
		
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			for (nX = 0; nX < m_nWidth; ++nX, ++nIndex1, nIndex2 += 4)
			{
				pCopy[nIndex1] = m_pMask[nIndex2];

				if (m_pMask[nIndex2])
					continue;

				if (nY - 1 >= 0				&&	m_pMask[nIndex2 - nLineSize])		pCopy[nIndex1] = 255;
				if (nX - 1 >= 0				&&	m_pMask[nIndex2 - 4])				pCopy[nIndex1] = 255;
				if (nX + 1 < m_nWidth		&&	m_pMask[nIndex2 + 4])				pCopy[nIndex1] = 255;
				if (nY + 1 < m_nHeight		&&	m_pMask[nIndex2 + nLineSize])		pCopy[nIndex1] = 255;
			}
		}

		for (nIndex1 = 0, nIndex2 = 0; nIndex1 < m_nWidth*m_nHeight; ++nIndex1, nIndex2 += 4)
		{
			m_pMask[nIndex2 + 0] = pCopy[nIndex1];
			m_pMask[nIndex2 + 1] = pCopy[nIndex1];
			m_pMask[nIndex2 + 2] = pCopy[nIndex1];
		}

		delete[] pCopy;

		return S_OK;
	}
	STDMETHOD(EffectErode)()
	{
		if (!Valid())
			return S_OK;

		int nX, nY;
		int nIndex1 = 0;
		int nIndex2 = 0;
		int nLineSize = 4*m_nWidth;
		BYTE* pCopy = new BYTE[m_nWidth*m_nHeight];
		
		for (nY = 0; nY < m_nHeight; ++nY)
		{
			for (nX = 0; nX < m_nWidth; ++nX, ++nIndex1, nIndex2 += 4)
			{
				pCopy[nIndex1] = m_pMask[nIndex2];

				if (!m_pMask[nIndex2])
					continue;

				if (nY - 1 >= 0				&&	!m_pMask[nIndex2 - nLineSize])		pCopy[nIndex1] = 0;
				if (nX - 1 >= 0				&&	!m_pMask[nIndex2 - 4])				pCopy[nIndex1] = 0;
				if (nX + 1 < m_nWidth		&&	!m_pMask[nIndex2 + 4])				pCopy[nIndex1] = 0;
				if (nY + 1 < m_nHeight		&&	!m_pMask[nIndex2 + nLineSize])		pCopy[nIndex1] = 0;
			}
		}

		for (nIndex1 = 0, nIndex2 = 0; nIndex1 < m_nWidth*m_nHeight; ++nIndex1, nIndex2 += 4)
		{
			m_pMask[nIndex2 + 0] = pCopy[nIndex1];
			m_pMask[nIndex2 + 1] = pCopy[nIndex1];
			m_pMask[nIndex2 + 2] = pCopy[nIndex1];
		}

		delete[] pCopy;

		return S_OK;
	}
	STDMETHOD(EffectThreshold)(long LeftWhite, long RightWhite)
	{
		if (!Valid())
			return S_OK;

		BYTE* pMap = new BYTE[256]; ZeroMemory(pMap, 256);
		int nIndex, nCount = 4*m_nWidth*m_nHeight;
		BYTE* pPtr = m_pMask;

		for (nIndex = LeftWhite; nIndex <= RightWhite; ++nIndex)
			pMap[nIndex] = 255;

		for (nIndex = 0; nIndex < nCount; ++nIndex, ++pPtr)
			*pPtr = pMap[*pPtr];

		delete[] pMap;

		return S_OK;
	}

		
public:
	
	int m_nWidth;
	int m_nHeight;
	BYTE* m_pMask;
	BYTE* m_pImage;
	Bitmap* m_pBitmap;
	
	CGdiPlusInit m_oGdiPlus;
	
public:
	
	ImageTransformMask3()
	{
		m_nWidth = 0;
		m_nHeight = 0;
		m_pMask = 0;
		m_pImage = 0;
		m_pBitmap = 0;

		m_oGdiPlus.Init();
	}
	~ImageTransformMask3()
	{
		Destroy();
	}
	
	void Destroy()
	{
		if (m_pBitmap)
			delete m_pBitmap;
		if (m_pMask)
			delete[] m_pMask;

		m_pMask = 0;
		m_pBitmap = 0;
		m_nWidth = 0;
		m_nHeight = 0;
	}
	BOOL Valid()
	{
		if (!m_pMask || !m_pBitmap || m_nWidth < 1 || m_nHeight < 1)
			return FALSE;

		return TRUE;
	}
	BOOL CreateFromImage(SAFEARRAY* Array)
	{
		if (!Array)
			return FALSE;

		Destroy();

		CreateEmpty(Array->rgsabound[1].cElements, Array->rgsabound[0].cElements);

		m_pImage = (BYTE*)(Array->pvData);

		if (!m_pImage)
			return FALSE;

		return TRUE;
	}
	inline BYTE Lightness(double r, double g, double b)
	{
		return (BYTE)(0.5*(max(r, max(g, b)) + min(r, min(g, b))));
	}
	inline BYTE Intensity(double r, double g, double b)
	{
		return (BYTE)(0.3*r + 0.59*g + 0.11*b);
	}
	void CopyFlip(BYTE* pDst, BOOL Flip)
	{
		if (!Flip)
		{
			memcpy(pDst, m_pMask, 4*m_nWidth*m_nHeight);
			return;
		}

		int nLineSize = 4*m_nWidth;
		BYTE* pS = m_pMask;
		BYTE* pD = pDst + nLineSize*(m_nHeight - 1);

		for (int y = 0; y < m_nHeight; ++y, pS += nLineSize, pD -= nLineSize)
			memcpy(pD, pS, nLineSize);
	}
};

