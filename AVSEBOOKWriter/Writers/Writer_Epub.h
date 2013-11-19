#pragma once
#include "Writer.h"

#define CODEPAGE_ISSUE_FIX
#include "../AVSOfficeUtils/ZLIB/zlib-1.2.3/contrib/minizip/zip.h"

#define BUFSIZE 2048
#define MAXHTMLSIZE 130000 // 130000 bytes

namespace NSEBook
{
	class CWriter_Epub
	{
	private:
		NSCommon::CStringWriter m_oWriter;
		NSCommon::CStringWriter m_oWriterTOC;
		NSCommon::CStringWriter m_oWriterTOCReferenses;
		LONG m_lCurrentHtml;

		CFormat* m_pFormat;
		LONG m_lSectionWrite;

		CString m_strFontsItems;
		bool	m_bIsFontsEmbedded;
		bool	m_bIsSaveCover;
		
	public:
		void CreateDocument(CWriter* pWriter)
		{
			m_bIsFontsEmbedded  = pWriter->m_bIsEmbeddedFonts;
			m_bIsSaveCover		= pWriter->m_bIsSaveCover;

			m_lSectionWrite = 1;
			m_lCurrentHtml = 1;

			// METAINF
			CString strMETA_INF = pWriter->m_oFormat.m_oTempManager.m_strTempDirectory + _T("\\META-INF");
			CDirectory::CreateDirectory(strMETA_INF);
			CString str1 = _T("<?xml version=\"1.0\"?><container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">\
<rootfiles><rootfile full-path=\"content.opf\" media-type=\"application/oebps-package+xml\"/></rootfiles></container>");
			CFile oFile1;
			oFile1.CreateFile(strMETA_INF + _T("\\container.xml"));
			oFile1.WriteStringUTF8(str1);
			oFile1.CloseFile();

			// mimetype
			CStringA str2 = "application/epub+zip";
			CFile oFile2;
			oFile2.CreateFile(pWriter->m_oFormat.m_oTempManager.m_strTempDirectory + _T("\\mimetype"));
			oFile2.WriteFile((void*)str2.GetBuffer(), str2.GetLength());
			oFile2.CloseFile();

			// css
			CString strCSS = _T(".heading1 { text-align: center; font-weight: bold; font-size: 2em; }\n\
.heading2 { text-align: center; font-weight: bold; font-size: 1.5em; }n\
.heading0 { text-indent: 2em; text-align: justify; font-size: 1em; }\n\
.image_inline { vertical-align: baseline; }\n\
.image_block { vertical-align: baseline; }\n\
.bookpagebreak { display: block; page-break-after:always; margin-bottom: 0; margin-left: 0; margin-right: 0; margin-top: 0 }\n\
p {text-indent: 1.5em; }\n\
.main { display: block; font-size: 1em; margin-bottom: 0; margin-left: 5pt; margin-right: 5pt; margin-top: 0; padding-left: 0; padding-right: 0; text-align: justify }\n\
.hyper { text-decoration: underline; }\n");

			if (m_bIsFontsEmbedded && (NULL != pWriter->m_pCurrentPage))
			{
				::CreateDirectory(pWriter->m_oFormat.m_oTempManager.m_strTempDirectory + _T("\\fonts"), NULL);

				CString strCssMem = _T("");
				m_strFontsItems = _T("");
				pWriter->m_pCurrentPage->m_oFontsGen.WriteFonts(pWriter->m_oFormat.m_oTempManager.m_strTempDirectory, strCssMem, m_strFontsItems);
				
				strCSS += strCssMem;
			}

			CFile oFile3;
			oFile3.CreateFile(pWriter->m_oFormat.m_oTempManager.m_strTempDirectory + _T("\\stylesheet.css"));
			oFile3.WriteStringUTF8(strCSS);
			oFile3.CloseFile();

			m_pFormat = &pWriter->m_oFormat;
			POSITION posSect = m_pFormat->m_listSections.GetHeadPosition();
			while (NULL != posSect)
			{
				if (1 != m_lCurrentHtml)
					DumpCurrentHtml(m_pFormat);

				CSection& oSection = m_pFormat->m_listSections.GetNext(posSect);
				DumpSection(&oSection);

				POSITION pos = oSection.m_arItems.GetHeadPosition();
				while (NULL != pos)
				{
					IEbookItem* pItem = oSection.m_arItems.GetNext(pos);

					switch (pItem->m_eType)
					{
					case IEbookItem::ebitParagraph:
						{
							WriteParagraph((CEbookParagraph*)pItem);
							break;
						}
					case IEbookItem::ebitTable:
						{
							WriteTable((CEbookTable*)pItem);
							break;
						}
					case IEbookItem::ebitGraphic:
						{
							WriteImage((CEbookGraphicObject*)pItem);
							break;
						}
					case IEbookItem::ebitNumbering:
						{
							WriteNumbering((CEbookNumbering*)pItem);
							break;
						}
					default:
						break;
					}
				}
			}

			DumpCurrentHtml(m_pFormat, true);
		}
		void CloseFile(CString strDstFile, CWriter* pWriter)
		{
			WriteOPF(m_pFormat);
			CompressEpubDir(pWriter->m_oFormat.m_oTempManager.m_strTempDirectory, strDstFile);
		}

