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
#ifndef _ASC_HTMLRENDERER_VGW2_H_
#define _ASC_HTMLRENDERER_VGW2_H_

#include "Common.h"
#include "SVGWriter.h"
#include "VMLWriter.h"
#include "../../DesktopEditor/graphics/GraphicsPath.h"

namespace NSHtmlRenderer
{
	class CVectorGraphicsWriter
	{
	public:
		CSVGWriter		m_oSVG;
		CVMLWriter		m_oVML;
		//CCanvasWriter	m_oCanvas;

		Aggplus::CGraphicsPathSimpleConverter*	m_pSimpleConverter;

		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;

		Aggplus::CMatrix*				m_pFullTransform;

		LONG					m_lCurrentObjectInPage;
		
		LONG					m_lCurrentPageNumber;
		double					m_dWidth;
		double					m_dHeight;

	public:
		CVectorGraphicsWriter()	: m_oSVG(), m_oVML()/*, m_oCanvas()*/
		{
			m_pSimpleConverter	= NULL;
			m_lCurrentObjectInPage = 0;
		}
		~CVectorGraphicsWriter()
		{
		}

		void SetSettings(NSStructures::CPen* pPen, NSStructures::CBrush* pBrush, Aggplus::CGraphicsPathSimpleConverter* pSimpleConverter)
		{
			m_pPen				= pPen;
			m_pBrush			= pBrush;
			m_pSimpleConverter	= pSimpleConverter;

			m_oSVG.SetSettings(m_pPen, m_pBrush);
			m_oVML.SetSettings(m_pPen, m_pBrush);
			//m_oCanvas.SetSettings(m_pPen, m_pBrush, m_pSimpleConverter);
		}

		
		inline void NewPage(double& dW, double& dH, const LONG& lPageNumber)
		{
			m_lCurrentPageNumber	= lPageNumber;
			m_dWidth				= dW;
			m_dHeight				= dH;

			m_oSVG.NewDocument(m_dWidth, m_dHeight, m_lCurrentPageNumber);
			m_oVML.NewDocument(m_dWidth, m_dHeight, m_lCurrentPageNumber);
		}
		inline void EndPage()
		{
			m_oSVG.CloseFile();
			m_oVML.CloseFile();
			//m_oCanvas.CloseFile();

			m_lCurrentObjectInPage = 0;
		}
		
		inline void WriteEndPath()
		{
			m_oSVG.WritePathEnd();
			m_oVML.WritePathEnd();
			//m_oCanvas.WritePathEnd();
		}		
		
		inline void WritePathStart()
		{
			m_oSVG.WritePathStart();
			m_oVML.WritePathStart();
			//m_oCanvas.WritePathStart();
		}
		inline void WritePathClose()
		{
			m_oSVG.WritePathClose();
			m_oVML.WritePathClose();
			//m_oCanvas.WritePathClose();
		}

		inline void WritePathMoveTo(double& x, double& y)
		{
			m_oSVG.WritePathMoveTo(x, y);
			m_oVML.WritePathMoveTo(x, y);
			//m_oCanvas.WritePathMoveTo(x,y);
		}
		inline void WritePathLineTo(double& x, double& y)
		{
			m_oSVG.WritePathLineTo(x, y);
			m_oVML.WritePathLineTo(x, y);
			//m_oCanvas.WritePathLineTo(x, y);
		}
		inline void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			m_oSVG.WritePathCurveTo(x1, y1, x2, y2, x3, y3);
			m_oVML.WritePathCurveTo(x1, y1, x2, y2, x3, y3);
			//m_oCanvas.WritePathCurveTo(x1, y1, x2, y2, x3, y3);
		}
		inline void WriteDrawPath(LONG lType, CImageInfo& oInfo, const double& dAngle)
		{
			// вот мега заглушка под некоторые таблицы из pdf
			// проблема в том, что приходит path нулевой толщины - а след-но он не рисуется.
			// здесь это отслеживаем и правим пат. Не очень хорошо, так как всякие пунктирности 
			// в таких патах - теряются при таком подходе
			if (0x00 == (lType & 0xFF))
			{
				double x = 0;
				double y = 0;
				double r = 0;
				double b = 0;
				m_pSimpleConverter->PathCommandGetBounds(x, y, r, b);

				r += x;
				b += y;

				if ((fabs(r - x) < 0.5) || (fabs(b - y) < 0.5))
				{
					m_pFullTransform->TransformPoint(x, y);
					m_pFullTransform->TransformPoint(r, b);	

					int _x = round(x);
					int _y = round(y);
					int _r = round(r);
					int _b = round(b);

					if ((_x == _r) || (_y == _b))
					{
						LONG lPenColor	= m_pPen->Color;
						LONG lPenAlpha	= m_pPen->Alpha;
						double dPenW	= m_pPen->Size;

						m_pPen->Color = m_pBrush->Color1;
						m_pPen->Alpha = m_pBrush->Alpha1;
						m_pPen->Size  = c_ag_1pxWidth;
						
						WriteEndPath();
						WritePathStart();
						WritePathMoveTo(x, y);
						WritePathLineTo(r, b);

						WriteDrawPath(0x01, oInfo, dAngle);

						m_pPen->Color = lPenColor;
						m_pPen->Alpha = lPenAlpha;
						m_pPen->Size  = dPenW;
						return;
					}
				}
			}
			
			m_oSVG.WriteDrawPath(lType, m_pFullTransform, m_pSimpleConverter, oInfo, dAngle);
			m_oVML.WriteDrawPath(lType, m_pFullTransform, m_pSimpleConverter, oInfo, dAngle);

			m_pSimpleConverter->PathCommandEnd();
			//m_oCanvas.WriteDrawPath(lType, m_pFullTransform, m_pSimpleConverter, lIdTx);
		}

