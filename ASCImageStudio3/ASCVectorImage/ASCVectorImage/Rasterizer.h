// Rasterizer.h : Declaration of the Rasterizer

#pragma once
#include "resource.h"       // main symbols


#include "svg/agg_svg_exception.h"
#include "svg/agg_svg_parser.h"
#include "svg/agg_svg_indexation_interpreter.h"
#include "svg/agg_svg_rendering_interpreter.h"
#include "svg/agg_svg_attributes_map.h"
#include "svg/agg_svg_rasterizer.h"

// IRasterizer
[
	object,
	uuid("A4353DB2-D2BF-40D4-90EF-B00DBEC9B6E1"),
	dual,	helpstring("IRasterizer Interface"),
	pointer_default(unique)
]
__interface IRasterizer : IDispatch
{
	[id(99)]  HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(100)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(101)] HRESULT Rasterize([in] BSTR src, [in] long scrType, [in] long resW, [in] long resH, [out, retval] IUnknown** pInterface);
	
};



// Rasterizer

[
	coclass,
	threading("apartment"),
	vi_progid("AVSVectorImage.Rasterizer"),
	progid("AVSVectorImage.Rasterizer.1"),
	version(1.0),
	uuid("94A0E1D4-E29D-4061-AB46-6928D348C4DD"),
	helpstring("Rasterizer Class")
]
class ATL_NO_VTABLE Rasterizer : 
	public IRasterizer
{
public:
	Rasterizer():m_attributes(m_settings)
	{
	}
	~Rasterizer()
	{
		Clear();
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}

	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}


	/*STDMETHOD(Open)(BSTR FilePath, VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;

		m_attributes.initial_zoom(); 
		m_storage.clear();
		reset();

		agg::svg::parser p(m_storage, m_attr_map);
		p.parse(FilePath);
		indexation();


		*Success = VARIANT_TRUE;

		return S_OK;
	}
	
	STDMETHOD(GetResult)(long resW, long resH, VARIANT* Result)
	{
		if (!Result || resW<1 || resH<1)
			return S_OK;

		MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;

		CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
		if (NULL == pMediaData)
		{
			return FALSE;
		}

		// specify settings
		pMediaData->put_Width(resW);
		pMediaData->put_Height(resH);
		pMediaData->put_AspectRatioX(resW);
		pMediaData->put_AspectRatioY(resH);
		pMediaData->put_Interlaced(VARIANT_FALSE);
		pMediaData->put_Stride(0, 4*resW);
		pMediaData->AllocateBuffer(4*resW*resH);

		// allocate necesasry buffer
		BYTE* pBufferPtr = 0;
		long nCreatedBufferSize = 0;
		pMediaData->get_Buffer(&pBufferPtr);
		pMediaData->get_BufferSize(&nCreatedBufferSize);
		pMediaData->put_Plane(0, pBufferPtr);

		// check for valid created buffer
		if (!pBufferPtr || nCreatedBufferSize != 4*resW*resH)
		{
			pMediaData->Release();
			return S_FALSE;
		}

		int strd = -4*resW;
		m_frame_buffer.create(resW, resH, false,strd , pBufferPtr);
		m_rasterizer.get_rasterizer().gamma(agg::gamma_none());

		agg::svg::renderer_rgba   renderer(m_rasterizer, m_frame_buffer);

		m_rasterizer.gamma(1.0);
		m_settings.gamma(1.0);

		m_attributes.window(0, 0,  2*(0+resW), 2*(0+resH));

		agg::svg::rendering_interpreter rin(  m_pipeline, m_attributes, renderer, m_id2elem_map,m_gradient_lut_cache);
		rin.ignore_viewBox(true);

		m_storage.traverse(rin);

		assert(0 == m_attributes.num_sessions());

		if (m_settings.gamma() != 1.0)
		{
			m_frame_buffer.pixfmt().apply_gamma_inv(m_settings.gamma_lut());
		}	

		m_rasterizer.reset_clipping();

		Result->vt = VT_UNKNOWN;
		Result->punkVal = pMediaData;

		return S_OK;
	}*/

	STDMETHOD(Rasterize)(BSTR src, long scrType, long resW, long resH, IUnknown** pInterface)
	{
		*pInterface = NULL;

		m_attributes.initial_zoom(); 
		m_storage.clear();
		reset();

		agg::svg::parser p(m_storage, m_attr_map);

		switch(scrType)
		{
		case 0:    //   Load SVG from file
			p.parse(src);
			break; 
		case 1:    //   Load SVG from buffer
			{
				_bstr_t data( src, true );
				p.parse_data(data, (size_t)data.length());
			}
		    break;
		default:
			return S_FALSE;
		}
			
		// save interface
		*pInterface = Rasterize( p, resW, resH );

		if( !(*pInterface) )
			return S_FALSE;

		return S_OK;
	}
