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
#ifndef _ASC_HTMLRENDERER_CANVASWRITER_H_
#define _ASC_HTMLRENDERER_CANVASWRITER_H_

#include "Common.h"
#include "../../DesktopEditor/graphics/GraphicsPath.h"

namespace NSHtmlRenderer
{
    class CCanvasWriter
	{
	public:
        NSStringUtils::CStringBuilder	m_oPath;
        NSStringUtils::CStringBuilder	m_oDocument;

		LONG							m_lCurDocumentID;
		LONG							m_lClippingPath;

		bool							m_bIsClipping;
		LONG							m_lClipMode;
		bool							m_bIsMoveTo;

		NSStructures::CPen*				m_pPen;
		NSStructures::CBrush*			m_pBrush;

		double							m_lWidth;
		double							m_lHeight;

		double							m_dDpiX;
		double							m_dDpiY;

	public:
		CCanvasWriter() : m_oPath(), m_oDocument()
		{
			m_lCurDocumentID		= 0;
			m_lClippingPath			= 0;

			m_pPen					= NULL;
			m_pBrush				= NULL;

			m_dDpiX					= 96;
			m_dDpiY					= 96;

			m_lClipMode				= c_nWindingFillMode;

			m_bIsClipping			= false;
			m_bIsMoveTo				= false;
		}

		void SetSettings(NSStructures::CPen* pPen, NSStructures::CBrush* pBrush)
		{
			m_pPen					= pPen;
			m_pBrush				= pBrush;
		}

        void CloseFile(std::wstring strFile = L"")
		{
            if (!strFile.empty())
			{
                NSFile::CFileBinary::SaveToFile(strFile, m_oDocument.GetData());
			}

			m_oDocument.ClearNoAttack();
			m_oPath.ClearNoAttack();
		}
		void NewDocument(double& dWidth, double& dHeigth)
		{
            CloseFile(L"");

			m_lWidth  = (int)dWidth;
			m_lHeight = (int)dHeigth;
		}

	public:

		inline void WritePathEnd()
		{
		}		
		inline void WritePathStart()
		{
			m_bIsMoveTo = false;
            m_oDocument.WriteString(L"b(c);\n", 6);
		}
		void WritePathClose()
		{
            m_oDocument.WriteString(L"x(c);\n", 6);
		}

		void WritePathMoveTo(double& x, double& y)
		{
            m_oDocument.WriteString(L"m(c,", 4);
            WriteIntsToStringBuilder(round(x), round(y), &m_oDocument);
            m_oDocument.WriteString(L");\n", 3);

			m_bIsMoveTo = true;
		}
		void WritePathLineTo(double& x, double& y)
		{
			if (false == m_bIsMoveTo)
				WritePathMoveTo(x, y);

            m_oDocument.WriteString(L"l(c,", 4);
            WriteIntsToStringBuilder(round(x), round(y), &m_oDocument);
            m_oDocument.WriteString(L");\n", 3);
		}
		void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			if (false == m_bIsMoveTo)
				WritePathMoveTo(x1, y1);
			
            m_oDocument.WriteString(L"cu(c,", 5);
            WriteIntsToStringBuilder(round(x1), round(y1), round(x2), round(y2), round(x3), round(y3), &m_oDocument);
            m_oDocument.WriteString(L");\n", 3);
		}
        void WriteDrawPath(LONG lType, Aggplus::CMatrix* pTransform, Aggplus::CGraphicsPathSimpleConverter* pConverter, LONG lTxId)
		{
			bool bStroke = false;

			if (m_pPen->Alpha == 0)
				lType &= 0xFF00;

			if ((-1 == lTxId) && (0 == m_pBrush->Alpha1))
				lType &= 0xFF;
			
			if ((lType & 0x01) == 0x01)
			{
                SetStrokeColor(m_pPen->Color, m_pPen->Alpha, &m_oDocument);

				bStroke = true;
			}
			if (lType > 0x01)
			{
				if (-1 != lTxId)
				{
					// текстура!
					double x = 0;
					double y = 0;
					double w = 0;
					double h = 0;

                    pConverter->PathCommandGetBounds(x, y, w, h);

					double r = x + w;
					double b = y + h;

					pTransform->TransformPoint(x, y);
					pTransform->TransformPoint(r, b);

					w = r - x;
					h = b - y;

                    m_oDocument.WriteString(L"img", 3);
                    m_oDocument.AddInt(lTxId);
                    m_oDocument.WriteString(L".src = \"media\\\\image", 20);
                    m_oDocument.AddInt(lTxId);
                    m_oDocument.WriteString(L".jpg\";img", 9);
                    m_oDocument.AddInt(lTxId);
                    m_oDocument.WriteString(L".onload = function(){c.drawImage(img", 36);
                    WriteIntsToStringBuilder(lTxId, round(x), round(y), round(w), round(h), &m_oDocument);
                    m_oDocument.WriteString(L");drawpage", 10);
                    m_oDocument.AddInt(m_lCurDocumentID);
                    m_oDocument.AddCharSafe('_');
                    m_oDocument.AddInt(lTxId);
                    m_oDocument.WriteString(L"(c);};\n}\nfunction drawpage", 26);
                    m_oDocument.AddInt(m_lCurDocumentID);
                    m_oDocument.AddCharSafe('_');
                    m_oDocument.AddInt(lTxId);
                    m_oDocument.WriteString(L"(c)\n{\n", 6);
				}
				else
				{		
                    SetFillColor(m_pBrush->Color1, m_pBrush->Alpha1, &m_oDocument);
                    m_oDocument.WriteString(L"f(c);\n", 6);
				}
			}

			if (bStroke)
			{
                m_oDocument.WriteString(L"s(c);\n", 6);
			}
		}

		void WritePathClip()
		{
			m_bIsClipping	= true;
			++m_lClippingPath;
		}
		void WritePathClipEnd()
		{
			if (!m_bIsClipping)
			{
                m_oDocument.WriteString(L"c.save();\n", 10);
			}
            m_bIsClipping = true;
            m_oDocument.WriteString(L"c.clip();\n", 10);
		}
		void WritePathResetClip()
		{
			if (m_bIsClipping)
			{
                m_oDocument.WriteString(L"c.restore();\n", 13);
			}

			m_bIsClipping = false;
		}

		inline void WriteStyleClip()
		{
		}
	};
}

#endif // _ASC_HTMLRENDERER_CANVASWRITER_H_
