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
#include <fstream>
#include <sstream>
#include <array>
#include <algorithm>

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

		while ((pos_end = sTmp.find(sDelimiter, pos_start)) != std::wstring::npos)
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

//
enum SystemType
{
	Debian = 0,
	RedHat,
	Empty
};

class CVm
{
public:
	std::wstring	m_sName;
	std::wstring	m_sGuid;

	// from showvminfo command
	std::wstring	m_sGuestOS;

	// real OS
	std::wstring	m_sOperationSystem;

	SystemType		m_eType;

	CVm()
	{
		m_sName = L"";
		m_sGuid = L"";
		m_sGuestOS = L"";
		m_sOperationSystem = L"";
		m_eType = Empty;
	}

	CVm(const std::wstring& sName, const std::wstring& sGuid, const std::wstring& sGuestOS, const SystemType& eType)
	{
		m_sName = sName;
		m_sGuid = sGuid;
		m_sGuestOS = sGuestOS;
		m_sOperationSystem = L"";
		m_eType = eType;
	}

	bool IsDebian()
	{
		return m_eType == Debian;
	}

	bool IsRedHat()
	{
		return m_eType == RedHat;
	}

	std::wstring ToString()
	{
		std::wstringstream sInfo;

		if ( m_sName.length() && m_sGuid.length() && m_sGuestOS.length() )
			sInfo << m_sName << L"-" << m_sGuestOS << L"-" << m_sGuid;

		return sInfo.str();
	}
};

class CVirtualBox
{
private:
	std::wstring m_sVbmPath;

	std::wstring m_sVmUser;
	std::wstring m_sVmPassword;

	std::wstring m_sBranch;
	std::wstring m_sVersion;

	std::wstring m_sDebianUrl;
	std::wstring m_sCentosUrl;
	std::wstring m_sOpSuseUrl;

	std::wstring m_sEditorsPath;
	std::wstring m_sSuccessOutput;

	std::wstring m_sConfigName;
	std::wstring m_sReportName;
	std::wstring m_sStdoutFile;

	std::wstring m_sRunScript;
	std::wstring m_sSetupScript;

	std::vector<CVm*> m_arrVms;
	CVm* m_pVm;

	std::wstring m_sDebianStart;
	std::wstring m_sRedHatStart;

	std::wstring m_sDebianScript;
	std::wstring m_sRedHatScript;

	bool m_bVerboseLog;

public:
	CVirtualBox()
	{
		m_pVm =				NULL;

		m_sDebianStart =	L"";
		m_sRedHatStart =	L"";

		m_sDebianScript =	L"";
		m_sRedHatScript =	L"";

		m_sVmUser =			L"";
		m_sVmPassword =		L"";

		m_sRunScript =		L"run";
		m_sSetupScript =	L"setup";

		m_sConfigName =		L"config";
		m_sReportName =		L"report.txt";
		m_sStdoutFile =		L"stdout.txt";

		m_sEditorsPath =	L"/opt/onlyoffice/desktopeditors/DesktopEditors";
		m_sSuccessOutput =	L"[DesktopEditors]: start page loaded";

		m_sBranch =			L"";
		m_sVersion =		L"";

		m_sDebianUrl =		L"";
		m_sCentosUrl =		L"";
		m_sOpSuseUrl =		L"";

		m_bVerboseLog =		false;

#ifdef WIN32
		m_sVbmPath =		L"\"c:\\Program Files\\Oracle\\VirtualBox\\VBoxManage.exe\"";
#endif
#ifdef LINUX
		m_sVbmPath =		L"/usr/lib/virtualbox/VBoxManage";
#endif
	}

	// VboxManage
	bool InitVms()
	{
		m_arrVms.clear();
		std::wstring sOutput = ExecuteCommand(L"list vms");

		std::vector<std::wstring> arrLines;
		if ( SplitStringAsVector(sOutput, L"\n", arrLines) )
		{
			for (size_t i = 0; i < arrLines.size(); i++)
			{
				std::wstring sLine = arrLines[i];

				std::wstring::size_type pos1 = sLine.find(L"{");
				std::wstring::size_type pos2 = sLine.find(L"}", pos1);

				if (pos1 != std::wstring::npos && pos2 != std::wstring::npos && pos2 > pos1)
				{
					std::wstring sGuid = sLine.substr(pos1, pos2 - pos1 + 1);
					std::wstring sName = sLine.substr(0, pos1 - 1);
					sName = CorrectValue(sName);
					std::wstring sOs = GetVmOS(sGuid);

					SystemType eType = Empty;
					std::wstring sOsLower = sOs;
					std::transform(sOsLower.begin(), sOsLower.end(), sOsLower.begin(), tolower);

					if ( sOsLower.find(L"ubuntu") != std::wstring::npos ||
						 sOsLower.find(L"debian") != std::wstring::npos)
						 eType = Debian;
					else
					if ( sOsLower.find(L"red hat") != std::wstring::npos ||
						 sOsLower.find(L"fedora") != std::wstring::npos ||
						 sOsLower.find(L"opensuse") != std::wstring::npos)
						eType = RedHat;

					m_arrVms.push_back(new CVm(sName, sGuid, sOs, eType));
				}
			}
		}

		return m_arrVms.size() > 0;
	}

