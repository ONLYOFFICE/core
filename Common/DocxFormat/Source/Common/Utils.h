#pragma once
#ifndef COMMON_UTILS_INCLUDE_H_
#define COMMON_UTILS_INCLUDE_H_

#include "../SystemUtility/SystemUtility.h"
#include <vector>

namespace Common
{
void readAllShapeTypes(const OOX::CPath& oPath, std::vector<CString>& aShapetypes);
} // namespace Common

#endif // COMMON_UTILS_INCLUDE_H_
