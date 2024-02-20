#include "CClipPath.h"

namespace SVG
{
	CClipPath::CClipPath(XmlUtils::CXmlNode &oNode)
		: CAppliedObject(oNode)
	{
		if (L"userSpaceOnUse" == oNode.GetAttribute(L"gradientUnits"))
			m_enUnits = ClipU_UserSpaceOnUse;
		else
			m_enUnits = ClipU_ObjectBoundingBox;
	}

	void CClipPath::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetClip(mAttributes, ushLevel, bHardMode);
		SetMask(mAttributes, ushLevel, bHardMode);
	}

	bool CClipPath::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (NULL == pRenderer || NULL == pFile)
			return false;

		ApplyClip(pRenderer, &m_oTransformtaion.m_oClip, pFile, oObjectBounds);
		
		for (const CRenderedObject* pObject : m_oContainer.m_arObjects)
			pObject->Draw(pRenderer, pFile, CommandeModeClip);

		return true;
	}

	CContainer<CRenderedObject> &CClipPath::GetContainer()
	{
		return m_oContainer;
	}
}
