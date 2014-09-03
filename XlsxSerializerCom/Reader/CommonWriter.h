#ifndef COMMON_WRITER
#define COMMON_WRITER

#include "../../ASCOfficeDocxFile2/BinWriter/StreamUtils.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"

namespace BinXlsxRW {
	class BinaryCommonWriter
	{
	public: 
		Streams::CBufferedStream &m_oStream;
		BinaryCommonWriter(Streams::CBufferedStream &oCBufferedStream);
		int WriteItemStart(BYTE type);
		void WriteItemEnd(int nStart);
		int WriteItemWithLengthStart();
		void WriteItemWithLengthEnd(int nStart);
		void WriteColor(const OOX::Spreadsheet::CColor& color, OOX::Spreadsheet::CIndexedColors* pIndexedColors, OOX::CTheme* theme);
		void WriteSafeArray(SAFEARRAY* pBinaryObj);
	};
}
#endif	// #ifndef COMMON_WRITER