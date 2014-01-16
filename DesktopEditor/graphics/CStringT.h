// !!! Особенности:
// 1. Если в MFC можно было так писать:
//   CString str1("qwerty");
//   CString str2;
//   str2.Format(TEXT("str1 = %s"), str1);
// То при использовании CStringT надо писать так:
//   CStringT str1("qwerty");
//   CStringT str2;
//   str2.Format(TEXT("str1 = %s"), (LPCTSTR)str1);


#ifndef __FILE__CSTRINGT__
#define __FILE__CSTRINGT__

#include <string>

namespace NSStrings
{

template <typename _T, typename _TCHAR>
class CStringT
{
protected:
	_T m_str;

public:
	CStringT() {}
	CStringT(const CStringT& src) { m_str = src.m_str; }

	CStringT(const char& ch) { _TCHAR c = (_TCHAR)ch; m_src = c; }
	CStringT(const wchar_t& ch) { _TCHAR c = (_TCHAR)ch; m_src = c; }

	// get data length
	int GetLength() const {return m_str.length();}
	// TRUE if zero length
	BOOL IsEmpty() const {return m_str.empty();}
	// clear contents to empty
	void Empty() {m_str.erase();}

	// return single character at zero-based index
	_TCHAR GetAt(int nIndex) const {return m_str[nIndex];}
	// return single character at zero-based index
	_TCHAR operator[](int nIndex) const {return m_str[nIndex];}
	// set a single character at zero-based index
	void SetAt(int nIndex, _TCHAR ch) {m_str[nIndex] = ch;}
	// return pointer to const string
	operator _TCHAR*() const {return m_str.c_str();}

	// ref-counted copy from another CStringT
	const CStringT& operator=(const CStringT& stringSrc) { if (&stringSrc != this) {m_str = (_TCHAR*)stringSrc;} return *this;}
	// set string content to single character
	const CStringT& operator=(wchar_t ch) {_TCHAR c = (_TCHAR)ch;  m_str = c; return *this;}
	const CStringT& operator=(char ch) {_TCHAR c = (_TCHAR)ch; m_str = c; return *this;}

   // concatenate from another CStringT
   const CStringT& operator+=(const CStringT& string) {m_str += string.m_str; return *this;}

   // concatenate a single character
   const CStringT& operator+=(char ch) {m_str += (_TCHAR)ch; return *this;}
   const CStringT& operator+=(wchar_t ch) {m_str += (_TCHAR)ch; return *this;}

   friend CStringT operator+(const CStringT& string1, const CStringT& string2) {CStringT str(string1); return (str += string2);}
   friend CStringT operator+(const CStringT& string, char ch) {CStringT str(string); return (str += ch);}
   friend CStringT operator+(char ch, const CStringT& string) {CStringT str(ch); return (str += string);}
   friend CStringT operator+(const CStringT& string, wchar_t ch) {CStringT str(string); return (str += ch);}
   friend CStringT operator+(wchar_t ch, const CStringT& string) {CStringT str(ch); return (str += string);}

   // string comparison
   // straight character comparison
   int Compare      (LPCTSTR lpsz) const {return _tcscmp  (this->operator LPCTSTR(), lpsz);}
   // compare ignoring case
   int CompareNoCase(LPCTSTR lpsz) const {return _tcsicmp (this->operator LPCTSTR(), lpsz);}
   // NLS aware comparison, case sensitive
   int Collate      (LPCTSTR lpsz) const {return _tcscoll (this->operator LPCTSTR(), lpsz);}
   // NLS aware comparison, case insensitive
   int CollateNoCase(LPCTSTR lpsz) const {return _tcsicoll(this->operator LPCTSTR(), lpsz);}

   // simple sub-string extraction

   // return nCount characters starting at zero-based nFirst
   CStringT Mid(int nFirst, int nCount) const { CStringT s; s.m_str = m_str.substr(); }
   // return all characters starting at zero-based nFirst
   CStringT Mid(int nFirst) const {return CStringKS(this->operator LPCTSTR()+max(0,nFirst));}
   // return first nCount characters in string
   CStringKS Left(int nCount) const {CStringKS str; for (int i=0; i<nCount; i++) str += GetAt(i); return str;}
   // return nCount characters from end of string
   CStringKS Right(int nCount) const {return Mid(GetLength()-max(0,nCount));}

