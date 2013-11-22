// AVSDVDSubpictureFrame.h : Declaration of the CAVSDVDSubpictureFrame

#pragma once
#include "resource.h"       // main symbols
#include "AVSMediaData.h"
#include "MemoryUtils.h"
#include <atlcoll.h>

// IAVSDVDSubpictureFrame
[object, uuid("BEE83C44-5BE4-4545-9657-6A47184ACB4F"), dual, pointer_default(unique)]
__interface IAVSDVDSubpictureFrame : IAVSMediaData
{
	[id(207000 + 1), propget] HRESULT Width([out, retval] long* pVal);
	[id(207000 + 1), propput] HRESULT Width([in] long newVal);
	[id(207000 + 2), propget] HRESULT Height([out, retval] long* pVal);
	[id(207000 + 2), propput] HRESULT Height([in] long newVal);

// Delay задержка от времени предыдущей Mark (для первой добавляемой от TimeStamp) 
// Color массив 4x4 YUV цветов с альфа 
//		SAFEARRAYBOUND rgsa[2];
//		rgsa[0].cElements = 4;
//		rgsa[0].lLbound = 0;
//		rgsa[1].cElements = 4;
//		rgsa[1].lLbound = 0;
//		Color = SafeArrayCreate(VT_I1,2,rgsa);
//		LPBYTE pColor = Color->pvData;
//		pColor[0]  = Y1; pColor[1]  = U1; pColor[2]  = V1; pColor[3]  = A1;
//		pColor[4]  = Y2; pColor[5]  = U2; pColor[6]  = V2; pColor[7]  = A2;
//		pColor[8]  = Y3; pColor[9]  = U3; pColor[10] = V3; pColor[11] = A3;
//		pColor[12] = Y4; pColor[13] = U4; pColor[14] = V4; pColor[15] = A4;
// если Color == NULL, то надо взять массив с предыдущей Mark
// если Left или Top отрицательные, то значения беруться с предыдущей Mark
	[id(207000 + 10)] HRESULT AddMark([in] double Delay, [in, satype("byte")] SAFEARRAY *Color, [in] long Left, [in] long Top);
//	окончание показа субтитров, Delay добавляется к последнему Mark 
//  теперь до следующего SetMark ничего накладываться не будет
	[id(207000 + 11)] HRESULT AddStopMark([in] double Delay);
	
	[id(207000 + 15), propget] HRESULT MarkCount([out, retval] long *pCount);
	[id(207000 + 16), propget] HRESULT Mark([in]long Index, [out] VARIANT_BOOL *pStopMark, [out] double *pDelay, [out, satype("byte")] SAFEARRAY **pColor, [out] long *pLeft, [out] long *pTop);

	[id(207000 + 20), propget] HRESULT HighlightEnable([out, retval] VARIANT_BOOL* pVal);
	[id(207000 + 20), propput] HRESULT HighlightEnable([in] VARIANT_BOOL newVal);
	[id(207000 + 21), propget] HRESULT HighlightLeft([out, retval] long* pVal);
	[id(207000 + 21), propput] HRESULT HighlightLeft([in] long newVal);
	[id(207000 + 22), propget] HRESULT HighlightTop([out, retval] long* pVal);
	[id(207000 + 22), propput] HRESULT HighlightTop([in] long newVal);
	[id(207000 + 23), propget] HRESULT HighlightWidth([out, retval] long* pVal);
	[id(207000 + 23), propput] HRESULT HighlightWidth([in] long newVal);
	[id(207000 + 24), propget] HRESULT HighlightHeight([out, retval] long* pVal);
	[id(207000 + 24), propput] HRESULT HighlightHeight([in] long newVal);

	//Index меняется от 0 до 4, цвет задается в YUV  c альфа каналом A 
	[id(207000 + 30), propget] HRESULT HighlightColorY([in] long Index, [out, retval] BYTE* pVal);
	[id(207000 + 30), propput] HRESULT HighlightColorY([in] long Index, [in] BYTE newVal);
	[id(207000 + 31), propget] HRESULT HighlightColorU([in] long Index, [out, retval] BYTE* pVal);
	[id(207000 + 31), propput] HRESULT HighlightColorU([in] long Index, [in] BYTE newVal);
	[id(207000 + 32), propget] HRESULT HighlightColorV([in] long Index, [out, retval] BYTE* pVal);
	[id(207000 + 32), propput] HRESULT HighlightColorV([in] long Index, [in] BYTE newVal);
	[id(207000 + 33), propget] HRESULT HighlightAlpha([in] long Index, [out, retval] BYTE* pVal);
	[id(207000 + 33), propput] HRESULT HighlightAlpha([in] long Index, [in] BYTE newVal);

	[id(207000 + 50)] HRESULT Draw([in] IAVSUncompressedVideoFrame *pFrame);
};


