/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../../../../../../Common/DocxFormat/Source/Base/Types_32.h"

namespace NSOfficeDrawing
{
	#define BINARY_READER_CHECK_OUT_RANGE(index, count)     \
	if (index > count)										\
	{														\
		index = count;										\
		return 0;											\
	}

	class CBinaryReader
	{
	private:
		BYTE* m_pBuffer;
		DWORD m_lCount;

		DWORD m_lOrigin;

	public:
		CBinaryReader(BYTE* pBuffer, DWORD lCount)
		{
			m_pBuffer = pBuffer;
			m_lCount = lCount;

			m_lOrigin = 0;
		}
		~CBinaryReader()
		{
			m_pBuffer = NULL;
			m_lCount = 0;
		}

	public:
		LONG ReadLONG() // int32 подразумевается
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += 4;

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(_INT32*)(m_pBuffer + lOldOrigin);
		}
		DWORD ReadDWORD()
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += 4;

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(DWORD*)(m_pBuffer + lOldOrigin);
		}
		WORD ReadWORD()
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += 2;

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(WORD*)(m_pBuffer + lOldOrigin);
		}
		SHORT ReadSHORT()
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += 2;

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(short*)(m_pBuffer + lOldOrigin);
		}
		double ReadDOUBLE()
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += sizeof(double);

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(SHORT*)(m_pBuffer + lOldOrigin);
		}
	};
}