   //  characters from beginning that are also in passed string
   CStringKS SpanIncluding(LPCTSTR lpszCharSet) const {CStringKS str; for (int i=0; i<GetLength(); i++) if(_tcschr(lpszCharSet, GetAt(i)) != NULL) str += GetAt(i); else break; return str;}
   // characters from beginning that are not also in passed string
   CStringKS SpanExcluding(LPCTSTR lpszCharSet) const {CStringKS str; for (int i=0; i<GetLength(); i++) if(_tcschr(lpszCharSet, GetAt(i)) == NULL) str += GetAt(i); else break; return str;}

   // upper/lower/reverse conversion

   // NLS aware conversion to uppercase
   void MakeUpper() {
      //TCHAR *szStr = new TCHAR[1+GetLength()];
      //_tcscpy(szStr, this->operator LPCTSTR());
      ::CharUpper((LPTSTR)this->operator LPCTSTR());//::CharUpper(szStr);
      //m_str = szStr;
      //delete szStr;
   }
   // NLS aware conversion to lowercase
   void MakeLower() {
      //TCHAR *szStr = new TCHAR[1+GetLength()];
      //_tcscpy(szStr, this->operator LPCTSTR());
      ::CharLower((LPTSTR)this->operator LPCTSTR());//::CharLower(szStr);
      //m_str = szStr;
      //delete szStr;
   }
   // reverse string right-to-left
   void MakeReverse() {
      int len = GetLength();
      for (int i=0; i<len/2; i++) {
         TCHAR ch = GetAt(i);
         SetAt(i, GetAt(len-i-1));
         SetAt(len-i-1, ch);
      }
   }

   // trimming whitespace (either side)

   // remove whitespace starting from right edge
   void TrimRight() {TrimRight(TEXT(' '));}
   // remove whitespace starting from left side
   void TrimLeft () {TrimLeft (TEXT(' '));}

   // trimming anything (either side)

   // remove continuous occurrences of chTarget starting from right
   void TrimRight(TCHAR chTarget) {
      for (int i=GetLength()-1; i>=0; i--) {
         if (GetAt(i) != chTarget) {
            operator =(Left(i+1));
            break;
         }
      }
   }
   // remove continuous occcurrences of characters in passed string,
   // starting from right
   void TrimRight(LPCTSTR lpszTargets) {
      int pos = -1;
      CStringKS strTargets(lpszTargets);
      for (int i=GetLength()-1; i>=0; i--) {
         if (strTargets.Find(GetAt(i)) != -1) {
            pos = i;
         } else {
            break;
         }
      }
      if (pos >= 0) {
         operator =(Left(pos));
      }
   }
   // remove continuous occurrences of chTarget starting from left
   void TrimLeft(TCHAR chTarget) {
      for (int i=0; i<GetLength(); i++) {
         if (GetAt(i) != chTarget) {
            operator =(Mid(i));
            break;
         }
      }
   }
   // remove continuous occcurrences of characters in
   // passed string, starting from left
   void TrimLeft(LPCTSTR lpszTargets) {
      int pos = -1;
      CStringKS strTargets(lpszTargets);
      for (int i=0; i<GetLength(); i++) {
         if (strTargets.Find(GetAt(i)) != -1) {
            pos = i;
         } else {
            break;
         }
      }
      if (pos >= 0) {
         operator =(Mid(i));
      }
   }

   // advanced manipulation

   // replace occurrences of chOld with chNew
   int Replace(TCHAR chOld, TCHAR chNew) {
      int count = 0;
      for (int i=0; i<GetLength(); i++) {
         if (GetAt(i) == chOld) {
            SetAt(i, chNew);
            count++;
         }
      }
      return count;
   }

