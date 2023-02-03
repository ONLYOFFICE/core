#include "CHeader.h"

namespace SVG
{
	CHeader::CHeader(CObjectBase *pParent) : CObjectBase(pParent)
	{

	}

	CHeader::~CHeader()
	{
	}

	bool CHeader::ReadFromXmlNode(XmlUtils::CXmlNode &oNode, const CGeneralStyle& oBaseStyle)
	{
		SaveNodeData(oNode, oBaseStyle);

		return true;
	}

	bool CHeader::Draw(IRenderer *pRenderer) const
	{
		if (NULL == pRenderer)
			return false;

		return true;
	}

	void CHeader::GetBounds(double &dX, double &dY, double &dWidth, double &dHeight)
	{
		dX      = m_oStyle.m_oDisplay.GetX()     .ToDouble();
		dY      = m_oStyle.m_oDisplay.GetY()     .ToDouble();
		dWidth  = m_oStyle.m_oDisplay.GetWidth() .ToDouble();
		dHeight = m_oStyle.m_oDisplay.GetHeight().ToDouble();
	}

	void CHeader::ApplyStyle(IRenderer *pRenderer, int& nTypePath) const
	{

	}
}
