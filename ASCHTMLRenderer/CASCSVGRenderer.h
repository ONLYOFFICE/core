#ifndef C_ASC_SVG_RENDERER
#define C_ASC_SVG_RENDERER

#include "../DesktopEditor/graphics/IRenderer.h"

#ifdef _WIN32
#include <atlbase.h>
#include <atlstr.h>
#else
#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

namespace Aggplus
{
	class CMatrix;
	class CGraphicsPathSimpleConverter;
}
class CFontManager;
namespace NSStrings
{
	class CStringWriter;
}
namespace NSHtmlRenderer
{
	class CSVGGraphicsWriter;
}
namespace NSStructures
{
	class CPen;
	class CBrush;
	class CFont;
}
namespace NSHtmlRenderer
{
	class IASCSVGRenderer : public IRenderer
	{
	public:
		virtual HRESULT CreateOfficeFile(const std::wstring& bsFileName, const LONG& lFonts) = 0;
		virtual HRESULT CloseFile(const LONG& lSave) = 0;
		virtual HRESULT get_Data(std::wstring* pbstrSVG) = 0; 
	};

	class CASCSVGRenderer : public IASCSVGRenderer
	{
	public:
		CASCSVGRenderer();
		~CASCSVGRenderer();

	public:
		// тип рендерера-----------------------------------------------------------------------------
		HRESULT get_Type(LONG* lType);
		//-------- Функции для работы со страницей --------------------------------------------------
		HRESULT NewPage();
		HRESULT get_Height(double* dHeight);
		HRESULT put_Height(const double& dHeight);
		HRESULT get_Width(double* dWidth);
		HRESULT put_Width(const double& dWidth);
		HRESULT get_DpiX(double* dDpiX);
		HRESULT get_DpiY(double* dDpiY);

		// pen --------------------------------------------------------------------------------------
		HRESULT get_PenColor(LONG* lColor);
		HRESULT put_PenColor(const LONG& lColor);
		HRESULT get_PenAlpha(LONG* lAlpha);
		HRESULT put_PenAlpha(const LONG& lAlpha);
		HRESULT get_PenSize(double* dSize);
		HRESULT put_PenSize(const double& dSize);
		HRESULT get_PenDashStyle(BYTE* val);
		HRESULT put_PenDashStyle(const BYTE& val);
		HRESULT get_PenLineStartCap(BYTE* val);
		HRESULT put_PenLineStartCap(const BYTE& val);
		HRESULT get_PenLineEndCap(BYTE* val);
		HRESULT put_PenLineEndCap(const BYTE& val);
		HRESULT get_PenLineJoin(BYTE* val);
		HRESULT put_PenLineJoin(const BYTE& val);
		HRESULT get_PenDashOffset(double* dOffset);
		HRESULT put_PenDashOffset(const double& dOffset);
		HRESULT get_PenAlign(LONG* lAlign);
		HRESULT put_PenAlign(const LONG& lAlign);
		HRESULT get_PenMiterLimit(double* dOffset);
		HRESULT put_PenMiterLimit(const double& dOffset);
		HRESULT PenDashPattern(double* pPattern, LONG lCount);

