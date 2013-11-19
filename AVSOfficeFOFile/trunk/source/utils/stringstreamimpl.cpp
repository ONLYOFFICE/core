/*!	\file stringstreamimpl.cpp
*	\brief	Реализация интерфейса IStream на основании класса stringstream
*	\author	nikolay.pogorskiy@avsmedia.net
*/

#include "utils\stringstreamimpl.h"
#include <string>
#include <sstream>

namespace AVS
{
	StringStreamImpl::StringStreamImpl()
		: _stringstream(NULL)
	{
		_refcount = 1;
		_stringstream = new std::stringstream();
	}
	StringStreamImpl::~StringStreamImpl()
	{
		if (_stringstream)
			delete _stringstream;
	}
	HRESULT StringStreamImpl::Stat(STATSTG *pStatstg, DWORD grfStatFlag)
	{
		return S_OK;
	}

	HRESULT StringStreamImpl::Seek(LARGE_INTEGER liDistanceToMove, DWORD dwOrigin, ULARGE_INTEGER *lpNewFilePointer)
	{
		return E_NOTIMPL;
	}

	HRESULT StringStreamImpl::Clone(IStream **)
	{
		return E_NOTIMPL;
	}

	HRESULT StringStreamImpl::UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)
	{
		return E_NOTIMPL;
	}

	HRESULT StringStreamImpl::LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)
	{
		return E_NOTIMPL;
	}

	HRESULT StringStreamImpl::Revert(void)
	{
		return E_NOTIMPL;
	}

	HRESULT StringStreamImpl::Commit(DWORD)
	{
		return S_OK;
		//return E_NOTIMPL;   
	}

	HRESULT StringStreamImpl::CopyTo(IStream *, ULARGE_INTEGER, ULARGE_INTEGER *, ULARGE_INTEGER *)
	{
		return E_NOTIMPL;
	}

	HRESULT StringStreamImpl::SetSize(ULARGE_INTEGER)
	{
		return E_NOTIMPL;
	}

	HRESULT StringStreamImpl::GetString(std::string *resultString)
	{
		if (!_stringstream)
			return E_UNEXPECTED;
		if (!resultString)
			return E_POINTER;
		*resultString = _stringstream->str();
		return S_OK;
	}

	HRESULT StringStreamImpl::Write(void const*pv, ULONG cb, ULONG *pcbWritten)
	{
		if (!_stringstream)
			return E_FAIL;
		_stringstream->write((char*)pv, cb);
		if (pcbWritten)
			*pcbWritten = cb;
		return S_OK;
	}

	HRESULT __stdcall StringStreamImpl::QueryInterface(IID const&iid, void **ppvObject)
	{
		if (iid == __uuidof(IUnknown) || iid == __uuidof(IStream) || iid == __uuidof(ISequentialStream))
		{
			*ppvObject = static_cast<IStream*>(this);
			AddRef();
			return S_OK;
		}
		else
			if (iid == __uuidof(IStringStream))
			{
				*ppvObject = static_cast<IStringStream*>(this);
				AddRef();
				return S_OK;
			}
			else
				return E_NOINTERFACE;
	}
	HRESULT __stdcall StringStreamImpl::Read(void *pv, ULONG cb, ULONG *pcbRead)
	{
		return E_NOTIMPL;
	}



}
