
#include "DFTTEXT.h"
#include <Logic/Biff_records/DataLabExt.h>
#include <Logic/Biff_records/StartObject.h>
#include <Logic/Biff_records/DefaultText.h>
#include <Logic/Biff_records/EndObject.h>

#include <Logic/Biff_unions/ATTACHEDLABEL.h>

namespace XLS
{


DFTTEXT::DFTTEXT()
{
}


DFTTEXT::~DFTTEXT()
{
}


BaseObjectPtr DFTTEXT::clone()
{
	return BaseObjectPtr(new DFTTEXT(*this));
}


// DFTTEXT = [DataLabExt StartObject] DefaultText ATTACHEDLABEL [EndObject]
const bool DFTTEXT::loadContent(BinProcessor& proc)
{
	if(proc.optional<DataLabExt>())
	{
		proc.mandatory<StartObject>();
		proc.mandatory<DefaultText>();
	}
	else
	{
		if(!proc.mandatory<DefaultText>())
		{
			return false;
		}
	}
	proc.mandatory<ATTACHEDLABEL>();
	proc.optional<EndObject>();

	return true;
}

} // namespace XLS

