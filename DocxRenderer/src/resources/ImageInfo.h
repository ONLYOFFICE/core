#pragma once
#include <string>
#include "../DesktopEditor/common/Types.h"

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
            ImageType m_eType {itPNG};
            UINT m_nId {0};
            std::wstring m_strFileName {L""};

        public:
            CImageInfo(){}

            CImageInfo(const CImageInfo &oSrc)
            {
                *this = oSrc;
            }

            CImageInfo& operator=(const CImageInfo &oSrc)
            {
                if (this == &oSrc)
                {
                    return *this;
                }

                m_eType = oSrc.m_eType;
                m_nId = oSrc.m_nId;
                m_strFileName = oSrc.m_strFileName;

                return *this;
            }

    };
}
