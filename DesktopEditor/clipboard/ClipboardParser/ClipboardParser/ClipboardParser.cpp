// ClipboardParser.cpp : Defines the entry point for the console application.
//

#include "ClipboardDataFetcher.h"
#include "IClipboardFetcher.h"

#include <iostream>

int main(int argc, char* argv[])
{
	CClipboardDataFetcher fabric;
	IClipboardFetcher* fetcher = fabric.createClipboardFetcher ();
	
	std::wstring sContentText;
	std::wstring sContentUnicodeText;
	std::wstring sContentHtml;
	
	bool bIsTextDataExists = fetcher->getTextContent (sContentText);
	bool bIsHtmlDataExists = fetcher->getHtmlContent (sContentHtml);
	bool bIsUnicodeTextDataExists = fetcher->getUnicodeTextContent (sContentUnicodeText);
	

	std::wcout << L"Clipboard HTML get data: " << (bIsHtmlDataExists ? L"Success" : L"Failed" ) << std::endl;
	if (bIsHtmlDataExists)
	{
		
		std::wcout << L"Content: " << sContentHtml << std::endl;
	}

	std::wcout << L"Clipboard TEXT get data: " << (bIsTextDataExists ? L"Success" : L"Failed" ) << std::endl;
	if (bIsTextDataExists)
	{
		std::wcout << L"Content: " << sContentText << std::endl;
	}

	std::wcout << L"Clipboard UNICODE TEXT get data: " << (bIsUnicodeTextDataExists ? L"Success" : L"Failed" ) << std::endl;
	if (bIsUnicodeTextDataExists)
	{ 
		std::wcout << L"Content: " << sContentUnicodeText << std::endl;
	}

	delete fetcher;

	return 0;
}

