#pragma once
#include "Format.h"
#include "../Logic/LogicPage.h"

namespace NSDocx
{
	class CWriter
	{
	public:
		LONG			m_lCurrentPageIndex;
		LONG			m_lCountPages;

		ILogicPage*		m_pCurrentPage;
		CFormat			m_oFormat;	

		CString			m_strSrcFileName;

		bool			m_bIsSaveCover;
		bool			m_bIsEmbeddedFonts;
		bool			m_bIsSrcCoverExist;

		AVSGraphics::IAVSEffectPainter* m_pPainter;
	public:
		CWriter()
		{
			m_pCurrentPage		= NULL;
			m_pPainter			= NULL;

			m_lCurrentPageIndex	= -1;
			m_lCountPages		= 0;

			m_strSrcFileName	= _T("");

			m_bIsSaveCover		= true;
			m_bIsEmbeddedFonts	= true;
			m_bIsSrcCoverExist	= false;
		}
		~CWriter()
		{
			RELEASEINTERFACE(m_pPainter);
		}

		CRenderers* CreateDocument(LONG lSrcType, LONG lDstType)
		{
			RELEASEOBJECT(m_pCurrentPage);
			m_oFormat.Clear();
			m_oFormat.m_oTempManager.InitTempDirectory();
			if (0 == lSrcType)
			{
				m_pCurrentPage = new CLogicPageDOCX();
				m_pCurrentPage->m_pImageManager = &m_oFormat.m_oImageManager;
				return NULL;
			}
			else
			{
				m_pCurrentPage = new CLogicPagePDF();
				CLogicPagePDF* pPagePDF = (CLogicPagePDF*)m_pCurrentPage;
				m_pCurrentPage->m_pImageManager = &m_oFormat.m_oImageManager;
				return &pPagePDF->m_oRenderers;
			}
			return NULL;
		}

		void SetParametres(NSStructures::CPen* pPen, NSStructures::CBrush* pBrush, NSStructures::CFont* pFont,
			NSStructures::CShadow* pShadow, NSStructures::CEdgeText* pEdge, 
			AVSGraphics::IAVSGraphicSimpleComverter* pSimpleConverter, NSCommon::CMatrix* pTransform)
		{
			m_pCurrentPage->m_pPen		= pPen;
			m_pCurrentPage->m_pBrush	= pBrush;
			m_pCurrentPage->m_pFont		= pFont;
			m_pCurrentPage->m_pShadow	= pShadow;
			m_pCurrentPage->m_pEdgeText	= pEdge;

			m_pCurrentPage->m_pSimpleGraphicsConverter	= pSimpleConverter;
			m_pCurrentPage->m_pTransform				= pTransform;

			m_pCurrentPage->InitProp();

			m_oFormat.m_oImageManager.m_strDstMedia		= m_oFormat.m_oTempManager.m_strTempImagesDirectory;
		}

		void CloseFile()
		{
			RELEASEOBJECT(m_pCurrentPage);
			m_oFormat.Clear();
			m_oFormat.m_oTempManager.RemoveTempDirectory();
		}
		void EndFile()
		{
			m_pCurrentPage->CloseFile(m_oFormat);
		}

		void BeginPage(double dWidth, double dHeight)
		{
			++m_lCurrentPageIndex;
			if (0 != m_lCurrentPageIndex)
				m_pCurrentPage->Clear();

			m_pCurrentPage->m_dWidth = dWidth;
			m_pCurrentPage->m_dHeight = dHeight;
			m_pCurrentPage->BeginPage();

			m_pCurrentPage->m_pPen->SetDefaultParams();
			m_pCurrentPage->m_pBrush->SetDefaultParams();
			m_pCurrentPage->m_pFont->SetDefaultParams();
		}
		void EndPage()
		{
			m_pCurrentPage->EndPage();
			m_pCurrentPage->ConvertToDocxPage(m_oFormat);

			m_pCurrentPage->Clear();
		}

