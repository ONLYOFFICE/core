
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
		m_DataLabExt = elements_.back();
		elements_.pop_back();

		proc.mandatory<StartObject>();	elements_.pop_back();

		proc.mandatory<DefaultText>();
		
		m_DefaultText = elements_.back();
		elements_.pop_back();
	}
	else
	{
		if(!proc.mandatory<DefaultText>())
		{
			return false;
		}
		m_DefaultText = elements_.back();
		elements_.pop_back();
	}

	proc.mandatory<ATTACHEDLABEL>();
	m_ATTACHEDLABEL = elements_.back();
	elements_.pop_back();

	if (proc.optional<EndObject>()) elements_.pop_back();

	return true;
}

} // namespace XLS

