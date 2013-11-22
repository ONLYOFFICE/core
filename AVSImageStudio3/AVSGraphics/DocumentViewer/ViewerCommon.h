#pragma once
#include "..\stdafx.h"
#include "atlcoll.h"
#include "TemporaryCS.h"

#include "..\Interfaces\AVSMetafile.h"
#include "DIB.h"
#include "..\Interfaces\XmlUtils.h"

namespace NSDocumentViewer
{
	// класс Гиперссылка
	class CHyperlinkEvent
	{
	public:
		virtual void HyperlinkURL(CString& strUrl)							= 0;
		virtual void HyperlinkPlace(LONG& lPageNum, CDoubleRect& oRect)		= 0;
	};

	class CHyperlink
	{
	public:
		CDoubleRect m_oBoundsSrc;
		CDoubleRect m_oBoundsDst;

		CString m_strURL;

		LONG m_lSrcPageIndex;
		LONG m_lDstPageIndex;

		BOOL m_bIsActivate;

	public:
		CHyperlink() : m_oBoundsSrc(), m_oBoundsDst()
		{
			m_strURL		= _T("");
			
			m_lSrcPageIndex = -1;
			m_lDstPageIndex = -1;

			m_bIsActivate  = FALSE;
		}
		CHyperlink(const CHyperlink& oSrc)
		{
			*this = oSrc;
		}
		~CHyperlink()
		{
		}
		CHyperlink& operator=(const CHyperlink& oSrc)
		{
			m_oBoundsSrc	= oSrc.m_oBoundsSrc;
			m_oBoundsDst	= oSrc.m_oBoundsDst;

			m_strURL		= oSrc.m_strURL;
			m_lDstPageIndex = oSrc.m_lDstPageIndex;
			m_lSrcPageIndex = oSrc.m_lSrcPageIndex;

			m_bIsActivate	= oSrc.m_bIsActivate;

			return *this;
		}

	public:
		inline void FromXml(CString& strXml)
		{
			XmlUtils::CXmlNode oNode;
			if (oNode.FromXmlString(strXml))
			{
				FromXml(oNode);
			}
		}
		inline void FromXml(XmlUtils::CXmlNode& oNodeLink)
		{
			XmlUtils::CXmlNode oNodeSource;
			if (oNodeLink.GetNode(_T("source"), oNodeSource))
			{
				m_lSrcPageIndex = XmlUtils::GetInteger(oNodeSource.GetAttributeOrValue(_T("page")));

				m_oBoundsSrc.left	= XmlUtils::GetDouble(oNodeSource.GetAttributeOrValue(_T("x")));
				m_oBoundsSrc.top 	= XmlUtils::GetDouble(oNodeSource.GetAttributeOrValue(_T("y")));

				m_oBoundsSrc.right	= m_oBoundsSrc.left + XmlUtils::GetDouble(oNodeSource.GetAttributeOrValue(_T("width")));
				m_oBoundsSrc.bottom = m_oBoundsSrc.top  + XmlUtils::GetDouble(oNodeSource.GetAttributeOrValue(_T("height")));
			}
			XmlUtils::CXmlNode oNodeTarget;
			if (oNodeLink.GetNode(_T("target"), oNodeTarget))
			{
				m_strURL = oNodeTarget.GetAttributeOrValue(_T("url"));

				if (_T("") == m_strURL)
				{		
					m_lDstPageIndex = XmlUtils::GetInteger(oNodeTarget.GetAttributeOrValue(_T("page")));

					m_oBoundsDst.left	= XmlUtils::GetDouble(oNodeTarget.GetAttributeOrValue(_T("x")));
					m_oBoundsDst.top 	= XmlUtils::GetDouble(oNodeTarget.GetAttributeOrValue(_T("y")));

					m_oBoundsDst.right	= m_oBoundsSrc.left + XmlUtils::GetDouble(oNodeTarget.GetAttributeOrValue(_T("width")));
					m_oBoundsDst.bottom = m_oBoundsSrc.top  + XmlUtils::GetDouble(oNodeTarget.GetAttributeOrValue(_T("height")));
				}
			}
		}

