/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "xlsx_data_validation.h"
#include "xlsx_utils.h"

#include <vector>
#include <map>

#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/CPSharedPtr.h>

#include "../formulasconvert/formulasconvert.h"
#include "../../../Common/DocxFormat/Source/XML/Utils.h"

namespace cpdoccore {
namespace oox {

class xlsx_dataValidation
{
public:
	xlsx_dataValidation() : showErrorMessage(false), showInputMessage(false), showDropDown(false), allowBlank(false) {}

	std::wstring ref;
	std::wstring activate_ref;
	std::wstring type;
	std::wstring formula1;			
	std::wstring formula2;
	std::wstring operator_;
	bool showErrorMessage;
	bool showInputMessage;
	bool showDropDown;
	bool allowBlank;
};
typedef shared_ptr<xlsx_dataValidation>::Type xlsx_dataValidation_ptr;

class xlsx_dataValidations_context::Impl
{
public:

    void serialize(std::wostream & _Wostream)
    {
        if (mapActivateDataValidations.empty()) return;
 		
        CP_XML_WRITER(_Wostream)
        {
			CP_XML_NODE(L"dataValidations")
			{      
				CP_XML_ATTR(L"count", mapActivateDataValidations.size());

				for (std::map<std::wstring, xlsx_dataValidation_ptr>::iterator it = mapActivateDataValidations.begin();
					it != mapActivateDataValidations.end(); ++it)
                {
					CP_XML_NODE(L"dataValidation")
                    {
 						CP_XML_ATTR(L"sqref", it->second->activate_ref);
 						CP_XML_ATTR(L"allowBlank", it->second->allowBlank);
 						CP_XML_ATTR(L"operator", it->second->operator_);
 						CP_XML_ATTR(L"showDropDown", it->second->showDropDown);
 						CP_XML_ATTR(L"showErrorMessage", it->second->showErrorMessage);
 						CP_XML_ATTR(L"showInputMessage", it->second->showInputMessage);
 						CP_XML_ATTR(L"type", it->second->type);

						if (!it->second->formula1.empty())
						{
							CP_XML_NODE(L"formula1")
							{
								CP_XML_STREAM() << it->second->formula1;
							}
						}

						if (!it->second->formula2.empty())
						{
							CP_XML_NODE(L"formula2")
							{
								CP_XML_STREAM() << it->second->formula2;
							}
						}
					}
                }        
            }
        }

		mapActivateDataValidations.clear();
    }

	std::map<std::wstring, xlsx_dataValidation_ptr>		mapDataValidations;			//for all tables
	std::map<std::wstring, xlsx_dataValidation_ptr>		mapActivateDataValidations; //in current table
};

xlsx_dataValidations_context::xlsx_dataValidations_context() :
			impl_(new xlsx_dataValidations_context::Impl())
{}

xlsx_dataValidations_context::~xlsx_dataValidations_context()
{
}
void xlsx_dataValidations_context::serialize(std::wostream & _Wostream)
{
    return impl_->serialize(_Wostream);
}
void xlsx_dataValidations_context::activate(const std::wstring & name, const std::wstring & ref)
{
	std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFindActivate = impl_->mapActivateDataValidations.find(name);

	if (pFindActivate != impl_->mapActivateDataValidations.end())
	{
		pFindActivate->second->activate_ref += L" " + ref;
	}
	else
	{
		std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFind = impl_->mapDataValidations.find(name);

		if (pFind != impl_->mapDataValidations.end())
		{
			pFind->second->activate_ref = ref;
			impl_->mapActivateDataValidations.insert(std::make_pair(name, pFind->second));
		}
	}
}
void xlsx_dataValidations_context::add_help_msg(const std::wstring & name, bool val)
{
	std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFind = impl_->mapDataValidations.find(name);

	if (pFind == impl_->mapDataValidations.end()) return;
	
	pFind->second->showInputMessage = val;
}

void xlsx_dataValidations_context::add_error_msg(const std::wstring & name, bool val)
{
	std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFind = impl_->mapDataValidations.find(name);

	if (pFind == impl_->mapDataValidations.end()) return;
	
	pFind->second->showErrorMessage = val;
}

void xlsx_dataValidations_context::add(const std::wstring & name, const std::wstring &ref)
{
	xlsx_dataValidation_ptr _new = xlsx_dataValidation_ptr(new xlsx_dataValidation());

	formulasconvert::odf2oox_converter converter;
	_new->ref = converter.convert_named_ref(ref, false, L";");

	impl_->mapDataValidations.insert(std::make_pair(name, _new));
}

void xlsx_dataValidations_context::add_formula(const std::wstring & name, const std::wstring & f) // todooo пооптимальней
{
	std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFind = impl_->mapDataValidations.find(name);

	if (pFind == impl_->mapDataValidations.end()) return;

	formulasconvert::odf2oox_converter converter;
	int pos = -1;
	std::wstring val;
	
	if ( 0 <= (pos = f.find(L"cell-content-is-in-list"))) //oooc: , of:
	{
		pFind->second->type = L"list";
		val = f.substr(24 + pos, f.size() - 25 - pos);
		
		if ( val.substr(0, 1) == L"\"") 
		{
			XmlUtils::replace_all(val, L"\"", L"");
		
			pFind->second->formula1 = converter.convert(val);

			if ( std::wstring::npos != val.find(L";") )//convert formula replacing ; on ,
			{
				pFind->second->formula1 = L"\"" + pFind->second->formula1 + L"\"";
			}
		}
		else
		{
			pFind->second->formula1 = converter.convert(val);
		}
	}
}

}
}
