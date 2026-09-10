#ifndef HTML2SVG_H
#define HTML2SVG_H

#include <string>

namespace SVG
{
	std::wstring ConvertForeignObjectToSvg(const std::wstring& wsHTML, const double& dWidth = 300.0, const std::wstring& wsRootAttrs = std::wstring());
}

#endif // HTML2SVG_H
