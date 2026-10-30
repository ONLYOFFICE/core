// SPDX-License-Identifier: AGPL-3.0-only
#pragma once

// Legacy Macintosh script charmaps use script-specific byte codes, not Unicode
// scalar values. Never interpret those codes as Unicode coverage or glyphs.
inline bool IsLegacyMacScriptCharMap(const FT_CharMap map)
{
	return map->platform_id == 1 && map->encoding_id != 0 && map->encoding != FT_ENCODING_UNICODE;
}
