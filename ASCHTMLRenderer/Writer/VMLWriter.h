#pragma once
#include "..\stdafx.h"
#include "StringWriter.h"

namespace NSHtmlRenderer
{
	using namespace NSStrings;

	static _bstr_t g_vml_bstr_ClosePath				= L"x";
	static CString g_vml_string_MoveTo				= _T("m%d,%d ");
	static CString g_vml_string_LineTo				= _T("l%d,%d ");
	static CString g_vml_string_CurveTo				= _T("c%d,%d,%d,%d,%d,%d ");

	static CString g_vml_string_shape_vml			= _T("<v:shape style=\"position:absolute; width:1; height:1\" stroked=\"%s\" strokecolor=\"#%x\" filled=\"%s\" fillcolor=\"#%x\" path=\"%s\" />\n");
	static CString g_vml_string_shape_vml_Dash		= _T("<v:shape style=\"position:absolute; width:1; height:1\" stroked=\"%s\" strokecolor=\"#%x\" filled=\"%s\" fillcolor=\"#%x\" path=\"%s\"><v:stroke dashstyle=\"dot\"/></v:shape>\n");

	static CString g_vml_string_shape_vmlTextureJPG = _T("<v:shape style=\"position:absolute; width:1; height:1\" stroked=\"%s\" strokecolor=\"#%x\" filled=\"%s\" path=\"%s\"><v:fill src=\"media/image%d.jpg\" type=\"frame\" opacity=\"%.2lf\"/></v:shape>\n");
	static CString g_vml_string_shape_vmlTexturePNG = _T("<v:shape style=\"position:absolute; width:1; height:1\" stroked=\"%s\" strokecolor=\"#%x\" filled=\"%s\" path=\"%s\"><v:fill src=\"media/image%d.png\" type=\"frame\" opacity=\"%.2lf\"/></v:shape>\n");
	
	static CString g_vml_string_shape_vmlAlpha		= _T("<v:shape style=\"position:absolute; width:1; height:1\" stroked=\"%s\" strokecolor=\"#%x\" filled=\"%s\" fillcolor=\"#%x\" path=\"%s\"><v:fill opacity=\"%.2lf\"/></v:shape>\n");
	static CString g_vml_string_shape_vmlAlphaDash	= _T("<v:shape style=\"position:absolute; width:1; height:1\" stroked=\"%s\" strokecolor=\"#%x\" filled=\"%s\" fillcolor=\"#%x\" path=\"%s\"><v:fill opacity=\"%.2lf\"/><v:stroke dashstyle=\"dot\"/></v:shape>\n");

	static CString g_vml_string_shape_image_jpg		= _T("<v:image src=\"media/image%d.jpg\" style=\"left:%d;top:%d;width:%d;height:%d;\"/>");
	static CString g_vml_string_shape_image_png		= _T("<v:image src=\"media/image%d.png\" style=\"left:%d;top:%d;width:%d;height:%d;\"/>");

	static CString g_vml_string_document			= _T("<xml xmlns:v=\"urn:schemas-microsoft-com:vml\">\n<v:group id=\"page%d\" style=\"position: absolute; width:1; height:1;\" coordsize=\"1 1\">\n");
	static _bstr_t g_vml_bstr_document_end			= L"</v:group>\n</xml>";

	static CString g_vml_string_frame				= _T("<v:vmlframe clip=\"true\" origin=\"0,0\" size=\"%d,%d\" src=\"page%d.vml#page%d\" unselectable=\"on\"/>\n");


	class CVMLWriter
	{
	public:
		CStringWriter					m_oPath;
		CStringWriter					m_oDocument;

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

		void CloseFile(CString strFile = _T(""))
		{
			if (_T("") != strFile)
			{
				m_oDocument.WriteString(g_vml_bstr_document_end);
				//CDirectory::SaveToFile(strFile, m_oDocument.GetCString());
				NSFile::CFileBinary oFile;
				oFile.CreateFile(std::wstring(strFile.GetString()));
				CStringA strA(m_oDocument.GetBuffer(), (int)m_oDocument.GetCurSize());
				oFile.WriteFile((BYTE*)strA.GetBuffer(), strA.GetLength());
			}

			if (3000000 < m_oDocument.GetSize())
				m_oDocument.Clear();

			m_oDocument.ClearNoAttack();
			m_oPath.ClearNoAttack();
		}
		void NewDocument(double& dWidth, double& dHeigth, LONG& lPageNumber)
		{
			CloseFile(_T(""));

			m_lWidth  = (int)dWidth;
			m_lHeight = (int)dHeigth;

			m_lCurDocumentID = lPageNumber;

			CString strDocumentStart = _T("");
			strDocumentStart.Format(g_vml_string_document, m_lCurDocumentID, (int)m_lWidth, (int)m_lHeight);
		
			m_oDocument.WriteString(strDocumentStart);
		}

	protected:
		inline static LONG ConvertColor(LONG lBGR)
		{
			return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
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
			
			m_oPath.WriteString(g_vml_bstr_ClosePath);
		}

