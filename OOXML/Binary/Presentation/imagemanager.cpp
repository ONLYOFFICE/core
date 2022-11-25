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

#include "imagemanager.h"

#ifndef DISABLE_FILE_DOWNLOADER
#include "../../../Common/Network/FileTransporter/include/FileTransporter.h"
#endif 

#include <list>
#include "../../../DesktopEditor/graphics/pro/Image.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../DesktopEditor/common/Directory.h"

#include "../../SystemUtility/File.h"
#include "../../SystemUtility/SystemUtility.h"

#include "../../Base/Unit.h"

// как все протестируем - уберем
#define SUPPORT_OLD_SVG_CONVERTATION
#ifdef SUPPORT_OLD_SVG_CONVERTATION
#include "../../../HtmlRenderer/include/ASCSVGWriter.h"
#endif

namespace NSShapeImageGen
{
	const long c_nMaxImageSize = 2000;

	CMediaManager::CMediaManager()
	{
		m_lMaxSizeImage = c_nMaxImageSize;
		m_lNextIDImage	= 0;
		m_lDstFormat	= 0;
		m_pFontManager = NULL;
	}
	CMediaManager::~CMediaManager()
	{
		m_pFontManager = NULL;
	}

	CMediaInfo CMediaManager::WriteImage(CBgraFrame& oImage, double& x, double& y, double& width, double& height)
	{
		CMediaInfo info;
		
		if (height < 0)
		{
			FlipY(oImage);
			height = -height;
			y -= height;
		}
		
		return GenerateImageID(oImage, (std::max)(1.0, width), (std::max)(1.0, height));
	}
	CMediaInfo CMediaManager::WriteImage(const std::wstring& strFile, double& x, double& y, double& width, double& height, const std::wstring& strAdditionalFile, int typeAdditionalFile)
	{
		bool bIsDownload = false;
		int n1 = (int)strFile.find(L"www");
		int n2 = (int)strFile.find(L"http");
		int n3 = (int)strFile.find(L"ftp");
		int n4 = (int)strFile.find(L"https");

		//если nI сранивать не с 0, то будут проблемы
		//потому что в инсталяции мы кладем файлы в /var/www...
		if (0 == n1 || 0 == n2 || 0 == n3 || 0 == n4)
			bIsDownload = true;

		if (bIsDownload)
		{
			std::wstring strFileUrl = strFile;
			
			XmlUtils::replace_all(strFileUrl, L"\\", L"/");
			XmlUtils::replace_all(strFileUrl, L"http:/", L"http://");
			XmlUtils::replace_all(strFileUrl, L"https:/", L"https://");
			XmlUtils::replace_all(strFileUrl, L"ftp:/",	L"ftp://");

			CMediaInfo oInfo;
			std::map<std::wstring, CMediaInfo>::iterator pPair = m_mapMediaFiles.find(strFileUrl);
			
			if (pPair != m_mapMediaFiles.end())
				return pPair->second;

			std::wstring strDownload;

#ifndef DISABLE_FILE_DOWNLOADER
			NSNetwork::NSFileTransport::CFileDownloader oDownloader(strFileUrl, true);
			if (oDownloader.DownloadSync())
			{
				strDownload = oDownloader.GetFilePath();
				
				CImageFileFormatChecker checker;
				if (false == checker.isImageFile(strDownload))
				{
					strDownload.clear();
				}
			}
#endif
			return GenerateImageID(strDownload, strFileUrl, (std::max)(1.0, width), (std::max)(1.0, height), strAdditionalFile, typeAdditionalFile);
		}
		
		if (strAdditionalFile.empty())
		{
			CMediaInfo info;
			NSFile::CFileBinary oFile;
			if (false == oFile.OpenFile(strFile) && std::wstring::npos == strFile.find(L"data:base64,"))
			{
				return info;
			}
			oFile.CloseFile();
		}
		

		if (width < 0 && height < 0)
			return GenerateImageID(strFile, L"", -1, -1, strAdditionalFile, typeAdditionalFile);

		return GenerateImageID(strFile, L"", (std::max)(1.0, width), (std::max)(1.0, height), strAdditionalFile, typeAdditionalFile);
	}
	CMediaInfo CMediaManager::WriteMedia(const std::wstring& strFile)
	{
		bool bIsDownload = false;
		int n1 = (int)strFile.find (L"www");
		int n2 = (int)strFile.find (L"http");
		int n3 = (int)strFile.find (L"ftp");
		int n4 = (int)strFile.find (L"https");

		//если nI сранивать не с 0, то будут проблемы
		//потому что в инсталяции мы кладем файлы в /var/www...
		if (0 == n1 || 0 == n2 || 0 == n3 || 0 == n4)
			bIsDownload = true;

		if (bIsDownload)
		{
			std::wstring strFileUrl = strFile;

			XmlUtils::replace_all(strFileUrl, L"\\",		L"/");
			XmlUtils::replace_all(strFileUrl, L"http:/",	L"http://");
			XmlUtils::replace_all(strFileUrl, L"https:/",	L"https://");
			XmlUtils::replace_all(strFileUrl, L"ftp:/",	L"ftp://");


			CMediaInfo oInfo;
			std::map<std::wstring, CMediaInfo>::iterator pPair = m_mapMediaFiles.find(strFileUrl);

			if (pPair != m_mapMediaFiles.end())
				return pPair->second;

			std::wstring strDownload;

#ifndef DISABLE_FILE_DOWNLOADER
			NSNetwork::NSFileTransport::CFileDownloader oDownloader(strFileUrl, true);
			if (oDownloader.DownloadSync())
			{
				strDownload = oDownloader.GetFilePath();
				
				CImageFileFormatChecker checker;
				if (false == checker.isImageFile(strDownload))
				{
					strDownload.clear();
				}
			}
#endif
			return GenerateMediaID(strDownload, strFileUrl);
		}

		return GenerateMediaID(strFile, L"");
	}
	void CMediaManager::SetFontManager(NSFonts::IFontManager* pFontManager)
	{
		m_pFontManager = pFontManager;
	}

