#pragma once
#include "..\stdafx.h"
#include "SVGWriter.h"

#define SVG_INLINE_MAX_SIZE		500000 // 500Kb
#define SVG_TO_RASTER_MIN_SIZE 50000000 // 1Mb

namespace NSHtmlRenderer
{
	#define USE_SIMPLE_GRAPHICS_NOSVG
	#define USE_BIG_GRAPHICS_TORASTER

	using namespace NSStrings;

	class CRendererGr
	{
	public:
		Graphics::IASCGraphicsRenderer*		m_pRenderer;
		MediaCore::IAVSUncompressedVideoFrame*	m_pFrame;

		Graphics::IASCMetafile*				m_pMetafile;		
		
		double							m_dWidth;
		double							m_dHeight;

		LONG							m_lWidthPix;
		LONG							m_lHeightPix;

		bool							m_bIsRasterNeed;

	public:
		CRendererGr()
		{
			m_pRenderer			= NULL;
			m_pFrame			= NULL;
			m_pMetafile			= NULL;

			CoCreateInstance(Graphics::CLSID_CASCMetafile, NULL, CLSCTX_ALL, Graphics::IID_IASCMetafile, (void**)&m_pMetafile);			
			
			m_dWidth			= -1;
			m_dHeight			= -1;

			m_lWidthPix			= -1;
			m_lHeightPix		= -1;
		}
		~CRendererGr()
		{
			RELEASEINTERFACE(m_pRenderer);
			RELEASEINTERFACE(m_pFrame);	
			RELEASEINTERFACE(m_pMetafile);
		}

		AVSINLINE void UncheckRaster()
		{
			m_bIsRasterNeed = false;

			VARIANT var;
			m_pMetafile->SetAdditionalParam(L"ClearNoAttack", var);
		}
		AVSINLINE void CheckRasterNeed(const bool& bIsRaster)
		{
			if (!m_bIsRasterNeed)
				m_bIsRasterNeed = bIsRaster;
		}

		void CheckRaster(double dWidth, double dHeight)
		{
			if (dWidth != m_dWidth || dHeight != m_dHeight)
			{
				RELEASEINTERFACE(m_pFrame);				

				m_dWidth	= dWidth;
				m_dHeight	= dHeight;
			}

			RELEASEINTERFACE(m_pRenderer);

			m_lWidthPix		= (LONG)(96 * m_dWidth / 25.4);
			m_lHeightPix	= (LONG)(96 * m_dHeight / 25.4);

			if (NULL == m_pFrame)
			{
				CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pFrame);
				
				m_pFrame->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
				m_pFrame->put_Width( m_lWidthPix );
				m_pFrame->put_Height( m_lHeightPix );
				m_pFrame->put_AspectRatioX( m_lWidthPix );
				m_pFrame->put_AspectRatioY( m_lHeightPix );
				m_pFrame->put_Interlaced( VARIANT_FALSE );
				m_pFrame->put_Stride( 0, 4 * m_lWidthPix );
				m_pFrame->AllocateBuffer( -1 );
			}

			BYTE* pBuffer = NULL;
			m_pFrame->get_Buffer(&pBuffer);

			memset(pBuffer, 0xFF, 4 * m_lWidthPix * m_lHeightPix);

			CoCreateInstance(Graphics::CLSID_CASCGraphicsRenderer, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicsRenderer, (void**)&m_pRenderer);
			
			m_pRenderer->put_Width(m_dWidth);
			m_pRenderer->put_Height(m_dHeight);
			m_pRenderer->CreateFromMediaData((IUnknown*)m_pFrame, 0, 0, m_lWidthPix, m_lHeightPix );
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
				return NULL;

			BYTE* pBuffer = NULL;
			m_pFrame->get_Buffer(&pBuffer);

			RECT rect = GetImageBounds(m_pFrame);

			if (((rect.right - rect.left) < 5) && ((rect.bottom - rect.top) < 5))
				return NULL;

			BYTE* pBufferSrcMem = pBuffer + 4 * rect.top * m_lWidthPix + 4 * rect.left;
			LONG lWidthShape	= rect.right - rect.left + 1;
			LONG lHeightShape	= rect.bottom - rect.top + 1;

			MediaCore::IAVSUncompressedVideoFrame* pShapePicture = NULL;
			CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, 
									MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pShapePicture);

