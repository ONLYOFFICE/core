#pragma once
#include "Text.h"
#include "Document.h"
#include "CalculatorCRC32.h"
#include "..\Graphics\Matrix.h"
#include "VectorGraphicsWriter2.h"

namespace NSHtmlRenderer
{
	class CPageWriter
	{
	public:
		LONG			m_lPageNumber;
		CStringWriter	m_oWriterPage;

		CText			m_oText;

		LONG			m_lPixWidth;
		LONG			m_lPixHeight;
		double			m_dDpiX;
		double			m_dDpiY;

		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;
		NSStructures::CFont*	m_pFont;

		BOOL					m_bIsClip;
		BOOL					m_bIsClipping;

		AVSGraphics::IAVSGraphicSimpleComverter* m_pSimpleConverter;
		CMatrix*				m_pFullTransform;

		LONG					m_lCurTxBrush;

		CVectorGraphicsWriter	m_oVectorWriter;

		CString					m_strDstDirectory;
		CString					m_strDstMedia;

		LONG					m_lIndexZ;

	public:
		CPageWriter() : m_lPageNumber(0), m_oVectorWriter()
		{
			m_oText.m_pWriter				= &m_oWriterPage;

			m_lPixWidth		= 0;
			m_lPixHeight	= 0;
			m_dDpiX			= 1;
			m_dDpiY			= 1;

			m_pPen			= NULL;
			m_pBrush		= NULL;
			m_pFont			= NULL;

			m_bIsClip		= FALSE;
			m_bIsClipping	= FALSE;

			m_pSimpleConverter = NULL;
			m_pFullTransform   = NULL;

			m_lCurTxBrush	= 0;
		}
		~CPageWriter()
		{
		}

	public:
		AVSINLINE void NewPage(double dDpiX, double dDpiY, LONG lPixW, LONG lPixH)
		{
			m_lPixWidth		= lPixW;
			m_lPixHeight	= lPixH;

			m_dDpiX			= dDpiX;
			m_dDpiY			= dDpiY;

			m_oText.NewPage(dDpiX, dDpiY);

			double w = (double)m_lPixWidth;
			double h = (double)m_lPixHeight;
			
			m_oVectorWriter.NewPage(w, h, m_lPageNumber);

			m_lCurTxBrush	= 0;
			m_lIndexZ		= 0;
		}

		AVSINLINE void SetSettings(NSStructures::CPen* pPen, NSStructures::CBrush* pBrush, NSStructures::CFont* pFont, CStyles* pStyles, NSHtmlRenderer::CMatrix* pTransform)
		{
			m_pPen		= pPen;
			m_pBrush	= pBrush;
			m_pFont		= pFont;

			m_oText.SetParams(pPen, pBrush, pFont, pStyles, pTransform);
		
			m_oVectorWriter.SetSettings(pPen, pBrush, m_pSimpleConverter);
		}
		AVSINLINE void ClosePage()
		{
			m_oVectorWriter.EndPage();
			m_oWriterPage.ClearNoAttack();

			m_oText.m_oCurrentLine.Clear();

			m_lCurTxBrush = 0;
		}

	public:
		void Write(CStringWriter* pWriterDoc, CStringWriter* pWriterScript, CStringWriter* pWriterThumbnails, CStringWriter* pTextMeasurer, double dHeightDoc, LONG lPageNumber, CDstInfo& oInfo)
		{
			CString strPage = _T("");
			strPage.Format(g_string_viewer_page, lPageNumber + 1, (LONG)0/*dHeightDoc*/, m_lPixWidth, (LONG)m_lPixHeight);
			pWriterDoc->WriteString(strPage);
			
			m_oText.ClosePage();

			if (m_oVectorWriter.IsGraphics())
			{
				m_oVectorWriter.WriteToDocument(pWriterDoc, oInfo);

				// чтобы работал клик
				CString strTransparentDiv = _T("<div style=\"position:absolute; top: 0px; width:100%; height:100%;\"></div>\n");
				pWriterDoc->WriteString(strTransparentDiv);
			}

			pWriterDoc->Write(m_oWriterPage);
			pWriterDoc->WriteString(g_bstr_viewer_end_div2);
		}

	public:
		AVSINLINE void WriteImage(double& x, double& y, double& width, double& height, CImageInfo& oInfo, double dAngle)
		{
			m_oVectorWriter.WriteImage(x, y, width, height, oInfo, dAngle);
		}

		AVSINLINE void WriteBeginPath()
		{
			m_oVectorWriter.WritePathStart();
		}

		AVSINLINE void WriteEndPath()
		{
			m_oVectorWriter.WriteEndPath();
		}		
		
		AVSINLINE void WritePathStart()
		{
			m_oVectorWriter.WritePathStart();
		}
		AVSINLINE void WritePathClose()
		{
			m_oVectorWriter.WritePathClose();
		}

		AVSINLINE void WritePathMoveTo(double& x, double& y)
		{
			m_oVectorWriter.WritePathMoveTo(x, y);
		}
		AVSINLINE void WritePathLineTo(double& x, double& y)
		{
			m_oVectorWriter.WritePathLineTo(x, y);
		}
		AVSINLINE void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			m_oVectorWriter.WritePathCurveTo(x1, y1, x2, y2, x3, y3);
		}
		AVSINLINE void WriteDrawPath(LONG lType, CImageInfo& oInfo, const double& dAngle)
		{
			m_oVectorWriter.WriteDrawPath(lType, oInfo, dAngle);
			m_oText.m_bIsNewLine = true;
		}
		AVSINLINE void WriteNewTableBox()
		{
			m_oText.m_bIsNewLine = true;
		}

		AVSINLINE void WritePathClip()
		{
			m_oVectorWriter.WritePathClip();
		}
		AVSINLINE void WritePathClipEnd()
		{
			m_oVectorWriter.WritePathClipEnd();
		}
		AVSINLINE void WritePathResetClip()
		{
			m_oVectorWriter.WritePathResetClip();
		}

		AVSINLINE void WriteText(BSTR bsText, BSTR bsGid, double x, double y, double width, double height, double baselineoffset)
		{
			//WriteGraphics();
			m_oText.CommandText(bsText, bsGid, x, y, width, height, baselineoffset, m_lIndexZ);
		}

