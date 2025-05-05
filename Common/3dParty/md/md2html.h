#ifndef MD2HTML_H
#define MD2HTML_H

#include <string>

#ifndef MDCONVERTER_DECL_EXPORT
#define MDCONVERTER_DECL_EXPORT
#else
#include "../../../DesktopEditor/common/base_export.h"
#define MDCONVERTER_DECL_EXPORT Q_DECL_EXPORT
#endif

namespace Md
{
std::string MDCONVERTER_DECL_EXPORT ConvertMdStringToHtml(const std::string& sMdString);
bool MDCONVERTER_DECL_EXPORT ConvertMdFileToHtml(const std::wstring& wsPathToMdFile, const std::wstring& wsPathToHtmlFile);
}

#endif // MD2HTML_H
