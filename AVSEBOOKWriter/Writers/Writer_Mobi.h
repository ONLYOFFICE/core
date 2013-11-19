#pragma once
#include "Writer.h"
#include "../MOBIFormat/MobiReader.h"

#define FCIS_FLIS TRUE

namespace NSEBook
{
	#define TEXT_REC_LEN 4096
	class CWriter_Mobi
	{
	public:
		class CRecord
		{
		public:
			BYTE* m_pData;
			LONG m_lLen;

		public:
			CRecord()
			{
				m_pData		= NULL;
				m_lLen		= 0;
			}
			~CRecord()
			{
				RELEASEARRAYOBJECTS(m_pData);
			}
		};

	private:
		NSCommon::CStringWriter m_oWriter;
		bool m_bIsCompression;

		CAtlList<CRecord*> m_arRecords;
		LONG m_lTextRecords;
		LONG m_lImageRecords;

		CFormat* m_pFormat;
		bool m_bIsCover;
		bool m_bIsAdditionalTextRec;

	public:
		CWriter_Mobi() : m_arRecords()
		{
			m_bIsCompression = true;
		}
		~CWriter_Mobi()
		{
			POSITION pos = m_arRecords.GetHeadPosition();
			while (NULL != pos)
			{
				CRecord* pRecord = m_arRecords.GetNext(pos);
				RELEASEOBJECT(pRecord);
			}
			m_arRecords.RemoveAll();
		}
	public:
		void CreateDocument(CWriter* pWriter)
		{
			// набиваем writer
			CString str1 = _T("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"/><guide></guide>\n\
<style type=\"text/css\">\n\
.heading1 { text-align: center; font-weight: bold; font-size: 2em; }\n\
.heading2 { text-align: center; font-weight: bold; font-size: 1.5em; }n\
.heading0 { text-indent: 2em; text-align: justify; font-size: 1em; }\n\
.image_inline { vertical-align: baseline; }\n\
.image_block { vertical-align: baseline; }\n\
.main { display: block; font-size: 1em; margin-bottom: 0; margin-left: 5pt; margin-right: 5pt; margin-top: 0; padding-left: 0; padding-right: 0; text-align: justify }\n\
.hyper { text-decoration: underline; }\n\
</style></head><body class=\"main\">");
			m_oWriter.WriteStringC(str1);

			/*CString str1 = _T("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"/><guide></guide>\n\
</head><body>");
			m_oWriter.WriteStringC(str1);*/

			m_bIsCover = pWriter->m_bIsSaveCover;

			m_pFormat = &pWriter->m_oFormat;
			POSITION posSect = m_pFormat->m_listSections.GetHeadPosition();
			while (NULL != posSect)
			{
				CSection& oSection = m_pFormat->m_listSections.GetNext(posSect);

				m_oWriter.WriteStringC(_T("<div><a/>"));

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

				if (NULL != posSect)
					m_oWriter.WriteStringC(_T("<mbp:pagebreak/></div>"));
			}

			m_oWriter.WriteStringC(_T("</body></html>"));

			#ifdef _DEBUG
			CString strXml = m_oWriter.GetCString();
			CDirectory::SaveToFile(_T("C:\\debug_mobi.html"), strXml);
			#endif
		}
		void CloseFile(CString strDstFile, CWriter* pWriter)
		{
			CFile oFile;
			oFile.CreateFile(strDstFile);

			// -------------- DEBUG ------------------------------
			if (FALSE)
			{
				CFile oFileD;
				oFileD.OpenFile(_T("C:\\images.html"));
				CString strH = oFileD.ReadStringFromUTF8((DWORD)oFileD.GetFileSize());
				oFileD.CloseFile();
				m_oWriter.WriteString(strH);
			}
			// ---------------------------------------------------

			// расчитываем записи под текст
			CString strXml = m_oWriter.GetCString();
			int nLength = strXml.GetLength();

			CStringA saStr;
			
			// Encoding Unicode to UTF-8
			WideCharToMultiByte(CP_UTF8, 0, strXml.GetBuffer(), nLength + 1, saStr.GetBuffer(nLength*3 + 1), nLength*3, NULL, NULL);
			saStr.ReleaseBuffer();

			LONG lSrcLen = saStr.GetLength();
			BYTE* pSrcBuffer = (BYTE*)saStr.GetBuffer();
			LONG lSrcCount = saStr.GetLength();

			m_lImageRecords	= (LONG)pWriter->m_oFormat.m_oImageManager.m_listImages.GetCount();

			GenerateRecords(pWriter, pSrcBuffer, lSrcLen);
			GenerateRecord0(&pWriter->m_oFormat, lSrcLen);

			CFormat* pFormat = &pWriter->m_oFormat;

			LONG lRecCount = m_lTextRecords + m_lImageRecords + 1 + 1;
			if (FCIS_FLIS)
				lRecCount += 2;

			NSMobi::CStream oStream;
			oStream.CreateFromBuffer(1000 + lRecCount * 8);
			
			NSMobi::CMobiReader::CPDBHeader oHeader1;
			oHeader1.WriteToStream(&oStream, pFormat->m_oMeta.m_strName, lRecCount);

			POSITION posRec = m_arRecords.GetHeadPosition();
			DWORD dwOffset = (DWORD)oStream.GetPosition() + lRecCount * 8;
			LONG i = 0;
			while (NULL != posRec)
			{
				CRecord* pRecord = m_arRecords.GetNext(posRec);
				
				oStream.WriteDWORD(dwOffset);
				
				DWORD dwFlag = (DWORD)(2 * i);
				dwFlag &= 0x00FFFFFF;

				oStream.WriteDWORD(dwFlag);

				dwOffset += (DWORD)pRecord->m_lLen;
				++i;
			}

			oStream.DumpToFile(oFile);

			posRec = m_arRecords.GetHeadPosition();
			while (NULL != posRec)
			{
				CRecord* pRecord = m_arRecords.GetNext(posRec);
				oFile.WriteFile((void*)pRecord->m_pData, pRecord->m_lLen);
			}

			oFile.CloseFile();
		}

