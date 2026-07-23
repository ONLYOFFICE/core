/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _PROCESS_ENV_H
#define _PROCESS_ENV_H

#include "../../Common/kernel_config.h"
#include "../xml/include/xmlutils.h"
#include <string>

namespace NSProcessEnv
{
	namespace Converter
	{
		static const char* gc_allowLocalRequest   = "allowNetworkRequest";
		static const char* gc_allowNetworkRequest = "allowNetworkRequest";
		static const char* gc_allowPrivateIP      = "allowPrivateIP";
		static const char* gc_proxy               = "proxy";
		static const char* gc_proxyUser           = "proxyUser";
		static const char* gc_proxyHeader         = "proxyHeader";
		static const char* gc_oformAsPdf          = "oformAsPdf";
	}

	// serialize
	KERNEL_DECL void Load(XmlUtils::CXmlNode& node);
	KERNEL_DECL void Load(const std::string& strUtf8);
	KERNEL_DECL void Load(const std::wstring& str);
	KERNEL_DECL std::string Save();

	// props
	KERNEL_DECL bool IsPresent(const char* key);

	KERNEL_DECL bool GetBoolValue(const char* key);
	KERNEL_DECL int GetIntValue(const char* key);
	KERNEL_DECL std::string GetStringValueA(const char* key);
	KERNEL_DECL std::wstring GetStringValue(const char* key);

	// return is override
	KERNEL_DECL bool SetBoolValue(const char* key, const bool& value);
	KERNEL_DECL bool SetIntValue(const char* key, const int& value);
	KERNEL_DECL bool SetStringValueA(const char* key, const char* value);
	KERNEL_DECL bool SetStringValue(const char* key, const wchar_t* value);
}

#endif // _PROCESS_ENV_H
