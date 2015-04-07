#pragma once
#include "atldefine.h"
#include "ImageConverterDefines.h"
#include "../Common/ImageFileFormatChecker.h"

#define APNG_FILETYPE 3

// IAVSXMLImageSource
[
	object,
	uuid("75791a70-f07e-11de-8a39-0800200c9a66"),
	dual,	helpstring("IAVSXMLImageSource Interface"),
	pointer_default(unique)
]
__interface IAVSImageInfo : IDispatch
{
	[id(40), helpstring("method Open")] HRESULT Open( [in] BSTR bstrFilename );
	[id(50), helpstring("method GetFrameCount")] HRESULT GetFrameCount( [out, retval] LONG* pnFrameCount );
	[id(60), helpstring("method GetFrameSize")] HRESULT GetFrameSize( [in] LONG nFrame, [out] LONG* pnWidth, [out] LONG* pnHeight );
	[id(70), helpstring("method GetFileType")] HRESULT GetFileType( [out, retval] LONG* pnFileType );
	[id(80), helpstring("method Close")] HRESULT Close();
};


[
	coclass,
	default(IAVSImageInfo),
	threading(apartment),
	vi_progid("AVSImageConverter.AVSImageInfo"),
	progid("AVSImageConverter.AVSImageInfo.1"),
	version(1.0),
	uuid("80bab9c0-f07e-11de-8a39-0800200c9a66"),
	helpstring("CAVSImageInfo Class")
]
class ATL_NO_VTABLE CAVSImageInfo : public IAVSImageInfo
{
private: CString m_sCurFilename;
private: ImageStudio::IImageTransforms* m_piTransform;
private: ImageVideoFile::IImageVideoFile4* m_piImageVideoFile;
private: bool m_bIsApng;
private: long m_nApngWidth;
private: long m_nApngHeight;
private: long m_nApngFrameCount;
private: long m_nApngType;
public:
	CAVSImageInfo() : m_piTransform(NULL)
	{
		CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)(&m_piTransform));
		CoCreateInstance( ImageVideoFile::CLSID_CImageVideoFile3 , NULL, CLSCTX_INPROC, ImageVideoFile::IID_IImageVideoFile4, (void**)(&m_piImageVideoFile));
		if( NULL != m_piImageVideoFile )
		{
			VARIANT vtResizeType;
			vtResizeType.vt = VT_I4;
			vtResizeType.lVal = 0; //не примен€ть resize на открытие
			m_piImageVideoFile->SetAdditionalParam( _T("resizetype"), vtResizeType );
			VARIANT vtWithAlpha;
			vtWithAlpha.vt = VT_BOOL;
			vtWithAlpha.lVal = VARIANT_TRUE; //оставл€ть прозрачность
			m_piImageVideoFile->SetAdditionalParam( _T("drawwithalpha"), vtWithAlpha );
		}
		Clear();
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEINTERFACE(m_piTransform);
		if( NULL != m_piImageVideoFile )
			m_piImageVideoFile->CloseFile();
		RELEASEINTERFACE(m_piImageVideoFile);
	}

