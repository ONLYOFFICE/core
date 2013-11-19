#pragma once

namespace STREAMS
{
	class CWordStreamWriter
	{
	public:
		CWordStreamWriter () : m_pBin (NULL), m_nBufPos (0), m_pMemBuffer (NULL)
		{

		}

		virtual ~CWordStreamWriter ()
		{
			Close ();
		}

		inline HRESULT Init (IStorage*	pStorage, ULONG nBPos)
		{
			if ( (NULL != pStorage) && (nBPos >= 0) )
			{
				m_nBufPos = nBPos;

				RELEASEINTERFACE(m_pBin);

				HRESULT hr = pStorage->OpenStream (L"WordDocument", NULL, STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE, NULL, &m_pBin);
				if (SUCCEEDED(hr))
					return S_OK;

				hr = pStorage->CreateStream (L"WordDocument", STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE, NULL, NULL, &m_pBin);
				if (SUCCEEDED(hr))
					return S_OK;
			}

			return S_FALSE;
		}

		inline void Close ()
		{
			RELEASEINTERFACE(m_pBin);
			RELEASEARRAYOBJECTS (m_pMemBuffer);

			m_nBufPos		=	0;
		}

		//
		inline BOOL Push (const void* pBuffer, ULONG nBufSize)
		{
			if ( (m_pBin != NULL) && (pBuffer != NULL) )
			{
				LARGE_INTEGER oStatus;

				oStatus.HighPart	=	0;
				oStatus.LowPart		=	m_nBufPos;

				ULONG nWBSize		=	0;

				if (SUCCEEDED(m_pBin->Seek (oStatus, STREAM_SEEK_SET, NULL)))
				{
					if (SUCCEEDED(m_pBin->Write (pBuffer, nBufSize, &nWBSize)))
					{
						m_nBufPos	+=	nWBSize;
						return TRUE;
					}
				}
			}

			return FALSE;
		}
		
		inline BOOL Push (const void* pBuffer, ULONG nBufSize, int nOffSet)
		{
			if ( (m_pBin != NULL) && (pBuffer != NULL) )
			{
				LARGE_INTEGER oStatus;

				oStatus.HighPart	=	0;
				oStatus.LowPart		=	nOffSet;

				ULONG nWBSize		=	0;

				if (SUCCEEDED(m_pBin->Seek (oStatus, STREAM_SEEK_SET, NULL)))
				{
					if (SUCCEEDED(m_pBin->Write (pBuffer, nBufSize, &nWBSize)))
					{
						m_nBufPos	+=	nOffSet;
						return TRUE;
					}
				}
			}

			return FALSE;
		}

		inline HRESULT Write (ULONG nBufPos, const void* pBuffer, ULONG nBufSize)
		{
			HRESULT hr	=	S_OK;

			if ((NULL != m_pBin) && (NULL != pBuffer) && (nBufPos >= 0) && (nBufSize >= 0))
			{
				LARGE_INTEGER oStatus;

				oStatus.HighPart	=	0;
				oStatus.LowPart		=	nBufPos;
			
				ULONG nWBSize		=	0;

				if (SUCCEEDED(m_pBin->Seek (oStatus, STREAM_SEEK_SET, NULL)))
				{
					if (SUCCEEDED(m_pBin->Write (pBuffer, nBufSize, &nWBSize)))
					{
						m_nBufPos	+=	nWBSize;
						return S_OK;
					}
				}
			}

			return S_FALSE;
		}

		inline void SetPosition (ULONG nPos)
		{
			m_nBufPos	=	nPos;
		}
		//
		inline ULONG BufferPos ()
		{
			return m_nBufPos;
		}

		inline IStream* Get ()
		{
			return m_pBin;
		}

		operator IStream* ()
		{
			return (IStream*)m_pBin;
		}

	private:

		IStream* m_pBin;
		ULONG	 m_nBufPos;	

		BYTE*	m_pMemBuffer;
	};

	class CSWordWriter : public Singleton<CSWordWriter>, public CWordStreamWriter
	{
	protected:
		CSWordWriter () : CWordStreamWriter() {}
		virtual ~CSWordWriter() {}

		friend class Singleton<CSWordWriter>;
	};
}