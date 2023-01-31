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

#include "xlsx_data_validation.h"
#include "xlsx_utils.h"

#include <vector>
#include <map>

#include <xml/simple_xml_writer.h>
#include "../../Common/CPSharedPtr.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

#include "../../Formulas/formulasconvert.h"
#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore {
namespace oox {

struct xlsx_dataValidation
{
	std::wstring ref;
	//std::wstring activate_ref;
	std::map<int, std::vector<int>> mapCells;

	std::wstring type;
	std::wstring formula1;			
	std::wstring formula2;
	std::wstring operator_;

	bool showErrorMessage = false;
	bool showInputMessage = false;
	bool showDropDown = false;
	bool allowBlank = false;

	std::wstring error_text;
	std::wstring error_title;

	std::wstring promt_text;
	std::wstring promt_title;

	std::wstring activate_ref;

	int error_type = 0;//stop
};
typedef shared_ptr<xlsx_dataValidation>::Type xlsx_dataValidation_ptr;

class xlsx_dataValidations_context::Impl
{
private:
	void prepare()
	{
		for (std::map<std::wstring, xlsx_dataValidation_ptr>::iterator it = mapActivateDataValidations.begin();
			it != mapActivateDataValidations.end(); ++it)
		{
			if (it->second->activate_ref.empty())
			{
				//prepare
				std::map<int, std::vector<std::pair<int, int>>> mapCells;
				for (std::map<int, std::vector<int>>::iterator jt = it->second->mapCells.begin(); jt != it->second->mapCells.end(); ++jt)
				{
					std::sort(jt->second.begin(), jt->second.end());
					std::vector<std::pair<int, int>> arr;
					MakePairVector(jt->second, arr);

					mapCells.insert(std::make_pair(jt->first, arr));
				}

				std::wstring activate_ref;
				std::map<int, std::vector<std::pair<int, int>>>::iterator jt1 = mapCells.begin();
				while (jt1 != mapCells.end())
				{
					for (size_t i = 0; i < jt1->second.size(); i++)
					{
						std::map<int, std::vector<std::pair<int, int>>>::iterator jt2 = jt1;
						while (jt2 != mapCells.end())
						{
							std::map<int, std::vector<std::pair<int, int>>>::iterator jt2_next = jt2; ++jt2_next;
							if (jt2_next == mapCells.end() || jt2->first + 1 != jt2_next->first)
								break;

							//size_t j = 0; 
							bool bFound = false;

							for (std::vector<std::pair<int, int>>::iterator v_jt2 = jt2_next->second.begin();
								v_jt2 != jt2_next->second.end(); ++v_jt2)
							{
								if (jt1->second[i].first == v_jt2->first ||
									jt1->second[i].second == v_jt2->second)
								{
									bFound = true;
									jt2_next->second.erase(v_jt2);
									break;
								}
							}
							if (false == bFound)
								break;

							jt2 = jt2_next;
						}
						if (!activate_ref.empty())
						{
							activate_ref += L" ";
						}
						activate_ref += oox::getCellAddress(jt1->first, jt1->second[i].first);

						if (jt1->first != jt2->first || jt1->second[i].first != jt1->second[i].second)
							activate_ref += L":" + oox::getCellAddress(jt2->first, jt1->second[i].second);
					}
					++jt1;
				}
				it->second->activate_ref = activate_ref;
			}
		}
	}
public:

