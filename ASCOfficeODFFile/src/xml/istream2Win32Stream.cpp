#include "precompiled_cpxml.h"

#include <istream>
#include <sstream>
#include <cpdoccore/CPSharedPtr.h>
#include "istream2Win32Stream.h"


namespace cpdoccore {
namespace xml {

namespace 
{
    std::stringstream emptyStream;
}

class istream2Win32Stream : public IStream
{
public:
    istream2Win32Stream(  shared_ptr< std::istream >::Type istreamPtr ) : m_RefCount(0), istream_(istreamPtr), istreamRef_( (istreamPtr) ? (*istreamPtr) : (emptyStream) )
    {}

    istream2Win32Stream( std::istream & istreamRef ) : m_RefCount(0), istream_(), istreamRef_( istreamRef )
    {}

    virtual ~istream2Win32Stream() {}

// IUnknown 
public:
    HRESULT STDMETHODCALLTYPE QueryInterface(const IID & riid, void ** ppvObject)
    {
        if (NULL == ppvObject)
            return E_POINTER;

        if (0 == memcmp(&riid, &__uuidof(IUnknown), sizeof(IID)))
        {
            *ppvObject = static_cast<IUnknown*>(this);
        }
        else if (0 == memcmp(&riid, &__uuidof(ISequentialStream), sizeof(IID)))
        {
            *ppvObject = static_cast<ISequentialStream*>(this);
        }
        else if (0 == memcmp(&riid, &__uuidof(IStream), sizeof(IID)))
        {
            *ppvObject = static_cast<IStream*>(this);
        }
        else
        {
            *ppvObject = NULL;
            return E_NOINTERFACE;
        }

        AddRef();
        return S_OK;    
    }

    ULONG STDMETHODCALLTYPE AddRef()
    {
        InterlockedIncrement(&m_RefCount);
        return m_RefCount;
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        InterlockedDecrement(&m_RefCount);
        if (0 == m_RefCount)
        {
            delete this;
        }
        return m_RefCount;
    }

// ISequentialStream
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
        /* [length_is][size_is][out] */ void *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG *pcbRead)
    {

        if (istreamRef_.eof())
        {
            if (pcbRead)
                *pcbRead = 0;
            return (0 == cb) ? S_OK : S_FALSE;
        }

        istreamRef_.read( (char*)pv, cb ); 
        const size_t readed = istreamRef_.gcount();
        if (pcbRead)
            *pcbRead = static_cast<ULONG>( readed );
        return (readed == cb) ? S_OK : S_FALSE;
    }

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
        /* [size_is][in] */ const void *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG *pcbWritten)
    {
        _ASSERTE(false);
        return E_NOTIMPL;
    }

// IStream
public:
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
        /* [in] */ LARGE_INTEGER dlibMove,
        /* [in] */ DWORD dwOrigin,
        /* [out] */ ULARGE_INTEGER *plibNewPosition)
    {
       
        std::ios_base::seek_dir seekDir; 
        if (dwOrigin == SEEK_SET)
        {
            seekDir = std::ios_base::beg;        
        }
        else if (dwOrigin == SEEK_CUR)
        {
            seekDir = std::ios_base::cur;        
        }
        else if (dwOrigin == SEEK_END)
        {
            seekDir = std::ios_base::end;
        }
        else
            return E_INVALIDARG;

        if (dlibMove.QuadPart > (::std::numeric_limits< std::istream::pos_type >::max)() ||
            dlibMove.QuadPart < (::std::numeric_limits< std::istream::pos_type >::min)())
        {
            _ASSERTE(false);
            return E_INVALIDARG;
        }

        istreamRef_.seekg( static_cast<std::streamoff>( dlibMove.QuadPart ), seekDir);    

        if (plibNewPosition)
            plibNewPosition->QuadPart = static_cast<ULONGLONG>( istreamRef_.tellg() );

        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE SetSize( 
        /* [in] */ ULARGE_INTEGER libNewSize)
    {
        _ASSERTE(false);
        return E_NOTIMPL;
    }

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo( 
        /* [unique][in] */ IStream *pstm,
        /* [in] */ ULARGE_INTEGER cb,
        /* [out] */ ULARGE_INTEGER *pcbRead,
        /* [out] */ ULARGE_INTEGER *pcbWritten)
    {
        _ASSERTE(false);
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE Commit( 
        /* [in] */ DWORD grfCommitFlags)
    {
        _ASSERTE(false);
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Revert( void)
    {
        _ASSERTE(false);
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE LockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType)
    {
        _ASSERTE(false);
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
        /* [in] */ ULARGE_INTEGER libOffset,
        /* [in] */ ULARGE_INTEGER cb,
        /* [in] */ DWORD dwLockType)
    {
        _ASSERTE(false);
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Stat( 
        /* [out] */ __RPC__out STATSTG *pstatstg,
        /* [in] */ DWORD grfStatFlag)
    {
        _ASSERTE(false);
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE Clone( 
        /* [out] */ __RPC__deref_out_opt IStream **ppstm)
    {
        _ASSERTE(false);
        return E_NOTIMPL;
    }

private:
    LONG m_RefCount;  
    shared_ptr< std::istream >::Type istream_;
    std::istream & istreamRef_;

};

::ATL::CComPtr< IStream > istreamToWin32IStream(shared_ptr< std::istream >::Type istreamPtr)
{
    return ::ATL::CComPtr<IStream>( new istream2Win32Stream(istreamPtr) );
}

::ATL::CComPtr< IStream > istreamToWin32IStream(std::istream & istreamRef)
{
    return ::ATL::CComPtr<IStream>( new istream2Win32Stream(istreamRef) );
}

}
}