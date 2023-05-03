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

#include <locale.h>

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
class CVersion
{
private:
	int m_major, m_minor, m_revision, m_build;

public:
	std::wstring m_sVersion;

	CVersion()
	{
		// Default if version is empty
		m_major = 1;
		m_minor = 0;
		m_revision = 0;
		m_build = 0;

		m_sVersion = L"1.0.0";
	}

	CVersion(std::wstring& sVersion)
	{
		m_sVersion = sVersion;
		swscanf_s(m_sVersion.c_str(), L"%d.%d.%d.%d", &m_major, &m_minor, &m_revision, &m_build);

		if (m_major < 0)		m_major = 0;
		if (m_minor < 0)		m_minor = 0;
		if (m_revision < 0)		m_revision = 0;
		if (m_build < 0)		m_build = 0;
	}

	bool operator > (CVersion& oVersion)
	{
		if (m_major > oVersion.m_major)
			return true;
		else if (m_major < oVersion.m_major)
			return false;

		if (m_minor > oVersion.m_minor)
			return true;
		else if (m_minor < oVersion.m_minor)
			return false;

		if (m_revision > oVersion.m_revision)
			return true;
		else if (m_revision < oVersion.m_revision)
			return false;

		if (m_build > oVersion.m_build)
			return true;
		else if (m_build < oVersion.m_build)
			return false;

		return false;
	}

	bool operator == (CVersion& oVersion)
	{
		return m_major == oVersion.m_major
				&& m_minor == oVersion.m_minor
				&& m_revision == oVersion.m_revision
				&& m_build == oVersion.m_build;
	}
};

class CPluginInfo
{
public:
	std::wstring m_sName;
	std::wstring m_sNameConfig;
	std::wstring m_sGuid;
	CVersion* m_pVersion;
	bool m_isValid;

	CPluginInfo()
	{
		m_sName = L"";
		m_sNameConfig = L"";
		m_sGuid = L"";
		m_pVersion = new CVersion();
		m_isValid = true;
	}

	CPluginInfo(std::wstring& sName, std::wstring& sNameConfig, std::wstring& sGuid, CVersion* pVersion)
	{
		m_sName = sName;
		m_sNameConfig = sNameConfig;
		m_sGuid = sGuid;
		m_pVersion = pVersion;
		m_isValid = true;
	}

	bool operator == (CPluginInfo& oPlugin)
	{
		return m_sName == oPlugin.m_sName
				&& m_sNameConfig == oPlugin.m_sNameConfig
				&& m_sGuid == oPlugin.m_sGuid
				&& m_pVersion == oPlugin.m_pVersion;
	}
};

class CPluginsManager
{
private:
	std::wstring m_sManagerGuid =		L"{AA2EA9B6-9EC2-415F-9762-634EE8D9A95E}";
	std::wstring m_sOldManagerGuid =	L"{8D67F3C5-7736-4BAE-A0F2-8C7127DC4BB8}";

public:
	std::wstring m_sPluginsDir;
	std::wstring m_sMarketplaceUrl;

	std::vector<std::wstring> m_arrInstall, m_arrRestore, m_arrUpdate, m_arrRemove;
	std::vector<CPluginInfo*> m_arrBackup, m_arrInstalled, m_arrMarketplace;

	CPluginsManager()
	{
		m_sPluginsDir = L"";
		m_sMarketplaceUrl = L"https://onlyoffice.github.io";
	}

	// Usability
	void PrintHelp()
	{
		Message(sHelpText);
	}

	// Set
	bool SetInstallPlugins(const std::wstring& sPluginsList)
	{
		return SplitStringAsVector(sPluginsList, L",", m_arrInstall);
	}

	bool SetRestorePlugins(const std::wstring& sPluginsList)
	{
		return SplitStringAsVector(sPluginsList, L",", m_arrRestore);
	}

	bool SetUpdatePlugins(const std::wstring& sPluginsList)
	{
		return SplitStringAsVector(sPluginsList, L",", m_arrUpdate);
	}

