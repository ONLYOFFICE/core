#include "CCircle.h"

namespace SVG
{
	CCircle::CCircle(CObjectBase *pParent, CGeneralStyle* pBaseStyle) : CObjectBase(pParent, pBaseStyle)
	{

	}

	CCircle::~CCircle()
	{

	}

	bool CCircle::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return false;

		m_dCx = oNode.GetAttributeDouble(L"cx");
		m_dCy = oNode.GetAttributeDouble(L"cy");
		m_dR  = oNode.GetAttributeDouble(L"r");

		SaveNodeData(oNode);

		return true;
	}

	bool CCircle::Draw(IRenderer *pRenderer)
	{
		if (NULL == pRenderer)
			return false;

		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand (c_nPathType);

		pRenderer->PathCommandStart ();

		pRenderer->PathCommandMoveTo(m_dCx + m_dR, m_dCy);
		pRenderer->PathCommandArcTo(m_dCx - m_dR, m_dCy - m_dR, m_dR * 2.0, m_dR * 2.0, 0, 360);

		pRenderer->DrawPath (nPathType);
		pRenderer->EndCommand (c_nPathType);

		pRenderer->PathCommandEnd ();

		return true;
	}

	void CCircle::ApplyStyle(IRenderer *pRenderer, int& nTypePath)
	{
		if (NULL == pRenderer || NULL == m_pStyle)
			return;

		CStyle oStyle = m_pStyle->GetStyle(GetFullPath());

		ApplyTransform(pRenderer, oStyle);
		ApplyStroke(pRenderer, oStyle, nTypePath);
		ApplyFill(pRenderer, oStyle, nTypePath);
	}

}
