#pragma once

#include "CompoundFileFormatError.h"

namespace EXCEPT
{;
namespace RT
{;

class EndOfStreamReached : public CompoundFileFormatError
{
public:
	explicit EndOfStreamReached(const std::string stream_name, const size_t num_read, const size_t num_requested);
};

} // namespace RT
} // namespace EXCEPT

