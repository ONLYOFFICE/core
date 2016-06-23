#ifndef _PDF_READER_FONT_FILE_TYPE1_H
#define _PDF_READER_FONT_FILE_TYPE1_H

#include "FontFileBase.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// CFontFileType1
	//------------------------------------------------------------------------

	class CFontFileType1 : public CFontFileBase
	{
	public:

		static CFontFileType1 *LoadFromBuffer(char *sBuffer, int nLen);
		static CFontFileType1 *LoadFromFile(wchar_t *wsFileName);

		virtual ~CFontFileType1();

		char *GetName();

		char **GetEncoding();

		void WriteEncoded(char **ppNewEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream);

	private:

		CFontFileType1(char *sBuffer, int nLen, bool bFreeData);

		char *GetNextLine(char *sLine);
		void Parse();

	private:

		char  *m_sName;
		char **m_arrEncoding;
		bool   m_bParsed;
	};
}

#endif // _PDF_READER_FONT_FILE_TYPE1_H