	bool SetRemovePlugins(const std::wstring& sPluginsList)
	{
		return SplitStringAsVector(sPluginsList, L",", m_arrRemove);
	}

	// Multi
	bool InstallPlugins()
	{
		bool bResult = true;
		Message(L"Install plugins ...", L"", true, true);

		InitPlugins();

		if (m_sPluginsDir.length() && m_arrInstall.size() && m_arrMarketplace.size())
		{
			for (size_t i = 0; i < m_arrInstall.size(); i++)
			{
				std::wstring sPlugin = m_arrInstall[i];
				bResult &= InstallPlugin(sPlugin);
			}
		}

		GetInstalledPlugins();

		return bResult;
	}

	bool RestorePlugins()
	{
		bool bResult = true;
		Message(L"Restore plugins ...", L"", true, true);

		InitPlugins(true);

		if (m_sPluginsDir.length() && m_arrRestore.size())
		{
			for (size_t i = 0; i < m_arrRestore.size(); i++)
			{
				std::wstring sPlugin = m_arrRestore[i];
				bResult &= RestorePlugin(sPlugin);
			}
		}

		GetInstalledPlugins();

		return bResult;
	}

	bool UpdatePlugins()
	{
		bool bResult = true;
		Message(L"Update plugins ...", L"", true, true);

		InitPlugins();

		if (m_sPluginsDir.length() && m_arrInstalled.size() && m_arrMarketplace.size())
		{
			for (size_t i = 0; i < m_arrUpdate.size(); i++)
			{
				std::wstring sPlugin = m_arrUpdate[i];
				bResult &= UpdatePlugin(sPlugin);
			}
		}

		GetInstalledPlugins();

		return bResult;
	}

	bool UpdateAllPlugins()
	{
		bool bResult = true;
		Message(L"Update all plugins ...", L"", true, true);

		InitPlugins();

		if (m_sPluginsDir.length() && m_arrInstalled.size() && m_arrMarketplace.size())
		{
			std::vector<CPluginInfo*>::iterator it;
			for (it = m_arrInstalled.begin(); it != m_arrInstalled.end(); it++)
			{
				bResult &= UpdatePlugin((*it)->m_sName);
			}
		}

		GetInstalledPlugins();

		return bResult;
	}

