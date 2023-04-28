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

#include <iostream>
#include <map>
#include <iomanip>
#include <filesystem>

#ifdef WIN32
#include <locale.h>
#endif

#include "help.h"
#include "../common/File.h"
#include "../common/Directory.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/graphics/BaseThread.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../Common/Network/FileTransporter/include/FileTransporter.h"

#ifdef CreateDirectory
#undef CreateDirectory
#endif

#ifdef GetTempPath
#undef GetTempPath
#endif

#ifdef LINUX
#include <unistd.h>
#include <stdio.h>
#endif

// Misc
std::wstring CorrectDir(const std::wstring& sDir)
{
	if (sDir.empty())
		return L"";

	const wchar_t* data = sDir.c_str();

	std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
	std::wstring::size_type pos2 = sDir.length();

	if (data[pos2 - 1] == '\"')
		--pos2;

	if (pos2 > 0 && ((data[pos2 - 1] == '\\') || (data[pos2 - 1] == '/')))
		--pos2;

	return sDir.substr(pos1, pos2 - pos1);
}

std::wstring CorrectValue(const std::wstring& value)
{
	if (value.empty())
		return L"";

	const wchar_t* data = value.c_str();

	std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
	std::wstring::size_type pos2 = value.length();

	if (data[pos2 - 1] == '\"')
		--pos2;

	return value.substr(pos1, pos2 - pos1);
}

// Manager
class CPluginsManager
{
private:
	std::wstring m_sManagerGuid =		L"{AA2EA9B6-9EC2-415F-9762-634EE8D9A95E}";
	std::wstring m_sOldManagerGuid =	L"{8D67F3C5-7736-4BAE-A0F2-8C7127DC4BB8}";

public:
	std::wstring m_sPluginsDir;
	std::wstring m_sMarketplaceUrl;

	std::vector<std::wstring> m_arrInstallPlugins;
	std::vector<std::wstring> m_arrRestorePlugins;
	std::vector<std::wstring> m_arrRemovePlugins;

	std::map<std::wstring, std::wstring> m_arrBackupPlugins;
	std::map<std::wstring, std::wstring> m_arrInstalledPlugins;

	// short_name - [long_name, GUID]
	std::map<std::wstring, std::pair<std::wstring, std::wstring>> m_arrMarketplacePlugins;

	CPluginsManager()
	{
		m_sPluginsDir = L"";
		m_sMarketplaceUrl = L"https://onlyoffice.github.io";
	}

	// Usability
	void ViewHelp()
	{
		Message(sHelpText);
	}

	// Set
	bool SetInstallPlugins(std::wstring& sPluginsList)
	{
		return SplitStringAsVector(sPluginsList, L",", m_arrInstallPlugins);
	}

	bool SetRestorePlugins(std::wstring& sPluginsList)
	{
		return SplitStringAsVector(sPluginsList, L",", m_arrRestorePlugins);
	}

	bool SetRemovePlugins(std::wstring& sPluginsList)
	{
		return SplitStringAsVector(sPluginsList, L",", m_arrRemovePlugins);
	}

	// Multi
	bool InstallPluginsList()
	{
		bool bResult = true;
		Message(L"Install plugins ...", L"", true, true);

		InitPlugins();

		if (m_sPluginsDir.length() && m_arrInstallPlugins.size() && m_arrMarketplacePlugins.size())
		{
			for (size_t i = 0; i < m_arrInstallPlugins.size(); i++)
			{
				std::wstring sPlugin = m_arrInstallPlugins[i];
				bResult &= InstallPlugin(sPlugin);
			}
		}

		GetInstalledPlugins();

		return bResult;
	}

	bool RestorePluginsList()
	{
		bool bResult = true;
		Message(L"Restore plugins ...", L"", true, true);

		InitPlugins(true);

		if (m_sPluginsDir.length() && m_arrRestorePlugins.size())
		{
			for (size_t i = 0; i < m_arrRestorePlugins.size(); i++)
			{
				std::wstring sPlugin = m_arrRestorePlugins[i];
				bResult &= RestorePlugin(sPlugin);
			}
		}

		GetInstalledPlugins();

		return bResult;
	}

