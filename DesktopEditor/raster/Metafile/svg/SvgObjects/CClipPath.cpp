#include "CClipPath.h"

namespace SVG
{
	CClipPath::CClipPath(XmlUtils::CXmlNode &oNode)
		: CAppliedObject(oNode)
	{}

	void CClipPath::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
//		SetClip(mAttributes, ushLevel, bHardMode);
//		SetMask(mAttributes, ushLevel, bHardMode);
	}

	bool CClipPath::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (NULL == pRenderer || NULL == pFile)
			return false;

		pRenderer->put_ClipMode(c_nClipRegionUnion);

		pRenderer->PathCommandStart();

		for (const CRenderedObject* pObject : m_oContainer.m_arObjects)
			pObject->Draw(pRenderer, pFile, CommandeModeClip);

		pRenderer->PathCommandEnd();

		return true;
	}

	CContainer<CRenderedObject> &CClipPath::GetContainer()
	{
		return m_oContainer;
	}
}
