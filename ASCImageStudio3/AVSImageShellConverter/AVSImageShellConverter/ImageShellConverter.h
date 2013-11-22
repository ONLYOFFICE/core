// ImageShellConverter.h : Declaration of the CImageShellConverter
#pragma once
#include "resource.h"       // main symbols
#include "Declarations.h"

#include "AVSImageShellConverter.h"
#include <atlhost.h>
#include <shlobj.h>
#include <comdef.h>
#include <wingdi.h>
#include "gdiplusex.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

struct __declspec(uuid("000214e4-0000-0000-c000-000000000046")) IContextMenu; 
struct __declspec(uuid("000214f4-0000-0000-c000-000000000046")) IContextMenu2; 
struct __declspec(uuid("bcfce0a0-ec17-11d0-8d10-00a0c90f2719")) IContextMenu3; 
//_COM_SMARTPTR_TYPEDEF(IContextMenu, __uuidof(IContextMenu));

//								ShellLink Interfaces
// IID										name				Status in Wine
// {000214ee-0000-0000-c000-000000000046}	IShellLinkA			implemented
// {000214f9-0000-0000-c000-000000000046}	IShellLinkW			implemented
// {0000010b-0000-0000-c000-000000000046}	IPersistFile		implemented
// {00000109-0000-0000-c000-000000000046}	IPersistStream		implemented
// {000214e8-0000-0000-c000-000000000046}	IShellExtInit		implemented
// {000214e4-0000-0000-c000-000000000046}	IContextMenu		implemented
// {000214f4-0000-0000-c000-000000000046}	IContextMenu2	
// {bcfce0a0-ec17-11d0-8d10-00a0c90f2719}	IContextMenu3	
// {00000122-0000-0000-c000-000000000046}	IDropTarget	
// {000214eb-0000-0000-c000-000000000046}	IExtractIconA	
// {000214fa-0000-0000-c000-000000000046}	IExtractIconW	
// {45e2b4ae-b1c3-11d0-b92f-00a0c90312e1}	IShellLinkDataList	implemented
// {00021500-0000-0000-c000-000000000046}	IQueryInfo	
// {37d84f60-42cb-11ce-8135-00aa004bb851}	IPersistPropertyBag	
// {fc4801a3-2ba9-11cf-a229-00aa003d7352}	IObjectWithSite		implemented
// {6d5140c1-7436-11ce-8034-00aa006009fa}	IServiceProvider	
// {89bcb740-6119-101a-bcb7-00dd010655af}	IFilter

// CImageShellConverter
class ATL_NO_VTABLE CImageShellConverter
	: public CComObjectRootEx<CComSingleThreadModel>
	, public CComCoClass<CImageShellConverter, &CLSID_ImageShellConverter>
	, public IDispatchImpl<IImageShellConverter, &IID_IImageShellConverter, &LIBID_AVSImageShellConverterLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
	, public IShellExtInit
//	, public IContextMenu,
	, public IContextMenu3
{
public:

	DECLARE_REGISTRY_RESOURCEID(IDR_IMAGESHELLCONVERTER)

	BEGIN_COM_MAP(CImageShellConverter)
		COM_INTERFACE_ENTRY(IImageShellConverter)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IShellExtInit)
		COM_INTERFACE_ENTRY(IContextMenu)
		COM_INTERFACE_ENTRY(IContextMenu2)
		COM_INTERFACE_ENTRY(IContextMenu3)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT	FinalConstruct();
	void	FinalRelease();

    // IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST pIDLFolder, LPDATAOBJECT pDataObj, HKEY hProgID);

	// IContextMenu
	STDMETHOD(GetCommandString)(UINT idCmd, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO pCmdInfo);
	STDMETHOD(QueryContextMenu)(HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags);

protected:
    // IContextMenu2
	STDMETHOD(HandleMenuMsg)(UINT, WPARAM, LPARAM);

    // IContextMenu3
	STDMETHOD(HandleMenuMsg2)(UINT, WPARAM, LPARAM, LRESULT*);

	STDMETHOD(MenuMessageHandler)(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	STDMETHOD(OnMeasureItem)(MEASUREITEMSTRUCT* pmis, LRESULT* pResult);
	STDMETHOD(OnDrawItem)(DRAWITEMSTRUCT* pdis, LRESULT* pResult);

protected:

	// file names
	CAtlArray <CString> m_arrFilePaths;

	// the icon we display in the context menu
	Bitmap* m_pBitmap;

	CAtlArray<Bitmap*> m_arrBitmaps;

	// menu items
	CSimpleArray<CProfile> m_arrMenuItemsImage;
	int m_nMenuCount;

	// exe path
	CString m_sExePath;
	// xml path
	CString m_sXMLPath;

	// menu type
	long m_lMenuType;

	UINT m_uOurItemID;
	UINT m_uSubMenu;

	// gdi+
	CGdiPlusInit m_oGdiPlusInit;

protected:
	BOOL ConvertImage(long lIndex);

	CString FileGetExtension (CString sFilePath);
	CString FileGetPath (CString sString);

	BOOL FileCheckExtension (CString sFilePath);
	BOOL ReadProgramPaths();
	BOOL LoadXml();

	void ClearImages();
};

OBJECT_ENTRY_AUTO(__uuidof(ImageShellConverter), CImageShellConverter)