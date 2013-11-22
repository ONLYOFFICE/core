/*!	\file	ImageMetaDataParser.cpp
*	\brief	Implementation of CImageMetaDataParser
*/

#include "stdafx.h"
#include "ImageMetaDataParser.h"
#include "..\metadataparsers\exifparser.h"
#include "..\metadataparsers\iptcparser.h"
#include "MetaDataTypes.h"

#include "ImageMetaDataExif.h"
#include "ImageMetaDataIPTC.h"

#include "comdef.h"

#include "MetaDataParsers.h"
#include "..\..\..\..\AVSVideoStudio3\Common\AVSUtils.h"


#include <string>

#define __METADATA_SYNC_ACCESS do {\
	CSynchAccess SynchAccess = m_hSyncMutex;\
}while(0);


std::wstring BSTR2WSTRING(BSTR bsStr)
{
	UINT uiByteLen = ::SysStringByteLen(bsStr);
	wchar_t * tStr = (wchar_t *)malloc(uiByteLen + 2);
	memset(tStr, 0, uiByteLen + 2);
	memcpy_s(tStr, uiByteLen + 2, bsStr, uiByteLen);
	std::wstring newStr = std::wstring(tStr);
	free(tStr);
	return newStr;
}

HRESULT CImageMetaDataParser::FinalConstruct()
{
	using namespace AVS::Image;
	MetaDataParserFactory::AddParser(
		MetaDataParserFactory::ParserInfo(
		*CImageJpegExifParser::GetGuid(),
		MetaDataTypes::Exif,
		CImageJpegExifParser::createRoutine,
		__uuidof(CImageMetaDataExif)));

	MetaDataParserFactory::AddParser(
		MetaDataParserFactory::ParserInfo(
		*CImageIPTCParser::GetGuid(),
		MetaDataTypes::IPTC,
		CImageIPTCParser::createRoutine,
		__uuidof(CImageMetaDataIPTC)));

	MetaDataParserFactory::AddParser(
		MetaDataParserFactory::ParserInfo(
		*CImageRawExifParser::GetGuid(),
		MetaDataTypes::Exif,
		CImageRawExifParser::createRoutine,
		__uuidof(CImageMetaDataExif)));

	return S_OK;
}

void CImageMetaDataParser::FinalRelease()
{
}

HRESULT CImageMetaDataParser::ParseFile(BSTR bstrFileName, DWORD dwMode)
{
	HRESULT hr;
	__METADATA_SYNC_ACCESS;
	m_MetaData.clear();

	for (int i = 0; i < AVS::Image::MetaDataParserFactory::Count(); i++)
	{
		try
		{
			AVS::Image::ImageMetaDataParser::AutoPtr metaDataParser = AVS::Image::MetaDataParserFactory::CreateParser(i);
			if (!metaDataParser->ParseFile(BSTR2WSTRING(bstrFileName)))
				continue;
			CComPtr<IImageMetaDataCommon> metaData;
			if FAILED(hr = metaData.CoCreateInstance(AVS::Image::MetaDataParserFactory::GetObjectGuid(i)))
				throw _com_error(hr);

			AVS::Image::Details::ImageMetaDataCommonImpl * metaDataCommonImpl = dynamic_cast<AVS::Image::Details::ImageMetaDataCommonImpl *>(metaData.p);
			_ASSERTE(NULL != metaDataCommonImpl);
			metaDataCommonImpl->ApplyData(metaDataParser.get());
			m_MetaData.push_back(MetaDataDesc((unsigned int)(AVS::Image::MetaDataParserFactory::GetType(i)), metaData));
		}
		catch(AVS::Image::Exception::metadata_parse_error)
		{
			ATLTRACE2("[ParseFile]: metadata parse error\n");
		}
		catch(_com_error & ex)
		{
			ATLTRACE2("[ParseFile]: com error: %d\n", ex.Error());
		}
		catch(std::exception & ex)
		{
			ATLTRACE2("[ParseFile]: std::exception: %s\n", ex.what());		
		}
	}
	return S_OK;
}

HRESULT CImageMetaDataParser::ParseStream(IStream * pStream, BSTR bstrFileName){return S_OK;}

HRESULT CImageMetaDataParser::get_AvailDataTypeCount(DWORD * dwCount)
{
	__METADATA_SYNC_ACCESS;
	if (NULL == dwCount)
		return E_POINTER;
	*dwCount = m_MetaData.size();
	return S_OK;
}

HRESULT CImageMetaDataParser::MetaData(DWORD dwNum, IUnknown ** pMetaData)
{
	__METADATA_SYNC_ACCESS;
	if (!(dwNum >= 0 && dwNum < m_MetaData.size()))
		return E_INVALIDARG;
	if (NULL == pMetaData)
		return E_POINTER;

	return m_MetaData.at(dwNum).second.QueryInterface(pMetaData);
}

HRESULT CImageMetaDataParser::MetaDataType(DWORD dwNum, DWORD * dwDataType)
{
	__METADATA_SYNC_ACCESS;
	if (!(dwNum >= 0 && dwNum < m_MetaData.size()))
		return E_INVALIDARG;
	if (NULL == dwDataType)
		return E_POINTER;
	*dwDataType = m_MetaData.at(dwNum).first;
	return S_OK;
}

HRESULT CImageMetaDataParser::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue){return E_NOTIMPL;}

HRESULT CImageMetaDataParser::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue){return E_NOTIMPL;}
