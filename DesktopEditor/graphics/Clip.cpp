/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
	CClipMulti::CClipMulti()
	{
		m_bIsClip = false;
		m_bIsClip2 = false;
	}

	CClipMulti::~CClipMulti()
	{
	}

	/**
	 * @brief CClipMulti::GetRasterizer
	 *
	 * @return Ponter to new rasterizer if there was no
	 *   clip yet, otherwise - nullptr
	 */
	CClipMulti::clip_rasterizer* CClipMulti::GetRasterizer()
	{
		if (!m_bIsClip)
		{
			m_rasterizer.reset();
			return &m_rasterizer;
		}

		return nullptr;
	}

	void CClipMulti::Create(LONG width, LONG height)
	{
		m_lWidth = width;
		m_lHeight = height;
		m_rasterizer.clip_box(0, 0, width, height);
		m_bIsClip = false;
		m_bIsClip2 = false;
	}

	/**
	 * @brief CClipMulti::GenerateClip
	 * @param pPath - vector graphics path for clip
	 * @param pMatrix - transform matrix
	 *
	 * Create new rasterizer from path with transform maatrix and start clip.
	 */
	void CClipMulti::GenerateClip(CGraphicsPath* pPath, CMatrix* pMatrix)
	{
		if (pPath == nullptr)
			return;

		m_rasterizer.reset();

		typedef agg::conv_transform<agg::path_storage> trans_type;
		trans_type trans(pPath->m_internal->m_agg_ps, pMatrix->m_internal->m_agg_mtx);

		typedef agg::conv_curve<trans_type> conv_crv_type;
		conv_crv_type c_c_path(trans);

		m_rasterizer.add_path(c_c_path);

		GenerateClip2(pPath->m_internal->m_bEvenOdd);
	}

	/**
	 * @brief CClipMulti::GenerateClip2
	 * @param bEvenOdd - fill mode
	 *
	 * Set fill mode and start clip 1.
	 */
	void CClipMulti::GenerateClip2(bool bEvenOdd)
	{
		m_rasterizer.filling_rule(bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

		m_bIsClip = true;
		m_bIsClip2 = false;
	}
	
	/**
	 * @brief CClipMulti::Combine
	 * @param bEvenOdd - fill mode
	 * @param op - boolean operation
	 * @param pRasterizer - another rasterizer for clip
	 *
	 * Performs the boolean operation. If there is no clip - create it.
	 * If the first paart of the clip, execute it in storage. Perform
	 * the second clip with storage.
	 */
	void CClipMulti::Combine(bool bEvenOdd, agg::sbool_op_e op, clip_rasterizer* pRasterizer)
	{
		if (!pRasterizer)
			return;

		if (!m_bIsClip)
			return GenerateClip2(bEvenOdd);

		if (!m_bIsClip2)
		{
			pRasterizer->filling_rule(bEvenOdd ? agg::fill_even_odd : agg::fill_non_zero);

			scanline_type sl1;
			scanline_type sl2;
			scanline_type sl;

			agg::sbool_combine_shapes_aa(op, m_rasterizer, *pRasterizer, sl1, sl2, sl, m_storage1);

			m_lCurStorage = 1;
		}
		else
		{
			pRasterizer->filling_rule(op ? agg::fill_even_odd : agg::fill_non_zero);

			scanline_type sl1;
			scanline_type sl2;
			scanline_type sl;

			agg::sbool_combine_shapes_aa(op, *pRasterizer, (m_lCurStorage == 1) ? m_storage1 : m_storage2, sl1, sl2, sl,
														   (m_lCurStorage == 1) ? m_storage2 : m_storage1);

			if (1 == m_lCurStorage)
				m_lCurStorage = 2;
			else
				m_lCurStorage = 1;
		}
		m_bIsClip2 = true;
	}

	bool CClipMulti::IsClip() const
	{
		return m_bIsClip;
	}

	bool CClipMulti::IsClip2() const
	{
		return m_bIsClip2;
	}

	void CClipMulti::Reset()
	{
		m_rasterizer.reset();
		m_bIsClip = false;
		m_bIsClip2 = false;
	}
}