   // replace occurrences of substring lpszOld with lpszNew;
   // empty lpszNew removes instances of lpszOld
   int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew) {
      int count = 0;
      if (lpszOld) {
         int pos = 0;
         size_t len = _tcslen(lpszOld);
         do {
            pos = Find(lpszOld, pos);
            if (pos == -1) break;
            count++;
            if (lpszNew) {
               m_str.replace(pos, len, lpszNew);
            } else {
               m_str.erase(pos, len);
            }
         } while (true);
      }
      return count;
   }
   // remove occurrences of chRemove
   int Remove(TCHAR chRemove) {
      int count = 0;
      int i=0;
      while (i<GetLength()) {
         if (GetAt(i) == chRemove) {
            Delete(i);
            count++;
         } else {
            i++;
         }
      }
      return count;
   }
   // insert character at zero-based index; concatenates
   // if index is past end of string
   int Insert(int nIndex, TCHAR ch) {
      m_str.insert(max(0, min(nIndex, GetLength())), 1, ch);
      return GetLength();
   }
   // insert substring at zero-based index; concatenates
   // if index is past end of string
   int Insert(int nIndex, LPCTSTR pstr) {
      if (pstr) {
         m_str.insert(max(0, min(nIndex, GetLength())), pstr);
      }
      return GetLength();
   }
   // delete nCount characters starting at zero-based index
   int Delete(int nIndex, int nCount = 1) {
      const int iRes = GetLength();
      if (nCount > 0) {
         nIndex = max(0, nIndex);
         nCount = min(nCount, GetLength()-nIndex);
         m_str.erase(nIndex, nCount);
      }
      return iRes;
   }

   // searching

   // find character starting at left, -1 if not found
   int Find(TCHAR ch) const {return m_str.find(ch);}
   // find character starting at right
   int ReverseFind(TCHAR ch) const {return m_str.rfind(ch);}
   // find character starting at zero-based index and going right
   int Find(TCHAR ch, int nStart) const {return m_str.find(ch, nStart);}
   // find first instance of any character in passed string
   int FindOneOf(LPCTSTR lpszCharSet) const {return m_str.find_first_of(lpszCharSet);}
   // find first instance of substring
   int Find(LPCTSTR lpszSub) const {return m_str.find(lpszSub);}
   // find first instance of substring starting at zero-based index
   int Find(LPCTSTR lpszSub, int nStart) const {return m_str.find(lpszSub, nStart);}

   // simple formatting

   // printf-like formatting using passed string
   void Format(LPCTSTR lpszFormat, ...) {
      va_list arglist;
      va_start(arglist, lpszFormat);
      FormatV(lpszFormat, arglist);
      va_end(arglist);
   }
   // printf-like formatting using referenced string resource
   void Format(UINT nFormatID, ...) {
      va_list arglist;
      va_start(arglist, nFormatID);
      CStringKS strLoad;
      BOOL bRes = strLoad.LoadString(nFormatID);
      if (bRes) {
         FormatV(strLoad, arglist);
      } else {
         Empty();
      }
      va_end(arglist);
   }
   // printf-like formatting using variable arguments parameter
   void FormatV(LPCTSTR lpszFormat, va_list argList) {
      size_t sizeBuff = 512;
      TCHAR *szFormat = NULL;
      int write = -1;
      do {
         szFormat = new TCHAR[sizeBuff*=2];
         write = _vsntprintf(szFormat, sizeBuff-1, lpszFormat, argList);
         if (write >= 0) {
            m_str = szFormat;
         }
         delete [] szFormat;
      } while (write < 0);
   }

   // formatting for localization (uses FormatMessage API)

   // format using FormatMessage API on passed string
   void FormatMessage(LPCTSTR lpszFormat, ...) {
      va_list argList;
      va_start(argList, lpszFormat);
      LPTSTR lpszTemp;
      if ((::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,lpszFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0) &&
          (lpszTemp == NULL))
      {
         *this = lpszTemp;
         LocalFree(lpszTemp);
      }
      va_end(argList);
   }
   // format using FormatMessage API on referenced string resource
   void FormatMessage(UINT nFormatID, ...) {
      CStringKS strFormat;
      if (strFormat.LoadString(nFormatID)) {
         va_list argList;
         va_start(argList, nFormatID);
         LPTSTR lpszTemp;
         if ((::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER, strFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0) &&
             (lpszTemp == NULL))
         {
            *this = lpszTemp;
            LocalFree(lpszTemp);
         }
         va_end(argList);
      }
   }