	void SetVm(CVm* pVm)
	{
		m_pVm = pVm;
	}

	std::vector<CVm*> GetStartVms()
	{
		std::vector<CVm*> arrResult;

		// Check Debian systems
		if ( m_sDebianStart.length() )
		{
			std::vector<CVm*> arrDebian = GetDebianVms();

			std::wstring sDebian = m_sDebianStart;
			std::vector<std::wstring> list;
			NSStringUtils::string_replace(sDebian, L", ", L",");

			if ( SplitStringAsVector(sDebian, L",", list) )
			{
				for (size_t i = 0; i < arrDebian.size(); i++)
				{
					if ( std::find(list.begin(), list.end(), arrDebian[i]->m_sName) != list.end() )
					{
						arrResult.push_back(arrDebian[i]);
					}
				}
			}
		}

		// Check RedHat systems
		if ( m_sRedHatStart.length() )
		{
			std::vector<CVm*> arrRedHat = GetRedHatVms();

			std::wstring sRedHat = m_sRedHatStart;
			std::vector<std::wstring> list;
			NSStringUtils::string_replace(sRedHat, L", ", L",");

			if ( SplitStringAsVector(sRedHat, L",", list) )
			{
				for (size_t i = 0; i < arrRedHat.size(); i++)
				{
					if ( std::find(list.begin(), list.end(), arrRedHat[i]->m_sName) != list.end() )
					{
						arrResult.push_back(arrRedHat[i]);
					}
				}
			}
		}

		return arrResult;
	}

	bool StartVm()
	{
		bool bResult = false;

		if ( m_pVm )
		{
			if (m_bVerboseLog)
			{
				WriteReport(L"Starting VM");
				WriteReport(L"---------------------------------------");
			}

			WriteReport(m_pVm->m_sName);

			if (m_bVerboseLog)
			{
				WriteReport(m_pVm->m_sGuestOS);
				WriteReport(m_pVm->m_sGuid);
			}

			std::wstring sCommand = L"startvm " + m_pVm->m_sGuid;
			std::wstring sOutput = ExecuteCommand(sCommand);
			bResult = sOutput.find(L"started") != std::wstring::npos;

			if (m_bVerboseLog)
				WriteReportResult(bResult);
		}

		return bResult;
	}

	bool ResetVm()
	{
		bool bResult = false;

		if ( m_pVm )
		{
			if (m_bVerboseLog)
				WriteReport(L"Restart VM");

			std::wstring sCommand = L"controlvm " + m_pVm->m_sGuid + L" reset";
			std::wstring sOutput = ExecuteCommand(sCommand);
			bResult = sOutput.find(L"") != std::wstring::npos;

			if (m_bVerboseLog)
				WriteReportResult(bResult);
		}

		return bResult;
	}

	bool StopVm(bool bSaveState = false)
	{
		bool bResult = false;

		if ( m_pVm )
		{
			if (m_bVerboseLog)
				WriteReport(L"Stop VM");

			std::wstring sCommand = L"controlvm " + m_pVm->m_sGuid + (bSaveState ? L" savestate" : L" poweroff");
			std::wstring sOutput = ExecuteCommand(sCommand);
			bResult = sOutput.find(L"") != std::wstring::npos;

			if (m_bVerboseLog)
				WriteReportResult(bResult);
		}

		return bResult;
	}

	void WaitLoadVm()
	{
		// Wait success or 10 min
		int iSleep = 5000;
		int iCount = 10 * 60 * 1000 / iSleep;

		if (m_bVerboseLog)
			WriteReport(L"Waiting loading");

		if ( m_pVm )
		{
			while ( (iCount > 0) && !IsVmLoggedIn() )
			{
				NSThreads::Sleep(iSleep);
				iCount--;
			}

			NSThreads::Sleep(30000);
		}

		if (m_bVerboseLog)
			WriteReportResult(iCount > 0);
	}