    void serialize(std::wostream & _Wostream)
    {
		return;

        if (mapActivateDataValidations.empty()) return;

		prepare();
 		
        CP_XML_WRITER(_Wostream)
        {
			CP_XML_NODE(L"dataValidations")
			{      
				CP_XML_ATTR(L"count", mapActivateDataValidations.size());

				for (std::map<std::wstring, xlsx_dataValidation_ptr>::iterator it = mapActivateDataValidations.begin();
					it != mapActivateDataValidations.end(); ++it)
                {
					if (std::wstring::npos != it->second->activate_ref.find(L" ")) continue;

					CP_XML_NODE(L"dataValidation")
                    {
 						CP_XML_ATTR(L"sqref", it->second->activate_ref);
 						CP_XML_ATTR(L"allowBlank", it->second->allowBlank);
						if (!it->second->operator_.empty())
						{
 							CP_XML_ATTR(L"operator", it->second->operator_);
						}
 						CP_XML_ATTR(L"showDropDown", it->second->showDropDown);
 						CP_XML_ATTR(L"showErrorMessage", it->second->showErrorMessage);
 						CP_XML_ATTR(L"showInputMessage", it->second->showInputMessage);
						if (!it->second->type.empty())
						{
 							CP_XML_ATTR(L"type", it->second->type);
						}
						if (!it->second->error_title.empty())
						{
							CP_XML_ATTR(L"errorTitle", it->second->error_title);
						}
						if (!it->second->error_text.empty())
						{
							CP_XML_ATTR(L"error", it->second->error_text);
						}
						if (!it->second->promt_title.empty())
						{
							CP_XML_ATTR(L"promptTitle", it->second->promt_title);
						}
						if (!it->second->promt_text.empty())
						{
							CP_XML_ATTR(L"prompt", it->second->promt_text);
						}

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
    }

	void serialize_x14(std::wostream & _Wostream)
	{
		if (mapActivateDataValidations.empty()) return;
		
		prepare();
		
		CP_XML_WRITER(_Wostream)
		{
			CP_XML_NODE(L"x14:dataValidations")
			{
				CP_XML_ATTR(L"count", mapActivateDataValidations.size());
				CP_XML_ATTR(L"xmlns:xm", L"http://schemas.microsoft.com/office/excel/2006/main");

				for (std::map<std::wstring, xlsx_dataValidation_ptr>::iterator it = mapActivateDataValidations.begin();
					it != mapActivateDataValidations.end(); ++it)
				{
					CP_XML_NODE(L"x14:dataValidation")
					{
						CP_XML_ATTR(L"allowBlank", it->second->allowBlank);
						if (!it->second->operator_.empty())
						{
							CP_XML_ATTR(L"operator", it->second->operator_);
						}
						CP_XML_ATTR(L"showDropDown", it->second->showDropDown);
						CP_XML_ATTR(L"showErrorMessage", it->second->showErrorMessage);
						CP_XML_ATTR(L"showInputMessage", it->second->showInputMessage);
						if (!it->second->type.empty())
						{
							CP_XML_ATTR(L"type", it->second->type);
						}
						if (!it->second->error_title.empty())
						{
							CP_XML_ATTR(L"errorTitle", it->second->error_title);
						}
						if (!it->second->error_text.empty())
						{
							CP_XML_ATTR(L"error", it->second->error_text);
						}
						switch (it->second->error_type)
						{
						case 1: CP_XML_ATTR(L"errorStyle", L"warning"); break;
						case 2: CP_XML_ATTR(L"errorStyle", L"information"); break;
						case 0: 
						default:
								CP_XML_ATTR(L"errorStyle", L"stop"); break;
						}
						if (!it->second->promt_title.empty())
						{
							CP_XML_ATTR(L"promptTitle", it->second->promt_title);
						}
						if (!it->second->promt_text.empty())
						{
							CP_XML_ATTR(L"prompt", it->second->promt_text);
						}
						if (!it->second->formula1.empty())
						{
							CP_XML_NODE(L"x14:formula1")
							{
								CP_XML_NODE(L"xm:f")
								{
									CP_XML_STREAM() << it->second->formula1;
								}
							}
						}
						if (!it->second->formula2.empty())
						{
							CP_XML_NODE(L"x14:formula2")
							{
								CP_XML_NODE(L"xm:f")
								{
									CP_XML_STREAM() << it->second->formula2;
								}
							}
						}
						CP_XML_NODE(L"xm:sqref")
						{
							CP_XML_STREAM() << it->second->activate_ref;
						}
					}
				}
			}
		}
	}
	std::map<std::wstring, xlsx_dataValidation_ptr>		mapDataValidations;			//for all tables
	std::map<std::wstring, xlsx_dataValidation_ptr>		mapActivateDataValidations; //in current table

private:
	void MakePairVector(std::vector<int> & arInput, std::vector<std::pair<int, int>> &arOutput)
	{
		int pos = arInput[0];
		for (size_t i = 1; i < arInput.size(); i++)
		{
			if (arInput[i-1] + 1  != arInput[i])
			{
				arOutput.push_back(std::make_pair(pos, arInput[i - 1]));
				pos = arInput[i];
			}
		}
		arOutput.push_back(std::make_pair(pos, arInput[arInput.size() - 1]));
	}
};

xlsx_dataValidations_context::xlsx_dataValidations_context() :
			impl_(new xlsx_dataValidations_context::Impl())
{}

xlsx_dataValidations_context::~xlsx_dataValidations_context()
{
}
void xlsx_dataValidations_context::clear()
{
	impl_->mapActivateDataValidations.clear();
}
void xlsx_dataValidations_context::serialize(std::wostream & _Wostream)
{
    return impl_->serialize(_Wostream);
}
void xlsx_dataValidations_context::serialize_x14(std::wostream & _Wostream)
{
	return impl_->serialize_x14(_Wostream);
}
void xlsx_dataValidations_context::activate(const std::wstring & name, int col, int row/*const std::wstring & ref*/)
{
	std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFindActivate = impl_->mapActivateDataValidations.find(name);
	
	if (pFindActivate == impl_->mapActivateDataValidations.end())
	{
		std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFind = impl_->mapDataValidations.find(name);

		xlsx_dataValidation_ptr _new;
		if (pFind != impl_->mapDataValidations.end())
		{
			_new = pFind->second;
		}
		else
		{
			_new = xlsx_dataValidation_ptr(new xlsx_dataValidation);		
		}
		impl_->mapActivateDataValidations.insert(std::make_pair(name, _new));

		pFindActivate = impl_->mapActivateDataValidations.find(name);
	}

	std::map<int, std::vector<int>>::iterator pFindCol = pFindActivate->second->mapCells.find(col);

	if (pFindCol == pFindActivate->second->mapCells.end())
	{
		std::vector<int> rows; rows.push_back(row);
		pFindActivate->second->mapCells.insert(std::make_pair(col, rows));
	}
	else
	{
		pFindCol->second.push_back(row);
	}
}
void xlsx_dataValidations_context::add(const std::wstring & name, const std::wstring &ref)
{
	xlsx_dataValidation_ptr _new = xlsx_dataValidation_ptr(new xlsx_dataValidation);

	formulasconvert::odf2oox_converter converter;
	_new->ref = converter.convert_named_ref(ref, false, L";");

	impl_->mapDataValidations.insert(std::make_pair(name, _new));
}
void xlsx_dataValidations_context::add_help_msg(const std::wstring & name, const std::wstring & title, const std::wstring & content, bool display)
{
	std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFind = impl_->mapDataValidations.find(name);

	if (pFind == impl_->mapDataValidations.end()) return;
	
	pFind->second->showInputMessage = display;
	pFind->second->promt_title = title;
	pFind->second->promt_text = content;
}

void xlsx_dataValidations_context::add_error_msg(const std::wstring & name, const std::wstring & title, const std::wstring & content, bool display, int type)
{
	std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFind = impl_->mapDataValidations.find(name);

	if (pFind == impl_->mapDataValidations.end()) return;
	
	pFind->second->showErrorMessage = display;
	pFind->second->error_title = title;
	pFind->second->error_text = content;
	pFind->second->error_type = type;
}
void xlsx_dataValidations_context::add_formula(const std::wstring & name, const std::wstring & formula) 
{
	std::map<std::wstring, xlsx_dataValidation_ptr>::iterator pFind = impl_->mapDataValidations.find(name);

	if (pFind == impl_->mapDataValidations.end()) return;

	formulasconvert::odf2oox_converter converter;

	std::vector<std::wstring> arrFormula;
	boost::algorithm::split_regex(arrFormula, formula, boost::wregex(L"and"));

	std::wstring val, val2, operator_;

	for (size_t i = 0; i < arrFormula.size(); ++i)
	{
		std::wstring & f = arrFormula[i];
		size_t pos;

		if (std::wstring::npos != (pos = f.find(L"cell-content-is-in-list"))) //oooc: , of:
		{
			pFind->second->type = L"list";
			val = f.substr(24 + pos, f.size() - 25 - pos);

			if (val.substr(0, 1) == L"\"")
			{
				std::wstring keep = val;
				XmlUtils::replace_all(val, L"\"", L"");

				pFind->second->formula1 = converter.convert(val);

				if ((std::wstring::npos != val.find(L";")) ||	//convert formula replacing ; on ,
					L"\"" + pFind->second->formula1 + L"\"" == keep)
				{
					pFind->second->formula1 = L"\"" + pFind->second->formula1 + L"\"";
				}
			}
			else
			{
				pFind->second->formula1 = converter.convert(val);
			}
		}
		else if (std::wstring::npos != (pos = f.find(L"is-true-formula")))
		{
			pFind->second->type = L"custom";
			val = f.substr(16 + pos, f.size() - 17 - pos);
			pFind->second->formula1 = converter.convert(val);
		}
		else if (std::wstring::npos != (pos = f.find(L"cell-content-is-date")))
		{
			pFind->second->type = L"date";
		}
		else if (std::wstring::npos != (pos = f.find(L"cell-content-is-decimal-number")))
		{
			pFind->second->type = L"decimal";
		}
		else if (std::wstring::npos != (pos = f.find(L"cell-content-is-time")))
		{
			pFind->second->type = L"time";
		}
		else if (std::wstring::npos != (pos = f.find(L"cell-content-is-whole-number")))
		{
			pFind->second->type = L"whole";
		}
		else if (std::wstring::npos != (pos = f.find(L"cell-content-text-length")))
		{
			pFind->second->type = L"textLength";
			operator_ = f.substr(pos + 24);
		}
		else if (std::wstring::npos != (pos = f.find(L"cell-content")))
		{
			operator_ = f.substr(pos + 12);
		}
	}
	if (false == operator_.empty())
	{
		size_t pos;
		if (std::wstring::npos != (pos = operator_.find(L"-is-not-between(")))
		{
			pFind->second->operator_ = L"notBetween";

			size_t pos2 = operator_.find(L",", pos + 15);
			size_t pos3 = operator_.rfind(L")");

			val = operator_.substr(pos + 15, pos2 - pos - 15);
			val2 = operator_.substr(pos2 + 1, pos3 - pos2 - 1);
		}
		else if (std::wstring::npos != (pos = operator_.find(L"-is-between(")))
		{
			pFind->second->operator_ = L"between";

			size_t pos2 = operator_.find(L",", pos + 12);
			size_t pos3 = operator_.rfind(L")");

			val = operator_.substr(pos + 12, pos2 - pos - 12);
			val2 = operator_.substr(pos2 + 1, pos3 - pos2 - 1);
		}
		else
		{
			size_t split = std::wstring::npos;
			if (std::wstring::npos != (pos = operator_.find(L"()==")))
			{
				pFind->second->operator_ = L"equal";
				split = 4;
			}
			else if (std::wstring::npos != (pos = operator_.find(L"()!=")))
			{
				pFind->second->operator_ = L"notEqual";
				split = 4;
			}
			else if (std::wstring::npos != (pos = operator_.find(L"()<=")))
			{
				pFind->second->operator_ = L"lessThanOrEqual";
				split = 4;
			}
			else if (std::wstring::npos != (pos = operator_.find(L"()<")))
			{
				pFind->second->operator_ = L"lessThan";
				split = 3;
			}
			else if (std::wstring::npos != (pos = operator_.find(L"()>=")))
			{
				pFind->second->operator_ = L"greaterThanOrEqual";
				split = 4;
			}
			else if (std::wstring::npos != (pos = operator_.find(L"()>")))
			{
				pFind->second->operator_ = L"greaterThan";
				split = 3;
			}
			if (split != std::wstring::npos)
			{
				val = operator_.substr(pos + split, operator_.length() - pos - split);
			}
		}

		pFind->second->formula1 = converter.convert(val);
		pFind->second->formula2 = converter.convert(val2);
	}
}

}
}
