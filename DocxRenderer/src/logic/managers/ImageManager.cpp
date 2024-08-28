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
		Aggplus::CImage image(strFile);
		return GenerateImageID(&image);
	}

	std::shared_ptr<CImageInfo> CImageManager::GenerateImageID(Aggplus::CImage* pImage)
	{
		if (m_pExternalStorage)
			return m_pExternalStorage->GenerateImageID(pImage);

		if (pImage->GetStride() < 0)
			FlipY(pImage);

		int size = pImage->GetStride() * pImage->GetHeight();
		if (size < 0)
			size = -size;

		BYTE* data = pImage->GetData();
		DWORD width = pImage->GetWidth();
		DWORD height = pImage->GetHeight();
		long stride = pImage->GetStride();

		DWORD sum = m_oCRC.Calc(data, size);
		auto find = m_mapImageData.find(sum);
		if (find != m_mapImageData.end())
			return find->second;

		++m_lNextIDImage;
		auto pInfo = std::make_shared<CImageInfo>();
		pInfo->m_nId = m_lNextIDImage;
		pInfo->m_eType = GetImageType(pImage);
		pInfo->m_strFileName = L"image" + std::to_wstring(pInfo->m_nId);
		pInfo->m_strFileName += ((pInfo->m_eType == CImageInfo::itJPG) ? L".jpg" : L".png");

		UINT format = (pInfo->m_eType == CImageInfo::itJPG) ? 3 : 4;
		std::wstring saved_file = m_strDstMedia + L"/" + pInfo->m_strFileName;

		if (width <= m_lMaxSizeImage && height <= m_lMaxSizeImage)
		{
			pImage->SaveFile(saved_file, format);
		}
		else
		{
			CBgraFrame oBgraFrame;
			oBgraFrame.put_Width(width);
			oBgraFrame.put_Height(height);
			oBgraFrame.put_Stride(stride);
			oBgraFrame.put_Data(data);

			if (width > m_lMaxSizeImage || height > m_lMaxSizeImage)
			{
				int lW = 0;
				int lH = 0;
				double dAspect = (double)width / height;

				if (width >= height)
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
				oBgraFrame.Resize(lW, lH, false);
			}
			oBgraFrame.SaveFile(saved_file, format);
		}
		m_mapImageData.insert(std::pair<DWORD, std::shared_ptr<CImageInfo>>(sum, pInfo));
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
}