		// brush ------------------------------------------------------------------------------------
		HRESULT get_BrushType(LONG* lType);
		HRESULT put_BrushType(const LONG& lType);
		HRESULT get_BrushColor1(LONG* lColor);
		HRESULT put_BrushColor1(const LONG& lColor);
		HRESULT get_BrushAlpha1(LONG* lAlpha);
		HRESULT put_BrushAlpha1(const LONG& lAlpha);
		HRESULT get_BrushColor2(LONG* lColor);
		HRESULT put_BrushColor2(const LONG& lColor);
		HRESULT get_BrushAlpha2(LONG* lAlpha);
		HRESULT put_BrushAlpha2(const LONG& lAlpha);
		HRESULT get_BrushTexturePath(std::wstring* bsPath);
		HRESULT put_BrushTexturePath(const std::wstring& bsPath);
		HRESULT get_BrushTextureMode(LONG* lMode);
		HRESULT put_BrushTextureMode(const LONG& lMode);
		HRESULT get_BrushTextureAlpha(LONG* lTxAlpha);
		HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha);
		HRESULT get_BrushLinearAngle(double* dAngle);
		HRESULT put_BrushLinearAngle(const double& dAngle);
		HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height);
		HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height);

		HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount);

		// font -------------------------------------------------------------------------------------
		HRESULT get_FontName(std::wstring* bsName);
		HRESULT put_FontName(const std::wstring& bsName);
		HRESULT get_FontPath(std::wstring* bsName);
		HRESULT put_FontPath(const std::wstring& bsName);
		HRESULT get_FontSize(double* dSize);
		HRESULT put_FontSize(const double& dSize);
		HRESULT get_FontStyle(LONG* lStyle)	;
		HRESULT put_FontStyle(const LONG& lStyle);
		HRESULT get_FontStringGID(INT* bGID);
		HRESULT put_FontStringGID(const INT& bGID);
		HRESULT get_FontCharSpace(double* dSpace);
		HRESULT put_FontCharSpace(const double& dSpace);
		HRESULT get_FontFaceIndex(int* lFaceIndex);
		HRESULT put_FontFaceIndex(const int& lFaceIndex);

		//-------- Функции для вывода текста --------------------------------------------------------
		HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset);
		HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset);

		HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags);
		HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags);

		//-------- Маркеры для команд ---------------------------------------------------------------
		HRESULT BeginCommand(const DWORD& lType);
		HRESULT EndCommand(const DWORD& lType);

		//-------- Функции для работы с Graphics Path -----------------------------------------------
		HRESULT PathCommandMoveTo(const double& x, const double& y);
		HRESULT PathCommandLineTo(const double& x, const double& y);
		HRESULT PathCommandLinesTo(double* points, const int& count);
		HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
		HRESULT PathCommandCurvesTo(double* points, const int& count);
		HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle);
		HRESULT PathCommandClose();
		HRESULT PathCommandEnd();
		HRESULT DrawPath(const LONG& nType);
		HRESULT PathCommandStart();
		HRESULT PathCommandGetCurrentPoint(double* x, double* y);

		HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset);
		HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset);

		HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags);
		HRESULT PathCommandTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags);

		//-------- Функции для вывода изображений ---------------------------------------------------
		HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h);
		HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha);

		// transform --------------------------------------------------------------------------------
		HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6);
		HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
		HRESULT ResetTransform();

		// -----------------------------------------------------------------------------------------
		HRESULT get_ClipMode(LONG* plMode);
		HRESULT put_ClipMode(const LONG& lMode);
		// additiaonal params ----------------------------------------------------------------------
		HRESULT SaveFile(const std::wstring& strFileSave);
		HRESULT ReInit();
		HRESULT IsRaster(bool* bVal);
		// additiaonal params ----------------------------------------------------------------------
		HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
		HRESULT CommandDouble(const LONG& lType, const double& dCommand);
		HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);
		// --------------------------------------------------------------------------------------------

		// file
		HRESULT CreateOfficeFile(const std::wstring& bsFileName, const LONG& lFonts);
		HRESULT CloseFile(const LONG& lSave);
		HRESULT get_Data(std::wstring* pbstrSVG); 

	private:

		NSStrings::CStringWriter*		m_pWriter;
		NSStrings::CStringWriter*		m_pWriterVML;
		std::wstring					m_strDstFile;

		Aggplus::CGraphicsPathSimpleConverter*	m_pSimpleGraphicsConverter;		// конвертер сложных гафических путей в простые
		CFontManager*				m_pFontManager;					// менеджер шрифтов

		Aggplus::CMatrix*				m_pBaseTransform;	// матрица перерасчета координатных осей (здесь: миллиметры -> пикселы)
		Aggplus::CMatrix*				m_pTransform;		// текущая матрица преобразований рендерера
		Aggplus::CMatrix*				m_pFullTransform;	// полная матрица преобразований (m_oBaseTransform * m_oTransform)

		LONG							m_lCurrentCommandType;	// текущая команда

		double							m_dDpiX;				
		double							m_dDpiY;

		double							m_dWidth;
		double							m_dHeight;

		LONG							m_lClipMode;

		LONG							m_lColorSchemeIndexBrush1;
		LONG							m_lColorSchemeIndexPen1;

		NSStructures::CPen*				m_pPen;				// настройки всей графики (скопирован ашник из AVSGraphics)
		NSStructures::CBrush*			m_pBrush;
		NSStructures::CFont*			m_pFont;

		NSStructures::CFont*			m_pInstalledFont;

		//todo
		//IASCWinFonts*					m_pFonts;

	protected:
		void CalculateFullTransform();

		inline void MoveTo(const double& dX, const double& dY);
		inline void LineTo(const double& dX, const double& dY);
		inline void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
		inline void Start();
		void End();
		void Close();

		void StartPage(double& dWidth, double& dHeight);
		void EndPage();

		void OnBaseMatrixUpdate();

		void _SetFont();
	};
}
#endif