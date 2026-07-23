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

#include <vector>
#include <iostream>
#include <algorithm>

// Commands
std::wstring sCmdHelp =                         L"--h";
std::wstring sCmdHelpFull =                     L"--help";

std::wstring sCmdPluginsDir =                   L"--directory";
std::wstring sCmdMarketplaceUrl =               L"--marketplace";

std::wstring sCmdPrintInstalled =               L"--print-installed";
std::wstring sCmdPrintRemoved =                 L"--print-removed";
std::wstring sCmdPrintMarketplace =             L"--print-marketplace";
std::wstring sCmdPrintBackup =                  L"--print-backup";

std::wstring sCmdReset =                        L"--reset";
std::wstring sCmdInstall =                      L"--install";
std::wstring sCmdRestore =                      L"--restore";
std::wstring sCmdUpdate =                       L"--update";
std::wstring sCmdUpdateAll =                    L"--update-all";
std::wstring sCmdAutorename =                   L"--autorename";
std::wstring sCmdRemove =                       L"--remove";
std::wstring sCmdRemoveAll =                    L"--remove-all";
std::wstring sCmdIgnore =                       L"--ignore";

// Settings
std::wstring sSetRemoved =                      L"--removed=";

std::vector<std::wstring> arrCommands {sCmdHelp, sCmdHelpFull, sCmdPluginsDir, sCmdMarketplaceUrl,
									  sCmdPrintInstalled, sCmdPrintRemoved, sCmdPrintMarketplace, sCmdPrintBackup,
									  sCmdReset, sCmdInstall, sCmdRestore, sCmdUpdate, sCmdUpdateAll, sCmdAutorename,
									  sCmdRemove, sCmdRemoveAll, sCmdIgnore};

bool IsCommandExists(std::wstring sCommand)
{
	return std::find(arrCommands.begin(), arrCommands.end(), sCommand) != arrCommands.end();
}

bool IsNeedSetValue(std::wstring sParam)
{
	return sParam == sCmdPluginsDir ||
		   sParam == sCmdInstall ||
		   sParam == sCmdRestore ||
		   sParam == sCmdUpdate ||
		   sParam == sCmdRemove ||
		   sParam == sCmdIgnore;
}

std::wstring sHelpText =
L"\nPlugins Manager Options\n\n" \

L"  --h                                            \n" \
L"  --help               = Print usage information.\n\n" \

L"  --directory          = Required parameter. Set plugins folder in quotes.\n" \
L"                         For example, --directory=\"/home/mark/plugins\"\n" \
L"                         The symbol = can be skipped for all parameters.\n" \
L"                         Must be first on the command line.\n\n" \

L"  --marketplace        = Optional parameter. Set plugins marketplace URL in quotes.\n" \
L"                         Default is https://onlyoffice.github.io\n\n" \

L"  --print-installed    = Optional parameter without value. Print installed plugins.\n\n" \

L"  --print-removed      = Optional parameter without value. Print removed plugins.\n\n" \

L"  --print-marketplace  = Optional parameter without value. Print available plugins from the marketplace.\n\n" \

L"  --print-backup       = Optional parameter without value. Print backup plugins.\n" \
L"                         Backup plugins can be restored using --restore option\n\n" \

L"  --reset              = Optional parameter without value. Reset user setting.\n\n" \

L"  --install            = Optional parameter. Set comma-separated quoted list of plugins to install.\n" \
L"                         Installing by plugin name,   for example, --install=\"photo editor\"\n" \
L"                                       GUID,          for example, --install=\"{7327FC95-16DA-41D9-9AF2-0E7F449F6800}\"\n" \
L"                                       URL,           for example, --install=\"https://anysite.com/deploy/chess.plugin\"\n" \
L"                                       file path,     for example, --install=\"/home/mark/work/speech.plugin\"\n" \
L"                                       config path,   for example, --install=\"/home/mark/work/plugins.json\"\n" \
L"                         is supported.\n" \
L"                         Combined installation is supported, for example, --install=\"photo editor, {7327FC95-16DA-41D9-9AF2-0E7F449F6800}, etc\".\n" \
L"                         File paths without quotes, plugin extensions: .zip, .plugin.\n\n" \

L"  --restore            = Optional parameter. Set comma-separated quoted list of plugins to restore.\n" \
L"                         Print backup plugins using --print-backup option.\n" \
L"                         Restoring by plugin name or GUID is supported.\n" \
L"                         For example, --restore=\"Youtube, {7327FC95-16DA-41D9-9AF2-0E7F449F6800}\"\n\n" \

L"  --update             = Optional parameter. Set comma-separated quoted list of plugins to update.\n" \
L"                         Updating by plugin name, GUID, config path is supported.\n" \
L"                         For example, --update=\"Youtube, PhotoEditor, {7327FC95-16DA-41D9-9AF2-0E7F449F6800}\"\n\n" \

L"  --update-all         = Optional parameter without value. Update all installed plugins from marketplace.\n\n" \

L"  --autorename         = Optional parameter without value.\n" \
L"                         Rename installed plugins folders from name to GUID.\n" \
L"                         For example, youtube -> {38E022EA-AD92-45FC-B22B-49DF39746DB4}\n\n" \

L"  --remove             = Optional parameter. Set comma-separated quoted list of plugins to remove.\n" \
L"                         Removing by plugin name, GUID, config path is supported.\n" \
L"                         For example, --remove=\"Youtube, PhotoEditor, {7327FC95-16DA-41D9-9AF2-0E7F449F6800}\"\n\n" \

L"  --remove-all         = Optional parameter without value. Remove all installed plugins.\n\n" \

L"  --ignore             = Optional parameter. Set comma-separated quoted list of plugins to ignore actions,\n" \
L"                         such as update, remove, rename or restore.\n" \
L"                         For example, --ignore=\"youtube, {7327FC95-16DA-41D9-9AF2-0E7F449F6800}\"\n" \
;
