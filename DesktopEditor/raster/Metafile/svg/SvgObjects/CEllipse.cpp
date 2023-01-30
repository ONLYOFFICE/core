#include "CEllipse.h"

namespace SVG
{
	CEllipse::CEllipse(CObjectBase *pParent) : CObjectBase(pParent)
	{}

	bool CEllipse::ReadFromXmlNode(XmlUtils::CXmlNode &oNode, const CGeneralStyle& oBaseStyle)
	{
		if (!oNode.IsValid())
			return false;

		m_dCx = oNode.GetAttributeDouble(L"cx");
		m_dCy = oNode.GetAttributeDouble(L"cy");
		m_dRx = oNode.GetAttributeDouble(L"rx");
		m_dRy = oNode.GetAttributeDouble(L"ry");

		SaveNodeData(oNode, oBaseStyle);

		return true;
	}

	bool CEllipse::Draw(IRenderer *pRenderer) const
	{
		if (NULL == pRenderer)
			return false;

		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand (c_nPathType);

		pRenderer->PathCommandStart ();

		pRenderer->PathCommandMoveTo(m_dCx + m_dRx, m_dCy);
		pRenderer->PathCommandArcTo(m_dCx - m_dRx, m_dCy - m_dRy, m_dRx * 2.0, m_dRy * 2.0, 0, 360);

		pRenderer->DrawPath (nPathType);
		pRenderer->EndCommand (c_nPathType);

		pRenderer->PathCommandEnd ();

		return true;
	}

	void CEllipse::ApplyStyle(IRenderer *pRenderer, int& nTypePath) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, nTypePath);
	}
}
