#ifndef _BUILD_CLIP_H_
#define _BUILD_CLIP_H_

#include "GraphicsPath.h"

#include "../agg-2.4/include/agg_basics.h"
#include "../agg-2.4/include/agg_rendering_buffer.h"
#include "../agg-2.4/include/agg_rasterizer_scanline_aa.h"
#include "../agg-2.4/include/agg_scanline_storage_aa.h"
#include "../agg-2.4/include/agg_scanline_storage_bin.h"

#include "../agg-2.4/include/agg_pixfmt_rgb.h"
#include "../agg-2.4/include/agg_pixfmt_gray.h"
#include "../agg-2.4/include/agg_alpha_mask_u8.h"
#include "../agg-2.4/include/agg_scanline_u.h"
#include "../agg-2.4/include/agg_scanline_p.h"
#include "../agg-2.4/include/agg_scanline_bin.h"

#include "../agg-2.4/include/agg_scanline_boolean_algebra.h"

#include <string>

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
	CClipMask();
	~CClipMask();

	void Destroy();
	void Reset();
	
public:
	void Create(LONG width, LONG height);
	void ResetClip();

	void GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix);

	agg::rendering_buffer GetRenderingBuffer();
	BYTE* GetMask();
	bool IsClip();
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
	CClip();
	~CClip();

	void Destroy();
	void Reset();
	
public:
	void Create(LONG width, LONG height);
	void ResetClip();

	void GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix);

	agg::rendering_buffer GetRenderingBuffer();
	BYTE* GetMask();
	bool IsClip();
};

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
	CClipMulti();
	~CClipMulti();

	void Create(LONG width, LONG height);
	void GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix);
	
	void Combine(CGraphicsPath* pPath, CMatrix* pMatrix, agg::sbool_op_e op);

	bool IsClip();
	bool IsClip2();

	void Reset();
};

}

#endif // _BUILD_CLIP_H_