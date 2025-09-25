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
#include "../../DesktopEditor/common/SystemUtils.h"
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

// Manager
enum PluginStatus
{
	Removed = 0,
	Installed,
	Backup
};

class CVersion
{
private:
	int m_major, m_minor, m_revision, m_build;

public:
	std::wstring m_sVersion;

	CVersion() : m_major(1), m_minor(0), m_revision(0), m_build(0), m_sVersion(L"1.0.0")
	{
	}

	CVersion(std::wstring& sVersion) : m_major(1), m_minor(0), m_revision(0), m_build(0), m_sVersion(L"1.0.0")
	{
		if ( sVersion.length() )
		{
			m_sVersion = sVersion;

			std::vector<std::wstring> arr;
			SplitStringAsVector(m_sVersion, L".", arr);

			for (size_t i = 0; i < arr.size(); i++)
			{
				int iValue = std::stoi(arr[i]);
				switch (i)
				{
				case 0: m_major =		iValue; break;
				case 1: m_minor =		iValue; break;
				case 2: m_revision =	iValue; break;
				case 3: m_build =		iValue; break;
				default: break;
				}
			}

			if (m_major < 0)		m_major = 0;
			if (m_minor < 0)		m_minor = 0;
			if (m_revision < 0)		m_revision = 0;
			if (m_build < 0)		m_build = 0;
		}
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
		return m_major == oVersion.m_major &&
			   m_minor == oVersion.m_minor &&
			   m_revision == oVersion.m_revision &&
			   m_build == oVersion.m_build &&
			   m_sVersion == oVersion.m_sVersion;
	}
};

class CPluginInfo
{
public:
	std::wstring m_sName;
	std::wstring m_sNameConfig;
	std::wstring m_sGuid;
	std::wstring m_sDir;

	CVersion* m_pVersion;
	bool m_isValid;
	bool m_isDirGuid;

	CPluginInfo()
	{
		m_sName = L"";
		m_sNameConfig = L"";
		m_sGuid = L"";
		m_sDir = L"";
		m_pVersion = new CVersion();
		m_isValid = true;
		m_isDirGuid  = true;
	}

	CPluginInfo(std::wstring& sName, std::wstring& sNameConfig, std::wstring& sGuid, CVersion* pVersion)
	{
		m_sName = sName;
		m_sNameConfig = sNameConfig;
		m_sGuid = sGuid;
		m_pVersion = pVersion;
		m_isValid = true;
	}

	bool operator == (const CPluginInfo& oPlugin)
	{
		return m_sName == oPlugin.m_sName &&
			   m_sNameConfig == oPlugin.m_sNameConfig &&
			   m_sGuid == oPlugin.m_sGuid &&
			   m_pVersion == oPlugin.m_pVersion;
	}
};

class CPluginsManager
{
private:
	std::wstring m_sManagerGuid =		L"{AA2EA9B6-9EC2-415F-9762-634EE8D9A95E}";
	std::wstring m_sOldManagerGuid =	L"{8D67F3C5-7736-4BAE-A0F2-8C7127DC4BB8}";

	std::wstring m_sPluginsDir;
	std::wstring m_sMarketplaceUrl;
	std::wstring m_sMarketplaceRepo;

	std::wstring m_sSettingsDir;
	std::wstring m_sSettingsFile;

	std::vector<std::wstring> m_arrInstall, m_arrRestore, m_arrUpdate, m_arrRemove, m_arrIgnore;
	std::vector<CPluginInfo*> m_arrInstalled, m_arrRemoved, m_arrMarketplace, m_arrBackup;

public:
	CPluginsManager()
	{
		m_sPluginsDir = L"";
		m_sMarketplaceUrl = L"https://onlyoffice.github.io";
		m_sMarketplaceRepo = L"https://github.com/ONLYOFFICE/onlyoffice.github.io";

		m_sSettingsDir = NSSystemUtils::GetAppDataDir() + L"/pluginsmanager";

#ifdef LINUX
		// GetAppDataDir creates folder with ONLYOFFICE on Linux
		// as result - two folders in lower/upper case, working with the correct folder
		NSStringUtils::string_replace(m_sSettingsDir, L"ONLYOFFICE", L"onlyoffice");
#endif

		m_sSettingsFile = m_sSettingsDir + L"/settings";
	}

