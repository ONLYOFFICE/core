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

#include "CCircle.h"

#include "CContainer.h"

namespace SVG
{
	CCircle::CCircle(CSvgReader& oReader, CRenderedObject* pParent)
		: CRenderedObject(oReader, pParent)
	{
		START_READ_ATTRIBUTES(oReader)
			SetAttribute(sAttributeName, oReader);
		END_READ_ATTRIBUTES(oReader)
	}

	void CCircle::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("cx" == sName)
			m_oCx.SetValue(oReader.GetText());
		else if ("cy" == sName)
			m_oCy.SetValue(oReader.GetText());
		else if ("r" == sName)
			m_oR.SetValue(oReader.GetText());
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	void CCircle::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
	}

	bool CCircle::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles *pOtherStyles, const CRenderedObject* pContexObject) const
	{
		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform, oMode))
			return false;

		TBounds oBounds = (NULL != m_pParent) ? m_pParent->GetBounds() : TBounds{0., 0., 0., 0.};

		double dX = m_oCx.ToDouble(NSCSS::Pixel, oBounds.m_dRight  - oBounds.m_dLeft);
		double dY = m_oCy.ToDouble(NSCSS::Pixel, oBounds.m_dBottom - oBounds.m_dTop);
		double dR = m_oR .ToDouble(NSCSS::Pixel);

		pRenderer->PathCommandMoveTo(dX + dR, dY);
		pRenderer->PathCommandArcTo(dX - dR, dY - dR, dR * 2.0, dR * 2.0, 0, 360);

		EndPath(pRenderer, pFile, oOldTransform, oMode, pOtherStyles, pContexObject);

		return true;
	}

	void CCircle::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath, const CRenderedObject* pContexObject) const
	{
		if (ApplyStroke(pRenderer, &pStyles->m_oStroke, false, pContexObject))
			nTypePath += c_nStroke;

		if (ApplyFill(pRenderer, &pStyles->m_oFill, pFile, true, pContexObject))
			nTypePath += c_nWindingFillMode;
	}

	TBounds CCircle::GetBounds(SvgMatrix* pTransform) const
	{
		TBounds oBounds;

		oBounds.m_dLeft   = (m_oCx - m_oR).ToDouble(NSCSS::Pixel);
		oBounds.m_dTop    = (m_oCy - m_oR).ToDouble(NSCSS::Pixel);
		oBounds.m_dRight  = (m_oCx + m_oR).ToDouble(NSCSS::Pixel);
		oBounds.m_dBottom = (m_oCy + m_oR).ToDouble(NSCSS::Pixel);

		if (nullptr != pTransform)
		{
			*pTransform += m_oTransformation.m_oTransform.GetMatrix();

			pTransform->GetFinalValue().TransformPoint(oBounds.m_dLeft,  oBounds.m_dTop   );
			pTransform->GetFinalValue().TransformPoint(oBounds.m_dRight, oBounds.m_dBottom);

			*pTransform -= m_oTransformation.m_oTransform.GetMatrix();
		}

		return oBounds;
	}

}
