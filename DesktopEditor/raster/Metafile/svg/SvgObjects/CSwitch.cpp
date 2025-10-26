#include "CSwitch.h"

namespace SVG 
{
	CSwitch::CSwitch(CSvgReader& oReader, CRenderedObject *pParent)
		: CRenderedObject(oReader, pParent)
	{}

	bool CSwitch::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pStyles, const CRenderedObject* pContexObject) const
	{
		for (const CRenderedObject* pObject : m_arObjects)
		{
			if (NULL != pObject && pObject->Draw(pRenderer, pFile, oMode, pStyles, pContexObject))
				return true;
		}

		return false;
	}

	TBounds CSwitch::GetBounds(SvgMatrix* pTransform) const
	{
		for (const CRenderedObject* pObject : m_arObjects)
			if (NULL != pObject)
				return pObject->GetBounds();

		return TBounds{0., 0., 0., 0.};
	}
}
