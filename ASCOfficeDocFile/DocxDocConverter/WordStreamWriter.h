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

		inline HRESULT Init (POLE::Storage*	pStorage, ULONG nBPos)
		{
			if ( (NULL != pStorage) && (nBPos >= 0) )
			{
				m_nBufPos = nBPos;

				RELEASEOBJECT(m_pBin);

				m_pBin = new POLE::Stream(pStorage, "WordDocument", true);

				if (m_pBin == NULL) return S_FALSE;

				return S_OK;
			}

			return S_FALSE;
		}

		inline void Close ()
		{
			RELEASEOBJECT(m_pBin);
			RELEASEARRAYOBJECTS (m_pMemBuffer);

			m_nBufPos		=	0;
		}

		//
		inline bool Push (const void* pBuffer, ULONG nBufSize)
		{
			if ( (m_pBin != NULL) && (pBuffer != NULL) )
			{
				ULONG nWBSize		=	0;

				m_pBin->seek (m_nBufPos);
				{
					nWBSize = m_pBin->write ((unsigned char*)pBuffer, nBufSize);
					if (nWBSize>=0)
					{
						m_nBufPos	+=	nWBSize;
						return TRUE;
					}
				}
			}

			return FALSE;
		}
		
		inline bool Push (const void* pBuffer, ULONG nBufSize, int nOffSet)
		{
			if ( (m_pBin != NULL) && (pBuffer != NULL) )
			{
				ULONG nWBSize		=	0;

				m_pBin->seek (nOffSet);
				{
					nWBSize = m_pBin->write ((unsigned char*)pBuffer, nBufSize);
					if (nWBSize>=0)
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
				m_pBin->seek (nBufPos);
				{
					int nWBSize = m_pBin->write ((unsigned char*)pBuffer, nBufSize);
					if (nWBSize>=0)
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

		inline POLE::Stream* Get ()
		{
			return m_pBin;
		}

		operator POLE::Stream* ()
		{
			return (POLE::Stream*)m_pBin;
		}

	private:

		POLE::Stream* m_pBin;
		ULONG	 m_nBufPos;	

		unsigned char*	m_pMemBuffer;
	};

	class CSWordWriter : public Singleton<CSWordWriter>, public CWordStreamWriter
	{
	protected:
		CSWordWriter () : CWordStreamWriter() {}
		virtual ~CSWordWriter() {}

		friend class Singleton<CSWordWriter>;
	};
}