#ifndef HTMLTOXHTML_H
#define HTMLTOXHTML_H

#include <string>

namespace HTML
{
std::wstring htmlToXhtml(std::string& sFileContent, bool bNeedConvert);
std::wstring mhtToXhtml(std::string& sFileContent);
}

#endif // HTMLTOXHTML_H
