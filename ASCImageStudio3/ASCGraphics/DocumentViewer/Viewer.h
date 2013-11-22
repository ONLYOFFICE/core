#pragma once
#include "..\stdafx.h"
#include "atlcoll.h"
#include "TemporaryCS.h"
#include "ViewerCommon.h"

#include "..\Interfaces\ASCMetafile.h"
#include "..\Interfaces\ASCGraphicsRenderer.h"
#include "..\Interfaces\BaseThread.h"

#include "..\Interfaces\StructuresGDI.h"
#include "..\Interfaces\ImageSerializeObjects2.h"

namespace NSDocumentViewer
{
	class IPageEvent
	{
	public:
		virtual void SendEventComplete()		= 0;
	};

	class CPageInfo
	{
	public:
		// размеры для векторного пересчета
		LONG m_lWidthPix;
		LONG m_lHeightPix;

		// был нужен раньше, теперь нет, но оставим на всякий
		IAVSFontManager* m_pManager;

	public:
		CPageInfo()
		{
			m_lWidthPix		= 0;
			m_lHeightPix	= 0;
			m_pManager		= 0;
		}
		~CPageInfo()
		{
			RELEASEINTERFACE(m_pManager);
		}
		CPageInfo& operator=(const CPageInfo& oSrc)
		{
			m_lWidthPix		= oSrc.m_lWidthPix;
			m_lHeightPix	= oSrc.m_lHeightPix;

			m_pManager		= oSrc.m_pManager;
			ADDREFINTERFACE(m_pManager);
			return *this;
		}
		CPageInfo(const CPageInfo& oSrc)
		{
			*this = oSrc;
		}
	};

	class CCachePage
	{
	public:
		enum PageStatus
		{
			psNone			= 0,
			psNeedRemoved	= 1,
			psRemoved		= 2,
			psNeedWorked	= 3,
			psWorked		= 4,
			psComplete		= 5,
			psStopNeedWork	= 6
		};
		
	private:
		LONG m_lCompleteWidth;
		LONG m_lCompleteHeight;

		CDIB* m_pCompleteDIB;

		LONG m_lWidth;
		LONG m_lHeight;

		CDIB* m_pDIB;

		IAVSMetafile* m_pMetafile;

		CRITICAL_SECTION m_oCS;

		BOOL m_bBreak;
		BOOL m_bIsBitsInitialize;

		CPageInfo m_oInfo;

		PageStatus m_eState;
		IPageEvent* m_pEvent;

		IAVSFontManager* m_pManager;

		CImageFilesCache* m_pImageCache;

	public:
		CCachePage(IPageEvent* pEvent = NULL)
		{
			m_lCompleteWidth	= 0;
			m_lCompleteHeight	= 0;

			m_pCompleteDIB		= NULL;

			m_lWidth			= 0;
			m_lHeight			= 0;

			m_pDIB				= NULL;

			m_pMetafile			= NULL;

			m_bIsBitsInitialize = FALSE;
			m_bBreak			= FALSE;

			m_eState			= psNone;
			m_pEvent			= pEvent;

			m_pManager			= NULL;
			m_pImageCache		= NULL;

			InitializeCriticalSection(&m_oCS);
		}
		~CCachePage()
		{
			DeleteCriticalSection(&m_oCS);

			RELEASEOBJECT(m_pCompleteDIB);
			RELEASEOBJECT(m_pDIB);
			RELEASEINTERFACE(m_pMetafile);

			RELEASEINTERFACE(m_pManager);
		}

	public:
		void Draw(CPainter* pPainter, CDoubleRect& oRect)
		{
			CTemporaryCS oCS(&m_oCS);

			if ((NULL == m_pCompleteDIB) && (NULL == m_pDIB))
			{
				// ничего не готово... рисуем белую страницу - это делается наверху
				return;
			}
		
			RECT rectDst;
			rectDst.left	= (LONG)oRect.left;
			rectDst.top		= (LONG)oRect.top;
			rectDst.right   = (LONG)oRect.right;
			rectDst.bottom  = (LONG)oRect.bottom;

			if (NULL == m_pCompleteDIB)
			{
				// значит еще идет первоначальная отрисовка

				// подождем инициализации битов
				
				// по идее это не нужно!!!
				// так как мы ждем, чтобы нарисовать белую страницу.
				// а она УЖЕ нарисована.
				// т.е. если биты не инициализированы, то можно не рисовать ничего
				// просто ретурн

				//while (!m_bIsBitsInitialize)
				//	Sleep(5);
				if (!m_bIsBitsInitialize)
					return;

				m_pDIB->Draw(pPainter, oRect);
				return;
			}

			m_pCompleteDIB->Draw(pPainter, oRect);
		}
		inline void Work(IAVSFontManager* pManager)
		{
			if ((m_oInfo.m_lWidthPix == m_lCompleteWidth) && (m_oInfo.m_lHeightPix == m_lCompleteHeight))
			{
				// страница готова и уже закэширована
				// ничего делать не нужно
				m_eState = psComplete;
				return;
			}
			if ((NULL != m_pDIB) && (m_oInfo.m_lWidthPix == m_lWidth) && (m_oInfo.m_lHeightPix == m_lHeight))
			{
				// уже идет расчет страницы. ничего стопить не нужно
				// просто наверху ждать нужно конца отрисовки

				// такой ситуации быть не может
				return;
			}

			RELEASEINTERFACE(m_pManager);
			m_pManager = pManager;
			ADDREFINTERFACE(m_pManager);

			// статус УЖЕ проставлен
			//SetStatus(psWorked);
			
			m_lWidth  = m_oInfo.m_lWidthPix;
			m_lHeight = m_oInfo.m_lHeightPix;

			RELEASEOBJECT(m_pDIB);

			m_lWidth  = max(1, min(20000, m_lWidth));
			m_lHeight = max(1, min(20000, m_lHeight));

			m_bIsBitsInitialize = FALSE;
			
			m_pDIB = new CDIB();
			BOOL bRes = m_pDIB->Create(m_lWidth, m_lHeight);

			if (!bRes)
			{
				RELEASEOBJECT(m_pDIB);
				
				m_lWidth  = 0;
				m_lHeight = 0;

				m_bIsBitsInitialize = TRUE;

				SetStatus(psNeedWorked);

				return;
			}

			LONG lBufferSize = 4 * m_lWidth * m_lHeight;

			memset(m_pDIB->m_pBits, 0xFF, lBufferSize);

			m_bIsBitsInitialize = TRUE;

			IAVSRenderer* pRenderer = NULL;
			CoCreateInstance(__uuidof(CAVSGraphicsRenderer), NULL, CLSCTX_ALL, __uuidof(IAVSRenderer), (void**)&pRenderer);

			CAVSGraphicsRenderer* p2DRenderer = (CAVSGraphicsRenderer*)pRenderer;
			
			p2DRenderer->SetFontManager(m_pManager);
			p2DRenderer->SetImageCache(m_pImageCache);

			CDoubleRect oRect;
			oRect.left		= 0;
			oRect.top		= 0;
			oRect.right		= m_lWidth;
			oRect.bottom	= m_lHeight;
							
			p2DRenderer->m_oRenderer.Create(m_pDIB->m_pBits, oRect, m_lWidth, m_lHeight, m_pDIB);
			m_pMetafile->Draw2(pRenderer, -1, -1, &m_bBreak);

			RELEASEINTERFACE(pRenderer);

			if (psWorked == m_eState)
			{
				// это значит не была застоплена отрисовка...
				Flip();
			}

			UpdateState();
		}
		inline void Flip()
		{
			CTemporaryCS oCS(&m_oCS);

			CDIB* pMem			= m_pCompleteDIB;
			
			m_pCompleteDIB		= m_pDIB;
			m_lCompleteWidth	= m_lWidth;
			m_lCompleteHeight	= m_lHeight;

			m_pDIB				= NULL;
			m_lWidth			= 0;
			m_lHeight			= 0;
			
			RELEASEOBJECT(pMem);
		}

