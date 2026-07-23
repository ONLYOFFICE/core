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

#include "GraphicUnit.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"

#ifdef BUILDING_WASM_MODULE
#include "../../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#endif

namespace OFD
{
CGraphicUnit::CGraphicUnit(CXmlReader& oReader)
	: m_bVisible(true), m_unDrawParam(0), m_oPenSettings(oReader)
{
	if (0 == oReader.GetAttributesCount() || !oReader.MoveToFirstAttribute())
		return;

	std::wstring wsAttributeName;

	do
	{
		wsAttributeName = oReader.GetName();

		if (L"Boundary" == wsAttributeName)
			m_oBoundary.Read(oReader.GetTextA());
		else if (L"Name" == wsAttributeName)
			m_wsName = oReader.GetText();
		else if (L"Visible" == wsAttributeName)
			m_bVisible = oReader.GetBoolean(true);
		else if (L"CTM" == wsAttributeName)
			m_oCTM.Read(oReader.GetTextA());
		else if (L"DrawParam" == wsAttributeName)
			m_unDrawParam = oReader.GetUInteger(true);
	} while (oReader.MoveToNextAttribute());

	oReader.MoveToElement();
}

void CGraphicUnit::Apply(IRenderer* pRenderer, TMatrix& oOldTransform) const
{
	if (nullptr == pRenderer)
		return;

	m_oPenSettings.Apply(pRenderer);

	pRenderer->GetTransform(&oOldTransform.m_dM11, &oOldTransform.m_dM12, &oOldTransform.m_dM21, &oOldTransform.m_dM22, &oOldTransform.m_dDx, &oOldTransform.m_dDy);

	Aggplus::CMatrix oTransform(oOldTransform.m_dM11, oOldTransform.m_dM12, oOldTransform.m_dM21, oOldTransform.m_dM22, oOldTransform.m_dDx, oOldTransform.m_dDy);
	const Aggplus::CMatrix oCurrentTransform(m_oCTM.m_dM11, m_oCTM.m_dM12, m_oCTM.m_dM21, m_oCTM.m_dM22, m_oBoundary.m_dX + m_oCTM.m_dDx, m_oBoundary.m_dY + m_oCTM.m_dDy);

	oTransform.Multiply(&oCurrentTransform);

	// Clipping
	pRenderer->put_ClipMode(c_nClipRegionTypeWinding | c_nClipRegionIntersect);

	pRenderer->BeginCommand(c_nResetClipType);
	pRenderer->EndCommand(c_nResetClipType);

	if (!m_oBoundary.Empty())
	{
		pRenderer->BeginCommand(c_nClipType);
		pRenderer->BeginCommand(c_nPathType);
		pRenderer->PathCommandStart();

		pRenderer->PathCommandMoveTo(m_oBoundary.m_dX, m_oBoundary.m_dY);
		pRenderer->PathCommandLineTo(m_oBoundary.m_dX + m_oBoundary.m_dWidth, m_oBoundary.m_dY);
		pRenderer->PathCommandLineTo(m_oBoundary.m_dX + m_oBoundary.m_dWidth, m_oBoundary.m_dY + m_oBoundary.m_dHeight);
		pRenderer->PathCommandLineTo(m_oBoundary.m_dX, m_oBoundary.m_dY + m_oBoundary.m_dHeight);
		pRenderer->PathCommandLineTo(m_oBoundary.m_dX, m_oBoundary.m_dY);

		pRenderer->EndCommand(c_nPathType);
		pRenderer->EndCommand(c_nClipType);
		pRenderer->PathCommandEnd();
	}

	pRenderer->SetTransform(oTransform.sx(), oTransform.shy(), oTransform.shx(), oTransform.sy(), oTransform.tx(), oTransform.ty());
}

void CGraphicUnit::ReadChildren(CXmlReader& oReader)
{
	if ("ofd:Actions" == oReader.GetNameA())
	{
		const int nActionDepth{oReader.GetDepth()};

		while (oReader.ReadNextSiblingNode(nActionDepth))
		{
			if("ofd:Action" == oReader.GetNameA())
				AddAction(new CAction(oReader));
		}
	}
}

TBox CGraphicUnit::GetBoundary() const
{
	return m_oBoundary;
}

#ifdef BUILDING_WASM_MODULE
void CGraphicUnit::GetLinks(NSWasm::CData& oRes) const
{
	for (const CAction* pActionElement : m_arActions)
	{
		const IAction* pAction{pActionElement->GetAction()};

		if (nullptr == pAction || (EActionType::Goto != pAction->GetType() && EActionType::URI != pAction->GetType()))
			continue;

		if (EActionType::URI == pAction->GetType())
			oRes.WriteString(((const CURI*)pAction)->GetURI());
		else if (EActionType::Goto == pAction->GetType())
		{
			const TBookmark* pBookMark{((const CGoto*)pAction)->GetBookmark()};

			if (nullptr == pBookMark)
				continue;

			oRes.WriteString(pBookMark->m_wsName);
		}

		oRes.WriteDouble(0.);
		oRes.WriteDouble(m_oBoundary.m_dX);
		oRes.WriteDouble(m_oBoundary.m_dY);
		oRes.WriteDouble(m_oBoundary.m_dWidth);
		oRes.WriteDouble(m_oBoundary.m_dHeight);
	}
}
#endif

void CGraphicUnit::AddAction(const CAction* pAction)
{
	if (nullptr != pAction)
		m_arActions.push_back(pAction);
}

}
