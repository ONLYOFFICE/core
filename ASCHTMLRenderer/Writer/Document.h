#pragma once
#include "Writer\File.h"

namespace NSHtmlRenderer
{
	class IBaseMatrixUpdater
	{
	public: 
		virtual void OnBaseMatrixUpdate(const double& dWidth, const double& dHeight) = 0;
	};

	class CPageInfo
	{
	private:
		double m_dWidthMM;
		double m_dHeightMM;

		bool m_bInitW;
		bool m_bInitH;

		IBaseMatrixUpdater* m_pUpdater;

	public:
		CPageInfo()
		{
			m_dWidthMM	= 190;
			m_dHeightMM = 270;

			m_bInitW	= false;
			m_bInitH	= false;

			m_pUpdater	= NULL;
		}
		CPageInfo(const CPageInfo& oSrc)
		{
			*this = oSrc;
		}
		CPageInfo& operator=(const CPageInfo& oSrc)
		{
			m_dWidthMM	= oSrc.m_dWidthMM;
			m_dHeightMM = oSrc.m_dHeightMM;

			m_bInitW	= oSrc.m_bInitW;
			m_bInitH	= oSrc.m_bInitH;

			m_pUpdater	= oSrc.m_pUpdater;
		}

		inline double GetWidth()
		{
			return m_dWidthMM;
		}
		inline double GetHeight()
		{
			return m_dHeightMM;
		}
		inline void SetWidth(const double& width)
		{
			m_dWidthMM	= width;
			m_bInitW	= true;

			if (m_bInitH && (NULL != m_pUpdater))
			{
				m_pUpdater->OnBaseMatrixUpdate(m_dWidthMM, m_dHeightMM);
			}
		}
		inline void SetHeight(const double& height)
		{
			m_dHeightMM	= height;
			m_bInitH	= true;

			if (m_bInitW && (NULL != m_pUpdater))
			{
				m_pUpdater->OnBaseMatrixUpdate(m_dWidthMM, m_dHeightMM);
			}
		}
		inline void SetUpdater(IBaseMatrixUpdater* pUpdater)
		{
			m_pUpdater = pUpdater;
		}
	};

	class CDocument
	{
	private:
		
	public:
		CAtlArray<CPageInfo> m_arrPages;
		IBaseMatrixUpdater*	 m_pUpdater;	

	public:
		CDocument() : m_arrPages()
		{
			m_pUpdater = NULL;
		}
		inline void SetUpdater(IBaseMatrixUpdater* pUpdater)
		{
			m_pUpdater = pUpdater;
		}

	public:
		
		inline void NewPage()
		{
			m_arrPages.Add();
			
			size_t nCount = m_arrPages.GetCount();
			m_arrPages[nCount - 1].SetUpdater(m_pUpdater);
		}
		inline void SetWidth(const double& dValue)
		{
			size_t nCount = m_arrPages.GetCount();
			if (nCount > 0)
			{
				m_arrPages[nCount - 1].SetWidth(dValue);
			}
		}
		inline void SetHeight(const double& dValue)
		{
			size_t nCount = m_arrPages.GetCount();
			if (nCount > 0)
			{
				m_arrPages[nCount - 1].SetHeight(dValue);
			}
		}

		// --------------------------------------------------------------------

		CString GetThumbnailsHTML()
		{
			CString strHTML = _T("<html>");

			strHTML += GetThumbnailsHeader();
			strHTML += GetThumbnailsBody();

			strHTML += _T("</html>");

			return strHTML;
		}
		CString GetViewerHTML()
		{
			CString strHTML = _T("<html>");

			strHTML += GetViewerHeader();
			strHTML += GetViewerBody();

			strHTML += _T("</html>");

			return strHTML;
		}

