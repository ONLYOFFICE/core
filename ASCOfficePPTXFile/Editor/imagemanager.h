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
#include "../../Common/FileDownloader.h"
#endif 

#include "WMFToImageConverter.h"
#include "../../Common/MediaFormatDefine.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../DesktopEditor/raster/Metafile/MetaFile.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/graphics/Image.h"

#include "../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include <list>

namespace NSShapeImageGen
{
	const long c_nMaxImageSize = 2000;

    static bool _CopyFile(CString strExists, CString strNew, LPVOID lpFunc, LPVOID lpData)
	{
        //::DeleteFile(strNew);
        //return ::CopyFileEx(strExists, strNew, lpFunc, lpData, false, 0);
        return CDirectory::CopyFile (strExists, strNew, lpFunc, lpData);
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
		LONG		m_lID;
		bool		m_bValid;
		CString		m_sName;

		CImageInfo()
		{
			m_eType		= itJPG;
			m_lID		= -1;
			m_bValid	= true;
			m_sName		= _T("image");
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
		void SetNameModificator(NSShapeImageGen::ImageType eType, bool bOle)
		{
			CString sPrefix = _T("");
			int nRes = 0;
			if(itWMF == eType)
				nRes += 1;
			if(itEMF == eType)
				nRes += 2;
			if(bOle)
				nRes += 4;
			if(0 != nRes)
				m_sName.Format(_T("display%dimage"), nRes);
		}

		AVSINLINE CString GetPath2()
		{
			CString _strExt = _T("png");
			switch (m_eType)
			{
			case itJPG:
				_strExt = _T("jpg");
				break;
			case itWMF:
				_strExt = _T("wmf");
				break;
			case itEMF:
				_strExt = _T("emf");
				break;
			case itSVG:
				_strExt = _T("svg");
				break;
			default:
				break;
			}

			CString strExt = _T("");
			strExt.Format(_T("%ls%d.%ls"), m_sName, m_lID, _strExt);
			return strExt;
		}
		AVSINLINE CString GetPathWithoutExtension()
		{
			CString strFilename = _T("");
			strFilename.Format(_T("%ls%d"), m_sName, m_lID);
			return strFilename;
		}
	};

	class COleInfo
	{
	public:
		CString m_sRid;
		CString m_sFilename;
		CString m_sOleProperty;
	};

	class CImageManager
	{
	public:
		std::map<CString, CImageInfo>		m_mapImagesFile;
		std::map<DWORD, CImageInfo>			m_mapImageData;

		std::vector<void*>					m_listDrawings;
		std::list<CImageInfo>				m_listImages;

		CString								m_strDstMedia;

		LONG								m_lMaxSizeImage;
		LONG								m_lNextIDImage;

		CCalculatorCRC32					m_oCRC;

		LONG								m_lDstFormat;

#ifdef BUILD_CONFIG_FULL_VERSION
		NSWMFToImageConverter::CImageExt	m_oImageExt;
#endif

		CFontManager* m_pFontManager;
	public:

		CImageManager()
		{
			m_lMaxSizeImage = c_nMaxImageSize;
			m_lNextIDImage	= 0;
			m_lDstFormat	= 0;
			m_pFontManager = NULL;
		}

		AVSINLINE void NewDocument()
		{
			m_strDstMedia	= _T("");
			m_lMaxSizeImage = 1200;
			m_lNextIDImage	= 0;

			m_mapImageData.clear();
			m_mapImagesFile.clear();
			m_listImages.clear();
		}

	public:
		template <typename T>
		void Serialize(T* pWriter)
		{
			pWriter->WriteINT(m_lMaxSizeImage);
			pWriter->WriteINT(m_lNextIDImage);
			pWriter->WriteINT(m_lDstFormat);
			pWriter->WriteString(m_strDstMedia);

			int lCount = (int)m_mapImagesFile.size();
			pWriter->WriteINT(lCount);

			for (std::map<CString, CImageInfo>::iterator pPair = m_mapImagesFile.begin(); pPair != m_mapImagesFile.end(); ++pPair)
			{
				pWriter->WriteString(pPair->first);
				pWriter->WriteINT((int)(pPair->second.m_eType));
				pWriter->WriteINT((int)(pPair->second.m_lID));
				pWriter->WriteBYTE(pPair->second.m_bValid ? 1 : 0);
			}

			lCount = (int)m_mapImageData.size();
			pWriter->WriteINT(lCount);

			for (std::map<DWORD, CImageInfo>::iterator pPair = m_mapImageData.begin(); pPair != m_mapImageData.end(); ++pPair)
			{
				pWriter->WriteULONG(pPair->first);
				pWriter->WriteINT((int)pPair->second.m_eType);
				pWriter->WriteINT((int)pPair->second.m_lID);
				pWriter->WriteBYTE(pPair->second.m_bValid ? 1 : 0);
			}
		}

		template <typename T>
		void Deserialize(T* pReader)
		{
			m_lMaxSizeImage = pReader->GetLong();
			m_lNextIDImage = pReader->GetLong();
			m_lDstFormat = pReader->GetLong();
			m_strDstMedia = pReader->GetString2();

			m_mapImageData.clear();
			m_mapImagesFile.clear();

			LONG lCount = pReader->GetLong();
			for (LONG i = 0; i < lCount; ++i)
			{
				CString sKey = pReader->GetString2();

				CImageInfo oInfo;
				oInfo.m_eType = (NSShapeImageGen::ImageType)pReader->GetLong();
				oInfo.m_lID = pReader->GetLong();
				oInfo.m_bValid = pReader->GetBool();

				m_mapImagesFile.insert(std::pair<CString, CImageInfo>(sKey, oInfo));
			}

			lCount = pReader->GetLong();
			for (LONG i = 0; i < lCount; ++i)
			{
				DWORD dwKey = (DWORD)pReader->GetULong();

				CImageInfo oInfo;
				oInfo.m_eType = (NSShapeImageGen::ImageType)pReader->GetLong();
				oInfo.m_lID = pReader->GetLong();
				oInfo.m_bValid = pReader->GetBool();

				m_mapImageData.insert(std::pair<DWORD, CImageInfo>(dwKey, oInfo));
			}
		}

	public:
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
		CImageInfo WriteImage(const CString& strFile, COleInfo* pOle, double& x, double& y, double& width, double& height)
		{
			bool bIsDownload = false;
			int n1 = strFile.Find(_T("www"));
			int n2 = strFile.Find(_T("http"));
			int n3 = strFile.Find(_T("ftp"));
			int n4 = strFile.Find(_T("https"));

            //если nI сранивать не с 0, то будут проблемы
            //потому что в инсталяции мы кладем файлы в /var/www...
            if (0 == n1 || 0 == n2 || 0 == n3 || 0 == n4)
				bIsDownload = true;

			if (bIsDownload)
			{

				CString strFile1 = strFile;
				strFile1.Replace(_T("\\"), _T("/"));
				strFile1.Replace(_T("http:/"), _T("http://"));
				strFile1.Replace(_T("https:/"), _T("https://"));
				strFile1.Replace(_T("ftp:/"), _T("ftp://"));


				CImageInfo oInfo;
				std::map<CString, CImageInfo>::iterator pPair = m_mapImagesFile.find(strFile1);
				
				if (pPair != m_mapImagesFile.end())
					return pPair->second;

				CString strDownload = _T("");

#ifndef DISABLE_FILE_DOWNLOADER

                CFileDownloader oDownloader(string2std_string(strFile1), true);
				oDownloader.Start( 1 );
				while ( oDownloader.IsRunned() )
				{
					::Sleep( 10 );
				}

				if ( oDownloader.IsFileDownloaded() )
					strDownload = std_string2string(oDownloader.GetFilePath());

#endif

                return GenerateImageID(strDownload, strFile1, pOle, (std::max)(1.0, width), (std::max)(1.0, height));


			}
			
			CImageInfo info;
			CFile oFile;
			if (S_OK != oFile.OpenFile(strFile))
				return info;
			
			oFile.CloseFile();

			if (-1 == width && -1 == height)
				return GenerateImageID(strFile, CString(L""), pOle, width, height);
            return GenerateImageID(strFile, CString(L""), pOle, (std::max)(1.0, width), (std::max)(1.0, height));
		}
		void SetFontManager(CFontManager* pFontManager)
		{
			m_pFontManager = pFontManager;

#ifdef BUILD_CONFIG_FULL_VERSION
			m_oImageExt.SetFontManager(pFontManager);
#endif
		}
	protected:
		inline void CopyFile(CString& strFileSrc, CString& strFileDst)
		{
			_CopyFile(strFileSrc, strFileDst, NULL, NULL);
		}

		bool CheckImageSimpleCopy(const CString& strFileSrc, CImageInfo& oInfo)
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
                CDirectory::CopyFile(strFileSrc, pathSaveItem.GetPath(), NULL, NULL);
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
                CDirectory::CopyFile(strFileSrc, pathSaveItem.GetPath(), NULL, NULL);

				return true;
			}
			return false;
        }

