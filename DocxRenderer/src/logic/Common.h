#ifndef DOCX_RENDERER_COMMON_H
#define DOCX_RENDERER_COMMON_H

#include "../DesktopEditor/common/StringBuilder.h"
#include "../DesktopEditor/common/StringUTF32.h"
#include "../DesktopEditor/common/CalculatorCRC32.h"
#include "../DesktopEditor/graphics/Matrix.h"
#include "../DesktopEditor/graphics/structures.h"
#include "../DesktopEditor/graphics/TemporaryCS.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include <algorithm>
#include <map>

namespace NSDocxRenderer
{
    inline LONG ConvertColor(LONG lBGR)
	{
		return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
    }

	class CBaseItem
	{
	public:
		enum ElemType
		{
			etParagraph	= 0,
			etImage		= 1,
			etShape		= 2
		};

		ElemType m_eType;

		CBaseItem()
		{
			m_eType = etShape;
		}
        virtual ~CBaseItem() {}

        virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) = 0;
	};

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

        CImageInfo()
        {
            m_eType = itPNG;
            m_nId = 0;
        }
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

		CImageManager()
		{
            m_strDstMedia	= L"";
            m_lMaxSizeImage = 1200;
			m_lNextIDImage	= 0;
		}

        inline void NewDocument()
		{
            m_strDstMedia	= L"";
            m_lMaxSizeImage = 1200;
			m_lNextIDImage	= 0;

            m_mapImageData.clear();
            m_mapImagesFile.clear();
		}

	public:
        CImageInfo WriteImage(Aggplus::CImage* pImage, double& x, double& y, double& width, double& height)
		{
			if (height < 0)
			{
                FlipY(pImage);
				height = -height;
				y -= height;
			}
			
            return GenerateImageID(pImage);
		}
        CImageInfo WriteImage(const std::wstring& strFile, double& x, double& y, double& width, double& height)
		{
			return GenerateImageID(strFile);
		}
	
	protected:
        inline void CopyFile(std::wstring& strFileSrc, std::wstring& strFileDst)
		{
            NSFile::CFileBinary::Copy(strFileSrc, strFileDst);
		}
        void SaveImage(const std::wstring& strFileSrc, CImageInfo& oInfo)
		{
            Aggplus::CImage oFrame(strFileSrc);
            if (NULL != oFrame.GetData())
                return SaveImage(&oFrame, oInfo);
		}
        void SaveImage(Aggplus::CImage* pImage, CImageInfo& oInfo)
		{
            if (NULL == pImage)
				return;

            int w = pImage->GetWidth();
            int h = pImage->GetHeight();

            oInfo.m_eType = GetImageType(pImage);

            int format = (oInfo.m_eType == CImageInfo::itJPG) ? 3 : 4;
            std::wstring sSavedFile = m_strDstMedia + L"/image" + std::to_wstring(oInfo.m_nId);
            sSavedFile += ((oInfo.m_eType == CImageInfo::itJPG) ? L".jpg" : L".png");

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

        CImageInfo GenerateImageID(Aggplus::CImage* pImage)
		{
            BYTE* pData = pImage->GetData();
            int nSize = pImage->GetStride() * pImage->GetHeight();
            if (nSize < 0)
                nSize = -nSize;

            DWORD dwSum = m_oCRC.Calc(pData, nSize);

            std::map<DWORD, CImageInfo>::iterator find = m_mapImageData.find(dwSum);
            if (find != m_mapImageData.end())
                return find->second;

            ++m_lNextIDImage;
            CImageInfo oInfo;
            oInfo.m_nId = m_lNextIDImage;
            SaveImage(pImage, oInfo);
            m_mapImageData.insert(std::pair<DWORD, CImageInfo>(dwSum, oInfo));

            return oInfo;
		}

        CImageInfo GenerateImageID(const std::wstring& strFileName)
		{
            std::map<std::wstring, CImageInfo>::iterator find = m_mapImagesFile.find(strFileName);
            if (find != m_mapImagesFile.end())
                return find->second;

            ++m_lNextIDImage;
            CImageInfo oInfo;
            oInfo.m_nId = m_lNextIDImage;
            SaveImage(strFileName, oInfo);
            m_mapImagesFile.insert(std::pair<std::wstring, CImageInfo>(strFileName, oInfo));

			return oInfo;
		}

        CImageInfo::ImageType GetImageType(Aggplus::CImage* pFrame)
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

        void FlipY(Aggplus::CImage* pImage)
		{
            if (NULL == pImage)
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

        void FlipX(CBgraFrame* pImage)
		{
            if (NULL == pImage)
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

	};
}

namespace NSDocxRenderer
{
	// 2-byte number
    inline short little_endian_2_big_endian( short s )
    {
		return ( ( s >> 8) & 0xff ) + ( ( s << 8 ) & 0xff00 );
    }

    /*========================================================================================================*/

    // 4-byte number
    inline int little_endian_2_big_endian( int i )
    {
		return ( ( i & 0xff ) << 24 ) + ( ( i & 0xff00 ) << 8 ) + ( ( i & 0xff0000 ) >> 8 ) + ( ( i >> 24 ) & 0xff );
    }
}

#endif // DOCX_RENDERER_COMMON_H