		AVSINLINE void WriteGraphics()
		{
			if (m_oVectorWriter.IsGraphics())
			{
				//m_oVectorWriter.WriteToDocument(&m_oWriterPage, m_strDstMedia);
				++m_lIndexZ;
			}
		}
	};

	class CWriter
	{
	private:
		CStringWriter m_oWriter;
		CStringWriter m_oWriterThumbnails;
		CStringWriter m_oWriterScript;

		CStringWriter m_oWriterTextMeasurer;
		CStyles		  m_oWriterCSS;

	public:
		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;
		NSStructures::CFont*	m_pFont;

		double m_dDpiX;
		double m_dDpiY;

		BOOL					m_bPathClosed;
		CPageWriter				m_oPage;

		CAtlMap<CString, CImageInfo>	m_mapImagesFile;
		CAtlMap<DWORD, CImageInfo>		m_mapImageData;

		CCalculatorCRC32		m_oCRC;

		LONG m_lWidthDocPix;
		LONG m_lHeightDocPix;
		LONG m_lHeightPagePix;

		CDstInfo m_oDstInfo;

		AVSGraphics::IAVSGraphicSimpleComverter* m_pSimpleConverter;

	public:

		double					m_dWidthPix;
		double					m_dHeigthPix;

	private:

		LONG					m_lNextIDShape;
		LONG					m_lNextIDImage;

		LONG					m_lCurrentPage;
		LONG					m_lPagesCount;

		BOOL					m_bIsClip;
		BOOL					m_bIsClipping;

		double					m_dHeightDoc;

		LONG					m_lMaxSizeImage;

	public:
		CString					m_strDstDirectory;
		CString					m_strDstMedia;
		CString					m_strDstDirectoryFiles;
		CString					m_strFileName;

	public:
		bool					m_bIsThumbnails;
		bool					m_bIsMenu;

	private:
		// здесь данные для накопления слов (а лучше и строки??)

	public:
		CWriter()
		{
			m_dDpiX = 96.0;
			m_dDpiY = 96.0;

			m_dWidthPix		= 0;
			m_dHeigthPix	= 0;

			m_lNextIDImage	= 0;
			m_lNextIDShape	= 0;

			m_lCurrentPage	= -1;
			m_bIsClip		= FALSE;
			m_bIsClipping	= FALSE;

			m_bPathClosed	= TRUE;
			m_dHeightDoc	= 0;

			m_pSimpleConverter = NULL;

			m_lMaxSizeImage = 800;

			m_lWidthDocPix		= 0;
			m_lHeightDocPix		= 0;
			m_lHeightPagePix	= 0;

			m_bIsThumbnails		= false;
			m_bIsMenu			= false;

			m_lPagesCount		= 0;
		}

		AVSINLINE NSHtmlRenderer::CStyles* GetStyles()
		{
			return &m_oWriterCSS;
		}

		void CreateFile(CString& strDir, CString& strFileName)
		{
			m_lPagesCount		= 0;

			m_strDstDirectory		= strDir;
			m_strDstDirectoryFiles	= m_strDstDirectory + _T("\\") + strFileName + _T("_files");
			m_strFileName			= strFileName;

			CDirectory::CreateDirectory(m_strDstDirectoryFiles);
			m_strDstMedia = m_strDstDirectoryFiles + _T("\\media");
			CDirectory::CreateDirectory(m_strDstMedia);

			if (m_bIsThumbnails)
				CDirectory::CreateDirectory(m_strDstDirectoryFiles + _T("\\thumbnails"));

			m_oPage.m_strDstDirectory	= m_strDstDirectoryFiles;
			m_oPage.m_strDstMedia		= m_strDstMedia;

			m_oDstInfo.m_strDstFilePath = m_strDstDirectoryFiles;
			m_oDstInfo.m_strDstMedia = m_strDstMedia;
			m_oDstInfo.m_strDstFilePath.Replace(_T("\\"), _T("/"));
			m_oDstInfo.m_strAdditionalPath = _T("");
			
			LONG lStart = m_oDstInfo.m_strDstFilePath.ReverseFind(TCHAR('/'));
			if (-1 != lStart)
			{
				m_oDstInfo.m_strAdditionalPath = m_oDstInfo.m_strDstFilePath.Mid(0, lStart);
				lStart = m_oDstInfo.m_strAdditionalPath.ReverseFind(TCHAR('/'));
				if (-1 != lStart)
				{
					m_oDstInfo.m_strAdditionalPath = m_oDstInfo.m_strDstFilePath.Mid(lStart + 1);
				}
			}

			m_oWriter.Clear();
			m_oWriterScript.Clear();
			m_oWriterThumbnails.Clear();
			m_oWriterTextMeasurer.Clear();
		}

		void SetSimpleConverter(AVSGraphics::IAVSGraphicSimpleComverter* pSimpleConverter, CMatrix* pMatrix)
		{
			m_pSimpleConverter = pSimpleConverter;
			m_oPage.m_pSimpleConverter = m_pSimpleConverter;
			m_oPage.m_pFullTransform = pMatrix;

			m_oPage.m_oVectorWriter.m_pSimpleConverter  = m_pSimpleConverter;
			m_oPage.m_oVectorWriter.m_pFullTransform	= pMatrix;
		}
				
		AVSINLINE void WriteText(BSTR bsText, BSTR bsGid, double x, double y, double width, double height, double baselineoffset)
		{
			m_oPage.WriteText(bsText, bsGid, x, y, width, height, baselineoffset);
		}
		void WriteImage(IUnknown* punkImage, double x, double y, double width, double height, double dAngle)
		{
			//if (width < 0)
			//{
			//	FlipX(punkImage);
			//	width = -width;
			//}
			if (height < 0)
			{
				FlipY(punkImage);
				height = -height;
				y -= height;
			}
			
			CImageInfo oID = GenerateImageID(punkImage);
			m_oPage.WriteImage(x, y, width, height, oID, dAngle);
		}
		AVSINLINE void WriteImage(CString& strFile, double x, double y, double width, double height, double dAngle)
		{
			CImageInfo oID = GenerateImageID(strFile);
			m_oPage.WriteImage(x, y, width, height, oID, dAngle);
		}
		
		AVSINLINE void WriteBeginPath()
		{
			m_oPage.WriteBeginPath();
		}

		AVSINLINE void WriteEndPath()
		{
			m_oPage.WriteEndPath();
		}		
		
		AVSINLINE void WritePathStart()
		{
			m_oPage.WritePathStart();
		}
		AVSINLINE void WritePathClose()
		{
			m_oPage.WritePathClose();
		}

		AVSINLINE void WritePathMoveTo(double& x, double& y)
		{
			m_oPage.WritePathMoveTo(x, y);
		}
		AVSINLINE void WritePathLineTo(double& x, double& y)
		{
			m_oPage.WritePathLineTo(x, y);
		}
		AVSINLINE void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			m_oPage.WritePathCurveTo(x1, y1, x2, y2, x3, y3);
		}
		AVSINLINE void WriteDrawPath(LONG lType, const double& dAngle)
		{
			CImageInfo oInfo;
			if ((lType > 0xFF) && (c_BrushTypeTexture == m_pBrush->Type))
			{
				oInfo = GenerateImageID(m_pBrush->TexturePath);
			}

			m_oPage.WriteDrawPath(lType, oInfo, dAngle);
		}

		AVSINLINE void WritePathClip()
		{
			m_oPage.WritePathClip();
		}
		AVSINLINE void WritePathClipEnd()
		{
			m_oPage.WritePathClipEnd();
		}
		AVSINLINE void WritePathResetClip()
		{
			m_oPage.WritePathResetClip();
		}

		void NewPage(double& dWidthPix, double& dHeightPix)
		{
			if (0 != m_lPagesCount)
			{
				CString strNewPageWrite = _T("<div style=\"position:relative;margin:0;padding:0;top:0px;width:100%;height:20px\"></div>");
			m_oWriter.WriteString(strNewPageWrite);
			}

			++m_lPagesCount;

			m_dWidthPix		= dWidthPix;
			m_dHeigthPix	= dHeightPix;

			if (m_lWidthDocPix < (LONG)dWidthPix)
				m_lWidthDocPix = (LONG)dWidthPix;
			if (m_lHeightPagePix < (LONG)dHeightPix)
				m_lHeightPagePix = (LONG)dHeightPix;

			m_lHeightDocPix += (LONG)dHeightPix;

			++m_lCurrentPage;

			m_oPage.m_lPageNumber = m_lCurrentPage;
			m_oPage.NewPage(m_dDpiX, m_dDpiY, (LONG)m_dWidthPix, (LONG)m_dHeigthPix);
		}
		void EndPage(bool bIsWEB)
		{
			if (m_bIsThumbnails)
			{
				// thumbnails
				CString strPageThumbnail = _T("");
				strPageThumbnail.Format(_T("<div class=\"blockpage\">\n\
												<div class=\"blockthumbnail\" align=\"center\">\n\
													<img align=\"center\" src=\"thumbnails/page%d.png\" onClick=\"OnChangePage(%d)\" width=\"%s\" height=\"%s\"/>\n\
													page%d\n\
												</div>\n\
											</div>\n"), m_lCurrentPage + 1, m_lCurrentPage + 1, _T("100%"), _T("90%"), m_lCurrentPage + 1);

				m_oWriterThumbnails.WriteString(strPageThumbnail);
			}

			// page
			//m_oWriter.WriteString(g_bstr_viewer_end_div);
			m_oPage.Write(&m_oWriter, &m_oWriterScript, &m_oWriterThumbnails, &m_oWriterTextMeasurer, /*m_dHeightDoc*/20 * m_lCurrentPage, m_lCurrentPage, m_oDstInfo);
			m_oPage.ClosePage();

			if (bIsWEB)
			{
				CString str = _T("");
				str.Format(_T("<script type=\"text/javascript\">this.__pagesLoaded = %d;</script>"), m_lCurrentPage + 1);
				m_oWriter.WriteString(str);
			}

			m_dHeightDoc += m_oPage.m_lPixHeight;
			m_dHeightDoc += 20; // const
		}

		void WriteStartDocument()
		{
			//// 1) главный документ
			//CString strHtml = 
			//	_T("<html>\n\
			//		  <head>\n\
			//			 <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\n\
			//			 <title>document viewer</title>\n\
			//		 </head>\n\
			//		 <frameset rows=\"50,*\" framespacing=\"0\" frameborder=\"0\">\n\
			//			 <frame src=\"menu.html\" name=\"menu\" noresize border=\"1\" bordercolor=\"#F0F0F0\" scrolling=\"no\"></frame>\n\
			//			 <frameset cols=\"*,200\">\n\
			//				 <frame id=\"id_viewer\" src=\"viewer.html\" name=\"viewer\" noresize></frame>\n\
			//				 <frame id=\"id_thumbnails\" src=\"thumbnails.html\" name=\"thumbnail\"></frame>\n\
			//			 </frameset>\n\
			//		 </frameset>\n\
			//	 </html>");
			//
			//CDirectory::SaveToFile(m_strDstDirectory + _T("\\document.html"), strHtml);

			//// 2) menu
			//CString strMenu = 
			//	_T("<html>\n\
			//		 <head>\n\
			//			 <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\n\
			//			 <title>menu</title>\n\
			//		 </head>\n\
			//			<body bgcolor=\"#5F5F5F\">\n\
			//			</body>\n\
			//	</html>");
			//
			//CDirectory::SaveToFile(m_strDstDirectory + _T("\\menu.html"), strMenu);

			//// 3) viewer
			//CString strViewer	= 
			//	_T("<!--[if IE]><!DOCTYPE html><![endif]-->\
			//	   <html xmlns:v=\"urn:schemas-microsoft-com:vml\">\n\
			//		 <head>\n\
			//			 <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\n\
			//			 <title>viewer</title>\n\
			//			 <link rel=\"stylesheet\" href=\"docstyles.css\" type=\"text/css\"/>\n\
			//			 <script language=\"JavaScript\">\n\
			//				 function OnChangePage(pageNum)\n\
			//				 {\nvar _div_name = \"page\" + pageNum;\nvar _div = document.getElementById(_div_name);\nif (_div)\n{\nscroll(0, _div.offsetTop);\n}\n}\n\
			//			 </script>\n\
			//			 <script type=\"text/javascript\" src=\"script.js\"></script>\n\
			//		 </head>\n\
			//		 <body style=\"margin: 0px;\" bgcolor=\"#CBCFD4\">\n");

			//m_oWriter.WriteString(strViewer);

			//// 4) thumbnails
			//CString strThumbnails = _T("<html>\
			//							<head>\n\
			//								<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\n\
			//								<title>thumbnails</title>\n\
			//								<style type=\"text/css\">\n\
			//									.blockpage {\n\
			//									width: 80%;\n\
			//									height: 200px;\n\
			//									background: #FEFEFE;\n\
			//									padding: 10px;\n\
			//									float: none;\n\
			//									text-align: center;\n\
			//									}\n\
			//									.blockthumbnail {\n\
			//									width: 100%;\n\
			//									height: 100%;\n\
			//									background: #FEFEFE;\n\
			//									padding: 0px;\n\
			//									float: none;\n\
			//									}\n\
			//								</style>\n\
			//								<script language=\"JavaScript\">\n\
			//									function OnChangePage(pageNum)\n\
			//									{\n\
			//										top.frames['viewer'].OnChangePage(pageNum);\n\
			//									}\n\
			//								</script>\n\
			//							</head>\n\
			//							<body bgcolor=\"#FEFEFE\">");

			//m_oWriterThumbnails.WriteString(strThumbnails);

			m_dHeightDoc = 0;

			m_oPage.m_oText.NewDocument();

			//m_oWriterScript.WriteString(g_bstr_basicscript);

			m_oWriterCSS.NewDocument();
			
			//m_oWriterTextMeasurer.WriteString(g_bstr_lineMeasure);			
		}
		void WriteEndDocument(CDocument& oDocument)
		{
			// скидывание canvas  - у нас SVG
			// CDirectory::SaveToFile(m_strDstDirectory + _T("\\script.js"), m_oWriterScript.GetData());

			// docstyles
			CDirectory::SaveToFile(m_strDstDirectoryFiles + _T("\\docstyles.css"), m_oWriterCSS.m_oWriterCSS.GetCString());
			m_oWriterCSS.WriteStylesForDiffBrowsers(m_strDstDirectoryFiles);

			// viewer.html
			CFile oFileViewer;
			oFileViewer.CreateFile(m_strDstDirectoryFiles + _T("\\viewer.html"));

			CString strViewer = _T("<!--[if IE]><!DOCTYPE html><![endif]-->\n\
<html xmlns:v=\"urn:schemas-microsoft-com:vml\">\n\
<head>\n\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\n\
<title>viewer</title>\n\
<link rel=\"stylesheet\" href=\"docstyles.css\" type=\"text/css\"/>\n\
<script language=\"JavaScript\">\n");

			CString strInfoDoc = _T("");
			strInfoDoc.Format(_T("var dZoomKoef = 1.0;var lWidthDoc = %d; var lHeightPage = %d; var lHeightDoc = %d;var bIsFitToPage = 0;var nPagesCount = %d;\n"), 
				m_lWidthDocPix, m_lHeightPagePix, m_lHeightDocPix, m_lPagesCount);

			strViewer += strInfoDoc;

			CString str2 = _T("var bIsIE 	= /*@cc_on ! @*/ false;\
var bIsOpera 	= (window.opera != undefined);\
if (bIsIE)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"css_ie.css\\\"/>\");\
}\
else if (bIsOpera)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"css_opera.css\\\"/>\");\
}\
else\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"css_other.css\\\"/>\");\
}\n\
</script>\n");
			strViewer += str2;

			oFileViewer.WriteStringUTF8(strViewer);			

			CString strMainDiv = _T("");
			strMainDiv.Format(_T("</head>\n\
<body style=\"padding: 0; margin: 0px;\" bgcolor=\"#eef0f2\">\n\
<div style=\"position:relative;margin:0px;padding:0px;height:16px;background-color:#eef0f2;\"></div>\n<div id=\"maindiv\" style=\"position: relative; width: %dpx; padding: 0; margin: 0 auto;\">\n"), m_lWidthDocPix);

			oFileViewer.WriteStringUTF8(strMainDiv);
			oFileViewer.WriteStringUTF8(m_oWriter.GetCString());

			CString strEndBody = _T("<div style=\"position:relative;margin:0px;padding:0px;height:16px;\"></div></div></body></html>");
			oFileViewer.WriteStringUTF8(strEndBody);
			
			oFileViewer.CloseFile();

			m_oPage.ClosePage();
			m_mapImageData.RemoveAll();
			m_mapImagesFile.RemoveAll();

			m_oWriterCSS.CloseDocument();

			// document.html
			if (m_bIsThumbnails)
			{
				CString strHtml = 
				_T("<html>\n\
<head>\n\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\n\
<title>document viewer</title>\n\
</head>\n\
<frameset cols=\"*,200\" framespacing=\"0\" frameborder=\"0\">\n\
<frame id=\"id_viewer\" src=\"") + m_strFileName + _T("_files/viewer.html\" name=\"viewer\" noresize></frame>\n\
 <frame id=\"id_thumbnails\" src=\"") + m_strFileName + _T("_files/thumbnails.html\" name=\"thumbnail\"></frame>\n\
</frameset>\n\
</frameset>\n\
</html>");
			
				CDirectory::SaveToFile(m_strDstDirectory + _T("\\") + m_strFileName + _T(".html"), strHtml);
			}
			else
			{
				CString strHtml = 
				_T("<html>\n\
<head>\n\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\n\
<title>document viewer</title>\n\
</head>\n\
<frameset framespacing=\"0\" frameborder=\"0\">\n\
 <frame id=\"id_viewer\" src=\"") + m_strFileName + _T("_files/viewer.html\" name=\"viewer\" noresize></frame>\n\
</frameset>\n\
</frameset>\n\
</html>");
			
				CDirectory::SaveToFile(m_strDstDirectory + _T("\\") + m_strFileName + _T(".html"), strHtml);
			}
		}

		void WriteEndDocument2(CDocument& oDocument)
		{
			// docstyles
			CDirectory::SaveToFile(m_strDstDirectoryFiles + _T("\\docstyles.css"), m_oWriterCSS.m_oWriterCSS.GetCString());
			m_oWriterCSS.WriteStylesForDiffBrowsers(m_strDstDirectoryFiles);

			// viewer.html
			CFile oFileViewer;
			oFileViewer.CreateFile(m_strDstDirectoryFiles + _T("\\viewer.html"));

			CString strViewer1 = _T("<!--[if IE]><!DOCTYPE html><![endif]-->\n\
<html xmlns:v=\"urn:schemas-microsoft-com:vml\">\n\
<head>\n\
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\n\
<title>viewer</title>\n\
<meta http-equiv=\"X-UA-Compatible\" content=\"IE=8;IE=9;chrome=1;\"/>\n\
<link rel=\"stylesheet\" href=\"") + m_oDstInfo.m_strAdditionalPath + _T("/docstyles.css\" type=\"text/css\"/>\n\
<link rel=\"stylesheet\" href=\"styles/docviewer.css\" type=\"text/css\"/>\
<script type=\"text/javascript\" src=\"jquery/jquery-1.6.1.min.js\"></script>\
<script type=\"text/javascript\" src=\"jquery/jquery.mousewheel.js\"></script>\
<script language=\"JavaScript\">\n");
			oFileViewer.WriteStringUTF8(strViewer1);

			CString strInfoDoc = _T("");
			strInfoDoc.Format(_T("var dZoomKoef = 1.0;var lWidthDoc = %d; var lHeightPage = %d; var lHeightDoc = %d;var bIsFitToPage = 0;var nPagesCount = %d;\n"), 
				m_lWidthDocPix, m_lHeightPagePix, m_lHeightDocPix, m_lPagesCount);

			CString str1 = _T("var arrayOffsets = [0");
			CStringWriter oWriterPagesInfo;
			oWriterPagesInfo.WriteString(str1);

			size_t offsetOld = 0;
			for (LONG i = 1; i < m_lPagesCount; ++i)
			{
				offsetOld += (size_t)(96 * oDocument.m_arrPages[i - 1].GetHeight() / 25.4);

				CString strPage = _T("");
				strPage.Format(_T(",%u"), offsetOld + 36);
				oWriterPagesInfo.WriteString(strPage);
			}

			if (m_oDstInfo.m_bIsWeb)
			{
				CString str2 = _T("];\n\
var bIsIE 	= /*@cc_on ! @*/ false;\
var bIsOpera 	= (window.opera != undefined);\
if (bIsIE)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"styles/css_ie.css\\\"/>\");\
}\
else if (bIsOpera)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"styles/css_opera.css\\\"/>\");\
}\
else\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"styles/css_other.css\\\"/>\");\
}</script>\n");
				oWriterPagesInfo.WriteString(str2);
			}
			else
			{
				CString str2 = _T("];\n\
var bIsIE 	= /*@cc_on ! @*/ false;\
var bIsOpera 	= (window.opera != undefined);\
if (bIsIE)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"css_ie.css\\\"/>\");\
}\
else if (bIsOpera)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"css_opera.css\\\"/>\");\
}\
else\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"css_other.css\\\"/>\");\
}</script>\n");
				oWriterPagesInfo.WriteString(str2);
			}
			
			strInfoDoc += oWriterPagesInfo.GetCString();
			oFileViewer.WriteStringUTF8(strInfoDoc);			

			CString strMainDiv = _T("");
			strMainDiv.Format(_T("<script type=\"text/javascript\" src=\"js/dochtmlviewer.js\"></script></head>\n\
<body class=\"viewerbody\">\n\
<div style=\"position:relative;margin:0px;padding:0px;height:16px;background-color:#eef0f2;\"></div>\n<div id=\"maindiv\" style=\"position: relative; width: %dpx; padding: 0; margin: 0 auto;\">\n"), m_lWidthDocPix);

			oFileViewer.WriteStringUTF8(strMainDiv);
			oFileViewer.WriteStringUTF8(m_oWriter.GetCString());

			CString strEndBody = _T("<div style=\"position:relative;margin:0;padding:0;top:0px;width:100%;height: 16px\"></div></div><script type=\"text/javascript\">SetPageCountToMenu();stopTimer();</script></body></html>");
			oFileViewer.WriteStringUTF8(strEndBody);
			
			oFileViewer.CloseFile();
		}

		void WriteEndDocument3(CDocument& oDocument)
		{
			// docstyles
			CDirectory::SaveToFile(m_strDstDirectoryFiles + _T("\\docstyles.css"), m_oWriterCSS.m_oWriterCSS.GetCString());
			m_oWriterCSS.WriteStylesForDiffBrowsers(m_strDstDirectoryFiles);

			// viewer.html
			CFile oFileViewer;
			oFileViewer.CreateFile(m_strDstDirectoryFiles + _T("\\viewer.html"));

			CString strViewer1 = _T("<!DOCTYPE html><html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"/>\
<title></title><meta http-equiv=\"X-UA-Compatible\" content=\"IE=8;IE=9;chrome=1;\"/>\
<link rel=\"stylesheet\" href=\"docstyles.css\" type=\"text/css\"/><script language=\"JavaScript\">");
			oFileViewer.WriteStringUTF8(strViewer1);

			CString strInfoDoc = _T("");
			strInfoDoc.Format(_T("var dZoomKoef = 1.0;var lWidthDoc = %d; var lHeightPage = %d; var lHeightDoc = %d;var bIsFitToPage = 0;var nPagesCount = %d;\n"), 
				m_lWidthDocPix, m_lHeightPagePix, m_lHeightDocPix, m_lPagesCount);

			CString str1 = _T("var arrayOffsets = [0");
			CStringWriter oWriterPagesInfo;
			oWriterPagesInfo.WriteString(str1);

			size_t offsetOld = 0;
			for (LONG i = 1; i < m_lPagesCount; ++i)
			{
				offsetOld += (size_t)(96 * oDocument.m_arrPages[i - 1].GetHeight() / 25.4);

				CString strPage = _T("");
				strPage.Format(_T(",%u"), offsetOld + 36);
				oWriterPagesInfo.WriteString(strPage);
			}

			if (m_oDstInfo.m_bIsWeb)
			{
				CString str2 = _T("];\n\
var bIsIE 	= /*@cc_on ! @*/ false;\
var bIsOpera 	= (window.opera != undefined);\
if (bIsIE)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"styles/css_ie.css\\\"/>\");\
}\
else if (bIsOpera)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"styles/css_opera.css\\\"/>\");\
}\
else\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"styles/css_other.css\\\"/>\");\
}</script>\n");
				oWriterPagesInfo.WriteString(str2);
			}
			else
			{
				CString str2 = _T("];\n\
var bIsIE 	= /*@cc_on ! @*/ false;\
var bIsOpera 	= (window.opera != undefined);\
if (bIsIE)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"css_ie.css\\\"/>\");\
}\
else if (bIsOpera)\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"css_opera.css\\\"/>\");\
}\
else\
{\
   document.write(\"<link rel=\\\"stylesheet\\\" type=\\\"text/css\\\" href=\\\"css_other.css\\\"/>\");\
}</script>\n");
				oWriterPagesInfo.WriteString(str2);
			}
			
			strInfoDoc += oWriterPagesInfo.GetCString();
			oFileViewer.WriteStringUTF8(strInfoDoc);

			CString strHead___ = _T("<script type=\"text/javascript\" src=\"common/dochtmlviewer.js\"></script>\
<link rel=\"stylesheet\" type=\"text/css\" href=\"common/MenuStyle.css\"/>\
<link rel=\"stylesheet\" type=\"text/css\" href=\"common/docviewer.css\"/>\
<link rel=\"stylesheet\" type=\"text/css\" href=\"common/clickmenu.css\"/>\
<link rel=\"stylesheet\" type=\"text/css\" href=\"common/jquery-ui.css\"/>\
<link rel=\"stylesheet\" type=\"text/css\" href=\"common/jquery-custom.css\"/>\
<!--[if IE 8]>\
<link rel=\"stylesheet\" type=\"text/css\" href=\"common/IE.css\"/>\
<![endif]-->\
<!--[if IE 9]>\
<link rel=\"stylesheet\" type=\"text/css\" href=\"common/IE9.css\"/>\
<![endif]-->\
<script type=\"text/javascript\" src=\"common/jquery-1.6.1.min.js\"></script>\
<script type=\"text/javascript\" src=\"common/jquery-ui.js\"></script>\
<script type=\"text/javascript\" src=\"common/jquery.clickmenu.js\"></script>\
<script type=\"text/javascript\" src=\"common/jquery.mousewheel.js\"></script>\
<script type=\"text/javascript\" src=\"common/docviewer.js\"></script>\
<script type=\"text/javascript\" src=\"common/common.js\"></script>\
<script type=\"text/javascript\">\n\
var isMobileAgent = /android|avantgo|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od|ad)|iris|kindle|lge |maemo|midp|mmp|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\\/|plucker|pocket|psp|symbian|treo|up\\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(navigator.userAgent || navigator.vendor || window.opera);\n\
$(function(){\n\
if (isMobileAgent)\n\
$(\"#menu\").hide();\n\
window.onresize = OnResizeBrowser;\n\
$(window).resize();\n\
var windowWidth = $(window).width();\n\
var windowHeight = $(window).height();\n\
startTimer();\n\
});\n\
function OnResizeBrowser()\n\
{\n\
var elemMenu = document.getElementById(\"menu\");\n\
var heightMenu = elemMenu.offsetHeight;\n\
var elemDiv = document.getElementById(\"content\");\n\
\n\
var allheight = 0;\n\
if (this.innerHeight)\n\
	allheight = this.innerHeight;\n\
else if (document.documentElement && document.documentElement.clientHeight)\n\
	allheight = document.documentElement.clientHeight;\n\
else if (document.body)\n\
	allheight = document.body.clientHeight;\n\
\n\
var __height = (allheight - heightMenu) + \"px\";\n\
elemDiv.style.height = __height;\n\
\n\
CheckFitToPage();\n\
}\n\
</script>\
</head>\
<body style=\"padding: 0; margin: 0 auto;overflow:hidden;\">\n\
<table id=\"menu\" class=\"utilFont\" style=\"width:100%\"><tr><td id=\"toolbar\"><table style=\"\"><tr>\n\
<td class=\"ToolbarIconOut selectableIcon\" title=\"ZoomOut\" onclick=\"funZoomOut()\"><div id=\"zoomOut\" class=\"iconToolbar\"><img class=\"ToolbarIcon ToolbarZoomOut\" src=\"common/img/spacer.gif\"/></div></td>\n\
<td class=\"ToolbarIconOut selectableIcon\" title=\"ZoomIn\" onclick=\"funZoomIn()\"><div id=\"zoomIn\" class=\"iconToolbar\"><img class=\"ToolbarIcon ToolbarZoomIn\" src=\"common/img/spacer.gif\"/></div></td>\n\
<td class=\"ToolbarIconOut selectableIcon\" title=\"ActualSize\" onclick=\"funZoom(0,null)\"><div id=\"actualSize\" class=\"iconToolbar\"><img class=\"ToolbarIcon ToolbarActualSize\" src=\"common/img/spacer.gif\"/></div></td>\n\
<td id=\"td_fitToPage\" class=\"ToolbarIconOut selectableIcon iconPressed2\" title=\"FitToPage\" onclick=\"funZoom(1,this)\"><div id=\"fitToPage\" class=\"iconToolbar\"><img class=\"ToolbarIcon ToolbarFitToPage\" src=\"common/img/spacer.gif\"/></div></td>\n\
<td id=\"td_fitToWidth\" class=\"ToolbarIconOut selectableIcon iconPressed2\" title=\"FitToWidth\" onclick=\"funZoom(2,this)\"><div id=\"fitToWidth\" class=\"iconToolbar\"><img class=\"ToolbarIcon ToolbarFitToWidth\" src=\"common/img/spacer.gif\"/></div></td>\n\
<td class=\"ToolbarIconOut\" style=\"min-width: 20px;\"><div><ul id=\"zoomMenu\"><li id=\"zoom\" class=\"item main textSelect textSelectBoard\"><span id=\"zoomValuePercent\">100%</span><ul>\n\
<li id=\"8\" class=\"SubItem\"  value=\"8\">8%</li><li id=\"12\" class=\"SubItem\" value=\"12\">12%</li><li id=\"25\" class=\"SubItem\" value=\"25\">25%</li>\
<li id=\"33\" class=\"SubItem\" value=\"33\">33%</li><li id=\"50\" class=\"SubItem\" value=\"50\">50%</li><li id=\"66\" class=\"SubItem\" value=\"66\">66%</li>\
<li id=\"75\" class=\"SubItem\" value=\"75\">75%</li><li id=\"100\" class=\"SubItem\" value=\"100\">100%</li><li id=\"125\" class=\"SubItem\" value=\"125\">125%</li>\
<li id=\"150\" class=\"SubItem\" value=\"150\">150%</li><li id=\"200\" class=\"SubItem\" value=\"200\">200%</li><li id=\"300\" class=\"SubItem\" value=\"300\">300%</li>\
<li id=\"400\" class=\"SubItem\" value=\"400\">400%</li><li id=\"500\" class=\"SubItem\" value=\"500\">500%</li></ul><div class=\"ToolbarDropDown dropdown\"></div></li></ul></div></td>\n\
<td class=\"toolbarSep\"><div class=\"ToolbarIconOut\"><img src=\"common/img/Border.png\"/></div></td>\n\
<td class=\"ToolbarIconOut selectableIcon iconToolbar\" title=\"PrevPage\" onclick=\"OnPagePrev()\"><div id=\"td_prevPage\" class=\"iconToolbar\"><img class=\"ToolbarIcon ToolbarPrevPage\" src=\"common/img/spacer.gif\"/></div></td>\n\
<td class=\"ToolbarIconOut selectableIcon iconToolbar\" title=\"NextPage\" onclick=\"OnPageNext()\"><div id=\"td_nextPage\" class=\"iconToolbar\"><img class=\"ToolbarIcon ToolbarNextPage\" src=\"common/img/spacer.gif\"/></div></td>\n\
<td class=\"\" id=\"ie8textarea\"><input class=\"txblock\" style=\"width: 28px;text-align: center;\" type=\"text\" id=\"pageNum\" value=\"1\" onkeypress=\"return onlyNumber(this, event)\"/></td>\n\
<td class=\"ToolbarIconOut\" style=\"border: 0 none;\"><p id=\"pageCounts\" class=\"txblock\" style=\"margin: 0 6px 0 0; padding: 0;\">/ 1</p></td>\n\
<td class=\"toolbarSep\" style=\"display:none;\"><div class=\"ToolbarIconOut\"><img src=\"common/img/Border.png\"/></div></td>\n\
<td id=\"downloadButton\" class=\"ToolbarIconOut selectableIcon iconToolbar\" style=\"display:none;\" title=\"LoadDocument\"><div id=\"td_load\" class=\"iconToolbar\"><img class=\"ToolbarIcon ToolbarLoadDoc\" src=\"common/img/spacer.gif\"/></div></td>\n\
<td class=\"ToolbarIconOut selectableIcon iconToolbar\" title=\"Print\" style=\"display:none;\"><div id=\"td_print\" class=\"iconToolbar\" style=\"margin-left: 1px;\"><img class=\"ToolbarIcon ToolbarPrint\" src=\"common/img/spacer.gif\"/></div></td>\n\
<td class=\"toolbarSep\" style=\"padding-left: 3px;\"><div class=\"ToolbarIconOut\" style=\"margin-left: -1px;\"><img src=\"common/img/Border.png\"/></div></td>\n\
</tr></table></td></tr><tr><td id=\"topmenuSep\" colspan=\"2\" class=\"separatingLine\"></td></tr></table>\n\
<table id=\"statusbar\" class=\"utilFont layerTop\"><tr><td id=\"netstatus\" style=\"width:177px; text-align: center; color:#FFF;\"></td></tr></table>\n\
<div id=\"content\" onclick=\"closeMenu();\"><div id=\"id_viewer\" class=\"viewerbody\" name=\"viewer\" width=\"100%\" height=\"100%\">\n\
<div style=\"position:relative;margin:0px;padding:0px;height:16px;background-color:#eef0f2;\"></div>");

			oFileViewer.WriteStringUTF8(strHead___);

			CString strMainDiv = _T("");
			strMainDiv.Format(_T("<div id=\"maindiv\" style=\"position: relative; width: %dpx; padding: 0; margin: 0 auto;\">\n"), m_lWidthDocPix);

			oFileViewer.WriteStringUTF8(strMainDiv);
			oFileViewer.WriteStringUTF8(m_oWriter.GetCString());

			CString strEndBody = _T("<div style=\"position:relative;margin:0;padding:0;top:0px;width:100%;height: 16px\"></div></div><script type=\"text/javascript\">SetPageCountToMenu();stopTimer();</script></div>\
</div><div id=\"vertScrollContainer\"><div class=\"spV\" style=\"width:18px\"><div id=\"scrollbarV\">\
<div id=\"ssV\"><img src=\"common/img/spacer.gif\" height=\"1\" width=\"1\"/></div></div></div></div>\
<div id=\"horzScrollContainer\"><div class=\"scH\"><div id=\"scrollbarH\"><div id=\"ssH\"><img src=\"common/img/spacer.gif\" height=\"1\" width=\"1\"/></div></div></div></div>\
<div id=\"blockUI\"></div></body></html>");
			oFileViewer.WriteStringUTF8(strEndBody);
			
			oFileViewer.CloseFile();

			OfficeUtils::IOfficeUtilsPtr ptrUtils;
			ptrUtils.CreateInstance(__uuidof(OfficeUtils::COfficeUtils));
			
			if (NULL != ptrUtils)
			{
				HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();

				CString strCommonFiles = m_strDstDirectoryFiles + _T("\\common.zip");
				LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_COMMON_ZIP), _T("HTML2"), strCommonFiles);

				CString strCommonDst = m_strDstDirectoryFiles + _T("\\common");
				BSTR input	= strCommonFiles.AllocSysString();
				BSTR output = strCommonDst.AllocSysString();

				::CreateDirectoryW(output, NULL);
				HRESULT hr = ptrUtils->ExtractToDirectory(input, output, NULL, 0);
				ptrUtils.Release();

				SysFreeString(input);
				SysFreeString(output);

				::DeleteFileW(input);
			}

			// обертка
			CString strHtml = _T("<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></meta>\n<meta http-equiv=\"X-UA-Compatible\" content=\"IE=8;IE=9;chrome=1;\"/>\n<title>document viewer</title>\n\
</head>\n<frameset framespacing=\"0\" frameborder=\"0\">\n<frame id=\"id_viewer\" src=\"") + m_strFileName + 
_T("_files/viewer.html\" name=\"viewer\" noresize></frame>\n</frameset>\n</frameset>\n</html>");
			
			CDirectory::SaveToFile(m_strDstDirectory + _T("\\") + m_strFileName + _T(".html"), strHtml);
		}

