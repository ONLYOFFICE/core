#include "CCircle.h"

namespace SVG
{
	CCircle::CCircle(CObjectBase *pParent) : CObjectBase(pParent)
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

		UpdateStyle(oNode);

		return true;
	}

	bool CCircle::Draw(IRenderer *pRenderer)
	{
		if (NULL == pRenderer)
			return false;

		ApplyStyle(pRenderer);

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand (c_nPathType);

		pRenderer->PathCommandStart ();

		pRenderer->PathCommandMoveTo(m_dCx + m_dR, m_dCy);
		pRenderer->PathCommandArcTo(m_dCx - m_dR, m_dCy - m_dR, m_dR * 2.0, m_dR * 2.0, 0, 360);

		pRenderer->DrawPath (c_nWindingFillMode);
		pRenderer->EndCommand (c_nPathType);

		pRenderer->PathCommandEnd ();

		return true;
	}

	void CCircle::ApplyStyle(IRenderer *pRenderer)
	{
		if (NULL == pRenderer)
			return;

		pRenderer->put_BrushColor1(m_oStyle.GetFillL());
		pRenderer->put_PenColor(m_oStyle.GetStrokeColor());
	}

}
