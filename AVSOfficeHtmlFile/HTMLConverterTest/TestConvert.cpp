#include "../HTMLReaderLib/LiteHTMLConverter.h"
#include "Windows.h"

int main (int argc, char *argv[ ], char *envp[ ])
{
	if (argc < 3)
	{
		TRACE (_T("needed parameters not found!"));
		return -1;
	}
	// remove "" from begin and end of string
	CString sInputFileName = argv[1];
	CString sOutputFileName = argv[2];


	// Opens input file for reading
	HANDLE hInputFile = ::CreateFile (sInputFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hInputFile)
	{
		TRACE (_T("Can't open file!! (last error: 0x%x)"), GetLastError());
		return -2;	
	}

	/*
	// Opens output file for wtitting
	HANDLE hOutputFile = ::CreateFile (sInputFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hOutputFile)
	{
		TRACE (_T("Can't open file!! (last error: 0x%x)"), GetLastError());
		return -3;	
	}
	*/

	// Create converter object
	CLiteHTMLConverter oConverter;
	IXHTMLWriter *pWriter = new CXHTMLWriter ();
	pWriter->SetOutput (sOutputFileName);
	oConverter.ToXHTML (hInputFile, pWriter);

	CloseHandle (hInputFile);
	//CloseHandle (hOutputFile);


	return 0;
}