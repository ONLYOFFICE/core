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

#include "../../../../DesktopEditor/common/File.h"
#include "../../../../DesktopEditor/graphics/BaseThread.h"
#include "../include/FileTransporter.h"

namespace NSNetwork
{
    namespace NSFileTransport
    {
        class CFileTransporterBase
        {
        public :
            CFileTransporterBase(const std::wstring &sDownloadFileUrl, bool bDelete)
            {
                m_sDownloadFilePath = L"";
                m_sDownloadFileUrl = sDownloadFileUrl;
                m_sUploadFilePath = L"";
                m_sUploadUrl = L"";

                m_bComplete = false;
                m_bDelete   = bDelete;
                m_eLoadType = DOWNLOADFILE;

                m_cData = NULL;
                m_nSize = 0;

//                m_bIsExit = nullptr;
            }

            CFileTransporterBase(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize)
            {
                m_sDownloadFilePath = L"";
                m_sDownloadFileUrl = L"";
                m_sUploadFilePath = L"";
                m_sUploadUrl = sUploadUrl;

                m_bComplete = false;
                m_bDelete   = true;
                m_eLoadType = UPLOADDATA;

                m_cData = cData;
                m_nSize = nSize;

//                m_bIsExit = nullptr;
            }

            CFileTransporterBase(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath)
            {
                m_sDownloadFilePath = L"";
                m_sDownloadFileUrl = L"";
                m_sUploadFilePath = sUploadFilePath;
                m_sUploadUrl = sUploadUrl;

                m_bComplete = false;
                m_bDelete   = true;
                m_eLoadType = UPLOADFILE;

                m_cData = NULL;
                m_nSize = 0;

//                m_bIsExit = nullptr;
            }

            virtual ~CFileTransporterBase ()
            {
                if ( m_sDownloadFilePath.length() > 0 && m_bDelete )
                {
                    NSFile::CFileBinary::Remove(m_sDownloadFilePath);
                    m_sDownloadFilePath = L"";
                }
//                m_bIsExit = nullptr;
            }

            virtual int DownloadFile() = 0;
            virtual int UploadFile() = 0;
            virtual int UploadData() = 0;

        public:
            std::wstring    m_sDownloadFilePath; // Путь к сохраненному файлу на диске
            std::wstring    m_sDownloadFileUrl;// Ссылка на скачивание файла
            std::wstring    m_sUploadFilePath; // Путь к файлу для выгрузки на сервер
            std::wstring    m_sUploadUrl;      // URL для выгрузки данных

            bool            m_bComplete;       // Закачался файл или нет
            bool            m_bDelete;         // Удалять ли файл в деструкторе

            typedef enum LoadType
            {
                DOWNLOADFILE,
                UPLOADFILE,
                UPLOADDATA
            } LoadType;

            LoadType m_eLoadType;              // Тип загрузки/выгрузки данных/файла

            const unsigned char*  m_cData;     // Данные в сыром виде для выгрузки
            int             m_nSize;           // Размер данных

            std::wstring    m_sResponse = L"";       // Ответ сервера

            std::function<void(int)> m_func_onComplete = nullptr;
            std::function<void(int)> m_func_onProgress = nullptr;
            std::function<bool(void)> m_check_aborted = nullptr;

//            std::atomic<bool>*   m_bIsExit; // Для остановки и выхода потока
        };

        class CFileTransporter_private : public NSThreads::CBaseThread
        {
        protected:
            // создаем в зависимости от платформы
            CFileTransporterBase* m_pInternal;

        public:
            CFileTransporterBase* GetInternal()
            {
                return m_pInternal;
            }

        public:
            CFileTransporter_private(const std::wstring &sDownloadFileUrl, bool bDelete = true);
            CFileTransporter_private(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize);
            CFileTransporter_private(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath);

            virtual ~CFileTransporter_private()
            {
                Stop();
                if (NULL != m_pInternal)
                    delete m_pInternal;
            }

            void SetDownloadFileUrl(const std::wstring &sDownloadFileUrl, bool bDelete = true)
            {
                m_pInternal->m_sDownloadFileUrl = sDownloadFileUrl;
                m_pInternal->m_bDelete = bDelete;
                m_pInternal->m_eLoadType = m_pInternal->DOWNLOADFILE;
            }

            void SetDownloadFilePath(const std::wstring& sDownloadFilePat)
            {
                m_pInternal->m_sDownloadFilePath = sDownloadFilePat;
            }

            std::wstring GetDownloadFilePath()
            {
                return m_pInternal->m_sDownloadFilePath;
            }

            bool IsFileDownloaded()
            {
                return m_pInternal->m_bComplete;
            }

            void SetUploadUrl(const std::wstring &sUploadUrl)
            {
                m_pInternal->m_sUploadUrl = sUploadUrl;
            }

            void SetUploadBinaryDara(const unsigned char* cData, const int nSize)
            {
                m_pInternal->m_cData = cData;
                m_pInternal->m_nSize = nSize;
                m_pInternal->m_eLoadType = m_pInternal->UPLOADDATA;
            }

            void SetUploadFilePath(const std::wstring &sUploadFilePath)
            {
                m_pInternal->m_sUploadFilePath = sUploadFilePath;
                m_pInternal->m_eLoadType = m_pInternal->UPLOADFILE;
            }

            std::wstring& GetResponse()
            {
                return m_pInternal->m_sResponse;
            }

            bool TransferSync()
            {
                this->Start( 1 );
                while ( this->IsRunned() )
                {
                    NSThreads::Sleep( 10 );
                }
                return IsFileDownloaded();
            }

            void TransferAsync()
            {
                this->Start( 1 );
            }

        protected :

            virtual DWORD ThreadProc ()
            {
                m_pInternal->m_bComplete = false;

                int hrResultAll = 0;
                if(m_pInternal->m_eLoadType == m_pInternal->DOWNLOADFILE)
                    hrResultAll = m_pInternal->DownloadFile();
                else if(m_pInternal->m_eLoadType == m_pInternal->UPLOADFILE)
                    hrResultAll = m_pInternal->UploadFile();
                else if(m_pInternal->m_eLoadType == m_pInternal->UPLOADDATA)
                    hrResultAll = m_pInternal->UploadData();

                if (0 == hrResultAll)
                    m_pInternal->m_bComplete = true;
                else
                {
                    if (NSFile::CFileBinary::Exists(m_pInternal->m_sDownloadFilePath))
                        NSFile::CFileBinary::Remove(m_pInternal->m_sDownloadFilePath);
                }

                if (m_pInternal->m_func_onComplete)
                    m_pInternal->m_func_onComplete(hrResultAll);

                m_bRunThread = FALSE;
                return 0;
            }
        };
    }
}
