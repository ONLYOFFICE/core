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
		// list - because there will always be few records - still faster than map. always move current to first position

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
