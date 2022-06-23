#pragma once
#include "BaseItem.h"
#include "ImageManager.h"

namespace NSDocxRenderer
{
    class CImage : public CBaseItem
	{
	public:
        std::wstring m_strPath;
        int	m_lID;

		double m_dLeft;
		double m_dTop;
		double m_dWidth;
		double m_dHeight;

		double m_dRotate;

	public:
        CImage();
        CImage(const CImage& oSrc);
        CImage(const CImageInfo& oInfo, const std::wstring& strDstMedia);
        virtual void Clear();

        CImage& operator=(const CImage& oSrc);
        virtual void ToXml(NSStringUtils::CStringBuilder& oWriter);
	};
}
