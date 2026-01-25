#include "OOXMLTags.h"

#include "../../Common/Network/FileTransporter/include/FileTransporter.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"
#include "../../DesktopEditor/common/ProcessEnv.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Path.h"

namespace HTML
{
bool IsSVG(const std::wstring& wsExtention)
{
	return L"svg" == wsExtention || L"svg+xml" == wsExtention;
}

bool NotValidExtension(const std::wstring& sExtention)
{
	return  sExtention != L"bmp" && sExtention != L"emf"  && sExtention != L"emz"  && sExtention != L"eps"  && sExtention != L"fpx" && sExtention != L"gif"  &&
			sExtention != L"jpe" && sExtention != L"jpeg" && sExtention != L"jpg"  && sExtention != L"jfif" && sExtention != L"pct" && sExtention != L"pict" &&
			sExtention != L"png" && sExtention != L"pntg" && sExtention != L"psd"  && sExtention != L"qtif" && sExtention != L"sgi" && sExtention != L"wmz"  &&
			sExtention != L"tga" && sExtention != L"tpic" && sExtention != L"tiff" && sExtention != L"tif"  && sExtention != L"wmf" && !IsSVG(sExtention);
}

bool ReadSVG(const std::wstring& wsSvg, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, const std::wstring& wsImagePath)
{
	if (wsSvg.empty())
		return false;

	MetaFile::IMetaFile* pSvgReader = MetaFile::Create(pFonts);
	if (!pSvgReader->LoadFromString(wsSvg))
	{
		RELEASEINTERFACE(pSvgReader);
		return false;
	}

	NSGraphics::IGraphicsRenderer* pGrRenderer = NSGraphics::Create();
	pGrRenderer->SetFontManager(pSvgReader->get_FontManager());

	double dX, dY, dW, dH;
	pSvgReader->GetBounds(&dX, &dY, &dW, &dH);

	if (dW < 0) dW = -dW;
	if (dH < 0) dH = -dH;

	double dOneMaxSize = (double)1000.;

	if (dW > dH && dW > dOneMaxSize)
	{
		dH *= (dOneMaxSize / dW);
		dW = dOneMaxSize;
	}
	else if (dH > dW && dH > dOneMaxSize)
	{
		dW *= (dOneMaxSize / dH);
		dH = dOneMaxSize;
	}

	int nWidth  = static_cast<int>(dW + 0.5);
	int nHeight = static_cast<int>(dH + 0.5);

	double dWidth  = 25.4 * nWidth / 96;
	double dHeight = 25.4 * nHeight / 96;

	BYTE* pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
	if (!pBgraData)
	{
		double dKoef = 2000.0 / (nWidth > nHeight ? nWidth : nHeight);

		nWidth = (int)(dKoef * nWidth);
		nHeight = (int)(dKoef * nHeight);

		dWidth  = 25.4 * nWidth / 96;
		dHeight = 25.4 * nHeight / 96;

		pBgraData = (BYTE*)malloc(nWidth * nHeight * 4);
	}

	if (!pBgraData)
		return false;

	unsigned int alfa = 0xffffff;
	//дефолтный тон должен быть прозрачным, а не белым
	//memset(pBgraData, 0xff, nWidth * nHeight * 4);
	for (int i = 0; i < nWidth * nHeight; i++)
	{
		((unsigned int*)pBgraData)[i] = alfa;
	}
	CBgraFrame oFrame;
	oFrame.put_Data(pBgraData);
	oFrame.put_Width(nWidth);
	oFrame.put_Height(nHeight);
	oFrame.put_Stride(-4 * nWidth);

	pGrRenderer->CreateFromBgraFrame(&oFrame);
	pGrRenderer->SetSwapRGB(false);
	pGrRenderer->put_Width(dWidth);
	pGrRenderer->put_Height(dHeight);

	// TODO: src directory as tmp - it's not good idea
	pSvgReader->SetTempDirectory(wsTempDir);
	pSvgReader->DrawOnRenderer(pGrRenderer, 0, 0, dWidth, dHeight);

	oFrame.SaveFile(wsImagePath + L".png", 4);
	oFrame.put_Data(NULL);

	RELEASEINTERFACE(pGrRenderer);

	if (pBgraData)
		free(pBgraData);

	RELEASEINTERFACE(pSvgReader);

	return true;
}

bool ReadBase64(const std::wstring& wsSrc, const std::wstring& wsImagePath, NSFonts::IApplicationFonts* pFonts, const std::wstring& wsTempDir, std::wstring& wsExtention)
{
	bool bRes = false;
	size_t nBase = wsSrc.find(L"/", 4);
	nBase++;

	size_t nEndBase = wsSrc.find(L";", nBase);
	if (nEndBase == std::wstring::npos)
		return false;

	wsExtention = wsSrc.substr(nBase, nEndBase - nBase);

	if (wsExtention == L"octet-stream")
		wsExtention = L"jpg";

	if (NotValidExtension(wsExtention))
		return false;

	nBase = wsSrc.find(L"base64", nEndBase);
	if (nBase == std::wstring::npos)
		return false;

	int nOffset = nBase + 7;
	int nSrcLen = (int)(wsSrc.length() - nBase + 1);
	int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
	if (nDecodeLen != 0)
	{
		BYTE* pImageData = new BYTE[nDecodeLen];

		if (!pImageData || FALSE == NSBase64::Base64Decode(wsSrc.c_str() + nOffset, nSrcLen, pImageData, &nDecodeLen))
			return bRes;

		if (IsSVG(wsExtention))
		{
			std::wstring wsSvg(pImageData, pImageData + nDecodeLen);
			bRes = ReadSVG(wsSvg, pFonts, wsTempDir, wsImagePath);
			wsExtention = L"png";
		}
		else
		{
			NSFile::CFileBinary oImageWriter;

			if (oImageWriter.CreateFileW(wsImagePath + L'.' + wsExtention))
				bRes = oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);

			oImageWriter.CloseFile();
		}

		RELEASEARRAYOBJECTS(pImageData);
	}

