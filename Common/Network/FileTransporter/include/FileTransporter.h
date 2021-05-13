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
            CFileTransporter(std::wstring &sDownloadFileUrl, bool bDelete = true); //download constructor
            CFileTransporter(std::wstring &sUploadPathUrl, unsigned char* cData, const int nSize); //upload constructor
            virtual ~CFileTransporter();

            void SetDownloadFilePath(const std::wstring& sPath);
            std::wstring GetDownloadFilePath();
            bool IsFileDownloaded();
            void SetDownloadProp(std::wstring &sDownloadFileUrl, bool bDelete = true);
            bool DownloadSync();
            void DownloadAsync();

            void SetUploadProp(std::wstring &url, unsigned char* data, const int size);
            bool UploadSync();
            void UploadAsync();

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
    }
}
