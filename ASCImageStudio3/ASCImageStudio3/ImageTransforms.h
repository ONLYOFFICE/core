#pragma once

#include "resource.h"
#include <Utilities.h>
#include <StreamUtils.h>
#include <XmlUtils.h>
#include <GdiPlusEx.h>
#include <Imaging.h>
#include "ImageTransformsProcessor.h"
	
[object, uuid("138755EF-0021-4f0a-80B8-B242DB2F43D0"), oleautomation, dual, pointer_default(unique)]
__interface IImageTransforms : IDispatch
{
	[id(101)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(102)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(201)] HRESULT SetXml([in] BSTR Xml, [out, retval] VARIANT_BOOL* Success);
	[id(202)] HRESULT SetTime([in] double Time);
	[id(203)] HRESULT SetSource([in] long Index, [in] VARIANT Source);
	[id(204)] HRESULT GetResult([in] long Index, [out] VARIANT* pResult);
	
	[id(301)] HRESULT Transform([out, retval] VARIANT_BOOL* Success);
};
	
[coclass, uuid("006DD71E-C9EB-494d-BDEF-5138A587F598"), threading(apartment), vi_progid("AVSImageStudio.ImageTransforms"), progid("AVSImageStudio.ImageTransforms.1"), version(1.0)]
class ATL_NO_VTABLE ImageTransforms : public IImageTransforms, public ImageStudio::Transforms::SerializedObjectTransformerEvents
{
	CGdiPlusInit m_oGdiPlusInit;

	ImageStudio::Serialize::SerializedObjectNamesManager m_oTransformNames;
	ImageStudio::Serialize::SerializedObjectList m_oTransformSerializer;
	
	ImageStudio::Transforms::SerializedObjectTransformer m_oStreamTransformer;
	
	ImageStudio::Transforms::AnimImageInfo m_oAnimImageInfo;

public:
	
	ImageTransforms()
	{
		// выставляем указатели менеджерам памяти и событий
		m_oTransformSerializer.SetNamesManager(&m_oTransformNames);
		m_oStreamTransformer.SetEventReceiver(this);
	}
	~ImageTransforms()
	{
		ImageStudio::Paint::Effects::OldPhoto::FreeMasks();
	}
	
	virtual BOOL OnProgress(double dPercents)
	{
		// TODO: raise the progress event

		return TRUE;
	}
	
public:
	
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		// инициализируем gdiplus
		if (!m_oGdiPlusInit.IsValid())
		{
			if (!m_oGdiPlusInit.Init())
				return S_OK;
		}

		CComBSTR ParamStr = ParamName;

		if( ParamStr == L"ImagePath" )
		{
			if( VT_EMPTY == ParamValue.vt || VT_BSTR == ParamValue.vt )
			{
				m_oAnimImageInfo.SetAnimImage( ParamValue.bstrVal );
			}
		}
		else
		if( ParamStr == L"ImageFrame" )
		{
			m_oAnimImageInfo.m_nCurFrame = ParamValue.lVal;
		}
		else
		if( ParamStr == L"OldPhotoMasksFolder" )
		{
			if(VT_BSTR == ParamValue.vt )
			{
				ImageStudio::Paint::Effects::OldPhoto::InitMasks( CString(ParamValue.bstrVal) );
			}
		}
		else
		if( ParamStr == L"SVGWidth" )
		{
			if( VT_I4 == ParamValue.vt )
				m_oStreamTransformer.m_nSVGWidth = ParamValue.intVal;
		}
		else
		if( ParamStr == L"SVGHeight" )
		{
			if( VT_I4 == ParamValue.vt )
				m_oStreamTransformer.m_nSVGHeight = ParamValue.intVal;
		}
		else
		if( ParamStr == L"LoadSVG" )
		{
			if(VT_BSTR == ParamValue.vt )
			{
				m_oStreamTransformer.LoadSVGFromXML( ParamValue.bstrVal );
			}
		}

		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		// инициализируем gdiplus
		if (!m_oGdiPlusInit.IsValid())
		{
			if (!m_oGdiPlusInit.Init())
				return S_OK;
		}

		CComBSTR ParamStr = ParamName;
		double tmpVals[2] = {0, 0};