	bool RemovePlugins()
	{
		bool bResult = true;
		Message(L"Remove plugins ...", L"", true, true);

		InitPlugins();

		if (m_sPluginsDir.length() && m_arrRemove.size() && m_arrMarketplace.size())
		{
			for (size_t i = 0; i < m_arrRemove.size(); i++)
			{
				std::wstring sPlugin = m_arrRemove[i];
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

		if (m_sPluginsDir.length() && m_arrInstalled.size())
		{
			std::vector<CPluginInfo*>::iterator it;
			for (it = m_arrInstalled.begin(); it != m_arrInstalled.end(); it++)
			{
				std::wstring sName = (*it)->m_sNameConfig;
				bResult &= RemovePlugin(sName);
			}
		}

		GetInstalledPlugins();

		return bResult;
	}

	// Local and Marketplace
	void GetInstalledPlugins(bool bPrint = true)
	{
		// Version control
		GetMarketPlugins(false);

		GetLocalPlugins(false, bPrint);
	}

	bool GetBackupPlugins(bool bPrint = true)
	{
		return GetLocalPlugins(true, bPrint);
	}

	bool GetMarketPlugins(bool bPrint = true)
	{
		bool bResult = false;

		if ( bPrint )
			Message(L"Initialize marketplace plugins ...", L"", true, true);

		if (m_sMarketplaceUrl.length())
		{
			m_arrMarketplace.clear();

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

							CPluginInfo* pPluginInfo = FindMarketPlugin(sPluginName);
							if ( !pPluginInfo )
							{
								pPluginInfo = FetchPluginInfo(sPluginName);
								if ( pPluginInfo )
								{
									m_arrMarketplace.push_back(pPluginInfo);

									if ( bPrint )
										MessagePluginInfo(sPluginName, pPluginInfo->m_pVersion->m_sVersion, pPluginInfo->m_sGuid);
								}
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
	CPluginInfo* FindLocalPlugin(const std::wstring& sPlugin, bool bInstalled = true)
	{
		CPluginInfo* pResult = NULL;

		if ( sPlugin.length() )
		{
			std::vector<CPluginInfo*>& arrPlugins = bInstalled ? m_arrInstalled : m_arrBackup;

			if ( arrPlugins.size() )
			{
				// Find by name or GUID
				std::wstring sPluginName = sPlugin;
				std::transform(sPluginName.begin(), sPluginName.end(), sPluginName.begin(), tolower);

				std::vector<CPluginInfo*>::iterator it;
				for (it = arrPlugins.begin(); it != arrPlugins.end(); it++)
				{
					if (IsGuid(sPlugin))
					{
						if ( (*it)->m_sGuid == sPlugin )
						{
							pResult = *it;
							break;
						}
					}
					else if ( (*it)->m_sName == sPluginName || (*it)->m_sNameConfig == sPluginName )
					{
						pResult = *it;
						break;
					}
				}
			}
		}

		return pResult;
	}

	CPluginInfo* FindMarketPlugin(const std::wstring& sPlugin)
	{
		CPluginInfo* pResult = NULL;

		if (m_arrMarketplace.size())
		{
			std::wstring _sPlugin = sPlugin;
			if ( !IsGuid(_sPlugin) )
				std::transform(_sPlugin.begin(), _sPlugin.end(), _sPlugin.begin(), tolower);

			std::vector<CPluginInfo*>::iterator it;
			for (it = m_arrMarketplace.begin(); it != m_arrMarketplace.end(); it++)
			{
				if ( (*it)->m_sName == _sPlugin ||
					 (*it)->m_sNameConfig == _sPlugin ||
					 (*it)->m_sGuid == _sPlugin )
				{
					pResult = *it;
					break;
				}
			}
		}

		return pResult;
	}

	bool InstallPlugin(const std::wstring& sPlugin, bool bPrint = true)
	{
		bool bResult = false;

		if (sPlugin.length())
		{
			bool isNeedDownload = true;
			std::wstring sPackageUrl = L"";

			std::wstring sTmpFile = NSFile::CFileBinary::GetTempPath() + L"/temp_asc_plugin.plugin";
			if (NSFile::CFileBinary::Exists(sTmpFile))
				NSFile::CFileBinary::Remove(sTmpFile);

			std::wstring sTempDir = m_sPluginsDir + L"/temp_asc_plugin_directory";
			std::wstring sTempDirExt = sTempDir;

			// Search by name or GUID
			CPluginInfo* pResult = FindMarketPlugin(sPlugin);

			if (pResult)
			{
				sPackageUrl = m_sMarketplaceUrl + L"/sdkjs-plugins/content/" + pResult->m_sName + L"/deploy/" + pResult->m_sName + L".plugin";
			}
			else if (IsNeedDownload(sPlugin))
			{
				// Try downlaod as is and unzip
				sPackageUrl = sPlugin;
			}
			else if (NSFile::CFileBinary::Exists(sPlugin))
			{
				sTmpFile = sPlugin;
				isNeedDownload = false;
			}

			if (isNeedDownload)
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

				CPluginInfo* pPluginInfo = ReadPluginInfo(sConfigFile);

				if (pPluginInfo)
				{
					std::wstring sPluginDir = m_sPluginsDir + L"/" + pPluginInfo->m_sGuid;

					if (NSDirectory::Exists(sPluginDir))
						NSDirectory::DeleteDirectory(sPluginDir);
					NSDirectory::CreateDirectory(sPluginDir);

					NSDirectory::CopyDirectory(sTempDirExt, sPluginDir);

					bResult = true;
				}

				NSDirectory::DeleteDirectory(sTempDir);
			}

			if ( isNeedDownload )
				NSFile::CFileBinary::Remove(sTmpFile);
		}

		if (bPrint)
			Message(L"Install plugin: " + sPlugin, BoolToStr(bResult), true);

		return bResult;
	}

	bool UpdatePlugin(const std::wstring& sPlugin)
	{
		bool bResult = true;
		std::wstring sVerToVer = L"";

		if ( sPlugin.length() )
		{
			CPluginInfo* pLocalPlugin = FindLocalPlugin(sPlugin);
			CPluginInfo* pMarketPlugin = FindMarketPlugin(sPlugin);

			// Check new version
			if ( pLocalPlugin && pMarketPlugin )
			{
				if ( *pMarketPlugin->m_pVersion > *pLocalPlugin->m_pVersion )
				{
					sVerToVer = L"(" + pLocalPlugin->m_pVersion->m_sVersion + L" -> " + pMarketPlugin->m_pVersion->m_sVersion + L")";

					bResult &= RemovePlugin(pLocalPlugin->m_sGuid, false);
					bResult &= InstallPlugin(pLocalPlugin->m_sGuid, false);

					Message(L"Update plugin: " + sPlugin + L" " + sVerToVer, BoolToStr(bResult), true);
				}
			}
		}

		return bResult;
	}

	bool RestorePlugin(const std::wstring& sPlugin)
	{
		bool bResult = false;

		if (sPlugin.length())
		{
			CPluginInfo* pResult = FindLocalPlugin(sPlugin, false);

			if ( pResult )
			{
				std::wstring sPluginDir = m_sPluginsDir + L"/" + pResult->m_sGuid;
				std::wstring sPluginBackupDir = m_sPluginsDir + L"/backup/" + pResult->m_sGuid;

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

	bool RemovePlugin(const std::wstring& sPlugin, bool bPrint = true)
	{
		bool bResult = false;

		if (sPlugin.length())
		{
			bool bBackup = false;
			CPluginInfo* pResult = FindLocalPlugin(sPlugin);

			// Try find in marketplace if name isn't short alias
			if ( !pResult )
				pResult = FindMarketPlugin(sPlugin);
			else
			{
				// Need create backup for plugin if doesn't exist in the marketplace
				CPluginInfo* pCheck = FindMarketPlugin(sPlugin);
				if ( !pCheck )
					bBackup = true;
			}

			if (pResult)
			{
				std::wstring sPluginDir = m_sPluginsDir + L"/" + pResult->m_sGuid;

				if (NSDirectory::Exists(sPluginDir))
				{
					if (bBackup)
					{
						std::wstring sBackupDir = m_sPluginsDir + L"/backup";
						std::wstring sPluginBackupDir = sBackupDir + L"/" + pResult->m_sGuid;

						if (!NSDirectory::Exists(sBackupDir))
							NSDirectory::CreateDirectory(sBackupDir);

						if (!NSDirectory::Exists(sPluginBackupDir))
							NSDirectory::DeleteDirectory(sPluginBackupDir);

						NSDirectory::CopyDirectory(sPluginDir, sPluginBackupDir);

						if (bPrint)
							Message(L"Backup plugin: " + sPlugin, BoolToStr(bBackup), true);
					}

					NSDirectory::DeleteDirectory(sPluginDir);
					bResult = true;
				}
			}
		}

		if (bPrint)
			Message(L"Remove plugin: " + sPlugin, BoolToStr(bResult), true);

		return bResult;
	}

	// Methods
	void InitPlugins(bool bBackup = false)
	{
		if ( !m_arrBackup.size() )
			GetBackupPlugins(false);

		// Backup plugins don't exist in the marketplace, without additional initialization
		if ( !bBackup )
		{
			if ( !m_arrInstall.size() )
				GetInstalledPlugins(false);

			if ( !m_arrMarketplace.size() )
				GetMarketPlugins(false);
		}
	}

	bool GetLocalPlugins(bool bBackup = false, bool bPrint = true)
	{
		bool bResult = false;

		if ( bPrint )
			Message(bBackup ? L"Backup plugins:" : L"Installed plugins:", L"", true, true);

		if (m_sPluginsDir.length())
		{
			std::vector<CPluginInfo*> arrPlugins;

			std::vector<std::wstring> arrDirs = NSDirectory::GetDirectories(m_sPluginsDir + (bBackup ? L"/backup" : L""));

			for (size_t i = 0; i < arrDirs.size(); ++i)
			{
				std::wstring sFile = arrDirs[i] + L"/config.json";

				if (NSFile::CFileBinary::Exists(sFile))
				{
					CPluginInfo* pPluginInfo = ReadPluginInfo(sFile);

					if ( pPluginInfo && pPluginInfo->m_isValid && !IsPluginManager(pPluginInfo->m_sGuid) )
					{
						if (std::find(arrPlugins.begin(), arrPlugins.end(), pPluginInfo) == arrPlugins.end())
						{
							// Sync short names with marketplace
							// for example, we can't find 'openai' in installed
							CPluginInfo* pMarketPlugin = FindMarketPlugin(pPluginInfo->m_sGuid);
							if ( pMarketPlugin &&  (pPluginInfo->m_sName != pMarketPlugin->m_sName) )
								pPluginInfo->m_sName = pMarketPlugin->m_sName;

							// Save
							arrPlugins.push_back(pPluginInfo);
							bResult = true;

							if ( bPrint )
							{
								// Check new version from marketplace
								std::wstring sVersion = pPluginInfo->m_pVersion->m_sVersion;

								if ( !bBackup && pMarketPlugin)
								{
									if ( *pMarketPlugin->m_pVersion > *pPluginInfo->m_pVersion )
										sVersion += L" (new " + pMarketPlugin->m_pVersion->m_sVersion + L")";
								}

								MessagePluginInfo(pPluginInfo->m_sNameConfig, sVersion, pPluginInfo->m_sGuid);
							}
						}
					}
				}
			}

			// Save to target array
			if ( bBackup )
				m_arrBackup = arrPlugins;
			else
				m_arrInstalled = arrPlugins;
		}
		else
		{
			Message(L"Set plugin folder. Use the following parameter: " + sCmdPluginsDir, L"", true);
		}

		return bResult;
	}

	bool IsGuid(const std::wstring& sStr)
	{
		return sStr.length() && sStr.at(0) == L'{' && sStr.at(sStr.length() - 1) == L'}';
	}

	bool IsNeedDownload(const std::wstring& FilePath)
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

	bool IsPluginManager(const std::wstring& sGuid)
	{
		return sGuid == m_sManagerGuid || sGuid == m_sOldManagerGuid;
	}

	bool SplitStringAsVector(const std::wstring& sData, const std::wstring& sDelimiter, std::vector<std::wstring>& arrOutput)
	{
		arrOutput.clear();

		if ( sData.length() )
		{
			std::wstring sTmp = sData;
			NSStringUtils::string_replace(sTmp, L", ", L",");

			size_t pos_start = 0, pos_end, delim_len = sDelimiter.length();
			std::wstring token = L"";

			while ((pos_end = sTmp.find(sDelimiter, pos_start)) != std::string::npos)
			{
				token = sTmp.substr(pos_start, pos_end - pos_start);
				pos_start = pos_end + delim_len;
				if (token.length())
					arrOutput.push_back(token);
			}

			token = sTmp.substr(pos_start);
			if ( token.length() )
				arrOutput.push_back(token);
		}

		return arrOutput.size() > 0;
	}

	bool DownloadFile(const std::wstring& sUrl, const std::wstring& sFile)
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

	CPluginInfo* FetchPluginInfo(const std::wstring& sPluginName)
	{
		CPluginInfo* pResult = NULL;

		std::wstring sTmpFile = NSFile::CFileBinary::GetTempPath() + L"/temp_asc_plugin.json";
		std::wstring sConfigUrl = m_sMarketplaceUrl + L"/sdkjs-plugins/content/" + sPluginName + L"/config.json";

		if (NSFile::CFileBinary::Exists(sTmpFile))
			NSFile::CFileBinary::Remove(sTmpFile);

		if (DownloadFile(sConfigUrl, sTmpFile))
		{
			pResult = ReadPluginInfo(sTmpFile);

			// Names can be different, for example: ChatGPT and openai
			pResult->m_sName = sPluginName;

			NSFile::CFileBinary::Remove(sTmpFile);
		}

		return pResult;
	}

	CPluginInfo* ReadPluginInfo(const std::wstring& sConfigFile)
	{
		CPluginInfo* pResult = NULL;
		std::wstring sJson = L"";

		if (NSFile::CFileBinary::ReadAllTextUtf8(sConfigFile, sJson))
		{
			pResult = new CPluginInfo();

			// GUID
			std::wstring::size_type pos1 = sJson.find(L"asc.{");
			std::wstring::size_type pos2 = sJson.find(L"}", pos1);

			if (pos1 != std::wstring::npos && pos2 != std::wstring::npos && pos2 > pos1)
			{
				pResult->m_sGuid = sJson.substr(pos1 + 4, pos2 - pos1 - 3);
			}

			// Name
			std::wstring sDelim = L"\"name\"";
			pos1 = sJson.find(sDelim);
			pos2 = sJson.find(L"\"", pos1 + sDelim.length());
			std::wstring::size_type pos3 = sJson.find(L"\"", pos2 + 1);

			if (pos1 != std::wstring::npos && pos2 != std::wstring::npos && pos3 != std::wstring::npos && pos3 > pos2)
			{
				std::wstring sName = sJson.substr(pos2 + 1, pos3 - pos2 - 1);
				std::transform(sName.begin(), sName.end(), sName.begin(), tolower);

				pResult->m_sName = sName;
				pResult->m_sNameConfig = sName;
			}

			// Version
			sDelim = L"\"version\"";
			pos1 = sJson.find(sDelim);
			pos2 = sJson.find(L"\"", pos1 + sDelim.length());
			pos3 = sJson.find(L"\"", pos2 + 1);

			if (pos1 != std::wstring::npos && pos2 != std::wstring::npos && pos3 != std::wstring::npos && pos3 > pos2)
			{
				std::wstring sVersion = sJson.substr(pos2 + 1, pos3 - pos2 - 1);
				pResult->m_pVersion = new CVersion(sVersion);
			}

			pResult->m_isValid = pResult->m_sGuid.length() &&
					pResult->m_sName.length() &&
					pResult->m_sNameConfig.length();
		}

		return pResult;
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
			std::wcout << L"------------------------------------------------------------------------------------" << std::endl;
	}

	void MessagePluginInfo(std::wstring sName, std::wstring sVersion, std::wstring sGuid)
	{
		std::wcout << std::left << std::setw(20) << sName.c_str()
				   << std::setw(26) << sVersion.c_str()
				   << std::setw(40) << sGuid.c_str() << std::endl;
	}
};

// Main
#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	setlocale(LC_ALL, "");

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
				oManager.PrintHelp();
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
				oManager.GetMarketPlugins();
			}
			else if (sKey == sCmdPrintBackup)
			{
				oManager.GetBackupPlugins();
			}

			// Install / Update / Remove
			else if (sKey == sCmdInstall)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
				{
					oManager.SetInstallPlugins(sValue);
					oManager.InstallPlugins();
				}
			}
			else if (sKey == sCmdRestore)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
				{
					oManager.SetRestorePlugins(sValue);
					oManager.RestorePlugins();
				}
			}
			else if (sKey == sCmdUpdate)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
				{
					oManager.SetUpdatePlugins(sValue);
					oManager.UpdatePlugins();
				}
			}
			else if (sKey == sCmdUpdateAll)
			{
				oManager.UpdateAllPlugins();
			}
			else if (sKey == sCmdRemove)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
				{
					oManager.SetRemovePlugins(sValue);
					oManager.RemovePlugins();
				}
			}
			else if (sKey == sCmdRemoveAll)
			{
				oManager.RemoveAllPlugins();
			}
		}
	}

	return 0;
}