	void CMediaManager::SaveImage(const std::wstring& strFileName, CMediaInfo& oInfo, LONG __width, LONG __height)
	{
		bool result = false;
		CImageFileFormatChecker checker;

		std::wstring sTempUnpacked;
		std::wstring strFileSrc = strFileName;

		COfficeUtils officeUtils(NULL);
		
		bool bIsImage = checker.isImageFile(strFileSrc);
		
		if (false == bIsImage && true == officeUtils.IsArchive(strFileSrc))
		{
			sTempUnpacked = m_strTempMedia + FILE_SEPARATOR_STR + L"zip_unpacked";
			NSDirectory::CreateDirectory(sTempUnpacked);

			if (S_OK == officeUtils.ExtractToDirectory(strFileSrc, sTempUnpacked, NULL, 0))
			{
				std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(sTempUnpacked, false);
				if (1 == arrFiles.size())
				{
					strFileSrc = arrFiles[0];
				}
			}
			else
			{//gzip
				BYTE *pData = NULL;
				DWORD nBytesCount = 0;

				NSFile::CFileBinary file;
				if (file.ReadAllBytes(strFileSrc, &pData, nBytesCount))
				{
					ULONG nBytesUncompress = nBytesCount * 10;
					BYTE* pDataUncompress = new BYTE[nBytesUncompress];
					if (S_OK == officeUtils.Uncompress(pDataUncompress, &nBytesUncompress, pData, nBytesCount))
					{
						std::wstring tempFile = NSFile::CFileBinary::CreateTempFileWithUniqueName(sTempUnpacked, L"img");
						file.CreateFileW(tempFile);
						file.WriteFile(pDataUncompress, nBytesUncompress);
						file.CloseFile();

						strFileSrc = tempFile;
					}
					if (pDataUncompress) delete []pDataUncompress; pDataUncompress = NULL;
					if (pData) delete []pData; pData = NULL;
				}
			}

			bIsImage = checker.isImageFile(strFileSrc);
		}
		if (bIsImage)
		{
			if (checker.eFileType == _CXIMAGE_FORMAT_JPG)
			{
				oInfo.m_eType = itJPG;

				OOX::CPath pathSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + oInfo.GetPath2();
				CDirectory::CopyFile(strFileSrc, pathSaveItem.GetPath());
				
				result = true;
			}
			else if (checker.eFileType == _CXIMAGE_FORMAT_PNG)
			{
				oInfo.m_eType = itPNG;

				OOX::CPath pathSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + oInfo.GetPath2();
				CDirectory::CopyFile(strFileSrc, pathSaveItem.GetPath());

				result = true;
			}
			else if (checker.eFileType == _CXIMAGE_FORMAT_WMF)
			{
				oInfo.m_eType = itWMF;

				OOX::CPath pathSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + oInfo.GetPath2();
				CDirectory::CopyFile(strFileSrc, pathSaveItem.GetPath());

				result = true;
			}
			else if (checker.eFileType == _CXIMAGE_FORMAT_EMF)
			{
				oInfo.m_eType = itEMF;

				OOX::CPath pathSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + oInfo.GetPath2();
				CDirectory::CopyFile(strFileSrc, pathSaveItem.GetPath());

				result = true;
			}
			else
			{
				CBgraFrame oBgraFrame;
				if (oBgraFrame.OpenFile(strFileSrc) == true)
				{
					SaveImage(oBgraFrame, oInfo, __width, __height);

					result = true;
				}
			}
		}

		if (false == result)
		{
			//конвертация неудачная - берем оригинальный файл
			OOX::CPath pathOriginal = strFileSrc;

			//default is itJPG. set itUnknown to use original extension
			oInfo.m_eType = itUnknown;
			oInfo.m_sExt = pathOriginal.GetExtention();

			std::wstring strSaveItem = oInfo.GetPathWithoutExtension();

			strSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + strSaveItem + pathOriginal.GetExtention();

			CDirectory::CopyFile(strFileSrc, strSaveItem);
		}

		if (false == sTempUnpacked.empty())
		{
			NSDirectory::DeleteDirectory(sTempUnpacked);
		}
	}
	void CMediaManager::SaveImage(CBgraFrame& oBgraFrame, CMediaInfo& oInfo, LONG __width, LONG __height)
	{
		LONG lWidth		= oBgraFrame.get_Width();
		LONG lHeight	= oBgraFrame.get_Height();

		oInfo.m_eType = GetImageType(oBgraFrame);

		std::wstring strSaveItem = L"";
		unsigned int nOutputFormat;
		if (itJPG == oInfo.m_eType)
		{
			strSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + oInfo.GetPath2();
			nOutputFormat = _CXIMAGE_FORMAT_JPG;
		}
		else
		{
			oInfo.m_eType = itPNG;
			strSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + oInfo.GetPath2();
			nOutputFormat = _CXIMAGE_FORMAT_PNG;
		}
		OOX::CPath pathSaveItem = strSaveItem;

		LONG lMaxSize = (std::min)((std::max)(lWidth, lHeight), m_lMaxSizeImage);

		if (!((lWidth <= lMaxSize) && (lHeight <= lMaxSize)))
		{
			LONG lW = 0;
			LONG lH = 0;
			double dAspect = (double)lWidth / lHeight;

			if (lWidth >= lHeight)
			{
				lW = lMaxSize;
				lH = (LONG)((double)lW / dAspect);
			}
			else
			{
				lH = lMaxSize;
				lW = (LONG)(dAspect * lH);
			}

			oBgraFrame.Resize(lW, lH);
		}
		oBgraFrame.SaveFile(std::wstring(pathSaveItem.GetPath()), nOutputFormat);
	}

