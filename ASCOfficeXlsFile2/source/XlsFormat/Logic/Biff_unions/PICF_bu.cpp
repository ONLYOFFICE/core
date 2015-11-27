
#include "PICF.h"
#include <Logic/Biff_records/Begin.h>
#include <Logic/Biff_records/PicF.h>
#include <Logic/Biff_records/End.h>

namespace XLS
{


PICF::PICF()
{
}


PICF::~PICF()
{
}


BaseObjectPtr PICF::clone()
{
	return BaseObjectPtr(new PICF(*this));
}


// PICF = Begin PicF End
const bool PICF::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Begin>())
	{
		return false;
	}								elements_.pop_back();
	proc.mandatory<PicF>();
	m_PicF = elements_.back();		elements_.pop_back();
	proc.mandatory<End>();			elements_.pop_back();

	return true;
}

} // namespace XLS