		LONG GetMemorySize()
		{
			return 4 * m_lCompleteWidth * m_lCompleteHeight;
		}
		
		inline IAVSMetafile* GetMeta()
		{
			return m_pMetafile;
		}
		inline void SetMeta(IAVSMetafile* pMeta)
		{
			RELEASEINTERFACE(m_pMetafile);
			m_pMetafile = pMeta;
			ADDREFINTERFACE(m_pMetafile);
		}
		inline void SetImageCache(CImageFilesCache* pCache)
		{
			m_pImageCache = pCache;
		}
		inline void SetInfo(const CPageInfo& oInfo)
		{
			m_oInfo = oInfo;
		}

		BOOL IsRecalc(const CPageInfo& oInfo)
		{
			if ((oInfo.m_lWidthPix == m_lCompleteWidth) && (oInfo.m_lHeightPix == m_lCompleteHeight))
			{
				// страница готова и уже закэширована
				// ничего делать не нужно
				return FALSE;
			}
			if ((oInfo.m_lWidthPix == m_lWidth) && (oInfo.m_lHeightPix == m_lHeight))
			{
				// уже идет расчет страницы. ничего стопить не нужно
				// просто наверху ждать нужно конца отрисовки
				return FALSE;
			}

			m_oInfo = oInfo;

			return TRUE;
		}

		inline void SetRemoved()
		{
			CTemporaryCS oCS(&m_oCS);

			if ((psComplete == m_eState) || (psNeedWorked == m_eState) || (psNone == m_eState) || (psRemoved == m_eState))
			{
				m_eState = psRemoved;
				return;
			}

			m_bBreak = TRUE;
			m_eState = psNeedRemoved;
		}
		inline PageStatus GetStatus()
		{
			CTemporaryCS oCS(&m_oCS);
			return m_eState;
		}
		inline void SetStatus(const PageStatus& eStatus)
		{
			CTemporaryCS oCS(&m_oCS);
			m_eState = eStatus;
		}

		inline void SetBreak()
		{
			m_bBreak = TRUE;
		}
		inline void SetNeedWork(const CPageInfo& oInfo)
		{
			m_oInfo  = oInfo;
			m_eState = psNeedWorked;
		}
		inline void UpdateState()
		{
			CTemporaryCS oCS(&m_oCS);
			switch (m_eState)
			{
			case psNeedRemoved:
				{
					m_eState = psRemoved;
					break;
				}
			case psWorked:
				{
					m_eState = psComplete;
					if (NULL != m_pEvent)
						m_pEvent->SendEventComplete();
					break;
				}
			case psStopNeedWork:
				{
					m_eState = psNeedWorked;
					break;
				}
			default:
				break;
			};

			m_bBreak = FALSE;
		}
		void CheckNeedWork(const CPageInfo& oInfo)
		{
			CTemporaryCS oCS(&m_oCS);

			BOOL bNeed = TRUE;
			if ((oInfo.m_lWidthPix == m_lCompleteWidth) && (oInfo.m_lHeightPix == m_lCompleteHeight))
			{
				// страница готова и уже закэширована
				// ничего делать не нужно
				bNeed = FALSE;
			}
			else if ((oInfo.m_lWidthPix == m_lWidth) && (oInfo.m_lHeightPix == m_lHeight))
			{
				// уже идет расчет страницы. ничего стопить не нужно
				// просто наверху ждать нужно конца отрисовки
				bNeed = FALSE;
			}

			if (FALSE == bNeed)
			{
				// по идее просто ретурн, но сделаем на всякий случай одну проверочку...
				// вдруг страница готова или расчитывается с нужными настройками,
				// но стоит на очередь удаления. Ситуация странная, но, в принципе, осуществимая
				if ((psNeedRemoved == m_eState) || (psRemoved == m_eState) || (psStopNeedWork == m_eState))
				{
					m_eState = ((psStopNeedWork == m_eState) || (psNeedRemoved == m_eState)) ? psWorked : psComplete;
				}
				return;
			}
			
			if ((psWorked == m_eState) || (psStopNeedWork == m_eState))
			{
				m_bBreak = TRUE;
				m_eState = psStopNeedWork;
			}
			else
			{
				m_eState = psNeedWorked;
			}
			m_oInfo = oInfo;
		}
		inline BOOL IsWorked()
		{
			//CTemporaryCS oCS(&m_oCS);
			return (psWorked == m_eState);
		}
		inline BOOL IsWorkedOrNeedWorked()
		{
			return ((psWorked == m_eState) || (psNeedWorked == m_eState));
		}
	};