		CString GetThumbnailBlockHTML(int nPageNum)
		{
			CString strPage = _T("");
			strPage.Format(_T("%d"), nPageNum);
			
			CString strResult = _T("<div class=\"blockpage\"><div class=\"blockthumbnail\" align=\"center\"><img align=\"center\" src=\"thumbnails\\page") +
									strPage + _T(".png\" onClick=\"OnChangePage(") + strPage + _T(")\" width=\"100%\" height=\"90%\"/>") + _T("page") + strPage + _T("</div></div>");
			return strResult;
		}
		CString GetThumbnailsBody()
		{
			CString strBody = _T("<body bgcolor=\"#FEFEFE\">");

			size_t nCount = m_arrPages.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				strBody += GetThumbnailBlockHTML((int)(i + 1));
			}

			strBody += _T("</body>");
			return strBody;
		}

		CString GetThumbnailsHeader()
		{
			CString strHead = _T("<head>\
									<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\
									<title>thumbnails</title>\
									<style type=\"text/css\">\
										.blockpage {\
										width: 80%;\
										height: 200px;\
										background: #FEFEFE;\
										padding: 10px;\
										float: none;\
										text-align: center;\
										}\
										.blockthumbnail {\
										width: 100%;\
										height: 100%;\
										background: #FEFEFE;\
										padding: 0px;\
										float: none;\
										}\
									</style>\
									<script language=\"JavaScript\">\
										function OnChangePage(pageNum)\
										{\
											top.frames['viewer'].OnChangePage(pageNum);\
										}\
									</script>\
								</head>");

			return strHead;
		}

		CString GetViewerHeader()
		{
			CString strHead = _T("<head>\
									 <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\
									 <title>viewer</title>\
									 <style type=\"text/css\">\
										 .blockpage {\
										 position: relative;\
										 left: 20%;\
										 width: 60%;\
										 height: 1200px;\
										 background: #FFFFFF;\
										 padding: 10px;\
										 border-style: outset;\
										 border-color: #F0F0F0;\
										 border-width: 2px 3px 3px 2px;\
										 float: none;\
										 text-align: center;\
										 }\
										 .blockpagebetween {\
										 width: 100%;\
										 height: 20px;\
										 background: #FEFEFE;\
										 padding: 0px;\
										 float: none;\
										 text-align: center;\
										 }\
										 .blockpagenatural {\
										 width: 100%;\
										 height: 100%;\
										 background: #FEFEFE;\
										 padding: 0px;\
										 float: none;\
										 }\
									 </style>\
									 <script language=\"JavaScript\">\
										 function OnChangePage(pageNum)\
										 {\
											var nPage = Number(pageNum);\
											var position = ((nPage - 1) * 1225 + (nPage - 1) * 20);\
											scroll(0, position);\
										 }\
									 </script>\
								 </head>");

			return strHead;
		}

		CString GetViewerBlockHTML(int nPageNum)
		{
			CString strPage = _T("");
			strPage.Format(_T("%d"), nPageNum);
			
			CString strResult = _T("<div class=\"blockpage\">\n<div class=\"blockpagenatural\" align=\"center\">\n<img align=\"center\" src=\"thumbnails\\page") +
									strPage + _T(".png\" onClick=\"OnChangePage(") + strPage + _T(")\" width=\"100%\" height=\"100%\"/>\n") + _T("</div>\n</div>\n") + _T("<div class=\"blockpagebetween\"></div>\n");
			return strResult;
		}
		CString GetViewerBody()
		{
			CString strBody = _T("<body bgcolor=\"#FEFEFE\">");

			size_t nCount = m_arrPages.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				strBody += GetViewerBlockHTML((int)(i + 1));
			}

			strBody += _T("</body>");

