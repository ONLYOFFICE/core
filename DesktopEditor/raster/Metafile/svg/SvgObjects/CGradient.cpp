/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "CGradient.h"
#include "../CSvgFile.h"

namespace SVG
{
	CStopElement::CStopElement(CSvgReader& oReader)
		: CObject(oReader)
	{}

	ObjectType CStopElement::GetType() const
	{
		return DataObject;
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

	CGradient::CGradient(CSvgReader& oReader)
	    : CAppliedObject(oReader), m_enGradientUnits(GradU_ObjectBoundingBox)
	{}

	void CGradient::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("href" == sName || "xlink:href" == sName)
			m_wsXlinkHref = oReader.GetText();
		else if ("gradientTransform" == sName)
			m_oTransform.SetMatrix(oReader.GetText(), 0, true);
		else if ("gradientUnits" == sName)
		{
			if (L"userSpaceOnUse" == oReader.GetText())
				m_enGradientUnits = GradU_UserSpaceOnUse;
		}
		else
			CAppliedObject::SetAttribute(sName, oReader);
	}

	void CGradient::SetData(const std::map<std::wstring, std::wstring>& mAttributes, unsigned short ushLevel, bool bHardMode)
	{}

	void CGradient::ReadChildrens(CSvgReader& oReader, CSvgFile* pSvgFile)
	{
		if (NULL == pSvgFile || NULL == pSvgFile->GetSvgCalculator())
			return;

		WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "stop")
			AddObject(CObject::Create<CStopElement>(oReader, pSvgFile));
		END_WHILE
	}

	bool CGradient::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (NULL == pRenderer || m_arObjects.empty())
			return false;

		std::vector<LONG> arColors;
		std::vector<double> arPositions;

		for (const CStopElement* pStopElement : m_arObjects)
		{
			arColors.push_back(((unsigned int)(pStopElement->GetColor().ToInt() | ((unsigned char)(255. * pStopElement->GetColor().GetOpacity()) << 24))));
			arPositions.push_back(pStopElement->GetOffset().ToDouble());
		}

		pRenderer->put_BrushGradientColors(arColors.data(), arPositions.data(), arColors.size());
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

	CGradient *CGradient::GetRefGradient(const CSvgFile *pFile) const
	{
		if (m_wsXlinkHref.empty() || NULL == pFile)
			return NULL;

		CGradient *pGradiend = dynamic_cast<CGradient*>(pFile->GetMarkedObject(m_wsXlinkHref));

		if (NULL == pGradiend)
			return NULL;

		CGradient *pRefGradient = pGradiend->GetRefGradient(pFile);

		return (NULL != pRefGradient) ? pRefGradient : pGradiend;
	}

	bool CGradient::ApplyRefGradient(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds) const
	{
		CGradient *pRefGradient = GetRefGradient(pFile);

		if (NULL == pRefGradient)
			return false;

		return pRefGradient->Apply(pRenderer, pFile, oObjectBounds);
	}

	CLinearGradient::CLinearGradient(CSvgReader& oReader)
	    : CGradient(oReader)
	{}

	void CLinearGradient::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("x1" == sName)
			m_oX1.SetValue(oReader.GetText());
		else if ("y1" == sName)
			m_oY1.SetValue(oReader.GetText());
		else if ("x2" == sName)
			m_oX2.SetValue(oReader.GetText());
		else if ("y2" == sName)
			m_oY2.SetValue(oReader.GetText());
		else
			CGradient::SetAttribute(sName, oReader);
	}

	bool CLinearGradient::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (!CGradient::Apply(pRenderer, pFile, oObjectBounds))
			return ApplyRefGradient(pRenderer, pFile, oObjectBounds);

		if (m_oX1 == m_oX2 && m_oY1 == m_oY2)
		{
			pRenderer->put_BrushType(c_BrushTypeSolid);
			pRenderer->put_BrushColor1(m_arObjects.back()->GetColor().ToInt());
			pRenderer->put_BrushAlpha1(m_arObjects.back()->GetOffset().ToInt());
			return true;
		}

		pRenderer->put_BrushType(c_BrushTypePathGradient1);

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

	CRadialGradient::CRadialGradient(CSvgReader& oReader)
	    : CGradient(oReader)
	{}

	void CRadialGradient::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("cx" == sName)
			m_oCx.SetValue(oReader.GetText());
		else if ("cy" == sName)
			m_oCy.SetValue(oReader.GetText());
		else if ("r" == sName)
			m_oR.SetValue(oReader.GetText());
		else
			CGradient::SetAttribute(sName, oReader);
	}

	bool CRadialGradient::Apply(IRenderer *pRenderer, const CSvgFile *pFile, const TBounds &oObjectBounds)
	{
		if (!CGradient::Apply(pRenderer, pFile, oObjectBounds) || m_oR.Zero())
			return ApplyRefGradient(pRenderer, pFile, oObjectBounds);

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
