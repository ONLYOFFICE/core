/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "math.h"
#include "CalculatorCRC32.h"

#include "../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "FontPicker.h"
using namespace NSFontCutter;

#ifndef AVSINLINE
#define AVSINLINE __forceinline
#endif

#ifndef DISABLE_FILE_DOWNLOADER
#include "../../Common/FileDownloader/FileDownloader.h"
#endif 

#include "WMFToImageConverter.h"
#include "../../Common/MediaFormatDefine.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../DesktopEditor/raster/Metafile/MetaFile.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/graphics/Image.h"

#include "../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include "../../HtmlRenderer/include/ASCSVGWriter.h"
#include <list>

namespace NSShapeImageGen
{
	const long c_nMaxImageSize = 2000;

    static bool _CopyFile(std::wstring strExists, std::wstring strNew, LPVOID lpFunc, LPVOID lpData)
	{
        return CDirectory::CopyFile (strExists, strNew);
	}

	enum ImageType
	{
		itJPG	= 0,
		itPNG	= 1,
		itVIF	= 2,
		itWMF	= 3,
		itEMF	= 4,
		itSVG   = 5
	};

	class CImageInfo
	{
	public:
		NSShapeImageGen::ImageType	m_eType;
		LONG						m_lID;
		bool						m_bValid;
		std::wstring				m_sName;

		CImageInfo()
		{
			m_eType		= itJPG;
			m_lID		= -1;
			m_bValid	= true;
			m_sName		= L"image";
		}
		CImageInfo(const CImageInfo& oSrc)
		{
			*this = oSrc;
		}
		CImageInfo& operator=(const CImageInfo& oSrc)
		{
			m_eType		= oSrc.m_eType;
			m_lID		= oSrc.m_lID;
			m_bValid	= oSrc.m_bValid;
			m_sName		= oSrc.m_sName;

			return *this;
		}
		void SetNameModificator(NSShapeImageGen::ImageType eType, int typeAdditionalFile )
		{
			int nRes = 0;
			if(itWMF == eType)			nRes += 1;
			if(itEMF == eType)			nRes += 2;
			if(typeAdditionalFile == 1)	nRes += 4;
			if(typeAdditionalFile == 2)	nRes += 8;
			
			if(0 != nRes)
				m_sName = L"display" + std::to_wstring(nRes) + L"image";
		}

		AVSINLINE std::wstring GetPath2()
		{
			std::wstring _strExt = L"png";
			switch (m_eType)
			{
			case itJPG:
				_strExt = L"jpg";
				break;
			case itWMF:
				_strExt = L"wmf";
				break;
			case itEMF:
				_strExt = L"emf";
				break;
			case itSVG:
				_strExt = L"svg";
				break;
			default:
				break;
			}
			return m_sName + std::to_wstring(m_lID) + L"." + _strExt;
		}
		AVSINLINE std::wstring GetPathWithoutExtension()
		{
			return m_sName + std::to_wstring(m_lID);
		}
	};

	class CImageManager
	{
	public:
		std::map<std::wstring, CImageInfo>	m_mapImagesFile;
		std::map<DWORD, CImageInfo>			m_mapImageData;

		std::vector<void*>					m_listDrawings;
		std::list<CImageInfo>				m_listImages;

		std::wstring						m_strDstMedia;

		LONG								m_lMaxSizeImage;
		LONG								m_lNextIDImage;

		CCalculatorCRC32					m_oCRC;
		LONG								m_lDstFormat;
		NSWMFToImageConverter::CImageExt	m_oImageExt;
		CFontManager*						m_pFontManager;

		CImageManager()
		{
			m_lMaxSizeImage = c_nMaxImageSize;
			m_lNextIDImage	= 0;
			m_lDstFormat	= 0;
			m_pFontManager = NULL;
		}

		AVSINLINE void NewDocument()
		{
			m_strDstMedia	= L"";
			m_lMaxSizeImage = 1200;
			m_lNextIDImage	= 0;

			m_mapImageData.clear();
			m_mapImagesFile.clear();
			m_listImages.clear();
		}

		CImageInfo WriteImage(CBgraFrame& punkImage, double& x, double& y, double& width, double& height)
		{
			CImageInfo info;
			//if (NULL == punkImage)
			//	return info;
			
			if (height < 0)
			{
				FlipY(punkImage);
				height = -height;
				y -= height;
			}
			
            return GenerateImageID(punkImage, (std::max)(1.0, width), (std::max)(1.0, height));
		}
		CImageInfo WriteImage(const std::wstring& strFile, double& x, double& y, double& width, double& height, const std::wstring& strAdditionalFile, int typeAdditionalFile)
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


				CImageInfo oInfo;
				std::map<std::wstring, CImageInfo>::iterator pPair = m_mapImagesFile.find(strFileUrl);
				
				if (pPair != m_mapImagesFile.end())
					return pPair->second;

				std::wstring strDownload;

#ifndef DISABLE_FILE_DOWNLOADER