			return strBody;
		}

		CString GetMenuHTML()
		{
			CString strHtml = _T("<html>\
									 <head>\
										 <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\
										 <title>menu</title>\
									 </head>\
										<body bgcolor=\"#5F5F5F\">\
										</body>\
								</html>");
			return strHtml;
		}
		CString GetDocumentHTML()
		{
			CString strHtml = _T("<html>\
									 <head>\
										 <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\
										 <title>document viewer</title>\
									 </head>\
									 <frameset rows=\"50,*\" framespacing=\"0\" frameborder=\"0\">\
										 <frame src=\"menu.html\" name=\"menu\" noresize border=\"1\" bordercolor=\"#F0F0F0\" scrolling=\"no\"></frame>\
										 <frameset cols=\"*,200\">\
											 <frame id=\"id_viewer\" src=\"viewer.html\" name=\"viewer\" noresize></frame>\
											 <frame id=\"id_thumbnails\" src=\"thumbnails.html\" name=\"thumbnail\"></frame>\
										 </frameset>\
									 </frameset>\
								 </html>");
			return strHtml;
		}

		static CStringA ConvertToUTF8 (CStringW aUnicodeXML)
		{
			// Convert to UTF-8
			int iSize = WideCharToMultiByte (CP_UTF8, 0, aUnicodeXML, -1, NULL, 0, NULL, NULL);
			if (iSize <= 0)
			{
				ATLTRACE2 ("ConvertToUTF8() error (size detection): 0x%x\n", GetLastError());
				return CStringA(aUnicodeXML); // Conversion to ANSI
			}
			CStringA sOutXML;
			if (0 == WideCharToMultiByte (CP_UTF8, 0, aUnicodeXML, -1, sOutXML.GetBuffer(iSize + 1), iSize, NULL, NULL))
			{
				ATLTRACE2 ("ConvertToUTF8() error (utf-8 conversion): 0x%x\n", GetLastError());
				return CStringA(aUnicodeXML);
			}
			sOutXML.ReleaseBuffer();
			return sOutXML;
		}

		void CreateDirectories(CString strHTML)
		{
			CDirectory::CreateDirectory(strHTML);
			CDirectory::CreateDirectory(strHTML, _T("thumbnails"));
		}
		
		void CreateHTMLs(CString strHTML)
		{
			CFile oFile;
			oFile.CreateFile(strHTML + _T("\\docviewer.html"));
			CString strDocHTML = GetDocumentHTML();
			CStringA str1 = ConvertToUTF8(strDocHTML);
			oFile.WriteFile((void*)str1.GetBuffer(), str1.GetLength());
			oFile.CloseFile();

			oFile.CreateFile(strHTML + _T("\\menu.html"));
			CString strMenuHTML = GetMenuHTML();
			CStringA str2 = ConvertToUTF8(strMenuHTML);
			oFile.WriteFile((void*)str2.GetBuffer(), str2.GetLength());
			oFile.CloseFile();	

			oFile.CreateFile(strHTML + _T("\\viewer.html"));
			CString strViewerHTML = GetViewerHTML();
			CStringA str3 = ConvertToUTF8(strViewerHTML);
			oFile.WriteFile((void*)str3.GetBuffer(), str3.GetLength());
			oFile.CloseFile();

			oFile.CreateFile(strHTML + _T("\\thumbnails.html"));
			CString strThHTML = GetThumbnailsHTML();
			CStringA str4 = ConvertToUTF8(strThHTML);
			oFile.WriteFile((void*)str4.GetBuffer(), str4.GetLength());
			oFile.CloseFile();
		}

	};

	class CThumbnails
	{
	private:
		Graphics::IASCRenderer*				m_pRenderer;
		MediaCore::IAVSUncompressedVideoFrame*	m_pFrame;

		LONG	m_lWidth;
		LONG	m_lHeight;

	public:
		CThumbnails()
		{
			m_pRenderer		= NULL;
			m_pFrame		= NULL;
			m_lWidth		= 0;
			m_lHeight		= 0;
		}
		~CThumbnails()
		{
			RELEASEINTERFACE(m_pRenderer);
			RELEASEINTERFACE(m_pFrame);
		}

	public:
		void Create(const double& dWidth, const double& dHeight, LONG lSizeMax = 200)
		{
			LONG lWidthNew	= 0;
			LONG lHeightNew = 0;

			if (dWidth >= dHeight)
			{
				lWidthNew	= lSizeMax;
				lHeightNew	= (LONG)((dHeight / dWidth) * lWidthNew);
			}
			else
			{
				lHeightNew	= lSizeMax;
				lWidthNew	= (LONG)((dWidth / dHeight) * lHeightNew);
			}

			if ((m_lWidth == lWidthNew) && (m_lHeight == lHeightNew) && (NULL != m_pFrame))
			{
				// размер не поменялся - значит и память перевыделять не нужно
				BYTE* pBuffer	= NULL;
				m_pFrame->get_Buffer(&pBuffer);
				memset(pBuffer, 0xFF, 4 * m_lWidth * m_lHeight);

				CreateRenderer();
				return;
			}
			
			RELEASEINTERFACE(m_pFrame);
			m_lWidth	= lWidthNew;
			m_lHeight	= lHeightNew;

			CreateMediaData();
			CreateRenderer();

			m_pRenderer->put_Width(dWidth);
			m_pRenderer->put_Height(dHeight);
		}
		inline void Save(CString& strFile)
		{
			SaveFrame(strFile);			
		}

	protected:
		void CreateMediaData()
		{
			RELEASEINTERFACE(m_pFrame);
			CoCreateInstance(__uuidof( MediaCore::CAVSUncompressedVideoFrame), NULL ,CLSCTX_INPROC_SERVER,
				__uuidof(MediaCore::IAVSUncompressedVideoFrame), (void**)&m_pFrame);

			m_pFrame->put_Width(m_lWidth);
			m_pFrame->put_Height(m_lHeight);
			m_pFrame->put_ColorSpace(64);
			m_pFrame->SetDefaultStrides();
			m_pFrame->AllocateBuffer(-1);

			BYTE* pBuffer = NULL;
			m_pFrame->get_Buffer(&pBuffer);
			memset(pBuffer, 0xFF, 4 * m_lWidth * m_lHeight);
		}
		void CreateRenderer()
		{
			// теперь на всякий случай (сбросить все состояния) - пересоздадим рендерер
			RELEASEINTERFACE(m_pRenderer);
			
			Graphics::IASCGraphicsRenderer* pGrRenderer;			
			CoCreateInstance(__uuidof( Graphics::CASCGraphicsRenderer), NULL, CLSCTX_INPROC_SERVER, 
				__uuidof(Graphics::IASCGraphicsRenderer), (void**)&pGrRenderer);
						
			//ставим FontManager
			VARIANT vtVariant;
			vtVariant.vt = VT_UNKNOWN;
			vtVariant.punkVal = NULL;
			pGrRenderer->SetAdditionalParam( L"FontManager", vtVariant );

			IUnknown* punkFrame = NULL;
			m_pFrame->QueryInterface(IID_IUnknown, (void**)&punkFrame);

			pGrRenderer->CreateFromMediaData(punkFrame, 0, 0, m_lWidth, m_lHeight);

			RELEASEINTERFACE(punkFrame);
			pGrRenderer->QueryInterface(Graphics::IID_IASCRenderer, (void**)&m_pRenderer);
			RELEASEINTERFACE(pGrRenderer);
		}

		void SaveFrame(const CString& strFile)
		{
			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT var;
			var.vt = VT_UNKNOWN;
			var.punkVal = (IUnknown*)m_pFrame;
			pTransform->SetSource(0, var);

			CString strXml = _T("<transforms><ImageFile-SaveAsPng destinationpath=\"") + strFile + _T("\" format=\"888\"></ImageFile-SaveAsPng></transforms>");
			
			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransform->SetXml(bsXml, &vbSuccess);
			SysFreeString(bsXml);

			pTransform->Transform(&vbSuccess);

			RELEASEINTERFACE(pTransform);
		}

	public:
		inline HRESULT get_Type(LONG* lType)
		{
			return m_pRenderer->get_Type(lType);
		}

		//-------- Функции для работы со страницей --------------------------------------------------
		inline HRESULT NewPage()
		{
			if (NULL == m_pRenderer)
				return S_OK;

			return m_pRenderer->NewPage();
		}
		inline HRESULT get_Height(double* dHeight)
		{
			return m_pRenderer->get_Height(dHeight);
		}
		inline HRESULT put_Height(double dHeight)
		{
			if (NULL == m_pRenderer)
				return S_OK;

			return m_pRenderer->put_Height(dHeight);
		}
		inline HRESULT get_Width(double* dWidth)
		{
			return m_pRenderer->get_Width(dWidth);
		}
		inline HRESULT put_Width(double dWidth)
		{
			if (NULL == m_pRenderer)
				return S_OK;

			return m_pRenderer->put_Width(dWidth);
		}
		inline HRESULT get_DpiX(double* dDpiX)
		{
			return m_pRenderer->get_DpiX(dDpiX);
		}
		inline HRESULT get_DpiY(double* dDpiY)
		{
			return m_pRenderer->get_DpiY(dDpiY);
		}

	// pen --------------------------------------------------------------------------------------
		inline HRESULT SetPen(BSTR bsXML)
		{
			return m_pRenderer->SetPen(bsXML);
		}
		inline HRESULT get_PenColor(LONG* lColor)
		{
			return m_pRenderer->get_PenColor(lColor);
		}
		inline HRESULT put_PenColor(LONG lColor)
		{
			return m_pRenderer->put_PenColor(lColor);
		}
		inline HRESULT get_PenAlpha(LONG* lAlpha)
		{
			return m_pRenderer->get_PenAlpha(lAlpha);
		}
		inline HRESULT put_PenAlpha(LONG lAlpha)
		{
			return m_pRenderer->put_PenAlpha(lAlpha);
		}
		inline HRESULT get_PenSize(double* dSize)
		{
			return m_pRenderer->get_PenSize(dSize);
		}
		inline HRESULT put_PenSize(double dSize)
		{
			return m_pRenderer->put_PenSize(dSize);
		}
		inline HRESULT get_PenDashStyle(BYTE* val)
		{
			return m_pRenderer->get_PenDashStyle(val);
		}
		inline HRESULT put_PenDashStyle(BYTE val)
		{
			return m_pRenderer->put_PenDashStyle(val);
		}
		inline HRESULT get_PenLineStartCap(BYTE* val)
		{
			return m_pRenderer->get_PenLineStartCap(val);
		}
		inline HRESULT put_PenLineStartCap(BYTE val)
		{
			return m_pRenderer->put_PenLineStartCap(val);
		}
		inline HRESULT get_PenLineEndCap(BYTE* val)
		{
			return m_pRenderer->get_PenLineEndCap(val);
		}
		inline HRESULT put_PenLineEndCap(BYTE val)
		{
			return m_pRenderer->put_PenLineEndCap(val);
		}
		inline HRESULT get_PenLineJoin(BYTE* val)
		{
			return m_pRenderer->get_PenLineJoin(val);
		}
		inline HRESULT put_PenLineJoin(BYTE val)
		{
			return m_pRenderer->put_PenLineJoin(val);
		}
		inline HRESULT get_PenDashOffset(double* val)
		{
			return m_pRenderer->get_PenDashOffset(val);
		}
		inline HRESULT put_PenDashOffset(double val)
		{
			return m_pRenderer->put_PenDashOffset(val);
		}
		inline HRESULT get_PenAlign(LONG* val)
		{
			return m_pRenderer->get_PenAlign(val);
		}
		inline HRESULT put_PenAlign(LONG val)
		{
			return m_pRenderer->put_PenAlign(val);
		}
		inline HRESULT get_PenMiterLimit(double* val)
		{
			return m_pRenderer->get_PenMiterLimit(val);
		}
		inline HRESULT put_PenMiterLimit(double val)
		{
			return m_pRenderer->put_PenMiterLimit(val);
		}
		inline HRESULT PenDashPattern(SAFEARRAY* pPattern)
		{
			return m_pRenderer->PenDashPattern(pPattern);
		}

	// brush ------------------------------------------------------------------------------------
		inline HRESULT SetBrush(BSTR bsXML)
		{
			return m_pRenderer->SetBrush(bsXML);
		}
		inline HRESULT get_BrushType(LONG* lType)
		{
			return m_pRenderer->get_BrushType(lType);
		}
		inline HRESULT put_BrushType(LONG lType)
		{
			return m_pRenderer->put_BrushType(lType);
		}
		inline HRESULT get_BrushColor1(LONG* lColor)
		{
			return m_pRenderer->get_BrushColor1(lColor);
		}
		inline HRESULT put_BrushColor1(LONG lColor)
		{
			return m_pRenderer->put_BrushColor1(lColor);
		}
		inline HRESULT get_BrushAlpha1(LONG* lAlpha)
		{
			return m_pRenderer->get_BrushAlpha1(lAlpha);
		}
		inline HRESULT put_BrushAlpha1(LONG lAlpha)
		{
			return m_pRenderer->put_BrushAlpha1(lAlpha);
		}
		inline HRESULT get_BrushColor2(LONG* lColor)
		{
			return m_pRenderer->get_BrushColor2(lColor);
		}
		inline HRESULT put_BrushColor2(LONG lColor)
		{
			return m_pRenderer->put_BrushColor2(lColor);
		}
		inline HRESULT get_BrushAlpha2(LONG* lAlpha)
		{
			return m_pRenderer->get_BrushAlpha2(lAlpha);
		}
		inline HRESULT put_BrushAlpha2(LONG lAlpha)
		{
			return m_pRenderer->put_BrushAlpha2(lAlpha);
		}
		inline HRESULT get_BrushTexturePath(BSTR* bsPath)
		{
			return m_pRenderer->get_BrushTexturePath(bsPath);
		}
		inline HRESULT put_BrushTexturePath(BSTR bsPath)
		{
			return m_pRenderer->put_BrushTexturePath(bsPath);
		}
		inline HRESULT get_BrushTextureMode(LONG* lMode)
		{
			return m_pRenderer->get_BrushTextureMode(lMode);
		}
		inline HRESULT put_BrushTextureMode(LONG lMode)
		{
			return m_pRenderer->put_BrushTextureMode(lMode);
		}
		inline HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)
		{
			return m_pRenderer->get_BrushTextureAlpha(lTxAlpha);
		}
		inline HRESULT put_BrushTextureAlpha(LONG lTxAlpha)
		{
			return m_pRenderer->put_BrushTextureAlpha(lTxAlpha);
		}
		inline HRESULT get_BrushLinearAngle(double* dAngle)
		{
			return m_pRenderer->get_BrushLinearAngle(dAngle);
		}
		inline HRESULT put_BrushLinearAngle(double dAngle)
		{
			return m_pRenderer->put_BrushLinearAngle(dAngle);
		}
		inline HRESULT BrushRect(BOOL val, double left, double top, double width, double height)
		{
			return m_pRenderer->BrushRect(val, left, top, width, height);
		}

	// font -------------------------------------------------------------------------------------
		inline HRESULT SetFont(BSTR bsXML)
		{
			return m_pRenderer->SetFont(bsXML);
		}
		inline HRESULT get_FontName(BSTR* bsName)
		{
			return m_pRenderer->get_FontName(bsName);
		}
		inline HRESULT put_FontName(BSTR bsName)
		{
			return m_pRenderer->put_FontName(bsName);
		}
		inline HRESULT get_FontPath(BSTR* bsName)
		{
			return m_pRenderer->get_FontPath(bsName);
		}
		inline HRESULT put_FontPath(BSTR bsName)
		{
			return m_pRenderer->put_FontPath(bsName);
		}
		inline HRESULT get_FontSize(double* dSize)
		{
			return m_pRenderer->get_FontSize(dSize);
		}
		inline HRESULT put_FontSize(double dSize)
		{
			return m_pRenderer->put_FontSize(dSize);
		}
		inline HRESULT get_FontStyle(LONG* lStyle)
		{
			return m_pRenderer->get_FontStyle(lStyle);
		}
		inline HRESULT put_FontStyle(LONG lStyle)
		{
			return m_pRenderer->put_FontStyle(lStyle);
		}
		inline HRESULT get_FontStringGID(BOOL* bGID)
		{
			return m_pRenderer->get_FontStringGID((LONG*)bGID);
		}
		inline HRESULT put_FontStringGID(BOOL bGID)
		{
			return m_pRenderer->put_FontStringGID(bGID);
		}
		inline HRESULT get_FontCharSpace(double* dSpace)
		{
			return m_pRenderer->get_FontCharSpace(dSpace);
		}
		inline HRESULT put_FontCharSpace(double dSpace)
		{
			return m_pRenderer->put_FontCharSpace(dSpace);
		}

	// shadow -----------------------------------------------------------------------------------
		inline HRESULT SetShadow(BSTR bsXML)
		{
			return m_pRenderer->SetShadow(bsXML);
		}
		inline HRESULT get_ShadowDistanceX(double* val)
		{
			return m_pRenderer->get_ShadowDistanceX(val);
		}
		inline HRESULT put_ShadowDistanceX(double val)
		{
			return m_pRenderer->put_ShadowDistanceX(val);
		}
		inline HRESULT get_ShadowDistanceY(double* val)
		{
			return m_pRenderer->get_ShadowDistanceY(val);
		}
		inline HRESULT put_ShadowDistanceY(double val)
		{
			return m_pRenderer->put_ShadowDistanceY(val);
		}
		inline HRESULT get_ShadowBlurSize(double* val)
		{
			return m_pRenderer->get_ShadowBlurSize(val);
		}
		inline HRESULT put_ShadowBlurSize(double val)
		{
			return m_pRenderer->put_ShadowBlurSize(val);
		}
		inline HRESULT get_ShadowColor(LONG* val)
		{
			return m_pRenderer->get_ShadowColor(val);
		}
		inline HRESULT put_ShadowColor(LONG val)
		{
			return m_pRenderer->put_ShadowColor(val);
		}
		inline HRESULT get_ShadowAlpha(LONG* val)
		{
			return m_pRenderer->get_ShadowAlpha(val);
		}
		inline HRESULT put_ShadowAlpha(LONG val)
		{
			return m_pRenderer->put_ShadowAlpha(val);
		}
		inline HRESULT get_ShadowVisible(BOOL* val)
		{
			return m_pRenderer->get_ShadowVisible((LONG*)val);
		}
		inline HRESULT put_ShadowVisible(BOOL val)
		{
			return m_pRenderer->put_ShadowVisible(val);
		}

	// edge -------------------------------------------------------------------------------------
		inline HRESULT SetEdgeText(BSTR bsXML)
		{
			return m_pRenderer->SetEdgeText(bsXML);
		}
		inline HRESULT get_EdgeVisible(LONG* val)
		{
			return m_pRenderer->get_EdgeVisible(val);
		}
		inline HRESULT put_EdgeVisible(LONG val)
		{
			return m_pRenderer->put_EdgeVisible(val);
		}
		inline HRESULT get_EdgeColor(LONG* val)
		{
			return m_pRenderer->get_EdgeColor(val);
		}
		inline HRESULT put_EdgeColor(LONG val)
		{
			return m_pRenderer->put_EdgeColor(val);
		}
		inline HRESULT get_EdgeAlpha(LONG* val)
		{
			return m_pRenderer->get_EdgeAlpha(val);
		}
		inline HRESULT put_EdgeAlpha(LONG val)
		{
			return m_pRenderer->put_EdgeAlpha(val);
		}
		inline HRESULT get_EdgeDist(double* val)
		{
			return m_pRenderer->get_EdgeDist(val);
		}
		inline HRESULT put_EdgeDist(double val)
		{
			return m_pRenderer->put_EdgeDist(val);
		}

	//-------- Функции для вывода текста --------------------------------------------------------
		inline HRESULT CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			return m_pRenderer->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		}
		inline HRESULT CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			return m_pRenderer->CommandDrawTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
		}

	//-------- Маркеры для команд ---------------------------------------------------------------
		inline HRESULT BeginCommand(DWORD lType)
		{
			return m_pRenderer->BeginCommand(lType);
		}
		inline HRESULT EndCommand(DWORD lType)
		{
			return m_pRenderer->EndCommand(lType);
		}

	//-------- Функции для работы с Graphics Path -----------------------------------------------
		inline HRESULT PathCommandMoveTo(double fX, double fY)
		{
			return m_pRenderer->PathCommandMoveTo(fX, fY);
		}
		inline HRESULT PathCommandLineTo(double fX, double fY)
		{
			return m_pRenderer->PathCommandLineTo(fX, fY);
		}
		inline HRESULT PathCommandLinesTo(SAFEARRAY* pPoints)
		{
			return m_pRenderer->PathCommandLinesTo(pPoints);
		}
		inline HRESULT PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
		{
			return m_pRenderer->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		}
		inline HRESULT PathCommandCurvesTo(SAFEARRAY* pPoints)
		{
			return m_pRenderer->PathCommandCurvesTo(pPoints);
		}
		inline HRESULT PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
		{
			return m_pRenderer->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
		}
		inline HRESULT PathCommandClose()
		{
			return m_pRenderer->PathCommandClose();
		}
		inline HRESULT PathCommandEnd()
		{
			return m_pRenderer->PathCommandEnd();
		}
		inline HRESULT DrawPath(long nType)
		{
			return m_pRenderer->DrawPath(nType);
		}
		inline HRESULT PathCommandStart()
		{
			return m_pRenderer->PathCommandStart();
		}
		inline HRESULT PathCommandGetCurrentPoint(double* fX, double* fY)
		{
			return m_pRenderer->PathCommandGetCurrentPoint(fX, fY);
		}
		inline HRESULT PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			return m_pRenderer->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		}
		inline HRESULT PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			return m_pRenderer->PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
		}

	//-------- Функции для вывода изображений ---------------------------------------------------
		inline HRESULT DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
		{
			return m_pRenderer->DrawImage(pInterface, fX, fY, fWidth, fHeight);
		}
		inline HRESULT DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
		{
			return m_pRenderer->DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);
		}

	// transform --------------------------------------------------------------------------------
		inline HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
		{
			return m_pRenderer->GetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
		}
		inline HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
		{
			return m_pRenderer->SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
		}

		inline HRESULT SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
		{
			return m_pRenderer->SetTransform(dA, dB, dC, dD, dE, dF);
		}
		inline HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
		{
			return m_pRenderer->GetTransform(pdA, pdB, pdC, pdD, pdE, pdF);
		}
		inline HRESULT ResetTransform(void)
		{
			return m_pRenderer->ResetTransform();
		}

	// -----------------------------------------------------------------------------------------
		inline HRESULT get_ClipMode(LONG* plMode)
		{
			return m_pRenderer->get_ClipMode(plMode);
		}
		inline HRESULT put_ClipMode(LONG lMode)
		{
			return m_pRenderer->put_ClipMode(lMode);
		}

	// additiaonal params ----------------------------------------------------------------------
		inline HRESULT SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
		{
			return m_pRenderer->SetAdditionalParam(ParamName, ParamValue);
		}
		inline HRESULT GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
		{
			return m_pRenderer->GetAdditionalParam(ParamName, ParamValue);
		}

	};
}