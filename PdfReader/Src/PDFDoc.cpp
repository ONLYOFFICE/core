/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "../../DesktopEditor/common/File.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "StringExt.h"
#include "Constants.h"
#include "GlobalParams.h"
#include "Page.h"
#include "Catalog.h"
#include "Stream.h"
#include "XRef.h"
#include "Link.h"
#include "OutputDevice.h"
#include "ErrorConstants.h"
#include "Lexer.h"
#include "Parser.h"
#include "SecurityHandler.h"
#include "Outline.h"
#include "PDFDoc.h"

#define HeaderSearchSize 1024 // Максимальное количество байт для считывания с начала файла, в которых мы ищем запись '%PDF'

namespace PdfReader
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// PDFDoc
	//-------------------------------------------------------------------------------------------------------------------------------
	PDFDoc::PDFDoc(GlobalParams *pGlobalParams, const std::wstring &wsFileName, const std::wstring &ownerPassword, const std::wstring &userPassword)
	{
		m_pGlobalParams = pGlobalParams;

		m_bValid     = false;
		m_eErrorCode = errorNone;

		m_pFile       = NULL;
		m_pFileBuffer = NULL;

		m_pStream  = NULL;
		m_pXref    = NULL;
		m_pCatalog = NULL;
		m_pOutline = NULL;

		m_wsFileName = wsFileName;

		NSFile::CFileBinary oFile;
		oFile.OpenFile(wsFileName);
		long lFileSize = oFile.GetFileSize();

		m_pFileBuffer = new unsigned char[lFileSize];

		if (!m_pFileBuffer)
		{
			oFile.CloseFile();
			// Не смогли выделить память под весь файл, тогда работаем непосредственно с самим файлом
			m_pFileBuffer = NULL;
			m_pFile = NSFile::CFileBinary::OpenFileNative(wsFileName, L"rb");

			// Создаем поток
			Object oTemp;
			oTemp.InitNull();
			m_pStream = new FileStream(m_pFile, 0, false, 0, &oTemp);
		}
		else
		{
			DWORD lReadedSize;
			DWORD lTotallReadedSize = 0;
			DWORD lCounter = 0;
			while (lTotallReadedSize != lFileSize)
			{
				oFile.ReadFile(m_pFileBuffer + lTotallReadedSize, lFileSize - lTotallReadedSize, lReadedSize);
				lTotallReadedSize += lReadedSize;
				lCounter++;

				if (lCounter > 0x0FFFFFFF)
				{
					lTotallReadedSize = 0;
					break;
				}
			}
			oFile.CloseFile();

			if (lTotallReadedSize != lFileSize)
			{
				m_pFileBuffer = NULL;
				m_bValid = false;
				return;
			}

			// Создаем поток
			Object oTemp;
			oTemp.InitNull();
			m_pStream = new MemoryStream((char*)m_pFileBuffer, 0, lFileSize, &oTemp);
		}

		if (!m_pStream)
			m_bValid = false;

		m_bValid = Setup(ownerPassword, userPassword);
	}
	bool PDFDoc::Setup(const std::wstring &ownerPassword, const std::wstring &userPassword)
	{
		m_pStream->Reset();

		// Проверяем заголовок PDF файла
		CheckHeader();

		// Считываем таблицу Xref
		m_pXref = new XRef(m_pStream);
		if (!m_pXref->CheckValidate())
		{
			// TO DO: Error "Couldn't read xref table"
			m_eErrorCode = (EError)m_pXref->GetErrorCode();
			return false;
		}

		// Проверяем зашифрован ли документ
		if (!CheckEncryption(ownerPassword, userPassword))
		{
			m_eErrorCode = errorEncrypted;
			return false;
		}

		// Считываем объект Catalog
		m_pCatalog = new Catalog(m_pGlobalParams, m_pXref);
		if (!m_pCatalog->CheckValidation())
		{
			// TO DO: Error "Couldn't read page catalog"
			m_eErrorCode = errorBadCatalog;
			return false;
		}

		// Считываем объект Outline
		m_pOutline = new Outline(m_pCatalog->GetOutline(), m_pXref);

		return true;
	}
	PDFDoc::~PDFDoc()
	{
		if (m_pOutline)
		{
			delete m_pOutline;
		}
		if (m_pCatalog)
		{
			delete m_pCatalog;
		}
		if (m_pXref)
		{
			delete m_pXref;
		}
		if (m_pStream)
		{
			delete m_pStream;
		}

		if (m_pFileBuffer)
		{
			delete[] m_pFileBuffer;
		}

		if (m_pFile)
		{
			fclose(m_pFile);
		}
	}
	void PDFDoc::CheckHeader()
	{
		char hdrBuf[HeaderSearchSize + 1];
		char *p;
		int nIndex = 0;

		m_dPDFVersion = 0;

		for (nIndex = 0; nIndex < HeaderSearchSize; ++nIndex)
		{
			hdrBuf[nIndex] = m_pStream->GetChar();
		}
		hdrBuf[HeaderSearchSize] = '\0';

		for (nIndex = 0; nIndex < HeaderSearchSize - 5; ++nIndex)
		{
			if (!strncmp(&hdrBuf[nIndex], "%PDF-", 5))
			{
				break;
			}
		}
		if (nIndex >= HeaderSearchSize - 5)
		{
			// TO DO: Error "May not be a PDF file"
			return;
		}

		m_pStream->SetStartPos(nIndex);

		if (!(p = strtok(&hdrBuf[nIndex + 5], " \t\n\r")))
		{
			// TO DO: Error "May not be a PDF file"
			return;
		}
		m_dPDFVersion = atof(p);

		if (!(hdrBuf[nIndex + 5] >= '0' && hdrBuf[nIndex + 5] <= '9') || m_dPDFVersion > SupportedPDFVersionNum + 0.0001)
		{
			// TO DO: Error "Unsupported PDF version"
		}
	}
	bool PDFDoc::CheckEncryption(const std::wstring &ownerPassword, const std::wstring &userPassword)
	{
		bool bEncrypted = false;
		bool bResult = true;

		Object oEncypt;
		m_pXref->GetTrailerDict()->DictLookup("Encrypt", &oEncypt);

		if ((bEncrypted = oEncypt.IsDict()))
		{
			SecurityHandler *pSecurityHandler = NULL;
			if ((pSecurityHandler = SecurityHandler::Make(this, &oEncypt)))
			{
				if (pSecurityHandler->CheckEncryption(ownerPassword, userPassword))
				{
					// Авторизация пройдена
					m_pXref->SetEncryption(pSecurityHandler->GetPermissionFlags(), pSecurityHandler->GetOwnerPasswordValid(), pSecurityHandler->GetFileKey(), pSecurityHandler->GetFileKeyLength(), pSecurityHandler->GetEncodingVersion(), pSecurityHandler->GetEncodingAlgorithm());
					bResult = true;
				}
				else
				{
					// Авторизация не пройдена
					bResult = false;
				}
				delete pSecurityHandler;
			}
			else
			{
				bResult = false;
			}
		}
		else
		{
			// Данный PDF файл не зашифрован
			bResult = true;
		}
		oEncypt.Free();
		return bResult;
	}
	void PDFDoc::DisplayPage(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, bool(*pAbortCheckCbk)(void *pData), void *pAbortCheckCbkData)
	{
		m_pCatalog->GetPage(nPageIndex)->Display(pOut, dHorDPI, dVerDPI, nRotate, bUseMediaBox, bCrop, bPrinting, m_pCatalog, pAbortCheckCbk, pAbortCheckCbkData);
	}
	void PDFDoc::DisplayPages(OutputDev *pOut, int nFirstPageIndex, int nLastPageIndex, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, bool(*pAbortCheckCbk)(void *pData), void *pAbortCheckCbkData)
	{
		for (int nPageIndex = nFirstPageIndex; nPageIndex <= nLastPageIndex; ++nPageIndex)
		{
			DisplayPage(pOut, nPageIndex, dHorDPI, dVerDPI, nRotate, bUseMediaBox, bCrop, bPrinting, pAbortCheckCbk, pAbortCheckCbkData);
		}
	}
	void PDFDoc::DisplayPageSlice(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, int nSliceX, int nSliceY, int nSliceW, int nSliceH, bool(*pAbortCheckCbk)(void *pData), void *pAbortCheckCbkData)
	{
		m_pCatalog->GetPage(nPageIndex)->DisplaySlice(pOut, dHorDPI, dVerDPI, nRotate, bUseMediaBox, bCrop, nSliceX, nSliceY, nSliceW, nSliceH, bPrinting, m_pCatalog, pAbortCheckCbk, pAbortCheckCbkData);
	}
	Links *PDFDoc::GetLinks(int nPageIndex)
	{
		return m_pCatalog->GetPage(nPageIndex)->GetLinks(m_pCatalog);
	}
	void PDFDoc::ProcessLinks(OutputDev *pOut, int nPageIndex)
	{
		m_pCatalog->GetPage(nPageIndex)->ProcessLinks(pOut, m_pCatalog);
	}
	bool PDFDoc::IsLinearized()
	{
		bool bLinearized = false;
		Object oTemp;
		oTemp.InitNull();
		Parser *pParser = new Parser(m_pXref, new Lexer(m_pXref, m_pStream->MakeSubStream(m_pStream->GetStartPos(), false, 0, &oTemp)), true);
		if (!pParser)
		{
			// TO DO: Error "Can't allocate memory"
			oTemp.Free();
			return false;
		}
		oTemp.Free();

		Object oNum, oGen, oObj, oDict;
		pParser->GetObject(&oNum);
		pParser->GetObject(&oGen);
		pParser->GetObject(&oObj);
		pParser->GetObject(&oDict);

		if (oNum.IsInt() && oGen.IsInt() && oObj.IsCommand("obj") && oDict.IsDict())
		{
			Object oVersion;
			oDict.DictLookup("Linearized", &oVersion);
			if (oVersion.IsNum() && oVersion.GetNum() > 0)
			{
				bLinearized = true;
			}
			oVersion.Free();
		}
		oDict.Free();
		oObj.Free();
		oGen.Free();
		oNum.Free();

		delete pParser;
		return bLinearized;
	}
}