		if (ParamStr == L"EffectsCount")
		{
			(*ParamValue).lVal = m_oTransformSerializer.GetCount();
		}
		else if (ParamStr == L"AutoContrast")
		{
			IUnknown* pSource = m_oStreamTransformer.GetMediaData( 0 );
			ImageStudio::Transforms::Core::COM::COM_GetAutoContrast( pSource, &(*ParamValue).dblVal );
		}
		else if (ParamStr == L"AutoLevelR")
		{
			IUnknown* pSource = m_oStreamTransformer.GetMediaData( 0 );
			ImageStudio::Transforms::Core::COM::COM_GetAutoLevels( pSource, &(*ParamValue).dblVal, &tmpVals[0], &tmpVals[1] );
		}
		else if (ParamStr == L"AutoLevelG")
		{
			IUnknown* pSource = m_oStreamTransformer.GetMediaData( 0 );
			ImageStudio::Transforms::Core::COM::COM_GetAutoLevels( pSource, &tmpVals[0], &(*ParamValue).dblVal, &tmpVals[1] );
		}
		else if (ParamStr == L"AutoLevelB")
		{
			IUnknown* pSource = m_oStreamTransformer.GetMediaData( 0 );
			ImageStudio::Transforms::Core::COM::COM_GetAutoLevels( pSource, &tmpVals[0], &tmpVals[1], &(*ParamValue).dblVal );
		}
		else if (ParamStr == L"AutoBrightness")
		{
			IUnknown* pSource = m_oStreamTransformer.GetMediaData( 0 );
			ImageStudio::Transforms::Core::COM::COM_GetAutoBrightness( pSource, &(*ParamValue).dblVal );
		}
		else if (ParamStr == L"AutoGamma")
		{
			IUnknown* pSource = m_oStreamTransformer.GetMediaData( 0 );
			ImageStudio::Transforms::Core::COM::COM_GetAutoGamma( pSource, &(*ParamValue).dblVal );
		}
		else if (ParamStr == L"AutoHue")
		{
			IUnknown* pSource = m_oStreamTransformer.GetMediaData( 0 );
			ImageStudio::Transforms::Core::COM::COM_GetAutoHue( pSource, &(*ParamValue).dblVal );
		}
		else if (ParamStr == L"AutoSaturation")
		{
			IUnknown* pSource = m_oStreamTransformer.GetMediaData( 0 );
			ImageStudio::Transforms::Core::COM::COM_GetAutoSaturation( pSource, &(*ParamValue).dblVal );
		}
		else if (ParamStr == L"AutoPosterize")
		{
			IUnknown* pSource = m_oStreamTransformer.GetMediaData( 0 );
			ImageStudio::Transforms::Core::COM::COM_GetAutoPosterize( pSource, &(*ParamValue).dblVal );
		}
		else if (ParamStr == L"CountFrames")
		{
			ParamValue->vt = VT_I4;
			ParamValue->lVal = m_oAnimImageInfo.GetCountFrames();
		}
		else if (ParamStr == L"FrameWidth")
		{
			ParamValue->vt = VT_I4;
			ParamValue->lVal = m_oAnimImageInfo.GetFrameWidth();
		}
		else if (ParamStr == L"FrameHeight")
		{
			ParamValue->vt = VT_I4;
			ParamValue->lVal = m_oAnimImageInfo.GetFrameHeight();
		}
		else if (ParamStr == L"FrameDelay")
		{
			ParamValue->vt = VT_I4;
			ParamValue->lVal = m_oAnimImageInfo.GetFrameDelay( ParamValue->lVal );
		}
		else if (ParamStr == L"LoopCount")
		{
			ParamValue->vt = VT_I4;
			ParamValue->lVal = m_oAnimImageInfo.GetLoopCount();
		}
		else if( ParamStr == L"FileType" )
		{
			ParamValue->vt = VT_I4;
			ParamValue->lVal = m_oAnimImageInfo.GetFileType();
		}

		return S_OK;
	}
	
	STDMETHOD(SetXml)(BSTR Xml, VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;

		// выставляем xml для последующих трансформаций
		if (m_oTransformSerializer.FromXmlString(CString(Xml), TRUE))
			*Success = VARIANT_TRUE;

		return S_OK;
	}
	STDMETHOD(SetTime)(double Time)
	{
		m_oStreamTransformer.SetTime(Time);

		return S_OK;
	}
	STDMETHOD(SetSource)(long Index, VARIANT Source)
	{
		// проверка на пересылку MemoryStorage
		if( Source.punkVal )
		{
			IMemoryStorage* pMemoryStorage = NULL;
			Source.punkVal->QueryInterface( __uuidof(IMemoryStorage), (void**)&pMemoryStorage );
			if( pMemoryStorage )
			{
				m_oStreamTransformer.SetMemoryStorage( pMemoryStorage );
				pMemoryStorage->Release();
				return S_OK;
			}

			/*
			FileCache::IAVSImageFilesCache* pFileCache = NULL;
			Source.punkVal->QueryInterface( __uuidof(FileCache::IAVSImageFilesCache), (void**)&pFileCache );
			if( pFileCache )
			{
				m_oStreamTransformer.SetFileCache( pFileCache );
				pFileCache->Release();
				return S_OK;
			}
			*/
		}

		// инициализируем gdiplus
		if (!m_oGdiPlusInit.IsValid())
		{
			if (!m_oGdiPlusInit.Init())
				return S_OK;
		}

		if( !m_oStreamTransformer.SetMediaData( Index, Source.punkVal ) )
			return S_FALSE;

		return S_OK;
	}
	STDMETHOD(GetResult)(long Index, VARIANT* Result)
	{
		if (!Result)
			return S_OK;

		Result->vt = VT_UNKNOWN;
		Result->punkVal = m_oStreamTransformer.GetMediaData( Index );

		return S_OK;
	}
	
	STDMETHOD(Transform)(VARIANT_BOOL* Success)
	{
		//ImageStudio::SimpleTimer timer;

		*Success = VARIANT_FALSE;

		// инициализируем gdiplus
		if (!m_oGdiPlusInit.IsValid())
		{
			if (!m_oGdiPlusInit.Init())
				return S_OK;
		}

		// производим трансформацию
		if (m_oStreamTransformer.Process(m_oTransformSerializer))
			*Success = VARIANT_TRUE;

		return S_OK;
	}
};
