/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#pragma once

#include "../../../DesktopEditor/graphics/MetafileToGraphicsRenderer.h"
#include "../../../DesktopEditor/graphics/Image.h"

#include "../../../DjVuFile/DjVu.h"
#include "../../../DocxRenderer/DocxRenderer.h"
#include "../../../PdfFile/PdfFile.h"
#include "../../../XpsFile/XpsFile.h"
#include "../../../OFDFile/OFDFile.h"
#include "../../../OfficeUtils/src/ZipFolder.h"

#include "common.h"

namespace NExtractTools
{
	_UINT32 bin2pdf(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		NSFonts::IApplicationFonts* pApplicationFonts = createApplicationFonts(params);

		CPdfFile pdfWriter(pApplicationFonts);
		pdfWriter.CreatePdf(params.getIsPDFA());
		pdfWriter.SetTempDirectory(convertParams.m_sTempDir);
		pdfWriter.SetDocumentInfo(params.getTitle(), L"", L"", L"");

		CConvertFromBinParams oBufferParams;
		oBufferParams.m_sThemesDirectory = convertParams.m_sThemesDir;

		std::wstring documentID = params.getDocumentID();
		if (false == documentID.empty())
			pdfWriter.SetDocumentID(documentID);

		std::wstring password = params.getSavePassword();
		if (false == password.empty())
			pdfWriter.SetPassword(password);

		int nReg = (convertParams.m_bIsPaid == false) ? 0 : 1;
		_UINT32 nRet = 0;
		if (params.getIsNoBase64())
		{
			nRet = S_OK == pdfWriter.OnlineWordToPdfFromBinary(sFrom, sTo, &oBufferParams) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			nRet = S_OK == pdfWriter.OnlineWordToPdf(sFrom, sTo, &oBufferParams) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		}
		RELEASEOBJECT(pApplicationFonts);
		return nRet;
	}

	_UINT32 bin2image(unsigned char* pBuffer, long lBufferLen, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		NSFonts::IApplicationFonts* pApplicationFonts = createApplicationFonts(params);
		NSOnlineOfficeBinToPdf::CMetafileToRenderterRaster imageWriter(NULL);

		imageWriter.SetMediaDirectory(convertParams.m_sMediaDirectory);
		imageWriter.SetThemesDirectory(convertParams.m_sThemesDir);
		imageWriter.SetInternalMediaDirectory(convertParams.m_sInternalMediaDirectory);
		imageWriter.SetTempDirectory(convertParams.m_sTempDir);
		imageWriter.SetApplication(pApplicationFonts);

		if (NULL != params.m_oThumbnail)
		{
			InputParamsThumbnail *oThumbnail = params.m_oThumbnail;
			if (NULL != oThumbnail->format)
			{
				imageWriter.SetRasterFormat(*oThumbnail->format);
			}
			if (NULL != oThumbnail->aspect)
			{
				imageWriter.SetSaveType(*oThumbnail->aspect);
			}
			if (NULL != oThumbnail->first)
			{
				imageWriter.SetIsOnlyFirst(*oThumbnail->first);
			}
			if (NULL != oThumbnail->width)
			{
				imageWriter.SetRasterW(*oThumbnail->width);
			}
			if (NULL != oThumbnail->height)
			{
				imageWriter.SetRasterH(*oThumbnail->height);
			}
		}
		std::wstring sThumbnailDir;
		if (imageWriter.GetIsOnlyFirst())
		{
			imageWriter.SetFileName(sTo);
		}
		else
		{
			std::wstring sFileAddon = L"image" + getExtentionByRasterFormat(imageWriter.GetRasterFormat());
			if (NULL == params.m_oThumbnail->zip || *(params.m_oThumbnail->zip))
			{
				sThumbnailDir = combinePath(convertParams.m_sTempDir, L"thumbnails");
				NSDirectory::CreateDirectory(sThumbnailDir);
				imageWriter.SetFileName(combinePath(sThumbnailDir, sFileAddon));
			}
			else
			{
				if (!NSDirectory::Exists(sTo))
					NSDirectory::CreateDirectory(sTo);
				imageWriter.SetFileName(sTo + FILE_SEPARATOR_STR + sFileAddon);
			}
		}
		_UINT32 nRes = imageWriter.ConvertBuffer(pBuffer, lBufferLen) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
		if (!sThumbnailDir.empty())
		{
			COfficeUtils oCOfficeUtils(NULL);
			nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
		}
		RELEASEOBJECT(pApplicationFonts);
		return nRes;
	}
	_UINT32 bin2imageBase64(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(sFrom))
			return AVS_FILEUTILS_ERROR_CONVERT;

