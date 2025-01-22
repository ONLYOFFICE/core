#pragma once

#include <map>

#include "../../../../DesktopEditor/common/CalculatorCRC32.h"

#include "ExternalImageStorage.h"

namespace NSDocxRenderer
{
	class CImageManager
	{
	public:
		std::map<DWORD, std::shared_ptr<CImageInfo>> m_mapImageData;
		std::wstring  m_strDstMedia {L""};
		IImageStorage* m_pExternalStorage = nullptr;

		CImageManager() = default;
		~CImageManager() = default;
		void Clear();

		std::shared_ptr<CImageInfo> WriteImage(Aggplus::CImage* pImage, double& x, double& y, double& width, double& height);
		std::shared_ptr<CImageInfo> WriteImage(const std::wstring& strFile, double& x, double& y, double& width, double& height);
		std::shared_ptr<CImageInfo> GenerateImageID(Aggplus::CImage* pImage);
		static CImageInfo::ImageType GetImageType(Aggplus::CImage* pFrame);

	private:
		std::shared_ptr<CImageInfo> GenerateImageID(const std::wstring& strFileName);
		void FlipY(Aggplus::CImage* pImage);

		int m_lMaxSizeImage {1200};
		int m_lNextIDImage {0};
		CCalculatorCRC32 m_oCRC;
	};
}
