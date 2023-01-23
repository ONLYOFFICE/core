/*
 * (c) Copyright Ascensio System SIA 2010-2021
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

#include <iostream>
#include <unistd.h>
#include "../../DesktopEditor/common/Directory.h"

#ifdef USE_EXTERNAL_TRANSPORT

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <regex>
#include <atomic>

#ifdef _MAC
#include <signal.h>
#endif

namespace NSNetwork
{
    namespace NSFileTransport
    {        
        std::string wget_url_validate(const std::string& url)
        {
            std::string::size_type pos = 0;
            const char* url_ptr = url.c_str();
            while ('-' == *url_ptr++) // '\0' => break
                ++pos;
            if (*url_ptr == '\0')
                return "";

            return url.substr(pos);
        }

        int download_external(const std::wstring& sUrl, const std::wstring& sOutput, std::function<void(int)> func_onProgress = nullptr, std::function<bool(void)> func_checkAborted = nullptr)
        {
            pid_t pid;
            int nReturnCode = -1;
            std::string sUrlA = U_TO_UTF8(sUrl);
            //sUrlA =("\"" + sUrlA + "\"");
            std::string sOutputA = U_TO_UTF8(sOutput);
            //sOutputA =("\"" + sOutputA + "\"");

            if (0 != nReturnCode && NSFile::CFileBinary::Exists(L"/usr/bin/curl"))
            {
                int pipefd[2];
                if(func_onProgress)
                    pipe(pipefd);

                pid = fork(); // create child process
                int status;

                switch (pid)
                {
                case -1: // error
                    break;

                case 0: // child process
                {
                    const char* nargs[10];
                    nargs[0] = "/usr/bin/curl";
                    nargs[1] = "--url";
                    nargs[2] = sUrlA.c_str();
                    nargs[3] = "--output";
                    nargs[4] = sOutputA.c_str();
                    func_onProgress == NULL ? nargs[5] = "--silent" : nargs[5] = "--progress-bar";
                    nargs[6] = "-L";
                    nargs[7] = "--connect-timeout";
                    nargs[8] = "10";
                    nargs[9] = NULL;

                    const char* nenv[3];
                    nenv[0] = "LD_PRELOAD=";
                    nenv[1] = "LD_LIBRARY_PATH=";
                    nenv[2] = NULL;

                    if(func_onProgress)
                    {
                        close(pipefd[0]);    // close reading end in the child

                        dup2(pipefd[1], 1);  // send stdout to the pipe
                        dup2(pipefd[1], 2);  // send stderr to the pipe

                        close(pipefd[1]);    // this descriptor is no longer needed
                    }

                    execve("/usr/bin/curl", (char * const *)nargs, (char * const *)nenv);
                    exit(EXIT_SUCCESS);
                    break;
                }
                default: // parent process, pid now contains the child pid
                    if(func_onProgress)
                    {
                        close(pipefd[1]);
                        // close the write end of the pipe in the parent
                        size_t size = 81;
                        char buffer[size];
                        std::string str;
                        ssize_t res = 1;
                        std::regex r(R"(\d+(?:\.\d+)?%)");
                        std::smatch sm;
                        std::string percentFull;
                        std::string percent;
                        int percentInt;

                        while (1)
                        {
                            if(func_checkAborted && func_checkAborted())
                            {
                                kill(pid, SIGTERM);
                                //while (-1 == waitpid(pid, &status, 0)); // wait for child to complete
                                return nReturnCode;
                            }

                            str.clear();
                            res = read(pipefd[0], buffer, sizeof(buffer));

                            if(res == 0)
                                break;

                            str.append(buffer);

                            if(regex_search(str, sm, r))
                            {
                                percentFull     = sm.str();
                                percent         = percentFull.substr(0, percentFull.find("."));
                                percentInt      = std::stoi(percent);

                                if(percentInt >= 0 && percentInt <= 100)
                                    func_onProgress(percentInt);
                            }

                            if(str.find("100.0%") != std::string::npos)
                                break;

                        }
                    }
                    else {
                        int waitres;
                        while (1) // wait for child to complete
                        {
                            if(func_checkAborted && func_checkAborted())
                            {
                                kill(pid, SIGTERM);
                                return nReturnCode;
                            }
                            else if((waitres = waitpid(pid, &status, WNOHANG)) > 0)
                            {
                                if (WIFEXITED(status))
                                {
                                    nReturnCode =  WEXITSTATUS(status);
                                }
                                break;
                            }
                        }
                    }
                }
            }

            if (0 != nReturnCode && NSFile::CFileBinary::Exists(L"/usr/bin/wget"))
            {
                std::string sUrlValidateA = wget_url_validate(sUrlA);

                pid = fork(); // create child process
                int status;

                switch (pid)
                {
                case -1: // error
                    break;

                case 0: // child process
                {
                    const char* nargs[8];
                    nargs[0] = "/usr/bin/wget";
                    nargs[1] = sUrlValidateA.c_str();
                    nargs[2] = "-O";
                    nargs[3] = sOutputA.c_str();
                    nargs[4] = "-q";
                    nargs[5] = "--connect-timeout=10";
                    nargs[6] = "--tries=2";
                    nargs[7] = NULL;

                    const char* nenv[2];
                    nenv[0] = "LD_PRELOAD=";
                    nenv[1] = NULL;

                    execve("/usr/bin/wget", (char * const *)nargs, (char * const *)nenv);
                    exit(EXIT_SUCCESS);
                    break;
                }
                default: // parent process, pid now contains the child pid
                    int waitres;
                    while (1) // wait for child to complete
                    {
                        if(func_checkAborted && func_checkAborted())
                        {
                            kill(pid, SIGTERM);
                            return nReturnCode;
                        }
                        else if((waitres = waitpid(pid, &status, WNOHANG)) > 0)
                        {
                            if (WIFEXITED(status))
                            {
                                nReturnCode =  WEXITSTATUS(status);
                            }
                            break;
                        }
                    }
                }
            }

            if (0 == nReturnCode)
            {
                if (!NSFile::CFileBinary::Exists(sOutput))
                    nReturnCode = -1;
            }

            return nReturnCode;
        }

        int uploaddata_external(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize)
        {
            int nReturnCode = -1;
            NSFile::CFileBinary oFileData;
            std::wstring tempFileName = NSFile::CFileBinary::GetTempPath() + L"/tmpFileForUpload";
            oFileData.CreateFileW(tempFileName);
            oFileData.WriteFile(cData, nSize);
            oFileData.CloseFile();

            std::string sUploadUrlA = U_TO_UTF8(sUploadUrl);


            if (0 != nReturnCode && NSFile::CFileBinary::Exists(L"/usr/bin/curl"))
            {
                pid_t pid = fork(); // create child process
                int status;

                switch (pid)
                {
                case -1: // error
                    break;

                case 0: // child process
                {
                    //curl --request POST --data-binary "@template_entry.xml" $URL
                    auto pathTofile(std::string("@") + std::string(tempFileName.begin(), tempFileName.end()));
                    const char* nargs[9];
                    nargs[0] = "/usr/bin/curl";
                    nargs[1] = "--request";
                    nargs[2] = "POST";
                    nargs[3] = "--data-binary";
                    nargs[4] = pathTofile.c_str();
                    nargs[5] = sUploadUrlA.c_str();
                    nargs[6] = "--connect-timeout";
                    nargs[7] = "10";
                    nargs[8] = NULL;

                    const char* nenv[3];
                    nenv[0] = "LD_PRELOAD=";
                    nenv[1] = "LD_LIBRARY_PATH=";
                    nenv[2] = NULL;

                    execve("/usr/bin/curl", (char * const *)nargs, (char * const *)nenv);
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
            }
            NSFile::CFileBinary::Remove(tempFileName);

            return nReturnCode;
        }

        int uploadfile_external(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath)
        {
            int nReturnCode = -1;

            std::string sUploadUrlA = U_TO_UTF8(sUploadUrl);
            std::string sUploadFilePathA = U_TO_UTF8(sUploadFilePath);
            std::string sUploadFileNameA = U_TO_UTF8(NSFile::GetFileName(sUploadFilePath));

            if (0 != nReturnCode && NSFile::CFileBinary::Exists(L"/usr/bin/curl"))
            {
                pid_t pid = fork(); // create child process
                int status;

                switch (pid)
                {
                case -1: // error
                    break;

                case 0: // child process
                {
                    //curl -v -F filename=image.jpg -F upload=@/home/vladimir/Pictures/Test.png $URL
                    auto filename(std::string("filename=") + sUploadFileNameA);
                    auto upload(std::string("upload=@") + sUploadFilePathA);
                    const char* nargs[10];
                    nargs[0] = "/usr/bin/curl";
                    nargs[1] = "-v";
                    nargs[2] = "-F";
                    nargs[3] = filename.c_str();
                    nargs[4] = "-F";
                    nargs[5] = upload.c_str();
                    nargs[6] = sUploadUrlA.c_str();
                    nargs[7] = "--connect-timeout";
                    nargs[8] = "10";
                    nargs[9] = NULL;

                    const char* nenv[3];
                    nenv[0] = "LD_PRELOAD=";
                    nenv[1] = "LD_LIBRARY_PATH=";
                    nenv[2] = NULL;

                    execve("/usr/bin/curl", (char * const *)nargs, (char * const *)nenv);
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
            }

            return nReturnCode;
        }
    }
}
#endif
