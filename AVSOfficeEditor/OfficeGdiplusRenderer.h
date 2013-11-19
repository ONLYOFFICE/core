#pragma once
#include "GraphicsRenderer.h"

// IAVSGdiplusRenderer
[
	object,
	uuid("A22EDC00-A578-4236-97B6-DE527319B8B9"),
	dual,	helpstring("IAVSGdiplusRenderer Interface"),
	pointer_default(unique)
]
__interface IAVSGdiplusRenderer : IAVSOfficeRendererTemplate
{
	[id(20000 + 1)]  HRESULT CreateFromMediaData(IUnknown* punkMediaData);
};

[dispinterface, uuid("92B3AAF5-C216-4af6-8C88-09300C042298")]
__interface _IAVSGdiplusRendererEvents
{
};

[coclass, uuid("0E8BC38A-DCC2-4cde-BEC9-4AC8859BA4BC"), event_source(com), threading(apartment), vi_progid("GdiplusRenderer"), progid("GdiplusRenderer.1"), version(1.0)]
class ATL_NO_VTABLE CAVSGdiplusRenderer
	:	public IAVSGdiplusRenderer, public IAVSOfficeRendererTemplate2
{
protected:
	CGdiplusRenderer m_oRenderer;

public:

	__event __interface _IAVSGdiplusRendererEvents;

	CAVSGdiplusRenderer() : m_oRenderer()
	{
	}

	~CAVSGdiplusRenderer()
	{
	}

protected:
	BOOL CheckValidate(BOOL bOnlyGraphics = FALSE)
	{
		return m_oRenderer.CheckValidate(bOnlyGraphics);
	}

	void Clear()
	{
		return m_oRenderer.Clear();
	}

public:
	// создание рендерера из картинки
	STDMETHOD(CreateFromMediaData)(IUnknown* punkMediaData)
	{
		return m_oRenderer.CreateFromMediaData(punkMediaData);
	}
	// ---------------------------------------------
	// функции для работы со страницей (здесь - только лишь для вычисления dpi)
	STDMETHOD(NewPage)()
	{
		// ну не влезло так не влезло
		return S_OK;
	}
	STDMETHOD(SetHeight)(float fHeight)
	{
		return m_oRenderer.SetHeight(fHeight);
	}
	STDMETHOD(SetWidth)(float fWidth)
	{
		return m_oRenderer.SetWidth(fWidth);
	}
	// ---------------------------------------------
	// функции состояния откисовщика ---------------
	STDMETHOD(SetPen)(BSTR bsXML)
	{
		return m_oRenderer.SetPen(bsXML);
	}
	STDMETHOD(SetBrush)(BSTR bsXML)
	{
		return m_oRenderer.SetBrush(bsXML);
	}
	STDMETHOD(SetFont)(BSTR bsXML)
	{
		return m_oRenderer.SetFont(bsXML);
	}
	STDMETHOD(SetShadow)(BSTR bsXML)
	{
		return m_oRenderer.SetShadow(bsXML);
	}
	STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		return m_oRenderer.SetEdgeText(bsXML);
	}
	// ---------------------------------------------
	// отрисовка текста
	STDMETHOD(CommandDrawText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		return m_oRenderer.CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	// ---------------------------------------------
	// GraphicPath Methods
	STDMETHOD(PathCommandMoveTo)(float fX, float fY)
	{
		return m_oRenderer.PathCommandMoveTo(fX, fY);
	}
	STDMETHOD(PathCommandLineTo)(float fX, float fY)
	{
		return m_oRenderer.PathCommandLineTo(fX, fY);
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY** ppPoints)
	{
		return m_oRenderer.PathCommandLinesTo(ppPoints);
	}

	STDMETHOD(PathCommandCurveTo)(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
	{
		return m_oRenderer.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY** ppPoints)
	{
		return m_oRenderer.PathCommandCurvesTo(ppPoints);
	}
	STDMETHOD(PathCommandArcTo)(float fX, float fY, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
	{
		return m_oRenderer.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	}
	STDMETHOD(PathCommandClose)()
	{
		return m_oRenderer.PathCommandClose();
	}
	STDMETHOD(PathCommandEnd)()
	{
		return m_oRenderer.PathCommandEnd();
	}

	STDMETHOD(PathCommandStart)()
	{
		return m_oRenderer.PathCommandStart();
	}
	STDMETHOD(PathCommandGetCurrentPoint)(float* fX, float* fY)
	{
		return m_oRenderer.PathCommandGetCurrentPoint(fX, fY);
	}
	
	STDMETHOD(DrawPath)(long nType)
	{
		return m_oRenderer.DrawPath(nType);
	}
	// ---------------------------------------------
	STDMETHOD(DrawImage)(IUnknown **pInterface, float fX, float fY, float fWidth, float fHeight)
	{
		return m_oRenderer.DrawImage(pInterface, fX, fY, fWidth, fHeight);
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

	// new
	STDMETHOD(BeginCommand)(DWORD lType)
	{
		return m_oRenderer.BeginCommand(lType);
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		return m_oRenderer.EndCommand(lType);
	}
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset, DWORD lFlags, BSTR sParams)
	{
		return CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, float fX, float fY, float fWidth, float fHeight)
	{
		return S_OK;
	}

	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		return m_oRenderer.GetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}
	STDMETHOD(SetCommandParams)(double dAngle,  double dLeft,  double dTop,  double dWidth,  double dHeight,  DWORD lFlags)
	{
		return m_oRenderer.SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}

	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		return m_oRenderer.SetTransform(dA, dB, dC, dD, dE, dF);
	}
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		return m_oRenderer.GetTransform(pdA, pdB, pdC, pdD, pdE, pdF);
	}
	STDMETHOD(ResetTransform)(void)
	{
		return m_oRenderer.ResetTransform();
	}

	STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		return m_oRenderer.get_ClipMode(plMode);
	}
	STDMETHOD(put_ClipMode)(LONG lMode)
	{
		return m_oRenderer.put_ClipMode(lMode);
	}

	STDMETHOD(PathCommandText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		return m_oRenderer.PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
};