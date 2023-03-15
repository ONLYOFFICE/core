#include "CImage.h"

#include "CStyle.h"
#include "CContainer.h"
#include "../../graphics/Image.h"
#include "../../../BgraFrame.h"

namespace SVG
{
	CImage::CImage(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{
		m_oRect.m_oX     .SetValue(oNode.GetAttribute(L"x"));
		m_oRect.m_oY     .SetValue(oNode.GetAttribute(L"y"));
		m_oRect.m_oWidth .SetValue(oNode.GetAttribute(L"width"));
		m_oRect.m_oHeight.SetValue(oNode.GetAttribute(L"height"));

		m_wsHref = oNode.GetAttribute(L"href", oNode.GetAttribute(L"xlink:href")); // TODO:: В дальнейшем возможно стоит реализовать отдельный класс CHref для всех типов ссылок
	}

	void CImage::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);
	}

	bool CImage::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		if (NULL == pRenderer || m_wsHref.empty() || bIsClip)
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX      = m_oRect.m_oX     .ToDouble(NSCSS::Pixel, dParentWidth);
		double dY      = m_oRect.m_oY     .ToDouble(NSCSS::Pixel, dParentHeight);
		double dWidth  = m_oRect.m_oWidth .ToDouble(NSCSS::Pixel, dParentWidth);
		double dHeight = m_oRect.m_oHeight.ToDouble(NSCSS::Pixel, dParentHeight);

		if (std::wstring::npos != m_wsHref.find(L"data:image/png;base64,"))
		{
			std::wstring wsImageData = m_wsHref.substr(22, m_wsHref.length() - 22);
			BYTE* pBuffer;
			int unSize = NSBase64::Base64DecodeGetRequiredLength(wsImageData.length());

			pBuffer = new BYTE[unSize];

			if (NULL == pBuffer)
				return false;

			NSBase64::Base64Decode(wsImageData.c_str(), wsImageData.length(), pBuffer, &unSize);

			CBgraFrame oBgraFrame;
			oBgraFrame.Decode(pBuffer, unSize);

			double dImageW = oBgraFrame.get_Width();
			double dImageH = oBgraFrame.get_Height();

			Aggplus::CImage oImage;
			oImage.Create(oBgraFrame.get_Data(), dImageW, dImageH, -4 * dImageW, true);

			if (dImageW / dImageH == dWidth / dHeight)
				pRenderer->DrawImage(&oImage, dX, dY, dWidth, dHeight);
			else if (dImageW / dWidth > dImageH / dHeight)
			{
				double dValue = dImageW / dWidth;
				pRenderer->DrawImage(&oImage, dX, dY + (dHeight - (dImageH / dValue)) / 2, dWidth, dImageH / dValue);
			}
			else
			{
				double dValue = dImageH / dHeight;
				pRenderer->DrawImage(&oImage, dX + (dWidth - (dImageW / dValue)) / 2, dY, dImageW / dValue, dHeight);
			}

			delete[] pBuffer;
		}
		return true;
	}

	CImage *CImage::Copy() const
	{
		return new CImage(*this);
	}

	void CImage::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
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
