#pragma once

namespace AVSDocFileFormat
{
	class Xst : public IOperand
	{
	public:
		Xst () : m_pBuffer(NULL), m_nSize(0)
		{
			m_nSize		=	2;
			m_pBuffer	=	new byte[m_nSize];
		
			if (m_pBuffer)
				memset (m_pBuffer, 0, m_nSize);
		}

		explicit Xst(const wchar_t* wstr) : m_pBuffer(NULL), m_nSize(0)
		{
			if ( wstr == NULL )
			{
				m_nSize = 2;
				m_pBuffer = new byte[m_nSize];
				
				if (m_pBuffer)
					memset(m_pBuffer, 0, m_nSize );
			}
			else
			{
				unsigned short wstrLen = wcslen( wstr );
				m_nSize = ( ( wstrLen * sizeof(wchar_t) ) + sizeof(unsigned short) );
				m_pBuffer = new byte[m_nSize];
				
				if (m_pBuffer)
				{
					memset( m_pBuffer, 0, m_nSize );
					FormatUtils::SetBytes(m_pBuffer, wstrLen );
					memcpy( (m_pBuffer + sizeof(unsigned short) ), wstr, (m_nSize - sizeof(unsigned short) ) );

					for (unsigned int i = 0; i < wstrLen; ++i)
					{
						m_arrXstElements.push_back(wstr[i]);
					}
				}
			}
		}

		explicit Xst(const wchar_t* rgtchar, unsigned short cch) : m_pBuffer(NULL), m_nSize(0)
		{
			if ( ( rgtchar == NULL ) || ( cch == 0 ) )
			{
				m_nSize		=	2;
				m_pBuffer	=	new byte[m_nSize];
				memset(m_pBuffer, 0, m_nSize );
			}
			else
			{
				m_nSize		=	((cch * sizeof(wchar_t)) + sizeof(cch));
				m_pBuffer	=	new byte[m_nSize];
				
				if (m_pBuffer)
				{
					memset(m_pBuffer, 0, m_nSize);
					FormatUtils::SetBytes(m_pBuffer, cch);
					memcpy((m_pBuffer + sizeof(cch)), rgtchar, (m_nSize - sizeof(cch)));

					for (unsigned int i = 0; i < cch; ++i)
					{
						m_arrXstElements.push_back(rgtchar[i]);
					}
				}
			}  
		}

		explicit Xst(bool bNull) : m_pBuffer(NULL), m_nSize(0)
		{
			unsigned short cch	=	1;
			wchar_t rgtchar		=	0;

			m_nSize				=	((cch * sizeof(wchar_t)) + sizeof(cch));
			m_pBuffer			=	new byte[m_nSize];
			if (m_pBuffer)
			{
				memset(m_pBuffer, 0, m_nSize);
				FormatUtils::SetBytes(m_pBuffer, cch);

				//memcpy((m_pBuffer + sizeof(cch)), rgtchar, (m_nSize - sizeof(cch)));

				m_arrXstElements.push_back(rgtchar);
			}
		}

		Xst(const Xst& _xst) : m_pBuffer(NULL), m_nSize(_xst.m_nSize)
		{
			m_pBuffer	=	new byte[m_nSize];
			
			if (m_pBuffer)
			{
				memset(m_pBuffer, 0, m_nSize);
				memcpy(m_pBuffer, _xst.m_pBuffer, m_nSize);
			}
		}

		virtual ~Xst()
		{
			RELEASEARRAYOBJECTS (m_pBuffer);
		}

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
			return m_nSize;
		}

		// 
		inline vector<wchar_t> GetElements() const
		{
			return m_arrXstElements;
		}
	private:

		byte*			m_pBuffer;
		unsigned int	m_nSize;
		vector<wchar_t>	m_arrXstElements;
	};

	class Xstz : public IOperand
	{
	public:
		Xstz() : m_pBuffer(NULL), m_nSize(0)
		{
			this->m_nSize = 4;
			this->m_pBuffer = new byte[this->m_nSize];
			memset( this->m_pBuffer, 0, this->m_nSize );
		}

		explicit Xstz(const Xst& _xst) : m_pBuffer(NULL), m_nSize(_xst.Size() + 2)
		{
			this->m_pBuffer = new byte[this->m_nSize];
			memset( this->m_pBuffer, 0, this->m_nSize );
			memcpy( this->m_pBuffer, (byte*)_xst, _xst.Size() );
		}

		Xstz(const Xstz& _xstz ):
		m_pBuffer(NULL), m_nSize(_xstz.m_nSize)
		{
			this->m_pBuffer = new byte[this->m_nSize];
			memset( this->m_pBuffer, 0, this->m_nSize );
			memcpy( this->m_pBuffer, _xstz.m_pBuffer, this->m_nSize );
		}

		virtual ~Xstz()
		{
			RELEASEARRAYOBJECTS (m_pBuffer);
		}

		virtual operator byte*() const
		{
			return this->m_pBuffer;
		}

		virtual operator const byte*() const
		{
			return (const byte*)this->m_pBuffer;
		}

		virtual unsigned int Size() const
		{
			return this->m_nSize;
		}

	private:

		byte*			m_pBuffer;
		unsigned int	m_nSize;
	};
}