	class ICacheDocument
	{
	public:
		virtual CCachePage* CalculatePage() = 0;
	};

	class CCalculateThread : public CBaseThread
	{
	private:
		ICacheDocument* m_pDocument;
		DWORD m_dwInterval;

		IAVSFontManager* m_pFont;

	public:
		CCalculateThread(DWORD dwAffinity = 0) : CBaseThread(dwAffinity)
		{
			m_pFont = NULL;

			m_pDocument = NULL;
			m_dwInterval = 10;
		}
		~CCalculateThread()
		{
			StopWork();

			RELEASEINTERFACE(m_pFont);
		}
		inline void SetDocument(ICacheDocument* pDoc)
		{
			m_pDocument = pDoc;
		}
		inline void Start(LONG lPriority)
		{
			StartWork(lPriority);

			// здесь еще можно интервал менять в зависисости от приоритета
		}

		void InitFonts()
		{
			CoCreateInstance(__uuidof(CAVSFontManager), NULL, CLSCTX_INPROC, __uuidof(IAVSFontManager), (void**)&m_pFont);
			m_pFont->Initialize(L"");
			m_pFont->SetDefaultFont(L"Arial");
		}
	
	protected:
		virtual DWORD ThreadProc()
		{
			CoInitialize(NULL);

			if (NULL == m_pFont)
			{
				InitFonts();
			}

			while (m_bRunThread)
			{
				CCachePage* pPage = m_pDocument->CalculatePage();
				if (NULL == pPage)
				{
					::Sleep(m_dwInterval);
				}
				else
				{
					pPage->Work(m_pFont);
				}
			}

			CoUninitialize();
			m_bRunThread = FALSE;
			return 0;
		}
	};

	
	class IWatermark
	{
	public:
		inline virtual BOOL IsInit()																= 0;
		virtual void Draw(CPainter* pPainter, CDoubleRect& oRect, double& dWidth, double& dHeight)	= 0;
		virtual void LoadFromXML(BSTR bsXml)														= 0;
		virtual void LoadFromXML(XmlUtils::CXmlNode& oNode)											= 0;
	};
	class CWatermarkGDI : public IWatermark
	{
	private:
		Gdiplus::Brush*			m_pGdiBrush;
		Gdiplus::StringFormat*	m_pGdiFormat;
		Gdiplus::Font*			m_pGdiFont;

		NSStructuresGDI::CBrush	m_oBrush;
		NSStructuresGDI::CFont	m_oFont;
		
		_bstr_t					m_sText;

		LONG					m_lMode;

	public:
		CWatermarkGDI()
		{
			m_pGdiFormat = new Gdiplus::StringFormat();
			m_pGdiFormat->SetAlignment(Gdiplus::StringAlignmentCenter);
			m_pGdiFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);

			m_pGdiBrush	= NULL;
			m_pGdiFont	= NULL;
			m_sText		= L"";

			m_lMode		= 0;
		}

		~CWatermarkGDI()
		{
			Clear();
			RELEASEOBJECT(m_pGdiFormat);
		}

		inline void Clear()
		{
			RELEASEOBJECT(m_pGdiFont);
			RELEASEOBJECT(m_pGdiBrush);
		}

		inline virtual BOOL IsInit()
		{
			return (NULL != m_pGdiBrush);
		}

		virtual void LoadFromXML(BSTR bsXML)
		{
			if (NULL == bsXML)
			{
				Clear();
				return;
			}

			CString strXml = (CString)bsXML;
			if (_T("") == strXml)
			{
				Clear();
				return;
			}

			XmlUtils::CXmlNode oNode;
			if (!oNode.FromXmlString(bsXML))
			{
				Clear();
				return;
			}

			XmlUtils::CXmlNode oNodeAttr;
			oNode.GetNode(_T("Attributes"), oNodeAttr);

			m_oBrush.FromXmlNode2(oNodeAttr);
			m_oFont.FromXmlNode2(oNodeAttr);

			m_sText = oNode.GetAttributeOrValue(_T("text"), _T(""));
			m_lMode = XmlUtils::GetInteger(oNode.GetAttributeOrValue(_T("mode"), _T("0")));

			RELEASEOBJECT(m_pGdiBrush);
			m_pGdiBrush = m_oBrush.GetBrush();
		}
		virtual void LoadFromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oNodeAttr;
			oNode.GetNode(_T("Attributes"), oNodeAttr);

			m_oBrush.FromXmlNode2(oNodeAttr);
			m_oFont.FromXmlNode2(oNodeAttr);

			m_sText = oNode.GetAttributeOrValue(_T("text"), _T(""));
			m_lMode = XmlUtils::GetInteger(oNode.GetAttributeOrValue(_T("mode"), _T("0")));

