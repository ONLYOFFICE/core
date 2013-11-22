// ImageShellConverter.cpp : Implementation of CImageShellConverter

#include "stdafx.h"
#include "ImageShellConverter.h"
#include "AVSUtils.h"
#include <XmlUtils.h>
#include <time.h>

BOOL CreateNewProcess(const CString& strProcessCommandLine)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	LPTSTR szCmdline = _tcsdup(strProcessCommandLine);

	// запускаем процесс
	BOOL bSuccess = ::CreateProcess(NULL,
		szCmdline,
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi);           // Pointer to PROCESS_INFORMATION structure

	// удал€ем временную пам€ть
	free(szCmdline);

	// закрываем handle'ы процесса
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return bSuccess;
}

// get rect of current(index) character
inline void GetRectCurrentCharacter(Gdiplus::Graphics* graphics, Gdiplus::StringFormat* stringFormat, Gdiplus::Font* font, BSTR Text, int length, int index, Gdiplus::RectF& rectString, Gdiplus::RectF& rectCharacter)
{
	CharacterRange charRanges[1] = {CharacterRange(index, 1)};
	Region pCharRangeRegions;
	stringFormat->SetMeasurableCharacterRanges(1, charRanges);
	graphics->MeasureCharacterRanges(Text, length, font, rectString, stringFormat, 1, &pCharRangeRegions);

	int count = 1;
	Matrix matrix;
	pCharRangeRegions.GetRegionScans(&matrix, &rectCharacter, &count);
}

// Get bitmap from resource
inline Bitmap* BitmapFromResource(HMODULE hInst, LPCTSTR pName, LPCTSTR pType)
{
	Bitmap* pBitmap = NULL;

	HRSRC hResource = ::FindResource(hInst, pName, pType);
	if (!hResource)
		return pBitmap;
	
	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
		return pBitmap;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)
		return pBitmap;

	HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (hBuffer)
	{
		void* pBuffer = ::GlobalLock(hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
			{
				pBitmap = Gdiplus::Bitmap::FromStream(pStream);
				pStream->Release();
				if (pBitmap)
				{ 
					if (pBitmap->GetLastStatus() != Gdiplus::Ok)
						RELEASEOBJECT(pBitmap);
				}
			}
			::GlobalUnlock(hBuffer);
		}
		::GlobalFree(hBuffer);
	}
	return pBitmap;
}

// Final Constructor
HRESULT CImageShellConverter::FinalConstruct()
{
	m_oGdiPlusInit.Init();

	// set icon bitmap
	m_pBitmap = BitmapFromResource(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDR_PNG_IMAGESHELLCONVERTER), "PNG");

	// set menu type
	m_lMenuType = MENU_NONE;

	// set aray ptr
	m_nMenuCount = 0;

	// set path
	m_sExePath = _T ("");
	m_sXMLPath = _T ("");

	return S_OK;
}

// Destructor
void CImageShellConverter::FinalRelease()
{
	m_arrFilePaths.RemoveAll ();

	RELEASEOBJECT(m_pBitmap);

	ClearImages();
}

// CImageShellConverter
HRESULT CImageShellConverter::Initialize(LPCITEMIDLIST pIDLFolder, LPDATAOBJECT pDataObj, HKEY hProgID)
{
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;

	// Look for CF_HDROP data in the data object.
	if (FAILED(pDataObj->GetData(&fmt, &stg)))
		return E_INVALIDARG;

	// Get a pointer to the actual data.
	hDrop = (HDROP) GlobalLock(stg.hGlobal);
	if (NULL == hDrop)
		return E_INVALIDARG;

	// Sanity check - make sure there is at least one filename.
	UINT uNumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	if (0 == uNumFiles)
	{
		GlobalUnlock(stg.hGlobal);
		ReleaseStgMedium(&stg);
		return E_INVALIDARG;
	}

	m_arrFilePaths.RemoveAll ();

	for (UINT nIndex = 0; nIndex < uNumFiles; ++nIndex)
	{
		// Name of the file
		CString sFilePath = _T ("");

		// Get the name of the file and store it in our member variable sFilePath.
		INT nPathSize = DragQueryFile (hDrop, 0, NULL, 0);
		if (0 == nPathSize)
			continue;
		nPathSize += 2;
		nPathSize = DragQueryFile (hDrop, nIndex, sFilePath.GetBuffer (nPathSize), nPathSize);
		sFilePath.ReleaseBuffer ();

		// check file extention
		if (FileCheckExtension (sFilePath))
			m_arrFilePaths.Add (sFilePath);
	}

	// release stg
	GlobalUnlock(stg.hGlobal);
	ReleaseStgMedium(&stg);

	if (0 >= m_arrFilePaths.GetCount ())
		return E_INVALIDARG;

	m_sXMLPath = _T ("");
	m_sExePath = _T ("");

	m_arrMenuItemsImage.RemoveAll ();

	// „итаем ветку реестра
	if (!ReadProgramPaths ())
		return E_INVALIDARG;
	// «агружаем пресет
	if (!LoadXml ())
		return E_INVALIDARG;

	ClearImages();
//	for (int nIndex = 0; nIndex < (int)m_nMenuCount - 1; ++nIndex)
//		m_arrBitmaps.Add (BitmapFromResource(_AtlBaseModule.GetModuleInstance(), MAKEINTRESOURCE(IDR_PNG_IMAGE), "PNG"));

	return S_OK;
}

