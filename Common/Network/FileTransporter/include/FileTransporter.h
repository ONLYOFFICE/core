/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include <string>
#include <functional>
#include "../../../kernel_config.h"

namespace NSNetwork 
{
	namespace NSFileTransport
	{
		class CSession_private;
		class KERNEL_DECL CSession
		{
		public:
			CSession();
			~CSession();

			void SetProperty(const std::string& name, const std::string& value);
			void Initialize();

		public:
			CSession_private* m_pInternal;
		};

		class KERNEL_DECL IFileTransporter
		{
		public:
			IFileTransporter() {}
			virtual ~IFileTransporter() {}

		public:
			// manager must release. FileTransporter save link only
			virtual void SetSession(CSession* session) = 0;

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

			virtual void SetSession(CSession* session);

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

			virtual void SetSession(CSession* session);

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
