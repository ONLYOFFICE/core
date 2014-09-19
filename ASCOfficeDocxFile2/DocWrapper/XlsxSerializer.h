#ifndef XLSX_SERIALIZER
#define XLSX_SERIALIZER

//todo
#include <atlbase.h>
#include <atlstr.h>

#include <string>

namespace NSBinPptxRW{
	class CDrawingConverter;
}

namespace BinXlsxRW {

	class CXlsxSerializer{
	private:
		CString m_sFontDir;
		CString m_sEmbeddedFontsDir;
		NSBinPptxRW::CDrawingConverter* m_pExternalDrawingConverter;
	public:
		CXlsxSerializer();
		~CXlsxSerializer();
		bool loadFromFile(CString& sSrcFileName, CString& sDstPath, CString& sXMLOptions, CString& sMediaDir);
		bool saveToFile(CString& sSrcFileName, CString& sDstPath, CString& sXMLOptions);
		bool loadChart(CString& sChartPath, unsigned char** ppBinary, long& lDataSize);
		bool saveChart(SAFEARRAY* pBinaryObj, long lStart, long lLength, CString& sFilename, CString& sContentTypePath, CString** sContentTypeElement);
		void setFontDir(CString& sFontDir);
		void setEmbeddedFontsDir(CString& sEmbeddedFontsDir);
		void setDrawingConverter(NSBinPptxRW::CDrawingConverter* pDrawingConverter);
	};
}
#endif	// #ifndef XLSX_SERIALIZER