	bool WaitInstall()
	{
		// Wait success or 10 min
		int iSleep = 5000;
		int iCount = 10 * 60 * 1000 / iSleep;

		if (m_bVerboseLog)
			WriteReport(L"Waiting installation");

		if ( m_pVm )
		{
			std::vector<std::wstring> arrProcess;

			if ( m_pVm->IsDebian() )
			{
				arrProcess.push_back(L"apt");
				arrProcess.push_back(L"dpkg");
			}
			else if ( m_pVm->IsRedHat() )
			{
				arrProcess.push_back(L"rpm");
			}

			while ( iCount > 0 )
			{
				bool bResult = IsLocationExists(m_sEditorsPath);
				for (size_t i = 0; i < arrProcess.size(); i++)
				{
					bResult &= !IsProcessExists(arrProcess[i]);
				}
				if ( bResult )
					break;

				NSThreads::Sleep(iSleep);
				iCount--;
			}
		}

		// True - installation, False - timeout
		if (m_bVerboseLog)
			WriteReportResult(iCount > 0);

		return iCount > 0;
	}

	bool WaitStdout()
	{
		// Wait success or 1 min
		int iSleep = 5000;
		int iCount = 1 * 60 * 1000 / iSleep;

		bool bStdout = false;

		if (m_bVerboseLog)
			WriteReport(L"Waiting stdout");

		if ( m_pVm )
		{
			std::wstring sStdoutFile = GetWorkingDir() + L"/" + m_sStdoutFile;

			while ( (iCount > 0) && (!IsLocationExists(sStdoutFile)) )
			{
				NSThreads::Sleep(iSleep);
				iCount--;
			}

			// File exists
			if ( iCount > 0 )
			{
				std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
										L" run --exe /bin/cat" +
										L" --username " + m_sVmUser +
										L" --password " + m_sVmPassword +
										L" --wait-stdout -- cat/arg0 " + sStdoutFile;

				std::wstring sOutput = ExecuteCommand(sCommand);

				if ( sOutput.find(m_sSuccessOutput) != std::wstring::npos )
					bStdout = true;
			}
		}

		if (m_bVerboseLog)
			WriteReportResult(bStdout);

		// True - installation, False - timeout
		return bStdout && ( iCount > 0 );
	}

	bool SaveScreenshot()
	{
		bool bResult = false;

		if ( m_pVm )
		{
			if (m_bVerboseLog)
				WriteReport(L"Saving screenshot");

			std::wstring sFilePath = GetReportDir() + L"/" + m_pVm->m_sName + L".png";

			if ( NSFile::CFileBinary::Exists(sFilePath) )
				NSFile::CFileBinary::Remove(sFilePath);

			std::wstring sCommand = L"controlvm " + m_pVm->m_sGuid + L" screenshotpng " + sFilePath;
			std::wstring sOutput = ExecuteCommand(sCommand);
			bResult = NSFile::CFileBinary::Exists(sFilePath);

			if (m_bVerboseLog)
				WriteReportResult(bResult);
		}

		return bResult;
	}

	bool CreateWorkingDir()
	{
		bool bResult = false;

		if ( m_pVm && m_sVmUser.length() )
		{
			std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
									L" --username " + m_sVmUser +
									L" --password " + m_sVmPassword +
									L" rmdir --recursive " + GetWorkingDir();

			std::wstring sOutput = ExecuteCommand(sCommand);

			sCommand = L"guestcontrol " + m_pVm->m_sGuid +
					   L" --username " + m_sVmUser +
					   L" --password " + m_sVmPassword +
					   L" mkdir " + GetWorkingDir();

			sOutput = ExecuteCommand(sCommand);
			//NSThreads::Sleep(10000);

			bResult = true;
		}

		return bResult;
	}

	bool CheckOperationSystem()
	{
		bool bResult = false;

		if ( m_pVm )
		{
			// Get real description
			m_pVm->m_sOperationSystem = L"";
			std::wstring sBin = L"/usr/bin/hostnamectl";

			std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
									L" run --exe " + sBin +
									L" --username " + m_sVmUser +
									L" --password " + m_sVmPassword +
									L" --wait-stdout";

			std::wstring sOutput = ExecuteCommand(sCommand);

			std::vector<std::wstring> arrLines;
			if ( SplitStringAsVector(sOutput, L"\n", arrLines) )
			{
				std::wstring sPrefix = L"Operating System:";

				for (size_t i = 0; i < arrLines.size(); i++)
				{
					std::wstring sLine = arrLines[i];

					std::wstring::size_type pos = sLine.find(sPrefix);
					if ( pos != std::wstring::npos )
					{
						m_pVm->m_sOperationSystem = sLine;
						pos = m_pVm->m_sOperationSystem.find(sPrefix + L" ");
						while ( pos != std::wstring::npos )
						{
							NSStringUtils::string_replace(m_pVm->m_sOperationSystem, sPrefix + L" ", sPrefix);
							pos = m_pVm->m_sOperationSystem.find(sPrefix + L" ");
						}
						NSStringUtils::string_replace(m_pVm->m_sOperationSystem, sPrefix, L"");

						bResult = true;
						break;
					}
				}
			}
		}

		return bResult;
	}

