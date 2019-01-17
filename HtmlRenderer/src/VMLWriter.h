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
#ifndef _ASC_HTMLRENDERER_VMLWRITER_H_
#define _ASC_HTMLRENDERER_VMLWRITER_H_

#include "Common.h"

namespace NSHtmlRenderer
{
    class CVMLWriter
    {
    public:
        NSStringUtils::CStringBuilder	m_oPath;
        NSStringUtils::CStringBuilder	m_oDocument;

        LONG							m_lCurDocumentID;
        LONG							m_lClippingPath;

        bool							m_bIsClipping;
        LONG							m_lClipMode;
        bool							m_bIsClippingNow;

        NSStructures::CPen*				m_pPen;
        NSStructures::CBrush*			m_pBrush;

        double							m_lWidth;
        double							m_lHeight;

        double							m_dDpiX;
        double							m_dDpiY;

    public:
        CVMLWriter() : m_oPath(), m_oDocument()
        {
            m_lCurDocumentID		= 0;
            m_lClippingPath			= 0;

            m_pPen					= NULL;
            m_pBrush				= NULL;

            m_dDpiX					= 96;
            m_dDpiY					= 96;

            m_lClipMode				= c_nWindingFillMode;

            m_bIsClipping			= false;
            m_bIsClippingNow		= false;
        }

        void SetSettings(NSStructures::CPen* pPen, NSStructures::CBrush* pBrush)
        {
            m_pPen					= pPen;
            m_pBrush				= pBrush;
        }

        void CloseFile(std::wstring strFile = L"")
        {
            if (L"" != strFile)
            {
                m_oDocument.WriteString(L"</v:group>\n</xml>", 17);
                NSFile::CFileBinary oFile;
                oFile.CreateFileW(strFile);
                BYTE* pData;
                LONG nDataSize;
                NSFile::CUtf8Converter::GetUtf8StringFromUnicode(m_oDocument.GetBuffer(), m_oDocument.GetCurSize(), pData, nDataSize);
                oFile.WriteFile(pData, nDataSize);
                RELEASEARRAYOBJECTS(pData);
            }

            if (3000000 < m_oDocument.GetSize())
                m_oDocument.Clear();

            m_oDocument.ClearNoAttack();
            m_oPath.ClearNoAttack();
        }
        void NewDocument(double& dWidth, double& dHeigth, LONG& lPageNumber)
        {
            CloseFile(L"");

            m_lWidth  = (int)dWidth;
            m_lHeight = (int)dHeigth;

            m_lCurDocumentID = lPageNumber;

            m_oDocument.AddSize(150);
            m_oDocument.WriteString(L"<xml xmlns:v=\"urn:schemas-microsoft-com:vml\">\n<v:group id=\"page", 63);
            m_oDocument.AddIntNoCheck((int)m_lCurDocumentID);
            m_oDocument.WriteString(L"\" style=\"position: absolute; width:1; height:1;\" coordsize=\"1 1\">\n", 66);
        }

    public:

        inline void WritePathEnd()
        {
            m_oPath.ClearNoAttack();
        }
        inline void WritePathStart()
        {
            m_oPath.ClearNoAttack();
        }
        void WritePathClose()
        {
            if (m_bIsClippingNow)
                return;

            m_oPath.AddCharSafe('x');
        }

