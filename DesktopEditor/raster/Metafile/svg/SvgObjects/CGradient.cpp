#include "CGradient.h"
#include "../CSvgFile.h"

namespace SVG
{
	CStopElement::CStopElement(XmlUtils::CXmlNode& oNode)
		: CObject(oNode)
	{}

	ObjectType CStopElement::GetType() const
	{
		return AppliedObject;
	}

	SvgDigit CStopElement::GetOffset() const
	{
		return m_oOffset;
	}

	SvgColor CStopElement::GetColor() const
	{
		return m_oColor;
	}

	void CStopElement::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"stop-color"))
			m_oColor.SetValue(mAttributes.at(L"stop-color"));

		if (mAttributes.end() != mAttributes.find(L"offset") && m_oOffset.SetValue(mAttributes.at(L"offset")) && m_oOffset.ToDouble() > 1.)
			m_oOffset /= 100;

		if (mAttributes.end() != mAttributes.find(L"stop-opacity"))
			m_oColor.SetOpacity(mAttributes.at(L"stop-opacity"));
	}

	CGradient::CGradient(XmlUtils::CXmlNode &oNode)
		: CAppliedObject(oNode)
	{
		m_wsXlinkHref = oNode.GetAttribute(L"href", oNode.GetAttribute(L"xlink:href"));
		m_oTransform.SetMatrix(oNode.GetAttribute(L"gradientTransform"), 0, true);

		if (L"userSpaceOnUse" == oNode.GetAttribute(L"gradientUnits"))
			m_enGradientUnits = GradU_UserSpaceOnUse;
		else
			m_enGradientUnits = GradU_ObjectBoundingBox;
	}

	void CGradient::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
	}

	bool CGradient::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (NULL == pRenderer)
			return false;

		if (m_arObjects.empty())
		{
			if (m_wsXlinkHref.empty() || NULL == pFile)
				return false;

			CGradient *pGradiend = dynamic_cast<CGradient*>(pFile->GetMarkedObject(m_wsXlinkHref));

			if (NULL == pGradiend)
				return false;
			
			pGradiend->Apply(pRenderer, pFile, oObjectBounds);
		}
		else
		{
			std::vector<LONG> arColors;
			std::vector<double> arPositions;
	
			for (const CStopElement* pStopElement : m_arObjects)
			{
				arColors.push_back(((unsigned int)(pStopElement->GetColor().ToInt() | ((unsigned char)(255. * pStopElement->GetColor().GetOpacity()) << 24))));
				arPositions.push_back(pStopElement->GetOffset().ToDouble());
			}
	
			pRenderer->put_BrushGradientColors(arColors.data(), arPositions.data(), arColors.size());
		}
		
		pRenderer->put_BrushTransform(m_oTransform.GetMatrix().GetFinalValue());
		
		return true;
	}
	
	void CGradient::ApplyTransform(IRenderer* pRenderer, const TBounds& oBounds, double& dAngle) const
	{
		if (NULL == pRenderer)
			return;
		
		TBounds oNewBounds(oBounds);
		Aggplus::CMatrix oMatrix = m_oTransform.GetMatrix().GetFinalValue();

		dAngle = oMatrix.rotation() * 180. / M_PI;

		if (GradU_ObjectBoundingBox == m_enGradientUnits)
		{
			oMatrix.Scale(oBounds.m_dRight - oBounds.m_dLeft, oBounds.m_dBottom - oBounds.m_dTop, Aggplus::MatrixOrderAppend);
			oMatrix.Scale(1 / (oBounds.m_dRight - oBounds.m_dLeft), 1 / (oBounds.m_dBottom - oBounds.m_dTop));
		}

		oMatrix.TransformPoint(oNewBounds.m_dLeft,  oNewBounds.m_dTop);
		oMatrix.TransformPoint(oNewBounds.m_dRight, oNewBounds.m_dBottom);

		pRenderer->BrushBounds(oNewBounds.m_dLeft, oNewBounds.m_dTop, oNewBounds.m_dRight - oNewBounds.m_dLeft, oNewBounds.m_dBottom - oNewBounds.m_dTop);
	}

	CLinearGradient::CLinearGradient(XmlUtils::CXmlNode& oNode)
		: CGradient(oNode)
	{
		m_oX1.SetValue(oNode.GetAttribute(L"x1"));
		m_oY1.SetValue(oNode.GetAttribute(L"y1"));
		m_oX2.SetValue(oNode.GetAttribute(L"x2"));
		m_oY2.SetValue(oNode.GetAttribute(L"y2"));
	}

	bool CLinearGradient::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (!CGradient::Apply(pRenderer, pFile, oObjectBounds))
			return false;

		pRenderer->put_BrushType(c_BrushTypePathGradient1);
		
		if (m_oX1 == m_oX2 && m_oY1 == m_oY2)
		{
			pRenderer->put_BrushType(c_BrushTypeSolid);
			pRenderer->put_BrushColor1(m_arObjects.back()->GetColor().ToInt());
			pRenderer->put_BrushAlpha1(m_arObjects.back()->GetOffset().ToInt());
			return true;
		}

		double dAngle = 0.;

		TBounds oNewBounds(oObjectBounds);

		if (GradU_ObjectBoundingBox == m_enGradientUnits)
		{
			oNewBounds.m_dLeft   += (oObjectBounds.m_dRight  - oObjectBounds.m_dLeft) * m_oX1.ToDouble(NSCSS::Pixel);
			oNewBounds.m_dTop    += (oObjectBounds.m_dBottom - oObjectBounds.m_dTop)  * m_oY1.ToDouble(NSCSS::Pixel);
			oNewBounds.m_dRight  *= (!m_oX2.Zero()) ? m_oX2.ToDouble(NSCSS::Pixel) : 1.;
			oNewBounds.m_dBottom *= (!m_oY2.Zero()) ? m_oY2.ToDouble(NSCSS::Pixel) : 1.;
		}
		else if (GradU_UserSpaceOnUse == m_enGradientUnits)
		{
			if (!m_oX1.Empty()) oNewBounds.m_dLeft   = m_oX1.ToDouble(NSCSS::Pixel);
			if (!m_oY1.Empty()) oNewBounds.m_dTop    = m_oY1.ToDouble(NSCSS::Pixel);
			if (!m_oX2.Empty()) oNewBounds.m_dRight  = m_oX2.ToDouble(NSCSS::Pixel);
			if (!m_oY2.Empty()) oNewBounds.m_dBottom = m_oY2.ToDouble(NSCSS::Pixel);
		}

		CGradient::ApplyTransform(pRenderer, oNewBounds, dAngle);

		dAngle += 180. * std::atan2(oNewBounds.m_dBottom - oNewBounds.m_dTop, oNewBounds.m_dRight - oNewBounds.m_dLeft) / M_PI;

		pRenderer->put_BrushLinearAngle(dAngle);

		return true;
	}

	CRadialGradient::CRadialGradient(XmlUtils::CXmlNode& oNode)
		: CGradient(oNode)
	{
		m_oCx.SetValue(oNode.GetAttribute(L"cx"));
		m_oCy.SetValue(oNode.GetAttribute(L"cy"));
		m_oR.SetValue(oNode.GetAttribute(L"r"));
	}

	bool CRadialGradient::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (!CGradient::Apply(pRenderer, pFile, oObjectBounds) || m_oR.Zero())
			return false;
		
		double dCX = (oObjectBounds.m_dRight + oObjectBounds.m_dLeft) / 2.;
		double dCY = (oObjectBounds.m_dBottom + oObjectBounds.m_dTop) / 2.;
		double dR  = oObjectBounds.m_dBottom - oObjectBounds.m_dTop;

		if (GradU_ObjectBoundingBox == m_enGradientUnits)
		{
			if (!m_oCx.Empty())
				dCX = oObjectBounds.m_dLeft + (oObjectBounds.m_dRight  - oObjectBounds.m_dLeft) * m_oCx.ToDouble(NSCSS::Pixel);
			if (!m_oCy.Empty())
				dCY = oObjectBounds.m_dTop + (oObjectBounds.m_dBottom - oObjectBounds.m_dTop) * m_oCy.ToDouble(NSCSS::Pixel);
			if (!m_oR.Empty())
				dR = (oObjectBounds.m_dBottom - oObjectBounds.m_dTop) * m_oR.ToDouble(NSCSS::Pixel);
		}
		else if (GradU_UserSpaceOnUse == m_enGradientUnits)
		{
			if (!m_oCx.Empty()) dCX = m_oCx.ToDouble(NSCSS::Pixel);
			if (!m_oCy.Empty()) dCY = m_oCy.ToDouble(NSCSS::Pixel);
			if (!m_oR.Empty())  dR  = m_oR .ToDouble(NSCSS::Pixel);
		}
		
		pRenderer->put_BrushType(c_BrushTypePathGradient2);
		pRenderer->BrushBounds(dCX - dR, dCY - dR, 2. * dR, 2. * dR);

		return true;
	}

}
