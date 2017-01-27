#pragma once
#include "Page.h"

namespace NSDocxRenderer
{
	static CString g_string_imageRID_png = _T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/image%d.png\"/>");
	static CString g_string_imageRID_jpg = _T("<Relationship Id=\"rId%d\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/image%d.jpg\"/>");

	static _bstr_t g_bstr_FontNameStart		= L"<w:font w:name=\"";
	static _bstr_t g_bstr_FontPanoseStart	= L"<w:panose1 w:val=\"";
	static _bstr_t g_bstr_FontCharsetStart	= L"<w:charset w:val=\"";
	static _bstr_t g_bstr_FontFamilyStart	= L"<w:family w:val=\"";
	static _bstr_t g_bstr_FontPitchTrue		= L"<w:pitch w:val=\"fixed\" />";
	static _bstr_t g_bstr_FontPitchFalse	= L"<w:pitch w:val=\"variable\" />";

	static CString g_string_SIG				= _T("<w:sig w:usb0=\"%08x\" w:usb1=\"%08x\" w:usb2=\"%08x\" w:usb3=\"%08x\" w:csb0=\"%08x\" w:csb1=\"%08x\"/>");
	static _bstr_t g_bstr_end				= L"\"/>";
	static _bstr_t g_bstr_end2				= L"\">";
	static _bstr_t g_bstr_FontNameEnd		= L"</w:font>";

	static CString g_string_DocumentStart	= L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<w:document xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\">\
<w:body>";

	static CString g_string_DocumentEnd		= L"</w:body></w:document>";

	class CDocument
	{
	public:
		NSStructures::CPen				m_oPen;
		NSStructures::CBrush			m_oBrush;
		NSStructures::CFont				m_oFont;
		NSStructures::CShadow			m_oShadow;
		NSStructures::CEdgeText			m_oEdge;

		NSStructures::CFont				m_oInstalledFont;

		AVSGraphics::IASCGraphicSimpleComverter*	m_pSimpleGraphicsConverter;
		AVSGraphics::IASCFontManager*				m_pFontManager;

		NSDocxRenderer::CMatrix						m_oTransform;

		LONG							m_lCurrentCommandType;

		LONG							m_lClipMode;
		CPage							m_oCurrentPage;

		IUnknown*						m_punkRenderer;

		CImageManager					m_oManager;

		double							m_dWidth;
		double							m_dHeight;

		double							m_dDpiX;
		double							m_dDpiY;

		CString							m_strTempDirectory;

		CFile							m_oDocumentStream;
		LONG							m_lPagesCount;

		NSDocxRenderer::CStringWriter	m_oWriter;
		bool							m_bIsNeedPDFTextAnalyzer;

	public:
		CDocument() : m_oWriter()
		{
			m_pSimpleGraphicsConverter  = NULL;
			m_lCurrentCommandType		= 0;

			m_pFontManager				= NULL;
			m_punkRenderer				= NULL;

			m_dWidth	= 0;
			m_dHeight	= 0;

			m_dDpiX		= 72;
			m_dDpiY		= 72;

			m_strTempDirectory			= _T("");
			m_lPagesCount				= 0;

			m_bIsNeedPDFTextAnalyzer	= false;
		}
		AVSINLINE void Clear()
		{
			m_lClipMode = 0;
			RELEASEINTERFACE(m_punkRenderer);
			RELEASEINTERFACE(m_pSimpleGraphicsConverter);
			RELEASEINTERFACE(m_pFontManager);
		}

		~CDocument()
		{
			m_lClipMode = 0;
			RELEASEINTERFACE(m_pSimpleGraphicsConverter);
			RELEASEINTERFACE(m_pFontManager);
		}

	public:

		AVSINLINE HRESULT NewPage()
		{
			if (0 != m_lPagesCount)
			{
				m_oCurrentPage.WriteSectionToFile(false, m_oWriter);
				m_oDocumentStream.WriteStringUTF8(m_oWriter.GetData());
				m_oWriter.ClearNoAttack();
			}
			
			m_oPen.SetDefaultParams();
			m_oBrush.SetDefaultParams();
			m_oFont.SetDefaultParams();
			m_oShadow.SetDefaultParams();
			m_oEdge.SetDefaultParams();

			m_oTransform.Reset();

			++m_lPagesCount;
			m_oCurrentPage.Clear();
			
			return S_OK;
		}
		AVSINLINE HRESULT get_Height(double* dHeight)
		{
			*dHeight = m_dHeight;
			return S_OK;
		}
		AVSINLINE HRESULT put_Height(double dHeight)
		{
			m_dHeight					= dHeight;
			m_oCurrentPage.m_dHeight	= dHeight;
			return S_OK;
		}
		AVSINLINE HRESULT get_Width(double* dWidth)
		{
			*dWidth = m_dWidth;
			return S_OK;
		}
		AVSINLINE HRESULT put_Width(double dWidth)
		{
			m_dWidth					= dWidth;
			m_oCurrentPage.m_dWidth		= dWidth;
			return S_OK;
		}
		AVSINLINE HRESULT get_DpiX(double* dDpiX)
		{
			*dDpiX = m_dDpiX;
			return S_OK;
		}
		AVSINLINE HRESULT get_DpiY(double* dDpiY)
		{
			*dDpiY = m_dDpiY;
			return S_OK;
		}
		//-------- Функции для задания настроек текста ----------------------------------------------
		// pen --------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetPen(BSTR bsXML)
		{
			m_oPen.FromXmlString((CString)bsXML);
			return S_OK;
		}
		AVSINLINE HRESULT get_PenColor(LONG* lColor)
		{
			*lColor = m_oPen.Color;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenColor(LONG lColor)
		{
			m_oPen.Color = lColor;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenAlpha(LONG* lAlpha)
		{
			*lAlpha = m_oPen.Alpha;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenAlpha(LONG lAlpha)
		{
			m_oPen.Alpha = lAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenSize(double* dSize)
		{
			*dSize = m_oPen.Size;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenSize(double dSize)
		{
			m_oPen.Size = dSize;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenDashStyle(BYTE* val)
		{
			*val = m_oPen.DashStyle;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenDashStyle(BYTE val)
		{
			m_oPen.DashStyle = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenLineStartCap(BYTE* val)
		{
			*val = m_oPen.LineStartCap;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineStartCap(BYTE val)
		{
			m_oPen.LineStartCap = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenLineEndCap(BYTE* val)
		{
			*val = m_oPen.LineEndCap;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineEndCap(BYTE val)
		{
			m_oPen.LineEndCap = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenLineJoin(BYTE* val)
		{
			*val = m_oPen.LineJoin;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineJoin(BYTE val)
		{
			m_oPen.LineJoin = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenDashOffset(double* val)
		{
			*val = m_oPen.DashOffset;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenDashOffset(double val)
		{
			m_oPen.DashOffset = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenAlign(LONG* val)
		{
			*val = m_oPen.Align;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenAlign(LONG val)
		{
			m_oPen.Align = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_PenMiterLimit(double* val)
		{
			*val = m_oPen.MiterLimit;
			return S_OK;
		}
		AVSINLINE HRESULT put_PenMiterLimit(double val)
		{
			m_oPen.MiterLimit = val;
			return S_OK;
		}
		AVSINLINE HRESULT PenDashPattern(SAFEARRAY* pPattern)
		{
			if (NULL != pPattern)
			{
				m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
			}

			return S_OK;
		}
		// brush ------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetBrush(BSTR bsXML)
		{
			m_oBrush.FromXmlString((CString)bsXML);
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushType(LONG* lType)
		{
			*lType = m_oBrush.Type;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushType(LONG lType)
		{
			m_oBrush.Type = lType;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushColor1(LONG* lColor)
		{
			*lColor = m_oBrush.Color1;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushColor1(LONG lColor)
		{
			m_oBrush.Color1 = lColor;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushAlpha1(LONG* lAlpha)
		{
			*lAlpha = m_oBrush.Alpha1;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushAlpha1(LONG lAlpha)
		{
			m_oBrush.Alpha1 = lAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushColor2(LONG* lColor)
		{
			*lColor = m_oBrush.Color2;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushColor2(LONG lColor)
		{
			m_oBrush.Color2 = lColor;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushAlpha2(LONG* lAlpha)
		{
			*lAlpha = m_oBrush.Alpha2;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushAlpha2(LONG lAlpha)
		{
			m_oBrush.Alpha2 = lAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushTexturePath(BSTR* bsPath)
		{
			*bsPath = m_oBrush.TexturePath.AllocSysString();
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTexturePath(BSTR bsPath)
		{
			m_oBrush.TexturePath = (CString)bsPath;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushTextureMode(LONG* lMode)
		{
			*lMode = m_oBrush.TextureMode;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTextureMode(LONG lMode)
		{
			m_oBrush.TextureMode = lMode;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)
		{
			*lTxAlpha = m_oBrush.TextureAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTextureAlpha(LONG lTxAlpha)
		{
			m_oBrush.TextureAlpha = lTxAlpha;
			return S_OK;
		}
		AVSINLINE HRESULT get_BrushLinearAngle(double* dAngle)
		{
			*dAngle = m_oBrush.LinearAngle;
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushLinearAngle(double dAngle)
		{
			m_oBrush.LinearAngle = dAngle;
			return S_OK;
		}
		AVSINLINE HRESULT BrushRect(BOOL val, double left, double top, double width, double height)
		{
			m_oBrush.Rectable = val;
			m_oBrush.Rect.X = (float)left;
			m_oBrush.Rect.Y = (float)top;
			m_oBrush.Rect.Width  = (float)width;
			m_oBrush.Rect.Height = (float)height;	

			return S_OK;
		}
		// font -------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetFont(BSTR bsXML)
		{
			m_oFont.FromXmlString((CString)bsXML);
			return S_OK;
		}
		AVSINLINE HRESULT get_FontName(BSTR* bsName)
		{
			*bsName = m_oFont.Name.AllocSysString();
			return S_OK;
		}
		AVSINLINE HRESULT put_FontName(BSTR bsName)
		{
			m_oFont.Name = (CString)bsName;
			return S_OK;
		}
		AVSINLINE HRESULT get_FontPath(BSTR* bsName)
		{
			*bsName = m_oFont.Path.AllocSysString();
			return S_OK;
		}
		AVSINLINE HRESULT put_FontPath(BSTR bsName)
		{
			m_oFont.Path = bsName;
			return S_OK;
		}
		AVSINLINE HRESULT get_FontSize(double* dSize)
		{
			*dSize = m_oFont.Size;
			return S_OK;
		}
		AVSINLINE HRESULT put_FontSize(double dSize)
		{
			m_oFont.Size = dSize;
			return S_OK;
		}
		AVSINLINE HRESULT get_FontStyle(LONG* lStyle)
		{
			*lStyle = m_oFont.GetStyle();
			return S_OK;
		}
		AVSINLINE HRESULT put_FontStyle(LONG lStyle)
		{
			m_oFont.SetStyle(lStyle);
			return S_OK;
		}
		AVSINLINE HRESULT get_FontStringGID(BOOL* bGID)
		{
			*bGID = m_oFont.StringGID;
			return S_OK;
		}
		AVSINLINE HRESULT put_FontStringGID(BOOL bGID)
		{
			m_oFont.StringGID = bGID;
			return S_OK;
		}
		AVSINLINE HRESULT get_FontCharSpace(double* dSpace)
		{
			*dSpace = m_oFont.CharSpace;
			return S_OK;
		}
		AVSINLINE HRESULT put_FontCharSpace(double dSpace)
		{
			m_oFont.CharSpace = dSpace;
			return S_OK;
		}
		// shadow -----------------------------------------------------------------------------------
		AVSINLINE HRESULT SetShadow(BSTR bsXML)
		{
			m_oShadow.FromXmlString((CString)bsXML);
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowDistanceX(double* val)
		{
			*val = m_oShadow.DistanceX;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowDistanceX(double val)
		{
			m_oShadow.DistanceX = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowDistanceY(double* val)
		{
			*val = m_oShadow.DistanceY;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowDistanceY(double val)
		{
			m_oShadow.DistanceY = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowBlurSize(double* val)
		{
			*val = m_oShadow.BlurSize;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowBlurSize(double val)
		{
			m_oShadow.BlurSize = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowColor(LONG* val)
		{
			*val = m_oShadow.Color;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowColor(LONG val)
		{
			m_oShadow.Color = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowAlpha(LONG* val)
		{
			*val = m_oShadow.Alpha;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowAlpha(LONG val)
		{
			m_oShadow.Alpha = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_ShadowVisible(BOOL* val)
		{
			*val = m_oShadow.Visible;
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowVisible(BOOL val)
		{
			m_oShadow.Visible = val;
			return S_OK;
		}
		// edge -------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetEdgeText(BSTR bsXML)
		{
			m_oEdge.FromXmlString((CString)bsXML);
			return S_OK;
		}
		AVSINLINE HRESULT get_EdgeVisible(LONG* val)
		{
			*val = m_oEdge.Visible;
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeVisible(LONG val)
		{
			m_oEdge.Visible = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_EdgeColor(LONG* val)
		{
			*val = m_oEdge.Color;
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeColor(LONG val)
		{
			m_oEdge.Color = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_EdgeAlpha(LONG* val)
		{
			*val = m_oEdge.Alpha;
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeAlpha(LONG val)
		{
			m_oEdge.Alpha = val;
			return S_OK;
		}
		AVSINLINE HRESULT get_EdgeDist(double* val)
		{
			*val = m_oEdge.Dist;
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeDist(double val)
		{
			m_oEdge.Dist = val;
			return S_OK;
		}

		//-------- Функции для вывода текста --------------------------------------------------------
		AVSINLINE HRESULT CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			double dAngleMatrix = m_oTransform.z_Rotation();
			if (abs(dAngleMatrix) > 1)
			{
				PathCommandEnd();
				BeginCommand(c_nPathType);
				PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
				DrawPath(c_nWindingFillMode);
				EndCommand(c_nPathType);
				PathCommandEnd();
				return S_OK;
			}

			m_oCurrentPage.WriteText(bsText, NULL, fX, fY, fWidth, fHeight, fBaseLineOffset, m_bIsNeedPDFTextAnalyzer);
			return S_OK;
		}
		AVSINLINE HRESULT CommandDrawTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			double dAngleMatrix = m_oTransform.z_Rotation();
			if (abs(dAngleMatrix) > 1)
			{
				PathCommandEnd();
				BeginCommand(c_nPathType);
				PathCommandTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
				DrawPath(c_nWindingFillMode);
				EndCommand(c_nPathType);
				PathCommandEnd();
				return S_OK;
			}

			m_oCurrentPage.WriteText(bsUnicodeText, bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset, m_bIsNeedPDFTextAnalyzer);
			return S_OK;
		}
		//-------- Маркеры для команд ---------------------------------------------------------------
		AVSINLINE HRESULT BeginCommand(DWORD lType)
		{
			m_lCurrentCommandType = (LONG)lType;
			m_oCurrentPage.m_lCurrentCommand	= m_lCurrentCommandType;

			return S_OK;
		}
		AVSINLINE HRESULT EndCommand(DWORD lType)
		{
			m_lCurrentCommandType				= -1;
			m_oCurrentPage.m_lCurrentCommand	= m_lCurrentCommandType;

			if (c_nPageType == lType)
			{
				// нужно записать страницу в файл
				m_oCurrentPage.Build();
				m_oCurrentPage.Write(m_oWriter);
			}
			else if (c_nPathType == lType)
			{
				m_oCurrentPage.End();
			}

			return S_OK;
		}
		//-------- Функции для работы с Graphics Path -----------------------------------------------
		AVSINLINE HRESULT PathCommandMoveTo(double fX, double fY)
		{
			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.MoveTo(fX, fY);
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandMoveTo(fX, fY);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandLineTo(double fX, double fY)
		{
			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.LineTo(fX, fY);
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandLineTo(fX, fY);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandLinesTo(SAFEARRAY* pPoints)
		{
			m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
		{
			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandCurvesTo(SAFEARRAY* pPoints)
		{
			m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
		{
			m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandClose()
		{
			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.Close();
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandClose();
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandEnd()
		{
			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.End();
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandEnd();
			}
			return S_OK;
		}
		AVSINLINE HRESULT DrawPath(long nType)
		{
			LONG lTxId = -1;
			if ((nType > 0xFF) && (c_BrushTypeTexture == m_oBrush.Type))
			{
				double x = 0;
				double y = 0;
				double w = 0;
				double h = 0;
				CImageInfo oInfo = m_oManager.WriteImage(m_oBrush.TexturePath, x, y, w, h);
				lTxId = oInfo.m_lID;
			}

			m_oCurrentPage.DrawPath(nType, lTxId);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandStart()
		{
			if (c_nSimpleGraphicType == m_lCurrentCommandType)
			{
				m_oCurrentPage.Start();
			}
			else
			{
				m_pSimpleGraphicsConverter->PathCommandStart();
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandGetCurrentPoint(double* fX, double* fY)
		{
			m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
			return S_OK;
		}

		AVSINLINE HRESULT PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			_SetFont();
			m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			if (NULL != bsGidText)
			{
				m_oFont.StringGID = TRUE;
				return PathCommandText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
			}
			
			m_oFont.StringGID = FALSE;
			return PathCommandText(bsUnicodeText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		}
		
		AVSINLINE HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
		{
			return S_OK;
		}
		AVSINLINE HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
		{
			ApplyTransform2(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
			return S_OK;
		}
		//-------- Функции для вывода изображений --------------------------------------------------
		AVSINLINE HRESULT DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
		{
			CImageInfo oInfo = m_oManager.WriteImage(pInterface, fX, fY, fWidth, fHeight);
			m_oCurrentPage.WriteImage(oInfo, fX, fY, fWidth, fHeight);
			return S_OK;
		}
		AVSINLINE HRESULT DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
		{
			CImageInfo oInfo = m_oManager.WriteImage((CString)bstrVal, fX, fY, fWidth, fHeight);
			m_oCurrentPage.WriteImage(oInfo, fX, fY, fWidth, fHeight);
			return S_OK;
		}
		//------------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
		{
			return S_OK;
		}
		AVSINLINE HRESULT GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
		{
			return S_OK;
		}

		AVSINLINE HRESULT SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
		{
			ApplyTransform(dA, dB, dC, dD, dE, dF);
			return S_OK;
		}
		AVSINLINE HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
		{
			return S_OK;
		}
		AVSINLINE HRESULT ResetTransform(void)
		{
			m_oTransform.Reset();
			return S_OK;
		}
		AVSINLINE HRESULT get_ClipMode(LONG* plMode)
		{
			*plMode = m_lClipMode;
			return S_OK;
		}
		AVSINLINE HRESULT put_ClipMode(LONG lMode)
		{
			m_lClipMode = lMode;
			return S_OK;
		}

	protected:
		AVSINLINE void ApplyTransform(double d1, double d2, double d3, double d4, double d5, double d6)
		{
			m_oTransform.SetElements(d1, d2, d3, d4, d5, d6);
		}

		void ApplyTransform2(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
		{
			if ((dWidth <= 1) || (dHeight <= 1))
				lFlags = 0;

			BOOL bFlipX = (0 != (c_nParamFlipX & lFlags));
			BOOL bFlipY = (0 != (c_nParamFlipY & lFlags));
			
			double m11 = bFlipX ? -1.0 : 1.0;
			double m22 = bFlipY ? -1.0 : 1.0;
			
			NSDocxRenderer::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

			if ((0 != dAngle) || (0 != lFlags))
			{
				double dCentreX = (dLeft + dWidth / 2.0);
				double dCentreY = (dTop + dHeight / 2.0);
				
				oMatrix.Translate(-dCentreX, -dCentreY	, Aggplus::MatrixOrderAppend);
				
				oMatrix.Rotate(dAngle			, Aggplus::MatrixOrderAppend);
				oMatrix.Scale(m11, m22					, Aggplus::MatrixOrderAppend);
				
				oMatrix.Translate(dCentreX, dCentreY	, Aggplus::MatrixOrderAppend);
			}

			m_oTransform = oMatrix;
		}

		void _SetFont()
		{
			if (NULL == m_pFontManager)
			{
				CoCreateInstance(__uuidof(AVSGraphics::CASCFontManager), NULL, CLSCTX_ALL, __uuidof(AVSGraphics::IASCFontManager), (void**)&m_pFontManager);
				m_pFontManager->Initialize(L"");
			}

			double dPix = m_oFont.CharSpace * m_dDpiX / 25.4;
			
			if (m_oInstalledFont.IsEqual(&m_oFont))
			{
				if (1 < m_dWidth)
				{
					m_pFontManager->SetCharSpacing(dPix);
				}
				return;
			}

			m_pFontManager->SetStringGID(m_oFont.StringGID);
			if (1 < m_dWidth)
			{
				m_pFontManager->SetCharSpacing(dPix);
			}

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
		
		bool CreateDocument(IUnknown* pRenderer, CString strTempDirectory)
		{
			HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
			m_strTempDirectory = strTempDirectory;

			// rels
			CString strRels = strTempDirectory + _T("\\_rels");
			CDirectory::CreateDirectory(strRels);
			
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_RELS), _T("DOCXREND"), strRels + _T("\\.rels"));

			// docProps
			CString strDocProps = strTempDirectory + _T("\\docProps");
			CDirectory::CreateDirectory(strDocProps);

			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_APP),  _T("DOCXREND"), strDocProps + _T("\\app.xml"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_CORE), _T("DOCXREND"), strDocProps + _T("\\core.xml"));

			// contentTypes
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_CONTENT_TYPES), _T("DOCXREND"), strTempDirectory + _T("\\[Content_Types].xml"));

			// word
			CString strWord = strTempDirectory + _T("\\word");
			CDirectory::CreateDirectory(strWord);

			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_SETTINGS),		_T("DOCXREND"), strWord + _T("\\settings.xml"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_STYLES),		_T("DOCXREND"), strWord + _T("\\styles.xml"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_WEBSETTINGS),	_T("DOCXREND"), strWord + _T("\\webSettings.xml"));

			// theme
			CString strTheme = strWord + _T("\\theme");
			CDirectory::CreateDirectory(strTheme);
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_DEFAULT_DOC_THEME),	_T("DOCXREND"), strTheme + _T("\\theme.xml"));

			// documentRels
			CDirectory::CreateDirectory(strWord + _T("\\_rels"));

			// Init
			Clear();

			CoCreateInstance(__uuidof(AVSGraphics::CASCGraphicSimpleComverter), NULL, CLSCTX_ALL, 
				__uuidof(AVSGraphics::IASCGraphicSimpleComverter), (void**)&m_pSimpleGraphicsConverter);

			m_punkRenderer = pRenderer;
			ADDREFINTERFACE(m_punkRenderer);
			m_pSimpleGraphicsConverter->put_Renderer(m_punkRenderer);
			m_lCurrentCommandType = 0;

			m_oCurrentPage.Init(&m_oFont, &m_oPen, &m_oBrush, &m_oShadow, &m_oEdge, &m_oTransform, m_pSimpleGraphicsConverter);

			m_oManager.NewDocument();
			// media
			m_oManager.m_strDstMedia = strWord + _T("\\media");
			CDirectory::CreateDirectory(m_oManager.m_strDstMedia);

			m_oCurrentPage.m_oManager.m_oFontTable.m_mapTable.RemoveAll();

			m_oDocumentStream.CloseFile();
			m_oDocumentStream.CreateFile(strWord + _T("\\document.xml"));
			m_oDocumentStream.WriteStringUTF8(g_string_DocumentStart);

			m_lPagesCount				= 0;
			m_oWriter.Clear();
			m_oWriter.AddSize(10000);

			return true;
		}

		void Close()
		{
			// сохраним rels (images & docs)
			CFile oFile;
			oFile.CreateFile(m_strTempDirectory + _T("\\word\\_rels\\document.xml.rels"));

			NSDocxRenderer::CStringWriter oWriter;

			CString strStart = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>\
<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>\
<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>\
<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>\
<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme.xml\"/>");

			oWriter.WriteString(strStart);

			POSITION pos = m_oManager.m_mapImageData.GetStartPosition();
			while (NULL != pos)
			{
				CAtlMap<DWORD, CImageInfo>::CPair* pPair = m_oManager.m_mapImageData.GetNext(pos);

				LONG lId = pPair->m_value.m_lID;
				if (pPair->m_value.m_eType == itPNG)
				{
					CString strImage = _T("");
					strImage.Format(g_string_imageRID_png, 10 + lId, lId);
					oWriter.WriteString(strImage);
				}
				else
				{
					CString strImage = _T("");
					strImage.Format(g_string_imageRID_jpg, 10 + lId, lId);
					oWriter.WriteString(strImage);
				}
			}

			pos = m_oManager.m_mapImagesFile.GetStartPosition();
			while (NULL != pos)
			{
				CAtlMap<CString, CImageInfo>::CPair* pPair = m_oManager.m_mapImagesFile.GetNext(pos);

				LONG lId = pPair->m_value.m_lID;
				if (pPair->m_value.m_eType == itPNG)
				{
					CString strImage = _T("");
					strImage.Format(g_string_imageRID_png, 10 + lId, lId);
					oWriter.WriteString(strImage);
				}
				else
				{
					CString strImage = _T("");
					strImage.Format(g_string_imageRID_jpg, 10 + lId, lId);
					oWriter.WriteString(strImage);
				}
			}

			CString strEnd = _T("</Relationships>"); 
			oWriter.WriteString(strEnd);

			oFile.WriteStringUTF8(oWriter.GetData());
			oFile.CloseFile();

			// сохраним fontTable
			CFile oFileFontTable;
			oFileFontTable.CreateFile(m_strTempDirectory + _T("\\word\\fontTable.xml"));

			oWriter.ClearNoAttack();

			strStart = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<w:fonts xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">");

			oWriter.WriteString(strStart);

			CFontTable* pFontTable = &m_oCurrentPage.m_oManager.m_oFontTable;

			pos = pFontTable->m_mapTable.GetStartPosition();
			while (NULL != pos)
			{
				CAtlMap<CString, CFontTableEntry>::CPair* pPair = pFontTable->m_mapTable.GetNext(pos);

				oWriter.WriteString(g_bstr_FontNameStart);
				oWriter.WriteString((CString)pPair->m_value.m_strFamilyName);
				oWriter.WriteString(g_bstr_end2);

				oWriter.WriteString(g_bstr_FontPanoseStart);
				oWriter.WriteString(pPair->m_value.m_strPANOSE);
				oWriter.WriteString(g_bstr_end);

				oWriter.WriteString(g_bstr_FontCharsetStart);
				oWriter.WriteString((CString)_T("00"));
				oWriter.WriteString(g_bstr_end);

				//oWriter.WriteString(g_bstr_FontFamilyStart);
				//oWriter.WriteString((CString)_T("roman"));
				//oWriter.WriteString(g_bstr_end);

				if (pPair->m_value.m_bIsFixedWidth)
				{
					oWriter.WriteString(g_bstr_FontPitchTrue);
				}
				else
				{
					oWriter.WriteString(g_bstr_FontPitchFalse);
				}

				CString strSig = _T("");
				strSig.Format(g_string_SIG, pPair->m_value.m_arSignature[0], pPair->m_value.m_arSignature[1], pPair->m_value.m_arSignature[2], 
					pPair->m_value.m_arSignature[3], pPair->m_value.m_arSignature[4], pPair->m_value.m_arSignature[5]);

				oWriter.WriteString(strSig);

				oWriter.WriteString(g_bstr_FontNameEnd);		
			}

			strEnd = _T("</w:fonts>");
			oWriter.WriteString(strEnd);

			oFileFontTable.WriteStringUTF8(oWriter.GetData());
			oFileFontTable.CloseFile();

			// document
			m_oCurrentPage.WriteSectionToFile(true, m_oWriter);
			m_oWriter.WriteString(g_string_DocumentEnd);
			m_oDocumentStream.WriteStringUTF8(m_oWriter.GetData());
			m_oWriter.ClearNoAttack();

			m_oDocumentStream.CloseFile();
		}

	protected:

		void LoadResourceFile(HINSTANCE hInst, LPCTSTR sResName, LPCTSTR sResType, const CString& strDstFile)
		{
			HRSRC hrRes = FindResource(hInst, sResName, sResType);
			if (!hrRes)
				return;

			HGLOBAL hGlobal = LoadResource(hInst, hrRes);
			DWORD sz = SizeofResource(hInst, hrRes);
			void* ptrRes = LockResource(hGlobal);
			
			CFile oFile;
			oFile.CreateFile(strDstFile);
			oFile.WriteFile(ptrRes, sz);

			UnlockResource(hGlobal);
			FreeResource(hGlobal);
		}			
	};
}