	protected:

		void GenerateRecords(CWriter* pWriter, BYTE* pSrcBuffer, LONG lSrcLen)
		{
			// сначала текст
			BYTE* inputStart = pSrcBuffer;
			LONG lCurrentLenTail = lSrcLen;

			LONG lDataSize = 0;
			while (lCurrentLenTail > 0)
			{
				BYTE* current = inputStart;

				LONG lCurrentRecLen = 0;
				while (lCurrentLenTail > 0)
				{
					if (0x00 == (0x80 & *current))
					{
						++current;
						++lCurrentRecLen;

						--lCurrentLenTail;
					}
					else if (0x00 == (0x20 & *current))
					{
						current += 2;
						lCurrentRecLen += 2;

						lCurrentLenTail -= 2;
					}
					else if (0x00 == (0x10 & *current))
					{
						current += 3;
						lCurrentRecLen += 3;

						lCurrentLenTail -= 3;
					}
					else
					{
						++current;
						++lCurrentRecLen;

						--lCurrentLenTail;
					}
					
					if (lCurrentRecLen >= TEXT_REC_LEN)
					{
						break;
					}
				}

				if (lCurrentRecLen == TEXT_REC_LEN)
				{
					if (!m_bIsCompression)
					{
						CRecord* pRecord	= new CRecord();
						pRecord->m_pData	= new BYTE[TEXT_REC_LEN];
						pRecord->m_lLen		= TEXT_REC_LEN;
						memcpy(pRecord->m_pData, inputStart, TEXT_REC_LEN);
						m_arRecords.AddTail(pRecord);

						lDataSize += pRecord->m_lLen;
					}
					else
					{
						CRecord* pRecord = new CRecord();
						NSMobi::CTextDecoder::PalmEncode(inputStart, TEXT_REC_LEN, pRecord->m_pData, pRecord->m_lLen, TEXT_REC_LEN);
						m_arRecords.AddTail(pRecord);

						lDataSize += pRecord->m_lLen;
					}
				}
				else if (lCurrentRecLen > TEXT_REC_LEN)
				{
					if (!m_bIsCompression)
					{
						CRecord* pRecord	= new CRecord();
						pRecord->m_pData	= new BYTE[lCurrentRecLen];
						pRecord->m_lLen		= lCurrentRecLen;
						memcpy(pRecord->m_pData, inputStart, lCurrentRecLen);
						m_arRecords.AddTail(pRecord);

						lDataSize += pRecord->m_lLen;
					}
					else
					{
						CRecord* pRecord = new CRecord();
						NSMobi::CTextDecoder::PalmEncode(inputStart, TEXT_REC_LEN, pRecord->m_pData, pRecord->m_lLen, lCurrentRecLen);
						m_arRecords.AddTail(pRecord);

						lDataSize += pRecord->m_lLen;
					}
				}
				else
				{
					if (FALSE)
					{
						BYTE* pData	= new BYTE[TEXT_REC_LEN];
						memset(pData, 32, TEXT_REC_LEN);
						memcpy(pData, inputStart, lCurrentRecLen);

						if (!m_bIsCompression)
						{
							CRecord* pRecord	= new CRecord();
							pRecord->m_pData	= pData;
							pRecord->m_lLen		= TEXT_REC_LEN;
							m_arRecords.AddTail(pRecord);

							lDataSize += pRecord->m_lLen;
						}
						else
						{
							CRecord* pRecord = new CRecord();
							NSMobi::CTextDecoder::PalmEncode(pData, lCurrentRecLen, pRecord->m_pData, pRecord->m_lLen, TEXT_REC_LEN);
							m_arRecords.AddTail(pRecord);
							RELEASEARRAYOBJECTS(pData);

							lDataSize += pRecord->m_lLen;
						}
					}
					else
					{
						if (!m_bIsCompression)
						{
							CRecord* pRecord	= new CRecord();
							pRecord->m_pData	= new BYTE[lCurrentRecLen];
							pRecord->m_lLen		= lCurrentRecLen;
							memcpy(pRecord->m_pData, inputStart, lCurrentRecLen);
							m_arRecords.AddTail(pRecord);

							lDataSize += pRecord->m_lLen;
						}
						else
						{
							CRecord* pRecord = new CRecord();
							NSMobi::CTextDecoder::PalmEncode(inputStart, lCurrentRecLen, pRecord->m_pData, pRecord->m_lLen, lCurrentRecLen);
							m_arRecords.AddTail(pRecord);

							lDataSize += pRecord->m_lLen;
						}
					}
				}

				inputStart = current;
			}

			m_bIsAdditionalTextRec = false;

			m_lTextRecords = (LONG)m_arRecords.GetCount();
			
			/*
			LONG lModule4 = lDataSize % 4;
			if (0 != lModule4)
			{
				CRecord* pRecord = new CRecord();
				pRecord->m_pData = new BYTE[pRecord->m_lLen];
				memset(pRecord->m_pData, 0, pRecord->m_lLen);
				m_arRecords.AddTail(pRecord);

				m_bIsAdditionalTextRec = true;
			}
			*/

			NSCommon::CImageManager* pManager = &pWriter->m_oFormat.m_oImageManager;
			CString strMediaPath = pWriter->m_oFormat.m_oTempManager.m_strTempImagesDirectory;
			POSITION posImage = pManager->m_listImages.GetHeadPosition();
			while (NULL != posImage)
			{
				NSCommon::CImageInfo& info = pManager->m_listImages.GetNext(posImage);

				CFile oImageFile;
				oImageFile.OpenFile(info.GetPath(strMediaPath));

				CRecord* pRecord = new CRecord();
				pRecord->m_lLen	 = (LONG)oImageFile.GetFileSize();
				pRecord->m_pData = new BYTE[pRecord->m_lLen];
				oImageFile.ReadFile(pRecord->m_pData, (DWORD)pRecord->m_lLen);
				m_arRecords.AddTail(pRecord);

				oImageFile.CloseFile();
			}
			
			GenerateEndRecords(lSrcLen);
		}