		DWORD dwFileSize = oFile.GetFileSize();
		BYTE *pFileContent = new BYTE[dwFileSize];
		if (!pFileContent)
		{
			oFile.CloseFile();
			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		DWORD dwReaded;
		oFile.ReadFile(pFileContent, dwFileSize, dwReaded);
		oFile.CloseFile();

		int nBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
		BYTE *pBuffer = new BYTE[nBufferLen];
		if (!pBuffer)
		{
			RELEASEARRAYOBJECTS(pFileContent);
			return AVS_FILEUTILS_ERROR_CONVERT;
		}

		_UINT32 nRes = 0;
		if (NSBase64::Base64Decode((const char *)pFileContent, dwFileSize, pBuffer, &nBufferLen))
		{
			convertParams.m_sMediaDirectory = NSDirectory::GetFolderPath(sFrom);
			nRes = bin2image(pBuffer, nBufferLen, sTo, params, convertParams);
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}

		RELEASEARRAYOBJECTS(pBuffer);
		RELEASEARRAYOBJECTS(pFileContent);

		return nRes;
	}

	_UINT32 doct_bin2pdf(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType,
						 const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::PDF;

		std::wstring sFileDir         = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = combinePath(sFileDir, L"media");
		std::wstring sPdfBinFile      = combinePath(convertParams.m_sTempDir, L"pdf.bin");

		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
		std::wstring sXml = getDoctXml(eFromType, eToType, sFrom, sPdfBinFile, sImagesDirectory, convertParams.m_sThemesDir, -1, L"", params);
		std::wstring sResult;
		oDoctRenderer.Execute(sXml, sResult);

		_UINT32 nRes = 0;
		if (sResult.find(L"error") != std::wstring::npos)
		{
			std::wcerr << _T("DoctRenderer:") << sResult << std::endl;
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			NSFonts::IApplicationFonts* pApplicationFonts = createApplicationFonts(params);

			CPdfFile pdfWriter(pApplicationFonts);
			pdfWriter.CreatePdf(params.getIsPDFA());
			pdfWriter.SetTempDirectory(convertParams.m_sTempDir);
			pdfWriter.SetDocumentInfo(params.getTitle(), L"", L"", L"");

			CConvertFromBinParams oBufferParams;
			oBufferParams.m_sThemesDirectory        = convertParams.m_sThemesDir;
			oBufferParams.m_sMediaDirectory         = sFileDir;
			oBufferParams.m_sInternalMediaDirectory = convertParams.m_sInternalMediaDirectory;

			std::wstring documentID = params.getDocumentID();
			if (false == documentID.empty())
				pdfWriter.SetDocumentID(documentID);

			std::wstring password = params.getSavePassword();
			if (false == password.empty())
				pdfWriter.SetPassword(password);

			if (!convertParams.m_sPdfOformMetaName.empty() && !convertParams.m_sPdfOformMetaData.empty())
			{
				BYTE* pFileMetaData = NULL;
				DWORD nFileMetaSize = 0;

				if (NSFile::CFileBinary::ReadAllBytes(convertParams.m_sPdfOformMetaData, &pFileMetaData, nFileMetaSize))
				{
					pdfWriter.AddMetaData(convertParams.m_sPdfOformMetaName, pFileMetaData, nFileMetaSize);
				}

				RELEASEARRAYOBJECTS(pFileMetaData);
			}

			int nReg = (convertParams.m_bIsPaid == false) ? 0 : 1;
			nRes = (S_OK == pdfWriter.OnlineWordToPdfFromBinary(sPdfBinFile, sTo, &oBufferParams)) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
			RELEASEOBJECT(pApplicationFonts);
		}
		// удаляем sPdfBinFile, потому что он не в Temp
		if (NSFile::CFileBinary::Exists(sPdfBinFile))
			NSFile::CFileBinary::Remove(sPdfBinFile);
		return nRes;
	}

