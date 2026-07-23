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
#include "../../DesktopEditor/common/Types.h"

namespace NSCommon
{
	template <typename Type> 
	class smart_ptr
	{
	protected:
		Type*			m_pData;
		mutable LONG*	m_pCountRef;
		
	public:
		smart_ptr()
		{
			m_pData		= NULL;
			m_pCountRef = NULL;
		}
		smart_ptr(Type* pPointer)
		{
			m_pData		= pPointer;
			m_pCountRef = new LONG(1);
		}
		smart_ptr(const smart_ptr<Type>& pPointer)
		{
			m_pData		= NULL;
			m_pCountRef	= NULL;
			*this = pPointer;
		}
		~smart_ptr()
		{
			Release();
		}

		inline void Release()
		{
			if (NULL == m_pCountRef)
				return;

			*m_pCountRef -= 1;
			if (0 >= *m_pCountRef)
			{
				if (m_pData)
					delete m_pData;
				delete m_pCountRef;
			}
			m_pData		= NULL;
			m_pCountRef	= NULL;
		}
		inline void AddRef()
		{
			if (!IsInit() || (NULL == m_pCountRef))
				return;
			*m_pCountRef += 1;
		}

        inline LONG GetCountReference()
        {
            return m_pCountRef ? *m_pCountRef : 0;
        }

		smart_ptr<Type>& operator=(const Type& oSrc)
		{
			Release();

			m_pData		= new Type(oSrc);
			m_pCountRef = new LONG(1);

			return *this;
		}
		smart_ptr<Type>& operator=(Type* pType)
		{
			Release();
			
			m_pData		= pType;
			m_pCountRef = new LONG(1);

			return *this;
		}
		smart_ptr<Type>& operator=(const smart_ptr<Type>& oSrc)
		{
			Release();

			if ((NULL == oSrc.m_pData) || (NULL == oSrc.m_pCountRef))
				return *this;
			
			*oSrc.m_pCountRef += 1;
			Attach(oSrc.m_pData, oSrc.m_pCountRef);
			return *this;
		}

		inline bool IsInit() const
		{ 
			return (NULL != m_pData); 
		}
		inline bool is_init() const
		{
			return IsInit();
		}

		template<class T> inline const bool is()const
		{
			if (!IsInit())
				return false;
			T* pResult = dynamic_cast<T*>(const_cast<Type*>(m_pData));
			return (NULL != pResult);
		}
		template<class T> inline const T& as()const
		{
			T* pResult = dynamic_cast<T*>(const_cast<Type*>(m_pData));
			return *pResult;
		}
		template<class T> inline T& as()
		{
			T* pResult = dynamic_cast<T*>(const_cast<Type*>(m_pData));
			return *pResult;
		}

		template <typename T>
		inline void Attach(T* pCast, const LONG* pCountRef)
		{
			m_pData		= pCast;
			m_pCountRef	= const_cast<LONG*>(pCountRef);
		}

		template<typename T> 
		inline smart_ptr<T> smart_dynamic_cast()const
		{
			smart_ptr<T> new_type;
			
			if ((NULL == m_pData) || (NULL == m_pCountRef))
				return new_type;
			
			T* pCast = dynamic_cast<T*>(m_pData);

			if (NULL == pCast)
				return new_type;

			*m_pCountRef += 1;

			new_type.Attach(pCast, m_pCountRef);
			
			return new_type;
		}
		inline Type* GetPointer() const { return  m_pData; }

		inline Type& operator*()  { return *m_pData; }
		inline Type* operator->() { return  m_pData; }

		inline const Type& operator*()  const { return *m_pData; }
		inline const Type* operator->() const { return  m_pData; }

		inline const Type& get() { return  *m_pData; } const
		
		inline void reset(Type* pPointer = NULL)
		{
			*this = pPointer;
		}
	};

	template <typename T>
	static inline void normalize_value(T& value, const T& min, const T& max)
	{
		if (value < min)
			value = min;
		else if (value > max)
			value = max;
	}
}

#ifndef _USE_NULLABLE_PROPERTY_
using namespace NSCommon;
#endif
