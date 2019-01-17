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
#ifndef _ASC_HTMLRENDERER_VGW_H_
#define _ASC_HTMLRENDERER_VGW_H_

#include "Common.h"
#include "../../DesktopEditor/graphics/GraphicsPath.h"
#include <vector>

namespace NSHtmlRenderer
{
	// приходится в html разруливать простую графику.
	// (линии (горизонтальные/вектикальные), ректы (не повернутые)).
	// для них не нужно генерить html5-код, который генерит тяжелую картинку
	// и подгружает ее в память

	const LONG g_lSimpleCommandsCountMax = 100;

	class CVectorGraphicsWriter
	{
	public:
		enum SimpleCommand
		{
			scMoveTo	= 0,
			scLineTo	= 1,
			scClose		= 2
		};
		class CSimpleCommand
		{
		public:
			SimpleCommand	m_eType;
			double			m_dX;
			double			m_dY;
		};

	public:
        NSStringUtils::CStringBuilder m_oWriterSimpleGraphics;
        NSStringUtils::CStringBuilder m_oWriterVMLPath;
        NSStringUtils::CStringBuilder m_oWriterCanvas;
        NSStringUtils::CStringBuilder m_oWriterVML;

        Aggplus::CGraphicsPathSimpleConverter* m_pSimpleConverter;

		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;

        Aggplus::CMatrix*		m_pFullTransform;

		double					m_dCurrentMoveToX;
		double					m_dCurrentMoveToY;

		bool					m_bIsSimpleGraphics;

	public:
        bool m_bIsSimple;

        bool m_bIsClip;
        bool m_bIsClipping;

		CSimpleCommand* m_arSimpleCommands;
		LONG m_lCountSC;

	public:
		CVectorGraphicsWriter()	: m_oWriterVML(), m_oWriterCanvas(), m_oWriterSimpleGraphics(), m_oWriterVMLPath()
		{
			m_bIsClip = FALSE;
			m_bIsClipping = FALSE;

			m_bIsSimple = TRUE;

			m_pSimpleConverter	= NULL;

			m_pPen				= NULL;
			m_pBrush			= NULL;

			m_pFullTransform	= NULL;

			m_dCurrentMoveToX	= 0;
			m_dCurrentMoveToY	= 0;

			m_lCountSC			= 0;

			m_bIsSimpleGraphics	= true;

			m_arSimpleCommands = new CSimpleCommand[g_lSimpleCommandsCountMax];
		}
		~CVectorGraphicsWriter()
		{
			RELEASEARRAYOBJECTS(m_arSimpleCommands);
		}

		inline void EndPage()
		{
			m_oWriterSimpleGraphics.ClearNoAttack();
			m_oWriterVMLPath.ClearNoAttack();
			m_oWriterCanvas.ClearNoAttack();
			m_oWriterVML.ClearNoAttack();

			m_bIsSimpleGraphics	= true;
		}
		
		inline void WriteBeginPath()
		{
            m_oWriterCanvas.WriteString(L"b(c);\n", 6);
		}

		inline void WriteEndPath()
		{
			m_oWriterVMLPath.ClearNoAttack();
			
            m_bIsSimple = true;
			m_lCountSC	= 0;
		}		
		
		void WritePathStart()
		{
            m_oWriterCanvas.WriteString(L"b(c);\n", 6);
		}
		void WritePathClose()
		{
            m_oWriterCanvas.WriteString(L"x(c);\n", 6);
			
			if (!m_bIsClipping)
			{
                m_oWriterVMLPath.AddCharSafe('x');
			}

			if (m_bIsSimple)
			{
				if (0 < m_lCountSC)
				{
					m_arSimpleCommands[m_lCountSC].m_eType	= scLineTo;
					m_arSimpleCommands[m_lCountSC].m_dX		= m_dCurrentMoveToX;
					m_arSimpleCommands[m_lCountSC].m_dY		= m_dCurrentMoveToY;

					++m_lCountSC;
					
					if (m_lCountSC == g_lSimpleCommandsCountMax)
					{
                        m_bIsSimple = false;
					}
				}
			}
		}

