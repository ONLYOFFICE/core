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

namespace ASCDocFileFormat
{
	class Xst : public IOperand
	{
	public:
		Xst () : m_pBuffer(NULL), m_nSize(0)
		{
			m_nSize		=	2;
			m_pBuffer	=	new unsigned char[m_nSize];
		
			if (m_pBuffer)
				memset (m_pBuffer, 0, m_nSize);
		}

		explicit Xst(const wchar_t* wstr) : m_pBuffer(NULL), m_nSize(0)
		{
			if ( wstr == NULL )
			{
				m_nSize = 2;
				m_pBuffer = new unsigned char[m_nSize];
				
				if (m_pBuffer)
					memset(m_pBuffer, 0, m_nSize );
			}
			else
			{
				unsigned short wstrLen = wcslen( wstr );
				m_nSize = ( ( wstrLen * sizeof(wchar_t) ) + sizeof(unsigned short) );
				m_pBuffer = new unsigned char[m_nSize];
				
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
				m_pBuffer	=	new unsigned char[m_nSize];
				memset(m_pBuffer, 0, m_nSize );
			}
			else
			{
				m_nSize		=	((cch * sizeof(wchar_t)) + sizeof(cch));
				m_pBuffer	=	new unsigned char[m_nSize];
				
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
			m_pBuffer			=	new unsigned char[m_nSize];
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
			m_pBuffer	=	new unsigned char[m_nSize];
			
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

		virtual operator unsigned char*() const
		{
			return m_pBuffer;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)m_pBuffer;
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

		unsigned char*			m_pBuffer;
		unsigned int	m_nSize;
		vector<wchar_t>	m_arrXstElements;
	};

	class Xstz : public IOperand
	{
	public:
		Xstz() : m_pBuffer(NULL), m_nSize(0)
		{
			this->m_nSize = 4;
			this->m_pBuffer = new unsigned char[this->m_nSize];
			memset( this->m_pBuffer, 0, this->m_nSize );
		}

		explicit Xstz(const Xst& _xst) : m_pBuffer(NULL), m_nSize(_xst.Size() + 2)
		{
			this->m_pBuffer = new unsigned char[this->m_nSize];
			memset( this->m_pBuffer, 0, this->m_nSize );
			memcpy( this->m_pBuffer, (unsigned char*)_xst, _xst.Size() );
		}

		Xstz(const Xstz& _xstz ):
		m_pBuffer(NULL), m_nSize(_xstz.m_nSize)
		{
			this->m_pBuffer = new unsigned char[this->m_nSize];
			memset( this->m_pBuffer, 0, this->m_nSize );
			memcpy( this->m_pBuffer, _xstz.m_pBuffer, this->m_nSize );
		}

		virtual ~Xstz()
		{
			RELEASEARRAYOBJECTS (m_pBuffer);
		}

		virtual operator unsigned char*() const
		{
			return this->m_pBuffer;
		}

		virtual operator const unsigned char*() const
		{
			return (const unsigned char*)this->m_pBuffer;
		}

		virtual unsigned int Size() const
		{
			return this->m_nSize;
		}

	private:

		unsigned char*			m_pBuffer;
		unsigned int	m_nSize;
	};
}