		void DumpSection(CSection* pSection)
		{
			DumpCurrentHtml(m_pFormat, true);

			CString strText = _T("");
			POSITION pos = pSection->m_arItems.GetHeadPosition();
			while (NULL != pos)
			{
				IEbookItem* pItem = pSection->m_arItems.GetNext(pos);
				if (IEbookItem::ebitParagraph == pItem->m_eType)
				{
					CEbookParagraph* pParagraph = (CEbookParagraph*)pItem;
					strText = pParagraph->GetAllText();
					break;
				}
			}

			CorrectString(strText);

			GUID guid;
			CoCreateGuid(&guid);
			CString strGuid = _T("");
			strGuid.Format(_T("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"), guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);


			CString s1 = _T("");
			s1.Format(_T("%d"), m_lSectionWrite++);
			CString s2 = _T("");
			s2.Format(_T("%d"), m_lCurrentHtml);
			
			CString strSect = _T("<navPoint class=\"chapter\" id=\"") + strGuid + _T("\" playOrder=\"") + s1 + _T("\"><navLabel><text>") + strText + 
				_T("</text></navLabel><content src=\"content_") + s2 + _T(".xhtml\"/></navPoint>");

			m_oWriterTOC.WriteStringC(strSect);

			CString strSectRef = _T("<p><a href=\"content_") + s2 + _T(".xhtml\"><span class=\"hyper\">") + strText + _T("</span></a></p>");
			m_oWriterTOCReferenses.WriteStringC(strSectRef);

		}

		void DumpCurrentHtml(NSEBook::CFormat* pFormat, bool bIsEnd = false)
		{
			if (bIsEnd)
			{
				if (0 == m_oWriter.GetCurSize())
					return;
			}
			
			CString str1 = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?><html xmlns=\"http://www.w3.org/1999/xhtml\"><head><title>") + 
				pFormat->m_oMeta.m_strName + _T("</title><meta content=\"http://www.w3.org/1999/xhtml; charset=utf-8\" http-equiv=\"Content-Type\"/>\
<link href=\"stylesheet.css\" type=\"text/css\" rel=\"stylesheet\"/><style type=\"text/css\"></style></head><body class=\"main\">");

			CString str2 = _T("</body></html>");

			CString strNum = _T("");

			strNum.Format(_T("%d"), m_lCurrentHtml);

			++m_lCurrentHtml;
			CString strSave = pFormat->m_oTempManager.m_strTempDirectory + _T("\\content_") + strNum + _T(".xhtml");
			CFile oFile;
			oFile.CreateFile(strSave);
			BYTE utf8_mem[3];
			utf8_mem[0]	= 0xEF;
			utf8_mem[1]	= 0xBB;
			utf8_mem[2]	= 0xBF;
			
			oFile.WriteFile((void*)utf8_mem, 3);
			oFile.WriteStringUTF8(str1);

			CString strBody = m_oWriter.GetCString();
			oFile.WriteStringUTF8(strBody);

			oFile.WriteStringUTF8(str2);
			oFile.CloseFile();

			m_oWriter.ClearNoAttack();
		}

	protected:

