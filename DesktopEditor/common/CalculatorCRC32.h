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
#ifndef _ASC_COMMON_CALCULATOR_CRC32_
#define _ASC_COMMON_CALCULATOR_CRC32_

#include <string>

const int g_clFilePartSize	= 20*1024;
class CCalculatorCRC32
{	
public:
	CCalculatorCRC32()
	{
        m_dwMagicWord   = 0xEDB88320;
        m_dwInitCrc     = 0xFFFFFFFF;
        m_bInitTable    = FALSE;
	}
public:
    unsigned int Calc(const unsigned char* pStream, int nSize)
	{
		InitCRCTable();
        unsigned int dwRes = m_dwInitCrc;
		for (int i=0;i<nSize;i++)
		{
			dwRes = m_arCRCTable[(dwRes ^ pStream[i])& 0xFF] ^ (dwRes >> 8);
		}

		dwRes = dwRes ^ 0xFFFFFFFF;
		return dwRes;
	}

private:
	void InitCRCTable()
	{
		if (m_bInitTable)
			return;

        unsigned int dwTemp;
		for (int i=0;i<256;i++)
		{
			dwTemp = i;
			for (int j=0;j<8;j++)
			{
				if (0x1==(dwTemp & 0x1))
					dwTemp = (dwTemp >> 1) ^ m_dwMagicWord;
				else
					dwTemp = dwTemp >> 1;
			}
			m_arCRCTable[i] = dwTemp;
		}
	}

    unsigned int m_dwMagicWord;
    unsigned int m_dwInitCrc;
    unsigned int m_arCRCTable[255];
    bool m_bInitTable;
};

#endif // _ASC_COMMON_CALCULATOR_CRC32_