		void SaveImage(const CString& strFileSrc, CImageInfo& oInfo, LONG __width, LONG __height)
		{
			if (CheckImageSimpleCopy(strFileSrc, oInfo))
				return;

			CBgraFrame oBgraFrame;
			if (oBgraFrame.OpenFile(std::wstring(strFileSrc.GetString())) == true)
			{
				SaveImage(oBgraFrame, oInfo, __width, __height);
			}
			else
			{
				//конвертация неудачная - берем оригинальный файл
				OOX::CPath pathOriginal = strFileSrc;

				CString strSaveItem = oInfo.GetPathWithoutExtension();

				strSaveItem =  m_strDstMedia + FILE_SEPARATOR_STR + strSaveItem + pathOriginal.GetExtention();

				CDirectory::CopyFile(strFileSrc, strSaveItem, NULL, NULL);
			}
		}
		void SaveImage(CBgraFrame& oBgraFrame, CImageInfo& oInfo, LONG __width, LONG __height)
		{
			LONG lWidth		= oBgraFrame.get_Width();
			LONG lHeight	= oBgraFrame.get_Height();

			oInfo.m_eType = GetImageType(oBgraFrame);

			CString strSaveItem = _T("");
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

		CImageInfo GenerateImageID(const CString& strFileName, const CString & strUrl, COleInfo* pOle, double dWidth, double dHeight)
		{
			CString sMapKey = strFileName;
			if(!strUrl.IsEmpty())
				sMapKey = strUrl;
			if(NULL != pOle)
				sMapKey += pOle->m_sFilename;
			CImageInfo oInfo;
			std::map<CString, CImageInfo>::iterator pPair = m_mapImagesFile.find(sMapKey);

			LONG lWidth		= (LONG)(dWidth * 96 / 25.4);
			LONG lHeight	= (LONG)(dHeight * 96 / 25.4);

			if (m_mapImagesFile.end() == pPair)
			{
				++m_lNextIDImage;
				oInfo.m_lID = m_lNextIDImage;

				LONG lImageType = m_oImageExt.GetImageType(strFileName);
				bool bVector = (1 == lImageType || 2 == lImageType);
				bool bOle = NULL != pOle;
				if(bVector)
					oInfo.m_eType = (1 == lImageType) ? itWMF : itEMF;
				oInfo.SetNameModificator(oInfo.m_eType, bOle);

				std::wstring strSaveDir		= m_strDstMedia + FILE_SEPARATOR_STR;
				std::wstring strSaveItemWE	= strSaveDir	+ string2std_string(oInfo.GetPathWithoutExtension());

				//copy ole bin
				if(bOle)
				{
					CString sCopyOlePath = std_string2string(strSaveItemWE) + _T(".bin");
					CDirectory::CopyFile(pOle->m_sFilename, sCopyOlePath, NULL, NULL);
					std::wstring sCopyOleProgPath = strSaveItemWE + _T(".txt");
					NSFile::CFileBinary::SaveToFile(sCopyOleProgPath, string2std_string(pOle->m_sOleProperty), false);
				}

				if (bVector)
				{
					//copy source vector image
					OOX::CPath pathSaveItem = std_string2string(strSaveDir) + oInfo.GetPath2();
					CDirectory::CopyFile(strFileName, pathSaveItem.GetPath(), NULL, NULL);

					::MetaFile::CMetaFile oMetafile(m_pFontManager->m_pApplication);
					if (oMetafile.LoadFromFile(strFileName))
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
							//-1 == lHeight имеет спецальное значение(берет размеры из файла)
							if(lWidth <= 0)			lWidth = -1;
							if(lHeight <= 0)		lHeight = -1;

							std::wstring strSaveItem = strSaveItemWE + _T(".png");
							oMetafile.ConvertToRaster(strSaveItem.c_str(), 4 /*CXIMAGE_FORMAT_PNG*/,  lWidth, lHeight);

							bool bIsSuccess = NSFile::CFileBinary::Exists(strSaveItem);
							if (bIsSuccess)
							{
								oInfo.m_eType = itPNG;

								m_mapImagesFile.insert(std::pair<CString,CImageInfo>(sMapKey, oInfo));
								m_listImages.push_back(oInfo);
								return oInfo;
							}
						}
						else
						{
							oInfo.m_eType = itSVG;

							oWriterSVG.SaveFile(strSaveItemWE + _T(".svg"));
							m_mapImagesFile.insert(std::pair<CString,CImageInfo>(sMapKey, oInfo));
							m_listImages.push_back(oInfo);
							return oInfo;
						}
					}
				}

				SaveImage(strFileName, oInfo, lWidth, lHeight);
			
				m_mapImagesFile.insert(std::pair<CString,CImageInfo>(sMapKey, oInfo));
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
