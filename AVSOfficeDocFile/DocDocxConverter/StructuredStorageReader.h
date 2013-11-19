#pragma once

namespace DocFileFormat
{
	class StructuredStorageReader
	{
	public:
		StructuredStorageReader () : m_pStorage(NULL)
		{

		}

		~StructuredStorageReader()
		{
			RELEASEINTERFACE(m_pStorage);
		}

		inline HRESULT SetFile (const WCHAR* filename)
		{
			return StgOpenStorage (filename, NULL, STGM_READ | STGM_TRANSACTED | STGM_SHARE_DENY_NONE, NULL, 0, &m_pStorage);	    
		}

		inline HRESULT GetStream (const OLECHAR *path, IStream** ppStream)
		{
			HRESULT result = S_FALSE;

			if (( m_pStorage != NULL ) && ( path != NULL ))
			{
				result = m_pStorage->OpenStream (path, NULL, ( STGM_READ | STGM_DIRECT | STGM_SHARE_EXCLUSIVE ), NULL, ppStream );
			}

			return result;
		}

		inline IStorage* GetStorage()
		{
			return m_pStorage;
		}

	private:

		IStorage* m_pStorage;
	};
}
