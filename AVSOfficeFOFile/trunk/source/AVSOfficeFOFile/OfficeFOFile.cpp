/*!	\file	OfficeFOFile.cpp
*	\brief	Implementation of COfficeFOFile	
*/

#include "precompiled_avsofficefofile.h"
#include <shlwapi.h>
#include "OfficeFOFile.h"
#include "utils/helpers.h"

#define MAX_PATH_LEN            256

#define XML_HEADER_STRING       "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?>"
#define PACKAGE_HEADER_STRING   "<oox:package xmlns:oox=\"urn:oox\">"
#define PACKAGE_FOOTER_STRING   "</oox:package>"
#define PART_HEADER_STRING      "<oox:part oox:name=\"%s\">"
#define PART_HEADER_STRING_L    "<oox:part oox:name=\""
#define PART_HEADER_STRING_R    "\">"
#define PART_FOOTER_STRING      "</oox:part>"

#define array_size( arr )       ( ( sizeof( arr ) ) / ( sizeof( arr[ 0 ] ) ) )

typedef enum
{
	READER_MODE_INIT           = 0,
	READER_MODE_QUESTION_MARK,
	READER_MODE_CONTENT
}
reader_mode;

static const char input_files[][MAX_PATH_LEN] = {	
	"_rels/.rels",
	"word/_rels/document.xml.rels",
	"word/settings.xml",
	"word/fontTable.xml", 
	"word/styles.xml",
	"word/numbering.xml",
	"word/document.xml",
	"docProps/app.xml",
	"docProps/core.xml",
};

static const char additional_relationships [ ] [ MAX_PATH_LEN ] =
{
	"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer", 
	"http://schemas.openxmlformats.org/officeDocument/2006/relationships/header", 
	"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme"	, 
	"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes", 
	"http://schemas.openxmlformats.org/officeDocument/2006/relationships/endnotes"
};

static const char nodeAvsProperties[] = "AvsProperties";
static const char nodeAvsPropertiesOutputFOFileName[] = "OutputFOFileName";
static const char nodeAvsPropertiesTemploraryDir[] = "TemploraryDirectory";

// COfficeFOFile
COfficeFOFile::COfficeFOFile()
{
	HRESULT hr;
	if FAILED(hr = LoadXSLT()){
		ATLTRACE2("Cannot load XSL-transform");
	}
}

HRESULT COfficeFOFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	return E_NOTIMPL;
}

HRESULT COfficeFOFile::MergeAndEncode(BSTR SrcPath, BSTR DestPath, std::wstring& Result)
{
	HRESULT hr;
	// загружено ли XSL-преобразование
	if (!CheckXSLTReady())
		return E_FAIL;

	// загружаем Docx из указанной папки
	CHECK_HR(LoadDocx(SrcPath, AVS::Utils::ExtractFileName(std::wstring(DestPath))));

	#ifdef _DEBUG
	m_MergedDocx->save(CComVariant(_T("c:\\temp\\debug_merged_doc.xml")));
	#endif

	CHECK_HR(DoXSLTransformImpl(m_XSLDoc, m_MergedDocx, Result));
	// копируем в выходную папку медиа-файлы из документа
	//CHECK_HR(CopyMediaFilesFromDocx(sSrcPath, sDstFileName));
	
	/*
	DEPRECATE 
	поставляем правильный путь вместе с xml, постпроцесс не требуется
	*/

	/* HECK_HR(ReplaceImagesPath(std::wstring(sDstFileName), outputXML)); */

	return S_OK;
}

HRESULT COfficeFOFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	std::wstring outputXML;
	HRESULT hr = MergeAndEncode(sSrcPath, sDstFileName, outputXML);
	if(hr != S_OK)
		return hr;

	std::string strOutputXml = AVS::Utils::WideStringToUTF8(outputXML);
	CString strTempXml(strOutputXml.c_str());
	CStringA strA = m_oEncoder.EncryptXML(strTempXml);
	BSTR bstrA = strA.AllocSysString();
	outputXML = std::wstring(bstrA);
	SysFreeString(bstrA);

	std::ofstream outputFile(std::wstring(sDstFileName).c_str());
	outputFile << AVS::Utils::WideStringToUTF8(outputXML);
	outputFile.close();

	return S_OK;
}

