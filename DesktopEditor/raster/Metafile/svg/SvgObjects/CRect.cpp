#include "CRect.h"
#include "CStyle.h"
#include "CContainer.h"
#include "../SvgTypes.h"

namespace SVG
{
	CRect::CRect(CObjectBase *pParent) : CObjectBase(pParent)
	{}

	CRect::~CRect()
	{}

	void CRect::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"x"))
			m_oX.SetValue(mAttributes.at(L"x"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"y"))
			m_oY.SetValue(mAttributes.at(L"y"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"width"))
			m_oWidth.SetValue(mAttributes.at(L"width"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"height"))
			m_oHeight.SetValue(mAttributes.at(L"height"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"rx"))
			m_oRx.SetValue(mAttributes.at(L"rx"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"ry"))
			m_oRy.SetValue(mAttributes.at(L"ry"), ushLevel, bHardMode);
	}

	bool CRect::ReadFromXmlNode(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return false;

		SaveNodeData(oNode);

		return true;
	}

	bool CRect::Draw(IRenderer *pRenderer) const
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

		double dX      = m_oX.ToDouble(NSCSS::Pixel, dParentWidth);
		double dY      = m_oY.ToDouble(NSCSS::Pixel, dParentHeight);
		double dWidth  = m_oWidth.ToDouble(NSCSS::Pixel, dParentWidth);
		double dHeight = m_oHeight.ToDouble(NSCSS::Pixel, dParentHeight);

		int nPathType = 0;

		ApplyStyle(pRenderer, nPathType);

		if (m_oRx.Empty() && m_oRy.Empty())
		{
			pRenderer->PathCommandStart();
			pRenderer->BeginCommand(c_nPathType);

			pRenderer->PathCommandStart();

			pRenderer->PathCommandMoveTo(dX, dY);
			pRenderer->PathCommandLineTo(dX + dWidth, dY);
			pRenderer->PathCommandLineTo(dX + dWidth, dY + dHeight);
			pRenderer->PathCommandLineTo(dX, dY + dHeight);
			pRenderer->PathCommandClose();

			pRenderer->DrawPath(nPathType);
			pRenderer->EndCommand(c_nPathType);
			pRenderer->PathCommandEnd();
		}
		else
		{
			double dRx = m_oRx.ToDouble(NSCSS::Pixel);
			double dRy = m_oRy.ToDouble(NSCSS::Pixel);

			pRenderer->PathCommandStart();
			pRenderer->BeginCommand(c_nPathType);

			pRenderer->PathCommandStart();

			pRenderer->PathCommandMoveTo(dX + dRx, dY);

			pRenderer->PathCommandLineTo(dX - dRx, dY);
			pRenderer->PathCommandArcTo(dX - dRx * 2.0, dY, dRx * 2.0, dRy * 2.0, 270.0, 90.0);

			pRenderer->PathCommandLineTo(dX, dY + dHeight - dRy);
			pRenderer->PathCommandArcTo(dX - dRx * 2.0, dY + dHeight - dRy * 2.0, dRx * 2.0, dRy * 2.0, 0.0, 90.0);

			pRenderer->PathCommandLineTo(dX + dWidth + dRx, dY + dHeight);
			pRenderer->PathCommandArcTo(dX + dWidth, dY + dHeight - dRy * 2.0, dRx * 2.0, dRy * 2.0, 90.0, 90.0);

			pRenderer->PathCommandLineTo(dX + dWidth, dY + dRy);
			pRenderer->PathCommandArcTo(dX + dWidth, dY, dRx * 2.0, dRy * 2.0, 180.0, 90.0);

			pRenderer->DrawPath(nPathType);
			pRenderer->EndCommand(c_nPathType);
			pRenderer->PathCommandEnd ();
		}

		return true;
	}

	void CRect::ApplyStyle(IRenderer *pRenderer, int& nTypePath) const
	{
		if (NULL == pRenderer)
			return;

		ApplyTransform(pRenderer);
		ApplyStroke(pRenderer, nTypePath);
		ApplyFill(pRenderer, nTypePath);
	}

}
