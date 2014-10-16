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
#include "../../ASCPresentationEditor/PPTXWriter/FileDownloader.h"
#endif 

#include "WMFToImageConverter.h"
#include "../../Common/MediaFormatDefine.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/graphics/Image.h"
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

#include <list>

namespace NSShapeImageGen
{
	const long c_nMaxImageSize = 2000;

	static BOOL _CopyFile(CString strExists, CString strNew, LPPROGRESS_ROUTINE lpFunc, LPVOID lpData) 
	{
        //::DeleteFile(strNew);
        //return ::CopyFileEx(strExists, strNew, lpFunc, lpData, FALSE, 0);
        return CDirectory::CopyFile (strExists, strNew, lpFunc, lpData);
	}

	enum ImageType
	{
		itJPG	= 0,
		itPNG	= 1,
		itVIF	= 2,
		itWMF	= 3,
		itEMF	= 4
	};

	class CImageInfo
	{
	public:
		NSShapeImageGen::ImageType	m_eType;
		LONG		m_lID;
		bool		m_bValid;

		CImageInfo()
		{
			m_eType		= itJPG;
			m_lID		= -1;
			m_bValid	= true;
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

			return *this;
		}

		AVSINLINE CString GetPath(const CString& strMedia)
		{
			CString strExt = _T("");
			strExt.Format(_T("\\image%d.%s"), m_lID, (itJPG == m_eType) ? _T("jpg") : _T("png"));
			return strMedia + strExt;
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
			default:
				break;
			}