// Create menu
HRESULT CImageShellConverter::QueryContextMenu(HMENU hMenu, UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags)
{
	// If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
	if (uFlags & CMF_DEFAULTONLY)
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);

	bool bUseOwnerDraw = false;

	HINSTANCE hinstShell = GetModuleHandle (_T("shell32"));
    if (NULL != hinstShell)
	{
        DLLGETVERSIONPROC pProc = (DLLGETVERSIONPROC) GetProcAddress(hinstShell, "DllGetVersion");
        if (NULL != pProc)
		{
            DLLVERSIONINFO rInfo = { sizeof(DLLVERSIONINFO) };

            if (SUCCEEDED(pProc(&rInfo)))
			{
                if (rInfo.dwMajorVersion > 4 || rInfo.dwMinorVersion >= 71)
                    bUseOwnerDraw = true;
			}
		}
	}

	UINT uidCmd = uidFirstCmd;
	m_uSubMenu = uidFirstCmd;

	// add separator
	InsertMenu(hMenu, uMenuIndex++, MF_SEPARATOR|MF_BYPOSITION, 0, NULL);

	// Creates the popup menu
	HMENU hmnuPopup = CreatePopupMenu();
	UINT uPopupMenuIndex = 0;
	for (int nIndex = 0; nIndex < m_nMenuCount; nIndex++)
		if (bUseOwnerDraw)
			InsertMenu(hmnuPopup, uPopupMenuIndex++, MF_STRING|MF_BYPOSITION|MF_OWNERDRAW, uidCmd++, m_arrMenuItemsImage[nIndex].m_sProfileName);
		else
			InsertMenu(hmnuPopup, uPopupMenuIndex++, MF_STRING|MF_BYPOSITION, uidCmd++, m_arrMenuItemsImage[nIndex].m_sProfileName);

	// Add popup menu to the context menu and draw icon
    MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
    mii.fMask = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE;
    mii.wID = uidCmd++;
    mii.hSubMenu = hmnuPopup;
	mii.fType  = bUseOwnerDraw ? MFT_OWNERDRAW : MFT_BITMAP;

	// Set menu srting
	CString sMenuString(c_sMenuString);
	mii.dwTypeData = sMenuString.GetBuffer();
	sMenuString.ReleaseBuffer();

    InsertMenuItem(hMenu, uMenuIndex, TRUE, &mii);

	m_uOurItemID = uidCmd - 1;
	
	// add separator
	InsertMenu(hMenu, ++uMenuIndex, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);

	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, uidCmd - uidFirstCmd);
}

// Set string command
HRESULT CImageShellConverter::GetCommandString(UINT idCmd, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
	USES_CONVERSION;

	// If Explorer is asking for a help string, copy our string into the supplied buffer.
	if (0 == (uFlags & GCS_HELPTEXT)) 
		return E_INVALIDARG;

	return E_INVALIDARG;
}

// Invoke command
HRESULT CImageShellConverter::InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo)
{
	// If lpVerb really points to a string, ignore this function call and bail out.
	if (0 != HIWORD( pCmdInfo->lpVerb))
		return E_INVALIDARG;

	// Get the command index
	long lIndex = LOWORD(pCmdInfo->lpVerb);
	if (lIndex >= 0 && lIndex < m_nMenuCount)
	{
		try{
			switch(m_lMenuType)
			{
			case MENU_IMAGE:
				if (!ConvertImage(lIndex))
					return E_INVALIDARG;
				break;
			default:
				break;
			}
		}
		catch(...)
		{
			return S_FALSE;
		}

		return S_OK;
	}

	return E_INVALIDARG;
}

