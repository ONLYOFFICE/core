#pragma once

#include "../../Common/3dParty/pole/pole.h"

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
			if(m_pStorage)
			{
				delete m_pStorage;
				m_pStorage = NULL;
			}
		}

		bool SetFile (const wchar_t* filename)
		{
			m_pStorage = new POLE::Storage(filename);
			
			if (m_pStorage)//return true;
			{	
				return m_pStorage->open();
			}
			return false;
		}

		bool GetStream (const char *path, POLE::Stream** ppStream)
		{
			if (( m_pStorage != NULL ) && ( path != NULL ))
			{
				*ppStream = //m_pStorage->stream(path);
					 new POLE::Stream(m_pStorage, path);
				//result = m_pStorage->OpenStream (path, NULL, ( STGM_READ | STGM_DIRECT | STGM_SHARE_EXCLUSIVE ), NULL, ppStream );
			}
			if (*ppStream) return true;
			return false;
		}

		inline POLE::Storage* GetStorage()
		{
			return m_pStorage;
		}

	private:

		POLE::Storage* m_pStorage;
	};
}
