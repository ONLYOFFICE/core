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

#include "XmlReader.h"

#include "Utils.h"

namespace OFD
{
CXmlReader::CXmlReader()
{}

std::wstring CXmlReader::GetTextValue(bool bIsAttribute)
{
	return (bIsAttribute) ? XmlUtils::CXmlLiteReader::GetText() : XmlUtils::CXmlLiteReader::GetText2();
}

std::string CXmlReader::GetTextValueA(bool bIsAttribute)
{
	return (bIsAttribute) ? XmlUtils::CXmlLiteReader::GetTextA() : XmlUtils::CXmlLiteReader::GetText2A();
}

bool CXmlReader::GetBoolean(bool bIsAttribute)
{
	bool bValue = false;
	StringToBoolean(GetTextValueA(bIsAttribute), bValue);
	return bValue;
}

int CXmlReader::GetInteger(bool bIsAttribute)
{
	int nValue = 0;
	StringToInteger(GetTextValueA(bIsAttribute), nValue);
	return nValue;
}

unsigned int CXmlReader::GetUInteger(bool bIsAttribute)
{
	unsigned int unValue = 0;
	StringToUInteger(GetTextValueA(bIsAttribute), unValue);
	return unValue;
}

double CXmlReader::GetDouble(bool bIsAttribute)
{
	double dValue = 0.;
	StringToDouble(GetTextValueA(bIsAttribute), dValue);
	return dValue;
}

std::vector<std::string> CXmlReader::GetArrayStrings(bool bIsAttribute)
{
	return Split(GetTextValueA(bIsAttribute), ' ');
}

std::vector<double> CXmlReader::GetArrayDoubles(bool bIsAttribute)
{
	const std::vector<std::string> arValues{Split(GetTextValueA(bIsAttribute), ' ')};

	if(arValues.empty())
		return std::vector<double>();

	std::vector<double> arDoubleValues(arValues.size(), 0.);

	for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
		StringToDouble(arValues[unIndex], arDoubleValues[unIndex]);

	return arDoubleValues;
}

std::vector<unsigned int> CXmlReader::GetArrayUInteger(bool bIsAttribute)
{
	const std::vector<std::string> arValues{Split(GetTextValueA(bIsAttribute), ' ')};

	if(arValues.empty())
		return std::vector<unsigned int>();

	std::vector<unsigned int> arUIntValues(arValues.size(), 0);

	for (unsigned int unIndex = 0; unIndex < arValues.size(); ++unIndex)
		StringToUInteger(arValues[unIndex], arUIntValues[unIndex]);

	return arUIntValues;
}
}