		static CStringA UnicodeToCodepage( CStringW sUni, UINT nCodepage = CP_UTF8 )
		{
			CStringA sUtf;
			int nBufSize = WideCharToMultiByte(nCodepage, 0, sUni, -1, NULL, NULL, NULL, NULL);
			LPSTR pBuff = new CHAR[ nBufSize + 1 ];
			WideCharToMultiByte(nCodepage, 0, sUni, -1, pBuff, nBufSize, NULL, NULL);
			pBuff[ nBufSize ] = 0;
			sUtf.Append( pBuff );
			delete pBuff;
			return sUtf;
		}

		int _WriteFileToZip( CString sSource, CString sSourceRel, zipFile zf, int nCompressionLevel )
		{
			int err = 0;
			if( Z_NO_COMPRESSION == nCompressionLevel )
				err = zipOpenNewFileInZip( zf, UnicodeToCodepage (sSourceRel, CP_OEMCP), NULL, NULL, 0, NULL, 0, NULL, NULL, nCompressionLevel );
			else
				err = zipOpenNewFileInZip( zf, UnicodeToCodepage (sSourceRel, CP_OEMCP), NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, nCompressionLevel );
			HANDLE hSourceFile = ::CreateFile( sSource, GENERIC_READ, 0, 0, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, 0);
			if( INVALID_HANDLE_VALUE == hSourceFile )
				err = -1;
			DWORD dwBytesRead = 1;
			BYTE bBuffer[ BUFSIZE ];
			while( -1 != err && 0 != dwBytesRead )
			{
				ReadFile( hSourceFile, bBuffer, BUFSIZE, &dwBytesRead, NULL );
				if( dwBytesRead > 0 )
					err = zipWriteInFileInZip( zf, bBuffer, dwBytesRead );
			}
			RELEASEHANDLE( hSourceFile );
			if( 0 == err )
				err = zipCloseFileInZip( zf );
			else
				zipCloseFileInZip( zf );
			return err;
		}

		int _CompressEpubDir( CString sSource, CString sPathRel, zipFile zf, int nCompresstionLevel )
		{
			DWORD dwFileAttrib = ::GetFileAttributes( sSource );
			if(  dwFileAttrib != INVALID_FILE_ATTRIBUTES )
			{
				DWORD dwResult = 0;
				if( 0 != (FILE_ATTRIBUTE_DIRECTORY & dwFileAttrib) )
				{
					HANDLE Handle;
					WIN32_FIND_DATA FindData;
					DWORDLONG Result = 0;

					Handle = FindFirstFile( ( sSource + _T("\\*.*") ), &FindData );
					if ( Handle == INVALID_HANDLE_VALUE )
						return 0;
					do
					{
						int nRes = 0;
						if( ( CString( FindData.cFileName ) != _T("mimetype") ) && ( CString( FindData.cFileName ) != _T(".") ) && ( CString( FindData.cFileName ) != _T("..") ) )
						{
							if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
							{
								if( _T("fonts") == CString( FindData.cFileName ) )
									dwResult += _CompressEpubDir( sSource + _T("\\") + FindData.cFileName, sPathRel + FindData.cFileName + _T("/"), zf, Z_NO_COMPRESSION );//фонты и так уже запакованы
								else
									dwResult += _CompressEpubDir( sSource + _T("\\") + FindData.cFileName, sPathRel + FindData.cFileName + _T("/"), zf, nCompresstionLevel );
							}
							else
								dwResult += _CompressEpubDir( sSource + _T("\\") + FindData.cFileName, sPathRel, zf, nCompresstionLevel );
						}
					}
					while( FindNextFile( Handle, &FindData ) != 0 );
					FindClose( Handle );
				}
				else
				{
					TCHAR tExt[256];
					TCHAR tFilename[256];
					_wsplitpath( sSource, NULL, NULL, tFilename, tExt );
					CString sExt = tExt;
					CString sFilename = tFilename;
					
					_WriteFileToZip( sSource, sPathRel + sFilename + sExt, zf, nCompresstionLevel );
				}

				return dwResult;
			}// 0 - все закончилось хорошо
			return 0;
		}
		int CompressEpubDir( CString sSource, CString sDestination )
		{
			int err = 0;
			zipFile zf = zipOpen( sDestination, APPEND_STATUS_CREATE );
			if( NULL == zf )
				return 1;

			//первый в архиве всегда должен быть mimetype
			CString sMimeFile = sSource + _T("\\mimetype");
			if( ::GetFileAttributes( sMimeFile ) != INVALID_FILE_ATTRIBUTES )
				err = _WriteFileToZip( sMimeFile, _T("mimetype"), zf, Z_NO_COMPRESSION );

			if( err == 0 )
			{
				//записываем все остальное
				err = _CompressEpubDir( sSource, _T(""), zf, Z_BEST_COMPRESSION );
			}
			if( 0 == err )
				err = zipClose( zf, NULL );
			else
				zipClose( zf, NULL );
			return err;
		}