STDMETHODIMP CImageShellConverter::HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // res - подставна€ переменна€ типа LRESULT. ќна не используетс€  
    // (IContextMenu2::HandleMenuMsg() не предоставл€ет способа 
    // возвращать результат), но она здесь дл€ того, чтобы
    // код в MenuMessageHandler() мог быть одинаковым   
    // независимо от того, через какой интерфейс
    // был получен вызов (IContextMenu2 или 3).

    LRESULT res;

    return MenuMessageHandler(uMsg, wParam, lParam, &res);
}

STDMETHODIMP CImageShellConverter::HandleMenuMsg2(UINT uMsg, WPARAM wParam, LPARAM lParam,  LRESULT* pResult)
{
//    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // ƒл€ сообщений, которые не имеют возвращаемых значений,     
    // pResult есть NULL. Ёто очень плохо со стороны MS, поскольку это     
    // вынуждает нас провер€ть корректность pResult перед использованием.  
    // ¬ы могли бы подумать, что указатель 
    // к возвращаемому значению всегда должен бы быть действительным, 
    // но нет. ≈сли он NULL, € создаю подставную переменную,   
    // поэтому код в MenuMessageHandler() всегда будет иметь     
    // действительный указатель pResult .                       

	if (NULL == pResult)
	{
		LRESULT res;
		return MenuMessageHandler(uMsg, wParam, lParam, &res);
	}
	else
	{
		return MenuMessageHandler(uMsg, wParam, lParam, pResult);
	}
}

STDMETHODIMP CImageShellConverter::MenuMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch(uMsg)
	{
	case WM_MEASUREITEM:
		return OnMeasureItem((MEASUREITEMSTRUCT*) lParam, pResult);
		break;

	case WM_DRAWITEM:
		return OnDrawItem((DRAWITEMSTRUCT*) lParam, pResult);
		break;
	}

    return S_OK;
}

inline void CalcTextSize(const CString &sText, LPSIZE size) 
{
	NONCLIENTMETRICS nc;
	ZeroMemory(&nc, sizeof(nc));
	nc.cbSize = sizeof(nc);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &nc, 0);
	HFONT hFont = ::CreateFontIndirectA(&nc.lfMenuFont);

	HDC hdc = CreateCompatibleDC(NULL);
	HGDIOBJ hFontOld = SelectObject(hdc, hFont);
	::GetTextExtentPoint32(hdc, sText, sText.GetLength(), size);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);
}

STDMETHODIMP CImageShellConverter::OnMeasureItem(MEASUREITEMSTRUCT* pmis, LRESULT* pResult)
{
    // Check that we're getting called for our own menu item
	if (m_uOurItemID != pmis->itemID && (m_uSubMenu > pmis->itemID || pmis->itemID > m_uSubMenu + m_nMenuCount))
        return S_OK;

    // Calculate the size of the menu item, which is the size of the thumbnail +
    // the border and padding (which provides some space at the edges of the item)
	SIZE size;
	if (m_uSubMenu <= pmis->itemID && pmis->itemID <= m_uSubMenu + m_nMenuCount)
	{
		CString sText((LPSTR)pmis->itemData);
		CalcTextSize(sText, &size);
//		size.cx += c_lBorderLeft + c_lIcoSize + c_lBorderRight;
		size.cy += 4;
	}
	else
	{
		CalcTextSize(c_sMenuString, &size);
		size.cx += c_lIcoSize + 4;
		size.cy += 4;
	}

    // Store the size of the item in the MEASUREITEMSTRUCT
    pmis->itemWidth = size.cx;
	pmis->itemHeight = max(size.cy, c_lIcoSize + 4);

    *pResult = TRUE;            // we handled the message

    return S_OK;
}

