#include "CImage.h"

#include "CStyle.h"
#include "CContainer.h"
#include "../../graphics/Image.h"
#include "../../../BgraFrame.h"

namespace SVG
{
	CImage::CImage(CObjectBase *pParent)
	    : CObjectBase(pParent)
	{}

	void CImage::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"x"))
			m_oRect.m_oX.SetValue(mAttributes.at(L"x"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y"))
			m_oRect.m_oY.SetValue(mAttributes.at(L"y"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"width"))
			m_oRect.m_oWidth.SetValue(mAttributes.at(L"width"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"height"))
			m_oRect.m_oHeight.SetValue(mAttributes.at(L"height"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"href")) // TODO:: В дальнейшем необходимо реализовать
			m_wsHref = mAttributes.at(L"href");             // отдельный класс CHref для всех типов ссылок
	}

	bool CImage::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return false;

		SaveNodeData(oNode);

		return true;
	}

	bool CImage::Draw(IRenderer *pRenderer) const
	{
		if (NULL == pRenderer || m_wsHref.empty())
			return false;

		double dParentWidth = 0, dParentHeight = 0;
		CContainer *pContainer = dynamic_cast<CContainer*>(m_pParent);

		if (NULL != pContainer)
		{
			dParentWidth  = pContainer->GetWindow().m_oWidth.ToDouble(NSCSS::Pixel);
			dParentHeight = pContainer->GetWindow().m_oWidth.ToDouble(NSCSS::Pixel);
		}

		double dX      = m_oRect.m_oX.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY      = m_oRect.m_oY.ToDouble(NSCSS::Pixel, dParentHeight);
		double dWidth  = m_oRect.m_oWidth.ToDouble(NSCSS::Pixel, dParentWidth);
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

	void CImage::ApplyStyle(IRenderer *pRenderer, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
	}
}