		void GenerateRecord0(NSEBook::CFormat* pFormat, LONG lSrcTextLen)
		{
			LONG lRecCount = m_lTextRecords + m_lImageRecords + 1 + 1;
			if (FCIS_FLIS)
				lRecCount += 2;

			NSMobi::CStream oStream;
			oStream.CreateFromBuffer(1024 * 1024); // 1mb

			// mobiheader
			if (m_bIsCompression)
				oStream.WriteUShort(2);
			else
				oStream.WriteUShort(1);

			oStream.WriteUShort(0);
			oStream.WriteLong(lSrcTextLen);
			oStream.WriteUShort((USHORT)(m_lTextRecords));
			oStream.WriteUShort(4096);
			oStream.WriteUShort(0);
			oStream.WriteUShort(0);

			DWORD uid = rand();

			oStream.WriteCStringA("MOBI");
			oStream.WriteDWORD(0xE8);
			oStream.WriteDWORD(0x02);
			oStream.WriteDWORD(65001);
			oStream.WriteDWORD(uid);
			oStream.WriteDWORD(0x06);

			oStream.WriteSet(0xFF, 8);

			oStream.WriteDWORD(0xFFFFFFFF);

			oStream.WriteSet(0xFF, 28);

			NSMobi::CStream oStreamEXTH;
			NSMobi::CStream oStreamTITLE;
			oStreamEXTH.CreateFromBuffer(1024 * 5);
			oStreamTITLE.CreateFromBuffer(1024 * 5);

			GenerateEXTH(oStreamEXTH, pFormat);
			GenerateTITLE(oStreamTITLE, pFormat);

			oStream.WriteDWORD((DWORD)(m_lTextRecords + 1));
			oStream.WriteDWORD((DWORD)(0xe8 + 16 + oStreamEXTH.GetPosition()));
			oStream.WriteDWORD((DWORD)oStreamTITLE.GetPosition());

			oStream.WriteDWORD(0);

			oStream.WriteSet(0, 8);

			oStream.WriteDWORD(6);

			if (0 == m_lImageRecords)
				oStream.WriteDWORD(0);
			else
				oStream.WriteDWORD(m_lTextRecords + 1);

			oStream.WriteSet(0, 16);

			oStream.WriteDWORD(0x50);
			oStream.WriteSet(0, 32);

			oStream.WriteDWORD(0xFFFFFFFF);
			oStream.WriteDWORD(0xFFFFFFFF);
			oStream.WriteDWORD(0);
			oStream.WriteDWORD(0);

			oStream.WriteSet(0, 12);
			oStream.WriteUShort(1);
			oStream.WriteUShort((USHORT)(lRecCount - 1));

			oStream.WriteByte(0);
			oStream.WriteByte(0);
			oStream.WriteByte(0);
			oStream.WriteByte(1);

			if (FCIS_FLIS)
			{
				oStream.WriteDWORD((DWORD)(m_lImageRecords + m_lTextRecords + 2));
				oStream.WriteDWORD(1);
				oStream.WriteDWORD((DWORD)(m_lImageRecords + m_lTextRecords + 1));
				oStream.WriteDWORD(1);
			}
			else
			{
				oStream.WriteDWORD(0xFFFFFFFF);
				oStream.WriteDWORD(1);
				oStream.WriteDWORD(0xFFFFFFFF);
				oStream.WriteDWORD(1);
			}

			oStream.WriteSet(0, 8);

			oStream.WriteDWORD(0xFFFFFFFF);
			oStream.WriteDWORD(0x00);
			oStream.WriteDWORD(0xFFFFFFFF);
			oStream.WriteDWORD(0xFFFFFFFF);

			oStream.WriteDWORD(1);

			oStream.WriteDWORD(0xFFFFFFFF);

			oStream.Write(&oStreamEXTH);
			oStream.Write(&oStreamTITLE);

			if (TRUE)
			{
				//oStream.WriteSet(0, 1024 * 8);
				oStream.WriteSet(0, 8);
			}

			int nPos = oStream.GetPosition();
			int mod = nPos % 4;
			if (0 != mod)
				oStream.WriteSet(0, 4 - mod);

			CRecord* pRecord = new CRecord();
			pRecord->m_lLen = oStream.GetPosition();
			pRecord->m_pData = new BYTE[pRecord->m_lLen];
			memcpy(pRecord->m_pData, oStream.GetData(), pRecord->m_lLen);
			m_arRecords.AddHead(pRecord);
		}