	_UINT32 doct_bin2image(NSDoctRenderer::DoctRendererFormat::FormatFile eFromType,
						   const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	{
		NSDoctRenderer::DoctRendererFormat::FormatFile eToType = NSDoctRenderer::DoctRendererFormat::FormatFile::IMAGE;

		std::wstring sFileDir         = NSDirectory::GetFolderPath(sFrom);
		std::wstring sImagesDirectory = combinePath(sFileDir, L"media");
		std::wstring sPdfBinFile      = combinePath(convertParams.m_sTempDir, L"pdf.bin");

		NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");
		std::wstring sXml = getDoctXml(eFromType, eToType, sFrom, sPdfBinFile, sImagesDirectory, convertParams.m_sThemesDir, -1, L"", params);
		std::wstring sResult;
		oDoctRenderer.Execute(sXml, sResult);

		_UINT32 nRes = 0;
		if (-1 != sResult.find(_T("error")))
		{
			std::wcerr << _T("DoctRenderer:") << sResult << std::endl;
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
		}
		else
		{
			BYTE *pData = NULL;
			DWORD nBytesCount;
			if (NSFile::CFileBinary::ReadAllBytes(sPdfBinFile, &pData, nBytesCount))
			{
				convertParams.m_sMediaDirectory = sFileDir;
				nRes = 0 == bin2image(pData, nBytesCount, sTo, params, convertParams) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
				RELEASEARRAYOBJECTS(pData);
			}
			else
			{
				nRes = AVS_FILEUTILS_ERROR_CONVERT;
			}
		}
		// delete sPdfBinFile, because it is not in Temp
		if (NSFile::CFileBinary::Exists(sPdfBinFile))
			NSFile::CFileBinary::Remove(sPdfBinFile);
		return nRes;
	}

	// from crossplatform (pdf)
	std::string checkPrintPages(InputParams &params)
	{
		if (NULL == params.m_sJsonParams)
			return "";

		std::wstring::size_type posNativeOptions = params.m_sJsonParams->find(L"\"nativeOptions\"");
		if (std::wstring::npos == posNativeOptions)
			return "";

		std::wstring::size_type posNativePages = params.m_sJsonParams->find(L"\"pages\":\"", posNativeOptions);
		if (std::wstring::npos == posNativePages)
			return "";

		posNativePages += 9;
		std::wstring::size_type posNativePages2 = params.m_sJsonParams->find(L"\"", posNativePages);
		if (std::wstring::npos == posNativePages2)
			return "";

		std::wstring sPages = params.m_sJsonParams->substr(posNativePages, posNativePages2 - posNativePages);
		if (L"all" == sPages)
			return "";

		if (L"current" == sPages)
		{
			std::wstring::size_type posCurrentPage = params.m_sJsonParams->find(L"\"currentPage\":", posNativeOptions);
			if (std::wstring::npos == posCurrentPage)
				return "";

			posCurrentPage += 14;
			std::wstring::size_type posCurrentPage2 = params.m_sJsonParams->find(L",", posCurrentPage);
			std::wstring::size_type posCurrentPage3 = params.m_sJsonParams->find(L"}", posCurrentPage);

			if (std::wstring::npos == posCurrentPage2)
			{
				if (std::wstring::npos == posCurrentPage3)
					return "";
				posCurrentPage2 = posCurrentPage3;
			}
			else if (std::wstring::npos != posCurrentPage3 && posCurrentPage3 < posCurrentPage2)
				posCurrentPage2 = posCurrentPage3;

			if (std::wstring::npos == posCurrentPage2)
				return "";

			sPages = params.m_sJsonParams->substr(posCurrentPage, posCurrentPage2 - posCurrentPage);
		}

		return U_TO_UTF8(sPages);
	}

	std::vector<bool> getPrintPages(const std::string &sPages, int nPagesCount)
	{
		const char *buffer = sPages.c_str();

		size_t nCur = 0;
		size_t nLen = sPages.length();

		std::vector<bool> arPages;
		for (int i = 0; i < nPagesCount; ++i)
			arPages.push_back(false);

		while (nCur < nLen)
		{
			size_t cur = nCur;
			while (cur < nLen && buffer[cur] != ',')
				++cur;

			int nStart = 0;
			int nEnd = 0;

			size_t curRec = nCur;
			while (curRec < cur)
			{
				char c = buffer[curRec++];
				if (c >= '0' && c <= '9')
					nStart = 10 * nStart + (c - '0');

				if (c == '-')
					break;
			}

			if (nStart == 0)
				nStart = 1;

			if (curRec == cur)
				nEnd = nStart;
			else
			{
				while (curRec < cur)
				{
					char c = buffer[curRec++];
					if (c >= '0' && c <= '9')
						nEnd = 10 * nEnd + (c - '0');

					if (c == '-')
						break;
				}

				if (0 == nEnd || nEnd > nPagesCount)
					nEnd = nPagesCount;
			}

			for (int i = nStart; i <= nEnd; ++i)
				arPages[i - 1] = true;

			nCur = cur;
			if (nCur < nLen)
				++nCur;
		}

		return arPages;
	}

	IOfficeDrawingFile* createDrawingFile(NSFonts::IApplicationFonts* pFonts, const int& nFormat)
	{
		switch (nFormat)
		{
		case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
			return new CPdfFile(pFonts);
		case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS:
			return new CXpsFile(pFonts);
		case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU:
			return new CDjVuFile(pFonts);
		case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_OFD:
			return new COFDFile(pFonts);
		default:
			break;
		}
		return NULL;
	}

	_UINT32 PdfDjvuXpsToRenderer(IOfficeDrawingFile** ppReader, IRenderer *pRenderer,
								 const std::wstring& sFrom, int nFormatFrom,
								 const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
								 NSFonts::IApplicationFonts* pApplicationFonts)
	{
		_UINT32 nRes = 0;
		IOfficeDrawingFile* pReader = createDrawingFile(pApplicationFonts, nFormatFrom);
		if (!pReader)
			return AVS_FILEUTILS_ERROR_CONVERT;

		*ppReader = pReader;
		pReader->SetTempDirectory(convertParams.m_sTempDir);

		std::wstring sPassword = params.getPassword();
		bool bResult = pReader->LoadFromFile(sFrom.c_str(), L"", sPassword, sPassword);

		if (bResult)
		{
			int nPagesCount = pReader->GetPagesCount();

			bool bIsUsePages = convertParams.m_sPrintPages.empty() ? false : true;
			std::vector<bool> arPages;
			if (bIsUsePages)
				arPages = getPrintPages(convertParams.m_sPrintPages, nPagesCount);

			for (int i = 0; i < nPagesCount; ++i)
			{
				if (bIsUsePages && !arPages[i])
					continue;

				pRenderer->NewPage();
				pRenderer->BeginCommand(c_nPageType);

				double dPageDpiX, dPageDpiY;
				double dWidth, dHeight;
				pReader->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

				dWidth *= 25.4 / dPageDpiX;
				dHeight *= 25.4 / dPageDpiY;

				pRenderer->put_Width(dWidth);
				pRenderer->put_Height(dHeight);

				pReader->DrawPageOnRenderer(pRenderer, i, NULL);

				pRenderer->EndCommand(c_nPageType);
			}
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
			if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
			{
				CPdfFile *pPdfReader = static_cast<CPdfFile *>(pReader);
				if (PdfFile::errorEncrypted == pPdfReader->GetError())
				{
					if (sPassword.empty())
					{
						if (!params.getDontSaveAdditional())
						{
							copyOrigin(sFrom, *params.m_sFileTo);
						}
						nRes = AVS_FILEUTILS_ERROR_CONVERT_DRM;
					}
					else
					{
						nRes = AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
					}
				}
			}
		}
		return nRes;
	}

	_UINT32 PdfDjvuXpsToImage(IOfficeDrawingFile** ppReader, const std::wstring& sFrom, int nFormatFrom,
							  const std::wstring& sTo, InputParams& params, ConvertParams& convertParams,
							  NSFonts::IApplicationFonts* pApplicationFonts)
	{
		_UINT32 nRes = 0;
		IOfficeDrawingFile* pReader = createDrawingFile(pApplicationFonts, nFormatFrom);
		if (!pReader)
			return AVS_FILEUTILS_ERROR_CONVERT;

		*ppReader = pReader;
		pReader->SetTempDirectory(convertParams.m_sTempDir);

		std::wstring sPassword = params.getPassword();
		bool bResult = pReader->LoadFromFile(sFrom.c_str(), L"", sPassword, sPassword);

		if (bResult)
		{
			// default as in CMetafileToRenderterRaster
			int nRasterFormat = 4;
			int nSaveType = 2;
			bool bIsOnlyFirst = true;
			bool bIsZip = true;
			int nRasterW = 100;
			int nRasterH = 100;
			if (NULL != params.m_oThumbnail)
			{
				InputParamsThumbnail *oThumbnail = params.m_oThumbnail;
				if (NULL != oThumbnail->format)
				{
					nRasterFormat = *oThumbnail->format;
				}
				if (NULL != oThumbnail->aspect)
				{
					nSaveType = *oThumbnail->aspect;
				}
				if (NULL != oThumbnail->first)
				{
					bIsOnlyFirst = *oThumbnail->first;
				}
				if (NULL != oThumbnail->zip)
				{
					bIsZip = *oThumbnail->zip;
				}
				if (NULL != oThumbnail->width)
				{
					nRasterW = *oThumbnail->width;
				}
				if (NULL != oThumbnail->height)
				{
					nRasterH = *oThumbnail->height;
				}
			}
			std::wstring sThumbnailDir;
			std::wstring sFileToExt;
			if (!bIsOnlyFirst)
			{
				if (bIsZip)
				{
					sThumbnailDir = combinePath(convertParams.m_sTempDir, L"thumbnails");
					NSDirectory::CreateDirectory(sThumbnailDir);
				}
				else
				{
					if (!NSDirectory::Exists(sTo))
						NSDirectory::CreateDirectory(sTo);
					sThumbnailDir = sTo;
				}
				sFileToExt = getExtentionByRasterFormat(nRasterFormat);
			}

			int nSaveFlags = (nSaveType & 0xF0) >> 4;
			nSaveType = nSaveType & 0x0F;

			int nPagesCount = pReader->GetPagesCount();
			if (bIsOnlyFirst)
				nPagesCount = 1;
			for (int i = 0; i < nPagesCount; ++i)
			{
				int nRasterWCur = nRasterW;
				int nRasterHCur = nRasterH;

				double dPageDpiX, dPageDpiY;
				double dWidth, dHeight;
				pReader->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

				if (nSaveFlags & 0x0F)
				{
					if (((dWidth < dHeight) && (nRasterWCur > nRasterHCur)) ||
						((dWidth > dHeight) && (nRasterWCur < nRasterHCur)))
					{
						int nTmp = nRasterWCur;
						nRasterWCur = nRasterHCur;
						nRasterHCur = nTmp;
					}
				}

				if (1 == nSaveType)
				{
					double dKoef1 = nRasterWCur / dWidth;
					double dKoef2 = nRasterHCur / dHeight;
					if (dKoef1 > dKoef2)
						dKoef1 = dKoef2;

					nRasterWCur = (int)(dWidth * dKoef1 + 0.5);
					nRasterHCur = (int)(dHeight * dKoef1 + 0.5);
				}
				else if (2 == nSaveType)
				{
					nRasterWCur = -1;
					nRasterHCur = -1;
				}
				std::wstring sFileTo;
				if (bIsOnlyFirst)
				{
					sFileTo = sTo;
				}
				else
				{
					sFileTo = sThumbnailDir + FILE_SEPARATOR_STR + L"image" + std::to_wstring(i + 1) + sFileToExt;
				}
				pReader->ConvertToRaster(i, sFileTo, nRasterFormat, nRasterWCur, nRasterHCur);
			}
			// zip
			if (!bIsOnlyFirst && bIsZip)
			{
				COfficeUtils oCOfficeUtils(NULL);
				nRes = S_OK == oCOfficeUtils.CompressFileOrDirectory(sThumbnailDir, sTo) ? nRes : AVS_FILEUTILS_ERROR_CONVERT;
			}
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT;
			if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom)
			{
				CPdfFile *pPdfReader = static_cast<CPdfFile *>(pReader);
				if (PdfFile::errorEncrypted == pPdfReader->GetError())
				{
					if (sPassword.empty())
					{
						if (!params.getDontSaveAdditional())
						{
							copyOrigin(sFrom, *params.m_sFileTo);
						}
						nRes = AVS_FILEUTILS_ERROR_CONVERT_DRM;
					}
					else
					{
						nRes = AVS_FILEUTILS_ERROR_CONVERT_PASSWORD;
					}
				}
			}
		}
		return nRes;
	}

