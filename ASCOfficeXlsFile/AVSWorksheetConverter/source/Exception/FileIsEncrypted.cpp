#include "stdafx.h"
#include "FileIsEncrypted.h"


namespace EXCEPT
{;
namespace STOP
{;

FileIsEncrypted::FileIsEncrypted()
: StopConversionException("The file is encrypted. No decryption procedure implemented yet. THE CONVERSION WOULD FAIL!!!")
{
}


} // namespace STOP
} // namespace EXCEPT