	CMediaInfo CMediaManager::GenerateImageID(CBgraFrame& punkData, double dWidth, double dHeight)
	{
		CMediaInfo oInfo;

		LONG lWidth		= (LONG)(dWidth * 96 / 25.4);
		LONG lHeight	= (LONG)(dHeight * 96 / 25.4);

		BYTE* pBuffer = punkData.get_Data();
		LONG lLen = 4 * punkData.get_Width() * punkData.get_Height();

		DWORD dwSum = m_oCRC.Calc(pBuffer, lLen);

		std::map<DWORD, CMediaInfo>::iterator pPair = m_mapMediaData.find(dwSum);
		if (m_mapMediaData.end() == pPair)
		{
			// нужно добавить
			++m_lNextIDImage;
			
			oInfo.m_lID = m_lNextIDImage;
			SaveImage(punkData, oInfo, lWidth, lHeight);
			
			m_mapMediaData.insert(std::make_pair(dwSum, oInfo));
		}
		else
		{
			oInfo = pPair->second;
		}

		return oInfo;
	}

	CMediaInfo CMediaManager::GenerateImageID(std::wstring strFileName, const std::wstring & strUrl, double dWidth, double dHeight, const std::wstring& strAdditionalFile, int typeAdditionalFile)
	{
		if (0 == strFileName.find(L"data:base64,"))
		{
			int nHeaderSize = 12;
			int nBase64DataSize = (int)strFileName.length() - nHeaderSize;

			int dstLen = NSBase64::Base64DecodeGetRequiredLength(nBase64DataSize);
			BYTE* pDstBuffer = new BYTE[dstLen];
			NSBase64::Base64Decode(strFileName.c_str() + nHeaderSize, nBase64DataSize, pDstBuffer, &dstLen);

			CImageFileFormatChecker checker;
			std::wstring sImageExtension = checker.DetectFormatByData(pDstBuffer, dstLen);
			std::wstring tempFilePath = m_strTempMedia + FILE_SEPARATOR_STR;
			
			strFileName = NSFile::CFileBinary::CreateTempFileWithUniqueName(tempFilePath, L"img") + L"." + sImageExtension;

			NSFile::CFileBinary oTempFile;
			oTempFile.CreateFile(strFileName);
			oTempFile.WriteFile((void*)pDstBuffer, (DWORD)dstLen);
			oTempFile.CloseFile();
			
			RELEASEARRAYOBJECTS(pDstBuffer);
		}

		std::wstring sMapKey = strFileName;
		
		if(!strUrl.empty())				sMapKey  = strUrl;
		if(!strAdditionalFile.empty())	sMapKey += strAdditionalFile;
		
		CMediaInfo oInfo;
		std::map<std::wstring, CMediaInfo>::iterator pPair = m_mapMediaFiles.find(sMapKey);

		LONG lWidth		= (LONG)(dWidth * 96 / 25.4);
		LONG lHeight	= (LONG)(dHeight * 96 / 25.4);

		if (m_mapMediaFiles.end() == pPair)
		{
			++m_lNextIDImage;
			oInfo.m_lID = m_lNextIDImage;

			LONG lImageType = m_oImageExt.GetImageType(strFileName);
			
			bool bVector = (1 == lImageType || 2 == lImageType);
			
			bool bOle	= !strAdditionalFile.empty() && (typeAdditionalFile == 1);
			bool bMedia = !strAdditionalFile.empty() && (typeAdditionalFile == 2);
			
			if (bVector)
				oInfo.m_eType = (1 == lImageType) ? itWMF : itEMF;

			oInfo.SetNameModificator(oInfo.m_eType, typeAdditionalFile);

			std::wstring strSaveDir		= m_strDstMedia + FILE_SEPARATOR_STR;
			std::wstring strSaveItemWE	= strSaveDir	+ oInfo.GetPathWithoutExtension();

			// copy ole bin or media
			if (bOle || bMedia)
			{
				std::wstring strExts;
				int nIndexExt = (int)strAdditionalFile.rfind(wchar_t('.'));
				if (-1 != nIndexExt)
					strExts = strAdditionalFile.substr(nIndexExt);

				if(bOle && strExts.empty()) strExts = L".bin";

				std::wstring sCopyOlePath = strSaveItemWE + strExts;
				CDirectory::CopyFile(strAdditionalFile, sCopyOlePath);
			}

			if (bVector)
			{
				//copy source vector image
				OOX::CPath pathSaveItem = strSaveDir + oInfo.GetPath2();
				CDirectory::CopyFile(strFileName, pathSaveItem.GetPath());

				::MetaFile::IMetaFile* pMetafile = MetaFile::Create(m_pFontManager->GetApplication());
				if (pMetafile->LoadFromFile(strFileName.c_str()))
				{
					// пробуем сохранить в svg напрямую из метафайлов
					std::wstring sInternalSvg = pMetafile->ConvertToSvg();

					if (!sInternalSvg.empty())
					{
						// тут размер не проверяем. сохраняем как есть
						oInfo.m_eType = itSVG;

						NSFile::CFileBinary::SaveToFile(strSaveItemWE + L".svg", sInternalSvg);
						m_mapMediaFiles.insert(std::make_pair(sMapKey, oInfo));

						RELEASEOBJECT(pMetafile);
						return oInfo;
					}

					double x = 0, y = 0, w = 0, h = 0;
					pMetafile->GetBounds(&x, &y, &w, &h);

					// ограничиваем размеры
					int nMaxSize = 1000;
					int nMinSize = 10;
					double dKoef = (double)nMaxSize / ((w >= h) ? w : h);

					int nPixW = (int)(dKoef * w + 0.5);
					int nPixH = (int)(dKoef * h + 0.5);

				#ifdef SUPPORT_OLD_SVG_CONVERTATION
					// пробуем сохранить в svg. большие/сложные файлы
					// сохраняем в растр
					NSHtmlRenderer::CASCSVGWriter oWriterSVG;
					oWriterSVG.SetFontManager(m_pFontManager);
					oWriterSVG.put_Width(nPixW);
					oWriterSVG.put_Height(nPixH);

					bool bRes = true;					
					try
					{
						bRes = pMetafile->DrawOnRenderer(&oWriterSVG, 0, 0, nPixW, nPixH);
					}
					catch (...)
					{
						bRes = false;
					}

					if (bRes)
					{
						bool bIsComplex = false;

						// растровые - сложные
						oWriterSVG.IsRaster(&bIsComplex);

						if (!bIsComplex)
						{
							LONG lSvgDataSize = 0;
							oWriterSVG.GetSVGDataSize(&lSvgDataSize);

							// больше 5 метров - сложные
							bIsComplex = (lSvgDataSize > 5 * 1024 * 1024);
						}

						if (!bIsComplex)
						{
							oInfo.m_eType = itSVG;

							oWriterSVG.SaveFile(strSaveItemWE + L".svg");
							m_mapMediaFiles.insert(std::make_pair(sMapKey, oInfo));

							RELEASEOBJECT(pMetafile);
							return oInfo;
						}
					}
				#endif

					// не смогли (или не захотели? (SUPPORT_OLD_SVG_CONVERTATION)) сконвертировать в svg.
					// пробуем в png
					if (lWidth <= 0 || lHeight <= 0)
					{
						// ограничиваем размеры в растре.
						if ((nMinSize <= w && w <= nMaxSize) && (nMinSize <= h && h <= nMaxSize))
						{
							lWidth = -1;
							lHeight = -1;
						}
						else
						{
							lWidth = nPixW;
							lHeight = nPixH;
						}
					}

					std::wstring strSaveItem = strSaveItemWE + L".png";
					pMetafile->ConvertToRaster(strSaveItem.c_str(), 4 /*CXIMAGE_FORMAT_PNG*/,  lWidth, lHeight);

					if (NSFile::CFileBinary::Exists(strSaveItem))
					{
						oInfo.m_eType = itPNG;

						m_mapMediaFiles.insert(std::make_pair(sMapKey, oInfo));

						RELEASEOBJECT(pMetafile);
						return oInfo;
					}
				}
				RELEASEOBJECT(pMetafile);
			}

			SaveImage(strFileName, oInfo, lWidth, lHeight);
			m_mapMediaFiles.insert(std::make_pair(sMapKey, oInfo));
		}
		else
		{
			oInfo = pPair->second;
		}

		return oInfo;
	}