	bool DownloadDistrib()
	{
		bool bResult = false;

		if ( m_pVm )
		{
			// Prepare url
			std::wstring sUrl = L"";

			CheckOperationSystem();

			if ( m_pVm->IsDebian() )
				sUrl = m_sDebianUrl;

			else if ( m_pVm->IsRedHat() )
			{
				std::wstring sOsLower = m_pVm->m_sOperationSystem;
				std::transform(sOsLower.begin(), sOsLower.end(), sOsLower.begin(), tolower);

				if ( sOsLower.find(L"centos") != std::wstring::npos ||
					 sOsLower.find(L"fedora") != std::wstring::npos)
					sUrl = m_sCentosUrl;
				else if ( sOsLower.find(L"opensuse") != std::wstring::npos )
					sUrl = m_sOpSuseUrl;
			}

			if (m_bVerboseLog)
				WriteReport(L"Start downloading: " + sUrl);

			std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
									L" run --exe /usr/bin/curl" +
									L" --username " + m_sVmUser +
									L" --password " + m_sVmPassword +
									L" --wait-stdout -- curl/arg0 " + sUrl +
									L" --output " + GetWorkingDir() + L"/" + NSFile::GetFileName(sUrl);

			std::wstring sOutput = ExecuteCommand(sCommand);

			// Wait flush to disk. Wait min
			NSThreads::Sleep(60000);
			bResult = true;

			if (m_bVerboseLog)
				WriteReportResult(bResult);
		}

		return bResult;
	}

	bool CopyScripts()
	{
		bool bResult = false;

		if ( m_pVm )
		{
			if (m_bVerboseLog)
				WriteReport(L"Copiyng scripts");

			// Setup
			std::wstring sData = L"";
			std::wstring sUrl = m_pVm->IsDebian() ? m_sDebianUrl : m_sCentosUrl;
			std::wstring sScriptPath = NSDirectory::GetTempPath() + L"/" + m_sSetupScript;
			std::wstring sDistribFile = NSFile::GetFileName(sUrl);

			if ( NSFile::CFileBinary::Exists(sScriptPath) )
				NSFile::CFileBinary::Remove(sScriptPath);

			if ( m_pVm->IsDebian() )
			{
				if ( m_sDebianScript.length() )
				{
					if ( NSFile::CFileBinary::Exists(m_sDebianScript) )
					{
						NSFile::CFileBinary::ReadAllTextUtf8(m_sDebianScript, sData);
					}
				}
				else
				{
					sData = L"#!/bin/bash\n" \
							L"echo \"Install DesktopEditors\"\n" \
							L"apt purge onlyoffice-desktopeditors -y\n" \
							L"dpkg -i ./" + sDistribFile + "\n" \
							L"apt install -f";
				}
			}
			else if ( m_pVm->IsRedHat() )
			{
				if ( m_sRedHatScript.length() )
				{
					NSFile::CFileBinary::ReadAllTextUtf8(m_sRedHatScript, sData);
				}
				else
				{
					sData = L"#!/bin/bash\n" \
							L"echo \"Install DesktopEditors\"\n" \
							L"rpm -e onlyoffice-desktopeditors\n" \
							L"rpm -i ./" + sDistribFile;
				}
			}

			NSFile::CFileBinary oFile;
			bResult = oFile.CreateFileW(sScriptPath);
			oFile.WriteStringUTF8(sData);
			oFile.CloseFile();

			std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
									L" --username " + m_sVmUser +
									L" --password " + m_sVmPassword +
									L" copyto " + sScriptPath + L" " + GetWorkingDir() + L"/" + m_sSetupScript;

			std::wstring sOutput = ExecuteCommand(sCommand);

			NSFile::CFileBinary::Remove(sScriptPath);
			//NSThreads::Sleep(10000);

			// Run
			sScriptPath = NSDirectory::GetTempPath() + L"/" + m_sRunScript;
			std::wstring sStdoutFile = GetWorkingDir() + L"/" + m_sStdoutFile;

			if ( NSFile::CFileBinary::Exists(sScriptPath) )
				NSFile::CFileBinary::Remove(sScriptPath);

			std::wstring sEditorsFolder = m_sEditorsPath;
			NSStringUtils::string_replace(sEditorsFolder, NSFile::GetFileName(m_sEditorsPath), L"");

			sData = L"#!/bin/bash\n" \
					L"LD_LIBRARY_PATH=" + sEditorsFolder + L" " + m_sEditorsPath + L" --ascdesktop-log-file=" + sStdoutFile;

			bResult &= oFile.CreateFileW(sScriptPath);
			oFile.WriteStringUTF8(sData);
			oFile.CloseFile();

			sCommand = L"guestcontrol " + m_pVm->m_sGuid +
					   L" --username " + m_sVmUser +
					   L" --password " + m_sVmPassword +
					   L" copyto " + sScriptPath + L" " + GetWorkingDir() + L"/" + m_sRunScript;

			sOutput = ExecuteCommand(sCommand);

			NSFile::CFileBinary::Remove(sScriptPath);
			//NSThreads::Sleep(10000);

			if (m_bVerboseLog)
				WriteReportResult(bResult);
		}

		return bResult;
	}