		void WriteOPF(NSEBook::CFormat* pFormat)
		{
			NSCommon::CStringWriter oOpf;
			oOpf.WriteStringC(_T("<?xml version='1.0' encoding='utf-8'?><package xmlns=\"http://www.idpf.org/2007/opf\" version=\"2.0\"\
 unique-identifier=\"bookid\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\
 xmlns:dcterms=\"http://purl.org/dc/terms/\"><metadata>"));

			oOpf.WriteStringC(_T("<dc:publisher>"));
			oOpf.WriteStringC(CorrectString2(pFormat->m_oMeta.m_strPublisher));
			oOpf.WriteStringC(_T("</dc:publisher>"));

			oOpf.WriteStringC(_T("<dc:language>"));
			//oOpf.WriteStringC(CorrectString2(pFormat->m_oMeta.m_strLanguage));
			oOpf.WriteStringC(_T("</dc:language>"));

			oOpf.WriteStringC(_T("<dc:creator>"));
			oOpf.WriteStringC(CorrectString2(pFormat->m_oMeta.m_strAuthor));
			oOpf.WriteStringC(_T("</dc:creator>"));

			oOpf.WriteStringC(_T("<dc:title>"));
			oOpf.WriteStringC(CorrectString2(pFormat->m_oMeta.m_strName));
			oOpf.WriteStringC(_T("</dc:title>"));

			oOpf.WriteStringC(_T("<dc:contributor>"));
			oOpf.WriteStringC(CorrectString2(pFormat->m_oMeta.m_strCreatorProgram));
			oOpf.WriteStringC(_T("</dc:contributor>"));

			if (0 != pFormat->m_oMeta.m_listAnnotation.GetCount())
			{
				oOpf.WriteStringC(_T("<dc:description>"));

				POSITION pos = pFormat->m_oMeta.m_listAnnotation.GetHeadPosition();
				while (NULL != pos)
				{
					IEbookItem* pItem = pFormat->m_oMeta.m_listAnnotation.GetNext(pos);

					switch (pItem->m_eType)
					{
					case IEbookItem::ebitParagraph:
						{
							CEbookParagraph* pPar = (CEbookParagraph*)pItem;
							oOpf.WriteStringC(CorrectString2(pPar->GetAllText()));
							break;
						}
					default:
						break;
					}
				}

				oOpf.WriteStringC(_T("</dc:description>"));
			}

			GUID guid;
			CoCreateGuid(&guid);
			CString strGuid = _T("");
			strGuid.Format(_T("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"), guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

			oOpf.WriteStringC(_T("<dc:identifier id=\"bookid\" opf:scheme=\"uuid\">"));
			oOpf.WriteStringC(strGuid);
			oOpf.WriteStringC(_T("</dc:identifier>"));

			if (m_bIsSaveCover)
			{
				LONG _lStartImage = max(m_lCurrentHtml, 1000);
				CString strCover = _T("");
				strCover.Format(_T("id%d"), _lStartImage);
				m_oWriter.WriteString(_T("<meta name=\"cover\" content=\"") + strCover + _T("\"/>"));
			}

			oOpf.WriteStringC(_T("</metadata><manifest><opf:item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\"/>\
<item href=\"book_toc.xhtml\" id=\"book_toc_id\" media-type=\"application/xhtml+xml\"/>\
<item href=\"stylesheet.css\" id=\"css\" media-type=\"text/css\"/>"));

			if (m_bIsSaveCover)
			{
				oOpf.WriteStringC(_T("<item id=\"cover\" href=\"cover.xhtml\" media-type=\"application/xhtml+xml\"/>"));
				
				// теперь сохраним xhtml
				CFile oFileCover;
				oFileCover.CreateFile(pFormat->m_oTempManager.m_strTempDirectory + _T("\\cover.xhtml"));
				BYTE utf8_mem[3];
				utf8_mem[0]	= 0xEF;
				utf8_mem[1]	= 0xBB;
				utf8_mem[2]	= 0xBF;

				bool bIsPNG = false;
				if (0 != pFormat->m_oImageManager.m_listImages.GetCount())
				{
					if (NSCommon::itPNG == pFormat->m_oImageManager.m_listImages.GetHead().m_eType)
						bIsPNG = true;
				}

				oFileCover.WriteFile((void*)utf8_mem, 3);

				if (!bIsPNG)
				{
					CString strCoverXHTML = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?><html xmlns=\"http://www.w3.org/1999/xhtml\">\
<head><title>Cover</title></head><body><div class=\"coverpage\">\
<img style=\"width:100%;height: 100%;\" alt=\"Cover\" src=\"media/image1.jpg\"/></div></body></html>");
					
					oFileCover.WriteStringUTF8(strCoverXHTML);
				}
				else
				{
					CString strCoverXHTML = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?><html xmlns=\"http://www.w3.org/1999/xhtml\">\
<head><title>Cover</title></head><body><div class=\"coverpage\">\
<img style=\"width:100%;height: 100%;\" alt=\"Cover\" src=\"media/image1.png\"/></div></body></html>");
					
					oFileCover.WriteStringUTF8(strCoverXHTML);
				}
				oFileCover.CloseFile();
			}

