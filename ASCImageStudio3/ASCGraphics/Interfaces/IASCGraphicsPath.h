#pragma once
#include "../stdafx.h"

#include "IASCStringFormat.h"
#include "IASCFontManager.h"
#include "IASCPattern.h"

const long c_ag_FillModeAlternate			= 0;
const long c_ag_FillModeWinding				= 1;

[object, uuid("60F0549A-5963-40e2-B7FC-71EEC8E1C8CC"), dual, pointer_default(unique)]
__interface IAVSGraphicsPath : IAVSGraphicsBase
{
	[id(100), propget]	HRESULT FillMode([out, retval] LONG* lMode);
	[id(100), propput]	HRESULT FillMode([in] LONG lMode);

	[id(101), propget]	HRESULT PathPoints([out, retval, satype("double")] SAFEARRAY** ppPoints);
	[id(102), propget]	HRESULT PointsCount([out, retval] LONG* lCount);

	[id(201)]			HRESULT StartFigure();
	[id(202)]			HRESULT CloseFigure();
	[id(203)]			HRESULT CloseAllFigures();

	[id(500)]			HRESULT AddArc([in] double x, [in] double y, [in] double width, [in] double height, [in] double startAngle, [in] double sweepAngle);
	[id(501)]			HRESULT AddBezier([in] double x1, [in] double y1, [in] double x2, [in] double y2, [in] double x3, [in] double y3, [in] double x4, [in] double y4);
	[id(502)]			HRESULT AddBeziers([in,  satype("double")] SAFEARRAY* ppPoints);
	[id(503)]			HRESULT AddCurve([in,  satype("double")] SAFEARRAY* ppPoints);
	[id(504)]			HRESULT AddLine([in] double x1, [in] double y1, [in] double x2, [in] double y2);
    [id(505)]			HRESULT AddLines([in,  satype("double")] SAFEARRAY* ppPoints);
	[id(506)]			HRESULT AddEllipse([in] double x, [in] double y, [in] double width, [in] double height);
	[id(507)]			HRESULT AddPath([in] IAVSGraphicsPath* pPath);
	[id(508)]			HRESULT AddPolygon([in,  satype("double")] SAFEARRAY* ppPoints);
	[id(509)]			HRESULT AddRectangle([in] double x, [in] double y, [in] double width, [in] double height);
	
	[id(600)]			HRESULT AddString([in] BSTR bstrText, [in] IAVSFontManager* pFont, [in] double x, [in] double y);
	[id(601)]			HRESULT AddString2([in] BSTR bstrText, [in] IAVSFontManager* pFont, [in] IAVSStringFormat* pFormat, [in] double x, [in] double y, [in] double width, [in] double height);

	[id(701)]			HRESULT PathCommandMoveTo  ([in] double fX,  [in] double fY);
	[id(702)]			HRESULT PathCommandLineTo  ([in] double fX,  [in] double fY);
	[id(703)]			HRESULT PathCommandLinesTo ([in,  satype("double")] SAFEARRAY* ppPoints);
	[id(704)]			HRESULT PathCommandCurveTo ([in] double fX1, [in] double fY1, [in] double fX2, [in] double fY2, [in] double fX3, [in] double fY3);
	[id(705)]			HRESULT PathCommandCurvesTo([in,  satype("double")] SAFEARRAY* ppPoints);
	[id(706)]			HRESULT PathCommandArcTo   ([in] double fX,  [in] double fY, [in] double fWidth, [in] double fHeight, [in] double fStartAngle, [in] double fSweepAngle);
	[id(707)]			HRESULT PathCommandClose();
	[id(708)]			HRESULT PathCommandEnd();
	[id(709)]			HRESULT PathCommandStart();
	[id(710)]			HRESULT GetCurrentPoint([out] double* fX, [out] double* fY);

	[id(800)]			HRESULT Pattern([in] IAVSGraphicsBase* pObject, [out, retval] IAVSPattern** ppPattern);
};

class ISimpleGraphicsPath
{
public:
	virtual bool _MoveTo(double x, double y)													= 0;
	virtual bool _LineTo(double x, double y)													= 0;
	virtual bool _CurveTo(double x1, double y1, double x2, double y2, double x3, double y3)		= 0;
	virtual bool _Close()																		= 0;
};