	bool RemoveScripts()
	{
		bool bResult = true;

		if ( m_pVm )
		{
			if (m_bVerboseLog)
				WriteReport(L"Removing scripts");

			std::vector<std::wstring> arrScipts;
			arrScipts.push_back(m_sRunScript);
			arrScipts.push_back(m_sSetupScript);

			for (size_t i = 0; i < arrScipts.size(); i++)
			{
				std::wstring sScriptPath = GetWorkingDir() + L"/" + arrScipts[i];

				if ( IsLocationExists(sScriptPath) )
				{
					std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
											L" --username " + m_sVmUser +
											L" --password " + m_sVmPassword +
											L" rm " + sScriptPath;

					std::wstring sOutput = ExecuteCommand(sCommand);

					bResult &= !IsLocationExists(sScriptPath);
				}
			}

			if (m_bVerboseLog)
				WriteReportResult(bResult);
		}

		return bResult;
	}

	bool RunEditors()
	{
		bool bResult = true;

		if ( m_pVm && IsLocationExists(m_sEditorsPath) )
		{
			if (m_bVerboseLog)
				WriteReport(L"Runing DesktopEditors");

			std::wstring sRunScript = GetWorkingDir() + L"/" + m_sRunScript;

			std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
									L" start " + sRunScript +
									L" --username " + m_sVmUser +
									L" --password " + m_sVmPassword +
									L" --putenv DISPLAY=:0.0";

			std::wstring sOutput = ExecuteCommand(sCommand);
		}

		return bResult;
	}

	bool IsReadyReset()
	{
		bool bResult = true;

		if ( m_pVm )
		{
			if (m_bVerboseLog)
				WriteReport(L"Ready restart");

			std::wstring sUrl = m_pVm->IsDebian() ? m_sDebianUrl : m_sCentosUrl;
			std::wstring sRunScript = GetWorkingDir() + L"/" + m_sSetupScript;
			std::wstring sSetupScript = GetWorkingDir() + L"/" + m_sSetupScript;
			std::wstring sDistribPath = GetWorkingDir() + L"/" + NSFile::GetFileName(sUrl);

			bResult = IsLocationExists(sRunScript) &&
					  IsLocationExists(sSetupScript) &&
					  IsLocationExists(sDistribPath);

			if (m_bVerboseLog)
				WriteReportResult(bResult);
		}

		return bResult;
	}

	bool IsEditorsRunned()
	{
		bool bResult = true;

		if ( m_pVm )
		{
			if (m_bVerboseLog)
				WriteReport(L"DesktopEditors runned");

			std::wstring sEditorProc = NSFile::GetFileName(m_sEditorsPath);
			bResult = IsProcessExists(sEditorProc);

			if (m_bVerboseLog)
				WriteReportResult(bResult);
		}

		return bResult;
	}

	// Report
	void CreateReport()
	{
		std::ofstream oFile;
		std::wstring sReportPath = GetReportDir() + L"/" + m_sReportName;
		oFile.open(U_TO_UTF8(sReportPath), std::ofstream::out | std::ofstream::trunc);
		oFile.close();
	}

	void WriteReport(const std::wstring& sText)
	{
		if ( sText.length() )
		{
			std::wofstream oFile;
			std::wstring sReportPath = GetReportDir() + L"/" + m_sReportName;
			oFile.open(U_TO_UTF8(sReportPath), std::ios_base::app);
			oFile << sText << std::endl;
			oFile.close();

			std::wcout << sText << std::endl;
		}
	}

	void WriteReportResult(const bool& bResult)
	{
		std::wofstream oFile;
		std::wstring sReportPath = GetReportDir() + L"/" + m_sReportName;
		oFile.open(U_TO_UTF8(sReportPath), std::ios_base::app);
		oFile << BoolToStr(bResult) + L"\n" << std::endl;
		oFile.close();

		std::wcout << BoolToStr(bResult) + L"\n" << std::endl;
	}

