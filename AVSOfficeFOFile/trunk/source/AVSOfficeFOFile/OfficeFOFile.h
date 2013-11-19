/*!	\file	OfficeFOFile.h
*			Declaration of the COfficeFOFile
*
*	\author	nikolay.pogorskiy@avsmedia.com
*/

#pragma once
#include "resource.h"       // main symbols

#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include "OfficeFileTemplate.h"
#include "utils\stringstreamimpl.h"
#include "..\..\..\..\Common\RSA\XMLEncoder.h"
#include "DecodingResources.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IOfficeFOFile
[
	object,
	uuid("534AD799-3396-4ADD-9E7E-7110F23D9640"),
	dual,	helpstring("IOfficeFOFile Interface"),
	pointer_default(unique)
]
__interface IOfficeFOFile : public IAVSOfficeFileTemplate
{
	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
	[id(3001 + 0)] HRESULT GetFOXml([in] BSTR SrcFile, [out, retval] BSTR* FOXML);
};


// _IOfficeFOFileEvents
[
	dispinterface,
	uuid("056E0424-5EBD-4DF1-89F8-717CE12832E1"),
	helpstring("_IOfficeFOFileEvents Interface")
]
__interface _IOfficeFOFileEvents: public  _IAVSOfficeFileTemplateEvents
{
};


// COfficeFOFile

[
	coclass,
	default(IOfficeFOFile, _IOfficeFOFileEvents),
	threading(apartment),
	support_error_info("IOfficeFOFile"),
	event_source(com),
	vi_progid("AVSOfficeFOFile.OfficeFOFile"),
	progid("AVSOfficeFOFile.OfficeFOFile.1"),
	version(1.0),
	uuid("4DFE8172-D6B0-40F7-BCFF-91D88C3A4C9E"),
	helpstring("OfficeFOFile Class")
]
class ATL_NO_VTABLE COfficeFOFile :
	public IOfficeFOFile
{
private:

	/*! Строка XSL-преобразовние */
	std::string m_xsltString;

	/*! DOM XSL-преобразовние */
	CComPtr<IXMLDOMDOCUMENT> m_XSLDoc;

	/*! XML документ, представляющий собой объединенный docx*/
	CComPtr<IXMLDOMDOCUMENT> m_MergedDocx;

	CXMLEncoder m_oEncoder;

public:

	/*!	\breif	Конвертировать из FO-формата в oox. Не реализовано, необходимость под вопросом. 	*/
	HRESULT LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);

	/*! \brief	Конвертировать из oox-формата в FO */
	HRESULT SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

	HRESULT GetFOXml(BSTR SrcFile, BSTR* FOXML);
private:
	/*! \brief	Загрузить XML из ресурсов в строку */
	static HRESULT LoadXMLFromResource(BSTR * xmlStr, WORD resourceID, LPTSTR resourceType);
	static HRESULT LoadXMLFromResource(std::string * xmlStr, WORD resourceID, LPTSTR resourceType);

	/*! Загрузить преобразование */
	HRESULT LoadXSLT();

	/*! Загрузить Docx */
	HRESULT LoadDocx(BSTR bstrPath);
	HRESULT LoadDocx(BSTR bstrPath, std::wstring & foFileName);

	/*! Объеденить Docx-папку в одну XML
		\param	bstrPath	путь к распакованному docx
	*/
	static HRESULT MergeDocxIntoSingleXML(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles);
	static HRESULT MergeDocxIntoSingleXML(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles, std::wstring& outputFOFileName);

	/*!
		\brief	Объеденить Docx-папку в одну XML
		\deprecate Ручной разбор XML в этой функции заменен на обработку с помощью xmllite (метод \a MergeDocxIntoSingleXMLImpl2)	
	*/
	static HRESULT MergeDocxIntoSingleXMLImpl1(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles);
	static HRESULT MergeDocxIntoSingleXMLImpl1(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles, std::wstring & outputFOFileName);
	/*! \brief	Объеденить Docx-папку в одну XML (ms xmllite)	*/
	static HRESULT MergeDocxIntoSingleXMLImpl2(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles);
	static HRESULT MergeDocxIntoSingleXMLImpl2(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles, std::wstring & outputFOFileName);

	static HRESULT CalcRels(BSTR bstrPath, std::vector<std::wstring> & relationshipsFiles, const std::string & relsName);
	static HRESULT CalcRels(BSTR bstrPath, std::vector<std::wstring> & relationshipsFiles)
	{
		// по умолчанию возвращаем relationships для элемента document
		return CalcRels(bstrPath, relationshipsFiles, std::string("document"));
	}

	static HRESULT GetRelationshipValues(CComPtr<IXMLDOMDOCUMENT> & xmlDoc, const std::string & relType, std::vector<std::wstring> & relTarget);

	/*! Проверить, загружено ли xsl-преобразование */
	bool	CheckXSLTReady() const;

	/*! Выполнить xsl-преобразование */
	static HRESULT DoXSLTransformImpl(IXMLDOMDOCUMENT * xslt, IXMLDOMDOCUMENT * xml, std::wstring & resultXML);
	
	/*! Создать экземпляр MSXML-документа указанной версии */
	static HRESULT CreateMSXMLDocument(IXMLDOMDOCUMENT ** ixmlDomDocument, long version);

	/*! Скопировать media-файлы из word/media в конечную папку */
	static HRESULT CopyMediaFilesFromDocx(BSTR bstrOOXPath, BSTR bstrFOPath);

	static HRESULT ReplaceImagesPath(const std::wstring & stringFOPath, std::wstring &foXML);

	HRESULT MergeAndEncode(BSTR SrcPath, BSTR DestPath, std::wstring& Result);

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		if (NULL == ParamValue)
			return S_FALSE;

		CString sParamName; sParamName = ParamName;
		if (g_csBlowfishKeyParamName == sParamName)
		{		
			ParamValue->punkVal = m_oEncoder.GetBlowfishKey();
		}		
		return S_OK;
	}

public:
	COfficeFOFile();

	__event __interface _IOfficeFOFileEvents;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT		FinalConstruct();
	void		FinalRelease();
};

