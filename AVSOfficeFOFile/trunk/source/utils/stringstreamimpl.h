/*!	\file	stringstreamimpl.h
*	\brief	Реализация интерфейса IStream на основании класса stringstream
*	\author	nikolay.pogorskiy@avsmedia.net
*/

#pragma once
#include <windows.h>
#include <string>
#include <sstream>

namespace AVS
{

	MIDL_INTERFACE("E275FD1F-9305-451c-8F27-6491388F0FA8")
	IStringStream: public IUnknown
	{
	public:
		virtual HRESULT STDMETHODCALLTYPE GetString(std::string * resultString) = 0;
	};

	class StringStreamImpl : public IStream, public IStringStream
	{
		StringStreamImpl();
		~StringStreamImpl();

	public:
		HRESULT static CreateStream(IStream ** ppStream)
		{
			if (!ppStream)
				return E_POINTER;

			*ppStream = new StringStreamImpl();
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE QueryInterface(IID const&iid, void **ppvObject);

		virtual ULONG STDMETHODCALLTYPE AddRef(void) 
		{ 
			return (ULONG)InterlockedIncrement(&_refcount); 
		}

		virtual ULONG STDMETHODCALLTYPE Release(void) 
		{
			ULONG res = (ULONG) InterlockedDecrement(&_refcount);
			if (res == 0) 
				delete this;
			return res;
		}

		// ISequentialStream Interface
	public:
		virtual HRESULT STDMETHODCALLTYPE Read(void *pv, ULONG cb, ULONG *pcbRead);
		virtual HRESULT STDMETHODCALLTYPE Write(void const*pv, ULONG cb, ULONG *pcbWritten);

		// AVS::IStringStream Interface
	public:
		virtual HRESULT STDMETHODCALLTYPE GetString(std::string *resultString) override;

		// IStream Interface
	public:
		virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER);
		virtual HRESULT STDMETHODCALLTYPE CopyTo(IStream *, ULARGE_INTEGER, ULARGE_INTEGER *, ULARGE_INTEGER *);
		virtual HRESULT STDMETHODCALLTYPE Commit(DWORD);
		virtual HRESULT STDMETHODCALLTYPE Revert(void);
		virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD);
		virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD);
		virtual HRESULT STDMETHODCALLTYPE Clone(IStream **);
		virtual HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER liDistanceToMove, DWORD dwOrigin, ULARGE_INTEGER *lpNewFilePointer);
		virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG *pStatstg, DWORD grfStatFlag);

	private:

		std::stringstream * _stringstream;

		LONG _refcount;
	};


}