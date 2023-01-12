#include "CHeader.h"

namespace SVG
{
	CHeader::CHeader(CObjectBase *pParent) : CObjectBase(pParent)
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

		SaveNodeData(oNode);

		return true;
	}

	bool CHeader::Draw(IRenderer *pRenderer, const CGeneralStyle* pBaseStyle) const
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

	void CHeader::ApplyStyle(IRenderer *pRenderer, int& nTypePath, const CGeneralStyle* pBaseStyle) const
	{

	}
}
