#pragma once


#include "Shapes/Shape.h"
#include "Shapes/BaseShape/Path.h"
#include "../../DesktopEditor/graphics/Matrix.h"
#include "../../DesktopEditor/graphics/IRenderer.h"

namespace Aggplus
{
	class CMatrix;
	class CGraphicsPathSimpleConverter;
}
class CFontManager;

namespace NSPresentationEditor
{
	//#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
	//#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
	//#endif

	//// ISVGConverter
	//[ object, uuid("4EE9D2C3-7317-4540-A205-DB78DBEB9BD0"), dual, pointer_default(unique) ]
	//__interface ISVGConverter : IASCRenderer
	//{
	//	[id(5000)] HRESULT OpenFile([in] BSTR bsFileName);
	//	[id(5001)] HRESULT CloseFile();
	//};

	//// _ISVGConverter
	//[uuid("8C6949B0-EEC9-45a8-AA70-B832204A423F"), dispinterface]
	//__interface _ISVGConverterEvents
	//{
	//};

	// CSVGConverter
	//[ coclass, default(ISVGConverter), threading(apartment), event_source(com), vi_progid("Drawing.svgcon"), progid("Drawing.svgcon.1"), version(1.0), uuid("3779970A-DA08-4245-866D-40C1779B48B0") ]
	class /*ATL_NO_VTABLE*/ CSVGConverter : public IRenderer//: public ISVGConverter
	{
	private:

	//public:
	//	__event __interface _ISVGConverterEvents;

	public:
		//DECLARE_PROTECT_FINAL_CONSTRUCT()
		
		CSVGConverter();
		~CSVGConverter();

	public:
	// тип рендерера-----------------------------------------------------------------------------
		virtual HRESULT get_Type(LONG* lType)				;
	//-------- Функции для работы со страницей --------------------------------------------------
		virtual HRESULT NewPage()							;
		virtual HRESULT get_Height(double* dHeight)			;
		virtual HRESULT put_Height(const double& dHeight)	;
		virtual HRESULT get_Width(double* dWidth)			;
		virtual HRESULT put_Width(const double& dWidth)		;
		virtual HRESULT get_DpiX(double* dDpiX)				;
		virtual HRESULT get_DpiY(double* dDpiY)				;

	// pen --------------------------------------------------------------------------------------
		virtual HRESULT get_PenColor(LONG* lColor)					;
		virtual HRESULT put_PenColor(const LONG& lColor)			;
		virtual HRESULT get_PenAlpha(LONG* lAlpha)					;
		virtual HRESULT put_PenAlpha(const LONG& lAlpha)			;
		virtual HRESULT get_PenSize(double* dSize)					;
		virtual HRESULT put_PenSize(const double& dSize)			;
		virtual HRESULT get_PenDashStyle(BYTE* val)					;
		virtual HRESULT put_PenDashStyle(const BYTE& val)			;
		virtual HRESULT get_PenLineStartCap(BYTE* val)				;
		virtual HRESULT put_PenLineStartCap(const BYTE& val)		;
		virtual HRESULT get_PenLineEndCap(BYTE* val)				;
		virtual HRESULT put_PenLineEndCap(const BYTE& val)			;
		virtual HRESULT get_PenLineJoin(BYTE* val)					;
		virtual HRESULT put_PenLineJoin(const BYTE& val)			;
		virtual HRESULT get_PenDashOffset(double* dOffset)			;
		virtual HRESULT put_PenDashOffset(const double& dOffset)	;
		virtual HRESULT get_PenAlign(LONG* lAlign)					;
		virtual HRESULT put_PenAlign(const LONG& lAlign)			;
		virtual HRESULT get_PenMiterLimit(double* dOffset)			;
		virtual HRESULT put_PenMiterLimit(const double& dOffset)	;
		virtual HRESULT PenDashPattern(double* pPattern, LONG lCount);

