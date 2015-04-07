#pragma once

#include "Const.h"
#include "StringWriter.h"

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
		CStringWriter m_oWriterSimpleGraphics;
		CStringWriter m_oWriterVMLPath;
		CStringWriter m_oWriterCanvas;
		CStringWriter m_oWriterVML;

		Graphics::IASCGraphicSimpleComverter* m_pSimpleConverter;

		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;

		CMatrix*				m_pFullTransform;

		double					m_dCurrentMoveToX;
		double					m_dCurrentMoveToY;

		bool					m_bIsSimpleGraphics;

	public:
		BOOL m_bIsSimple;

		BOOL m_bIsClip;
		BOOL m_bIsClipping;

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
			m_oWriterCanvas.WriteString(g_bstr_canvas_StartPath);
		}

		inline void WriteEndPath()
		{
			m_oWriterVMLPath.ClearNoAttack();
			
			m_bIsSimple = TRUE;
			m_lCountSC	= 0;
		}		
		
		void WritePathStart()
		{
			m_oWriterCanvas.WriteString(g_bstr_canvas_StartPath);
		}
		void WritePathClose()
		{
			m_oWriterCanvas.WriteString(g_bstr_canvas_ClosePath);
			
			if (!m_bIsClipping)
			{
				m_oWriterVMLPath.WriteString(g_bstr_vml_ClosePath);
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
						m_bIsSimple = FALSE;
					}
				}
			}
		}

		void WritePathMoveTo(double& x, double& y)
		{
			CString str1 = _T("");
			//str1.Format(g_string_canvas_MoveTo, x, y);
			str1.Format(g_string_canvas_MoveToD, round(x), round(y));
			m_oWriterCanvas.WriteString(str1);

			if (!m_bIsClipping)
			{
				CString str2 = _T("");
				str2.Format(g_string_vml_MoveTo, round(x), round(y));
				m_oWriterVMLPath.WriteString(str2);
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
					m_bIsSimple = FALSE;
				}
			}
		}
		void WritePathLineTo(double& x, double& y)
		{
			if (0 == m_oWriterVMLPath.GetCurSize())
			{
				CString str3 = _T("");
				//str3.Format(g_string_canvas_MoveTo, x, y);
				str3.Format(g_string_canvas_MoveToD, round(x), round(y));
				m_oWriterCanvas.WriteString(str3);
				
				str3 = _T("");
				str3.Format(g_string_vml_MoveTo, round(x), round(y));

				m_oWriterVMLPath.WriteString(str3);
			}
			
			CString str1 = _T("");
			//str1.Format(g_string_canvas_LineTo, x, y);
			str1.Format(g_string_canvas_LineToD, round(x), round(y));
			m_oWriterCanvas.WriteString(str1);

			if (!m_bIsClipping)
			{
				CString str2 = _T("");
				str2.Format(g_string_vml_LineTo, round(x), round(y));
				m_oWriterVMLPath.WriteString(str2);
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
							m_bIsSimple = FALSE;
						}
					}
				}
			}
		}
		void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			if (0 == m_oWriterVMLPath.GetCurSize())
			{
				CString str3 = _T("");
				//str3.Format(g_string_canvas_MoveTo, x1, y1);
				str3.Format(g_string_canvas_MoveToD, round(x1), round(y1));
				m_oWriterCanvas.WriteString(str3);
				
				str3 = _T("");
				str3.Format(g_string_vml_MoveTo, round(x1), round(y1));

				m_oWriterVMLPath.WriteString(str3);
			}
			
			CString str1 = _T("");
			//str1.Format(g_string_canvas_CurveTo, x1, y1, x2, y2, x3, y3);
			str1.Format(g_string_canvas_CurveToD, round(x1), round(y1), round(x2), round(y2), round(x3), round(y3));
			m_oWriterCanvas.WriteString(str1);

			if (!m_bIsClipping)
			{
				CString str2 = _T("");
				str2.Format(g_string_vml_CurveTo, round(x1), round(y1), round(x2), round(y2), round(x3), round(y3));
				m_oWriterVMLPath.WriteString(str2);
			}

			m_bIsSimple = FALSE;
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
							CString strRect = _T("");
							strRect.Format(g_string_rect, 0, strLineColor, strLineColor, (double)m_pPen->Alpha / 255.0, round(x), round(y), round(w), round(h));

							m_oWriterSimpleGraphics.WriteString(strRect);
						}
						else
						{
							if (0x00 == (lType & 0xFF))
							{
								CString strRect = _T("");
								strRect.Format(g_string_rect, 0, strFillColor, strLineColor, (double)m_pBrush->Alpha1 / 255.0, round(x), round(y), round(w), round(h));

								m_oWriterSimpleGraphics.WriteString(strRect);
							}
							else if (lType < 0xFF)
							{
								CString strRect = _T("");
								strRect.Format(g_string_rect, 1, strFillColor, strLineColor, (double)m_pPen->Alpha / 255.0, round(x), round(y), round(w), round(h));

								m_oWriterSimpleGraphics.WriteString(strRect);
							}
							else
							{
								double dAlpha = (double)m_pBrush->Alpha1 / 255.0;

								CString strRect = _T("");
								strRect.Format(g_string_rect, 1, strFillColor, strLineColor, dAlpha, round(x), round(y), round(w), round(h));

								m_oWriterSimpleGraphics.WriteString(strRect);
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
			
			CString strStroked	= _T("false");
			CString strFilled	= _T("false");

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
				CString strStrokeStyle = SetStrokeColor(m_pPen->Color, m_pPen->Alpha);
				
				//CString strStrokeStyle = _T("");
				//strStrokeStyle.Format(g_string_canvas_StrokeColor, ConvertColor(m_pPen->Color));

				m_oWriterCanvas.WriteString(strStrokeStyle);

				bStroke = true;

				if (0x00 != m_pPen->Alpha)
				{
					strStroked = _T("true");
				}
			}
			if (lType > 0x01)
			{
				if (-1 != lIDTx)
				{
					// текстура!
					CString strImage = _T("");

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

					strImage.Format(g_string_canvas_drawImage, lIDTx, lIDTx, lIDTx, lIDTx, lIDTx, round(x), round(y), round(w), round(h), lPageNumber, lCurTxNumber, lPageNumber, lCurTxNumber);
					m_oWriterCanvas.WriteString(strImage);
				}
				else
				{		
					CString strFillStyle = SetFillColor(m_pBrush->Color1, m_pBrush->Alpha1);

					//CString strFillStyle = _T("");
					//strFillStyle.Format(g_string_canvas_FillColor, ConvertColor(m_pBrush->Color1));

					m_oWriterCanvas.WriteString(strFillStyle);
					m_oWriterCanvas.WriteString(g_bstr_canvas_Fill);
				}

				strFilled = _T("true");
			}

			if (bStroke)
			{
				m_oWriterCanvas.WriteString(g_bstr_canvas_Stroke);
			}

			CString str = m_oWriterVMLPath.GetData();
			CString strVML = _T("");
			if (-1 != lIDTx)
			{
				strVML.Format(g_string_shape_vmlTexture, strStroked, ConvertColor(m_pPen->Color), strFilled, str, lIDTx, (double)m_pBrush->Alpha1 / 255);
			}
			else if (0xFF == m_pBrush->Alpha1)
			{
				strVML.Format(g_string_shape_vml, strStroked, ConvertColor(m_pPen->Color), strFilled, ConvertColor(m_pBrush->Color1), str);
			}
			else
			{
				strVML.Format(g_string_shape_vmlAlpha, strStroked, ConvertColor(m_pPen->Color), strFilled, ConvertColor(m_pBrush->Color1), str, (double)m_pBrush->Alpha1 / 255);
			}
			m_oWriterVML.WriteString(strVML);

			if (-1 != lIDTx)
			{
				++lCurTxNumber;
			}
		}

		void WritePathClip()
		{
			m_bIsClipping	= TRUE;
		}
		void WritePathClipEnd()
		{
			m_bIsClipping = FALSE;

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
					m_oWriterCanvas.WriteString(g_bstr_canvas_Save);
				}
				m_bIsClip = TRUE;
				m_oWriterCanvas.WriteString(g_bstr_canvas_Clip);
			}
		}
		void WritePathResetClip()
		{
			if (m_bIsClip)
			{
				m_oWriterCanvas.WriteString(g_bstr_canvas_Restore);
			}

			m_bIsClip		= FALSE;
			m_bIsClipping	= FALSE;
		}

	protected:
		
		bool IsRect(double& x, double& y, double& w, double& h)
		{
			if (1 >= m_lCountSC)
				return false;
			
			double dPrevX	= m_arSimpleCommands[0].m_dX;
			double dPrevY	= m_arSimpleCommands[0].m_dY;

			long lCurDirection = 0;

			CSimpleArray<double> arX;
			CSimpleArray<double> arY;

			arX.Add(dPrevX);
			arY.Add(dPrevY);

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
				
				arX.Add(dX);
				arY.Add(dY);

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

			int lCount = arX.GetSize();
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