	bool applyCompiledChangesPdf(CPdfFile* pFile, const std::wstring& sCompiledChangesPath, CConvertFromBinParams& oConvertParams, const std::wstring& sTo)
	{
		bool bRes = false;
		NSFile::CFileBinary oFile;
		if (oFile.OpenFile(sCompiledChangesPath))
		{
			char signature[4];
			if (oFile.ReadFile((BYTE*)signature, 4))
			{
				if ('%' == signature[0] && 'P' == signature[1] && 'D' == signature[2] && 'F' == signature[3])
				{
					DWORD dwChangesSize = (DWORD)(oFile.GetFileSize() - 4);
					BYTE* pChangesData = new BYTE[dwChangesSize];

					if (oFile.ReadFile(pChangesData, dwChangesSize))
					{
						if (!pFile->EditPdf(sTo))
							return false;

						bRes = (S_OK == pFile->AddToPdfFromBinary(pChangesData, (unsigned int)dwChangesSize, &oConvertParams));
						RELEASEARRAYOBJECTS(pChangesData);
					}
				}
			}

			oFile.CloseFile();
		}
		return bRes;
	}

	bool applyChangesPdf(const std::wstring& sFrom, const std::wstring& sTo,
						 NSFonts::IApplicationFonts* pApplicationFonts,
						 InputParams& params, ConvertParams& convertParams,
						 std::vector<std::wstring>& changes,
						 const std::wstring& sResultDirectory)
	{
		CPdfFile oPdfResult(pApplicationFonts);
		oPdfResult.SetTempDirectory(convertParams.m_sTempDir);
		oPdfResult.SetDocumentInfo(params.getTitle(), L"", L"", L"");

		std::wstring documentID = params.getDocumentID();
		if (!documentID.empty())
			oPdfResult.SetDocumentID(documentID);

		std::wstring password = params.getPassword();
		if (!oPdfResult.LoadFromFile(sFrom, L"", password, password))
		{
			if (oPdfResult.GetError() == 4)
			{
				// if password does not changed - old password may be not sended
				password = params.getSavePassword();
				if (!oPdfResult.LoadFromFile(sFrom, L"", password, password))
					return false;

				RELEASEOBJECT(params.m_sPassword);
				params.m_sPassword = new std::wstring(params.getSavePassword());
			}
			else
				return false;
		}

		CConvertFromBinParams oConvertParams;
		oConvertParams.m_sInternalMediaDirectory = NSFile::GetDirectoryName(sFrom);
		oConvertParams.m_sMediaDirectory = oConvertParams.m_sInternalMediaDirectory;

		bool bIsCompiledChanges = false;
		if (changes.size() > 0)
		{
			bIsCompiledChanges = applyCompiledChangesPdf(&oPdfResult, changes[0], oConvertParams, sTo);

			if (!bIsCompiledChanges)
			{
				NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");

				std::wstring sPdfFileCompiledChanges = NSFile::CFileBinary::CreateTempFileWithUniqueName(convertParams.m_sTempDir, L"PDF_");
				if (NSFile::CFileBinary::Exists(sPdfFileCompiledChanges))
					NSFile::CFileBinary::Remove(sPdfFileCompiledChanges);

				std::wstring sXml = getDoctXml(NSDoctRenderer::DoctRendererFormat::PDF,
											   NSDoctRenderer::DoctRendererFormat::PDF,
											   sFrom, sPdfFileCompiledChanges,
											   oConvertParams.m_sInternalMediaDirectory, convertParams.m_sThemesDir,
											   -1, L"", params);

				std::wstring sResult = L"";
				oDoctRenderer.SetAdditionalParam(NSDoctRenderer::AdditionalParamType::DRAWINGFILE, (void*)&oPdfResult);
				oDoctRenderer.Execute(sXml, sResult);

				if (NSFile::CFileBinary::Exists(sPdfFileCompiledChanges))
					bIsCompiledChanges = applyCompiledChangesPdf(&oPdfResult, sPdfFileCompiledChanges, oConvertParams, sTo);
				else
				{
					// changes is exist but compiled file does not changed (compiled changes returns null in js)
					oPdfResult.EditPdf(sTo);
				}
			}
		}

		if (!sResultDirectory.empty() && !params.getDontSaveAdditional())
		{
			//apply and zip changes
			CZipFolderMemory oFolderWithChanges = CZipFolderMemory();
			for (std::vector<std::wstring>::const_iterator i = changes.begin(); i != changes.end(); i++)
			{
				BYTE* pChangesData = NULL;
				DWORD dwChangesSize = 0;
				if (NSFile::CFileBinary::ReadAllBytes(*i, &pChangesData, dwChangesSize))
				{
					//add changes to zip
					std::wstring sFilename = NSSystemPath::GetFileName(*i);
					oFolderWithChanges.write(sFilename, pChangesData, dwChangesSize);
				}
			}
			//add images
			//todo pDoctRenderer->GetImagesInChanges need or not?
			std::wstring sImagesDirectory = combinePath(oConvertParams.m_sMediaDirectory, L"media");
			std::vector<std::wstring> aImages = NSDirectory::GetFiles(sImagesDirectory);
			for (std::vector<std::wstring>::const_iterator i = aImages.begin(); i != aImages.end(); i++)
			{
				BYTE* pImageData = NULL;
				DWORD dwImageSize = 0;
				if (NSFile::CFileBinary::ReadAllBytes(*i, &pImageData, dwImageSize))
				{
					std::wstring sFilename = NSSystemPath::GetFileName(*i);
					oFolderWithChanges.write(sFilename, pImageData, dwImageSize);
				}
			}
			//save changes.zip
			IFolder::CBuffer* pBuffer = oFolderWithChanges.finalize();
			std::wstring sToChanges = combinePath(sResultDirectory, L"changes.zip");
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sToChanges);
			oFile.WriteFile(pBuffer->Buffer, pBuffer->Size);
			oFile.CloseFile();
			RELEASEOBJECT(pBuffer);
		}