template <class CoT, class T>
class CAVSDVDSubpictureFrameImpl : public CAVSMediaDataImpl<CoT, T>
{
public:
	CAVSDVDSubpictureFrameImpl()
		: m_bHighlight(FALSE)
#ifdef BOUND_CHECK_ENABLE
		, m_bBound(FALSE)
		, m_lBoundTop(0)
		, m_lBoundBottom(0)
		, m_lBoundPaletteMarkIndex(-1)		
#endif
	{
		m_lMediaType = MEDIAFORMAT_DVD_SUBPICTURE;
	}
	STDMETHOD(CreateDuplicate)(long DublicateType, IAVSMediaData** pMediaData)
	{
		CAVSMediaDataImpl<CoT, T>::CreateDuplicate(DublicateType, pMediaData);
		if (NULL == *pMediaData)
			return S_OK;

		IAVSDVDSubpictureFrame* pAVSSubPicture = NULL;
		(*pMediaData)->QueryInterface(__uuidof(IAVSDVDSubpictureFrame), (void**)(&pAVSSubPicture));
		if (NULL == pAVSSubPicture)
		{
			(*pMediaData)->Release();
			*pMediaData = NULL;
			return S_OK;
		}

		pAVSSubPicture->put_Width(m_lWidth);
		pAVSSubPicture->put_Height(m_lHeight);

		long lMarkCount = (long)m_arMark.GetCount();
		for (long lMarkIndex = 0; lMarkIndex<lMarkCount; lMarkIndex++)
		{
			if (m_arMark[lMarkIndex].m_bStopMark)
				pAVSSubPicture->AddStopMark(m_arMark[lMarkIndex].m_dDelay);
			else
			{
				pAVSSubPicture->AddMark(m_arMark[lMarkIndex].m_dDelay, m_arMark[lMarkIndex].m_psaColor, m_arMark[lMarkIndex].m_lLeft, m_arMark[lMarkIndex].m_lTop);
			}
		}
		pAVSSubPicture->put_HighlightEnable((m_bHighlight ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSSubPicture->put_HighlightLeft(m_oHighlightRect.m_lLeft);
		pAVSSubPicture->put_HighlightTop(m_oHighlightRect.m_lTop);
		pAVSSubPicture->put_HighlightWidth(m_oHighlightRect.m_lWidth);
		pAVSSubPicture->put_HighlightHeight(m_oHighlightRect.m_lHeight);

		for (int i=0;i<DVD_SUBPICTURE_COLOR_COUNT;i++)
		{
			pAVSSubPicture->put_HighlightColorY(i, m_oHighlightPalette.m_arColorY[i]);
			pAVSSubPicture->put_HighlightColorU(i, m_oHighlightPalette.m_arColorU[i]);
			pAVSSubPicture->put_HighlightColorV(i, m_oHighlightPalette.m_arColorV[i]);
			pAVSSubPicture->put_HighlightAlpha(i, m_oHighlightPalette.m_arColorA[i]);
		}

		RELEASEINTERFACE(pAVSSubPicture)
		return S_OK;
	}

	STDMETHOD(get_Width)(long* pVal)
	{
		*pVal = m_lWidth;
		return S_OK;
	}
	STDMETHOD(put_Width)(long newVal)
	{
		m_lWidth = newVal;
		return S_OK;
	}
	STDMETHOD(get_Height)(long* pVal)
	{
		*pVal = m_lHeight;
		return S_OK;
	}
	STDMETHOD(put_Height)(long newVal)
	{
		m_lHeight = newVal;		
		return S_OK;
	}
	STDMETHOD(AddMark)(double Delay, SAFEARRAY *Color, long Left, long Top)
	{
		m_bDurationExist = VARIANT_TRUE;
		m_dDuration = Delay;
		m_arMark.Add(SMark(Color, Delay, Left, Top));
		return S_OK;
	}
	STDMETHOD(AddStopMark)(double Delay)
	{
		m_bDurationExist = VARIANT_TRUE;
		m_dDuration = Delay;
		m_arMark.Add(SMark(Delay));
		return S_OK;
	}
	
	STDMETHOD(get_MarkCount)(long *pCount)
	{
		*pCount = (long)m_arMark.GetCount();
		return S_OK;
	}
	STDMETHOD(get_Mark)(long Index, VARIANT_BOOL *pStopMark, double *pDelay, SAFEARRAY **pColor, long *pLeft, long *pTop)
	{
		if ((Index<0)||(Index>(long)m_arMark.GetCount()))
			return S_FALSE;

		const SMark &oMark = m_arMark[Index];
		*pStopMark = oMark.m_bStopMark;
		*pDelay = oMark.m_dDelay;
		*pLeft = oMark.m_lLeft;
		*pTop = oMark.m_lTop;
		if (NULL!=oMark.m_psaColor)
			SafeArrayCopy(oMark.m_psaColor, pColor);
		
		return S_OK;
	}
//
	STDMETHOD(get_HighlightEnable)(VARIANT_BOOL* pVal)
	{
		*pVal = m_bHighlight ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_HighlightEnable)(VARIANT_BOOL newVal)
	{
		m_bHighlight = (VARIANT_FALSE!=newVal);
#ifdef BOUND_CHECK_ENABLE
		m_bBound = FALSE;
#endif
		return S_OK;
	}
	STDMETHOD(get_HighlightLeft)(long* pVal)
	{
		*pVal = m_oHighlightRect.m_lLeft;
		return S_OK;
	}
	STDMETHOD(put_HighlightLeft)(long newVal)
	{
		m_oHighlightRect.m_lLeft = newVal;
		return S_OK;
	}
	STDMETHOD(get_HighlightTop)(long* pVal)
	{
		*pVal = m_oHighlightRect.m_lTop;
		return S_OK;
	}
	STDMETHOD(put_HighlightTop)(long newVal)
	{
		m_oHighlightRect.m_lTop = newVal;
#ifdef BOUND_CHECK_ENABLE
		CheckHighlightOnChangeBound();
#endif
		return S_OK;
	}
	STDMETHOD(get_HighlightWidth)(long* pVal)
	{
		*pVal = m_oHighlightRect.m_lWidth;
		return S_OK;
	}
	STDMETHOD(put_HighlightWidth)(long newVal)
	{
		m_oHighlightRect.m_lWidth = newVal;
		return S_OK;
	}
	STDMETHOD(get_HighlightHeight)(long* pVal)
	{
		*pVal = m_oHighlightRect.m_lHeight;
		return S_OK;
	}
	STDMETHOD(put_HighlightHeight)(long newVal)
	{
		m_oHighlightRect.m_lHeight = newVal;
#ifdef BOUND_CHECK_ENABLE
		CheckHighlightOnChangeBound();
#endif
		return S_OK;
	}
	STDMETHOD(get_HighlightColorY)(long Index, BYTE* pVal)
	{
		*pVal = 0;
		if ((Index<0)||(Index>=DVD_SUBPICTURE_COLOR_COUNT))
			return S_OK;
		*pVal = m_oHighlightPalette.m_arColorY[Index];
		return S_OK;
	}
	STDMETHOD(put_HighlightColorY)(long Index, BYTE newVal)
	{
		if ((Index<0)||(Index>=DVD_SUBPICTURE_COLOR_COUNT))
			return S_OK;
		m_oHighlightPalette.m_arColorY[Index] = newVal;
		return S_OK;
	}
	STDMETHOD(get_HighlightColorU)(long Index, BYTE* pVal)
	{
		*pVal = 0;
		if ((Index<0)||(Index>=DVD_SUBPICTURE_COLOR_COUNT))
			return S_OK;
		*pVal = m_oHighlightPalette.m_arColorU[Index];
		return S_OK;
	}
	STDMETHOD(put_HighlightColorU)(long Index, BYTE newVal)
	{
		if ((Index<0)||(Index>=DVD_SUBPICTURE_COLOR_COUNT))
			return S_OK;
		m_oHighlightPalette.m_arColorU[Index] = newVal;
		return S_OK;
	}
	STDMETHOD(get_HighlightColorV)(long Index, BYTE* pVal)
	{
		*pVal = 0;
		if ((Index<0)||(Index>=DVD_SUBPICTURE_COLOR_COUNT))
			return S_OK;
		*pVal = m_oHighlightPalette.m_arColorV[Index];
		return S_OK;
	}
	STDMETHOD(put_HighlightColorV)(long Index, BYTE newVal)
	{
		if ((Index<0)||(Index>=DVD_SUBPICTURE_COLOR_COUNT))
			return S_OK;
		m_oHighlightPalette.m_arColorV[Index] = newVal;
		return S_OK;
	}
	STDMETHOD(get_HighlightAlpha)(long Index, BYTE* pVal)
	{
		*pVal = 0;
		if ((Index<0)||(Index>=DVD_SUBPICTURE_COLOR_COUNT))
			return S_OK;
		*pVal = m_oHighlightPalette.m_arColorA[Index];
		return S_OK;
	}
	STDMETHOD(put_HighlightAlpha)(long Index, BYTE newVal)
	{
		if ((Index<0)||(Index>=DVD_SUBPICTURE_COLOR_COUNT))
			return S_OK;

#ifdef BOUND_CHECK_ENABLE
		m_bBound = FALSE;
#endif
		m_oHighlightPalette.m_arColorA[Index] = max(min(newVal,15),0);
		m_oHighlightPalette.m_arColorAInv[Index] = 15 - m_oHighlightPalette.m_arColorA[Index];
		return S_OK;
	}
//
	STDMETHOD(Draw)(IAVSUncompressedVideoFrame *pFrame)
	{		
		long lLeft = 0;
		long lTop = 0;
		long lPaletteMarkIndex = 0;
		if (!GetFrameMarkSetting(pFrame, lLeft, lTop, lPaletteMarkIndex))
			return S_OK;
		SPalette oCurrentPalette; oCurrentPalette = m_arMark[lPaletteMarkIndex].m_psaColor;		
		
#ifdef BOUND_CHECK_ENABLE
		if (lPaletteMarkIndex!=m_lBoundPaletteMarkIndex)
			m_bBound = FALSE;
#endif

		SYUVFrame oFrame;
		if (!GetFrameSetting(pFrame, oFrame))
			return S_OK;

#ifdef BOUND_CHECK_ENABLE
		CalculateBound(oCurrentPalette);
		m_lBoundPaletteMarkIndex = lPaletteMarkIndex;
		if ((m_bBound)&&(m_lBoundBottom <= m_lBoundTop))
			return S_OK;
#endif

		if ((!m_bHighlight)||(0==m_oHighlightRect.m_lWidth)||(0==m_oHighlightRect.m_lHeight))
		{
			DrawNoHightlight(oFrame, lLeft, lTop, oCurrentPalette);
			return S_OK;
		}
		SRect oHighlightRect = m_oHighlightRect;
		oHighlightRect.m_lLeft -= lLeft;
		oHighlightRect.m_lTop -= lTop;

		oHighlightRect.Normalize();
		oHighlightRect.Align();
		if (oHighlightRect.m_lLeft<0)
		{
			oHighlightRect.m_lWidth += oHighlightRect.m_lLeft;
			oHighlightRect.m_lLeft = 0;
		}
		if (oHighlightRect.m_lTop<0)
		{
			oHighlightRect.m_lHeight += oHighlightRect.m_lTop;
			oHighlightRect.m_lTop = 0;
		}
		if ((0==oHighlightRect.m_lWidth)||(0==oHighlightRect.m_lHeight))
		{
			DrawNoHightlight(oFrame, lLeft, lTop, oCurrentPalette);
			return S_OK;
		}
		SRect oGeneralRect(lLeft, lTop, m_lWidth, m_lHeight);
		oGeneralRect.Normalize();
		oGeneralRect.Align();

		if ((oGeneralRect.m_lWidth<=oHighlightRect.m_lLeft)&&(oGeneralRect.m_lHeight<=oHighlightRect.m_lTop))
		{
			DrawNoHightlight(oFrame, lLeft, lTop, oCurrentPalette);
			return S_OK;
		}
		if (oHighlightRect.GetRight()>=oGeneralRect.m_lWidth)
			oHighlightRect.m_lWidth = oGeneralRect.m_lWidth - oHighlightRect.m_lLeft;
		if (oHighlightRect.GetBottom()>=oGeneralRect.m_lHeight)
			oHighlightRect.m_lHeight = oGeneralRect.m_lHeight - oHighlightRect.m_lTop;

		if ((oGeneralRect.GetRight() <= oHighlightRect.m_lLeft) && (oGeneralRect.GetBottom()<=oHighlightRect.m_lTop))
		{
			DrawNoHightlight(oFrame, lLeft, lTop, oCurrentPalette);
			return S_OK;
		}

		
		LPBYTE pYTable = NULL;
		LPBYTE pUTable = NULL;
		LPBYTE pVTable = NULL;
		long lSPStride = abs(m_lWidth);	
		if (oHighlightRect.m_lLeft>0)
		{
			if (!CreateColorTransformTable(oCurrentPalette, pYTable, pUTable, pVTable))
				return S_OK;
			SRect oTemp = oGeneralRect;
			oTemp.m_lWidth = oHighlightRect.m_lLeft;
			LPBYTE pSP = m_pMediaBuffer->GetBuffer();
#ifdef BOUND_CHECK_ENABLE
			if (m_bBound)
			{
				oTemp.m_lTop += m_lBoundTop;
				pSP += m_lBoundTop*lSPStride;
				oTemp.m_lHeight = m_lBoundBottom - m_lBoundTop + 1;
			}
#endif
			Draw(oFrame, pSP, oTemp, pYTable, pUTable, pVTable);
		}
		if (oHighlightRect.GetRight()+1<m_lWidth)
		{
			if ((NULL==pYTable)||(NULL==pUTable)||(NULL==pVTable))
			{
				DeleteTransformTable(pYTable, pUTable, pVTable);
				if (!CreateColorTransformTable(oCurrentPalette, pYTable, pUTable, pVTable))
					return S_OK;
			}
			SRect oTemp = oGeneralRect;
			oTemp.m_lLeft += oHighlightRect.GetRight()+1;
			oTemp.m_lWidth = oGeneralRect.m_lWidth - (oHighlightRect.GetRight()+1);
			LPBYTE pSP = m_pMediaBuffer->GetBuffer();
			pSP += oHighlightRect.GetRight()+1;
#ifdef BOUND_CHECK_ENABLE
			if (m_bBound)
			{
				oTemp.m_lTop += m_lBoundTop;
				pSP += m_lBoundTop*lSPStride;
				oTemp.m_lHeight = m_lBoundBottom - m_lBoundTop + 1;
			}
#endif
			Draw(oFrame, pSP, oTemp, pYTable, pUTable, pVTable);
		}
#ifdef BOUND_CHECK_ENABLE
		if ((oHighlightRect.m_lTop>0)&&
			((!m_bBound)||(oHighlightRect.m_lTop > m_lBoundTop)))
#else
		if (oHighlightRect.m_lTop>0)
#endif
		{
			if ((NULL==pYTable)||(NULL==pUTable)||(NULL==pVTable))
			{
				DeleteTransformTable(pYTable, pUTable, pVTable);
				if (!CreateColorTransformTable(oCurrentPalette, pYTable, pUTable, pVTable))
					return S_OK;
			}
			SRect oTemp = oGeneralRect;
			oTemp.m_lLeft += oHighlightRect.m_lLeft;
			oTemp.m_lWidth = oHighlightRect.m_lWidth;
			oTemp.m_lHeight = oHighlightRect.m_lTop;
			LPBYTE pSP = m_pMediaBuffer->GetBuffer();
			pSP += oHighlightRect.m_lLeft;
#ifdef BOUND_CHECK_ENABLE
			if (m_bBound)
			{
				oTemp.m_lTop += m_lBoundTop;
				pSP += m_lBoundTop*lSPStride;
				oTemp.m_lHeight = oHighlightRect.m_lTop - m_lBoundTop;
			}
#endif
			Draw(oFrame, pSP, oTemp, pYTable, pUTable, pVTable);
		}
#ifdef BOUND_CHECK_ENABLE
		if ((oHighlightRect.GetBottom()+1<oGeneralRect.m_lHeight)&&
			((!m_bBound)||(oHighlightRect.GetBottom()+1<m_lBoundBottom)))
#else
		if (oHighlightRect.GetBottom()+1<oGeneralRect.m_lHeight)
#endif
		{
			if ((NULL==pYTable)||(NULL==pUTable)||(NULL==pVTable))
			{
				DeleteTransformTable(pYTable, pUTable, pVTable);
				if (!CreateColorTransformTable(oCurrentPalette, pYTable, pUTable, pVTable))
					return S_OK;
			}
			SRect oTemp = oGeneralRect;
			oTemp.m_lLeft += oHighlightRect.m_lLeft;
			oTemp.m_lWidth = oHighlightRect.m_lWidth;;
			oTemp.m_lTop += oHighlightRect.GetBottom()+1;
			oTemp.m_lHeight = oGeneralRect.m_lHeight - (oHighlightRect.GetBottom()+1);
			LPBYTE pSP = m_pMediaBuffer->GetBuffer();
			pSP += oHighlightRect.m_lLeft + (oHighlightRect.GetBottom()+1)*lSPStride;
#ifdef BOUND_CHECK_ENABLE
			if (m_bBound)
				oTemp.m_lHeight = m_lBoundBottom - oHighlightRect.GetBottom();
#endif
			Draw(oFrame, pSP, oTemp, pYTable, pUTable, pVTable);
		}
		DeleteTransformTable(pYTable, pUTable, pVTable);

		if (!CreateColorTransformTable(m_oHighlightPalette, pYTable, pUTable, pVTable))
			return S_OK;

		LPBYTE pSP = m_pMediaBuffer->GetBuffer();
		pSP += oHighlightRect.m_lLeft + oHighlightRect.m_lTop*lSPStride;
		oHighlightRect.m_lLeft += oGeneralRect.m_lLeft;
		oHighlightRect.m_lTop += oGeneralRect.m_lTop;
		Draw(oFrame, pSP, oHighlightRect, pYTable, pUTable, pVTable);
		DeleteTransformTable(pYTable, pUTable, pVTable);

		return S_OK;
	}


protected:
	struct SPalette
	{
		SPalette()
		{
			InitArray();
		}
		SPalette& operator =(LPSAFEARRAY psa)
		{
			if (NULL==psa)
			{
				InitArray();
				return *this;
			}
			long lLbound0,lUbound0;
			SafeArrayGetLBound(psa, 1, &lLbound0);
			SafeArrayGetUBound(psa, 1, &lUbound0);
			long lLbound1,lUbound1;
			SafeArrayGetLBound(psa, 2, &lLbound1);
			SafeArrayGetUBound(psa, 2, &lUbound1);
			LPBYTE ppPtr = (LPBYTE)psa->pvData;
			if ((4==(lUbound0-lLbound0+1))&&(4==lUbound1-lLbound1+1))
			{
				for (int i=0;i<DVD_SUBPICTURE_COLOR_COUNT; i++)
				{
					m_arColorY[i] = *ppPtr; ppPtr++;
					m_arColorU[i] = *ppPtr; ppPtr++;
					m_arColorV[i] = *ppPtr; ppPtr++;
					m_arColorA[i] = *ppPtr; ppPtr++;
					m_arColorAInv[i] = 15 - m_arColorA[i];
				}
			}
			return *this;
		}
		BYTE m_arColorY[DVD_SUBPICTURE_COLOR_COUNT];
		BYTE m_arColorU[DVD_SUBPICTURE_COLOR_COUNT];
		BYTE m_arColorV[DVD_SUBPICTURE_COLOR_COUNT];
		BYTE m_arColorA[DVD_SUBPICTURE_COLOR_COUNT];
		BYTE m_arColorAInv[DVD_SUBPICTURE_COLOR_COUNT];
	private:
		void InitArray()
		{
			memset(m_arColorY, 0, DVD_SUBPICTURE_COLOR_COUNT*sizeof(BYTE));
			memset(m_arColorU, 0, DVD_SUBPICTURE_COLOR_COUNT*sizeof(BYTE));
			memset(m_arColorV, 0, DVD_SUBPICTURE_COLOR_COUNT*sizeof(BYTE));
			memset(m_arColorA, 0, DVD_SUBPICTURE_COLOR_COUNT*sizeof(BYTE));
			memset(m_arColorAInv, 15, DVD_SUBPICTURE_COLOR_COUNT*sizeof(BYTE));
		}
	};
	struct SYUVFrame
	{
		SYUVFrame()
			: m_pYPlane(NULL)
			, m_pUPlane(NULL)
			, m_pVPlane(NULL)
			, m_lYStride(0)
			, m_lUStride(0)
			, m_lVStride(0)
			, m_lWidth(0)
			, m_lHeight(0)
			, m_lColorSpace(0)
		{			
		}
		LPBYTE m_pYPlane;
		LPBYTE m_pUPlane;
		LPBYTE m_pVPlane;

		long m_lYStride;
		long m_lUStride;
		long m_lVStride;

		long m_lWidth;
		long m_lHeight;

		long m_lColorSpace;
	};
	struct SRect
	{
		SRect()
			: m_lLeft(0)
			, m_lTop(0)
			, m_lWidth(0)
			, m_lHeight(0)
		{
		}
		SRect(long lLeft, long lTop, long lWidth, long lHeight)
			: m_lLeft(lLeft)
			, m_lTop(lTop)
			, m_lWidth(lWidth)
			, m_lHeight(lHeight)
		{
		}
		void Normalize()
		{
			if (m_lWidth<0)
			{
				m_lLeft += m_lWidth;
				m_lWidth = -m_lWidth;
			}
			if (m_lHeight<0)
			{
				m_lTop += m_lHeight;
				m_lHeight = -m_lHeight;
			}
		}
		void Align(long nAlign = 2)
		{
			m_lLeft		= (m_lLeft/nAlign)*nAlign;
			m_lTop		= (m_lTop/nAlign)*nAlign;
			m_lWidth	= (m_lWidth/nAlign)*nAlign;
			m_lHeight	= (m_lHeight/nAlign)*nAlign;
		}
		long GetRight() const {return m_lLeft + m_lWidth - 1;}
		long GetBottom() const {return m_lTop + m_lHeight - 1;}
		long m_lLeft;
		long m_lTop;
		long m_lWidth;
		long m_lHeight;
	};
	struct SMark
	{
		SMark()
			: m_psaColor(NULL)
			, m_dDelay(0.0)
			, m_lLeft(-1)
			, m_lTop(-1)
			, m_bStopMark(FALSE)
		{
		}
		SMark(double dDelay)
			: m_psaColor(NULL)
			, m_dDelay(dDelay)
			, m_lLeft(-1)
			, m_lTop(-1)
			, m_bStopMark(TRUE)
		{
		}
		SMark(LPSAFEARRAY psaColor, double dDelay, long lLeft, long lTop)
			: m_dDelay(dDelay)
			, m_lLeft(lLeft)
			, m_lTop(lTop)
			, m_bStopMark(FALSE)
		{
			if (NULL!=psaColor)
				SafeArrayCopy(psaColor, &m_psaColor);
		}
		SMark(const SMark &oSrc)
			: m_psaColor(NULL)
			, m_dDelay(oSrc.m_dDelay)
			, m_lLeft(oSrc.m_lLeft)
			, m_lTop(oSrc.m_lTop)
			, m_bStopMark(oSrc.m_bStopMark)
		{
			if (NULL!=oSrc.m_psaColor)
				SafeArrayCopy(oSrc.m_psaColor, &m_psaColor);
		}
		~SMark()
		{
			if (NULL!=m_psaColor)
				SafeArrayDestroy(m_psaColor);
		}
		BOOL m_bStopMark;
		double m_dDelay;
		long m_lLeft;
		long m_lTop;
		LPSAFEARRAY m_psaColor;
	};
	
	CAtlArray<SMark> m_arMark;
	long m_lWidth;
	long m_lHeight;

	
	BOOL m_bHighlight;
	SPalette m_oHighlightPalette;
	SRect m_oHighlightRect;

#ifdef BOUND_CHECK_ENABLE
	BOOL m_bBound;
	long m_lBoundTop;
	long m_lBoundBottom;
	long m_lBoundPaletteMarkIndex;

	void CheckHighlightOnChangeBound()
	{
		if (!m_bBound)
			return;
		long lTop = min(m_oHighlightRect.m_lTop, m_oHighlightRect.m_lTop + m_oHighlightRect.m_lHeight);
		long lBottom = max(m_oHighlightRect.m_lTop, m_oHighlightRect.m_lTop + m_oHighlightRect.m_lHeight);
		if ((lTop<m_lBoundTop)||(lBottom>m_lBoundTop))
			m_bBound = FALSE;
	}
	void CalculateBound(const SPalette &oCurrentPalette)
	{
		if (m_bBound)
			return;
		long lHeight = abs(m_lHeight);
		long lWidth = abs(m_lWidth);
		if (!m_bHighlight)
			CalculateBoundNoHightlight(oCurrentPalette);
		else
		{
			SRect oHighlightRect = m_oHighlightRect;
			oHighlightRect.Normalize(); 
			long lHighlightTop = min(oHighlightRect.m_lTop, lHeight);
			long lHighlightBottom = min(oHighlightRect.GetBottom(), lHeight - 1);
			if (lHighlightTop>=lHighlightBottom)
			{
				CalculateBoundNoHightlight(oCurrentPalette);
				return;
			}

			BYTE nMask = 0;
			for (int i=0;i<DVD_SUBPICTURE_COLOR_COUNT;i++)
			{
				if (0!=oCurrentPalette.m_arColorA[i])
					nMask |= (1<<i);
			}
			LPBYTE pSPPtr = m_pMediaBuffer->GetBuffer();
			BOOL bFound = FALSE;
			for (m_lBoundTop=0;m_lBoundTop<lHighlightTop;m_lBoundTop++)
			{
				for (int j=0;j<lWidth;j++, pSPPtr++)
				{
					if (0!=((1<<(*pSPPtr)) & nMask))
					{
						bFound = TRUE;
						break;
					}
				}
				if (bFound)
					break;
			}
			pSPPtr = m_pMediaBuffer->GetBuffer() + (lHeight-1)*lWidth;
			bFound = FALSE;
			for (m_lBoundBottom = lHeight-1; m_lBoundBottom>lHighlightBottom; m_lBoundBottom--)
			{
				for (int j=0;j<lWidth;j++, pSPPtr++)
				{
					if (0!=((1<<(*pSPPtr)) & nMask))
					{
						bFound = TRUE;
						break;
					}
				}
				if (bFound)
					break;
				pSPPtr -= 2*lWidth;
			}
		}
	}
	void CalculateBoundNoHightlight(const SPalette &oCurrentPalette)
	{
		long lHeight = abs(m_lHeight);
		long lWidth = abs(m_lWidth);
		BYTE nMask = 0;
		for (int i=0;i<DVD_SUBPICTURE_COLOR_COUNT;i++)
		{
			if (0!=oCurrentPalette.m_arColorA[i])
				nMask |= (1<<i);
		}
		LPBYTE pSPPtr = m_pMediaBuffer->GetBuffer();
		BOOL bFound = FALSE;
		for (m_lBoundTop=0;m_lBoundTop<lHeight;m_lBoundTop++)
		{
			for (int j=0;j<lWidth;j++, pSPPtr++)
			{
				if (0!=((1<<(*pSPPtr)) & nMask))
				{
					bFound = TRUE;
					break;
				}
			}
			if (bFound)
				break;

		}
		if (m_lBoundTop == lHeight)
		{
			m_lBoundBottom = m_lBoundTop;
			m_bBound = TRUE;
			return;
		}
		pSPPtr = m_pMediaBuffer->GetBuffer() + (lHeight-1)*lWidth;
		bFound = FALSE;
		for (m_lBoundBottom = lHeight-1; m_lBoundBottom>m_lBoundTop; m_lBoundBottom--)
		{
			for (int j=0;j<lWidth;j++, pSPPtr++)
			{
				if (0!=((1<<(*pSPPtr)) & nMask))
				{
					bFound = TRUE;
					break;
				}
			}
			if (bFound)
				break;
			pSPPtr -= 2*lWidth;
		}
		m_bBound = TRUE;
	}

#endif

	MemoryUtils::CMemoryUtils m_oMemoryUtils;

	BOOL GetFrameMarkSetting(IAVSUncompressedVideoFrame *pFrame, long &lLeft, long &lTop, long &lPaletteMarkIndex)
	{
		if (NULL==pFrame)
			return FALSE;
		double dFrameTime = 0.0;
		pFrame->get_TimeStamp(&dFrameTime);
		if (dFrameTime<m_dTimeStamp)
			return FALSE;
		long lMarkCount = (long)m_arMark.GetCount();
		if (0==lMarkCount)
			return FALSE;
		long lMarkIndex = 0;
		//первый маркер должен быть с нулевой задержкой (см. спецификацию стр. VI5-79)
		for (; lMarkIndex<lMarkCount-1; lMarkIndex++)
		{
			if (m_dTimeStamp + m_arMark[lMarkIndex+1].m_dDelay > dFrameTime)
				break;			
		}
		if (lMarkIndex>=lMarkCount)
			return FALSE;
		
		if (m_arMark[lMarkIndex].m_bStopMark)
			return FALSE;//не надо рисовать был вызван Stop но еще не было после него Start

		lLeft = m_arMark[lMarkIndex].m_lLeft;
		lTop = m_arMark[lMarkIndex].m_lTop;		
		//надо найти первый маркер с выставленными Left и Top в крайнем случае это будет нулевой маркер
		//в нем всегда есть команда SetDisplayArea (см. спецификацию Annex L)
		for (long lTempIndex = lMarkIndex - 1;(((lLeft<0)||(lTop))&&(lTempIndex>=0));lTempIndex--)
		{
			lLeft = m_arMark[lTempIndex].m_lLeft;
			lTop = m_arMark[lTempIndex].m_lTop;
		}

		lPaletteMarkIndex = lMarkIndex;
		LPSAFEARRAY psaTempColor = m_arMark[lPaletteMarkIndex].m_psaColor;
		if (NULL==psaTempColor)
		{
			for (long lTempIndex = lMarkIndex - 1; lTempIndex>=0; lTempIndex--)
			{
				if (NULL!=m_arMark[lTempIndex].m_psaColor)
				{
					lPaletteMarkIndex = lTempIndex;
					break;
				}
			}
		}
		return TRUE;
	}

	BOOL GetFrameSetting(IAVSUncompressedVideoFrame *pFrame, SYUVFrame &oFrame)
	{
		if (NULL==pFrame)
			return FALSE;
				
		pFrame->get_ColorSpace(&oFrame.m_lColorSpace);
		if ((CSP_I420 != (oFrame.m_lColorSpace & CSP_COLOR_MASK))&&
			(CSP_YV12 != (oFrame.m_lColorSpace & CSP_COLOR_MASK)))
		{
			ATLTRACE("Color space not supported");
			return FALSE;
		}

		pFrame->get_Width(&oFrame.m_lWidth);
		pFrame->get_Height(&oFrame.m_lHeight);
		pFrame->get_Plane(0, &oFrame.m_pYPlane);
		pFrame->get_Stride(0, &oFrame.m_lYStride);

		if (CSP_I420 != (oFrame.m_lColorSpace & CSP_COLOR_MASK))
		{
			pFrame->get_Plane(1, &oFrame.m_pUPlane);
			pFrame->get_Stride(1, &oFrame.m_lUStride);
			pFrame->get_Plane(2, &oFrame.m_pVPlane);
			pFrame->get_Stride(2, &oFrame.m_lVStride);
		}
		else
		{
			pFrame->get_Plane(2, &oFrame.m_pUPlane);
			pFrame->get_Stride(2, &oFrame.m_lUStride);
			pFrame->get_Plane(1, &oFrame.m_pVPlane);
			pFrame->get_Stride(1, &oFrame.m_lVStride);
		}
		if ((NULL==oFrame.m_pYPlane)||(NULL==oFrame.m_pUPlane)||(NULL==oFrame.m_pVPlane)||
			(oFrame.m_lWidth>oFrame.m_lYStride)||
			(oFrame.m_lWidth>2*oFrame.m_lUStride)||
			(oFrame.m_lWidth>2*oFrame.m_lVStride))
			return FALSE;
		return TRUE;
	}
	BOOL CreateColorTransformTable(const SPalette &oPalette, LPBYTE &pYTable, LPBYTE &pUTable, LPBYTE &pVTable)
	{
		pYTable = new BYTE[256*DVD_SUBPICTURE_COLOR_COUNT];
		if (NULL==pYTable)
			return FALSE;
		pUTable = new BYTE[256*DVD_SUBPICTURE_COLOR_COUNT];
		if (NULL==pUTable)
		{
			delete []pYTable;
			return FALSE;
		}
		pVTable = new BYTE[256*DVD_SUBPICTURE_COLOR_COUNT];
		if (NULL==pVTable)
		{
			delete []pYTable;
			delete []pUTable;
			return FALSE;
		}

		WORD arColorYA[DVD_SUBPICTURE_COLOR_COUNT];
		WORD arColorUA[DVD_SUBPICTURE_COLOR_COUNT];
		WORD arColorVA[DVD_SUBPICTURE_COLOR_COUNT];

		for (int i=0;i<DVD_SUBPICTURE_COLOR_COUNT;i++)
		{
			arColorYA[i] = oPalette.m_arColorY[i]*oPalette.m_arColorA[i];
			arColorUA[i] = oPalette.m_arColorU[i]*oPalette.m_arColorA[i];
			arColorVA[i] = oPalette.m_arColorV[i]*oPalette.m_arColorA[i];
			//так нельзя будет переполнение оно отбросится и при делении на 15 получим не правильный результат
			//for (int j=0;j<256;j++, pVTablePtr++)
			//{
			//	*pVTablePtr = oPalette.m_arColorAInv[i]*j; 
			//}
		}

		LPBYTE pYTablePtr = pYTable;
		LPBYTE pUTablePtr = pUTable;
		LPBYTE pVTablePtr = pVTable;
		pVTablePtr = pVTable;
		for (int i=0;i<DVD_SUBPICTURE_COLOR_COUNT;i++)
		{
			if (0==oPalette.m_arColorAInv[i])
			{
				memset(pYTablePtr, arColorYA[i]/15, 256); pYTablePtr+=256;
				memset(pUTablePtr, arColorUA[i]/15, 256); pUTablePtr+=256;
				memset(pVTablePtr, arColorVA[i]/15, 256); pVTablePtr+=256;
				continue;
			}
			WORD wTemp = 0;
			for (int j=0; j<256;j++,pYTablePtr++, pUTablePtr++, pVTablePtr++)
			{
				wTemp = oPalette.m_arColorAInv[i]*j;
				//*pYTablePtr = (m_arColorAInv[i]*j + arColorYA[i])/15;
				//*pUTablePtr = (m_arColorAInv[i]*j + arColorUA[i])/15;
				//*pVTablePtr = (m_arColorAInv[i]*j + arColorVA[i])/15;
				*pYTablePtr = (wTemp + arColorYA[i])/15;
				*pUTablePtr = (wTemp + arColorUA[i])/15;
				*pVTablePtr = (wTemp + arColorVA[i])/15;
			}
		}
		return TRUE;
	}

	void DeleteTransformTable(LPBYTE &pYTable, LPBYTE &pUTable, LPBYTE &pVTable)
	{
		if (NULL!=pYTable)
			delete []pYTable;
		pYTable = NULL;
		if (NULL!=pUTable)
			delete []pUTable;
		pUTable = NULL;
		if (NULL!=pVTable)
			delete []pVTable;
		pVTable = NULL;
	}
	void DrawNoHightlight(SYUVFrame &oFrame, long lLeft, long lTop, const SPalette &oCurrentPalette)
	{
		LPBYTE pSP = m_pMediaBuffer->GetBuffer();
		SRect oRect(lLeft, lTop, m_lWidth, m_lHeight);
#ifdef BOUND_CHECK_ENABLE
		if (m_bBound)
		{
			if (m_lBoundBottom <= m_lBoundTop)
				return;
			oRect.m_lTop += m_lBoundTop;
			pSP += m_lBoundTop*abs(m_lWidth);
			oRect.m_lHeight = m_lBoundBottom - m_lBoundTop + 1;
		}
#endif		
		LPBYTE pYTable = NULL;
		LPBYTE pUTable = NULL;
		LPBYTE pVTable = NULL;
		if (!CreateColorTransformTable(oCurrentPalette, pYTable, pUTable, pVTable))
			return;
		Draw(oFrame, pSP, oRect, pYTable, pUTable, pVTable);
		delete []pYTable;
		delete []pUTable;
		delete []pVTable;
	}
	//oSPRect - прямоугольник в координатах фрейма в котором надо отрисовать SP	
	void Draw(SYUVFrame &oFrame, LPBYTE pSP, const SRect &oSPRect, LPBYTE pYTable, LPBYTE pUTable, LPBYTE pVTable)
	{
		long lSPStride = abs(m_lWidth);
		SRect oInFrameRect = oSPRect;
		oInFrameRect.Normalize();
		oInFrameRect.Align();
		if (oInFrameRect.m_lLeft<0)
		{
			pSP -= oInFrameRect.m_lLeft;
			oInFrameRect.m_lWidth += oInFrameRect.m_lLeft;//уменьшаем ширину
			oInFrameRect.m_lLeft = 0;
		}
		if (oInFrameRect.m_lTop<0)
		{
			pSP -= lSPStride*oInFrameRect.m_lTop;
			oInFrameRect.m_lHeight += oInFrameRect.m_lTop;//уменьшаем высоту
			oInFrameRect.m_lTop = 0;
		}

		if ((oInFrameRect.m_lLeft>=oFrame.m_lWidth)||(oInFrameRect.m_lTop>oFrame.m_lHeight)||
			(oInFrameRect.m_lWidth<=0)||(oInFrameRect.m_lHeight<=0))
			return;
		long lTemp = ((oFrame.m_lWidth - oInFrameRect.m_lLeft)/2)*2;
		if (lTemp<=0)
			return;
		if (oInFrameRect.m_lWidth>lTemp)
			oInFrameRect.m_lWidth = lTemp;
		lTemp = ((oFrame.m_lHeight - oInFrameRect.m_lTop)/2)*2;
		if (lTemp<=0)
			return;
		if (oInFrameRect.m_lHeight>lTemp)
			oInFrameRect.m_lHeight = lTemp;
		Draw(oFrame, pSP, lSPStride, oInFrameRect, pYTable, pUTable, pVTable);
	}
	/*
		oFrame - свойства фрейма в котором отрисовываем SP
		pSP - ссылка на SP (может быть не на начало, а на память которую надо отрисовать)
		oInFrameRect - прямоугольник внутри YUV фрейма в котором надо отрисовать SP
		pYTable, pUTable, pVTable - таблицы цветов 
	*/
	void Draw(SYUVFrame &oFrame, LPBYTE pSP, long lSPStride, const SRect &oInFrameRect, LPBYTE pYTable, LPBYTE pUTable, LPBYTE pVTable)
	{
		if (0!=(oFrame.m_lColorSpace & CSP_VFLIP))
		{
			LPBYTE pYPtr = oFrame.m_pYPlane + (oFrame.m_lHeight - oInFrameRect.m_lTop - 1)*oFrame.m_lYStride + oInFrameRect.m_lLeft;
			LPBYTE pSPPtr = pSP;
			long lSPPitch = lSPStride - oInFrameRect.m_lWidth;
			long lFramePitch = -oFrame.m_lYStride - oInFrameRect.m_lWidth;
			for (int i=0; i<oInFrameRect.m_lHeight; i++)
			{
				for (int j=0; j<oInFrameRect.m_lWidth; j++, pYPtr++, pSPPtr++)
				{
					//*pYPtr = (m_arColorAInv[*pSPPtr]*(*pYPtr) + m_arColorA[*pSPPtr]*(m_arColorY[*pSPPtr]))/15;
					*pYPtr = pYTable[((*pSPPtr)<<8)+*pYPtr];
				}
				pSPPtr+=lSPPitch;
				pYPtr+=lFramePitch;
			}
			
			LPBYTE pUPtr = oFrame.m_pUPlane + (oFrame.m_lHeight/2 - oInFrameRect.m_lTop/2 - 1)*oFrame.m_lUStride + oInFrameRect.m_lLeft/2;
			LPBYTE pVPtr = oFrame.m_pVPlane + (oFrame.m_lHeight/2 - oInFrameRect.m_lTop/2 - 1)*oFrame.m_lVStride + oInFrameRect.m_lLeft/2;

			pSPPtr = pSP;
			lSPPitch = 2*lSPStride - oInFrameRect.m_lWidth;//2* потому что надо добить текущую строку + пропустить следующую
			long lUFramePitch = -oFrame.m_lUStride - oInFrameRect.m_lWidth/2;
			long lVFramePitch = -oFrame.m_lVStride - oInFrameRect.m_lWidth/2;
			for (int i=0;i<oInFrameRect.m_lHeight/2;i++)
			{
				for (int j=0;j<oInFrameRect.m_lWidth/2;j++, pUPtr++, pVPtr++, pSPPtr+=2)
				{
					//*pUPtr = (m_arColorAInv[*pSPPtr]*(*pUPtr) + m_arColorA[*pSPPtr]*(m_arColorU[*pSPPtr]))/15;
					//*pVPtr = (m_arColorAInv[*pSPPtr]*(*pVPtr) + m_arColorA[*pSPPtr]*(m_arColorV[*pSPPtr]))/15;
					*pUPtr = pUTable[((*pSPPtr)<<8)+*pUPtr];
					*pVPtr = pVTable[((*pSPPtr)<<8)+*pVPtr];
				}
				pSPPtr += lSPPitch;
				pUPtr += lUFramePitch;
				pVPtr += lVFramePitch;
			}
		}
		else
		{
			LPBYTE pYPtr = oFrame.m_pYPlane + oInFrameRect.m_lTop*oFrame.m_lYStride + oInFrameRect.m_lLeft;
			LPBYTE pSPPtr = pSP;
			long lSPPitch = lSPStride - oInFrameRect.m_lWidth;
			long lFramePitch = oFrame.m_lYStride - oInFrameRect.m_lWidth;
			for (int i=0; i<oInFrameRect.m_lHeight; i++)
			{
				for (int j=0; j<oInFrameRect.m_lWidth; j++, pYPtr++, pSPPtr++)
				{
					//*pYPtr = (m_arColorAInv[*pSPPtr]*(*pYPtr) + m_arColorA[*pSPPtr]*(m_arColorY[*pSPPtr]))/15;
					*pYPtr = pYTable[((*pSPPtr)<<8)+*pYPtr];
				}
				pSPPtr+=lSPPitch;
				pYPtr+=lFramePitch;
			}
			LPBYTE pUPtr = oFrame.m_pUPlane + oInFrameRect.m_lTop/2*oFrame.m_lUStride + oInFrameRect.m_lLeft/2;
			LPBYTE pVPtr = oFrame.m_pVPlane + oInFrameRect.m_lTop/2*oFrame.m_lVStride + oInFrameRect.m_lLeft/2;

			pSPPtr = pSP;
			lSPPitch = 2*lSPStride - oInFrameRect.m_lWidth;//2* потому что надо добить текущую строку + пропустить следующую
			long lUFramePitch = oFrame.m_lUStride - oInFrameRect.m_lWidth/2;
			long lVFramePitch = oFrame.m_lVStride - oInFrameRect.m_lWidth/2;
			for (int i=0;i<oInFrameRect.m_lHeight/2;i++)
			{
				for (int j=0;j<oInFrameRect.m_lWidth/2;j++, pUPtr++, pVPtr++, pSPPtr+=2)
				{
					//*pUPtr = (m_arColorAInv[*pSPPtr]*(*pUPtr) + m_arColorA[*pSPPtr]*(m_arColorU[*pSPPtr]))/15;
					//*pVPtr = (m_arColorAInv[*pSPPtr]*(*pVPtr) + m_arColorA[*pSPPtr]*(m_arColorV[*pSPPtr]))/15;
					*pUPtr = pUTable[((*pSPPtr)<<8)+*pUPtr];
					*pVPtr = pVTable[((*pSPPtr)<<8)+*pVPtr];
				}
				pSPPtr += lSPPitch;
				pUPtr += lUFramePitch;
				pVPtr += lVFramePitch;
			}
		}
	}
};

// CAVSDVDSubpictureFrame

[coclass, uuid("C97B1CB0-AEB6-41BB-8CFB-74AEC476ECC5"), threading(apartment), vi_progid("AVSDVDSubpicture.AVSDVDSubpictureFrame"), progid("AVSDVDSubpicture.AVSDVDSubpictureFram.1"), version(1.0)]
class ATL_NO_VTABLE CAVSDVDSubpictureFrame : public CAVSDVDSubpictureFrameImpl<CAVSDVDSubpictureFrame, IAVSDVDSubpictureFrame>
{
};