public:
	STDMETHOD(Open)( BSTR bstrFilename )
	{
		if( m_sCurFilename == CString(bstrFilename) )
			return S_OK;

		Clear();

		m_sCurFilename = bstrFilename;
		if( NULL == m_piTransform || NULL == m_piImageVideoFile )
			return IMAGECONVERTER_UNEXPECTED_ERROR;
		HRESULT hRes = S_OK;

		CImageFileFormatChecker checker;
		if(checker.isPngFile(m_sCurFilename))
		{
			m_bIsApng = true;
			//ќткрываем файл
			BSTR bstrCurFilename = m_sCurFilename.AllocSysString();
			hRes = m_piImageVideoFile->OpenFile( bstrCurFilename );
			SysFreeString( bstrCurFilename );
			if( SUCCEEDED(hRes) )
			{				
				//запоминаем параметры
				long nInternalImageVideoFileType = 0;
				m_piImageVideoFile->get_FileType( &nInternalImageVideoFileType );

				VARIANT vtGetAddPar;
				if( 2 == nInternalImageVideoFileType ) //APNG
				{
					m_nApngType = APNG_FILETYPE;
					m_piImageVideoFile->GetAdditionalParam( _T("framescount"), &vtGetAddPar );
					m_nApngFrameCount = vtGetAddPar.lVal;
					m_piImageVideoFile->GetAdditionalParam( _T("SourceOriginalWidth"), &vtGetAddPar );
					m_nApngWidth = vtGetAddPar.lVal;
					m_piImageVideoFile->GetAdditionalParam( _T("SourceOriginalHeight"), &vtGetAddPar );
					m_nApngHeight = vtGetAddPar.lVal;
				}
				else //PNG
				{
					m_nApngType = 0;
					m_nApngFrameCount = 0;
					m_nApngWidth = -1;
					m_nApngHeight = -1;
				}

				//закрываем
				m_piImageVideoFile->CloseFile();
			}
		}
		else
		{
			m_bIsApng = false;

			BSTR bstrCurFilename = m_sCurFilename.AllocSysString();
			VARIANT vSetParam;
			vSetParam.vt = VT_BSTR;
			vSetParam.bstrVal = bstrCurFilename;
			m_piTransform->SetAdditionalParam( L"ImagePath", vSetParam);
			SysFreeString( bstrCurFilename );
		}
		return hRes;
	}
	STDMETHOD(GetFrameCount)( LONG* pnFrameCount )
	{
		if( NULL == pnFrameCount )
			return S_FALSE;
		(*pnFrameCount) = 0;

		if( NULL == m_piTransform )
			return IMAGECONVERTER_UNEXPECTED_ERROR;

		if( true == m_bIsApng )
			(*pnFrameCount) =  m_nApngFrameCount;
		else
		{
			VARIANT vGetParam;
			vGetParam = m_piTransform->GetAdditionalParam( L"CountFrames" );
			(*pnFrameCount) =  vGetParam.lVal;
		}
		return S_OK;
	}
	STDMETHOD(GetFrameSize)( LONG nFrame, LONG* pnWidth, LONG* pnHeight )
	{
		if( NULL == pnWidth ||  NULL == pnHeight  )
			return S_FALSE;
		(*pnWidth) = 0;
		(*pnHeight) = 0;

		if( NULL == m_piTransform )
			return IMAGECONVERTER_UNEXPECTED_ERROR;
		if( true == m_bIsApng )
		{
			if( nFrame >= 0 && nFrame < m_nApngFrameCount )
			{
				(*pnWidth) =  m_nApngWidth;
				(*pnHeight) =  m_nApngHeight;
			}
			else
			{
				(*pnWidth) =  -1;
				(*pnHeight) =  -1;
			}
		}
		else
		{
			VARIANT vSetParam2;
			vSetParam2.vt = VT_I4;
			vSetParam2.lVal = nFrame;
			m_piTransform->SetAdditionalParam( L"ImageFrame", vSetParam2);

			VARIANT vGetParamWidth;
			m_piTransform->raw_GetAdditionalParam( L"FrameWidth", &vGetParamWidth );
			(*pnWidth) =  vGetParamWidth.lVal;
			VARIANT vGetParamHeight;
			m_piTransform->raw_GetAdditionalParam( L"FrameHeight", &vGetParamHeight );
			(*pnHeight) =  vGetParamHeight.lVal;
		}
		return S_OK;
	}
	STDMETHOD(GetFileType)( LONG* pnFileType )
	{
		if( NULL == pnFileType )
			return S_FALSE;
		(*pnFileType) = 0;

		if( NULL == m_piTransform )
			return IMAGECONVERTER_UNEXPECTED_ERROR;

		if( true == m_bIsApng )
			(*pnFileType) =  m_nApngType;
		else
		{
			VARIANT vGetParam;
			vGetParam = m_piTransform->GetAdditionalParam( L"FileType" );
			(*pnFileType) =  vGetParam.lVal;
		}
		return S_OK;
	}
	STDMETHOD(Close)()
	{
		Open( _T("") );
		return S_OK;
	}
private: void Clear()
		 {
			m_bIsApng = false;
			m_nApngWidth = -1;
			m_nApngHeight = -1;
			m_nApngFrameCount = 0;
			m_nApngType = 0;
		 }
};