	return bRes;
}

bool GetStatusUsingExternalLocalFiles()
{
	if (NSProcessEnv::IsPresent(NSProcessEnv::Converter::gc_allowPrivateIP))
		return NSProcessEnv::GetBoolValue(NSProcessEnv::Converter::gc_allowPrivateIP);

	return true;
}

bool CanUseThisPath(const std::wstring& wsPath, const std::wstring& wsSrcPath, const std::wstring& wsCorePath, bool bIsAllowExternalLocalFiles)
{
	if (bIsAllowExternalLocalFiles)
		return true;

	if (!wsCorePath.empty())
	{
		const std::wstring wsFullPath = NSSystemPath::ShortenPath(NSSystemPath::Combine(wsSrcPath, wsPath));

		std::wstring::const_iterator itFirst{wsFullPath.cbegin()};
		std::wstring::const_iterator itSecond{wsCorePath.cbegin()};

		for (;itFirst != wsFullPath.cend() && itSecond != wsCorePath.cend(); ++itFirst, ++itSecond)
		{
			if (*itFirst != *itSecond)
				return false;
		}

		return true;
	}

	if (wsPath.length() >= 3 && L"../" == wsPath.substr(0, 3))
		return false;

	return true;
}

std::wstring NormalizePath(const std::wstring& wsPath)
{
	return NSSystemPath::ShortenPath(wsPath);
}

std::wstring GetFileExtention(const std::wstring& wsFilePath)
{
	return NSFile::GetFileExtention(wsFilePath);
}

std::wstring GetFileName(const std::wstring& wsFilePath)
{
	return NSFile::GetFileName(wsFilePath);
}

bool DownloadImage(const std::wstring& wsHref, const std::wstring& wsDst)
{
	NSNetwork::NSFileTransport::CFileDownloader oDownloadImg(wsHref, false);
	oDownloadImg.SetFilePath(wsDst);
	return oDownloadImg.DownloadSync();
}

bool ReadAllTextUtf8(const std::wstring& wsFilePath, std::wstring& wsText)
{
	return NSFile::CFileBinary::ReadAllTextUtf8(wsFilePath, wsText);
}

bool RemoveFile(const std::wstring& wsFilePath)
{
	return NSFile::CFileBinary::Remove(wsFilePath);
}

std::wstring CombinePaths(const std::wstring& wsFirstPath, const std::wstring& wsSecondPath)
{
	return NSSystemPath::Combine(wsFirstPath, wsSecondPath);
}

bool CopyImage(std::wstring wsImageSrc, const std::wstring& wsSrc, const std::wstring& wsDst, bool bIsAllowExternalLocalFiles)
{
	bool bRes = false;
	bool bAllow = true;

	if (!bIsAllowExternalLocalFiles)
	{
		wsImageSrc = NSSystemPath::NormalizePath(wsImageSrc);
		std::wstring wsStartSrc = NSSystemPath::NormalizePath(wsSrc);
		bAllow = wsImageSrc.substr(0, wsStartSrc.length()) == wsStartSrc;
	}
	if (bAllow)
		bRes = NSFile::CFileBinary::Copy(wsImageSrc, wsDst);

	return bRes;
}

bool UpdateImageData(const std::wstring& wsImagePath, TImageData& oImageData)
{
	CBgraFrame oBgraFrame;
	if (!oBgraFrame.OpenFile(wsImagePath))
	{
		NSFile::CFileBinary::Remove(wsImagePath);
		return false;
	}

	if (0 != oImageData.m_unWidth || 0 != oImageData.m_unHeight)
	{
		const double dMaxScale = std::max(oImageData.m_unWidth  / oBgraFrame.get_Width(),
		                                  oImageData.m_unHeight / oBgraFrame.get_Height());

		oImageData.m_unWidth  = oBgraFrame.get_Width()  * dMaxScale;
		oImageData.m_unHeight = oBgraFrame.get_Height() * dMaxScale;
	}
	else
	{
		oImageData.m_unWidth  = oBgraFrame.get_Width();
		oImageData.m_unHeight = oBgraFrame.get_Height();

		if (oImageData.m_unWidth > oImageData.m_unHeight)
		{
			int nW = oImageData.m_unWidth * 9525;
			nW = (nW > 7000000 ? 7000000 : nW);
			oImageData.m_unHeight = (int)((double)oImageData.m_unHeight * (double)nW / (double)oImageData.m_unWidth);
			oImageData.m_unWidth = nW;
		}
		else
		{
			int nH = oImageData.m_unHeight * 9525;
			nH = (nH > 8000000 ? 8000000 : nH);
			int nW = (int)((double)oImageData.m_unWidth * (double)nH / (double)oImageData.m_unHeight);
			if (nW > 7000000)
			{
				nW = 7000000;
				oImageData.m_unHeight = (int)((double)oImageData.m_unHeight * (double)nW / (double)oImageData.m_unWidth);
			}
			else
				oImageData.m_unHeight = nH;
			oImageData.m_unWidth = nW;
		}
	}

	return true;
}

}
