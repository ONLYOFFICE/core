#ifndef MD2HTML_H
#define MD2HTML_H

#include <string>

std::string ConvertMdStringToHtml(const std::string& sMdString);
bool ConvertMdFileToHtml(const std::wstring& wsPathToMdFile, const std::wstring& wsPathToHtmlFile);

#endif // MD2HTML_H
