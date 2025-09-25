/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
