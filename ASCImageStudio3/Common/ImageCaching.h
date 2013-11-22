#pragma once

#include <atlcoll.h>

namespace ImageStudio
{
	namespace Caching
	{
		/* @desc: */
		// Типы объектов, которые могут храниться в кэше
		const int c_nTypeUnknown = 0;			// неизвестный тип объекта
		const int c_nTypeInterface = 1;			// тип объекта - интерфейс IUnknown*
		const int c_nTypePointer = 2;			// тип объекта - неопределенный указатель на класс, в этом случае пользователь сам контроллирует, что за объект у него в cache
		
		// Типы статусов, которые могут иметь объекты кэша (необходимы для своевременного удаления объектов из кэша)
		const int c_nStatusUnused = 0;			// объект никем не используется и должен быть удален из кэша
		const int c_nStatusUsed = 1;			// объект кем-то используется, и должен быть оставлен в кэше
		/* @desc: */
		
		class CCacheItem
		{
		public:

			CCacheItem()
			{
				m_nType = c_nTypeUnknown;
				m_nStatus = c_nStatusUsed;
			}
			virtual ~CCacheItem()
			{
			}
			
			int GetType() const
			{
				return m_nType;
			}
			int GetStatus()
			{
				return m_nStatus;
			}
			void SetStatus(int nStatus)
			{
				m_nStatus = nStatus;
			}
			
		protected:
			
			int m_nType;
			int m_nStatus;

			/* @desc:
			ImageStudio-Cache-CCacheItem
			{
				int m_nType = c_nTypeUnknown;	// тип объекта (должен выставляться в конструкторе, см. c_nType...)
				int m_nStatus = c_nStatusUsed;	// статус объекта (см. c_nStatus...)
			}
			*/
		};
		class CCacheItemInterface : public CCacheItem
		{
		public:
			
			CCacheItemInterface(IUnknown* pInterface)
			{
				m_nType = c_nTypeInterface;

				m_pInterface = pInterface;

				if (NULL != m_pInterface)
					m_pInterface->AddRef();
			}
			virtual ~CCacheItemInterface()
			{
				if (NULL != m_pInterface)
					m_pInterface->Release();
			}
			
			IUnknown* GetInterface()
			{
				return m_pInterface;
			}
			
		protected:
			
			IUnknown* m_pInterface;
		};
		class CCacheItemPointer : public CCacheItem
		{
		public:
			
			CCacheItemPointer()
			{
				m_nType = c_nTypePointer;
			}
			virtual ~CCacheItemPointer()
			{
			}
		};
		class СCacheManager
		{
		public:

			СCacheManager()
			{
			}
			virtual ~СCacheManager()
			{
				Clear();
			}
			
			void Add(int nID, CCacheItem* pItem)
			{
				Remove(nID);

				m_mapItems.SetAt(nID, pItem);
			}
			void Remove(int nID)
			{
				ATL::CAtlMap<int, CCacheItem*>::CPair* pPair = m_mapItems.Lookup(nID);

				if (NULL == pPair)
					return;

				if (NULL != pPair->m_value)
				{
					delete pPair->m_value;
					pPair->m_value = NULL;
				}

				m_mapItems.RemoveKey(nID);
			}
			void Clear()
			{
				POSITION pos = m_mapItems.GetStartPosition();

				while (NULL != pos)
				{
					CCacheItem* pItem = m_mapItems.GetNextValue(pos);

					if (NULL != pItem)
						delete pItem;
				}

				m_mapItems.RemoveAll();
			}
			void Clear(ATL::CAtlArray<int>& arrIDsToKeep)
			{
				// выставляем всем объектам статусы c_nStatusUnused
				SetItemsStatus(c_nStatusUnused);

				// тем, которые используются в данный момент - выставляем статус c_nStatusUsed
				SetItemsStatus(arrIDsToKeep, c_nStatusUsed);

				// удаляем те, которые все еще имеют статус c_nStatusUnused
				RemoveUnusedItems();
			}
			
			CCacheItem* GetAt(int nID)
			{
				ATL::CAtlMap<int, CCacheItem*>::CPair* pPair = m_mapItems.Lookup(nID);

				if (NULL != pPair)
					return pPair->m_value;

				return NULL;
			}
			CCacheItem* operator[] (int nID)
			{
				return GetAt(nID);
			}
			
		protected:
			
			void SetItemsStatus(int nStatus)
			{
				POSITION pos = m_mapItems.GetStartPosition();

				while (NULL != pos)
				{
					CCacheItem* pItem = m_mapItems.GetNextValue(pos);

					if (NULL != pItem)
						pItem->SetStatus(nStatus);
				}
			}
			void SetItemsStatus(ATL::CAtlArray<int>& arrIDs, int nStatus)
			{
				int nCount = (int)arrIDs.GetCount();
				for (int index = 0; index < nCount; ++index)
				{
					CCacheItem* pItem = GetAt(arrIDs[index]);

					if (NULL == pItem)
						continue;

					pItem->SetStatus(nStatus);
				}
			}
			void RemoveUnusedItems()
			{
				POSITION pos = m_mapItems.GetStartPosition();
				POSITION posOld = NULL;

				while (NULL != pos)
				{
					posOld = pos;

					CCacheItem* pItem = m_mapItems.GetNextValue(pos);

					if (NULL != pItem)
					{
						if (pItem->GetStatus() == c_nStatusUsed)
							continue;

						delete pItem;
					}

					if (NULL != posOld)
						m_mapItems.RemoveAtPos(posOld);
				}
			}
			
		protected:
			
			ATL::CAtlMap<int, CCacheItem*> m_mapItems;
		};
	}
}