	bool RemovePluginsList()
	{
		bool bResult = true;
		Message(L"Remove plugins ...", L"", true, true);

		InitPlugins();

		if (m_sPluginsDir.length() && m_arrRemovePlugins.size() && m_arrMarketplacePlugins.size())
		{
			for (size_t i = 0; i < m_arrRemovePlugins.size(); i++)
			{
				std::wstring sPlugin = m_arrRemovePlugins[i];
				bResult &= RemovePlugin(sPlugin);
			}
		}

		GetInstalledPlugins();

		return bResult;
	}

	bool RemoveAllPlugins()
	{
		bool bResult = true;
		Message(L"Remove all installed plugins ...", L"", true, true);

		InitPlugins();

		if (m_sPluginsDir.length() && m_arrInstalledPlugins.size())
		{
			std::map<std::wstring, std::wstring>::iterator it;

			for (it = m_arrInstalledPlugins.begin(); it != m_arrInstalledPlugins.end(); it++)
			{
				std::wstring sGuid = it->second;
				bResult &= RemovePlugin(sGuid);
			}
		}

		GetInstalledPlugins();

		return bResult;
	}

	// Local and Marketplace
	void GetInstalledPlugins(bool bPrint = true)
	{
		GetFolderPlugins(m_arrInstalledPlugins, false, bPrint);
	}

	bool GetBackupPlugins(bool bPrint = true)
	{
		return GetFolderPlugins(m_arrBackupPlugins, true, bPrint);
	}

	bool GetMarketplacePlugins(bool bPrint = true)
	{
		bool bResult = false;

		if ( bPrint )
			Message(L"Initialize marketplace plugins ...", L"", true, true);

		if (m_sMarketplaceUrl.length())
		{
			m_arrMarketplacePlugins.clear();

			std::wstring sConfigUrl = m_sMarketplaceUrl + L"/store/config.json";
			std::wstring sTmpFile = NSFile::CFileBinary::GetTempPath() + L"/temp_asc_plugins_config.json";

			if (DownloadFile(sConfigUrl, sTmpFile))
			{
				std::wstring sJson = L"";
				if (NSFile::CFileBinary::ReadAllTextUtf8(sTmpFile, sJson))
				{
					NSStringUtils::string_replace(sJson, L"\n", L"");
					NSStringUtils::string_replace(sJson, L"\r", L"");
					NSStringUtils::string_replace(sJson, L"\t", L"");
					NSStringUtils::string_replace(sJson, L"[", L"");
					NSStringUtils::string_replace(sJson, L"]", L"");
					NSStringUtils::string_replace(sJson, L"\"", L"");

					NSFile::CFileBinary::Remove(sTmpFile);

					std::vector<std::wstring> arr;
					if (SplitStringAsVector(sJson, L",", arr))
					{
						for (size_t i = 0; i < arr.size(); i++)
						{
							std::wstring sPluginName = arr[i];
							std::transform(sPluginName.begin(), sPluginName.end(), sPluginName.begin(), tolower);

							if (m_arrMarketplacePlugins.find(sPluginName) == m_arrMarketplacePlugins.end())
							{
								std::wstring sGuid = L"";
								std::wstring sName = L"";

								if (FetchPluginInfo(sPluginName, sName, sGuid))
								{
									std::wstring sNameLow = sName;
									std::transform(sNameLow.begin(), sNameLow.end(), sNameLow.begin(), tolower);

									m_arrMarketplacePlugins.insert(std::pair<std::wstring, std::pair<std::wstring, std::wstring>>(sPluginName, std::make_pair(sNameLow, sGuid)));
								}

								if ( bPrint )
									MessageTableView(sPluginName, sGuid);
							}
						}
					}
				}
			}
		}

		return bResult;
	}

private:
	// Single
	std::wstring FindLocalPlugin(std::map<std::wstring, std::wstring>& arrPlugins, std::wstring& sPlugin)
	{
		std::wstring sGuid = L"";

		if (arrPlugins.size())
		{
			if (IsGuid(sPlugin))
			{
				// Try work as is
				sGuid = sPlugin;
			}
			else
			{
				// Find by name
				// Plugin name from application interface or config
				// Case doesn't matter

				std::wstring sPluginName = sPlugin;
				std::transform(sPluginName.begin(), sPluginName.end(), sPluginName.begin(), tolower);

				if (arrPlugins.find(sPluginName) != arrPlugins.end())
					sGuid = arrPlugins.at(sPluginName);
			}
		}

		return sGuid;
	}