STDMETHODIMP CImageShellConverter::OnDrawItem(DRAWITEMSTRUCT* pdis, LRESULT* pResult)
{
	Bitmap* pBitmap = NULL;
	CString sText = "";
	int nSpace = 0;

	// Check that we're getting called for our own menu item
    if (m_uOurItemID == pdis->itemID)
	{
		SIZE size;
		CalcTextSize("W", &size);
		nSpace = max(size.cy + 4, c_lIcoSize + 4);

		sText = c_sMenuString;
		pBitmap = m_pBitmap;
	}
	else if (m_uSubMenu <= pdis->itemID && pdis->itemID <= m_uSubMenu + m_nMenuCount)
	{
		sText = m_arrMenuItemsImage[pdis->itemID - m_uSubMenu].m_sProfileName;
		if (m_arrBitmaps.GetCount() > pdis->itemID - m_uSubMenu)
			pBitmap = m_arrBitmaps[pdis->itemID - m_uSubMenu];
		else
		{
			SIZE size;
			CString sText((LPSTR)pdis->itemData);
			CalcTextSize(sText, &size);
			size.cx += 4;

			nSpace = (abs(pdis->rcItem.right - pdis->rcItem.left) - size.cx)/2;
		}
	}
	else
		return S_OK;

	HDC hdc = pdis->hDC;
	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, pdis->rcItem.right - pdis->rcItem.left, pdis->rcItem.bottom - pdis->rcItem.top);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(memdc, hBitmap);

	Graphics* pGraphics = new Graphics(memdc);
	if (!pGraphics)
	{
		SelectObject(memdc, hBitmapOld);
		DeleteDC(memdc);
		DeleteObject(hBitmap);

		return S_OK;
	}

	Color oColor, oColorText;
	if (pdis->itemState & ODS_SELECTED)
	{
		oColor.SetFromCOLORREF(GetSysColor(COLOR_HIGHLIGHT));
		oColorText.SetFromCOLORREF(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		oColor.SetFromCOLORREF(GetSysColor(COLOR_MENU));
		oColorText.SetFromCOLORREF(GetSysColor(COLOR_MENUTEXT));
	}
	pGraphics->Clear(oColor);

	BSTR bstrText = sText.AllocSysString();

	Gdiplus::Font oFont(hdc);
	PointF oPoint;
	if (0 == nSpace)
		oPoint.X = c_lBorderLeft + c_lIcoSize + c_lBorderRight;
	else
		oPoint.X = nSpace;

	StringFormat sFormat;
	sFormat.SetAlignment(StringAlignment::StringAlignmentCenter);
	sFormat.SetLineAlignment(StringAlignment::StringAlignmentNear);
	PointF oPointText(0, 0);
	RectF oTextRect;
	pGraphics->MeasureString(bstrText, sText.GetLength(), &oFont, oPointText, &sFormat, &oTextRect);

	oPoint.Y = ((pdis->rcItem.bottom - pdis->rcItem.top) - oTextRect.Height)/2;

	RectF oRectNew;
	GetRectCurrentCharacter(pGraphics, &sFormat, &oFont, bstrText, sText.GetLength(), 0, oTextRect, oRectNew);
	oPoint.X -= oRectNew.X;

	SolidBrush oBrush(oColorText);
	pGraphics->DrawString(bstrText, sText.GetLength(), &oFont, oPoint, &sFormat, &oBrush);
	SysFreeString(bstrText);

	Rect oRect(0, ((pdis->rcItem.bottom - pdis->rcItem.top) - c_lIcoSize)/2, c_lIcoSize, c_lIcoSize);
	
	if (nSpace == 0)
		oRect.X = c_lBorderLeft;
	else
		oRect.X = (nSpace - c_lIcoSize)/2;

	pGraphics->DrawImage(pBitmap, oRect);

	Rect rc(pdis->rcItem.left + 1, pdis->rcItem.top + 2, c_lIcoSize, c_lIcoSize);

	BitBlt(hdc, pdis->rcItem.left, pdis->rcItem.top, pdis->rcItem.right - pdis->rcItem.left, pdis->rcItem.bottom - pdis->rcItem.top, memdc, 0, 0, SRCCOPY);

	delete pGraphics;
	pGraphics = NULL;

	SelectObject(memdc, hBitmapOld);
	DeleteDC(memdc);
	DeleteObject(hBitmap);

	*pResult = TRUE;
	return S_OK;
}


