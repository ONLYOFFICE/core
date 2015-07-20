#pragma once
#include "Text.h"
#include "Document.h"
#include "CalculatorCRC32.h"
#include "..\Graphics\Matrix.h"

namespace NSHtmlRenderer
{
	static _bstr_t _g_canvas_bstr_StartPath		= L"c.beginPath();";
	static _bstr_t _g_canvas_bstr_ClosePath		= L"c.closePath();";

	static CString _g_canvas_string_MoveToD		= _T("c.moveTo(%d,%d);");
	static CString _g_canvas_string_LineToD		= _T("c.lineTo(%d,%d);");
	static CString _g_canvas_string_CurveToD	= _T("c.bezierCurveTo(%d,%d,%d,%d,%d,%d);");

	static _bstr_t _g_canvas_bstr_Stroke		= L"c.stroke();";
	static _bstr_t _g_canvas_bstr_Fill			= L"c.fill();";

	static _bstr_t _g_canvas_bstr_Save			= L"c.save();\n";
	static _bstr_t _g_canvas_bstr_Restore		= L"c.restore();\n";
	static _bstr_t _g_canvas_bstr_Clip			= L"c.clip();\n";

	static CString _g_canvas_string_css_color	= _T("rgb(%d,%d,%d)");

	static CString _g_canvas_string_FillColor		= _T("c.fillStyle = \"#%06x\";");
	static CString _g_canvas_string_FillColorA		= _T("c.fillStyle = \"rgba(%d,%d,%d,%.2lf)\";");
	static CString _g_canvas_string_StrokeColor		= _T("c.strokeStyle = \"#%06x\";");
	static CString _g_canvas_string_StrokeColorA	= _T("c.strokeStyle = \"rgba(%d,%d,%d,%.2lf)\";");
	static CString _g_canvas_string_drawImageJPG	= _T("var img = new Image();img.src = \"media\\\\image%d.jpg\";img.onload = function(){c.drawImage(img,%d,%d,%d,%d);doc.p%d_%d(doc,page,c);};\n}\nthis.p%d_%d = function(doc,page,c)\n{\n");
	static CString _g_canvas_string_drawImagePNG	= _T("var img = new Image();img.src = \"media\\\\image%d.png\";img.onload = function(){c.drawImage(img,%d,%d,%d,%d);doc.p%d_%d(doc,page,c);};\n}\nthis.p%d_%d = function(doc,page,c)\n{\n");

	using namespace NSStrings;
	class CWriter2
	{
	private:
		CStringWriter m_oWriterPage;

	public:
		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;
		NSStructures::CFont*	m_pFont;

		NSStructures::CPen		m_oLastPen;
		NSStructures::CBrush	m_oLastBrush;
		NSStructures::CFont		m_oLastFont;

		double m_dDpiX;
		double m_dDpiY;

		BOOL					m_bPathClosed;

		CAtlMap<CString, CImageInfo>	m_mapImagesFile;
		CAtlMap<DWORD, CImageInfo>		m_mapImageData;

		CCalculatorCRC32		m_oCRC;

		LONG m_lWidthDocPix;
		LONG m_lHeightDocPix;
		LONG m_lHeightPagePix;

		Graphics::IASCGraphicSimpleComverter* m_pSimpleConverter;
		CFile m_oFileWriter;

	public:

		double					m_dWidthPix;
		double					m_dHeigthPix;

	private:

		LONG					m_lNextIDShape;
		LONG					m_lNextIDImage;

		LONG					m_lCurrentPage;
		LONG					m_lCurrentFunctionPage;
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
		CWriter2()
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

			m_lPagesCount		= 0;
		}


