/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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