			for (LONG lStartText = 1; lStartText < m_lCurrentHtml; ++lStartText)
			{
				CString strText = _T("");
				strText.Format(_T("<item href=\"content_%d.xhtml\" id=\"id%d\" media-type=\"application/xhtml+xml\"/>"), lStartText, lStartText);
				oOpf.WriteStringC(strText);
			}

			LONG lStartImage = max(m_lCurrentHtml, 1000);
			POSITION posImage = pFormat->m_oImageManager.m_listImages.GetHeadPosition();
			while (NULL != posImage)
			{
				CImageInfo& info = pFormat->m_oImageManager.m_listImages.GetNext(posImage);

				if (itJPG == info.m_eType)
				{
					CString strText = _T("");
					strText.Format(_T("<item href=\"media/image%d.jpg\" id=\"id%d\" media-type=\"image/jpeg\"/>"), info.m_lID, lStartImage);
					oOpf.WriteStringC(strText);
				}
				else
				{
					CString strText = _T("");
					strText.Format(_T("<item href=\"media/%d.png\" id=\"id%d\" media-type=\"image/png\"/>"), info.m_lID, lStartImage);
					oOpf.WriteStringC(strText);
				}
				++lStartImage;
			}

			oOpf.WriteStringC(m_strFontsItems);

			oOpf.WriteStringC(_T("</manifest><spine toc=\"ncx\">"));

			if (m_bIsSaveCover)
			{
				oOpf.WriteStringC(_T("<itemref idref=\"cover\"/>"));
			}

			for (LONG i = 1; i < m_lCurrentHtml; ++i)
			{
				CString strText = _T("");
				strText.Format(_T("<itemref idref=\"id%d\"/>"), i);
				oOpf.WriteStringC(strText);
			}
			if (FALSE)
			{
				oOpf.WriteStringC(_T("<itemref idref=\"book_toc_id\"/>"));
			}

			oOpf.WriteStringC(_T("</spine><guide><reference href=\"book_toc.xhtml#maintoc\" type=\"toc\" title=\"Table of Contents\"/></guide></package>"));

			CFile oFile;
			oFile.CreateFile(pFormat->m_oTempManager.m_strTempDirectory + _T("\\content.opf"));
			CString strCont = oOpf.GetCString();
			oFile.WriteStringUTF8(strCont);
			oFile.CloseFile();

			// toc.ncx
			CFile oFileTOC;
			oFileTOC.CreateFile(pFormat->m_oTempManager.m_strTempDirectory + _T("\\toc.ncx"));

