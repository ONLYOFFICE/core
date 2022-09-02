#include "Document.h"

namespace NSDocxRenderer
{
    CDocument::CDocument(IRenderer* pRenderer, NSFonts::IApplicationFonts* pFonts) :
        m_pAppFonts(pFonts), m_oCurrentPage(pFonts)
    {
        m_oSimpleGraphicsConverter.SetRenderer(pRenderer);
        m_oWriter.AddSize(10000);
    }
    void CDocument::Clear()
    {
        m_oPen.SetDefaultParams();
        m_oBrush.SetDefaultParams();
        m_oFont.SetDefaultParams();
        m_oShadow.SetDefaultParams();
        m_oEdge.SetDefaultParams();

        m_oTransform.Reset();

        m_lClipMode = 0;

        m_lPagesCount = 0;
        m_oWriter.Clear();
   }

    CDocument::~CDocument() {
        m_lClipMode = 0;
        RELEASEINTERFACE(m_pFontManager);
    }

    HRESULT CDocument::NewPage()
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
    HRESULT CDocument::get_Height(double* dHeight)
    {
        *dHeight = m_dHeight;
        return S_OK;
    }
    HRESULT CDocument::put_Height(double dHeight)
    {
        m_dHeight					= dHeight;
        m_oCurrentPage.m_dHeight	= dHeight;
        return S_OK;
    }
    HRESULT CDocument::get_Width(double* dWidth)
    {
        *dWidth = m_dWidth;
        return S_OK;
    }
    HRESULT CDocument::put_Width(double dWidth)
    {
        m_dWidth					= dWidth;
        m_oCurrentPage.m_dWidth		= dWidth;
        return S_OK;
    }
    HRESULT CDocument::get_DpiX(double* dDpiX)
    {
        *dDpiX = m_dDpiX;
        return S_OK;
    }
    HRESULT CDocument::get_DpiY(double* dDpiY)
    {
        *dDpiY = m_dDpiY;
        return S_OK;
    }
    //-------- Функции для задания настроек текста ----------------------------------------------
    // pen --------------------------------------------------------------------------------------
    HRESULT CDocument::get_PenColor(LONG* lColor)
    {
        *lColor = m_oPen.Color;
        return S_OK;
    }
    HRESULT CDocument::put_PenColor(LONG lColor)
    {
        m_oPen.Color = lColor;
        return S_OK;
    }
    HRESULT CDocument::get_PenAlpha(LONG* lAlpha)
    {
        *lAlpha = m_oPen.Alpha;
        return S_OK;
    }
    HRESULT CDocument::put_PenAlpha(LONG lAlpha)
    {
        m_oPen.Alpha = lAlpha;
        return S_OK;
    }
    HRESULT CDocument::get_PenSize(double* dSize)
    {
        *dSize = m_oPen.Size;
        return S_OK;
    }
    HRESULT CDocument::put_PenSize(double dSize)
    {
        m_oPen.Size = dSize;
        return S_OK;
    }
    HRESULT CDocument::get_PenDashStyle(BYTE* val)
    {
        *val = m_oPen.DashStyle;
        return S_OK;
    }
    HRESULT CDocument::put_PenDashStyle(BYTE val)
    {
        m_oPen.DashStyle = val;
        return S_OK;
    }
    HRESULT CDocument::get_PenLineStartCap(BYTE* val)
    {
        *val = m_oPen.LineStartCap;
        return S_OK;
    }
    HRESULT CDocument::put_PenLineStartCap(BYTE val)
    {
        m_oPen.LineStartCap = val;
        return S_OK;
    }
    HRESULT CDocument::get_PenLineEndCap(BYTE* val)
    {
        *val = m_oPen.LineEndCap;
        return S_OK;
    }
    HRESULT CDocument::put_PenLineEndCap(BYTE val)
    {
        m_oPen.LineEndCap = val;
        return S_OK;
    }
    HRESULT CDocument::get_PenLineJoin(BYTE* val)
    {
        *val = m_oPen.LineJoin;
        return S_OK;
    }
    HRESULT CDocument::put_PenLineJoin(BYTE val)
    {
        m_oPen.LineJoin = val;
        return S_OK;
    }
    HRESULT CDocument::get_PenDashOffset(double* val)
    {
        *val = m_oPen.DashOffset;
        return S_OK;
    }
    HRESULT CDocument::put_PenDashOffset(double val)
    {
        m_oPen.DashOffset = val;
        return S_OK;
    }
    HRESULT CDocument::get_PenAlign(LONG* val)
    {
        *val = m_oPen.Align;
        return S_OK;
    }
    HRESULT CDocument::put_PenAlign(LONG val)
    {
        m_oPen.Align = val;
        return S_OK;
    }
    HRESULT CDocument::get_PenMiterLimit(double* val)
    {
        *val = m_oPen.MiterLimit;
        return S_OK;
    }
    HRESULT CDocument::put_PenMiterLimit(double val)
    {
        m_oPen.MiterLimit = val;
        return S_OK;
    }
    HRESULT CDocument::PenDashPattern(double* pPattern, LONG lCount)
    {
        if (nullptr != pPattern)
        {
            m_oPen.SetDashPattern(pPattern, lCount);
        }

        return S_OK;
    }
    // brush ------------------------------------------------------------------------------------
    HRESULT CDocument::get_BrushType(LONG* lType)
    {
        *lType = m_oBrush.Type;
        return S_OK;
    }
    HRESULT CDocument::put_BrushType(LONG lType)
    {
        m_oBrush.Type = lType;
        return S_OK;
    }
    HRESULT CDocument::get_BrushColor1(LONG* lColor)
    {
        *lColor = m_oBrush.Color1;
        return S_OK;
    }
    HRESULT CDocument::put_BrushColor1(LONG lColor)
    {
        m_oBrush.Color1 = lColor;
        return S_OK;
    }
    HRESULT CDocument::get_BrushAlpha1(LONG* lAlpha)
    {
        *lAlpha = m_oBrush.Alpha1;
        return S_OK;
    }
    HRESULT CDocument::put_BrushAlpha1(LONG lAlpha)
    {
        m_oBrush.Alpha1 = lAlpha;
        return S_OK;
    }
    HRESULT CDocument::get_BrushColor2(LONG* lColor)
    {
        *lColor = m_oBrush.Color2;
        return S_OK;
    }
    HRESULT CDocument::put_BrushColor2(LONG lColor)
    {
        m_oBrush.Color2 = lColor;
        return S_OK;
    }
    HRESULT CDocument::get_BrushAlpha2(LONG* lAlpha)
    {
        *lAlpha = m_oBrush.Alpha2;
        return S_OK;
    }
    HRESULT CDocument::put_BrushAlpha2(LONG lAlpha)
    {
        m_oBrush.Alpha2 = lAlpha;
        return S_OK;
    }
    HRESULT CDocument::get_BrushTexturePath(std::wstring* sPath)
    {
        *sPath = m_oBrush.TexturePath;
        return S_OK;
    }
    HRESULT CDocument::put_BrushTexturePath(const std::wstring& sPath)
    {
        m_oBrush.TexturePath = sPath;
        return S_OK;
    }
    HRESULT CDocument::get_BrushTextureMode(LONG* lMode)
    {
        *lMode = m_oBrush.TextureMode;
        return S_OK;
    }
    HRESULT CDocument::put_BrushTextureMode(LONG lMode)
    {
        m_oBrush.TextureMode = lMode;
        return S_OK;
    }
    HRESULT CDocument::get_BrushTextureAlpha(LONG* lTxAlpha)
    {
        *lTxAlpha = m_oBrush.TextureAlpha;
        return S_OK;
    }
    HRESULT CDocument::put_BrushTextureAlpha(LONG lTxAlpha)
    {
        m_oBrush.TextureAlpha = lTxAlpha;
        return S_OK;
    }
    HRESULT CDocument::get_BrushLinearAngle(double* dAngle)
    {
        *dAngle = m_oBrush.LinearAngle;
        return S_OK;
    }
    HRESULT CDocument::put_BrushLinearAngle(double dAngle)
    {
        m_oBrush.LinearAngle = dAngle;
        return S_OK;
    }
    HRESULT CDocument::BrushRect(bool val, double left, double top, double width, double height)
    {
        m_oBrush.Rectable = val ? 1 : 0;
        m_oBrush.Rect.X = (float)left;
        m_oBrush.Rect.Y = (float)top;
        m_oBrush.Rect.Width  = (float)width;
        m_oBrush.Rect.Height = (float)height;

        return S_OK;
    }
    // font -------------------------------------------------------------------------------------
    HRESULT CDocument::get_FontName(std::wstring* sName)
    {
        *sName = m_oFont.Name;
        return S_OK;
    }
    HRESULT CDocument::put_FontName(std::wstring sName)
    {
        m_oFont.Name = sName;
        return S_OK;
    }
    HRESULT CDocument::get_FontPath(std::wstring* sPath)
    {
        *sPath = m_oFont.Path;
        return S_OK;
    }
    HRESULT CDocument::put_FontPath(std::wstring sPath)
    {
        m_oFont.Path = sPath;
        return S_OK;
    }
    HRESULT CDocument::get_FontSize(double* dSize)
    {
        *dSize = m_oFont.Size;
        return S_OK;
    }
    HRESULT CDocument::put_FontSize(double dSize)
    {
        m_oFont.Size = dSize;
        return S_OK;
    }
    HRESULT CDocument::get_FontStyle(LONG* lStyle)
    {
        *lStyle = m_oFont.GetStyle();
        return S_OK;
    }
    HRESULT CDocument::put_FontStyle(LONG lStyle)
    {
        m_oFont.SetStyle(lStyle);
        return S_OK;
    }
    HRESULT CDocument::get_FontStringGID(INT* bGID)
    {
        *bGID = m_oFont.StringGID;
        return S_OK;
    }
    HRESULT CDocument::put_FontStringGID(INT bGID)
    {
        m_oFont.StringGID = bGID;
        return S_OK;
    }
    HRESULT CDocument::get_FontCharSpace(double* dSpace)
    {
        *dSpace = m_oFont.CharSpace;
        return S_OK;
    }
    HRESULT CDocument::put_FontCharSpace(double dSpace)
    {
        m_oFont.CharSpace = dSpace;
        return S_OK;
    }
    HRESULT CDocument::get_FontFaceIndex(int* lFaceIndex)
    {
        *lFaceIndex = m_oFont.FaceIndex;
        return S_OK;
    }
    HRESULT CDocument::put_FontFaceIndex(const int& lFaceIndex)
    {
        m_oFont.FaceIndex = lFaceIndex;
        return S_OK;
    }
    // shadow -----------------------------------------------------------------------------------
    HRESULT CDocument::get_ShadowDistanceX(double* val)
    {
        *val = m_oShadow.DistanceX;
        return S_OK;
    }
    HRESULT CDocument::put_ShadowDistanceX(double val)
    {
        m_oShadow.DistanceX = val;
        return S_OK;
    }
    HRESULT CDocument::get_ShadowDistanceY(double* val)
    {
        *val = m_oShadow.DistanceY;
        return S_OK;
    }
    HRESULT CDocument::put_ShadowDistanceY(double val)
    {
        m_oShadow.DistanceY = val;
        return S_OK;
    }
    HRESULT CDocument::get_ShadowBlurSize(double* val)
    {
        *val = m_oShadow.BlurSize;
        return S_OK;
    }
    HRESULT CDocument::put_ShadowBlurSize(double val)
    {
        m_oShadow.BlurSize = val;
        return S_OK;
    }
    HRESULT CDocument::get_ShadowColor(LONG* val)
    {
        *val = m_oShadow.Color;
        return S_OK;
    }
    HRESULT CDocument::put_ShadowColor(LONG val)
    {
        m_oShadow.Color = val;
        return S_OK;
    }
    HRESULT CDocument::get_ShadowAlpha(LONG* val)
    {
        *val = m_oShadow.Alpha;
        return S_OK;
    }
    HRESULT CDocument::put_ShadowAlpha(LONG val)
    {
        m_oShadow.Alpha = val;
        return S_OK;
    }
    HRESULT CDocument::get_ShadowVisible(INT* val)
    {
        *val = m_oShadow.Visible;
        return S_OK;
    }
    HRESULT CDocument::put_ShadowVisible(INT val)
    {
        m_oShadow.Visible = val;
        return S_OK;
    }
    // edge -------------------------------------------------------------------------------------
    HRESULT CDocument::get_EdgeVisible(LONG* val)
    {
        *val = m_oEdge.Visible;
        return S_OK;
    }
    HRESULT CDocument::put_EdgeVisible(LONG val)
    {
        m_oEdge.Visible = val;
        return S_OK;
    }
    HRESULT CDocument::get_EdgeColor(LONG* val)
    {
        *val = m_oEdge.Color;
        return S_OK;
    }
    HRESULT CDocument::put_EdgeColor(LONG val)
    {
        m_oEdge.Color = val;
        return S_OK;
    }
    HRESULT CDocument::get_EdgeAlpha(LONG* val)
    {
        *val = m_oEdge.Alpha;
        return S_OK;
    }
    HRESULT CDocument::put_EdgeAlpha(LONG val)
    {
        m_oEdge.Alpha = val;
        return S_OK;
    }
    HRESULT CDocument::get_EdgeDist(double* val)
    {
        *val = m_oEdge.Dist;
        return S_OK;
    }
    HRESULT CDocument::put_EdgeDist(double val)
    {
        m_oEdge.Dist = val;
        return S_OK;
    }

