
#include "../../DesktopEditor/common/Types.h"
#include <string>

struct ProgressCallback;

HRESULT ConvertOO2OOX(const std::wstring & srcPath, const std::wstring & dstPath, const std::wstring & fontsPath, bool bOnlyPresentation, const ProgressCallback* CallBack);
