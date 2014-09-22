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
		bool loadFromFile(CString& sSrcFileName, CString& sDstPath, CString& sXMLOptions, CString& sThemePath, CString& sMediaPath);
		bool saveToFile(CString& sSrcFileName, CString& sDstPath, CString& sXMLOptions);
		bool getXmlContent(NSBinPptxRW::CBinaryFileReader& oBufferedStream, long lLength, CString& sOutputXml);
		bool getBinaryContent(CString& bsTxContent, NSBinPptxRW::CBinaryFileWriter& oBufferedStream, long& lDataSize);
		void setFontDir(CString& sFontDir);
		void setEmbeddedFontsDir(CString& sEmbeddedFontsDir);
		void setIsNoBase64Save(bool bIsNoBase64Save);
		void setSaveChartAsImg(bool bSaveChartAsImg);
	};
}
#endif	// #ifndef DOCX_SERIALIZER