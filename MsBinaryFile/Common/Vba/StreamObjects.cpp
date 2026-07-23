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

#include "StreamObjects.h"
#include "VbaRecordType.h"

#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>

namespace VBA
{;

bool DirStreamObject::loadContent()
{
	if (!reader) return false;
		
	InformationRecord = boost::make_shared<PROJECTINFORMATION>(reader);
	ReferencesRecord = boost::make_shared<PROJECTREFERENCES>(reader);
	ModulesRecord = boost::make_shared<PROJECTMODULES>(reader);
		
	return (InformationRecord && ReferencesRecord && ModulesRecord);
}

bool ModuleStreamObject::loadContent()
{
	if (!reader) return false;
	
	SourceCode = convert_string_icu((char*)reader->getData(), (unsigned int)reader->getDataSize(), reader->CodePage);

	return true;
}
bool ProjectStreamObject::loadContent()
{
	if (!reader) return false;

	std::string strProps((char*)reader->getData(), reader->getDataSize());

	std::vector<std::string> arrProps;
	boost::algorithm::split(arrProps, strProps, boost::algorithm::is_any_of("\n"), boost::algorithm::token_compress_on);

	for (size_t i = 0; i < arrProps.size(); ++i)
	{
		if ((arrProps[i].length() > 10) && (arrProps[i].substr(0, 10) == "BaseClass="))
		{
			DesignerModules.push_back(std::wstring(arrProps[i].begin() + 10, arrProps[i].end() - 1));
		}
	}
	return true;
}
bool VBFrameObject::loadContent()
{
	if (!reader) return false;

	std::wstring strProps = convert_string_icu((char*)reader->getData(), (unsigned int)reader->getDataSize(), reader->CodePage);

	std::vector<std::wstring> arrProps;
	boost::algorithm::split(arrProps, strProps, boost::algorithm::is_any_of(L"\n"), boost::algorithm::token_compress_on);

	for (size_t i = 0; i < arrProps.size(); ++i)
	{
		std::vector<std::wstring> arrProp;
		boost::algorithm::split(arrProp, arrProps[i].substr(0, arrProps[i].length() - 1), boost::algorithm::is_any_of(L"="), boost::algorithm::token_compress_on);
		if (arrProp.size() == 2)
		{
			boost::algorithm::trim(arrProp[0]);
			boost::algorithm::trim(arrProp[1]);

			Props.push_back(std::make_pair(arrProp[0], arrProp[1]));
		}
	}
	return true;
}

bool FormControlStream::loadContent()
{
	if (!reader) return false;

	unsigned char MinorVersion, MajorVersion;
	_UINT16 cbForm;
	
	*reader >> MinorVersion >> MajorVersion >> cbForm;

	Control = boost::make_shared<FormControl>(reader); 
	
	bool bSiteData = Control->BooleanProperties ? (!Control->BooleanProperties->FORM_FLAG_DONTSAVECLASSTABLE) : true;
	SiteData = boost::make_shared<FormSiteData>(reader, bSiteData);

	if ((Control->BooleanProperties) && (Control->BooleanProperties->FORM_FLAG_DESINKPERSISTED))
	{
		DesignExData = boost::make_shared<FormDesignExData>(reader);
	}

	return true;
}

} // namespace VBA
