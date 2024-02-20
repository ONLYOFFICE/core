#include "CUse.h"
#include "../CSvgFile.h"

namespace SVG
{
	CUse::CUse(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent)
		: CRenderedObject(oNode, pParent)
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
		if (NULL == pRenderer || !m_oTransformtaion.m_bDraw)
			return false;
		
		Aggplus::CMatrix oTransform;
		
		if (!StartPath(pRenderer, pFile, oTransform))
			return false;
		
		double dM11, dM12, dM21, dM22, dDx, dDy;
		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

		Aggplus::CMatrix oNewTransform(dM11, dM12, dM21, dM22, dDx, dDy);
		oNewTransform.Translate(m_oX.ToDouble(NSCSS::Pixel), m_oY.ToDouble(NSCSS::Pixel));

		pRenderer->SetTransform(oNewTransform.sx(), oNewTransform.shy(), oNewTransform.shx(), oNewTransform.sy(), oNewTransform.tx(), oNewTransform.ty());

		const CRenderedObject *pFoundObj = dynamic_cast<CRenderedObject*>(pFile->GetMarkedObject(m_wsHref));

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
		
		EndPath(pRenderer, pFile, oTransform);

		return true;
	}

	TBounds CUse::GetBounds() const
	{
		return TBounds{0., 0., 0., 0.};
	}
}