	std::wstring FindMarketplacePlugin(std::wstring& sPlugin, bool bReturnGuid = false)
	{
		std::wstring sName = L"";

		if (m_arrMarketplacePlugins.size())
		{
			if (IsGuid(sPlugin))
			{
				std::map<std::wstring, std::pair<std::wstring, std::wstring>>::iterator it;
				for (it = m_arrMarketplacePlugins.begin(); it != m_arrMarketplacePlugins.end(); it++)
				{
					if (it->second.second == sPlugin)
					{
						sName = bReturnGuid ? it->second.second : it->first;
						break;
					}
				}
			}
			else
			{
				std::wstring sPluginName = sPlugin;
				std::transform(sPluginName.begin(), sPluginName.end(), sPluginName.begin(), tolower);

				if (m_arrMarketplacePlugins.find(sPluginName) != m_arrMarketplacePlugins.end())
				{
					sName = bReturnGuid ? m_arrMarketplacePlugins.at(sPluginName).second : sPluginName;
				}
				else
				{
					// Search second, long name
					std::map<std::wstring, std::pair<std::wstring, std::wstring>>::iterator it;
					for (it = m_arrMarketplacePlugins.begin(); it != m_arrMarketplacePlugins.end(); it++)
					{
						if (it->second.first == sPluginName)
						{
							sName = bReturnGuid ? it->second.second : it->first;
							break;
						}
					}
				}
			}
		}

		return sName;
	}

	bool InstallPlugin(std::wstring& sPlugin)
	{
		bool bResult = false;

		if (sPlugin.length())
		{
			bool isDownload = true;
			std::wstring sPackageUrl = L"";

			std::wstring sTmpFile = NSFile::CFileBinary::GetTempPath() + L"/temp_asc_plugin.plugin";
			if (NSFile::CFileBinary::Exists(sTmpFile))
				NSFile::CFileBinary::Remove(sTmpFile);

			std::wstring sTempDir = m_sPluginsDir + L"/temp_asc_plugin_directory";
			std::wstring sTempDirExt = sTempDir;

			// Search by name or GUID
			std::wstring sName = FindMarketplacePlugin(sPlugin);

			if (sName.length())
			{
				sPackageUrl = m_sMarketplaceUrl + L"/sdkjs-plugins/content/" + sName + L"/deploy/" + sName + L".plugin";
			}
			else if (IsNeedDownload(sPlugin))
			{
				// Try downlaod as is and unzip
				sPackageUrl = sPlugin;
			}
			else if (NSFile::CFileBinary::Exists(sPlugin))
			{
				sTmpFile = sPlugin;
				isDownload = false;
			}

			if (isDownload)
			{
				DownloadFile(sPackageUrl, sTmpFile);
			}

			if (NSDirectory::Exists(sTempDir))
				NSDirectory::DeleteDirectory(sTempDir);
			NSDirectory::CreateDirectory(sTempDir);

			COfficeUtils oOfficeUtils(NULL);
			if (S_OK == oOfficeUtils.ExtractToDirectory(sTmpFile, sTempDirExt, NULL, 0))
			{
				std::wstring sConfigFile = sTempDirExt + L"/config.json";

				// zip with subfolder
				if (!NSFile::CFileBinary::Exists(sConfigFile))
				{
					std::vector<std::wstring> arrDirs = NSDirectory::GetDirectories(sTempDirExt);
					if (arrDirs.size() == 1)
					{
						sTempDirExt = arrDirs[0];
						sConfigFile = sTempDirExt + L"/config.json";
					}
				}

				std::wstring sGuid = ReadPluginGuid(sConfigFile);

				if (sGuid.size())
				{
					std::wstring sPluginDir = m_sPluginsDir + L"/" + sGuid;

					if (NSDirectory::Exists(sPluginDir))
						NSDirectory::DeleteDirectory(sPluginDir);
					NSDirectory::CreateDirectory(sPluginDir);

					NSDirectory::CopyDirectory(sTempDirExt, sPluginDir);

					bResult = true;
				}

				NSDirectory::DeleteDirectory(sTempDir);
			}

			if ( isDownload )
				NSFile::CFileBinary::Remove(sTmpFile);
		}

		Message(L"Install plugin: " + sPlugin, BoolToStr(bResult), true);

		return bResult;
	}