		inline BOOL IsActivate(const double& x, const double& y)
		{
			m_bIsActivate = m_oBoundsSrc.IsPointInside(x, y);
			return m_bIsActivate;
		}
		inline BOOL Activate(const double& x, const double& y, CHyperlinkEvent* pEvent)
		{
			BOOL bOld = m_bIsActivate;

			m_bIsActivate = IsActivate(x, y);

			if (bOld && m_bIsActivate && NULL != pEvent)
			{
				if (_T("") == m_strURL)
				{
					pEvent->HyperlinkPlace(m_lDstPageIndex, m_oBoundsDst);
				}
				else
				{
					pEvent->HyperlinkURL(m_strURL);
				}
				return TRUE;
			}

			return FALSE;
		}
	};

	class CHyperlinks
	{
	public:
		CAtlArray<CHyperlink> m_arLinks;

	public:
		CHyperlinks() : m_arLinks()
		{
		}
		CHyperlinks(const CHyperlinks& oSrc)
		{
			*this = oSrc;
		}
		~CHyperlinks()
		{
		}
		CHyperlinks& operator=(const CHyperlinks& oSrc)
		{
			m_arLinks.Copy(oSrc.m_arLinks);
			return *this;
		}

		BOOL IsActivate(const double& x, const double& y)
		{
			size_t nCount = m_arLinks.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (m_arLinks[i].IsActivate(x, y))
					return TRUE;
			}
			return FALSE;
		}
		BOOL Activate(const double& x, const double& y, CHyperlinkEvent* pEvent)
		{
			size_t nCount = m_arLinks.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (m_arLinks[i].Activate(x, y, pEvent))
					return TRUE;
			}
			return FALSE;
		}
	};
	
	// класс страницы.
	// содержит только информацию о ней (размеры и команды)
	class CPage
	{
	public:
		double m_dWidthMM;
		double m_dHeightMM;

		LONG m_lPixWidth;
		LONG m_lPixHeight;

		LONG m_lShadowOffsetHor;
		LONG m_lShadowOffsetVer;

		// нужно для быстрой и удобной навигации по страницам
		double m_dPositionVer;

		// нужно для показа видимой страницы (табнейлы)
		double m_dPositionVer2;

		CHyperlinks m_oLinks;

		IAVSMetafile* m_pPage;

	public:
		CPage() : m_oLinks()
		{
			m_dWidthMM	= 210;
			m_dHeightMM	= 190;

			SetDpi(96.0, 96.0);

			m_lShadowOffsetHor = 3;
			m_lShadowOffsetVer = 3;

			m_dPositionVer = 0;
			m_dPositionVer2 = 0;

			m_pPage = NULL;
		}
		~CPage()
		{
			RELEASEINTERFACE(m_pPage);
		}

	public:
		inline void SetDpi(double dDpiX, double dDpiY)
		{
			m_lPixWidth		= (LONG)(dDpiX * m_dWidthMM  / c_dInch_to_MM);
			m_lPixHeight	= (LONG)(dDpiY * m_dHeightMM / c_dInch_to_MM);
		}
		inline void CalculatePix(const double& dScaleX, const double& dScaleY)
		{
			m_lPixWidth		= (LONG)(m_dWidthMM  * dScaleX);
			m_lPixHeight	= (LONG)(m_dHeightMM * dScaleY);
		}

		inline LONG Width()
		{
			return m_lPixWidth + m_lShadowOffsetHor;
		}
		inline LONG Height()
		{
			return m_lPixHeight + m_lShadowOffsetVer;
		}

		void Draw(IUnknown* punkRenderer, BOOL* pBreak)
		{
			if (NULL != m_pPage)
			{
				m_pPage->Draw(punkRenderer, pBreak);
			}
		}
		void Draw2(IUnknown* pRenderer, BOOL* pBreak)
		{
			if (NULL != m_pPage)
			{
				m_pPage->Draw2(pRenderer, -1, -1, pBreak);
			}
		}
	};

	// класс для отрисовки страницы
	// содержит страницу и ее рект для отрисовки
	class CDrawingPage
	{
	public:
		CPage* m_pPage;

		CDoubleRect m_oGlobalBounds;
		CDoubleRect m_oPageBounds;

		LONG m_lIndexInSource;

	public:

		CDrawingPage(CPage* pPage = NULL)
		{
			m_pPage		= pPage;
		}

		~CDrawingPage()
		{
		}

		double GetPercentVisibleVer(LONG lHeightControl)
		{
			double dH = m_oGlobalBounds.GetHeight();
			if (dH < 0.001)
				return 1.0;
			
			double dTop		= max(0, m_oGlobalBounds.top);
			double dBottom	= min((double)lHeightControl, m_oGlobalBounds.bottom);

			return (dBottom - dTop) / dH;
		}
		double GetPercentVisibleHor(LONG lWidthControl)
		{
			double dW = m_oGlobalBounds.GetWidth();
			if (dW < 0.001)
				return 1.0;
			
			double dLeft	= max(0, m_oGlobalBounds.left);
			double dRight	= min((double)lWidthControl, m_oGlobalBounds.right);

			return (dRight - dLeft) / dW;
		}

		void GetPercentsVisiblePage(double& dLeft, double& dTop, double& dWidth, double& dHeight, const LONG& lWidthControl, const LONG& lHeightControl)
		{
			double dWidthControl  = (double)lWidthControl;
			double dHeightControl = (double)lHeightControl;

			double dWidthPage  = max(0.001, m_oGlobalBounds.GetWidth());
			double dHeightPage = max(0.001, m_oGlobalBounds.GetHeight());

			// left
			dLeft = 0;
			if (m_oGlobalBounds.left < 0)
				dLeft = -m_oGlobalBounds.left;
			
			dLeft /= dWidthPage;

			// top
			dTop = 0;
			if (m_oGlobalBounds.top < 0)
				dTop = -m_oGlobalBounds.top;

			dTop /= dHeightPage;

			// width
			double __dLeft	= max(0, m_oGlobalBounds.left);
			double __dRight	= min(dWidthControl, m_oGlobalBounds.right);

			dWidth = (__dRight - __dLeft) / dWidthPage;

			// height
			double __dTop		= max(0, m_oGlobalBounds.top);
			double __dBottom	= min(dHeightControl, m_oGlobalBounds.bottom);

			dHeight =  (__dBottom - __dTop) / dHeightPage;
		}
		
		friend bool operator==(const CDrawingPage& oPage1, const CDrawingPage& oPage2)
		{
			if (oPage1.m_pPage != oPage2.m_pPage)
				return false;

			if ((oPage1.m_oGlobalBounds.left	!= oPage2.m_oGlobalBounds.left)		||
				(oPage1.m_oGlobalBounds.top		!= oPage2.m_oGlobalBounds.top)		||
				(oPage1.m_oGlobalBounds.right	!= oPage2.m_oGlobalBounds.right)	||
				(oPage1.m_oGlobalBounds.bottom	!= oPage2.m_oGlobalBounds.bottom))
				return false;

			return true;
		}

		friend bool operator!=(const CDrawingPage& oPage1, const CDrawingPage& oPage2)
		{
			return (oPage1 == oPage2) ? false : true;
		}

		CDrawingPage& operator=(const CDrawingPage& oSrc)
		{
			m_pPage			= oSrc.m_pPage;
			m_oGlobalBounds	= oSrc.m_oGlobalBounds;

			m_lIndexInSource = oSrc.m_lIndexInSource;

			m_oPageBounds	= oSrc.m_oPageBounds;
			return *this;
		}

		CDrawingPage(const CDrawingPage& oSrc)
		{
			*this = oSrc;
		}

		void Inscribed()
		{
			double dWidth1  = m_oPageBounds.GetWidth();
			double dHeight1 = m_oPageBounds.GetHeight();

			double dWidth2	= m_pPage->m_dWidthMM;
			double dHeight2	= m_pPage->m_dHeightMM;

			double dAspect1 = dWidth1 / dHeight1;
			double dAspect2 = dWidth2 / dHeight2;

			if (dAspect1 >= dAspect2)
			{
				m_oGlobalBounds.top		= m_oPageBounds.top;
				m_oGlobalBounds.bottom	= m_oPageBounds.bottom;

				double __dWidth			= dWidth2 * dHeight1 / dHeight2;

				m_oGlobalBounds.left	= m_oPageBounds.left + (dWidth1 - __dWidth) / 2.0;
				m_oGlobalBounds.right	= m_oGlobalBounds.left + __dWidth;
			}
			else
			{
				m_oGlobalBounds.left	= m_oPageBounds.left;
				m_oGlobalBounds.right	= m_oPageBounds.right;

				double __dHeight		= dHeight2 * dWidth1 / dWidth2;

				m_oGlobalBounds.top		= m_oPageBounds.top + (dHeight1 - __dHeight) / 2.0;
				m_oGlobalBounds.bottom	= m_oGlobalBounds.top + __dHeight;
			}
		}
	};

	class CDrawingDocument
	{
	public:
		CAtlArray<CDrawingPage> m_arPages;

	public:
		CDrawingDocument() : m_arPages()
		{
		}
		~CDrawingDocument()
		{
			Clear();
		}

	public:
		inline void Clear()
		{
			m_arPages.RemoveAll();
		}
		friend bool operator==(const CDrawingDocument& oDoc1, const CDrawingDocument& oDoc2)
		{
			LONG lCount1 = (LONG)oDoc1.m_arPages.GetCount();
			LONG lCount2 = (LONG)oDoc2.m_arPages.GetCount();

			if (lCount1 != lCount2)
				return false;

			for (LONG i = 0; i < lCount1; ++i)
			{
				if (oDoc1.m_arPages[i] != oDoc2.m_arPages[i])
					return false;
			}

			return true;
		}
		
		friend bool operator!=(const CDrawingDocument& oDoc1, const CDrawingDocument& oDoc2)
		{
			return (oDoc1 == oDoc2) ? false : true;
		}

		CDrawingDocument& operator=(const CDrawingDocument& oSrc)
		{
			m_arPages.Copy(oSrc.m_arPages);
			return *this;
		}
		CDrawingDocument(const CDrawingDocument& oSrc)
		{
			*this = oSrc;
		}
	};

	// это для того, чтобы был один интерфейс для всех режимов
	// (кол-во страниц по горизонтали)
	// вроде как использоваться будет только 1 или 2
	class CBlockPages
	{
	public:
		CAtlArray<CPage*> m_arPages;

	public:
		CBlockPages() : m_arPages()
		{
		}
		~CBlockPages()
		{
			DeletePages();
		}
		CBlockPages& operator=(const CBlockPages& oSrc)
		{
			m_arPages.Copy(oSrc.m_arPages);
			return *this;
		}
		CBlockPages(const CBlockPages& oSrc)
		{
			*this = oSrc;
		}

	public:
		inline LONG GetWidth(const LONG& lPageBetween)
		{
			size_t nCount = m_arPages.GetCount();
			if (1 == nCount)
			{
				return m_arPages[0]->m_lPixWidth;
			}
			else if (2 == nCount)
			{
				//return m_arPages[0]->m_lPixWidth + m_arPages[1]->m_lPixWidth + lPageBetween;
				return 2 * max(m_arPages[0]->m_lPixWidth, m_arPages[1]->m_lPixWidth) + lPageBetween;
			}
			return 0;
		}

		inline double GetWidthMM(double dPlus)
		{
			size_t nCount = m_arPages.GetCount();
			if (1 == nCount)
			{
				return m_arPages[0]->m_dWidthMM;
			}
			else if (2 == nCount)
			{
				//return m_arPages[0]->m_dWidthMM + m_arPages[1]->m_dWidthMM + dPlus;
				return 2 * max(m_arPages[0]->m_dWidthMM, m_arPages[1]->m_dWidthMM) + dPlus;
			}
			return 0;
		}

		inline LONG GetHeight()
		{
			size_t nCount = m_arPages.GetCount();
			if (1 == nCount)
			{
				return m_arPages[0]->m_lPixHeight;
			}
			else if (2 == nCount)
			{
				return max(m_arPages[0]->m_lPixHeight, m_arPages[1]->m_lPixHeight);
			}
			return 0;
		}

		inline double GetHeightMM()
		{
			size_t nCount = m_arPages.GetCount();
			if (1 == nCount)
			{
				return m_arPages[0]->m_dHeightMM;
			}
			else if (2 == nCount)
			{
				return max(m_arPages[0]->m_dHeightMM, m_arPages[1]->m_dHeightMM);
			}
			return 0;
		}
		inline void DeletePages()
		{
			size_t nCount = m_arPages.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				CPage* pPage = m_arPages[i];
				RELEASEOBJECT(pPage);
			}
			m_arPages.RemoveAll();
		}
		inline void DeletePagesNoAttack()
		{
			m_arPages.RemoveAll();
		}

		inline void SetPositionVer(double dPosition)
		{
			for (size_t i = 0; i < m_arPages.GetCount(); ++i)
			{
				m_arPages[i]->m_dPositionVer = dPosition;
			}
		}
		inline void SetPositionVer2(double dPosition)
		{
			for (size_t i = 0; i < m_arPages.GetCount(); ++i)
			{
				m_arPages[i]->m_dPositionVer2 = dPosition;
			}
		}

		inline void CalculatePix(const double& dScaleX, const double& dScaleY)
		{
			size_t nCount = m_arPages.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				m_arPages[i]->CalculatePix(dScaleX, dScaleY);
			}
		}

		inline LONG AppendDrawingPages(CAtlArray<CDrawingPage>* pArray, const LONGLONG& lCenterHor, const double& dTop, const double& dBorderTop, LONG lBetween)
		{
			size_t nCount = m_arPages.GetCount();

			// а вот тут непонятно как делать. добавят возможность смотреть 
			// произвольное количество страниц - сделаю
			if (1 == nCount)
			{
				CPage* pPage = m_arPages[0];
				CDrawingPage oPage(pPage);
				oPage.m_oGlobalBounds.left		= lCenterHor - (double)(pPage->m_lPixWidth) / 2.0;
				oPage.m_oGlobalBounds.right		= lCenterHor + (double)(pPage->m_lPixWidth) / 2.0;

				oPage.m_oGlobalBounds.top		= dTop;
				oPage.m_oGlobalBounds.bottom	= (double)(oPage.m_oGlobalBounds.top + pPage->m_lPixHeight);

				if (oPage.m_oGlobalBounds.bottom > -dBorderTop)
				{
					pArray->Add(oPage);
				}
				
				return (LONG)(oPage.m_oGlobalBounds.bottom + lBetween);
			}
			else if (2 == nCount)
			{
				CPage* pPage1 = m_arPages[0];
				CPage* pPage2 = m_arPages[1];

				LONG lHeight1 = pPage1->m_lPixHeight;
				LONG lHeight2 = pPage2->m_lPixHeight;

				LONG lHeightMax = max(lHeight1, lHeight2);

				double dCentreV = dTop + ((double)lHeightMax) / 2;

				LONGLONG lRight1 = lCenterHor - lBetween / 2;

				CDrawingPage oPage1(pPage1);
				oPage1.m_oGlobalBounds.left		= (double)(lRight1 - pPage1->m_lPixWidth);
				oPage1.m_oGlobalBounds.right	= (double)lRight1;

				oPage1.m_oGlobalBounds.top		= dCentreV - (double)pPage1->m_lPixHeight / 2;
				oPage1.m_oGlobalBounds.bottom	= dCentreV + (double)pPage1->m_lPixHeight / 2;

				LONGLONG lLeft2  = lCenterHor + lBetween / 2;

				CDrawingPage oPage2(pPage2);
				oPage2.m_oGlobalBounds.left		= (double)(lLeft2);
				oPage2.m_oGlobalBounds.right	= (double)(lLeft2 + pPage2->m_lPixWidth);

				oPage2.m_oGlobalBounds.top		= dCentreV - (double)pPage2->m_lPixHeight / 2;
				oPage2.m_oGlobalBounds.bottom	= dCentreV + (double)pPage2->m_lPixHeight / 2;

				if (oPage1.m_oGlobalBounds.bottom > -dBorderTop)
				{
					pArray->Add(oPage1);
				}
				if (oPage2.m_oGlobalBounds.bottom > -dBorderTop)
				{
					pArray->Add(oPage2);
				}

				return (LONG)(dCentreV + (double)lHeightMax / 2 + lBetween);
			}

			return 0;
		}

		inline LONG AppendDrawingPages2(CAtlArray<CDrawingPage>* pArray, const LONGLONG& lCenterHor, const double& dTop, const double& dBorderTop, LONG lBetween)
		{
			size_t nCount = m_arPages.GetCount();

			// а вот тут непонятно как делать. добавят возможность смотреть 
			// произвольное количество страниц - сделаю
			if (1 == nCount)
			{
				CPage* pPage = m_arPages[0];
				CDrawingPage oPage(pPage);
				oPage.m_oGlobalBounds.left		= lCenterHor - (double)(pPage->m_lPixWidth) / 2.0;
				oPage.m_oGlobalBounds.right		= lCenterHor + (double)(pPage->m_lPixWidth) / 2.0;

				oPage.m_oGlobalBounds.top		= dTop;
				oPage.m_oGlobalBounds.bottom	= (double)(oPage.m_oGlobalBounds.top + pPage->m_lPixHeight);
				
				return (LONG)(oPage.m_oGlobalBounds.bottom + lBetween);
			}
			else if (2 == nCount)
			{
				CPage* pPage1 = m_arPages[0];
				CPage* pPage2 = m_arPages[1];

				LONG lHeight1 = pPage1->m_lPixHeight;
				LONG lHeight2 = pPage2->m_lPixHeight;

				LONG lHeightMax = max(lHeight1, lHeight2);

				double dCentreV = dTop + ((double)lHeightMax) / 2;

				LONGLONG lRight1 = lCenterHor - lBetween / 2;

				CDrawingPage oPage1(pPage1);
				oPage1.m_oGlobalBounds.left		= (double)(lRight1 - pPage1->m_lPixWidth);
				oPage1.m_oGlobalBounds.right	= (double)lRight1;

				oPage1.m_oGlobalBounds.top		= dCentreV - (double)pPage1->m_lPixHeight / 2;
				oPage1.m_oGlobalBounds.bottom	= dCentreV + (double)pPage1->m_lPixHeight / 2;

				LONGLONG lLeft2  = lCenterHor + lBetween / 2;

				CDrawingPage oPage2(pPage2);
				oPage2.m_oGlobalBounds.left		= (double)(lLeft2);
				oPage2.m_oGlobalBounds.right	= (double)(lLeft2 + pPage2->m_lPixWidth);

				oPage2.m_oGlobalBounds.top		= dCentreV - (double)pPage2->m_lPixHeight / 2;
				oPage2.m_oGlobalBounds.bottom	= dCentreV + (double)pPage2->m_lPixHeight / 2;

				pArray->Add(oPage1);
				pArray->Add(oPage2);

				return (LONG)(dCentreV + (double)lHeightMax / 2 + lBetween);
			}

			return 0;
		}
	};

	// самый верхний класс в иерархии
	// просто массив блоков.
	// важная функция - отдать часть блоков отдельным документом.
	// и надо реализовать здесь синхронизацию потоков
	class CDocument
	{
	public:
		CAtlArray<CBlockPages> m_arBlocks;
		LONG m_lCountPagesInBlock;

		CRITICAL_SECTION m_oCS;

		// только один раз можно выставить гиперссылки
		BOOL m_bIsSetupHyperlinks;

	public:
		CDocument() : m_arBlocks()
		{
			m_lCountPagesInBlock = 1;

			InitializeCriticalSection(&m_oCS);
		}
		~CDocument()
		{
			DeleteCriticalSection(&m_oCS);
		}

	public:
		
		inline ULONGLONG GetWidth(const LONG& lBetween)
		{
			CTemporaryCS oCS(&m_oCS);
			
			ULONGLONG lWidth = 0;
			size_t nCount = m_arBlocks.GetCount();

			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				ULONGLONG lNew = (ULONGLONG)m_arBlocks[nIndex].GetWidth(lBetween);
				if (lWidth < lNew)
				{
					lWidth = lNew;					
				}
			}
			return lWidth;			
		}
		inline ULONGLONG GetHeightMax()
		{
			CTemporaryCS oCS(&m_oCS);
			
			ULONGLONG lHeight = 0;
			size_t nCount = m_arBlocks.GetCount();

			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				ULONGLONG lNew = (ULONGLONG)m_arBlocks[nIndex].GetHeight();
				if (lHeight < lNew)
				{
					lHeight = lNew;					
				}
			}
			return lHeight;			
		}

		inline double GetWidthMM(double dPlus)
		{
			CTemporaryCS oCS(&m_oCS);
			
			double max = 0;
			size_t nCount = m_arBlocks.GetCount();

			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				double lNew = m_arBlocks[nIndex].GetWidthMM(dPlus);
				if (max < lNew)
				{
					max = lNew;					
				}
			}
			return max;			
		}
		inline double GetHeightMM()
		{
			CTemporaryCS oCS(&m_oCS);
			
			double max = 0;
			size_t nCount = m_arBlocks.GetCount();

			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				double lNew = m_arBlocks[nIndex].GetHeightMM();
				if (max < lNew)
				{
					max = lNew;					
				}
			}
			return max;	
		}

		inline ULONGLONG GetHeight()
		{
			CTemporaryCS oCS(&m_oCS);
			
			ULONGLONG lHeight = 0;
			size_t nCount = m_arBlocks.GetCount();

			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				//lHeight += m_arPages[nIndex]->Height();
				lHeight += m_arBlocks[nIndex].GetHeight();
			}
			return lHeight;
		}

		inline void CalculatePix(const double& dScaleX, const double& dScaleY)
		{
			CTemporaryCS oCS(&m_oCS);
			
			size_t nCount = m_arBlocks.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				m_arBlocks[i].CalculatePix(dScaleX, dScaleY);
			}
		}

		inline void CloseFile()
		{
			CTemporaryCS oCS(&m_oCS);
			
			size_t nCount = m_arBlocks.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				m_arBlocks[i].DeletePages();
			}

			m_arBlocks.RemoveAll();
		}

		inline void Clear(bool bIsAttack = true)
		{
			CTemporaryCS oCS(&m_oCS);
			
			if (!bIsAttack)
			{
				size_t nCountB = m_arBlocks.GetCount();
				for (size_t i = 0; i < nCountB; ++i)
				{
					m_arBlocks[i].DeletePagesNoAttack();
				}
			}

			m_arBlocks.RemoveAll();
		}
		inline void Remove(LONG lStartPage)
		{
			LONG lBlock = lStartPage / m_lCountPagesInBlock;
			LONG lPage	= lStartPage % m_lCountPagesInBlock;

			LONG lBlocks = (LONG)m_arBlocks.GetCount();
			if ((0 > lBlock) || (lBlock >= lBlocks))
				return;

			LONG lPages = (LONG)m_arBlocks[lBlock].m_arPages.GetCount();
			if ((0 > lPage) || (lPage >= lPages))
				return;


			m_arBlocks[lBlock].m_arPages.RemoveAt(lPage, lPages - lPage);

			if (0 == lPage)
			{
				m_arBlocks.RemoveAt(lBlock, lBlocks - lBlock);
			}
			else
			{
				m_arBlocks.RemoveAt(lBlock + 1, lBlocks - lBlock - 1);
			}
		}

		inline LONG GetPixelsBetween(LONG lOne)
		{
			return (m_lCountPagesInBlock - 1) * lOne;
		}

		inline void GetCurrentDocument(CDocument* pDocument, LONG lStartBlock, LONG lCountBlocks)
		{
			CTemporaryCS oCS(&m_oCS);
			
			pDocument->m_arBlocks.RemoveAll();
			
			pDocument->m_lCountPagesInBlock = m_lCountPagesInBlock;
			pDocument->Clear();

			if (0 > lStartBlock)
				lStartBlock = 0;

			LONG lCount = (LONG)m_arBlocks.GetCount();

			LONG lCountMax = lCount - lStartBlock;
			if (lCountBlocks > lCountMax)
				lCountBlocks = lCountMax;

			for (LONG i = 0; i < lCountBlocks; ++i)
			{
				pDocument->m_arBlocks.Add(m_arBlocks[lStartBlock + i]);
			}
		}

		inline void AddPage(CPage* pPage)
		{
			CTemporaryCS oCS(&m_oCS);

			size_t nCount = m_arBlocks.GetCount();
			if (0 == nCount)
			{
				m_arBlocks.Add();
				m_arBlocks[nCount].m_arPages.Add(pPage);

				BSTR bsLinks = NULL;
				if (S_OK == pPage->m_pPage->get_Hypelinks(&bsLinks))
				{
					CString strXml = (CString)bsLinks;
					HyperlinkSetup(strXml);
					SysFreeString(bsLinks);
				}

				return;
			}

			if (m_arBlocks[nCount - 1].m_arPages.GetCount() < (size_t)m_lCountPagesInBlock)
			{
				m_arBlocks[nCount - 1].m_arPages.Add(pPage);

				BSTR bsLinks = NULL;
				if (S_OK == pPage->m_pPage->get_Hypelinks(&bsLinks))
				{
					CString strXml = (CString)bsLinks;
					HyperlinkSetup(strXml);
					SysFreeString(bsLinks);
				}

				return;
			}
			
			m_arBlocks.Add();
			m_arBlocks[nCount].m_arPages.Add(pPage);

			BSTR bsLinks = NULL;
			if (S_OK == pPage->m_pPage->get_Hypelinks(&bsLinks))
			{
				CString strXml = (CString)bsLinks;
				HyperlinkSetup(strXml);
				SysFreeString(bsLinks);
			}
		}

		inline CBlockPages GetBlock(size_t nIndex)
		{
			CTemporaryCS oCS(&m_oCS);
			return m_arBlocks[nIndex];
		}

		inline size_t GetPagesCount()
		{
			CTemporaryCS oCS(&m_oCS);
			
			LONG lCount = (LONG)m_arBlocks.GetCount();
			if (0 == lCount)
				return 0;

			size_t nResult = m_lCountPagesInBlock * (lCount - 1);
			nResult += m_arBlocks[lCount - 1].m_arPages.GetCount();
			return nResult;
		}

		CPage* GetPage(LONG lIndexPage)
		{
			LONG lBlock = lIndexPage / m_lCountPagesInBlock;
			LONG lPage = lIndexPage % m_lCountPagesInBlock;

			if ((0 > lBlock) || (lBlock >= (LONG)m_arBlocks.GetCount()))
				return NULL;

			if ((0 > lPage) || (lPage >= (LONG)m_arBlocks[lBlock].m_arPages.GetCount()))
				return NULL;

			return m_arBlocks[lBlock].m_arPages[lPage];
		}

		bool SetCountPagesInBlock(LONG lCountPages)
		{
			if (m_lCountPagesInBlock == lCountPages)
				return false;

			CAtlArray<CPage*> arPages;
			size_t nPages = GetPagesCount();

			for (size_t i = 0; i < nPages; ++i)
			{
				CPage* pPage = GetPage((LONG)i);
				arPages.Add(pPage);
			}

			Clear(false);
			m_lCountPagesInBlock = lCountPages;

			for (size_t i = 0; i < nPages; ++i)
			{
				AddPage(arPages[i]);
			}

			return true;
		}

		void HyperlinkSetup(CString& strXmlLinks)
		{
			if (m_bIsSetupHyperlinks && _T("") == strXmlLinks)
				return;

			m_bIsSetupHyperlinks = TRUE;
			
			XmlUtils::CXmlNode oNodeLinker;
			if (oNodeLinker.FromXmlString(strXmlLinks))
			{
				XmlUtils::CXmlNodes oNodeLinks;
				if (oNodeLinker.GetNodes(_T("link"), oNodeLinks))
				{
					LONG lCount = oNodeLinks.GetCount();
					LONG lCountPages = (LONG)GetPagesCount();

					for (LONG i = 0; i < lCount; ++i)
					{
						XmlUtils::CXmlNode oLink;
						oNodeLinks.GetAt(i, oLink);

						CHyperlink oHL;
						oHL.FromXml(oLink);
						
						CPage* pPage = GetPage(oHL.m_lSrcPageIndex);
						
						if (NULL != pPage)
						{
							pPage->m_oLinks.m_arLinks.Add(oHL);
						}												
					}
				}
			}
		}
	};

}