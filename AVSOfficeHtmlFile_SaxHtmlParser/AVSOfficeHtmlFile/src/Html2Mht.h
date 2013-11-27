#pragma once
#include <string>
#include "CHtmlExport.h"

bool html_to_mht( const std::wstring& html_path, const std::wstring& mht_path );



class HtmlToMht
{
public:

	HtmlToMht  ( const std::wstring& sHtmlPath, const std::wstring& sMhtPath );
    ~HtmlToMht ();
    bool convert ();

private:
	void AddHTMLPageToMHT( const CString& sHTMLPath, CHtmlExport& oExport );
	void AddHTMLLineToMHT( CString sLine, CHtmlExport& oExport );
	void AddBitmap( const CString& sReplace, CHtmlExport& oExport );
	void AddStyleSheet( const CString& sReplace, CHtmlExport& oExport );
	const std::wstring getFilePath(const std::wstring& fileName);

private:
	std::wstring m_HtmlPath;
	std::wstring m_MhtPath;
};