		void GenerateEndRecords(LONG lTextLength)
		{
			if (FCIS_FLIS)
			{
				NSMobi::CStream oStream;
				oStream.FromBuffer(new BYTE[100], 100);

				oStream.WriteCStringA("FLIS");
				oStream.WriteDWORD(0x08);
				oStream.WriteUShort(0x41);
				oStream.WriteDWORD(0x00);
				oStream.WriteDWORD(0xFFFF);
				oStream.WriteUShort(0xFFFF);
				oStream.WriteDWORD(0x00010003);
				oStream.WriteDWORD(0x03);
				oStream.WriteDWORD(0x01);
				oStream.WriteDWORD(0xFFFFFFFF);
				
				CRecord* pRecord = new CRecord();
				pRecord->m_lLen = (LONG)oStream.GetPosition();
				pRecord->m_pData = new BYTE[pRecord->m_lLen];
				memcpy(pRecord->m_pData, oStream.GetData(), pRecord->m_lLen);
				
				m_arRecords.AddTail(pRecord);

				oStream.Seek(0);

				oStream.WriteCStringA("FCIS");
				oStream.WriteDWORD(0x14);
				oStream.WriteDWORD(0x10);
				oStream.WriteDWORD(0x01);
				oStream.WriteDWORD(0x00);
				oStream.WriteDWORD((DWORD)lTextLength);
				oStream.WriteDWORD(0x00);
				oStream.WriteDWORD(0x20);
				oStream.WriteDWORD(0x08);
				oStream.WriteDWORD(0x00010001);
				oStream.WriteDWORD(0x00);

				CRecord* pRecord2 = new CRecord();
				pRecord2->m_lLen = (LONG)oStream.GetPosition();
				pRecord2->m_pData = new BYTE[pRecord2->m_lLen];
				memcpy(pRecord2->m_pData, oStream.GetData(), pRecord2->m_lLen);

				m_arRecords.AddTail(pRecord2);

				CRecord* pRecordEnd = new CRecord();
				pRecordEnd->m_lLen = 4;
				pRecordEnd->m_pData = new BYTE[4];
				pRecordEnd->m_pData[0] = 0xE9;
				pRecordEnd->m_pData[0] = 0x8E;
				pRecordEnd->m_pData[0] = 0x0D;
				pRecordEnd->m_pData[0] = 0x0A;

				m_arRecords.AddTail(pRecordEnd);
			}
			else
			{
				CRecord* pRecordEnd = new CRecord();
				pRecordEnd->m_lLen = 4;
				pRecordEnd->m_pData = new BYTE[4];
				pRecordEnd->m_pData[0] = 0xE9;
				pRecordEnd->m_pData[0] = 0x8E;
				pRecordEnd->m_pData[0] = 0x0D;
				pRecordEnd->m_pData[0] = 0x0A;

				m_arRecords.AddTail(pRecordEnd);
			}
		}