			CString str1 = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?><ncx version=\"2005-1\" xmlns=\"http://www.daisy.org/z3986/2005/ncx/\">\
<head><meta name=\"dtb:uid\" content=\"") + strGuid + _T("\"/><meta name=\"dtb:depth\" content=\"2\"/>\
<meta content=\"Online Media Technologies Ltd.\" name=\"dtb:generator\"/><meta content=\"0\" name=\"dtb:totalPageCount\"/>\
<meta content=\"0\" name=\"dtb:maxPageNumber\"/></head>\
<docTitle><text>") + pFormat->m_oMeta.m_strName + _T("</text></docTitle><docAuthor><text>") + pFormat->m_oMeta.m_strAuthor + _T("</text></docAuthor><navMap>");
			oFileTOC.WriteStringUTF8(str1);
			m_oWriterTOC.WriteStringC(_T("</navMap></ncx>"));

			CString str2 = m_oWriterTOC.GetCString();
			oFileTOC.WriteStringUTF8(str2);
			oFileTOC.CloseFile();

			CFile oFileBook_toc;
			oFileBook_toc.CreateFile(pFormat->m_oTempManager.m_strTempDirectory + _T("\\book_toc.xhtml"));

			CString str11 = _T("<?xml version='1.0' encoding='utf-8'?><html xmlns=\"http://www.w3.org/1999/xhtml\"><head>\
<title>Table of Content</title><meta content=\"http://www.w3.org/1999/xhtml; charset=utf-8\" http-equiv=\"Content-Type\"/>\
<link href=\"stylesheet.css\" type=\"text/css\" rel=\"stylesheet\"/><style type=\"text/css\"></style></head>\
<body class=\"main\"><div id=\"maintoc\"><p class=\"heading1\"><span>Table of Contents</span></p>");

			oFileBook_toc.WriteStringUTF8(str11);
			m_oWriterTOCReferenses.WriteStringC(_T("</div></body></html>"));

			CString str22 = m_oWriterTOCReferenses.GetCString();
			oFileBook_toc.WriteStringUTF8(str22);
			oFileBook_toc.CloseFile();
		}

		AVSINLINE void CheckSplitHtml()
		{
			if (m_oWriter.GetCurSize() > MAXHTMLSIZE)
				DumpCurrentHtml(m_pFormat);
		}

		AVSINLINE void WriteImage(NSEBook::CEbookGraphicObject* pImage)
		{
			CString strImage = _T("");
			strImage.Format(_T("<div style=\"text-align:center;\"><img class=\"image_block\" src=\"media/image%d.%s\" alt=\"\"/></div>"), pImage->m_oInfo.m_lID, 
					(NSCommon::itJPG == pImage->m_oInfo.m_eType) ? _T("jpg") : _T("png"));
			m_oWriter.WriteStringC(strImage);

			if (pImage->m_bIsPageBreakAfter)
			{
				 m_oWriter.WriteStringC(_T("<div class=\"bookpagebreak\"/>"));
			}

			CheckSplitHtml();
		}
		AVSINLINE void WriteParagraph(NSEBook::CEbookParagraph* pParagraph)
		{
			//pParagraph->CheckShadows();

			if (1 == pParagraph->m_lStyle)
				m_oWriter.WriteStringC(_T("<p class=\"heading1\">"));
			else if (2 == pParagraph->m_lStyle)
				m_oWriter.WriteStringC(_T("<p class=\"heading2\">"));
			else
			{
				if (0 == pParagraph->m_lTextAlign)
				{
					m_oWriter.WriteStringC(_T("<p class=\"heading0\">"));
				}
				else if (1 == pParagraph->m_lTextAlign)
				{
					m_oWriter.WriteStringC(_T("<p class=\"heading0\" style=\"text-align:center;\">"));
				}
				else if (2 == pParagraph->m_lTextAlign)
				{
					m_oWriter.WriteStringC(_T("<p class=\"heading0\" style=\"text-align:right;\">"));
				}
				else
				{
					m_oWriter.WriteStringC(_T("<p class=\"heading0\">"));
				}
			}

			NSEBook::CEbookParagraph::CSpan* pSpan = NULL;
			
			POSITION posLine = pParagraph->m_arLines.GetHeadPosition();
			while (NULL != posLine)
			{
				NSEBook::CEbookParagraph::CLine& oLine = pParagraph->m_arLines.GetNext(posLine);

				POSITION posSpan = oLine.m_listSpans.GetHeadPosition();
				while (NULL != posSpan)
				{
					NSEBook::CEbookParagraph::CSpan& oSpan = oLine.m_listSpans.GetNext(posSpan);
					if ((NULL == posSpan) && (NULL != posLine))
						oSpan.m_oText.CheckLastSpanLine();

					if (NULL == pSpan)
						pSpan = &oSpan;
					else if (oSpan.m_oText.IsEqualLast(pSpan->m_oText, ((pSpan->m_dX + pSpan->m_dWidthSpan - oSpan.m_dX) > 1) ? TRUE : FALSE) && (oSpan.m_oImageInfo.m_lID == -1) && (pSpan->m_oImageInfo.m_lID == -1))
					{
						// это тень!
						continue;
					}
					else if (pSpan->m_oFont.IsEqual(&oSpan.m_oFont) && pSpan->m_oBrush.IsEqual(&oSpan.m_oBrush) 
						&& (oSpan.m_oImageInfo.m_lID == -1) && (pSpan->m_oImageInfo.m_lID == -1))
					{
						pSpan->m_oText += oSpan.m_oText;
						double mem = oSpan.m_dX + oSpan.m_dWidthSpan - pSpan->m_dX;
						if (mem > pSpan->m_dWidthSpan)
							pSpan->m_dWidthSpan = mem;
					}
					else
					{
						WriteSpan(pSpan);
						pSpan = &oSpan;
					}
				}
			}

			if (NULL != pSpan)
			{
				WriteSpan(pSpan);
			}

			m_oWriter.WriteStringC(_T("</p>"));

			if (pParagraph->m_bIsPageBreakAfter)
			{
				m_oWriter.WriteStringC(_T("<div class=\"bookpagebreak\"></div>"));
				//m_oWriter.WriteStringC(_T("<p style=\"page-break-after:always\"/>"));
			}

			CheckSplitHtml();
		}

		AVSINLINE void WriteSpan(NSEBook::CEbookParagraph::CSpan* pSpan)
		{
			if (-1 != pSpan->m_oImageInfo.m_lID)
			{
				CString strImage = _T("");
				strImage.Format(_T("<img class=\"image_inline\" src=\"media/image%d.%s\" alt=\"\"/>"), pSpan->m_oImageInfo.m_lID, 
					(NSCommon::itJPG == pSpan->m_oImageInfo.m_eType) ? _T("jpg") : _T("png"));
				m_oWriter.WriteStringC(strImage);
				return;
			}
			CString strStyle1 = _T("");
			CString strStyle2 = _T("");

			if (m_bIsFontsEmbedded)
			{
				CString strName = pSpan->m_oFont.Name;
				strName.Replace(_T(" "), _T("_avs_space_"));
				
				strStyle1 = (_T("<span style=\"font-family:") + strName + _T(";"));
				if (pSpan->m_oFont.Bold)
					strStyle1 += _T("font-weight:bold;");
				else
					strStyle1 += _T("font-weight:normal;");

				if (pSpan->m_oFont.Italic)
					strStyle1 += _T("font-style:italic;\">");
				else
					strStyle1 += _T("font-style:normal;\">");

				strStyle2 = _T("</span>");
			}
			else
			{
				if (pSpan->m_oFont.Bold)
				{
					strStyle1 += _T("<b>");
					strStyle2 += _T("</b>");
				}
				if (pSpan->m_oFont.Italic)
				{
					strStyle1 += _T("<i>");
					strStyle2 = (_T("</i>") + strStyle2);
				}
				if (pSpan->m_oFont.Underline != 0) 
				{
					strStyle1 += _T("<u>");
					strStyle2 = (_T("</u>") + strStyle2);
				}
			}

			if (pSpan->m_dBaselineOffset < 0)
			{
				strStyle1 += _T("<sub>");
				strStyle2 = (_T("</sub>") + strStyle2);
			}
			if (pSpan->m_dBaselineOffset > 0)
			{
				strStyle1 += _T("<sup>");
				strStyle2 = (_T("</sup>") + strStyle2);
			}

			m_oWriter.WriteStringC(strStyle1);
			m_oWriter.WriteTextXML(pSpan->m_oText);
			m_oWriter.WriteStringC(strStyle2);
		}
		AVSINLINE void WriteTable(NSEBook::CEbookTable* pTable)
		{
			m_oWriter.WriteString(pTable->m_strTableXml);
			if (pTable->m_bIsPageBreakAfter)
			{
				 m_oWriter.WriteStringC(_T("<div class=\"bookpagebreak\"/>"));
			}
			CheckSplitHtml();
		}
		AVSINLINE void WriteNumbering(NSEBook::CEbookNumbering* pNumbering)
		{
			CheckSplitHtml();
		}
	};
}