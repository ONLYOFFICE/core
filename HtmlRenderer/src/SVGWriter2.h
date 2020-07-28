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
#ifndef _ASC_HTMLRENDERER_SVGWRITER2_H_
#define _ASC_HTMLRENDERER_SVGWRITER2_H_

#include "Common2.h"
#include "SVGWriter.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"

#define SVG_INLINE_MAX_SIZE		500000 // 500Kb
#define SVG_TO_RASTER_MIN_SIZE 50000000 // 1Mb

const long c_nClipType2	= 0x0021; // c_nClipType + 1

namespace NSHtmlRenderer
{
	#define USE_SIMPLE_GRAPHICS_NOSVG
    //#define USE_BIG_GRAPHICS_TORASTER // разкоментировать, как научимся скидывать в растр!!! (пока свг не кроссплатформенный)

	class CRendererGr
	{
	public:
        NSGraphics::IGraphicsRenderer*		m_pRenderer;
        CBgraFrame*             m_pFrame;
		
        double					m_dWidth;
        double					m_dHeight;

        LONG					m_lWidthPix;
        LONG					m_lHeightPix;

        bool					m_bIsRasterNeed;

	public:
		CRendererGr()
		{
			m_pRenderer			= NULL;
			m_pFrame			= NULL;

			m_dWidth			= -1;
			m_dHeight			= -1;

			m_lWidthPix			= -1;
			m_lHeightPix		= -1;
		}
		~CRendererGr()
		{
            RELEASEOBJECT(m_pRenderer);
            RELEASEOBJECT(m_pFrame);
		}

        inline void UncheckRaster()
		{
			m_bIsRasterNeed = false;
		}
        inline void CheckRasterNeed(const bool& bIsRaster)
		{
			if (!m_bIsRasterNeed)
				m_bIsRasterNeed = bIsRaster;
		}

		void CheckRaster(double dWidth, double dHeight)
		{
			if (dWidth != m_dWidth || dHeight != m_dHeight)
			{
                RELEASEOBJECT(m_pFrame);

				m_dWidth	= dWidth;
				m_dHeight	= dHeight;
			}

            RELEASEOBJECT(m_pRenderer);

			m_lWidthPix		= (LONG)(96 * m_dWidth / 25.4);
			m_lHeightPix	= (LONG)(96 * m_dHeight / 25.4);

			if (NULL == m_pFrame)
			{
                m_pFrame = new CBgraFrame();
                m_pFrame->put_Width((int)m_lWidthPix);
                m_pFrame->put_Height((int)m_lHeightPix);
                m_pFrame->put_Stride(4 * ((int)m_lWidthPix));

                BYTE* pData = new BYTE[4 * m_lWidthPix * m_lHeightPix];
                memset(pData, 0xFF, 4 * m_lWidthPix * m_lHeightPix);
                m_pFrame->put_Data(pData);
			}

            m_pRenderer = NSGraphics::Create();
			
			m_pRenderer->put_Width(m_dWidth);
			m_pRenderer->put_Height(m_dHeight);
            m_pRenderer->CreateFromBgraFrame(m_pFrame);
		}

		template<typename T>
		void CheckRasterData(T pPage, double& dWidth, double& dHeight)
		{
			if (m_bIsRasterNeed)
			{
				CheckRaster(dWidth, dHeight);
				GetRasterData(pPage);
			}
			UncheckRaster();
		}

		template<typename T>
		void GetRasterData(T pPage)
		{
			// определяем размеры картинки
			if (NULL == m_pFrame)
				return;

            BYTE* pBuffer = m_pFrame->get_Data();

			RECT rect = GetImageBounds(m_pFrame);

			if (((rect.right - rect.left) < 5) && ((rect.bottom - rect.top) < 5))
				return;

			BYTE* pBufferSrcMem = pBuffer + 4 * rect.top * m_lWidthPix + 4 * rect.left;
			LONG lWidthShape	= rect.right - rect.left + 1;
			LONG lHeightShape	= rect.bottom - rect.top + 1;

            CBgraFrame* pShapePicture = new CBgraFrame();
            pShapePicture->put_Width((int)lWidthShape);
            pShapePicture->put_Height((int)lHeightShape);
            pShapePicture->put_Stride(4 * ((int)lWidthShape));

            BYTE* pBufferDst = new BYTE[4 * lWidthShape * lHeightShape];
            m_pFrame->put_Data(pBufferDst);

			for (LONG lLine = 0; lLine < lHeightShape; ++lLine)
			{
				memcpy(pBufferDst, pBufferSrcMem, 4 * lWidthShape);
				pBufferDst		+= 4 * lWidthShape;
				pBufferSrcMem	+= 4 * m_lWidthPix;
			}

			double dL = 25.4 * rect.left / 96.0;
			double dT = 25.4 * rect.top / 96.0;
			double dW = 25.4 * lWidthShape / 96.0;
			double dH = 25.4 * lHeightShape / 96.0;

            double dHeightMM	= 25.4 * m_lWidthPix / 96.0;
			dT = (dHeightMM - dT - dH);
			
            pPage->WriteImage(pShapePicture, dL, dT, dW, dH);

            RELEASEOBJECT(pShapePicture);
		}
	};

	class CDoubleBounds
	{
	public:
		bool IsCleared;

		double x;
		double y;
		double r;
		double b;

	public:
		CDoubleBounds()
		{
			Clear();
		}

		void Clear()
		{
			IsCleared = true;

			x = 10000000.0;
			y = 10000000.0;
			r = -10000000.0;
			b = -10000000.0;
		}

        inline void CheckPoint(const double& _x, const double& _y)
		{
			IsCleared = false;
			if (x > _x)
				x = _x;
			if (y > _y)
				y = _y;
			if (r < _x)
				r = _x;
			if (b < _y)
				b = _y;
		}

        inline void Intersect(const CDoubleBounds& oBounds)
		{
			if (IsCleared)
			{
				x = oBounds.x;
				y = oBounds.y;
				r = oBounds.r;
				b = oBounds.b;

				IsCleared = false;
			}
			else
			{
				if (oBounds.x > x)
					x = oBounds.x;

				if (oBounds.y > y)
					y = oBounds.y;

				if (oBounds.r < r)
					r = oBounds.r;

				if (oBounds.b < b)
					b = oBounds.b;
			}
		}
	};

	class CSVGWriter2
	{
	public:
        NSStringUtils::CStringBuilder   m_oPath;
        NSStringUtils::CStringBuilder	m_oDocument;

		LONG							m_lCurDocumentID;
		LONG							m_lClippingPath;
		LONG							m_lPatternID;

		bool							m_bIsClipping;
		bool							m_bIsNeedUpdateClip;
		LONG							m_lClipMode;

		NSStructures::CPen*				m_pPen;
		NSStructures::CBrush*			m_pBrush;

		NSStructures::CPen*				m_pLastPen;
		NSStructures::CBrush*			m_pLastBrush;

        Aggplus::CMatrix*						m_pTransform;

		int								m_lWidth;
		int								m_lHeight;

		double							m_dDpiX;
		double							m_dDpiY;

		CClipSVG2						m_oClip;

		// здесь храним пат в координатах не трансформированных 
		// (чтобы, если заливки сложные - можно было делать трансформы
		// непосредственно при использовании графического пути).
		double*							m_pCoordsArray;
        unsigned int    				m_lCoordsSize;
		double*							m_pCoordsArrayCur;
        unsigned int					m_lCoordsSizeCur;

