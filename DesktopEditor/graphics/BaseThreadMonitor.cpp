/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "./BaseThreadMonitor.h"

namespace NSThreads
{
	CBaseThreadMonitor::CBaseThreadMonitor()
	{
		m_bIsInit = false;
		m_pReceiver = NULL;
		m_funcRelease = nullptr;

		m_oCS.InitializeCriticalSection();
	}

	CBaseThreadMonitor::~CBaseThreadMonitor()
	{
		m_oCS.DeleteCriticalSection();
	}

	CBaseThreadMonitor& CBaseThreadMonitor::Get()
	{
		static CBaseThreadMonitor instance;
		return instance;
	}

	bool CBaseThreadMonitor::Init(void* receiver)
	{
		CTemporaryCS oCS(&m_oCS);

		if (m_bIsInit)
			return false;

		m_bIsInit = true;
		m_pReceiver = receiver;
		return true;
	}

	bool CBaseThreadMonitor::Destroy()
	{
		CTemporaryCS oCS(&m_oCS);

		if (!m_bIsInit)
			return false;

		m_bIsInit = false;
		m_pReceiver = NULL;
		m_listThreads.clear();
		return true;
	}

	bool CBaseThreadMonitor::IsInit()
	{
		CTemporaryCS oCS(&m_oCS);
		return m_bIsInit;
	}

	NSCriticalSection::CRITICAL_SECTION* CBaseThreadMonitor::GetCS()
	{
		return &m_oCS;
	}

	CBaseThread* CBaseThreadMonitor::GetBaseThread(const ASC_THREAD_ID& nThreadId)
	{
		// лист - потому что будет всегда мало записей - и то будет быстрее мапа. двигаем всегда текущий на первое место

		if (m_listThreads.size() == 0)
			return NULL;

		std::list<CBaseThreadInfo>::iterator i = m_listThreads.begin();

		if (i->ID == nThreadId)
			return i->Instance;

		i++;
		while (i != m_listThreads.end())
		{
			if (i->ID == nThreadId)
			{
				CBaseThreadInfo last;
				last.ID = i->ID;
				last.Instance = i->Instance;

				m_listThreads.erase(i);
				m_listThreads.insert(m_listThreads.begin(), last);
				
				return last.Instance;
			}
			i++;
		}

		return NULL;
	}

	void CBaseThreadMonitor::SetReleaseHandler(std::function<void(void* initializer, CBaseThread*)> func)
	{
		CTemporaryCS oCS(&m_oCS);
		m_funcRelease = func;
	}

	void CBaseThreadMonitor::Register(CBaseThread* pInstance)
	{
		CTemporaryCS oCS(&m_oCS);
		if (!m_bIsInit)
			return;
		m_listThreads.push_back({NSThreads::GetCurrentThreadId(), pInstance});
	}

	void CBaseThreadMonitor::Unregister(CBaseThread* pInstance)
	{
		CTemporaryCS oCS(&m_oCS);
		if (!m_bIsInit)
			return;
		for (std::list<CBaseThreadInfo>::iterator i = m_listThreads.begin(); i != m_listThreads.end(); i++)
		{
			if (i->Instance == pInstance)
			{
				m_listThreads.erase(i);
				if (m_funcRelease)
					m_funcRelease(m_pReceiver, pInstance);
				return;
			}
		}
	}
}
