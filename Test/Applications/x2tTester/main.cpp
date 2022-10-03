#include <iostream>

#include "../../../Common/OfficeFileFormats.h"
#include "../../../Common/OfficeFileFormatChecker.h"

#include "../../../DesktopEditor/graphics/Timer.h"
#include "../../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../DesktopEditor/fontengine/ApplicationFontsWorker.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"

#ifdef LINUX
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#endif

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	// creating temporary xml file with params
	NSStringUtils::CStringBuilder builder;
	builder.WriteString(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");

	builder.WriteString(L"<Root>");

	builder.WriteString(L"<m_sFileFrom>");
	builder.WriteString(L"");
	builder.WriteString(L"</m_sFileFrom>");

	builder.WriteString(L"<m_sFileTo>");
	builder.WriteString(L"");
	builder.WriteString(L"</m_sFileTo>");

	builder.WriteString(L"</Root>");

	std::wstring xml_params = builder.GetData();
	std::wstring xml_params_file = L"";
	NSFile::CFileBinary::SaveToFile(xml_params_file, xml_params, true);

	std::wstring x2t_path = L"";
	std::wstring command = x2t_path + L' ' + xml_params_file;
	wchar_t *ptr_command = new wchar_t[command.size() + 1];
	memcpy(ptr_command, command.c_str(), command.length() * sizeof(wchar_t));
	ptr_command[command.size()] = L'\0';

	PROCESS_INFORMATION processinfo;
	STARTUPINFO sturtupinfo;
	ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
	ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
	sturtupinfo.cb = sizeof(sturtupinfo);

	BOOL bool_result = CreateProcessW(NULL, ptr_command,
								  NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

	WaitForSingleObject(processinfo.hProcess, INFINITE);
	RELEASEARRAYOBJECTS(ptr_command);

	DWORD exit_code = 0;
	GetExitCodeProcess(processinfo.hProcess, &exit_code);
	std::cout << exit_code << std::endl;

	// NSFile::CFileBinary::Remove(xml_params_file);

	return 0;
}

// minimal test
// normal paths
// configuration
// csv report
// linux support
// multitask
// etc..
