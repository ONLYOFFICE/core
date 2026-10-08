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
	    : CAppliedObject(oReader), m_enGradientUnits(GradU_None)
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
			else if (L"objectBoundingBox" == oReader.GetText())
				m_enGradientUnits = GradU_ObjectBoundingBox;
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
		InheritRefGradient(pFile);

		if (NULL == pRenderer || m_arObjects.empty())
			return false;

		LONG lAlpha{255};
		pRenderer->get_BrushAlpha1(&lAlpha);

		std::vector<LONG> arColors;
		std::vector<double> arPositions;

		for (const CStopElement* pStopElement : m_arObjects)
		{
			arColors.push_back(((unsigned int)(pStopElement->GetColor().ToInt() | ((unsigned char)(lAlpha * pStopElement->GetColor().GetOpacity()) << 24))));
			arPositions.push_back(pStopElement->GetOffset().ToDouble());
		}

		pRenderer->put_BrushGradientColors(arColors.data(), arPositions.data(), arColors.size());
		pRenderer->put_BrushTransform(m_oTransform.GetMatrix().GetFinalValue());

		return true;
	}

	CGradient *CGradient::GetRefGradient(const CSvgFile *pFile) const
	{
		if (m_wsXlinkHref.empty() || NULL == pFile)
			return NULL;

		return dynamic_cast<CGradient*>(pFile->GetMarkedObject(m_wsXlinkHref));
	}

	void CGradient::InheritRefGradient(const CSvgFile *pFile)
	{
		CGradient *pRefGradient = GetRefGradient(pFile);

		m_wsXlinkHref.clear();

		if (NULL != pRefGradient)
		{
			pRefGradient->InheritRefGradient(pFile);

			if (m_arObjects.empty())
			{
				for (CStopElement* pStopElement : pRefGradient->m_arObjects)
				{
					pStopElement->AddRef();
					m_arObjects.push_back(pStopElement);
				}
			}

			if (GradU_None == m_enGradientUnits)
				m_enGradientUnits = pRefGradient->m_enGradientUnits;

			if (m_oTransform.Empty())
				m_oTransform = pRefGradient->m_oTransform;
		}

		if (GradU_None == m_enGradientUnits)
			m_enGradientUnits = GradU_ObjectBoundingBox;
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
			return false;

		if (!m_oX2.Empty() && m_oX1 == m_oX2 && m_oY1 == m_oY2)
		{
			pRenderer->put_BrushType(c_BrushTypeSolid);
			pRenderer->put_BrushColor1(m_arObjects.back()->GetColor().ToInt());
			pRenderer->put_BrushAlpha1(m_arObjects.back()->GetOffset().ToInt());
			return true;
		}

		pRenderer->put_BrushType(c_BrushTypePathGradient1);

		const bool bObjectBoundingBox{GradU_ObjectBoundingBox == m_enGradientUnits};
		const double dBase{bObjectBoundingBox ? 1. : 0.};

		double dX1{m_oX1.ToDouble(NSCSS::Pixel, dBase)};
		double dY1{m_oY1.ToDouble(NSCSS::Pixel, dBase)};
		double dX2{m_oX2.ToDouble(NSCSS::Pixel, dBase)};
		double dY2{m_oY2.ToDouble(NSCSS::Pixel, dBase)};

		if (m_oX2.Empty())
			dX2 = bObjectBoundingBox ? 1. : oObjectBounds.m_dRight;

		double dBandX{dX1 - (dY2 - dY1)};
		double dBandY{dY1 + (dX2 - dX1)};

		const Aggplus::CMatrix oMatrix{m_oTransform.GetMatrix().GetFinalValue()};

		const auto ToUserSpace = [&](double& dX, double& dY)
		{
			oMatrix.TransformPoint(dX, dY);

			if (!bObjectBoundingBox)
				return;

			dX = oObjectBounds.m_dLeft + (oObjectBounds.m_dRight  - oObjectBounds.m_dLeft) * dX;
			dY = oObjectBounds.m_dTop  + (oObjectBounds.m_dBottom - oObjectBounds.m_dTop)  * dY;
		};

		ToUserSpace(dX1, dY1);
		ToUserSpace(dX2, dY2);
		ToUserSpace(dBandX, dBandY);

		dBandX -= dX1;
		dBandY -= dY1;

		double dWidth {dX2 - dX1};
		double dHeight{dY2 - dY1};

		const double dBandSquared{dBandX * dBandX + dBandY * dBandY};

		if (!Equals(0., dBandSquared))
		{
			const double dShift{(dWidth * dBandX + dHeight * dBandY) / dBandSquared};

			dWidth  -= dShift * dBandX;
			dHeight -= dShift * dBandY;
		}

		pRenderer->BrushBounds(std::min(dX1, dX1 + dWidth), std::min(dY1, dY1 + dHeight), std::abs(dWidth), std::abs(dHeight));
		pRenderer->put_BrushLinearAngle(180. * std::atan2(dHeight, dWidth) / M_PI);

		return true;
	}

	void CLinearGradient::InheritRefGradient(const CSvgFile *pFile)
	{
		const CLinearGradient *pRefGradient = dynamic_cast<const CLinearGradient*>(GetRefGradient(pFile));

		CGradient::InheritRefGradient(pFile);

		if (NULL == pRefGradient)
			return;

		if (m_oX1.Empty()) m_oX1 = pRefGradient->m_oX1;
		if (m_oY1.Empty()) m_oY1 = pRefGradient->m_oY1;
		if (m_oX2.Empty()) m_oX2 = pRefGradient->m_oX2;
		if (m_oY2.Empty()) m_oY2 = pRefGradient->m_oY2;
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
			return false;

		double dCX = (oObjectBounds.m_dRight + oObjectBounds.m_dLeft) / 2.;
		double dCY = (oObjectBounds.m_dBottom + oObjectBounds.m_dTop) / 2.;
		double dR  = (oObjectBounds.m_dBottom - oObjectBounds.m_dTop) / 2.;

		if (GradU_ObjectBoundingBox == m_enGradientUnits)
		{
			if (!m_oCx.Empty())
				dCX = oObjectBounds.m_dLeft + (oObjectBounds.m_dRight  - oObjectBounds.m_dLeft) * m_oCx.ToDouble(NSCSS::Pixel, 1.);
			if (!m_oCy.Empty())
				dCY = oObjectBounds.m_dTop + (oObjectBounds.m_dBottom - oObjectBounds.m_dTop) * m_oCy.ToDouble(NSCSS::Pixel, 1.);
			if (!m_oR.Empty())
				dR = (oObjectBounds.m_dBottom - oObjectBounds.m_dTop) * m_oR.ToDouble(NSCSS::Pixel, 1.);
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

	void CRadialGradient::InheritRefGradient(const CSvgFile *pFile)
	{
		const CRadialGradient *pRefGradient = dynamic_cast<const CRadialGradient*>(GetRefGradient(pFile));

		CGradient::InheritRefGradient(pFile);

		if (NULL == pRefGradient)
			return;

		if (m_oCx.Empty()) m_oCx = pRefGradient->m_oCx;
		if (m_oCy.Empty()) m_oCy = pRefGradient->m_oCy;
		if (m_oR.Empty())  m_oR  = pRefGradient->m_oR;
	}

}