    //-------- Функции для вывода текста --------------------------------------------------------
    HRESULT CDocument::CommandDrawTextPrivate(const int* pUnicodes, const int* pGids, int nCount,
                                              const double& dX, const double& dY, const double& dW,
                                              const double& dH, const double& dBaseLineOffset)
    {
        double dAngleMatrix = m_oTransform.z_Rotation();
        if (fabs(dAngleMatrix) > 1 || m_oTransform.sx() < 0 || m_oTransform.sy() < 0)
        {
            //note У повернутых символов не приходят координаты.
            _SetFont();
            PathCommandEnd();
            BeginCommand(c_nPathType);
            m_oSimpleGraphicsConverter.PathCommandText2(pUnicodes, pGids, nCount, m_pFontManager, dX, dY, dW, dH);
            DrawPath(c_nWindingFillMode);
            EndCommand(c_nPathType);
            PathCommandEnd();
            return S_OK;
        }

        m_oCurrentPage.CollectTextData((unsigned int*)pUnicodes, (unsigned int*)pGids, nCount, dX, dY, dW, dH, 0, m_bIsNeedPDFTextAnalyzer);
        return S_OK;
    }

    HRESULT CDocument::CommandDrawTextCHAR(const int& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
    {
        return CommandDrawTextPrivate(&lUnicode, nullptr, 1, dX, dY, dW, dH);
    }
    HRESULT CDocument::CommandDrawTextExCHAR(const int& lUnicode, const int& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
    {
        return CommandDrawTextPrivate(&lUnicode, &lGid, 1, dX, dY, dW, dH);
    }
    HRESULT CDocument::CommandDrawText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
    {
        unsigned int nLen = 0;
        unsigned int* pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, nLen);
        if (nLen == 0)
            return S_OK;
        CommandDrawTextPrivate((int*)pUnicodes, nullptr, nLen, dX, dY, dW, dH);
        delete [] pUnicodes;
        return S_OK;
    }
    HRESULT CDocument::CommandDrawTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
    {
        unsigned int nLen = 0;
        unsigned int* pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, nLen);
        if (nLen == 0)
            return S_OK;
		if (nLen != nGidsCount && 0 != nGidsCount)
        {
            delete [] pUnicodes;
            return S_OK;
        }

		CommandDrawTextPrivate((int*)pUnicodes, (0 == nGidsCount) ? nullptr : (int*)pGids, (int)nLen, dX, dY, dW, dH);
        delete [] pUnicodes;
        return S_OK;
    }
    //-------- Маркеры для команд ---------------------------------------------------------------
    HRESULT CDocument::BeginCommand(DWORD lType)
    {
        if (c_nPageType == lType && m_bIsDisablePageCommand)
            return S_OK;

        m_lCurrentCommandType = (LONG)lType;
        m_oCurrentPage.m_lCurrentCommand	= m_lCurrentCommandType;

        if (c_nTextType == lType)
            m_oCurrentPage.m_dLastTextX_block = -1;

        return S_OK;
    }
    HRESULT CDocument::EndCommand(DWORD lType)
    {
        if (c_nPageType == lType && m_bIsDisablePageCommand)
            return S_OK;

        m_lCurrentCommandType				= -1;
        m_oCurrentPage.m_lCurrentCommand	= m_lCurrentCommandType;

        if (c_nPageType == lType)
        {
            // нужно записать страницу в файл
            m_oCurrentPage.AnalyzeCollectedShapes();
            m_oCurrentPage.AnalyzeCollectedSymbols();
            m_oCurrentPage.AnalyzeLines();
            m_oCurrentPage.BuildByType();
            m_oCurrentPage.ToXml(m_oWriter);
        }
        else if (c_nPathType == lType)
        {
            m_oCurrentPage.End();
        }

        if (c_nTextType == lType)
            m_oCurrentPage.m_dLastTextX_block = -1;

        return S_OK;
    }
    //-------- Функции для работы с Graphics Path -----------------------------------------------
    HRESULT CDocument::PathCommandMoveTo(double fX, double fY)
    {
        if (c_nSimpleGraphicType == m_lCurrentCommandType)
        {
            m_oCurrentPage.MoveTo(fX, fY);
        }
        else
        {
            m_oSimpleGraphicsConverter.PathCommandMoveTo(fX, fY);
        }
        return S_OK;
    }
    HRESULT CDocument::PathCommandLineTo(double fX, double fY)
    {
        if (c_nSimpleGraphicType == m_lCurrentCommandType)
        {
            m_oCurrentPage.LineTo(fX, fY);
        }
        else
        {
            m_oSimpleGraphicsConverter.PathCommandLineTo(fX, fY);
        }
        return S_OK;
    }
    HRESULT CDocument::PathCommandLinesTo(double* pPoints, LONG lCount)
    {
        m_oSimpleGraphicsConverter.PathCommandLinesTo(pPoints, lCount);
        return S_OK;
    }
    HRESULT CDocument::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
    {
        if (c_nSimpleGraphicType == m_lCurrentCommandType)
        {
            m_oCurrentPage.CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
        }
        else
        {
            m_oSimpleGraphicsConverter.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
        }
        return S_OK;
    }
    HRESULT CDocument::PathCommandCurvesTo(double* pPoints, LONG lCount)
    {
        m_oSimpleGraphicsConverter.PathCommandCurvesTo(pPoints, lCount);
        return S_OK;
    }
    HRESULT CDocument::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
    {
        m_oSimpleGraphicsConverter.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
        return S_OK;
    }
    HRESULT CDocument::PathCommandClose()
    {
        if (c_nSimpleGraphicType == m_lCurrentCommandType)
        {
            m_oCurrentPage.Close();
        }
        else
        {
            m_oSimpleGraphicsConverter.PathCommandClose();
        }
        return S_OK;
    }
    HRESULT CDocument::PathCommandEnd()
    {
        if (c_nSimpleGraphicType == m_lCurrentCommandType)
        {
            m_oCurrentPage.End();
        }
        else
        {
            m_oSimpleGraphicsConverter.PathCommandEnd();
        }
        return S_OK;
    }
    HRESULT CDocument::DrawPath(long nType)
    {
        std::shared_ptr<CImageInfo> pInfo = nullptr;

        if ((nType > 0xFF) && (c_BrushTypeTexture == m_oBrush.Type))
        {
            double x = 0;
            double y = 0;
            double w = 0;
            double h = 0;
            pInfo = m_oImageManager.WriteImage(m_oBrush.TexturePath, x, y, w, h);
        }

        m_oCurrentPage.DrawPath(nType, pInfo);
        return S_OK;
    }
    HRESULT CDocument::PathCommandStart()
    {
        if (c_nSimpleGraphicType == m_lCurrentCommandType)
        {
            m_oCurrentPage.Start();
        }
        else
        {
            m_oSimpleGraphicsConverter.PathCommandStart();
        }
        return S_OK;
    }
    HRESULT CDocument::PathCommandGetCurrentPoint(double* fX, double* fY)
    {
        m_oSimpleGraphicsConverter.PathCommandGetCurrentPoint(fX, fY);
        return S_OK;
    }