HRESULT COfficeFOFile::GetFOXml(BSTR SrcFile, BSTR* FOXML)
{
	BSTR l_bstrEmpty(L"");
	std::wstring outputXML;
	HRESULT hr = MergeAndEncode(SrcFile, l_bstrEmpty, outputXML);
	if(hr != S_OK)
		return hr;

	//std::string strOutputXml = AVS::Utils::WideStringToUTF8(outputXML);
	CString strTempXml(outputXML.c_str());
	CStringA strA = m_oEncoder.EncryptXML(strTempXml);
	*FOXML = strA.AllocSysString();
	//outputXML = std::wstring(bstrA);
	//SysFreeString(bstrA);

	return S_OK;
}

HRESULT COfficeFOFile::LoadXMLFromResource(BSTR * xmlStr, WORD resourceID, LPTSTR resourceType)
{
	if (resourceID == IDR_HTML2)
	{
#ifdef _DEBUG
		std::string strXml;
		LoadXMLFromResource(&strXml, resourceID, resourceType);

		std::string xslStringWithoutBOM;
		if ((unsigned char)strXml[0] == 0xEF
			&& (unsigned char)strXml[1] == 0xBB
			&& (unsigned char)strXml[2] == 0xBF)
			xslStringWithoutBOM	= &strXml[3];
		else
			xslStringWithoutBOM	= strXml;

		*xmlStr = ::SysAllocString(AVS::Utils::UTF8ToWideString(xslStringWithoutBOM).c_str());
#else
		CString strXml	= readFileFromRresource(IDR_REGISTRY1);
		*xmlStr	= strXml.AllocSysString();
#endif
	}

	return S_OK;
}
HRESULT COfficeFOFile::LoadXMLFromResource(std::string * xmlStr, WORD resourceID, LPTSTR resourceType)
{	
	if (!xmlStr)
		return E_POINTER;

	HINSTANCE hResInstance = ATL::_pModule->GetResourceInstance();
	if (!hResInstance)
		return E_FAIL;

	HRSRC hRes = FindResource((HMODULE)hResInstance, MAKEINTRESOURCE(resourceID), resourceType);
	if (!hRes)
		return E_FAIL;

	DWORD sizeofXML = SizeofResource((HMODULE) hResInstance, hRes); 

	HGLOBAL hData = LoadResource((HMODULE) hResInstance, hRes);

	if (!hData)
		return false;

	LPVOID lpData = LockResource(hData);
	if (!lpData)
		return E_FAIL;

	char * resourceXML = new char[sizeofXML + 1];
	memset(reinterpret_cast<void*>(resourceXML), 0, sizeofXML+1);
	memcpy_s(reinterpret_cast<void*>(resourceXML), sizeofXML + 1, lpData, sizeofXML);
	*xmlStr = (char*)resourceXML;
	free(resourceXML);
	return S_OK;
}

HRESULT COfficeFOFile::LoadXSLT()
{
	HRESULT hr;
	m_xsltString = "";
	m_XSLDoc.Release();

	BSTR xslTempString;
	CHECK_HR(LoadXMLFromResource(&xslTempString, IDR_HTML2, RT_HTML));

	CComPtr<IXMLDOMDOCUMENT> pXSLDoc;

	CHECK_HR(hr = CreateMSXMLDocument(&pXSLDoc, MS_XML_VERSION));
		
	VARIANT_BOOL bSuccessful = VARIANT_FALSE;

	hr = pXSLDoc->loadXML(xslTempString, &bSuccessful);
	SysFreeString(xslTempString);
	if (FAILED(hr) || bSuccessful == VARIANT_FALSE)
		return hr;

	m_XSLDoc.Attach(pXSLDoc.Detach());
	return S_OK;
}

HRESULT COfficeFOFile::LoadDocx(BSTR bstrPath)
{
	return LoadDocx(bstrPath, std::wstring(L""));
}


