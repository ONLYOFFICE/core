#include "CEllipse.h"

#include "CStyle.h"
#include "CContainer.h"

namespace SVG
{
	CEllipse::CEllipse(CObjectBase *pParent) : CObjectBase(pParent)
	{}

	void CEllipse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
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

	bool CEllipse::Draw(IRenderer *pRenderer) const
	{
		if (NULL == pRenderer)
			return false;

		double dParentWidth = 0, dParentHeight = 0;
		CContainer *pContainer = dynamic_cast<CContainer*>(m_pParent);

		if (NULL != pContainer)
		{
			dParentWidth  = pContainer->GetWidth().ToDouble(NSCSS::Pixel);
			dParentHeight = pContainer->GetHeight().ToDouble(NSCSS::Pixel);
		}

		double dX  = m_oCx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY  = m_oCy.ToDouble(NSCSS::Pixel, dParentHeight);
		double dRx = m_oRx.ToDouble(NSCSS::Pixel, dParentWidth);
		double dRy = m_oRy.ToDouble(NSCSS::Pixel, dParentHeight);

		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);

		pRenderer->PathCommandStart();
		pRenderer->BeginCommand(c_nPathType);

		pRenderer->PathCommandStart();

		pRenderer->PathCommandMoveTo(dX + dRx, dY);
		pRenderer->PathCommandArcTo(dX - dRx, dY - dRy, dRx * 2.0, dRy * 2.0, 0, 360);

		pRenderer->DrawPath(nPathType);
		pRenderer->EndCommand(c_nPathType);

		pRenderer->PathCommandEnd();

		return true;
	}

	void CEllipse::ApplyStyle(IRenderer *pRenderer, int& nTypePath) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, nTypePath);
	}
}
