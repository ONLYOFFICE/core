
#include "Dv.h"

namespace XLS
{

BaseObjectPtr Dv::clone()
{
	return BaseObjectPtr(new Dv(*this));
}


void Dv::writeFields(CFRecord& record)
{

}


void Dv::readFields(CFRecord& record)
{
	_UINT32 flags;
	record >> flags;

	valType = static_cast<unsigned char>(GETBITS(flags, 0, 3));
	errStyle = static_cast<unsigned char>(GETBITS(flags, 4, 6));
	
	fStrLookup	= GETBIT(flags, 7);
	fAllowBlank = GETBIT(flags, 8);
	fSuppressCombo = GETBIT(flags, 9);
	mdImeMode = static_cast<unsigned char>(GETBITS(flags, 10, 17));
	fShowInputMsg = GETBIT(flags, 18);
	fShowErrorMsg = GETBIT(flags, 19);
	typOperator = static_cast<unsigned char>(GETBITS(flags, 20, 23));

	record >> PromptTitle >> ErrorTitle >> Prompt >> Error;
	formula1.load(record);
	formula2.load(record);
	record >> sqref;
}

} // namespace XLS