HRESULT COfficeFOFile::LoadDocx(BSTR bstrPath, std::wstring & foFileName)
{
	HRESULT hr;

	// вычисляем дополнительные файлы, которые необходимо объеденить
	std::vector<std::wstring> relationshipsFiles;
	CalcRels(bstrPath, relationshipsFiles);

	std::string mergedDocx;
	CHECK_HR(MergeDocxIntoSingleXML(bstrPath, mergedDocx, relationshipsFiles, foFileName));

	CComPtr<IXMLDOMDOCUMENT> pMergedDocx;
	CHECK_HR(CreateMSXMLDocument(&pMergedDocx, MS_XML_VERSION));

	VARIANT_BOOL bSuccessful = VARIANT_FALSE;
	pMergedDocx->put_validateOnParse(VARIANT_FALSE);
	hr = pMergedDocx->loadXML(CComBSTR(AVS::Utils::UTF8ToWideString(mergedDocx).c_str()), &bSuccessful);
	if (S_FALSE == hr)
		return E_FAIL;

	if (FAILED(hr) || bSuccessful == VARIANT_FALSE)
		return hr;

	m_MergedDocx.Attach(pMergedDocx.Detach());

	return S_OK;
}

HRESULT COfficeFOFile::DoXSLTransformImpl(IXMLDOMDOCUMENT * xslt, IXMLDOMDOCUMENT * xml, std::wstring & resultXML)
{
	HRESULT hr;
	IXMLDOMElement * xslRootElm = NULL;
	hr = xslt->get_documentElement(&xslRootElm);
	BSTR bsResultXML = NULL;
	if (S_OK != (hr = xml->transformNode(xslRootElm, &bsResultXML)))
		return hr;

	resultXML = std::wstring(bsResultXML);
	SysFreeString(bsResultXML);
	return S_OK;
}

HRESULT COfficeFOFile::MergeDocxIntoSingleXMLImpl1(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles)
{
	return MergeDocxIntoSingleXMLImpl1(bstrPath, mergedDocx, additionalFiles, std::wstring(L""));
}

// DEPRECATE
// from AVSOfficeHtmpFile
HRESULT COfficeFOFile::MergeDocxIntoSingleXMLImpl1(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles, std::wstring & outputFOFileName)
{
	/*
	Это плохая функция, надо переписать под XML-движок
	*/

	if (!bstrPath)
		return E_INVALIDARG;

	std::wstring input_dir = bstrPath;
	std::stringstream outputString;

	char      buff      [ 512 ]                         = { 0 };
	wchar_t   wbuff     [ MAX_PATH_LEN ]                = { 0 };
	size_t    len                                       = 0;
	
	outputString << XML_HEADER_STRING;
	outputString << PACKAGE_HEADER_STRING;

	std::vector<std::string> inputFilesArray;
	for (UINT i = 0; i < array_size( input_files ); ++i)
		inputFilesArray.push_back(input_files[i]);

	for (UINT i = 0; i < additionalFiles.size(); ++i)
		inputFilesArray.push_back(AVS::Utils::WideStringToANSI(additionalFiles[i]));

	for (UINT i = 0; i < inputFilesArray.size(); ++i)
	{
		FILE       * tmp                           = NULL;
		wchar_t      tmp_path [ MAX_PATH_LEN * 2 ] = { 0 };
		reader_mode  mode                          = READER_MODE_INIT;

		swprintf_s(tmp_path, MAX_PATH_LEN * 2, L"%s/%s",  input_dir.c_str(), AVS::Utils::ANSIToWideString(inputFilesArray[ i ]).c_str() );
		_wfopen_s(&tmp, tmp_path, L"r+" );
		if ( tmp )
		{
			outputString << PART_HEADER_STRING_L << inputFilesArray[ i ] << PART_HEADER_STRING_R;

			while ( !feof( tmp ) )
			{
				char c = fgetc( tmp );
				switch ( mode )
				{
				case READER_MODE_INIT:
					if ( c == '?' )
					{
						mode = READER_MODE_QUESTION_MARK;
					}
					break;
				case READER_MODE_QUESTION_MARK:
					mode = ( c == '>' ) ? READER_MODE_CONTENT : READER_MODE_INIT;
					break;
				case READER_MODE_CONTENT:
					outputString << c;
					break;
				default:
					break;
				}
			}
			outputString << PART_FOOTER_STRING;
			fclose( tmp );
		}
		else
			continue;
	}

	if (outputFOFileName != L"")
	{
		outputString << PART_HEADER_STRING_L << nodeAvsProperties << PART_HEADER_STRING_R;
		outputString << "<" << "oox:" << nodeAvsProperties << ">";
		outputString << "<" << "oox:" << nodeAvsPropertiesOutputFOFileName << " value=\"" << AVS::Utils::WideStringToUTF8(outputFOFileName) << "\"/>";
		outputString << "</" << "oox:" << nodeAvsProperties << ">";
		outputString << PART_FOOTER_STRING;
		outputString << PACKAGE_FOOTER_STRING ;
	}

	mergedDocx = outputString.str();

	std::ofstream outputFile(L"e:/01.xml");
	outputFile << mergedDocx;
	outputFile.close();


	return S_OK;
}