	public:
		AVSINLINE void MoveTo(double& x, double& y)
		{
			m_pCurrentPage->MoveTo(x, y);
		}
		AVSINLINE void LineTo(double& x, double& y)
		{
			m_pCurrentPage->LineTo(x, y);
		}
		AVSINLINE void CurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			m_pCurrentPage->CurveTo(x1, y1, x2, y2, x3, y3);
		}
		AVSINLINE void Close()
		{
			m_pCurrentPage->Close();
		}
		AVSINLINE void BeginPath()
		{
			m_pCurrentPage->BeginPath();
		}
		AVSINLINE void EndPath()
		{
			m_pCurrentPage->EndPath();
		}
		AVSINLINE void WriteText(BSTR bsText, BSTR bsGid, double& x, double& y, double& width, double& height, double& baselineoffset)
		{
			m_pCurrentPage->WriteText(bsText, bsGid, x, y, width, height, baselineoffset);
		}
		AVSINLINE void WriteImage(IUnknown* punkImage, double& x, double& y, double& width, double& height)
		{
			size_t nCountImages = m_oFormat.m_oImageManager.m_listImages.GetCount();
			if (m_bIsSaveCover && (0 == nCountImages))
			{
				// делаем ковер
				double _x = 0;
				double _y = 0;
				double _w = 210;
				double _h = 297;
				m_oFormat.m_oImageManager.WriteImage(punkImage, _x, _y, _w, _h);
				return;
			}
			else if ((0 == nCountImages) && m_bIsSrcCoverExist)
			{
				// это ковер, а его сохранять не нужно
				return;
			}

			m_pCurrentPage->WriteImage(punkImage, x, y, width, height);
		}
		AVSINLINE void WriteImage(CString strFilePath, double& x, double& y, double& width, double& height)
		{
			size_t nCountImages = m_oFormat.m_oImageManager.m_listImages.GetCount();
			if (m_bIsSaveCover && (0 == nCountImages))
			{
				// делаем ковер
				double _x = 0;
				double _y = 0;
				double _w = 210;
				double _h = 297;
				m_oFormat.m_oImageManager.WriteImage(strFilePath, _x, _y, _w, _h);
				return;
			}
			else if ((0 == nCountImages) && m_bIsSrcCoverExist)
			{
				// это ковер, а его сохранять не нужно
				return;
			}

			m_pCurrentPage->WriteImage(strFilePath, x, y, width, height);
		}
		AVSINLINE void WriteTable(NSDocx::ILogicItem* pItem)
		{
			m_pCurrentPage->WriteTable(pItem);
		}
		AVSINLINE void BeginShape()
		{
			m_pCurrentPage->BeginShape();
		}
		AVSINLINE void EndShape()
		{
			m_pCurrentPage->EndShape();
		}
		AVSINLINE void DrawPath(long nType)
		{
			m_pCurrentPage->DrawPath(nType);
		}

	public:
		void InitWatermark()
		{
			if (NULL == m_pCurrentPage)
				return;

			if (NULL == m_pPainter)
			{
				m_pCurrentPage->m_oInfoWatermark.m_lID	= -1;
				m_pCurrentPage->m_oRectWatermark.left	= 0;
				m_pCurrentPage->m_oRectWatermark.top	= 0;
				m_pCurrentPage->m_oRectWatermark.right	= 0;
				m_pCurrentPage->m_oRectWatermark.bottom	= 0;
				
				return;
			}
			
			LONG lWidthPix	= 400;
			LONG lHeightPix	= 75;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, 
				MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			pFrame->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
			pFrame->put_Width( lWidthPix );
			pFrame->put_Height( lHeightPix );
			pFrame->put_AspectRatioX( lWidthPix );
			pFrame->put_AspectRatioY( lHeightPix );
			pFrame->put_Interlaced( VARIANT_FALSE );
			pFrame->put_Stride( 0, 4 * lWidthPix );
			pFrame->AllocateBuffer( -1 );

			BYTE* pBuffer = NULL;
			pFrame->get_Buffer(&pBuffer);
			memset(pBuffer, 0xFF, 4 * lWidthPix * lHeightPix);

			AVSGraphics::IAVSGraphicsRenderer* pRenderer = NULL;
			CoCreateInstance(AVSGraphics::CLSID_CAVSGraphicsRenderer, NULL, CLSCTX_ALL, 
				AVSGraphics::IID_IAVSGraphicsRenderer, (void**)&pRenderer);

			//ставим FontManager
			VARIANT vtVariant;
			vtVariant.vt = VT_UNKNOWN;
			vtVariant.punkVal = NULL;
			pRenderer->SetAdditionalParam( L"FontManager", vtVariant );

			IUnknown* punkWatermark = NULL;
			pFrame->QueryInterface(IID_IUnknown, (void**)&punkWatermark);

			pRenderer->put_Width(140);
			pRenderer->put_Height(30);
			pRenderer->CreateFromMediaData(punkWatermark, 0, 0, lWidthPix, lHeightPix);

			m_pPainter->Draw((IUnknown*)pRenderer);

			RELEASEINTERFACE(pFrame);
			RELEASEINTERFACE(pRenderer);

			m_pCurrentPage->m_oRectWatermark.left	= 0;
			m_pCurrentPage->m_oRectWatermark.top	= 0;
			m_pCurrentPage->m_oRectWatermark.right	= 140;
			m_pCurrentPage->m_oRectWatermark.bottom	= 30;

			double _x = 0;
			double _y = 0;
			double _w = 140;
			double _h = 30;

			m_pCurrentPage->m_oInfoWatermark = m_oFormat.m_oImageManager.WriteImage(punkWatermark, _x, _y, _w, _h);
		}
	};
}