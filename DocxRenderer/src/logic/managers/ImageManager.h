#pragma once
#include "../DesktopEditor/common/CalculatorCRC32.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../../resources/ImageInfo.h"
#include <map>
#include <memory>

namespace NSDocxRenderer
{
    class CImageManager
    {
        public:
            std::map<std::wstring, std::shared_ptr<CImageInfo>>	m_mapImagesFile;
            std::map<DWORD, std::shared_ptr<CImageInfo>>        m_mapImageData;

            std::wstring                        m_strDstMedia {L""};

            int                                 m_lMaxSizeImage {1200};
            int                                 m_lNextIDImage {0};

            CCalculatorCRC32                    m_oCRC;

        public:

            CImageManager(){};

            void NewDocument();

        public:
            std::shared_ptr<CImageInfo> WriteImage(Aggplus::CImage* pImage, double& x, double& y, double& width, double& height);

            std::shared_ptr<CImageInfo> WriteImage(const std::wstring& strFile, double& x, double& y, double& width, double& height);

        protected:
            void CopyFile(std::wstring& strFileSrc, std::wstring& strFileDst);

            void SaveImage(const std::wstring& strFileSrc, std::shared_ptr<CImageInfo> pInfo);

            void SaveImage(Aggplus::CImage* pImage, std::shared_ptr<CImageInfo> pInfo);

            std::shared_ptr<CImageInfo> GenerateImageID(Aggplus::CImage* pImage);

            std::shared_ptr<CImageInfo> GenerateImageID(const std::wstring& strFileName);

            CImageInfo::ImageType GetImageType(Aggplus::CImage* pFrame);

            void FlipY(Aggplus::CImage* pImage);

            void FlipX(CBgraFrame* pImage);
    };
}
