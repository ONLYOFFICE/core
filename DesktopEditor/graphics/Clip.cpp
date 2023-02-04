/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
	}
	CClipMulti::~CClipMulti()
	{
	}

	void CClipMulti::Create(LONG width, LONG height)
	{
		m_lWidth = width;
		m_lHeight = height;
		m_rasterizer.clip_box(0, 0, width, height);
		m_bIsClip = false;
		m_bIsClip2 = false;
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
		m_rasterizer.filling_rule(pPath->m_internal->m_bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

		m_bIsClip = true;
		m_bIsClip2 = false;
	}
	
	void CClipMulti::Combine(CGraphicsPath* pPath, CMatrix* pMatrix, agg::sbool_op_e op)
	{
		if (!m_bIsClip)
			return GenerateClip(pPath, pMatrix);

		if (!m_bIsClip2)
		{
			// смешивать надо с растерайзером
			agg::rasterizer_scanline_aa<> rasterizer;
			rasterizer.clip_box(0, 0, m_lWidth, m_lHeight);

			typedef agg::conv_transform<agg::path_storage> trans_type;
			trans_type trans(pPath->m_internal->m_agg_ps, pMatrix->m_internal->m_agg_mtx);

			typedef agg::conv_curve<trans_type> conv_crv_type;
			conv_crv_type c_c_path(trans);
				 
			rasterizer.add_path(c_c_path);
			rasterizer.filling_rule(pPath->m_internal->m_bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

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
			rasterizer.clip_box(0, 0, m_lWidth, m_lHeight);

			typedef agg::conv_transform<agg::path_storage> trans_type;
			trans_type trans(pPath->m_internal->m_agg_ps, pMatrix->m_internal->m_agg_mtx);

			typedef agg::conv_curve<trans_type> conv_crv_type;
			conv_crv_type c_c_path(trans);
				 
			rasterizer.add_path(c_c_path);
			rasterizer.filling_rule(pPath->m_internal->m_bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

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

	bool CClipMulti::IsClip()
	{
		return m_bIsClip;
	}
	bool CClipMulti::IsClip2()
	{
		return m_bIsClip2;
	}

	void CClipMulti::Reset()
	{
		m_rasterizer.reset();

		//m_storage1.prepare();
		//m_storage2.prepare();

		m_bIsClip = false;
		m_bIsClip2 = false;
	}
}
