#include "CRect.h"
#include "CStyle.h"
#include "../SvgTypes.h"

namespace SVG
{
	CRect::CRect(CObjectBase *pParent) : CObjectBase(pParent)
	{

	}

	CRect::~CRect()
	{

	}

	bool CRect::ReadFromXmlNode(XmlUtils::CXmlNode &oNode, const CGeneralStyle& oBaseStyle)
	{
		if (!oNode.IsValid())
			return false;

		SaveNodeData(oNode, oBaseStyle);

		return true;
	}

	bool CRect::Draw(IRenderer *pRenderer) const
	{
		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);

		double dX     = m_oStyle.m_pDisplay.GetX();
		double dY     = m_oStyle.m_pDisplay.GetY();
		double dWidth = m_oStyle.m_pDisplay.GetWidth();
		double dHeight= m_oStyle.m_pDisplay.GetHeight();

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand(c_nPathType);

		pRenderer->PathCommandStart();

		pRenderer->PathCommandMoveTo(dX, dY);
		pRenderer->PathCommandLineTo(dX + dWidth, dY);
		pRenderer->PathCommandLineTo(dX + dWidth, dY + dHeight);
		pRenderer->PathCommandLineTo(dX, dY + dHeight);
		pRenderer->PathCommandClose();

		pRenderer->DrawPath(nPathType);
		pRenderer->EndCommand(c_nPathType);
		pRenderer->PathCommandEnd();


//		if ((fabs(m_dRx) < 0.000001) && (fabs(m_dRy) < 0.000001))
//		{
//			pRenderer->PathCommandStart();
//			pRenderer->BeginCommand(c_nPathType);

//			pRenderer->PathCommandStart();

//			pRenderer->PathCommandMoveTo(m_dX, m_dY);
//			pRenderer->PathCommandLineTo(m_dX + dWidth, m_dY);
//			pRenderer->PathCommandLineTo(m_dX + dWidth, m_dY + m_dHeight);
//			pRenderer->PathCommandLineTo(m_dX, m_dY + m_dHeight);
//			pRenderer->PathCommandClose();

//			pRenderer->DrawPath(nPathType);
//			pRenderer->EndCommand(c_nPathType);
//			pRenderer->PathCommandEnd();
//		}
//		else
//		{
//			pRenderer->PathCommandStart();
//			pRenderer->BeginCommand(c_nPathType);

//			pRenderer->PathCommandStart();

//			pRenderer->PathCommandMoveTo(m_dX + m_dRx, m_dY);

//			pRenderer->PathCommandLineTo(m_dX - m_dRx, m_dY);
//			pRenderer->PathCommandArcTo(m_dX - m_dRx * 2.0, m_dY, m_dRx * 2.0, m_dRy * 2.0, 270.0, 90.0);

//			pRenderer->PathCommandLineTo(m_dX, m_dY + m_dHeight - m_dRy);
//			pRenderer->PathCommandArcTo(m_dX - m_dRx * 2.0, m_dY + m_dHeight - m_dRy * 2.0, m_dRx * 2.0, m_dRy * 2.0, 0.0, 90.0);

//			pRenderer->PathCommandLineTo(m_dX + dWidth + m_dRx, m_dY + m_dHeight);
//			pRenderer->PathCommandArcTo(m_dX + dWidth, m_dY + m_dHeight - m_dRy * 2.0, m_dRx * 2.0, m_dRy * 2.0, 90.0, 90.0);

//			pRenderer->PathCommandLineTo(m_dX + dWidth, m_dY + m_dRy);
//			pRenderer->PathCommandArcTo(m_dX + dWidth, m_dY, m_dRx * 2.0, m_dRy * 2.0, 180.0, 90.0);

//			pRenderer->DrawPath(nPathType);
//			pRenderer->EndCommand(c_nPathType);
//			pRenderer->PathCommandEnd ();
//		}

		return true;
	}

	void CRect::ApplyStyle(IRenderer *pRenderer, int& nTypePath) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, nTypePath);
	}

}