/*! Merge XML под xmllite-движок
*/
HRESULT COfficeFOFile::MergeDocxIntoSingleXMLImpl2(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles)
{
	return MergeDocxIntoSingleXMLImpl2(bstrPath, mergedDocx, additionalFiles, std::wstring(L""));
}

/*! Merge XML под xmllite-движок
*/
HRESULT COfficeFOFile::MergeDocxIntoSingleXMLImpl2(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles, std::wstring & outputFOFileName)
{
	HRESULT hr;
	CComPtr<IXmlWriter> pWriter;
	CComPtr<IStream> pOutFileStream;
	
	CHECK_HR(AVS::StringStreamImpl::CreateStream(&pOutFileStream));
	CHECK_HR(CreateXmlWriter(__uuidof(IXmlWriter), (void**)&pWriter, NULL));

	if (!pOutFileStream)
		return E_FAIL;

	CHECK_HR(pWriter->SetOutput(pOutFileStream));
	CHECK_HR(pWriter->WriteStartDocument(XmlStandalone_Yes));
		
	wchar_t * oox_ns = L"oox";
	wchar_t * oox_uri = L"urn:oox";

	CHECK_HR(pWriter->WriteStartElement(oox_ns, L"package", oox_uri));

	std::vector<std::string> inputFilesArray;
	for (UINT i = 0; i < array_size( input_files ); ++i)
		inputFilesArray.push_back(input_files[i]);

	for (UINT i = 0; i < additionalFiles.size(); ++i)
		inputFilesArray.push_back(AVS::Utils::WideStringToANSI(additionalFiles[i]));

	std::wstring input_dir = bstrPath;
	for (UINT i = 0; i < inputFilesArray.size(); ++i)
	{
		std::wstring tmp_path;
		tmp_path = input_dir + L"/" + AVS::Utils::ANSIToWideString(inputFilesArray[i]);

		CComPtr<IStream> pFileStream;
		CComPtr<IXmlReader> pReader;

		if (FAILED(hr = SHCreateStreamOnFileW(tmp_path.c_str(), STGM_READ, &pFileStream)) || !pFileStream)
			continue;

		CHECK_HR(CreateXmlReader(__uuidof(IXmlReader), (void**) &pReader, NULL));
		CHECK_HR(pReader->SetInput(pFileStream));
		CHECK_HR(pWriter->WriteStartElement(oox_ns, L"part", oox_uri));
		CHECK_HR(pWriter->WriteAttributeString(oox_ns, L"name", oox_uri, AVS::Utils::ANSIToWideString(inputFilesArray[i]).c_str()));
		CHECK_HR(pWriter->WriteNode(pReader, true));
		CHECK_HR(pWriter->WriteFullEndElement());
	}


	CHECK_HR(pWriter->WriteStartElement(oox_ns, L"part", oox_uri));
	CHECK_HR(pWriter->WriteAttributeString(oox_ns, L"name", oox_uri, AVS::Utils::ANSIToWideString(nodeAvsProperties).c_str() ));

	CHECK_HR(pWriter->WriteStartElement(oox_ns, AVS::Utils::ANSIToWideString(nodeAvsProperties).c_str(), oox_uri));
	if (outputFOFileName != L"")
	{
		CHECK_HR(pWriter->WriteStartElement(oox_ns, AVS::Utils::ANSIToWideString(nodeAvsPropertiesOutputFOFileName).c_str(), oox_uri));
		CHECK_HR(pWriter->WriteAttributeString(NULL, L"value", NULL, outputFOFileName.c_str() ));
		CHECK_HR(pWriter->WriteEndElement());
	}

	CHECK_HR(pWriter->WriteStartElement(oox_ns, AVS::Utils::ANSIToWideString(nodeAvsPropertiesTemploraryDir).c_str(), oox_uri));
	CHECK_HR(pWriter->WriteAttributeString(NULL, L"value", NULL, input_dir.c_str() ));
	CHECK_HR(pWriter->WriteEndElement());

	CHECK_HR(pWriter->WriteEndElement());

	CHECK_HR(pWriter->WriteEndElement());

	CHECK_HR(pWriter->WriteEndElement());
	CHECK_HR(pWriter->WriteEndDocument());
	CHECK_HR(pWriter->Flush());
	
	CComPtr<AVS::IStringStream> pStringStream;
	CHECK_HR(pOutFileStream.QueryInterface(&pStringStream));
	CHECK_HR(pStringStream->GetString(&mergedDocx));

	return S_OK;
}

