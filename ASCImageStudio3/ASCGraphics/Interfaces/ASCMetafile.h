#pragma once
#include "ASCRenderer.h"
#include "..\DocumentViewer\Metafile.h"
#include "..\..\ASCOfficeStudio\Common\OfficeFileTemplate.h"


#define PROPERTY_META(NameBase, Name, Type)				\
	STDMETHOD(get_##NameBase##Name)(Type* pVal)			\
	{													\
		if (NULL == pVal)								\
			return S_FALSE;								\
		*pVal =	m_oPage.m_o##NameBase.##Name;			\
		return S_OK;									\
	}													\
	STDMETHOD(put_##NameBase##Name)(Type Val)			\
	{													\
		m_oPage.##NameBase##Name(Val);					\
		return S_OK;									\
	}

[coclass, uuid("40D33F34-B681-46be-8649-FFBE7146569D"), threading(apartment), vi_progid("AVSMetafile"), progid("AVSMetafile.1"), version(1.0)]
class ATL_NO_VTABLE CAVSMetafile :	public IAVSMetafile, public IAVSRenderer2
{
protected:

	CMetafile m_oPage;

	// чтобы рисовать прямо из файла (пока djvu)
	IAVSOfficePages* m_pDocInstance;
	LONG m_lPageIndex;

	DWORD m_lCommandType;
	CString m_strHyperLinks;

public:

	CAVSMetafile() : m_oPage()
	{
		m_pDocInstance	= NULL;
		m_lPageIndex	= NULL;

		m_lCommandType  = c_nNone;
	}

	~CAVSMetafile()
	{
		RELEASEINTERFACE(m_pDocInstance);
	}

public:
	STDMETHOD(get_Type)(LONG* lType)
	{
		return S_OK;
	}
	STDMETHOD(NewPage)()
	{
		return S_OK;
	}

	STDMETHOD(get_Height)(double* fHeight)
	{
		if (NULL != fHeight)
			*fHeight = m_oPage.m_fHeight;
		return S_OK;
	}
	STDMETHOD(get_Width)(double* fWidth)
	{		
		if (NULL != fWidth)
			*fWidth = m_oPage.m_fWidth;
		return S_OK;
	}
	STDMETHOD(put_Height)(double fHeight)
	{
		m_oPage.m_fHeight = fHeight;
		return S_OK;
	}
	STDMETHOD(put_Width)(double fWidth)
	{		
		m_oPage.m_fWidth = fWidth;		
		return S_OK;
	}
	STDMETHOD(get_DpiX)(double* dDpiX)
	{
		return S_FALSE;
	}
	STDMETHOD(get_DpiY)(double* dDpiY)
	{
		return S_FALSE;
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
	PROPERTY_META(Pen, Color, LONG)
	PROPERTY_META(Pen, Alpha, LONG)
	PROPERTY_META(Pen, Size, double)
	PROPERTY_META(Pen, DashStyle, BYTE)
	PROPERTY_META(Pen, LineStartCap, BYTE)
	PROPERTY_META(Pen, LineEndCap, BYTE)
	PROPERTY_META(Pen, LineJoin, BYTE)
	PROPERTY_META(Pen, DashOffset, double)
	PROPERTY_META(Pen, Align, LONG)
	PROPERTY_META(Pen, MiterLimit, double)

	STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern)
	{
		if (NULL == pPattern)
			return S_FALSE;

		m_oPage.PenDashPatern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
		return S_OK;
	}
	STDMETHOD(BrushRect)(BOOL b, double x, double y, double width, double height)
	{
		Gdiplus::RectF oRect((float)x, (float)y, (float)width, (float)height);
		m_oPage.BrushRect(b, oRect);
		return S_OK;
	}

	PROPERTY_META(Brush, Color1, LONG)
	PROPERTY_META(Brush, Color2, LONG)
	PROPERTY_META(Brush, Alpha1, LONG)
	PROPERTY_META(Brush, Alpha2, LONG)
	PROPERTY_META(Brush, TextureAlpha, LONG)

	STDMETHOD(get_BrushTexturePath)(BSTR* bsPath)
	{
		*bsPath = m_oPage.m_oBrush.TexturePath.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_BrushTexturePath)(BSTR bsPath)
	{
		m_oPage.m_oBrush.TexturePath = (CString)bsPath;
		return S_OK;
	}

	PROPERTY_META(Brush, TextureMode, LONG)
	PROPERTY_META(Brush, Type, LONG)
	PROPERTY_META(Brush, LinearAngle, double)

	STDMETHOD(get_FontName)(BSTR* bsPath)
	{
		*bsPath = m_oPage.m_oFont.Name.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_FontName)(BSTR bsPath)
	{
		m_oPage.FontName((CString)bsPath);
		return S_OK;
	}
	STDMETHOD(get_FontPath)(BSTR* bsPath)
	{
		*bsPath = m_oPage.m_oFont.Path.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_FontPath)(BSTR bsPath)
	{
		m_oPage.FontPath((CString)bsPath);
		return S_OK;
	}
	PROPERTY_META(Font, Size, double)
	
	STDMETHOD(get_FontStyle)(LONG* val)
	{
		*val = m_oPage.m_oFont.GetStyle();
		return S_OK;
	}
	STDMETHOD(put_FontStyle)(LONG val)
	{
		m_oPage.FontStyle(val);
		return S_OK;
	}

	PROPERTY_META(Font, StringGID, BOOL)
	PROPERTY_META(Font, CharSpace, double);

	PROPERTY_META(Shadow, Visible, BOOL)
	PROPERTY_META(Shadow, DistanceX, double)
	PROPERTY_META(Shadow, DistanceY, double)
	PROPERTY_META(Shadow, BlurSize, double)
	PROPERTY_META(Shadow, Color, LONG)
	PROPERTY_META(Shadow, Alpha, LONG)

	PROPERTY_META(Edge, Visible, LONG)
	PROPERTY_META(Edge, Dist, double)
	PROPERTY_META(Edge, Color, LONG)
	PROPERTY_META(Edge, Alpha, LONG)

	// ---------------------------------------------
	// отрисовка текста
	STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		if (c_nHyperlinkType == m_lCommandType)
			m_strHyperLinks = (CString)bsText;
		
		m_oPage.DrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		return S_OK;
	}
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		if (c_nHyperlinkType == m_lCommandType)
			m_strHyperLinks = (CString)bsText;

		m_oPage.DrawTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags, FALSE);
		return S_OK;
	}
	// ---------------------------------------------
	// GraphicPath Methods
	STDMETHOD(PathCommandMoveTo)(double fX, double fY)
	{
		m_oPage.PathCommandMoveTo(fX, fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLineTo)(double fX, double fY)
	{
		m_oPage.PathCommandLineTo(fX, fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints)
	{
		m_oPage.PathCommandLinesTo(pPoints);
		return S_OK;
	}

	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		m_oPage.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		return S_OK;
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints)
	{
		m_oPage.PathCommandCurvesTo(pPoints);
		return S_OK;
	}
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
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
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY)
	{
		return S_OK;
	}
	
	STDMETHOD(DrawPath)(long nType)
	{
		m_oPage.DrawPath(nType);
		return S_OK;		
	}
	// ---------------------------------------------
	STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
	{
		m_oPage.DrawImage(pInterface, fX, fY, fWidth, fHeight);
		return S_OK;	
	}
	// ---------------------------------------------
	STDMETHOD(DrawImageFromFile)(BSTR bstrPath, double fX, double fY, double fWidth, double fHeight)
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
	STDMETHOD(Draw)(IUnknown* punkRenderer, BOOL* pBreak)
	{		
		IAVSRenderer* pRenderer = NULL;
		punkRenderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRenderer);

		if (NULL == pRenderer)
			return S_FALSE;

		if (NULL == m_pDocInstance)
		{
			m_oPage.Draw2(pRenderer, pBreak);
		}
		else
		{
			pRenderer->put_Width(m_oPage.m_fWidth);
			pRenderer->put_Height(m_oPage.m_fHeight);
			m_pDocInstance->DrawPage(m_lPageIndex, punkRenderer, pBreak);
		}

		RELEASEINTERFACE(pRenderer);

		return S_OK;
	}
	STDMETHOD(Draw2)(IUnknown* pRenderer, double dDPIX, double dDPIY, BOOL* pBreak)
	{
		if (NULL == pRenderer)
			return S_FALSE;

		//C2DRenderer* p2D = dynamic_cast<C2DRenderer*>(pRenderer);
		IAVSRenderer* pRen = NULL;
		pRenderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRen);

		if (NULL == m_pDocInstance)
		{
			m_oPage.Draw2(pRen, pBreak);
		}
		else
		{
			pRen->put_Width(m_oPage.m_fWidth);
			pRen->put_Height(m_oPage.m_fHeight);
			m_pDocInstance->DrawPage(m_lPageIndex, pRenderer, pBreak);
		}
		RELEASEINTERFACE(pRen);

		return S_OK;
	}

	STDMETHOD(BeginCommand)(DWORD lType)
	{
		m_lCommandType = lType;
		m_oPage.BeginCommand(lType);
		return S_OK;	
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		m_oPage.EndCommand(lType);
		return S_OK;
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

	STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		m_oPage.PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		return S_OK;
	}
	STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		m_oPage.DrawTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags, TRUE);
		return S_OK;
	}

	STDMETHOD(Save)(BSTR bsFile, DWORD dwType, BSTR bsParams)
	{
		return S_OK;
	}

	STDMETHOD(SetPageInstance)(IUnknown* punkInstance, LONG lPage)
	{
		RELEASEINTERFACE(m_pDocInstance);
		m_lPageIndex = -1;

		if (NULL != punkInstance)
		{
			punkInstance->QueryInterface(__uuidof(IAVSOfficePages), (void**)&m_pDocInstance);
			m_lPageIndex = lPage;
		}		

		return S_OK;
	}
	STDMETHOD(get_Hypelinks)(BSTR* bsLinks)
	{
		if ((NULL == bsLinks) || (_T("") != m_strHyperLinks))
			return S_FALSE;

		*bsLinks = m_strHyperLinks.AllocSysString();
		return S_OK;
	}
	STDMETHOD(SetAdditionalCommands)(BSTR bsXML)
	{
		m_oPage.SetAdditionalCommands(bsXML);
		return S_OK;
	}

	// -------------------------------------
	STDMETHOD(CommandLong)(LONG lCommand)
	{
		m_oPage.CommandLong(lCommand);
		return S_OK;
	}
	STDMETHOD(CommandDouble)(double dCommand)
	{
		m_oPage.CommandDouble(dCommand);
		return S_OK;
	}
	STDMETHOD(CommandString)(BSTR bsCommand)
	{
		m_oPage.CommandString(bsCommand);
		return S_OK;
	}
	STDMETHOD(CommandVariant)(VARIANT vCommand)
	{
		return S_OK;
	}

	STDMETHOD(CommandLong2)(LONG lType, LONG lCommand)
	{
		m_oPage.CommandLong2(lType, lCommand);
		return S_OK;
	}
	STDMETHOD(CommandDouble2)(LONG lType, double dCommand)
	{
		m_oPage.CommandDouble2(lType, dCommand);
		return S_OK;
	}
	STDMETHOD(CommandString2)(LONG lType, BSTR bsCommand)
	{
		m_oPage.CommandString2(lType, bsCommand);
		return S_OK;
	}
	STDMETHOD(CommandVariant2)(LONG lType, VARIANT vCommand)
	{
		return S_OK;
	}
};