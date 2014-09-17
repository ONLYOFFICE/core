#ifndef ASC_OFFICE_PPTX_FILE
#define ASC_OFFICE_PPTX_FILE

//todo
#include <atlbase.h>
#include <atlstr.h>
#include "../Common/DocxFormat/Source/Base/Base.h"
#include "PPTXFormat/PPTXEvent.h"

typedef void (*load_from_resource)(void*, int, CString&);
typedef bool (*extract_to_directory)(void*, CString&, CString&);
typedef bool (*compress_from_directory)(void*, CString&, CString&);
typedef bool (*progress_operation)(void*, long, long);

class CGdiPlusInit;
namespace PPTX
{
	class Folder;
}

class CPPTXFile : public PPTX::IPPTXEvent
{
private:
	//todo
	//OfficeUtils::IOfficeUtils*		m_pOfficeUtils;
	PPTX::Folder*					m_pFolder;
	CStringW						m_strTempDir;
	CString							m_strDirectory;

	// writer to ppty
	CString		m_strFontDirectory;
	CString		m_strMediaDirectory;
	BOOL		m_bIsUseSystemFonts;
	CString		m_strEmbeddedFontsDirectory;

	CString		m_strFolderThemes;

	CGdiPlusInit* m_pInit;
	load_from_resource m_fCallbackResource;
	extract_to_directory m_fCallbackExtract;
	compress_from_directory m_fCallbackCompress;
	progress_operation m_fCallbackProgress;
	void* m_pCallbackArg;
public:

	CPPTXFile(load_from_resource fCallbackResource, extract_to_directory fCallbackExtract, compress_from_directory fCallbackCompress, progress_operation fCallbackProgress, void* pCallbackArg);

	~CPPTXFile();
public:
	HRESULT LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
public:
	HRESULT SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);
public:
	STDMETHOD(get_TempDirectory)(BSTR* pVal);
	STDMETHOD(put_TempDirectory)(BSTR newVal);
public:
	STDMETHOD(GetDVDXml)(BSTR* pbstrPTTXml);
	STDMETHOD(GetBluRayXml)(BSTR* pbstrDVDXml);
public:
	STDMETHOD(get_DrawingXml)(BSTR* pVal);
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);
	virtual bool Progress(long ID, long Percent);
	// to PPTY
	STDMETHOD(SetMediaDir)(BSTR bsMediaDir);
	STDMETHOD(SetFontDir)(BSTR bsFontDir);
	STDMETHOD(SetThemesDir)(BSTR bsDir);
	STDMETHOD(SetUseSystemFonts)(VARIANT_BOOL useSystemFonts);
	STDMETHOD(OpenFileToPPTY)(BSTR bsInput, BSTR bsOutput);
	STDMETHOD(OpenDirectoryToPPTY)(BSTR bsInput, BSTR bsOutput);
	STDMETHOD(ConvertPPTYToPPTX)(BSTR bsInput, BSTR bsOutput);
private:

	INT32 RemoveDirOrFile(CString sPath, bool bIsRemoveHead = true);
};
#endif //ASC_OFFICE_PPTX_FILE