HRESULT COfficeFOFile::MergeDocxIntoSingleXML(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles)
{
	return COfficeFOFile::MergeDocxIntoSingleXML(bstrPath, mergedDocx, additionalFiles, std::wstring(L""));
}

HRESULT COfficeFOFile::MergeDocxIntoSingleXML(BSTR bstrPath, std::string & mergedDocx, const std::vector<std::wstring> & additionalFiles, std::wstring & outputFOFileName)
{
	HRESULT hr;
	if SUCCEEDED(hr = MergeDocxIntoSingleXMLImpl2(bstrPath, mergedDocx, additionalFiles, outputFOFileName))
		return hr;

	// не получилось использовать xmllite (отсутствует библиотека?)
	return MergeDocxIntoSingleXMLImpl1(bstrPath, mergedDocx, additionalFiles, outputFOFileName);
}

bool COfficeFOFile::CheckXSLTReady() const
{
	return (!!m_XSLDoc);
}

HRESULT COfficeFOFile::FinalConstruct()
{
	return S_OK;
}

void COfficeFOFile::FinalRelease()
{
}


HRESULT COfficeFOFile::CreateMSXMLDocument(IXMLDOMDOCUMENT ** ixmlDomDocument, long version)
{
	HRESULT hr;
	CComPtr<IXMLDOMDOCUMENT> xmlDomDocument;
	switch(version)
	{
	case 6:
		if FAILED(hr = xmlDomDocument.CoCreateInstance(__uuidof(DOMDocument60)))
		{
			// пытаемся создать MSXML более ранней версии
			return CreateMSXMLDocument(ixmlDomDocument, 4);
		}
		else
		{
			*ixmlDomDocument = xmlDomDocument.Detach();
			return hr;
		}
	case 4:
		CHECK_HR(xmlDomDocument.CoCreateInstance(__uuidof(DOMDocument40)));

		*ixmlDomDocument = xmlDomDocument.Detach();
		return hr;
	default:
		return E_INVALIDARG;
	}
}