   // input and output
   //friend CArchive& operator<<(CArchive& ar, const CStringKS& string);
   //friend CArchive& operator>>(CArchive& ar, CStringKS& string);

   // load from string resource
   BOOL LoadString(UINT nID) {
      size_t sizeBuff = 512;
      bool bRepeat;
      int iRes = 0;
      TCHAR *szFormat = NULL;
      do {
         szFormat = new TCHAR[sizeBuff*=2];
         iRes = ::LoadString(::GetModuleHandle(NULL), nID, szFormat, sizeBuff);
         bRepeat = (iRes+1 >= sizeBuff);
         if (!bRepeat)
            m_str = szFormat;
         delete [] szFormat;
      } while (bRepeat);
      return (iRes > 0);
   }

#ifndef _UNICODE
   // ANSI <-> OEM support (convert string in place)

   // convert string from ANSI to OEM in-place
   void AnsiToOem() {::CharToOem((LPCTSTR)this->operator LPCTSTR(), (LPSTR) this->operator LPCTSTR());}
   // convert string from OEM to ANSI in-place
   void OemToAnsi() {::OemToChar((LPCSTR) this->operator LPCTSTR(), (LPTSTR)this->operator LPCTSTR());}
#endif

#ifndef _AFX_NO_BSTR_SUPPORT
   // OLE BSTR support (use for OLE automation)

   // return a BSTR initialized with this CStringKS's data
   BSTR AllocSysString() const {USES_CONVERSION; return SysAllocString(T2COLE(m_str.c_str()));}
   // reallocates the passed BSTR, copies content of this CStringKS to it
   BSTR SetSysString(BSTR* pbstr) const {BSTR szRes = AllocSysString(); if (pbstr) {::SysFreeString(*pbstr); *pbstr=szRes;} return szRes;}
#endif

   // Access to string implementation buffer as "C" character array

   // get pointer to modifiable buffer at least as long as nMinBufLength
   LPTSTR GetBuffer(int nMinBufLength) {
      if (GetLength() < nMinBufLength) {
         m_str.resize(nMinBufLength);
      }
      return (LPTSTR)this->operator LPCTSTR();
   }
   // release buffer, setting length to nNewLength (or to first nul if -1)
   void ReleaseBuffer(int nNewLength = -1) {
      if (nNewLength != -1) {
         nNewLength = max(0, nNewLength);
         m_str.resize(nNewLength);
      }
      FreeExtra();
   }
   // get pointer to modifiable buffer exactly as long as nNewLength
   LPTSTR GetBufferSetLength(int nNewLength) { return GetBuffer(nNewLength);}
   // release memory allocated to but unused by string
   void FreeExtra() {
      int pos = Find(TEXT('\0'));
      if (pos != -1) {
         m_str.resize(pos);
      }
   }

};

class CStringA
{

};

class CStringKS {
private:
   #ifdef UNICODE
      typedef std::wstring tstring;
   #else
      typedef std:: string tstring;
   #endif // UNICODE
   tstring m_str;
public:

// Constructors

   // constructs empty CStringKS
   CStringKS() {}
   // copy constructor
   CStringKS(const CStringKS& stringSrc) : m_str((LPCTSTR)stringSrc) {}
   // from a single character
   CStringKS(TCHAR ch, int nRepeat = 1) : m_str(tstring(nRepeat, ch)) {}
   // from an ANSI string (converts to TCHAR)
   CStringKS(LPCSTR  lpsz) {if (lpsz!=NULL) {USES_CONVERSION; m_str = A2CT(lpsz);}}
   // from a UNICODE string (converts to TCHAR)
   CStringKS(LPCWSTR lpsz) {if (lpsz!=NULL) {USES_CONVERSION; m_str = W2CT(lpsz);}}
   // subset of characters from an ANSI string (converts to TCHAR)
   CStringKS(LPCSTR lpch, int nLength)  : m_str((LPCTSTR)(CStringKS(lpch).Left(nLength))) {}
   // subset of characters from a UNICODE string (converts to TCHAR)
   CStringKS(LPCWSTR lpch, int nLength) : m_str((LPCTSTR)(CStringKS(lpch).Left(nLength))) {}
   // from unsigned characters
   CStringKS(const unsigned char* psz) {this->operator=(psz);}

// Attributes & Operations

