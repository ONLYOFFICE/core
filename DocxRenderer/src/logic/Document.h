#pragma once
#include "Page.h"
#include "../resources/resources.h"

namespace NSDocxRenderer
{
    class CDocument
	{
	public:
        NSFonts::IApplicationFonts* m_pAppFonts;

		NSStructures::CPen				m_oPen;
		NSStructures::CBrush			m_oBrush;
		NSStructures::CFont				m_oFont;
		NSStructures::CShadow			m_oShadow;
		NSStructures::CEdgeText			m_oEdge;

		NSStructures::CFont				m_oInstalledFont;

        NSFonts::IFontManager*                m_pFontManager;
        Aggplus::CGraphicsPathSimpleConverter m_oSimpleGraphicsConverter;

        Aggplus::CMatrix				m_oTransform;

		LONG							m_lCurrentCommandType;

		LONG							m_lClipMode;
		CPage							m_oCurrentPage;

        CImageManager					m_oManager;

		double							m_dWidth;
		double							m_dHeight;

		double							m_dDpiX;
		double							m_dDpiY;

        std::wstring					m_strTempDirectory;
        std::wstring                    m_strDstFilePath;

        NSFile::CFileBinary				m_oDocumentStream;
		LONG							m_lPagesCount;

        NSStringUtils::CStringBuilder	m_oWriter;
        bool							m_bIsNeedPDFTextAnalyzer;

        bool                            m_bIsDisablePageCommand; // disable commands inside draw function

	public:
        CDocument(IRenderer* pRenderer, NSFonts::IApplicationFonts* pFonts) : m_oWriter(), m_oCurrentPage(pFonts)
		{
            m_pAppFonts = pFonts;
            m_oSimpleGraphicsConverter.SetRenderer(pRenderer);
			m_lCurrentCommandType		= 0;

			m_dWidth	= 0;
			m_dHeight	= 0;

			m_dDpiX		= 72;
			m_dDpiY		= 72;

            m_strTempDirectory			= L"";
			m_lPagesCount				= 0;

			m_bIsNeedPDFTextAnalyzer	= false;
            m_pFontManager = NULL;

            m_bIsDisablePageCommand = false;
		}
        void Clear()
		{
			m_lClipMode = 0;            
		}

		~CDocument()
		{
            m_lClipMode = 0;
            RELEASEINTERFACE(m_pFontManager);
		}

	public:

        HRESULT NewPage()
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
        HRESULT get_Height(double* dHeight)
		{
			*dHeight = m_dHeight;
			return S_OK;
		}
        HRESULT put_Height(double dHeight)
		{
			m_dHeight					= dHeight;
			m_oCurrentPage.m_dHeight	= dHeight;
			return S_OK;
		}
        HRESULT get_Width(double* dWidth)
		{
			*dWidth = m_dWidth;
			return S_OK;
		}
        HRESULT put_Width(double dWidth)
		{
			m_dWidth					= dWidth;
			m_oCurrentPage.m_dWidth		= dWidth;
			return S_OK;
		}
        HRESULT get_DpiX(double* dDpiX)
		{
			*dDpiX = m_dDpiX;
			return S_OK;
		}
        HRESULT get_DpiY(double* dDpiY)
		{
			*dDpiY = m_dDpiY;
			return S_OK;
		}
		//-------- Функции для задания настроек текста ----------------------------------------------
		// pen --------------------------------------------------------------------------------------
        HRESULT get_PenColor(LONG* lColor)
		{
			*lColor = m_oPen.Color;
			return S_OK;
		}
        HRESULT put_PenColor(LONG lColor)
		{
			m_oPen.Color = lColor;
			return S_OK;
		}
        HRESULT get_PenAlpha(LONG* lAlpha)
		{
			*lAlpha = m_oPen.Alpha;
			return S_OK;
		}
        HRESULT put_PenAlpha(LONG lAlpha)
		{
			m_oPen.Alpha = lAlpha;
			return S_OK;
		}
        HRESULT get_PenSize(double* dSize)
		{
			*dSize = m_oPen.Size;
			return S_OK;
		}
        HRESULT put_PenSize(double dSize)
		{
			m_oPen.Size = dSize;
			return S_OK;
		}
        HRESULT get_PenDashStyle(BYTE* val)
		{
			*val = m_oPen.DashStyle;
			return S_OK;
		}
        HRESULT put_PenDashStyle(BYTE val)
		{
			m_oPen.DashStyle = val;
			return S_OK;
		}
        HRESULT get_PenLineStartCap(BYTE* val)
		{
			*val = m_oPen.LineStartCap;
			return S_OK;
		}
        HRESULT put_PenLineStartCap(BYTE val)
		{
			m_oPen.LineStartCap = val;
			return S_OK;
		}
        HRESULT get_PenLineEndCap(BYTE* val)
		{
			*val = m_oPen.LineEndCap;
			return S_OK;
		}
        HRESULT put_PenLineEndCap(BYTE val)
		{
			m_oPen.LineEndCap = val;
			return S_OK;
		}
        HRESULT get_PenLineJoin(BYTE* val)
		{
			*val = m_oPen.LineJoin;
			return S_OK;
		}
        HRESULT put_PenLineJoin(BYTE val)
		{
			m_oPen.LineJoin = val;
			return S_OK;
		}
        HRESULT get_PenDashOffset(double* val)
		{
			*val = m_oPen.DashOffset;
			return S_OK;
		}
        HRESULT put_PenDashOffset(double val)
		{
			m_oPen.DashOffset = val;
			return S_OK;
		}
        HRESULT get_PenAlign(LONG* val)
		{
			*val = m_oPen.Align;
			return S_OK;
		}
        HRESULT put_PenAlign(LONG val)
		{
			m_oPen.Align = val;
			return S_OK;
		}
        HRESULT get_PenMiterLimit(double* val)
		{
			*val = m_oPen.MiterLimit;
			return S_OK;
		}
        HRESULT put_PenMiterLimit(double val)
		{
			m_oPen.MiterLimit = val;
			return S_OK;
		}
        HRESULT PenDashPattern(double* pPattern, LONG lCount)
		{
			if (NULL != pPattern)
			{
                m_oPen.SetDashPattern(pPattern, lCount);
			}

			return S_OK;
		}
		// brush ------------------------------------------------------------------------------------
        HRESULT get_BrushType(LONG* lType)
		{
			*lType = m_oBrush.Type;
			return S_OK;
		}
        HRESULT put_BrushType(LONG lType)
		{
			m_oBrush.Type = lType;
			return S_OK;
		}
        HRESULT get_BrushColor1(LONG* lColor)
		{
			*lColor = m_oBrush.Color1;
			return S_OK;
		}
        HRESULT put_BrushColor1(LONG lColor)
		{
			m_oBrush.Color1 = lColor;
			return S_OK;
		}
        HRESULT get_BrushAlpha1(LONG* lAlpha)
		{
			*lAlpha = m_oBrush.Alpha1;
			return S_OK;
		}
        HRESULT put_BrushAlpha1(LONG lAlpha)
		{
			m_oBrush.Alpha1 = lAlpha;
			return S_OK;
		}
        HRESULT get_BrushColor2(LONG* lColor)
		{
			*lColor = m_oBrush.Color2;
			return S_OK;
		}
        HRESULT put_BrushColor2(LONG lColor)
		{
			m_oBrush.Color2 = lColor;
			return S_OK;
		}
        HRESULT get_BrushAlpha2(LONG* lAlpha)
		{
			*lAlpha = m_oBrush.Alpha2;
			return S_OK;
		}
        HRESULT put_BrushAlpha2(LONG lAlpha)
		{
			m_oBrush.Alpha2 = lAlpha;
			return S_OK;
		}
        HRESULT get_BrushTexturePath(std::wstring* sPath)
		{
            *sPath = m_oBrush.TexturePath;
			return S_OK;
		}
        HRESULT put_BrushTexturePath(const std::wstring& sPath)
		{
            m_oBrush.TexturePath = sPath;
			return S_OK;
		}
        HRESULT get_BrushTextureMode(LONG* lMode)
		{
			*lMode = m_oBrush.TextureMode;
			return S_OK;
		}
        HRESULT put_BrushTextureMode(LONG lMode)
		{
			m_oBrush.TextureMode = lMode;
			return S_OK;
		}
        HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)
		{
			*lTxAlpha = m_oBrush.TextureAlpha;
			return S_OK;
		}
        HRESULT put_BrushTextureAlpha(LONG lTxAlpha)
		{
			m_oBrush.TextureAlpha = lTxAlpha;
			return S_OK;
		}
        HRESULT get_BrushLinearAngle(double* dAngle)
		{
			*dAngle = m_oBrush.LinearAngle;
			return S_OK;
		}
        HRESULT put_BrushLinearAngle(double dAngle)
		{
			m_oBrush.LinearAngle = dAngle;
			return S_OK;
		}
        HRESULT BrushRect(bool val, double left, double top, double width, double height)
		{
            m_oBrush.Rectable = val ? 1 : 0;
			m_oBrush.Rect.X = (float)left;
			m_oBrush.Rect.Y = (float)top;
			m_oBrush.Rect.Width  = (float)width;
			m_oBrush.Rect.Height = (float)height;	

			return S_OK;
		}
		// font -------------------------------------------------------------------------------------
        HRESULT get_FontName(std::wstring* sName)
		{
            *sName = m_oFont.Name;
			return S_OK;
		}
        HRESULT put_FontName(std::wstring sName)
		{
            m_oFont.Name = sName;
			return S_OK;
		}
        HRESULT get_FontPath(std::wstring* sPath)
		{
            *sPath = m_oFont.Path;
			return S_OK;
		}
        HRESULT put_FontPath(std::wstring sPath)
		{
            m_oFont.Path = sPath;
			return S_OK;
		}
        HRESULT get_FontSize(double* dSize)
		{
			*dSize = m_oFont.Size;
			return S_OK;
		}
        HRESULT put_FontSize(double dSize)
		{
			m_oFont.Size = dSize;
			return S_OK;
		}
        HRESULT get_FontStyle(LONG* lStyle)
		{
			*lStyle = m_oFont.GetStyle();
			return S_OK;
		}
        HRESULT put_FontStyle(LONG lStyle)
		{
			m_oFont.SetStyle(lStyle);
			return S_OK;
		}
        HRESULT get_FontStringGID(INT* bGID)
		{
			*bGID = m_oFont.StringGID;
			return S_OK;
		}
        HRESULT put_FontStringGID(INT bGID)
		{
			m_oFont.StringGID = bGID;
			return S_OK;
		}
        HRESULT get_FontCharSpace(double* dSpace)
		{
			*dSpace = m_oFont.CharSpace;
			return S_OK;
		}
        HRESULT put_FontCharSpace(double dSpace)
		{
			m_oFont.CharSpace = dSpace;
			return S_OK;
		}
        HRESULT get_FontFaceIndex(int* lFaceIndex)
        {
            *lFaceIndex = m_oFont.FaceIndex;
            return S_OK;
        }
        HRESULT put_FontFaceIndex(const int& lFaceIndex)
        {
            m_oFont.FaceIndex = lFaceIndex;
            return S_OK;
        }
		// shadow -----------------------------------------------------------------------------------
        HRESULT get_ShadowDistanceX(double* val)
		{
			*val = m_oShadow.DistanceX;
			return S_OK;
		}
        HRESULT put_ShadowDistanceX(double val)
		{
			m_oShadow.DistanceX = val;
			return S_OK;
		}
        HRESULT get_ShadowDistanceY(double* val)
		{
			*val = m_oShadow.DistanceY;
			return S_OK;
		}
        HRESULT put_ShadowDistanceY(double val)
		{
			m_oShadow.DistanceY = val;
			return S_OK;
		}
        HRESULT get_ShadowBlurSize(double* val)
		{
			*val = m_oShadow.BlurSize;
			return S_OK;
		}
        HRESULT put_ShadowBlurSize(double val)
		{
			m_oShadow.BlurSize = val;
			return S_OK;
		}
        HRESULT get_ShadowColor(LONG* val)
		{
			*val = m_oShadow.Color;
			return S_OK;
		}
        HRESULT put_ShadowColor(LONG val)
		{
			m_oShadow.Color = val;
			return S_OK;
		}
        HRESULT get_ShadowAlpha(LONG* val)
		{
			*val = m_oShadow.Alpha;
			return S_OK;
		}
        HRESULT put_ShadowAlpha(LONG val)
		{
			m_oShadow.Alpha = val;
			return S_OK;
		}
        HRESULT get_ShadowVisible(INT* val)
		{
			*val = m_oShadow.Visible;
			return S_OK;
		}
        HRESULT put_ShadowVisible(INT val)
		{
			m_oShadow.Visible = val;
			return S_OK;
		}
		// edge -------------------------------------------------------------------------------------
        HRESULT get_EdgeVisible(LONG* val)
		{
			*val = m_oEdge.Visible;
			return S_OK;
		}
        HRESULT put_EdgeVisible(LONG val)
		{
			m_oEdge.Visible = val;
			return S_OK;
		}
        HRESULT get_EdgeColor(LONG* val)
		{
			*val = m_oEdge.Color;
			return S_OK;
		}
        HRESULT put_EdgeColor(LONG val)
		{
			m_oEdge.Color = val;
			return S_OK;
		}
        HRESULT get_EdgeAlpha(LONG* val)
		{
			*val = m_oEdge.Alpha;
			return S_OK;
		}
        HRESULT put_EdgeAlpha(LONG val)
		{
			m_oEdge.Alpha = val;
			return S_OK;
		}
        HRESULT get_EdgeDist(double* val)
		{
			*val = m_oEdge.Dist;
			return S_OK;
		}
        HRESULT put_EdgeDist(double val)
		{
			m_oEdge.Dist = val;
			return S_OK;
		}