	// Usability
	void PrintHelp()
	{
		Message(sHelpText);
	}

	// Set
	bool SetDirectory(const std::wstring& sDir)
	{
		bool bResult = false;
		std::wstring sInfo = L"";

		if ( sDir.length() )
		{
			m_sPluginsDir = sDir;

			// Check access
			if ( NSDirectory::Exists(m_sPluginsDir) )
			{
				std::wstring sTestDir = m_sPluginsDir + L"/pm-check-access-denied";
				if ( NSDirectory::CreateDirectory(sTestDir) )
				{
					NSDirectory::DeleteDirectory(sTestDir);
					bResult = true;
				}
				else
					sInfo = L"Folder: " + m_sPluginsDir + L"\n" + L"access is denied. Check permissions. Exit";
			}
			else
				sInfo = L"Folder: " + m_sPluginsDir + L"\n" + L"does not exist. Exit";
		}

		if ( sInfo.length() )
			Message(sInfo, L"", true);

		return bResult;
	}

	bool SetMarketplace(const std::wstring& sUrl)
	{
		bool bResult = false;

		if ( sUrl.length() )
		{
			if (0 == sUrl.find(L"https://onlyoffice.github.io"))
			{
				m_sMarketplaceUrl = L"https://onlyoffice.github.io";
				m_sMarketplaceRepo = L"https://github.com/ONLYOFFICE/onlyoffice.github.io";
			}
			else if (0 == sUrl.find(L"https://onlyoffice-plugins.github.io/onlyoffice.github.io"))
			{
				m_sMarketplaceUrl = L"https://onlyoffice-plugins.github.io/onlyoffice.github.io";
				m_sMarketplaceRepo = L"https://github.com/ONLYOFFICE-PLUGINS/onlyoffice.github.io";
			}
			else
			{
				m_sMarketplaceUrl = sUrl;
				m_sMarketplaceRepo = L"";
				std::wstring::size_type posDelimeter = m_sMarketplaceUrl.find(';');
				if (posDelimeter != std::wstring::npos)
				{
					m_sMarketplaceUrl = sUrl.substr(0, posDelimeter);
					m_sMarketplaceRepo = sUrl.substr(posDelimeter + 1);
				}
			}

			bResult = true;
		}

		return bResult;
	}

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

	bool SetIgnorePlugins(const std::wstring& sPluginsList)
	{
		return SplitStringAsVector(sPluginsList, L",", m_arrIgnore);
	}

	// Settings
	void ResetSettings()
	{
		Message(L"Reset settings", BoolToStr(true), false);

		if (NSFile::CFileBinary::Exists(m_sSettingsFile))
			NSFile::CFileBinary::Remove(m_sSettingsFile);

		m_arrRemoved.clear();
	}

	bool ReadSettings()
	{
		// Format
		// name|version|guid, ...

		m_arrRemoved.clear();

		if (NSFile::CFileBinary::Exists(m_sSettingsFile))
		{
			std::wstring sData = L"";
			if (NSFile::CFileBinary::ReadAllTextUtf8(m_sSettingsFile, sData))
			{
				std::wstring::size_type pos1 = sData.find(sSetRemoved);
				std::wstring::size_type pos2 = sData.find(L"\n", pos1);

				if (pos1 != std::wstring::npos && pos2 != std::wstring::npos && pos2 > pos1)
				{
					std::wstring sRemovedGuids = sData.substr(pos1 + sSetRemoved.length(), pos2 - pos1 - sSetRemoved.length());
					NSStringUtils::string_replace(sRemovedGuids, L"\r", L"");
					sRemovedGuids = CorrectValue(sRemovedGuids);

					std::vector<std::wstring> arrTmp;
					if ( SplitStringAsVector(sRemovedGuids, L",", arrTmp) )
					{
						for (size_t i = 0; i < arrTmp.size(); i++)
						{
							std::vector<std::wstring> arrItems;
							if ( SplitStringAsVector(arrTmp[i], L"|", arrItems) && (arrItems.size() == 3) )
							{
								CPluginInfo* pPluginInfo = new CPluginInfo(arrItems[0], arrItems[0], arrItems[2], new CVersion(arrItems[1]));
								m_arrRemoved.push_back(pPluginInfo);
							}
						}
					}
				}
			}
		}

		return m_arrRemoved.size() > 0;
	}

