#pragma once
#include "GraphicsPath.h"

#include "..\include\agg_basics.h"
#include "..\include\agg_rendering_buffer.h"
#include "..\include\agg_rasterizer_scanline_aa.h"
#include "..\include\agg_scanline_storage_aa.h"
#include "..\include\agg_scanline_storage_bin.h"

#include "..\include\agg_pixfmt_rgb.h"
#include "..\include\agg_pixfmt_gray.h"
#include "..\include\agg_alpha_mask_u8.h"
#include "..\include\agg_scanline_u.h"
#include "..\include\agg_scanline_p.h"
#include "..\include\agg_scanline_bin.h"

#include "..\include\agg_scanline_boolean_algebra.h"

namespace Aggplus
{

class CClipMask
{
	typedef agg::renderer_base<agg::pixfmt_gray8> ren_base;
    typedef agg::renderer_scanline_aa_solid<ren_base> renderer;

	friend class CGraphicsPath;
public:
	BYTE*					m_pMask;
	LONG					m_lWidth;
	LONG					m_lHeight;

	agg::rendering_buffer	m_alpha_rbuf;

	agg::rasterizer_scanline_aa<> m_rasterizer;

	agg::pixfmt_gray8		m_pixf;
	ren_base				m_base_renderer;
	renderer				m_renderer;

	agg::scanline_p8		m_sl;

	bool					m_bIsClip;

public:
	CClipMask() : m_pixf(m_alpha_rbuf)
	{
		m_pMask = NULL;
		m_bIsClip = false;

		m_lWidth	= 0;
		m_lHeight	= 0;
	}

	~CClipMask()
	{
		Destroy();
	}

	void Destroy()
	{
		if (NULL != m_pMask)
			delete [] m_pMask;
	}

	void Reset()
	{
		m_bIsClip = false;
	}
	
public:
	void Create(LONG width, LONG height)
	{
		Destroy();
		m_pMask = new BYTE[width * height];

		m_alpha_rbuf.attach(m_pMask, width, height, width);
		m_lWidth = width;
		m_lHeight = height;

		m_pixf.attach(m_alpha_rbuf);
		m_base_renderer.attach(m_pixf);		
		m_renderer.attach(m_base_renderer);

		m_renderer.color(agg::gray8(0xFF, 0xFF));
	}

	void ResetClip()
	{
	}

	void GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix)
	{
		if (NULL == pPath)
			return;

        memset(m_pMask, 0, m_lWidth * m_lHeight);
		
		m_rasterizer.reset();

		typedef agg::conv_transform<agg::path_storage> trans_type;
		trans_type trans(pPath->m_agg_ps, pMatrix->m_agg_mtx);

		typedef agg::conv_curve<trans_type> conv_crv_type;
		conv_crv_type c_c_path(trans);
			 
		m_rasterizer.add_path(c_c_path);

        agg::render_scanlines(m_rasterizer, m_sl, m_renderer);

		m_bIsClip = true;
	}

	agg::rendering_buffer GetRenderingBuffer()
	{
		return m_alpha_rbuf;
	}
	BYTE* GetMask()
	{
		return m_pMask;
	}
	bool IsClip()
	{
		return m_bIsClip;
	}
};

class CClip
{
	typedef agg::renderer_base<agg::pixfmt_gray8> ren_base;
    typedef agg::renderer_scanline_aa_solid<ren_base> renderer;

	friend class CGraphicsPath;
public:
	BYTE*					m_pMask;
	LONG					m_lWidth;
	LONG					m_lHeight;

	agg::rendering_buffer	m_alpha_rbuf;

	agg::rasterizer_scanline_aa<> m_rasterizer;

	agg::pixfmt_gray8		m_pixf;
	ren_base				m_base_renderer;
	renderer				m_renderer;

	agg::scanline_p8		m_sl;

	bool					m_bIsClip;

public:
	CClip() : m_pixf(m_alpha_rbuf)
	{
		m_pMask = NULL;
		m_bIsClip = false;

		m_lWidth	= 0;
		m_lHeight	= 0;
	}

	~CClip()
	{
		Destroy();
	}

	void Destroy()
	{
		if (NULL != m_pMask)
			delete [] m_pMask;
	}

	void Reset()
	{
		m_bIsClip = false;
	}
	
public:
	void Create(LONG width, LONG height)
	{
	}

	void ResetClip()
	{
	}

	void GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix)
	{
		if (NULL == pPath)
			return;

		m_rasterizer.reset();

		typedef agg::conv_transform<agg::path_storage> trans_type;
		trans_type trans(pPath->m_agg_ps, pMatrix->m_agg_mtx);

		typedef agg::conv_curve<trans_type> conv_crv_type;
		conv_crv_type c_c_path(trans);
			 
		m_rasterizer.add_path(c_c_path);

		m_bIsClip = true;
	}

	agg::rendering_buffer GetRenderingBuffer()
	{
		return m_alpha_rbuf;
	}
	BYTE* GetMask()
	{
		return m_pMask;
	}
	bool IsClip()
	{
		return m_bIsClip;
	}
};

