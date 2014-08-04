#pragma once
#ifndef COMMON_UTILS_INCLUDE_H_
#define COMMON_UTILS_INCLUDE_H_

#include "../SystemUtility/SystemUtility.h"
#include <vector>

namespace Common
{
#ifdef _WIN32
    void readAllShapeTypes(const OOX::CPath& oPath, CSimpleArray<CString>& aShapetypes);
#else
	void readAllShapeTypes(const OOX::CPath& oPath, std::vector<CString>& aShapetypes);
#endif

} // namespace Common

#endif // COMMON_UTILS_INCLUDE_H_
