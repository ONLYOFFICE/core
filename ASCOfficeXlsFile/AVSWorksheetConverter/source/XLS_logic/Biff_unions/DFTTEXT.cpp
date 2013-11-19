#include "stdafx.h"
#include "DFTTEXT.h"
#include <XLS_logic/Biff_records/DataLabExt.h>
#include <XLS_logic/Biff_records/StartObject.h>
#include <XLS_logic/Biff_records/DefaultText.h>
#include <XLS_logic/Biff_records/EndObject.h>

#include <XLS_logic/Biff_unions/ATTACHEDLABEL.h>

namespace XLS
{;


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

