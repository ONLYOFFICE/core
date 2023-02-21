#include "CLine.h"

#include "CStyle.h"
#include "CContainer.h"

namespace SVG
{
	CLine::CLine(XmlUtils::CXmlNode& oNode, CSvgGraphicsObject* pParent)
	    : CSvgGraphicsObject(oNode, pParent)
	{}

	void CLine::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"x1"))
			m_oX1.SetValue(mAttributes.at(L"x1"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y1"))
			m_oY1.SetValue(mAttributes.at(L"y1"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"x2"))
			m_oX2.SetValue(mAttributes.at(L"x2"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y2"))
			m_oY2.SetValue(mAttributes.at(L"y2"), ushLevel, bHardMode);
	}

	bool CLine::Draw(IRenderer *pRenderer, const CDefs *pDefs) const
	{
		if (NULL == pRenderer)
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dParentWidth  = oBounds.m_dRight  - oBounds.m_dLeft;
		double dParentHeight = oBounds.m_dBottom - oBounds.m_dTop;

		double dX1 = m_oX1.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY1 = m_oY1.ToDouble(NSCSS::Pixel, dParentHeight);
		double dX2 = m_oX2.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY2 = m_oY2.ToDouble(NSCSS::Pixel, dParentHeight);

		int nPathType = 0;
		Aggplus::CMatrix oOldMatrix(1., 0., 0., 1., 0, 0);

		ApplyStyle(pRenderer, pDefs, nPathType, oOldMatrix);

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand(c_nPathType);

		pRenderer->PathCommandStart();

		pRenderer->PathCommandMoveTo(dX1, dY1);
		pRenderer->PathCommandLineTo(dX2, dY2);

		pRenderer->DrawPath(nPathType);
		pRenderer->EndCommand(c_nPathType);
		pRenderer->PathCommandEnd();

		pRenderer->SetTransform(oOldMatrix.sx(), oOldMatrix.shy(), oOldMatrix.shx(), oOldMatrix.sy(), oOldMatrix.tx(), oOldMatrix.ty());

		return true;
	}

	void CLine::ApplyStyle(IRenderer *pRenderer, const CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyStroke(pRenderer, nTypePath);
	}

	TBounds CLine::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oX1.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oY1.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = m_oX2.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = m_oY2.ToDouble(NSCSS::Pixel);

		return oBounds;
	}
}