BOOL CImageShellConverter::ConvertImage(long lIndex)
{
	CString sFilePath = FileGetPath (m_sXMLPath);
	if (_T ("") == sFilePath || 0 > lIndex || m_arrMenuItemsImage.GetSize () <= lIndex)
		return FALSE;

	LONG lHash = (LONG) time (NULL);
	CString sKey = _T (""); sKey.Format (g_csPathTemp, lHash);
	sFilePath += sKey;

	XmlUtils::CXmlWriter oXmlWriter;
	oXmlWriter.WriteNodeBegin (g_csXmlNodeFiles, TRUE);
	oXmlWriter.WriteAttribute (g_csXmlNodePGUID, m_arrMenuItemsImage [lIndex].m_sProfileGUID);
	oXmlWriter.WriteNodeEnd (g_csXmlNodeFiles, TRUE, FALSE);

	for (INT nIndex = 0; nIndex < (INT)m_arrFilePaths.GetCount (); ++nIndex)
	{
		oXmlWriter.WriteNodeBegin (g_csXmlNodeFile, TRUE);
		oXmlWriter.WriteAttribute (g_csXmlNodePName, m_arrFilePaths [nIndex]);
		oXmlWriter.WriteNodeEnd (g_csXmlNodeFile, TRUE, FALSE);
		oXmlWriter.WriteNodeEnd (g_csXmlNodeFile);
	}

	oXmlWriter.WriteNodeEnd (g_csXmlNodeFiles);

	oXmlWriter.SaveToFile (sFilePath);

	CreateNewProcess (m_sExePath + g_csParamExe + sFilePath);

	return TRUE;
}

CString CImageShellConverter::FileGetPath(CString sString)
{
	if (sString.GetLength() < 1)
		return _T("");
	
	int pos = sString.ReverseFind('\\');
	if (pos < sString.GetLength() - 1)
		pos += 1; 
	
	return sString.Mid(0, pos);
}

CString CImageShellConverter::FileGetExtension (CString sFilePath)
{
	if (sFilePath.GetLength() < 1) 
		return _T("");
	
	int pos1 = sFilePath.ReverseFind('\\');
	int pos2 = sFilePath.ReverseFind('.');
	
	if (pos2 <= pos1)
		return CString("");
	
	return sFilePath.Mid(pos2 + 1, sFilePath.GetLength() - 1 - pos2);
}

BOOL CImageShellConverter::FileCheckExtension (CString sFilePath)
{
	CString sExt = FileGetExtension (sFilePath);
	sExt.MakeLower();

	// check is image
	for (int nIndex = 0; nIndex < c_lExtImageCount; nIndex++)
		if (sExt == c_arExtImage[nIndex])
		{
			m_lMenuType = MENU_IMAGE;
			return TRUE;
		}

	m_lMenuType = MENU_NONE;

	return FALSE;
}

BOOL CImageShellConverter::ReadProgramPaths()
{
	HKEY hKey = NULL;

	TCHAR tsData[_MAX_PATH];
	DWORD dwDataSize = sizeof tsData;
	DWORD dwType = REG_SZ;

	LONG lResult = RegOpenKeyEx (HKEY_CURRENT_USER, g_csPathKeyDir, 0, KEY_QUERY_VALUE, &hKey);
	if (ERROR_SUCCESS == lResult)
	{
		lResult = RegQueryValueEx (hKey, g_csPathKeyNameXML, NULL, &dwType, (LPBYTE)(tsData), &dwDataSize);
		if (ERROR_SUCCESS == lResult)
			m_sXMLPath = tsData;

		lResult = RegQueryValueEx (hKey, g_csPathKeyNameExe, NULL, &dwType, (LPBYTE)(tsData), &dwDataSize);
		if (ERROR_SUCCESS == lResult)
			m_sExePath = tsData;
	}

	RegCloseKey (hKey);

	return (ERROR_SUCCESS == lResult);
}

BOOL CImageShellConverter::LoadXml()
{
	XmlUtils::CXmlReader oXmlReader;
	if (FALSE == oXmlReader.OpenFromFile (m_sXMLPath))
		return FALSE;
	if (FALSE == oXmlReader.ReadNodeList (g_csXmlNodeProfile))
		return FALSE;

	for (INT nIndex = 0; nIndex < oXmlReader.GetLengthList (); ++nIndex)
	{
		CProfile oProfile;
		oProfile.m_sProfileName = oXmlReader.ReadNodeAttribute (nIndex, g_csXmlNodePName);
		oProfile.m_sProfileGUID = oXmlReader.ReadNodeAttribute (nIndex, g_csXmlNodePGUID);

		m_arrMenuItemsImage.Add (oProfile);
	}

	m_nMenuCount = m_arrMenuItemsImage.GetSize ();

	return TRUE;
}

void CImageShellConverter::ClearImages()
{
	for (int nIndex = 0; nIndex < (int)m_arrBitmaps.GetCount(); ++nIndex)
	{
		Bitmap* pBitmap = m_arrBitmaps.GetAt(nIndex);

		RELEASEOBJECT(pBitmap);
	}

	m_arrBitmaps.RemoveAll();	
}