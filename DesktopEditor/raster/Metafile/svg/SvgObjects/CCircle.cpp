#include "CCircle.h"

namespace SVG
{
	CCircle::CCircle(CObjectBase *pParent) : CObjectBase(pParent)
	{

	}

	CCircle::~CCircle()
	{

	}

	bool CCircle::ReadFromXmlNode(XmlUtils::CXmlNode &oNode, const CGeneralStyle& oBaseStyle)
	{
		if (!oNode.IsValid())
			return false;

//		m_dCx = oNode.GetAttributeDouble(L"cx");
//		m_dCy = oNode.GetAttributeDouble(L"cy");
//		m_dR  = oNode.GetAttributeDouble(L"r");

		SaveNodeData(oNode, oBaseStyle);

		return true;
	}

	bool CCircle::Draw(IRenderer *pRenderer) const
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

	void CCircle::ApplyStyle(IRenderer *pRenderer, int& nTypePath) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, nTypePath);
	}

}