//class CClipMulti
//{
//	typedef agg::scanline_p8 scanline_type;
//
//public:
//	agg::scanline_storage_aa8* m_storage;
//
//public:
//	CClipMulti()
//	{
//		m_storage = NULL;
//	}
//	~CClipMulti()
//	{
//		if (NULL != m_storage)
//		{
//			delete m_storage;
//		}
//	}
//
//	void Create(LONG width, LONG height)
//	{
//	}
//
//	void GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix)
//	{
//		if (NULL != m_storage)
//		{
//			delete m_storage;
//		}
//		m_storage = new agg::scanline_storage_aa8();
//		
//		agg::rasterizer_scanline_aa<> rasterizer;
//
//		typedef agg::conv_transform<agg::path_storage> trans_type;
//		trans_type trans(pPath->m_agg_ps, pMatrix->m_agg_mtx);
//
//		typedef agg::conv_curve<trans_type> conv_crv_type;
//		conv_crv_type c_c_path(trans);
//			 
//		rasterizer.add_path(c_c_path);
//
//		scanline_type sl; 
//		agg::render_scanlines(rasterizer, sl, *m_storage);
//	}
//	
//	void Combine(CGraphicsPath* pPath, CMatrix* pMatrix, agg::sbool_op_e op)
//	{
//		if (NULL == m_storage)
//			return GenerateClip(pPath, pMatrix);
//
//		agg::scanline_storage_aa8* storage1 = m_storage;
//		agg::scanline_storage_aa8 storage2;
//
//		m_storage = new agg::scanline_storage_aa8();
//
//		agg::rasterizer_scanline_aa<> rasterizer;
//
//		typedef agg::conv_transform<agg::path_storage> trans_type;
//		trans_type trans(pPath->m_agg_ps, pMatrix->m_agg_mtx);
//
//		typedef agg::conv_curve<trans_type> conv_crv_type;
//		conv_crv_type c_c_path(trans);
//			 
//		rasterizer.add_path(c_c_path);
//
//		scanline_type sl; 
//		agg::render_scanlines(rasterizer, sl, storage2);
//
//		scanline_type sl1;
//		scanline_type sl2;
//		agg::sbool_combine_shapes_aa(op, *storage1, storage2, sl1, sl2, sl, *m_storage);
//
//		delete storage1;
//	}
//
//	bool IsClip()
//	{
//		return (NULL != m_storage);
//	}
//
//	void Reset()
//	{
//		if (NULL != m_storage)
//		{
//			m_storage->prepare();
//			m_storage = NULL;
//		}
//	}
//};

class CClipMulti
{
	typedef agg::scanline_p8 scanline_type;

public:
	agg::rasterizer_scanline_aa<> m_rasterizer;
	
	agg::scanline_storage_aa8 m_storage1;
	agg::scanline_storage_aa8 m_storage2;

	long m_lCurStorage;
	
	bool m_bIsClip;
	bool m_bIsClip2;

public:
	CClipMulti()
	{
		m_bIsClip = false;
		m_bIsClip2 = false;
	}
	~CClipMulti()
	{
	}

	void Create(LONG width, LONG height)
	{
		m_rasterizer.clip_box(0, 0, width, height);
		m_bIsClip = false;
		m_bIsClip2 = false;
	}

	void GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix)
	{
		if (NULL == pPath)
			return;

		m_rasterizer.reset();

		typedef agg::conv_transform<agg::path_storage> trans_type;
		trans_type trans(pPath->m_agg_ps, pMatrix->m_agg_mtx);

		typedef agg::conv_curve<trans_type> conv_crv_type;
		conv_crv_type c_c_path(trans);
			 
		m_rasterizer.add_path(c_c_path);

		m_bIsClip = true;
		m_bIsClip2 = false;
	}
	
	void Combine(CGraphicsPath* pPath, CMatrix* pMatrix, agg::sbool_op_e op)
	{
		if (!m_bIsClip)
			return GenerateClip(pPath, pMatrix);

		if (!m_bIsClip2)
		{
			// смешивать надо с растерайзером
			agg::rasterizer_scanline_aa<> rasterizer;

			typedef agg::conv_transform<agg::path_storage> trans_type;
			trans_type trans(pPath->m_agg_ps, pMatrix->m_agg_mtx);

			typedef agg::conv_curve<trans_type> conv_crv_type;
			conv_crv_type c_c_path(trans);
				 
			rasterizer.add_path(c_c_path);
			rasterizer.filling_rule(pPath->m_bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

			scanline_type sl1; 
			scanline_type sl2;
			scanline_type sl;

			agg::sbool_combine_shapes_aa(op, m_rasterizer, rasterizer, sl1, sl2, sl, m_storage1);

			m_lCurStorage = 1;
		}
		else
		{
			// надо смешивать со стораджем
			agg::rasterizer_scanline_aa<> rasterizer;

			typedef agg::conv_transform<agg::path_storage> trans_type;
			trans_type trans(pPath->m_agg_ps, pMatrix->m_agg_mtx);

			typedef agg::conv_curve<trans_type> conv_crv_type;
			conv_crv_type c_c_path(trans);
				 
			rasterizer.add_path(c_c_path);
			rasterizer.filling_rule(pPath->m_bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

			scanline_type sl1; 
			scanline_type sl2;
			scanline_type sl; 

			agg::sbool_combine_shapes_aa(op, rasterizer, (m_lCurStorage == 1) ? m_storage1 : m_storage2, sl1, sl2, sl, 
														(m_lCurStorage == 1) ? m_storage2 : m_storage1);

			if (1 == m_lCurStorage)
			{
				//m_storage1.prepare();
				m_lCurStorage = 2;
			}
			else
			{
				//m_storage2.prepare();
				m_lCurStorage = 1;
			}			
		}
		m_bIsClip2 = true;
	}

	bool IsClip()
	{
		return m_bIsClip;
	}
	bool IsClip2()
	{
		return m_bIsClip2;
	}

	void Reset()
	{
		m_rasterizer.reset();

		//m_storage1.prepare();
		//m_storage2.prepare();

		m_bIsClip = false;
		m_bIsClip2 = false;
	}
};

}