			pShapePicture->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
			pShapePicture->put_Width( lWidthShape );
			pShapePicture->put_Height( lHeightShape );
			pShapePicture->put_AspectRatioX( lWidthShape );
			pShapePicture->put_AspectRatioY( lHeightShape );
			pShapePicture->put_Interlaced( VARIANT_FALSE );
			pShapePicture->put_Stride( 0, 4 * lWidthShape );
			pShapePicture->AllocateBuffer( -1 );

			BYTE* pBufferDst = NULL;
			pShapePicture->get_Buffer(&pBufferDst);
			
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

			double dHeightMM	= 25.4 * m_lHeight / 96.0;
			dT = (dHeightMM - dT - dH);
			
			pPage->WriteImage((IUnknown*)pShapePicture, dL, dT, dW, dH);

			RELEASEINTERFACE(pShapePicture);
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

		AVSINLINE void CheckPoint(const double& _x, const double& _y)
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

		AVSINLINE void Intersect(const CDoubleBounds& oBounds)
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

		NSStructures::CPen*				m_pLastPen;
		NSStructures::CBrush*			m_pLastBrush;

		CMatrix*						m_pTransform;

		int								m_lWidth;
		int								m_lHeight;

		double							m_dDpiX;
		double							m_dDpiY;

		CClipSVG2						m_oClip;

		// здесь храним пат в координатах не трансформированных 
		// (чтобы, если заливки сложные - можно было делать трансформы
		// непосредственно при использовании графического пути).
		double*							m_pCoordsArray;
		ULONG							m_lCoordsSize;
		double*							m_pCoordsArrayCur;
		ULONG							m_lCoordsSizeCur;

		bool							m_bIsCurveToExist;

		BYTE*							m_pPathTypes;
		ULONG							m_lPathTypesSize;
		BYTE*							m_pPathTypesCur;
		ULONG							m_lPathTypesSizeCur;

		double							m_dCoordsScaleX;
		double							m_dCoordsScaleY;

		ULONG							m_lEmtyDocChecker;

		BYTE*							m_pBase64Code;
		CDoubleBounds					m_oTextClipBounds;

		// переменная говорит о том, какой клип для текста записан сейчас
		// если true - то послана команда ResetTextClipRect
		// если false - то нет
		bool							m_bIsTextClipWriteCleared;
		// были ли новые clip'ы
		bool							m_bIsIntersectNewClipRect;

		// сохранение в растр. (если сложная заливка, или слишком большая векторная графика)
		//CRendererGr						m_oGrRenderer;
		//Graphics::IASCMetafile*		m_pGrRenderer;

		// клип для картинок. И для конвертации сложной векторной графики в растр
		CMetafile						m_oClipMetafile;

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

