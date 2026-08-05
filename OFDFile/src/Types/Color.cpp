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

#include "Color.h"

#include "../Res.h"

namespace OFD
{
CColor::CColor(CXmlReader& oXmlReader)
    : m_oValues{0, 0, 0, 0}, m_nIndex(0), m_chAlpha(255), m_unColodSpaceID(0)
{
	if (0 == oXmlReader.GetAttributesCount() || !oXmlReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;

	do
	{
		sAttributeName = oXmlReader.GetNameA();

		if ("ColorSpace" == sAttributeName)
			m_unColodSpaceID = oXmlReader.GetUInteger(true);
		else if ("Value" == sAttributeName)
		{
			const std::vector<double> arValues{oXmlReader.GetArrayDoubles(true)};

			for (unsigned int unIndex = 0; unIndex < (std::min)((unsigned int)arValues.size(), (unsigned int)4); ++unIndex)
				m_oValues[unIndex] = static_cast<BYTE>(arValues[unIndex]);
		}
		else if ("Alpha" == sAttributeName)
			m_chAlpha = (std::max)((BYTE)0, (BYTE)(std::min)((unsigned int)255, oXmlReader.GetUInteger(true)));
		else if ("Index" == sAttributeName)
			m_nIndex = oXmlReader.GetInteger(true);
	} while (oXmlReader.MoveToNextAttribute());

	oXmlReader.MoveToElement();
}

int CColor::ToInt(const CRes* pPublicRes) const
{
	if (nullptr == pPublicRes)
		return BLACK_COLOR_INT;

	const CColorSpace* pColorSpace = pPublicRes->GetColorSpace(m_unColodSpaceID);

	CColorSpace::EType eColoSpaceType{CColorSpace::EType::RGB};

	if (nullptr != pColorSpace)
		eColoSpaceType = pColorSpace->GetType();

	switch(eColoSpaceType)
	{
		case CColorSpace::EType::GRAY:
			return (255 << 24) | (m_oValues[0] << 16) | (m_oValues[0] << 8) | (m_oValues[0] << 0);
		case CColorSpace::EType::RGB:
			return (255 << 24) | (m_oValues[2] << 16) | (m_oValues[1] << 8) | (m_oValues[0] << 0);
		case CColorSpace::EType::CMYK:
		{
			const float cF = m_oValues[0] / 255.0f;
			const float mF = m_oValues[1] / 255.0f;
			const float yF = m_oValues[2] / 255.0f;
			const float kF = m_oValues[3] / 255.0f;

			const float r = (1.0f - cF) * (1.0f - kF);
			const float g = (1.0f - mF) * (1.0f - kF);
			const float b = (1.0f - yF) * (1.0f - kF);

			const unsigned char rByte = static_cast<unsigned char>(r * 255);
			const unsigned char gByte = static_cast<unsigned char>(g * 255);
			const unsigned char bByte = static_cast<unsigned char>(b * 255);

			return (rByte << 0) | (gByte << 8) | (bByte << 16) | (255 << 24);
		}
	}

	return BLACK_COLOR_INT;
}

BYTE CColor::GetAlpha() const
{
	return m_chAlpha;
}
}
