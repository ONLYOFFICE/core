/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _ASC_HTMLRENDERER_WRITER_H_
#define _ASC_HTMLRENDERER_WRITER_H_

#include "SVGWriter2.h"
#include <map>
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../DesktopEditor/common/CalculatorCRC32.h"
#include "Text.h"
#include "Document.h"
#include "../../DesktopEditor/fontengine/FontConverter.h"

namespace NSHtmlRenderer
{
    static double	__g_matrix_eps	= 0.0001;

    class CTileInfo
    {
        /*
        _T("<htmltiling x=\"%.2lf\" y=\"%.2lf\" countx=\"%.2lf\" county=\"%.2lf\" stepx=\"%.2lf\" stepy=\"%.2lf\">\
    <bbox x=\".2lf\" y=\"%.2lf\" r=\"%.2lf\" b=\"%.2lf\" />\
    <transform m1=\"%.2lf\" m2=\"%.2lf\" m3=\"%.2lf\" m4=\"%.2lf\" m5=\"%.2lf\" m6=\"%.2lf\" />\
    </htmltiling>
        */

    public:
        double x;
        double y;
        double countx;
        double county;
        double stepx;
        double stepy;

        double bbox_x;
        double bbox_y;
        double bbox_r;
        double bbox_b;

        double transform_1;
        double transform_2;
        double transform_3;
        double transform_4;
        double transform_5;
        double transform_6;

    public:
        CTileInfo()
        {
            Clear();
        }

    public:

        void Clear()
        {
            x = 0;
            y = 0;
            countx = 0;
            county = 0;
            stepx = 0;
            stepy = 0;

            bbox_x = 0;
            bbox_y = 0;
            bbox_r = 0;
            bbox_b = 0;

            transform_1 = 1;
            transform_2 = 0;
            transform_3 = 0;
            transform_4 = 1;
            transform_5 = 0;
            transform_6 = 0;
        }

        void LoadFromXml(const std::wstring& strXml)
        {
            XmlUtils::CXmlNode oNode;
            if (oNode.FromXmlString(strXml))
            {
                x		= oNode.ReadAttributeDouble(L"x", 0);
                y		= oNode.ReadAttributeDouble(L"y", 0);
                countx	= oNode.ReadAttributeDouble(L"countx", 0);
                county	= oNode.ReadAttributeDouble(L"county", 0);
                stepx	= oNode.ReadAttributeDouble(L"stepx", 0);
                stepy	= oNode.ReadAttributeDouble(L"stepy", 0);

                XmlUtils::CXmlNode oNodeBox;
                if (oNode.GetNode(L"bbox", oNodeBox))
                {
                    bbox_x	= oNode.ReadAttributeDouble(L"x", 0);
                    bbox_y	= oNode.ReadAttributeDouble(L"y", 0);
                    bbox_r	= oNode.ReadAttributeDouble(L"r", 0);
                    bbox_b	= oNode.ReadAttributeDouble(L"b", 0);
                }

                XmlUtils::CXmlNode oNodeTr;
                if (oNode.GetNode(L"transform", oNodeTr))
                {
                    transform_1	= oNode.ReadAttributeDouble(L"m1", 0);
                    transform_2	= oNode.ReadAttributeDouble(L"m2", 0);
                    transform_3	= oNode.ReadAttributeDouble(L"m3", 0);
                    transform_4	= oNode.ReadAttributeDouble(L"m4", 0);
                    transform_5	= oNode.ReadAttributeDouble(L"m5", 0);
                    transform_6	= oNode.ReadAttributeDouble(L"m6", 0);
                }
            }
        }
    };

    class CGraphicsDumper
    {
    public:
        NSGraphics::IGraphicsRenderer*		m_pRenderer;
        CBgraFrame*             m_pFrame;

        double					m_dWidth;
        double					m_dHeight;

        int 					m_lWidthPix;
        int 					m_lHeightPix;

        RECT					m_oBounds;

        CTileInfo				m_oTile;

    public:
        CGraphicsDumper()
        {
            m_pRenderer			= NULL;
            m_pFrame			= NULL;

            m_dWidth			= -1;
            m_dHeight			= -1;

            m_lWidthPix			= -1;
            m_lHeightPix		= -1;

            m_oBounds.left		= 0;
            m_oBounds.top		= 0;
            m_oBounds.right		= 0;
            m_oBounds.bottom	= 0;
        }
        ~CGraphicsDumper()
        {
            RELEASEOBJECT(m_pRenderer);
            RELEASEOBJECT(m_pFrame);
        }

        void NewPage(double dWidth, double dHeight)
        {
            if (dWidth != m_dWidth || dHeight != m_dHeight)
            {
                RELEASEOBJECT(m_pFrame);

                m_dWidth	= dWidth;
                m_dHeight	= dHeight;
            }

            RELEASEOBJECT(m_pRenderer);

            m_lWidthPix		= (int)(96 * dWidth / 25.4);
            m_lHeightPix	= (int)(96 * dHeight / 25.4);

            if (NULL == m_pFrame)
            {
                m_pFrame = new CBgraFrame();
                m_pFrame->put_Width(m_lWidthPix);
                m_pFrame->put_Height(m_lHeightPix);
                m_pFrame->put_Stride(4 * m_lWidthPix * m_lHeightPix);
                m_pFrame->put_Data(new BYTE[4 * m_lWidthPix * m_lHeightPix]);
            }

            BYTE* pBuffer = m_pFrame->get_Data();
            memset(pBuffer, 0xFF, 4 * m_lWidthPix * m_lHeightPix);

            m_pRenderer = NSGraphics::Create();
            m_pRenderer->put_Width(m_dWidth);
            m_pRenderer->put_Height(m_dHeight);
            m_pRenderer->CreateFromBgraFrame(m_pFrame);
        }

        CBgraFrame* ConvertVectorGraphics()
        {
            BYTE* pBuffer = m_pFrame->get_Data();

            BYTE* pBufferSrcMem = pBuffer + 4 * m_oBounds.top * m_lWidthPix + 4 * m_oBounds.left;
            int lWidthShape     = (int)(m_oBounds.right - m_oBounds.left + 1);
            int lHeightShape	= (int)(m_oBounds.bottom - m_oBounds.top + 1);

            CBgraFrame* pReturnFrame = new CBgraFrame();
            pReturnFrame->put_Width(lWidthShape);
            pReturnFrame->put_Height(lHeightShape);
            pReturnFrame->put_Stride(4 * lWidthShape * lHeightShape);

            BYTE* pBufferDst = new BYTE[4 * lWidthShape * lHeightShape];
            pReturnFrame->put_Data(pBufferDst);

            for (LONG lLine = 0; lLine < lHeightShape; ++lLine)
            {
                memcpy(pBufferDst, pBufferSrcMem, 4 * lWidthShape);

                pBufferDst		+= 4 * lWidthShape;
                pBufferSrcMem	+= 4 * m_lWidthPix;
            }

            return pReturnFrame;
        }

    public:
        inline HRESULT get_Type(LONG* lType)
        {
            if (m_pRenderer)
               return m_pRenderer->get_Type(lType);
            return S_OK;
        }

    //-------- Функции для работы со страницей --------------------------------------------------
        inline HRESULT NewPage()
        {
            if (m_pRenderer)
               return m_pRenderer->NewPage();
            return S_OK;
        }
        inline HRESULT get_Height(double* dHeight)
        {
            if (m_pRenderer)
               return m_pRenderer->get_Height(dHeight);
            return S_OK;
        }
        inline HRESULT put_Height(const double& dHeight)
        {
            if (m_pRenderer)
               return m_pRenderer->put_Height(dHeight);
            return S_OK;
        }
        inline HRESULT get_Width(double* dWidth)
        {
            if (m_pRenderer)
               return m_pRenderer->get_Width(dWidth);
            return S_OK;
        }
        inline HRESULT put_Width(const double& dWidth)
        {
            if (m_pRenderer)
               return m_pRenderer->put_Width(dWidth);
            return S_OK;
        }
        inline HRESULT get_DpiX(double* dDpiX)
        {
            if (m_pRenderer)
               return m_pRenderer->get_DpiX(dDpiX);
            return S_OK;
        }
        inline HRESULT get_DpiY(double* dDpiY)
        {
            if (m_pRenderer)
               return m_pRenderer->get_DpiY(dDpiY);
            return S_OK;
        }