        void WritePathMoveTo(double& x, double& y)
        {
            if (m_bIsClippingNow)
                return;

            m_oPath.AddSize(30);
            m_oPath.AddCharNoSafe('m');
            m_oPath.AddIntNoCheck(round(x));
            m_oPath.AddCharNoSafe(',');
            m_oPath.AddIntNoCheck(round(y));
            m_oPath.AddCharNoSafe(' ');
        }
        void WritePathLineTo(double& x, double& y)
        {
            if (m_bIsClippingNow)
                return;

            if (0 == m_oPath.GetCurSize())
            {
                WritePathMoveTo(x, y);
            }
            m_oPath.AddSize(30);
            m_oPath.AddCharNoSafe('l');
            m_oPath.AddIntNoCheck(round(x));
            m_oPath.AddCharNoSafe(',');
            m_oPath.AddIntNoCheck(round(y));
            m_oPath.AddCharNoSafe(' ');
        }
        void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
        {
            if (0 == m_oPath.GetCurSize())
            {
                WritePathMoveTo(x1, y1);
            }

            m_oPath.AddSize(80);
            m_oPath.AddCharNoSafe('c');
            m_oPath.AddIntNoCheck(round(x1));
            m_oPath.AddCharNoSafe(',');
            m_oPath.AddIntNoCheck(round(y1));
            m_oPath.AddCharNoSafe(',');
            m_oPath.AddIntNoCheck(round(x2));
            m_oPath.AddCharNoSafe(',');
            m_oPath.AddIntNoCheck(round(y2));
            m_oPath.AddCharNoSafe(',');
            m_oPath.AddIntNoCheck(round(x3));
            m_oPath.AddCharNoSafe(',');
            m_oPath.AddIntNoCheck(round(y3));
            m_oPath.AddCharNoSafe(' ');
        }
        void WriteDrawPath(LONG lType, Aggplus::CMatrix* pTransform, Aggplus::CGraphicsPathSimpleConverter* pConverter, CImageInfo& oInfo, const double& dAngle)
        {
            if (m_oPath.GetCurSize() < 3)
                return;

            int nStrokeColor = ConvertColor(m_pPen->Color);

            m_oDocument.WriteString(L"<v:shape style=\"position:absolute; width:1; height:1\" stroked=\"", 63);

            bool bStroke = false;
            bool bFilled = false;

            if (m_pPen->Alpha == 0)
                lType &= 0xFF00;

            if ((-1 == oInfo.m_lID) && (0 == m_pBrush->Alpha1))
                lType &= 0xFF;

            // canvas
            if ((lType & 0x01) == 0x01)
            {
                bStroke = true;
            }
            if (lType > 0x01)
            {
                bFilled = true;
            }

            if (bStroke)
                m_oDocument.WriteString(L"true", 4);
            else
                m_oDocument.WriteString(L"false", 5);

            m_oDocument.WriteString(L"\" strokecolor=\"", 15);
            m_oDocument.WriteHexColor3(nStrokeColor);

            m_oDocument.WriteString(L"\" filled=\"", 10);

            if (bFilled)
                m_oDocument.WriteString(L"true", 4);
            else
                m_oDocument.WriteString(L"false", 5);

            m_oDocument.WriteString(L"\" ", 2);

            if (-1 != oInfo.m_lID)
            {
                if (itJPG == oInfo.m_eType)
                {
                    m_oDocument.WriteString(L"path=\"", 6);
                    m_oDocument.Write(m_oPath);
                    m_oDocument.WriteString(L"\"><v:fill src=\"media/image", 26);
                    m_oDocument.AddSize(10);
                    m_oDocument.AddIntNoCheck(oInfo.m_lID);
                    m_oDocument.WriteString(L".jpg\" type=\"frame\" opacity=\"", 28);
                    m_oDocument.AddIntNoCheckDel100((100 * m_pBrush->TextureAlpha) / 255);
                    m_oDocument.WriteString(L"\"/></v:shape>\n", 14);
                }
                else
                {
                    m_oDocument.WriteString(L"path=\"", 6);
                    m_oDocument.Write(m_oPath);
                    m_oDocument.WriteString(L"\"><v:fill src=\"media/image", 26);
                    m_oDocument.AddSize(10);
                    m_oDocument.AddIntNoCheck(oInfo.m_lID);
                    m_oDocument.WriteString(L".png\" type=\"frame\" opacity=\"", 28);
                    m_oDocument.AddIntNoCheckDel100((100 * m_pBrush->TextureAlpha) / 255);
                    m_oDocument.WriteString(L"\"/></v:shape>\n", 14);
                }
            }
            else if (0xFF == m_pBrush->Alpha1)
            {
                if (0x00 == (lType & 0xFF) || (0 == m_pPen->DashStyle))
                {
                    m_oDocument.WriteString(L"fillcolor=\"", 11);
                    m_oDocument.WriteHexColor3(ConvertColor(m_pBrush->Color1));
                    m_oDocument.WriteString(L"\" path=\"", 8);
                    m_oDocument.Write(m_oPath);
                    m_oDocument.WriteString(L"\" />\n", 5);
                }
                else
                {
                    m_oDocument.WriteString(L"fillcolor=\"", 11);
                    m_oDocument.WriteHexColor3(ConvertColor(m_pBrush->Color1));
                    m_oDocument.WriteString(L"\" path=\"", 8);
                    m_oDocument.Write(m_oPath);
                    m_oDocument.WriteString(L"\"><v:stroke dashstyle=\"dot\"/></v:shape>\n", 40);
                }

            }
            else
            {
                if (0x00 == (lType & 0xFF) || (0 == m_pPen->DashStyle))
                {
                    m_oDocument.WriteString(L"fillcolor=\"", 11);
                    m_oDocument.WriteHexColor3(ConvertColor(m_pBrush->Color1));
                    m_oDocument.WriteString(L"\" path=\"", 8);
                    m_oDocument.Write(m_oPath);
                    m_oDocument.WriteString(L"\"><v:fill opacity=\"", 19);
                    m_oDocument.AddIntNoCheckDel100((100 * m_pBrush->Alpha1) / 255);
                    m_oDocument.WriteString(L"\" />\n", 5);
                }
                else
                {
                    m_oDocument.WriteString(L"fillcolor=\"", 11);
                    m_oDocument.WriteHexColor3(ConvertColor(m_pBrush->Color1));
                    m_oDocument.WriteString(L"\" path=\"", 8);
                    m_oDocument.Write(m_oPath);
                    m_oDocument.WriteString(L"\"><v:fill opacity=\"", 19);
                    m_oDocument.AddIntNoCheckDel100((100 * m_pBrush->Alpha1) / 255);
                    m_oDocument.WriteString(L"\"><v:stroke dashstyle=\"dot\"/></v:shape>\n", 40);
                }
            }
        }

