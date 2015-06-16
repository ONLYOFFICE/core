#ifndef _PDF_WRITER_PDFRENDERER_H
#define _PDF_WRITER_PDFRENDERER_H

#include "../DesktopEditor/graphics/IRenderer.h"

namespace PdfWriter
{
	class CDocument;
	class CPage;
};

class CPdfRenderer : public IRenderer
{
public:
	CPdfRenderer();
	~CPdfRenderer();

	// тип рендерера-----------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType);
	//-------- Функции для работы со страницей --------------------------------------------------
	virtual HRESULT NewPage();
	virtual HRESULT get_Height(double* dHeight);
	virtual HRESULT put_Height(const double& dHeight);
	virtual HRESULT get_Width(double* dWidth);
	virtual HRESULT put_Width(const double& dWidth);
	virtual HRESULT get_DpiX(double* dDpiX);
	virtual HRESULT get_DpiY(double* dDpiY);

	// pen --------------------------------------------------------------------------------------
	virtual HRESULT get_PenColor(LONG* lColor)					= 0;
	virtual HRESULT put_PenColor(const LONG& lColor)			= 0;
	virtual HRESULT get_PenAlpha(LONG* lAlpha)					= 0;
	virtual HRESULT put_PenAlpha(const LONG& lAlpha)			= 0;
	virtual HRESULT get_PenSize(double* dSize)					= 0;
	virtual HRESULT put_PenSize(const double& dSize)			= 0;
	virtual HRESULT get_PenDashStyle(BYTE* val)					= 0;
	virtual HRESULT put_PenDashStyle(const BYTE& val)			= 0;
	virtual HRESULT get_PenLineStartCap(BYTE* val)				= 0;
	virtual HRESULT put_PenLineStartCap(const BYTE& val)		= 0;
	virtual HRESULT get_PenLineEndCap(BYTE* val)				= 0;
	virtual HRESULT put_PenLineEndCap(const BYTE& val)			= 0;
	virtual HRESULT get_PenLineJoin(BYTE* val)					= 0;
	virtual HRESULT put_PenLineJoin(const BYTE& val)			= 0;
	virtual HRESULT get_PenDashOffset(double* dOffset)			= 0;
	virtual HRESULT put_PenDashOffset(const double& dOffset)	= 0;
	virtual HRESULT get_PenAlign(LONG* lAlign)					= 0;
	virtual HRESULT put_PenAlign(const LONG& lAlign)			= 0;
	virtual HRESULT get_PenMiterLimit(double* dOffset)			= 0;
	virtual HRESULT put_PenMiterLimit(const double& dOffset)	= 0;
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount)= 0;

	// brush ------------------------------------------------------------------------------------
	virtual HRESULT get_BrushType(LONG* lType)							= 0;
	virtual HRESULT put_BrushType(const LONG& lType)					= 0;
	virtual HRESULT get_BrushColor1(LONG* lColor)						= 0;
	virtual HRESULT put_BrushColor1(const LONG& lColor)					= 0;
	virtual HRESULT get_BrushAlpha1(LONG* lAlpha)						= 0;
	virtual HRESULT put_BrushAlpha1(const LONG& lAlpha)					= 0;
	virtual HRESULT get_BrushColor2(LONG* lColor)						= 0;
	virtual HRESULT put_BrushColor2(const LONG& lColor)					= 0;
	virtual HRESULT get_BrushAlpha2(LONG* lAlpha)						= 0;
	virtual HRESULT put_BrushAlpha2(const LONG& lAlpha)					= 0;
	virtual HRESULT get_BrushTexturePath(std::wstring* bsPath)			= 0;
	virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath)	= 0;
	virtual HRESULT get_BrushTextureMode(LONG* lMode)					= 0;
	virtual HRESULT put_BrushTextureMode(const LONG& lMode)				= 0;
	virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)				= 0;
	virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha)			= 0;
	virtual HRESULT get_BrushLinearAngle(double* dAngle)				= 0;
	virtual HRESULT put_BrushLinearAngle(const double& dAngle)			= 0;
	virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height) = 0;
	virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height) = 0;

	virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount) = 0;

	// font -------------------------------------------------------------------------------------
	virtual HRESULT get_FontName(std::wstring* bsName)			= 0;
	virtual HRESULT put_FontName(const std::wstring& bsName)	= 0;
	virtual HRESULT get_FontPath(std::wstring* bsName)			= 0;
	virtual HRESULT put_FontPath(const std::wstring& bsName)	= 0;
	virtual HRESULT get_FontSize(double* dSize)					= 0;
	virtual HRESULT put_FontSize(const double& dSize)			= 0;
	virtual HRESULT get_FontStyle(LONG* lStyle)					= 0;
	virtual HRESULT put_FontStyle(const LONG& lStyle)			= 0;
	virtual HRESULT get_FontStringGID(INT* bGID)				= 0;
	virtual HRESULT put_FontStringGID(const INT& bGID)			= 0;
	virtual HRESULT get_FontCharSpace(double* dSpace)			= 0;
	virtual HRESULT put_FontCharSpace(const double& dSpace)		= 0;
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex)			= 0;
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex)	= 0;

	//-------- Функции для вывода текста --------------------------------------------------------
	virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) = 0;
	virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) = 0;

	virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) = 0;
	virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) = 0;
	virtual HRESULT CommandDrawTextPdf(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const std::wstring& wsSrcCodeText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) { return 0; };

	//-------- Маркеры для команд ---------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType)	= 0;
	virtual HRESULT EndCommand(const DWORD& lType)		= 0;

	//-------- Функции для работы с Graphics Path -----------------------------------------------
	virtual HRESULT PathCommandMoveTo(const double& x, const double& y)			= 0;
	virtual HRESULT PathCommandLineTo(const double& x, const double& y)			= 0;
	virtual HRESULT PathCommandLinesTo(double* points, const int& count)		= 0;
	virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)	 = 0;
	virtual HRESULT PathCommandCurvesTo(double* points, const int& count)		= 0;
	virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle) = 0;
	virtual HRESULT PathCommandClose()											= 0;
	virtual HRESULT PathCommandEnd()											= 0;
	virtual HRESULT DrawPath(const LONG& nType)									= 0;
	virtual HRESULT PathCommandStart()											= 0;
	virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y)			= 0;

	virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) = 0;
	virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) = 0;

	virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) = 0;
	virtual HRESULT PathCommandTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) = 0;
	virtual HRESULT PathCommandTextPdf(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const std::wstring& bsSrcCodeText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) { return 0; };

	//-------- Функции для вывода изображений ---------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)		= 0;
	virtual HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255)	= 0;

	// transform --------------------------------------------------------------------------------
	virtual HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		return S_OK;
	}
	virtual HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		if ((dWidth <= 1) || (dHeight <= 1))
			lFlags = 0;

		INT bFlipX = (0 != (c_nParamFlipX & lFlags));
		INT bFlipY = (0 != (c_nParamFlipY & lFlags));

		float m11 = bFlipX ? -1.0f : 1.0f;
		float m22 = bFlipY ? -1.0f : 1.0f;

		Aggplus::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

		if ((0 != dAngle) || (0 != lFlags))
		{
			double dCentreX = (double)(dLeft + dWidth / 2.0);
			double dCentreY = (double)(dTop + dHeight / 2.0);

			oMatrix.Translate(-dCentreX, -dCentreY, Aggplus::MatrixOrderAppend);

			oMatrix.Rotate(dAngle, Aggplus::MatrixOrderAppend);
			oMatrix.Scale(m11, m22, Aggplus::MatrixOrderAppend);

			oMatrix.Translate(dCentreX, dCentreY, Aggplus::MatrixOrderAppend);
		}
		double mass[6];
		oMatrix.GetElements(mass);
		SetTransform(mass[0], mass[1], mass[2], mass[3], mass[4], mass[5]);
		return S_OK;
	}
	virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6) = 0;
	virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)	= 0;
	virtual HRESULT ResetTransform() = 0;

	// -----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* plMode)			= 0;
	virtual HRESULT put_ClipMode(const LONG& lMode)		= 0;

	// additiaonal params ----------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand)			= 0;
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand)		= 0;
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand)	= 0;

private:

	bool IsValid()
	{
		return m_bValid;
	}
	void SetError()
	{
		m_bValid = true;
	}

private:

	PdfWriter::CDocument* m_pDocument;
	PdfWriter::CPage*     m_pPage;

	bool                  m_bValid;
	double                m_dPageHeight;
	double                m_dPageWidth;
};

#endif // _PDF_WRITER_PDFRENDERER_H