		bool							m_bIsCurveToExist;

		BYTE*							m_pPathTypes;
        unsigned int					m_lPathTypesSize;
		BYTE*							m_pPathTypesCur;
        unsigned int					m_lPathTypesSizeCur;

		double							m_dCoordsScaleX;
		double							m_dCoordsScaleY;

        unsigned int					m_lEmtyDocChecker;

		BYTE*							m_pBase64Code;
		CDoubleBounds					m_oTextClipBounds;

        // переменная говорит о том, какой клип для текста записан сейчас
		// если true - то послана команда ResetTextClipRect
		// если false - то нет
		bool							m_bIsTextClipWriteCleared;
		// были ли новые clip'ы
		bool							m_bIsIntersectNewClipRect;

        // сохранение в растр. (если сложная заливка, или слишком большая векторная графика)
        //CRendererGr					m_oGrRenderer;
		//Graphics::IASCMetafile*		m_pGrRenderer;

        // клип для картинок. для конвертации сложной векторной графики в растр
		CMetafile						m_oClipMetafile;

        int                             m_nDEBUG_svg_index;

		#ifdef USE_SIMPLE_GRAPHICS_NOSVG
		
		CMetafile		m_oVectors;
		bool			m_bIsSimpleGraphics;
		bool			m_bIsSimpleSetupBrush;
		LONG			m_lBrushColorOld;
		LONG			m_lBrushAlphaOld;
		
		#endif

	public:
		CSVGWriter2() : m_oPath(), m_oDocument()
		{
			m_lCurDocumentID		= 0;
			m_lClippingPath			= 0;
			m_lPatternID			= 0;

			m_pPen					= NULL;
			m_pBrush				= NULL;

			m_pLastPen				= NULL;
			m_pLastBrush			= NULL;

			m_dDpiX					= 96;
			m_dDpiY					= 96;

			m_dCoordsScaleX			= m_dDpiX / 25.4;
			m_dCoordsScaleY			= m_dDpiY / 25.4;

			m_lClipMode				= c_nClipRegionTypeWinding;

			m_bIsClipping			= false;
			m_bIsNeedUpdateClip		= false;

			m_pCoordsArray = NULL;
			m_lCoordsSize = 0;
			m_pCoordsArrayCur = NULL;
			m_lCoordsSizeCur = 0;

			m_pPathTypes = NULL;
			m_lPathTypesSize = 0;
			m_pPathTypesCur = NULL;
			m_lPathTypesSizeCur = 0;

			m_lEmtyDocChecker	= 0;

			m_pBase64Code = new BYTE[2 * SVG_INLINE_MAX_SIZE];
			m_bIsTextClipWriteCleared = true;
			m_bIsIntersectNewClipRect = false;

			m_bIsCurveToExist = false;

            m_nDEBUG_svg_index = 0;

#ifdef USE_SIMPLE_GRAPHICS_NOSVG
			m_bIsSimpleGraphics = true;
			m_bIsSimpleSetupBrush = false;

			m_lBrushColorOld = 0;
			m_lBrushAlphaOld = 255;
#endif
		}
		~CSVGWriter2()
		{
			RELEASEARRAYOBJECTS(m_pBase64Code);
		}