		void CreateFile(CString& strDir, CString& strFileName)
		{
			m_lPagesCount		= 0;
			m_lCurrentPage		= -1;

			m_strDstDirectory		= strDir;
			m_strDstDirectoryFiles	= m_strDstDirectory + _T("\\") + strFileName + _T("_files");
			m_strFileName			= strFileName;

			CDirectory::CreateDirectory(m_strDstDirectoryFiles);
			m_strDstMedia = m_strDstDirectoryFiles + _T("\\media");
			CDirectory::CreateDirectory(m_strDstMedia);

			CString strFileScript = m_strDstDirectoryFiles + _T("\\scripts");
			CDirectory::CreateDirectory(strFileScript);

			CString strDocRendererS = strFileScript + _T("\\documentrenderer.js");
			m_oFileWriter.CreateFile(strDocRendererS);

			CStringA strStart = "function CDocumentRenderer(){\nthis.bIsRepaint = 0;\nthis.sendRepaint = function(){\nthis.bIsRepaint = 1;\n}\nthis.checkRepaint = function(){\nif (this.bIsRepaint == 1){\nthis.bIsRepaint = 0;return 1;\n}\nreturn 0;\n}\n";
			m_oFileWriter.WriteFile((void*)strStart.GetBuffer(), strStart.GetLength());

			m_oWriterPage.Clear();
		}

		void SetSimpleConverter(Graphics::IASCGraphicSimpleComverter* pSimpleConverter, CMatrix* pMatrix)
		{
			m_pSimpleConverter = pSimpleConverter;
		}
				
		AVSINLINE void WriteText(BSTR bsText, BSTR bsGid, double x, double y, double width, double height, double baselineoffset)
		{
			if (!m_oLastBrush.IsEqual(m_pBrush))
			{
				m_oLastBrush = *m_pBrush;
				SetFillColor();
			}
			if (!m_oLastFont.IsEqual(m_pFont))
			{
				m_oLastFont = *m_pFont;
				SetFontStyle();
			}

			CString strText = (CString)bsText;
			strText.Replace(_T("\\"), _T("\\\\"));
			strText.Replace(_T("\""), _T("\\\""));

			CString str = _T("c.fillText(\"") + strText;
			CString strPos = _T("");
			strPos.Format(_T("\",%d,%d);\n"), (int)x, (int)(y + (baselineoffset * 96 / 25.4)));
			str += strPos;
			m_oWriterPage.WriteString(str);
		}
		void WriteImage(IUnknown* punkImage, double x, double y, double width, double height, double dAngle)
		{
			if (height < 0)
			{
				FlipY(punkImage);
				height = -height;
				y -= height;
			}
			
			CImageInfo oID = GenerateImageID(punkImage);
			if (oID.m_eType == itJPG)
			{
				CString strW = _T("");
				strW.Format(_g_canvas_string_drawImageJPG, oID.m_lID, (int)x, (int)y, (int)width, (int)height, m_lCurrentPage, m_lCurrentFunctionPage,
					m_lCurrentPage, m_lCurrentFunctionPage);
				m_oWriterPage.WriteString(strW);
				++m_lCurrentFunctionPage;
			}
			else
			{
				CString strW = _T("");
				strW.Format(_g_canvas_string_drawImagePNG, oID.m_lID, (int)x, (int)y, (int)width, (int)height, m_lCurrentPage, m_lCurrentFunctionPage,
					m_lCurrentPage, m_lCurrentFunctionPage);
				m_oWriterPage.WriteString(strW);
				++m_lCurrentFunctionPage;
			}
		}
		AVSINLINE void WriteImage(CString& strFile, double x, double y, double width, double height, double dAngle)
		{
			CImageInfo oID = GenerateImageID(strFile);

			if (oID.m_eType == itJPG)
			{
				CString strW = _T("");
				strW.Format(_g_canvas_string_drawImageJPG, oID.m_lID, (int)x, (int)y, (int)width, (int)height, m_lCurrentPage, m_lCurrentFunctionPage,
					m_lCurrentPage, m_lCurrentFunctionPage);
				m_oWriterPage.WriteString(strW);
				++m_lCurrentFunctionPage;
			}
			else
			{
				CString strW = _T("");
				strW.Format(_g_canvas_string_drawImagePNG, oID.m_lID, (int)x, (int)y, (int)width, (int)height, m_lCurrentPage, m_lCurrentFunctionPage,
					m_lCurrentPage, m_lCurrentFunctionPage);
				m_oWriterPage.WriteString(strW);
				++m_lCurrentFunctionPage;
			}
		}
		
		AVSINLINE void WriteBeginPath()
		{
			m_oWriterPage.WriteString(_g_canvas_bstr_StartPath);
		}

