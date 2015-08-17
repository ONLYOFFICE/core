
#include "Feat.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{

Feat::Feat()
{
}


Feat::~Feat()
{
}


BaseObjectPtr Feat::clone()
{
	return BaseObjectPtr(new Feat(*this));
}


void Feat::writeFields(CFRecord& record)
{
#pragma message("####################### Feat record is not implemented")
	Log::error("Feat record is not implemented.");
	//record << some_value;
}


void Feat::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_Feat);
	record >> frtHeader;
	record >> isf;
	record.skipNunBytes(5); // reserved
	record >> cref >> cbFeatData;
	record.skipNunBytes(2); // reserved
	std::wstring  sqref_str;
	for (int i = 0; i < cref ; ++i)
	{
		Ref8U reff;
		record >> reff;
		refs.push_back(BiffStructurePtr(new Ref8U(reff)));
		sqref_str += std::wstring (reff.toString().c_str()) + ((i == cref - 1) ? L"" : L" ");
	}
	sqref = sqref_str;

	switch(isf)
	{
		case SharedFeatureType::ISFPROTECTION:
			record >> protection;
			break;
		case SharedFeatureType::ISFFEC2:
			record >> formula_err;
			break;
		case SharedFeatureType::ISFFACTOID:
			record >> smart_tag;
			break;
	}

}

} // namespace XLS

