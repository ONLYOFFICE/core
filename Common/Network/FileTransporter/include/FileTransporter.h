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
#pragma once

#include <string>
#include <functional>
#include "../../../kernel_config.h"

namespace NSNetwork 
{
    namespace NSFileTransport
    {
        //typedef void (*CFileTransporter_OnComplete)(int error);
        // <return> cancel: 1, else 0
        //typedef int (*CFileTransporter_OnProgress)(int percent);

        class KERNEL_DECL IFileTransporter
        {
        public:
            IFileTransporter() {}
            virtual ~IFileTransporter() {}

        public:
            // thread
            virtual void Start(int lPriority) = 0;
            virtual int GetPriority() = 0;
            virtual void Suspend() = 0;
            virtual void Resume() = 0;
            virtual void Stop() = 0;
            virtual void StopNoJoin() = 0;
            virtual void Cancel() = 0;
            virtual int IsRunned() = 0;

            //events
            virtual void SetEvent_OnProgress(std::function<void(int)>) = 0;
            virtual void SetEvent_OnComplete(std::function<void(int)>) = 0;
        };

#ifdef _MAC
        KERNEL_DECL void SetARCEnabled(const bool& enabled);
        KERNEL_DECL bool GetARCEnabled();
#endif

        class CFileTransporter_private;

        class KERNEL_DECL CFileDownloader: public IFileTransporter
        {
        public:
            CFileDownloader(std::wstring sFileUrl, bool bDelete = true);
            virtual ~CFileDownloader();

            void SetFilePath(const std::wstring& sFilePath);
            std::wstring GetFilePath();

            bool IsFileDownloaded();

            void SetFileUrl(const std::wstring &sFileUrl, bool bDelete = true);

            bool DownloadSync();
            void DownloadAsync();

        public:
            virtual void Start(int lPriority);
            virtual int GetPriority();
            virtual void Suspend();
            virtual void Resume();
            virtual void Stop();
            virtual void StopNoJoin();
            virtual void Cancel();
            virtual int IsRunned();
            virtual void SetEvent_OnProgress(std::function<void(int)>);
            virtual void SetEvent_OnComplete(std::function<void(int)>);

        private:
            CFileTransporter_private* m_pInternal;
        };

        class KERNEL_DECL CFileUploader: public IFileTransporter
        {
        public:
            CFileUploader(std::wstring sUrl, const unsigned char* cData, const int nSize);
            CFileUploader(std::wstring sUrl, std::wstring sFilePath);
            virtual ~CFileUploader();

            void SetUrl(const std::wstring& sUrl);
            void SetBinaryData(const unsigned char* data, const int size);
            void SetFilePath(const std::wstring &sFilePath);

            bool UploadSync();
            void UploadAsync();

            std::wstring GetResponse();

        public:
            virtual void Start(int lPriority);
            virtual int GetPriority();
            virtual void Suspend();
            virtual void Resume();
            virtual void Stop();
            virtual void StopNoJoin();
            virtual void Cancel();
            virtual int IsRunned();
            virtual void SetEvent_OnProgress(std::function<void(int)>);
            virtual void SetEvent_OnComplete(std::function<void(int)>);

        private:
            CFileTransporter_private* m_pInternal;
        };
    }
}