	bool SaveSettings()
	{
		bool bResult = false;

		if ( m_arrRemoved.size() )
		{
			std::wstring sData = sSetRemoved;
			std::wstring sDelim = L"|";

			for (size_t i = 0; i < m_arrRemoved.size(); i++)
			{
				sData += m_arrRemoved[i]->m_sName + sDelim +
						 m_arrRemoved[i]->m_pVersion->m_sVersion + sDelim +
						 m_arrRemoved[i]->m_sGuid +
						 ((i < m_arrRemoved.size() - 1) ? L"," : L"");
			}
			sData += L"\n";

			if ( !NSDirectory::Exists(m_sSettingsDir) )
				NSDirectory::CreateDirectories(m_sSettingsDir);

			if ( NSFile::CFileBinary::Exists(m_sSettingsFile) )
				NSFile::CFileBinary::Remove(m_sSettingsFile);

			NSFile::CFileBinary oFile;
			bResult = oFile.CreateFileW(m_sSettingsFile);
			oFile.WriteStringUTF8(sData);
			oFile.CloseFile();
		}

		return bResult;
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

		if ( bResult )
			GetInstalledPlugins();

		return bResult;
	}

	bool UpdatePlugins()
	{
		bool bResult = true;
		Message(L"Update plugins ...", L"", true, true);

		InitPlugins();

		if (m_sPluginsDir.length() && m_arrMarketplace.size())
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

	bool RemovePlugins(bool bAll = false)
	{
		bool bResult = true;
		Message(bAll ? L"Remove all installed plugins ..." : L"Remove plugins ...", L"", true, true);

		InitPlugins();

		if ( bAll )
		{
			m_arrRemove.clear();
			for (size_t i = 0; i < m_arrInstalled.size(); i++)
			{
				m_arrRemove.push_back(m_arrInstalled[i]->m_sName);
			}
		}

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

	void RenamePlugins()
	{
		bool bResult = false;
		Message(L"Rename plugins ...", L"", true, true);

		InitPlugins();

		for (size_t i = 0; i < m_arrInstalled.size(); i++)
		{
			CPluginInfo* pPluginInfo = m_arrInstalled[i];

			if ( !pPluginInfo->m_isDirGuid )
			{
				std::wstring sPluginDir = m_sPluginsDir + L"/" + pPluginInfo->m_sGuid;

				if ( NSDirectory::Exists(sPluginDir) )
					NSDirectory::DeleteDirectory(sPluginDir);

				bResult = NSDirectory::CreateDirectory(sPluginDir);
				bResult &= NSDirectory::CopyDirectory(pPluginInfo->m_sDir, sPluginDir);

				if ( bResult )
					NSDirectory::DeleteDirectory(pPluginInfo->m_sDir);

				std::wstring sPrintInfo = L"Rename plugin: " + pPluginInfo->m_sName + L" -> " + pPluginInfo->m_sGuid;
				Message(sPrintInfo, BoolToStr(bResult), true);
			}
		}

		if ( bResult )
			GetInstalledPlugins();
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

		if ( !m_arrMarketplace.size() && m_sMarketplaceUrl.length() )
		{
			m_arrMarketplace.clear();

			std::wstring sConfigUrl = m_sMarketplaceUrl + L"/store/config.json";
			std::wstring sTmpFile = NSFile::CFileBinary::GetTempPath() + L"/temp_asc_plugins_config.json";

			if (DownloadFile(sConfigUrl, sTmpFile))
			{
				std::vector<std::wstring> arr;
				if ( ReadStoreConfig(sTmpFile, arr) )
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
									MessagePluginInfo(pPluginInfo->m_sNameConfig, pPluginInfo->m_pVersion->m_sVersion, pPluginInfo->m_sGuid);
							}
						}
					}
				}
				NSFile::CFileBinary::Remove(sTmpFile);
			}
		}

		return bResult;
	}

	void PrintRemovedPlugins()
	{
		Message(L"Removed plugins:", L"", true, true);

		for (size_t i = 0; i < m_arrRemoved.size(); i++)
		{
			MessagePluginInfo(m_arrRemoved[i]->m_sName, m_arrRemoved[i]->m_pVersion->m_sVersion, m_arrRemoved[i]->m_sGuid);
		}
	}

