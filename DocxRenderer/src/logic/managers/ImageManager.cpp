#include "ImageManager.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../../resources/Constants.h"

namespace NSDocxRenderer
{
	IImageStorage::IImageStorage(){}
	IImageStorage::~IImageStorage(){}

	class CDataImageStorage : public IImageStorage
	{
	private:
		std::map<DWORD, std::shared_ptr<CImageInfo>> m_mapImageData;
		std::map<int, std::string> m_mapImages;

		int m_lMaxSizeImage{1200};
		int m_lNextIDImage{0};

		CCalculatorCRC32 m_oCRC;

	public:
		CDataImageStorage() : IImageStorage()
		{
		}
		virtual ~CDataImageStorage()
		{
		}

		virtual std::shared_ptr<CImageInfo> GenerateImageID(Aggplus::CImage* pImage)
		{
			BYTE* pData = pImage->GetData();
			DWORD nWidth = pImage->GetWidth();
			DWORD nHeight = pImage->GetHeight();
			long nStride = pImage->GetStride();

			int nSize = pImage->GetStride() * nHeight;
			if (nSize < 0)
				nSize = -nSize;

			DWORD dwSum = m_oCRC.Calc(pData, nSize);

			auto find = m_mapImageData.find(dwSum);
			if (find != m_mapImageData.end())
				return find->second;

			++m_lNextIDImage;

			auto pInfo = std::make_shared<NSDocxRenderer::CImageInfo>();
			pInfo->m_nId = m_lNextIDImage;
			pInfo->m_eType = CImageManager::GetImageType(pImage);
			pInfo->m_strFileName = L"image" + std::to_wstring(pInfo->m_nId) + ((pInfo->m_eType == CImageInfo::itJPG) ? L".jpg" : L".png");

			CBgraFrame oBgraFrame;
			oBgraFrame.put_Width(nWidth);
			oBgraFrame.put_Height(nHeight);
			oBgraFrame.put_Stride(nStride);
			oBgraFrame.put_Data(pData);
			bool bIsResized = false;

			if (nWidth > m_lMaxSizeImage || nHeight > m_lMaxSizeImage)
			{
				int lW = 0;
				int lH = 0;
				double dAspect = (double)nWidth / nHeight;

				if (nWidth >= nHeight)
				{
					lW = m_lMaxSizeImage;
					lH = (int)((double)lW / dAspect);
					if (lH < 1) lH = 1;
				}
				else
				{
					lH = m_lMaxSizeImage;
					lW = (int)(dAspect * lH);
					if (lW < 1) lW = 1;
				}

				bIsResized = true;
				oBgraFrame.Resize(lW, lH, false);
			}

			BYTE* pEncodeBuffer = NULL;
			int nEncodeBufferSize = 0;
			oBgraFrame.Encode(pEncodeBuffer, nEncodeBufferSize, (pInfo->m_eType == CImageInfo::itJPG) ? 3 : 4);

			if (!bIsResized)
				oBgraFrame.put_Data(NULL);

			int nBase64DataSize = NSBase64::Base64EncodeGetRequiredLength(nEncodeBufferSize);
			int nHeaderSize = (pInfo->m_eType == CImageInfo::itPNG) ? 22 : 23;

			char* pBase64Data = new char[nBase64DataSize + nHeaderSize];
			if (pInfo->m_eType == CImageInfo::itPNG)
				memcpy(pBase64Data, "data:image/png;base64,", nHeaderSize);
			else
				memcpy(pBase64Data, "data:image/jpeg;base64,", nHeaderSize);

			NSBase64::Base64Encode(pEncodeBuffer, nEncodeBufferSize, (BYTE*)pBase64Data + nHeaderSize, &nBase64DataSize, NSBase64::B64_BASE64_FLAG_NOCRLF);
			RELEASEARRAYOBJECTS(pEncodeBuffer);

			m_mapImages.insert(std::pair<int, std::string>((int)pInfo->m_nId, std::string(pBase64Data, nHeaderSize + nBase64DataSize)));
			RELEASEARRAYOBJECTS(pBase64Data);

			m_mapImageData.insert(std::pair<DWORD, std::shared_ptr<CImageInfo>>(dwSum, pInfo));
			return pInfo;
		}

		virtual std::string* GetBase64(const int& nRId)
		{
			std::map<int, std::string>::iterator iter = m_mapImages.find(nRId - c_iStartingIdForImages);
			if (iter == m_mapImages.end())
				return NULL;

			return &iter->second;
		}
	};

	IImageStorage* CreateWasmImageStorage()
	{
		return new CDataImageStorage();
	}
}

namespace NSDocxRenderer
{
	void CImageManager::Clear()
	{
		m_strDstMedia	= L"";
		m_lMaxSizeImage = 1200;
		m_lNextIDImage	= 0;

		m_mapImageData.clear();
		m_mapImagesFile.clear();
	}

	std::shared_ptr<CImageInfo> CImageManager::WriteImage(Aggplus::CImage* pImage, double& x, double& y, double& width, double& height)
	{
		if (height < 0)
		{
			FlipY(pImage);
			height = -height;
			y -= height;
		}

		return GenerateImageID(pImage);
	}

	std::shared_ptr<CImageInfo> CImageManager::WriteImage(const std::wstring& strFile, double& x, double& y, double& width, double& height)
	{
		return GenerateImageID(strFile);
	}

	void CImageManager::CopyFile(std::wstring& strFileSrc, std::wstring& strFileDst)
	{
		NSFile::CFileBinary::Copy(strFileSrc, strFileDst);
	}