    // pen --------------------------------------------------------------------------------------
        inline HRESULT get_PenColor(LONG* lColor)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenColor(lColor);
            return S_OK;
        }
        inline HRESULT put_PenColor(const LONG& lColor)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenColor(lColor);
            return S_OK;
        }
        inline HRESULT get_PenAlpha(LONG* lAlpha)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenAlpha(lAlpha);
            return S_OK;
        }
        inline HRESULT put_PenAlpha(const LONG& lAlpha)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenAlpha(lAlpha);
            return S_OK;
        }
        inline HRESULT get_PenSize(double* dSize)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenSize(dSize);
            return S_OK;
        }
        inline HRESULT put_PenSize(const double& dSize)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenSize(dSize);
            return S_OK;
        }
        inline HRESULT get_PenDashStyle(BYTE* val)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenDashStyle(val);
            return S_OK;
        }
        inline HRESULT put_PenDashStyle(const BYTE& val)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenDashStyle(val);
            return S_OK;
        }
        inline HRESULT get_PenLineStartCap(BYTE* val)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenLineStartCap(val);
            return S_OK;
        }
        inline HRESULT put_PenLineStartCap(const BYTE& val)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenLineStartCap(val);
            return S_OK;
        }
        inline HRESULT get_PenLineEndCap(BYTE* val)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenLineEndCap(val);
            return S_OK;
        }
        inline HRESULT put_PenLineEndCap(const BYTE& val)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenLineEndCap(val);
            return S_OK;
        }
        inline HRESULT get_PenLineJoin(BYTE* val)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenLineJoin(val);
            return S_OK;
        }
        inline HRESULT put_PenLineJoin(const BYTE& val)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenLineJoin(val);
            return S_OK;
        }
        inline HRESULT get_PenDashOffset(double* dOffset)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenDashOffset(dOffset);
            return S_OK;
        }
        inline HRESULT put_PenDashOffset(const double& dOffset)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenDashOffset(dOffset);
            return S_OK;
        }
        inline HRESULT get_PenAlign(LONG* lAlign)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenAlign(lAlign);
            return S_OK;
        }
        inline HRESULT put_PenAlign(const LONG& lAlign)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenAlign(lAlign);
            return S_OK;
        }
        inline HRESULT get_PenMiterLimit(double* dOffset)
        {
            if (m_pRenderer)
               return m_pRenderer->get_PenMiterLimit(dOffset);
            return S_OK;
        }
        inline HRESULT put_PenMiterLimit(const double& dOffset)
        {
            if (m_pRenderer)
               return m_pRenderer->put_PenMiterLimit(dOffset);
            return S_OK;
        }
        inline HRESULT PenDashPattern(double* pPattern, LONG lCount)
        {
            if (m_pRenderer)
               return m_pRenderer->PenDashPattern(pPattern, lCount);
            return S_OK;
        }

    // brush ------------------------------------------------------------------------------------
        inline HRESULT get_BrushType(LONG* lType)
        {
            if (m_pRenderer)
               return m_pRenderer->get_BrushType(lType);
            return S_OK;
        }
        inline HRESULT put_BrushType(const LONG& lType)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushType(lType);
            return S_OK;
        }
        inline HRESULT get_BrushColor1(LONG* lColor)
        {
            if (m_pRenderer)
               return m_pRenderer->get_BrushColor1(lColor);
            return S_OK;
        }
        inline HRESULT put_BrushColor1(const LONG& lColor)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushColor1(lColor);
            return S_OK;
        }
        inline HRESULT get_BrushAlpha1(LONG* lAlpha)
        {
            if (m_pRenderer)
               return m_pRenderer->get_BrushAlpha1(lAlpha);
            return S_OK;
        }
        inline HRESULT put_BrushAlpha1(const LONG& lAlpha)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushAlpha1(lAlpha);
            return S_OK;
        }
        inline HRESULT get_BrushColor2(LONG* lColor)
        {
            if (m_pRenderer)
               return m_pRenderer->get_BrushColor2(lColor);
            return S_OK;
        }
        inline HRESULT put_BrushColor2(const LONG& lColor)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushColor2(lColor);
            return S_OK;
        }
        inline HRESULT get_BrushAlpha2(LONG* lAlpha)
        {
            if (m_pRenderer)
               return m_pRenderer->get_BrushAlpha2(lAlpha);
            return S_OK;
        }
        inline HRESULT put_BrushAlpha2(const LONG& lAlpha)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushAlpha2(lAlpha);
            return S_OK;
        }
        inline HRESULT get_BrushTexturePath(std::wstring* bsPath)
        {
            if (m_pRenderer)
               return m_pRenderer->get_BrushTexturePath(bsPath);
            return S_OK;
        }
        inline HRESULT put_BrushTexturePath(const std::wstring& bsPath)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushTexturePath(bsPath);
            return S_OK;
        }
        inline HRESULT get_BrushTextureMode(LONG* lMode)
        {
            if (m_pRenderer)
               return m_pRenderer->get_BrushTextureMode(lMode);
            return S_OK;
        }
        inline HRESULT put_BrushTextureMode(const LONG& lMode)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushTextureMode(lMode);
            return S_OK;
        }
        inline HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)
        {
            if (m_pRenderer)
               return m_pRenderer->get_BrushTextureAlpha(lTxAlpha);
            return S_OK;
        }
        inline HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushTextureAlpha(lTxAlpha);
            return S_OK;
        }
        inline HRESULT get_BrushLinearAngle(double* dAngle)
        {
            if (m_pRenderer)
               return m_pRenderer->get_BrushLinearAngle(dAngle);
            return S_OK;
        }
        inline HRESULT put_BrushLinearAngle(const double& dAngle)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushLinearAngle(dAngle);
            return S_OK;
        }
        inline HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
        {
            if (m_pRenderer)
               return m_pRenderer->BrushRect(val, left, top, width, height);
            return S_OK;
        }
        inline HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height)
        {
            if (m_pRenderer)
               return m_pRenderer->BrushBounds(left, top, width, height);
            return S_OK;
        }
        inline HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
        {
            if (m_pRenderer)
               return m_pRenderer->put_BrushGradientColors(lColors, pPositions, nCount);
            return S_OK;
        }

    // font -------------------------------------------------------------------------------------
        inline HRESULT get_FontName(std::wstring* bsName)
        {
            if (m_pRenderer)
               return m_pRenderer->get_FontName(bsName);
            return S_OK;
        }
        inline HRESULT put_FontName(const std::wstring& bsName)
        {
            if (m_pRenderer)
               return m_pRenderer->put_FontName(bsName);
            return S_OK;
        }
        inline HRESULT get_FontPath(std::wstring* bsName)
        {
            if (m_pRenderer)
               return m_pRenderer->get_FontPath(bsName);
            return S_OK;
        }
        inline HRESULT put_FontPath(const std::wstring& bsName)
        {
            if (m_pRenderer)
               return m_pRenderer->put_FontPath(bsName);
            return S_OK;
        }
        inline HRESULT get_FontSize(double* dSize)
        {
            if (m_pRenderer)
               return m_pRenderer->get_FontSize(dSize);
            return S_OK;
        }
        inline HRESULT put_FontSize(const double& dSize)
        {
            if (m_pRenderer)
               return m_pRenderer->put_FontSize(dSize);
            return S_OK;
        }
        inline HRESULT get_FontStyle(LONG* lStyle)
        {
            if (m_pRenderer)
               return m_pRenderer->get_FontStyle(lStyle);
            return S_OK;
        }
        inline HRESULT put_FontStyle(const LONG& lStyle)
        {
            if (m_pRenderer)
               return m_pRenderer->put_FontStyle(lStyle);
            return S_OK;
        }
        inline HRESULT get_FontStringGID(INT* bGID)
        {
            if (m_pRenderer)
               return m_pRenderer->get_FontStringGID(bGID);
            return S_OK;
        }
        inline HRESULT put_FontStringGID(const INT& bGID)
        {
            if (m_pRenderer)
               return m_pRenderer->put_FontStringGID(bGID);
            return S_OK;
        }
        inline HRESULT get_FontCharSpace(double* dSpace)
        {
            if (m_pRenderer)
               return m_pRenderer->get_FontCharSpace(dSpace);
            return S_OK;
        }
        inline HRESULT put_FontCharSpace(const double& dSpace)
        {
            if (m_pRenderer)
               return m_pRenderer->put_FontCharSpace(dSpace);
            return S_OK;
        }
        inline HRESULT get_FontFaceIndex(int* lFaceIndex)
        {
            if (m_pRenderer)
               return m_pRenderer->get_FontFaceIndex(lFaceIndex);
            return S_OK;
        }
        inline HRESULT put_FontFaceIndex(const int& lFaceIndex)
        {
            if (m_pRenderer)
               return m_pRenderer->put_FontFaceIndex(lFaceIndex);
            return S_OK;
        }

    //-------- Функции для вывода текста --------------------------------------------------------
        inline HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
        {
            if (m_pRenderer)
               return m_pRenderer->CommandDrawTextCHAR(c, x, y, w, h);
            return S_OK;
        }
        inline HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
        {
            if (m_pRenderer)
               return m_pRenderer->CommandDrawText(bsText, x, y, w, h);
            return S_OK;
        }
        inline HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
        {
            if (m_pRenderer)
               return m_pRenderer->CommandDrawTextExCHAR(c, gid, x, y, w, h);
            return S_OK;
        }
        inline HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
        {
            if (m_pRenderer)
               return m_pRenderer->CommandDrawTextEx(bsUnicodeText, pGids, nGidsCount, x, y, w, h);
            return S_OK;
        }

    //-------- Маркеры для команд ---------------------------------------------------------------
        inline HRESULT BeginCommand(const DWORD& lType)
        {
            if (m_pRenderer)
               return m_pRenderer->BeginCommand(lType);
            return S_OK;
        }
        inline HRESULT EndCommand(const DWORD& lType)
        {
            if (m_pRenderer)
               return m_pRenderer->EndCommand(lType);
            return S_OK;
        }

    //-------- Функции для работы с Graphics Path -----------------------------------------------
        inline HRESULT PathCommandMoveTo(const double& x, const double& y)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandMoveTo(x, y);
            return S_OK;
        }
        inline HRESULT PathCommandLineTo(const double& x, const double& y)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandLineTo(x, y);
            return S_OK;
        }
        inline HRESULT PathCommandLinesTo(double* points, const int& count)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandLinesTo(points, count);
            return S_OK;
        }
        inline HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
            return S_OK;
        }
        inline HRESULT PathCommandCurvesTo(double* points, const int& count)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandCurvesTo(points, count);
            return S_OK;
        }
        inline HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandArcTo(x, y, w, h, startAngle, sweepAngle);
            return S_OK;
        }
        inline HRESULT PathCommandClose()
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandClose();
            return S_OK;
        }
        inline HRESULT PathCommandEnd()
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandEnd();
            return S_OK;
        }
        inline HRESULT DrawPath(const LONG& nType)
        {
            if (m_pRenderer)
               return m_pRenderer->DrawPath(nType);
            return S_OK;
        }
        inline HRESULT PathCommandStart()
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandStart();
            return S_OK;
        }
        inline HRESULT PathCommandGetCurrentPoint(double* x, double* y)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandGetCurrentPoint(x, y);
            return S_OK;
        }
        inline HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandTextCHAR(c, x, y, w, h);
            return S_OK;
        }
        inline HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandText(bsText, x, y, w, h);
            return S_OK;
        }
        inline HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandTextExCHAR(c, gid, x, y, w, h);
            return S_OK;
        }
        inline HRESULT PathCommandTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
        {
            if (m_pRenderer)
               return m_pRenderer->PathCommandTextEx(bsUnicodeText, pGids, nGidsCount, x, y, w, h);
            return S_OK;
        }

    //-------- Функции для вывода изображений ---------------------------------------------------
        inline HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)
        {
            if (m_pRenderer)
               return m_pRenderer->DrawImage(pImage, x, y, w, h);
            return S_OK;
        }

        inline HRESULT DrawImageFromFile(const std::wstring& sPath, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255)
        {
            if (m_pRenderer)
               return m_pRenderer->DrawImageFromFile(sPath, x, y, w, h, lAlpha);
            return S_OK;
        }

    // transform --------------------------------------------------------------------------------
        inline HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6)
        {
            if (m_pRenderer)
               return m_pRenderer->SetTransform(m1, m2, m3, m4, m5, m6);
            return S_OK;
        }
        inline HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
        {
            if (m_pRenderer)
               return m_pRenderer->GetTransform(pdA, pdB, pdC, pdD, pdE, pdF);
            return S_OK;
        }
        inline HRESULT ResetTransform()
        {
            if (m_pRenderer)
               return m_pRenderer->ResetTransform();
            return S_OK;
        }

    // -----------------------------------------------------------------------------------------
        inline HRESULT get_ClipMode(LONG* plMode)
        {
            if (m_pRenderer)
               return m_pRenderer->get_ClipMode(plMode);
            return S_OK;
        }
        inline HRESULT put_ClipMode(const LONG& lMode)
        {
            if (m_pRenderer)
               return m_pRenderer->put_ClipMode(lMode);
            return S_OK;
        }

        inline virtual HRESULT StartConvertCoordsToIdentity()
        {
            if (m_pRenderer)
                m_pRenderer->StartConvertCoordsToIdentity();
            return S_OK;
        }
        inline virtual HRESULT EndConvertCoordsToIdentity()
        {
            if (m_pRenderer)
                m_pRenderer->EndConvertCoordsToIdentity();
            return S_OK;
        }
    };


    class CPageMeta
    {
    public:
        double m_dWidth;
        double m_dHeight;
        LONG m_lStart;
        LONG m_lEnd;

    public:
        CPageMeta()
        {
            m_dWidth = 0;
            m_dHeight = 0;
            m_lStart = 0;
            m_lEnd = 0;
        }
        CPageMeta(const CPageMeta& oSrc)
        {
            *this = oSrc;
        }

        CPageMeta& operator=(const CPageMeta& oSrc)
        {
            m_dWidth = oSrc.m_dWidth;
            m_dHeight = oSrc.m_dHeight;
            m_lStart = oSrc.m_lStart;
            m_lEnd = oSrc.m_lEnd;

            return *this;
        }
    };

    class CFontEmbedded
    {
    public:
        std::map<int, BYTE>     m_mapChars;

        std::wstring			m_strFontName;
        LONG					m_lFontStyle;
        int						m_lFontPathLen;

        std::wstring			m_strFontPath;
        DWORD					m_dwCRC32;

    public:
        CFontEmbedded() : m_mapChars()
        {
            m_strFontName		= L"";
            m_strFontPath		= L"";

            m_lFontPathLen		= 0;
            m_lFontStyle		= 0;
            m_dwCRC32			= 0;
        }
        CFontEmbedded(const CFontEmbedded& oSrc)
        {
            *this = oSrc;
        }
        CFontEmbedded& operator=(const CFontEmbedded& oSrc)
        {
            m_strFontPath		= oSrc.m_strFontPath;
            m_strFontName		= oSrc.m_strFontName;
            m_lFontStyle		= oSrc.m_lFontStyle;
            m_dwCRC32			= oSrc.m_dwCRC32;

            m_lFontPathLen		= oSrc.m_lFontPathLen;

            for (std::map<int, BYTE>::const_iterator i = oSrc.m_mapChars.begin(); i != oSrc.m_mapChars.end(); i++)
            {
                m_mapChars.insert(std::pair<int, BYTE>(i->first, i->second));
            }
            return *this;
        }

    public:
        void AddString(const int* sText, const int& len)
        {
            if (NULL != sText)
            {
                for (int i = 0; i < len; ++i)
                {
                    if (m_mapChars.end() == m_mapChars.find(sText[i]))
                        m_mapChars.insert(std::pair<int, BYTE>(sText[i], 1));
                }
            }
        }
        void GenerateArray(int*& pData, int& nCount)
        {
            nCount = m_mapChars.size();
            if (0 == nCount)
                return;

            pData = new int[nCount];

            int* pDataCur = pData;
            for (std::map<int, BYTE>::const_iterator i = m_mapChars.begin(); i != m_mapChars.end(); i++)
            {
                *pDataCur++ = i->first;
            }
        }
    };

    class CFontDstGenerator
    {
    public:
        CFontEmbedded*	m_pFonts;
        LONG			m_lCurrentIndex;
        LONG			m_lCountFonts;
        LONG			m_lSize;

        std::map<std::wstring, DWORD> m_mapFontPathCRC;
        std::map<DWORD, LONG> m_mapFontCRCIndex;
        CCalculatorCRC32 m_oCalc;

        std::wstring   m_strCurrentFontPath;
        int            m_lCurrentFontStyle;
        int            m_lCurrentFontPathLen;
        std::map<std::wstring, int> m_mapFontPathToIndex;

    public:
        CFontDstGenerator()
        {
            m_lSize			= 50;
            m_pFonts		= NULL;
            m_lCurrentIndex	= -1;
            m_lCountFonts	= 0;

            m_strCurrentFontPath = L"";
            m_lCurrentFontPathLen = 0;
            m_lCurrentFontStyle = -1;

            Grow();
        }
        ~CFontDstGenerator()
        {
            RELEASEARRAYOBJECTS(m_pFonts);
        }
    public:

        LONG AddFont(NSStructures::CFont* pFont, CFontManagerBase* pBase, const int* symbols, const int& count)
        {
            // TODO:
            // нужно учитывать FaceIndex. В пдфах не бывает никогда таких файлов (но могут)
            // поэтому решил не менять, чтобы не нарушить. Как будет время - сделать зависимость кэша
            // не только от пути, но и от индекса

            bool bIsDumpFontEmptyPath = false;

            int nLen = (int)pFont->Path.length();
            if (nLen == 0)
            {
                pFont->Path = pBase->GetFontPath(pFont);
                nLen = pFont->Path.length();
                bIsDumpFontEmptyPath = true;
            }

            LONG lNewStyle = pFont->GetStyle2();

            bool bIsFontChanged = true;
            if (nLen == m_lCurrentFontPathLen && (m_lCurrentFontStyle == lNewStyle))
            {
                if (m_strCurrentFontPath == pFont->Path)
                    bIsFontChanged = false;
            }

            if (!bIsFontChanged)
            {
                m_pFonts[m_lCurrentIndex].AddString(symbols, count);

                if (bIsDumpFontEmptyPath)
                    pFont->Path = L"";

                return m_lCurrentIndex;
            }

            std::map<std::wstring, int>::const_iterator pPairIndex = m_mapFontPathToIndex.find(pFont->Path);
            if (m_mapFontPathToIndex.end() != pPairIndex)
            {
                m_lCurrentIndex = pPairIndex->second;

                NSHtmlRenderer::CFontEmbedded* pEmb = &m_pFonts[m_lCurrentIndex];

                m_strCurrentFontPath = pFont->Path;
                m_lCurrentFontPathLen = nLen;
                m_lCurrentFontStyle = lNewStyle;

                pEmb->AddString(symbols, count);

                if (bIsDumpFontEmptyPath)
                    pFont->Path = L"";

                return m_lCurrentIndex;
            }

            // такой путь еще не приходил.
            // первым делом проверим, не совпадает ли он с каким-то (по чексумме)
            NSFile::CFileBinary oFile;
            oFile.OpenFile(pFont->Path);
            DWORD lFontFileSize = (DWORD)oFile.GetFileSize();
            BYTE* pFontPathData = new BYTE[lFontFileSize];
            DWORD dwRead = 0;
            oFile.ReadFile(pFontPathData, lFontFileSize, dwRead);
            DWORD dwCRC32 = m_oCalc.Calc(pFontPathData, (int)lFontFileSize);

            oFile.CloseFile();
            RELEASEARRAYOBJECTS(pFontPathData);

            std::map<DWORD, LONG>::const_iterator pPair2 = m_mapFontCRCIndex.find(dwCRC32);
            if (m_mapFontCRCIndex.end() == pPair2)
            {
                // шрифт реально новый
                m_mapFontCRCIndex.insert(std::pair<DWORD, LONG>(dwCRC32, m_lCountFonts));
                m_mapFontPathToIndex.insert(std::pair<std::wstring, int>(pFont->Path, m_lCountFonts));
            }
            else
            {
                m_lCurrentIndex = pPair2->second;
                m_mapFontPathToIndex.insert(std::pair<std::wstring, int>(pFont->Path, m_lCurrentIndex));

                m_strCurrentFontPath = pFont->Path;
                m_lCurrentFontPathLen = nLen;
                m_lCurrentFontStyle = lNewStyle;

                m_pFonts[m_lCurrentIndex].AddString(symbols, count);

                if (bIsDumpFontEmptyPath)
                    pFont->Path = L"";

                return m_lCurrentIndex;
            }

            if (m_lCountFonts == m_lSize)
            {
                // нужно перевыделить память
                Grow();
            }

            m_lCurrentIndex = m_lCountFonts;
            ++m_lCountFonts;

            NSHtmlRenderer::CFontEmbedded* pCurEmb = &m_pFonts[m_lCurrentIndex];
            pCurEmb->m_strFontName = pFont->Name;
            pCurEmb->m_strFontPath = pFont->Path;
            pCurEmb->m_lFontStyle	= lNewStyle;
            pCurEmb->m_lFontPathLen = nLen;
            pCurEmb->AddString(symbols, count);

            // теперь нужно найти к нему путь...
            if (0 == nLen)
            {
                pCurEmb->m_strFontPath = pBase->GetFontPath(pFont);
                pCurEmb->m_lFontPathLen = pCurEmb->m_strFontPath.length();
            }

            if (bIsDumpFontEmptyPath)
                pFont->Path = L"";

            m_strCurrentFontPath = pCurEmb->m_strFontPath;
            m_lCurrentFontPathLen = pCurEmb->m_lFontPathLen;
            m_lCurrentFontStyle = pCurEmb->m_lFontStyle;

            return m_lCurrentIndex;
        }


    public:
        void WriteFonts(NSFonts::IFontManager* pFontManager, const std::wstring& strFolderDst, bool bIsGid = false)
        {
            std::wstring sDstF = strFolderDst;
            NSDirectory::CreateDirectory(sDstF);

            std::wstring strAllTypes = L"";

            std::wstring strTempFont = sDstF + L"/font.tmp";

            CFontConverter oFontConverter;

            LONG lFontConverterFlag = 16;
            if (bIsGid)
                lFontConverterFlag |= 0x0080;

            for (LONG lIndex = 0; lIndex < m_lCountFonts; ++lIndex)
            {
                CFontEmbedded& oCur = m_pFonts[lIndex];
                pFontManager->LoadFontFromFile(oCur.m_strFontPath, 0, 12, 72, 72);

                std::wstring sName = L"";

                std::wstring sNameSrc = pFontManager->GetName();
                int nNameLen = (int)sNameSrc.length();
                const wchar_t* pNameStr = sNameSrc.c_str();
                for (int i = 0; i < nNameLen; ++i)
                {
                    if ((pNameStr[i] >= 'a' && pNameStr[i] <= 'z') ||
                        (pNameStr[i] >= 'A' && pNameStr[i] <= 'Z') ||
                        (pNameStr[i] >= '0' && pNameStr[i] <= '9'))
                    {
                        sName.push_back(pNameStr[i]);
                    }
                    else
                    {
                        sName.push_back((wchar_t)'_');
                    }
                }

                strAllTypes += (L"embedded" + std::to_wstring(lIndex));
                strAllTypes += L": ";
                strAllTypes += pFontManager->GetFontType();
                strAllTypes += L", ";
                strAllTypes += sName;
                strAllTypes += L"\n";

                int* symbols = NULL;
                int symbolsCount = 0;
                oCur.GenerateArray(symbols, symbolsCount);

                // есть проблема с композитными глифами (буква ё). пока отключу конвертацию
                lFontConverterFlag = 0;
                oFontConverter.ToOTF(oCur.m_strFontPath, strTempFont, (unsigned int*)symbols, symbolsCount, sName, lFontConverterFlag); // TRUETYPE only

                RELEASEARRAYOBJECTS(symbols);

                // dump font
                NSFile::CFileBinary oFileFontFile;
                oFileFontFile.OpenFile(strTempFont);

                int nInputLen = (int)oFileFontFile.GetFileSize();
                BYTE* pData = new BYTE[nInputLen];
                DWORD dwReadSize = 0;
                oFileFontFile.ReadFile(pData, (DWORD)nInputLen, dwReadSize);
                oFileFontFile.CloseFile();

                char* pOutput = NULL;
                int nOutputLen = 0;
                NSFile::CBase64Converter::Encode(pData, nInputLen, pOutput, nOutputLen, NSBase64::B64_BASE64_FLAG_NOCRLF);

                NSFile::CFileBinary oFileFontFileJS;
                oFileFontFileJS.CreateFileW(sDstF + L"/embedded" + std::to_wstring(lIndex) + L".js");

                std::string sHeader = "window[\"embedded" + std::to_string(lIndex) + "\"] = \"" + std::to_string(nInputLen) + ";";
                oFileFontFileJS.WriteFile((const BYTE*)sHeader.c_str(), sHeader.length());
                oFileFontFileJS.WriteFile((const BYTE*)pOutput, nOutputLen);
                std::string sFooter = "\";";
                oFileFontFileJS.WriteFile((const BYTE*)sFooter.c_str(), sFooter.length());

                RELEASEARRAYOBJECTS(pOutput);
                RELEASEARRAYOBJECTS(pData);

                NSFile::CFileBinary::Remove(strTempFont);
            }

            NSFile::CFileBinary oFileFontFileJS_type;
            oFileFontFileJS_type.CreateFileW(sDstF + L"/types.txt");
            oFileFontFileJS_type.WriteStringUTF8(strAllTypes);
            oFileFontFileJS_type.CloseFile();
        }

    protected:
        void Grow()
        {
            if (NULL == m_pFonts)
            {
                m_pFonts = new CFontEmbedded[m_lSize];
                return;
            }

            m_lSize *= 2;
            CFontEmbedded* pNewBuffer = new CFontEmbedded[m_lSize];
            for (LONG i = 0; i < m_lCountFonts; ++i)
            {
                pNewBuffer[i] = m_pFonts[i];
            }
            RELEASEARRAYOBJECTS(m_pFonts);
            m_pFonts = pNewBuffer;
        }
    };

    class CWriter
    {
    public:
        CMetafile		m_oPage;
        CSVGWriter2		m_oSVGWriter;

        std::vector<CPageMeta>	m_arrPages;
        bool					m_bIsBigPicture;

        int 					m_lTilingCounter;

    public:
        NSStructures::CPen*		m_pPen;
        NSStructures::CBrush*	m_pBrush;
        NSStructures::CFont*	m_pFont;

        NSStructures::CPen		m_oLastPen;
        NSStructures::CBrush	m_oLastBrush;

        int 					m_lCurrentFont;
        double					m_dCurrentFontSize;

        CFontDstGenerator               m_oDstFontGenerator;
        NSHtmlRenderer::CFontManager    m_oFontManager;

        CHText					m_oSmartText;

        double m_dDpiX;
        double m_dDpiY;

        bool					m_bPathClosed;

        std::map<std::wstring, CImageInfo>	m_mapImagesFile;
        std::map<DWORD, CImageInfo>         m_mapImageData;

        CCalculatorCRC32		m_oCRC;

        double m_dWidthDocMM;
        double m_dHeightDocMM;
        double m_dHeightPageMM;

        Aggplus::CGraphicsPathSimpleConverter*	m_pSimpleConverter;
        NSFile::CFileBinary m_oFileWriter;

        bool m_bIsGids;

        NSFonts::IApplicationFonts*      m_pApplicationFonts;

    public:

        double					m_dWidth;
        double					m_dHeight;
        bool					m_bIsImageFromVectors;

    private:

        LONG					m_lNextIDShape;
        LONG					m_lNextIDImage;

        LONG					m_lCurrentPage;
        LONG					m_lCurrentFunctionPage;
        LONG					m_lPagesCount;

        LONG					m_lMaxSizeImage;

    public:
        std::wstring			m_strDstDirectory;
        std::wstring			m_strDstMedia;
        std::wstring			m_strDstDirectoryFiles;
        std::wstring			m_strFileName;

        Aggplus::CMatrix*   	m_pTransform;
        Aggplus::CMatrix		m_oLastTransform;

        int 					m_lCurrentDumpSize;

        int 					m_lSrcFileType;

        bool					m_bIsClipping;
        bool					m_bIsSimpleGraphics;

        bool                    m_bIsOnlyTextMode;

    public:
        CWriter()
        {
            m_dDpiX = 96.0;
            m_dDpiY = 96.0;

            m_dWidth	= 0;
            m_dHeight	= 0;

            m_lNextIDImage	= 0;
            m_lNextIDShape	= 0;

            m_lCurrentPage	= -1;

            m_bPathClosed	= true;

            m_pSimpleConverter = NULL;

            m_lMaxSizeImage = 800;

            m_dWidthDocMM	= 0;
            m_dHeightDocMM	= 0;
            m_dHeightPageMM	= 0;

            m_lPagesCount		= 0;

            m_bIsImageFromVectors = false;

            m_lSrcFileType = 0;

            m_bIsClipping = false;
            m_bIsSimpleGraphics = false;
            m_lTilingCounter = 0;

            m_bIsOnlyTextMode = false;
        }

        void SetApplicationFonts(NSFonts::IApplicationFonts* pFonts)
        {
            m_pApplicationFonts = pFonts;
        }

        void SetSimpleConverter(Aggplus::CGraphicsPathSimpleConverter* pSimpleConverter, Aggplus::CMatrix* pMatrix)
        {
            m_pSimpleConverter = pSimpleConverter;
            m_pTransform = pMatrix;

            m_oSVGWriter.m_pTransform = m_pTransform;

            m_oSVGWriter.m_pPen = m_pPen;
            m_oSVGWriter.m_pBrush = m_pBrush;

            m_oSVGWriter.m_pLastPen = &m_oLastPen;
            m_oSVGWriter.m_pLastBrush = &m_oLastBrush;
        }

        void WriteText(const int* pUnicodes, const int* pGids, const int& nCount, const double& x, const double& y,
                      const double& width, const double& height, const bool& bIsChangedFontParamBetweenDrawText)
        {
            if (m_lSrcFileType == AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)
            {
                // не нужно ничего писать из djvu
                return;
            }

            CheckVectorGraphics();
            CheckTectClipRect();

            bool bIsDumpFont = false;

            if (m_lCurrentFont == -1 || bIsChangedFontParamBetweenDrawText)
            {
                const int* pSymbols = (NULL == pGids) ? pUnicodes : pGids;
                LONG lCurrentFontIndex = m_oDstFontGenerator.AddFont(m_pFont, &m_oFontManager, pSymbols, nCount);
                if ((lCurrentFontIndex != m_lCurrentFont) || (m_pFont->Size != m_dCurrentFontSize))
                {
                    m_lCurrentFont		= lCurrentFontIndex;
                    m_dCurrentFontSize	= m_pFont->Size;

                    bIsDumpFont = true;
                }
            }
            else
            {
                const int* pSymbols = (NULL == pGids) ? pUnicodes : pGids;
                m_oDstFontGenerator.m_pFonts[m_lCurrentFont].AddString(pSymbols, nCount);
            }

            if (NULL != pGids)
                m_bIsGids = true;

            m_oSmartText.CommandText(pUnicodes, pGids, nCount, x, y, width, height, bIsDumpFont, this);
            return;
        }

        inline void CheckTectClipRect()
        {
            // смотрим, нужно ли обновить рект для клиппирования текста
            if (m_oSVGWriter.m_bIsTextClipWriteCleared)
            {
                if (!m_oSVGWriter.m_oTextClipBounds.IsCleared)
                {
                    m_oSmartText.DumpLine();
                    // записать клип
                    m_oPage.WriteCommandType(CMetafile::ctCommandTextClipRect);
                    m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.x);
                    m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.y);
                    m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.r);
                    m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.b);
                    m_oSVGWriter.m_bIsTextClipWriteCleared = false;
                }
            }
            else
            {
                if (m_oSVGWriter.m_oTextClipBounds.IsCleared)
                {
                    m_oSmartText.DumpLine();
                    // записать команду сброса клипа текстректа
                    m_oPage.WriteCommandType(CMetafile::ctCommandTextClipRectReset);
                    m_oSVGWriter.m_bIsTextClipWriteCleared = true;
                    return;
                }
                else if (m_oSVGWriter.m_bIsIntersectNewClipRect)
                {
                    m_oSmartText.DumpLine();
                    // записать клип
                    m_oPage.WriteCommandType(CMetafile::ctCommandTextClipRect);
                    m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.x);
                    m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.y);
                    m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.r);
                    m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.b);
                    m_oSVGWriter.m_bIsIntersectNewClipRect = false;
                }
            }
        }

        inline void WriteBeginPath()
        {
        }
        inline void WriteEndPath()
        {
            m_oSVGWriter.WritePathEnd();
        }
        inline void WritePathStart()
        {
        }
        inline void WritePathClose()
        {
            m_oSVGWriter.WritePathClose();
        }
        inline void WritePathMoveTo(const double& x, const double& y)
        {
            m_oSVGWriter.WritePathMoveTo(x, y);
        }
        inline void WritePathLineTo(const double& x, const double& y)
        {
            m_oSVGWriter.WritePathLineTo(x, y);
        }
        inline void WritePathCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
        {
            m_oSVGWriter.WritePathCurveTo(x1, y1, x2, y2, x3, y3);
        }
        inline void WriteDrawPath(LONG lType)
        {
            /*
            if (lType > 0xFF)
            {
                if (!m_oLastBrush.IsEqual(m_pBrush))
                {
                    m_oLastBrush = *m_pBrush;
                    SetFillColor(true);
                }
            }
            if (lType & 0x01)
            {
                if (!m_oLastPen.IsEqual(m_pPen))
                {
                    m_oLastPen = *m_pPen;
                    SetStrokeColor(true);
                    SetLineWidth(true);
                }
            }

            m_oVectors.WriteCommandType(CMetafile::ctDrawPath);
            m_oVectors.WriteLONG(lType);
            */

            CImageInfo oInfo;
            if ((lType > 0xFF) && (c_BrushTypeTexture == m_pBrush->Type))
            {
                oInfo = GenerateImageID(m_pBrush->TexturePath);

                if (TRUE)
                {
                    // пока делаем так
                    double x = 0;
                    double y = 0;
                    double w = 0;
                    double h = 0;
                    m_pSimpleConverter->PathCommandGetBounds(x, y, w, h);

                    CheckVectorGraphics();

                    /*
                    bool bIsClip = (m_oSVGWriter.m_oClipMetafile.GetPosition() > 0) ? true : false;
                    if (bIsClip)
                        m_oPage.Write(m_oSVGWriter.m_oClipMetafile);
                    */
                    LONG _oldPos = m_oSVGWriter.WriteTempClip();
                    m_oPage.Write(m_oSVGWriter.m_oClipMetafile);

                    WriteImage2(oInfo, x, y, w, h);

                    m_oSVGWriter.m_oClipMetafile.Seek(_oldPos);

                    /*
                    if (bIsClip)
                        m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);
                    */
                    m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);

                    lType &= 0xFF;
                }
            }

            m_oSVGWriter.WriteDrawPath(lType, m_pSimpleConverter, oInfo);
        }
        inline void WritePathClip()
        {
           m_oSVGWriter.WritePathClip();
        }
        inline void WritePathClipEnd()
        {
           m_oSVGWriter.WritePathClipEnd();
        }
        inline void WritePathResetClip()
        {
           m_oSVGWriter.WritePathResetClip();
        }

        void NewPage(const double& dWidthMM, const double& dHeightMM)
        {
            ++m_lPagesCount;

            CPageMeta oInfo;
            oInfo.m_dWidth = dWidthMM;
            oInfo.m_dHeight = dHeightMM;
            oInfo.m_lStart = m_lCurrentDumpSize;
            oInfo.m_lEnd = 0;

            m_dWidth = dWidthMM;
            m_dHeight = dHeightMM;

            m_arrPages.push_back(oInfo);

            m_oLastBrush.Color1 = -1;
            m_oLastPen.Color	= -1;

            m_lCurrentFont		= -1;
            m_dCurrentFontSize	= 0.0;

            m_oLastTransform.Reset();
            m_pTransform->Reset();

            m_bIsBigPicture = false;

            m_oSVGWriter.NewDocument(m_dWidth, m_dHeight, m_lPagesCount - 1);
        }
        void EndPage()
        {
            m_oSmartText.ClosePage();
            CheckVectorGraphics();

            m_arrPages[m_arrPages.size() - 1].m_lEnd = m_lCurrentDumpSize + m_oPage.GetPosition();
            m_lCurrentDumpSize += m_oPage.GetPosition();

            if (!m_bIsOnlyTextMode)
                m_oFileWriter.WriteFile(m_oPage.GetData(), m_oPage.GetPosition());

            m_oPage.Clear();
        }

        void GetLastPageInfo(int& paragraphs, int& words, int& symbols, int& spaces, std::string& sBase64Data)
        {
            m_oSmartText.ClosePage();

            paragraphs = (int)m_oSmartText.m_lCountParagraphs;
            words = (int)m_oSmartText.m_lCountWords;
            spaces = (int)m_oSmartText.m_lCountSpaces;
            symbols = (int)m_oSmartText.m_lCountSymbols;
            sBase64Data = "";

            if (m_lPagesCount > 0)
            {
                char* pDst = NULL;
                int nDst = 0;
                NSFile::CBase64Converter::Encode(m_oPage.GetData(), (int)m_oPage.GetPosition(), pDst, nDst, NSBase64::B64_BASE64_FLAG_NOCRLF);

                if (0 < nDst)
                    sBase64Data = std::string(pDst, (size_t)nDst);

                sBase64Data = std::to_string((int)m_oPage.GetPosition()) + ";" + sBase64Data;

                RELEASEARRAYOBJECTS(pDst);
            }
        }

        inline void CheckVectorGraphics()
        {
            /*
            if (0 < m_oVectors.GetPosition() && m_bIsSimpleGraphics && !m_oSVGWriter.m_bIsClipping)
            {
                m_oPage.Write(m_oVectors);
            }
            else if (m_oSVGWriter.m_oDocument.GetCurSize() > 0)
            {
                WriteImageID_SVG();
            }

            m_oVectors.ClearNoAttack();
            */
            if (m_oSVGWriter.m_lEmtyDocChecker < (ULONG)m_oSVGWriter.m_oDocument.GetCurSize())
                WriteImageID_SVG();
        }

        inline void WritePattern(CBgraFrame* pPattern, CTileInfo& oTile)
        {
            CheckVectorGraphics();
            m_oSmartText.DumpLine();
            SetTransformToDocument(true);

            NSHtmlRenderer::CImageInfo oInfo = GenerateImageID(pPattern);

            bool bIsClip = (m_oSVGWriter.m_oClipMetafile.GetPosition() > 0) ? true : false;
            if (bIsClip)
                m_oPage.Write(m_oSVGWriter.m_oClipMetafile);

            WriteImagePattrern(oInfo, oTile);

            if (bIsClip)
                m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);
        }

        inline void WriteImage(IGrObject* pImage, double x, double y, double width, double height)
        {
            CheckVectorGraphics();
            m_oSmartText.DumpLine();
            SetTransformToDocument(true);

            bool bIsClip = (m_oSVGWriter.m_oClipMetafile.GetPosition() > 0) ? true : false;
            if (bIsClip)
                m_oPage.Write(m_oSVGWriter.m_oClipMetafile);

            NSHtmlRenderer::CImageInfo oInfo = GenerateImageID(pImage);
            //m_oSVGWriter.WriteImage(oInfo, x, y, width, height);
            WriteImage2(oInfo, x, y, width, height);

            if (bIsClip)
                m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);
        }
        inline void WriteImage(const std::wstring& sPath, double x, double y, double width, double height)
        {
            if (m_lTilingCounter > 0)
            {
                NSHtmlRenderer::CImageInfo oInfo = GenerateImageID(sPath);
                m_oSVGWriter.WriteImage(oInfo, x, y, width, height);
                return;
            }

            CheckVectorGraphics();
            m_oSmartText.DumpLine();
            SetTransformToDocument(true);

            bool bIsClip = (m_oSVGWriter.m_oClipMetafile.GetPosition() > 0) ? true : false;
            if (bIsClip)
                m_oPage.Write(m_oSVGWriter.m_oClipMetafile);

            NSHtmlRenderer::CImageInfo oInfo = GenerateImageID(sPath);
            //m_oSVGWriter.WriteImage(oInfo, x, y, width, height);
            WriteImage2(oInfo, x, y, width, height);

            if (bIsClip)
                m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);
        }

        NSHtmlRenderer::CImageInfo GenerateImageID(IGrObject* pGrObject)
        {
            CBgraFrame* pFrame = new CBgraFrame();
            pFrame->FromImage(pGrObject, false);

            NSHtmlRenderer::CImageInfo _info = GenerateImageID(pFrame, true);

            pFrame->put_Data(NULL);
            RELEASEOBJECT(pFrame);
            return _info;
        }

        NSHtmlRenderer::CImageInfo GenerateImageID(CBgraFrame* pFrame, bool bIsFromObject = false)
        {
            CImageInfo oInfo;

            if (NULL == pFrame)
                return oInfo;

            int nLen = 4 * pFrame->get_Width() * pFrame->get_Height();
            BYTE* pBuffer = pFrame->get_Data();

            DWORD dwSum = (DWORD)m_oCRC.Calc(pBuffer, nLen);

            std::map<DWORD, CImageInfo>::const_iterator pPair = m_mapImageData.find(dwSum);
            if (m_mapImageData.end() == pPair)
            {
                // нужно добавить
                ++m_lNextIDImage;

                oInfo.m_lID = m_lNextIDImage;
                SaveImage(pFrame, oInfo, !bIsFromObject);

                m_mapImageData.insert(std::pair<DWORD, CImageInfo>(dwSum, oInfo));
            }
            else
            {
                pFrame->put_Data(NULL);
                oInfo = pPair->second;
            }

            return oInfo;
        }
        CImageInfo GenerateImageID(const std::wstring& strFileName)
        {
            CImageInfo oInfo;
            std::map<std::wstring, CImageInfo>::const_iterator pPair = m_mapImagesFile.find(strFileName);

            if (m_mapImagesFile.end() == pPair)
            {
                // нужно добавить
                ++m_lNextIDImage;

                oInfo.m_lID = m_lNextIDImage;

                CBgraFrame oFrame;
                oFrame.OpenFile(strFileName);
                SaveImage(&oFrame, oInfo, true);

                m_mapImagesFile.insert(std::pair<std::wstring, CImageInfo>(strFileName, oInfo));
            }
            else
            {
                oInfo = pPair->second;
            }

            return oInfo;
        }

        ImageType GetImageType(CBgraFrame* pFrame)
        {
            BYTE* pBufferMem = pFrame->get_Data() + 3;
            LONG lCountPix = pFrame->get_Width() * pFrame->get_Height();

            for (LONG i = 0; i < lCountPix; ++i, pBufferMem += 4)
            {
                if (255 != *pBufferMem)
                    return itPNG;
            }
            return itJPG;
        }

        inline void SaveImage(CBgraFrame* pFrame, CImageInfo& oInfo, bool bIsDestroy)
        {
            oInfo.m_eType = GetImageType(pFrame);

            int lWidth = pFrame->get_Width();
            int lHeight = pFrame->get_Height();

            bool bIsResized = false;
            if (true || (lWidth <= m_lMaxSizeImage) && (lHeight <= m_lMaxSizeImage))
            {
                // не ресайзим
            }
            else
            {
                LONG lW = 0;
                LONG lH = 0;
                double dAspect = (double)lWidth / lHeight;

                if (lWidth >= lHeight)
                {
                    lW = m_lMaxSizeImage;
                    lH = (LONG)((double)lW / dAspect);
                }
                else
                {
                    lH = m_lMaxSizeImage;
                    lW = (LONG)(dAspect * lH);
                }

                bIsResized = true;
                pFrame->Resize(lW, lH, bIsDestroy);
            }

            std::wstring strSave = m_strDstMedia + L"/image" + std::to_wstring(oInfo.m_lID) + ((itJPG == oInfo.m_eType) ? L".jpg" : L".png");
            pFrame->SaveFile(strSave, (itJPG == oInfo.m_eType) ? 3 : 4);

            if (!bIsDestroy && !bIsResized)
                pFrame->put_Data(NULL);
        }

        inline void WriteImage2(NSHtmlRenderer::CImageInfo& oID, const double& x, const double& y, const double& w, const double& h)
        {
            SetTransformToDocument(true);

            if (fabs(m_pTransform->shx()) < 0.0000001 && fabs(m_pTransform->shy()) < 0.0000001 &&
                m_pTransform->sx() >= 0 && m_pTransform->sy() >= 0)
            {
                double xx = x;
                double yy = y;
                double rr = x + w;
                double bb = y + h;
                m_pTransform->TransformPoint(xx, yy);
                m_pTransform->TransformPoint(rr, bb);

                if (oID.m_eType == itJPG)
                {
                    m_oPage.WriteCommandType(CMetafile::ctDrawImage);
                    m_oPage.WriteBYTE(0);
                    m_oPage.WriteLONG(oID.m_lID);
                    m_oPage.WriteDouble(xx);
                    m_oPage.WriteDouble(yy);
                    m_oPage.WriteDouble(rr - xx);
                    m_oPage.WriteDouble(bb - yy);
                }
                else
                {
                    m_oPage.WriteCommandType(CMetafile::ctDrawImage);
                    m_oPage.WriteBYTE(1);
                    m_oPage.WriteLONG(oID.m_lID);
                    m_oPage.WriteDouble(xx);
                    m_oPage.WriteDouble(yy);
                    m_oPage.WriteDouble(rr - xx);
                    m_oPage.WriteDouble(bb - yy);
                }
            }
            else
            {
                if (oID.m_eType == itJPG)
                {
                    m_oPage.WriteCommandType(CMetafile::ctDrawImage);
                    m_oPage.WriteBYTE(10);
                    m_oPage.WriteLONG(oID.m_lID);
                    m_oPage.WriteDouble(x);
                    m_oPage.WriteDouble(y);
                    m_oPage.WriteDouble(w);
                    m_oPage.WriteDouble(h);

                    m_oPage.WriteDouble(m_pTransform->sx());
                    m_oPage.WriteDouble(m_pTransform->shy());
                    m_oPage.WriteDouble(m_pTransform->shx());
                    m_oPage.WriteDouble(m_pTransform->sy());
                    m_oPage.WriteDouble(m_pTransform->tx());
                    m_oPage.WriteDouble(m_pTransform->ty());
                }
                else
                {
                    m_oPage.WriteCommandType(CMetafile::ctDrawImage);
                    m_oPage.WriteBYTE(11);
                    m_oPage.WriteLONG(oID.m_lID);
                    m_oPage.WriteDouble(x);
                    m_oPage.WriteDouble(y);
                    m_oPage.WriteDouble(w);
                    m_oPage.WriteDouble(h);

                    m_oPage.WriteDouble(m_pTransform->sx());
                    m_oPage.WriteDouble(m_pTransform->shy());
                    m_oPage.WriteDouble(m_pTransform->shx());
                    m_oPage.WriteDouble(m_pTransform->sy());
                    m_oPage.WriteDouble(m_pTransform->tx());
                    m_oPage.WriteDouble(m_pTransform->ty());
                }
            }
        }

        inline void WriteImagePattrern(NSHtmlRenderer::CImageInfo& oID, CTileInfo& oTile)
        {
            SetTransformToDocument(true);
            double dKoef = 25.4 / m_dDpiX;

            double xx = oTile.bbox_x * dKoef;
            double yy = oTile.bbox_y * dKoef;
            double rr = oTile.bbox_r * dKoef;
            double bb = oTile.bbox_b * dKoef;

            if (oID.m_eType == itJPG)
            {
                m_oPage.WriteCommandType(CMetafile::ctDrawImage);
                m_oPage.WriteBYTE(0);
                m_oPage.WriteLONG(oID.m_lID);
                m_oPage.WriteDouble(xx);
                m_oPage.WriteDouble(yy);
                m_oPage.WriteDouble(rr - xx);
                m_oPage.WriteDouble(bb - yy);
            }
            else
            {
                m_oPage.WriteCommandType(CMetafile::ctDrawImage);
                m_oPage.WriteBYTE(1);
                m_oPage.WriteLONG(oID.m_lID);
                m_oPage.WriteDouble(xx);
                m_oPage.WriteDouble(yy);
                m_oPage.WriteDouble(rr - xx);
                m_oPage.WriteDouble(bb - yy);
            }
        }

        void WriteImageID_SVG()
        {
            m_oSmartText.DumpLine();
            SetTransformToDocument(true);

#if 0
            m_oSVGWriter.DEBUG_DumpSVG(m_strDstMedia);
#endif

            bool bIsBIG = false;

#ifdef USE_SIMPLE_GRAPHICS_NOSVG

            if (m_oSVGWriter.m_bIsSimpleGraphics)
            {
                if (m_oSVGWriter.m_oVectors.GetPosition() > SVG_TO_RASTER_MIN_SIZE)
                    bIsBIG = true;

                if (!bIsBIG)
                {
                    m_oPage.Write(m_oSVGWriter.m_oVectors);
                    m_oSVGWriter.NewSVG();
                    return;
                }
            }

#endif

            if (!bIsBIG)
            {
                if (m_oSVGWriter.m_oDocument.GetCurSize() > SVG_TO_RASTER_MIN_SIZE)
                    bIsBIG = true;
            }

            bool bIsNeedWriteEndSVG = true;
            if (bIsBIG)
            {
#ifdef USE_BIG_GRAPHICS_TORASTER
                bool bIsBIGWrite = WriteSVGAsBitmap();
                bIsNeedWriteEndSVG = false;
                if (bIsBIGWrite)
                {
                    m_oSVGWriter.NewSVG();
                    return;
                }
#endif
            }

            if ((m_oSVGWriter.m_oDocument.GetCurSize() > SVG_INLINE_MAX_SIZE))
            {
                ++m_lNextIDImage;

                std::wstring strSaveItem = m_strDstMedia + L"/image" + std::to_wstring(m_lNextIDImage) + L".svg";
                m_oSVGWriter.CloseFile2(strSaveItem, bIsNeedWriteEndSVG);

                m_oPage.WriteCommandType(CMetafile::ctDrawImage);
                m_oPage.WriteBYTE(2);
                m_oPage.WriteLONG(m_lNextIDImage);
            }
            else
            {
                m_oPage.WriteCommandType(CMetafile::ctDrawImage);
                m_oPage.WriteBYTE(3);
                m_oSVGWriter.CloseFile3(&m_oPage, bIsNeedWriteEndSVG);
            }

            m_oSVGWriter.NewSVG();
        }


        bool WriteSVGAsBitmap()
        {
            // TODO:
            return true;
        }

        inline void SetTransformToDocument(bool bIsFromGraphics)
        {
            if (NULL == m_pTransform)
                return;

            if (bIsFromGraphics)
            {
                if (m_oLastTransform.IsIdentity2(__g_matrix_eps))
                {
                    // ничего делать не нужно
                }
                else
                {
                    m_oPage.WriteCommandType(CMetafile::ctResetTransform);
                    m_oLastTransform.Reset();
                }
            }
            else
            {
                if (true)
                {
                    m_oLastTransform = *m_pTransform;

                    //m_oPage.Write(CMetafile::ctSetTransform, m->sx, m->shy, m->shx, m->sy, m->tx, m->ty);
                    m_oPage.WriteCommandType(CMetafile::ctSetTransform);
                    m_oPage.WriteDouble(m_pTransform->sx());
                    m_oPage.WriteDouble(m_pTransform->shy());
                    m_oPage.WriteDouble(m_pTransform->shx());
                    m_oPage.WriteDouble(m_pTransform->sy());
                    m_oPage.WriteDouble(m_pTransform->tx());
                    m_oPage.WriteDouble(m_pTransform->ty());
                    return;
                }

                if (Aggplus::CMatrix::IsEqual(m_pTransform, &m_oLastTransform, __g_matrix_eps, false))
                {
                    // ничего делать не нужно
                    return;
                }

                if (m_pTransform->IsIdentity2(__g_matrix_eps) && m_oLastTransform.IsIdentity2(__g_matrix_eps))
                {
                    // ничего делать не нужно
                    m_oLastTransform.Reset();
                }
                else
                {
                    if (m_pTransform->IsIdentity2(__g_matrix_eps))
                    {
                        // ничего делать не нужно
                        m_oPage.WriteCommandType(CMetafile::ctResetTransform);
                        m_oLastTransform.Reset();
                        return;
                    }

                    m_oLastTransform = *m_pTransform;

                    //m_oPage.Write(CMetafile::ctSetTransform, m->sx, m->shy, m->shx, m->sy, m->tx, m->ty);
                    m_oPage.WriteCommandType(CMetafile::ctSetTransform);
                    m_oPage.WriteDouble(m_pTransform->sx());
                    m_oPage.WriteDouble(m_pTransform->shy());
                    m_oPage.WriteDouble(m_pTransform->shx());
                    m_oPage.WriteDouble(m_pTransform->sy());
                    m_oPage.WriteDouble(m_pTransform->tx());
                    m_oPage.WriteDouble(m_pTransform->ty());
                }
            }
        }

        /////////////////////////////////////////////////////
        void WriteStartDocument()
        {
            m_bIsGids = false;
        }

        void WriteEndDocument(CDocument& oDocument, bool bIsNoBase64)
        {
            CMetafile oDocInfo;
            oDocInfo.WriteLONG(m_lPagesCount);
            oDocInfo.WriteLONG(m_oSmartText.m_lCountParagraphs);
            oDocInfo.WriteLONG(m_oSmartText.m_lCountWords);
            oDocInfo.WriteLONG(m_oSmartText.m_lCountSymbols);
            oDocInfo.WriteLONG(m_oSmartText.m_lCountSpaces);

            oDocInfo.WriteLONG(m_oDstFontGenerator.m_lCountFonts);

            LONG lOffset = (6 + 4 * m_lPagesCount) * sizeof(int);
            for (LONG i = 0; i < m_lPagesCount; i++)
            {
                CPageMeta& oMeta = m_arrPages[i];
                oDocInfo.WriteDouble(oMeta.m_dWidth);
                oDocInfo.WriteDouble(oMeta.m_dHeight);
                oDocInfo.WriteLONG(oMeta.m_lStart + lOffset);
                oDocInfo.WriteLONG(oMeta.m_lEnd + lOffset);
            }

            m_oFileWriter.CloseFile();

            NSFile::CFileBinary oFilePages;
            oFilePages.OpenFile(m_strDstDirectoryFiles + L"/document_temp.bin");

            LONG lMetaSize = (LONG)oFilePages.GetFileSize();
            LONG lSizeAll = oDocInfo.GetPosition() + lMetaSize;

            BYTE* pData = new BYTE[lSizeAll];
            memcpy(pData, oDocInfo.GetData(), oDocInfo.GetPosition());

            DWORD dwRead = 0;
            oFilePages.ReadFile(pData + oDocInfo.GetPosition(), lMetaSize, dwRead);

            oFilePages.CloseFile();

            NSFile::CFileBinary::Remove(m_strDstDirectoryFiles + L"/document_temp.bin");

            if (bIsNoBase64)
            {
                NSFile::CFileBinary oDestinationFile;
                oDestinationFile.CreateFileW(m_strDstDirectoryFiles + L"/Editor.bin");
                oDestinationFile.WriteFile(pData, (DWORD)lSizeAll);
                oDestinationFile.CloseFile();

                RELEASEARRAYOBJECTS(pData);
            }
            else
            {
                char* pOutput = NULL;
                int nOutputLen = 0;
                NSFile::CBase64Converter::Encode(pData, lSizeAll, pOutput, nOutputLen, NSBase64::B64_BASE64_FLAG_NOCRLF);

                RELEASEARRAYOBJECTS(pData);

                std::string sDstLen = std::to_string(nOutputLen);
                sDstLen += ";";

                NSFile::CFileBinary _file;
                _file.CreateFileW(m_strDstDirectoryFiles + L"/Editor.bin");

                _file.WriteFile((BYTE*)sDstLen.c_str(), (DWORD)sDstLen.length());
                _file.WriteFile((BYTE*)pOutput, (DWORD)nOutputLen);

                _file.CloseFile();

                RELEASEARRAYOBJECTS(pOutput);
            }

            NSFonts::IFontManager* pFontManager = m_pApplicationFonts->GenerateFontManager();
            m_oDstFontGenerator.WriteFonts(pFontManager, m_strDstDirectoryFiles + L"/fonts", m_bIsGids);
            RELEASEOBJECT(pFontManager);
        }

        void CreateFile(std::wstring& strDir, std::wstring& strFileName)
        {
            m_lPagesCount		= 0;
            m_lCurrentPage		= -1;

            m_strDstDirectory		= strDir;
            m_strDstDirectoryFiles	= m_strDstDirectory + L"/" + strFileName;// + _T("_files");
            m_strFileName			= strFileName;

            if (!m_bIsOnlyTextMode)
                NSDirectory::CreateDirectory(m_strDstDirectoryFiles);
            m_strDstMedia = m_strDstDirectoryFiles + L"/media";
            if (!m_bIsOnlyTextMode)
                NSDirectory::CreateDirectory(m_strDstMedia);

            std::wstring strFileFonts = m_strDstDirectoryFiles + L"/fonts";
            if (!m_bIsOnlyTextMode)
                NSDirectory::CreateDirectory(strFileFonts);

            std::wstring strDocRendererS = m_strDstDirectoryFiles + L"/document_temp.bin";
            if (!m_bIsOnlyTextMode)
                m_oFileWriter.CreateFileW(strDocRendererS);

            m_oPage.Clear();
            m_arrPages.clear();
            m_bIsGids = false;
            m_lCurrentDumpSize = 0;

            m_oSmartText.SetParams(this);

            m_oSmartText.m_lCountParagraphs = 0;
            m_oSmartText.m_lCountWords = 0;
            m_oSmartText.m_lCountSymbols = 0;
            m_oSmartText.m_lCountSpaces = 0;
        }
    };
}

#endif // _ASC_HTMLRENDERER_WRITER_H_
