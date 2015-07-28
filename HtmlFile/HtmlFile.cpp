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

    wchar_t* pCommandLine = NULL;
    if (!sXml.empty())
    {
        pCommandLine = new wchar_t[sXml.length() + 1];
        memcpy(pCommandLine, sXml.c_str(), sXml.length() * sizeof(wchar_t));
        pCommandLine[sXml.length()] = (wchar_t)'\0';
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
#endif

#ifdef LINUX
    pid_t pid = fork(); // create child process
    int status;

    std::string sProgramm = U_TO_UTF8(sInternal);
    std::string sXmlA = "";
    if (!sXml.empty())
        sXmlA = U_TO_UTF8(sXml);

    FILE* f = fopen("/home/oleg/a.txt", "a+");
    fprintf(f, "HtmlCodeAfterFork\n");
    fclose(f);

    switch (pid)
    {
    case -1: // error
        break;

    case 0: // child process
    {
        execl(sProgramm.c_str(), sXmlA.empty() ? NULL : sXmlA.c_str(), 0); // run the command
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

    return nReturnCode;
}