	bool RestorePlugin(std::wstring& sPlugin)
	{
		bool bResult = false;

		if (sPlugin.length())
		{
			std::wstring sGuid = FindLocalPlugin(m_arrBackupPlugins, sPlugin);

			if ( sGuid.length() )
			{
				std::wstring sPluginDir = m_sPluginsDir + L"/" + sGuid;
				std::wstring sPluginBackupDir = m_sPluginsDir + L"/backup/" + sGuid;

				if (NSDirectory::Exists(sPluginBackupDir))
				{
					NSDirectory::CopyDirectory(sPluginBackupDir, sPluginDir);
					NSDirectory::DeleteDirectory(sPluginBackupDir);

					bResult = true;
				}
			}
		}

		Message(L"Restore plugin: " + sPlugin, BoolToStr(bResult), true);

		return bResult;
	}

	bool RemovePlugin(std::wstring& sPlugin)
	{
		bool bResult = false;

		if (sPlugin.length())
		{
			bool bBackup = false;
			std::wstring sGuid = FindLocalPlugin(m_arrInstalledPlugins, sPlugin);

			// Try find in marketplace if name isn't short alias
			if ( !sGuid.length() )
				sGuid = FindMarketplacePlugin(sPlugin, true);
			else
			{
				// Need create backup for plugin if doesn't exist in the marketplace
				std::wstring sCheckGuid = FindMarketplacePlugin(sPlugin, true);
				if ( !sCheckGuid.length() )
					bBackup = true;
			}

			if (sGuid.length())
			{
				std::wstring sPluginDir = m_sPluginsDir + L"/" + sGuid;

				if (NSDirectory::Exists(sPluginDir))
				{
					if (bBackup)
					{
						std::wstring sBackupDir = m_sPluginsDir + L"/backup";
						std::wstring sPluginBackupDir = sBackupDir + L"/" + sGuid;

						if (!NSDirectory::Exists(sBackupDir))
							NSDirectory::CreateDirectory(sBackupDir);

						if (!NSDirectory::Exists(sPluginBackupDir))
							NSDirectory::DeleteDirectory(sPluginBackupDir);

						NSDirectory::CopyDirectory(sPluginDir, sPluginBackupDir);

						Message(L"Backup plugin: " + sPlugin, BoolToStr(bBackup), true);
					}

					NSDirectory::DeleteDirectory(sPluginDir);
					bResult = true;
				}
			}
		}

		Message(L"Remove plugin: " + sPlugin, BoolToStr(bResult), true);

		return bResult;
	}

	// Methods
	void InitPlugins(bool bBackup = false)
	{
		if ( !m_arrBackupPlugins.size() )
			GetBackupPlugins(false);

		// Backup plugins don't exist in the marketplace, without additional initialization
		if ( !bBackup )
		{
			if ( !m_arrInstallPlugins.size() )
				GetInstalledPlugins(false);

			if ( !m_arrMarketplacePlugins.size() )
				GetMarketplacePlugins(false);
		}
	}

	bool GetFolderPlugins(std::map<std::wstring, std::wstring>& arrPlugins, bool bBackup = false, bool bPrint = true)
	{
		bool bResult = false;

		if ( bPrint )
			Message(bBackup ? L"Backup plugins:" : L"Installed plugins:", L"", true, true);

		if (m_sPluginsDir.length())
		{
			arrPlugins.clear();

			std::vector<std::wstring> arrDirs = NSDirectory::GetDirectories(m_sPluginsDir + (bBackup ? L"/backup" : L""));

			for (size_t i = 0; i < arrDirs.size(); ++i)
			{
				std::wstring sFile = arrDirs[i] + L"/config.json";

				if (NSFile::CFileBinary::Exists(sFile))
				{
					std::wstring sGuid = ReadPluginGuid(sFile);
					std::wstring sPluginName = ReadPluginName(sFile);
					std::transform(sPluginName.begin(), sPluginName.end(), sPluginName.begin(), tolower);

					if ( !IsPluginManager(sGuid) && sPluginName.length() && sGuid.length() )
					{
						if (arrPlugins.find(sPluginName) == arrPlugins.end())
						{
							bResult = true;
							arrPlugins.insert(std::pair<std::wstring, std::wstring>(sPluginName, sGuid));

							if ( bPrint )
								MessageTableView(sPluginName, sGuid);
						}
					}
				}
			}
		}
		else
		{
			Message(L"Set plugin folder. Use the following parameter: " + sCmdPluginsDir, L"", true);
		}

		return bResult;
	}

