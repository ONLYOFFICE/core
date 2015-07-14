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
