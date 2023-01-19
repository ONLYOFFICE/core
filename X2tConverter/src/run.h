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
#ifndef X2TRUN_H
#define X2TRUN_H

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/graphics/BaseThread.h"

#ifdef _LINUX
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#endif

namespace NSX2T
{
	int Convert(const std::wstring& sConverterDirectory, const std::wstring sXmlPath)
	{
		int nReturnCode = 0;
		std::wstring sConverterExe = sConverterDirectory + L"/x2t";

#ifdef WIN32
		NSStringUtils::string_replace(sConverterExe, L"/", L"\\");

		sConverterExe += L".exe";
		std::wstring sApp = L"x2t ";

		STARTUPINFO sturtupinfo;
		ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
		sturtupinfo.cb = sizeof(STARTUPINFO);

		sApp += (L"\"" + sXmlPath + L"\"");
		wchar_t* pCommandLine = NULL;
		if (true)
		{
			pCommandLine = new wchar_t[sApp.length() + 1];
			memcpy(pCommandLine, sApp.c_str(), sApp.length() * sizeof(wchar_t));
			pCommandLine[sApp.length()] = (wchar_t)'\0';
		}

		HANDLE ghJob = CreateJobObject(NULL, NULL);

		if (ghJob)
		{
			JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };

			// Configure all child processes associated with the job to terminate when the
			jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
			if ( 0 == SetInformationJobObject( ghJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
			{
				CloseHandle(ghJob);
				ghJob = NULL;
			}
		}

		PROCESS_INFORMATION processinfo;
		ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
		BOOL bResult = CreateProcessW(sConverterExe.c_str(), pCommandLine,
									  NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

		if (bResult && ghJob)
		{
			AssignProcessToJobObject(ghJob, processinfo.hProcess);
		}

		::WaitForSingleObject(processinfo.hProcess, INFINITE);

		RELEASEARRAYOBJECTS(pCommandLine);

		//get exit code
		DWORD dwExitCode = 0;
		if (GetExitCodeProcess(processinfo.hProcess, &dwExitCode))
		{
			nReturnCode = (int)dwExitCode;
		}

		CloseHandle(processinfo.hProcess);
		CloseHandle(processinfo.hThread);

		if (ghJob)
		{
			CloseHandle(ghJob);
			ghJob = NULL;
		}

#endif

#ifdef LINUX
		pid_t pid = fork(); // create child process
		int status;

		std::string sProgramm = U_TO_UTF8(sConverterExe);
		std::string sXmlA = U_TO_UTF8(sXmlPath);

		switch (pid)
		{
		case -1: // error
			break;

		case 0: // child process
		{
			std::string sLibraryDir = sProgramm;
			std::string sPATH = sProgramm;
			if (std::string::npos != sProgramm.find_last_of('/'))
			{
				sLibraryDir = "LD_LIBRARY_PATH=" + sProgramm.substr(0, sProgramm.find_last_of('/'));
				sPATH = "PATH=" + sProgramm.substr(0, sProgramm.find_last_of('/'));
			}

#ifdef _MAC
			sLibraryDir = "DY" + sLibraryDir;
#endif

			const char* nargs[3];
			nargs[0] = sProgramm.c_str();
			nargs[1] = sXmlA.c_str();
			nargs[2] = NULL;

#ifndef _MAC
			const char* nenv[2];
			nenv[0] = sLibraryDir.c_str();
			nenv[1] = NULL;
#else
			const char* nenv[3];
			nenv[0] = sLibraryDir.c_str();
			nenv[1] = sPATH.c_str();
			nenv[2] = NULL;
#endif

			execve(sProgramm.c_str(),
				   (char * const *)nargs,
				   (char * const *)nenv);
			exit(EXIT_SUCCESS);
			break;
		}
		default: // parent process, pid now contains the child pid
			while (-1 == waitpid(pid, &status, 0)); // wait for child to complete
			if(WIFSIGNALED(status))
			{
				nReturnCode = status;
			}
			else if (WIFEXITED(status))
			{
				nReturnCode =  WEXITSTATUS(status);
			}
			break;
		}
#endif

		return nReturnCode;
	}
}

#endif // X2TRUN_H
