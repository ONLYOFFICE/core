#pragma once
#include "../DesktopEditor/common/StringBuilder.h"
#include "../DesktopEditor/common/CalculatorCRC32.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include <map>

namespace NSDocxRenderer
{
    class CImageInfo
    {
        public:
            enum ImageType
            {
                itPNG = 0,
                itJPG = 1
            };

        public:
            ImageType m_eType;
            int m_nId;

            CImageInfo(): m_eType(itPNG), m_nId(0) {}
    };

    class CImageManager
    {
        public:
            std::map<std::wstring, CImageInfo>	m_mapImagesFile;
            std::map<DWORD, CImageInfo>         m_mapImageData;

            std::wstring                        m_strDstMedia;

            int                                 m_lMaxSizeImage;
            int                                 m_lNextIDImage;

            CCalculatorCRC32                    m_oCRC;

        public:

            CImageManager();

            void NewDocument();

        public:
            CImageInfo WriteImage(Aggplus::CImage* pImage, double& x, double& y, double& width, double& height);

            CImageInfo WriteImage(const std::wstring& strFile, double& x, double& y, double& width, double& height);

        protected:
            void CopyFile(std::wstring& strFileSrc, std::wstring& strFileDst);

            void SaveImage(const std::wstring& strFileSrc, CImageInfo& oInfo);

            void SaveImage(Aggplus::CImage* pImage, CImageInfo& oInfo);

            CImageInfo GenerateImageID(Aggplus::CImage* pImage);

            CImageInfo GenerateImageID(const std::wstring& strFileName);

            CImageInfo::ImageType GetImageType(Aggplus::CImage* pFrame);

            void FlipY(Aggplus::CImage* pImage);

            void FlipX(CBgraFrame* pImage);
    };
}
