#pragma once

#include "../OfficeDrawing/Document.h"

#include "StylesWriter.h"
#include "ImageManager.h"

namespace NSPresentationEditor
{
	AVSINLINE std::wstring GetPhType(const LONG& lType)
	{
		switch (lType)
		{
			case 0: return _T("body");
			case 1: return _T("chart");
			case 2: return _T("clipArt");
			case 3: return _T("ctrTitle");
			case 4: return _T("dgm");
			case 5: return _T("dt");
			case 6: return _T("ftr");
			case 7: return _T("hdr");
			case 8: return _T("media");
			case 9: return _T("obj");
			case 10: return _T("pic");
			case 11: return _T("sldImg");
			case 12: return _T("sldNum");
			case 13: return _T("subTitle");
			case 14: return _T("tbl");
			case 15: return _T("title");
			default: break;
		}
		return _T("body");
	}

	class CShapeWriter : public IRenderer
	{
	private:
		NSPresentationEditor::CStringWriter		m_oWriterPath;
		NSPresentationEditor::CStringWriter		m_oWriterVML;
		NSPresentationEditor::CStringWriter		m_oWriter;
		NSPresentationEditor::CDoubleRect		m_oBounds;
		NSPresentationEditor::CDoubleRect		m_oTextRect;
		NSPresentationEditor::CMetricInfo		m_oMetricInfo;

		NSPresentationEditor::CShapeElement*	m_pShapeElement;
		NSPresentationEditor::CImageElement*	m_pImageElement;
		
		NSPresentationEditor::CRelsGenerator*	m_pRels;

		LONG									m_lNextShapeID;
		bool									m_bIsWriteGeom;

		bool									m_bWordArt;
		bool									m_bTextBox;
	public:	

		CShapeWriter();
		~CShapeWriter()
		{
			RELEASEOBJECT(m_pSimpleGraphicsConverter);
			RELEASEOBJECT(m_pSimpleGraphicsConverter);
		}

		AVSINLINE void InitNextId()
		{
			m_lNextShapeID = 1000;
		}
		AVSINLINE void SetShape(CImageElement* pShapeElem)
		{
			m_pSimpleGraphicsConverter->PathCommandEnd();
			m_pImageElement = pShapeElem;
            m_pShapeElement = NULL;

			m_oMetricInfo	= m_pImageElement->m_oMetric;
			m_oBounds		= m_pImageElement->m_rcBounds;
			m_oTextRect		= m_oBounds;

			m_bIsWriteGeom	= true;
			m_bWordArt		= false;
			m_bTextBox		= false;

			m_oWriter.ClearNoAttack();
			m_oWriterPath.ClearNoAttack();
			m_oWriterVML.ClearNoAttack();	
		}
		AVSINLINE void SetShape(CShapeElement* pShapeElem)
		{
			m_pSimpleGraphicsConverter->PathCommandEnd();
			m_pShapeElement = pShapeElem;
            m_pImageElement = NULL;

			m_oMetricInfo	= m_pShapeElement->m_oMetric;
			m_oBounds		= m_pShapeElement->m_rcBounds;
			m_oTextRect		= m_oBounds;

			m_bIsWriteGeom	= true;
			m_bWordArt		= false;
			m_bTextBox		= false;

			if (m_pShapeElement)
			{
				m_pShapeElement->m_oShape.GetTextRect(m_oTextRect);
			}

			m_oWriter.ClearNoAttack();
			m_oWriterPath.ClearNoAttack();
			m_oWriterVML.ClearNoAttack();
		}
		AVSINLINE void SetIsWriteGeom(bool bIsWrite)
		{
			m_bIsWriteGeom = bIsWrite;
		}
//--------------------------------------------------------------------
			CString ConvertShape	();
			CString ConvertImage	();
			CString ConvertLine		(CPen		& pen);
			CString	ConvertShadow	(CShadow	& shadow);
			CString	ConvertBrush	(CBrush		& brush);
	static	CString	ConvertColor	(CColor		& color, long alpha);
// тип рендерера-----------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType)	;
//-------- Функции для работы со страницей --------------------------------------------------
	virtual HRESULT NewPage()				;
	virtual HRESULT get_Height(double* dHeight);
	virtual HRESULT put_Height(const double& dHeight);
	virtual HRESULT get_Width(double* dWidth);
	virtual HRESULT put_Width(const double& dWidth)	;
	virtual HRESULT get_DpiX(double* dDpiX)	;
	virtual HRESULT get_DpiY(double* dDpiY)	;

// pen --------------------------------------------------------------------------------------
	virtual HRESULT get_PenColor(LONG* lColor)		;
	virtual HRESULT put_PenColor(const LONG& lColor);
	virtual HRESULT get_PenAlpha(LONG* lAlpha)		;
	virtual HRESULT put_PenAlpha(const LONG& lAlpha);
	virtual HRESULT get_PenSize(double* dSize)		;
	virtual HRESULT put_PenSize(const double& dSize);
	virtual HRESULT get_PenDashStyle(BYTE* val)		;
	virtual HRESULT put_PenDashStyle(const BYTE& val);
	virtual HRESULT get_PenLineStartCap(BYTE* val)	;
	virtual HRESULT put_PenLineStartCap(const BYTE& val)	;
	virtual HRESULT get_PenLineEndCap(BYTE* val)	;
	virtual HRESULT put_PenLineEndCap(const BYTE& val);
	virtual HRESULT get_PenLineJoin(BYTE* val)		;
	virtual HRESULT put_PenLineJoin(const BYTE& val);
	virtual HRESULT get_PenDashOffset(double* dOffset);
	virtual HRESULT put_PenDashOffset(const double& dOffset);
	virtual HRESULT get_PenAlign(LONG* lAlign)		;
	virtual HRESULT put_PenAlign(const LONG& lAlign);
	virtual HRESULT get_PenMiterLimit(double* dOffset);
	virtual HRESULT put_PenMiterLimit(const double& dOffset);
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount);

// brush ------------------------------------------------------------------------------------
	virtual HRESULT get_BrushType(LONG* lType)				;
	virtual HRESULT put_BrushType(const LONG& lType)		;
	virtual HRESULT get_BrushColor1(LONG* lColor)			;
	virtual HRESULT put_BrushColor1(const LONG& lColor)		;
	virtual HRESULT get_BrushAlpha1(LONG* lAlpha)			;
	virtual HRESULT put_BrushAlpha1(const LONG& lAlpha)		;
	virtual HRESULT get_BrushColor2(LONG* lColor)			;
	virtual HRESULT put_BrushColor2(const LONG& lColor)		;
	virtual HRESULT get_BrushAlpha2(LONG* lAlpha)			;
	virtual HRESULT put_BrushAlpha2(const LONG& lAlpha)		;
	virtual HRESULT get_BrushTexturePath(std::wstring* bsPath); 
	virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath); 
	virtual HRESULT get_BrushTextureMode(LONG* lMode)		;
	virtual HRESULT put_BrushTextureMode(const LONG& lMode)	;
	virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)	;
	virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha);
	virtual HRESULT get_BrushLinearAngle(double* dAngle)	;
	virtual HRESULT put_BrushLinearAngle(const double& dAngle);
	virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height);
	virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height);

	virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount);

