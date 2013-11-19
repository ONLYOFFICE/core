#ifndef _FONT_FILE_BASE_H
#define _FONT_FILE_BASE_H

//------------------------------------------------------------------------

typedef void (*FontFileOutputFunc)(void *pStream, char *sData, int nLen);

//------------------------------------------------------------------------
// CFontFileBase
//------------------------------------------------------------------------

class CFontFileBase 
{
public:

  virtual ~CFontFileBase();

protected:

  CFontFileBase(char *sFile, int nLen, BOOL bFreeFileData);

  static char *ReadFile(wchar_t *wsFileName, int *pnFileLen);

  // S = signed / U = unsigned
  // 8/16/32/Var = word length, in bytes
  // BE = big endian
  int          GetS8    (int nPos, BOOL *pbSuccess);
  int          GetU8    (int nPos, BOOL *pbSuccess);
  int          GetS16BE (int nPos, BOOL *pbSuccess);
  int          GetU16BE (int nPos, BOOL *pbSuccess);
  int          GetS32BE (int nPos, BOOL *pbSuccess);
  unsigned int GetU32BE (int nPos, BOOL *pbSuccess);
  unsigned int GetUVarBE(int nPos, int nSize, BOOL *pbSuccess);

  BOOL CheckRegion(int nPos, int nSize);

protected:

  unsigned char *m_sFileData;
  unsigned char *m_sFile;
  int            m_nLen;
  BOOL           m_bFreeFileData;

};

#endif /* _FONT_FILE_BASE_H */
