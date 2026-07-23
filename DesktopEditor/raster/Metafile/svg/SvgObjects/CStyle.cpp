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

#include "CStyle.h"
#include "CObjectBase.h"

#include "../../../../../Common/3dParty/html/css/src/CCssCalculator_Private.h"

namespace SVG
{
	CSvgCalculator::CSvgCalculator()
	    : m_pInternal(new NSCSS::CCssCalculator_Private)
	{
		m_pInternal->SetDpi(96);
	}

	CSvgCalculator::~CSvgCalculator()
	{
		if (NULL != m_pInternal)
			delete m_pInternal;
	}

	void CSvgCalculator::AddStyles(const std::wstring &wsStyles)
	{
		m_pInternal->AddStyles(wsStyles);
	}

	void CSvgCalculator::Clear()
	{
		m_pInternal->Clear();
	}

	void CSvgCalculator::SetData(CObject *pSvgObject) const
	{
		if (NULL == pSvgObject)
			return;

		const std::vector<NSCSS::CNode> arSelectors = pSvgObject->GetFullPath();

		std::vector<std::wstring> arNodes = m_pInternal->CalculateAllNodes(arSelectors, 0, arSelectors.size());
		std::vector<std::wstring> arPrevNodes;

		for (size_t i = 0; i < arSelectors.size(); ++i)
		{
			pSvgObject->SetData(arSelectors[i].m_mAttributes, i + 1);

			for (const NSCSS::CElement* oElement : m_pInternal->FindElements(arNodes, arPrevNodes))
				pSvgObject->SetData(oElement->GetStyle(), i + 1);

			if (!arSelectors[i].m_wsStyle.empty())
				pSvgObject->SetData(arSelectors[i].m_wsStyle, i + 1, true);
		}

		return;
	}
}
