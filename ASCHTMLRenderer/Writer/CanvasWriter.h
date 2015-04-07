#pragma once
#include "..\stdafx.h"
#include "StringWriter.h"

namespace NSHtmlRenderer
{
	static _bstr_t g_canvas_bstr_StartPath		= L"b(c);\n";
	static _bstr_t g_canvas_bstr_ClosePath		= L"x(c);\n";

	static CString g_canvas_string_MoveToD		= _T("m(c,%d,%d);\n");
	static CString g_canvas_string_LineToD		= _T("l(c,%d,%d);\n");
	static CString g_canvas_string_CurveToD		= _T("cu(c,%d,%d,%d,%d,%d,%d);\n");

	static _bstr_t g_canvas_bstr_Stroke			= L"s(c);\n";
	static _bstr_t g_canvas_bstr_Fill			= L"f(c);\n";

	static _bstr_t g_canvas_bstr_Save			= L"c.save();\n";
	static _bstr_t g_canvas_bstr_Restore		= L"c.restore();\n";
	static _bstr_t g_canvas_bstr_Clip			= L"c.clip();\n";

	static CString g_canvas_string_css_color	= _T("rgb(%d,%d,%d)");

	static CString g_canvas_string_FillColor	= _T("c.fillStyle = \"#%06x\";\n");
	static CString g_canvas_string_FillColorA	= _T("c.fillStyle = \"rgba(%d,%d,%d,%.2lf)\";\n");
	static CString g_canvas_string_StrokeColor	= _T("c.strokeStyle = \"#%06x\";\n");
	static CString g_canvas_string_StrokeColorA	= _T("c.strokeStyle = \"rgba(%d,%d,%d,%.2lf)\";\n");
	static CString g_canvas_string_drawImage	= _T("img%d = new Image();img%d.src = \"media\\\\image%d.jpg\";img%d.onload = function(){c.drawImage(img%d,%d,%d,%d,%d);drawpage%d_%d(c);};\n}\nfunction drawpage%d_%d(c)\n{\n");

	class CCanvasWriter
	{
	public:
		CStringWriter					m_oPath;
		CStringWriter					m_oDocument;

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

		void CloseFile(CString strFile = _T(""))
		{
			if (_T("") != strFile)
			{
				NSFile::CFileBinary::SaveToFile(std::wstring(strFile.GetString()), std::wstring(m_oDocument.GetCString().GetString()));
			}

			m_oDocument.ClearNoAttack();
			m_oPath.ClearNoAttack();
		}
		void NewDocument(double& dWidth, double& dHeigth)
		{
			CloseFile(_T(""));

			m_lWidth  = (int)dWidth;
			m_lHeight = (int)dHeigth;
		}

	protected:
		inline static LONG ConvertColor(LONG lBGR)
		{
			return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
		} 
	public:

		inline void WritePathEnd()
		{
		}		
		inline void WritePathStart()
		{
			m_bIsMoveTo = false;
			m_oDocument.WriteString(g_canvas_bstr_StartPath);
		}
		void WritePathClose()
		{
			m_oDocument.WriteString(g_canvas_bstr_ClosePath);
		}

		void WritePathMoveTo(double& x, double& y)
		{
			CString strPath = _T("");
			strPath.Format(g_canvas_string_MoveToD, round(x), round(y));
			m_oDocument.WriteString(strPath);

			m_bIsMoveTo = true;
		}
		void WritePathLineTo(double& x, double& y)
		{
			if (false == m_bIsMoveTo)
				WritePathMoveTo(x, y);

			CString strPath = _T("");
			strPath.Format(g_vml_string_LineTo, round(x), round(y));
			m_oPath.WriteString(strPath);
		}
		void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			if (false == m_bIsMoveTo)
				WritePathMoveTo(x1, y1);
			
			CString strPath = _T("");
			strPath.Format(g_vml_string_CurveTo, round(x1), round(y1), round(x2), round(y2), round(x3), round(y3));
			m_oPath.WriteString(strPath);
		}
		void WriteDrawPath(LONG lType, Aggplus::CMatrix* pTransform, Graphics::IASCGraphicSimpleComverter* pConverter, LONG lTxId)
		{
			bool bStroke = false;

			if (m_pPen->Alpha == 0)
				lType &= 0xFF00;

			if ((-1 == lTxId) && (0 == m_pBrush->Alpha1))
				lType &= 0xFF;
			
			if ((lType & 0x01) == 0x01)
			{
				CString strStrokeStyle = SetStrokeColor(m_pPen->Color, m_pPen->Alpha);
				m_oDocument.WriteString(strStrokeStyle);

				bStroke = true;
			}
			if (lType > 0x01)
			{
				if (-1 != lTxId)
				{
					// текстура!
					CString strImage = _T("");

					double x = 0;
					double y = 0;
					double w = 0;
					double h = 0;

					pConverter->PathCommandGetBounds(&x, &y, &w, &h);

					double r = x + w;
					double b = y + h;

					pTransform->TransformPoint(x, y);
					pTransform->TransformPoint(r, b);

					w = r - x;
					h = b - y;

					strImage.Format(g_canvas_string_drawImage, lTxId, lTxId, lTxId, lTxId, lTxId, round(x), round(y), round(w), round(h), m_lCurDocumentID, lTxId, m_lCurDocumentID, lTxId);
					m_oDocument.WriteString(strImage);
				}
				else
				{		
					CString strFillStyle = SetFillColor(m_pBrush->Color1, m_pBrush->Alpha1);

					m_oDocument.WriteString(strFillStyle);
					m_oDocument.WriteString(g_canvas_bstr_Fill);
				}
			}

			if (bStroke)
			{
				m_oDocument.WriteString(g_canvas_bstr_Stroke);
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
				m_oDocument.WriteString(g_canvas_bstr_Save);
			}
			m_bIsClipping = TRUE;
			m_oDocument.WriteString(g_canvas_bstr_Clip);
		}
		void WritePathResetClip()
		{
			if (m_bIsClipping)
			{
				m_oDocument.WriteString(g_canvas_bstr_Restore);
			}

			m_bIsClipping = false;
		}

		inline void WriteStyleClip()
		{
		}
	};
}