#include "CEllipse.h"

#include "CStyle.h"
#include "CContainer.h"

namespace SVG
{
	CEllipse::CEllipse(CObjectBase *pParent) : CObjectBase(pParent)
	{}

	void CEllipse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"cx"))
			m_oCx.SetValue(mAttributes.at(L"cx"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"cy"))
			m_oCy.SetValue(mAttributes.at(L"cy"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"rx"))
			m_oRx.SetValue(mAttributes.at(L"rx"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"ry"))
			m_oRy.SetValue(mAttributes.at(L"ry"), ushLevel, bHardMode);
	}

	bool CEllipse::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return false;

		SaveNodeData(oNode);

		return true;
	}

	bool CEllipse::Draw(IRenderer *pRenderer, CDefs *pDefs) const
	{
		if (NULL == pRenderer)
			return false;

		double dParentWidth = 0, dParentHeight = 0;
		CContainer *pContainer = dynamic_cast<CContainer*>(m_pParent);

		if (NULL != pContainer)
		{
			dParentWidth  = pContainer->GetWindow().m_oWidth.ToDouble(NSCSS::Pixel);
			dParentHeight = pContainer->GetWindow().m_oWidth.ToDouble(NSCSS::Pixel);
		}

		double dX  = m_oCx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY  = m_oCy.ToDouble(NSCSS::Pixel, dParentHeight);
		double dRx = m_oRx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dRy = m_oRy.ToDouble(NSCSS::Pixel, dParentHeight);

		int nPathType = 0;
		Aggplus::CMatrix oOldMatrix(1., 0., 0., 1., 0, 0);

		ApplyStyle(pRenderer, pDefs, nPathType, oOldMatrix);

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand(c_nPathType);

		pRenderer->PathCommandStart();

		pRenderer->PathCommandMoveTo(dX + dRx, dY);
		pRenderer->PathCommandArcTo(dX - dRx, dY - dRy, dRx * 2.0, dRy * 2.0, 0, 360);

		pRenderer->DrawPath(nPathType);
		pRenderer->EndCommand(c_nPathType);

		pRenderer->PathCommandEnd();

		pRenderer->SetTransform(oOldMatrix.sx(), oOldMatrix.shy(), oOldMatrix.shx(), oOldMatrix.sy(), oOldMatrix.tx(), oOldMatrix.ty());

		return true;
	}

	void CEllipse::ApplyStyle(IRenderer *pRenderer, CDefs *pDefs, int& nTypePath, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer, oOldMatrix);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, pDefs, nTypePath);
	}

	TBounds CEllipse::GetBounds() const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = m_oCx.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = m_oCy.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = oBounds.m_dLeft + m_oRx.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = oBounds.m_dTop  + m_oRy.ToDouble(NSCSS::Pixel);;

		return oBounds;
	}
}