		void WritePathMoveTo(double& x, double& y)
		{
            m_oWriterCanvas.WriteString(L"m(c,", 4);
            WriteIntsToStringBuilder(round(x), round(y), &m_oWriterCanvas);
            m_oWriterCanvas.WriteString(L");\n", 3);

			if (!m_bIsClipping)
			{
                m_oWriterVMLPath.AddCharSafe('m');
                WriteIntsToStringBuilder(round(x), round(y), &m_oWriterVMLPath);
                m_oWriterVMLPath.AddCharSafe(' ');
			}

			if (m_bIsSimple)
			{
				m_arSimpleCommands[m_lCountSC].m_eType	= scMoveTo;
				m_arSimpleCommands[m_lCountSC].m_dX		= x;
				m_arSimpleCommands[m_lCountSC].m_dY		= y;

				m_dCurrentMoveToX = x;
				m_dCurrentMoveToY = y;

				++m_lCountSC;
				if (m_lCountSC == g_lSimpleCommandsCountMax)
				{
                    m_bIsSimple = false;
				}
			}
		}
		void WritePathLineTo(double& x, double& y)
		{
			if (0 == m_oWriterVMLPath.GetCurSize())
			{
                m_oWriterCanvas.WriteString(L"m(c,", 4);
                WriteIntsToStringBuilder(round(x), round(y), &m_oWriterCanvas);
                m_oWriterCanvas.WriteString(L");\n", 3);

                m_oWriterVMLPath.AddCharSafe('m');
                WriteIntsToStringBuilder(round(x), round(y), &m_oWriterVMLPath);
                m_oWriterVMLPath.AddCharSafe(' ');
			}
			
            m_oWriterCanvas.WriteString(L"l(c,", 4);
            WriteIntsToStringBuilder(round(x), round(y), &m_oWriterCanvas);
            m_oWriterCanvas.WriteString(L");\n", 3);

            if (!m_bIsClipping)
            {
                m_oWriterVMLPath.AddCharSafe('l');
                WriteIntsToStringBuilder(round(x), round(y), &m_oWriterVMLPath);
                m_oWriterVMLPath.AddCharSafe(' ');
            }

			if (m_bIsSimple)
			{
				if (0 == m_lCountSC)
				{
					m_arSimpleCommands[m_lCountSC].m_eType	= scMoveTo;
					m_arSimpleCommands[m_lCountSC].m_dX		= x;
					m_arSimpleCommands[m_lCountSC].m_dY		= y;

					m_dCurrentMoveToX	= x;
					m_dCurrentMoveToY	= y;

					++m_lCountSC;
				}
				else
				{
					double _x = m_arSimpleCommands[m_lCountSC - 1].m_dX;
					double _y = m_arSimpleCommands[m_lCountSC - 1].m_dY;

					if ((FABS(_x - x) < 0.01) || (FABS(_y - y) < 0.01))
					{
						// продолжаем симпл!
						m_arSimpleCommands[m_lCountSC].m_eType	= scLineTo;
						m_arSimpleCommands[m_lCountSC].m_dX		= x;
						m_arSimpleCommands[m_lCountSC].m_dY		= y;

						++m_lCountSC;

						if (m_lCountSC == g_lSimpleCommandsCountMax)
						{
                            m_bIsSimple = false;
						}
					}
				}
			}
		}
		void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			if (0 == m_oWriterVMLPath.GetCurSize())
			{
                m_oWriterCanvas.WriteString(L"m(c,", 4);
                WriteIntsToStringBuilder(round(x), round(y), &m_oWriterCanvas);
                m_oWriterCanvas.WriteString(L");\n", 3);

                m_oWriterVMLPath.AddCharSafe('m');
                WriteIntsToStringBuilder(round(x), round(y), &m_oWriterVMLPath);
                m_oWriterVMLPath.AddCharSafe(' ');
			}
			
