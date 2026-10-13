/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 * SPDX-License-Identifier: AGPL-3.0-only
 * See LICENSE for additional terms.
 */
#pragma once

#include <string>
#include <regex>

namespace OOX
{
namespace Spreadsheet
{
namespace WpsCellImages
{
static const wchar_t* const RelationshipType = L"http://www.wps.cn/officeDocument/2020/cellImage";

// Only a complete, literal DISPIMG call can be replaced by a drawing.
// Expressions, dynamic IDs and other display modes must remain formulas.
inline bool GetImageId(const std::wstring& formula, std::wstring& id)
{
	static const std::wregex pattern(
		LR"wps(^\s*=?\s*@?(?:_xlfn\.)?DISPIMG\s*\(\s*"([^"]+)"\s*[,;]\s*1\s*\)\s*$)wps",
		std::regex_constants::icase);
	std::wsmatch match;
	if (!std::regex_match(formula, match, pattern))
		return false;
	id = match[1].str();
	return true;
}

} // namespace WpsCellImages
} // namespace Spreadsheet
} // namespace OOX
