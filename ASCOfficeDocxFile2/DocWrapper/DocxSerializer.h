#ifndef DOCX_SERIALIZER
#define DOCX_SERIALIZER

#ifdef _WIN32
#include <atlbase.h>
#include <atlstr.h>
#else
#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

namespace Writers{
	class FileWriter;
}
namespace NSBinPptxRW{
	class CBinaryFileReader;
	class CBinaryFileWriter;
}
namespace BinDocxRW {
	class BinaryFileWriter;
	class CDocxSerializer{
	private:
		CString m_sFontDir;
		CString m_sEmbeddedFontsDir;
		bool m_bIsNoBase64Save;
		bool m_bSaveChartAsImg;
		BinaryFileWriter* m_oBinaryFileWriter;
		Writers::FileWriter* m_pCurFileWriter;
	public:
		CDocxSerializer();
		bool ConvertDocxToDoct(const CString& sSrcFileName, const CString& sDstFileName, const CString& sTmpDir, const CString& sXMLOptions);
		bool ConvertDoctToDocx(const CString& sSrcFileName, const CString& sDstFileName, const CString& sTmpDir, const CString& sXMLOptions, const CString& sThemePath, const CString& sMediaPath);
		bool loadFromFile(const CString& sSrcFileName, const CString& sDstPath, const CString& sXMLOptions, const CString& sThemePath, const CString& sMediaPath);
		bool saveToFile(const CString& sSrcFileName, const CString& sDstPath, const CString& sXMLOptions);
		bool getXmlContent(NSBinPptxRW::CBinaryFileReader& oBufferedStream, long lLength, CString& sOutputXml);
		bool getBinaryContent(const CString& bsTxContent, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize);
		void setFontDir(const CString& sFontDir);
		void setEmbeddedFontsDir(const CString& sEmbeddedFontsDir);
		void setIsNoBase64Save(bool bIsNoBase64Save);
		void setSaveChartAsImg(bool bSaveChartAsImg);
	};
}
#endif	// #ifndef DOCX_SERIALIZER