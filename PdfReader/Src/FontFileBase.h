#ifndef _PDF_READER_FONT_FILE_BASE_H
#define _PDF_READER_FONT_FILE_BASE_H

namespace PdfReader
{
	typedef void(*FontFileOutputFunc)(void *pStream, char *sData, int nLen);

	//------------------------------------------------------------------------
	// CFontFileBase
	//------------------------------------------------------------------------

	class CFontFileBase
	{
	public:

		virtual ~CFontFileBase();

	protected:

		CFontFileBase(char *sFile, int nLen, bool bFreeFileData);

		static char *ReadFile(wchar_t *wsFileName, int *pnFileLen);

		// S = signed / U = unsigned
		// 8/16/32/Var = word length, in bytes
		// BE = big endian
		int          GetS8(int nPos, bool *pbSuccess);
		int          GetU8(int nPos, bool *pbSuccess);
		int          GetS16BE(int nPos, bool *pbSuccess);
		int          GetU16BE(int nPos, bool *pbSuccess);
		int          GetS32BE(int nPos, bool *pbSuccess);
		unsigned int GetU32BE(int nPos, bool *pbSuccess);
		unsigned int GetUVarBE(int nPos, int nSize, bool *pbSuccess);

		bool CheckRegion(int nPos, int nSize);

	protected:

		unsigned char *m_sFileData;
		unsigned char *m_sFile;
		int            m_nLen;
		bool           m_bFreeFileData;

	};
}

#endif // _PDF_READER_FONT_FILE_BASE_H
