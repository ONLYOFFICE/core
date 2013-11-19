#pragma once

#include "CP.h"

namespace AVSDocFileFormat
{
	class EmptyPLCData {};

	template <class D = EmptyPLCData> class PLC : public IOperand
	{
	public:

		PLC() : m_pBuffer(NULL), m_nSizeInBytes(0)
		{

		}

		PLC(const vector<CP>& _aCP, const vector<D>& _aData = vector<D>()) : m_pBuffer(NULL), m_nSizeInBytes(0)
		{
			if ( ( typeid(D) == typeid(EmptyPLCData) ) || ( ( typeid(D) != typeid(EmptyPLCData) ) && ( _aCP.size() == ( _aData.size() + 1 ) ) ) )
			{
				m_nSizeInBytes = ( _aCP.size() * sizeof(CP) );

				for (vector<D>::const_iterator iter = _aData.begin(); iter != _aData.end(); ++iter)
				{
					m_nSizeInBytes += sizeof(D);
				}

				if (m_nSizeInBytes != 0)
				{
					m_pBuffer = new byte[m_nSizeInBytes];

					if (NULL != m_pBuffer)
					{
						unsigned int offset = 0;

						memset(m_pBuffer, 0, m_nSizeInBytes);

						for (vector<CP>::const_iterator iter = _aCP.begin(); iter != _aCP.end(); ++iter)
						{
							unsigned int cp = *iter;
							memcpy((m_pBuffer + offset), &cp, sizeof(cp));
							offset += sizeof(cp);
						}

						for (vector<D>::const_iterator iter = _aData.begin(); iter != _aData.end(); ++iter)
						{
							memcpy( ( m_pBuffer + offset ), &(*iter), sizeof(D) );
							offset += sizeof(D);
						}
					}
				}
			}
		}

		PLC(const PLC& oPLC) : m_pBuffer(NULL), m_nSizeInBytes(oPLC.m_nSizeInBytes)
		{
			m_pBuffer = new byte[m_nSizeInBytes];

			if (NULL != m_pBuffer)
			{
				memset(m_pBuffer, 0, m_nSizeInBytes);
				memcpy(m_pBuffer, oPLC.m_pBuffer, m_nSizeInBytes);
			}
		}

		virtual ~PLC()
		{
			RELEASEARRAYOBJECTS (m_pBuffer);
		}

		//
		bool operator == (const PLC& oPLC)
		{
			return ((m_nSizeInBytes == oPLC.m_nSizeInBytes) && (memcmp(m_pBuffer, oPLC.m_pBuffer, m_nSizeInBytes) == 0));
		}

		bool operator != (const PLC& oPLC)
		{
			return !(this->operator == (oPLC));
		}

		PLC& operator  = (const PLC& oPLC)
		{
			if (*this != oPLC)
			{
				RELEASEARRAYOBJECTS (m_pBuffer);

				m_nSizeInBytes	=	oPLC.m_nSizeInBytes;
				m_pBuffer		=	new byte[m_nSizeInBytes];

				if (NULL != m_pBuffer)
				{
					memcpy(m_pBuffer, oPLC.m_pBuffer, m_nSizeInBytes );
				}  
			}

			return *this;
		}

		//
		virtual operator byte*() const
		{
			return m_pBuffer;
		}

		virtual operator const byte*() const
		{
			return (const byte*)m_pBuffer;
		}

		virtual unsigned int Size() const
		{
			return m_nSizeInBytes;
		}
	private:

		byte*			m_pBuffer;
		unsigned int	m_nSizeInBytes;
	};
}