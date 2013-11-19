// DOCXWriter.cpp : Implementation of CAVSDocxRenderer

#include "stdafx.h"
#include "DocxRenderer.h"


// CDOCXWriter

STDMETHODIMP CAVSOfficeDocxRenderer3::get_Type(LONG* lType)
{
	if (NULL != lType)
		*lType = c_nDocxWriter;
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::NewPage()
{
	m_dWidth	= -1;
	m_dHeight	= -1;

	m_bPageAdded = false;
	m_lCommandFlags = 0;
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::get_Height(double* dHeight)
{
	*dHeight = m_dHeight;
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::put_Height(double dHeight)
{
	m_dHeight = dHeight;

	if (0 < m_dWidth && !m_bPageAdded)
	{
		m_oWriter.BeginPage(m_dWidth, m_dHeight);
		m_bPageAdded = true;
	}

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::get_Width(double* dWidth)
{
	*dWidth = m_dWidth;
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::put_Width(double dWidth)
{
	m_dWidth = dWidth;

	if (0 < m_dHeight && !m_bPageAdded)
	{
		m_oWriter.BeginPage(m_dWidth, m_dHeight);
		m_bPageAdded = true;
	}

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::get_DpiX(double* dDpiX)
{
	*dDpiX = m_dDpiX;
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::get_DpiY(double* dDpiY)
{
	*dDpiY = m_dDpiY;
	return S_OK;
}

/*========================================================================================================*/

// pen --------------------------------------------------------------------------------------
STDMETHODIMP CAVSOfficeDocxRenderer3::SetPen(BSTR bsXML)
{
	m_oPen.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetPen(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenColor(LONG lColor)
{
	m_oPen.Color = lColor;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenColor(lColor);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenAlpha(LONG lAlpha)
{
	m_oPen.Alpha = lAlpha;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenAlpha(lAlpha);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenSize(double dSize)
{
	m_oPen.Size = dSize;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenSize(dSize);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenDashStyle(BYTE val)
{
	m_oPen.DashStyle = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenDashStyle(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenLineStartCap(BYTE val)
{
	m_oPen.LineStartCap = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenLineStartCap(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenLineEndCap(BYTE val)
{
	m_oPen.LineEndCap = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenLineEndCap(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenLineJoin(BYTE val)
{
	m_oPen.LineJoin = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenLineJoin(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenDashOffset(double val)
{
	m_oPen.DashOffset = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenDashOffset(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenAlign(LONG val)
{
	m_oPen.Align = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenAlign(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_PenMiterLimit(double val)
{
	m_oPen.MiterLimit = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenMiterLimit(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PenDashPattern(SAFEARRAY* pPattern)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
	}

	if (NULL != m_pRenderers)
		m_pRenderers->PenDashPattern(pPattern);

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP CAVSOfficeDocxRenderer3::SetBrush(BSTR bsXML)
{
	m_oBrush.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetBrush(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_BrushType(LONG lType)
{
	m_oBrush.Type = lType;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushType(lType);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_BrushColor1(LONG lColor)
{
	m_oBrush.Color1 = lColor;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushColor1(lColor);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_BrushAlpha1(LONG lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushAlpha1(lAlpha);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_BrushColor2(LONG lColor)
{
	m_oBrush.Color2 = lColor;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushColor2(lColor);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_BrushAlpha2(LONG lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushAlpha2(lAlpha);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_BrushTexturePath(BSTR* bsPath)
{
	*bsPath = m_oBrush.TexturePath.AllocSysString();
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_BrushTexturePath(BSTR bsPath)
{
	m_oBrush.TexturePath = (CString)bsPath;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushTexturePath(bsPath);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_BrushTextureMode(LONG lMode)
{
	m_oBrush.TextureMode = lMode;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushTextureMode(lMode);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_BrushTextureAlpha(LONG lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushTextureAlpha(lTxAlpha);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_BrushLinearAngle(double dAngle)
{
	m_oBrush.LinearAngle = dAngle;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushLinearAngle(dAngle);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::BrushRect(BOOL val, double left, double top, double width, double height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;

	if (NULL != m_pRenderers)
		m_pRenderers->BrushRect(val, left, top, width, height);

	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP CAVSOfficeDocxRenderer3::SetFont(BSTR bsXML)
{
	m_oFont.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetFont(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_FontName(BSTR* bsName)
{
	*bsName = m_oFont.Name.AllocSysString();
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_FontName(BSTR bsName)
{
	m_oFont.Name = (CString)bsName;

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontName(bsName);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_FontPath(BSTR* bsName)
{
	*bsName = m_oFont.Path.AllocSysString();
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_FontPath(BSTR bsName)
{
	m_oFont.Path = bsName;

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontPath(bsName);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_FontSize(double dSize)
{
	m_oFont.Size = dSize;

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontSize(dSize);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_FontStyle(LONG lStyle)
{
	m_oFont.SetStyle(lStyle);

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontStyle(lStyle);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_FontStringGID(BOOL* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_FontStringGID(BOOL bGID)
{
	m_oFont.StringGID = bGID;

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontStringGID(bGID);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_FontCharSpace(double dSpace)
{
	m_oFont.CharSpace = dSpace;

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontCharSpace(dSpace);

	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP CAVSOfficeDocxRenderer3::SetShadow(BSTR bsXML)
{
	m_oShadow.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetShadow(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_ShadowDistanceX(double val)
{
	m_oShadow.DistanceX = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowDistanceX(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_ShadowDistanceY(double val)
{
	m_oShadow.DistanceY = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowDistanceY(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_ShadowBlurSize(double val)
{
	m_oShadow.BlurSize = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowBlurSize(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_ShadowColor(LONG val)
{
	m_oShadow.Color = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowColor(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_ShadowAlpha(LONG val)
{
	m_oShadow.Alpha = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowAlpha(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_ShadowVisible(BOOL* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_ShadowVisible(BOOL val)
{
	m_oShadow.Visible = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowVisible(val);

	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP CAVSOfficeDocxRenderer3::SetEdgeText(BSTR bsXML)
{
	m_oEdge.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetEdgeText(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_EdgeVisible(LONG val)
{
	m_oEdge.Visible = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_EdgeVisible(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_EdgeColor(LONG val)
{
	m_oEdge.Color = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_EdgeColor(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_EdgeAlpha(LONG val)
{
	m_oEdge.Alpha = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_EdgeAlpha(val);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_EdgeDist(double val)
{
	m_oEdge.Dist = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_EdgeDist(val);

	return S_OK;
}

// ------------------------------------------------------------------------------------------
/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (!CheckNeedCommands())
		return S_OK;

	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	if ((abs(m_dTransformAngle) > 1) || (m_bIsMetafileDrawing == true))
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}

	double x = fX;
	double y = fY;
	m_oTransform.TransformPoint(x, y);

	double r = fX + fWidth;
	double b = fY + fHeight;
	m_oTransform.TransformPoint(r, b);

	r -= x;
	b -= y;

	double dInputBaseOffset = fBaseLineOffset;
	m_oWriter.WriteText(bsText, NULL, x, y, r, b, fBaseLineOffset);

	if (NULL != m_pRenderers)
		m_pRenderers->CommandDrawText(bsText, fX, fY, fWidth, fHeight, dInputBaseOffset);

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::CommandDrawTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if (!CheckNeedCommands())
		return S_OK;

	if ((abs(m_dTransformAngle) > 1) || (m_bIsMetafileDrawing == true))
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}

	double x = fX;
	double y = fY;
	m_oTransform.TransformPoint(x, y);

	double r = fX + fWidth;
	double b = fY + fHeight;
	m_oTransform.TransformPoint(r, b);

	r -= x;
	b -= y;

	double dInputBaseOffset = fBaseLineOffset;
	m_oWriter.WriteText(bsUnicodeText, bsGidText, x, y, r, b, fBaseLineOffset);

	if (NULL != m_pRenderers)
		m_pRenderers->CommandDrawTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, dInputBaseOffset, lFlags);

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::BeginCommand(DWORD lType)
{
	m_lCurrentCommandType = lType;
	SendBeginCommandsToFlags();

	if ((c_nShapeGroup == m_lCurrentCommandType) || (c_nShape == m_lCurrentCommandType))
		m_oWriter.BeginShape();

	if (NULL != m_pRenderers)
		m_pRenderers->BeginCommand(lType);

	if (!CheckNeedCommands())
		return S_OK;

	if (c_nImageType == lType)
	{
		m_bIsMetafileDrawing = true;
	}

	if (c_nBlock == lType)
	{
		if (0 == m_lSrcType)
		{
			NSDocx::CLogicPageDOCX* page = dynamic_cast<NSDocx::CLogicPageDOCX*>(m_oWriter.m_pCurrentPage);
			if (NULL != page)
				page->BeginParagraph();
		}
	}
	else if (c_nLine == lType)
	{
		if (0 == m_lSrcType)
		{
			NSDocx::CLogicPageDOCX* page = dynamic_cast<NSDocx::CLogicPageDOCX*>(m_oWriter.m_pCurrentPage);
			if (NULL != page)
				page->BeginLine();
		}
	}

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::EndCommand(DWORD lType)
{
	if (NULL != m_pRenderers)
		m_pRenderers->EndCommand(lType);

	if ((c_nShapeGroup == lType) || (c_nShape == lType))
		m_oWriter.EndShape();

	if (c_nPageType == lType)
	{
		m_oWriter.EndPage();
	}

	SendEndCommandsToFlags(lType);
	m_lCurrentCommandType = -1;

	if (!CheckNeedCommands())
		return S_OK;

	if (c_nImageType == lType)
	{
		m_bIsMetafileDrawing = false;
	}

	if (c_nBlock == lType)
	{
		if (0 == m_lSrcType)
		{
			NSDocx::CLogicPageDOCX* page = dynamic_cast<NSDocx::CLogicPageDOCX*>(m_oWriter.m_pCurrentPage);
			if (NULL != page)
				page->EndParagraph();
		}
	}
	else if (c_nLine == lType)
	{
		if (0 == m_lSrcType)
		{
			NSDocx::CLogicPageDOCX* page = dynamic_cast<NSDocx::CLogicPageDOCX*>(m_oWriter.m_pCurrentPage);
			if (NULL != page)
				page->EndLine();
		}
	}

	return S_OK;
}

/*========================================================================================================*/

// path commands ----------------------------------------------------------------------------
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandMoveTo(double fX, double fY)
{
	if (!CheckNeedCommands())
		return S_OK;

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		MoveTo(fX, fY);
	}
	else
	{
		double x = fX;
		double y = fY;
		m_oFullTransform.TransformPoint(x, y);
		m_pSimpleGraphicsConverter->PathCommandMoveTo(x, y);

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandMoveTo(fX, fY);
	}

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandLineTo(double fX, double fY)
{
	if (!CheckNeedCommands())
		return S_OK;

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		LineTo(fX, fY);
	}
	else
	{
		double x = fX;
		double y = fY;
		m_oFullTransform.TransformPoint(x, y);
		m_pSimpleGraphicsConverter->PathCommandLineTo(x, y);

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandLineTo(fX, fY);
	}

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	if (!CheckNeedCommands())
		return S_OK;

	m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);

	if (NULL != m_pRenderers)
		m_pRenderers->PathCommandLinesTo(pPoints);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	if (!CheckNeedCommands())
		return S_OK;

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	else
	{
		double x1 = fX1;
		double y1 = fY1;
		m_oFullTransform.TransformPoint(x1, y1);

		double x2 = fX1;
		double y2 = fY2;
		m_oFullTransform.TransformPoint(x2, y2);

		double x3 = fX3;
		double y3 = fX3;
		m_oFullTransform.TransformPoint(x3, y3);

		m_pSimpleGraphicsConverter->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	if (!CheckNeedCommands())
		return S_OK;

	m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);

	if (NULL != m_pRenderers)
		m_pRenderers->PathCommandCurvesTo(pPoints);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	if (!CheckNeedCommands())
		return S_OK;

	m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);

	if (NULL != m_pRenderers)
		m_pRenderers->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandClose()
{
	if (!CheckNeedCommands())
		return S_OK;

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Close();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandClose();

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandClose();
	}

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandEnd()
{
	if (!CheckNeedCommands())
		return S_OK;

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		PathEnd();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandEnd();
	}

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandStart()
{
	if (!CheckNeedCommands())
		return S_OK;

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		PathStart();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandStart();

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandStart();
	}

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	if (!CheckNeedCommands())
		return S_OK;

	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}

STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (!CheckNeedCommands())
		return S_OK;

	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);

	if (NULL != m_pRenderers)
		m_pRenderers->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::PathCommandTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if (!CheckNeedCommands())
		return S_OK;

	if (NULL != m_pRenderers)
		m_pRenderers->PathCommandTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
	
	if (NULL != bsGidText)
	{
		m_oFont.StringGID = TRUE;
		return PathCommandText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	
	m_oFont.StringGID = FALSE;
	return PathCommandText(bsUnicodeText, fX, fY, fWidth, fHeight, fBaseLineOffset);
}
STDMETHODIMP CAVSOfficeDocxRenderer3::DrawPath(long nType)
{
	if (!CheckNeedCommands())
		return S_OK;

	if (NULL != m_pRenderers)
		m_pRenderers->DrawPath(nType);

	m_oWriter.DrawPath(nType);

	return S_OK;
}

/*========================================================================================================*/
// transform commands -----------------------------------------------------------------------
STDMETHODIMP CAVSOfficeDocxRenderer3::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	ApplyTransform2(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);

	if (NULL != m_pRenderers)
		m_pRenderers->SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	ApplyTransform(dA, dB, dC, dD, dE, dF);

	if (NULL != m_pRenderers)
		m_pRenderers->SetTransform(dA, dB, dC, dD, dE, dF);

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::ResetTransform(void)
{
	m_oTransform.Reset();

	if (NULL != m_pRenderers)
		m_pRenderers->ResetTransform();

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::put_ClipMode(LONG lMode)
{
	m_lClipMode = lMode;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ClipMode(lMode);

	return S_OK;
}
/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	if (!CheckNeedCommands())
		return S_OK;

	if (NULL != m_pRenderers)
		m_pRenderers->DrawImage(pInterface, fX, fY, fWidth, fHeight);

	m_oWriter.WriteImage(pInterface, fX, fY, fWidth, fHeight);

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	if (!CheckNeedCommands())
		return S_OK;

	if (NULL != m_pRenderers)
		m_pRenderers->DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);

	m_oWriter.WriteImage((CString)bstrVal, fX, fY, fWidth, fHeight);

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	if (NULL != m_pRenderers)
		m_pRenderers->SetAdditionalParam(ParamName, ParamValue);

	CString sParamName = ParamName;
	if (_T("EffectPainter") == sParamName && VT_UNKNOWN == ParamValue.vt)
	{
		RELEASEINTERFACE(m_oWriter.m_pPainter);
		
		if (NULL != ParamValue.punkVal)
		{
			ParamValue.punkVal->QueryInterface(AVSGraphics::IID_IAVSEffectPainter, (void**)&m_oWriter.m_pPainter);
		}
	}

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}

/*========================================================================================================*/

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::Initialize(BSTR bsXMLOptions)
{
	BaseInitialize(bsXMLOptions);
	return S_OK;
}

STDMETHODIMP CAVSOfficeDocxRenderer3::SetMetadata(BSTR bsXMLOptions)
{
	m_oWriter.m_oFormat.m_oMeta.Clear();

	XmlUtils::CXmlNode oNodeMeta;
	if (oNodeMeta.FromXmlString((CString)bsXMLOptions))
	{
		m_oWriter.m_oFormat.m_oMeta.m_strAuthor		= oNodeMeta.ReadValueString(_T("author"));
		m_oWriter.m_oFormat.m_oMeta.m_strName		= oNodeMeta.ReadValueString(_T("name"));
		m_oWriter.m_oFormat.m_oMeta.m_strPublisher	= oNodeMeta.ReadValueString(_T("publisher"));

		m_oWriter.m_bIsSrcCoverExist				= (1 == oNodeMeta.ReadValueInt(_T("coverpage")));
		
		XmlUtils::CXmlNode oNodeAnno;
		if (oNodeMeta.GetNode(_T("annotation"), oNodeAnno))
		{
			XmlUtils::CXmlNodes oNodes;
			if (oNodeAnno.GetChilds(oNodes))
			{
				int nCount = oNodes.GetCount();

				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNodeS;
					oNodes.GetAt(i, oNodeS);

					CString sName = oNodeS.GetName();

					if (_T("#text") == sName)
					{
						NSDocx::CDocxParagraph* pParagraph = new NSDocx::CDocxParagraph();
						pParagraph->m_arLines.AddTail();
						NSDocx::CDocxParagraph::CSpan oSpan;
						oSpan.m_oText += oNodeS.GetXml();
						pParagraph->m_arLines.GetTail().m_listSpans.AddTail(oSpan);
						m_oWriter.m_oFormat.m_oMeta.m_listAnnotation.AddTail(pParagraph);
					}
					else if (_T("p") == sName)
					{
						NSDocx::CDocxParagraph* pParagraph = new NSDocx::CDocxParagraph();
						pParagraph->m_arLines.AddTail();
						NSDocx::CDocxParagraph::CSpan oSpan;
						oSpan.m_oText += oNodeS.GetText();
						pParagraph->m_arLines.GetTail().m_listSpans.AddTail(oSpan);
						m_oWriter.m_oFormat.m_oMeta.m_listAnnotation.AddTail(pParagraph);
					}
				}
			}
		}
	}
	return S_OK;
}

/*========================================================================================================*/
STDMETHODIMP CAVSOfficeDocxRenderer3::CreateOfficeFile(BSTR bsFilePath, LONG lFileType)
{
	m_lDstType = lFileType;
	
	m_oWriter.m_oFormat.m_oImageManager.m_lDstFormat = m_lDstType;
	m_oWriter.m_oFormat.m_oTempManager.m_strTempDirectory = m_strTempDirectory;
	m_pRenderers = m_oWriter.CreateDocument(m_lSrcType, m_lDstType);
	m_oWriter.SetParametres(&m_oPen, &m_oBrush, &m_oFont, &m_oShadow, &m_oEdge, m_pSimpleGraphicsConverter, &m_oTransform);

	m_strDstFilePath = (CString)bsFilePath;

	// ----------------cover-------------------------
	if (!m_oWriter.m_bIsSrcCoverExist && m_oWriter.m_bIsSaveCover)
	{
		SaveCover();
	}
	// ----------------------------------------------

	m_oWriter.InitWatermark();

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSOfficeDocxRenderer3::CloseFile()
{
	m_oWriter.EndFile();
	if (false)
		m_oWriter.m_oFormat.SaveToFile(m_oWriter.m_oFormat.m_oTempManager.m_strTempDirectory + _T("\\format.xml"));

	if (_T("") == m_oWriter.m_oFormat.m_oMeta.m_strName)
	{
		CString strName = m_strDstFilePath;
		int nEnd = strName.ReverseFind(TCHAR('.'));
		int nStr1 = strName.ReverseFind(TCHAR('\\'));
		int nStr2 = strName.ReverseFind(TCHAR('/'));

		int nStr = max(nStr1, nStr2);

		if ((-1 != nStr) && (-1 != nEnd) && (nStr < nEnd))
		{
			m_oWriter.m_oFormat.m_oMeta.m_strName = strName.Mid(nStr + 1, nEnd - nStr - 1);
		}
	}
	
	switch (m_lDstType)
	{
	case 0:
		{
			NSDocx::CWriter_Docx epub;
			epub.CreateDocument(&m_oWriter);
			epub.CloseFile(m_strDstFilePath, &m_oWriter);
			break;
		}	
	default:
		break;
	}

	m_oWriter.CloseFile();

	return S_OK;
}

/*========================================================================================================*/

// AVSRenderer2
STDMETHODIMP CAVSOfficeDocxRenderer3::CommandLong(LONG lCommand)
{
	if (lCommand == c_nPageBreak)
	{
		if (NULL != m_oWriter.m_pCurrentPage)
		{
			m_oWriter.m_pCurrentPage->WritePagePreak();
		}
	}
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::CommandDouble(double dCommand)
{
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::CommandString(BSTR bsCommand)
{
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::CommandVariant(VARIANT vCommand)
{
	return S_OK;
}

STDMETHODIMP CAVSOfficeDocxRenderer3::CommandLong2(LONG lType, LONG lCommand)
{
	if (c_nBlockHeadingLevel == lType)
	{
		NSDocx::CLogicPageDOCX* page = dynamic_cast<NSDocx::CLogicPageDOCX*>(m_oWriter.m_pCurrentPage);
		if (NULL != page)
			page->SetParagraphStyle(lCommand);
	}
	else if (c_nBlockNumberingLevel == lType)
	{
		NSDocx::CLogicPageDOCX* page = dynamic_cast<NSDocx::CLogicPageDOCX*>(m_oWriter.m_pCurrentPage);
		if (NULL != page)
			page->SetParagraphNumLevel(lCommand);
	}
	else if (c_nBaselineShift == lType)
	{
		NSDocx::CLogicPageDOCX* page = dynamic_cast<NSDocx::CLogicPageDOCX*>(m_oWriter.m_pCurrentPage);
		if (NULL != page)
			page->SetBaselineOffset((double)lCommand);
	}
	else if (c_nBlockHorzAlign == lType)
	{
		NSDocx::CLogicPageDOCX* page = dynamic_cast<NSDocx::CLogicPageDOCX*>(m_oWriter.m_pCurrentPage);
		if (NULL != page)
			page->SetParTextAlign(lCommand);
	}
	
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::CommandDouble2(LONG lType, double dCommand)
{
	if (c_nTableTop == lType)
	{
		NSDocx::CLogicPageDOCX* page = dynamic_cast<NSDocx::CLogicPageDOCX*>(m_oWriter.m_pCurrentPage);
		if (NULL != page)
			page->WriteTableTop(dCommand);
	}

	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::CommandString2(LONG lType, BSTR bsCommand)
{
	if (c_nTableXml == lType)
	{
		NSDocx::CLogicTable* pTable	= new NSDocx::CLogicTable();
		pTable->m_bIsEmbeddedFonts		= m_oWriter.m_bIsEmbeddedFonts;
		pTable->m_lDstFormat			= m_lDstType;
		pTable->m_pFormat				= &m_oWriter.m_oFormat;
		pTable->m_pFontManager			= &m_oWriter.m_pCurrentPage->m_oFontManager;
		pTable->m_pFontsDst				= &m_oWriter.m_pCurrentPage->m_oFontsGen;

		pTable->LoadFromXml((CString)bsCommand);
		m_oWriter.WriteTable(pTable);
	}
	return S_OK;
}
STDMETHODIMP CAVSOfficeDocxRenderer3::CommandVariant2(LONG lType, VARIANT vCommand)
{
	return S_OK;
}