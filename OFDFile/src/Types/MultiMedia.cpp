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

#include "MultiMedia.h"

#include "../Utils/Utils.h"

namespace OFD
{
CMultiMedia::CMultiMedia(CXmlReader& oXmlReader, const std::wstring& wsRootPath)
	: IOFDElement(oXmlReader), m_eType(EType::Image)
{
	if (0 != oXmlReader.GetAttributesCount() && oXmlReader.MoveToFirstAttribute())
	{
		std::string sArgumentName;
		do
		{
			sArgumentName = oXmlReader.GetNameA();

			 if ("Type" == sArgumentName)
			{
				const std::string sValue{oXmlReader.GetTextA()};

				if ("Image" == sValue)
					m_eType = EType::Image;
				else if ("Audio" == sValue)
					m_eType = EType::Audio;
				else if ("Video" == sValue)
					m_eType = EType::Video;
			}
		} while (oXmlReader.MoveToNextAttribute());
	}

	oXmlReader.MoveToElement();

	if (oXmlReader.IsEmptyNode())
		return;

	const int nDepth = oXmlReader.GetDepth();

	while (oXmlReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:MediaFile" == oXmlReader.GetNameA())
		{
			const std::wstring wsPath{oXmlReader.GetText2()};

			if (CanUseThisPath(wsPath, wsRootPath))
				m_wsFilePath = CombinePaths(wsRootPath, wsPath);

			break;
		}
	}
}

CMultiMedia::EType CMultiMedia::GetType() const
{
	return m_eType;
}

std::wstring CMultiMedia::GetFilePath() const
{
	return m_wsFilePath;
}
}