	bool IsGuid(std::wstring& sStr)
	{
		return sStr.length() && sStr.at(0) == L'{' && sStr.at(sStr.length() - 1) == L'}';
	}

	bool IsNeedDownload(std::wstring& FilePath)
	{
		std::wstring::size_type n1 = FilePath.find(L"www.");
		std::wstring::size_type n2 = FilePath.find(L"http://");
		std::wstring::size_type n3 = FilePath.find(L"ftp://");
		std::wstring::size_type n4 = FilePath.find(L"https://");

		if (n1 != std::wstring::npos && n1 < 10)
			return true;
		if (n2 != std::wstring::npos && n2 < 10)
			return true;
		if (n3 != std::wstring::npos && n3 < 10)
			return true;
		if (n4 != std::wstring::npos && n4 < 10)
			return true;

		return false;
	}

	bool IsPluginManager(std::wstring& sGuid)
	{
		return sGuid == m_sManagerGuid || sGuid == m_sOldManagerGuid;
	}

	bool SplitStringAsVector(const std::wstring& sStr, const std::wstring& sDelimiter, std::vector<std::wstring>& arrOutput)
	{
		arrOutput.clear();

		if (sStr.length())
		{
			std::wstring _str = sStr;
			NSStringUtils::string_replace(_str, L", ", L",");

			size_t pos_start = 0, pos_end, delim_len = sDelimiter.length();
			std::wstring token = L"";

			while ((pos_end = _str.find(sDelimiter, pos_start)) != std::string::npos)
			{
				token = _str.substr(pos_start, pos_end - pos_start);
				pos_start = pos_end + delim_len;
				if (token.length())
					arrOutput.push_back(token);
			}

			token = _str.substr(pos_start);
			if (token.length())
				arrOutput.push_back(token);
		}

		return arrOutput.size() > 0;
	}

	bool DownloadFile(std::wstring& sUrl, std::wstring& sFile)
	{
		if (NSFile::CFileBinary::Exists(sFile))
			NSFile::CFileBinary::Remove(sFile);

		NSNetwork::NSFileTransport::CFileDownloader oDownloader(sUrl, false);
		oDownloader.SetFilePath(sFile);
		oDownloader.Start(0);
		while (oDownloader.IsRunned())
		{
			NSThreads::Sleep(10);
		}

		return NSFile::CFileBinary::Exists(sFile);
	}

	bool FetchPluginInfo(std::wstring& sPluginName, std::wstring& sName, std::wstring& sGuid)
	{
		bool bResult = false;

		std::wstring sTmpFile = NSFile::CFileBinary::GetTempPath() + L"/temp_asc_plugin.json";
		std::wstring sConfigUrl = m_sMarketplaceUrl + L"/sdkjs-plugins/content/" + sPluginName + L"/config.json";

		if (NSFile::CFileBinary::Exists(sTmpFile))
			NSFile::CFileBinary::Remove(sTmpFile);

		if (DownloadFile(sConfigUrl, sTmpFile))
		{
			sName = ReadPluginName(sTmpFile);
			sGuid = ReadPluginGuid(sTmpFile);

			NSFile::CFileBinary::Remove(sTmpFile);

			bResult = true;
		}

		return bResult;
	}

