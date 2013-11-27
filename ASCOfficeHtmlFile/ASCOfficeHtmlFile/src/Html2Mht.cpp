#include "stdafx.h"
#include "Html2Mht.h"
//#include "Utility.h"
#include "strings.h"
#include "../prj/UnicodeTextFile.h"



HtmlToMht::HtmlToMht(const std::wstring& sHtmlPath, const std::wstring& sMhtPath) :
    m_HtmlPath( sHtmlPath ),
    m_MhtPath( sMhtPath )
{
}


HtmlToMht::~HtmlToMht()
{
}


bool HtmlToMht::convert()
{    
	CHtmlExport mhtExport;
	AddHTMLPageToMHT(m_HtmlPath.c_str(), mhtExport);
	mhtExport.Build(m_MhtPath.c_str());
	return true;
}


void HtmlToMht::AddHTMLPageToMHT(const CString& sHTMLPath, CHtmlExport& oExport)
{
	std::list<std::wstring> content = NUnicodeTextFile::ReadFileToList (sHTMLPath);
	for(std::list<std::wstring>::iterator i = content.begin(); i != content.end(); ++i) 
	{
		AddHTMLLineToMHT ((*i).c_str(), oExport);
	}
	/*

	std::wstring path = (std::wstring)sHTMLPath;
	TxtFile file(path);
	std::list<std::wstring> content;
	
	if (file.isUtf8())
		content = transform(file.readUtf8(), Encoding::utf82unicode);
	else if (file.isUnicode())
		content = file.readUnicode();
	else
		content = transform(file.readAnsi(), Encoding::ansi2unicode);

	BOOST_FOREACH(std::wstring& sLine, content)
	{
		AddHTMLLineToMHT(sLine.c_str(), oExport);
	}
	*/
}


void HtmlToMht::AddHTMLLineToMHT(CString sLine, CHtmlExport& oExport)
{
	CString sUpdateLine;

	CStringW beginSrc = L"src=\"";
	CStringW endSrc   = L"\"";
	int beginSrcLen = 5;
	int endSrcLen = 1;

	CStringW beginLink = L"<link";
	CStringW endLink   = L">";
	int beginLinkLen = 5;
	int endLinkLen = 1;

	CStringW beginHref = L"href=\"";
	CStringW endHref   = L"\"";
	int beginHrefLen = 6;
	int endHrefLen = 1;

	int nPosBL = sLine.Find(beginLink, 0);
	int nPosEL = sLine.Find(endLink, nPosBL + beginLinkLen);

	while ( ( -1 != nPosBL )&& ( -1 != nPosEL ) )
	{
		sUpdateLine += sLine.Left(nPosBL);
		CStringW sReplace = sLine.Mid(nPosBL + beginLinkLen, nPosEL - (nPosBL + beginLinkLen));

		int nPosBH = sReplace.Find(beginHref, 0);
		int nPosEH = sReplace.Find(endHref, nPosBH + beginHrefLen);
		if ( ( -1 != nPosBH )&& ( -1 != nPosEH ) ) 
		{
			CStringW href = sReplace.Mid(nPosBH + beginHrefLen, nPosEH - (nPosBH + beginHrefLen));
			AddStyleSheet(href, oExport);
		}
		sUpdateLine += beginLink;
		sUpdateLine += sReplace;
		sUpdateLine += endLink;
		sLine.Delete(0,nPosEL + endLinkLen);
		nPosBL = sLine.Find(beginLinkLen, 0);
		nPosEL = sLine.Find(endLinkLen, nPosBL + beginLinkLen);
	}

	
	int nPosBS = sLine.Find(beginSrc, 0);
	int nPosES = sLine.Find(endSrc, nPosBS + beginSrcLen);
	while ( ( -1 != nPosBS )&& ( -1 != nPosES ) )
	{
		sUpdateLine += sLine.Left(nPosBS);
		CStringW sReplace = sLine.Mid(nPosBS + beginSrcLen, nPosES - (nPosBS + beginSrcLen));
		AddBitmap(sReplace, oExport);
		sUpdateLine += beginSrc;
		sUpdateLine += sReplace;
		sUpdateLine += endSrc;
		sLine.Delete(0,nPosES + endSrcLen);
		nPosBS = sLine.Find(beginSrc, 0);
		nPosES = sLine.Find(endSrc, nPosBS + beginSrcLen);
	}	
	sUpdateLine += sLine;
	oExport.AddString(sUpdateLine);	
}


void HtmlToMht::AddBitmap(const CString& sReplace, CHtmlExport& oExport)
{
	CStringW sImageFilePath;
	sImageFilePath = (getFilePath((std::wstring)sReplace)).c_str();

	oExport.AddBitmap(sReplace, sImageFilePath);
}


void HtmlToMht::AddStyleSheet(const CString& sReplace, CHtmlExport& oExport)
{
	CStringW sCssFilePath;
	sCssFilePath = (getFilePath((std::wstring)sReplace)).c_str();

	oExport.AddStyleSheet(sReplace, sCssFilePath);
}


const std::wstring HtmlToMht::getFilePath(const std::wstring& fileName)
{
	std::wstring path = m_HtmlPath;
	size_t right_slash = path.find_last_of(L"/");
	if (std::wstring::npos == right_slash )
		right_slash = 0;
	size_t left_slash = path.find_last_of(L"\\");
	if (std::wstring::npos == left_slash )
		left_slash = 0;
	
	path = strings::substring_before(path, max(right_slash, left_slash)) + L"\\";
	return (path + fileName);
}


bool html_to_mht( const std::wstring& html_path, const std::wstring& mht_path )
{
    HtmlToMht htmlToMht( html_path, mht_path );
    return htmlToMht.convert();
}