		void CloseFile(CString strFile = _T(""))
		{
			if (_T("") != strFile)
			{
				m_oDocument.WriteString(g_svg_bstr_svgClose);
				//CDirectory::SaveToFile(strFile, m_oDocument.GetCString());
				CFile oFile;
				oFile.CreateFile(strFile);
				CStringA strA(m_oDocument.GetBuffer(), (int)m_oDocument.GetCurSize());
				oFile.WriteFile(strA.GetBuffer(), strA.GetLength());
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

		BSTR GetSVGXml()
		{
			m_oClip.WriteEnd(m_oDocument);
			m_oDocument.WriteString(g_svg_bstr_svgClose);
			
			CStringA strA(m_oDocument.GetBuffer(), (int)m_oDocument.GetCurSize());
			BSTR ret = strA.AllocSysString();
			return ret;
		}

		void CloseFile2(CString strFile, bool bIsNeedEnd = true)
		{
			if (bIsNeedEnd)
			{
				m_oClip.WriteEnd(m_oDocument);
				m_oDocument.WriteString(g_svg_bstr_svgClose);
			}

			CFile oFile;
			oFile.CreateFile(strFile);
			CStringA strA(m_oDocument.GetBuffer(), (int)m_oDocument.GetCurSize());
			oFile.WriteFile(strA.GetBuffer(), strA.GetLength());

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
				m_oDocument.WriteString(g_svg_bstr_svgClose);
			}

			CStringA strA(m_oDocument.GetBuffer(), (int)m_oDocument.GetCurSize());
			int nSize = strA.GetLength();

			int nOutputLen = 2 * SVG_INLINE_MAX_SIZE;
			Base64Encode((BYTE*)strA.GetBuffer(), nSize, (LPSTR)m_pBase64Code, &nOutputLen, ATL_BASE64_FLAG_NOCRLF);

			m_pMeta->WriteLONG(nOutputLen);
			m_pMeta->Write(m_pBase64Code, nOutputLen);

			m_oDocument.ClearNoAttack();
			m_oPath.ClearNoAttack();

			if (m_bIsClipping)
			{
				m_bIsNeedUpdateClip = true;
			}

			m_lPatternID	= 0;

			return nOutputLen;
		}

		void NewDocument(const double& dWidth, const double& dHeigth, const LONG& lPageNumber)
		{
			m_lWidth  = (int)(dWidth * m_dCoordsScaleX);
			m_lHeight = (int)(dHeigth * m_dCoordsScaleY);

			m_oClip.m_lWidth	= m_lWidth;
			m_oClip.m_lHeight	= m_lHeight;

			m_lCurDocumentID = lPageNumber;

			CString strSVG = _T("");
			strSVG.Format(g_svg_string_svgOpen, m_lWidth, m_lHeight, m_lWidth, m_lHeight);

			m_oDocument.ClearNoAttack();
			m_oDocument.WriteString(strSVG);

			m_oClip.Clear();

			m_lClippingPath = 0;
			m_bIsClipping = false;
			m_bIsNeedUpdateClip = false;

			m_lEmtyDocChecker = (ULONG)m_oDocument.GetCurSize();

			//m_pGrRenderer = m_oGrRenderer.m_pMetafile;
		}
	protected:
		inline static LONG ConvertColor(LONG lBGR)
		{
			return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
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
		void WriteDrawPath(LONG nType, Graphics::IASCGraphicSimpleComverter* pConverter, CImageInfo& oInfo)
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

			BOOL bStroke	= (0x01 == (0x01 & nType));

			if (nPenW == 0 && bStroke)
				nPenW = 1;

			BOOL bFill		= (0x01 < nType);
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
				double w = 0;
				double h = 0;
				pConverter->PathCommandGetBounds(&x, &y, &w, &h);

				if (m_pBrush->TextureMode == c_BrushTextureModeStretch || true)
				{
					// 1) пишем паттерн
					agg::trans_affine* mtx = &m_pTransform->m_agg_mtx;
					double _tx = mtx->tx * m_dCoordsScaleX;
					double _ty = mtx->ty * m_dCoordsScaleY;

					CString strPatt = _T("");
					
					double _w = w * m_dCoordsScaleX;
					double _h = h * m_dCoordsScaleY;
					if (itJPG == oInfo.m_eType)
					{
						strPatt.Format(g_svg_string_pattern_jpg_mtx, m_lPatternID, round(_w), round(_h), round(_w), round(_h),
							mtx->sx, mtx->shy, mtx->shx, mtx->sy, _tx, _ty, round(_w), round(_h), oInfo.m_lID);
					}
					else
					{
						strPatt.Format(g_svg_string_pattern_png_mtx, m_lPatternID, round(_w), round(_h), round(_w), round(_h),
							mtx->sx, mtx->shy, mtx->shx, mtx->sy, _tx, _ty, round(_w), round(_h), oInfo.m_lID);
					}
					m_oDocument.WriteString(strPatt);
				}
				else
				{
					// TODO:
				}

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
			{
				strMode = _T("evenodd");
				
				#ifdef USE_SIMPLE_GRAPHICS_NOSVG
				m_bIsSimpleGraphics = false;
				#endif
			}
			
			if (!bStroke)
			{
				if (bIsLine)
				{
					CString strStyle = _T("");
					strStyle.Format(g_svg_string_vml_StyleStroke, nColorBrush, 1, (double)m_pBrush->Alpha1 / 255);

					m_oDocument.WriteString(g_svg_bstr_vml_Path);
					m_oDocument.WriteString(strStyle);
					WriteStyleClip();
					m_oDocument.WriteString(g_svg_bstr_path_d);
					m_oDocument.Write(m_oPath);
					m_oDocument.WriteString(g_svg_bstr_path_d_end);
					m_oDocument.WriteString(g_svg_bstr_nodeClose);
					return;
				}

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

			m_oClip.m_arPaths.Add(m_oPath.GetCString());
			m_oClip.m_arTypes.Add(m_lClipMode);
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
			BOOL bIsClipping = FALSE;
			if ((1 < h) && (1 < w))
			{
				WriteClip();
				bIsClipping = m_bIsClipping;
			}

			double dCentreX = x + w / 2.0;
			double dCentreY = y + h / 2.0;

			bool bIsNoNeedTransform = m_pTransform->m_agg_mtx.is_identity_main(0.0001);

			if (bIsNoNeedTransform)
			{
				double _x = x + m_pTransform->m_agg_mtx.tx;
				double _y = y + m_pTransform->m_agg_mtx.ty;

				_x *= m_dCoordsScaleX;
				_y *= m_dCoordsScaleY;

				double _w = w * m_dCoordsScaleX;
				double _h = h * m_dCoordsScaleY;

				CString strImage = _T("");
				if (itJPG == oInfo.m_eType)
				{
					/*if (bIsClipping)
					{
						strImage.Format(g_svg_string_image_clip_jpg1, round(_x), round(_y), round(_w), round(_h), m_lClippingPath - 1, oInfo.m_lID);
					}
					else*/
					{
						strImage.Format(g_svg_string_image_jpg1, round(_x), round(_y), round(_w), round(_h), oInfo.m_lID);
					}
				}
				else
				{
					/*if (bIsClipping)
					{
						strImage.Format(g_svg_string_image_clip_png1, round(_x), round(_y), round(_w), round(_h), m_lClippingPath - 1, oInfo.m_lID);
					}
					else*/
					{
						strImage.Format(g_svg_string_image_png1, round(_x), round(_y), round(_w), round(_h), oInfo.m_lID);
					}
				}

				m_oDocument.WriteString(strImage);
			}
			else
			{
				double _tx = m_pTransform->m_agg_mtx.tx * m_dCoordsScaleX;
				double _ty = m_pTransform->m_agg_mtx.ty * m_dCoordsScaleY;

				double _x = x * m_dCoordsScaleX;
				double _y = y * m_dCoordsScaleY;

				double _w = w * m_dCoordsScaleX;
				double _h = h * m_dCoordsScaleY;

				agg::trans_affine* mtx = &m_pTransform->m_agg_mtx;

				CString strImage = _T("");
				if (itJPG == oInfo.m_eType)
				{
					/*if (bIsClipping)
					{
						strImage.Format(g_svg_string_image_clip_jpg_mtx, _x, _y, _w, _h, m_lClippingPath - 1, oInfo.m_lID, mtx->sx, mtx->shy, mtx->shx, mtx->sy, _tx, _ty);
					}
					else*/
					{
						strImage.Format(g_svg_string_image_jpg_mtx, _x, _y, _w, _h, oInfo.m_lID, mtx->sx, mtx->shy, mtx->shx, mtx->sy, _tx, _ty);
					}
				}
				else
				{
					/*if (bIsClipping)
					{
						strImage.Format(g_svg_string_image_clip_png_mtx, _x, _y, _w, _h, m_lClippingPath - 1, oInfo.m_lID, mtx->sx, mtx->shy, mtx->shx, mtx->sy, _tx, _ty);
					}
					else*/
					{
						strImage.Format(g_svg_string_image_png_mtx, _x, _y, _w, _h, oInfo.m_lID, mtx->sx, mtx->shy, mtx->shx, mtx->sy, _tx, _ty);
					}
				}

				m_oDocument.WriteString(strImage);
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
				CString strClip = _T("");
				strClip.Format(g_svg_string_clip, m_lClippingPath - 1);
				m_oDocument.WriteString(strClip);
			}
		}

		void CheckSizeVectorB(const int& len = 1)
		{
			if (NULL != m_pPathTypes)
			{
				ULONG nNewSize = (ULONG)(m_lPathTypesSizeCur + len);
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
				ULONG nNewSize = (ULONG)(m_lCoordsSizeCur + len);
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
				ULONG lSize = m_lCoordsSizeCur;
				for (ULONG i = 0; i < lSize; i += 2)
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
					ULONG lSize = m_lCoordsSizeCur;
					for (ULONG i = 0; i < lSize; i += 2)
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
					ULONG lSize = m_lCoordsSizeCur;
					for (ULONG i = 0; i < lSize; i += 2)
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

			ULONG lCurrentCoord = 0;
			for (ULONG nCommand = 0; nCommand < m_lPathTypesSizeCur; ++nCommand)
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

				m_oClipMetafile.WriteLONG(CMetafile::ctEndCommand, c_nClipType);
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

			m_oClipMetafile.WriteLONG(CMetafile::ctEndCommand, c_nClipType);
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

			ULONG lCurrentCoord = 0;
			for (ULONG nCommand = 0; nCommand < m_lPathTypesSizeCur; ++nCommand)
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
			ULONG lSize = m_lCoordsSizeCur;
			for (ULONG i = 0; i < lSize; i += 2)
			{
				
				oBounds.CheckPoint(m_pCoordsArray[i] * m_dCoordsScaleX, m_pCoordsArray[i + 1] * m_dCoordsScaleY);
			}
		}

		AVSINLINE void NewSVG()
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