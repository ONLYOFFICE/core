
#pragma once

#include <string>

class CJBig2File 
{
public:
	CJBig2File();
	
	bool MemoryToJBig2(unsigned char* pBufferBGRA ,int BufferSize, int nWidth, int nHeight, std::wstring sDstFileName, bool isBGRA = true);

	bool m_bDuplicateLineRemoval;
	bool m_bPDFMode;
	bool m_bSymbolMode;
	bool m_bRefine;
	bool m_bUpscale2x;
	bool m_bUpscale4x;
	bool m_bSegment;
	double m_dTreshold;
	int m_nBwTreshold;

	std::wstring m_sBaseName;
	std::string m_sOutputTreshold;
};
