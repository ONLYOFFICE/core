#pragma once
#include "Format.h"
#include "../Logic/LogicPage.h"

namespace NSEBook
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

	public:
		CWriter()
		{
			m_pCurrentPage		= NULL;

			m_lCurrentPageIndex	= -1;
			m_lCountPages		= 0;

			m_strSrcFileName	= _T("");

			m_bIsSaveCover		= true;
			m_bIsEmbeddedFonts	= true;
			m_bIsSrcCoverExist	= false;
		}
		~CWriter()
		{
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
			ASCGraphics::IASCGraphicSimpleComverter* pSimpleConverter, NSCommon::CMatrix* pTransform)
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
			m_pCurrentPage->ConvertToEbookPage(m_oFormat);

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
		AVSINLINE void WriteTable(NSEBook::ILogicItem* pItem)
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
	};
}