		void ReInit()
		{
			m_bIsTextClipWriteCleared = true;
			m_bIsIntersectNewClipRect = false;
			m_oTextClipBounds.Clear();
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
                m_oDocument.WriteString(L"</svg>", 6);
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

        std::wstring GetSVGXml()
		{
			m_oClip.WriteEnd(m_oDocument);
            m_oDocument.WriteString(L"</svg>", 6);

            return m_oDocument.GetData();
		}

        void DEBUG_DumpSVG(const std::wstring& sTempPath)
        {
            std::wstring sEndDebug = L"";
            int nCountWriteClips = m_oClip.m_lCountWriteClips;
            while (nCountWriteClips > 0)
            {
                sEndDebug += L"</g>\n";
                --nCountWriteClips;
            }
            sEndDebug += L"</svg>";

            m_nDEBUG_svg_index++;

            std::wstring sDocument = m_oDocument.GetData() + sEndDebug;

            NSFile::CFileBinary::SaveToFile(sTempPath + L"/svg_" + std::to_wstring(m_nDEBUG_svg_index) + L".svg", sDocument);
        }

        void CloseFile2(std::wstring strFile, bool bIsNeedEnd = true)
		{
			if (bIsNeedEnd)
			{
				m_oClip.WriteEnd(m_oDocument);
                m_oDocument.WriteString(L"</svg>", 6);
			}

            NSFile::CFileBinary::SaveToFile(strFile, m_oDocument.GetData());
			m_oDocument.ClearNoAttack();
			m_oPath.ClearNoAttack();

			if (m_bIsClipping)
			{
				m_bIsNeedUpdateClip = true;
			}
			
			m_lPatternID	= 0;
		}
		int CloseFile3(NSHtmlRenderer::CMetafile* m_pMeta, bool bIsNeedEnd = true)
		{
			if (bIsNeedEnd)
			{
				m_oClip.WriteEnd(m_oDocument);
                m_oDocument.WriteString(L"</svg>", 6);
			}

            BYTE* pDataSrc = NULL;
            LONG nLenSrc = 0;
            NSFile::CUtf8Converter::GetUtf8StringFromUnicode(m_oDocument.GetBuffer(), (LONG)m_oDocument.GetCurSize(), pDataSrc, nLenSrc);

            char* pBase64 = NULL;
            int nLenDst = 2 * SVG_INLINE_MAX_SIZE;
            NSBase64::Base64Encode(pDataSrc, (int)nLenSrc, m_pBase64Code, &nLenDst);

            RELEASEARRAYOBJECTS(pDataSrc);

            m_pMeta->WriteLONG(nLenDst);
            m_pMeta->Write(m_pBase64Code, nLenDst);

			m_oDocument.ClearNoAttack();
			m_oPath.ClearNoAttack();

			if (m_bIsClipping)
			{
				m_bIsNeedUpdateClip = true;
			}

			m_lPatternID	= 0;

            return nLenDst;
		}

		void NewDocument(const double& dWidth, const double& dHeigth, const LONG& lPageNumber)
		{
			m_lWidth  = (int)(dWidth * m_dCoordsScaleX);
			m_lHeight = (int)(dHeigth * m_dCoordsScaleY);

			m_oClip.m_lWidth	= m_lWidth;
			m_oClip.m_lHeight	= m_lHeight;

			m_lCurDocumentID = lPageNumber;

            m_oDocument.ClearNoAttack();

            m_oDocument.WriteString(L"<svg width=\"", 12);
            m_oDocument.AddInt(m_lWidth);
            m_oDocument.WriteString(L"px\" height=\"", 12);
            m_oDocument.AddInt(m_lHeight);
            m_oDocument.WriteString(L"px\" viewBox=\"0 0 ", 17);
            m_oDocument.AddInt(m_lWidth);
            m_oDocument.AddCharSafe(' ');
            m_oDocument.AddInt(m_lHeight);
            m_oDocument.WriteString(L"\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", 95);

			m_oClip.Clear();

			m_lClippingPath = 0;
			m_bIsClipping = false;
			m_bIsNeedUpdateClip = false;

            m_lEmtyDocChecker = (unsigned int)m_oDocument.GetCurSize();
		}

	public:

		inline void WritePathEnd()
		{
			m_bIsCurveToExist = false;
			m_oPath.ClearNoAttack();

			ClearVectorCoords();
		}		
		inline void WritePathStart()
		{
			m_bIsCurveToExist = false;
			m_oPath.ClearNoAttack();

			ClearVectorCoords();
		}
		void WritePathClose()
		{
			CheckSizeVectorB();
			*m_pPathTypesCur++ = 3;
			++m_lPathTypesSizeCur;

			/*
			m_oPath.AddSize(2);
			m_oPath.AddCharNoCheck('Z');
			m_oPath.AddSpaceNoCheck();
			*/
		}

		void WritePathMoveTo(const double& x, const double& y)
		{
			CheckSizeVectorB();
			*m_pPathTypesCur++ = 0;
			++m_lPathTypesSizeCur;

			CheckSizeVectorD(2);
			m_pCoordsArrayCur[0] = x;
			m_pCoordsArrayCur[1] = y;
			
			m_pTransform->TransformPoint(m_pCoordsArrayCur[0], m_pCoordsArrayCur[1]);

			m_pCoordsArrayCur += 2;
			m_lCoordsSizeCur += 2;

			/*
			m_oPath.AddSize(30);
			m_oPath.AddCharNoCheck('M');
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round(x));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round(y));
			m_oPath.AddSpaceNoCheck();
			*/
		}
		void WritePathLineTo(const double& x, const double& y)
		{
			if (0 == m_lPathTypesSizeCur)
			{
				WritePathMoveTo(x, y);
				return;
			}

			CheckSizeVectorB();
			*m_pPathTypesCur++ = 1;
			++m_lPathTypesSizeCur;

			CheckSizeVectorD(2);
			m_pCoordsArrayCur[0] = x;
			m_pCoordsArrayCur[1] = y;
			
			m_pTransform->TransformPoint(m_pCoordsArrayCur[0], m_pCoordsArrayCur[1]);

			if (fabs(m_pCoordsArray[m_lCoordsSizeCur - 2] - m_pCoordsArrayCur[0]) < 0.1 && 
				fabs(m_pCoordsArray[m_lCoordsSizeCur - 1] - m_pCoordsArrayCur[1]) < 0.1)
			{
				// попали в текущую точку. не добавляем
				--m_pPathTypesCur;
				--m_lPathTypesSizeCur;
				return;
			}

			m_pCoordsArrayCur += 2;
			m_lCoordsSizeCur += 2;

			/*
			m_oPath.AddSize(30);
			m_oPath.AddCharNoCheck('L');
			m_oPath.AddSpaceNoCheck();

			m_oPath.AddIntNoCheck(round(x));
			m_oPath.AddCharNoCheck(',');
			m_oPath.AddIntNoCheck(round(y));
			m_oPath.AddSpaceNoCheck();
			*/
		}
		void WritePathCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
		{
			if (0 == m_lPathTypesSizeCur)
				WritePathMoveTo(x1, y1);

			CheckSizeVectorB();
			*m_pPathTypesCur++ = 2;
			++m_lPathTypesSizeCur;

			CheckSizeVectorD(6);
			m_pCoordsArrayCur[0] = x1;
			m_pCoordsArrayCur[1] = y1;
			m_pCoordsArrayCur[2] = x2;
			m_pCoordsArrayCur[3] = y2;
			m_pCoordsArrayCur[4] = x3;
			m_pCoordsArrayCur[5] = y3;

			m_pTransform->TransformPoint(m_pCoordsArrayCur[0], m_pCoordsArrayCur[1]);
			m_pTransform->TransformPoint(m_pCoordsArrayCur[2], m_pCoordsArrayCur[3]);
			m_pTransform->TransformPoint(m_pCoordsArrayCur[4], m_pCoordsArrayCur[5]);

			m_pCoordsArrayCur += 6;
			m_lCoordsSizeCur += 6;

			m_bIsCurveToExist = true;
			
			/*
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
			*/
		}

        void WriteDrawPath(LONG nType, Aggplus::CGraphicsPathSimpleConverter* pConverter, CImageInfo& oInfo)
		{            
			if (m_lPathTypesSizeCur == 0)
				return;

			WriteClip();

			#ifdef USE_SIMPLE_GRAPHICS_NOSVG
			if (m_bIsClipping)
				m_bIsSimpleGraphics = false;
			#endif

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

			bool bIsNeedTransform = true;
			// первым делом нужно понять, будем ли мы трансформировать пути
			if (nType > 0xFF && m_pBrush->Type == c_BrushTypeTexture)
			{
				bIsNeedTransform = false;
			}

			int nPenW = 1;
			if (0 != m_pPen->Size)
			{
				if (!bIsNeedTransform)
				{
					nPenW = (int)(m_pPen->Size);
				}
				else
				{
					double _x1 = 0;
					double _y1 = 0;
					double _x2 = 1;
					double _y2 = 1;
					m_pTransform->TransformPoint(_x1, _y1);
					m_pTransform->TransformPoint(_x2, _y2);

					double dScaleTransform = sqrt(((_x2 - _x1) * (_x2 - _x1) + (_y2 - _y1) * (_y2 - _y1)) / 2) * m_dCoordsScaleX;
					nPenW = (int)(m_pPen->Size * dScaleTransform);
				}				
			}

            bool bStroke	= (0x01 == (0x01 & nType));

			if (nPenW == 0 && bStroke)
				nPenW = 1;

            bool bFill		= (0x01 < nType);
			bool bIsLine = false;

			#ifdef USE_SIMPLE_GRAPHICS_NOSVG

			if (m_bIsSimpleGraphics)
			{
				if (bFill && ((0 != (nType & c_nEvenOddFillMode)) || m_pBrush->Type != c_BrushTypeSolid))
					m_bIsSimpleGraphics = false;

				if (bStroke && m_pPen->DashStyle != 0)
					m_bIsSimpleGraphics = false;
			}

			if (m_bIsSimpleGraphics)
			{
				// пишем вектор и настройки brush/pen
				m_oVectors.WriteCommandType(CMetafile::ctPathCommandStart);
				WriteToPathToSimpleVector();

				bIsLine = WriteToPathToSVGPath(false, (bFill && !bStroke) ? true : false);
				
				if (bFill && !bIsLine)
				{
					if (!m_pLastBrush->IsEqual(m_pBrush))
					{
						if (!m_bIsSimpleSetupBrush)
						{
							m_lBrushColorOld = m_pLastBrush->Color1;
							m_lBrushAlphaOld = m_pLastBrush->Alpha1;
							m_bIsSimpleSetupBrush = true;
						}

						*m_pLastBrush = *m_pBrush;
						
						m_oVectors.WriteCommandType(CMetafile::ctBrushColor1);
						LONG lBGR	= m_pBrush->Color1;

						m_oVectors.WriteBYTE((BYTE)(lBGR & 0xFF));
						m_oVectors.WriteBYTE((BYTE)((lBGR >> 8) & 0xFF));
						m_oVectors.WriteBYTE((BYTE)((lBGR >> 16) & 0xFF));
						m_oVectors.WriteBYTE((BYTE)m_pBrush->Alpha1);
					}
				}
				else if (bIsLine)
				{
					LONG _C = m_pPen->Color;
					LONG _A = m_pPen->Alpha;
					double _W = m_pPen->Size;

					m_pPen->Color = m_pBrush->Color1;
					m_pPen->Alpha = m_pBrush->Alpha1;
					m_pPen->Size = 1 / m_dCoordsScaleX;

					if (!m_pLastPen->IsEqual(m_pPen))
					{
						*m_pLastPen = *m_pPen;
						
						m_oVectors.WriteCommandType(CMetafile::ctPenColor);
						LONG lBGR	= m_pPen->Color;

						m_oVectors.WriteBYTE((BYTE)(lBGR & 0xFF));
						m_oVectors.WriteBYTE((BYTE)((lBGR >> 8) & 0xFF));
						m_oVectors.WriteBYTE((BYTE)((lBGR >> 16) & 0xFF));
						m_oVectors.WriteBYTE((BYTE)m_pPen->Alpha);

						m_oVectors.WriteCommandType(CMetafile::ctPenSize);
						m_oVectors.WriteDouble((double)nPenW / m_dCoordsScaleX);
					}
					
					m_pPen->Color = _C;
					m_pPen->Alpha = _A;
					m_pPen->Size = _W;
				}
				if (bStroke)
				{
					if (!m_pLastPen->IsEqual(m_pPen))
					{
						*m_pLastPen = *m_pPen;
						
						m_oVectors.WriteCommandType(CMetafile::ctPenColor);
						LONG lBGR	= m_pPen->Color;

						m_oVectors.WriteBYTE((BYTE)(lBGR & 0xFF));
						m_oVectors.WriteBYTE((BYTE)((lBGR >> 8) & 0xFF));
						m_oVectors.WriteBYTE((BYTE)((lBGR >> 16) & 0xFF));
						m_oVectors.WriteBYTE((BYTE)m_pPen->Alpha);

						m_oVectors.WriteCommandType(CMetafile::ctPenSize);
						m_oVectors.WriteDouble((double)nPenW / m_dCoordsScaleX);
					}
				}

				m_oVectors.WriteCommandType(CMetafile::ctDrawPath);
				m_oVectors.WriteLONG(bIsLine ? 1 : nType);

				m_oVectors.WriteCommandType(CMetafile::ctPathCommandStart);
			}
			else
			{
				bIsLine = WriteToPathToSVGPath(false, (bFill && !bStroke) ? true : false);
			}

			#else
			bIsLine = WriteToPathToSVGPath(false, (bFill && !bStroke) ? true : false);
			#endif
			
			if (!bFill)
			{
				// stroke
                m_oDocument.WriteString(L"<path style=\"fill:none;stroke:", 30);
                if (bStroke)
                    m_oDocument.WriteHexColor3(m_pPen->Color);
                else
                    m_oDocument.WriteString(L"none");
                m_oDocument.WriteString(L";stroke-width:", 14);
                m_oDocument.AddInt(nPenW);
                m_oDocument.WriteString(L";stroke-opacity:", 16);
                m_oDocument.AddDouble((double)m_pPen->Alpha / 255, 2);
                if (m_pPen->DashStyle == 0)
                    m_oDocument.WriteString(L";\" ", 3);
                else
                    m_oDocument.WriteString(L";stroke-dasharray: 2,2;\" ", 25);

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
				double w = 0;
				double h = 0;
                pConverter->PathCommandGetBounds(x, y, w, h);

				if (m_pBrush->TextureMode == c_BrushTextureModeStretch || true)
				{
					// 1) пишем паттерн
                    double _tx = m_pTransform->tx() * m_dCoordsScaleX;
                    double _ty = m_pTransform->ty() * m_dCoordsScaleY;

                    double _w = w * m_dCoordsScaleX;
                    double _h = h * m_dCoordsScaleY;

                    m_oDocument.WriteString(L"<pattern id=\"pt", 15);
                    m_oDocument.AddInt(m_lPatternID);
                    m_oDocument.WriteString(L"\" patternUnits=\"userSpaceOnUse\" x=\"0\" y=\"0\" width=\"", 51);
                    m_oDocument.AddInt(round(_w));
                    m_oDocument.WriteString(L"\" height=\"", 10);
                    m_oDocument.AddInt(round(_h));
                    m_oDocument.WriteString(L"\" viewBox=\"0 0 ", 15);
                    m_oDocument.AddInt(round(_w));
                    m_oDocument.AddCharSafe(' ');
                    m_oDocument.AddInt(round(_h));
                    m_oDocument.WriteString(L"\" transform=\"matrix(");
                    m_oDocument.AddDouble(m_pTransform->sx(), 3);
                    m_oDocument.AddCharSafe(',');
                    m_oDocument.AddDouble(m_pTransform->shy(), 3);
                    m_oDocument.AddCharSafe(',');
                    m_oDocument.AddDouble(m_pTransform->shx(), 3);
                    m_oDocument.AddCharSafe(',');
                    m_oDocument.AddDouble(m_pTransform->sy(), 3);
                    m_oDocument.AddCharSafe(',');
                    m_oDocument.AddDouble(_tx, 3);
                    m_oDocument.AddCharSafe(',');
                    m_oDocument.AddDouble(_ty, 3);
                    m_oDocument.WriteString(L")\"><image x=\"0\" y=\"0\" width=\"", 29);
                    m_oDocument.AddInt(round(_w));
                    m_oDocument.WriteString(L"\" height=\"", 10);
                    m_oDocument.AddInt(round(_h));
                    m_oDocument.WriteString(L"\" xlink:href=\"image", 19);
                    m_oDocument.AddInt(oInfo.m_lID);
                    if (itJPG == oInfo.m_eType)
                        m_oDocument.WriteString(L".jpg\" preserveAspectRatio=\"none\"/></pattern>", 44);
                    else
                        m_oDocument.WriteString(L".png\" preserveAspectRatio=\"none\"/></pattern>", 44);
                }
				else
				{
					// TODO:
				}

                m_oDocument.WriteString(L"<path style=\"fill:url(#pt", 25);
                m_oDocument.AddInt(m_lPatternID);
                m_oDocument.WriteString(L");fill-opacity:");
                m_oDocument.AddDouble((double)m_pBrush->Alpha1 / 255, 2);
                if (nType & c_nEvenOddFillMode)
                    m_oDocument.WriteString(L";fill-rule:evenodd;", 19);
                else
                    m_oDocument.WriteString(L";fill-rule:nonzero;", 19);

                if (!bStroke)
                {
                    m_oDocument.WriteString(L"stroke:none;\" ", 14);
                }
                else
                {
                    m_oDocument.WriteString(L"stroke:", 7);
                    m_oDocument.WriteHexColor3(m_pPen->Color);
                    m_oDocument.WriteString(L";stroke-width:", 14);
                    m_oDocument.AddInt(nPenW);
                    m_oDocument.WriteString(L";stroke-opacity:", 16);
                    m_oDocument.AddDouble((double)m_pPen->Alpha / 255, 2);
                    m_oDocument.WriteString(L"\" ", 2);
                }

                WriteStyleClip();
                m_oDocument.WriteString(L" d=\"", 4);
                m_oDocument.Write(m_oPath);
                m_oDocument.WriteString(L"\" />\n", 5);

                ++m_lPatternID;
				return;
			}
			
			int nColorBrush	= ConvertColor(m_pBrush->Color1);
            if (nType & c_nEvenOddFillMode)
			{
                #ifdef USE_SIMPLE_GRAPHICS_NOSVG
				m_bIsSimpleGraphics = false;
				#endif
			}
			
			if (!bStroke)
			{
				if (bIsLine)
				{
                    m_oDocument.WriteString(L"<path style=\"fill:none;stroke:", 30);
                    m_oDocument.WriteHexColor3(m_pBrush->Color1);
                    m_oDocument.WriteString(L";stroke-width:", 14);
                    m_oDocument.AddInt(1);
                    m_oDocument.WriteString(L";stroke-opacity:", 16);
                    m_oDocument.AddDouble((double)m_pBrush->Alpha1 / 255, 2);
                    m_oDocument.WriteString(L";\" ", 3);

                    WriteStyleClip();
                    m_oDocument.WriteString(L" d=\"", 4);
                    m_oDocument.Write(m_oPath);
                    m_oDocument.WriteString(L"\" />\n", 5);
					return;
				}

                m_oDocument.WriteString(L"<path style=\"fill:", 18);
                m_oDocument.WriteHexColor3(m_pBrush->Color1);
                m_oDocument.WriteString(L";fill-opacity:", 14);
                m_oDocument.AddDouble((double)m_pBrush->Alpha1 / 255, 2);
                if (nType & c_nEvenOddFillMode)
                    m_oDocument.WriteString(L";fill-rule:evenodd;stroke:none\" ", 32);
                else
                    m_oDocument.WriteString(L";fill-rule:nonzero;stroke:none\" ", 32);

				WriteStyleClip();
                m_oDocument.WriteString(L" d=\"", 4);
                m_oDocument.Write(m_oPath);
                m_oDocument.WriteString(L"\" />\n", 5);
				return;
			}

            m_oDocument.WriteString(L"<path style=\"fill:", 18);
            m_oDocument.WriteHexColor3(m_pBrush->Color1);
            m_oDocument.WriteString(L";fill-opacity:", 14);
            m_oDocument.AddDouble((double)m_pBrush->Alpha1 / 255, 2);
            if (nType & c_nEvenOddFillMode)
                m_oDocument.WriteString(L";fill-rule:evenodd;stroke:", 26);
            else
                m_oDocument.WriteString(L";fill-rule:nonzero;stroke:", 26);
            m_oDocument.WriteHexColor3(m_pPen->Color);
            m_oDocument.WriteString(L";stroke-width:", 14);
            m_oDocument.AddInt(nPenW);
            m_oDocument.WriteString(L";stroke-opacity:", 16);
            m_oDocument.AddDouble((double)m_pPen->Alpha / 255, 2);
            m_oDocument.WriteString(L";\" ", 3);

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
			/*
			if (1 == m_lPathTypesSizeCur && m_pPathTypes[0] <= 1)
			{
				// не надо клип делать. пат странен)
				return;
			}
			else if (2 == m_lPathTypesSizeCur && 
				((m_pPathTypes[0] == 0 || m_pPathTypes[1] == 0) && (m_pPathTypes[0] <= 1 && m_pPathTypes[1] <= 1)))
			{
				// пат все еще странен
				return;
			}
			*/
			m_oClip.WriteEnd(m_oDocument);

			m_bIsIntersectNewClipRect = true;
			WriteToPathToSVGPath(true);

			if (0 == m_oPath.GetCurSize())
				return;

            m_oClip.m_arPaths.push_back(m_oPath.GetData());
            m_oClip.m_arTypes.push_back(m_lClipMode);
		}
		void WritePathResetClip()
		{
			m_bIsClipping		= false;
			m_bIsNeedUpdateClip	= false;
			m_oClip.Clear();
			m_oTextClipBounds.Clear();

			m_oClipMetafile.ClearNoAttack();

			m_oClip.WriteEnd(m_oDocument);
		}

        void WriteImage(CImageInfo& oInfo, const double& x, const double& y, const double& w, const double& h)
		{
            bool bIsClipping = false;
			if ((1 < h) && (1 < w))
			{
				WriteClip();
				bIsClipping = m_bIsClipping;
			}

			double dCentreX = x + w / 2.0;
			double dCentreY = y + h / 2.0;

            bool bIsNoNeedTransform = m_pTransform->IsIdentity2(0.0001);

			if (bIsNoNeedTransform)
			{
                double _x = x + m_pTransform->tx();
                double _y = y + m_pTransform->ty();

				_x *= m_dCoordsScaleX;
				_y *= m_dCoordsScaleY;

				double _w = w * m_dCoordsScaleX;
				double _h = h * m_dCoordsScaleY;

				if (itJPG == oInfo.m_eType)
				{
					/*if (bIsClipping)
					{
                        //("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" clip-path=\"url(#clip%d)\" xlink:href=\"image%d.jpg\" preserveAspectRatio=\"none\"/>");
						strImage.Format(g_svg_string_image_clip_jpg1, round(_x), round(_y), round(_w), round(_h), m_lClippingPath - 1, oInfo.m_lID);
					}
					else*/
					{
                        m_oDocument.WriteString(L"<image x=\"", 10);
                        m_oDocument.AddInt(round(_x));
                        m_oDocument.WriteString(L"\" y=\"", 5);
                        m_oDocument.AddInt(round(_y));
                        m_oDocument.WriteString(L"\" width=\"", 9);
                        m_oDocument.AddInt( round(_w));
                        m_oDocument.WriteString(L"\" height=\"", 10);
                        m_oDocument.AddInt( round(_h));
                        m_oDocument.WriteString(L"\" xlink:href=\"image", 19);
                        m_oDocument.AddInt(oInfo.m_lID);
                        m_oDocument.WriteString(L".jpg\" preserveAspectRatio=\"none\"/>", 34);
					}
				}
				else
				{
					/*if (bIsClipping)
					{
                        //("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" clip-path=\"url(#clip%d)\" xlink:href=\"image%d.png\" preserveAspectRatio=\"none\"/>");
						strImage.Format(g_svg_string_image_clip_png1, round(_x), round(_y), round(_w), round(_h), m_lClippingPath - 1, oInfo.m_lID);
					}
					else*/
					{
                        m_oDocument.WriteString(L"<image x=\"", 10);
                        m_oDocument.AddInt(round(_x));
                        m_oDocument.WriteString(L"\" y=\"", 5);
                        m_oDocument.AddInt(round(_y));
                        m_oDocument.WriteString(L"\" width=\"", 9);
                        m_oDocument.AddInt( round(_w));
                        m_oDocument.WriteString(L"\" height=\"", 10);
                        m_oDocument.AddInt( round(_h));
                        m_oDocument.WriteString(L"\" xlink:href=\"image", 19);
                        m_oDocument.AddInt(oInfo.m_lID);
                        m_oDocument.WriteString(L".png\" preserveAspectRatio=\"none\"/>", 34);
					}
				}
			}
			else
			{
                double _tx = m_pTransform->tx() * m_dCoordsScaleX;
                double _ty = m_pTransform->ty() * m_dCoordsScaleY;

				double _x = x * m_dCoordsScaleX;
				double _y = y * m_dCoordsScaleY;

				double _w = w * m_dCoordsScaleX;
				double _h = h * m_dCoordsScaleY;

				if (itJPG == oInfo.m_eType)
				{
					/*if (bIsClipping)
					{
                        //_T("<image x=\"%.3lf\" y=\"%.3lf\" width=\"%.3lf\" height=\"%.3lf\" clip-path=\"url(#clip%d)\" xlink:href=\"image%d.jpg\" preserveAspectRatio=\"none\" transform=\"matrix(%.3lf, %.3lf,%.3lf,%.3lf, %.3lf,%.3lf)\"/>");
						strImage.Format(g_svg_string_image_clip_jpg_mtx, _x, _y, _w, _h, m_lClippingPath - 1, oInfo.m_lID, mtx->sx, mtx->shy, mtx->shx, mtx->sy, _tx, _ty);
					}
					else*/
					{
                        m_oDocument.WriteString(L"<image x=\"", 10);
                        m_oDocument.AddDouble(_x, 3);
                        m_oDocument.WriteString(L"\" y=\"", 5);
                        m_oDocument.AddDouble(_y, 3);
                        m_oDocument.WriteString(L"\" width=\"", 9);
                        m_oDocument.AddDouble(_w, 3);
                        m_oDocument.WriteString(L"\" height=\"", 10);
                        m_oDocument.AddDouble(_h, 3);
                        m_oDocument.WriteString(L"\" xlink:href=\"image", 19);
                        m_oDocument.AddInt(oInfo.m_lID);
                        m_oDocument.WriteString(L".jpg\" preserveAspectRatio=\"none\" transform=\"matrix(/>", 53);

                        m_oDocument.AddDouble(m_pTransform->sx(), 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(m_pTransform->shy(), 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(m_pTransform->shx(), 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(m_pTransform->sy(), 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(_tx, 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(_ty, 3);
                        m_oDocument.WriteString(L")\"/>", 4);
					}
				}
				else
				{
					/*if (bIsClipping)
					{
                        //_T("<image x=\"%.3lf\" y=\"%.3lf\" width=\"%.3lf\" height=\"%.3lf\" clip-path=\"url(#clip%d)\" xlink:href=\"image%d.jpg\" preserveAspectRatio=\"none\" transform=\"matrix(%.3lf, %.3lf,%.3lf,%.3lf, %.3lf,%.3lf)\"/>");
						strImage.Format(g_svg_string_image_clip_png_mtx, _x, _y, _w, _h, m_lClippingPath - 1, oInfo.m_lID, mtx->sx, mtx->shy, mtx->shx, mtx->sy, _tx, _ty);
					}
					else*/
					{
                        m_oDocument.WriteString(L"<image x=\"", 10);
                        m_oDocument.AddDouble(_x, 3);
                        m_oDocument.WriteString(L"\" y=\"", 5);
                        m_oDocument.AddDouble(_y, 3);
                        m_oDocument.WriteString(L"\" width=\"", 9);
                        m_oDocument.AddDouble(_w, 3);
                        m_oDocument.WriteString(L"\" height=\"", 10);
                        m_oDocument.AddDouble(_h, 3);
                        m_oDocument.WriteString(L"\" xlink:href=\"image", 19);
                        m_oDocument.AddInt(oInfo.m_lID);
                        m_oDocument.WriteString(L".png\" preserveAspectRatio=\"none\" transform=\"matrix(/>", 53);

                        m_oDocument.AddDouble(m_pTransform->sx(), 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(m_pTransform->shy(), 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(m_pTransform->shx(), 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(m_pTransform->sy(), 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(_tx, 3);
                        m_oDocument.AddCharSafe(',');
                        m_oDocument.AddDouble(_ty, 3);
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

		inline void WriteStyleClip()
		{
			// сейчас не пишем (оборачиваем в g)
			return;

			if (m_bIsClipping)
			{
                //_T("clip-path=\"url(#clip%d)\" ");
                m_oDocument.WriteString(L"clip-path=\"url(#clip", 20);
                m_oDocument.AddInt(m_lClippingPath - 1);
                m_oDocument.WriteString(L")\" ", 3);
			}
		}

		void CheckSizeVectorB(const int& len = 1)
		{
			if (NULL != m_pPathTypes)
			{
                unsigned int nNewSize = (unsigned int)(m_lPathTypesSizeCur + len);
				if (nNewSize >= m_lPathTypesSize)
				{
					if (nNewSize >= m_lPathTypesSize)
					{
						while (nNewSize >= m_lPathTypesSize)
						{
							m_lPathTypesSize *= 2;
						}
						
						BYTE* pNew = new BYTE[m_lPathTypesSize];
						memcpy(pNew, m_pPathTypes, m_lPathTypesSizeCur);

						RELEASEARRAYOBJECTS(m_pPathTypes);
						m_pPathTypes = pNew;
						m_pPathTypesCur = m_pPathTypes + m_lPathTypesSizeCur;
					}
				}
			}
			else
			{
				m_lPathTypesSize	= 1000;
				m_pPathTypes		= new BYTE[m_lPathTypesSize];
				m_pPathTypesCur		= m_pPathTypes;
				m_lPathTypesSizeCur = 0;
			}
		}

		void CheckSizeVectorD(const int& len)
		{
			if (NULL != m_pCoordsArray)
			{
                unsigned int nNewSize = (unsigned int)(m_lCoordsSizeCur + len);
				if (nNewSize >= m_lCoordsSize)
				{
					while (nNewSize >= m_lCoordsSize)
					{
						m_lCoordsSize *= 2;
					}
					
					double* pNew = new double[m_lCoordsSize];
					memcpy(pNew, m_pCoordsArray, m_lCoordsSizeCur * sizeof(double));

					RELEASEARRAYOBJECTS(m_pCoordsArray);
					m_pCoordsArray = pNew;
					m_pCoordsArrayCur = m_pCoordsArray + m_lCoordsSizeCur;
				}
			}
			else
			{
				m_lCoordsSize		= 1000;
				m_pCoordsArray		= new double[m_lCoordsSize];
				m_pCoordsArrayCur	= m_pCoordsArray;	
				m_lCoordsSizeCur	= 0;
			}
		}

		void ClearVectorCoords()
		{
			m_lCoordsSizeCur = 0;
			m_pCoordsArrayCur = m_pCoordsArray;

			m_lPathTypesSizeCur = 0;
			m_pPathTypesCur = m_pPathTypes;
		}

		bool WriteToPathToSVGPath(bool bIsClipping = false, bool bIsNeedAnalyzeLine = false)
		{
			m_oPath.ClearNoAttack();

			CDoubleBounds oBounds;

			if (!bIsNeedAnalyzeLine)
			{
                unsigned int lSize = m_lCoordsSizeCur;
                for (unsigned int i = 0; i < lSize; i += 2)
				{
					if (bIsClipping)
					{
						oBounds.CheckPoint(m_pCoordsArray[i], m_pCoordsArray[i + 1]);
					}

					m_pCoordsArray[i] *= m_dCoordsScaleX;
					m_pCoordsArray[i + 1] *= m_dCoordsScaleY;
				}
			}
			else
			{
				if (!bIsClipping && m_lPathTypesSizeCur <= 4 && !m_bIsCurveToExist)
				{
                    unsigned int lSize = m_lCoordsSizeCur;
                    for (unsigned int i = 0; i < lSize; i += 2)
					{
						m_pCoordsArray[i] *= m_dCoordsScaleX;
						m_pCoordsArray[i + 1] *= m_dCoordsScaleY;
					}

					switch (m_lPathTypesSizeCur)
					{
					case 2:
						{
							if (0 == m_pPathTypes[0] && 1 == m_pPathTypes[1])
							{
								m_oPath.AddSize(60);
								m_oPath.AddCharNoCheck('M');
								m_oPath.AddSpaceNoCheck();

								m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[0]));
								m_oPath.AddCharNoCheck(',');
								m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[1]));
								m_oPath.AddSpaceNoCheck();

								m_oPath.AddCharNoCheck('L');
								m_oPath.AddSpaceNoCheck();

								m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[2]));
								m_oPath.AddCharNoCheck(',');
								m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[3]));
								m_oPath.AddSpaceNoCheck();
								return true;
							}
							break;
						}
					case 3:
						{
							if (0 == m_pPathTypes[0] && 1 == m_pPathTypes[1])
							{
								if (3 == m_pPathTypes[2])									
								{
									m_oPath.AddSize(60);
									m_oPath.AddCharNoCheck('M');
									m_oPath.AddSpaceNoCheck();

									m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[0]));
									m_oPath.AddCharNoCheck(',');
									m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[1]));
									m_oPath.AddSpaceNoCheck();

									m_oPath.AddCharNoCheck('L');
									m_oPath.AddSpaceNoCheck();

									m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[2]));
									m_oPath.AddCharNoCheck(',');
									m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[3]));
									m_oPath.AddSpaceNoCheck();
									return true;
								}
								else
								{
									if ((fabs(m_pCoordsArray[4] - m_pCoordsArray[0]) < 0.2) && 
										(fabs(m_pCoordsArray[5] - m_pCoordsArray[1]) < 0.2))
									{
										m_oPath.AddSize(60);
										m_oPath.AddCharNoCheck('M');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[0]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[1]));
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddCharNoCheck('L');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[2]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[3]));
										m_oPath.AddSpaceNoCheck();
										return true;
									}
									else if ((fabs(m_pCoordsArray[2] - m_pCoordsArray[0]) < 0.2) && 
										(fabs(m_pCoordsArray[3] - m_pCoordsArray[1]) < 0.2))
									{
										m_oPath.AddSize(60);
										m_oPath.AddCharNoCheck('M');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[0]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[1]));
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddCharNoCheck('L');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[4]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[5]));
										m_oPath.AddSpaceNoCheck();
										return true;
									}
								}									
							}
							break;
						}
					case 4:
						{
							if (0 == m_pPathTypes[0] && 1 == m_pPathTypes[1] && 1 == m_pPathTypes[2])
							{
								if ((fabs(m_pCoordsArray[4] - m_pCoordsArray[0]) < 0.2) && 
									(fabs(m_pCoordsArray[5] - m_pCoordsArray[1]) < 0.2))
								{
									if (3 == m_pPathTypes[3])
									{
										m_oPath.AddSize(60);
										m_oPath.AddCharNoCheck('M');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[0]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[1]));
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddCharNoCheck('L');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[2]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[3]));
										m_oPath.AddSpaceNoCheck();
										return true;
									}
									else if (1 == m_pPathTypes[3] && ((fabs(m_pCoordsArray[6] - m_pCoordsArray[2]) < 0.2) && 
										(fabs(m_pCoordsArray[7] - m_pCoordsArray[3]) < 0.2)))
									{
										m_oPath.AddSize(60);
										m_oPath.AddCharNoCheck('M');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[0]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[1]));
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddCharNoCheck('L');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[2]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[3]));
										m_oPath.AddSpaceNoCheck();
										return true;
									}
								}
								else if ((fabs(m_pCoordsArray[2] - m_pCoordsArray[0]) < 0.2) && 
									(fabs(m_pCoordsArray[3] - m_pCoordsArray[1]) < 0.2))
								{
									if (3 == m_pPathTypes[3])
									{
										m_oPath.AddSize(60);
										m_oPath.AddCharNoCheck('M');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[0]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[1]));
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddCharNoCheck('L');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[4]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[5]));
										m_oPath.AddSpaceNoCheck();
										return true;
									}
									else if (1 == m_pPathTypes[3] && ((fabs(m_pCoordsArray[6] - m_pCoordsArray[2]) < 0.2) && 
										(fabs(m_pCoordsArray[7] - m_pCoordsArray[3]) < 0.2)))
									{
										m_oPath.AddSize(60);
										m_oPath.AddCharNoCheck('M');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[0]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[1]));
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddCharNoCheck('L');
										m_oPath.AddSpaceNoCheck();

										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[4]));
										m_oPath.AddCharNoCheck(',');
										m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[5]));
										m_oPath.AddSpaceNoCheck();
										return true;
									}
								}
							}
							break;
						}
					default:
						break;
					}
				}
				else
				{
                    unsigned int lSize = m_lCoordsSizeCur;
                    for (unsigned int i = 0; i < lSize; i += 2)
					{
						if (bIsClipping)
						{
							oBounds.CheckPoint(m_pCoordsArray[i], m_pCoordsArray[i + 1]);
						}

						m_pCoordsArray[i] *= m_dCoordsScaleX;
						m_pCoordsArray[i + 1] *= m_dCoordsScaleY;
					}
				}
			}

			if (bIsClipping)
				m_oTextClipBounds.Intersect(oBounds);

            unsigned int lCurrentCoord = 0;
            for (unsigned int nCommand = 0; nCommand < m_lPathTypesSizeCur; ++nCommand)
			{
				switch (m_pPathTypes[nCommand])
				{
				case 0:
					{
						m_oPath.AddSize(30);
						m_oPath.AddCharNoCheck('M');
						m_oPath.AddSpaceNoCheck();

						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddCharNoCheck(',');
						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddSpaceNoCheck();
						break;
					}
				case 1:
					{
						m_oPath.AddSize(30);
						m_oPath.AddCharNoCheck('L');
						m_oPath.AddSpaceNoCheck();

						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddCharNoCheck(',');
						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddSpaceNoCheck();
						break;
					}
				case 2:
					{
						m_oPath.AddSize(80);
						m_oPath.AddCharNoCheck('C');
						m_oPath.AddSpaceNoCheck();
						
						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddCharNoCheck(',');
						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddSpaceNoCheck();

						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddCharNoCheck(',');
						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddSpaceNoCheck();

						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddCharNoCheck(',');
						m_oPath.AddIntNoCheckDel10(round(10 * m_pCoordsArray[lCurrentCoord++]));
						m_oPath.AddSpaceNoCheck();
						break;
					}
				case 3:
					{
						m_oPath.AddSize(2);
						m_oPath.AddCharNoCheck('Z');
						m_oPath.AddSpaceNoCheck();
						break;
					}
				default:
					break;
				}
			}

			if (bIsClipping)
			{
				m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandStart);
				m_oClipMetafile.WriteLONG(CMetafile::ctBeginCommand, c_nClipType);			

				lCurrentCoord = 0;
				for (ULONG nCommand = 0; nCommand < m_lPathTypesSizeCur; ++nCommand)
				{
					switch (m_pPathTypes[nCommand])
					{
					case 0:
						{
							m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandMoveTo);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleX);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleY);
							break;
						}
					case 1:
						{
							m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandLineTo);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleX);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleY);
							break;
						}
					case 2:
						{
							m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandCurveTo);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleX);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleY);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleX);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleY);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleX);
							m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++] / m_dCoordsScaleY);
							break;
						}
					case 3:
						{
							m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandClose);
							break;
						}
					default:
						break;
					}
				}

                m_oClipMetafile.WriteLONG(CMetafile::ctEndCommand, c_nClipType2);
                m_oClipMetafile.WriteLONG(m_lClipMode);
				m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandEnd);
			}

			return false;
		}

		LONG WriteTempClip()
		{
			LONG nRet = m_oClipMetafile.GetPosition();

			m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandStart);
			m_oClipMetafile.WriteLONG(CMetafile::ctBeginCommand, c_nClipType);			

			LONG lCurrentCoord = 0;
			for (ULONG nCommand = 0; nCommand < m_lPathTypesSizeCur; ++nCommand)
			{
				switch (m_pPathTypes[nCommand])
				{
				case 0:
					{
						m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandMoveTo);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						break;
					}
				case 1:
					{
						m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandLineTo);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						break;
					}
				case 2:
					{
						m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandCurveTo);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oClipMetafile.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						break;
					}
				case 3:
					{
						m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandClose);
						break;
					}
				default:
					break;
				}
			}

            m_oClipMetafile.WriteLONG(CMetafile::ctEndCommand, c_nClipType2);
            m_oClipMetafile.WriteLONG(m_lClipMode);
			m_oClipMetafile.WriteCommandType(CMetafile::ctPathCommandEnd);

			return nRet;
		}