   // get data length
   int GetLength() const {return m_str.length();}
   // TRUE if zero length
   BOOL IsEmpty() const {return m_str.empty();}
   // clear contents to empty
   void Empty() {m_str.erase();}

   // return single character at zero-based index
   TCHAR GetAt(int nIndex) const {return m_str[nIndex];}
   // return single character at zero-based index
   TCHAR operator[](int nIndex) const {return m_str[nIndex];}
   // set a single character at zero-based index
   void SetAt(int nIndex, TCHAR ch) {m_str[nIndex] = ch;}
   // return pointer to const string
   operator LPCTSTR() const {return m_str.c_str();}

   // overloaded assignment

   // ref-counted copy from another CStringKS
   const CStringKS& operator=(const CStringKS& stringSrc) { if (&stringSrc != this) {m_str = (LPCTSTR)stringSrc;} return *this;}
   // set string content to single character
   const CStringKS& operator=(TCHAR ch) {m_str = ch; return *this;}
#ifdef _UNICODE
   const CStringKS& operator=(char ch) {TCHAR tCh = ch; m_str = tCh; return *this;}
#endif
   // copy string content from ANSI string (converts to TCHAR)
   const CStringKS& operator=(LPCSTR  lpsz) {m_str = (LPCTSTR)CStringKS(lpsz); return *this;}
   // copy string content from UNICODE string (converts to TCHAR)
   const CStringKS& operator=(LPCWSTR lpsz) {m_str = (LPCTSTR)CStringKS(lpsz); return *this;}
   // copy string content from unsigned chars
   const CStringKS& operator=(const unsigned char* psz) {USES_CONVERSION; m_str = A2CT((const char*)psz); return *this;}

   // string concatenation

   // concatenate from another CStringKS
   const CStringKS& operator+=(const CStringKS& string) {m_str += (LPCTSTR)string; return *this;}

   // concatenate a single character
   const CStringKS& operator+=(TCHAR ch) {m_str += ch; return *this;}
#ifdef _UNICODE
   // concatenate an ANSI character after converting it to TCHAR
   const CStringKS& operator+=(char ch) {TCHAR tCh = ch; m_str += tCh; return *this;}
#endif
   // concatenate a UNICODE character after converting it to TCHAR
   const CStringKS& operator+=(LPCTSTR lpsz) {m_str += lpsz; return *this;}

   friend CStringKS operator+(const CStringKS& string1, const CStringKS& string2) {CStringKS str(string1); return (str += string2);}
   friend CStringKS operator+(const CStringKS& string, TCHAR ch) {CStringKS str(string); return (str += ch    );}
   friend CStringKS operator+(TCHAR ch, const CStringKS& string) {CStringKS str(ch    ); return (str += string);}
#ifdef _UNICODE
   friend CStringKS operator+(const CStringKS& string, char ch) {CStringKS str(string); TCHAR tCh = ch; return (str += tCh);}
   friend CStringKS operator+(char ch, const CStringKS& string) {TCHAR tCh = ch; CStringKS str(tCh); return (str += string);}
#endif
   friend CStringKS operator+(const CStringKS& string, LPCTSTR lpsz) {CStringKS str(string); return (str += lpsz  );}
   friend CStringKS operator+(LPCTSTR lpsz, const CStringKS& string) {CStringKS str(lpsz  ); return (str += string);}

   // string comparison

   // straight character comparison
   int Compare      (LPCTSTR lpsz) const {return _tcscmp  (this->operator LPCTSTR(), lpsz);}
   // compare ignoring case
   int CompareNoCase(LPCTSTR lpsz) const {return _tcsicmp (this->operator LPCTSTR(), lpsz);}
   // NLS aware comparison, case sensitive
   int Collate      (LPCTSTR lpsz) const {return _tcscoll (this->operator LPCTSTR(), lpsz);}
   // NLS aware comparison, case insensitive
   int CollateNoCase(LPCTSTR lpsz) const {return _tcsicoll(this->operator LPCTSTR(), lpsz);}