			CString strExt = _T("");
			strExt.Format(_T("image%d.%s"), m_lID, _strExt);
			return strExt;
		}
	};

	class CImageManager
	{
	public:
		std::map<CString, CImageInfo>	m_mapImagesFile;
		std::map<DWORD, CImageInfo>		m_mapImageData;

		std::vector<void*>				m_listDrawings;
		std::list<CImageInfo>			m_listImages;

		CString							m_strDstMedia;

		LONG							m_lMaxSizeImage;
		LONG							m_lNextIDImage;

		CCalculatorCRC32				m_oCRC;

		LONG							m_lDstFormat;

#ifdef BUILD_CONFIG_FULL_VERSION
		NSWMFToImageConverter::CImageExt	m_oExt;
#endif

		CFontManager* m_pFontManager;
	public:

		CImageManager()
		{
			m_strDstMedia	= _T("");
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
			
			return GenerateImageID(punkImage, max(1.0, width), max(1.0, height));
		}
        CImageInfo WriteImage(const CString& strFile, double& x, double& y, double& width, double& height)
		{
			bool bIsDownload = false;
			int n1 = strFile.Find(_T("www"));
			int n2 = strFile.Find(_T("http"));
			int n3 = strFile.Find(_T("ftp"));
			int n4 = strFile.Find(_T("https"));

			if (((n1 >= 0) && (n1 < 10)) || ((n2 >= 0) && (n2 < 10)) || ((n3 >= 0) && (n3 < 10)) || ((n4 >= 0) && (n4 < 10)))
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

				CFileDownloader oDownloader(strFile1, TRUE);
				oDownloader.Start( 1 );
				while ( oDownloader.IsRunned() )
				{
					::Sleep( 10 );
				}

				if ( oDownloader.IsFileDownloaded() )
					strDownload = oDownloader.GetFilePath();

#endif

				return GenerateImageID_2(strDownload, strFile1, max(1.0, width), max(1.0, height));


			}
			
			CImageInfo info;
			CFile oFile;
			if (S_OK != oFile.OpenFile(strFile))
				return info;
			
			oFile.CloseFile();

			if (-1 == width && -1 == height)
				return GenerateImageID(strFile, width, height);
			return GenerateImageID(strFile, max(1.0, width), max(1.0, height));
		}
		void SetFontManager(CFontManager* pFontManager)
		{
			m_pFontManager = pFontManager;
			m_oExt.SetFontManager(pFontManager);
		}
	protected:
		inline void CopyFile(CString& strFileSrc, CString& strFileDst)
		{
			_CopyFile(strFileSrc, strFileDst, NULL, NULL);
		}

		bool CheckImageSimpleCopy(CString& strFileSrc, CImageInfo& oInfo)
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
				CString strSaveItem = _T("");
				strSaveItem.Format(_T("\\image%d.jpg"), oInfo.m_lID);
				CDirectory::CopyFile(strFileSrc, m_strDstMedia + strSaveItem , NULL, NULL);
				oInfo.m_eType = itJPG;
				return true;
			}

			// png
			if ( (0x89 == pBuffer[0]) && (0x50 == pBuffer[1]) && (0x4E == pBuffer[2]) && (0x47 == pBuffer[3])
				&& (0x0D == pBuffer[4]) && (0x0A == pBuffer[5]) && (0x1A == pBuffer[6]) && (0x0A == pBuffer[7])
				&& (0x00 == pBuffer[8]) && (0x00 == pBuffer[9]) && (0x00 == pBuffer[10]) && (0x0D == pBuffer[11])
				&& (0x49 == pBuffer[12]) && (0x48 == pBuffer[13]) && (0x44 == pBuffer[14]) && (0x52 == pBuffer[15]))
			{
				CString strSaveItem = _T("");
				strSaveItem.Format(_T("\\image%d.png"), oInfo.m_lID);
				CDirectory::CopyFile(strFileSrc, m_strDstMedia + strSaveItem , NULL, NULL);
				oInfo.m_eType = itPNG;
				return true;
			}
			return false;
		}
		void SaveImage(CString& strFileSrc, CImageInfo& oInfo, LONG __width, LONG __height)
		{
			if (CheckImageSimpleCopy(strFileSrc, oInfo))
				return;

			CBgraFrame oBgraFrame;
			oBgraFrame.OpenFile(std::wstring(strFileSrc.GetString()));

			SaveImage(oBgraFrame, oInfo, __width, __height);
		}
		void SaveImage(CBgraFrame& oBgraFrame, CImageInfo& oInfo, LONG __width, LONG __height)
		{
			LONG lWidth		= oBgraFrame.get_Width();
			LONG lHeight	= oBgraFrame.get_Height();

			oInfo.m_eType = GetImageType(oBgraFrame);

			CString strSaveItem = _T("");
			uint32_t nOutputFormat;
			if (itJPG == oInfo.m_eType)
			{
				strSaveItem.Format(_T("%s\\image%d.jpg"), m_strDstMedia, oInfo.m_lID);
				nOutputFormat = _CXIMAGE_FORMAT_JPG;
			}
			else
			{
				strSaveItem.Format(_T("%s\\image%d.png"), m_strDstMedia, oInfo.m_lID);
				nOutputFormat = _CXIMAGE_FORMAT_PNG;
			}

			LONG lMaxSize = min(max(lWidth, lHeight), m_lMaxSizeImage);

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
			oBgraFrame.SaveFile(std::wstring(strSaveItem.GetString()), nOutputFormat);
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

		CImageInfo GenerateImageID(CString& strFileName, double dWidth, double dHeight)
		{
			CImageInfo oInfo;
			std::map<CString, CImageInfo>::iterator pPair = m_mapImagesFile.find(strFileName);

			LONG lWidth		= (LONG)(dWidth * 96 / 25.4);
			LONG lHeight	= (LONG)(dHeight * 96 / 25.4);

			if (m_mapImagesFile.end() == pPair)
			{
#ifdef BUILD_CONFIG_FULL_VERSION
				LONG lImageType = m_oExt.GetImageType(strFileName);

				if (1 == lImageType || 2 == lImageType)
				{
					++m_lNextIDImage;
					oInfo.m_lID = m_lNextIDImage;
					oInfo.m_eType = (1 == lImageType) ? itWMF : itEMF;

					CString strSaveItem = _T("");
					strSaveItem.Format(_T("\\image%d."), oInfo.m_lID);
					strSaveItem = m_strDstMedia + strSaveItem;

					double dKoef = 100 * 96 / 25.4;
					bool bIsSuccess = m_oExt.Convert(strFileName, LONG(dWidth * dKoef), LONG(dHeight * dKoef), strSaveItem + _T("svg"));
					if (bIsSuccess)
					{
						if (itWMF == oInfo.m_eType)
						{
							CDirectory::CopyFile(strFileName, strSaveItem + _T("wmf"), NULL, NULL);
						}
						else
						{
							CDirectory::CopyFile(strFileName, strSaveItem + _T("emf"), NULL, NULL);
						}

						m_mapImagesFile.insert(std::pair<CString,CImageInfo>(strFileName, oInfo));
						m_listImages.push_back(oInfo);
						return oInfo;
					}
					else
					{
						//случай wmf/emf преобризованного в Bitmap или ошибки
						NSHtmlRenderer::CASCImage oImage;
						oImage.put_FontManager(m_pFontManager);
						oImage.LoadFromFile(std::wstring(strFileName.GetString()));
						Aggplus::CImage* pImage = oImage.get_BitmapImage();
						if(NULL != pImage)
						{
							CBgraFrame oBgraFrame;
							oBgraFrame.put_Width(pImage->GetWidth());
							oBgraFrame.put_Height(pImage->GetHeight());
							oBgraFrame.put_Stride(pImage->GetStride());
							oBgraFrame.put_Data(pImage->GetData());
							SaveImage(oBgraFrame, oInfo, lWidth, lHeight);
							//чтобы в деструкторе не удалялось
							oBgraFrame.put_Data(NULL);
							
							m_mapImagesFile.insert(std::pair<CString,CImageInfo>(strFileName, oInfo));
							m_listImages.push_back(oInfo);
							return oInfo;
						}
						else
						{
							--m_lNextIDImage;
							oInfo.m_eType = itJPG;
						}
					}
				}
#endif

				// нужно добавить
				++m_lNextIDImage;
				
				oInfo.m_lID = m_lNextIDImage;
				SaveImage(strFileName, oInfo, lWidth, lHeight);
			
				m_mapImagesFile.insert(std::pair<CString,CImageInfo>(strFileName, oInfo));
				m_listImages.push_back(oInfo);
			}
			else
			{
				oInfo = pPair->second;
			}

			return oInfo;
		}

		CImageInfo GenerateImageID_2(CString& strFileName, CString& strUrl, double dWidth, double dHeight)
		{
			CImageInfo oInfo;
			LONG lWidth		= (LONG)(dWidth * 96 / 25.4);
			LONG lHeight	= (LONG)(dHeight * 96 / 25.4);

#ifdef BUILD_CONFIG_FULL_VERSION
			LONG lImageType = m_oExt.GetImageType(strFileName);

			if (1 == lImageType || 2 == lImageType)
			{
				++m_lNextIDImage;
				oInfo.m_lID = m_lNextIDImage;
				oInfo.m_eType = (1 == lImageType) ? itWMF : itEMF;

				CString strSaveItem = _T("");
				strSaveItem.Format(_T("\\image%d."), oInfo.m_lID);
				strSaveItem = m_strDstMedia + strSaveItem;

				double dKoef = 100 * 96 / 25.4;
				bool bIsSuccess = m_oExt.Convert(strFileName, LONG(dWidth * dKoef), LONG(dHeight * dKoef), strSaveItem + _T("svg"));
				if (bIsSuccess)
				{
					if (itWMF == oInfo.m_eType)
					{
						CDirectory::CopyFile(strFileName, strSaveItem + _T("wmf"), NULL, NULL);
					}
					else
					{
						CDirectory::CopyFile(strFileName, strSaveItem + _T("emf"), NULL, NULL);
					}
					m_mapImagesFile.insert(std::pair<CString,CImageInfo>(strFileName, oInfo));
					m_listImages.push_back(oInfo);
					return oInfo;
				}
				else
				{
					--m_lNextIDImage;
					oInfo.m_eType = itJPG;
				}
			}
#endif

			// нужно добавить
			++m_lNextIDImage;
			
			oInfo.m_lID = m_lNextIDImage;
			SaveImage(strFileName, oInfo, lWidth, lHeight);
			m_mapImagesFile.insert(std::pair<CString,CImageInfo>(strUrl, oInfo));
			m_listImages.push_back(oInfo);
			
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
	};	
}
