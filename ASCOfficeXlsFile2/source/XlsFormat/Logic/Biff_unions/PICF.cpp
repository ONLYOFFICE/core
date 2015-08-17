
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
	}
	proc.mandatory<PicF>();
	proc.mandatory<End>();

	return true;
}

} // namespace XLS

