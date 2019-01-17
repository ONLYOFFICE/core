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

#include "CP.h"

namespace ASCDocFileFormat
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
					m_pBuffer = new unsigned char[m_nSizeInBytes];

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
			m_pBuffer = new unsigned char[m_nSizeInBytes];

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
				m_pBuffer		=	new unsigned char[m_nSizeInBytes];

				if (NULL != m_pBuffer)
				{
					memcpy(m_pBuffer, oPLC.m_pBuffer, m_nSizeInBytes );
				}  
			}

			return *this;
		}

		//
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
			return m_nSizeInBytes;
		}
	private:

		unsigned char*			m_pBuffer;
		unsigned int	m_nSizeInBytes;
	};
}