// font -------------------------------------------------------------------------------------
	virtual HRESULT get_FontName(std::wstring* bsName);
	virtual HRESULT put_FontName(const std::wstring& bsName);
	virtual HRESULT get_FontPath(std::wstring* bsName);
	virtual HRESULT put_FontPath(const std::wstring& bsName);
	virtual HRESULT get_FontSize(double* dSize)		;
	virtual HRESULT put_FontSize(const double& dSize);
	virtual HRESULT get_FontStyle(LONG* lStyle)		;
	virtual HRESULT put_FontStyle(const LONG& lStyle);
	virtual HRESULT get_FontStringGID(INT* bGID)	;
	virtual HRESULT put_FontStringGID(const INT& bGID);
	virtual HRESULT get_FontCharSpace(double* dSpace);
	virtual HRESULT put_FontCharSpace(const double& dSpace)	;
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex);
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex);

//-------- Функции для вывода текста --------------------------------------------------------
    virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h) ;
    virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h) ;

    virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h) ;
    virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h);

//-------- Маркеры для команд ---------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType);
	virtual HRESULT EndCommand(const DWORD& lType)	;

//-------- Функции для работы с Graphics Path -----------------------------------------------
	virtual HRESULT PathCommandMoveTo(const double& x, const double& y);
	virtual HRESULT PathCommandLineTo(const double& x, const double& y);
	virtual HRESULT PathCommandLinesTo(double* points, const int& count)	;
	virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)	;
	virtual HRESULT PathCommandCurvesTo(double* points, const int& count)	;
	virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle);
	virtual HRESULT PathCommandClose()								;
	virtual HRESULT PathCommandEnd()								;
	virtual HRESULT DrawPath(const LONG& nType)						;
	virtual HRESULT PathCommandStart()								;
	virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y);

    virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h) ;
    virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h);

    virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h);
    virtual HRESULT PathCommandTextEx(const std::wstring& sText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h) ;

//-------- Функции для вывода изображений ---------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)	;
	virtual HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255);	

// transform --------------------------------------------------------------------------------
	virtual HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags);
	virtual HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags);
	virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6);
	virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
	virtual HRESULT ResetTransform();

