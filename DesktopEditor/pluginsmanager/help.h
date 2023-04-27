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
std::wstring sCmdHelp =                         L"--help";
std::wstring sCmdMakeConfig =                   L"--make-config";
std::wstring sCmdPluginsDir =                   L"--plugins-dir";
std::wstring sCmdMarketplaceUrl =               L"--marketplace-url";
std::wstring sCmdMarketplacePlugins =           L"--marketplace-plugins";
std::wstring sCmdInstalledPlugins =             L"--installed-plugins";
std::wstring sCmdInstallPluginsList =           L"--install-plugins-list";
std::wstring sCmdRemovePluginsList =            L"--remove-plugins-list";

std::wstring sHelpText =
L"\nPlugins Manager\n\n" \
L"This file describes the settings to be used by the utility\n\n" \

L"Options\n" \
L"  --help                       = Print usage information\n" \
L"  --make-config                = Optional. Create an empty configuration file\n" \
L"  --plugins-dir                = Required. Set plugins folder. Must be first on the command line\n" \
L"  --marketplace-url            = Optional. Set plugins marketplace URL. Default is https://onlyoffice.github.io\n" \
L"  --marketplace-plugins        = Required. Initialize and print a list of available plugins from the marketplace\n" \
L"  --installed-plugins          = Required. Initialize and print a list of installed plugins\n" \
L"  --install-plugins-list       = Optional. Comma-separated list of plugins to install\n" \
L"  --remove-plugins-list        = Optional. Comma-separated list of plugins to remove\n"
;

std::wstring sConfigText =
L"\n# Plugins Manager Configuration\n" \
L"# This file describes the settings to be used by the utility\n\n" \

L"# Required parameter. Set path to plugins folder in quotes\n" \
L"--plugins-dir=\"\"\n\n" \

L"# Optional parameter. Set plugins marketplace URL. Default is https://onlyoffice.github.io\n" \
L"--marketplace-url=\"\"\n\n" \

L"# Required parameter. Initialize and print a list of available plugins from the marketplace. Parameter without value\n" \
L"--marketplace-plugins\n\n" \

L"# Required parameter. Initialize and print a list of installed plugins. Parameter without value\n" \
L"--installed-plugins\n\n" \

L"# Optional parameter. Comma-separated list of plugins to install\n" \
L"# Installing by plugin name, GUID, plugin URL, file path (without commas, extensions: .zip, .plugin) is supported\n" \
L"# For example, \"photo editor, {7327FC95-16DA-41D9-9AF2-0E7F449F6800}, c:\\\\plugins\\myplugin.plugin, https://onlyoffice.github.io/sdkjs-plugins/content/translator/deploy/translator.plugin\"\n" \
L"--install-plugins-list=\"\"\n\n" \

L"# Optional parameter. Comma-separated list of plugins to remove\n" \
L"# Removing by plugin name or by GUID is supported\n" \
L"# For example, \"Youtube, PhotoEditor, {7327FC95-16DA-41D9-9AF2-0E7F449F6800}\"\n" \
L"--remove-plugins-list=\"\"\n"
;