#ifdef USE_SIMPLE_GRAPHICS_NOSVG

		void WriteToPathToSimpleVector()
		{			
			if (m_lPathTypesSizeCur <= 4 && !m_bIsCurveToExist)
			{
				switch (m_lPathTypesSizeCur)
				{
				case 2:
					{
						if (0 == m_pPathTypes[0] && 1 == m_pPathTypes[1])
						{
							m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
							m_oVectors.WriteDouble(m_pCoordsArray[0]);
							m_oVectors.WriteDouble(m_pCoordsArray[1]);

							m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
							m_oVectors.WriteDouble(m_pCoordsArray[2]);
							m_oVectors.WriteDouble(m_pCoordsArray[3]);
							return;
						}
						break;
					}
				case 3:
					{
						if (0 == m_pPathTypes[0] && 1 == m_pPathTypes[1])
						{
							if (3 == m_pPathTypes[2])									
							{
								m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
								m_oVectors.WriteDouble(m_pCoordsArray[0]);
								m_oVectors.WriteDouble(m_pCoordsArray[1]);

								m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
								m_oVectors.WriteDouble(m_pCoordsArray[2]);
								m_oVectors.WriteDouble(m_pCoordsArray[3]);
								return;
							}
							else
							{
								if ((fabs(m_pCoordsArray[4] - m_pCoordsArray[0]) < 0.2) && 
									(fabs(m_pCoordsArray[5] - m_pCoordsArray[1]) < 0.2))
								{
									m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
									m_oVectors.WriteDouble(m_pCoordsArray[0]);
									m_oVectors.WriteDouble(m_pCoordsArray[1]);

									m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
									m_oVectors.WriteDouble(m_pCoordsArray[2]);
									m_oVectors.WriteDouble(m_pCoordsArray[3]);
									return;
								}
								else if ((fabs(m_pCoordsArray[2] - m_pCoordsArray[0]) < 0.2) && 
									(fabs(m_pCoordsArray[3] - m_pCoordsArray[1]) < 0.2))
								{
									m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
									m_oVectors.WriteDouble(m_pCoordsArray[0]);
									m_oVectors.WriteDouble(m_pCoordsArray[1]);

									m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
									m_oVectors.WriteDouble(m_pCoordsArray[4]);
									m_oVectors.WriteDouble(m_pCoordsArray[5]);
									return;
								}
							}									
						}
						break;
					}
				case 4:
					{
						if (0 == m_pPathTypes[0] && 1 == m_pPathTypes[1] && 1 == m_pPathTypes[2])
						{
							if ((fabs(m_pCoordsArray[4] - m_pCoordsArray[0]) < 0.2) && 
								(fabs(m_pCoordsArray[5] - m_pCoordsArray[1]) < 0.2))
							{
								if (3 == m_pPathTypes[3])
								{
									m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
									m_oVectors.WriteDouble(m_pCoordsArray[0]);
									m_oVectors.WriteDouble(m_pCoordsArray[1]);

									m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
									m_oVectors.WriteDouble(m_pCoordsArray[2]);
									m_oVectors.WriteDouble(m_pCoordsArray[3]);
									return;
								}
								else if (1 == m_pPathTypes[3] && ((fabs(m_pCoordsArray[6] - m_pCoordsArray[2]) < 0.2) && 
									(fabs(m_pCoordsArray[7] - m_pCoordsArray[3]) < 0.2)))
								{
									m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
									m_oVectors.WriteDouble(m_pCoordsArray[0]);
									m_oVectors.WriteDouble(m_pCoordsArray[1]);

									m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
									m_oVectors.WriteDouble(m_pCoordsArray[2]);
									m_oVectors.WriteDouble(m_pCoordsArray[3]);
									return;
								}
							}
							else if ((fabs(m_pCoordsArray[2] - m_pCoordsArray[0]) < 0.2) && 
								(fabs(m_pCoordsArray[3] - m_pCoordsArray[1]) < 0.2))
							{
								if (3 == m_pPathTypes[3])
								{
									m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
									m_oVectors.WriteDouble(m_pCoordsArray[0]);
									m_oVectors.WriteDouble(m_pCoordsArray[1]);

									m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
									m_oVectors.WriteDouble(m_pCoordsArray[4]);
									m_oVectors.WriteDouble(m_pCoordsArray[5]);
									return;
								}
								else if (1 == m_pPathTypes[3] && ((fabs(m_pCoordsArray[6] - m_pCoordsArray[2]) < 0.2) && 
									(fabs(m_pCoordsArray[7] - m_pCoordsArray[3]) < 0.2)))
								{
									m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
									m_oVectors.WriteDouble(m_pCoordsArray[0]);
									m_oVectors.WriteDouble(m_pCoordsArray[1]);

									m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
									m_oVectors.WriteDouble(m_pCoordsArray[4]);
									m_oVectors.WriteDouble(m_pCoordsArray[5]);
									return;
								}
							}
						}
						break;
					}
				default:
					break;
				}
			}

            unsigned int lCurrentCoord = 0;
            for (unsigned int nCommand = 0; nCommand < m_lPathTypesSizeCur; ++nCommand)
			{
				switch (m_pPathTypes[nCommand])
				{
				case 0:
					{
						m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						break;
					}
				case 1:
					{
						m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						break;
					}
				case 2:
					{
						m_oVectors.WriteCommandType(CMetafile::ctPathCommandCurveTo);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						m_oVectors.WriteDouble(m_pCoordsArray[lCurrentCoord++]);
						break;
					}
				case 3:
					{
						m_oVectors.WriteCommandType(CMetafile::ctPathCommandClose);
						break;
					}
				default:
					break;
				}
			}			
		}

#endif

		void CheckPathTextRect(CDoubleBounds& oBounds)
		{
            unsigned int lSize = m_lCoordsSizeCur;
            for (unsigned int i = 0; i < lSize; i += 2)
			{				
				oBounds.CheckPoint(m_pCoordsArray[i] * m_dCoordsScaleX, m_pCoordsArray[i + 1] * m_dCoordsScaleY);
			}
		}

        inline void NewSVG()
		{
			m_oDocument.SetCurSize(m_lEmtyDocChecker);

			#ifdef USE_SIMPLE_GRAPHICS_NOSVG
			
			if (m_bIsSimpleSetupBrush && !m_bIsSimpleGraphics)
			{
				m_pLastBrush->Color1 = m_lBrushColorOld;
				m_pLastBrush->Alpha1 = m_lBrushAlphaOld;				
			}

			m_oVectors.ClearNoAttack();
			m_bIsSimpleGraphics = true;
			m_bIsSimpleSetupBrush = false;

			#endif
		}
	};
}

#endif // _ASC_HTMLRENDERER_SVGWRITER2_H_