        void WriteImage(CImageInfo oInfo, const double& x, const double& y, const double& w, const double& h, const double& dAngle)
        {
            m_oDocument.WriteString(L"<v:image src=\"media/image", 25);
            m_oDocument.AddSize(100);
            m_oDocument.AddIntNoCheck(oInfo.m_lID);
            if (itJPG == oInfo.m_eType)
                m_oDocument.WriteString(L".jpg\" style=\"left:", 18);
            else
                m_oDocument.WriteString(L".png\" style=\"left:", 18);
            m_oDocument.AddIntNoCheck(round(x));
            m_oDocument.WriteString(L";top:", 5);
            m_oDocument.AddIntNoCheck(round(y));
            m_oDocument.WriteString(L";width:", 7);
            m_oDocument.AddIntNoCheck(round(w));
            m_oDocument.WriteString(L";height:", 8);
            m_oDocument.AddIntNoCheck(round(h));
            m_oDocument.WriteString(L";\"/>", 4);
        }

        void WritePathClip()
        {
            m_bIsClipping	= true;
            m_bIsClippingNow = true;
            ++m_lClippingPath;
        }
        void WritePathClipEnd()
        {
            m_bIsClippingNow = false;
        }
        void WritePathResetClip()
        {
            m_bIsClipping = false;
        }

        inline void WriteStyleClip()
        {
        }

        void WriteToMainHtml(NSStringUtils::CStringBuilder* pWriter, const CDstInfo& oInfo)
        {
            if (!oInfo.m_bIsWeb)
            {
                pWriter->WriteString(L"<v:vmlframe clip=\"true\" origin=\"0,0\" size=\"", 43);
                pWriter->AddSize(100);
                pWriter->AddIntNoCheck((int)m_lWidth);
                pWriter->AddCharNoSafe(',');
                pWriter->AddIntNoCheck((int)m_lHeight);
                pWriter->WriteString(L"\" src=\"page", 11);
                pWriter->AddIntNoCheck(m_lCurDocumentID);
                pWriter->WriteString(L".vml#page", 9);
                pWriter->AddIntNoCheck(m_lCurDocumentID);
                pWriter->WriteString(L"\" unselectable=\"on\"/>\n", 22);

                std::wstring sPath = oInfo.m_strDstFilePath + L"\\page" + std::to_wstring(m_lCurDocumentID) + L".vml";
                CloseFile(sPath);
            }
            else
            {
                pWriter->WriteString(L"<v:vmlframe clip=\"true\" origin=\"0,0\" size=\"", 43);
                pWriter->AddSize(100);
                pWriter->AddIntNoCheck((int)m_lWidth);
                pWriter->AddCharNoSafe(',');
                pWriter->AddIntNoCheck((int)m_lHeight);
                pWriter->WriteString(L"\" src=\"", 7);
                pWriter->WriteString(oInfo.m_strAdditionalPath);
                pWriter->WriteString(L"/page", 5);
                pWriter->AddIntNoCheck(m_lCurDocumentID);
                pWriter->WriteString(L".vml#page", 9);
                pWriter->AddIntNoCheck(m_lCurDocumentID);
                pWriter->WriteString(L"\" unselectable=\"on\"/>\n", 22);

                std::wstring sPath = oInfo.m_strDstFilePath + L"\\page" + std::to_wstring(m_lCurDocumentID) + L".vml";
                CloseFile(sPath);
            }
        }
    };
}

#endif // _ASC_HTMLRENDERER_VMLWRITER_H_
