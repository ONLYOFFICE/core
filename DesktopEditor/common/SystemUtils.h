/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _SYSTEMUTILS_H
#define _SYSTEMUTILS_H

#include <string>
#include "../../Common/kernel_config.h"

namespace NSSystemUtils
{
	static const wchar_t* gc_EnvApplicationName = L"APPLICATION_NAME";
	static const wchar_t* gc_EnvApplicationNameDefault = L"ONLYOFFICE";
	static const wchar_t* gc_EnvCompanyName = L"COMPANY_NAME";
	static const wchar_t* gc_EnvCompanyNameDefault = L"Ascensio System SIA Copyright (c) 2022";
	static const wchar_t* gc_EnvMethodEncrypt = L"METHOD_CRYPT";
	static const wchar_t* gc_EnvMethodEncryptDefault = L"Strong";
	static const wchar_t* gc_EnvCreator = L"CREATOR";
	static const wchar_t* gc_EnvCreated = L"CREATED";
	static const wchar_t* gc_EnvLastModifiedBy = L"LAST_MODIFIED_BY";
	static const wchar_t* gc_EnvModified = L"MODIFIED";
	static const wchar_t* gc_EnvMemoryLimit = L"X2T_MEMORY_LIMIT";
	static const wchar_t* gc_EnvMemoryLimitDefault = L"4GiB";

    KERNEL_DECL std::string GetEnvVariableA(const std::wstring& strName);
    KERNEL_DECL std::wstring GetEnvVariable(const std::wstring& strName);
    KERNEL_DECL std::wstring GetAppDataDir();
    KERNEL_DECL std::wstring GetBuildBranding();
}
#endif // _SYSTEMUTILS_H