	void RemoveReport()
	{
		if ( NSDirectory::Exists(GetReportDir()) )
			NSDirectory::DeleteDirectory(GetReportDir());
	}

	// Config
	bool IsConfigExists()
	{
		return NSFile::CFileBinary::Exists(m_sConfigName);
	}

	bool CreateConfig()
	{
		std::wstring sDebian = L"";
		std::wstring sRedHat = L"";
		std::vector<CVm*> arrDebian = GetDebianVms();
		std::vector<CVm*> arrRedHat = GetRedHatVms();

		for (size_t i = 0; i < arrDebian.size(); i++)
		{
			sDebian += arrDebian[i]->m_sName + (i < arrDebian.size() - 1 ? L", " : L"");
		}
		for (size_t i = 0; i < arrRedHat.size(); i++)
		{
			sRedHat += arrRedHat[i]->m_sName + (i < arrRedHat.size() - 1 ? L", " : L"");
		}

		std::wstring sData = sCfgUser + L"\n" +
							 sCfgPassword + L"\n\n" +
							 sCfgBranch + L"7.4.0\n" +
							 sCfgVersion + L"173\n\n" +
							 sCfgDebianStart + sDebian + L"\n" +
							 sCfgRedhatStart + sRedHat + L"\n\n" +
							 sCfgDebianScript + L"\n" +
							 sCfgRedhatScript + L"\n\n" +
							 sCfgVerboseLog + L"0\n";

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(m_sConfigName);
		oFile.WriteStringUTF8(sData);
		oFile.CloseFile();

		return IsConfigExists();
	}

	bool ReadConfig()
	{
		bool bResult = false;

		if ( IsConfigExists() )
		{
			std::wstring sData = L"";

			NSFile::CFileBinary oFile;
			if ( oFile.ReadAllTextUtf8(m_sConfigName, sData) )
			{
				std::vector<std::wstring> arrLines;
				if ( SplitStringAsVector(sData, L"\n", arrLines) )
				{
					for (size_t i = 0; i < arrLines.size(); i++)
					{
						std::wstring sLine = arrLines[i];

						// Auth
						if ( sLine.find(sCfgUser) != std::wstring::npos )
							m_sVmUser = sLine.substr(sCfgUser.length());
						else if ( sLine.find(sCfgPassword) != std::wstring::npos )
							m_sVmPassword = sLine.substr(sCfgPassword.length());

						// URL
						else if ( sLine.find(sCfgBranch) != std::wstring::npos )
							m_sBranch = sLine.substr(sCfgBranch.length());
						else if ( sLine.find(sCfgVersion) != std::wstring::npos )
							m_sVersion = sLine.substr(sCfgVersion.length());

						// Systems
						else if ( sLine.find(sCfgDebianStart) != std::wstring::npos )
							m_sDebianStart = sLine.substr(sCfgDebianStart.length());
						else if ( sLine.find(sCfgRedhatStart) != std::wstring::npos )
							m_sRedHatStart = sLine.substr(sCfgRedhatStart.length());

						// Custom setup scripts
						else if ( sLine.find(sCfgDebianScript) != std::wstring::npos )
							m_sDebianScript = sLine.substr(sCfgDebianScript.length());
						else if ( sLine.find(sCfgRedhatScript) != std::wstring::npos )
							m_sRedHatScript = sLine.substr(sCfgRedhatScript.length());

						// Log
						else if ( sLine.find(sCfgVerboseLog) != std::wstring::npos )
							m_bVerboseLog = sLine.substr(sCfgVerboseLog.length()) == L"1";
					}

					// Prepare urls
					if ( m_sBranch.length() && m_sVersion.length() )
					{
						std::wstring sAmazonS3 = L"https://s3.eu-west-1.amazonaws.com/repo-doc-onlyoffice-com/desktop/linux";

						if ( m_sDebianStart.length() )
						{
							m_sDebianUrl = sAmazonS3 + L"/debian/onlyoffice-desktopeditors_{BRANCH}-{VERSION}_amd64.deb";
							NSStringUtils::string_replace(m_sDebianUrl, L"{BRANCH}", m_sBranch);
							NSStringUtils::string_replace(m_sDebianUrl, L"{VERSION}", m_sVersion);
						}
						if ( m_sRedHatStart.length() )
						{
							m_sCentosUrl = sAmazonS3 + L"/rhel/onlyoffice-desktopeditors-{BRANCH}-{VERSION}.el7.x86_64.rpm";
							NSStringUtils::string_replace(m_sCentosUrl, L"{BRANCH}", m_sBranch);
							NSStringUtils::string_replace(m_sCentosUrl, L"{VERSION}", m_sVersion);

							m_sOpSuseUrl = sAmazonS3 + L"/suse/onlyoffice-desktopeditors-{BRANCH}-{VERSION}.suse12.x86_64.rpm";
							NSStringUtils::string_replace(m_sOpSuseUrl, L"{BRANCH}", m_sBranch);
							NSStringUtils::string_replace(m_sOpSuseUrl, L"{VERSION}", m_sVersion);
						}
					}
				}

				oFile.CloseFile();
			}
		}

		return bResult;
	}

private:
	std::wstring GetWorkingDir()
	{
		std::wstring sDir = L"";

		if ( m_sVmUser.length() )
			sDir = L"/home/" + m_sVmUser + L"/vboxtester";

		return sDir;
	}

