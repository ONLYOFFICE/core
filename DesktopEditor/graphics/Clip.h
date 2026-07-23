/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _BUILD_CLIP_H_
#define _BUILD_CLIP_H_

#include "GraphicsPath_private.h"

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

	agg::rasterizer_scanline_aa<agg::rasterizer_sl_clip_dbl> m_rasterizer;

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

	agg::rasterizer_scanline_aa<agg::rasterizer_sl_clip_dbl> m_rasterizer;

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
public:
	typedef agg::scanline_p8 scanline_type;
	typedef agg::rasterizer_scanline_aa<agg::rasterizer_sl_clip_dbl> clip_rasterizer;

	clip_rasterizer m_rasterizer;
	
	agg::scanline_storage_aa8 m_storage1;
	agg::scanline_storage_aa8 m_storage2;

	long m_lCurStorage;
	
	bool m_bIsClip;
	bool m_bIsClip2;
	bool m_bIsEmpty;

	LONG m_lWidth;
	LONG m_lHeight;

public:
	CClipMulti();
	~CClipMulti();

	clip_rasterizer* GetRasterizer();

	void Create(LONG width, LONG height);
	void GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix);
	void GenerateClip2(bool bEvenOdd);
	
	void Combine(bool bEvenOdd, agg::sbool_op_e op, clip_rasterizer* pRasterizer);

	bool IsClip();
	bool IsClip2();
	bool IsEmpty();

	void Reset();
	void SetEmpty();
};

}

#endif // _BUILD_CLIP_H_
