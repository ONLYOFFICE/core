/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#pragma once
#include <iostream>

// Commands
std::wstring sCmdHelp =                         L"--h";
std::wstring sCmdHelpFull =                     L"--help";
std::wstring sCmdPluginsDir =                   L"--directory";
std::wstring sCmdMarketplaceUrl =               L"--marketplace";
std::wstring sCmdInstallPluginsList =           L"--install";
std::wstring sCmdRemovePluginsList =            L"--remove";
std::wstring sCmdRemoveAllPlugins =             L"--remove-all";

std::wstring sHelpText =
L"\nPlugins Manager Options\n\n" \

L"  --h                                            \n" \
L"  --help               = Print usage information.\n\n" \

L"  --directory          = Required parameter. Set plugins folder in quotes.\n" \
L"                         Must be first on the command line.\n\n" \

L"  --marketplace        = Optional parameter. Set plugins marketplace URL in quotes.\n" \
L"                         Default is https://onlyoffice.github.io\n\n" \

L"  --install            = Optional parameter. Set comma-separated quoted list of plugins to install.\n" \
L"                         Installing by plugin name, GUID, URL, direct file path (without commas) is supported.\n" \
L"                         For example, --install=\"photo editor, {7327FC95-16DA-41D9-9AF2-0E7F449F6800}, /home/mark/plugins/speech.plugin, https://anysite.com/deploy/translator.plugin\"\n" \
L"                         File extensions: .zip, .plugin\n\n" \

L"  --remove             = Optional parameter. Set comma-separated quoted list of plugins to remove.\n" \
L"                         Removing by plugin name or GUID is supported.\n" \
L"                         For example, --remove=\"Youtube, PhotoEditor, {7327FC95-16DA-41D9-9AF2-0E7F449F6800}\"\n\n" \

L"  --remove-all         = Optional parameter. Remove all installed plugins.\n"
L"                         Parameter without value.\n" \
;