	std::wstring GetReportDir()
	{
		std::wstring sAppPath = NSFile::GetProcessDirectory();
		sAppPath += L"/report";

		if ( !NSDirectory::Exists(sAppPath) )
			NSDirectory::CreateDirectory(sAppPath);

		return sAppPath;
	}

	bool IsLocationExists(const std::wstring& sPath)
	{
		// check ile or folder
		bool bResult = false;

		if ( m_pVm && sPath.length() )
		{
			std::wstring sFile = L"";
			std::wstring sFolder = sPath;
			std::vector<std::wstring> arrParts;
			if ( SplitStringAsVector(sPath, L"/", arrParts) )
			{
				sFile = arrParts[arrParts.size() - 1];
				NSStringUtils::string_replace(sFolder, L"/" + sFile, L"");
			}

			std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
									L" run --exe /bin/ls" +
									L" --username " + m_sVmUser +
									L" --password " + m_sVmPassword +
									L" --wait-stdout -- ls/arg0 " + sFolder;

			std::wstring sOutput = ExecuteCommand(sCommand);
			if ( sOutput.length() )
			{
				if ( SplitStringAsVector(sOutput, L"\n", arrParts) )
					bResult = std::find(arrParts.begin(), arrParts.end(), sFile) != arrParts.end();
			}
		}

		return bResult;
	}

	bool IsProcessExists(const std::wstring& sName)
	{
		bool bResult = false;

		if ( m_pVm && sName.length() )
		{
			std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
									L" run --exe /bin/ps" +
									L" --username " + m_sVmUser +
									L" --password " + m_sVmPassword +
									L" --wait-stdout -- ps/arg0 -e";

			std::wstring sOutput = ExecuteCommand(sCommand);

			if ( sOutput.length() )
			{
				std::vector<std::wstring> arrLines;
				std::vector<std::wstring> arrParts;

				if ( SplitStringAsVector(sOutput, L"\n", arrLines) )
				{
					for (size_t i = 0; i < arrLines.size(); i++)
					{
						std::wstring sLine = arrLines[i];
						if ( (i > 0) && SplitStringAsVector(sLine, L" ", arrParts) )
						{
							if ( arrParts[arrParts.size() - 1] == sName )
							{
								bResult = true;
								break;
							}
						}
					}
				}
			}
		}

		return bResult;
	}

	bool IsVmLoggedIn()
	{
		bool bResult = false;

		if ( m_pVm )
		{
			// whoami check
			std::wstring sBin = L"/usr/bin/whoami";
			if ( m_pVm->IsRedHat() )
				NSStringUtils::string_replace(sBin, L"/usr", L"");

			std::wstring sCommand = L"guestcontrol " + m_pVm->m_sGuid +
									L" run --exe " + sBin +
									L" --username " + m_sVmUser +
									L" --password " + m_sVmPassword +
									L" --wait-stdout";

			std::wstring sOutput = ExecuteCommand(sCommand);
			bool bWhoami = sOutput.find(m_sVmUser) != std::wstring::npos;

			// uptime check
			sBin = L"/usr/bin/uptime";
			if ( m_pVm->IsRedHat() )
				NSStringUtils::string_replace(sBin, L"/usr", L"");

			sCommand = L"guestcontrol " + m_pVm->m_sGuid +
					   L" run --exe " + sBin +
					   L" --username " + m_sVmUser +
					   L" --password " + m_sVmPassword +
					   L" --wait-stdout";

			sOutput = ExecuteCommand(sCommand);
			bool bUptime = sOutput.find(L"user") != std::wstring::npos;

			// TODO check: ps -e | grep X

			// connection
			sCommand = L"guestcontrol " + m_pVm->m_sGuid +
					   L" run --exe /usr/bin/curl" +
					   L" --username " + m_sVmUser +
					   L" --password " + m_sVmPassword +
					   L" --wait-stdout -- curl/arg0 -I http://www.google.com";

			sOutput = ExecuteCommand(sCommand);
			bool bConnection = sOutput.find(L"200 OK") != std::wstring::npos;

			bResult = (bWhoami || bUptime) && bConnection;
		}

		return bResult;
	}

