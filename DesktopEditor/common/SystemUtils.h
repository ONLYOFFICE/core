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
#pragma once

#include <string>
#include "../../Common/kernel_config.h"
#include "../../OdfFile/Common/logging.h"

#define VALUE_STRINGIFY(d) L##d
#define VALUE_TO_STR(v)    VALUE_STRINGIFY(v)

namespace NSSystemUtils
{
	static const wchar_t* gc_EnvApplicationName = L"APPLICATION_NAME";
#ifndef APPLICATION_NAME_DEFAULT
	static const wchar_t* gc_EnvApplicationNameDefault = L"ONLYOFFICE";
#else
	static const wchar_t* gc_EnvApplicationNameDefault = VALUE_TO_STR(APPLICATION_NAME_DEFAULT);
#endif
	static const wchar_t* gc_EnvCompanyName = L"COMPANY_NAME";
#ifndef COMPANY_NAME_DEFAULT
	static const wchar_t* gc_EnvCompanyNameDefault = L"Ascensio System SIA Copyright (c) 2022";
#else
	static const wchar_t* gc_EnvCompanyNameDefault = VALUE_TO_STR(COMPANY_NAME_DEFAULT);
#endif
	static const wchar_t* gc_EnvMethodEncrypt = L"METHOD_CRYPT";
	static const wchar_t* gc_EnvMethodEncryptDefault = L"Strong";
	static const wchar_t* gc_EnvCreator = L"CREATOR";
	static const wchar_t* gc_EnvCreated = L"CREATED";
	static const wchar_t* gc_EnvLastModifiedBy = L"LAST_MODIFIED_BY";
	static const wchar_t* gc_EnvModified = L"MODIFIED";
	static const wchar_t* gc_EnvMemoryLimit = L"X2T_MEMORY_LIMIT";
	static const wchar_t* gc_EnvMemoryLimitDefault = L"4GiB";
	static const wchar_t* gc_EnvSigningKeystorePassphrase = L"SIGNING_KEYSTORE_PASSPHRASE";

	KERNEL_DECL std::string GetEnvVariableA(const std::wstring& strName);
	KERNEL_DECL std::wstring GetEnvVariable(const std::wstring& strName);
	KERNEL_DECL std::wstring GetAppDataDir();
	KERNEL_DECL std::wstring GetBuildBranding();

	enum class SystemDirectoryType
	{
		Documents = 0,
		Downloads = 1,
		Desktop = 2,

		Undefined = 255
	};
	KERNEL_DECL std::wstring GetSystemDirectory(const SystemDirectoryType& type);
}