	std::wstring ReadPluginGuid(std::wstring& sConfigFile)
	{
		std::wstring sGuid = L"";
		std::wstring sJson = L"";

		if (NSFile::CFileBinary::ReadAllTextUtf8(sConfigFile, sJson))
		{
			std::wstring::size_type pos1 = sJson.find(L"asc.{");
			std::wstring::size_type pos2 = sJson.find(L"}", pos1);

			if (pos1 != std::wstring::npos && pos2 != std::wstring::npos && pos2 > pos1)
			{
				sGuid = sJson.substr(pos1 + 4, pos2 - pos1 - 3);
			}
		}

		return sGuid;
	}

	std::wstring ReadPluginName(std::wstring& sConfigFile)
	{
		std::wstring sName = L"";
		std::wstring sJson = L"";

		if (NSFile::CFileBinary::ReadAllTextUtf8(sConfigFile, sJson))
		{
			std::wstring sDelim = L"\"name\"";
			std::wstring::size_type pos1 = sJson.find(sDelim);
			std::wstring::size_type pos2 = sJson.find(L"\"", pos1 + sDelim.length());
			std::wstring::size_type pos3 = sJson.find(L"\"", pos2 + 1);

			if (pos1 != std::wstring::npos && pos2 != std::wstring::npos && pos3 != std::wstring::npos && pos3 > pos2)
			{
				sName = sJson.substr(pos2 + 1, pos3 - pos2 - 1);
			}
		}

		return sName;
	}

	std::wstring BoolToStr(bool bResult)
	{
		return bResult ? L"OK" : L"ERROR";
	}

	// Terminal
	void Message(std::wstring sText, std::wstring sResult = L"", bool bUseIndent = false, bool bSeparator = false)
	{
		if (bUseIndent)
			std::wcout << L"" << std::endl;

		std::wcout << sText.c_str() << std::endl;
		if (sResult.length())
			std::wcout << sResult.c_str() << std::endl;

		if (bSeparator)
			std::wcout << L"---------------------------------------------------------------------" << std::endl;
	}

	void MessageTableView(std::wstring sCol1, std::wstring sCol2)
	{
		std::wcout << std::setw(30) << std::left << sCol1.c_str() << sCol2.c_str() << std::endl;
	}
};

// Main
#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
#ifdef _WIN32
	setlocale(LC_ALL, "");
#endif

	CPluginsManager oManager;

	// Parse arguments
	for (int i = 0; i < argc; ++i)
	{
#ifdef WIN32
		std::wstring sParam(argv[i]);
#else
		std::string sParamA(argv[i]);
		std::wstring sParam = UTF8_TO_U(sParamA);
#endif

		if (sParam.find(L"--") == 0)
		{
			std::wstring sKey = L"";
			std::wstring sValue = L"";

			std::wstring::size_type _pos = sParam.find('=');
			if (std::wstring::npos == _pos)
			{
				sKey = sParam;
			}
			else
			{
				sKey = sParam.substr(0, _pos);
				sValue = sParam.substr(_pos + 1);
			}

			// Usability
			if (sKey == sCmdHelp || sKey == sCmdHelpFull)
			{
				oManager.ViewHelp();
			}

			// Settings
			else if (sKey == sCmdPluginsDir)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
					oManager.m_sPluginsDir = sValue;
			}
			else if (sKey == sCmdMarketplaceUrl)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
					oManager.m_sMarketplaceUrl = sValue;
			}

			// Print
			else if (sKey == sCmdPrintInstalled)
			{
				oManager.GetInstalledPlugins();
			}
			else if (sKey == sCmdPrintMarketplace)
			{
				oManager.GetMarketplacePlugins();
			}
			else if (sKey == sCmdPrintBackup)
			{
				oManager.GetBackupPlugins();
			}

			// Install / Remove
			else if (sKey == sCmdInstallPluginsList)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
				{
					oManager.SetInstallPlugins(sValue);
					oManager.InstallPluginsList();
				}
			}
			else if (sKey == sCmdRestorePluginsList)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
				{
					oManager.SetRestorePlugins(sValue);
					oManager.RestorePluginsList();
				}
			}
			else if (sKey == sCmdRemovePluginsList)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
				{
					oManager.SetRemovePlugins(sValue);
					oManager.RemovePluginsList();
				}
			}
			else if (sKey == sCmdRemoveAllPlugins)
			{
				oManager.RemoveAllPlugins();
			}
		}
	}

	return 0;
}

