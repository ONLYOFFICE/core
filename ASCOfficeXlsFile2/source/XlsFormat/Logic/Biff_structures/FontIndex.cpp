
#include "FontIndex.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FontIndex::clone()
{
	return BiffStructurePtr(new FontIndex(*this));
}


const unsigned short FontIndex::getValue() const
{	
	unsigned short index = static_cast<unsigned short>(val.get_value_or(0));
	return index < 4 ? index : index - 1;
}
void	FontIndex::setValue(unsigned short val_)
{
	if (val_ < 4) val = val_;
	else val = val_;

}


} // namespace XLS

