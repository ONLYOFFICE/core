#include "CClipPath.h"

namespace SVG
{
	CClipPath::CClipPath(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent, NSFonts::IFontManager *pFontManager)
	    : CGraphicsContainer(oNode, pParent), CDefObject(oNode, pParent)
	{}

	void CClipPath::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetClip(mAttributes, ushLevel, bHardMode);
	}

	bool CClipPath::Apply(IRenderer *pRenderer, const CDefs *pDefs, const TBounds &oObjectBounds)
	{
		if (NULL == pRenderer || NULL == pDefs)
			return false;

		ApplyClip(pRenderer, pDefs);

		for (const CSvgGraphicsObject* pGraphicsObject : m_arObjects)
			pGraphicsObject->Draw(pRenderer, pDefs, true);

		return true;
	}
}
