// OfficeUtils.h : Declaration of the COfficeUtils

#pragma once
#include "resource.h"       // main symbols
#include "ZipUtils.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IOfficeUtils
[
	object,
	uuid("D1E2A35D-AD68-4E0E-9FF2-859155151328"),
	dual,	helpstring("IOfficeUtils Interface"),
	pointer_default(unique)
]
__interface IOfficeUtils : IDispatch
{
	[id(1), helpstring("method ExtractToDirectory")] HRESULT ExtractToDirectory([in] BSTR zipFile, [in] BSTR unzipDir, [in] BSTR password, [in] SHORT extract_without_path);
	[id(2), helpstring("method CompressFileOrDirectory")] HRESULT CompressFileOrDirectory([in] BSTR name, [in] BSTR outputFile, [in] SHORT level);
	[id(3), helpstring("method Uncompress")] HRESULT Uncompress([out] BYTE* destBuf, [in,out] ULONG* destSize, [in] BYTE* sourceBuf, [in] ULONG sourceSize);
	[id(4), helpstring("method Compress")] HRESULT Compress([out] BYTE* destBuf, [in,out] ULONG* destSize, [in] BYTE* sourceBuf, [in] ULONG sourceSize, [in] SHORT level);
};

// IExtractedFileEvent
[
	object,
	uuid("6BA9C2ED-263A-456d-882F-646DA4CE1FEA"),
	dual,    helpstring("IExtractedFileEvent Interface"),
	pointer_default(unique)
]
__interface IExtractedFileEvent : IDispatch
{
	[id(9), helpstring("method ExtractedFile")] HRESULT ExtractedFile([in] BSTR file_name, [in, satype("unsigned char")] SAFEARRAY** arr);
};

// IRequestFileEvent
[
	object,
	uuid("6013A180-406F-48fc-94BD-B0AC8B72CC0E"),
	dual,    helpstring("IRequestFileEvent Interface"),
	pointer_default(unique)
]
__interface IRequestFileEvent : IDispatch
{
	[id(11), helpstring("method RequestFile")] HRESULT RequestFile([out] BSTR* file_name, [out, satype("unsigned char")] SAFEARRAY** arr, [out, retval] VARIANT_BOOL* is_data_attached);
};

// IOfficeUtils2
[
	object,
	uuid("F9C00AE2-7B59-4210-B348-5E34B8F495D7"),
	dual,	helpstring("IOfficeUtils2 Interface"),
	pointer_default(unique)
]
__interface IOfficeUtils2 : IOfficeUtils
{
	[id(5), helpstring("method IsArchive")] HRESULT IsArchive([in] BSTR filename);
	[id(6), helpstring("method IsFileExistInArchive")] HRESULT IsFileExistInArchive([in] BSTR zipFile, [in] BSTR filePath);
	[id(7), helpstring("method LoadFileFromArchive")] HRESULT LoadFileFromArchive([in] BSTR zipFile, [in] BSTR filePath, [out] BYTE** fileInBytes);	
	[id(8), helpstring("method ExtractFilesToMemory")] HRESULT ExtractFilesToMemory([in] BSTR zipFile, [in] IExtractedFileEvent* data_receiver, [out, retval] VARIANT_BOOL* result);	
	[id(10), helpstring("method CompressFilesFromMemory")] HRESULT CompressFilesFromMemory([in] BSTR zipFile, [in] IRequestFileEvent* data_source, [in] SHORT compression_level, [out, retval] VARIANT_BOOL* result);	
};

// _IAVSOfficeUtilsEvents
[
	dispinterface,
	uuid("FB7DE28F-2E10-4dc8-813E-943701B9FB81"),
	helpstring("_IAVSOfficeUtilsEvents Interface")
]

__interface _IAVSOfficeUtilsEvents
{
	//Max Value nPercent == 1000000
	//Example 23,56 % == 235600
	[id(1), helpstring("method OnProgress")] HRESULT OnProgress([in] LONG nID, [in] LONG nPercent, [in, out, ref] SHORT* Cancel);
};

// COfficeUtils

[
	coclass,
	default(IOfficeUtils, _IAVSOfficeUtilsEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeUtils.OfficeUtils"),
	progid("AVSOfficeUtils.OfficeUtils.1"),
	version(1.0),
	uuid("27AC89C1-0995-46FA-90A5-01EE850A09AC"),
	helpstring("OfficeUtils Class")
]
class ATL_NO_VTABLE COfficeUtils :
	public IOfficeUtils2
{
public:
	COfficeUtils()
	{
	}

    __event __interface _IAVSOfficeUtilsEvents;
	
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(ExtractToDirectory)(BSTR zipFile, BSTR unzipDir, BSTR password, SHORT extract_without_path);
	STDMETHOD(CompressFileOrDirectory)(BSTR name, BSTR outputFile, SHORT level);
	STDMETHOD(Uncompress)(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize);
	STDMETHOD(Compress)(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize, SHORT level);

	STDMETHOD(IsArchive)(BSTR filename);
	STDMETHOD(IsFileExistInArchive)(BSTR zipFile, BSTR filePath);
	STDMETHOD(LoadFileFromArchive)(BSTR zipFile, BSTR filePath, BYTE** fileInBytes);

	STDMETHOD(ExtractFilesToMemory)(BSTR zipFile, IExtractedFileEvent* data_receiver, VARIANT_BOOL* result);
	STDMETHOD(CompressFilesFromMemory)(BSTR zipFile, IRequestFileEvent* data_source, SHORT compression_level, VARIANT_BOOL* result);

protected:
	static void OnProgressFunc( LPVOID lpParam, long nID, long nPercent, short* Cancel );
};

