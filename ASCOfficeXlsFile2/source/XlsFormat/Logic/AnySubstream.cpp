
#include "AnySubstream.h"
#include <Logic/AnyObject.h>

namespace XLS
{;


AnySubstream::AnySubstream()
{
}


AnySubstream::~AnySubstream()
{
}


BaseObjectPtr AnySubstream::clone()
{
	return BaseObjectPtr(new AnySubstream(*this));
}


// AnySubstream = 
const bool AnySubstream::loadContent(BinProcessor& proc)
{
/*
	try
	{
		while(1)
		{
			AnyObject any(own_tag);
			any.read(reader, p_MANDATORY);

			if(any.tag_name == "EOF")
			{
				break;
			}
		}
	}
	catch(EXCEPT::RT::CompoundFileFormatError&)
	{
	}
*/
	return true;
}

} // namespace XLS