		void GenerateEXTH(NSMobi::CStream& oStream, NSEBook::CFormat* pFormat)
		{
			oStream.WriteCStringA("EXTH");
			
			BYTE* pDataLen = oStream.GetCurrentData();
			oStream.WriteDWORD(0);
			oStream.WriteDWORD(0);

			DWORD dwCountRecords = 0;
			if (_T("") != pFormat->m_oMeta.m_strAuthor)
			{
				CStringA strAuthor = ConverToUTF8(pFormat->m_oMeta.m_strAuthor);
				NSMobi::CMobiReader::CEXTHHeader::WriteRec(oStream, 100, strAuthor);
				++dwCountRecords;
			}
			if (_T("") != pFormat->m_oMeta.m_strCreatorProgram)
			{
				CStringA strAuthor = ConverToUTF8(pFormat->m_oMeta.m_strCreatorProgram);
				NSMobi::CMobiReader::CEXTHHeader::WriteRec(oStream, 108, strAuthor);
				++dwCountRecords;
			}
			if (_T("") != pFormat->m_oMeta.m_strPublisher)
			{
				CStringA strAuthor = ConverToUTF8(pFormat->m_oMeta.m_strPublisher);
				NSMobi::CMobiReader::CEXTHHeader::WriteRec(oStream, 101, strAuthor);
				++dwCountRecords;
			}
			if (m_bIsCover)
			{
				NSMobi::CMobiReader::CEXTHHeader::WriteRec(oStream, 201, (DWORD)0);
				++dwCountRecords;
			}
			if (0 != pFormat->m_oMeta.m_listAnnotation.GetCount())
			{
				CString strAnno = _T("");
				POSITION pos = pFormat->m_oMeta.m_listAnnotation.GetHeadPosition();
				while (NULL != pos)
				{
					NSEBook::IEbookItem* pItem = pFormat->m_oMeta.m_listAnnotation.GetNext(pos);
					if (NSEBook::IEbookItem::ebitParagraph == pItem->m_eType)
					{
						strAnno += _T("<p>");
						strAnno += CorrectString2(((NSEBook::CEbookParagraph*)pItem)->GetAllText());
						strAnno += _T("</p>");						
					}
				}
						
				CStringA strAnnoA = ConverToUTF8(strAnno);
				NSMobi::CMobiReader::CEXTHHeader::WriteRec(oStream, 103, strAnnoA);
				++dwCountRecords;
			}
			
			LONG lLen = oStream.GetPosition();
			LONG lPad = 4 - (lLen % 4);

			DWORD dwSize1 = (DWORD)(lLen + 12);
			DWORD dwSize2 = dwCountRecords;

			*pDataLen = (BYTE)(dwSize1 >> 24);
			++pDataLen;
			*pDataLen = (BYTE)(dwSize1 >> 16);
			++pDataLen;
			*pDataLen = (BYTE)(dwSize1 >> 8);
			++pDataLen;
			*pDataLen = (BYTE)dwSize1;
			++pDataLen;

			*pDataLen = (BYTE)(dwSize2 >> 24);
			++pDataLen;
			*pDataLen = (BYTE)(dwSize2 >> 16);
			++pDataLen;
			*pDataLen = (BYTE)(dwSize2 >> 8);
			++pDataLen;
			*pDataLen = (BYTE)dwSize2;
			++pDataLen;

			if (4 != lPad)
				oStream.WriteSet(0, lPad);
		}

