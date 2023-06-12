#include "CMask.h"

namespace SVG
{
	CMask::CMask(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent, NSFonts::IFontManager *pFontManager)
		: CClipPath(oNode, pParent, pFontManager)
	{

	}

	bool CMask::Apply(IRenderer *pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds)
	{
		if (NULL == pRenderer || NULL == pDefs)
			return false;

		CGraphicsContainer::ApplyMask(pRenderer, &m_oStyles.m_oMask, pDefs);

		pRenderer->BeginCommand(c_nMaskType);
		pRenderer->PathCommandStart();

		for (const CSvgGraphicsObject* pGraphicsObject : m_arObjects)
			pGraphicsObject->Draw(pRenderer, pDefs);

		pRenderer->EndCommand(c_nMaskType);
		pRenderer->PathCommandEnd();

		return true;
	}
}
