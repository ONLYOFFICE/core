// OfficeFileTemplate.h : Declaration of the CAVSOfficeFileTemplate

#pragma once

// IAVSOfficeFileTemplate
[
	object,
	uuid("3FC4EC15-9467-4D66-AD6A-A0C0BAEDD3CD"),
	dual,	helpstring("IAVSOfficeFileTemplate Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeFileTemplate : IDispatch
{
	[id(1), helpstring("method LoadFromFile")] HRESULT LoadFromFile([in] BSTR sSrcFileName, [in] BSTR sDstPath, [in] BSTR sXMLOptions);
	[id(2), helpstring("method SaveToFile")] HRESULT SaveToFile([in] BSTR sDstFileName, [in] BSTR sSrcPath, [in] BSTR sXMLOptions);
};

// IAVSOfficeFile
[
	object,
	uuid("5cc68050-bf08-11de-8a39-0800200c9a66"),
	dual,	helpstring("IAVSOfficeFile Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeFile
{
	[id(510), helpstring("method OpenFile")] HRESULT OpenFile( [in] BSTR bstrFilename, [in] BSTR bstrOptions );
	[id(520), helpstring("method CloseFile")] HRESULT CloseFile();
	[id(530), helpstring("method StartPreview")] HRESULT StartParse();
	[id(540), helpstring("method StopPreview")] HRESULT StopParse();
	[id(550), helpstring("method SuspendPreview")] HRESULT SuspendParse();
	[id(560), helpstring("method ResumePreview")] HRESULT ResumeParse();
	[id(570), helpstring("method ReadPage")] HRESULT ReadPage( [in] LONG nPageNumber, [out, retval] IUnknown** ppunkMediaData );
	[id(580), helpstring("method DrawPage")] HRESULT DrawPage( [in] LONG nPageNumber, [in] IUnknown* punkMediaData );
	[id(581), helpstring("method PrintPage")] HRESULT PrintPage( [in] LONG nPageNumber, [in] LONG* pHDC, [in] BSTR bstrXMLMode );
	[id(590), helpstring("method GetPageCount")] HRESULT GetPageCount( [out, retval] LONG* pnC );
	[id(600), helpstring("method GetConverter")] HRESULT GetInternalInteface( [out, retval] IUnknown** punkFileConverter );
	[id(610), helpstring("method GetPageSize")] HRESULT GetPageSize([in] LONG lPageNumber, [out] DOUBLE* pfWidth, [out] DOUBLE* pfHeight);

	[id(620), helpstring("method SetAdditionalParam")] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(630), helpstring("method GetAdditionalParam")] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(10), propget] HRESULT FileType( [out, retval] LONG* pnType );
	[id(10), propput] HRESULT FileType( [in] LONG nType );

	[id(20), propget] HRESULT OfficeViewer( [out, retval] IUnknown** ppunkVal );
	[id(20), propput] HRESULT OfficeViewer( [in] IUnknown* punkVal );

	[id(30), propget] HRESULT FileInfo( [out, retval] BSTR* pbstrInfo );
	[id(30), propput] HRESULT FileInfo( [in] BSTR bstrInfo );

	[id(40), propget] HRESULT IsEdited( [out, retval] VARIANT_BOOL* pvIsEdited);

	[id(50), propget] HRESULT Title( [out, retval] BSTR* pbstrVal );
	[id(50), propput] HRESULT Title( [in] BSTR bstrVal );
	[id(60), propget] HRESULT Subject( [out, retval] BSTR* pbstrVal );
	[id(60), propput] HRESULT Subject( [in] BSTR bstrVal );
	[id(70), propget] HRESULT Keywords( [out, retval] BSTR* pbstrVal );
	[id(70), propput] HRESULT Keywords( [in] BSTR bstrVal );
	[id(80), propget] HRESULT Comments( [out, retval] BSTR* pbstrVal );
	[id(80), propput] HRESULT Comments( [in] BSTR bstrVal );
	[id(90), propget] HRESULT Author( [out, retval] BSTR* pbstrVal );
	[id(90), propput] HRESULT Author( [in] BSTR bstrVal );
	[id(100), propget] HRESULT RevisionNumber( [out, retval] BSTR* pbstrVal );
	[id(100), propput] HRESULT RevisionNumber( [in] BSTR bstrVal );
	[id(110), propget] HRESULT Width( [out, retval] LONG* pnVal );
	[id(110), propput] HRESULT Width( [in] LONG nVal );
	[id(120), propget] HRESULT Height( [out, retval] LONG* pnVal );
	[id(120), propput] HRESULT Height( [in] LONG nVal );
	[id(130), propget] HRESULT ResizeMode( [out, retval] LONG* pnVal );
	[id(130), propput] HRESULT ResizeMode( [in] LONG nVal );
	[id(140), propget] HRESULT Watermark( [out, retval] BSTR* pbstrVal );
	[id(140), propput] HRESULT Watermark( [in] BSTR bstrVal );

	[id(150), propget] HRESULT IsParseFileComplete( [out, retval] VARIANT_BOOL* pvIsParseFileComplete );
	[id(160), propget] HRESULT MaxFilesCount([out,retval] LONG *pnVal);
	[id(160), propput] HRESULT MaxFilesCount([in] LONG nVal);

	[id(170), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
	[id(170), propput] HRESULT TempDirectory( [in] BSTR bstrDir );
	[id(180), propget] HRESULT Password( [out, retval] BSTR* pbstrPassword );
	[id(180), propput] HRESULT Password( [in] BSTR bstrPassword );
	[id(171), propget] HRESULT Permissions( [out, retval] LONG* pnPermissions );
	[id(173), propget] HRESULT Status([out, retval] LONG* pVal);

	[id(190), propget] HRESULT CommandRenderer( [out, retval] IUnknown** ppunkRend );
};
//OfficeFile Status
const SHORT c_shOfficeFileIdle				= 0x0000;
const SHORT c_shOfficeFileOpen				= 0x0001;
const SHORT c_shOfficeFileParsing			= 0x0002;
const SHORT c_shOfficeFileSuspended			= 0x0003;
const SHORT c_shOfficeFileParsingComplete	= 0x0004;

const LONG c_nMaxProgressPercent   = 1000000;

// _IAVSOfficeFileTemplateEvents
[
	dispinterface,
	uuid("0171E41A-57D7-489d-816A-14FC9813CCE1"),
	helpstring("_IAVSOfficeFileTemplateEvents Interface")
]

__interface _IAVSOfficeFileTemplateEvents
{
	//Max Value nPercent == 1000000
	//Example 23,56 % == 235600
	[id(1), helpstring("method OnProgress")] HRESULT OnProgress([in] LONG nID, [in] LONG nPercent);
};

const SHORT c_shProgressContinue = 0x0000;
const SHORT c_shProgressCancel   = 0x0001;

// _IAVSOfficeFileTemplateEvents2
[
	dispinterface,
	uuid("9764153E-DAEB-40dd-94D4-A2C39218AF64"),
	helpstring("_IAVSOfficeFileTemplateEvents2 Interface")
]

__interface _IAVSOfficeFileTemplateEvents2
{
	//Max Value nPercent == 1000000
	//Example 23,56 % == 235600
	[id(2), helpstring("method OnProgressEx")] HRESULT OnProgressEx([in] LONG nID, [in] LONG nPercent, [in, out, ref] SHORT* Cancel);
};

// IAVSOfficePage
[ object, uuid("BEAB78C2-D3B5-4a71-9E4D-E5D0D51C439F"),	dual, pointer_default(unique) ]
__interface IAVSOfficePages : IDispatch
{
	[id(100000 + 1), propget]	HRESULT PagesCount([out, retval] LONG* plCount);
	[id(100000 + 2)]			HRESULT GetPageInfo([in] int nPage, [out] double* pdWidth, [out] double* pdHeight, [out] double* pdDpiX, [out] double* pdDpiY);
	[id(100000 + 3)]			HRESULT DrawPage([in] int nPage, [in] IUnknown* punkRenderer, [in] BOOL* pBreak);
};
