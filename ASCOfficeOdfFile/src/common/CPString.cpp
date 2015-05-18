
#include "../../include/cpdoccore/CPString.h"
#include "../../../DesktopEditor/common/File.h"

namespace cpdoccore {

std::string ansi_to_utf8(const std::string & _AnsiString)
{
	std::wstring unicodeString(_AnsiString.begin(), _AnsiString.end());
	return utf16_to_utf8(unicodeString);
}

std::wstring utf8_to_utf16(const std::string & _Utf8String)
{
	return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)_Utf8String.c_str(), _Utf8String.length());
}

std::string utf16_to_utf8(const std::wstring & _Utf16String)
{
	return NSFile::CUtf8Converter::GetUtf8StringFromUnicode(_Utf16String);
}

}
