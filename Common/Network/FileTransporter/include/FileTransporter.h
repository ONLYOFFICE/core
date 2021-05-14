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
#include "../../../kernel_config.h"

namespace NSNetwork 
{
    namespace NSFileTransport
    {
        typedef void (*CFileTransporter_OnComplete)(int error);
        // <return> cancel: 1, else 0
        typedef int (*CFileTransporter_OnProgress)(int percent);

        class CFileTransporter_private;
        class KERNEL_DECL CFileTransporter
        {
        protected:
            // создаем в зависимости от платформы
            CFileTransporter_private* m_pInternal;

        #ifdef _MAC
            static bool m_bIsARCEnabled;
        #endif

        public:
            CFileTransporter(const std::wstring &sDownloadFileUrl, bool bDelete = true); //download constructor
            CFileTransporter(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize); //upload data constructor
            CFileTransporter(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath); //upload file constructor
            virtual ~CFileTransporter();

            void SetDownloadFilePath(const std::wstring& sDownloadFilePath);
            std::wstring GetDownloadFilePath();
            bool IsFileDownloaded();
            void SetDownloadFileUrl(const std::wstring &sDownloadFileUrl, bool bDelete = true);
            bool DownloadSync();
            void DownloadAsync();

            void SetUploadUrl(const std::wstring &sUploadUrl);
            void SetUploadBinaryData(const unsigned char* data, const int size);
            void SetUploadFilePath(const std::wstring &sUploadFilePath);
            bool UploadSync();
            void UploadAsync();

        public:
            std::wstring& GetResponse();

            void Start(int lPriority);
            void Suspend();
            void Resume();
            void Stop();

            int IsSuspended();
            int IsRunned();
            int GetError();

            int GetPriority();

            void CheckSuspend();

            //events
            void SetEvent_OnProgress(CFileTransporter_OnProgress);
            void SetEvent_OnComplete(CFileTransporter_OnComplete);

        #ifdef _MAC
            static void SetARCEnabled(const bool& enabled);
            static bool GetARCEnabled();
        #endif
        };

        class KERNEL_DECL CFileDownloader: public CFileTransporter
        {
        public:

            CFileDownloader(std::wstring sDownloadFileUrl, bool bDelete) :
            CFileTransporter(sDownloadFileUrl, bDelete)
            {

            }

            virtual ~CFileDownloader()
            {

            }

            void SetFilePath(const std::wstring& sDownloadFilePath)
            {
                CFileTransporter::SetDownloadFilePath(sDownloadFilePath);
            }

            std::wstring GetFilePath()
            {
            	return CFileTransporter::GetDownloadFilePath();
            }

            bool IsFileDownloaded()
            {
            	return CFileTransporter::IsFileDownloaded();
            }

            void SetDownloadFileUrl(const std::wstring &sDownloadFileUrl, bool bDelete = true)
            { 
            	CFileTransporter::SetDownloadFileUrl(sDownloadFileUrl, bDelete);
            }

            bool DownloadSync()
            {
            	return CFileTransporter::DownloadSync();
            }

            void DownloadAsync()
            {
            	CFileTransporter::DownloadAsync();
            }
        };

        class KERNEL_DECL CFileUploader: public CFileTransporter
        {
        public:

            CFileUploader(std::wstring sUploadUrl, const unsigned char* cData, const int nSize) :
            CFileTransporter(sUploadUrl, cData, nSize)
            {

            }

            CFileUploader(std::wstring sUploadUrl, std::wstring sUploadFilePath) :
            CFileTransporter(sUploadUrl, sUploadFilePath)
            {

            }

            virtual ~CFileUploader()
            {

            }

            void SetUploadUrl(const std::wstring& sUploadUrl)
            {
                CFileTransporter::SetUploadUrl(sUploadUrl);
            }

            std::wstring SetUploadBinaryData(const unsigned char* data, const int size)
            {
            	CFileTransporter::SetUploadBinaryData(data, size);
            }

            void SetUploadFilePath(const std::wstring &sUploadFilePath)
            {
            	CFileTransporter::SetUploadFilePath(sUploadFilePath);
            }

            bool UploadSync()
            {
            	return CFileTransporter::UploadSync();
            }

            void UploadAsync()
            {
            	CFileTransporter::UploadAsync();
            }
        };
    }
}
