#include "CHeader.h"

namespace SVG
{
	CHeader::CHeader(CObjectBase *pParent, CGeneralStyle* pBaseStyle) : CObjectBase(pParent, pBaseStyle)
	{

	}

	CHeader::~CHeader()
	{
	}

	bool CHeader::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		m_dX      = oNode.ReadAttributeDouble(L"x");
		m_dY      = oNode.ReadAttributeDouble(L"y");
		m_dWidth  = oNode.ReadAttributeDouble(L"width");
		m_dHeight = oNode.ReadAttributeDouble(L"height");

		if (0 == m_dWidth || 0 == m_dHeight)
			return false;

		return true;
	}

	bool CHeader::Draw(IRenderer *pRenderer)
	{
		if (NULL == pRenderer)
			return false;

		return true;
	}

	void CHeader::GetBounds(double &dX, double &dY, double &dWidth, double &dHeight)
	{
		dX      = m_dX;
		dY      = m_dY;
		dWidth  = m_dWidth;
		dHeight = m_dHeight;
	}

	void CHeader::ApplyStyle(IRenderer *pRenderer)
	{

	}
}