	std::vector<CVm*> GetDebianVms()
	{
		std::vector<CVm*> arrVms;

		for (size_t i = 0; i < m_arrVms.size(); i++)
		{
			if ( m_arrVms[i]->IsDebian() )
			{
				arrVms.push_back(m_arrVms[i]);
			}
		}

		return arrVms;
	}

	std::vector<CVm*> GetRedHatVms()
	{
		std::vector<CVm*> arrVms;

		for (size_t i = 0; i < m_arrVms.size(); i++)
		{
			if ( m_arrVms[i]->IsRedHat() )
			{
				arrVms.push_back(m_arrVms[i]);
			}
		}

		return arrVms;
	}

	std::wstring GetVmOS(const std::wstring& sGuid)
	{
		return ParseVmInfo(sGuid, L"Guest OS");
	}

	std::wstring ParseVmInfo(const std::wstring& sGuid, const std::wstring& sPref)
	{
		std::wstring sStatus = L"";

		if ( sGuid.length() && sPref.length() )
		{
			std::wstring command = L"showvminfo " + sGuid;
			std::wstring sOutput = ExecuteCommand(command);

			std::vector<std::wstring> arrLines;
			if ( SplitStringAsVector(sOutput, L"\n", arrLines) )
			{
				std::wstring sPrefix = sPref + L":";

				for (size_t i = 0; i < arrLines.size(); i++)
				{
					std::wstring sLine = arrLines[i];

					std::wstring::size_type pos = sLine.find(sPrefix);
					if ( pos != std::wstring::npos )
					{
						sStatus = sLine;
						pos = sStatus.find(sPrefix + L" ");
						while ( pos != std::wstring::npos )
						{
							NSStringUtils::string_replace(sStatus, sPrefix + L" ", sPrefix);
							pos = sStatus.find(sPrefix + L" ");
						}
						NSStringUtils::string_replace(sStatus, sPrefix, L"");
						break;
					}
				}
			}
		}

		return sStatus;
	}

	std::wstring ExecuteCommand(const std::wstring& sArgs)
	{
		std::wstring sResult = L"";

		std::wstring sCommand = m_sVbmPath + L" " + sArgs;

#ifdef WIN32
		std::array<wchar_t, 128> aBuffer;
		FILE* pipe = _wpopen(sCommand.c_str(), L"r");
#endif
#ifdef LINUX
		std::array<char, 128> aBuffer;
		FILE* pipe = popen(U_TO_UTF8(sCommand).c_str(), "r");
#endif
		if (!pipe)
			return sResult;

#ifdef WIN32
		while ( fgetws(aBuffer.data(), 128, pipe) != NULL )
		{
			sResult += aBuffer.data();
		}
#endif
#ifdef LINUX
		while ( fgets(aBuffer.data(), 128, pipe) != NULL )
		{
			std::string sBuf = aBuffer.data();
			sResult += UTF8_TO_U(sBuf);
		}
#endif

		return sResult;
	}

	std::wstring BoolToStr(bool bResult)
	{
		return bResult ? L"OK" : L"ERROR";
	}
};

// Main
#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	// Test
	CVirtualBox oTester;
	oTester.InitVms();

	if ( !oTester.IsConfigExists() )
	{
		oTester.CreateConfig();
		return 0;
	}
	oTester.ReadConfig();

	oTester.RemoveReport();
	oTester.CreateReport();

	std::vector<CVm*> arrStartVms = oTester.GetStartVms();

	for (size_t i = 0; i < arrStartVms.size(); i++)
	{
		CVm* pVm = arrStartVms[i];
		std::wstring sGuid = pVm->m_sGuid;
		std::wstring sName = pVm->m_sName;

		oTester.SetVm(pVm);

		oTester.StartVm();
		oTester.WaitLoadVm();

		oTester.CreateWorkingDir();
		oTester.CopyScripts();
		oTester.DownloadDistrib();

		bool bPassed = false;
		if ( oTester.IsReadyReset() )
		{
			oTester.ResetVm();
			oTester.WaitLoadVm();

			if ( oTester.WaitInstall() )
			{
				oTester.RunEditors();
				oTester.WaitStdout();

				if ( oTester.IsEditorsRunned() )
				{
					oTester.SaveScreenshot();
					bPassed = true;
				}
			}
		}

		oTester.WriteReport(bPassed ? L"PASSED\n" : L"FAILED\n");

		oTester.RemoveScripts();
		oTester.StopVm();
	}

	return 0;
}
