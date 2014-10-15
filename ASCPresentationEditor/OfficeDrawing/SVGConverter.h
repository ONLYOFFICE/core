#pragma once
#include "../stdafx.h"

#include "Shapes\Shape.h"
#include "Shapes\BaseShape\Path.h"
#include "..\..\DesktopEditor\graphics\Matrix.h"
#include "../../ASCImageStudio3/ASCGraphics/Interfaces/ASCRenderer.h"

namespace NSPresentationEditor
{
	#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
	#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
	#endif

	// ISVGConverter
	[ object, uuid("4EE9D2C3-7317-4540-A205-DB78DBEB9BD0"), dual, pointer_default(unique) ]
	__interface ISVGConverter : IASCRenderer
	{
		[id(5000)] HRESULT OpenFile([in] BSTR bsFileName);
		[id(5001)] HRESULT CloseFile();
	};

	// _ISVGConverter
	[uuid("8C6949B0-EEC9-45a8-AA70-B832204A423F"), dispinterface]
	__interface _ISVGConverterEvents
	{
	};

	// CSVGConverter
	[ coclass, default(ISVGConverter), threading(apartment), event_source(com), vi_progid("Drawing.svgcon"), progid("Drawing.svgcon.1"), version(1.0), uuid("3779970A-DA08-4245-866D-40C1779B48B0") ]
	class ATL_NO_VTABLE CSVGConverter : 
		public ISVGConverter
	{
	private:

	public:
		__event __interface _ISVGConverterEvents;

	public:
		DECLARE_PROTECT_FINAL_CONSTRUCT()
		
		CSVGConverter();
		~CSVGConverter()
		{
		}

	public:
		// ------------------------------------ renderer --------------------------------------------
		STDMETHOD(get_Type)(LONG* lType);
		//-------- Функции для работы со страницей --------------------------------------------------
		STDMETHOD(NewPage)();
		STDMETHOD(get_Height)(double* dHeight);
		STDMETHOD(put_Height)(double dHeight);
		STDMETHOD(get_Width)(double* dWidth);
		STDMETHOD(put_Width)(double dWidth);

		STDMETHOD(get_DpiX)(double* dDpiX);
		STDMETHOD(get_DpiY)(double* dDpiY);
	// pen --------------------------------------------------------------------------------------
		STDMETHOD(SetPen)(BSTR bsXML);
		STDMETHOD(get_PenColor)(LONG* lColor);
		STDMETHOD(put_PenColor)(LONG lColor);
		STDMETHOD(get_PenAlpha)(LONG* lAlpha);
		STDMETHOD(put_PenAlpha)(LONG lAlpha);
		STDMETHOD(get_PenSize)(double* dSize);
		STDMETHOD(put_PenSize)(double dSize);
		STDMETHOD(get_PenDashStyle)(BYTE* val);
		STDMETHOD(put_PenDashStyle)(BYTE val);
		STDMETHOD(get_PenLineStartCap)(BYTE* val);
		STDMETHOD(put_PenLineStartCap)(BYTE val);
		STDMETHOD(get_PenLineEndCap)(BYTE* val);
		STDMETHOD(put_PenLineEndCap)(BYTE val);
		STDMETHOD(get_PenLineJoin)(BYTE* val);
		STDMETHOD(put_PenLineJoin)(BYTE val);
		STDMETHOD(get_PenDashOffset)(double* val);
		STDMETHOD(put_PenDashOffset)(double val);
		STDMETHOD(get_PenAlign)(LONG* val);
		STDMETHOD(put_PenAlign)(LONG val);
		STDMETHOD(get_PenMiterLimit)(double* val);
		STDMETHOD(put_PenMiterLimit)(double val);
		STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern);
	// brush ------------------------------------------------------------------------------------
		STDMETHOD(SetBrush)(BSTR bsXML);
		STDMETHOD(get_BrushType)(LONG* lType);
		STDMETHOD(put_BrushType)(LONG lType);
		STDMETHOD(get_BrushColor1)(LONG* lColor);
		STDMETHOD(put_BrushColor1)(LONG lColor);
		STDMETHOD(get_BrushAlpha1)(LONG* lAlpha);
		STDMETHOD(put_BrushAlpha1)(LONG lAlpha);
		STDMETHOD(get_BrushColor2)(LONG* lColor);
		STDMETHOD(put_BrushColor2)(LONG lColor);
		STDMETHOD(get_BrushAlpha2)(LONG* lAlpha);
		STDMETHOD(put_BrushAlpha2)(LONG lAlpha);
		STDMETHOD(get_BrushTexturePath)(BSTR* bsPath);
		STDMETHOD(put_BrushTexturePath)(BSTR bsPath);
		STDMETHOD(get_BrushTextureMode)(LONG* lMode);
		STDMETHOD(put_BrushTextureMode)(LONG lMode);
		STDMETHOD(get_BrushTextureAlpha)(LONG* lTxAlpha);
		STDMETHOD(put_BrushTextureAlpha)(LONG lTxAlpha);
		STDMETHOD(get_BrushLinearAngle)(double* dAngle);
		STDMETHOD(put_BrushLinearAngle)(double dAngle);
		STDMETHOD(BrushRect)(BOOL val, double left, double top, double width, double height);
	// font -------------------------------------------------------------------------------------
		STDMETHOD(SetFont)(BSTR bsXML);
		STDMETHOD(get_FontName)(BSTR* bsName);
		STDMETHOD(put_FontName)(BSTR bsName);
		STDMETHOD(get_FontPath)(BSTR* bsName);
		STDMETHOD(put_FontPath)(BSTR bsName);
		STDMETHOD(get_FontSize)(double* dSize);
		STDMETHOD(put_FontSize)(double dSize);
		STDMETHOD(get_FontStyle)(LONG* lStyle);
		STDMETHOD(put_FontStyle)(LONG lStyle);
		STDMETHOD(get_FontStringGID)(BOOL* bGID);
		STDMETHOD(put_FontStringGID)(BOOL bGID);
		STDMETHOD(get_FontCharSpace)(double* dSpace);
		STDMETHOD(put_FontCharSpace)(double dSpace);
	// shadow -----------------------------------------------------------------------------------
		STDMETHOD(SetShadow)(BSTR bsXML);
		STDMETHOD(get_ShadowDistanceX)(double* val);
		STDMETHOD(put_ShadowDistanceX)(double val);
		STDMETHOD(get_ShadowDistanceY)(double* val);
		STDMETHOD(put_ShadowDistanceY)(double val);
		STDMETHOD(get_ShadowBlurSize)(double* val);
		STDMETHOD(put_ShadowBlurSize)(double val);
		STDMETHOD(get_ShadowColor)(LONG* val);
		STDMETHOD(put_ShadowColor)(LONG val);
		STDMETHOD(get_ShadowAlpha)(LONG* val);
		STDMETHOD(put_ShadowAlpha)(LONG val);
		STDMETHOD(get_ShadowVisible)(BOOL* val);
		STDMETHOD(put_ShadowVisible)(BOOL val);
	// edge -------------------------------------------------------------------------------------
		STDMETHOD(SetEdgeText)(BSTR bsXML);
		STDMETHOD(get_EdgeVisible)(LONG* val);
		STDMETHOD(put_EdgeVisible)(LONG val);
		STDMETHOD(get_EdgeColor)(LONG* val);
		STDMETHOD(put_EdgeColor)(LONG val);
		STDMETHOD(get_EdgeAlpha)(LONG* val);
		STDMETHOD(put_EdgeAlpha)(LONG val);
		STDMETHOD(get_EdgeDist)(double* val);
		STDMETHOD(put_EdgeDist)(double val);
	//-------- Функции для вывода текста --------------------------------------------------------
		STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
		STDMETHOD(CommandDrawTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);
	//-------- Маркеры для команд ---------------------------------------------------------------
		STDMETHOD(BeginCommand)(DWORD lType);
		STDMETHOD(EndCommand)(DWORD lType);
	//-------- Функции для работы с Graphics Path -----------------------------------------------
		STDMETHOD(PathCommandMoveTo)(double fX, double fY);
		STDMETHOD(PathCommandLineTo)(double fX, double fY);
		STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints);
		STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3);
		STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints);
		STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
		STDMETHOD(PathCommandClose)();
		STDMETHOD(PathCommandEnd)();
		STDMETHOD(DrawPath)(long nType);
		STDMETHOD(PathCommandStart)();
		STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY);
		STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
		STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);
	//-------- Функции для вывода изображений ---------------------------------------------------
		STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight);
		STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight);
	// transform --------------------------------------------------------------------------------
		STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags);
		STDMETHOD(SetCommandParams)(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags);
		STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF);
		STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
		STDMETHOD(ResetTransform)(void);
	// -----------------------------------------------------------------------------------------
		STDMETHOD(get_ClipMode)(LONG* plMode);
		STDMETHOD(put_ClipMode)(LONG lMode);
	// additiaonal params ----------------------------------------------------------------------
		STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);
		STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);
	// --------------------------------------------------------------------------------------------

	// file
		STDMETHOD(OpenFile)(BSTR bsFilePath);
		STDMETHOD(CloseFile)();

	public:
		HRESULT FinalConstruct();
		void FinalRelease();

	private:
		
		ASCGraphics::IASCGraphicSimpleComverter*	m_pSimpleGraphicsConverter;		// конвертер сложных гафических путей в простые
		ASCGraphics::IASCFontManager*				m_pFontManager;					// менеджер шрифтов

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

		void _SetFont()
		{
			if (NULL == m_pFontManager)
			{
				CoCreateInstance(__uuidof(ASCGraphics::CASCFontManager), NULL, CLSCTX_ALL, __uuidof(ASCGraphics::IASCFontManager), (void**)&m_pFontManager);
				m_pFontManager->Initialize(L"");
			}

			m_pFontManager->SetStringGID(m_oFont.StringGID);

			if (_T("") == m_oFont.Path)
			{
				BSTR bsName = m_oFont.Name.AllocSysString();
				m_pFontManager->LoadFontByName(bsName, (float)m_oFont.Size, m_oFont.GetStyle(), m_dDpiX, m_dDpiY);
				SysFreeString(bsName);
			}
			else
			{
				BSTR bsName = m_oFont.Path.AllocSysString();
				m_pFontManager->LoadFontFromFile(bsName, (float)m_oFont.Size, m_dDpiX, m_dDpiY, 0);
				SysFreeString(bsName);
			}

			m_oInstalledFont = m_oFont;
		}

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