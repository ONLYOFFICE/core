// AVSGraphicsPath.h : Declaration of the AVSGraphicsPath

#pragma once
#include "..\stdafx.h"
#include "..\resource.h"       // main symbols
#include "Registration.h"

#include "..\Common\GraphicsPath.h"
#include "Structures.h"
#include "..\Interfaces\IAVSGraphicsPath.h"
#include "..\Interfaces\IAVSPen.h"
#include "..\Interfaces\IAVSBrush.h"

#include "AVSSolidPattern.h"

// CAVSGraphicsPath
[coclass, uuid("4C016283-8AB5-4fb8-B6BC-86F2AB8A95CF"), threading(apartment), vi_progid("AVSGraphics.GraphicsPath"), progid("AVSGraphics.GraphicsPath"), version(1.0), support_error_info(IAVSGraphicsPath), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSGraphicsPath
	:	public IAVSGraphicsPath
{
protected:
	CGraphicsPath m_oPath;
	LONG m_lFillMode;

	friend class CAVSGraphics;
	friend class CGraphicsPath;

public:

	CAVSGraphicsPath() : m_oPath()
	{
	}

	~CAVSGraphicsPath()
	{
	}

public:
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(CreateDublicate)(IAVSGraphicsBase** ppGraphicsBase)
	{
		if (NULL == ppGraphicsBase)
			return S_FALSE;

		RELEASEINTERFACE((*ppGraphicsBase));
		if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSGraphicsPath), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsBase), (void**)ppGraphicsBase)))
		{
			((CAVSGraphicsPath*)(*ppGraphicsBase))->m_oPath = m_oPath;
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(get_FillMode)(LONG* lMode)
	{
		if (NULL != lMode)
			return S_FALSE;
		
		*lMode = m_lFillMode;
		return S_OK;
	}
	STDMETHOD(put_FillMode)(LONG lMode)
	{
		m_lFillMode = lMode;

		m_oPath.m_lMode	= m_lFillMode;
		return S_OK;
	}

	STDMETHOD(get_PathPoints)(SAFEARRAY** lpArray)
	{
		SAFEARRAYBOUND rgsabTypes[1];
		rgsabTypes[0].lLbound = 0;

		LONG lCount = m_oPath.GetCount();

		rgsabTypes[0].cElements = 2 * lCount;
		(*lpArray) = SafeArrayCreate(VT_R8, 1, rgsabTypes);

		m_oPath.GetPoints((double*)(*lpArray)->pvData);

		return S_OK;
	}
	STDMETHOD(get_PointsCount)(LONG* lCount)
	{
		if (NULL == lCount)
			return S_FALSE;

		*lCount = m_oPath.GetCount();
		return S_OK;
	}

	STDMETHOD(StartFigure)()
	{
		BOOL bRes = m_oPath.PathCommandStart();
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(CloseFigure)()
	{
		BOOL bRes = m_oPath.PathCommandClose();
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(CloseAllFigures)()
	{
		BOOL bRes = m_oPath.PathCommandClose();
		return bRes ? S_OK : S_FALSE;
	}

	STDMETHOD(AddArc)(double x, double y, double width, double height, double startAngle, double sweepAngle)
	{
		BOOL bRes = m_oPath.AddArc(x, y, width, height, startAngle, sweepAngle);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(AddBezier)(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
	{
		BOOL bRes = m_oPath.AddBezier(x1, y1, x2, y2, x3, y3, x4, y4);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(AddBeziers)(LPSAFEARRAY lpArray)
	{
		if (NULL == lpArray)
			return S_FALSE;

		LONG lCount = lpArray->rgsabound[0].cElements;
		double* pData = (double*)lpArray->pvData;

		m_oPath.AddBeziers(pData, lCount);

		return S_OK;
	}
	STDMETHOD(AddCurve)(LPSAFEARRAY lpArray)
	{
		if (NULL == lpArray)
			return S_FALSE;

		LONG lCount = lpArray->rgsabound[0].cElements;
		double* pData = (double*)lpArray->pvData;

		m_oPath.AddCurve(pData, lCount);

		return S_OK;
	}
	STDMETHOD(AddLine)(double x1, double y1, double x2, double y2)
	{
		BOOL bRes = m_oPath.AddLine(x1, y1, x2, y2);
		return bRes ? S_OK : S_FALSE;
	}
    STDMETHOD(AddLines)(LPSAFEARRAY lpArray)
	{
		if (NULL == lpArray)
			return S_FALSE;

		LONG lCount = lpArray->rgsabound[0].cElements;
		double* pData = (double*)lpArray->pvData;

		m_oPath.AddLines(pData, lCount);

		return S_OK;
	}
	STDMETHOD(AddEllipse)(double x, double y, double width, double height)
	{
		BOOL bRes = m_oPath.AddEllipse(x, y, width, height);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(AddPath)(IAVSGraphicsPath* pPath)
	{
		if (NULL == pPath)
			return S_FALSE;

		BOOL bRes = m_oPath.AddPath(((CAVSGraphicsPath*)pPath)->m_oPath);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(AddPolygon)(LPSAFEARRAY lpArray)
	{
		if (NULL == lpArray)
			return S_FALSE;

		LONG lCount = lpArray->rgsabound[0].cElements;
		double* pData = (double*)lpArray->pvData;

		BOOL bRes = m_oPath.AddLines(pData, lCount);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(AddRectangle)(double x, double y, double width, double height)
	{
		BOOL bRes = m_oPath.AddRectangle(x, y, width, height);
		return bRes ? S_OK : S_FALSE;
	}
	
	STDMETHOD(AddString)(BSTR bstrText, IAVSFontManager* pFont, double x, double y)
	{
		pFont->LoadString(bstrText, (float)x, (float)y);

		IAVSGraphicsBase* pNew = NULL;
		pFont->GetStringPath2(&pNew);

		AddPath((IAVSGraphicsPath*)pNew);

		RELEASEINTERFACE(pNew);

		return S_OK;
	}
	STDMETHOD(AddString2)(BSTR bstrText, IAVSFontManager* pFont, IAVSStringFormat* pFormat, double x, double y, double width, double height)
	{
		return S_OK;
	}

	STDMETHOD(PathCommandMoveTo)(double fX, double fY)
	{
		BOOL bRes = m_oPath.PathCommandMoveTo(fX, fY);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandLineTo)(double fX, double fY)
	{
		BOOL bRes = m_oPath.PathCommandLineTo(fX, fY);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandLinesTo)(LPSAFEARRAY lpArray)
	{
		if (NULL == lpArray)
			return S_FALSE;

		LONG lCount = lpArray->rgsabound[0].cElements;
		double* pData = (double*)lpArray->pvData;

		BOOL bRes = m_oPath.PathCommandLinesTo(pData, lCount);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		BOOL bRes = (0 == m_oPath.CurveTo(fX1, fY1, fX2, fY2, fX3, fY3));
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandCurvesTo)(LPSAFEARRAY lpArray)
	{
		if (NULL == lpArray)
			return S_FALSE;

		LONG lCount = lpArray->rgsabound[0].cElements;
		double* pData = (double*)lpArray->pvData;

		BOOL bRes = m_oPath.PathCommandCurvesTo(pData, lCount);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		BOOL bRes = m_oPath.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandClose)()
	{
		BOOL bRes = m_oPath.PathCommandClose();
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandEnd)()
	{
		BOOL bRes = m_oPath.PathCommandEnd();
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandStart)()
	{
		BOOL bRes = m_oPath.PathCommandStart();
		return bRes ? S_OK : S_FALSE;
	}
	STDMETHOD(GetCurrentPoint)(double* fX, double* fY)
	{
		if (NULL == fX || NULL == fY)
			return S_FALSE;

		double x = 0;
		double y = 0;

		m_oPath.GetCurPoint(fX, fY);
		return S_OK;
	}

	STDMETHOD(Pattern)(IAVSGraphicsBase* pObject, IAVSPattern** ppPattern)
	{
		return m_oPath.Pattern(pObject, ppPattern);
	}
};