			RELEASEOBJECT(m_pGdiBrush);
			m_pGdiBrush = m_oBrush.GetBrush();
		}

		virtual void Draw(CPainter* pPainter, CDoubleRect& oRect, double& dWidth, double& dHeight)
		{
			Gdiplus::Graphics oGraphics(pPainter->m_hMemDC);
			oGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

			double dSizeFont = m_oFont.Size * 0.3528 * oRect.GetHeight() / dHeight;
			m_pGdiFont = m_oFont.GetFontBySize(dSizeFont);

			GenerateTransform(&oGraphics, oRect, dWidth, dHeight);

			Gdiplus::RectF oLayout((float)oRect.left, (float)oRect.top, (float)oRect.GetWidth(), (float)oRect.GetHeight());
			oGraphics.DrawString(m_sText.GetBSTR(), m_sText.length(), m_pGdiFont, oLayout, m_pGdiFormat, m_pGdiBrush);	

			RELEASEOBJECT(m_pGdiFont);
		}
		
		void GenerateTransform(Gdiplus::Graphics* pGraphics, CDoubleRect& oRect, double& dWidth, double& dHeight)
		{
			pGraphics->ResetTransform();
			switch (m_lMode)
			{
			case 0:
				{
					return;
				}
			case 1:
				{
					double dAngle = atan(dHeight / dWidth);
					dAngle = 180 * dAngle / M_PI;

					REAL fX = (REAL)oRect.GetCentreX();
					REAL fY = (REAL)oRect.GetCentreY();
					
					pGraphics->TranslateTransform(-fX, -fY, Gdiplus::MatrixOrderAppend);
					pGraphics->RotateTransform(dAngle, Gdiplus::MatrixOrderAppend);
					pGraphics->TranslateTransform(fX, fY, Gdiplus::MatrixOrderAppend);

					break;					
				}
			case 2:
				{
					REAL fX = (REAL)oRect.GetCentreX();
					REAL fY = (REAL)oRect.GetCentreY();
					
					pGraphics->TranslateTransform(-fX, -fY, Gdiplus::MatrixOrderAppend);
					pGraphics->RotateTransform(-90.0, Gdiplus::MatrixOrderAppend);
					pGraphics->TranslateTransform(fX, fY, Gdiplus::MatrixOrderAppend);

					break;
				}
			case 3:
				{
					double dAngle = atan(dHeight / dWidth);
					dAngle = 180 * dAngle / M_PI;

					REAL fX = (REAL)oRect.GetCentreX();
					REAL fY = (REAL)oRect.GetCentreY();
					
					pGraphics->TranslateTransform(-fX, -fY, Gdiplus::MatrixOrderAppend);
					pGraphics->RotateTransform(dAngle, Gdiplus::MatrixOrderAppend);
					pGraphics->TranslateTransform(fX, fY, Gdiplus::MatrixOrderAppend);

					break;
				}
			case 4:
				{
					REAL fX = (REAL)oRect.GetCentreX();
					REAL fY = (REAL)oRect.GetCentreY();
					
					pGraphics->TranslateTransform(-fX, -fY, Gdiplus::MatrixOrderAppend);
					pGraphics->RotateTransform(180.0, Gdiplus::MatrixOrderAppend);
					pGraphics->TranslateTransform(fX, fY, Gdiplus::MatrixOrderAppend);

					break;
				}
			case 5:
				{
					double dAngle = atan(dHeight / dWidth);
					dAngle = 180 * dAngle / M_PI;

					REAL fX = (REAL)oRect.GetCentreX();
					REAL fY = (REAL)oRect.GetCentreY();
					
					pGraphics->TranslateTransform(-fX, -fY, Gdiplus::MatrixOrderAppend);
					pGraphics->RotateTransform(-(dAngle + 180.0), Gdiplus::MatrixOrderAppend);
					pGraphics->TranslateTransform(fX, fY, Gdiplus::MatrixOrderAppend);

					break;
				}
			case 6:
				{
					REAL fX = (REAL)oRect.GetCentreX();
					REAL fY = (REAL)oRect.GetCentreY();
					
					pGraphics->TranslateTransform(-fX, -fY, Gdiplus::MatrixOrderAppend);
					pGraphics->RotateTransform(90.0, Gdiplus::MatrixOrderAppend);
					pGraphics->TranslateTransform(fX, fY, Gdiplus::MatrixOrderAppend);

					break;
				}
			case 7:
				{
					double dAngle = atan(dHeight / dWidth);
					dAngle = 180 * dAngle / M_PI;

					REAL fX = (REAL)oRect.GetCentreX();
					REAL fY = (REAL)oRect.GetCentreY();
					
					pGraphics->TranslateTransform(-fX, -fY, Gdiplus::MatrixOrderAppend);
					pGraphics->RotateTransform(360 - dAngle, Gdiplus::MatrixOrderAppend);
					pGraphics->TranslateTransform(fX, fY, Gdiplus::MatrixOrderAppend);

					break;
				}
			default:
				break;
			}
		}
	};

	class CWatermark : public IWatermark
	{
	protected:
		class CEdgePainter : public ImageStudio::Serialize::Paint::Common::ITextPainter, public ISimpleGraphicsPath
		{
		public:
			NSStructures::CFont m_oFont;
			NSStructures::CEdgeText m_oEdge;
			IAVSFontManager* m_pManager;

			IAVSRenderer* m_pRenderer;

		public:
			CEdgePainter() : m_oEdge()
			{
				m_pManager = NULL;
				m_pRenderer = NULL;
			}
			~CEdgePainter()
			{
				RELEASEINTERFACE(m_pManager);
			}

		public:
			virtual void CommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, IAVSRenderer *pRenderer)
			{
				if (0 == m_oEdge.Visible)
				{
					pRenderer->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
					return;
				}

				if (NULL == m_pManager)
				{
					CoCreateInstance(__uuidof(CAVSFontManager), NULL, CLSCTX_INPROC, __uuidof(IAVSFontManager), (void**)&m_pManager);
					m_pManager->Initialize(L"");
					
					CString strDefaultFontName = _T("Arial");
					BSTR bstrDefaultFontName = strDefaultFontName.AllocSysString();
					m_pManager->SetDefaultFont(bstrDefaultFontName);
					SysFreeString(bstrDefaultFontName);
				}

				m_pRenderer = pRenderer;

				LONG lPenColor		= 0;
				LONG lPenAlpha		= 0;
				double lPenWidth	= 0;
				m_pRenderer->get_PenColor(&lPenColor);
				m_pRenderer->get_PenAlpha(&lPenAlpha);
				m_pRenderer->get_PenSize(&lPenWidth);

				m_pRenderer->put_PenColor(m_oEdge.Color);
				m_pRenderer->put_PenAlpha(m_oEdge.Alpha);
				m_pRenderer->put_PenSize(0.5);

				double dDpiX = 72.0;
				double dDpiY = 72.0;
				m_pRenderer->get_DpiX(&dDpiX);
				m_pRenderer->get_DpiY(&dDpiY);

				if (_T("") == m_oFont.Path)
				{
					BSTR bsName = m_oFont.Name.AllocSysString();
					m_pManager->LoadFontByName(bsName, (float)m_oFont.Size, m_oFont.GetStyle(), dDpiX, dDpiY);
					SysFreeString(bsName);
				}
				else
				{
					BSTR bsName = m_oFont.Path.AllocSysString();
					m_pManager->LoadFontFromFile(bsName, (float)m_oFont.Size, dDpiX, dDpiY);
					SysFreeString(bsName);
				}

				m_pManager->LoadString(bsText, (float)fX, (float)(fY + fBaseLineOffset));

				m_pRenderer->BeginCommand(c_nPathType);
				
				CAVSFontManager* pMan = (CAVSFontManager*)m_pManager;
				pMan->GetStringPath3(this);

				m_pRenderer->DrawPath(c_nStroke | c_nWindingFillMode);
				m_pRenderer->PathCommandEnd();
				m_pRenderer->EndCommand(c_nPathType);

				LONG lBrushAlpha = 0x00;
				m_pRenderer->get_BrushAlpha1(&lBrushAlpha);
				m_pRenderer->put_BrushAlpha1(0x00);
				m_pRenderer->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);

				m_pRenderer->put_BrushAlpha1(lBrushAlpha);

				m_pRenderer->put_PenColor(lPenColor);
				m_pRenderer->put_PenAlpha(lPenAlpha);
				m_pRenderer->put_PenSize(lPenWidth);
			}

		public:
			virtual bool _MoveTo(double x, double y)
			{
				return (S_OK == m_pRenderer->PathCommandMoveTo(x, y));
			}
			virtual bool _LineTo(double x, double y)	
			{
				return (S_OK == m_pRenderer->PathCommandLineTo(x, y));
			}
			virtual bool _CurveTo(double x1, double y1, double x2, double y2, double x3, double y3)
			{
				return (S_OK == m_pRenderer->PathCommandCurveTo(x1, y1, x2, y2, x3, y3));
			}
			virtual bool _Close()
			{
				return (S_OK == m_pRenderer->PathCommandClose());
			}
		};

	private:
		NSStructures::CBrush	m_oBrush;
		NSStructures::CFont		m_oFont;
		NSStructures::CFormat	m_oFormat;

		CEdgePainter			m_oEdgePainter;
		
		_bstr_t					m_sText;

		LONG					m_lMode;

		ImageStudio::Serialize::Paint::Common::CTextMeasurer m_oMeasurer;

	public:
		CWatermark()
		{
			m_sText		= L"";
			m_lMode		= 0;
		}

		~CWatermark()
		{
		}

		inline void Clear()
		{
			m_sText		= L"";
		}

		inline virtual BOOL IsInit()
		{
			return (0 != m_sText.length());
		}

		virtual void LoadFromXML(BSTR bsXML)
		{
			if (NULL == bsXML)
			{
				Clear();
				return;
			}

			CString strXml = (CString)bsXML;
			if (_T("") == strXml)
			{
				Clear();
				return;
			}

			XmlUtils::CXmlNode oNode;
			if (!oNode.FromXmlString(bsXML))
			{
				Clear();
				return;
			}

			XmlUtils::CXmlNode oNodeAttr;
			oNode.GetNode(_T("Attributes"), oNodeAttr);

			m_oBrush.FromXmlNode2(oNodeAttr);
			m_oFont.FromXmlNode2(oNodeAttr);
			m_oFormat.FromXmlNode(oNodeAttr);
			
			m_oEdgePainter.m_oFont = m_oFont;
			m_oEdgePainter.m_oEdge.FromXmlNode(oNodeAttr);

			m_sText = oNode.GetAttributeOrValue(_T("text"), _T(""));
			m_lMode = XmlUtils::GetInteger(oNode.GetAttributeOrValue(_T("mode"), _T("0")));
		}

		virtual void LoadFromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlUtils::CXmlNode oNodeAttr;
			oNode.GetNode(_T("Attributes"), oNodeAttr);

			m_oBrush.FromXmlNode2(oNodeAttr);
			m_oFont.FromXmlNode2(oNodeAttr);
			m_oFormat.FromXmlNode(oNodeAttr);

			m_oEdgePainter.m_oFont = m_oFont;
			m_oEdgePainter.m_oEdge.FromXmlNode(oNodeAttr);

			m_sText = oNode.GetAttributeOrValue(_T("text"), _T(""));
			m_lMode = XmlUtils::GetInteger(oNode.GetAttributeOrValue(_T("mode"), _T("0")));
		}

		virtual void Draw(CPainter* pPainter, CDoubleRect& oRect, double& dWidth, double& dHeight)
		{
			if (NULL == pPainter->m_pRenderer)
				return;

			((CAVSGraphicsRenderer*)pPainter->m_pRenderer)->CheckFontManager();

			pPainter->SetDpi(oRect.GetWidth() * c_dInch_to_MM / dWidth, oRect.GetHeight() * c_dInch_to_MM / dHeight);

			CDoubleRect oNewRect;
			oNewRect.left		= oRect.left	* pPainter->m_dKoefPixToMM_X;
			oNewRect.top		= oRect.top		* pPainter->m_dKoefPixToMM_Y;
			oNewRect.right		= oRect.right	* pPainter->m_dKoefPixToMM_X;
			oNewRect.bottom		= oRect.bottom	* pPainter->m_dKoefPixToMM_Y;
			
			((CAVSGraphicsRenderer*)pPainter->m_pRenderer)->SetClip(oNewRect.left, oNewRect.top, oNewRect.GetWidth(), oNewRect.GetHeight());

			GenerateTransform(pPainter->m_pRenderer, oNewRect, dWidth, dHeight);
			CheckRect(oNewRect);

			Gdiplus::RectF oLayout((float)oNewRect.left, (float)oNewRect.top, (float)oNewRect.GetWidth(), (float)oNewRect.GetHeight());
			m_oMeasurer.DrawTextInRect(m_sText.GetBSTR(), oLayout, pPainter->m_pRenderer, m_oFont, m_oBrush, m_oFormat, NULL);

			pPainter->m_pRenderer->ResetTransform();
		}

		virtual void DrawOnRenderer(IAVSRenderer* pRenderer, CDoubleRect& oRect)
		{
			ImageStudio::Serialize::Paint::Common::CTextMeasurer oMeasurer;

			pRenderer->ResetTransform();
			pRenderer->PathCommandEnd();
			pRenderer->put_PenLineStartCap(Aggplus::LineCapSquare);
			pRenderer->put_PenLineEndCap(Aggplus::LineCapSquare);
			//pRenderer->SetCommandParams(0, 0, 0, 0, 0, 0);
			
			GenerateTransform(pRenderer, oRect);

			CheckRect(oRect);

			Gdiplus::RectF oLayout((float)oRect.left, (float)oRect.top, (float)oRect.GetWidth(), (float)oRect.GetHeight());
			oMeasurer.DrawTextInRect(m_sText.GetBSTR(), oLayout, pRenderer, m_oFont, m_oBrush, m_oFormat, &m_oEdgePainter);

			pRenderer->ResetTransform();
			pRenderer->PathCommandEnd();
			//pRenderer->SetCommandParams(0, 0, 0, 0, 0, 0);
		}

		virtual void CheckRect(CDoubleRect& oRect)
		{
			switch (m_lMode)
			{
			case 1:
			case 3:
			case 5:
			case 7:
				{
					double dCx = oRect.GetCentreX();
					double dCy = oRect.GetCentreY();

					double dW = oRect.GetWidth();
					double dH = oRect.GetHeight();
										
					double dDiag = sqrt(dW * dW + dH * dH);

					oRect.left	= dCx - dDiag / 2.0;
					oRect.right = dCx + dDiag / 2.0;

					break;
				}
			case 2:
			case 6:
				{
					double dH	= oRect.GetHeight();
					double dCx	= oRect.GetCentreX();

					oRect.left	= dCx - dH / 2.0;
					oRect.right = dCx + dH / 2.0;
					break;
				}
			default:
				break;
			}
		}
				
		void GenerateTransform(IAVSRenderer* pRenderer, CDoubleRect& oRect, double& dWidth, double& dHeight)
		{
			if (0 == m_lMode)
				return;
			
			switch (m_lMode)
			{
			case 1:
				{
					double dAngle = atan(dHeight / dWidth);
					dAngle = 180 * dAngle / M_PI;

					Aggplus::CMatrix oMatrix;
					oMatrix.RotateAt(dAngle, oRect.GetCentreX(), oRect.GetCentreY(), Aggplus::MatrixOrderAppend);
					
					double matrix[6];
					oMatrix.GetElements(matrix);

					pRenderer->SetTransform(matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

					break;					
				}
			case 2:
				{
					Aggplus::CMatrix oMatrix;
					oMatrix.RotateAt(-90.0, oRect.GetCentreX(), oRect.GetCentreY(), Aggplus::MatrixOrderAppend);
					
					double matrix[6];
					oMatrix.GetElements(matrix);

					pRenderer->SetTransform(matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

					break;
				}
			case 3:
				{
					double dAngle = atan(dHeight / dWidth);
					dAngle = 180 * dAngle / M_PI;

					Aggplus::CMatrix oMatrix;
					oMatrix.RotateAt(dAngle, oRect.GetCentreX(), oRect.GetCentreY(), Aggplus::MatrixOrderAppend);
						
					double matrix[6];
					oMatrix.GetElements(matrix);

					pRenderer->SetTransform(matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

					break;
				}
			case 4:
				{
					Aggplus::CMatrix oMatrix;
					oMatrix.RotateAt(180.0, oRect.GetCentreX(), oRect.GetCentreY(), Aggplus::MatrixOrderAppend);
						
					double matrix[6];
					oMatrix.GetElements(matrix);

					pRenderer->SetTransform(matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

					break;
				}
			case 5:
				{
					double dAngle = atan(dHeight / dWidth);
					dAngle = 180 * dAngle / M_PI;

					Aggplus::CMatrix oMatrix;
					oMatrix.RotateAt(-dAngle + 180.0, oRect.GetCentreX(), oRect.GetCentreY(), Aggplus::MatrixOrderAppend);
						
					double matrix[6];
					oMatrix.GetElements(matrix);

					pRenderer->SetTransform(matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

					break;
				}
			case 6:
				{
					Aggplus::CMatrix oMatrix;
					oMatrix.RotateAt(90, oRect.GetCentreX(), oRect.GetCentreY(), Aggplus::MatrixOrderAppend);
					
					double matrix[6];
					oMatrix.GetElements(matrix);

					pRenderer->SetTransform(matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

					break;
				}
			case 7:
				{
					double dAngle = atan(dHeight / dWidth);
					dAngle = 180 * dAngle / M_PI;

					Aggplus::CMatrix oMatrix;
					oMatrix.RotateAt(360 - dAngle, oRect.GetCentreX(), oRect.GetCentreY(), Aggplus::MatrixOrderAppend);
					
					double matrix[6];
					oMatrix.GetElements(matrix);

					pRenderer->SetTransform(matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5]);

					break;
				}
			default:
				break;
			}
		}

		void GenerateTransform(IAVSRenderer* pRenderer, CDoubleRect& oRect)
		{
			if (0 == m_lMode)
				return;

			double dWidth	= 0;
			double dHeight	= 0;

			pRenderer->get_Width(&dWidth);
			pRenderer->get_Height(&dHeight);

			return GenerateTransform(pRenderer, oRect, dWidth, dHeight);
		}
	};

	class CCacheDocument : public IPageEvent, public ICacheDocument
	{
	private:
		LONGLONG m_lMemoryUsageMaximum;
		
		CAtlArray<CCalculateThread> m_arThreads;
		LONG m_lCountThreads;
		
		CRITICAL_SECTION m_oCS;

		CAtlList<CCachePage*> m_arPages;
		BOOL m_bIsRepaint;

		LONG m_lThreadsPriority;

		IWatermark* m_pWatermark;

		CImageFilesCache m_oCache;

	public:
		CCacheDocument(LONG lPriority = THREAD_PRIORITY_NORMAL) : m_arThreads(), m_arPages(), m_oCache()
		{
			m_lMemoryUsageMaximum	= (LONGLONG)(1024 * 1024 * 500); // 500 Mb

			m_bIsRepaint			= FALSE;
			m_lThreadsPriority		= lPriority;

			InitCountThreads();

			m_pWatermark			= NULL;

			InitializeCriticalSection(&m_oCS);
		}
		~CCacheDocument()
		{
			Stop();

			POSITION pos = m_arPages.GetHeadPosition();
			while (NULL != pos)
			{
				CCachePage* pPage = m_arPages.GetNext(pos);
				RELEASEOBJECT(pPage);
			}
			m_arPages.RemoveAll();
			
			m_arThreads.RemoveAll();

			RELEASEOBJECT(m_pWatermark);

			DeleteCriticalSection(&m_oCS);
		}

	protected:		
		void InitCountThreads()
		{
			DWORD_PTR dwProcessAffinityMask		= 0;
			DWORD_PTR dwSystemAffinityMask		= 0;
			if (!GetProcessAffinityMask(GetCurrentProcess(), &dwProcessAffinityMask, &dwSystemAffinityMask))
				return;

			LONG lRet = 0;
			DWORD_PTR dwValue = dwProcessAffinityMask;
			for (int i = 0; i < sizeof(DWORD_PTR); i++)
			{
				if (0!=(1 & dwValue))
					lRet++;
				dwValue = (dwValue >> 1);
			}

			//m_lCountThreads = max(1, 3 * lRet / 2);
			m_lCountThreads = max(1, lRet);

			//m_lCountThreads = 1;

			for (LONG i = 0; i < m_lCountThreads; ++i)
			{
				m_arThreads.Add();
				m_arThreads[i].SetDocument(this);
			}
		}

		BOOL IsWorked()
		{
			// нужно вернуть, работает ли хотя бы одна страница (чтобы постепенно их отрисовывать)
			POSITION pos = m_arPages.GetHeadPosition();
			while (NULL != pos)
			{
				CCachePage* pPage = m_arPages.GetNext(pos);
				if ((NULL != pPage) && (pPage->IsWorked()))
				{
					return TRUE;
				}
			}

			return FALSE;
		}

		BOOL IsWorkedOrNeedWorked()
		{
			// нужно вернуть, работает ли хотя бы одна страница (чтобы постепенно их отрисовывать)
			POSITION pos = m_arPages.GetHeadPosition();
			while (NULL != pos)
			{
				CCachePage* pPage = m_arPages.GetNext(pos);
				if ((NULL != pPage) && (pPage->IsWorkedOrNeedWorked()))
				{
					return TRUE;
				}
			}

			return FALSE;
		}

	public:
		void Calculate(CDrawingDocument& oCalculateDocument, CDrawingDocument& oRenderingDocument)
		{
			CTemporaryCS oCS(&m_oCS);
			
			size_t nCountRend = oRenderingDocument.m_arPages.GetCount();
			
			if (0 == nCountRend)
			{
				// нужно удалить ненужные страницы, т.е. все

				POSITION pos = m_arPages.GetHeadPosition();
				while (NULL != pos)
				{
					CCachePage* pPage = m_arPages.GetNext(pos);
					if (NULL != pPage)
					{
						pPage->SetRemoved();
					}
				}

				return;
			}

			BYTE* pCheckNew = new BYTE[nCountRend];
			memset(pCheckNew, 0x01, nCountRend);

			size_t nIndex = 0;
			BOOL bIsFound = FALSE;

			POSITION pos = m_arPages.GetHeadPosition();
			while (NULL != pos)
			{
				CCachePage* pPage = m_arPages.GetNext(pos);
				if (NULL != pPage)
				{
					bIsFound = FALSE;

					for (nIndex = 0; nIndex < nCountRend; ++nIndex)
					{
						CDrawingPage& pFound = oRenderingDocument.m_arPages[nIndex];
						if (pFound.m_pPage->m_pPage == pPage->GetMeta())
						{
							CPageInfo oInfo;
							oInfo.m_lWidthPix		= (LONG)pFound.m_oGlobalBounds.GetWidth();
							oInfo.m_lHeightPix		= (LONG)pFound.m_oGlobalBounds.GetHeight();

							pPage->CheckNeedWork(oInfo);
							
							// это не новая страница
							pCheckNew[nIndex] = 0x00;
							bIsFound = TRUE;
							break;
						}
					}

					if (FALSE == bIsFound)
					{
						pPage->SetRemoved();
					}				
				}
			}

			// теперь просто нужно добавить новые
			for (size_t i = 0; i < nCountRend; ++i)
			{
				if (0x01 == pCheckNew[i])
				{
					CDrawingPage& pFound = oRenderingDocument.m_arPages[i];
					
					CPageInfo oInfo;
					oInfo.m_lWidthPix		= (LONG)pFound.m_oGlobalBounds.GetWidth();
					oInfo.m_lHeightPix		= (LONG)pFound.m_oGlobalBounds.GetHeight();

					CCachePage* pNewPage = new CCachePage(this);
					pNewPage->SetMeta(pFound.m_pPage->m_pPage);
					pNewPage->SetImageCache(&m_oCache);
					pNewPage->SetNeedWork(oInfo);

					m_arPages.AddTail(pNewPage);
				}
			}

			delete[] pCheckNew;			
		}
		void Stop()
		{
			CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

			// сначала всем страницам проставим BREAK
			POSITION pos = m_arPages.GetHeadPosition();
			while (NULL != pos)
			{
				CCachePage* pPage = m_arPages.GetNext(pos);
				if (NULL != pPage)
				{
					pPage->SetBreak();
				}
			}

			RELEASEOBJECT(pCS);

			// теперь стопим потоки
			for (LONG i = 0; i < m_lCountThreads; ++i)
			{
				m_arThreads[i].StopWork();
			}

			pCS = new CTemporaryCS(&m_oCS);

			// теперь удаляем все страницы
			pos = m_arPages.GetHeadPosition();
			while (NULL != pos)
			{
				CCachePage* pPage = m_arPages.GetNext(pos);
				RELEASEOBJECT(pPage);
			}
			m_arPages.RemoveAll();

			RELEASEOBJECT(pCS);

			m_oCache.Clear();

		}
		void Start()
		{
			// просто стартуем потоки
			for (LONG i = 0; i < m_lCountThreads; ++i)
			{
				m_arThreads[i].Start(m_lThreadsPriority);
			}
		}
		CCachePage* CalculatePage()
		{
			CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

			CCachePage* pCachePage = NULL;

			POSITION pos	= m_arPages.GetHeadPosition();
			while (NULL != pos)
			{
				POSITION posOld = pos;
				CCachePage* pPage = m_arPages.GetNext(pos);
				if ((NULL == pPage) || (CCachePage::psRemoved == pPage->GetStatus()))
				{
					// удаляем страницу
					RELEASEOBJECT(pPage);
					m_arPages.RemoveAt(posOld);
					continue;
				}
				if ((NULL == pCachePage) && (CCachePage::psNeedWorked == pPage->GetStatus()))
				{
					pCachePage = pPage;
					// не выходим, чтобы удалить все ненужные

					// + проставим, что она не Need, раз мы ее нашли уже
					pCachePage->SetStatus(CCachePage::psWorked);
				}
			}

			RELEASEOBJECT(pCS);

			return pCachePage;
		}
		inline BOOL IsRepaint()
		{
			CTemporaryCS oCS(&m_oCS);
			return (m_bIsRepaint || IsWorked());
		}
		inline BOOL IsRepaintOrWorked()
		{
			CTemporaryCS oCS(&m_oCS);
			return (m_bIsRepaint || IsWorkedOrNeedWorked());
		}
		inline BOOL IsGlobalRepaint()
		{
			CTemporaryCS oCS(&m_oCS);
			return m_bIsRepaint;
		}
		inline BOOL IsRepaintReset()
		{
			CTemporaryCS oCS(&m_oCS);
			BOOL bRes = (m_bIsRepaint || IsWorked());
			m_bIsRepaint = FALSE;
			return bRes;
		}
		void SendRepaint()
		{
			CTemporaryCS oCS(&m_oCS);
			m_bIsRepaint = TRUE;
			return;
		}
		
		inline virtual void SendEventComplete()
		{
			//CTemporaryCS oCS(&m_oCS);
			m_bIsRepaint = TRUE;
		}

		void Draw(CPainter* pPainter, CDrawingDocument& oRenderingDocument, LONG lBorderLeft, LONG lBorderTop)
		{
			CTemporaryCS oCS(&m_oCS);

			size_t nCount = oRenderingDocument.m_arPages.GetCount();

			if ((NULL == m_pWatermark) || (!m_pWatermark->IsInit()))
			{			
				for (size_t i = 0; i < nCount; ++i)
				{
					CCachePage* pCachePage = GetPage(oRenderingDocument.m_arPages[i].m_pPage);
					if (NULL != pCachePage)
					{
						CDoubleRect oRect = oRenderingDocument.m_arPages[i].m_oGlobalBounds;
						oRect.Offset(lBorderLeft, lBorderTop);
						
						pCachePage->Draw(pPainter, oRect);
					}
				}
			}
			else
			{
				for (size_t i = 0; i < nCount; ++i)
				{
					CCachePage* pCachePage = GetPage(oRenderingDocument.m_arPages[i].m_pPage);
					if (NULL != pCachePage)
					{
						CDoubleRect oRect = oRenderingDocument.m_arPages[i].m_oGlobalBounds;
						oRect.Offset(lBorderLeft, lBorderTop);
						
						pCachePage->Draw(pPainter, oRect);

						double dWidth	= 0;
						double dHeight	= 0;
						pCachePage->GetMeta()->get_Width(&dWidth);
						pCachePage->GetMeta()->get_Height(&dHeight);

						m_pWatermark->Draw(pPainter, oRect, dWidth, dHeight);
					}
				}				
			}
		}

		void SetWatermark(BSTR bsXml)
		{
			CTemporaryCS oCS(&m_oCS);
			
			if (NULL == m_pWatermark)
			{
				m_pWatermark = new CWatermark();
			}

			m_pWatermark->LoadFromXML(bsXml);
		}

		void InitFonts()
		{
			//for (size_t i = 0; i < m_arThreads.GetCount(); ++i)
			//{
			//	m_arThreads[i].InitFonts();
			//}
		}

	protected:
			
		CCachePage* GetPage(CPage* pPage)
		{
			// ищем страницу с нужным метафайлом
			
			if (NULL == pPage)
				return NULL;

			POSITION pos = m_arPages.GetHeadPosition();
			while (NULL != pos)
			{
				CCachePage* pCachePage = m_arPages.GetNext(pos);
				if ((NULL != pPage) && (pCachePage->GetMeta() == pPage->m_pPage))
				{
					return pCachePage;
				}
			}
			return NULL;
		}
	};
}