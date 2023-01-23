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
#ifndef _ASC_HTMLRENDERER_SVGWRITER_H_
#define _ASC_HTMLRENDERER_SVGWRITER_H_

#include "Common.h"
#include <vector>
#include <math.h>
#include "../../DesktopEditor/graphics/GraphicsPath.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"

namespace NSHtmlRenderer
{
	class CClipSVG
	{
	public:
		std::vector<std::wstring> m_arPaths;
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

		void Write(NSStringUtils::CStringBuilder& oWriter, LONG& lCurrentClipPath)
		{
			// сначала запишем все пути
			size_t nCount = m_arPaths.size();

			for (size_t i = 0; i < nCount; ++i)
			{
				oWriter.WriteString(L"<clipPath id=\"clip", 18);
				oWriter.AddInt(lCurrentClipPath);
				oWriter.WriteString(L"\"><path id=\"path", 16);
				oWriter.AddInt(lCurrentClipPath);
				oWriter.WriteString(L"\" d=\"", 5);

				oWriter.WriteString(m_arPaths[i]);

				if (c_nClipRegionTypeWinding == m_arTypes[i])
					oWriter.WriteString(L"\" clip-rule=\"nonzero\" /></clipPath>", 35);
				else
					oWriter.WriteString(L"\" clip-rule=\"evenodd\" /></clipPath>", 35);

				++lCurrentClipPath;
			}

			LONG lWritePathID = (LONG)lCurrentClipPath - 2;
			// теперь запишем пересечения
			for (size_t i = 1; i < nCount; ++i)
			{
				oWriter.WriteString(L"<clipPath id=\"clip", 18);
				oWriter.AddInt(lCurrentClipPath);
				oWriter.WriteString(L"\" clip-path=\"url(#clip", 22);
				oWriter.AddInt(lCurrentClipPath - 1);
				WriteFormatted(L")\"><use x=\"0\" y=\"0\" width=\"", (int)m_lWidth, L"\" height=\"", (int)m_lHeight, L"\" xlink:href=\"#path", (int)lWritePathID, L"\"/></clipPath>", &oWriter);

				++lCurrentClipPath;
				--lWritePathID;
			}
		}

		void Write2(NSStringUtils::CStringBuilder& oWriter, LONG& lCurrentClipPath)
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

			oWriter.WriteString(L"<clipPath id=\"clip", 18);
			oWriter.AddInt(lCurrentClipPath);
			oWriter.WriteString(L"\"><path id=\"path", 16);
			oWriter.AddInt(lCurrentClipPath);
			oWriter.WriteString(L"\" d=\"", 5);

			for (size_t i = 0; i < nCount; ++i)
			{
				oWriter.WriteString(m_arPaths[i]);
			}

			if (dMemoryClipTypes == 0)
				oWriter.WriteString(L"\" clip-rule=\"nonzero\" /></clipPath>", 35);
			else
				oWriter.WriteString(L"\" clip-rule=\"evenodd\" /></clipPath>", 35);

			++lCurrentClipPath;
		}

