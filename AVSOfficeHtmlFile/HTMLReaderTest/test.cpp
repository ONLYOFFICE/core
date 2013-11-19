#include "../HTMLReaderLib/LiteHTMLReader.h"
//#include <sys\types.h> 
#include <sys\stat.h> 

int FileSize (const char * szFileName)
{ 
  struct stat fileStat; 
  int err = stat( szFileName, &fileStat ); 
  if (0 != err) 
	  return 0; 
  return fileStat.st_size; 
}


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
		UNUSED_ALWAYS(dwAppData);
		TRACE (_T("BeginParse\n"));
		bAbort = false;
	}

	virtual void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		UNUSED_ALWAYS(pTag);
		UNUSED_ALWAYS(dwAppData);
		bAbort = false;

		TRACE (_T("StartTag: %s\n"), pTag->getTagName());
	}

	virtual void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		UNUSED_ALWAYS(pTag);
		UNUSED_ALWAYS(dwAppData);
		bAbort = false;

		TRACE (_T("EndTag: %s\n"), pTag->getTagName());
	}
	
	virtual void Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
	{
		UNUSED_ALWAYS(rText);
		UNUSED_ALWAYS(dwAppData);
		bAbort = false;

		TRACE (_T("Characters: %s\n"), rText);
	}
	
	virtual void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
	{
		UNUSED_ALWAYS(rComment);
		UNUSED_ALWAYS(dwAppData);
		bAbort = false;

		TRACE (_T("Comment: %s\n"), rComment);
	}

	virtual void EndParse(DWORD dwAppData, bool bIsAborted)
	{
		UNUSED_ALWAYS(dwAppData);
		UNUSED_ALWAYS(bIsAborted);

		TRACE (_T("EndParse\n"));
	}

};

int main (int argc, char *argv[ ], char *envp[ ])
{
	CHTMLReaderTest oTest;
	

	if (argc < 2)
	{
		TRACE (_T("no parameters found!"));
		return -1;
	}
	// remove "" from begin and end of string
	CString sFileName = argv[1];
	
	HANDLE hFile = ::CreateFile (sFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		TRACE (_T("Can't open file!! (last error: 0x%x)"), GetLastError());
		return -2;	
	}

	UINT uiOpened = oTest.ReadFile (hFile);
	CloseHandle (hFile);
	return 0;
}