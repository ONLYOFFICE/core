/*!	\file	ImageMetaDataParser.h
*			Declaration of the CImageMetaDataParser
*/

#pragma once
#include "resource.h"       // main symbols
#include "IImageMetaData.h"
#include <utility>
#include <vector>




// _IImageMetaDataParser1Events
[
	dispinterface,
	uuid("ECABE8C6-4052-47C7-AC8A-AF4246D81B89")
]
__interface _IImageMetaDataParserEvents
{
};


/*!	\class	CImageMetaDataParser
*/
[
	coclass,
	default(IImageMetaDataParser, _IImageMetaDataParserEvents),
	threading(apartment),
	support_error_info("IImageMetaDataParser"),
	event_source(com),
	vi_progid("AVSImageMetaData.ImageMetaDataParser"),
	progid("AVSImageMetaData.ImageMetaDataParser.1"),
	version(1.0),
	uuid("48591DB7-909B-4E99-8E91-9AC93F84CD1F")
]
class ATL_NO_VTABLE CImageMetaDataParser :
	public IImageMetaDataParser
{
public:
	STDMETHOD(ParseFile)(BSTR bstrFileName, DWORD dwMode) override;
	STDMETHOD(ParseStream)(IStream * pStream, BSTR bstrFileName) override;
	STDMETHOD(get_AvailDataTypeCount)(DWORD * dwCount) override;
	STDMETHOD(MetaData)(DWORD dwNum, IUnknown ** pMetaData) override;
	STDMETHOD(MetaDataType)(DWORD dwNum, DWORD * dwDataType) override;
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue) override;
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue) override;

public:
	typedef std::pair<unsigned int, CComPtr<IImageMetaDataCommon> > MetaDataDesc;
	typedef std::vector<MetaDataDesc> AvailMetaDataArray;

private:
	AvailMetaDataArray m_MetaData;

	CHandle m_hSyncMutex;

public:
	CImageMetaDataParser()
	{
		m_hSyncMutex.Attach(CreateMutex(NULL, FALSE, NULL));
	}
	
	~CImageMetaDataParser()
	{
		m_hSyncMutex.Close();
	}

	__event __interface _IImageMetaDataParserEvents;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();

	void FinalRelease();

public:

};