		void WritePathMoveTo(double& x, double& y)
		{
			if (m_bIsClippingNow)
				return;

			CString strPath = _T("");
			strPath.Format(g_vml_string_MoveTo, round(x), round(y));
			m_oPath.WriteString(strPath);
		}
		void WritePathLineTo(double& x, double& y)
		{
			if (m_bIsClippingNow)
				return;

			if (0 == m_oPath.GetCurSize())
			{
				WritePathMoveTo(x, y);
			}
			CString strPath = _T("");
			strPath.Format(g_vml_string_LineTo, round(x), round(y));
			m_oPath.WriteString(strPath);
		}
		void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			if (0 == m_oPath.GetCurSize())
			{
				WritePathMoveTo(x1, y1);
			}
			
			CString strPath = _T("");
			strPath.Format(g_vml_string_CurveTo, round(x1), round(y1), round(x2), round(y2), round(x3), round(y3));
			m_oPath.WriteString(strPath);
		}
		void WriteDrawPath(LONG lType, Aggplus::CMatrix* pTransform, Aggplus::CGraphicsPathSimpleConverter* pConverter, CImageInfo& oInfo, const double& dAngle)
		{
			if (m_oPath.GetCurSize() < 3)
				return;

			CString strStroked	= _T("false");
			CString strFilled	= _T("false");

			bool bStroke = false;

			if (m_pPen->Alpha == 0)
				lType &= 0xFF00;

			if ((-1 == oInfo.m_lID) && (0 == m_pBrush->Alpha1))
				lType &= 0xFF;
			
			// canvas
			if ((lType & 0x01) == 0x01)
			{
				bStroke = true;
				strStroked = _T("true");
			}
			if (lType > 0x01)
			{
				strFilled = _T("true");
			}

			CString strVML = _T("");
			if (-1 != oInfo.m_lID)
			{
				if (itJPG == oInfo.m_eType)
				{
					strVML.Format(g_vml_string_shape_vmlTextureJPG, strStroked, ConvertColor(m_pPen->Color), strFilled, 
						m_oPath.GetCString(), oInfo.m_lID, (double)m_pBrush->TextureAlpha / 255);
				}
				else
				{
					strVML.Format(g_vml_string_shape_vmlTexturePNG, strStroked, ConvertColor(m_pPen->Color), strFilled, 
						m_oPath.GetCString(), oInfo.m_lID, (double)m_pBrush->TextureAlpha / 255);
				}
			}
			else if (0xFF == m_pBrush->Alpha1)
			{
				if (0x00 == (lType & 0xFF) || (0 == m_pPen->DashStyle))
				{
					strVML.Format(g_vml_string_shape_vml, strStroked, ConvertColor(m_pPen->Color), strFilled, 
						ConvertColor(m_pBrush->Color1), m_oPath.GetCString());
				}
				else
				{
					strVML.Format(g_vml_string_shape_vml_Dash, strStroked, ConvertColor(m_pPen->Color), strFilled, 
						ConvertColor(m_pBrush->Color1), m_oPath.GetCString());
				}

			}
			else
			{
				if (0x00 == (lType & 0xFF) || (0 == m_pPen->DashStyle))
				{
					strVML.Format(g_vml_string_shape_vmlAlpha, strStroked, ConvertColor(m_pPen->Color), strFilled, 
						ConvertColor(m_pBrush->Color1), m_oPath.GetCString(), (double)m_pBrush->Alpha1 / 255);
				}
				else
				{
					strVML.Format(g_vml_string_shape_vmlAlphaDash, strStroked, ConvertColor(m_pPen->Color), strFilled, 
						ConvertColor(m_pBrush->Color1), m_oPath.GetCString(), (double)m_pBrush->Alpha1 / 255);
				}
			}
			m_oDocument.WriteString(strVML);
		}

		void WriteImage(CImageInfo oInfo, const double& x, const double& y, const double& w, const double& h, const double& dAngle)
		{
			CString strImage = _T("");
			if (itJPG == oInfo.m_eType)
				strImage.Format(g_vml_string_shape_image_jpg, oInfo.m_lID, round(x), round(y), round(w), round(h));
			else
				strImage.Format(g_vml_string_shape_image_png, oInfo.m_lID, round(x), round(y), round(w), round(h));

			m_oDocument.WriteString(strImage);
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

		void WriteToMainHtml(CStringWriter* pWriter, const CDstInfo& oInfo)
		{
			if (!oInfo.m_bIsWeb)
			{
				CString strFrame = _T("");
				strFrame.Format(g_vml_string_frame, (int)m_lWidth, (int)m_lHeight, m_lCurDocumentID, m_lCurDocumentID);

				pWriter->WriteString(strFrame);

				CString strVml = _T("");
				strVml.Format(_T("\\page%d.vml"), m_lCurDocumentID);
				CloseFile(oInfo.m_strDstFilePath + strVml);
			}
			else
			{
				CString s1 = _T("");
				s1.Format(_T("<v:vmlframe clip=\"true\" origin=\"0,0\" size=\"%d,%d\" src=\""), (int)m_lWidth, (int)m_lHeight);
				
				s1 += oInfo.m_strAdditionalPath;

				CString s2 = _T("");
				s2.Format(_T("/page%d.vml#page%d\" unselectable=\"on\"/>\n"), m_lCurDocumentID, m_lCurDocumentID);
				s1 += s2;

				pWriter->WriteString(s1);
	
				CString strVml = _T("");
				strVml.Format(_T("\\page%d.vml"), m_lCurDocumentID);
				CloseFile(oInfo.m_strDstFilePath + strVml);
			}
		}
	};
}