   // simple sub-string extraction

   // return nCount characters starting at zero-based nFirst
   CStringKS Mid(int nFirst, int nCount) const {return Mid(nFirst).Left(nCount);}
   // return all characters starting at zero-based nFirst
   CStringKS Mid(int nFirst) const {return CStringKS(this->operator LPCTSTR()+max(0,nFirst));}
   // return first nCount characters in string
   CStringKS Left(int nCount) const {CStringKS str; for (int i=0; i<nCount; i++) str += GetAt(i); return str;}
   // return nCount characters from end of string
   CStringKS Right(int nCount) const {return Mid(GetLength()-max(0,nCount));}

   //  characters from beginning that are also in passed string
   CStringKS SpanIncluding(LPCTSTR lpszCharSet) const {CStringKS str; for (int i=0; i<GetLength(); i++) if(_tcschr(lpszCharSet, GetAt(i)) != NULL) str += GetAt(i); else break; return str;}
   // characters from beginning that are not also in passed string
   CStringKS SpanExcluding(LPCTSTR lpszCharSet) const {CStringKS str; for (int i=0; i<GetLength(); i++) if(_tcschr(lpszCharSet, GetAt(i)) == NULL) str += GetAt(i); else break; return str;}

   // upper/lower/reverse conversion

   // NLS aware conversion to uppercase
   void MakeUpper() {
      //TCHAR *szStr = new TCHAR[1+GetLength()];
      //_tcscpy(szStr, this->operator LPCTSTR());
      ::CharUpper((LPTSTR)this->operator LPCTSTR());//::CharUpper(szStr);
      //m_str = szStr;
      //delete szStr;
   }
   // NLS aware conversion to lowercase
   void MakeLower() {
      //TCHAR *szStr = new TCHAR[1+GetLength()];
      //_tcscpy(szStr, this->operator LPCTSTR());
      ::CharLower((LPTSTR)this->operator LPCTSTR());//::CharLower(szStr);
      //m_str = szStr;
      //delete szStr;
   }
   // reverse string right-to-left
   void MakeReverse() {
      int len = GetLength();
      for (int i=0; i<len/2; i++) {
         TCHAR ch = GetAt(i);
         SetAt(i, GetAt(len-i-1));
         SetAt(len-i-1, ch);
      }
   }

   // trimming whitespace (either side)

   // remove whitespace starting from right edge
   void TrimRight() {TrimRight(TEXT(' '));}
   // remove whitespace starting from left side
   void TrimLeft () {TrimLeft (TEXT(' '));}

   // trimming anything (either side)

   // remove continuous occurrences of chTarget starting from right
   void TrimRight(TCHAR chTarget) {
      for (int i=GetLength()-1; i>=0; i--) {
         if (GetAt(i) != chTarget) {
            operator =(Left(i+1));
            break;
         }
      }
   }
   // remove continuous occcurrences of characters in passed string,
   // starting from right
   void TrimRight(LPCTSTR lpszTargets) {
      int pos = -1;
      CStringKS strTargets(lpszTargets);
      for (int i=GetLength()-1; i>=0; i--) {
         if (strTargets.Find(GetAt(i)) != -1) {
            pos = i;
         } else {
            break;
         }
      }
      if (pos >= 0) {
         operator =(Left(pos));
      }
   }
   // remove continuous occurrences of chTarget starting from left
   void TrimLeft(TCHAR chTarget) {
      for (int i=0; i<GetLength(); i++) {
         if (GetAt(i) != chTarget) {
            operator =(Mid(i));
            break;
         }
      }
   }
   // remove continuous occcurrences of characters in
   // passed string, starting from left
   void TrimLeft(LPCTSTR lpszTargets) {
      int pos = -1;
      CStringKS strTargets(lpszTargets);
      for (int i=0; i<GetLength(); i++) {
         if (strTargets.Find(GetAt(i)) != -1) {
            pos = i;
         } else {
            break;
         }
      }
      if (pos >= 0) {
         operator =(Mid(i));
      }
   }

