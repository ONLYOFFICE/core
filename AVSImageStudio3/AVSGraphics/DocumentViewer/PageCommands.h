#pragma once
#include "..\stdafx.h"

#include "..\..\..\AVSOfficeStudio\Common\Interfaces.h"
#include "AVSMetafile.h"

[coclass, uuid("41403DF2-5EB1-4310-AF9B-93FD1BEEB866"), threading(apartment), vi_progid("PageCommands"), progid("PageCommands.1"), version(1.0)]
class ATL_NO_VTABLE CPageCommands :	public IAVSPageCommands, public IAVSOfficeRendererTemplate2
{
protected:

	CMetafile m_oPage;

public:

	CPageCommands() : m_oPage()
	{
	}

	~CPageCommands()
	{
	}

public:
	STDMETHOD(NewPage)()
	{
		return S_OK;
	}

	STDMETHOD(get_Height)(float* fHeight)
	{
		if (NULL != fHeight)
			*fHeight = m_oPage.m_fHeight;
		return S_OK;
	}
	STDMETHOD(get_Width)(float* fWidth)
	{		
		if (NULL != fWidth)
			*fWidth = m_oPage.m_fWidth;
		return S_OK;
	}
	STDMETHOD(SetHeight)(float fHeight)
	{
		m_oPage.m_fHeight = fHeight;
		return S_OK;
	}
	STDMETHOD(SetWidth)(float fWidth)
	{		
		m_oPage.m_fWidth = fWidth;		
		return S_OK;
	}
	// ---------------------------------------------
	// функции состояния откисовщика ---------------
	STDMETHOD(SetPen)(BSTR bsXML)
	{
		m_oPage.PenXML(bsXML);
		return S_OK;
	}
	STDMETHOD(SetBrush)(BSTR bsXML)
	{
		m_oPage.BrushXML(bsXML);
		return S_OK;
	}
	STDMETHOD(SetFont)(BSTR bsXML)
	{
		m_oPage.FontXML(bsXML);	
		return S_OK;
	}
	STDMETHOD(SetShadow)(BSTR bsXML)
	{
		m_oPage.ShadowXML(bsXML);
		return S_OK;
	}
	STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		m_oPage.EdgeXML(bsXML);
		return S_OK;
	}
	// ---------------------------------------------
	// отрисовка текста
	STDMETHOD(CommandDrawText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		m_oPage.DrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		return S_OK;
	}
	// ---------------------------------------------
	// GraphicPath Methods
	STDMETHOD(PathCommandMoveTo)(float fX, float fY)
	{
		m_oPage.PathCommandMoveTo(fX, fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLineTo)(float fX, float fY)
	{
		m_oPage.PathCommandLineTo(fX, fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY** ppPoints)
	{
		m_oPage.PathCommandLinesTo(ppPoints);
		return S_OK;
	}

	STDMETHOD(PathCommandCurveTo)(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
	{
		m_oPage.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		return S_OK;
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY** ppPoints)
	{
		m_oPage.PathCommandCurvesTo(ppPoints);
		return S_OK;
	}
	STDMETHOD(PathCommandArcTo)(float fX, float fY, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
	{
		m_oPage.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
		return S_OK;
	}
	STDMETHOD(PathCommandClose)()
	{
		m_oPage.PathCommandClose();
		return S_OK;
	}
	STDMETHOD(PathCommandEnd)()
	{
		m_oPage.PathCommandEnd();
		return S_OK;
	}

	STDMETHOD(PathCommandStart)()
	{
		m_oPage.PathCommandStart();
		return S_OK;
	}
	STDMETHOD(PathCommandGetCurrentPoint)(float* fX, float* fY)
	{
		return S_OK;
	}
	
	STDMETHOD(DrawPath)(long nType)
	{
		m_oPage.DrawPath(nType);
		return S_OK;		
	}
	// ---------------------------------------------
	STDMETHOD(DrawImage)(IUnknown **pInterface, float fX, float fY, float fWidth, float fHeight)
	{
		m_oPage.DrawImage(*pInterface, fX, fY, fWidth, fHeight);
		return S_OK;	
	}
	// ---------------------------------------------
	STDMETHOD(DrawImageFromFile)(BSTR bstrPath, float fX, float fY, float fWidth, float fHeight)
	{
		m_oPage.DrawImageFromFile(bstrPath, fX, fY, fWidth, fHeight);
		return S_OK;	
	}
	//----------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	
	// отрисовка страницы
	STDMETHOD(Draw)(IUnknown* punkRenderer, double dDPIX, double dDPIY, BOOL* pBreak)
	{
		/*IAVSOfficeRendererTemplate* pRenderer = NULL;
		punkRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate), (void**)&pRenderer);

		if (NULL == pRenderer)
			return S_FALSE;

		m_oPage.Draw2(pRenderer, dDPIX, dDPIY, pBreak);

		RELEASEINTERFACE(pRenderer);*/

		return S_OK;
	}
	STDMETHOD(Draw2)(IUnknown* pRenderer, double dDPIX, double dDPIY, BOOL* pBreak)
	{
		if (NULL == pRenderer)
			return S_FALSE;

		C2DRenderer* p2D = dynamic_cast<C2DRenderer*>(pRenderer);

		m_oPage.Draw(p2D, pBreak);
		return S_OK;
	}

	STDMETHOD(BeginCommand)(DWORD lType)
	{
		m_oPage.BeginCommand(lType);
		return S_OK;	
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		m_oPage.EndCommand(lType);
		return S_OK;
	}
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset, DWORD lFlags, BSTR sParams)
	{
		return CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		return S_OK;
	}
	STDMETHOD(SetCommandParams)(double dAngle,  double dLeft,  double dTop,  double dWidth,  double dHeight,  DWORD lFlags)
	{
		m_oPage.SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
		return S_OK;
	}

	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		m_oPage.SetTransform(dA, dB, dC, dD, dE, dF);
		return S_OK;
	}
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		return S_FALSE;
	}
	STDMETHOD(ResetTransform)(void)
	{
		m_oPage.ResetTransform();
		return S_OK;
	}

	STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		return S_OK;
	}
	STDMETHOD(put_ClipMode)(LONG lMode)
	{
		m_oPage.WriteClipMode(lMode);
		return S_OK;
	}

	STDMETHOD(PathCommandText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		m_oPage.PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		return S_OK;
	}

	STDMETHOD(InitAdvancedCommands)()
	{
		m_oPage.InitAdvancedCommands();
		return S_OK;
	}
	STDMETHOD(DestroyAdvancedCommands)()
	{
		m_oPage.DestroyAdvancedCommands();
		return S_OK;
	}
};