	CMediaInfo CMediaManager::GenerateMediaID(const std::wstring& strFileName, const std::wstring& strUrl)
	{
		std::wstring sMapKey;
		
		if(!strUrl.empty()) sMapKey = strUrl;
		else				sMapKey = strFileName;
		
		CMediaInfo oInfo;
		std::map<std::wstring, CMediaInfo>::iterator pFind = m_mapMediaFiles.find(sMapKey);

		if (m_mapMediaFiles.end() == pFind)
		{
			++m_lNextIDImage;
			oInfo.m_lID = m_lNextIDImage;

			oInfo.m_eType = itMedia;
			oInfo.m_sName = L"media";

			std::wstring strSaveItemWE	= m_strDstMedia + FILE_SEPARATOR_STR + oInfo.GetPath2();

			int nIndexExt = (int)strFileName.rfind(wchar_t('.'));
			if (-1 != nIndexExt)
				oInfo.m_sExt = strFileName.substr(nIndexExt);

			std::wstring strCopyMediaPath = strSaveItemWE + oInfo.m_sExt;

			CDirectory::CopyFile(strFileName, strCopyMediaPath);

			m_mapMediaFiles.insert(std::make_pair(sMapKey, oInfo));
		}
		else
		{
			oInfo = pFind->second;
		}

		return oInfo;
	}

