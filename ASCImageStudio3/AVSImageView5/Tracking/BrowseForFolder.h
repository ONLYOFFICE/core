// BrowseForFolder.h: interface for the CBrowseForFolder class.
//////////////////////////////////////////////////////////////////////
#pragma once

#include "shlwapi.h"

LPCTSTR szSection		= _T("Browse Folder");
LPCTSTR szEntryFolder	= _T("Current Folder");

class CBrowseForFolder  
{
public:
	// Construction/Destruction
	CBrowseForFolder()
	{
		m_sInitDir = AfxGetApp()->GetProfileString(szSection, szEntryFolder);
	}
	virtual ~CBrowseForFolder()
	{
		AfxGetApp()->WriteProfileString(szSection, szEntryFolder, m_sInitDir);
	}

	void SetTitle(CString sTitle)
	{ 
		m_sTitle = sTitle; 
	}

	void SetInfoStr(CString sStr)
	{ 
		m_sInfoStr = sStr; 
	}

	void CBrowseForFolder::SetInitDir(CString sInitDir)
	{
		m_sInitDir = sInitDir;
	}

	CString GetFolder(CWnd *wndParent)
	{
		CString sRet;
		char* buf = new char[MAX_PATH];
		BROWSEINFO bwInfo;
		bwInfo.hwndOwner = wndParent->m_hWnd;
		bwInfo.pidlRoot = NULL;
		bwInfo.pszDisplayName = buf;	
		bwInfo.lpszTitle = m_sInfoStr;
		bwInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_NEWDIALOGSTYLE;

		bwInfo.lpfn = BrowseCallbackProc;
		bwInfo.lParam = NULL; 
		bwInfo.iImage = NULL; 
		LPITEMIDLIST pItemList = SHBrowseForFolder(&bwInfo);
		if(pItemList != NULL)
		{
			char* path = new char[MAX_PATH];
			SHGetPathFromIDList(pItemList, path);
			
			sRet = path;		
			delete []path;
		}

		delete []buf;
		m_sInitDir.UnlockBuffer();

		if ("" != sRet)
			m_sInitDir = sRet;

		return sRet;
	}

protected:
	static int CALLBACK BrowseCallbackProc(HWND  hwnd, UINT  uMsg, LPARAM  lParam, LPARAM  lpData)
	{
		if (BFFM_INITIALIZED == uMsg)
		{
			::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)m_sInitDir.LockBuffer());
			if (!m_sTitle.IsEmpty())
				SetWindowText(hwnd, m_sTitle);
		}
		else if (BFFM_SELCHANGED == uMsg)
		{
			// Set the status window to the currently selected path.
			TCHAR szDir[MAX_PATH], szCompactDir[MAX_PATH];
			if (SHGetPathFromIDList((LPITEMIDLIST) lParam , szDir)) 
			{
				PathCompactPathEx(szCompactDir, szDir, 50, 0);
				SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szCompactDir);
			}
		}            
		return NULL;
	}

protected:
	static CString m_sInitDir;
	static CString m_sTitle;
	//заголовок
	CString m_sInfoStr;
	//информационная строка
};

CString CBrowseForFolder::m_sInitDir = _T("");
CString CBrowseForFolder::m_sTitle   = _T("");
