#include "CSwitch.h"

namespace SVG 
{
	CSwitch::CSwitch(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent)
		: CRenderedObject(oNode, pParent)
	{
	}

	bool CSwitch::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pStyles) const
	{
		for (const CRenderedObject* pObject : m_arObjects)
		{
			if (NULL != pObject && pObject->Draw(pRenderer, pFile, oMode, pStyles))
				return true;
		}

		return false;
	}

	TBounds CSwitch::GetBounds() const
	{
		for (const CRenderedObject* pObject : m_arObjects)
			if (NULL != pObject)
				return pObject->GetBounds();

		return TBounds{0., 0., 0., 0.};
	}
}
