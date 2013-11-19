#pragma once
#include "Writer.h"

namespace NSEBook
{
	class CWriter_Fb2
	{
	public:
		NSCommon::CStringWriter m_oWriter;

	public:
		void CreateDocument(CWriter* pWriter)
		{
			m_oWriter.ClearNoAttack();

			CFormat* pFormat = &pWriter->m_oFormat;
			
			m_oWriter.WriteStringC(_T("<?xml version=\"1.0\" encoding=\"utf-8\"?>"));
			m_oWriter.WriteStringC(_T("<FictionBook xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.gribuser.ru/xml/fictionbook/2.0\">"));

			m_oWriter.WriteStringC(_T("<description><title-info>"));
			m_oWriter.WriteString(_T("<author><first-name></first-name><last-name>") + pFormat->m_oMeta.m_strAuthor + _T("</last-name></author>"));
			m_oWriter.WriteString(_T("<book-title>") + pFormat->m_oMeta.m_strName + _T("</book-title>"));
			m_oWriter.WriteString(_T("<lang>") + pFormat->m_oMeta.m_strLanguage + _T("</lang>"));

			if (pWriter->m_bIsSaveCover)
			{
				CString strCover = _T("#image1.jpg");
				if (0 != pWriter->m_oFormat.m_oImageManager.m_listImages.GetCount())
				{
					if (NSCommon::itPNG == pWriter->m_oFormat.m_oImageManager.m_listImages.GetHead().m_eType)
						strCover = _T("#image1.png");
				}
				m_oWriter.WriteString(_T("<coverpage><image xlink:href=\"") + strCover + _T("\"/></coverpage>"));
			}

			if (0 != pFormat->m_oMeta.m_listAnnotation.GetCount())
			{
				m_oWriter.WriteStringC(_T("<annotation>"));

				POSITION pos = pFormat->m_oMeta.m_listAnnotation.GetHeadPosition();
				while (NULL != pos)
				{
					IEbookItem* pItem = pFormat->m_oMeta.m_listAnnotation.GetNext(pos);

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

				m_oWriter.WriteStringC(_T("</annotation>"));
			}

			m_oWriter.WriteStringC(_T("</title-info>"));

			m_oWriter.WriteStringC(_T("<document-info><author><first-name></first-name><last-name></last-name></author><program-used>Online Media Technologies Ltd.</program-used><date value=\"2011-07-26\">26.07.2011</date><id>"));
			GUID guid;
			CoCreateGuid(&guid);
			CString strGuid = _T("");
			strGuid.Format(_T("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"), guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
			m_oWriter.WriteString(strGuid);			
			m_oWriter.WriteStringC(_T("</id><version>1.0</version></document-info>"));

			m_oWriter.WriteString(_T("<publish-info><publisher>") + pFormat->m_oMeta.m_strPublisher + _T("</publisher></publish-info></description>"));
			
			m_oWriter.WriteString(_T("<body>"));

			POSITION posSect = pFormat->m_listSections.GetHeadPosition();
			while (NULL != posSect)
			{
				m_oWriter.WriteStringC(_T("<section>"));
				CSection& oSection = pFormat->m_listSections.GetNext(posSect);

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
				m_oWriter.WriteStringC(_T("</section>"));
			}

			m_oWriter.WriteString(_T("</body>"));
		}
		void CloseFile(CString strDstFile, CWriter* pWriter)
		{
			CFile oFile;
			oFile.CreateFile(strDstFile);

			CString strData = m_oWriter.GetCString();
			oFile.WriteStringUTF8(strData);
			
			m_oWriter.Clear();

			POSITION pos = pWriter->m_oFormat.m_oImageManager.m_mapImagesFile.GetStartPosition();
			CString strMedia = pWriter->m_oFormat.m_oTempManager.m_strTempImagesDirectory;
			while (NULL != pos)
			{
				CImageInfo& info = pWriter->m_oFormat.m_oImageManager.m_mapImagesFile.GetNextValue(pos);
				WriteImageToFile(oFile, info, strMedia);
			}
			pos = pWriter->m_oFormat.m_oImageManager.m_mapImageData.GetStartPosition();
			while (NULL != pos)
			{
				CImageInfo& info = pWriter->m_oFormat.m_oImageManager.m_mapImageData.GetNextValue(pos);
				WriteImageToFile(oFile, info, strMedia);
			}

			CString strEnd = _T("</FictionBook>");
			oFile.WriteStringUTF8(strEnd);
			
			oFile.CloseFile();
		}

		void WriteImageToFile(CFile& oFile, CImageInfo& info, CString& strMedia)
		{
			CFile oImageFile;
			oImageFile.OpenFile(info.GetPath(strMedia));
			int nInputLen = (int)oImageFile.GetFileSize();
			BYTE* pData = new BYTE[nInputLen];
			oImageFile.ReadFile(pData, nInputLen);

			int nOutputLen = Base64EncodeGetRequiredLength(nInputLen);
			BYTE* pOutput = new BYTE[nOutputLen];
			Base64Encode(pData, nInputLen, (LPSTR)pOutput, &nOutputLen);

			CString strBinary = _T("");
			if (itJPG == info.m_eType)
				strBinary.Format(_T("<binary content-type=\"image/jpg\" id=\"image%d.jpg\">"), info.m_lID);
			else
				strBinary.Format(_T("<binary content-type=\"image/png\" id=\"image%d.png\">"), info.m_lID);

			oFile.WriteStringUTF8(strBinary);
			oFile.WriteFile((void*)pOutput, nOutputLen);
			CString strBinaryEnd = _T("</binary>");
			oFile.WriteStringUTF8(strBinaryEnd);

			RELEASEARRAYOBJECTS(pData);
			RELEASEARRAYOBJECTS(pOutput);
		}

	protected:

		AVSINLINE void WriteImage(NSEBook::CEbookGraphicObject* pImage)
		{
			CString strImage = _T("");
			strImage.Format(_T("<image xlink:href=\"#image%d.%s\"/>"), pImage->m_oInfo.m_lID, 
				(NSCommon::itJPG == pImage->m_oInfo.m_eType) ? _T("jpg") : _T("png"));
			m_oWriter.WriteStringC(strImage);
		}
		AVSINLINE void WriteParagraph(NSEBook::CEbookParagraph* pParagraph)
		{
			//pParagraph->CheckShadows();

			if (1 == pParagraph->m_lStyle)
				m_oWriter.WriteStringC(_T("<title>"));
			else if (2 == pParagraph->m_lStyle)
				m_oWriter.WriteStringC(_T("<subtitle>"));
			
			m_oWriter.WriteStringC(_T("<p>"));
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

			if (1 == pParagraph->m_lStyle)
				m_oWriter.WriteStringC(_T("</title>"));
			else if (2 == pParagraph->m_lStyle)
				m_oWriter.WriteStringC(_T("</subtitle>"));
		}

		AVSINLINE void WriteSpan(NSEBook::CEbookParagraph::CSpan* pSpan)
		{
			if (-1 != pSpan->m_oImageInfo.m_lID)
			{
				CString strImage = _T("");
				strImage.Format(_T("<image xlink:href=\"#image%d.%s\"/>"), pSpan->m_oImageInfo.m_lID, 
					(NSCommon::itJPG == pSpan->m_oImageInfo.m_eType) ? _T("jpg") : _T("png"));
				m_oWriter.WriteStringC(strImage);
				return;
			}
			CString strStyle1 = _T("");
			CString strStyle2 = _T("");
			
			if (pSpan->m_oFont.Bold)
			{
				strStyle1 += _T("<strong>");
				strStyle2 += _T("</strong>");
			}
			if (pSpan->m_oFont.Italic)
			{
				strStyle1 += _T("<emphasis>");
				strStyle2 = (_T("</emphasis>") + strStyle2);
			}
			if (pSpan->m_oFont.Strikeout != 0) 
			{
				strStyle1 += _T("<strikethrough>");
				strStyle2 = (_T("</strikethrough>") + strStyle2);
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
		}
		AVSINLINE void WriteNumbering(NSEBook::CEbookNumbering* pNumbering)
		{

		}
	};
}