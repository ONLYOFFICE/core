#include <string>

namespace cpdoccore {

// преобразование строк

std::string     ansi_to_utf8(const std::string & _AnsiString);
std::wstring    utf8_to_utf16(const std::string & _Utf8String);
std::string     utf16_to_utf8(const std::wstring & _Utf16String);

}