		AVSINLINE void WriteEndPath()
		{
			m_oWriterPage.WriteString(_g_canvas_bstr_StartPath);				
		}		
		
		AVSINLINE void WritePathStart()
		{
			
		}
		AVSINLINE void WritePathClose()
		{
			m_oWriterPage.WriteString(_g_canvas_bstr_ClosePath);			
		}

		AVSINLINE void WritePathMoveTo(double& x, double& y)
		{
			CString str = _T("");
			str.Format(_g_canvas_string_MoveToD, (int)x, (int)y);
			m_oWriterPage.WriteString(str);
		}
		AVSINLINE void WritePathLineTo(double& x, double& y)
		{
			CString str = _T("");
			str.Format(_g_canvas_string_LineToD, (int)x, (int)y);
			m_oWriterPage.WriteString(str);			
		}
		AVSINLINE void WritePathCurveTo(double& x1, double& y1, double& x2, double& y2, double& x3, double& y3)
		{
			CString str = _T("");
			str.Format(_g_canvas_string_CurveToD, (int)x1, (int)y1, (int)x2, (int)y2, (int)x3, (int)y3);
			m_oWriterPage.WriteString(str);	
		}
		AVSINLINE void WriteDrawPath(LONG lType, const double& dAngle)
		{
			if (lType > 0xFF)
			{
				if (!m_oLastBrush.IsEqual(m_pBrush))
				{
					m_oLastBrush = *m_pBrush;
					SetFillColor();
				}
				m_oWriterPage.WriteString(_g_canvas_bstr_Fill);
			}
			if (lType & 0x01)
			{
				if (!m_oLastPen.IsEqual(m_pPen))
				{
					m_oLastPen = *m_pPen;
					SetStrokeColor();
				}
				m_oWriterPage.WriteString(_g_canvas_bstr_Stroke);
			}						
		}

		AVSINLINE void WritePathClip()
		{
		}
		AVSINLINE void WritePathClipEnd()
		{
		}		
		AVSINLINE void WritePathResetClip()
		{
		}

		AVSINLINE void SetStrokeColor()
		{
			LONG lBGR	= m_pPen->Color;
			LONG lA		= m_pPen->Alpha;

			BYTE R = (BYTE)(lBGR & 0xFF);
			BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
			BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

			CString strTemp = _T("");
			strTemp.Format(_g_canvas_string_StrokeColorA, R, G, B, (double)lA / 255);
			m_oWriterPage.WriteString(strTemp);
		}
		AVSINLINE void SetFillColor()
		{
			LONG lBGR	= m_pBrush->Color1;
			LONG lA		= m_pBrush->Alpha1;

			BYTE R = (BYTE)(lBGR & 0xFF);
			BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
			BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

			CString strTemp = _T("");
			strTemp.Format(_g_canvas_string_FillColorA, R, G, B, (double)lA / 255);
			m_oWriterPage.WriteString(strTemp);
		}
		AVSINLINE void SetFontStyle()
		{
			CString strFormat = _T("");
			if (m_pFont->Bold && m_pFont->Italic)
				strFormat = _T("bold italic ");
			else if (m_pFont->Bold)
				strFormat = _T("bold ");
			else if (m_pFont->Italic)
				strFormat = _T("italic ");

			CString strSize = _T("");
			strSize.Format(_T("%dpt "), (int)m_pFont->Size);

			strFormat += strSize;
			strFormat += m_pFont->Name;
			
			strFormat = _T("c.font=\"") + strFormat + _T("\";");
			m_oWriterPage.WriteString(strFormat);
		}

