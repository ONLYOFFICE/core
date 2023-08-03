#include "CUse.h"
#include "../CSvgFile.h"

namespace SVG
{
	CUse::CUse(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent, const CSvgFile* pFile)
	: CRenderedObject(oNode, pParent), m_pFile(pFile)
	{
		m_wsHref = oNode.GetAttribute(L"href", oNode.GetAttribute(L"xlink:href"));

		m_oX     .SetValue(oNode.GetAttribute(L"x"));
		m_oY     .SetValue(oNode.GetAttribute(L"y"));
		m_oWidth .SetValue(oNode.GetAttribute(L"width"));
		m_oHeight.SetValue(oNode.GetAttribute(L"height"));
	}

	CUse::~CUse()
	{}

	void CUse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
	}

	bool CUse::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles* pOtherStyles) const
	{
		if (NULL == pRenderer || !m_oStyles.m_bDisplay)
			return false;

		double dM11, dM12, dM21, dM22, dRx, dRy;
		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dRx + m_oX.ToDouble(NSCSS::Pixel) * dM11, dRy + m_oY.ToDouble(NSCSS::Pixel) * dM22);

		const CRenderedObject *pFoundObj = dynamic_cast<CRenderedObject*>(m_pFile->GetMarkedObject(m_wsHref));

		if (NULL != pFoundObj)
		{
			if (NULL != pOtherStyles)
			{
				TSvgStyles oNewStyles(m_oStyles);
				oNewStyles += *pOtherStyles;
				pFoundObj->Draw(pRenderer, pFile, oMode, &oNewStyles);
			}
			else
				pFoundObj->Draw(pRenderer, pFile, oMode, &m_oStyles);
		}
		pRenderer->SetTransform(dM11, dM12, dM21, dM22, dRx, dRy);

		return true;
	}

	void CUse::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath) const
	{}

	TBounds CUse::GetBounds() const
	{
		return TBounds{0., 0., 0., 0.};
	}
}
