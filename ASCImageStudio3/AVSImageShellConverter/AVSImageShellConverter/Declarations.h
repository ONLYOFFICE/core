#pragma once


const long c_lBorderLeft	= 6;	//Как у Microsoft (SendTo например)
const long c_lBorderRight	= 6;	//Как у Microsoft (SendTo например)
const long c_lIcoSize		= 16;	//Как у Microsoft (SendTo например)

static const CString c_sMenuString	= "AVS Convert to...";

#define	MENU_NONE		0
#define	MENU_IMAGE		1

const long c_lExtImageCount		= 13;
const CString c_arExtImage [c_lExtImageCount] = {_T("bmp"), _T("gif"), _T("jpg"), _T("jpeg"), _T("jpe"),
												 _T("jfif"), _T("png"), _T("tif"), _T("tiff"), _T("pcx"),
												 _T("ras"), _T("psd"), _T("cr2")};

// Названия ветки в реестре и ключевого поля для окна статуса XviD
const CString g_csPathKeyDir		= _T ("SOFTWARE\\AVS4YOU\\ImageConverter\\ShellExtension");
const CString g_csPathKeyNameXML	= _T ("PathToXml");
const CString g_csPathKeyNameExe	= _T ("PathToExe");

const CString g_csXmlNodeProfile	= _T ("profile");
const CString g_csXmlNodePName		= _T ("name");
const CString g_csXmlNodePGUID		= _T ("GUID");

const CString g_csXmlNodeFiles		= _T ("files");
const CString g_csXmlNodeFile		= _T ("file");

const CString g_csPathTemp			= _T ("Temp_%d.xml");

const CString g_csParamExe			= _T (" -f ");

struct CProfile
{
	CString m_sProfileName;
	CString m_sProfileGUID;
};