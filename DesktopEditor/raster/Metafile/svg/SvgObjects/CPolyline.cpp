#include "CPolyline.h"

namespace SVG
{
	CPolyline::CPolyline(CObjectBase *pParent) : CObjectBase(pParent)
	{}

	CPolyline::~CPolyline()
	{}

	bool CPolyline::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return false;

		std::wstring wsValue = oNode.GetAttribute(L"points");

		if (wsValue.empty())
			return false;

		m_arValues = StrUtils::ReadDoubleValues(wsValue);

		SaveNodeData(oNode);

		return m_arValues.size() >= 4;
	}

	bool CPolyline::Draw(IRenderer *pRenderer, const CGeneralStyle* pBaseStyle) const
	{
		if (NULL == pRenderer || m_arValues.size() < 4)
			return false;

		int nPathType = 0;

		BeginDraw(pRenderer, nPathType, pBaseStyle);
		DrawLines(pRenderer);
		EndDraw(pRenderer, nPathType);

		return true;
	}

	void CPolyline::ApplyStyle(IRenderer *pRenderer, int &nTypePath, const CGeneralStyle* pBaseStyle) const
	{
		if (NULL == pRenderer || NULL == pBaseStyle)
			return;

		CStyle oStyle = pBaseStyle->GetStyle(GetFullPath());

		ApplyTransform(pRenderer, oStyle);
		ApplyStroke(pRenderer, oStyle, nTypePath, true);
		ApplyFill(pRenderer, oStyle, nTypePath, true);
	}

	void CPolyline::BeginDraw(IRenderer *pRenderer, int &nTypePath, const CGeneralStyle* pBaseStyle) const
	{
		ApplyStyle(pRenderer, nTypePath, pBaseStyle);

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand ( c_nPathType );

		pRenderer->PathCommandMoveTo(m_arValues[0], m_arValues[1]);
	}

	void CPolyline::DrawLines(IRenderer *pRenderer) const
	{
		for (unsigned int unIndex = 2; unIndex < m_arValues.size(); unIndex += 2)
			pRenderer->PathCommandLineTo(m_arValues[unIndex + 0], m_arValues[unIndex + 1]);
	}

	void CPolyline::EndDraw(IRenderer *pRenderer, int &nTypePath) const
	{
		pRenderer->DrawPath (nTypePath);
		pRenderer->EndCommand (c_nPathType);
		pRenderer->PathCommandEnd();
	}

	CPolygon::CPolygon(CObjectBase *pParent)	: CPolyline(pParent)
	{}

	CPolygon::~CPolygon()
	{}

	void CPolygon::EndDraw(IRenderer *pRenderer, int &nTypePath) const
	{
		pRenderer->PathCommandClose();

		pRenderer->DrawPath (nTypePath);
		pRenderer->EndCommand (c_nPathType);
		pRenderer->PathCommandEnd();
	}

}
