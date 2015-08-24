
#include "LBL.h"
#include <Logic/Biff_records/Lbl.h>
#include <Logic/Biff_records/NameCmt.h>
#include <Logic/Biff_records/NameFnGrp12.h>
#include <Logic/Biff_records/NamePublish.h>

#include <utils.h>
#include <simple_xml_writer.h>

namespace XLS
{


LBL::LBL()
{
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

	std::wstring name;
	if (lbl->fBuiltin)	name = lbl->Name.value().get_value_or(L"");
	
	if (name.empty())	name = lbl->Name_bin.value();
	
    NameCmt namecmt(name);
    if (proc.optional(namecmt))
	{
	}
	proc.optional<NameFnGrp12>();
	proc.optional<NamePublish>();

	GlobalWorkbookInfoPtr	global_info_ = proc.getGlobalWorkbookInfo();

	global_info_->defineNames.push_back(name);


	return true;
}
int LBL::serialize(std::wostream & stream)
{
	Lbl *lbl = dynamic_cast<Lbl*>(m_Lbl.get());
	if (lbl == NULL) return 0;
		
	const std::wstring & value = lbl->rgce.getAssembledFormula();

	if (value.empty()) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"definedName")
		{
			std::wstring name;
			if (lbl->fBuiltin)	name = lbl->Name.value().get_value_or(L"");
			
			if (name.empty())	name = lbl->Name_bin.value();

			CP_XML_ATTR(L"name", xml::utils::replace_text_to_xml(name));

			CP_XML_STREAM() << xml::utils::replace_text_to_xml(value);
		}
	}
	return 0;
}
} // namespace XLS