		inline void Clear()
		{
			m_arPaths.clear();
			m_arTypes.clear();
		}
		inline bool IsInit()
		{
			return (0 != m_arPaths.size());
		}
	};

	class CClipSVG2
	{
	public:
		std::vector<std::wstring> m_arPaths;
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

		void Write(NSStringUtils::CStringBuilder& oWriter, LONG& lCurrentClipPath)
		{
			// сначала запишем все пути
			size_t nCount = m_arPaths.size();

			LONG lOld = lCurrentClipPath;
			for (size_t i = 0; i < nCount; ++i)
			{
				oWriter.WriteString(L"<clipPath id=\"clip", 18);
				oWriter.AddInt(lCurrentClipPath);
				oWriter.WriteString(L"\"><path id=\"path", 16);
				oWriter.AddInt(lCurrentClipPath);
				oWriter.WriteString(L"\" d=\"", 5);

				oWriter.WriteString(m_arPaths[i]);

				if (c_nClipRegionTypeWinding == m_arTypes[i])
					oWriter.WriteString(L"\" clip-rule=\"nonzero\" /></clipPath>", 35);
				else
					oWriter.WriteString(L"\" clip-rule=\"evenodd\" /></clipPath>", 35);

				++lCurrentClipPath;
			}

			m_lCountWriteClips = (LONG)nCount;

			for (LONG i = 0; i < m_lCountWriteClips; i++)
			{
				oWriter.WriteString(L"<g clip-path=\"url(#clip", 23);
				oWriter.AddInt(lOld++);
				oWriter.WriteString(L")\">", 3);
			}
		}

		void WriteEnd(NSStringUtils::CStringBuilder& oWriter)
		{
			while (m_lCountWriteClips > 0)
			{
				oWriter.WriteString(L"</g>\n", 5);
				--m_lCountWriteClips;
			}
		}

		inline void Clear()
		{
			m_arPaths.clear();
			m_arTypes.clear();
		}
		inline bool IsInit()
		{
			return (0 != m_arPaths.size());
		}
	};

	class CSVGWriter
	{
	public:
		NSStringUtils::CStringBuilder	m_oPath;
		NSStringUtils::CStringBuilder	m_oDocument;

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

		void CloseFile(std::wstring strFile = L"")
		{
			if (!strFile.empty())
			{
				m_oDocument.WriteString(L"</svg>", 5);
				NSFile::CFileBinary::SaveToFile(strFile, m_oDocument.GetData());
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

			WriteFormatted(L"<svg width=\"", m_lWidth, L"px\" height=\"", m_lHeight, L"px\" viewBox=\"0 0 ", m_lWidth, L" ", m_lHeight,
						   L"\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", &m_oDocument);

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

			WriteFormatted(L"<svg width=\"", m_lWidth, L"px\" height=\"", m_lHeight, L"px\" viewBox=\"0 0 ", m_lWidth, L" ", m_lHeight,
						   L"\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", &m_oDocument);

			m_oClip.Clear();

			m_lClippingPath = 0;
			m_bIsClipping = false;
			m_bIsNeedUpdateClip = false;
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
			m_oPath.AddSize(30);
			m_oPath.AddCharNoCheck('M');
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round2(x));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round2(y));
			m_oPath.AddSpaceNoCheck();
		}
		void WritePathLineTo(double& x, double& y)
		{
			if (0 == m_oPath.GetCurSize())
			{
				WritePathMoveTo(x, y);
			}

			m_oPath.AddSize(30);
			m_oPath.AddCharNoCheck('L');
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round2(x));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round2(y));
			m_oPath.AddSpaceNoCheck();
		}
		void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			if (0 == m_oPath.GetCurSize())
			{
				WritePathMoveTo(x1, y1);
			}

			m_oPath.AddSize(80);
			m_oPath.AddCharNoCheck('C');
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round2(x1));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round2(y1));
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round2(x2));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round2(y2));
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round2(x3));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round2(y3));
			m_oPath.AddSpaceNoCheck();
		}
		void WriteHatchPattern()
		{
			std::wstring sPatternId = std::to_wstring(m_lPatternID++);

			m_oDocument.WriteString(L"<pattern id=\"hatch" + sPatternId + L"\" x=\"0\" y=\"0\" width=\"800\" height=\"800\" patternUnits=\"userSpaceOnUse\">");
			m_oDocument.WriteString(L"<image preserveAspectRatio=\"none\" x=\"0\" y=\"0\" width=\"800\" height=\"800\" xlink:href=\"data:image/png;base64,");

			std::string sHatchPattern = NSGraphics::GetHatchBase64(m_pBrush->TexturePath,
																   m_pBrush->Color1 & 0xFF, (m_pBrush->Color1 >> 8) & 0xFF, (m_pBrush->Color1 >> 16) & 0xFF, m_pBrush->Alpha1 & 0xFF,
																   m_pBrush->Color2 & 0xFF, (m_pBrush->Color2 >> 8) & 0xFF, (m_pBrush->Color2 >> 16) & 0xFF, m_pBrush->Alpha2 & 0xFF);

			m_oDocument.WriteString(UTF8_TO_U(sHatchPattern));

			m_oDocument.WriteString(L"\" /></pattern>");
		}
		void WriteDrawPath(LONG nType, Aggplus::CMatrix* pTransform, Aggplus::CGraphicsPathSimpleConverter* pConverter, CImageInfo& oInfo, const double& dAngle)
		{
			if (m_oPath.GetCurSize() < 3)
				return;

			WriteClip();

			double dScaleTransform = (pTransform->sx() + pTransform->sy()) / 2.0;
			int nPenW		= int(m_pPen->Size * dScaleTransform * SVG_WRITER_SCALE);

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

			bool bStroke	= (0x01 == (0x01 & nType));
			bool bFill		= (0x01 < nType);

			if (!bFill)
			{
				// stroke
				m_oDocument.WriteString(L"<path ", 6);
				m_oDocument.WriteString(L"style=\"fill:none;stroke:", 24);
				m_oDocument.WriteHexColor3(m_pPen->Color);
				m_oDocument.WriteString(L";stroke-width:", 14);
				m_oDocument.AddInt(nPenW);
				m_oDocument.WriteString(L"px;stroke-opacity:", 18);
				m_oDocument.AddIntDel100(100 * m_pPen->Alpha / 255);

				if (m_pPen->DashStyle == 0)
				{
					m_oDocument.WriteString(L";\" ", 3);
				}
				else
				{
					m_oDocument.WriteString(L";stroke-dasharray: 2,2;\" ", 25);
				}

				WriteStyleClip();
				m_oDocument.WriteString(L" d=\"", 4);
				m_oDocument.Write(m_oPath);
				m_oDocument.WriteString(L"\" />\n", 5);
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

				// пока заглушка
				return WriteImage(oInfo, x, y, r - x, b - y, dAngle);

#if 0
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
#endif
			}

			if (!bStroke)
			{
				if (c_BrushTypeHatch1 == m_pBrush->Type)
					WriteHatchPattern();

				m_oDocument.WriteString(L"<path ", 6);
				m_oDocument.WriteString(L"style=\"fill:", 12);

				if (c_BrushTypeHatch1 == m_pBrush->Type)
				{
					m_oDocument.WriteString(L"url(#hatch" + std::to_wstring(m_lPatternID - 1) + L")");
				}
				else
				{
					m_oDocument.WriteHexColor3(m_pBrush->Color1);
					m_oDocument.WriteString(L";fill-opacity:", 14);
					m_oDocument.AddIntDel100(100 * m_pBrush->Alpha1 / 255);
				}

				if (nType & c_nEvenOddFillMode)
					m_oDocument.WriteString(L";fill-rule:evenodd;stroke:none\"", 31);
				else
					m_oDocument.WriteString(L";fill-rule:nonzero;stroke:none\"", 31);

				WriteStyleClip();
				m_oDocument.WriteString(L" d=\"", 4);
				m_oDocument.Write(m_oPath);
				m_oDocument.WriteString(L"\" />\n", 5);
				return;
			}

			if (c_BrushTypeHatch1 == m_pBrush->Type)
				WriteHatchPattern();

			m_oDocument.WriteString(L"<path ", 6);
			m_oDocument.WriteString(L"style=\"fill:", 12);

			if (c_BrushTypeHatch1 == m_pBrush->Type)
			{
				m_oDocument.WriteString(L"url(#hatch" + std::to_wstring(m_lPatternID - 1) + L")");
			}
			else
			{
				m_oDocument.WriteHexColor3(m_pBrush->Color1);
				m_oDocument.WriteString(L";fill-opacity:", 14);
				m_oDocument.AddIntDel100(100 * m_pBrush->Alpha1 / 255);
			}

			if (nType & c_nEvenOddFillMode)
				m_oDocument.WriteString(L";fill-rule:evenodd;stroke:", 26);
			else
				m_oDocument.WriteString(L";fill-rule:nonzero;stroke:", 26);
			m_oDocument.WriteHexColor3(m_pPen->Color);
			m_oDocument.WriteString(L";stroke-width:", 14);
			m_oDocument.AddInt(nPenW);
			m_oDocument.WriteString(L";stroke-opacity:", 16);
			m_oDocument.AddIntDel100(100 * m_pPen->Alpha / 255);
			m_oDocument.WriteString(L"\" ", 2);

			WriteStyleClip();
			m_oDocument.WriteString(L" d=\"", 4);
			m_oDocument.Write(m_oPath);
			m_oDocument.WriteString(L"\" />\n", 5);
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

			std::wstring sNewClip = m_oPath.GetData();
			int nSizeCurrent = m_oClip.m_arPaths.size();
			if (nSizeCurrent != 0)
			{
				if (m_oClip.m_arTypes[nSizeCurrent - 1] == m_lClipMode && m_oClip.m_arPaths[nSizeCurrent - 1] == sNewClip)
					return;
			}

			m_oClip.m_arPaths.push_back(sNewClip);
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
			bool bIsClipping = false;
			if ((1 < h) && (1 < w) && (1 > fabs(dAngle)))
			{
				WriteClip();
				bIsClipping = m_bIsClipping;
			}

			double dCentreX = x + w / 2.0;
			double dCentreY = y + h / 2.0;

			dCentreX *= SVG_WRITER_SCALE;
			dCentreY *= SVG_WRITER_SCALE;

			bool bIsRotate = (abs(dAngle) > 1) ? true : false;

			if (itJPG == oInfo.m_eType)
			{
				if (bIsClipping)
				{
					m_oDocument.WriteString(L"<image x=\"", 10);
					m_oDocument.AddInt(round2(x));
					m_oDocument.WriteString(L"\" y=\"", 5);
					m_oDocument.AddInt(round2(y));
					m_oDocument.WriteString(L"\" width=\"", 10);
					m_oDocument.AddInt(round2(w));
					m_oDocument.WriteString(L"\" height=\"", 11);
					m_oDocument.AddInt(round2(h));
					m_oDocument.WriteString(L"\" clip-path=\"url(#clip", 22);
					m_oDocument.AddInt(m_lClippingPath - 1);
					m_oDocument.WriteString(L")\" xlink:href=\"media/image", 26);
					m_oDocument.AddInt(oInfo.m_lID);
					if (!bIsRotate)
					{
						m_oDocument.WriteString(L".jpg\" preserveAspectRatio=\"none\"/>", 34);
					}
					else
					{
						m_oDocument.WriteString(L".jpg\" preserveAspectRatio=\"none\" transform=\"rotate(", 51);
						m_oDocument.AddDouble(dAngle, 4);
						m_oDocument.AddCharSafe(',');
						m_oDocument.AddDouble(dCentreX, 2);
						m_oDocument.AddCharSafe(',');
						m_oDocument.AddDouble(dCentreY, 2);
						m_oDocument.WriteString(L")\"/>", 4);
					}
				}
				else
				{
					m_oDocument.WriteString(L"<image x=\"", 10);
					m_oDocument.AddInt(round2(x));
					m_oDocument.WriteString(L"\" y=\"", 5);
					m_oDocument.AddInt(round2(y));
					m_oDocument.WriteString(L"\" width=\"", 10);
					m_oDocument.AddInt(round2(w));
					m_oDocument.WriteString(L"\" height=\"", 11);
					m_oDocument.AddInt(round2(h));
					m_oDocument.WriteString(L"\" xlink:href=\"media/image", 25);
					m_oDocument.AddInt(oInfo.m_lID);

					if (!bIsRotate)
					{
						m_oDocument.WriteString(L".jpg\" preserveAspectRatio=\"none\"/>", 34);
					}
					else
					{
						m_oDocument.WriteString(L".jpg\" preserveAspectRatio=\"none\" transform=\"rotate(", 51);
						m_oDocument.AddDouble(dAngle, 4);
						m_oDocument.AddCharSafe(',');
						m_oDocument.AddDouble(dCentreX, 2);
						m_oDocument.AddCharSafe(',');
						m_oDocument.AddDouble(dCentreY, 2);
						m_oDocument.WriteString(L")\"/>", 4);
					}
				}
			}
			else
			{
				if (bIsClipping)
				{
					m_oDocument.WriteString(L"<image x=\"", 10);
					m_oDocument.AddInt(round2(x));
					m_oDocument.WriteString(L"\" y=\"", 5);
					m_oDocument.AddInt(round2(y));
					m_oDocument.WriteString(L"\" width=\"", 10);
					m_oDocument.AddInt(round2(w));
					m_oDocument.WriteString(L"\" height=\"", 11);
					m_oDocument.AddInt(round2(h));
					m_oDocument.WriteString(L"\" clip-path=\"url(#clip", 22);
					m_oDocument.AddInt(m_lClippingPath - 1);
					m_oDocument.WriteString(L")\" xlink:href=\"media/image", 26);
					m_oDocument.AddInt(oInfo.m_lID);
					if (!bIsRotate)
					{
						m_oDocument.WriteString(L".png\" preserveAspectRatio=\"none\"/>", 34);
					}
					else
					{
						m_oDocument.WriteString(L".png\" preserveAspectRatio=\"none\" transform=\"rotate(", 51);
						m_oDocument.AddDouble(dAngle, 4);
						m_oDocument.AddCharSafe(',');
						m_oDocument.AddDouble(dCentreX, 2);
						m_oDocument.AddCharSafe(',');
						m_oDocument.AddDouble(dCentreY, 2);
						m_oDocument.WriteString(L")\"/>", 4);
					}
				}
				else
				{
					m_oDocument.WriteString(L"<image x=\"", 10);
					m_oDocument.AddInt(round2(x));
					m_oDocument.WriteString(L"\" y=\"", 5);
					m_oDocument.AddInt(round2(y));
					m_oDocument.WriteString(L"\" width=\"", 10);
					m_oDocument.AddInt(round2(w));
					m_oDocument.WriteString(L"\" height=\"", 11);
					m_oDocument.AddInt(round2(h));
					m_oDocument.WriteString(L"\" xlink:href=\"media/image", 25);
					m_oDocument.AddInt(oInfo.m_lID);

					if (!bIsRotate)
					{
						m_oDocument.WriteString(L".png\" preserveAspectRatio=\"none\"/>", 34);
					}
					else
					{
						m_oDocument.WriteString(L".png\" preserveAspectRatio=\"none\" transform=\"rotate(", 51);
						m_oDocument.AddDouble(dAngle, 4);
						m_oDocument.AddCharSafe(',');
						m_oDocument.AddDouble(dCentreX, 2);
						m_oDocument.AddCharSafe(',');
						m_oDocument.AddDouble(dCentreY, 2);
						m_oDocument.WriteString(L")\"/>", 4);
					}
				}
			}
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

		inline void WriteToMainHtml_1(NSStringUtils::CStringBuilder* pWriter, const CDstInfo& oInfo)
		{
			if (!oInfo.m_bIsWeb)
			{
				pWriter->WriteString(L"<object class=\"_svg\" data=\"page", 31);
				pWriter->AddInt(m_lCurDocumentID);
				pWriter->WriteString(L".svg\" type=\"image/svg+xml\">", 27);

				CloseFile(oInfo.m_strDstFilePath + L"/page" + std::to_wstring(m_lCurDocumentID) + L".svg");
			}
			else
			{
				pWriter->WriteString(L"<object class=\"_svg\" data=\"", 27);
				pWriter->WriteString(oInfo.m_strAdditionalPath);
				pWriter->WriteString(L"/page", 5);
				pWriter->AddInt(m_lCurDocumentID);
				pWriter->WriteString(L".svg\" type=\"image/svg+xml\">", 27);

				CloseFile(oInfo.m_strDstFilePath + L"/page" + std::to_wstring(m_lCurDocumentID) + L".svg");
			}
		}
		inline void WriteToMainHtml_2(NSStringUtils::CStringBuilder* pWriter)
		{
			pWriter->WriteString(L"</object>", 9);
		}

		inline void WriteStyleClip()
		{
			if (m_bIsClipping)
			{
				if (!m_oDocument.IsSpace())
					m_oDocument.AddCharSafe(' ');

				m_oDocument.WriteString(L"clip-path=\"url(#clip", 20);
				m_oDocument.AddInt(m_lClippingPath - 1);
				m_oDocument.WriteString(L")\" ", 3);
			}
		}
	};
}

#endif // _ASC_HTMLRENDERER_SVGWRITER_H_
