#include "CPolyline.h"

namespace SVG
{
	CPolyline::CPolyline(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{}

	void CPolyline::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"points"))
			m_arValues = NSCSS::NS_STATIC_FUNCTIONS::ReadDoubleValues(mAttributes.at(L"points"));
	}

	bool CPolyline::Draw(IRenderer *pRenderer, const CDefs *pDefs) const
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

	void CPolyline::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, pDefs, nTypePath, true);
	}

	TBounds CPolyline::GetBounds() const
	{
		TBounds oBounds{0., 0., 0., 0.};

		for (unsigned int unIndex = 2; unIndex < m_arValues.size(); unIndex += 2)
		{
			oBounds.m_dLeft   = std::min(oBounds.m_dLeft,   m_arValues[unIndex + 0]);
			oBounds.m_dTop    = std::min(oBounds.m_dTop,    m_arValues[unIndex + 1]);
			oBounds.m_dRight  = std::max(oBounds.m_dRight,  m_arValues[unIndex + 0]);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, m_arValues[unIndex + 1]);
		}

		return oBounds;
	}

	void CPolyline::BeginDraw(IRenderer *pRenderer, const CDefs *pDefs, int &nTypePath, Aggplus::CMatrix& oOldMatrix) const
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

	CPolygon::CPolygon(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CPolyline(oNode, pParent)
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