private:
	// Single
	CPluginInfo* FindLocalPlugin(const std::wstring& sPlugin, PluginStatus status = Installed)
	{
		CPluginInfo* pResult = NULL;

		if ( sPlugin.length() )
		{
			std::vector<CPluginInfo*> arrPlugins;
			switch (status)
			{
			case Removed:
				arrPlugins = m_arrRemoved;
				break;
			case Backup:
				arrPlugins = m_arrBackup;
				break;
			default:
				arrPlugins = m_arrInstalled;
				break;
			}

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

	bool InstallPlugin(const std::wstring& sPlugin, bool bDirGuid = true, bool bPrint = true)
	{
		bool bResult = false;
		std::wstring sPrintInfo = L"";

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
			CPluginInfo* pPlugin = FindMarketPlugin(sPlugin);

			if (pPlugin)
			{
				sPackageUrl = m_sMarketplaceUrl + L"/sdkjs-plugins/content/" + pPlugin->m_sName + L"/deploy/" + pPlugin->m_sName + L".plugin";
				if (!m_sMarketplaceRepo.empty())
					sPackageUrl = m_sMarketplaceRepo + L"/releases/latest/download/" + pPlugin->m_sName + L".plugin";
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
			else
			{
				sPrintInfo = L"The plugin not found.\n" \
					L"Check plugin name, GUID using --print-marketplace option or check file path";
			}
			//

			if (isNeedDownload)
			{
				if ( sPackageUrl.length() && !DownloadFile(sPackageUrl, sTmpFile) )
					sPrintInfo = L"Can't download: " + sPackageUrl;
			}

			if (NSFile::CFileBinary::Exists(sTmpFile))
			{
				if (NSDirectory::Exists(sTempDir))
					NSDirectory::DeleteDirectory(sTempDir);
				NSDirectory::CreateDirectory(sTempDir);

				// Archive
				COfficeUtils oOfficeUtils(NULL);
				if (S_OK == oOfficeUtils.IsArchive(sTmpFile) &&
					S_OK == oOfficeUtils.ExtractToDirectory(sTmpFile, sTempDirExt, NULL, 0))
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
					if ( pPluginInfo )
					{
						CPluginInfo* pInstalled = FindLocalPlugin(pPluginInfo->m_sGuid);

						if ( pInstalled )
						{
							sPrintInfo = L"Already installed";
							bResult = true;
						}
						else if ( pPluginInfo )
						{
							std::wstring sPluginDir = m_sPluginsDir + L"/" + (bDirGuid ? pPluginInfo->m_sGuid : pPluginInfo->m_sName);

							// Check settings
							// Can install if user hasn't deleted the plugin before
							CPluginInfo* pRemoved = FindLocalPlugin(pPluginInfo->m_sGuid, Removed);
							if ( !pRemoved )
							{
								if ( NSDirectory::Exists(sPluginDir) )
									NSDirectory::DeleteDirectory(sPluginDir);
								NSDirectory::CreateDirectory(sPluginDir);

								NSDirectory::CopyDirectory(sTempDirExt, sPluginDir);

								bResult = true;
							}
							else
							{
								sPrintInfo = L"Installation cancelled. The plugin has been removed before.\n" \
									L"Use --reset option to reset settings";
							}
						}
					}
				}
				// Config
				else
				{
					std::vector<std::wstring> arrPlugins;
					if ( ReadConfigJson(sTmpFile, arrPlugins) )
					{
						// Recursion installation
						bool _bResult = true;
						for(size_t i = 0; i < arrPlugins.size(); i++)
						{
							_bResult &= InstallPlugin(arrPlugins[i], bDirGuid, bPrint);
						}
						bResult = _bResult;
					}
				}

				NSDirectory::DeleteDirectory(sTempDir);
			}

			if ( isNeedDownload )
				NSFile::CFileBinary::Remove(sTmpFile);
		}

		if (bPrint)
			Message(L"Install plugin: " + sPlugin, sPrintInfo.length() ? sPrintInfo : BoolToStr(bResult), true);

		return bResult;
	}

	bool UpdatePlugin(const std::wstring& sPlugin)
	{
		bool bResult = true;
		std::wstring sVerToVer = L"";

		if ( sPlugin.length() )
		{
			// Check config file
			if ( NSFile::CFileBinary::Exists(sPlugin) )
			{
				std::vector<std::wstring> arrPlugins;
				if ( ReadConfigJson(sPlugin, arrPlugins) )
				{
					// Update if plugin exists
					// Install if plugin is not installed and not removed before
					bool _bResult = true;
					for(size_t i = 0; i < arrPlugins.size(); i++)
					{
						CPluginInfo* pPlugin = FindLocalPlugin(arrPlugins[i]);
						if ( pPlugin )
							_bResult &= UpdatePlugin(arrPlugins[i]);
						else
						{
							_bResult &= InstallPlugin(arrPlugins[i]);
							// Update installed
							GetLocalPlugins(false, false);
						}
					}
					bResult = _bResult;
				}
			}
			else
			{
				CPluginInfo* pLocalPlugin = FindLocalPlugin(sPlugin);
				CPluginInfo* pMarketPlugin = FindMarketPlugin(sPlugin);

				if ( !pLocalPlugin )
				{
					bResult = false;
					Message(L"The plugin not found: " + sPlugin, BoolToStr(bResult), true);
				}

				// Check new version
				if ( pLocalPlugin && pMarketPlugin )
				{
					if ( *pMarketPlugin->m_pVersion > *pLocalPlugin->m_pVersion )
					{
						sVerToVer = L"(" + pLocalPlugin->m_pVersion->m_sVersion + L" -> " + pMarketPlugin->m_pVersion->m_sVersion + L")";

						bResult &= RemovePlugin(pLocalPlugin->m_sGuid, false, false);
						bResult &= InstallPlugin(pLocalPlugin->m_sGuid, pLocalPlugin->m_isDirGuid, false);

						Message(L"Update plugin: " + sPlugin + L" " + sVerToVer, BoolToStr(bResult), true);
					}
					else if ( *pMarketPlugin->m_pVersion == *pLocalPlugin->m_pVersion )
					{
						Message(L"Update plugin: " + sPlugin + L". No updates available", BoolToStr(bResult), true);
					}
				}
			}
		}

		return bResult;
	}

	bool RestorePlugin(const std::wstring& sPlugin)
	{
		bool bResult = false;
		std::wstring sPrintInfo = L"";

		if (sPlugin.length())
		{
			CPluginInfo* pPlugin = FindLocalPlugin(sPlugin, Backup);

			if ( pPlugin )
			{
				std::wstring sPluginDir = m_sPluginsDir + L"/" + pPlugin->m_sGuid;
				std::wstring sPluginBackupDir = m_sPluginsDir + L"/backup/" + pPlugin->m_sGuid;

				if (NSDirectory::Exists(sPluginBackupDir))
				{
					NSDirectory::CopyDirectory(sPluginBackupDir, sPluginDir);
					NSDirectory::DeleteDirectory(sPluginBackupDir);

					bResult = true;
				}
			}
			else
			{
				sPrintInfo = L"The plugin not found in the backup list, check using --print-backup\n" \
					L"This option is available for plugins that are not in the marketplace.\n" \
					L"Use --install command";
			}
		}

		Message(L"Restore plugin: " + sPlugin, sPrintInfo.length() ? sPrintInfo : BoolToStr(bResult), true);

		return bResult;
	}

	bool RemovePlugin(const std::wstring& sPlugin, bool bSave = true, bool bPrint = true)
	{
		bool bResult = false;

		if (sPlugin.length())
		{
			// Check config file
			if ( NSFile::CFileBinary::Exists(sPlugin) )
			{
				std::vector<std::wstring> arrPlugins;
				if ( ReadConfigJson(sPlugin, arrPlugins) )
				{
					// Recursion removing
					bool _bResult = true;
					for(size_t i = 0; i < arrPlugins.size(); i++)
					{
						_bResult &= RemovePlugin(arrPlugins[i], bSave, bPrint);
					}
					bResult = _bResult;
				}
			}

			bool bBackup = false;
			CPluginInfo* pPlugin = FindLocalPlugin(sPlugin);

			// Try find in marketplace if name isn't short alias
			if ( !pPlugin )
				pPlugin = FindMarketPlugin(sPlugin);
			else
			{
				// Need create backup for plugin if doesn't exist in the marketplace
				CPluginInfo* pCheck = FindMarketPlugin(sPlugin);
				if ( !pCheck )
					bBackup = true;
			}

			if (pPlugin)
			{
				// Plugin folder can be without GUID
				std::wstring sPluginDir = m_sPluginsDir + L"/" + pPlugin->m_sGuid;
				if ( !NSDirectory::Exists(sPluginDir) )
					sPluginDir = pPlugin->m_sDir;

				if (NSDirectory::Exists(sPluginDir))
				{
					if (bBackup)
					{
						std::wstring sBackupDir = m_sPluginsDir + L"/backup";

						std::wstring sPluginBackupDir = sBackupDir + L"/" + pPlugin->m_sGuid;

						if ( !NSDirectory::Exists(sBackupDir) )
							NSDirectory::CreateDirectory(sBackupDir);

						if ( NSDirectory::Exists(sPluginBackupDir) )
							NSDirectory::DeleteDirectory(sPluginBackupDir);

						NSDirectory::CopyDirectory(sPluginDir, sPluginBackupDir);

						if (bPrint)
							Message(L"Backup plugin: " + sPlugin, BoolToStr(bBackup), true);
					}

					NSDirectory::DeleteDirectory(sPluginDir);

					// Update installed
					GetLocalPlugins(false, false);

					// Check duplicate (bug #62807)
					CPluginInfo* pDuplicate = FindLocalPlugin(pPlugin->m_sGuid);
					if ( pDuplicate )
					{
						bool bRes = RemovePlugin(pDuplicate->m_sName, false, false);
						if (bPrint)
							Message(L"Remove duplicate plugin: " + pDuplicate->m_sName, BoolToStr(bRes), true);
					}

					// Save to settings
					CPluginInfo* pRemoved = FindLocalPlugin(pPlugin->m_sGuid, Removed);
					if ( bSave && !pRemoved )
						m_arrRemoved.push_back(pPlugin);

					bResult = true;
				}
			}
		}

		if (bPrint)
		{
			std::wstring sInfo = L"Remove plugin: " + sPlugin;
			Message(sInfo, BoolToStr(bResult), true);
		}

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
			GetInstalledPlugins(false);
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
						// Save plugin folder for updating by name
						pPluginInfo->m_sDir = arrDirs[i];
						pPluginInfo->m_isDirGuid = IsFolderGuid(arrDirs[i]);

						if (std::find(arrPlugins.begin(), arrPlugins.end(), pPluginInfo) == arrPlugins.end())
						{
							// Sync short names with marketplace
							// for example, we can't find 'openai' in installed
							CPluginInfo* pMarketPlugin = FindMarketPlugin(pPluginInfo->m_sGuid);
							if ( pMarketPlugin &&  (pPluginInfo->m_sName != pMarketPlugin->m_sName) )
								pPluginInfo->m_sName = pMarketPlugin->m_sName;

							// Check ignored
							bool bIgnored = false;
							if ( m_arrIgnore.size() )
							{
								for (size_t i = 0; i < m_arrIgnore.size(); i++)
								{
									std::wstring sName = m_arrIgnore[i];
									std::wstring sGuid = m_arrIgnore[i];

									std::transform(sName.begin(), sName.end(), sName.begin(), tolower);

									if ( pPluginInfo->m_sName == sName ||
										pPluginInfo->m_sNameConfig == sName ||
										pPluginInfo->m_sGuid == sGuid)
									{
										bIgnored = true;
									}
								}
							}

							// Save
							if ( !bIgnored )
							{
								arrPlugins.push_back(pPluginInfo);
								bResult = true;
							}

							if ( bPrint )
							{
								// Check new version from marketplace
								std::wstring sVersion = pPluginInfo->m_pVersion->m_sVersion;

								if ( !bBackup && pMarketPlugin)
								{
									if ( *pMarketPlugin->m_pVersion > *pPluginInfo->m_pVersion )
										sVersion += L" (new " + pMarketPlugin->m_pVersion->m_sVersion + L")";
								}

								MessagePluginInfo(pPluginInfo->m_sNameConfig, sVersion, pPluginInfo->m_sGuid, bIgnored);
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
		return (sStr.length() && sStr.at(0) == L'{') && (sStr.at(sStr.length() - 1) == L'}');
	}

	bool IsFolderGuid(const std::wstring& sStr)
	{
		bool bResult = false;

		if ( sStr.length() )
		{
			std::wstring sFolder = sStr;
			std::vector<std::wstring> arrParts;

			NSStringUtils::string_replace(sFolder, L"\\", L"/");

			if ( SplitStringAsVector(sFolder, L"/", arrParts) )
			{
				std::wstring sDir = arrParts[arrParts.size() - 1];
				bResult = IsGuid(sDir);
			}
		}

		return bResult;
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

	bool ReadConfigJson(const std::wstring& sFile, std::vector<std::wstring>& arrOutput)
	{
		// [ "...", "...", "..." ]
		std::wstring sJson = L"";

		try
		{
			if (NSFile::CFileBinary::Exists(sFile) && NSFile::CFileBinary::ReadAllTextUtf8(sFile, sJson))
			{
				std::wstring sDelim = L"\"";
				std::wstring::size_type pos1 = sJson.find(sDelim);

				while ( pos1 != std::wstring::npos )
				{
					std::wstring::size_type pos2 = sJson.find(sDelim, pos1 + 1);

					if (pos1 != std::wstring::npos && pos2 > pos1)
						arrOutput.push_back(sJson.substr(pos1 + 1, pos2 - pos1 - 1));

					pos1 = sJson.find(sDelim, pos2 + 1);
				}
			}
		}
		catch(...)
		{
			Message(L"Can't read file: " + sFile, L"", true);
		}

		return arrOutput.size() > 0;
	}

	bool ReadStoreConfig(const std::wstring& sFile, std::vector<std::wstring>& arrOutput)
	{
		// [ { "name": "", "discussion": "" }, ... ]
		std::wstring sJson = L"";
		if (NSFile::CFileBinary::Exists(sFile) && NSFile::CFileBinary::ReadAllTextUtf8(sFile, sJson))
		{
			std::wstring sDelim = L"\"name\": \"";
			std::wstring::size_type pos1 = sJson.find(sDelim);

			while ( pos1 != std::wstring::npos )
			{
				std::wstring::size_type pos2 = sJson.find(L"\"", pos1 + sDelim.length());

				if (pos1 != std::wstring::npos && pos2 > pos1)
					arrOutput.push_back(sJson.substr(pos1 + sDelim.length(), pos2 - pos1 - sDelim.length()));

				pos1 = sJson.find(sDelim, pos2 + 1);
			}
		}

		return arrOutput.size() > 0;
	}

	void StringReplaceExt(std::wstring& sText, const std::wstring& sFrom, const std::wstring& sTo)
	{
		if ( sText.length() )
		{
			std::wstring::size_type pos = sText.find(sFrom);
			while ( pos != std::wstring::npos )
			{
				NSStringUtils::string_replace(sText, sFrom, sTo);
				pos = sText.find(sFrom);
			}
		}
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
		return bResult ? L"OK" : L"CANCELED";
	}

	// Terminal
	void Message(const std::wstring& sText, std::wstring sResult = L"", bool bUseIndent = false, bool bSeparator = false)
	{
		if (bUseIndent)
			std::wcout << L"" << std::endl;

		std::wcout << sText.c_str() << std::endl;
		if ( sResult.length() )
			std::wcout << sResult.c_str() << std::endl;

		if ( bSeparator )
			std::wcout << L"------------------------------------------------------------------------------------" << std::endl;
	}

	void MessagePluginInfo(const std::wstring& sName, const std::wstring& sVersion, const std::wstring& sGuid, bool bIgnore = false)
	{
		std::wstring sName_ = sName;
		if ( bIgnore )
			sName_ = sName_ + L"*";

		std::wcout << std::left << std::setw(20) << sName_.c_str()
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
	oManager.ReadSettings();

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

			// Parse key - value
			std::wstring::size_type pos = sParam.find('=');
			if ( pos == std::wstring::npos )
			{
				sKey = sParam;

				if ( IsNeedSetValue(sKey))
				{
					if (i < argc - 1)
					{
						i++;
#ifdef WIN32
						sValue = std::wstring(argv[i]);
#else
						std::string sValueA(argv[i]);
						sValue = UTF8_TO_U(sValueA);
#endif
					}

					// Checks if value or next key exist
					if ( !sValue.length() || (sValue.find(L"--") == 0) )
					{
						std::wcout << L"\nError. Check input parameters\n";
						return 1;
					}
				}
			}
			else
			{
				sKey = sParam.substr(0, pos);
				sValue = sParam.substr( pos + 1 );
			}

			// Check key
			if ( !IsCommandExists(sKey) )
			{
				std::wcout << L"\nError. Unknown parameter " << sKey << L"\n" << "Print usage information --help\n";
				return 1;
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
				if ( !oManager.SetDirectory(sValue) )
					return 1;
			}
			else if (sKey == sCmdMarketplaceUrl)
			{
				sValue = CorrectValue(sValue);
				oManager.SetMarketplace(sValue);
			}
			else if (sKey == sCmdAutorename)
			{
				oManager.RenamePlugins();
			}

			// Print
			else if (sKey == sCmdPrintInstalled)
			{
				oManager.GetInstalledPlugins();
			}
			else if (sKey == sCmdPrintRemoved)
			{
				oManager.PrintRemovedPlugins();
			}
			else if (sKey == sCmdPrintMarketplace)
			{
				oManager.GetMarketPlugins();
			}
			else if (sKey == sCmdPrintBackup)
			{
				oManager.GetBackupPlugins();
			}

			// Reset / Install / Update / Remove
			else if (sKey == sCmdReset)
			{
				oManager.ResetSettings();
			}
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
				oManager.RemovePlugins(true);
			}
			else if (sKey == sCmdIgnore)
			{
				sValue = CorrectValue(sValue);
				if (sValue.length())
				{
					oManager.SetIgnorePlugins(sValue);
				}
			}
		}
	}

	oManager.SaveSettings();

	return 0;
}

