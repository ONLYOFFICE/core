#pragma once

const long g_clFilePartSize	= 20*1024;
class CCalculatorCRC32
{	
public:
	CCalculatorCRC32()
	{
		m_dwMagicWord = 0xEDB88320;
		m_dwInitCrc = 0xFFFFFFFF;	
        m_bInitTable = false;
	}
public:
	DWORD Calc(BYTE const*pStream, int nSize)
	{
		InitCRCTable();
		DWORD dwRes = m_dwInitCrc;
		for (int i=0;i<nSize;i++)
		{
			dwRes = m_arCRCTable[(dwRes ^ pStream[i])& 0xFF] ^ (dwRes >> 8);
		}

		dwRes = dwRes ^ 0xFFFFFFFF;
		return dwRes;
	}

	DWORD Calc(const CString &sStream)
	{
		InitCRCTable();
		DWORD dwRes = m_dwInitCrc;
		int nSize = sStream.GetLength();
		for (int i=0;i<nSize;i++)
		{
			dwRes = m_arCRCTable[(dwRes ^ (BYTE)sStream[i]) & 0xFF] ^ (dwRes >> 8);
		}

		dwRes = dwRes ^ 0xFFFFFFFF;
		return dwRes;
	}
	DWORD CalcPartFile(const CString &sFilepath)
	{
		DWORD dwRet = 0xFFFFFFFF;
		//LPBYTE pBuffer = new BYTE[g_clFilePartSize];
		//if (NULL==pBuffer)
		//	return dwRet;
		//FILE *pFile = fopen(sFilepath, "rb");
		//if (NULL==pFile)
		//{
		//	delete [] pBuffer;
		//	return dwRet;
		//}

		//size_t nReaded = fread(pBuffer, 1, 1024, pFile);
		//fclose(pFile);

		//dwRet = CCalculatorCRC32::Calc(pBuffer, nReaded);
		//
		////ATLTRACE("CRC32: 0x%08X\n", dwRet);
		//delete [] pBuffer;
		return dwRet;
	}
private:
	void InitCRCTable()
	{
		if (m_bInitTable)
			return;

		DWORD dwTemp;
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

	DWORD m_dwMagicWord;
	DWORD m_dwInitCrc;	
	DWORD m_arCRCTable[255];
    bool m_bInitTable;
};
