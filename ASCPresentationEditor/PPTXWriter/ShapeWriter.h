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

	[ coclass, default(IASCRenderer), threading(apartment), vi_progid("Drawing.SW"), progid("Drawing.SW.1"), version(1.0), uuid("3C1D8C77-D249-47a4-8D7A-7F7BEE66741E") ]
	class ATL_NO_VTABLE CShapeWriter : public IASCRenderer
	{
	private:
		NSPresentationEditor::CStringWriter m_oWriterPath;
		NSPresentationEditor::CStringWriter m_oWriterVML;
		NSPresentationEditor::CStringWriter m_oWriter;
		NSPresentationEditor::CDoubleRect	m_oBounds;
		NSPresentationEditor::CDoubleRect	m_oTextRect;
		NSPresentationEditor::CMetricInfo	m_oMetricInfo;

		NSPresentationEditor::CShapeElement* m_pShapeElement;
		
		NSPresentationEditor::CRelsGenerator*	m_pRels;

		LONG									m_lNextShapeID;
		bool								m_bIsWriteGeom;

	public:	

		DECLARE_PROTECT_FINAL_CONSTRUCT()

		CShapeWriter()
		{
			m_pShapeElement = NULL;
			CoCreateInstance(ASCGraphics::CLSID_CASCGraphicSimpleComverter, NULL, CLSCTX_ALL, 
				ASCGraphics::IID_IASCGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);

			m_pRels			= NULL;
			m_lNextShapeID	= 1000;

			m_bIsWriteGeom = true;
		}
		~CShapeWriter()
		{
			RELEASEINTERFACE(m_pSimpleGraphicsConverter);
		}

		AVSINLINE void InitNextId()
		{
			m_lNextShapeID = 1000;
		}

		AVSINLINE void SetShape(CShapeElement* pShapeElem)
		{
			m_pSimpleGraphicsConverter->PathCommandEnd();
			m_pShapeElement = pShapeElem;

			m_oMetricInfo	= m_pShapeElement->m_oMetric;
			m_oBounds		= m_pShapeElement->m_rcBounds;
			m_oTextRect		= m_oBounds;

			m_bIsWriteGeom	= true;

			m_pShapeElement->m_oShape.GetTextRect(m_oTextRect);

			m_oWriter.ClearNoAttack();
			m_oWriterPath.ClearNoAttack();
			m_oWriterVML.ClearNoAttack();
		}
		AVSINLINE void SetIsWriteGeom(bool bIsWrite)
		{
			m_bIsWriteGeom = bIsWrite;
		}
		AVSINLINE CString ConvertShape()
		{
			m_oWriter.WriteString(_T("<p:sp>"));

			WriteShapeInfo();

			CGeomShapeInfo oInfo;
			oInfo.SetBounds(m_oBounds);

			oInfo.m_dRotate = m_pShapeElement->m_dRotate;
			oInfo.m_bFlipH	= m_pShapeElement->m_bFlipH;
			oInfo.m_bFlipV	= m_pShapeElement->m_bFlipV;

			oInfo.m_lOriginalWidth	= (LONG)m_pShapeElement->m_rcBoundsOriginal.GetWidth();
			oInfo.m_lOriginalHeight	= (LONG)m_pShapeElement->m_rcBoundsOriginal.GetHeight();

			m_oWriter.WriteString(_T("<p:spPr>"));

			if (m_bIsWriteGeom)
			{
				CString strPosition = _T("");
				if (0 == m_pShapeElement->m_dRotate)
				{
					strPosition.Format(_T("<a:xfrm><a:off x=\"%d\" y=\"%d\"/><a:ext cx=\"%d\" cy=\"%d\"/></a:xfrm>"), (size_t)m_pShapeElement->m_rcBoundsOriginal.left, (size_t)m_pShapeElement->m_rcBoundsOriginal.top,
						(size_t)(m_pShapeElement->m_rcBoundsOriginal.right - m_pShapeElement->m_rcBoundsOriginal.left),
						(size_t)(m_pShapeElement->m_rcBoundsOriginal.bottom - m_pShapeElement->m_rcBoundsOriginal.top));
				}
				else
				{
					strPosition.Format(_T("<a:xfrm rot=\"%d\" flipH=\"%d\" flipV=\"%d\"><a:off x=\"%d\" y=\"%d\"/><a:ext cx=\"%d\" cy=\"%d\"/></a:xfrm>"),
						(int)(m_pShapeElement->m_dRotate * 60000), m_pShapeElement->m_bFlipH ? 1 : 0, m_pShapeElement->m_bFlipV ? 1 : 0,
						(int)m_pShapeElement->m_rcBoundsOriginal.left, (int)m_pShapeElement->m_rcBoundsOriginal.top,
						(int)(m_pShapeElement->m_rcBoundsOriginal.right - m_pShapeElement->m_rcBoundsOriginal.left),
						(int)(m_pShapeElement->m_rcBoundsOriginal.bottom - m_pShapeElement->m_rcBoundsOriginal.top));
				}
				m_oWriter.WriteString(strPosition);
			}

			if (m_pShapeElement->m_oShape.m_lDrawType & c_ShapeDrawType_Graphic)
			{
				m_pShapeElement->m_oShape.ToRenderer(dynamic_cast<IRenderer*>(this), oInfo, m_oMetricInfo, 0.0, 1.0);
			}

			if (m_oWriterVML.GetCurSize() >= 10)
			{
				if (_T("") == m_pShapeElement->m_oShape.m_strPPTXShape)
				{
					m_oWriter.WriteString(_T("<a:custGeom>"));

					double dW = max(m_oBounds.GetWidth(), 0.1);
					double dH = max(m_oBounds.GetHeight(), 0.1);

					int __l = (int)((m_oTextRect.left	 - m_oBounds.left)	* 100000 / dW);
					int __t = (int)((m_oTextRect.top	 - m_oBounds.top)	* 100000 / dH);
					int __r = (int)((m_oTextRect.right	 - m_oBounds.left)	* 100000 / dW);
					int __b = (int)((m_oTextRect.bottom - m_oBounds.top)	* 100000 / dH);

					size_t __nCount = m_pShapeElement->m_oShape.m_oText.m_arParagraphs.size();
					if (0 == __nCount || (0x00 == (m_pShapeElement->m_oShape.m_lDrawType & c_ShapeDrawType_Text)))
					{
						m_oWriter.WriteString(_T("<a:rect l=\"l\" t=\"t\" r=\"r\" b=\"b\"/>"));
					}
					else
					{
						CString strGuides = _T("");
						strGuides.Format(_T("<a:gdLst><a:gd name=\"_l\" fmla=\"*/ w %d 100000\"/><a:gd name=\"_t\" fmla=\"*/ h %d 100000\"/>\
											<a:gd name=\"_r\" fmla=\"*/ w %d 100000\"/><a:gd name=\"_b\" fmla=\"*/ h %d 100000\"/></a:gdLst>"), __l, __t, __r, __b);
						m_oWriter.WriteString(strGuides);

						m_oWriter.WriteString(_T("<a:rect l=\"_l\" t=\"_t\" r=\"_r\" b=\"_b\"/>"));
					}

					m_oWriter.WriteString(_T("<a:pathLst>"));
					m_oWriter.Write(m_oWriterVML);
					m_oWriter.WriteString(_T("</a:pathLst></a:custGeom>"));
				}
				else
				{
					m_oWriter.WriteString(m_pShapeElement->m_oShape.m_strPPTXShape);
				}

				CBrush* pBrush = &m_pShapeElement->m_oShape.m_oBrush;
				
				if (pBrush->Type == c_BrushTypeTexture)
				{
					CString strRid = m_pRels->WriteImage(pBrush->TexturePath);

					CString strWrite = _T("<a:blipFill dpi=\"0\" rotWithShape=\"1\"><a:blip r:embed=\"") + strRid + 
						_T("\"/><a:srcRect/><a:stretch><a:fillRect/></a:stretch></a:blipFill>");

					m_oWriter.WriteString(strWrite);
				}
				else if (pBrush->Color1.m_lSchemeIndex == -1)
				{
					if (255 == pBrush->Alpha1)
					{
						CString str = _T("");
						str.Format(_T("<a:solidFill><a:srgbClr val=\"%06x\"/></a:solidFill>"), pBrush->Color1.GetLONG_RGB());
						m_oWriter.WriteString(str);
					}
					else
					{
						CString str = _T("");
						str.Format(_T("<a:solidFill><a:srgbClr val=\"%06x\"><a:alpha val=\"%d\"/></a:srgbClr></a:solidFill>"), pBrush->Color1.GetLONG_RGB(), (int)(pBrush->Alpha1 * 100000 / 255));
						m_oWriter.WriteString(str);
					}
				}
				else
				{
					if (255 == pBrush->Alpha1)
					{
						CString str = _T("");
						str.Format(_T("<a:solidFill><a:schemeClr val=\"%s\"/></a:solidFill>"), CStylesWriter::GetColorInScheme(pBrush->Color1.m_lSchemeIndex));
						m_oWriter.WriteString(str);
					}
					else
					{
						CString str = _T("");
						str.Format(_T("<a:solidFill><a:schemeClr val=\"%s\"><a:alpha val=\"%d\"/></a:schemeClr></a:solidFill>"), CStylesWriter::GetColorInScheme(pBrush->Color1.m_lSchemeIndex), (int)(pBrush->Alpha1 * 100000 / 255));
						m_oWriter.WriteString(str);
					}
				}
				//m_oWriter.WriteString(_T("<a:effectLst/>"));

				CPen* pPen = &m_pShapeElement->m_oShape.m_oPen;
				CString strLine = _T("");
				strLine.Format(_T("<a:ln w=\"%d\">"), (int)(pPen->Size * 36000));
				m_oWriter.WriteString(strLine);

				if (pPen->Color.m_lSchemeIndex == -1)
				{
					if (255 == pPen->Alpha)
					{
						CString str = _T("");
						str.Format(_T("<a:solidFill><a:srgbClr val=\"%06x\"/></a:solidFill>"), pPen->Color.GetLONG_RGB());
						m_oWriter.WriteString(str);
					}
					else
					{
						CString str = _T("");
						str.Format(_T("<a:solidFill><a:srgbClr val=\"%06x\"><a:alpha val=\"%d\"/></a:srgbClr></a:solidFill>"), pPen->Color.GetLONG_RGB(), (int)(pPen->Alpha * 100000 / 255));
						m_oWriter.WriteString(str);
					}
				}
				else
				{
					if (255 == pPen->Alpha)
					{
						CString str = _T("");
						str.Format(_T("<a:solidFill><a:schemeClr val=\"%s\"/></a:solidFill>"), CStylesWriter::GetColorInScheme(pPen->Color.m_lSchemeIndex));
						m_oWriter.WriteString(str);
					}
					else
					{
						CString str = _T("");
						str.Format(_T("<a:solidFill><a:schemeClr val=\"%s\"><a:alpha val=\"%d\"/></a:schemeClr></a:solidFill>"), CStylesWriter::GetColorInScheme(pPen->Color.m_lSchemeIndex), (int)(pPen->Alpha * 100000 / 255));
						m_oWriter.WriteString(str);
					}
				}

				m_oWriter.WriteString(_T("<a:round/><a:headEnd/><a:tailEnd/></a:ln>"));
			}

			m_oWriter.WriteString(_T("</p:spPr>"));			

			WriteTextInfo();
			m_oWriter.WriteString(_T("</p:sp>"));
			return m_oWriter.GetData();
		}

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
		// -----------------------------------------------------------------------------------------

	public:
		HRESULT FinalConstruct();
		void FinalRelease();

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
			CString str1 = _T("<p:nvSpPr>");
			m_oWriter.WriteString(str1);

			CString strShapeID = _T("");
			strShapeID.Format(_T("<p:cNvPr id=\"%d\" name=\"Shape %d\"/><p:cNvSpPr><a:spLocks noGrp=\"1\" noChangeArrowheads=\"1\"/></p:cNvSpPr>"),
				m_lNextShapeID, m_lNextShapeID);
			m_oWriter.WriteString(strShapeID);

			++m_lNextShapeID;

			if (-1 != m_pShapeElement->m_lPlaceholderType)
			{
				if (15 == m_pShapeElement->m_lPlaceholderType)
					m_pShapeElement->m_lPlaceholderID = -1;
				if (0 == m_pShapeElement->m_lPlaceholderType)
					m_pShapeElement->m_lPlaceholderID = 1;

				if (-1 == m_pShapeElement->m_lPlaceholderID)
				{
					CString strPlaceholder = _T("");
					strPlaceholder.Format(_T("<p:nvPr><p:ph type=\"%s\"/></p:nvPr>"), GetPhType(m_pShapeElement->m_lPlaceholderType));
					m_oWriter.WriteString(strPlaceholder);
				}
				else
				{
					CString strPlaceholder = _T("");
					strPlaceholder.Format(_T("<p:nvPr><p:ph type=\"%s\" idx=\"%d\"/></p:nvPr>"), 
						GetPhType(m_pShapeElement->m_lPlaceholderType), m_pShapeElement->m_lPlaceholderID);
					m_oWriter.WriteString(strPlaceholder);
				}
			}
			else
			{
				m_oWriter.WriteString(_T("<p:nvPr/>"));
			}
			
			CString str2 = _T("</p:nvSpPr>");
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

			CString str1 = _T("<p:txBody>");
			m_oWriter.WriteString(str1);

			CString str2 = _T("<a:bodyPr lIns=\"0\" tIns=\"0\" rIns=\"0\" bIns=\"0\"/>");
			m_oWriter.WriteString(str2);

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
					m_oWriter.WriteString(_T(" fontAlgn=\"") + strProp + _T("\""));
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
					m_oWriter.WriteString(_T(" algn=\"") + strProp + _T("\""));
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
						str.Format(_T("<a:spcBef><a:spcPct val=\"%d\"/></a:spcBef>"), -val * 10000);
						m_oWriter.WriteString(str);
					}
				}

				if (pPF->hasBullet.is_init())
				{
					if (pPF->hasBullet.get())
					{
						m_oWriter.WriteString(_T("<a:buChar char=\"•\"/>"));
					}
					else
					{
						m_oWriter.WriteString(_T("<a:buNone/>"));
					}
				}

				m_oWriter.WriteString(_T("</a:pPr>"));

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
								m_oWriter.WriteString(_T("<a:endParaRPr lang=\"en-US\"/>"));
							}
							continue;
						}
					}

					NSPresentationEditor::CTextCFRun* pCF = &pParagraph->m_arSpans[nSpan].m_oRun;

					bool bIsBr = (_T("\n") == pParagraph->m_arSpans[nSpan].m_strText) ? true : false;
					
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
							m_oWriter.WriteString(_T(" b=\"1\""));
						else
							m_oWriter.WriteString(_T(" b=\"0\""));
					}
					if (pCF->FontItalic.is_init())
					{
						if (pCF->FontItalic.get())
							m_oWriter.WriteString(_T(" i=\"1\""));
						else
							m_oWriter.WriteString(_T(" i=\"0\""));
					}
					m_oWriter.WriteString(_T(">"));

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
						m_oWriter.WriteString(_T("<a:latin typeface=\"") + pCF->FontProperties->strFontName + _T("\"/>"));
					}

					m_oWriter.WriteString(_T("</a:rPr>"));

					if (!bIsBr)
					{
						CString strT1 = _T("<a:t>");
						m_oWriter.WriteString(strT1);

						CString strT = pParagraph->m_arSpans[nSpan].m_strText;
						NSPresentationEditor::CorrectXmlString(strT);
						m_oWriter.WriteString(strT);

						CString strT2 = _T("</a:t>");
						m_oWriter.WriteString(strT2);

						m_oWriter.WriteString(_T("</a:r>"));
					}
					else
					{
						m_oWriter.WriteString(_T("</a:br>"));
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