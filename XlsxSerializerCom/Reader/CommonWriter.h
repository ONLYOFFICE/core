#ifndef COMMON_WRITER
#define COMMON_WRITER

#include "../../ASCOfficePPTXFile/Editor/BinaryFileReaderWriter.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"

namespace BinXlsxRW {
	class BinaryCommonWriter
	{
	public: 
		NSBinPptxRW::CBinaryFileWriter &m_oStream;
		BinaryCommonWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream);
		int WriteItemStart(BYTE type);
		void WriteItemEnd(int nStart);
		int WriteItemWithLengthStart();
		void WriteItemWithLengthEnd(int nStart);
		void WriteColor(const OOX::Spreadsheet::CColor& color, OOX::Spreadsheet::CIndexedColors* pIndexedColors, OOX::CTheme* theme);
		void WriteBytesArray(BYTE* pData, long nDataSize);
	};
}
#endif	// #ifndef COMMON_WRITER