	// brush ------------------------------------------------------------------------------------
		virtual HRESULT get_BrushType(LONG* lType)							;
		virtual HRESULT put_BrushType(const LONG& lType)					;
		virtual HRESULT get_BrushColor1(LONG* lColor)						;
		virtual HRESULT put_BrushColor1(const LONG& lColor)					;
		virtual HRESULT get_BrushAlpha1(LONG* lAlpha)						;
		virtual HRESULT put_BrushAlpha1(const LONG& lAlpha)					;
		virtual HRESULT get_BrushColor2(LONG* lColor)						;
		virtual HRESULT put_BrushColor2(const LONG& lColor)					;
		virtual HRESULT get_BrushAlpha2(LONG* lAlpha)						;
		virtual HRESULT put_BrushAlpha2(const LONG& lAlpha)					;
		virtual HRESULT get_BrushTexturePath(std::wstring* bsPath)			; 
		virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath)	; 
		virtual HRESULT get_BrushTextureMode(LONG* lMode)					;
		virtual HRESULT put_BrushTextureMode(const LONG& lMode)				;
		virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)				;
		virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha)			;
		virtual HRESULT get_BrushLinearAngle(double* dAngle)				;
		virtual HRESULT put_BrushLinearAngle(const double& dAngle)			;
		virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height) ;
		virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height) { return S_OK;}

		virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount) { return S_OK;}

	// font -------------------------------------------------------------------------------------
		virtual HRESULT get_FontName(std::wstring* bsName)			;
		virtual HRESULT put_FontName(const std::wstring& bsName)	;
		virtual HRESULT get_FontPath(std::wstring* bsName)			;
		virtual HRESULT put_FontPath(const std::wstring& bsName)	;
		virtual HRESULT get_FontSize(double* dSize)					;
		virtual HRESULT put_FontSize(const double& dSize)			;
		virtual HRESULT get_FontStyle(LONG* lStyle)					;
		virtual HRESULT put_FontStyle(const LONG& lStyle)			;
		virtual HRESULT get_FontStringGID(INT* bGID)				;
		virtual HRESULT put_FontStringGID(const INT& bGID)			;
		virtual HRESULT get_FontCharSpace(double* dSpace)			;
		virtual HRESULT put_FontCharSpace(const double& dSpace)		;
		virtual HRESULT get_FontFaceIndex(int* lFaceIndex)			{ return S_OK;}
		virtual HRESULT put_FontFaceIndex(const int& lFaceIndex)	{ return S_OK;}

	//-------- Функции для вывода текста --------------------------------------------------------
		virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) { return S_OK;}
		virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) ;
		
		virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) { return S_OK;}
		virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) ;

	//-------- Маркеры для команд ---------------------------------------------------------------
		virtual HRESULT BeginCommand(const DWORD& lType)	;
		virtual HRESULT EndCommand(const DWORD& lType)		;

	//-------- Функции для работы с Graphics Path -----------------------------------------------
		virtual HRESULT PathCommandMoveTo(const double& x, const double& y)			;
		virtual HRESULT PathCommandLineTo(const double& x, const double& y)			;
		virtual HRESULT PathCommandLinesTo(double* points, const int& count)		;
		virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)	 ;
		virtual HRESULT PathCommandCurvesTo(double* points, const int& count)		;
		virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle) ;
		virtual HRESULT PathCommandClose()											;
		virtual HRESULT PathCommandEnd()											;
		virtual HRESULT DrawPath(const LONG& nType)									;
		virtual HRESULT PathCommandStart()											;
		virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y)			;

		virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) { return S_OK;}
		virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) ;
		
		virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) { return S_OK;}
		virtual HRESULT PathCommandTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) ;

	//-------- Функции для вывода изображений ---------------------------------------------------
		virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)		;
		virtual HRESULT DrawImageFromFile(const std::wstring& fn, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255)	;	

	// transform --------------------------------------------------------------------------------
		virtual HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags);
		virtual HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags);
		virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6) ;
		virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)	;
		virtual HRESULT ResetTransform() ;

	// -----------------------------------------------------------------------------------------
		virtual HRESULT get_ClipMode(LONG* plMode)			;
		virtual HRESULT put_ClipMode(const LONG& lMode)		;

	// additiaonal params ----------------------------------------------------------------------
		virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand)			{ return S_OK;}
		virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand)		{ return S_OK;}
		virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand)	{ return S_OK;}	

	// additiaonal params ----------------------------------------------------------------------
		HRESULT SetAdditionalParam (BSTR ParamName, VARIANT ParamValue);
		HRESULT GetAdditionalParam (BSTR ParamName, VARIANT* ParamValue);
	// --------------------------------------------------------------------------------------------

	// file
		HRESULT OpenFile (BSTR bsFilePath);
		HRESULT CloseFile ();
	// additional 

		HRESULT get_ShadowDistanceX (double* val);
		HRESULT put_ShadowDistanceX (const double& val);
		HRESULT get_ShadowDistanceY (double* val);
		HRESULT put_ShadowDistanceY (const double& val);
		HRESULT get_ShadowBlurSize	(double* val);
		HRESULT put_ShadowBlurSize	(const double& val);
		HRESULT get_ShadowColor		(LONG* val);
		HRESULT put_ShadowColor		(const LONG& val);
		HRESULT get_ShadowAlpha		(LONG* val);
		HRESULT put_ShadowAlpha		(const LONG&  val);
		HRESULT get_ShadowVisible	(INT* val);
		HRESULT put_ShadowVisible	(const INT& val);
		// edge -------------------------------------------------------------------------------------
		HRESULT get_EdgeVisible		(LONG* val);
		HRESULT put_EdgeVisible		(const LONG& val);
		HRESULT get_EdgeColor		(LONG* val);
		HRESULT put_EdgeColor		(const LONG& val);
		HRESULT get_EdgeAlpha		(LONG* val);
		HRESULT put_EdgeAlpha		(const LONG& val);
		HRESULT get_EdgeDist		(double* val);
		HRESULT put_EdgeDist		(const double& val);
	private:
		
		Aggplus::CGraphicsPathSimpleConverter*		m_pSimpleGraphicsConverter;		// конвертер сложных гафических путей в простые
		CFontManager*								m_pFontManager;					// менеджер шрифтов

		Aggplus::CMatrix							m_oBaseTransform;	// матрица перерасчета координатных осей (здесь: миллиметры -> пикселы)
		Aggplus::CMatrix							m_oTransform;		// текущая матрица преобразований рендерера
		Aggplus::CMatrix							m_oFullTransform;	// полная матрица преобразований (m_oBaseTransform * m_oTransform)

		double							m_dTransformAngle;

		LONG							m_lCurrentCommandType;	// текущая команда

		double							m_dDpiX;				
		double							m_dDpiY;

		LONG							m_lClipMode;

		CPen							m_oPen;				// настройки всей графики (скопирован ашник из AVSGraphics)
		CBrush							m_oBrush;
		CFont							m_oFont;
		CShadow							m_oShadow;
		CEdgeText						m_oEdge;

		CFont							m_oInstalledFont;

		// шейп для конвертации
		CPath							m_oSvgPath;
		CBrush							m_oSvgBrush;
		CPen							m_oSvgPen;

		double							m_dWidth;
		double							m_dHeight;

		double							m_dOffsetX;
		double							m_dOffsetY;
		double							m_dOffsetW;
		double							m_dOffsetH;

		bool							m_bIsStrokeScheme;
		bool							m_bIsFillScheme;

	protected:
		void CalculateFullTransform()
		{
			m_oFullTransform	= m_oBaseTransform;
			m_oFullTransform.Multiply(&m_oTransform, Aggplus::MatrixOrderPrepend);

			m_dTransformAngle	= m_oTransform.z_Rotation();
		}

		inline void MoveTo(const double& dX, const double& dY)
		{
			//CPartPath oPart;
			//m_oSvgPath.m_arParts.Add(oPart);

			double x = dX;
			double y = dY;
			m_oFullTransform.TransformPoint(x, y);

			int lIndexPath = m_oSvgPath.m_arParts.size() - 1;
			if (lIndexPath < 0)
				return;

			CSlice oSlice(rtMoveTo);
			
			oSlice.AddParam(x);
			oSlice.AddParam(y);

			m_oSvgPath.m_arParts[lIndexPath].m_arSlices.push_back(oSlice);
		}
		inline void LineTo(const double& dX, const double& dY)
		{
			double x = dX;
			double y = dY;
			m_oFullTransform.TransformPoint(x, y);

			int lIndexPath = m_oSvgPath.m_arParts.size() - 1;
			if (lIndexPath < 0)
				return;
			
			CSlice oSlice(rtLineTo);
			
			oSlice.AddParam(x);
			oSlice.AddParam(y);

			m_oSvgPath.m_arParts[lIndexPath].m_arSlices.push_back(oSlice);
		}
		inline void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
		{
			double _x1 = x1;
			double _y1 = y1;
			m_oFullTransform.TransformPoint(_x1, _y1);

			double _x2 = x2;
			double _y2 = y2;
			m_oFullTransform.TransformPoint(_x2, _y2);

			double _x3 = x3;
			double _y3 = y3;
			m_oFullTransform.TransformPoint(_x3, _y3);

			int lIndexPath = m_oSvgPath.m_arParts.size() - 1;
			if (lIndexPath < 0)
				return;
			
			CSlice oSlice(rtCurveTo);

			oSlice.AddParam(_x1);
			oSlice.AddParam(_y1);
			oSlice.AddParam(_x2);
			oSlice.AddParam(_y2);
			oSlice.AddParam(_x3);
			oSlice.AddParam(_y3);

			m_oSvgPath.m_arParts[lIndexPath].m_arSlices.push_back(oSlice);
		}
		void Start()
		{
			CPartPath oPart;
			m_oSvgPath.m_arParts.push_back(oPart);
		}
		void End()
		{
		}
		void Close()
		{
			int lIndexPath = m_oSvgPath.m_arParts.size() - 1;
			if (lIndexPath < 0)
				return;

			CSlice oSlice(rtClose);
			m_oSvgPath.m_arParts[lIndexPath].m_arSlices.push_back(oSlice);
		}

		void _SetFont();

	public:
		void SetOffsetXY(double dX, double dY, double dW, double dH)
		{
			/*
			m_dOffsetX = dX * 96 / 25.4;
			m_dOffsetY = dY * 96 / 25.4;
			m_dOffsetW = dW * 96 / 25.4;
			m_dOffsetH = dH * 96 / 25.4;
			*/
			m_dOffsetX = dX;
			m_dOffsetY = dY;
			m_dOffsetW = dW;
			m_dOffsetH = dH;

			m_bIsFillScheme = false;
			m_bIsStrokeScheme = false;
		}
		void SetSettings(CPen& oPen, CBrush& oBrush, CPath& oPath)
		{
			oPen		= m_oSvgPen;
			oBrush		= m_oSvgBrush;
			oPath		= m_oSvgPath;
		}
	};
}