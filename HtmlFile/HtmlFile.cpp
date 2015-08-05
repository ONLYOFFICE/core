#include "HtmlFile.h"
#include "../DesktopEditor/common/File.h"

#ifdef LINUX
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#endif

CHtmlFile::CHtmlFile()
{

}

CHtmlFile::~CHtmlFile()
{

}

int CHtmlFile::Convert(const std::wstring& sXml, const std::wstring& sPathInternal)
{
    std::wstring sInternal = sPathInternal;
    if (sInternal.empty())
        sInternal = NSFile::GetProcessDirectory() + L"/HtmlFileInternal/";

    sInternal += L"HtmlFileInternal";

#ifdef WIN32
    sInternal += L".exe";
#endif

    int nReturnCode = 0;

#ifdef WIN32
    STARTUPINFO sturtupinfo;
    ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
    sturtupinfo.cb = sizeof(STARTUPINFO);

    std::wstring sTempFileForParams = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"XML");
    NSFile::CFileBinary oFile;
    oFile.CreateFileW(sTempFileForParams);
    oFile.WriteStringUTF8(sXml, true);
    oFile.CloseFile();

    std::wstring sApp = L"HtmlFileInternal <html>" + sTempFileForParams;
    wchar_t* pCommandLine = NULL;
    if (!sXml.empty())
    {
        pCommandLine = new wchar_t[sApp.length() + 1];
        memcpy(pCommandLine, sApp.c_str(), sApp.length() * sizeof(wchar_t));
        pCommandLine[sApp.length()] = (wchar_t)'\0';
    }

    PROCESS_INFORMATION processinfo;
    ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
    BOOL bResult = CreateProcessW(sInternal.c_str(), pCommandLine,
                               NULL, NULL, TRUE, NULL, NULL, NULL, &sturtupinfo, &processinfo);

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

    NSFile::CFileBinary::Remove(sTempFileForParams);
#endif

#ifdef LINUX
    std::wstring sTempFileForParams = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"XML");
    NSFile::CFileBinary oFile;
    oFile.CreateFileW(sTempFileForParams);
    oFile.WriteStringUTF8(sXml, true);
    oFile.CloseFile();

    pid_t pid = fork(); // create child process
    int status;

    std::string sProgramm = U_TO_UTF8(sInternal);
    std::string sXmlA = "<html>" + U_TO_UTF8(sTempFileForParams);

    switch (pid)
    {
    case -1: // error
        break;

    case 0: // child process
    {
        std::string sLibraryDir = sProgramm;
        if (std::string::npos != sProgramm.find_last_of('/'))
            sLibraryDir = "LD_LIBRARY_PATH=" + sProgramm.substr(0, sProgramm.find_last_of('/'));

        const char* nargs[2];
        nargs[0] = sXmlA.c_str();
        nargs[1] = NULL;

        const char* nenv[3];
        nenv[0] = sLibraryDir.c_str();
        nenv[1] = "DISPLAY=:0";
        nenv[2] = NULL;

        execve(sProgramm.c_str(),
               (char * const *)nargs,
               (char * const *)nenv);
        exit(EXIT_SUCCESS);
        break;
    }
    default: // parent process, pid now contains the child pid
        while (-1 == waitpid(pid, &status, 0)); // wait for child to complete
        if (WIFEXITED(status))
        {
            nReturnCode =  WEXITSTATUS(status);
        }
        break;
    }
#endif

    NSFile::CFileBinary::Remove(sTempFileForParams);
    return nReturnCode;
}
