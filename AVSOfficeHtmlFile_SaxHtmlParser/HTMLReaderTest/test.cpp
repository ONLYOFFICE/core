#include "../HTMLReaderLib/LiteHTMLReader.h"
//#include <sys\types.h> 
#include <sys\stat.h> 

#include "atlbase.h"
#include "atlstr.h"

int FileSize (const char * szFileName)
{ 
  struct stat fileStat; 
  int err = stat( szFileName, &fileStat ); 
  if (0 != err) 
	  return 0; 
  return fileStat.st_size; 
}

CLiteHTMLElemAttr::CNamedColors CLiteHTMLElemAttr::_namedColors(166 /* block size */);

class CHTMLReaderTest: public ILiteHTMLReaderEvents
{
protected:
	CLiteHTMLReader m_oReader;
public:
	CHTMLReaderTest ()
	{
		m_oReader.setEventHandler (this);
	}
	UINT Read (LPCTSTR aHtml)
	{
		return m_oReader.Read (aHtml);
	}
	UINT ReadFile (HANDLE aFile)
	{
		return m_oReader.ReadFile (aFile);
	}

public:
	virtual void BeginParse(DWORD dwAppData, bool &bAbort)
	{
		ATLTRACE2 (_T("BeginParse\n"));
		bAbort = false;
	}

	virtual void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		bAbort = false;

		ATLTRACE2 (_T("StartTag: %s\n"), pTag->getTagName());
	}

	virtual void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		bAbort = false;

		ATLTRACE2 (_T("EndTag: %s\n"), pTag->getTagName());
	}
	
	virtual void Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
	{
		bAbort = false;

		ATLTRACE2 (_T("Characters: %s\n"), rText);
	}
	
	virtual void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
	{
		bAbort = false;

		ATLTRACE2 (_T("Comment: %s\n"), rComment);
	}

	virtual void EndParse(DWORD dwAppData, bool bIsAborted)
	{
		ATLTRACE2 (_T("EndParse\n"));
	}

};

int main (int argc, char *argv[ ], char *envp[ ])
{
	CHTMLReaderTest oTest;
	

	if (argc < 2)
	{
		ATLTRACE2 (_T("no parameters found!"));
		return -1;
	}
	// remove "" from begin and end of string
	CStringA sFileNameA = argv[1];
	CString sFileName;
	sFileName = sFileNameA;
	
	HANDLE hFile = ::CreateFile (sFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ATLTRACE2 (_T("Can't open file!! (last error: 0x%x)"), GetLastError());
		return -2;	
	}

	UINT uiOpened = oTest.ReadFile (hFile);
	CloseHandle (hFile);
	return 0;
}