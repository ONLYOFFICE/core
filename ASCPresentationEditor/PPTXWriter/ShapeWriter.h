#pragma once

#include "../OfficeDrawing/Document.h"

#include "StylesWriter.h"
#include "ImageManager.h"

namespace NSPresentationEditor
{
	AVSINLINE CString GetPhType(const LONG& lType)
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

			m_oMetricInfo	= m_pImageElement->m_oMetric;
			m_oBounds		= m_pImageElement->m_rcBounds;
			m_oTextRect		= m_oBounds;

			m_bIsWriteGeom	= true;

			m_oWriter.ClearNoAttack();
			m_oWriterPath.ClearNoAttack();
			m_oWriterVML.ClearNoAttack();	
		}
		AVSINLINE void SetShape(CShapeElement* pShapeElem)
		{
			m_pSimpleGraphicsConverter->PathCommandEnd();
			m_pShapeElement = pShapeElem;

			m_oMetricInfo	= m_pShapeElement->m_oMetric;
			m_oBounds		= m_pShapeElement->m_rcBounds;
			m_oTextRect		= m_oBounds;

			m_bIsWriteGeom	= true;

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
		CString ConvertShape();
		CString ConvertImage();
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

		void WriteShapeInfo()
		{
			m_oWriter.WriteString(std::wstring(L"<p:nvSpPr>"));

			CString strShapeID = _T("");
            strShapeID.Format(L"%d", m_lNextShapeID);

			m_oWriter.WriteString(std::wstring(L"<p:cNvPr id=\"") + string2std_string(strShapeID) + L"\"");

			if (m_pShapeElement->m_sName.empty()) m_pShapeElement->m_sName = std::wstring(L"Shape ") +  string2std_string(strShapeID);

			m_oWriter.WriteString(std::wstring(L" name=\"") + m_pShapeElement->m_sName + L"\"");

			m_oWriter.WriteString(std::wstring(L"></p:cNvPr><p:cNvSpPr><a:spLocks noGrp=\"1\" noChangeArrowheads=\"1\"/></p:cNvSpPr>"));

			++m_lNextShapeID;

			if (-1 != m_pShapeElement->m_lPlaceholderType)
			{
				if (15 == m_pShapeElement->m_lPlaceholderType)
					m_pShapeElement->m_lPlaceholderID = -1;
				if (0 == m_pShapeElement->m_lPlaceholderType)
					m_pShapeElement->m_lPlaceholderID = 1;

				if (-1 == m_pShapeElement->m_lPlaceholderID)
				{
                    CString strPlaceholder = _T("<p:nvPr><p:ph type=\"") + GetPhType(m_pShapeElement->m_lPlaceholderType) +_T("\"/></p:nvPr>");

                    m_oWriter.WriteString(strPlaceholder);
				}
				else
				{
                    CString strIdx; strIdx.Format(_T("%d"), m_pShapeElement->m_lPlaceholderID);
                    CString strPlaceholder = _T("<p:nvPr><p:ph type=\"") + GetPhType(m_pShapeElement->m_lPlaceholderType) + _T("\" idx=\"") + strIdx + _T("\"/></p:nvPr>");

					m_oWriter.WriteString(strPlaceholder);
				}
			}
			else
			{
				m_oWriter.WriteString(std::wstring(L"<p:nvPr/>"));
			}
			
			CString str2 = _T("</p:nvSpPr>");
			m_oWriter.WriteString(str2);
		}

		void WriteImageInfo()
		{
			m_oWriter.WriteString(std::wstring(L"<p:nvPicPr>"));

			CString strShapeID;
			strShapeID.Format(_T("%d"),	m_lNextShapeID);			
			
			m_oWriter.WriteString(std::wstring(L"<p:cNvPr id=\"") +string2std_string(strShapeID) + L"\"" );
			
			if (m_pImageElement->m_sName.empty()) m_pImageElement->m_sName = std::wstring(L"Image ") +  string2std_string(strShapeID);
			
			m_oWriter.WriteString(std::wstring(L" name=\"") + m_pImageElement->m_sName + L"\"");

			m_oWriter.WriteString(std::wstring(L"></p:cNvPr><p:cNvPicPr><a:spLocks noGrp=\"1\" noChangeAspect=\"1\"/></p:cNvPicPr>"));

			++m_lNextShapeID;

			if (-1 != m_pImageElement->m_lPlaceholderType)
			{
				if (15 == m_pImageElement->m_lPlaceholderType)
					m_pImageElement->m_lPlaceholderID = -1;
				if (0 == m_pImageElement->m_lPlaceholderType)
					m_pImageElement->m_lPlaceholderID = 1;

				if (-1 == m_pImageElement->m_lPlaceholderID)
				{
                    CString strPlaceholder = _T("<p:nvPr><p:ph type=\"") + GetPhType(m_pImageElement->m_lPlaceholderType) +_T("\"/></p:nvPr>");

                    m_oWriter.WriteString(strPlaceholder);
				}
				else
				{
                    CString strIdx; strIdx.Format(_T("%d"), m_pImageElement->m_lPlaceholderID);
                    CString strPlaceholder = _T("<p:nvPr><p:ph type=\"") + GetPhType(m_pImageElement->m_lPlaceholderType) + _T("\" idx=\"") + strIdx + _T("\"/></p:nvPr>");

					m_oWriter.WriteString(strPlaceholder);
				}
			}
			else
			{
				m_oWriter.WriteString(std::wstring(L"<p:nvPr/>"));
			}
			
			CString str2 = _T("</p:nvPicPr>");
			m_oWriter.WriteString(str2);
		}

		void WriteTextInfo()
		{
			size_t nCount = m_pShapeElement->m_oShape.m_oText.m_arParagraphs.size();
			if (/*0 == nCount || */(0x00 == (m_pShapeElement->m_oShape.m_lDrawType & c_ShapeDrawType_Text)))
				return;

			if (0 == nCount)
			{
				CString strEmptyText = _T("<p:txBody><a:bodyPr/><a:lstStyle/><a:p><a:endParaRPr dirty=\"0\"/></a:p></p:txBody>");
				m_oWriter.WriteString(strEmptyText);
				return;
			}

			m_oWriter.WriteString(std::wstring(L"<p:txBody>"));

			m_oWriter.WriteString(std::wstring(L"<a:bodyPr lIns=\"0\" tIns=\"0\" rIns=\"0\" bIns=\"0\""));

			if (m_pShapeElement->m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical == 0 ) m_oWriter.WriteString(" anchor=\"t\"");
			if (m_pShapeElement->m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical == 2 ) m_oWriter.WriteString(" anchor=\"b\"");
			m_oWriter.WriteString(std::wstring(L"/>"));

			CString str3 = _T("<a:lstStyle>");
			m_oWriter.WriteString(str3);

			CStylesWriter::ConvertStyles(m_pShapeElement->m_oShape.m_oText.m_oStyles, m_oMetricInfo, m_oWriter);

			CString str4 = _T("</a:lstStyle>");
			m_oWriter.WriteString(str4);

			for (size_t nIndexPar = 0; nIndexPar < nCount; ++nIndexPar)
			{
				NSPresentationEditor::CParagraph* pParagraph = &m_pShapeElement->m_oShape.m_oText.m_arParagraphs[nIndexPar];

				CString _str1 = _T("");
				_str1.Format(_T("<a:p><a:pPr lvl=\"%d\""), pParagraph->m_lTextLevel);
				m_oWriter.WriteString(_str1);

				NSPresentationEditor::CTextPFRun* pPF = &pParagraph->m_oPFRun;

				if (pPF->fontAlign.is_init())
				{
					CString strProp = CStylesWriter::GetFontAlign(pPF->fontAlign.get());
					m_oWriter.WriteString(std::wstring(L" fontAlgn=\"") + string2std_string(strProp) + _T("\""));
				}
				if (pPF->leftMargin.is_init())
				{
					CString strProp = _T("");
					strProp.Format(_T(" marL=\"%d\""), pPF->leftMargin.get());
					m_oWriter.WriteString(strProp);
				}
				if (pPF->indent.is_init())
				{
					CString strProp = _T("");
					strProp.Format(_T(" indent=\"%d\""), pPF->indent.get());
					m_oWriter.WriteString(strProp);
				}
				if (pPF->textAlignment.is_init())
				{
					CString strProp = CStylesWriter::GetTextAlign(pPF->textAlignment.get());
					m_oWriter.WriteString(std::wstring(L" algn=\"") + string2std_string(strProp) + _T("\""));
				}
				if (pPF->defaultTabSize.is_init())
				{
					CString strProp = _T("");
					strProp.Format(_T(" defTabSz=\"%d\""), pPF->defaultTabSize.get());
					m_oWriter.WriteString(strProp);
				}
				CString _str2 = _T(">");
				m_oWriter.WriteString(_str2);

				double dKoef1 = 3.52777778;
				if (pPF->lineSpacing.is_init())
				{
					LONG val = pPF->lineSpacing.get();
					if (val > 0)
					{
						CString str = _T("");
						str.Format(_T("<a:lnSpc><a:spcPts val=\"%d\"/></a:lnSpc>"), (int)(val / dKoef1));
						m_oWriter.WriteString(str);
					}
					else
					{
						CString str = _T("");
						str.Format(_T("<a:lnSpc><a:spcPct val=\"%d\"/></a:lnSpc>"), -val * 1000);
						m_oWriter.WriteString(str);
					}
				}
				if (pPF->spaceAfter.is_init())
				{
					LONG val = pPF->spaceAfter.get();
					if (val > 0)
					{
						CString str = _T("");
						str.Format(_T("<a:spcAft><a:spcPts val=\"%d\"/></a:spcAft>"), (int)(val / dKoef1));
						m_oWriter.WriteString(str);
					}
					else
					{
						CString str = _T("");
						str.Format(_T("<a:spcAft><a:spcPct val=\"%d\"/></a:spcAft>"), -val * 1000);
						m_oWriter.WriteString(str);
					}
				}
				if (pPF->spaceBefore.is_init())
				{
					LONG val = pPF->spaceBefore.get();
					if (val > 0)
					{
						CString str = _T("");
						str.Format(_T("<a:spcBef><a:spcPts val=\"%d\"/></a:spcBef>"), (int)(val / dKoef1));
						m_oWriter.WriteString(str);
					}
					else
					{
						CString str = _T("");
						str.Format(_T("<a:spcBef><a:spcPct val=\"%d\"/></a:spcBef>"), -val * 1000);
						m_oWriter.WriteString(str);
					}
				}

				if (pPF->hasBullet.is_init())
				{
					if (pPF->hasBullet.get())
					{
 						wchar_t bu = 0x2022;
						m_oWriter.WriteString(std::wstring(L"<a:buChar char=\""));
						if (pPF->bulletChar.is_init())
						{
							bu = pPF->bulletChar.get();
						}
                        m_oWriter.WriteString(std::wstring(&bu, 1));
                        m_oWriter.WriteString(std::wstring(L"\"/>"));
					}
					else
					{
						m_oWriter.WriteString(std::wstring(L"<a:buNone/>"));
					}
				}

				m_oWriter.WriteString(std::wstring(L"</a:pPr>"));

				size_t nCountSpans = pParagraph->m_arSpans.size();
				for (size_t nSpan = 0; nSpan < nCountSpans; ++nSpan)
				{
					if (TRUE)
					{
						if ((nSpan == (nCountSpans - 1)) && (_T("\n") == pParagraph->m_arSpans[nSpan].m_strText))
						{
							NSPresentationEditor::CTextCFRun* pCF = &pParagraph->m_arSpans[nSpan].m_oRun;
							if (pCF->Size.is_init())
							{
								CString strProp = _T("");
								strProp.Format(_T("<a:endParaRPr lang=\"en-US\" sz=\"%d\"/>"), (int)(100 * pCF->Size.get()));
								m_oWriter.WriteString(strProp);
							}
							else
							{
								m_oWriter.WriteString(std::wstring(L"<a:endParaRPr lang=\"en-US\"/>"));
							}
							continue;
						}
					}

					NSPresentationEditor::CTextCFRun* pCF = &pParagraph->m_arSpans[nSpan].m_oRun;

					bool bIsBr = false;
					
					if (pParagraph->m_arSpans[nSpan].m_strText.GetLength() >0) 
					{
						if(_T("\n") == pParagraph->m_arSpans[nSpan].m_strText)	bIsBr=true;
					}
					
					if (bIsBr)
					{
						CString strRun1 = _T("<a:br><a:rPr");
						m_oWriter.WriteString(strRun1);
					}
					else
					{
						CString strRun1 = _T("<a:r><a:rPr");
						m_oWriter.WriteString(strRun1);
					}					
					
					if (pCF->Size.is_init())
					{
						CString strProp = _T("");
						strProp.Format(_T(" sz=\"%d\""), (int)(100 * pCF->Size.get()));
						m_oWriter.WriteString(strProp);
					}				
					if (pCF->FontBold.is_init())
					{
						if (pCF->FontBold.get())
							m_oWriter.WriteString(std::wstring(L" b=\"1\""));
						else
							m_oWriter.WriteString(std::wstring(L" b=\"0\""));
					}
					if (pCF->FontItalic.is_init())
					{
						if (pCF->FontItalic.get())
							m_oWriter.WriteString(std::wstring(L" i=\"1\""));
						else
							m_oWriter.WriteString(std::wstring(L" i=\"0\""));
					}
					m_oWriter.WriteString(std::wstring(L">"));

					if (pCF->Color.is_init())
					{
						if (pCF->Color->m_lSchemeIndex != -1)
						{
							CString strProp = _T("<a:solidFill><a:schemeClr val=\"") + CStylesWriter::GetColorInScheme(pCF->Color->m_lSchemeIndex) + _T("\"/></a:solidFill>");
							m_oWriter.WriteString(strProp);
						}
						else
						{
							CString strColor = _T("");
							strColor.Format(_T("%06x"), pCF->Color->GetLONG_RGB());

							CString strProp = _T("<a:solidFill><a:srgbClr val=\"") + strColor + _T("\"/></a:solidFill>");
							m_oWriter.WriteString(strProp);
						}
					}

					if (pCF->Typeface.is_init())
					{
						if (0 == pCF->Typeface.get())
						{
							CString strProp = _T("<a:latin typeface=\"+mj-lt\"/>");
							m_oWriter.WriteString(strProp);
						}
						else
						{
							CString strProp = _T("<a:latin typeface=\"+mn-lt\"/>");
							m_oWriter.WriteString(strProp);
						}
					}
					else if (pCF->FontProperties.is_init())
					{
						m_oWriter.WriteString(std::wstring(L"<a:latin typeface=\"") + pCF->FontProperties->strFontName + _T("\"/>"));
					}

					m_oWriter.WriteString(std::wstring(L"</a:rPr>"));

					if (!bIsBr)
					{
						CString strT1 = _T("<a:t>");
						m_oWriter.WriteString(strT1);

						CString strT = pParagraph->m_arSpans[nSpan].m_strText;
						NSPresentationEditor::CorrectXmlString(strT);
						m_oWriter.WriteString(strT);

						CString strT2 = _T("</a:t>");
						m_oWriter.WriteString(strT2);

						m_oWriter.WriteString(std::wstring(L"</a:r>"));
					}
					else
					{
						m_oWriter.WriteString(std::wstring(L"</a:br>"));
					}
				}

				CString strEndPar = _T("</a:p>");
				m_oWriter.WriteString(strEndPar);
			}

			CString str5 = _T("</p:txBody>");
			m_oWriter.WriteString(str5);
		}
	};
}
