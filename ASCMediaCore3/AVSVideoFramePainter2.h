// AVSVideoFramePainter.h 

#pragma once
#include "resource.h"       // main symbols
#include "DrawLogo.h"



// IAVSVideoFrameDrawing
[ object, uuid("6ACCCD45-B500-4141-8111-90DB4CEDE0C8"), dual, pointer_default(unique)]
__interface IAVSVideoFramePainter2 : IDispatch
{
	[id(209500 + 10)] HRESULT SetPicture ([in] IUnknown* pFrame);
	[id(209500 + 20)] HRESULT Draw ([in] IUnknown* pFrame, [in] long Left, [in] long Top);
};

// CAVSVideoFrameColorTransform
[ coclass, threading(apartment), vi_progid("AVSMediaCore3.IAVSVideoFramePainter2"), progid("AVSMediaCore3.IAVSVideoFramePainter.2"), version(1.0), uuid("05B7DB51-7EF4-4363-9B2A-0EF156F97A5A")]
class ATL_NO_VTABLE CAVSVideoFramePainter2 : 
	public IAVSVideoFramePainter2
{
protected:
	CDrawLogo m_oLogoPainter;
public:
	CAVSVideoFramePainter2 ()
	{
	}
public:

	STDMETHOD (SetPicture) (IUnknown* pFrame)
	{
		if (NULL == pFrame)
		{
			m_oLogoPainter.SetLogo (NULL);
			return S_OK;
		}

		IAVSUncompressedVideoFrame *pLogo = NULL;
		pFrame->QueryInterface (&pLogo);
		m_oLogoPainter.SetLogo (pLogo);
		pLogo->Release ();		

		return S_OK;
	}
	STDMETHOD (Draw) (IUnknown* pFrame, long Left, long Top)
	{
		if (NULL == pFrame)
			return S_OK;

		IMediaFrame *pMF = NULL;
		pFrame->QueryInterface (&pMF);

		if (pMF)
		{
			m_oLogoPainter.DrawLogo (pMF, Left, Top);
			pMF->Release ();
		}

		return S_OK;
	}

};