		void GenerateTITLE(NSMobi::CStream& oStream, NSEBook::CFormat* pFormat)
		{
			CStringA str = ConverToUTF8(pFormat->m_oMeta.m_strName);
			oStream.WriteCStringA(str);			
		}

		CStringA ConverToUTF8(CString& strXml)
		{
			// расчитываем записи под текст
			int nLength = strXml.GetLength();

			CStringA saStr;
			
			// Encoding Unicode to UTF-8
			WideCharToMultiByte(CP_UTF8, 0, strXml.GetBuffer(), nLength + 1, saStr.GetBuffer(nLength*3 + 1), nLength*3, NULL, NULL);
			saStr.ReleaseBuffer();

			return saStr;
		}

		/////////
		AVSINLINE void WriteImage(NSEBook::CEbookGraphicObject* pImage)
		{
			CString strImage = _T("");
			LONG width = (LONG)(c_dpiBook * pImage->m_rcBounds.GetWidth() / c_dInchToMM);
			LONG height = (LONG)(c_dpiBook * pImage->m_rcBounds.GetHeight() / c_dInchToMM);
			strImage.Format(_T("<img recindex=\"%d\" align=\"baseline\" height=\"%d\" width=\"%d\" alt=\"\"/>"), pImage->m_oInfo.m_lID, 
				height, width);
			m_oWriter.WriteStringC(strImage);

			if (pImage->m_bIsPageBreakAfter)
			{
				 m_oWriter.WriteStringC(_T("<mbp:pagebreak/>"));
			}
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
				 m_oWriter.WriteStringC(_T("<mbp:pagebreak/>"));
			}
		}

		AVSINLINE void WriteSpan(NSEBook::CEbookParagraph::CSpan* pSpan)
		{
			if (-1 != pSpan->m_oImageInfo.m_lID)
			{
				CString strImage = _T("");
				LONG width = (LONG)(c_dpiBook * pSpan->m_dImageWidth / c_dInchToMM);
				LONG height = (LONG)(c_dpiBook * pSpan->m_dImageHeight / c_dInchToMM);
				strImage.Format(_T("<img recindex=\"%d\" align=\"baseline\" height=\"%d\" width=\"%d\" alt=\"\"/>"), pSpan->m_oImageInfo.m_lID, 
					height, width);
				m_oWriter.WriteStringC(strImage);
				return;
			}
			CString strStyle1 = _T("");
			CString strStyle2 = _T("");
			
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
			m_oWriter.WriteTextHTML(pSpan->m_oText);
			m_oWriter.WriteStringC(strStyle2);
		}
		AVSINLINE void WriteTable(NSEBook::CEbookTable* pTable)
		{
			m_oWriter.WriteString(pTable->m_strTableXml);

			if (pTable->m_bIsPageBreakAfter)
			{
				 m_oWriter.WriteStringC(_T("<mbp:pagebreak/>"));
			}
		}
		AVSINLINE void WriteNumbering(NSEBook::CEbookNumbering* pNumbering)
		{
		}
	};
}