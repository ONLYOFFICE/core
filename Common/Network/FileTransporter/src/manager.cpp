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

#include <list>
#include "./../include/manager.h"
#include "./../../../../DesktopEditor/graphics/TemporaryCS.h"
#include "./../../../../DesktopEditor/graphics/BaseThread.h"

namespace ASC
{
	class CDownloadTask : public IDownloadTask, public NSThreads::CBaseThread
	{
	public:
		std::wstring Directory;
		std::wstring FileName;
		std::wstring Url;

		DownloadStatus Status;
		int ID;
		void* Observer;
		std::function<void(CDownloadTask*)> Handler;

		bool IsDeleted;

		CDownloadManager_private* m_manager;

	public:
		CDownloadTask(CDownloadManager_private* manager) : IDownloadTask()
		{
			m_manager = manager;

			IsDeleted = false;
			Observer = nullptr;
			Handler = nullptr;
		}
		virtual ~CDownloadTask()
		{
		}

		virtual DWORD ThreadProc();

	public:
		std::wstring GetPath()
		{
			return Directory + L"/" + FileName;
		}
		DownloadStatus GetStatus()
		{
			return Status;
		}
		std::wstring GetUrl()
		{
			return Url;
		}
	};

	class CDownloadManager_private
	{
	public:
		NSCriticalSection::CRITICAL_SECTION m_oCS;

		NSNetwork::NSFileTransport::CSession m_oSession;

		std::list<CDownloadTask*> m_arTasks;
		std::list<CDownloadTask*> m_arWaitTasks;

		int m_nMaxConcurrentDownloadCount;

		int m_nTaskID;
		int m_nPriority;

	public:
		CDownloadManager_private()
		{
			m_oCS.InitializeCriticalSection();
			m_nMaxConcurrentDownloadCount = 5;

			m_nTaskID = 0;
			m_nPriority = 0;
		}
		~CDownloadManager_private()
		{
			Clear();
		}

	public:
		IDownloadTask* Create(const std::wstring& url,
							  const std::wstring& directory,
							  const std::wstring& filename,
							  void* observer,
							  std::function<void(IDownloadTask*)> handler)
		{
			CTemporaryCS oCS(&m_oCS);
			CDownloadTask* pTask = new CDownloadTask(this);

			pTask->Url = url;
			pTask->Directory = directory;
			pTask->FileName = filename;
			pTask->Observer = observer;
			pTask->Handler = handler;
			pTask->ID = m_nTaskID++;

			if (m_nMaxConcurrentDownloadCount > (int)m_arTasks.size())
			{
				m_arTasks.push_back(pTask);
				pTask->DestroyOnFinish();
				pTask->Start(m_nPriority);
			}
			else
			{
				m_arWaitTasks.push_back(pTask);
			}

			return pTask;
		}

		void OnDownloadTask(CDownloadTask* task)
		{
			m_oCS.Enter();

			for (std::list<CDownloadTask*>::iterator iter = m_arTasks.begin(); iter != m_arTasks.end(); iter++)
			{
				if (task == (*iter))
				{
					m_arTasks.erase(iter);
					break;
				}
			}

			if (!m_arWaitTasks.empty())
			{
				std::list<CDownloadTask*>::iterator iter = m_arWaitTasks.begin();
				CDownloadTask* pNewTask = *iter;
				m_arWaitTasks.erase(iter);

				m_arTasks.push_back(pNewTask);
				pNewTask->DestroyOnFinish();
				pNewTask->Start(m_nPriority);
			}

			m_oCS.Leave();

			if (task->Handler)
				task->Handler(task);
		}
		void OnDestroyObserver(void* observer)
		{
			CTemporaryCS oCS(&m_oCS);

			for (std::list<CDownloadTask*>::iterator iter = m_arTasks.begin(); iter != m_arTasks.end(); iter++)
			{
				(*iter)->Handler = nullptr;
			}

			for (std::list<CDownloadTask*>::iterator iter = m_arWaitTasks.begin(); iter != m_arWaitTasks.end();)
			{
				CDownloadTask* pTask = *iter;
				if (pTask->Observer == observer)
				{
					iter = m_arTasks.erase(iter);
					delete pTask;
				}
				else
					iter++;
			}
		}
		void Clear()
		{
			m_oCS.Enter();
			
			for (std::list<CDownloadTask*>::iterator iter = m_arWaitTasks.begin(); iter != m_arWaitTasks.end();)
			{
				CDownloadTask* pTask = *iter;
				delete pTask;
			}
			m_arWaitTasks.clear();

			for (std::list<CDownloadTask*>::iterator iter = m_arTasks.begin(); iter != m_arTasks.end(); iter++)
			{
				(*iter)->Handler = nullptr;
			}

			m_oCS.Leave();

			while (!m_arTasks.empty())
				NSThreads::Sleep(100);
		}
	};

	DWORD CDownloadTask::ThreadProc()
	{
		NSNetwork::NSFileTransport::CFileDownloader oDownloader(Url, false);
		oDownloader.SetSession(&m_manager->m_oSession);

		oDownloader.SetFilePath(GetPath());

		oDownloader.Start( 0 );
		while ( oDownloader.IsRunned() )
		{
			NSThreads::Sleep( 10 );
		}

		Status = oDownloader.IsFileDownloaded() ? ASC::Success : ASC::Error;

		m_manager->OnDownloadTask(this);
		return 0;
	}

	// INTERFACE
	IDownloadTask::IDownloadTask() {}
	IDownloadTask::~IDownloadTask() {}

	CDownloadManager::CDownloadManager()
	{
		m_internal = new CDownloadManager_private();
	}
	CDownloadManager::~CDownloadManager()
	{
		Clear();
		delete m_internal;
	}

	NSNetwork::NSFileTransport::CSession* CDownloadManager::GetSession()
	{
		return &m_internal->m_oSession;
	}

	void CDownloadManager::SetMaxConcurrentDownloadCount(const int& count)
	{
		m_internal->m_nMaxConcurrentDownloadCount = count;
	}
	void CDownloadManager::AddTask(const std::wstring& url,
								   const std::wstring& directory,
								   const std::wstring& filename,
								   void* observer,
								   std::function<void(IDownloadTask*)> handler)
	{
		m_internal->Create(url, directory, filename, observer, handler);
	}
	void CDownloadManager::OnDestroyObserver(void* observer)
	{
		m_internal->OnDestroyObserver(observer);
	}
	void CDownloadManager::Clear()
	{
		m_internal->Clear();
	}

	bool CDownloadManager::DownloadExternal(const std::wstring& url, const std::wstring& path)
	{
		NSNetwork::NSFileTransport::CFileDownloader oDownloader(url, false);
		oDownloader.SetFilePath(path);

		oDownloader.Start( 0 );
		while ( oDownloader.IsRunned() )
		{
			NSThreads::Sleep( 10 );
		}

		return oDownloader.IsFileDownloaded();
	}
}
