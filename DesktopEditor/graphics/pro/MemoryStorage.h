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
#ifndef _GRAPHICS_MEMORY_STORAGE_H_
#define _GRAPHICS_MEMORY_STORAGE_H_

#include "../config.h"
#include "../../common/Types.h"
#include "../../common/StringExt.h"
#include "../../common/File.h"
#include <string>
#include <map>

namespace NSBase
{
	class GRAPHICS_DECL CBaseRefCounter
	{
	protected:
		volatile int m_lRef;

	public:
		CBaseRefCounter();
		virtual ~CBaseRefCounter();

		virtual int AddRef();
		virtual int Release();
	};

	void GRAPHICS_DECL Release(CBaseRefCounter* base);
} // namespace NSBase

namespace NSMemoryStorage
{
	class GRAPHICS_DECL IMemoryStream : public NSBase::CBaseRefCounter
	{
	public:
		IMemoryStream() : NSBase::CBaseRefCounter() {}
		virtual ~IMemoryStream() {}

		virtual int CreateFromFile(const std::wstring& strFileName, unsigned char* pDataUse = NULL) = 0;
		virtual int CreateFromMemory(BYTE* pData, LONG lSize, bool bClear) = 0;
		virtual void GetMemory(BYTE*& pData, LONG& lSize) = 0;
	};

	template<typename TStream>
	class IMemoryStorage : public NSBase::CBaseRefCounter
	{
	public:
		IMemoryStorage() : NSBase::CBaseRefCounter() {}
		virtual ~IMemoryStorage() {}

		virtual bool Add(const std::wstring& id, BYTE* data, LONG size, bool bClear = false) = 0;
		virtual bool Remove(const std::wstring& id) = 0;
		virtual TStream* Get(const std::wstring& id) = 0;
		virtual std::wstring GenerateId() = 0;
		virtual void Clear() = 0;
	};

	template<typename TStream>
	class CDefaultMemoryStorage : public IMemoryStorage<TStream>
	{
	private:
		std::map<std::wstring, TStream*> m_mapStreams;
		int m_nInternalCounter;

	protected:
		virtual TStream* CreateStream() = 0;

	public:
		CDefaultMemoryStorage() : IMemoryStorage<TStream>(), m_nInternalCounter(1) {}
		virtual ~CDefaultMemoryStorage() { Clear(); }

		virtual bool Add(const std::wstring& id, BYTE* data, LONG size, bool bClear = false) override
		{
			std::wstring sFile = id;
			NSStringExt::Replace(sFile, L"\\", L"/");
			if (m_mapStreams.find(sFile) != m_mapStreams.end())
				return false;
			TStream* pStream = CreateStream();
			pStream->CreateFromMemory(data, size, bClear);
			m_mapStreams.insert(std::make_pair(sFile, pStream));
			return true;
		}
		virtual bool Remove(const std::wstring& id) override
		{
			std::wstring sFile = id;
			NSStringExt::Replace(sFile, L"\\", L"/");
			typename std::map<std::wstring, TStream*>::iterator it = m_mapStreams.find(sFile);
			if (it == m_mapStreams.end())
				return false;
			RELEASEINTERFACE(it->second);
			m_mapStreams.erase(it);
			return true;
		}
		virtual void Clear() override
		{
			for (typename std::map<std::wstring, TStream*>::iterator it = m_mapStreams.begin(); it != m_mapStreams.end(); ++it)
				RELEASEINTERFACE(it->second);
			m_mapStreams.clear();
		}
		virtual TStream* Get(const std::wstring& id) override
		{
			std::wstring sFile = id;
			NSStringExt::Replace(sFile, L"\\", L"/");
			typename std::map<std::wstring, TStream*>::iterator it = m_mapStreams.find(sFile);
			return it != m_mapStreams.end() ? it->second : NULL;
		}
		virtual std::wstring GenerateId() override
		{
			std::wstring sKey;
			do
			{
				sKey = L"storage_internal_" + std::to_wstring(m_nInternalCounter++);
			} while (m_mapStreams.find(sKey) != m_mapStreams.end());
			return sKey;
		}
	};

	class CDefaultMemoryStream : public IMemoryStream
	{
	public:
		BYTE* m_pData;
		LONG  m_lSize;
		bool  m_bIsAttach;

	public:
		CDefaultMemoryStream() : IMemoryStream(), m_pData(NULL), m_lSize(0), m_bIsAttach(false) {}
		virtual ~CDefaultMemoryStream()
		{
			if (!m_bIsAttach)
				RELEASEARRAYOBJECTS(m_pData);
		}

		virtual int CreateFromFile(const std::wstring& strFileName, unsigned char* pDataUse = NULL) override
		{
			NSFile::CFileBinary oFile;
			if (!oFile.OpenFile(strFileName))
				return FALSE;

			m_lSize = oFile.GetFileSize();

			if (NULL == pDataUse)
				m_pData = new BYTE[m_lSize];
			else
			{
				m_bIsAttach = true;
				m_pData = pDataUse;
			}

			DWORD dwRead = 0;
			DWORD dwNeedRead = (DWORD)m_lSize;
			oFile.ReadFile(m_pData, dwNeedRead, dwRead);

			if (dwNeedRead != dwRead)
			{
				if (!m_bIsAttach)
					RELEASEARRAYOBJECTS(m_pData);
				m_lSize = 0;
				return FALSE;
			}

			oFile.CloseFile();
			return TRUE;
		}

		virtual int CreateFromMemory(BYTE* pData, LONG lSize, bool bClear) override
		{
			if (bClear)
			{
				m_pData = new BYTE[lSize];
				memcpy(m_pData, pData, lSize);
				m_bIsAttach = false;
			}
			else
			{
				m_pData = pData;
				m_bIsAttach = true;
			}
			m_lSize = lSize;

			if (!m_pData)
			{
				m_lSize = 0;
				return FALSE;
			}
			return TRUE;
		}

		virtual void GetMemory(BYTE*& pData, LONG& lSize) override
		{
			pData = m_pData;
			lSize = m_lSize;
		}
	};

} // namespace NSMemoryStorage

#endif // _GRAPHICS_MEMORY_STORAGE_H_