	MediaType CMediaManager::GetImageType(CBgraFrame& pFrame)
	{
		if (2 == m_lDstFormat)
			return itJPG;

		LONG lWidth		= pFrame.get_Width();
		LONG lHeight	= pFrame.get_Height();
		BYTE* pBuffer	= pFrame.get_Data();

		BYTE* pBufferMem = pBuffer + 3;
		LONG lCountPix = lWidth * lHeight;

		for (LONG i = 0; i < lCountPix; ++i, pBufferMem += 4)
		{
			if (255 != *pBufferMem)
				return itPNG;
		}
		return itJPG;
	}
	void CMediaManager::FlipY(CBgraFrame& punkImage)
	{
		BYTE* pBuffer	= punkImage.get_Data();
		LONG lWidth		= punkImage.get_Width();
		LONG lHeight	= punkImage.get_Height();
		LONG lStride	= punkImage.get_Stride();

		if (lStride < 0)
			lStride = -lStride;
		
		if ((lWidth * 4) != lStride)
			return;

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
	}
	void CMediaManager::FlipX(CBgraFrame& punkImage)
	{
		BYTE* pBuffer	= punkImage.get_Data();
		LONG lWidth		= punkImage.get_Width();
		LONG lHeight	= punkImage.get_Height();
		LONG lStride	= punkImage.get_Stride();

		if (lStride < 0)
			lStride = -lStride;
		
		if ((lWidth * 4) != lStride)
		{
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
	}
}
