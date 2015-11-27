
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
	proc.mandatory<Begin>();			elements_.pop_back();
	proc.mandatory<LD>();
	proc.mandatory<End>();				elements_.pop_back();

	return true;
}

int DAT::serialize (std::wostream & _stream)
{
	if (elements_.empty()) return 0;

	LD *LD_ = dynamic_cast<LD *>(elements_.back().get());
	if (LD_)
		LD_->serialize(_stream);

	return 0;
	
}

} // namespace XLS