protected:
		AVSINLINE void CopyFile(CString& strFileSrc, CString& strFileDst)
		{
			CDirectory::CopyFile(strFileSrc, strFileDst, NULL, NULL);
		}
		void SaveImage(CString& strFileSrc, CImageInfo& oInfo)
		{
			CString strLoadXml = _T("<transforms><ImageFile-LoadImage sourcepath=\"") + strFileSrc + _T("\"/></transforms>");

			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT_BOOL vbRes = VARIANT_FALSE;
			BSTR bsLoad = strLoadXml.AllocSysString();
			pTransform->SetXml(bsLoad, &vbRes);
			SysFreeString(bsLoad);

			pTransform->Transform(&vbRes);

			VARIANT var;
			var.punkVal = NULL;
			pTransform->GetResult(0, &var);

			if (NULL == var.punkVal)
			{
				RELEASEINTERFACE(pTransform);
				return;
			}

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			var.punkVal->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			RELEASEINTERFACE((var.punkVal));

			if (NULL == pFrame)
			{
				RELEASEINTERFACE(pTransform);
				return;
			}

			LONG lWidth		= 0;
			LONG lHeight	= 0;
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);

			oInfo.m_eType = GetImageType(pFrame);

			RELEASEINTERFACE(pFrame);

			CString strSaveItem = _T("");
			strSaveItem.Format(_T("\\image%d."), oInfo.m_lID);
			if (itJPG == oInfo.m_eType)
			{
				strSaveItem = _T("<ImageFile-SaveAsJpeg destinationpath=\"") + m_strDstMedia + strSaveItem + _T("jpg\" format=\"888\"/>");
			}
			else
			{
				strSaveItem = _T("<ImageFile-SaveAsPng destinationpath=\"") + m_strDstMedia + strSaveItem + _T("png\" format=\"888\"/>");
			}

			CString strXml = _T("");

			LONG lMaxSizeImage = m_lMaxSizeImage;
			if (oInfo.m_lID < 3)
				lMaxSizeImage = 1200;

			if ((lWidth <= lMaxSizeImage) && (lHeight <= lMaxSizeImage))
			{
				strXml = _T("<transforms>") + strSaveItem + _T("</transforms>");
			}
			else
			{
				LONG lW = 0;
				LONG lH = 0;
				double dAspect = (double)lWidth / lHeight;

				if (lWidth >= lHeight)
				{
					lW = lMaxSizeImage;
					lH = (LONG)((double)lW / dAspect);
				}
				else
				{
					lH = lMaxSizeImage;
					lW = (LONG)(dAspect * lH);
				}

				CString strResize = _T("");
				strResize.Format(_T("<ImageTransform-TransformResize type=\"65536\" width=\"%d\" height=\"%d\"/>"), lW, lH);

				strXml = _T("<transforms>") + strResize + strSaveItem + _T("</transforms>");
			}
			
			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransform->SetXml(bsXml, &vbSuccess);
			SysFreeString(bsXml);

			pTransform->Transform(&vbSuccess);

			RELEASEINTERFACE(pTransform);
		}
		void SaveImage(IUnknown* punkImage, CImageInfo& oInfo)
		{
			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			if (NULL == pFrame)
				return;

			LONG lWidth		= 0;
			LONG lHeight	= 0;
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);

			oInfo.m_eType = GetImageType(pFrame);

			RELEASEINTERFACE(pFrame);
			
			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT var;
			var.vt = VT_UNKNOWN;
			var.punkVal = punkImage;
			pTransform->SetSource(0, var);

			CString strSaveItem = _T("");
			strSaveItem.Format(_T("\\image%d."), oInfo.m_lID);
			if (itJPG == oInfo.m_eType)
			{
				strSaveItem = _T("<ImageFile-SaveAsJpeg destinationpath=\"") + m_strDstMedia + strSaveItem + _T("jpg\" format=\"888\"/>");
			}
			else
			{
				strSaveItem = _T("<ImageFile-SaveAsPng destinationpath=\"") + m_strDstMedia + strSaveItem + _T("png\" format=\"888\"/>");
			}

			LONG lMaxSizeImage = m_lMaxSizeImage;
			if (oInfo.m_lID < 3)
				lMaxSizeImage = 1200;

			CString strXml = _T("");
			if ((lWidth <= lMaxSizeImage) && (lHeight <= lMaxSizeImage))
			{
				strXml = _T("<transforms>") + strSaveItem + _T("</transforms>");
			}
			else
			{
				LONG lW = 0;
				LONG lH = 0;
				double dAspect = (double)lWidth / lHeight;

				if (lWidth >= lHeight)
				{
					lW = lMaxSizeImage;
					lH = (LONG)((double)lW / dAspect);
				}
				else
				{
					lH = lMaxSizeImage;
					lW = (LONG)(dAspect * lH);
				}

				CString strResize = _T("");
				strResize.Format(_T("<ImageTransform-TransformResize type=\"65536\" width=\"%d\" height=\"%d\"/>"), lW, lH);

				strXml = _T("<transforms>") + strResize + strSaveItem + _T("</transforms>");
			}
			
			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransform->SetXml(bsXml, &vbSuccess);
			SysFreeString(bsXml);

			pTransform->Transform(&vbSuccess);

			RELEASEINTERFACE(pTransform);
		}

		CImageInfo GenerateImageID(IUnknown* punkData)
		{
			CImageInfo oInfo;

			if (NULL == punkData)
				return oInfo;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkData->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			BYTE* pBuffer = NULL;
			LONG lLen = 0;

			pFrame->get_Buffer(&pBuffer);
			pFrame->get_BufferSize(&lLen);

			DWORD dwSum = m_oCRC.Calc(pBuffer, lLen);

			CAtlMap<DWORD, CImageInfo>::CPair* pPair = m_mapImageData.Lookup(dwSum);
			if (NULL == pPair)
			{
				// нужно добавить
				++m_lNextIDImage;
				
				oInfo.m_lID = m_lNextIDImage;
				SaveImage(punkData, oInfo);
				m_mapImageData.SetAt(dwSum, oInfo);
			}
			else
			{
				oInfo = pPair->m_value;
			}

			RELEASEINTERFACE(pFrame);

			return oInfo;
		}

		CImageInfo GenerateImageID(CString& strFileName)
		{
			CImageInfo oInfo;
			CAtlMap<CString, CImageInfo>::CPair* pPair = m_mapImagesFile.Lookup(strFileName);

			if (NULL == pPair)
			{
				// нужно добавить
				++m_lNextIDImage;
				
				oInfo.m_lID = m_lNextIDImage;
				SaveImage(strFileName, oInfo);
				m_mapImagesFile.SetAt(strFileName, oInfo);
			}
			else
			{
				oInfo = pPair->m_value;
			}

			return oInfo;
		}

		ImageType GetImageType(MediaCore::IAVSUncompressedVideoFrame* pFrame)
		{
			LONG lWidth		= 0;
			LONG lHeight	= 0;
			BYTE* pBuffer	= NULL;

			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);
			pFrame->get_Buffer(&pBuffer);

			BYTE* pBufferMem = pBuffer + 3;
			LONG lCountPix = lWidth * lHeight;

			for (LONG i = 0; i < lCountPix; ++i, pBufferMem += 4)
			{
				if (255 != *pBufferMem)
					return itPNG;
			}
			return itJPG;
		}

		void FlipY(IUnknown* punkImage)
		{
			if (NULL == punkImage)
				return;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			if (NULL == pFrame)
				return;

			BYTE* pBuffer	= NULL;
			LONG lWidth		= 0;
			LONG lHeight	= 0;
			LONG lStride	= 0;

			pFrame->get_Buffer(&pBuffer);
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);
			pFrame->get_Stride(0, &lStride);

			if (lStride < 0)
				lStride = -lStride;
			
			if ((lWidth * 4) != lStride)
			{
				RELEASEINTERFACE(pFrame);
				return;
			}

			BYTE* pBufferMem = new BYTE[lStride];

			BYTE* pBufferEnd = pBuffer + lStride * (lHeight - 1);

			LONG lCountV = lHeight / 2;

			for (LONG lIndexV = 0; lIndexV < lCountV; ++lIndexV)
			{
				memcpy(pBufferMem, pBuffer, lStride);
				memcpy(pBuffer, pBufferEnd, lStride);
				memcpy(pBufferEnd, pBufferMem, lStride);
				
				pBuffer		+= lStride;
				pBufferEnd	-= lStride;
			}

			RELEASEARRAYOBJECTS(pBufferMem);
			RELEASEINTERFACE(pFrame);
		}

		void FlipX(IUnknown* punkImage)
		{
			if (NULL == punkImage)
				return;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			if (NULL == pFrame)
				return;

			BYTE* pBuffer	= NULL;
			LONG lWidth		= 0;
			LONG lHeight	= 0;
			LONG lStride	= 0;

			pFrame->get_Buffer(&pBuffer);
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);
			pFrame->get_Stride(0, &lStride);

			if (lStride < 0)
				lStride = -lStride;
			
			if ((lWidth * 4) != lStride)
			{
				RELEASEINTERFACE(pFrame);
				return;
			}

			DWORD* pBufferDWORD	= (DWORD*)pBuffer;

			LONG lW2 = lWidth / 2;
			for (LONG lIndexV = 0; lIndexV < lHeight; ++lIndexV)
			{
				DWORD* pMem1 = pBufferDWORD;
				DWORD* pMem2 = pBufferDWORD + lWidth - 1;
				
				LONG lI = 0;
				while (lI < lW2)
				{
					DWORD dwMem = *pMem1;
					*pMem1++ = *pMem2;
					*pMem2-- = dwMem;
				}
			}

			RELEASEINTERFACE(pFrame);
		}

	public:

		void LoadResourceFile(HINSTANCE hInst, LPCTSTR sResName, LPCTSTR sResType, const CString& strDstFile)
		{
			HRSRC hrRes = FindResource(hInst, sResName, sResType);
			if (!hrRes)
				return;

			HGLOBAL hGlobal = LoadResource(hInst, hrRes);
			DWORD sz = SizeofResource(hInst, hrRes);
			void* ptrRes = LockResource(hGlobal);
			
			CFile oFile;
			oFile.CreateFile(strDstFile);
			oFile.WriteFile(ptrRes, sz);

			UnlockResource(hGlobal);
			FreeResource(hGlobal);
		}
	};
}