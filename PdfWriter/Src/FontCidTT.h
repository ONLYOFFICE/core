#ifndef _PDF_WRITER_SRC_FONTCIDTT_H
#define _PDF_WRITER_SRC_FONTCIDTT_H

#include "Font.h"
#include "Objects.h"
#include <string>
#include <map>
#include <vector>

#include "../../DesktopEditor/fontengine/FontManager.h"
#include "../../DesktopEditor/common/File.h"
#include FT_TRUETYPE_TABLES_H

namespace PdfWriter
{
	class CXref;
	class CStream;
	class CFontFileTrueType;
	class CDocument;
	//----------------------------------------------------------------------------------------
	// CFontFileBase
	//----------------------------------------------------------------------------------------
	class CFontCidTrueType : public CFontDict
	{
	public:

		CFontCidTrueType(CXref* pXref, CDocument* pDocument, const std::wstring& wsFontPath, unsigned int unIndex);
		~CFontCidTrueType();
		unsigned char* EncodeString(unsigned int* pUnicodes, unsigned int unLen);
		unsigned int   GetWidth(unsigned short ushCode);
		EFontType      GetFontType()
		{
			return fontCIDType2;
		}

	private:

		void BeforeWrite();
		bool GetWidthsAndGids(unsigned short** ppCodeToGid, unsigned int** pWidths, unsigned char** ppGlyphs, unsigned int& unGlyphsCount);
		void CreateCIDFont2(CDictObject* pFont);
		void WriteToUnicode();
		bool OpenFontFace();
		void CloseFontFace();

	private:

		std::wstring                             m_wsFontPath;
		unsigned int                             m_unFontIndex;

		CFontFileTrueType*                       m_pFontFile;
		CDictObject*                             m_pFontFileDict;
		CStream*                                 m_pCidToGidMapStream;
		CStream*                                 m_pToUnicodeStream;
		CDictObject*                             m_pFont;
		CDictObject*                             m_pFontDescriptor;

		unsigned short                           m_ushCodesCount;   // Количество закодированных символов
		std::map<unsigned int, unsigned short>   m_mUnicodeToCode;  // Мап Юникод->код символа
		std::vector<unsigned int>                m_vUnicodes;       // Обратный мап код символа -> юникод

		std::vector<unsigned short>              m_vCodeToGid;
		std::vector<unsigned int>                m_vWidths;
		std::map<unsigned short, bool>           m_mGlyphs;

		FT_Face                                  m_pFace;
		FT_Byte*                                 m_pFaceMemory;
		int                                      m_nGlyphsCount;
		int                                      m_nSymbolicCmap;

		friend class CDocument;
	};
}

#endif // _PDF_WRITER_SRC_FONTCIDTT_H