private:
	agg::svg::dom_storage             m_storage;
	agg::svg::dom_storage::map_type   m_id2elem_map;
	agg::svg::global_settings         m_settings;
	agg::svg::attributes              m_attributes;
	agg::svg::attributes_map          m_attr_map;

	agg::svg::pipeline                m_pipeline;
	agg::svg::gradient_lut_cache      m_gradient_lut_cache;

	void  Clear()
	{
		m_storage.clear();	
		reset();
	}

	void indexation()
	{
		agg::svg::indexation_interpreter consumer(m_id2elem_map);

		m_storage.traverse(consumer);

		m_id2elem_map.sort_state(agg::svg::on);	
	}

	void reset()
	{
		m_id2elem_map.clear();
		m_attributes.clear();
	}


	//  rasterizators
	MediaCore::IAVSUncompressedVideoFrame* Rasterize( agg::svg::parser& p, long resW, long resH )
	{
		MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;

		CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
		if (NULL == pMediaData)
		{
			return NULL;
		}

		indexation();

		int svg_w = (resW<1)?m_storage.get_width():resW;
		if (svg_w<1)
			svg_w = 100;

		int svg_h = (resH<1)?m_storage.get_height():resH;
		if (svg_h<1)
			svg_h = 100;

		// specify settings
		pMediaData->put_Width(svg_w);
		pMediaData->put_Height(svg_h);
		pMediaData->put_AspectRatioX(svg_w);
		pMediaData->put_AspectRatioY(svg_h);
		pMediaData->put_Interlaced(VARIANT_FALSE);
		pMediaData->put_Stride(0, 4*svg_w);
		pMediaData->AllocateBuffer(4*svg_w*svg_h);

		// allocate necesasry buffer
		BYTE* pBufferPtr = 0;
		long nCreatedBufferSize = 0;
		pMediaData->get_Buffer(&pBufferPtr);
		pMediaData->get_BufferSize(&nCreatedBufferSize);
		pMediaData->put_Plane(0, pBufferPtr);

		// check for valid created buffer
		if (!pBufferPtr || nCreatedBufferSize != 4*svg_w*svg_h)
		{
			pMediaData->Release();
			return NULL;
		}

		memset(pBufferPtr, 0, 4*svg_w*svg_h);

		int strd = -4*svg_w;
		m_frame_buffer.create(svg_w, svg_h, false,strd , pBufferPtr);
		m_rasterizer.get_rasterizer().gamma(agg::gamma_none());

		agg::svg::renderer_rgba   renderer(m_rasterizer, m_frame_buffer);

		m_rasterizer.gamma(1.0);
		m_settings.gamma(1.0);

		m_attributes.window(0, 0,  2*(0+svg_w), 2*(0+svg_h));

		agg::svg::rendering_interpreter rin(  m_pipeline, m_attributes, renderer, m_id2elem_map,m_gradient_lut_cache);
		rin.ignore_viewBox(true);

		m_storage.traverse(rin);

		assert(0 == m_attributes.num_sessions());

		if (m_settings.gamma() != 1.0)
		{
			m_frame_buffer.pixfmt().apply_gamma_inv(m_settings.gamma_lut());
		}	

		m_rasterizer.reset_clipping();


		// save interface
		return pMediaData;
	}

	
	agg::svg::frame_buffer_rgba       m_frame_buffer;
	agg::svg::rasterizer              m_rasterizer;
};
