#include "CUse.h"
#include "../CSvgFile.h"

namespace SVG
{
	CUse::CUse(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent, const CSvgFile* pFile)
	    : CSvgGraphicsObject(oNode, pParent), m_pUsedObject(NULL)
	{
		if (NULL != pFile)
		{
			std::wstring wsHref = oNode.GetAttribute(L"href", oNode.GetAttribute(L"xlink:href"));

			const CSvgGraphicsObject *pFoundObj = pFile->GetMarkedObject(wsHref);
			if (NULL != pFoundObj)
				m_pUsedObject = pFoundObj->Copy();
		}

		m_oX     .SetValue(oNode.GetAttribute(L"x"));
		m_oY     .SetValue(oNode.GetAttribute(L"y"));
		m_oWidth .SetValue(oNode.GetAttribute(L"width"));
		m_oHeight.SetValue(oNode.GetAttribute(L"height"));
	}

	CUse::~CUse()
	{
		if (NULL != m_pUsedObject)
			delete m_pUsedObject;
	}

	void CUse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
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

	CSvgGraphicsObject *CUse::Copy() const
	{
		return new CUse(*this);
	}

	void CUse::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix &oOldMatrix) const
	{
	}

	TBounds CUse::GetBounds() const
	{
		return TBounds{0., 0., 0., 0.};
	}
}