		//-------- Функции для вывода текста --------------------------------------------------------
        HRESULT CommandDrawTextPrivate(const int* pUnicodes, const int* pGids, int nCount, const double& dX, const double& dY, const double& dW, const double& dH, const double& dBaseLineOffset = 0)
        {
            double dAngleMatrix = m_oTransform.z_Rotation();
            if (abs(dAngleMatrix) > 1)
            {
                _SetFont();
                PathCommandEnd();
                BeginCommand(c_nPathType);
                m_oSimpleGraphicsConverter.PathCommandText2(pUnicodes, pGids, nCount, m_pFontManager, dX, dY, dW, dH);
                DrawPath(c_nWindingFillMode);
                EndCommand(c_nPathType);
                PathCommandEnd();
                return S_OK;
            }

            m_oCurrentPage.WriteText((unsigned int*)pUnicodes, (unsigned int*)pGids, nCount, dX, dY, dW, dH, 0, m_bIsNeedPDFTextAnalyzer);
            return S_OK;
        }

        HRESULT CommandDrawTextCHAR(const int& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
        {
            return CommandDrawTextPrivate(&lUnicode, NULL, 1, dX, dY, dW, dH);
        }
        HRESULT CommandDrawTextExCHAR(const int& lUnicode, const int& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
        {
            return CommandDrawTextPrivate(&lUnicode, &lGid, 1, dX, dY, dW, dH);
        }
        virtual HRESULT CommandDrawText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
        {
            unsigned int nLen = 0;
            unsigned int* pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, nLen);
            if (nLen == 0)
                return S_OK;
            CommandDrawTextPrivate((int*)pUnicodes, NULL, nLen, dX, dY, dW, dH);
            delete [] pUnicodes;
            return S_OK;
        }
        virtual HRESULT CommandDrawTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
        {
            unsigned int nLen = 0;
            unsigned int* pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsUnicodeText, nLen);
            if (nLen == 0)
                return S_OK;
            if (nLen != nGidsCount)
            {
                delete [] pUnicodes;
                return S_OK;
            }

            CommandDrawTextPrivate((int*)pUnicodes, (int*)pGids, (int)nLen, dX, dY, dW, dH);
            delete [] pUnicodes;
            return S_OK;
        }
		//-------- Маркеры для команд ---------------------------------------------------------------
        HRESULT BeginCommand(DWORD lType)
		{
            if (c_nPageType == lType && m_bIsDisablePageCommand)
                return S_OK;

			m_lCurrentCommandType = (LONG)lType;
			m_oCurrentPage.m_lCurrentCommand	= m_lCurrentCommandType;

			return S_OK;
		}
        HRESULT EndCommand(DWORD lType)
		{
            if (c_nPageType == lType && m_bIsDisablePageCommand)
                return S_OK;

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
        HRESULT PathCommandMoveTo(double fX, double fY)
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
        HRESULT PathCommandLineTo(double fX, double fY)
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
        HRESULT PathCommandLinesTo(double* pPoints, LONG lCount)
		{
            m_oSimpleGraphicsConverter.PathCommandLinesTo(pPoints, lCount);
			return S_OK;
		}
        HRESULT PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
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
        HRESULT PathCommandCurvesTo(double* pPoints, LONG lCount)
		{
            m_oSimpleGraphicsConverter.PathCommandCurvesTo(pPoints, lCount);
			return S_OK;
		}
        HRESULT PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
		{
            m_oSimpleGraphicsConverter.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
			return S_OK;
		}
        HRESULT PathCommandClose()
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
        HRESULT PathCommandEnd()
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
        HRESULT DrawPath(long nType)
		{
			LONG lTxId = -1;
			if ((nType > 0xFF) && (c_BrushTypeTexture == m_oBrush.Type))
			{
				double x = 0;
				double y = 0;
				double w = 0;
				double h = 0;
				CImageInfo oInfo = m_oManager.WriteImage(m_oBrush.TexturePath, x, y, w, h);
                lTxId = oInfo.m_nId;
			}

			m_oCurrentPage.DrawPath(nType, lTxId);
			return S_OK;
		}
        HRESULT PathCommandStart()
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
        HRESULT PathCommandGetCurrentPoint(double* fX, double* fY)
		{
            m_oSimpleGraphicsConverter.PathCommandGetCurrentPoint(fX, fY);
			return S_OK;
		}

        HRESULT PathCommandTextCHAR(const int& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
        {
            _SetFont();
            m_oSimpleGraphicsConverter.PathCommandText2(&lUnicode, NULL, 1, m_pFontManager, dX, dY, dW, dH);
            return S_OK;
        }
        HRESULT PathCommandTextExCHAR(const int& lUnicode, const int& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
        {
            _SetFont();
            m_oSimpleGraphicsConverter.PathCommandText2(&lUnicode, &lGid, 1, m_pFontManager, dX, dY, dW, dH);
            return S_OK;
        }
        HRESULT PathCommandText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
        {
            _SetFont();
            m_oSimpleGraphicsConverter.PathCommandText(wsUnicodeText, m_pFontManager, dX, dY, dW, dH, 0);
            return S_OK;
        }
        HRESULT PathCommandTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
        {
            _SetFont();
            m_oSimpleGraphicsConverter.PathCommandText2(wsUnicodeText, (const int*)pGids, nGidsCount, m_pFontManager, dX, dY, dW, dH);
            return S_OK;
        }

        HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
		{
			return S_OK;
		}
        HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
		{
			ApplyTransform2(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
			return S_OK;
		}
		//-------- Функции для вывода изображений --------------------------------------------------
        HRESULT DrawImage(IGrObject* pImage, double fX, double fY, double fWidth, double fHeight)
		{
            CImageInfo oInfo = m_oManager.WriteImage((Aggplus::CImage*)pImage, fX, fY, fWidth, fHeight);
			m_oCurrentPage.WriteImage(oInfo, fX, fY, fWidth, fHeight);
			return S_OK;
		}
        HRESULT DrawImageFromFile(const std::wstring& sVal, double fX, double fY, double fWidth, double fHeight)
		{
            CImageInfo oInfo = m_oManager.WriteImage(sVal, fX, fY, fWidth, fHeight);
			m_oCurrentPage.WriteImage(oInfo, fX, fY, fWidth, fHeight);
			return S_OK;
		}
		//------------------------------------------------------------------------------------------
        HRESULT SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
		{
			ApplyTransform(dA, dB, dC, dD, dE, dF);
			return S_OK;
		}
        HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
		{
			return S_OK;
		}
        HRESULT ResetTransform(void)
		{
			m_oTransform.Reset();
			return S_OK;
		}
        HRESULT get_ClipMode(LONG* plMode)
		{
			*plMode = m_lClipMode;
			return S_OK;
		}
        HRESULT put_ClipMode(LONG lMode)
		{
			m_lClipMode = lMode;
			return S_OK;
		}

	protected:
        void ApplyTransform(double d1, double d2, double d3, double d4, double d5, double d6)
		{
			m_oTransform.SetElements(d1, d2, d3, d4, d5, d6);
		}

		void ApplyTransform2(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
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

		void _SetFont()
		{
			if (NULL == m_pFontManager)
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

	public:
		
        bool CreateDocument()
		{
            CreateTemplate(m_strTempDirectory);

			// Init
			Clear();

			m_lCurrentCommandType = 0;
            m_oCurrentPage.Init(&m_oFont, &m_oPen, &m_oBrush, &m_oShadow, &m_oEdge, &m_oTransform, &m_oSimpleGraphicsConverter);

			m_oManager.NewDocument();
			// media
            m_oManager.m_strDstMedia = m_strTempDirectory + L"/word/media";
            NSDirectory::CreateDirectory(m_oManager.m_strDstMedia);

            m_oCurrentPage.m_oManager.m_oFontTable.m_mapTable.clear();

			m_oDocumentStream.CloseFile();
            m_oDocumentStream.CreateFileW(m_strTempDirectory + L"/word/document.xml");
            m_oDocumentStream.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<w:document xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\">\
<w:body>");

            m_lPagesCount = 0;
			m_oWriter.Clear();
			m_oWriter.AddSize(10000);

			return true;
		}

		void Close()
		{
			// сохраним rels (images & docs)
            NSStringUtils::CStringBuilder oWriter;

            oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\
<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>\
<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>\
<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>\
<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>\
<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme.xml\"/>");

            for (std::map<DWORD, CImageInfo>::iterator iterImage = m_oManager.m_mapImageData.begin(); iterImage != m_oManager.m_mapImageData.end(); iterImage++)
            {
                CImageInfo& oInfo = iterImage->second;

                oWriter.WriteString(L"<Relationship Id=\"rId");
                oWriter.AddInt(10 + oInfo.m_nId);
                oWriter.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/image");
                oWriter.AddInt(oInfo.m_nId);
                (oInfo.m_eType == CImageInfo::itPNG) ? oWriter.WriteString(L".png\"/>") : oWriter.WriteString(L".jpg\"/>");
            }

            for (std::map<std::wstring, CImageInfo>::iterator iterImage = m_oManager.m_mapImagesFile.begin(); iterImage != m_oManager.m_mapImagesFile.end(); iterImage++)
            {
                CImageInfo& oInfo = iterImage->second;

                oWriter.WriteString(L"<Relationship Id=\"rId");
                oWriter.AddInt(10 + oInfo.m_nId);
                oWriter.WriteString(L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/image");
                oWriter.AddInt(oInfo.m_nId);
                (oInfo.m_eType == CImageInfo::itPNG) ? oWriter.WriteString(L".png\"/>") : oWriter.WriteString(L".jpg\"/>");
            }

            oWriter.WriteString(L"</Relationships>");

            NSFile::CFileBinary::SaveToFile(m_strTempDirectory + L"/word/_rels/document.xml.rels", oWriter.GetData());
            oWriter.ClearNoAttack();

			// сохраним fontTable			
            oWriter.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\
<w:fonts xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">");

            CFontTable* pFontTable = &m_oCurrentPage.m_oManager.m_oFontTable;
            for (std::map<std::wstring, CFontTableEntry>::iterator iterFont = pFontTable->m_mapTable.begin(); iterFont != pFontTable->m_mapTable.end(); iterFont++)
            {
                CFontTableEntry& oEntry = iterFont->second;

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

			// document
			m_oCurrentPage.WriteSectionToFile(true, m_oWriter);
            m_oWriter.WriteString(L"</w:body></w:document>");
			m_oDocumentStream.WriteStringUTF8(m_oWriter.GetData());
			m_oWriter.ClearNoAttack();

			m_oDocumentStream.CloseFile();
		}
	};
}
