#pragma once
#include "Base.h"
#include "../../../../ServerComponents/DesktopEditor/common/Types.h"

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

		AVSINLINE void Release()
		{
			if (!IsInit() || (NULL == m_pCountRef))
				return;

			*m_pCountRef -= 1;
			if (0 >= *m_pCountRef)
			{
				delete m_pData;
				delete m_pCountRef;
			}
			m_pData		= NULL;
			m_pCountRef	= NULL;
		}
		AVSINLINE void AddRef()
		{
			if (!IsInit() || (NULL == m_pCountRef))
				return;
			*m_pCountRef += 1;
		}

	public:
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

	public:

		AVSINLINE bool IsInit() const
		{ 
			return (NULL != m_pData); 
		}
		AVSINLINE bool is_init() const
		{
			return IsInit();
		}

		template<class T> AVSINLINE const bool is()const
		{
			if (!IsInit())
				return false;
			T* pResult = dynamic_cast<T*>(const_cast<Type*>(m_pData));
			return (NULL != pResult);
		}
		template<class T> AVSINLINE const T& as()const
		{
			T* pResult = dynamic_cast<T*>(const_cast<Type*>(m_pData));
			return *pResult;
		}
		template<class T> AVSINLINE T& as()
		{
			T* pResult = dynamic_cast<T*>(const_cast<Type*>(m_pData));
			return *pResult;
		}

		template <typename T>
		AVSINLINE void Attach(T* pCast, const LONG* pCountRef)
		{
			m_pData		= pCast;
			m_pCountRef	= const_cast<LONG*>(pCountRef);
		}

		template<typename T> 
		AVSINLINE smart_ptr<T> smart_dynamic_cast()const
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

		AVSINLINE Type& operator*()  { return *m_pData; }
		AVSINLINE Type* operator->() { return  m_pData; }

		AVSINLINE const Type& operator*()  const { return *m_pData; }
		AVSINLINE const Type* operator->() const { return  m_pData; }

		AVSINLINE const Type& get() { return  *m_pData; } const
		
		AVSINLINE void reset(Type* pPointer = NULL)
		{
			*this = pPointer;
		}
	};

	template <typename T>
	static AVSINLINE void normalize_value(T& value, const T& min, const T& max)
	{
		if (value < min)
			value = min;
		else if (value > max)
			value = max;
	}
}
