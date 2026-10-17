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
#include "Clip.h"

namespace Aggplus
{
	/////////////////////////////////////////////////////////////////////////////////////
	CClipMask::CClipMask() : m_pixf(m_alpha_rbuf)
	{
		m_pMask = NULL;
		m_bIsClip = false;

		m_lWidth	= 0;
		m_lHeight	= 0;
	}

	CClipMask::~CClipMask()
	{
		Destroy();
	}

	void CClipMask::Destroy()
	{
		if (NULL != m_pMask)
			delete [] m_pMask;
	}

	void CClipMask::Reset()
	{
		m_bIsClip = false;
	}
	
	void CClipMask::Create(LONG width, LONG height)
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

	void CClipMask::ResetClip()
	{
	}

	void CClipMask::GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix)
	{
		if (NULL == pPath)
			return;

        memset(m_pMask, 0, m_lWidth * m_lHeight);
		
		m_rasterizer.reset();

		typedef agg::conv_transform<agg::path_storage> trans_type;
		trans_type trans(pPath->m_internal->m_agg_ps, pMatrix->m_internal->m_agg_mtx);

		typedef agg::conv_curve<trans_type> conv_crv_type;
		conv_crv_type c_c_path(trans);
			 
		m_rasterizer.add_path(c_c_path);

        agg::render_scanlines(m_rasterizer, m_sl, m_renderer);

		m_bIsClip = true;
	}

	agg::rendering_buffer CClipMask::GetRenderingBuffer()
	{
		return m_alpha_rbuf;
	}
	BYTE* CClipMask::GetMask()
	{
		return m_pMask;
	}
	bool CClipMask::IsClip()
	{
		return m_bIsClip;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	CClip::CClip() : m_pixf(m_alpha_rbuf)
	{
		m_pMask = NULL;
		m_bIsClip = false;

		m_lWidth	= 0;
		m_lHeight	= 0;
	}

	CClip::~CClip()
	{
		Destroy();
	}

	void CClip::Destroy()
	{
		if (NULL != m_pMask)
			delete [] m_pMask;
	}

	void CClip::Reset()
	{
		m_bIsClip = false;
	}
	
	void CClip::Create(LONG width, LONG height)
	{
	}

	void CClip::ResetClip()
	{
	}

	void CClip::GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix)
	{
		if (NULL == pPath)
			return;

		m_rasterizer.reset();

		typedef agg::conv_transform<agg::path_storage> trans_type;
		trans_type trans(pPath->m_internal->m_agg_ps, pMatrix->m_internal->m_agg_mtx);

		typedef agg::conv_curve<trans_type> conv_crv_type;
		conv_crv_type c_c_path(trans);
			 
		m_rasterizer.add_path(c_c_path);

		m_bIsClip = true;
	}

	agg::rendering_buffer CClip::GetRenderingBuffer()
	{
		return m_alpha_rbuf;
	}
	BYTE* CClip::GetMask()
	{
		return m_pMask;
	}
	bool CClip::IsClip()
	{
		return m_bIsClip;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	CClipMulti::CClipMulti()
	{
		m_bIsClip = false;
		m_bIsClip2 = false;
		m_bIsEmpty = false;
	}
	CClipMulti::~CClipMulti()
	{
	}

	CClipMulti::clip_rasterizer* CClipMulti::GetRasterizer()
	{
		if (!m_bIsClip)
		{
			m_rasterizer.reset();
			return &m_rasterizer;
		}
		return NULL;
	}

	void CClipMulti::Create(LONG width, LONG height)
	{
		m_lWidth = width;
		m_lHeight = height;
		m_rasterizer.clip_box(0, 0, width, height);
		m_bIsClip = false;
		m_bIsClip2 = false;
		m_bIsEmpty = false;
	}

	void CClipMulti::GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix)
	{
		if (NULL == pPath)
			return;

		m_rasterizer.reset();

		typedef agg::conv_transform<agg::path_storage> trans_type;
		trans_type trans(pPath->m_internal->m_agg_ps, pMatrix->m_internal->m_agg_mtx);

		typedef agg::conv_curve<trans_type> conv_crv_type;
		conv_crv_type c_c_path(trans);

		m_rasterizer.add_path(c_c_path);

		GenerateClip2(pPath->m_internal->m_bEvenOdd);
	}

	void CClipMulti::GenerateClip2(bool bEvenOdd)
	{
		m_rasterizer.filling_rule(bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

		m_bIsClip = true;
		m_bIsClip2 = false;
	}
	
	void CClipMulti::Combine(bool bEvenOdd, agg::sbool_op_e op, clip_rasterizer* pRasterizer)
	{
		if (!pRasterizer)
			return;

		if (!m_bIsClip)
			return GenerateClip2(bEvenOdd);

		pRasterizer->filling_rule(bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);
		scanline_type sl1;
		scanline_type sl2;
		scanline_type sl;

		if (!pRasterizer->rewind_scanlines())
		{
			if (op == agg::sbool_and)
				SetEmpty();
			return;
		}

		if (!m_bIsClip2)
		{
			// need to blend with rasterizer
			agg::sbool_combine_shapes_aa(op, m_rasterizer, *pRasterizer, sl1, sl2, sl, m_storage1);

			m_lCurStorage = 1;
		}
		else
		{
			// need to blend with storage
			agg::sbool_combine_shapes_aa(op, (m_lCurStorage == 1) ? m_storage1 : m_storage2, *pRasterizer, sl1, sl2, sl,
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

	bool CClipMulti::IsClip()
	{
		return m_bIsClip;
	}
	bool CClipMulti::IsClip2()
	{
		return m_bIsClip2;
	}
	bool CClipMulti::IsEmpty()
	{
		return m_bIsEmpty;
	}

	void CClipMulti::Reset()
	{
		m_rasterizer.reset();

		//m_storage1.prepare();
		//m_storage2.prepare();

		m_bIsClip = false;
		m_bIsClip2 = false;
		m_bIsEmpty = false;
	}
	void CClipMulti::SetEmpty()
	{
		m_bIsEmpty = true;
		m_bIsClip2 = true;
		m_lCurStorage = 1;
		//m_storage1.prepare();
	}
}
