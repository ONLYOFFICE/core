/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "StreamObjects.h"
#include "VbaRecordType.h"

#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>

namespace VBA
{;

bool DirStreamObject::loadContent()
{
	InformationRecord = boost::make_shared<PROJECTINFORMATION>(reader);
	ReferencesRecord = boost::make_shared<PROJECTREFERENCES>(reader);
	ModulesRecord = boost::make_shared<PROJECTMODULES>(reader);
		
	return (InformationRecord && ReferencesRecord && ModulesRecord);
}

bool ModuleStreamObject::loadContent()
{
    SourceCode = convert_string_icu((char*)reader->getData(), (unsigned int)reader->getDataSize(), reader->CodePage);

	return true;
}
bool ProjectStreamObject::loadContent()
{
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