				CFileDownloader oDownloader(strFileUrl, true);
				if (oDownloader.DownloadSync())
				{
					strDownload = oDownloader.GetFilePath();
				}

#endif

				return GenerateImageID(strDownload, strFileUrl, (std::max)(1.0, width), (std::max)(1.0, height), strAdditionalFile, typeAdditionalFile);


			}
			
			if (strAdditionalFile.empty())
			{
				CImageInfo info;
				CFile oFile;
				if (S_OK != oFile.OpenFile(strFile))
					return info;
				oFile.CloseFile();
			}
			

			if (width < 0 && height < 0)	return GenerateImageID(strFile, L"", -1, -1, strAdditionalFile, typeAdditionalFile);
											return GenerateImageID(strFile, L"", (std::max)(1.0, width), (std::max)(1.0, height), strAdditionalFile, typeAdditionalFile);
		}
		void SetFontManager(CFontManager* pFontManager)
		{
			m_pFontManager = pFontManager;
		}
	protected:
		inline void CopyFile(std::wstring& strFileSrc, std::wstring& strFileDst)
		{
			_CopyFile(strFileSrc, strFileDst, NULL, NULL);
		}

		bool CheckImageSimpleCopy(const std::wstring& strFileSrc, CImageInfo& oInfo)
		{
			CFile oFile;
			HRESULT hr = oFile.OpenFile(strFileSrc);
			if (hr != S_OK)
				return false;

			if (20 > oFile.GetFileSize())
				return false;

			ULONG64 max_size = 3 * 1024 * 1024; // 4 Mb
			if (max_size < oFile.GetFileSize())
				return false;

			BYTE pBuffer[20];
			oFile.ReadFile(pBuffer, 20);

			// jpg
			if ( (0xFF == pBuffer[0]) && (0xD8 == pBuffer[1]) && (0xFF == pBuffer[2]) )
			{
				oInfo.m_eType = itJPG;

				OOX::CPath pathSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + oInfo.GetPath2();
                CDirectory::CopyFile(strFileSrc, pathSaveItem.GetPath());
				return true;
			}

			// png
			if ( (0x89 == pBuffer[0]) && (0x50 == pBuffer[1]) && (0x4E == pBuffer[2]) && (0x47 == pBuffer[3])
				&& (0x0D == pBuffer[4]) && (0x0A == pBuffer[5]) && (0x1A == pBuffer[6]) && (0x0A == pBuffer[7])
				&& (0x00 == pBuffer[8]) && (0x00 == pBuffer[9]) && (0x00 == pBuffer[10]) && (0x0D == pBuffer[11])
				&& (0x49 == pBuffer[12]) && (0x48 == pBuffer[13]) && (0x44 == pBuffer[14]) && (0x52 == pBuffer[15]))
			{
                oInfo.m_eType = itPNG;

                OOX::CPath pathSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + oInfo.GetPath2();
                CDirectory::CopyFile(strFileSrc, pathSaveItem.GetPath());

				return true;
			}
			return false;
        }

		void SaveImage(const std::wstring& strFileSrc, CImageInfo& oInfo, LONG __width, LONG __height)
		{
			if (CheckImageSimpleCopy(strFileSrc, oInfo))
				return;

			CBgraFrame oBgraFrame;
			if (oBgraFrame.OpenFile(strFileSrc) == true)
			{
				SaveImage(oBgraFrame, oInfo, __width, __height);
			}
			else
			{
				//конвертация неудачная - берем оригинальный файл
				OOX::CPath pathOriginal = strFileSrc;

				std::wstring strSaveItem = oInfo.GetPathWithoutExtension();

				strSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + strSaveItem + pathOriginal.GetExtention();

                CDirectory::CopyFile(strFileSrc, strSaveItem);
			}
		}
		void SaveImage(CBgraFrame& oBgraFrame, CImageInfo& oInfo, LONG __width, LONG __height)
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

		CImageInfo GenerateImageID(CBgraFrame& punkData, double dWidth, double dHeight)
		{
			CImageInfo oInfo;

			//if (NULL == punkData)
			//	return oInfo;

			LONG lWidth		= (LONG)(dWidth * 96 / 25.4);
			LONG lHeight	= (LONG)(dHeight * 96 / 25.4);

			BYTE* pBuffer = punkData.get_Data();
			LONG lLen = 4 * punkData.get_Width() * punkData.get_Height();

			DWORD dwSum = m_oCRC.Calc(pBuffer, lLen);

			std::map<DWORD, CImageInfo>::iterator pPair = m_mapImageData.find(dwSum);
			if (m_mapImageData.end() == pPair)
			{
				// нужно добавить
				++m_lNextIDImage;
				
				oInfo.m_lID = m_lNextIDImage;
				SaveImage(punkData, oInfo, lWidth, lHeight);
				
				m_mapImageData.insert(std::pair<DWORD,CImageInfo>(dwSum, oInfo));
				m_listImages.push_back(oInfo);
			}
			else
			{
				oInfo = pPair->second;
			}

			return oInfo;
		}

		CImageInfo GenerateImageID(const std::wstring& strFileName, const std::wstring & strUrl, double dWidth, double dHeight, const std::wstring& strAdditionalFile, int typeAdditionalFile)
		{
			std::wstring sMapKey = strFileName;
			
			if(!strUrl.empty())				sMapKey  = strUrl;
			if(!strAdditionalFile.empty())	sMapKey += strAdditionalFile;
			
			CImageInfo oInfo;
			std::map<std::wstring, CImageInfo>::iterator pPair = m_mapImagesFile.find(sMapKey);

			LONG lWidth		= (LONG)(dWidth * 96 / 25.4);
			LONG lHeight	= (LONG)(dHeight * 96 / 25.4);

			if (m_mapImagesFile.end() == pPair)
			{
				++m_lNextIDImage;
				oInfo.m_lID = m_lNextIDImage;

				LONG lImageType = m_oImageExt.GetImageType(strFileName);
				
				bool bVector = (1 == lImageType || 2 == lImageType);
				
				bool bOle	= !strAdditionalFile.empty() && (typeAdditionalFile == 1);
				bool bMedia = !strAdditionalFile.empty() && (typeAdditionalFile == 2);
				
				if(bVector)
					oInfo.m_eType = (1 == lImageType) ? itWMF : itEMF;

				oInfo.SetNameModificator(oInfo.m_eType, typeAdditionalFile);

				std::wstring strSaveDir		= m_strDstMedia + FILE_SEPARATOR_STR;
				std::wstring strSaveItemWE	= strSaveDir	+ std::wstring(oInfo.GetPathWithoutExtension());

				//copy ole bin or media
				if(bOle || bMedia)
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

					::MetaFile::CMetaFile oMetafile(m_pFontManager->m_pApplication);
					if (oMetafile.LoadFromFile(strFileName.c_str()))
					{
						double x = 0, y = 0, w = 0, h = 0;
						oMetafile.GetBounds(&x, &y, &w, &h);

						double _max = (w >= h) ? w : h;
						double dKoef = 100000.0 / _max;

						int WW = (int)(dKoef * w + 0.5);
						int HH = (int)(dKoef * h + 0.5);

						NSHtmlRenderer::CASCSVGWriter oWriterSVG;
						oWriterSVG.SetFontManager(m_pFontManager);
						oWriterSVG.put_Width(WW);
						oWriterSVG.put_Height(HH);
						oMetafile.DrawOnRenderer(&oWriterSVG, 0, 0, WW, HH);

						bool bIsRaster = true;
						oWriterSVG.IsRaster(&bIsRaster);
						if(bIsRaster)
						{
							//случай растрового wmf/emf
							if (lWidth <= 0 || lHeight <= 0)
							{
								int nMaxPixSize = 1000;
								int nMinPixSize = 10;
								//usually bound of raster wmf from 0 to 1
								if ((nMinPixSize <= w && w <= nMaxPixSize) && (nMinPixSize <= h && h <= nMaxPixSize))
								{
									lWidth = -1;
									lHeight = -1;
								}
								else
								{
									double dKoef = nMaxPixSize / _max;

									lWidth = (LONG)(dKoef * w + 0.5);
									lHeight = (LONG)(dKoef * h + 0.5);
								}
							}

							std::wstring strSaveItem = strSaveItemWE + L".png";
							oMetafile.ConvertToRaster(strSaveItem.c_str(), 4 /*CXIMAGE_FORMAT_PNG*/,  lWidth, lHeight);

							bool bIsSuccess = NSFile::CFileBinary::Exists(strSaveItem);
							if (bIsSuccess)
							{
								oInfo.m_eType = itPNG;

								m_mapImagesFile.insert(std::pair<std::wstring,CImageInfo>(sMapKey, oInfo));
								m_listImages.push_back(oInfo);
								return oInfo;
							}
						}
						else
						{
							oInfo.m_eType = itSVG;

							oWriterSVG.SaveFile(strSaveItemWE + L".svg");
							m_mapImagesFile.insert(std::pair<std::wstring,CImageInfo>(sMapKey, oInfo));
							m_listImages.push_back(oInfo);
							return oInfo;
						}
					}
				}

				SaveImage(strFileName, oInfo, lWidth, lHeight);
			
				m_mapImagesFile.insert(std::pair<std::wstring,CImageInfo>(sMapKey, oInfo));
				m_listImages.push_back(oInfo);
			}
			else
			{
				oInfo = pPair->second;
			}

			return oInfo;
		}

		ImageType GetImageType(CBgraFrame& pFrame)
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
		void FlipY(CBgraFrame& punkImage)
		{
			//if (NULL == punkImage)
			//	return;

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

		void FlipX(CBgraFrame& punkImage)
		{
			//if (NULL == punkImage)
			//	return;

            BYTE* pBuffer	= punkImage.
                    get_Data();
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
	};	
}
