#include "CPolyline.h"

namespace SVG
{
	CPolyline::CPolyline(CObjectBase *pParent) : CObjectBase(pParent)
	{}

	CPolyline::~CPolyline()
	{}

	void CPolyline::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
	}

	bool CPolyline::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return false;

		std::wstring wsValue = oNode.GetAttribute(L"points");

		if (wsValue.empty())
			return false;

		m_arValues = NSCSS::NS_STATIC_FUNCTIONS::ReadDoubleValues(wsValue);

		SaveNodeData(oNode);

		return m_arValues.size() >= 4;
	}

	bool CPolyline::Draw(IRenderer *pRenderer, CDefs *pDefs) const
	{
		if (NULL == pRenderer || m_arValues.size() < 4)
			return false;

		int nPathType = 0;
		Aggplus::CMatrix oOldMatrix(1., 0., 0., 1., 0, 0);

		BeginDraw(pRenderer, pDefs, nPathType, oOldMatrix);
		DrawLines(pRenderer);
		EndDraw(pRenderer, nPathType);

		pRenderer->SetTransform(oOldMatrix.sx(), oOldMatrix.shy(), oOldMatrix.shx(), oOldMatrix.sy(), oOldMatrix.tx(), oOldMatrix.ty());

		return true;
	}

	void CPolyline::ApplyStyle(IRenderer *pRenderer, CDefs *pDefs, int &nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, pDefs, nTypePath, true);
	}

	void CPolyline::BeginDraw(IRenderer *pRenderer, CDefs *pDefs, int &nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		ApplyStyle(pRenderer, pDefs, nTypePath, oOldMatrix);

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

	CPolygon::CPolygon(CObjectBase *pParent) : CPolyline(pParent)
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
