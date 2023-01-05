#include "CPolyline.h"

namespace SVG
{
	CPolyline::CPolyline(CObjectBase *pParent, CGeneralStyle *pBaseStyle) : CObjectBase(pParent, pBaseStyle)
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

	bool CPolyline::Draw(IRenderer *pRenderer)
	{
		if (NULL == pRenderer || m_arValues.size() < 4)
			return false;

		int nPathType = 0;

		BeginDraw(pRenderer, nPathType);
		DrawLines(pRenderer);
		EndDraw(pRenderer, nPathType);

		return true;
	}

	void CPolyline::ApplyStyle(IRenderer *pRenderer, int &nTypePath)
	{
		if (NULL == pRenderer)
			return;

		CStyle oStyle = m_pStyle->GetStyle({m_oXmlNode});

		ApplyTransform(pRenderer, oStyle);
		ApplyStroke(pRenderer, oStyle, nTypePath, true);
		ApplyFill(pRenderer, oStyle, nTypePath, true);
	}

	void CPolyline::BeginDraw(IRenderer *pRenderer, int &nTypePath)
	{
		ApplyStyle(pRenderer, nTypePath);

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand ( c_nPathType );

		pRenderer->PathCommandMoveTo(m_arValues[0], m_arValues[1]);
	}

	void CPolyline::DrawLines(IRenderer *pRenderer)
	{
		for (unsigned int unIndex = 2; unIndex < m_arValues.size(); unIndex += 2)
			pRenderer->PathCommandLineTo(m_arValues[unIndex + 0], m_arValues[unIndex + 1]);
	}

	void CPolyline::EndDraw(IRenderer *pRenderer, int &nTypePath)
	{
		pRenderer->DrawPath (nTypePath);
		pRenderer->EndCommand (c_nPathType);
		pRenderer->PathCommandEnd();
	}

	CPolygon::CPolygon(CObjectBase *pParent, CGeneralStyle *pBaseStyle)	: CPolyline(pParent, pBaseStyle)
	{}

	CPolygon::~CPolygon()
	{}

	void CPolygon::EndDraw(IRenderer *pRenderer, int &nTypePath)
	{
		pRenderer->PathCommandClose();

		pRenderer->DrawPath (nTypePath);
		pRenderer->EndCommand (c_nPathType);
		pRenderer->PathCommandEnd();
	}

}