	void CImageManager::SaveImage(const std::wstring& strFileSrc, std::shared_ptr<CImageInfo> pInfo)
	{
		Aggplus::CImage oFrame(strFileSrc);
		if (nullptr != oFrame.GetData())
			return SaveImage(&oFrame, pInfo);
	}

	void CImageManager::SaveImage(Aggplus::CImage* pImage, std::shared_ptr<CImageInfo> pInfo)
	{
		if (nullptr == pImage)
			return;

		int w = pImage->GetWidth();
		int h = pImage->GetHeight();

		pInfo->m_eType = GetImageType(pImage);

		UINT format = (pInfo->m_eType == CImageInfo::itJPG) ? 3 : 4;
		pInfo->m_strFileName = L"image" + std::to_wstring(pInfo->m_nId);
		pInfo->m_strFileName += ((pInfo->m_eType == CImageInfo::itJPG) ? L".jpg" : L".png");

		std::wstring sSavedFile = m_strDstMedia + L"/" + pInfo->m_strFileName;

		if (w <= m_lMaxSizeImage && h <= m_lMaxSizeImage)
		{
			pImage->SaveFile(sSavedFile, format);
		}
		else
		{
			int lW = 0;
			int lH = 0;
			double dAspect = (double)w / h;

			if (w >= h)
			{
				lW = m_lMaxSizeImage;
				lH = (int)((double)lW / dAspect);
			}
			else
			{
				lH = m_lMaxSizeImage;
				lW = (LONG)(dAspect * lH);
			}

			// TODO: resize
			pImage->SaveFile(sSavedFile, format);
		}
	}

	std::shared_ptr<CImageInfo> CImageManager::GenerateImageID(Aggplus::CImage* pImage)
	{
		if (m_pExternalStorage)
			return m_pExternalStorage->GenerateImageID(pImage);

		if (pImage->GetStride() > 0)
			FlipY(pImage);

		BYTE* pData = pImage->GetData();
		int nSize = pImage->GetStride() * pImage->GetHeight();
		if (nSize < 0)
			nSize = -nSize;

		DWORD dwSum = m_oCRC.Calc(pData, nSize);

		auto find = m_mapImageData.find(dwSum);
		if (find != m_mapImageData.end())
			return find->second;

		++m_lNextIDImage;
		auto pInfo = std::make_shared<CImageInfo>();
		pInfo->m_nId = m_lNextIDImage;
		SaveImage(pImage, pInfo);
		m_mapImageData.insert(std::pair<DWORD, std::shared_ptr<CImageInfo>>(dwSum, pInfo));

		return pInfo;
	}

	std::shared_ptr<CImageInfo> CImageManager::GenerateImageID(const std::wstring& strFileName)
	{
		auto find = m_mapImagesFile.find(strFileName);
		if (find != m_mapImagesFile.end())
			return find->second;

		++m_lNextIDImage;
		auto pInfo = std::make_shared<CImageInfo>();
		pInfo->m_nId = m_lNextIDImage;
		SaveImage(strFileName, pInfo);
		m_mapImagesFile.insert(std::pair<std::wstring, std::shared_ptr<CImageInfo>>(strFileName, pInfo));

		return pInfo;
	}

	CImageInfo::ImageType CImageManager::GetImageType(Aggplus::CImage* pFrame)
	{
		int w = pFrame->GetWidth();
		int h = pFrame->GetHeight();
		BYTE* pBuffer = pFrame->GetData();

		BYTE* pBufferMem = pBuffer + 3;
		LONG lCountPix = w * h;

		for (LONG i = 0; i < lCountPix; ++i, pBufferMem += 4)
		{
			if (255 != *pBufferMem)
				return CImageInfo::itPNG;
		}
		return CImageInfo::itJPG;
	}

	void CImageManager::FlipY(Aggplus::CImage* pImage)
	{
		if (nullptr == pImage)
			return;

		int w = pImage->GetWidth();
		int h = pImage->GetHeight();
		BYTE* pBuffer = pImage->GetData();
		int stride = pImage->GetStride();

		if (stride < 0)
			stride = -stride;

		if ((w * 4) != stride)
			return;

		BYTE* pBufferMem = new BYTE[stride];

		BYTE* pBufferEnd = pBuffer + stride * (h - 1);

		LONG lCountV = h / 2;

		for (LONG lIndexV = 0; lIndexV < lCountV; ++lIndexV)
		{
			memcpy(pBufferMem, pBuffer, stride);
			memcpy(pBuffer, pBufferEnd, stride);
			memcpy(pBufferEnd, pBufferMem, stride);

			pBuffer		+= stride;
			pBufferEnd	-= stride;
		}

		RELEASEARRAYOBJECTS(pBufferMem);
	}

	void CImageManager::FlipX(CBgraFrame* pImage)
	{
		if (nullptr == pImage)
			return;

		int w = pImage->get_Width();
		int h = pImage->get_Height();
		BYTE* pBuffer = pImage->get_Data();
		int stride = pImage->get_Stride();

		if (stride < 0)
			stride = -stride;

		if ((w * 4) != stride)
			return;

		DWORD* pBufferDWORD	= (DWORD*)pBuffer;

		LONG lW2 = w / 2;
		for (LONG lIndexV = 0; lIndexV < h; ++lIndexV)
		{
			DWORD* pMem1 = pBufferDWORD;
			DWORD* pMem2 = pBufferDWORD + w - 1;

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