		void NewPage(double& dWidthPix, double& dHeightPix)
		{	
			++m_lPagesCount;

			m_dWidthPix		= dWidthPix;
			m_dHeigthPix	= dHeightPix;

			if (m_lWidthDocPix < (LONG)dWidthPix)
				m_lWidthDocPix = (LONG)dWidthPix;
			if (m_lHeightPagePix < (LONG)dHeightPix)
				m_lHeightPagePix = (LONG)dHeightPix;

			m_lHeightDocPix += (LONG)dHeightPix;
			++m_lCurrentPage;

			m_lCurrentFunctionPage = 0;

			CString strStartPage = _T("");
			strStartPage.Format(_T("this.p%d = function(doc,page){var c = page.cachedImage.image.ctx;c.setTransform(1,0,0,1,0,0);\n"), m_lCurrentPage);
			m_oWriterPage.WriteString(strStartPage);

			m_oLastBrush.Color1 = -1;
			m_oLastPen.Color	= -1;
			m_oLastFont.Name	= _T("");
		}
		void EndPage()
		{
			CString strEndPage = _T("\nif (true) {page.stopRendering();} doc.sendRepaint();}\n");
			m_oWriterPage.WriteString(strEndPage);

			CString strData = (CString)m_oWriterPage.GetCString();
			m_oFileWriter.WriteStringUTF8(strData);
			m_oWriterPage.ClearNoAttack();
		}

		void WriteStartDocument()
		{
			m_dHeightDoc = 0;
		}
		void WriteEndDocument(CDocument& oDocument)
		{
			CString strDst = _T("");
			strDst.Format(_T("\n// ------------------------------------\nthis.pagesCount = %d;\nthis.arrayMethods = new Array(this.pagesCount);\n\
							 // ------------------------------------\nthis.drawpage = function(page){this.arrayMethods[page.pageIndex](this,page);}\n// ------------------------------------\n"), (int)oDocument.m_arrPages.GetCount());

			CString strInitDoc = _T("function InitDocumentPages(_arrayPages){\n");

			int nPagesCount = (int)oDocument.m_arrPages.GetCount();
			for (int i = 0; i < nPagesCount; ++i)
			{
				CString str1 = _T("");
				str1.Format(_T("this.arrayMethods[%d] = this.p%d;\n"), i, i);
				strDst += str1;

				CString str2 = _T("");
				str2.Format(_T("_arrayPages[%d] = new CPage(%.1lf, %.1lf, %d);\n"), i, oDocument.m_arrPages[i].GetWidth(), oDocument.m_arrPages[i].GetHeight(), i);
				strInitDoc += str2;
			}

			strDst += _T("}");
			strInitDoc += _T("\n}");

			strDst += strInitDoc;

			CStringA strMemory = (CStringA)strDst;
			m_oFileWriter.WriteFile(strMemory.GetBuffer(), strMemory.GetLength());
			m_oFileWriter.CloseFile();

			HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_SCRIPT_PAGE),	_T("HTML2"),	m_strDstDirectoryFiles + _T("\\scripts\\page.js"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_SCRIPT_IM),		_T("HTML2"),	m_strDstDirectoryFiles + _T("\\scripts\\images.js"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_SCRIPT_MAIN),	_T("HTML2"),	m_strDstDirectoryFiles + _T("\\scripts\\main.js"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_SCRIPT_CM),		_T("HTML2"),	m_strDstDirectoryFiles + _T("\\scripts\\cachemanager.js"));
			LoadResourceFile(hInst, MAKEINTRESOURCE(IDB_SCRIPT_VIEW),	_T("HTML2"),	m_strDstDirectoryFiles + _T("\\viewer.html"));
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

			if ((lWidth <= m_lMaxSizeImage) && (lHeight <= m_lMaxSizeImage))
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
					lW = m_lMaxSizeImage;
					lH = (LONG)((double)lW / dAspect);
				}
				else
				{
					lH = m_lMaxSizeImage;
					lW = (LONG)(dAspect * lH);
				}

				CString strResize = _T("");
				strResize.Format(_T("<ImageTransform-TransformResize width=\"%d\" height=\"%d\"/>"), lW, lH);

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

			CString strXml = _T("");
			if ((lWidth <= m_lMaxSizeImage) && (lHeight <= m_lMaxSizeImage))
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
					lW = m_lMaxSizeImage;
					lH = (LONG)((double)lW / dAspect);
				}
				else
				{
					lH = m_lMaxSizeImage;
					lW = (LONG)(dAspect * lH);
				}

				CString strResize = _T("");
				strResize.Format(_T("<ImageTransform-TransformResize width=\"%d\" height=\"%d\"/>"), lW, lH);

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