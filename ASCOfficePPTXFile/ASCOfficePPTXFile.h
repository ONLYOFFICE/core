/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef ASC_OFFICE_PPTX_FILE
#define ASC_OFFICE_PPTX_FILE

//todo
#if defined(_WIN32) || defined (_WIN64)
    #include <atlbase.h>
    #include <atlstr.h>
#else
    #include "../DesktopEditor/common/ASCVariant.h"
    #include "../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../Common/DocxFormat/Source/Base/Base.h"
#include "PPTXFormat/PPTXEvent.h"


typedef void (*load_from_resource)(void*, int, CString&);
typedef bool (*extract_to_directory)(void*, CString&, CString&);
typedef bool (*compress_from_directory)(void*, CString&, CString&);
typedef bool (*progress_operation)(void*, long, long);

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
    CString                         m_strTempDir;
	CString							m_strDirectory;

	// writer to ppty
	CString		m_strFontDirectory;
	CString		m_strMediaDirectory;
    bool		m_bIsUseSystemFonts;
	CString		m_strEmbeddedFontsDirectory;

	CString		m_strFolderThemes;

	//load_from_resource m_fCallbackResource;
	extract_to_directory m_fCallbackExtract;
	compress_from_directory m_fCallbackCompress;
	progress_operation m_fCallbackProgress;
	void* m_pCallbackArg;
public:

	CPPTXFile(extract_to_directory fCallbackExtract, compress_from_directory fCallbackCompress, progress_operation fCallbackProgress, void* pCallbackArg);

	~CPPTXFile();

	HRESULT LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);

	HRESULT SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

#if defined(_WIN32) || defined (_WIN64)
	STDMETHOD(get_TempDirectory)(BSTR* pVal);
	STDMETHOD(put_TempDirectory)(BSTR newVal);

	STDMETHOD(GetDVDXml)(BSTR* pbstrPTTXml);
	STDMETHOD(GetBluRayXml)(BSTR* pbstrDVDXml);

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
    STDMETHOD(ConvertPPTYToPPTX)(BSTR bsInput, BSTR bsOutput, BSTR bsThemesFolder);
#else
    HRESULT get_TempDirectory(BSTR* pVal);
    HRESULT put_TempDirectory(BSTR newVal);

    HRESULT GetDVDXml(BSTR* pbstrPTTXml);
    HRESULT GetBluRayXml(BSTR* pbstrDVDXml);

    HRESULT get_DrawingXml(BSTR* pVal);
    HRESULT SetAdditionalParam(BSTR ParamName, VARIANT ParamValue);
    HRESULT GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue);

    virtual bool Progress(long ID, long Percent);

    // to PPTY
    HRESULT SetMediaDir(BSTR bsMediaDir);
    HRESULT SetFontDir(BSTR bsFontDir);
    HRESULT SetThemesDir(BSTR bsDir);
    HRESULT SetUseSystemFonts(VARIANT_BOOL useSystemFonts);
    HRESULT OpenFileToPPTY(BSTR bsInput, BSTR bsOutput);
    HRESULT OpenDirectoryToPPTY(BSTR bsInput, BSTR bsOutput);
    HRESULT ConvertPPTYToPPTX(BSTR bsInput, BSTR bsOutput, BSTR bsThemesFolder);
#endif
};
#endif //ASC_OFFICE_PPTX_FILE
