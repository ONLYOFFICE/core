
#include "DAT.h"
#include <Logic/Biff_records/Dat.h>
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_unions/LD.h>
#include <Logic/Biff_records/End.h>

namespace XLS
{


DAT::DAT()
{
}


DAT::~DAT()
{
}


BaseObjectPtr DAT::clone()
{
	return BaseObjectPtr(new DAT(*this));
}


// DAT = Dat Begin LD End
const bool DAT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Dat>())
	{
		return false;
	}
	m_Dat = elements_.back();			elements_.pop_back();		
	proc.mandatory<Begin>();			elements_.pop_back();
	proc.mandatory<LD>();
	m_LD = elements_.back();			elements_.pop_back();		
	proc.mandatory<End>();				elements_.pop_back();

	return true;
}

int DAT::serialize (std::wostream & _stream)
{
	Dat * dat = dynamic_cast<Dat*>(m_Dat.get());

	LD *ld = dynamic_cast<LD *>(m_LD.get());


	return 0;
	
}

} // namespace XLS

