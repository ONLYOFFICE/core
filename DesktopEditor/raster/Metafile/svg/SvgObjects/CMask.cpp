#include "CMask.h"

namespace SVG
{
	CMask::CMask(XmlUtils::CXmlNode &oNode)
		: CClipPath(oNode)
	{}

	bool CMask::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (NULL == pRenderer || NULL == pFile)
			return false;

		pRenderer->BeginCommand(c_nMaskType);
		pRenderer->PathCommandStart();

		for (const CRenderedObject* pObject : m_oContainer.m_arObjects)
			pObject->Draw(pRenderer, pFile, CommandeModeMask);

		pRenderer->EndCommand(c_nMaskType);
		pRenderer->PathCommandEnd();

		return true;
	}
}