		inline void WritePathClip()
		{
			m_oSVG.WritePathClip();
			m_oVML.WritePathClip();
			//m_oCanvas.WritePathClip();
		}
		inline void WritePathClipEnd()
		{
			m_oSVG.WritePathClipEnd();
			m_oVML.WritePathClipEnd();
			//m_oCanvas.WritePathClipEnd();
		}
		inline void SetClipMode(LONG lClipMode)
		{
			m_oSVG.m_lClipMode = lClipMode;
			m_oVML.m_lClipMode = lClipMode;
		}
		inline void WriteImage(double& x, double& y, double& width, double& height, CImageInfo& oInfo, double dAngle)
		{
			m_oSVG.WriteImage(oInfo, x, y, width, height, dAngle);
			m_oVML.WriteImage(oInfo, x, y, width, height, dAngle);
		}
		inline void WritePathResetClip()
		{
			m_oSVG.WritePathResetClip();
			m_oVML.WritePathResetClip();
			//m_oCanvas.WritePathResetClip();
		}

		inline bool IsGraphics()
		{
			// 10 цифр на номер страницы
            // LEN(<xml xmlns:v=\"urn:schemas-microsoft-com:vml\">\n<v:group id=\"page%d\" style=\"position: absolute; width:1; height:1;\" coordsize=\"1 1\">\n) = 131
            return ((131 + 10) < (int)m_oVML.m_oDocument.GetCurSize());
		}

        inline void WriteToDocument(NSStringUtils::CStringBuilder* pDocument, const CDstInfo& oInfo)
		{
			m_oSVG.WriteToMainHtml_1(pDocument, oInfo);
			m_oVML.WriteToMainHtml(pDocument, oInfo);
			m_oSVG.WriteToMainHtml_2(pDocument);
		}
	};

	class CSVGGraphicsWriter
	{
	public:
		CSVGWriter		m_oSVG;
		Aggplus::CGraphicsPathSimpleConverter*	m_pSimpleConverter;

		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;

		Aggplus::CMatrix*				m_pFullTransform;

		double					m_dWidth;
		double					m_dHeight;

	public:
		CSVGGraphicsWriter()	: m_oSVG()
		{
			m_pSimpleConverter	= NULL;			
		}
		~CSVGGraphicsWriter()
		{
		}

		void SetSettings(NSStructures::CPen* pPen, NSStructures::CBrush* pBrush, Aggplus::CGraphicsPathSimpleConverter* pSimpleConverter)
		{
			m_pPen				= pPen;
			m_pBrush			= pBrush;
			m_pSimpleConverter	= pSimpleConverter;

			m_oSVG.SetSettings(m_pPen, m_pBrush);
		}

		
		inline void NewPage(double& dW, double& dH)
		{
			m_dWidth				= dW;
			m_dHeight				= dH;
		}
		inline void EndPage()
		{
			m_oSVG.CloseFile();			
		}
		
		inline void WriteEndPath()
		{
			m_oSVG.WritePathEnd();
		}		
		
		inline void WritePathStart()
		{
			m_oSVG.WritePathStart();
		}
		inline void WritePathClose()
		{
			m_oSVG.WritePathClose();
		}

		inline void WritePathMoveTo(double& x, double& y)
		{
			m_oSVG.WritePathMoveTo(x, y);
		}
		inline void WritePathLineTo(double& x, double& y)
		{
			m_oSVG.WritePathLineTo(x, y);
		}
		inline void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			m_oSVG.WritePathCurveTo(x1, y1, x2, y2, x3, y3);
		}
		inline void WriteDrawPath(LONG lType)
		{	
			CImageInfo oInfo;
			m_oSVG.WriteDrawPath(lType, m_pFullTransform, m_pSimpleConverter, oInfo, 0.0);
			m_pSimpleConverter->PathCommandEnd();
		}

		inline void WritePathClip()
		{
			m_oSVG.WritePathClip();
		}
		inline void WritePathClipEnd()
		{
			m_oSVG.WritePathClipEnd();
		}
		inline void SetClipMode(LONG lClipMode)
		{
			m_oSVG.m_lClipMode = lClipMode;
		}
		inline void WriteImage(double& x, double& y, double& width, double& height, CImageInfo& oInfo, double dAngle)
		{
			m_oSVG.WriteImage(oInfo, x, y, width, height, dAngle);
		}
		inline void WritePathResetClip()
		{
			m_oSVG.WritePathResetClip();
		}

		inline bool IsGraphics()
		{
			// 10 цифр на номер страницы
            // LEN(<xml xmlns:v=\"urn:schemas-microsoft-com:vml\">\n<v:group id=\"page%d\" style=\"position: absolute; width:1; height:1;\" coordsize=\"1 1\">\n) = 131
            return ((131 + 10) < (int)m_oSVG.m_oDocument.GetCurSize());
		}
	};
}

#endif // _ASC_HTMLRENDERER_VGW2_H_
