#pragma once

#include <atlcoll.h>

#pragma comment ( lib, "Rpcrt4.lib" )


[ object, uuid("FEB10A4D-FEAE-4e94-8DB2-939462D89F68"), dual, helpstring("IMemoryStorage Interface"), pointer_default(unique) ]
__interface IMemoryStorage : IDispatch
{
	[id(100)] HRESULT Add ( [in] BSTR Name, [in] IUnknown* pUnknown );
	[id(105)] HRESULT AddUnique ( [in] IUnknown* pUnknown, [out, retval] BSTR* NameUnique );
	[id(110)] HRESULT Get ( [in] BSTR Name, [out, retval] IUnknown** pUnknown );

	[id(120)] HRESULT RemoveAll ( );
	[id(130)] HRESULT RemoveAt ( [in] BSTR Name );
	
	[id(200)] HRESULT SetAdditionalParam ( [in] BSTR ParamName, [in] VARIANT ParamValue );
	[id(300)] HRESULT GetAdditionalParam ( [in] BSTR ParamName, [out, retval] VARIANT* ParamValue );
};


[ coclass, default(IMemoryStorage),
threading(apartment), vi_progid("IMemoryStorage.MemoryStorage"), progid("MemoryStorage.MemoryStorage.1"),
version(1.0), uuid("E3D8112C-228B-43b9-AA7A-D1C64827C7C4"), helpstring("CMemoryStorage Class") ]

#ifdef  UNICODE
#define RPC_STR RPC_WSTR
#else
#define RPC_STR RPC_CSTR
#endif

class ATL_NO_VTABLE CMemoryStorage : public IMemoryStorage
{
public:

	CMemoryStorage(void)
	{
		ZeroMemory( &m_oCS, sizeof(CRITICAL_SECTION) );
		InitializeCriticalSection( &m_oCS );
	}

	virtual ~CMemoryStorage(void)
	{
		RemoveAll ();

		DeleteCriticalSection( &m_oCS );
	}

public:

	STDMETHOD ( Add ( BSTR Name, IUnknown* pUnknown ) )
	{
		EnterCriticalSection( &m_oCS );

		if ( false == m_MemoryMap.Lookup ( CString ( Name ), pUnknown ) )
			m_MemoryMap.SetAt ( CString ( Name ), pUnknown );

		LeaveCriticalSection( &m_oCS );
		return S_OK;
	}

	STDMETHOD ( AddUnique ( IUnknown* pUnknown, BSTR* NameUnique ) )
	{
		EnterCriticalSection( &m_oCS );

		CString TimeUniqueGuid = GetCurTimeGuid();

		GUID guid;
		if( SUCCEEDED( CoCreateGuid( &guid ) ) )
		{
			TCHAR *guid_str = NULL;
			
			if( RPC_S_OK == UuidToString ( (UUID*)&guid, (RPC_STR*)&guid_str ) )
			{
				CString StrGuid( (TCHAR*)guid_str );
				StrGuid.Replace(_T("-"), _T("_"));

				ATL::CAtlMap<CString, IUnknown*>::CPair* pPair = m_MemoryMap.Lookup ( StrGuid );

				if ( NULL != pPair )
				{
					*NameUnique = TimeUniqueGuid.AllocSysString();
					m_MemoryMap.SetAt( TimeUniqueGuid, pUnknown );

					RpcStringFree( (RPC_STR*)&guid_str );

					LeaveCriticalSection( &m_oCS );
					return S_OK;
				}

				*NameUnique = StrGuid.AllocSysString();
				m_MemoryMap.SetAt( StrGuid, pUnknown );

				RpcStringFree( (RPC_STR*)&guid_str );

				LeaveCriticalSection( &m_oCS );
				return S_OK;
			}
		}

		*NameUnique = TimeUniqueGuid.AllocSysString();
		m_MemoryMap.SetAt( TimeUniqueGuid, pUnknown );

		LeaveCriticalSection( &m_oCS );
		return S_OK; 
	}

	STDMETHOD ( Get ( BSTR Name, IUnknown** pUnknown ) )
	{
		EnterCriticalSection( &m_oCS );

		*pUnknown	=	NULL;

		ATL::CAtlMap<CString, IUnknown*>::CPair* pPair = m_MemoryMap.Lookup ( CString ( Name ) );
		
		if ( NULL != pPair )
			*pUnknown	=	pPair->m_value;

		LeaveCriticalSection( &m_oCS );
		return S_OK;
	}
	
	STDMETHOD ( RemoveAll ( ) )
	{
		EnterCriticalSection( &m_oCS );
		
		m_MemoryMap.RemoveAll ();

		LeaveCriticalSection( &m_oCS );
		return S_OK;
	}

	STDMETHOD ( RemoveAt ( BSTR Name ) )
	{
		EnterCriticalSection( &m_oCS );

		m_MemoryMap.RemoveKey ( CString ( Name ) );

		LeaveCriticalSection( &m_oCS );
		return S_OK;
	}
	
	STDMETHOD ( SetAdditionalParam ( BSTR ParamName, VARIANT ParamValue ) )
	{

		return S_OK;
	}

	STDMETHOD ( GetAdditionalParam ( BSTR ParamName, VARIANT* ParamValue ) )
	{

		return S_OK;
	}

private:

	CString GetCurTimeGuid ()
	{
		SYSTEMTIME oSystemTime;
		::GetSystemTime ( &oSystemTime );

		CString Unique;
		Unique.Format ( _T("_%d_%d_%d_%d_%d_"), oSystemTime.wDayOfWeek, oSystemTime.wHour, 
		oSystemTime.wMinute, oSystemTime.wSecond, oSystemTime.wMilliseconds );

		return Unique;
	}

private:

	CAtlMap <CString, IUnknown*>	m_MemoryMap;
	CRITICAL_SECTION                m_oCS; 

};

#undef RPC_STR