            m_oWriterCanvas.WriteString(L"cu(c,", 5);
            WriteIntsToStringBuilder(round(x1), round(y1), round(x2), round(y2), round(x3), round(y3), &m_oWriterCanvas);
            m_oWriterCanvas.WriteString(L");\n", 3);

            if (!m_bIsClipping)
            {
                m_oWriterVMLPath.AddCharSafe('c');
                WriteIntsToStringBuilder(round(x1), round(y1), round(x2), round(y2), round(x3), round(y3), &m_oWriterVMLPath);
                m_oWriterVMLPath.AddCharSafe(' ');
            }

            m_bIsSimple = false;
		}
		void WriteDrawPath(LONG lType, LONG lIDTx, LONG& lCurTxNumber, LONG& lPageNumber)
		{
			if (m_bIsSimple && (-1 == lIDTx))
			{
				// пишем по-крутому
				double x = 0;
				double y = 0;
				double w = 0;
				double h = 0;

				bool bIsRect = IsRect(x, y, w, h);

				if (bIsRect)
				{
					if ((0 == w && 0 == h))
						return;
					
					w = max(1, w);
					h = max(1, h);

                    if (bIsRect)
					{
						CString strLineColor = GetStringColor(m_pPen->Color);
						CString strFillColor = GetStringColor(m_pBrush->Color1);

						if (0 == m_pBrush->Alpha1)
							strFillColor = _T("transparent");
						
						if ((1 >= w) || (1 >= h) && (lType < 0xFF))
						{
                            m_oWriterSimpleGraphics.WriteString(L"<div class=\"rt\" style=\"border-width: 0px; background-color: ", 60);
                            SetStringColor(m_pPen->Color, &m_oWriterSimpleGraphics);
                            m_oWriterSimpleGraphics.WriteString(L"; border-color: ", 16);
                            SetStringColor(m_pPen->Color, &m_oWriterSimpleGraphics);
                            m_oWriterSimpleGraphics.WriteString(L"; opacity:", 10);
                            m_oWriterSimpleGraphics.AddIntNoCheckDel100(100 * m_pPen->Alpha / 255);
                            m_oWriterSimpleGraphics.AddSize(100);
                            m_oWriterSimpleGraphics.WriteString(L"; left: ", 8);
                            m_oWriterSimpleGraphics.AddIntNoCheck(round(x));
                            m_oWriterSimpleGraphics.WriteString(L"px; top: ", 9);
                            m_oWriterSimpleGraphics.AddIntNoCheck(round(y));
                            m_oWriterSimpleGraphics.WriteString(L"px; width: ", 11);
                            m_oWriterSimpleGraphics.AddIntNoCheck(round(w));
                            m_oWriterSimpleGraphics.WriteString(L"px; height: ", 12);
                            m_oWriterSimpleGraphics.AddIntNoCheck(round(h));
                            m_oWriterSimpleGraphics.WriteString(L"px;\"></div>\n");
						}
						else
						{
							if (0x00 == (lType & 0xFF))
							{
                                m_oWriterSimpleGraphics.WriteString(L"<div class=\"rt\" style=\"border-width: 0px; background-color: ", 60);
                                if (0 == m_pBrush->Alpha1)
                                    m_oWriterSimpleGraphics.WriteString(L"transparent", 11);
                                else
                                    SetStringColor(m_pBrush->Color1, &m_oWriterSimpleGraphics);
                                m_oWriterSimpleGraphics.WriteString(L"; border-color: ", 16);
                                SetStringColor(m_pPen->Color, &m_oWriterSimpleGraphics);
                                m_oWriterSimpleGraphics.WriteString(L"; opacity:", 10);
                                m_oWriterSimpleGraphics.AddIntNoCheckDel100(100 * m_pBrush->Alpha1 / 255);
                                m_oWriterSimpleGraphics.AddSize(100);
                                m_oWriterSimpleGraphics.WriteString(L"; left: ", 8);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(x));
                                m_oWriterSimpleGraphics.WriteString(L"px; top: ", 9);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(y));
                                m_oWriterSimpleGraphics.WriteString(L"px; width: ", 11);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(w));
                                m_oWriterSimpleGraphics.WriteString(L"px; height: ", 12);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(h));
                                m_oWriterSimpleGraphics.WriteString(L"px;\"></div>\n");
							}
							else if (lType < 0xFF)
							{
                                m_oWriterSimpleGraphics.WriteString(L"<div class=\"rt\" style=\"border-width: 1px; background-color: ", 60);
                                if (0 == m_pBrush->Alpha1)
                                    m_oWriterSimpleGraphics.WriteString(L"transparent", 11);
                                else
                                    SetStringColor(m_pBrush->Color1, &m_oWriterSimpleGraphics);
                                m_oWriterSimpleGraphics.WriteString(L"; border-color: ", 16);
                                SetStringColor(m_pPen->Color, &m_oWriterSimpleGraphics);
                                m_oWriterSimpleGraphics.WriteString(L"; opacity:", 10);
                                m_oWriterSimpleGraphics.AddIntNoCheckDel100(100 * m_pPen->Alpha / 255);
                                m_oWriterSimpleGraphics.AddSize(100);
                                m_oWriterSimpleGraphics.WriteString(L"; left: ", 8);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(x));
                                m_oWriterSimpleGraphics.WriteString(L"px; top: ", 9);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(y));
                                m_oWriterSimpleGraphics.WriteString(L"px; width: ", 11);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(w));
                                m_oWriterSimpleGraphics.WriteString(L"px; height: ", 12);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(h));
                                m_oWriterSimpleGraphics.WriteString(L"px;\"></div>\n");
							}
							else
							{
                                m_oWriterSimpleGraphics.WriteString(L"<div class=\"rt\" style=\"border-width: 1px; background-color: ", 60);
                                if (0 == m_pBrush->Alpha1)
                                    m_oWriterSimpleGraphics.WriteString(L"transparent", 11);
                                else
                                    SetStringColor(m_pBrush->Color1, &m_oWriterSimpleGraphics);
                                m_oWriterSimpleGraphics.WriteString(L"; border-color: ", 16);
                                SetStringColor(m_pPen->Color, &m_oWriterSimpleGraphics);
                                m_oWriterSimpleGraphics.WriteString(L"; opacity:", 10);
                                m_oWriterSimpleGraphics.AddIntNoCheckDel100(100 * m_pBrush->Alpha1 / 255);
                                m_oWriterSimpleGraphics.AddSize(100);
                                m_oWriterSimpleGraphics.WriteString(L"; left: ", 8);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(x));
                                m_oWriterSimpleGraphics.WriteString(L"px; top: ", 9);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(y));
                                m_oWriterSimpleGraphics.WriteString(L"px; width: ", 11);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(w));
                                m_oWriterSimpleGraphics.WriteString(L"px; height: ", 12);
                                m_oWriterSimpleGraphics.AddIntNoCheck(round(h));
                                m_oWriterSimpleGraphics.WriteString(L"px;\"></div>\n");
							}
						}
					}
				}
				else
				{
					m_bIsSimpleGraphics = false;
				}
			}
			else
			{
				m_bIsSimpleGraphics = false;
			}
			
            std::wstring strStroked	= L"false";
            std::wstring strFilled	= L"false";

            //if (0x00 != (0x02 & (lType >> 8)) && ((lType & 0xFF) == 0x00))
			//{
			//	lType = 1;
			//	m_pPen->Color = m_pBrush->Color1;
			//	m_pPen->Alpha = m_pBrush->Alpha1;
			//}

			bool bStroke = false;

			if (m_pPen->Alpha == 0)
				lType &= 0xFF00;

			if ((-1 == lIDTx) && (0 == m_pBrush->Alpha1))
				lType &= 0xFF;
			
			// canvas
			if ((lType & 0x01) == 0x01)
			{
                SetStrokeColor(m_pPen->Color, m_pPen->Alpha, &m_oWriterCanvas);

				bStroke = true;

				if (0x00 != m_pPen->Alpha)
				{
                    strStroked = L"true";
				}
			}
			if (lType > 0x01)
			{
				if (-1 != lIDTx)
				{
					double x = 0;
					double y = 0;
					double w = 0;
					double h = 0;

					m_pSimpleConverter->PathCommandGetBounds(&x, &y, &w, &h);

					double r = x + w;
					double b = y + h;

					m_pFullTransform->TransformPoint(x, y);
					m_pFullTransform->TransformPoint(r, b);

					w = r - x;
					h = b - y;

                    m_oWriterCanvas.WriteString(L"img", 3);
                    m_oWriterCanvas.AddInt(lIDTx);
                    m_oWriterCanvas.WriteString(L".src = \"media\\\\image", 20);
                    m_oWriterCanvas.AddInt(lIDTx);
                    m_oWriterCanvas.WriteString(L".jpg\";img", 9);
                    m_oWriterCanvas.AddInt(lIDTx);
                    m_oWriterCanvas.WriteString(L".onload = function(){c.drawImage(img", 36);
                    WriteIntsToStringBuilder(lIDTx, round(x), round(y), round(w), round(h), &m_oWriterCanvas);
                    m_oWriterCanvas.WriteString(L");drawpage", 10);
                    m_oWriterCanvas.AddInt(lPageNumber);
                    m_oWriterCanvas.AddCharSafe('_');
                    m_oWriterCanvas.AddInt(lCurTxNumber);
                    m_oWriterCanvas.WriteString(L"(c);};\n}\nfunction drawpage", 26);
                    m_oWriterCanvas.AddInt(lPageNumber);
                    m_oWriterCanvas.AddCharSafe('_');
                    m_oWriterCanvas.AddInt(lCurTxNumber);
                    m_oWriterCanvas.WriteString(L"(c)\n{\n", 6);
				}
				else
				{		
                    SetFillColor(m_pBrush->Color1, m_pBrush->Alpha1, &m_oWriterCanvas);
                    m_oWriterCanvas.WriteString(L"f(c);\n", 6);
				}

                strFilled = L"true";
			}

			if (bStroke)
			{
                m_oWriterCanvas.WriteString(L"s(c);\n", 6);
			}

            if (-1 != lIDTx)
			{
                m_oWriterVML.WriteString(L"<v:shape style=\"position:absolute; width:1; height:1\" coordsize=\"1 1\" stroked=\"", 79);
                m_oWriterVML.WriteString(strStroked);
                m_oWriterVML.WriteString(L"\" strokecolor=\"", 15);
                m_oWriterVML.WriteHexColor3(ConvertColor(m_pPen->Color));
                m_oWriterVML.WriteString(L"\" filled=\"", 10);
                m_oWriterVML.WriteString(strFilled);
                m_oWriterVML.WriteString(L"\" path=\"", 8);
                m_oWriterVML.Write(m_oWriterVMLPath);
                m_oWriterVML.WriteString(L"\"><v:fill src=\"media\\image", 26);
                m_oWriterVML.AddInt(lIDTx);
                m_oWriterVML.WriteString(L".jpg\" type=\"frame\" opacity=\"", 28);
                m_oWriterVML.AddIntDel100(100 * m_pBrush->Alpha1 / 255);
                m_oWriterVML.WriteString(L"\"/></v:shape>\n", 14);

			}
			else if (0xFF == m_pBrush->Alpha1)
			{
                m_oWriterVML.WriteString(L"<v:shape style=\"position:absolute; width:1; height:1\" coordsize=\"1 1\" stroked=\"", 79);
                m_oWriterVML.WriteString(strStroked);
                m_oWriterVML.WriteString(L"\" strokecolor=\"", 15);
                m_oWriterVML.WriteHexColor3(ConvertColor(m_pPen->Color));
                m_oWriterVML.WriteString(L"\" filled=\"", 10);
                m_oWriterVML.WriteString(strFilled);
                m_oWriterVML.WriteString(L"\" fillcolor=\"", 13);
                m_oWriterVML.WriteHexColor3(ConvertColor(m_pBrush->Color1));
                m_oWriterVML.WriteString(L"\" path=\"", 8);
                m_oWriterVML.Write(m_oWriterVMLPath);
                m_oWriterVML.WriteString(L"\" />\n", 5);
			}
			else
			{
                m_oWriterVML.WriteString(L"<v:shape style=\"position:absolute; width:1; height:1\" coordsize=\"1 1\" stroked=\"", 79);
                m_oWriterVML.WriteString(strStroked);
                m_oWriterVML.WriteString(L"\" strokecolor=\"", 15);
                m_oWriterVML.WriteHexColor3(ConvertColor(m_pPen->Color));
                m_oWriterVML.WriteString(L"\" filled=\"", 10);
                m_oWriterVML.WriteString(strFilled);
                m_oWriterVML.WriteString(L"\" fillcolor=\"", 13);
                m_oWriterVML.WriteHexColor3(ConvertColor(m_pBrush->Color1));
                m_oWriterVML.WriteString(L"\" path=\"", 8);
                m_oWriterVML.Write(m_oWriterVMLPath);
                m_oWriterVML.WriteString(L"\"><v:fill opacity=\"", 19);
                m_oWriterVML.AddIntDel100(100 * m_pBrush->Alpha1 / 255);
                m_oWriterVML.WriteString(L"\"/></v:shape>\n", 14);
			}
			m_oWriterVML.WriteString(strVML);

			if (-1 != lIDTx)
			{
				++lCurTxNumber;
			}
		}

		void WritePathClip()
		{
            m_bIsClipping	= true;
		}
		void WritePathClipEnd()
		{
            m_bIsClipping = false;

			double x = 0;
			double y = 0;
			double w = 0;
			double h = 0;

			m_pSimpleConverter->PathCommandGetBounds(&x, &y, &w, &h);
			if ((0 > w) || (0 > h))
			{
				// никакого клипа нет!
				return;
			}
			else
			{
				if (!m_bIsClip)
				{
                    m_oWriterCanvas.WriteString(L"c.save();\n", 10);
				}
                m_bIsClip = true;
                m_oWriterCanvas.WriteString(L"c.clip();\n", 10);
			}
		}
		void WritePathResetClip()
		{
			if (m_bIsClip)
			{
                m_oWriterCanvas.WriteString(L"c.restore();\n", 13);
			}

            m_bIsClip		= false;
            m_bIsClipping	= false;
		}

	protected:
		
		bool IsRect(double& x, double& y, double& w, double& h)
		{
			if (1 >= m_lCountSC)
				return false;
			
			double dPrevX	= m_arSimpleCommands[0].m_dX;
			double dPrevY	= m_arSimpleCommands[0].m_dY;

			long lCurDirection = 0;

            std::vector<double> arX;
            std::vector<double> arY;

            arX.push_back(dPrevX);
            arY.push_back(dPrevY);

			double dXmin = dPrevX;
			double dXmax = dPrevX;
			double dYmin = dPrevY;
			double dYmax = dPrevY;

			double _dX = m_arSimpleCommands[m_lCountSC - 1].m_dX;
			double _dY = m_arSimpleCommands[m_lCountSC - 1].m_dY;

			for (LONG i = 1; i < m_lCountSC; ++i)
			{
				CSimpleCommand* pComm = &m_arSimpleCommands[i];

				if (scLineTo != pComm->m_eType)
					return false;

				long lDir = SkipCurDirection(lCurDirection, i, dPrevX, dPrevY);
				
				double dX = _dX;
				double dY = _dY;
				
				if (i < m_lCountSC)
				{
					dX = m_arSimpleCommands[i - 1].m_dX;
					dY = m_arSimpleCommands[i - 1].m_dY;
				}
				
				if (dX > dXmax) 
					dXmax = dX;
				else if (dX < dXmin)
					dXmin = dX;

				if (dY > dYmax)
					dYmax = dY;
				else if (dY < dYmin)
					dYmin = dY;
				
                arX.push_back(dX);
                arY.push_back(dY);

				lCurDirection = lDir;
			}

			// все, массивы заполнены.
			// теперь осталось определить, рект ли это
			// должно: 
			// 1) кроме минимумов и максимумов ничего не может быть.
			// 2) все точки ректа должны присутствовать

			bool b1 = false;
			bool b2 = false;
			bool b3 = false;
			bool b4 = false;

            int lCount = (int)arX.size();
			for (int i = 0; i < lCount; ++i)
			{
				double ___x = arX[i];
				double ___y = arY[i];
				bool bBreak = false;

				if (IsEqualPoint(___x, ___y, dXmin, dYmin))
					b1 = true;
				else if (IsEqualPoint(___x, ___y, dXmax, dYmin))
					b2 = true;
				else if (IsEqualPoint(___x, ___y, dXmin, dYmax))
					b4 = true;
				else if (IsEqualPoint(___x, ___y, dXmax, dYmax))
					b3 = true;
				else
					return false;
			}

			x = dXmin;
			y = dYmin;
			w = dXmax - dXmin;
			h = dYmax - dYmin;

			if (b1 && b2 && !b3 && !b4)
				return true;
			if (b1 && !b2 && !b3 && b4)
				return true;
			if (!b1 && b2 && b3 && !b4)
				return true;
			if (!b1 && !b2 && b3 && b4)
				return true;
			if (b1 && b2 && b3 && b4)
				return true;

			return false;
		}

		// direction: (0 - вправо, 1 - вниз, 2 - влево, 3 - вверх)
		inline long SkipCurDirection(const long& lDirection, long& lIndexCur, double& dPrevX, double& dPrevY)
		{
			for (; lIndexCur < m_lCountSC; ++lIndexCur)
			{
				long lDir = GetCurDirection(lIndexCur, dPrevX, dPrevY);

				dPrevX = m_arSimpleCommands[lIndexCur].m_dX;
				dPrevY = m_arSimpleCommands[lIndexCur].m_dY;

				if ((-1 == lDir) || (lDir == lDirection))
					continue;

				return lDir;
			}
			return -1;
		}
		inline long GetCurDirection(const long& lIndex, const double& dPrevX, const double& dPrevY)
		{
			double x = m_arSimpleCommands[lIndex].m_dX;
			double y = m_arSimpleCommands[lIndex].m_dY;
						
			if (FABS(y - dPrevY) < 0.01)
			{
				if (FABS(x - dPrevX) < 0.01)
				{
					return -1;
				}

				return (x >= dPrevX) ? 0 : 2;
			}
			else 
			{
				return (y >= dPrevY) ? 1 : 3;
			}
		}

		inline bool IsEqualPoint(double& x, double&y, double& _x, double& _y)
		{
			if ((FABS(x - _x) < 0.1) && (FABS(y - _y) < 0.1))
				return true;

			return false;
		}

		bool IsTwoRectOneDirection()
		{
			return true;
		}

		LONG SkipRect(LONG& lStart, LONG& lEnd)
		{

		}
	};
}

#endif // _ASC_HTMLRENDERER_VGW_H_
