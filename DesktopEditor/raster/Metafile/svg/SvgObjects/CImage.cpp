#include "CImage.h"

#include "../CSvgFile.h"
#include "../../graphics/Image.h"
#include "../../../BgraFrame.h"
#include "../../../../common/Path.h"
#include "../../../../common/ProcessEnv.h"

namespace SVG
{
	CImage::CImage(XmlUtils::CXmlNode& oNode, CRenderedObject* pParent)
		: CRenderedObject(oNode, pParent)
	{
		m_oRect.m_oX     .SetValue(oNode.GetAttribute(L"x"));
		m_oRect.m_oY     .SetValue(oNode.GetAttribute(L"y"));
		m_oRect.m_oWidth .SetValue(oNode.GetAttribute(L"width"));
		m_oRect.m_oHeight.SetValue(oNode.GetAttribute(L"height"));

		m_wsHref = oNode.GetAttribute(L"href", oNode.GetAttribute(L"xlink:href")); // TODO:: В дальнейшем возможно стоит реализовать отдельный класс CHref для всех типов ссылок
	}

	bool CImage::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles) const
	{
		Aggplus::CMatrix oOldTransform;

		if (m_wsHref.empty() || !StartPath(pRenderer, pFile, oOldTransform, oMode))
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX      = m_oRect.m_oX     .ToDouble(NSCSS::Pixel, dParentWidth);
		double dY      = m_oRect.m_oY     .ToDouble(NSCSS::Pixel, dParentHeight);
		double dWidth  = m_oRect.m_oWidth .ToDouble(NSCSS::Pixel, dParentWidth);
		double dHeight = m_oRect.m_oHeight.ToDouble(NSCSS::Pixel, dParentHeight);

		BYTE* pBuffer = NULL;
		DWORD ulSize = 0;
		size_t unStart = m_wsHref.find(L"data:image");

		if (std::wstring::npos != unStart)
		{
			size_t unType = m_wsHref.find(L";base64", unStart);

			if (std::wstring::npos == unType)
				return false;

			const std::wstring wsImageType = m_wsHref.substr(unStart + 11, unType - unStart - 11);

			if (L"png" != wsImageType && L"jpeg" != wsImageType)
				return false;
	
			std::wstring wsImageData = m_wsHref.substr(unType + 8, m_wsHref.length() - unType - 8);
			ulSize = NSBase64::Base64DecodeGetRequiredLength(wsImageData.length());

			pBuffer = new BYTE[ulSize];

			NSBase64::Base64Decode(wsImageData.c_str(), wsImageData.length(), pBuffer, &(int&)ulSize);
		}

		#ifndef METAFILE_DISABLE_FILESYSTEM
		std::wstring wsFilePath = NSSystemPath::ShortenPath(m_wsHref);

		bool bIsAllowExternalLocalFiles = true;
		if (NSProcessEnv::IsPresent(NSProcessEnv::Converter::gc_allowPrivateIP))
			bIsAllowExternalLocalFiles = NSProcessEnv::GetBoolValue(NSProcessEnv::Converter::gc_allowPrivateIP);

		if (!bIsAllowExternalLocalFiles && wsFilePath.length() >= 3 && L"../" == wsFilePath.substr(0, 3))
			return true;

		wsFilePath = pFile->GetWorkingDirectory() + L'/' + wsFilePath;

		if (!NSFile::CFileBinary::Exists(wsFilePath) || !NSFile::CFileBinary::ReadAllBytes(wsFilePath, &pBuffer, ulSize))
			return false;
		#endif

		if (NULL == pBuffer)
			return false;

		CBgraFrame oBgraFrame;
		oBgraFrame.Decode(pBuffer, ulSize);

		double dImageW = oBgraFrame.get_Width();
		double dImageH = oBgraFrame.get_Height();

		Aggplus::CImage oImage;
		oImage.Create(oBgraFrame.get_Data(), dImageW, dImageH, -4 * dImageW, true);

		if (dImageW / dWidth > dImageH / dHeight)
		{
			double dValue = dImageW / dWidth;
			dY += (dHeight - (dImageH / dValue)) / 2.;
			dHeight = dImageH / dValue;
		}
		else if (dImageW / dWidth < dImageH / dHeight)
		{
			double dValue = dImageH / dHeight;
			dX += (dWidth - (dImageW / dValue)) / 2.;
			dWidth = dImageW / dValue;
		}

		if (CommandeModeClip != oMode)
			pRenderer->DrawImage(&oImage, dX, dY, dWidth, dHeight);
		else
		{
			pRenderer->PathCommandMoveTo(dX, dY);
			pRenderer->PathCommandLineTo(dX + dWidth, dY);
			pRenderer->PathCommandLineTo(dX + dWidth, dY + dHeight);
			pRenderer->PathCommandLineTo(dX, dY + dHeight);
			pRenderer->PathCommandClose();
		}

		EndPath(pRenderer, pFile, oOldTransform, oMode, pOtherStyles);

		delete[] pBuffer;

		return true;
	}

	TBounds CImage::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oRect.m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oRect.m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = oBounds.m_dLeft + m_oRect.m_oWidth.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = oBounds.m_dTop  + m_oRect.m_oHeight.ToDouble(NSCSS::Pixel);

		return oBounds;
	}
}
