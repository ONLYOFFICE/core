#include "CLine.h"

namespace SVG
{
	CLine::CLine(CObjectBase *pParent) : CObjectBase(pParent)
	{

	}

	CLine::~CLine()
	{
	}

	bool CLine::ReadFromXmlNode(XmlUtils::CXmlNode &oNode, const CGeneralStyle& oBaseStyle)
	{
		if (!oNode.IsValid())
			return false;

		m_dX1 = oNode.GetAttributeDouble(L"x1");
		m_dY1 = oNode.GetAttributeDouble(L"y1");
		m_dX2 = oNode.GetAttributeDouble(L"x2");
		m_dY2 = oNode.GetAttributeDouble(L"y2");

		SaveNodeData(oNode, oBaseStyle);

		return true;
	}

	bool CLine::Draw(IRenderer *pRenderer) const
	{
		if (NULL == pRenderer)
			return false;

		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);
		pRenderer->PathCommandStart();
		pRenderer->BeginCommand (c_nPathType);

		pRenderer->PathCommandStart ();

		pRenderer->PathCommandMoveTo (m_dX1, m_dY1);
		pRenderer->PathCommandLineTo (m_dX2, m_dY2);

		pRenderer->DrawPath (nPathType);
		pRenderer->EndCommand (c_nPathType);
		pRenderer->PathCommandEnd ();

		return true;
	}

	void CLine::ApplyStyle(IRenderer *pRenderer, int& nTypePath) const
	{
		if (NULL == pRenderer )
			return;

		ApplyTransform(pRenderer);
		ApplyStroke(pRenderer, nTypePath);
	}
}