HRESULT COfficeFOFile::CopyMediaFilesFromDocx(BSTR bstrOOXPath, BSTR bstrFOPath)
{
	std::wstring outputDirName = std::wstring(bstrFOPath) + L".files";
	std::wstring inputDirMask = std::wstring(bstrOOXPath) + L"/word/media/*.*";
	WIN32_FIND_DATAW FindFileData = { 0 };
	HANDLE hCopy = FindFirstFileW(inputDirMask.c_str(), &FindFileData );
	BOOL findReslut = (INVALID_HANDLE_VALUE != hCopy);
	if (findReslut)
	{
		if (!CreateDirectoryW(outputDirName.c_str(), NULL))
		{
			DWORD lastError = GetLastError();
			if (ERROR_ALREADY_EXISTS != lastError)
				return E_FAIL;
		}
	}

    while (findReslut)
    {
		std::wstring file_from = std::wstring(bstrOOXPath) + L"/word/media/" + std::wstring(FindFileData.cFileName);
		std::wstring file_to = outputDirName + L"/" + std::wstring(FindFileData.cFileName);
		CopyFileW(file_from.c_str(), file_to.c_str(), FALSE );
        findReslut = FindNextFileW( hCopy, &FindFileData );
    }

	return S_OK;
}

/*
	\deprecate	Поставляем правильные пути и подстановка выполняется в xslt-преобразовании
*/
HRESULT COfficeFOFile::ReplaceImagesPath(const std::wstring & stringFOPath, std::wstring &foXML)
{
	std::wstring imagePath = std::wstring(L".\\") + AVS::Utils::ExtractFileName(stringFOPath) + L".files";
	boost::algorithm::replace_all(foXML, L"avsooximagesource:media", imagePath);
	return S_OK;
}


HRESULT COfficeFOFile::CalcRels(BSTR bstrPath, std::vector<std::wstring> & relationshipsFiles, const std::string & relsName)
{
	HRESULT hr;
	std::wstring input_dir = bstrPath;
	CComPtr<IXMLDOMDOCUMENT> pXMLDoc;
	CHECK_HR(CreateMSXMLDocument(&pXMLDoc, MS_XML_VERSION));

	input_dir += std::wstring(L"\\word\\_rels\\") + AVS::Utils::ANSIToWideString(relsName) + std::wstring(L".xml.rels");
	
	VARIANT_BOOL isSuccessful = VARIANT_FALSE;
	CHECK_HR(pXMLDoc->load(CComVariant(input_dir.c_str()), &isSuccessful));
	if (isSuccessful != VARIANT_TRUE)
		return S_OK;
	
	std::vector<std::wstring> reslutVector;
	for (int i = 0; i < array_size(additional_relationships); ++i)
		GetRelationshipValues(pXMLDoc, additional_relationships[i], reslutVector);
	
	relationshipsFiles = reslutVector;
	return S_OK;
}

HRESULT COfficeFOFile::GetRelationshipValues(CComPtr<IXMLDOMDOCUMENT> & xmlDoc, const std::string & relType, std::vector<std::wstring> & relTarget)
{
	HRESULT hr;
	std::vector<std::wstring> resultVector;

	std::string queryString = std::string("//*[@Type='") + relType + std::string("']/@Target");
	CComPtr<IXMLDOMNodeList> pNodeList;
	CHECK_HR(xmlDoc->selectNodes(CComBSTR(queryString.c_str()), &pNodeList));
	if (!pNodeList)
		return E_UNEXPECTED;

	long nodeListLength = 0;
	CHECK_HR(pNodeList->get_length(&nodeListLength));
	if (0 == nodeListLength)
	{
		ATLTRACE2("Warning: rels for type %s not found\n", relType.c_str());
		return S_OK;
	}

	for (int nodeIndex = 0; nodeIndex < nodeListLength; ++nodeIndex)
	{
		CComPtr<IXMLDOMNode> pCurrentNode;
		if FAILED(pNodeList->get_item(nodeIndex, &pCurrentNode))
		{
			ATLTRACE2("Warning: cannot get item from node list\n");
			continue;
		}
		CComVariant pNodeValue;
		if FAILED(pCurrentNode->get_nodeValue(&pNodeValue))
		{
			ATLTRACE2("Warning: cannot get node value\n");
			continue;
		}
		if (VT_BSTR == pNodeValue.vt)
		{
			relTarget.push_back(std::wstring(L"word/") + std::wstring(pNodeValue.bstrVal));
			relTarget.push_back(std::wstring(L"word/_rels/") + std::wstring(pNodeValue.bstrVal) + std::wstring(L".rels"));
		}else
		{
			ATLTRACE2("Warning: unknown attribute type");
		}
	}
	return S_OK;	
}


