#pragma once

#include "IASCPen.h"
#include "IASCBrush.h"
#include "IASCGraphicsPath.h"
#include "IASCGraphicsState.h"
#include "IASCImage.h"
#include "IASCStringFormat.h"
#include "IASCFontManager.h"

[object, uuid("AEC96193-2754-4340-9AEB-365D539C7044"), dual, pointer_default(unique)]
__interface IAVSGraphics: IAVSGraphicsBase
{
	// pPixels - пикселы для рисования (могут быть получены откуда угодно - картинка, контекст устройства)
	// важен параметр lPitch - для возможности создания IAVSGraphics из IAVSGraphics,
	// чтобы в любой момент мы могли задать dpi любой области вывода. Этой возможности нет в gdi,
	// там это реализовано с помощью метафайлов. У нас метафайл будет рисовать прямо на графиксе, с выставленным lPitch
	[id(001)]			HRESULT Create([in] BYTE* pPixels, [in] LONG lWidth, [in] LONG lHeight, [in] LONG lStride, [in] LONG lPitch, [in] LONG lColorSpace);

	// этот метод отдает часть для отрисовки в ней. (см описание [id(001)])
	[id(002)]			HRESULT CreatePart([in] LONG lLeft, [in] LONG lTop, [in] LONG lWidth, [in] LONG lHeight, [out, retval] IAVSGraphics** ppPart);


	// функции для работы с DPI, системой координат
	[id(101), propget]	HRESULT DpiX([out, retval] double* lDpiX);
	[id(101), propput]	HRESULT DpiX([in] double lDpiX);

	[id(102), propget]	HRESULT DpiY([out, retval] double* lDpiY);
	[id(102), propput]	HRESULT DpiY([in] double lDpiY);

	[id(110)]			HRESULT SetPageWidth([in] double lWidth, [in] LONG lUnit);
	[id(111)]			HRESULT SetPageHeight([in] double lHeight, [in] LONG lUnit);
	
	[id(112), propget]	HRESULT PageUnit([out, retval] LONG* lUnit);
	[id(112), propput]	HRESULT PageUnit([in] LONG lUnit);

	[id(120), propget]	HRESULT Transform([out, retval] IAVSMatrix** ppTransform);
	[id(120), propput]	HRESULT Transform([in] IAVSMatrix* pTransform);

	[id(121), propget]	HRESULT BaseTransform([out, retval] IAVSMatrix** ppTransform);
	[id(121), propput]	HRESULT BaseTransform([in] IAVSMatrix* pTransform);

	[id(123)]			HRESULT TranslateTransform([in] double dX, [in] double dY, [in] LONG lMatrixOrder);
	[id(124)]			HRESULT RotateTransform([in] double dAngle, [in] LONG lMatrixOrder);
	[id(125)]			HRESULT ScaleTransform([in] double dScaleX, [in] double dScaleY, [in] LONG lMatrixOrder);

	[id(126)]			HRESULT MultiplyTransform([in] IAVSMatrix* pTransform, [in] LONG lMatrixOrder);

	[id(131)]			HRESULT Save([out, retval] IAVSGraphicsState** pState);
	[id(132)]			HRESULT Restore([in] IAVSGraphicsState* pState);

	// функции отсечения
	[id(200)]			HRESULT SetClip1([in] double dLeft, [in] double dTop, [in] double dWidth, [in] double dHeight, [in] LONG lCombineMode);
	[id(201)]			HRESULT SetClip2([in] IAVSGraphicsPath* pPath, [in] LONG lCombineMode);
	[id(202)]			HRESULT SetClip3([in] IUnknown* pRegion, [in] LONG lCombineMode);
	[id(251)]			HRESULT ExludeClip1([in] double dLeft, [in] double dTop, [in] double dWidth, [in] double dHeight, [in] LONG lCombineMode);
	[id(252)]			HRESULT ExludeClip2([in] IAVSGraphicsPath* pPath, [in] LONG lCombineMode);
	[id(253)]			HRESULT ExludeClip3([in] IUnknown* pRegion, [in] LONG lCombineMode);


	// измерение текста
	[id(300)]			HRESULT MeasureString([in] BSTR bstrText, [in] IAVSFontManager* pManager, [out] double* lWidth, [out] double* lHeight);

	// тут еще надо продумать зачеркивание, подчеркивание. реализовывать нужно, понятно, в методе DrawString

	[id(350)]			HRESULT Clear([in] LONG lColor);

	// настройки отрисовщика
	[id(401), propget]	HRESULT SmoothingMode([out, retval] LONG* lMode);
	[id(401), propput]	HRESULT SmoothingMode([in] LONG lMode);

	[id(402), propget]	HRESULT TextRenderingHint([out, retval] LONG* lMode);
	[id(402), propput]	HRESULT TextRenderingHint([in] LONG lMode);

	[id(403), propget]	HRESULT CompositingMode([out, retval] LONG* lMode);
	[id(403), propput]	HRESULT CompositingMode([in] LONG lMode);

	[id(404), propget]	HRESULT CompositingQuality([out, retval] LONG* lMode);
	[id(404), propput]	HRESULT CompositingQuality([in] LONG lMode);

	[id(405), propget]	HRESULT InterpolationMode([out, retval] LONG* lMode);
	[id(405), propput]	HRESULT InterpolationMode([in] LONG lMode);

	[id(406), propget]	HRESULT PixelOffsetMode([out, retval] LONG* lMode);
	[id(406), propput]	HRESULT PixelOffsetMode([in] LONG lMode);

	// отрисовка примитивов (без заливки)
	[id(500)]			HRESULT DrawArc([in] IAVSPen* pPen, [in] double x, [in] double y, [in] double width, [in] double height, [in] double startAngle, [in] double sweepAngle);
	[id(501)]			HRESULT DrawBezier([in] IAVSPen* pPen, [in] double x1, [in] double y1, [in] double x2, [in] double y2, [in] double x3, [in] double y3, [in] double x4, [in] double y4);
	[id(502)]			HRESULT DrawBeziers([in] IAVSPen* pPen, [in,  satype("double")] LPSAFEARRAY ppPoints);
	[id(503)]			HRESULT DrawCurve([in] IAVSPen* pPen, [in,  satype("double")] LPSAFEARRAY ppPoints);
	[id(504)]			HRESULT DrawLine([in] IAVSPen* pPen, [in] double x1, [in] double y1, [in] double x2, [in] double y2);
    [id(505)]			HRESULT DrawLines([in] IAVSPen* pPen, [in,  satype("double")] LPSAFEARRAY ppPoints);
	[id(506)]			HRESULT DrawEllipse([in] IAVSPen* pPen, [in] double x, [in] double y, [in] double width, [in] double height);
	[id(507)]			HRESULT DrawPath([in] IAVSPen* pPen, [in] IAVSGraphicsPath* pPath);

	// заливка
	[id(550)]			HRESULT FillEllipse([in] IAVSBrush* pBrush, [in] double x, [in] double y, [in] double width, [in] double height);
	[id(551)]			HRESULT FillPath([in] IAVSBrush* pBrush, [in] IAVSGraphicsPath* pPath);
	[id(552)]			HRESULT FillPolygon([in] IAVSBrush* pBrush, [in,  satype("double")] LPSAFEARRAY ppPoints);

	// отрисовка картинки
	[id(591)]			HRESULT DrawImage([in] IAVSImage* pImage, [in] double x, [in] double y, [in] double width, [in] double height);
	[id(592)]			HRESULT DrawImage2([in] IAVSImage* pImage, [in,  satype("double")] LPSAFEARRAY ppPoints);
	[id(593)]			HRESULT DrawImageUnscaled([in] IAVSImage* pImage, [in] double x, [in] double y);

	// отрисовка текста
	[id(600)]			HRESULT DrawString([in] BSTR bstrText, [in] IAVSFontManager* pFont, [in] IAVSBrush* pBrush, [in] double x, [in] double y);
	[id(601)]			HRESULT DrawString2([in] BSTR bstrText, [in] IAVSFontManager* pFont, [in] IAVSBrush* pBrush, 
														[in] IAVSStringFormat* pFormat, [in] double x, [in] double y, [in] double width, [in] double height);
};