#include "CRect.h"
#include "CStyle.h"
#include "../SvgTypes.h"

namespace SVG
{
	CRect::CRect(CObjectBase *pParent, CGeneralStyle* pBaseStyle) : CObjectBase(pParent, pBaseStyle)
	{

	}

	CRect::~CRect()
	{

	}

	bool CRect::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return false;

		m_dX      = oNode.GetAttributeDouble(L"x");
		m_dY      = oNode.GetAttributeDouble(L"y");
		m_dWidth  = oNode.GetAttributeDouble(L"width");
		m_dHeight = oNode.GetAttributeDouble(L"height");
		m_dRx     = oNode.GetAttributeDouble(L"rx");
		m_dRy     = oNode.GetAttributeDouble(L"ry");

		if (0 == m_dWidth || 0 == m_dHeight)
			return false;

		SaveNodeData(oNode);

		return true;
	}

	bool CRect::Draw(IRenderer *pRenderer)
	{
		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);

		if ((fabs(m_dRx) < 0.000001) && (fabs(m_dRy) < 0.000001))
		{
			pRenderer->PathCommandStart();
			pRenderer->BeginCommand(c_nPathType);

			pRenderer->PathCommandStart();

			pRenderer->PathCommandMoveTo(m_dX, m_dY);
			pRenderer->PathCommandLineTo(m_dX + m_dWidth, m_dY);
			pRenderer->PathCommandLineTo(m_dX + m_dWidth, m_dY + m_dHeight);
			pRenderer->PathCommandLineTo(m_dX, m_dY + m_dHeight);
			pRenderer->PathCommandClose();

			pRenderer->DrawPath(nPathType);
			pRenderer->EndCommand(c_nPathType);
			pRenderer->PathCommandEnd();
		}
		else
		{
			pRenderer->PathCommandStart();
			pRenderer->BeginCommand(c_nPathType);

			pRenderer->PathCommandStart();

			pRenderer->PathCommandMoveTo(m_dX + m_dRx, m_dY);

			pRenderer->PathCommandLineTo(m_dX - m_dRx, m_dY);
			pRenderer->PathCommandArcTo(m_dX - m_dRx * 2.0, m_dY, m_dRx * 2.0, m_dRy * 2.0, 270.0, 90.0);

			pRenderer->PathCommandLineTo(m_dX, m_dY + m_dHeight - m_dRy);
			pRenderer->PathCommandArcTo(m_dX - m_dRx * 2.0, m_dY + m_dHeight - m_dRy * 2.0, m_dRx * 2.0, m_dRy * 2.0, 0.0, 90.0);

			pRenderer->PathCommandLineTo(m_dX + m_dWidth + m_dRx, m_dY + m_dHeight);
			pRenderer->PathCommandArcTo(m_dX + m_dWidth, m_dY + m_dHeight - m_dRy * 2.0, m_dRx * 2.0, m_dRy * 2.0, 90.0, 90.0);

			pRenderer->PathCommandLineTo(m_dX + m_dWidth, m_dY + m_dRy);
			pRenderer->PathCommandArcTo(m_dX + m_dWidth, m_dY, m_dRx * 2.0, m_dRy * 2.0, 180.0, 90.0);

			pRenderer->DrawPath(nPathType);
			pRenderer->EndCommand(c_nPathType);
			pRenderer->PathCommandEnd ();
		}

		return true;
	}

	void CRect::ApplyStyle(IRenderer *pRenderer, int& nTypePath)
	{
		if (NULL == pRenderer)
			return;

		CStyle oStyle = m_pStyle->GetStyle({m_oXmlNode});

		ApplyTransform(pRenderer, oStyle);
		ApplyStroke(pRenderer, oStyle, nTypePath);
		ApplyFill(pRenderer, oStyle, nTypePath);
	}

}
