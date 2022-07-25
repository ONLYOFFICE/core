#pragma once


#include <sstream>
#include <fstream>
#include <memory>
#include "../../DesktopEditor/common/Types.h"

namespace CFCPP
{
using Stream = std::shared_ptr<std::iostream>;
std::streamsize Length(const Stream& st);
}
