#pragma once
#include "BaseItem.h"
#include "ImageManager.h"

namespace NSDocxRenderer
{
    class CImage : public CBaseItem
	{
	public:
         std::wstring m_strPath {L""};
         int	m_lID {-1};
         double m_dRotate {0.0};

	public:
        CImage();
        CImage(const CImage& oSrc);
        CImage(const CImageInfo& oInfo, const std::wstring& strDstMedia);
        void Clear() override final;

        CImage& operator=(const CImage& oSrc);
        void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;
	};
}