   // advanced manipulation

   // replace occurrences of chOld with chNew
   int Replace(TCHAR chOld, TCHAR chNew) {
      int count = 0;
      for (int i=0; i<GetLength(); i++) {
         if (GetAt(i) == chOld) {
            SetAt(i, chNew);
            count++;
         }
      }
      return count;
   }

   // replace occurrences of substring lpszOld with lpszNew;
   // empty lpszNew removes instances of lpszOld
   int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew) {
      int count = 0;
      if (lpszOld) {
         int pos = 0;
         size_t len = _tcslen(lpszOld);
         do {
            pos = Find(lpszOld, pos);
            if (pos == -1) break;
            count++;
            if (lpszNew) {
               m_str.replace(pos, len, lpszNew);
            } else {
               m_str.erase(pos, len);
            }
         } while (true);
      }
      return count;
   }
   // remove occurrences of chRemove
   int Remove(TCHAR chRemove) {
      int count = 0;
      int i=0;
      while (i<GetLength()) {
         if (GetAt(i) == chRemove) {
            Delete(i);
            count++;
         } else {
            i++;
         }
      }
      return count;
   }
   // insert character at zero-based index; concatenates
   // if index is past end of string
   int Insert(int nIndex, TCHAR ch) {
      m_str.insert(max(0, min(nIndex, GetLength())), 1, ch);
      return GetLength();
   }
   // insert substring at zero-based index; concatenates
   // if index is past end of string
   int Insert(int nIndex, LPCTSTR pstr) {
      if (pstr) {
         m_str.insert(max(0, min(nIndex, GetLength())), pstr);
      }
      return GetLength();
   }
   // delete nCount characters starting at zero-based index
   int Delete(int nIndex, int nCount = 1) {
      const int iRes = GetLength();
      if (nCount > 0) {
         nIndex = max(0, nIndex);
         nCount = min(nCount, GetLength()-nIndex);
         m_str.erase(nIndex, nCount);
      }
      return iRes;
   }

   // searching

   // find character starting at left, -1 if not found
   int Find(TCHAR ch) const {return m_str.find(ch);}
   // find character starting at right
   int ReverseFind(TCHAR ch) const {return m_str.rfind(ch);}
   // find character starting at zero-based index and going right
   int Find(TCHAR ch, int nStart) const {return m_str.find(ch, nStart);}
   // find first instance of any character in passed string
   int FindOneOf(LPCTSTR lpszCharSet) const {return m_str.find_first_of(lpszCharSet);}
   // find first instance of substring
   int Find(LPCTSTR lpszSub) const {return m_str.find(lpszSub);}
   // find first instance of substring starting at zero-based index
   int Find(LPCTSTR lpszSub, int nStart) const {return m_str.find(lpszSub, nStart);}

   // simple formatting

   // printf-like formatting using passed string
   void Format(LPCTSTR lpszFormat, ...) {
      va_list arglist;
      va_start(arglist, lpszFormat);
      FormatV(lpszFormat, arglist);
      va_end(arglist);
   }
   // printf-like formatting using referenced string resource
   void Format(UINT nFormatID, ...) {
      va_list arglist;
      va_start(arglist, nFormatID);
      CStringKS strLoad;
      BOOL bRes = strLoad.LoadString(nFormatID);
      if (bRes) {
         FormatV(strLoad, arglist);
      } else {
         Empty();
      }
      va_end(arglist);
   }
   // printf-like formatting using variable arguments parameter
   void FormatV(LPCTSTR lpszFormat, va_list argList) {
      size_t sizeBuff = 512;
      TCHAR *szFormat = NULL;
      int write = -1;
      do {
         szFormat = new TCHAR[sizeBuff*=2];
         write = _vsntprintf(szFormat, sizeBuff-1, lpszFormat, argList);
         if (write >= 0) {
            m_str = szFormat;
         }
         delete [] szFormat;
      } while (write < 0);
   }

   // formatting for localization (uses FormatMessage API)

   // format using FormatMessage API on passed string
   void FormatMessage(LPCTSTR lpszFormat, ...) {
      va_list argList;
      va_start(argList, lpszFormat);
      LPTSTR lpszTemp;
      if ((::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,lpszFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0) &&
          (lpszTemp == NULL))
      {
         *this = lpszTemp;
         LocalFree(lpszTemp);
      }
      va_end(argList);
   }
   // format using FormatMessage API on referenced string resource
   void FormatMessage(UINT nFormatID, ...) {
      CStringKS strFormat;
      if (strFormat.LoadString(nFormatID)) {
         va_list argList;
         va_start(argList, nFormatID);
         LPTSTR lpszTemp;
         if ((::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER, strFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0) &&
             (lpszTemp == NULL))
         {
            *this = lpszTemp;
            LocalFree(lpszTemp);
         }
         va_end(argList);
      }
   }

   // input and output
   //friend CArchive& operator<<(CArchive& ar, const CStringKS& string);
   //friend CArchive& operator>>(CArchive& ar, CStringKS& string);

   // load from string resource
   BOOL LoadString(UINT nID) {
      size_t sizeBuff = 512;
      bool bRepeat;
      int iRes = 0;
      TCHAR *szFormat = NULL;
      do {
         szFormat = new TCHAR[sizeBuff*=2];
         iRes = ::LoadString(::GetModuleHandle(NULL), nID, szFormat, sizeBuff);
         bRepeat = (iRes+1 >= sizeBuff);
         if (!bRepeat)
            m_str = szFormat;
         delete [] szFormat;
      } while (bRepeat);
      return (iRes > 0);
   }

