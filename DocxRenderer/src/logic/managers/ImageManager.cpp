#include "ImageManager.h"
#include "../DesktopEditor/common/Directory.h"

namespace NSDocxRenderer
{
    void CImageManager::NewDocument()
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