    HRESULT CDocument::PathCommandTextCHAR(const int& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
    {
        _SetFont();
        m_oSimpleGraphicsConverter.PathCommandText2(&lUnicode, nullptr, 1, m_pFontManager, dX, dY, dW, dH);
        return S_OK;
    }
    HRESULT CDocument::PathCommandTextExCHAR(const int& lUnicode, const int& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
    {
        _SetFont();
        m_oSimpleGraphicsConverter.PathCommandText2(&lUnicode, &lGid, 1, m_pFontManager, dX, dY, dW, dH);
        return S_OK;
    }
    HRESULT CDocument::PathCommandText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
    {
        _SetFont();
        m_oSimpleGraphicsConverter.PathCommandText(wsUnicodeText, m_pFontManager, dX, dY, dW, dH, 0);
        return S_OK;
    }
    HRESULT CDocument::PathCommandTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
    {
        _SetFont();
        m_oSimpleGraphicsConverter.PathCommandText2(wsUnicodeText, (const int*)pGids, nGidsCount, m_pFontManager, dX, dY, dW, dH);
        return S_OK;
    }

    HRESULT CDocument::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
    {
        return S_OK;
    }
    HRESULT CDocument::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
    {
        ApplyTransform2(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
        return S_OK;
    }
    //-------- Функции для вывода изображений --------------------------------------------------
    HRESULT CDocument::DrawImage(IGrObject* pImage, double fX, double fY, double fWidth, double fHeight)
    {
        m_oCurrentPage.WriteImage(m_oImageManager.WriteImage((Aggplus::CImage*)pImage, fX, fY, fWidth, fHeight), fX, fY, fWidth, fHeight);
        return S_OK;
    }
    HRESULT CDocument::DrawImageFromFile(const std::wstring& sVal, double fX, double fY, double fWidth, double fHeight)
    {
        m_oCurrentPage.WriteImage(m_oImageManager.WriteImage(sVal, fX, fY, fWidth, fHeight), fX, fY, fWidth, fHeight);
        return S_OK;
    }
    //------------------------------------------------------------------------------------------
    HRESULT CDocument::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
    {
        ApplyTransform(dA, dB, dC, dD, dE, dF);
        return S_OK;
    }
    HRESULT CDocument::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
    {
        return S_OK;
    }
    HRESULT CDocument::ResetTransform(void)
    {
        m_oTransform.Reset();
        return S_OK;
    }
    HRESULT CDocument::get_ClipMode(LONG* plMode)
    {
        *plMode = m_lClipMode;
        return S_OK;
    }
    HRESULT CDocument::put_ClipMode(LONG lMode)
    {
        m_lClipMode = lMode;
        return S_OK;
    }

    void CDocument::ApplyTransform(double d1, double d2, double d3, double d4, double d5, double d6)
    {
        m_oTransform.SetElements(d1, d2, d3, d4, d5, d6);
    }

    void CDocument::ApplyTransform2(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
    {
        if ((dWidth <= 1) || (dHeight <= 1))
            lFlags = 0;

        bool bFlipX = (0 != (c_nParamFlipX & lFlags));
        bool bFlipY = (0 != (c_nParamFlipY & lFlags));

        double m11 = bFlipX ? -1.0 : 1.0;
        double m22 = bFlipY ? -1.0 : 1.0;

        Aggplus::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

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

    void CDocument::_SetFont()
    {
        if (nullptr == m_pFontManager)
        {
            m_pFontManager = NSFontManager::CreateFontManager(m_pAppFonts);
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

        if (m_oFont.Path.empty())
        {
            m_pFontManager->LoadFontByName(m_oFont.Name, (float)m_oFont.Size, m_oFont.GetStyle(), m_dDpiX, m_dDpiY);
        }
        else
        {
            m_pFontManager->LoadFontFromFile(m_oFont.Path, m_oFont.FaceIndex, (float)m_oFont.Size, m_dDpiX, m_dDpiY);
        }

        m_oInstalledFont = m_oFont;
    }

    bool CDocument::CreateDocument()
    {
        CreateTemplate(m_strTempDirectory);

        // Init
        Clear();

        m_lCurrentCommandType = 0;
        m_oCurrentPage.Init(&m_oFont, &m_oPen, &m_oBrush, &m_oShadow, &m_oEdge, &m_oTransform, &m_oSimpleGraphicsConverter, &m_oStyleManager);

        m_oImageManager.NewDocument();
        m_oStyleManager.NewDocument();
        // media
        m_oImageManager.m_strDstMedia = m_strTempDirectory + L"/word/media";
        NSDirectory::CreateDirectory(m_oImageManager.m_strDstMedia);

        m_oCurrentPage.m_oFontManager.Init();

        m_oDocumentStream.CloseFile();
        m_oDocumentStream.CreateFileW(m_strTempDirectory + L"/word/document.xml");
        m_oDocumentStream.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
        <w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
                    xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\" \
                    xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\" \
                    xmlns:cx2=\"http://schemas.microsoft.com/office/drawing/2015/10/21/chartex\" \
                    xmlns:cx3=\"http://schemas.microsoft.com/office/drawing/2016/5/9/chartex\" \
                    xmlns:cx4=\"http://schemas.microsoft.com/office/drawing/2016/5/10/chartex\" \
                    xmlns:cx5=\"http://schemas.microsoft.com/office/drawing/2016/5/11/chartex\" \
                    xmlns:cx6=\"http://schemas.microsoft.com/office/drawing/2016/5/12/chartex\" \
                    xmlns:cx7=\"http://schemas.microsoft.com/office/drawing/2016/5/13/chartex\" \
                    xmlns:cx8=\"http://schemas.microsoft.com/office/drawing/2016/5/14/chartex\" \
                    xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
                    xmlns:aink=\"http://schemas.microsoft.com/office/drawing/2016/ink\" \
                    xmlns:am3d=\"http://schemas.microsoft.com/office/drawing/2017/model3d\" \
                    xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
                    xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
                    xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
                    xmlns:v=\"urn:schemas-microsoft-com:vml\" \
                    xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
                    xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" \
                    xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
                    xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
                    xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
                    xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
                    xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
                    xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
                    xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" \
                    xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" \
                    xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" \
                    xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" \
                    xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" \
                    xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
                    xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
                    xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
                    xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
                    mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex w16sdtdh wp14\">\
                <w:body>");

        return true;
    }

    void CDocument::Close()
    {
        BuildDocumentXmlRels();
        BuildFontTableXml();
        BuildStylesXml();

        // document
        m_oCurrentPage.WriteSectionToFile(true, m_oWriter);
        m_oWriter.WriteString(L"</w:body></w:document>");
        m_oDocumentStream.WriteStringUTF8(m_oWriter.GetData());
        m_oWriter.ClearNoAttack();

        m_oDocumentStream.CloseFile();
    }

    void CDocument::BuildDocumentXmlRels()
    {
        // сохраним rels (images & docs)
        NSStringUtils::CStringBuilder oWriter;

        oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
                            <Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
                <Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>\
                <Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>\
                <Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>\
                <Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>\
                <Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme.xml\"/>");

        for (const auto& pImage : m_oImageManager.m_mapImageData)
        {
            auto pInfo = pImage.second;

            oWriter.WriteString(L"<Relationship Id=\"rId");
            oWriter.AddInt(c_iStartingIdForImages + pInfo->m_nId);
            oWriter.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/");
            oWriter.WriteString(pInfo->m_strFileName);
            oWriter.WriteString(L"\"/>");
        }

        for (const auto& pImage : m_oImageManager.m_mapImagesFile)
        {
            auto pInfo = pImage.second;;

            oWriter.WriteString(L"<Relationship Id=\"rId");
            oWriter.AddInt(c_iStartingIdForImages + pInfo->m_nId);
            oWriter.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/");
            oWriter.WriteString(pInfo->m_strFileName);
            oWriter.WriteString(L"\"/>");
        }

        oWriter.WriteString(L"</Relationships>");

        NSFile::CFileBinary::SaveToFile(m_strTempDirectory + L"/word/_rels/document.xml.rels", oWriter.GetData());
        oWriter.ClearNoAttack();
    }

    void CDocument::BuildFontTableXml()
    {
        NSStringUtils::CStringBuilder oWriter;
        // сохраним fontTable
        oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
            <w:fonts xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
                xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
                xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
                xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
                xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
                xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" \
                xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" \
                xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" \
                xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" \
                xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" \
                mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex w16sdtdh\">");

        CFontTable* pFontTable = &m_oCurrentPage.m_oFontManager.m_oFontTable;
        for (std::map<std::wstring, CFontTableEntry>::iterator iterFont = pFontTable->m_mapTable.begin(); iterFont != pFontTable->m_mapTable.end(); iterFont++)
        {
            CFontTableEntry& oEntry = iterFont->second;

            if (oEntry.m_strFamilyName.empty())
            {
                continue;
            }

            oWriter.WriteString(L"<w:font w:name=\"");
            oWriter.WriteEncodeXmlString(oEntry.m_strFamilyName);
            oWriter.WriteString(L"\">");

            oWriter.WriteString(L"<w:panose1 w:val=\"");
            oWriter.WriteString(oEntry.m_strPANOSE);
            oWriter.WriteString(L"\"/>");

            if (oEntry.m_bIsFixedWidth)
                oWriter.WriteString(L"<w:pitch w:val=\"fixed\" />");
            else
                oWriter.WriteString(L"<w:pitch w:val=\"variable\" />");

            oWriter.WriteString(L"<w:charset w:val=\"00\"/>");

            oWriter.WriteString(L"<w:sig w:usb0=\"");
            oWriter.WriteHexInt4(oEntry.m_arSignature[0]);
            oWriter.WriteString(L"\" w:usb1=\"");
            oWriter.WriteHexInt4(oEntry.m_arSignature[1]);
            oWriter.WriteString(L"\" w:usb2=\"");
            oWriter.WriteHexInt4(oEntry.m_arSignature[2]);
            oWriter.WriteString(L"\" w:usb3=\"");
            oWriter.WriteHexInt4(oEntry.m_arSignature[3]);
            oWriter.WriteString(L"\" w:csb0=\"");
            oWriter.WriteHexInt4(oEntry.m_arSignature[4]);
            oWriter.WriteString(L"\" w:csb1=\"");
            oWriter.WriteHexInt4(oEntry.m_arSignature[5]);
            oWriter.WriteString(L"\"/>");

            oWriter.WriteString(L"</w:font>");
        }

        oWriter.WriteString(L"</w:fonts>");
        NSFile::CFileBinary::SaveToFile(m_strTempDirectory + L"/word/fontTable.xml", oWriter.GetData());
    }

    void CDocument::BuildStylesXml()
    {
        NSStringUtils::CStringBuilder oWriter;

        // сохраним styles
        oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
            <w:styles xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
             xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
             xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
             xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
             xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
             xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\" \
             xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\" \
             xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\" \
             xmlns:w16sdtdh=\"http://schemas.microsoft.com/office/word/2020/wordml/sdtdatahash\" \
             xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\" \
             mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex w16sdtdh\">");

        oWriter.WriteString(L"<w:docDefaults>");
        oWriter.WriteString(L"<w:rPrDefault><w:rPr>");
        oWriter.WriteString(L"<w:lang w:val=\"en-US\" w:eastAsia=\"en-US\" w:bidi=\"ar-SA\"/><w:rFonts w:eastAsiaTheme=\"minorHAnsi\" w:ascii=\"Times New Roman\" w:hAnsi=\"Times New Roman\" w:cs=\"Times New Roman\"/>");
        oWriter.WriteString(L"</w:rPr></w:rPrDefault>");
        oWriter.WriteString(L"<w:pPrDefault><w:pPr/></w:pPrDefault>");
        oWriter.WriteString(L"</w:docDefaults>");

        oWriter.WriteString(L"<w:latentStyles w:defLockedState=\"false\" w:defUIPriority=\"99\" w:defSemiHidden=\"true\" w:defUnhideWhenUsed=\"true\" w:defQFormat=\"false\" w:count=\"267\">");
        oWriter.WriteString(L"<w:lsdException w:name=\"Normal\" w:semiHidden=\"false\" w:uiPriority=\"0\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"heading 1\" w:semiHidden=\"false\" w:uiPriority=\"9\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"heading 2\" w:uiPriority=\"9\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"heading 3\" w:uiPriority=\"9\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"heading 4\" w:uiPriority=\"9\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"heading 5\" w:uiPriority=\"9\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"heading 6\" w:uiPriority=\"9\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"heading 7\" w:uiPriority=\"9\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"heading 8\" w:uiPriority=\"9\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"heading 9\" w:uiPriority=\"9\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"toc 1\" w:uiPriority=\"39\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"toc 2\" w:uiPriority=\"39\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"toc 3\" w:uiPriority=\"39\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"toc 4\" w:uiPriority=\"39\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"toc 5\" w:uiPriority=\"39\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"toc 6\" w:uiPriority=\"39\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"toc 7\" w:uiPriority=\"39\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"toc 8\" w:uiPriority=\"39\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"toc 9\" w:uiPriority=\"39\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"caption\" w:uiPriority=\"35\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Title\" w:semiHidden=\"false\" w:uiPriority=\"10\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Default Paragraph Font\" w:uiPriority=\"1\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Subtitle\" w:semiHidden=\"false\" w:uiPriority=\"11\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Strong\" w:semiHidden=\"false\" w:uiPriority=\"22\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Emphasis\" w:semiHidden=\"false\" w:uiPriority=\"20\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Table Grid\" w:semiHidden=\"false\" w:uiPriority=\"59\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Placeholder Text\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"No Spacing\" w:semiHidden=\"false\" w:uiPriority=\"1\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Shading\" w:semiHidden=\"false\" w:uiPriority=\"60\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light List\" w:semiHidden=\"false\" w:uiPriority=\"61\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Grid\" w:semiHidden=\"false\" w:uiPriority=\"62\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 1\" w:semiHidden=\"false\" w:uiPriority=\"63\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 2\" w:semiHidden=\"false\" w:uiPriority=\"64\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 1\" w:semiHidden=\"false\" w:uiPriority=\"65\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 2\" w:semiHidden=\"false\" w:uiPriority=\"66\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 1\" w:semiHidden=\"false\" w:uiPriority=\"67\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 2\" w:semiHidden=\"false\" w:uiPriority=\"68\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 3\" w:semiHidden=\"false\" w:uiPriority=\"69\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Dark List\" w:semiHidden=\"false\" w:uiPriority=\"70\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Shading\" w:semiHidden=\"false\" w:uiPriority=\"71\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful List\" w:semiHidden=\"false\" w:uiPriority=\"72\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Grid\" w:semiHidden=\"false\" w:uiPriority=\"73\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Shading Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"60\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light List Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"61\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Grid Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"62\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 1 Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"63\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 2 Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"64\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 1 Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"65\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Revision\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"List Paragraph\" w:semiHidden=\"false\" w:uiPriority=\"34\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Quote\" w:semiHidden=\"false\" w:uiPriority=\"29\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Intense Quote\" w:semiHidden=\"false\" w:uiPriority=\"30\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 2 Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"66\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 1 Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"67\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 2 Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"68\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 3 Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"69\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Dark List Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"70\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Shading Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"71\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful List Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"72\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Grid Accent 1\" w:semiHidden=\"false\" w:uiPriority=\"73\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Shading Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"60\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light List Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"61\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Grid Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"62\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 1 Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"63\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 2 Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"64\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 1 Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"65\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 2 Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"66\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 1 Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"67\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 2 Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"68\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 3 Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"69\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Dark List Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"70\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Shading Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"71\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful List Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"72\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Grid Accent 2\" w:semiHidden=\"false\" w:uiPriority=\"73\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Shading Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"60\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light List Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"61\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Grid Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"62\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 1 Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"63\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 2 Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"64\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 1 Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"65\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 2 Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"66\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 1 Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"67\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 2 Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"68\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 3 Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"69\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Dark List Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"70\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Shading Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"71\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful List Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"72\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Grid Accent 3\" w:semiHidden=\"false\" w:uiPriority=\"73\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Shading Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"60\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light List Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"61\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Grid Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"62\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 1 Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"63\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 2 Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"64\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 1 Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"65\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 2 Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"66\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 1 Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"67\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 2 Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"68\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 3 Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"69\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Dark List Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"70\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Shading Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"71\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful List Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"72\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Grid Accent 4\" w:semiHidden=\"false\" w:uiPriority=\"73\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Shading Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"60\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light List Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"61\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Grid Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"62\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 1 Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"63\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 2 Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"64\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 1 Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"65\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 2 Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"66\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 1 Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"67\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 2 Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"68\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 3 Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"69\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Dark List Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"70\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Shading Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"71\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful List Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"72\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Grid Accent 5\" w:semiHidden=\"false\" w:uiPriority=\"73\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Shading Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"60\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light List Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"61\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Light Grid Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"62\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 1 Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"63\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Shading 2 Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"64\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 1 Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"65\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium List 2 Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"66\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 1 Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"67\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 2 Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"68\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Medium Grid 3 Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"69\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Dark List Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"70\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Shading Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"71\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful List Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"72\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Colorful Grid Accent 6\" w:semiHidden=\"false\" w:uiPriority=\"73\" w:unhideWhenUsed=\"false\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Subtle Emphasis\" w:semiHidden=\"false\" w:uiPriority=\"19\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Intense Emphasis\" w:semiHidden=\"false\" w:uiPriority=\"21\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Subtle Reference\" w:semiHidden=\"false\" w:uiPriority=\"31\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Intense Reference\" w:semiHidden=\"false\" w:uiPriority=\"32\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Book Title\" w:semiHidden=\"false\" w:uiPriority=\"33\" w:unhideWhenUsed=\"false\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"Bibliography\" w:uiPriority=\"37\"/>");
        oWriter.WriteString(L"<w:lsdException w:name=\"TOC Heading\" w:uiPriority=\"39\" w:qFormat=\"true\"/>");
        oWriter.WriteString(L"</w:latentStyles>");

        oWriter.WriteString(L"<w:style w:type=\"paragraph\" w:default=\"1\" w:styleId=\"Normal\"><w:name w:val=\"Normal\"/>");
        oWriter.WriteString(L"</w:style>");

        oWriter.WriteString(L"<w:style w:type=\"character\" w:default=\"1\" w:styleId=\"DefaultParagraphFont\">");
        oWriter.WriteString(L"<w:name w:val=\"Default Paragraph Font\"/>");
        oWriter.WriteString(L"<w:uiPriority w:val=\"1\"/>");
        oWriter.WriteString(L"<w:semiHidden/>");
        oWriter.WriteString(L"<w:unhideWhenUsed/>");
        oWriter.WriteString(L"<w:noProof/>"); //отключение проверки орфографии
        oWriter.WriteString(L"<w:b w:val=\"0\"/>");
        oWriter.WriteString(L"<w:bCs w:val=\"0\"/>");
        oWriter.WriteString(L"<w:i w:val=\"0\"/>");
        oWriter.WriteString(L"<w:iCs w:val=\"0\"/>");
        oWriter.WriteString(L"<w:color w:val=\"111111\"/>");
        oWriter.WriteString(L"</w:style>");

        oWriter.WriteString(L"<w:style w:type=\"table\" w:default=\"1\" w:styleId=\"TableNormal\">");
        oWriter.WriteString(L"<w:name w:val=\"Normal Table\"/>");
        oWriter.WriteString(L"<w:semiHidden/>");
        oWriter.WriteString(L"<w:unhideWhenUsed/>");
        oWriter.WriteString(L"<w:qFormat/>");
        oWriter.WriteString(L"<w:tblPr>");
        oWriter.WriteString(L"<w:tblInd w:w=\"0\" w:type=\"dxa\"/>");
        oWriter.WriteString(L"<w:tblCellMar>");
        oWriter.WriteString(L"<w:top w:w=\"0\" w:type=\"dxa\"/>");
        oWriter.WriteString(L"<w:left w:w=\"108\" w:type=\"dxa\"/>");
        oWriter.WriteString(L"<w:bottom w:w=\"0\" w:type=\"dxa\"/>");
        oWriter.WriteString(L"<w:right w:w=\"108\" w:type=\"dxa\"/>");
        oWriter.WriteString(L"</w:tblCellMar>");
        oWriter.WriteString(L"</w:tblPr>");
        oWriter.WriteString(L"</w:style>");

        oWriter.WriteString(L"<w:style w:type=\"numbering\" w:default=\"1\" w:styleId=\"NoList\">");
        oWriter.WriteString(L"<w:name w:val=\"No List\"/>");
        oWriter.WriteString(L"<w:semiHidden/>");
        oWriter.WriteString(L"<w:unhideWhenUsed/>");
        oWriter.WriteString(L"<w:uiPriority w:val=\"99\"/>");
        oWriter.WriteString(L"</w:style>");

        for (const auto &pStyle : m_oStyleManager.m_arStyles)
        {
            pStyle->ToXml(oWriter);
        }

        oWriter.WriteString(L"</w:styles>");

        NSFile::CFileBinary::SaveToFile(m_strTempDirectory + L"/word/styles.xml", oWriter.GetData());
    }
}
