#pragma once

#include "exceptions.h"

namespace EXCEPT
{;
namespace STOP
{;

class FileIsEncrypted : public StopConversionException
{
public:
	explicit FileIsEncrypted();
};

} // namespace STOP
} // namespace EXCEPT

