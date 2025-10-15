#include "CContainer.h"

#include "../CSvgFile.h"
#include "../SvgUtils.h"

namespace SVG
{
	CGraphicsContainer::CGraphicsContainer(CSvgReader& oReader, CRenderedObject *pParent)
		: CRenderedObject(oReader, pParent)
	{}

	void CGraphicsContainer::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("x" == sName)
			m_oWindow.m_oX.SetValue(oReader.GetDouble());
		else if ("y" == sName)
			m_oWindow.m_oY.SetValue(oReader.GetDouble());
		else if ("width" == sName)
			m_oWindow.m_oWidth.SetValue(oReader.GetDouble());
		else if ("height" == sName)
			m_oWindow.m_oHeight.SetValue(oReader.GetDouble());
		else if ("viewBox" == sName)
		{
			const std::vector<double> arValues{StrUtils::ReadDoubleValues(oReader.GetText())};
			if (4 == arValues.size())
			{
				m_oViewBox.m_oX      = arValues[0];
				m_oViewBox.m_oY      = arValues[1];
				m_oViewBox.m_oWidth  = arValues[2];
				m_oViewBox.m_oHeight = arValues[3];
			}
		}
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	bool CGraphicsContainer::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform, CommandeModeDraw))
			return false;

		for (const CRenderedObject* pObject : m_arObjects)
			pObject->Draw(pRenderer, pFile, oMode, pOtherStyles, pContexObject);

		EndPath(pRenderer, pFile, oOldTransform, CommandeModeDraw, pOtherStyles, pContexObject);

		return true;
	}

	TRect CGraphicsContainer::GetWindow() const
	{
		return m_oWindow;
	}

	TRect CGraphicsContainer::GetViewBox() const
	{
		if (m_oViewBox.m_oWidth.Empty() || m_oViewBox.m_oHeight.Empty())
			return m_oWindow;

		return m_oViewBox;
	}

	TBounds CGraphicsContainer::GetBounds() const
	{
		TBounds oBounds, oTempBounds;

		oBounds.m_dLeft    = oBounds.m_dRight  = m_oWindow.m_oX.ToDouble(NSCSS::Pixel);
		oBounds.m_dTop     = oBounds.m_dBottom = m_oWindow.m_oY.ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  += m_oWindow.m_oWidth.ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom += m_oWindow.m_oHeight.ToDouble(NSCSS::Pixel);

		for (const CRenderedObject* pObject : m_arObjects)
		{
			oTempBounds = pObject->GetBounds();
			oBounds.m_dLeft   = std::min(oBounds.m_dLeft, oTempBounds.m_dLeft);
			oBounds.m_dTop    = std::min(oBounds.m_dTop, oTempBounds.m_dTop);
			oBounds.m_dRight  = std::max(oBounds.m_dRight, oTempBounds.m_dRight);
			oBounds.m_dBottom = std::max(oBounds.m_dBottom, oTempBounds.m_dBottom);
		}

		return oBounds;
	}

}
