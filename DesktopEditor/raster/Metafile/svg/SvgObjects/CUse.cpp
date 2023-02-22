#include "CUse.h"

namespace SVG
{
	CUse::CUse(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent)
	    : CSvgGraphicsObject(oNode, pParent), m_pUsedObject(NULL)
	{
		if (NULL != pParent)
		{
			CGraphicsContainer *pContainer = dynamic_cast<CGraphicsContainer*>(pParent);
			if (NULL != pContainer)
			{
				std::wstring wsHref;
				wsHref = oNode.GetAttribute(L"href",       wsHref);
				wsHref = oNode.GetAttribute(L"xlink:href", wsHref);

				CSvgGraphicsObject *pFoundObj = pContainer->GetObject(wsHref);
				if (NULL != pFoundObj)
					m_pUsedObject = pFoundObj->Copy();
			}
		}
	}

	CUse::~CUse()
	{
		if (NULL != m_pUsedObject)
			delete m_pUsedObject;
	}

	void CUse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"x"))
			m_oX.SetValue(mAttributes.at(L"x"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y"))
			m_oY.SetValue(mAttributes.at(L"y"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"width"))
			m_oWidth.SetValue(mAttributes.at(L"width"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"height"))
			m_oHeight.SetValue(mAttributes.at(L"height"), ushLevel, bHardMode);

		if (NULL != m_pUsedObject)
			m_pUsedObject->SetData(mAttributes, 0, false);
	}

	bool CUse::Draw(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		double dM11, dM12, dM21, dM22, dRx, dRy;
		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dRx + m_oX.ToDouble(NSCSS::Pixel) * dM11, dRy + m_oY.ToDouble(NSCSS::Pixel) * dM22);

		if (NULL != m_pUsedObject)
			m_pUsedObject->Draw(pRenderer, pDefs, bIsClip);

		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dRx, dRy);

		return true;
	}

	void CUse::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{
	}

	TBounds CUse::GetBounds() const
	{
		return TBounds{0., 0., 0., 0.};
	}
}
