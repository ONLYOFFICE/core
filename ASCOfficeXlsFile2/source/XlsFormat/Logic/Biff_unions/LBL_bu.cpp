
#include "LBL.h"
#include <Logic/Biff_records/Lbl.h>
#include <Logic/Biff_records/NameCmt.h>
#include <Logic/Biff_records/NameFnGrp12.h>
#include <Logic/Biff_records/NamePublish.h>

#include <utils.h>

namespace XLS
{


LBL::LBL()
{
	isSerialize = false;
}


LBL::~LBL()
{
}


BaseObjectPtr LBL::clone()
{
	return BaseObjectPtr(new LBL(*this));
}


// LBL = Lbl [NameCmt] [NameFnGrp12] [NamePublish]
const bool LBL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Lbl>())
	{
		return false;
	}
	m_Lbl = elements_.back();
	elements_.pop_back();

	Lbl *lbl = dynamic_cast<Lbl*>(m_Lbl.get());
	if (!lbl) return false;

	std::wstring name;
	std::wstring comment;

	if (lbl->fBuiltin)	name = lbl->Name.value().get_value_or(L"");	
	if (name.empty())	name = lbl->Name_bin.value();

	std::wstring value = lbl->rgce.getAssembledFormula();
	
    NameCmt namecmt(name);
    if (proc.optional(namecmt))
	{
		if (name.empty())
			name = namecmt.name.value();
		comment = namecmt.comment.value();
	}
	if (proc.optional<NameFnGrp12>())
	{
	}
	if (proc.optional<NamePublish>())
	{
	}

	GlobalWorkbookInfoPtr	global_info_ = proc.getGlobalWorkbookInfo();


	if (!value.empty() && !name.empty())
	{
		int ind_sheet = lbl->itab;

		std::map<std::wstring, std::vector<std::wstring>>::iterator it = global_info_->mapDefineNames.find(name);
		
		if (it != global_info_->mapDefineNames.end())
		{
			while ( it->second.size() <= ind_sheet)
			{
				it->second.push_back(L"");
			}
			it->second[ind_sheet] = value;
			//it->second.push_back(value);
		}
		else
		{
			std::vector<std::wstring> ar(ind_sheet + 1);
		
			ar[ind_sheet] = value;
			//ar.push_back(value);

			global_info_->mapDefineNames.insert(std::pair<std::wstring, std::vector<std::wstring>>(name, ar));
		}
		isSerialize = true;
	}
	else
	{
		if (lbl->fFunc)
		{
			if (name == L"FORMULA") //"general_formulas.xls"
					name = L"_xludf." + name;
		}
	}
	global_info_->arDefineNames.push_back(name);// для имен функций - todooo ... не все функции корректны !! БДИ !!
	return true;
}
int LBL::serialize(std::wostream & stream)
{
	//if (isSerialize == false) return 0;

	Lbl *lbl = dynamic_cast<Lbl*>(m_Lbl.get());
	if (lbl == NULL) return 0;
		
	std::wstring value = lbl->rgce.getAssembledFormula();

	if (value.empty()) return 0;

	int res = 0;
	if (lbl->itab == 0 && (res = value.find(L"!")) < 0)
	{
		value = std::wstring(L"#REF!");
	}

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"definedName")
		{
			std::wstring name;
			if (lbl->fBuiltin)	name = lbl->Name.value().get_value_or(L"");
			
			if (name.empty())	name = lbl->Name_bin.value();

			CP_XML_ATTR(L"name", xml::utils::replace_text_to_xml(name));

			if (lbl->itab != 0)
			{
				CP_XML_ATTR(L"localSheetId", lbl->itab - 1);
			}

			CP_XML_STREAM() << xml::utils::replace_text_to_xml(value);
		}
	}
	return 0;
}
} // namespace XLS

