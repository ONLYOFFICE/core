#ifndef XLSX_SERIALIZER
#define XLSX_SERIALIZER

//todo
#include <atlbase.h>
#include <atlstr.h>

#include <string>

namespace PPTXFile{
struct IAVSOfficeDrawingConverter;
}

namespace BinXlsxRW {

	class CXlsxSerializer{
	private:
		CString m_sFontDir;
		CString m_sEmbeddedFontsDir;
	public:
		CXlsxSerializer();
		~CXlsxSerializer();
		bool loadFromFile(CString& sSrcFileName, CString& sDstPath, CString& sXMLOptions, CString& sMediaDir);
		bool saveToFile(CString& sSrcFileName, CString& sDstPath, CString& sXMLOptions);
		bool loadChart(CString& sChartPath, unsigned char** ppBinary, long& lDataSize);
		bool saveChart(SAFEARRAY* pBinaryObj, long lStart, long lLength, CString& sFilename, CString& sContentTypePath, CString** sContentTypeElement);
		void setFontDir(CString& sFontDir);
		void setEmbeddedFontsDir(CString& sEmbeddedFontsDir);
		void setDrawingConverter(IUnknown* pDocument);

		//todo
#ifdef _WIN32
		PPTXFile::IAVSOfficeDrawingConverter* m_pExternalDrawingConverter;
		IUnknown* m_pInterface;
		void setComInterface(IUnknown* pInterface);
#endif
	};
}
#endif	// #ifndef XLSX_SERIALIZER