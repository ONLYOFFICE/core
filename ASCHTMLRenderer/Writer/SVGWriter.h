#pragma once
//#include "..\stdafx.h"
#include "StringWriter.h"
#include <vector>

namespace NSHtmlRenderer
{
	using namespace NSStrings;

	static CString g_svg_string_svgOpen				= _T("<svg width=\"%dpx\" height=\"%dpx\" viewBox=\"0 0 %d %d\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n");
	static CString g_svg_bstr_svgClose				= L"</svg>";

	static CString g_svg_string_image				= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"%ls\" preserveAspectRatio=\"none\"/>\n");

	static CString g_svg_bstr_graphicOpen			= L"<g>\n";
	static CString g_svg_bstr_graphicClose			= L"</g>\n";

	static CString g_svg_bstr_path_d				= L" d=\"";
	static CString g_svg_bstr_path_d_end			= L"\" ";
	static CString g_svg_string_clip				= _T("clip-path=\"url(#clip%d)\" ");

	static CString g_svg_string_clipCreate			= _T("<clipPath id=\"clip%d\"><path id=\"path%d\" d=\"");
	static CString g_svg_bstr_clipendWinding		= _T("\" clip-rule=\"nonzero\" /></clipPath>");
	static CString g_svg_bstr_clipendEvenodd		= _T("\" clip-rule=\"evenodd\" /></clipPath>");

	static CString g_svg_string_gclip				= _T("<g clip-path=\"url(#clip%d)\">");

	static CString g_svg_bstr_vml_ClosePath			= L"Z ";
	static CString g_svg_string_vml_MoveTo			= _T("M %d,%d ");
	static CString g_svg_string_vml_LineTo			= _T("L %d,%d ");
	static CString g_svg_string_vml_CurveTo			= _T("C %d,%d %d,%d %d,%d ");
	static CString g_svg_bstr_vml_Path				= L"<path ";
	static CString g_svg_bstr_nodeClose				= L" />\n";
	static CString g_svg_string_vml_StyleStrokeDash = _T("style=\"fill:none;stroke:#%06x;stroke-width:%dpx;stroke-opacity:%.2lf;stroke-dasharray: 2,2;\" ");
	static CString g_svg_string_vml_StyleStroke		= _T("style=\"fill:none;stroke:#%06x;stroke-width:%dpx;stroke-opacity:%.2lf;\" ");
	static CString g_svg_string_vml_StyleFill		= _T("style=\"fill:#%06x;fill-opacity:%.2lf;fill-rule:%ls;stroke:none\" ");
	static CString g_svg_string_vml_Style			= _T("style=\"fill:#%06x;fill-opacity:%.2lf;fill-rule:%ls;stroke:#%06x;stroke-width:%dpx;stroke-opacity:%.2lf\" ");

	static CString g_svg_string_image_clip_jpg		= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" clip-path=\"url(#clip%d)\" xlink:href=\"media/image%d.jpg\" preserveAspectRatio=\"none\"/>");
	static CString g_svg_string_image_clip_jpg_rot	= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" clip-path=\"url(#clip%d)\" xlink:href=\"media/image%d.jpg\" preserveAspectRatio=\"none\" transform=\"rotate(%.4lf, %.2lf,%.2lf)\"/>");
	static CString g_svg_string_image_clip_png		= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" clip-path=\"url(#clip%d)\" xlink:href=\"media/image%d.png\" preserveAspectRatio=\"none\"/>");
	static CString g_svg_string_image_clip_png_rot	= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" clip-path=\"url(#clip%d)\" xlink:href=\"media/image%d.png\" preserveAspectRatio=\"none\" transform=\"rotate(%.4lf, %.2lf,%.2lf)\"/>");

	static CString g_svg_string_image_clip_jpg1		= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" clip-path=\"url(#clip%d)\" xlink:href=\"image%d.jpg\" preserveAspectRatio=\"none\"/>");
	static CString g_svg_string_image_clip_png1		= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" clip-path=\"url(#clip%d)\" xlink:href=\"image%d.png\" preserveAspectRatio=\"none\"/>");

	static CString g_svg_string_image_clip_jpg_mtx	= _T("<image x=\"%.3lf\" y=\"%.3lf\" width=\"%.3lf\" height=\"%.3lf\" clip-path=\"url(#clip%d)\" xlink:href=\"image%d.jpg\" preserveAspectRatio=\"none\" transform=\"matrix(%.3lf, %.3lf,%.3lf,%.3lf, %.3lf,%.3lf)\"/>");
	static CString g_svg_string_image_clip_png_mtx	= _T("<image x=\"%.3lf\" y=\"%.3lf\" width=\"%.3lf\" height=\"%.3lf\" clip-path=\"url(#clip%d)\" xlink:href=\"image%d.png\" preserveAspectRatio=\"none\" transform=\"matrix(%.3lf, %.3lf,%.3lf,%.3lf, %.3lf,%.3lf)\"/>");

	static CString g_svg_string_image_jpg			= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"media/image%d.jpg\" preserveAspectRatio=\"none\"/>");
	static CString g_svg_string_image_jpg_rot		= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"media/image%d.jpg\" preserveAspectRatio=\"none\" transform=\"rotate(%.4lf, %.2lf,%.2lf)\"/>");
	static CString g_svg_string_image_png			= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"media/image%d.png\" preserveAspectRatio=\"none\"/>");
	static CString g_svg_string_image_png_rot		= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"media/image%d.png\" preserveAspectRatio=\"none\" transform=\"rotate(%.4lf, %.2lf,%.2lf)\"/>");

	static CString g_svg_string_image_jpg1			= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"image%d.jpg\" preserveAspectRatio=\"none\"/>");
	static CString g_svg_string_image_png1			= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"image%d.png\" preserveAspectRatio=\"none\"/>");

	static CString g_svg_string_image_jpg_mtx		= _T("<image x=\"%.3lf\" y=\"%.3lf\" width=\"%.3lf\" height=\"%.3lf\" xlink:href=\"image%d.jpg\" preserveAspectRatio=\"none\" transform=\"matrix(%.3lf, %.3lf,%.3lf,%.3lf, %.3lf,%.3lf)\"/>");
	static CString g_svg_string_image_png_mtx		= _T("<image x=\"%.3lf\" y=\"%.3lf\" width=\"%.3lf\" height=\"%.3lf\" xlink:href=\"image%d.png\" preserveAspectRatio=\"none\" transform=\"matrix(%.3lf, %.3lf,%.3lf,%.3lf, %.3lf,%.3lf)\"/>");

	static CString g_svg_string_pattern_png			= _T("<pattern id=\"pt%d\" patternUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\" transform=\"translate(%d,%d);\">\
														 <image x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" xlink:href=\"media/image%d.png\" preserveAspectRatio=\"none\"/></pattern>");
	static CString g_svg_string_pattern_jpg			= _T("<pattern id=\"pt%d\" patternUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\" transform=\"translate(%d,%d);\">\
														 <image x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" xlink:href=\"media/image%d.jpg\" preserveAspectRatio=\"none\"/></pattern>");


	static CString g_svg_string_pattern_png_mtx		= _T("<pattern id=\"pt%d\" patternUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\" transform=\"matrix(%.3lf, %.3lf,%.3lf,%.3lf, %.3lf,%.3lf)\">\
														 <image x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" xlink:href=\"image%d.png\" preserveAspectRatio=\"none\"/></pattern>");
	static CString g_svg_string_pattern_jpg_mtx		= _T("<pattern id=\"pt%d\" patternUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\" transform=\"matrix(%.3lf, %.3lf,%.3lf,%.3lf, %.3lf,%.3lf)\">\
														 <image x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" xlink:href=\"image%d.jpg\" preserveAspectRatio=\"none\"/></pattern>");


	static CString g_svg_string_vml_StyleFillTx		= _T("style=\"fill:url(#pt%d);fill-opacity:%.2lf;fill-rule:%ls;stroke:none;\" ");
	static CString g_svg_string_vml_StyleTx			= _T("style=\"fill:url(#pt%d);fill-opacity:%.2lf;fill-rule:%ls;stroke:#%06x;stroke-width:%dpx;stroke-opacity:%.2lf\" ");

	static CString g_svg_object_start				= _T("<object class=\"_svg\" data=\"page%d.svg\" type=\"image/svg+xml\">");
	static CString g_svg_object_end					= L"</object>";

	static CString g_string_intersect_clip			= _T("<clipPath id=\"clip%d\" clip-path=\"url(#clip%d)\"><use x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" xlink:href=\"#path%d\"/></clipPath>");

	class CClipSVG
	{
	public:
		std::vector<CString> m_arPaths;
		std::vector<LONG> m_arTypes;

		LONG m_lWidth;
		LONG m_lHeight;

		CClipSVG() : m_arPaths(), m_arTypes()
		{
			m_lWidth	= 0;
			m_lHeight	= 0;
		}
		~CClipSVG()
		{
		}

		void Write(NSHtmlRenderer::CStringWriter& oWriter, LONG& lCurrentClipPath)
		{
			// сначала запишем все пути
			size_t nCount = m_arPaths.size();

			for (size_t i = 0; i < nCount; ++i)
			{
				CString strClip = _T("");
				strClip.Format(g_svg_string_clipCreate, lCurrentClipPath, lCurrentClipPath);
				oWriter.WriteString(strClip);
				oWriter.WriteString(m_arPaths[i]);

				if (c_nClipRegionTypeWinding == m_arTypes[i])
					oWriter.WriteString(g_svg_bstr_clipendWinding);
				else
					oWriter.WriteString(g_svg_bstr_clipendEvenodd);

				++lCurrentClipPath;
			}

			LONG lWritePathID = (LONG)lCurrentClipPath - 2;
			// теперь запишем пересечения
			for (size_t i = 1; i < nCount; ++i)
			{
				CString strWrite = _T("");
				strWrite.Format(g_string_intersect_clip, lCurrentClipPath, lCurrentClipPath - 1, m_lWidth, m_lHeight, lWritePathID);
				oWriter.WriteString(strWrite);
				++lCurrentClipPath;
				--lWritePathID;
			}
		}

		void Write2(NSHtmlRenderer::CStringWriter& oWriter, LONG& lCurrentClipPath)
		{
			// сначала запишем все пути
			size_t nCount = m_arPaths.size();
			double dMemoryClipTypes = 0;
			for (size_t i = 0; i < nCount; ++i)
			{
				dMemoryClipTypes += m_arTypes[i];
			}
			dMemoryClipTypes /= nCount;
			if (0 != dMemoryClipTypes && 1 != dMemoryClipTypes)
				return Write(oWriter, lCurrentClipPath);

			CString strClip = _T("");
			strClip.Format(g_svg_string_clipCreate, lCurrentClipPath, lCurrentClipPath);
			oWriter.WriteString(strClip);

			for (size_t i = 0; i < nCount; ++i)
			{
				oWriter.WriteString(m_arPaths[i]);
			}

			if (dMemoryClipTypes == 0)
				oWriter.WriteString(g_svg_bstr_clipendWinding);
			else
				oWriter.WriteString(g_svg_bstr_clipendEvenodd);

			++lCurrentClipPath;
		}

        AVSINLINE void Clear()
		{
			m_arPaths.clear();
			m_arTypes.clear();
		}
        AVSINLINE bool IsInit()
		{
			return (0 != m_arPaths.size());
		}
	};

	class CClipSVG2
	{
	public:
		std::vector<CString> m_arPaths;
		std::vector<LONG> m_arTypes;

		LONG m_lWidth;
		LONG m_lHeight;

		LONG m_lCountWriteClips;

		CClipSVG2() : m_arPaths(), m_arTypes()
		{
			m_lWidth	= 0;
			m_lHeight	= 0;

			m_lCountWriteClips = 0;
		}
		~CClipSVG2()
		{
		}

		void Write(NSHtmlRenderer::CStringWriter& oWriter, LONG& lCurrentClipPath)
		{
			// сначала запишем все пути
			size_t nCount = m_arPaths.size();

			LONG lOld = lCurrentClipPath;
			for (size_t i = 0; i < nCount; ++i)
			{
				CString strClip = _T("");
				strClip.Format(g_svg_string_clipCreate, lCurrentClipPath, lCurrentClipPath);
				oWriter.WriteString(strClip);
				oWriter.WriteString(m_arPaths[i]);

				if (c_nClipRegionTypeWinding == m_arTypes[i])
					oWriter.WriteString(g_svg_bstr_clipendWinding);
				else
					oWriter.WriteString(g_svg_bstr_clipendEvenodd);

				++lCurrentClipPath;
			}

			m_lCountWriteClips = (LONG)nCount;

			for (LONG i = 0; i < m_lCountWriteClips; i++)
			{
				CString strWrite = _T("");
				strWrite.Format(g_svg_string_gclip, lOld++);
				oWriter.WriteString(strWrite);
			}
		}

		void WriteEnd(NSHtmlRenderer::CStringWriter& oWriter)
		{
			while (m_lCountWriteClips > 0)
			{
				oWriter.WriteString(g_svg_bstr_graphicClose);
				--m_lCountWriteClips;
			}			
		}

        AVSINLINE void Clear()
		{
			m_arPaths.clear();
			m_arTypes.clear();
		}
        AVSINLINE bool IsInit()
		{
			return (0 != m_arPaths.size());
		}
	};

	class CSVGWriter
	{
	public:
		CStringWriter					m_oPath;
		CStringWriter					m_oDocument;

		LONG							m_lCurDocumentID;
		LONG							m_lClippingPath;
		LONG							m_lPatternID;

		bool							m_bIsClipping;
		bool							m_bIsNeedUpdateClip;
		LONG							m_lClipMode;

		NSStructures::CPen*				m_pPen;
		NSStructures::CBrush*			m_pBrush;

		int								m_lWidth;
		int								m_lHeight;

		double							m_dDpiX;
		double							m_dDpiY;

		CClipSVG						m_oClip;

	public:
		CSVGWriter() : m_oPath(), m_oDocument()
		{
			m_lCurDocumentID		= 0;
			m_lClippingPath			= 0;
			m_lPatternID			= 0;

			m_pPen					= NULL;
			m_pBrush				= NULL;

			m_dDpiX					= 96;
			m_dDpiY					= 96;

			m_lClipMode				= c_nClipRegionTypeWinding;

			m_bIsClipping			= false;
			m_bIsNeedUpdateClip		= false;
		}

		void ReInit()
		{
			m_oClip.Clear();
			m_oPath.ClearNoAttack();
			m_oDocument.ClearNoAttack();

			m_lCurDocumentID		= 0;
			m_lClippingPath			= 0;
			m_lPatternID			= 0;

			m_dDpiX					= 96;
			m_dDpiY					= 96;

			m_lClipMode				= c_nClipRegionTypeWinding;

			m_bIsClipping			= false;
			m_bIsNeedUpdateClip		= false;
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
				m_oDocument.WriteString(g_svg_bstr_svgClose);
				//CDirectory::SaveToFile(strFile, m_oDocument.GetCString());
				NSFile::CFileBinary oFile;
                oFile.CreateFileW(std::wstring(strFile.GetString()));
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

			m_oClip.Clear();
			m_lClippingPath = 0;
			m_lPatternID	= 0;
			m_bIsClipping = false;
		}
		void NewDocument(double& dWidth, double& dHeigth, LONG& lPageNumber)
		{
			m_lWidth  = (int)dWidth;
			m_lHeight = (int)dHeigth;

			m_oClip.m_lWidth	= m_lWidth;
			m_oClip.m_lHeight	= m_lHeight;

			m_lCurDocumentID = lPageNumber;

			CString strSVG = _T("");
			strSVG.Format(g_svg_string_svgOpen, m_lWidth, m_lHeight, m_lWidth, m_lHeight);

			m_oDocument.WriteString(strSVG);

			m_oClip.Clear();

			m_lClippingPath = 0;
			m_bIsClipping = false;
			m_bIsNeedUpdateClip = false;
		}
		void NewDocument(int& lWidth, int& lHeigth)
		{
			m_lWidth  = lWidth;
			m_lHeight = lHeigth;

			m_oClip.m_lWidth	= m_lWidth;
			m_oClip.m_lHeight	= m_lHeight;

			CString strSVG = _T("");
			strSVG.Format(g_svg_string_svgOpen, m_lWidth, m_lHeight, m_lWidth, m_lHeight);

			m_oClip.Clear();

			m_oDocument.WriteString(strSVG);

			m_lClippingPath = 0;
			m_bIsClipping = false;
			m_bIsNeedUpdateClip = false;
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
			m_oPath.AddSize(2);
			m_oPath.AddCharNoCheck('Z');
			m_oPath.AddSpaceNoCheck();
		}

		void WritePathMoveTo(double& x, double& y)
		{
			/*
			CString strPath = _T("");
			strPath.Format(g_svg_string_vml_MoveTo, round(x), round(y));
			m_oPath.WriteString(strPath);
			*/

			m_oPath.AddSize(30);
			m_oPath.AddCharNoCheck('M');
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round(x));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round(y));
			m_oPath.AddSpaceNoCheck();
		}
		void WritePathLineTo(double& x, double& y)
		{
			if (0 == m_oPath.GetCurSize())
			{
				WritePathMoveTo(x, y);
			}
			/*
			CString strPath = _T("");
			strPath.Format(g_svg_string_vml_LineTo, round(x), round(y));
			m_oPath.WriteString(strPath);
			*/

			m_oPath.AddSize(30);
			m_oPath.AddCharNoCheck('L');
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round(x));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round(y));
			m_oPath.AddSpaceNoCheck();
		}
		void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			if (0 == m_oPath.GetCurSize())
			{
				WritePathMoveTo(x1, y1);
			}
			
			/*
			CString strPath = _T("");
			strPath.Format(g_svg_string_vml_CurveTo, round(x1), round(y1), round(x2), round(y2), round(x3), round(y3));
			m_oPath.WriteString(strPath);
			*/

			m_oPath.AddSize(80);
			m_oPath.AddCharNoCheck('C');
			m_oPath.AddSpaceNoCheck();
			
			m_oPath.AddIntNoCheck(round(x1));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round(y1));
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round(x2));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round(y2));
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round(x3));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round(y3));
			m_oPath.AddSpaceNoCheck();
		}
		void WriteDrawPath(LONG nType, Aggplus::CMatrix* pTransform, Aggplus::CGraphicsPathSimpleConverter* pConverter, CImageInfo& oInfo, const double& dAngle)
		{
			if (m_oPath.GetCurSize() < 3)
				return;

			WriteClip();

			double dScaleTransform = (pTransform->m_agg_mtx.sx + pTransform->m_agg_mtx.sy) / 2.0;
			int nPenW		= int(m_pPen->Size * dScaleTransform);

			if (0 == nPenW)
				nPenW = 1;

			if (0 == m_pPen->Alpha)
				nType &= 0xFF00;

			if (c_BrushTypeTexture == m_pBrush->Type)
			{
				if (0 == m_pBrush->TextureAlpha)
					nType &= 0xFF;
			}
			else
			{
				if (0 == m_pBrush->Alpha1)
					nType &= 0xFF;
			}
			
			BOOL bStroke	= (0x01 == (0x01 & nType));
			BOOL bFill		= (0x01 < nType);

			if (!bFill)
			{
				// stroke
				int nColorPen	= ConvertColor(m_pPen->Color);

				CString strStyle = _T("");

				if (m_pPen->DashStyle == 0)
				{
					strStyle.Format(g_svg_string_vml_StyleStroke, nColorPen, nPenW, (double)m_pPen->Alpha / 255);
				}
				else
				{
					strStyle.Format(g_svg_string_vml_StyleStrokeDash, nColorPen, nPenW, (double)m_pPen->Alpha / 255);
				}

				m_oDocument.WriteString(g_svg_bstr_vml_Path);
				m_oDocument.WriteString(strStyle);
				WriteStyleClip();
				m_oDocument.WriteString(g_svg_bstr_path_d);
				m_oDocument.Write(m_oPath);
				m_oDocument.WriteString(g_svg_bstr_path_d_end);
				m_oDocument.WriteString(g_svg_bstr_nodeClose);
				return;
			}
			else if (c_BrushTypeTexture == m_pBrush->Type)
			{
				double x = 0;
				double y = 0;
				double r = 0;
				double b = 0;

				pConverter->PathCommandGetBounds(x, y, r, b);
				r += x;
				b += y;

				if (0 == dAngle)
				{
					pTransform->TransformPoint(x, y);
					pTransform->TransformPoint(r, b);
				}
				else
				{
					Aggplus::CMatrix oTemp  = *pTransform;
					
					double dCx = (x + r) / 2;
					double dCy = (y + b) / 2;
					pTransform->TransformPoint(dCx, dCy);
					oTemp.RotateAt(-dAngle, dCx, dCy, Aggplus::MatrixOrderAppend);

					oTemp.TransformPoint(x, y);
					oTemp.TransformPoint(r, b);
				}

				int _x = round(x);
				int _y = round(y);
				int _w = round(r - x);
				int _h = round(b - y);

				// пока заглушка
				return WriteImage(oInfo, _x, _y, _w, _h, dAngle);

				CString strPattern = _T("");

				/*
				if (itJPG == oInfo.m_eType)
				{
					strPattern.Format(g_svg_string_pattern_jpg, m_lPatternID, _w, _h, _w, _h, _x, _y, _w, _h, oInfo.m_lID);
				}
				else
				{
					strPattern.Format(g_svg_string_pattern_png, m_lPatternID, _w, _h, _w, _h, _x, _y, _w, _h, oInfo.m_lID);
				}
				*/

				m_oDocument.WriteString(strPattern);

				CString strMode = _T("nonzero");
				if (nType & c_nEvenOddFillMode)
					strMode = _T("evenodd");

				if (!bStroke)
				{
					CString strStyle = _T("");
					strStyle.Format(g_svg_string_vml_StyleFillTx, m_lPatternID, (double)m_pBrush->Alpha1 / 255, strMode);

					m_oDocument.WriteString(g_svg_bstr_vml_Path);
					m_oDocument.WriteString(strStyle);
					WriteStyleClip();
					m_oDocument.WriteString(g_svg_bstr_path_d);
					m_oDocument.Write(m_oPath);
					m_oDocument.WriteString(g_svg_bstr_path_d_end);
					m_oDocument.WriteString(g_svg_bstr_nodeClose);
				}
				else
				{
					int nPenColor = ConvertColor(m_pPen->Color);

					CString strStyle = _T("");
					strStyle.Format(g_svg_string_vml_StyleTx, m_lPatternID, (double)m_pBrush->Alpha1 / 255, strMode, nPenColor, nPenW, (double)m_pPen->Alpha / 255);

					m_oDocument.WriteString(g_svg_bstr_vml_Path);
					m_oDocument.WriteString(strStyle);
					WriteStyleClip();
					m_oDocument.WriteString(g_svg_bstr_path_d);
					m_oDocument.Write(m_oPath);
					m_oDocument.WriteString(g_svg_bstr_path_d_end);
					m_oDocument.WriteString(g_svg_bstr_nodeClose);
				}

				++m_lPatternID;
				return;
			}
			
			int nColorBrush	= ConvertColor(m_pBrush->Color1);
			CString strMode = _T("nonzero");
			if (nType & c_nEvenOddFillMode)
				strMode = _T("evenodd");
			
			if (!bStroke)
			{
				CString strStyle = _T("");
				strStyle.Format(g_svg_string_vml_StyleFill, nColorBrush, (double)m_pBrush->Alpha1 / 255, strMode);

				m_oDocument.WriteString(g_svg_bstr_vml_Path);
				m_oDocument.WriteString(strStyle);
				WriteStyleClip();
				m_oDocument.WriteString(g_svg_bstr_path_d);
				m_oDocument.Write(m_oPath);
				m_oDocument.WriteString(g_svg_bstr_path_d_end);
				m_oDocument.WriteString(g_svg_bstr_nodeClose);
				return;
			}
			
			int nPenColor = ConvertColor(m_pPen->Color);

			CString strStyle = _T("");
			strStyle.Format(g_svg_string_vml_Style, nColorBrush, (double)m_pBrush->Alpha1 / 255, strMode, nPenColor, nPenW, (double)m_pPen->Alpha / 255);

			m_oDocument.WriteString(g_svg_bstr_vml_Path);
			m_oDocument.WriteString(strStyle);
			WriteStyleClip();
			m_oDocument.WriteString(g_svg_bstr_path_d);
			m_oDocument.Write(m_oPath);
			m_oDocument.WriteString(g_svg_bstr_path_d_end);
			m_oDocument.WriteString(g_svg_bstr_nodeClose);
		}

		void WritePathClip()
		{
			m_bIsClipping		= true;
			m_bIsNeedUpdateClip	= true;
		}
		void WritePathClipEnd()
		{
			if (0 == m_oPath.GetCurSize())
				return;

			m_oClip.m_arPaths.push_back(m_oPath.GetCString());
			m_oClip.m_arTypes.push_back(m_lClipMode);
		}
		void WritePathResetClip()
		{
			m_bIsClipping		= false;
			m_bIsNeedUpdateClip	= false;
			m_oClip.Clear();
		}

		void WriteImage(CImageInfo& oInfo, const double& x, const double& y, const double& w, const double& h, const double& dAngle)
		{
			BOOL bIsClipping = FALSE;
			if ((1 < h) && (1 < w) && (1 > fabs(dAngle)))
			{
				WriteClip();
				bIsClipping = m_bIsClipping;
			}

			double dCentreX = x + w / 2.0;
			double dCentreY = y + h / 2.0;

			bool bIsRotate = (abs(dAngle) > 1) ? true : false;
			
			CString strImage = _T("");
			if (itJPG == oInfo.m_eType)
			{
				if (bIsClipping)
				{
					if (!bIsRotate)
					{
						strImage.Format(g_svg_string_image_clip_jpg, round(x), round(y), round(w), round(h), m_lClippingPath - 1, oInfo.m_lID);
					}
					else
					{
						strImage.Format(g_svg_string_image_clip_jpg_rot, round(x), round(y), round(w), round(h), m_lClippingPath - 1, oInfo.m_lID, dAngle, dCentreX, dCentreY);
					}
				}
				else
				{
					if (!bIsRotate)
					{
						strImage.Format(g_svg_string_image_jpg, round(x), round(y), round(w), round(h), oInfo.m_lID);
					}
					else
					{
						strImage.Format(g_svg_string_image_jpg_rot, round(x), round(y), round(w), round(h), oInfo.m_lID, dAngle, dCentreX, dCentreY);
					}
				}
			}
			else
			{
				if (bIsClipping)
				{
					if (!bIsRotate)
					{
						strImage.Format(g_svg_string_image_clip_png, round(x), round(y), round(w), round(h), m_lClippingPath - 1, oInfo.m_lID);
					}
					else
					{
						strImage.Format(g_svg_string_image_clip_png_rot, round(x), round(y), round(w), round(h), m_lClippingPath - 1, oInfo.m_lID, dAngle, dCentreX, dCentreY);
					}
				}
				else
				{
					if (!bIsRotate)
					{
						strImage.Format(g_svg_string_image_png, round(x), round(y), round(w), round(h), oInfo.m_lID);
					}
					else
					{
						strImage.Format(g_svg_string_image_png_rot, round(x), round(y), round(w), round(h), oInfo.m_lID, dAngle, dCentreX, dCentreY);
					}
				}
			}

			m_oDocument.WriteString(strImage);
		}
		
		inline void WriteClip()
		{
			if (m_bIsClipping && m_bIsNeedUpdateClip && (m_oClip.IsInit()))
			{
				m_oClip.Write(m_oDocument, m_lClippingPath);
				//m_oClip.Clear();
				m_bIsNeedUpdateClip = false;
			}
		}

		inline void WriteToMainHtml_1(CStringWriter* pWriter, const CDstInfo& oInfo)
		{
			if (!oInfo.m_bIsWeb)
			{
				CString strObject = _T("");
				strObject.Format(g_svg_object_start, m_lCurDocumentID);
				
				pWriter->WriteString(strObject);

				CString strSVG = _T("");
				strSVG.Format(_T("/page%d.svg"), m_lCurDocumentID);
				CloseFile(oInfo.m_strDstFilePath + strSVG);
			}
			else
			{
				CString s = _T("");
				s.Format(_T("/page%d.svg\" type=\"image/svg+xml\">"), m_lCurDocumentID);
				CString strObject = _T("<object class=\"_svg\" data=\"") + oInfo.m_strAdditionalPath + s;

				pWriter->WriteString(strObject);
				
				CString strSVG = _T("");
				strSVG.Format(_T("/page%d.svg"), m_lCurDocumentID);
				CloseFile(oInfo.m_strDstFilePath + strSVG);
			}
		}
		inline void WriteToMainHtml_2(CStringWriter* pWriter)
		{
			pWriter->WriteString(g_svg_object_end);			
		}

		inline void WriteStyleClip()
		{
			if (m_bIsClipping)
			{
				CString strClip = _T("");
				strClip.Format(g_svg_string_clip, m_lClippingPath - 1);
				m_oDocument.WriteString(strClip);
			}
		}
	};
}