// -----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* plMode);
	virtual HRESULT put_ClipMode(const LONG& lMode)	;

// additiaonal params ----------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand);
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand)	;
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand);	
//////////////////////////////////////////////////////////////////////////////////////////////////
		// shadow -----------------------------------------------------------------------------------
		HRESULT SetShadow (BSTR bsXML);
		HRESULT get_ShadowDistanceX (double* val);
		HRESULT put_ShadowDistanceX (double val);
		HRESULT get_ShadowDistanceY (double* val);
		HRESULT put_ShadowDistanceY (double val);
		HRESULT get_ShadowBlurSize (double* val);
		HRESULT put_ShadowBlurSize (double val);
		HRESULT get_ShadowColor (LONG* val);
		HRESULT put_ShadowColor (LONG val);
		HRESULT get_ShadowAlpha (LONG* val);
		HRESULT put_ShadowAlpha (LONG val);
        HRESULT get_ShadowVisible (bool* val);
        HRESULT put_ShadowVisible (bool val);
		// edge -------------------------------------------------------------------------------------
		HRESULT SetEdgeText (BSTR bsXML);
		HRESULT get_EdgeVisible (LONG* val);
		HRESULT put_EdgeVisible (LONG val);
		HRESULT get_EdgeColor (LONG* val);
		HRESULT put_EdgeColor (LONG val);
		HRESULT get_EdgeAlpha (LONG* val);
		HRESULT put_EdgeAlpha (LONG val);
		HRESULT get_EdgeDist (double* val);
		HRESULT put_EdgeDist (double val);


		HRESULT SetFont(BSTR bsXML);
		HRESULT SetPen(BSTR bsXML);
		HRESULT SetBrush(BSTR bsXML);
	// additiaonal params ----------------------------------------------------------------------
		HRESULT SetAdditionalParam (BSTR ParamName, VARIANT ParamValue);
		HRESULT GetAdditionalParam (BSTR ParamName, VARIANT* ParamValue);
	public:
		void SetRelsGenerator(CRelsGenerator* pGenerator)
		{
			m_pRels = pGenerator;
		}

	protected:
		void CalculateFullTransform()
		{
			m_oFullTransform	= m_oBaseTransform;
			m_oFullTransform.Multiply(&m_oTransform, Aggplus::MatrixOrderPrepend);

			m_dTransformAngle	= m_oTransform.z_Rotation();
		}
		void _SetFont()
		{
			if (NULL == m_pFontManager)
			{
				m_pFontManager = new CFontManager();
				m_pFontManager->Initialize();
			}

			m_pFontManager->SetStringGID(m_oFont.StringGID);

			if (_T("") == m_oFont.Path)
			{
				m_pFontManager->LoadFontByName(m_oFont.Name, (float)m_oFont.Size, m_oFont.GetStyle(), m_dDpiX, m_dDpiY);
			}
			else
			{
				m_pFontManager->LoadFontFromFile(m_oFont.Path, (float)m_oFont.Size, m_dDpiX, m_dDpiY, 0);
            }

			m_oInstalledFont = m_oFont;
		}


	public:

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

		double							m_dWidth;
		double							m_dHeight;

	public:
		inline void MoveTo(const double& dX, const double& dY)
		{
			double x = dX;
			double y = dY;
			m_oFullTransform.TransformPoint(x, y);

			CString str = _T("");
			str.Format(_T("<a:moveTo><a:pt x=\"%d\" y=\"%d\"/></a:moveTo>"), (LONG)x, (LONG)y);
			m_oWriterPath.WriteString(str);
		}
		inline void LineTo(const double& dX, const double& dY)
		{
			double x = dX;
			double y = dY;
			m_oFullTransform.TransformPoint(x, y);

			CString str = _T("");
			str.Format(_T("<a:lnTo><a:pt x=\"%d\" y=\"%d\"/></a:lnTo>"), (LONG)x, (LONG)y);
			m_oWriterPath.WriteString(str);
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

			CString str = _T("");
			str.Format(_T("<a:cubicBezTo><a:pt x=\"%d\" y=\"%d\"/><a:pt x=\"%d\" y=\"%d\"/><a:pt x=\"%d\" y=\"%d\"/></a:cubicBezTo>"), 
				(LONG)_x1, (LONG)_y1, (LONG)_x2, (LONG)_y2, (LONG)_x3, (LONG)_y3);
			m_oWriterPath.WriteString(str);
		}
		void Start()
		{
			m_oWriterPath.ClearNoAttack();
		}
		void End()
		{
			m_oWriterPath.ClearNoAttack();
		}
		void Close()
		{
			CString str = _T("<a:close/>");
			m_oWriterPath.WriteString(str);
		}

		void WriteShapeInfo();

		void WriteImageInfo();
		void WriteTextInfo();
	};
}