#ifndef _UNICODE
   // ANSI <-> OEM support (convert string in place)

   // convert string from ANSI to OEM in-place
   void AnsiToOem() {::CharToOem((LPCTSTR)this->operator LPCTSTR(), (LPSTR) this->operator LPCTSTR());}
   // convert string from OEM to ANSI in-place
   void OemToAnsi() {::OemToChar((LPCSTR) this->operator LPCTSTR(), (LPTSTR)this->operator LPCTSTR());}
#endif

#ifndef _AFX_NO_BSTR_SUPPORT
   // OLE BSTR support (use for OLE automation)

   // return a BSTR initialized with this CStringKS's data
   BSTR AllocSysString() const {USES_CONVERSION; return SysAllocString(T2COLE(m_str.c_str()));}
   // reallocates the passed BSTR, copies content of this CStringKS to it
   BSTR SetSysString(BSTR* pbstr) const {BSTR szRes = AllocSysString(); if (pbstr) {::SysFreeString(*pbstr); *pbstr=szRes;} return szRes;}
#endif

   // Access to string implementation buffer as "C" character array

   // get pointer to modifiable buffer at least as long as nMinBufLength
   LPTSTR GetBuffer(int nMinBufLength) {
      if (GetLength() < nMinBufLength) {
         m_str.resize(nMinBufLength);
      }
      return (LPTSTR)this->operator LPCTSTR();
   }
   // release buffer, setting length to nNewLength (or to first nul if -1)
   void ReleaseBuffer(int nNewLength = -1) {
      if (nNewLength != -1) {
         nNewLength = max(0, nNewLength);
         m_str.resize(nNewLength);
      }
      FreeExtra();
   }
   // get pointer to modifiable buffer exactly as long as nNewLength
   LPTSTR GetBufferSetLength(int nNewLength) { return GetBuffer(nNewLength);}
   // release memory allocated to but unused by string
   void FreeExtra() {
      int pos = Find(TEXT('\0'));
      if (pos != -1) {
         m_str.resize(pos);
      }
   }

   // Use LockBuffer/UnlockBuffer to turn refcounting off

   // turn refcounting back on
   LPTSTR LockBuffer();
   // turn refcounting off
   void UnlockBuffer();

// Implementation
public:
   ~CStringKS() {}
   int GetAllocLength() const {return GetLength();};
};

} // NSStrings

#endif // __FILE__CSTRINGKS__