		oPdfResult.Close();
		return true;
	}

	_UINT32 fromCrossPlatform(const std::wstring& sFromSrc, int nFormatFrom,
							  const std::wstring& sTo_, int nFormatTo,
							  InputParams& params, ConvertParams& convertParams)
	{
		std::wstring sTo = sTo_;
		_UINT32 nRes = 0;
		NSFonts::IApplicationFonts *pApplicationFonts = createApplicationFonts(params);

		std::wstring sFrom = sFromSrc;
		if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatFrom ||
			AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDFA == nFormatFrom)
		{
			if (params.getFromChanges())
			{
				params.setFromChanges(false);
				std::wstring sChangesDir = NSDirectory::GetFolderPath(sFrom) + FILE_SEPARATOR_STR + L"changes";
				std::vector<std::wstring> arChanges = NSDirectory::GetFiles(sChangesDir);

				sFrom = NSFile::CFileBinary::CreateTempFileWithUniqueName(convertParams.m_sTempDir, L"PDF_");
				if (NSFile::CFileBinary::Exists(sFrom))
					NSFile::CFileBinary::Remove(sFrom);

				std::wstring sResultDirectory = NSDirectory::GetFolderPath(sTo);
				if (!applyChangesPdf(sFromSrc, sFrom, pApplicationFonts, params, convertParams, arChanges, sResultDirectory))
				{
					if (NSFile::CFileBinary::Exists(sFrom))
						NSFile::CFileBinary::Remove(sFrom);

					sFrom = sFromSrc;
				}
			}
		}

		if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
		{
			std::string sPages = checkPrintPages(params);

			if (nFormatFrom == nFormatTo && !params.getIsPDFA())
			{
				if (!sPages.empty())
				{
					std::wstring sCurrentTmp = L"";
					sCurrentTmp =NSFile::CFileBinary::CreateTempFileWithUniqueName(convertParams.m_sTempDir, L"PDF_");
					if (NSFile::CFileBinary::Exists(sCurrentTmp))
						NSFile::CFileBinary::Remove(sCurrentTmp);

					CPdfFile oPdfPages(pApplicationFonts);
					oPdfPages.SetTempDirectory(convertParams.m_sTempDir);

					std::wstring sPassword = params.getPassword();
					if (oPdfPages.LoadFromFile(sFrom.c_str(), L"", sPassword, sPassword) && oPdfPages.EditPdf(sCurrentTmp))
					{
						int nPagesCount = oPdfPages.GetPagesCount();
						std::vector<bool> arPages = getPrintPages(convertParams.m_sPrintPages, nPagesCount);

						for (int i = 0; i < nPagesCount; ++i)
						{
							if (!arPages[i])
								oPdfPages.DeletePage(i);
						}

						oPdfPages.Close();
					}
					else
					{
						sCurrentTmp = L"";
					}

					if (!sCurrentTmp.empty())
					{
						if (sFrom != sFromSrc)
						{
							NSFile::CFileBinary::Remove(sFrom);
						}
						sFrom = sCurrentTmp;
					}
				}

				if (params.getPassword() != params.getSavePassword())
				{
					std::wstring sCurrentTmp = L"";
					sCurrentTmp =NSFile::CFileBinary::CreateTempFileWithUniqueName(convertParams.m_sTempDir, L"PDF_");
					if (NSFile::CFileBinary::Exists(sCurrentTmp))
						NSFile::CFileBinary::Remove(sCurrentTmp);

					CPdfFile oPdfPages(pApplicationFonts);
					oPdfPages.SetTempDirectory(convertParams.m_sTempDir);

					std::wstring sPassword = params.getPassword();
					if (oPdfPages.LoadFromFile(sFrom.c_str(), L"", sPassword, sPassword))
					{
						oPdfPages.ChangePassword(sCurrentTmp, params.getSavePassword());
						oPdfPages.Close();
					}
					else
					{
						sCurrentTmp = L"";
					}

					if (!sCurrentTmp.empty())
					{
						if (sFrom != sFromSrc)
						{
							NSFile::CFileBinary::Remove(sFrom);
						}
						sFrom = sCurrentTmp;
					}
				}

				if ((NULL == params.m_sJsonParams) ||
					(std::wstring::npos == params.m_sJsonParams->find(L"\"watermark\":")))
				{
					if (sFrom == sFromSrc)
					{
						nRes = NSFile::CFileBinary::Copy(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
					}
					else
					{
						nRes = NSFile::CFileBinary::Move(sFrom, sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
					}
				}
				else
				{
					NSDoctRenderer::CDoctrenderer oDoctRenderer(NULL != params.m_sAllFontsPath ? *params.m_sAllFontsPath : L"");

					std::wstring sWatermarkTmp = NSFile::CFileBinary::CreateTempFileWithUniqueName(convertParams.m_sTempDir, L"WWW_");
					if (NSFile::CFileBinary::Exists(sWatermarkTmp))
						NSFile::CFileBinary::Remove(sWatermarkTmp);

					std::wstring sXml = getDoctXml(NSDoctRenderer::DoctRendererFormat::DOCT,
												   NSDoctRenderer::DoctRendererFormat::WATERMARK,
												   L"", sWatermarkTmp, L"", convertParams.m_sThemesDir, -1, L"", params);

					std::wstring sResult = L"";
					oDoctRenderer.Execute(sXml, sResult);

					if (sResult.empty())
					{
						std::wstring password = params.getSavePassword();
						CPdfFile oPdfResult(pApplicationFonts);
						if (!oPdfResult.LoadFromFile(sFrom, L"", password, password))
							return false;

						if (!oPdfResult.EditPdf(sTo))
							return false;

						Aggplus::CImage oImageW(sWatermarkTmp);

						double dW = 0, dH = 0, dDpiX = 0, dDpiY = 0;

						int nPagesCount = oPdfResult.GetPagesCount();
						for (int nPage = 0; nPage < nPagesCount; ++nPage)
						{
							oPdfResult.EditPage(nPage);

							oPdfResult.GetPageInfo(nPage, &dW, &dH, &dDpiX, &dDpiY);

							double dPageW_MM = dW * 25.4 / dDpiX;
							double dPageH_MM = dH * 25.4 / dDpiY;

							double dImageW_MM = 25.4 * oImageW.GetWidth() / dDpiX;
							double dImageH_MM = 25.4 * oImageW.GetHeight() / dDpiY;

							oPdfResult.DrawImage(&oImageW, (dPageW_MM - dImageW_MM) / 2, (dPageH_MM - dImageH_MM) / 2, dImageW_MM, dImageH_MM);
						}

						oPdfResult.Close();
					}

					if (NSFile::CFileBinary::Exists(sWatermarkTmp))
						NSFile::CFileBinary::Remove(sWatermarkTmp);
				}
				sFrom = sFromSrc;
			}
			else
			{
				CPdfFile pdfWriter(pApplicationFonts);
				pdfWriter.CreatePdf(params.getIsPDFA());
				pdfWriter.SetTempDirectory(convertParams.m_sTempDir);
				pdfWriter.SetDocumentInfo(params.getTitle(), L"", L"", L"");

				std::wstring documentID = params.getDocumentID();
				if (false == documentID.empty())
					pdfWriter.SetDocumentID(documentID);

				std::wstring password = params.getSavePassword();
				if (!password.empty())
					pdfWriter.SetPassword(password);

				IOfficeDrawingFile *pReader = NULL;
				convertParams.m_sPrintPages = sPages;
				nRes = PdfDjvuXpsToRenderer(&pReader, &pdfWriter, sFrom, nFormatFrom, sTo, params, convertParams, pApplicationFonts);
				if (SUCCEEDED_X2T(nRes))
					nRes = S_OK == pdfWriter.SaveToFile(sTo) ? 0 : AVS_FILEUTILS_ERROR_CONVERT;
				RELEASEOBJECT(pReader);
			}
		}
		else if (0 != (AVS_OFFICESTUDIO_FILE_CANVAS & nFormatTo) && params.needConvertToOrigin(nFormatFrom))
		{
			//todo remove this code. copy outside x2t
			copyOrigin(sFrom, *params.m_sFileTo);
		}
		else if (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
		{
			IOfficeDrawingFile *pReader = NULL;
			nRes = PdfDjvuXpsToImage(&pReader, sFrom, nFormatFrom, sTo, params, convertParams, pApplicationFonts);
			RELEASEOBJECT(pReader);
		}
		else
		{
			bool bChangeExt = false;
			switch (nFormatTo)
			{
			case AVS_OFFICESTUDIO_FILE_OTHER_OOXML:
				*params.m_nFormatTo = nFormatTo = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX; bChangeExt = true;
				break;
			case AVS_OFFICESTUDIO_FILE_OTHER_ODF:
				*params.m_nFormatTo = nFormatTo = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT; bChangeExt = true;
				break;
			}
			if (bChangeExt)
			{
				size_t nIndex = sTo.rfind('.');
				COfficeFileFormatChecker FileFormatChecker;
				if (-1 != nIndex)
					sTo.replace(nIndex, std::wstring::npos, FileFormatChecker.GetExtensionByType(*params.m_nFormatTo));
				else
					sTo.append(FileFormatChecker.GetExtensionByType(*params.m_nFormatTo));
			}
			IOfficeDrawingFile *pReader = NULL;
			switch (nFormatFrom)
			{
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF:
				pReader = new CPdfFile(pApplicationFonts);
				break;
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS:
				pReader = new CXpsFile(pApplicationFonts);
				break;
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_OFD:
				pReader = new COFDFile(pApplicationFonts);
				break;
			case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU:
				pReader = new CDjVuFile(pApplicationFonts);
				break;
			default:
				break;
			}

			if (pReader)
			{
				pReader->SetTempDirectory(convertParams.m_sTempDir);

				std::wstring sPassword = params.getPassword();
				pReader->LoadFromFile(sFrom, L"", sPassword, sPassword);

				CDocxRenderer oDocxRenderer(pApplicationFonts);

				NSDocxRenderer::TextAssociationType taType = NSDocxRenderer::TextAssociationType::tatPlainLine;
				if (params.m_oTextParams)
				{
					InputParamsText *oTextParams = params.m_oTextParams;
					if (oTextParams->m_nTextAssociationType)
					// taType = static_cast<NSDocxRenderer::TextAssociationType>(*oTextParams->m_nTextAssociationType);
					{
						switch (*oTextParams->m_nTextAssociationType)
						{
						case 0:
							taType = NSDocxRenderer::TextAssociationType::tatBlockChar;
							break;
						case 1:
							taType = NSDocxRenderer::TextAssociationType::tatBlockLine;
							break;
						case 2:
							taType = NSDocxRenderer::TextAssociationType::tatPlainLine;
							break;
						case 3:
							taType = NSDocxRenderer::TextAssociationType::tatPlainParagraph;
							break;
						default:
							break;
						}
					}
				}
				oDocxRenderer.SetTextAssociationType(taType);

				std::wstring sTempDirOut = combinePath(convertParams.m_sTempDir, L"output");
				if (!NSDirectory::Exists(sTempDirOut))
					NSDirectory::CreateDirectory(sTempDirOut);

				std::wstring sTempDirOutTmp = combinePath(convertParams.m_sTempDir, L"output_tmp");
				if (!NSDirectory::Exists(sTempDirOutTmp))
					NSDirectory::CreateDirectory(sTempDirOutTmp);

				oDocxRenderer.SetTempFolder(sTempDirOutTmp);
				nRes = oDocxRenderer.Convert(pReader, sTempDirOut, false);

				if (nRes == S_OK)
					nRes = fromDocxDir(sTempDirOut, sTo, nFormatTo, params, convertParams);
			}
			else
				nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
			RELEASEOBJECT(pReader);
		}
		RELEASEOBJECT(pApplicationFonts);

		if (sFrom != sFromSrc && NSFile::CFileBinary::Exists(sFrom))
			NSFile::CFileBinary::Remove(sFrom);
		return nRes;
	}
	_UINT32 fromCanvasPdf(const std::wstring& sFrom, int nFormatFrom,
						  const std::wstring& sTo, int nFormatTo,
						  InputParams& params, ConvertParams& convertParams)
	{
		_UINT32 nRes = 0;
		if (AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF == nFormatTo)
		{
			nRes = bin2pdf(sFrom, sTo, params, convertParams);
		}
		else if (0 != (AVS_OFFICESTUDIO_FILE_IMAGE & nFormatTo))
		{
			nRes = bin2imageBase64(sFrom, sTo, params, convertParams);
		}
		else
		{
			nRes = AVS_FILEUTILS_ERROR_CONVERT_PARAMS;
		}
		return nRes;
	}
}
