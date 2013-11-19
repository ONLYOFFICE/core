#include "stdafx.h"
#include "EndOfStreamReached.h"


namespace EXCEPT
{;
namespace RT
{;

EndOfStreamReached::EndOfStreamReached(const std::string stream_name, const size_t num_read, const size_t num_requested)
	: CompoundFileFormatError("End of stream \"" + stream_name + "\" reached. Only " + STR::int2str(num_read, 10) + 
								" bytes of " + STR::int2str(num_requested, 10) + " have been read", S_OK)
{
}

} // namespace RT
} // namespace EXCEPT
