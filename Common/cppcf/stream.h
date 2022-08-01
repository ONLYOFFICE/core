#pragma once


#include <sstream>
#include <fstream>
#include <memory>
#include "../../DesktopEditor/common/Types.h"

namespace CFCPP
{
using Stream = std::shared_ptr<std::iostream>;

std::string CorrectUnixPath(const std::string original);

Stream OpenStream(std::wstring filename, bool bRewrite = false);
Stream OpenStream(std::string filename, bool bRewrite = false);

bool IsOpen(const Stream& st);
std::streamsize Length(const Stream& st);
}
