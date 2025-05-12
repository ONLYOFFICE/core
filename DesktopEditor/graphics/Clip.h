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

/**
 * @class CClipMulti
 * @brief Class	providing boolean logic
 *   for a raster graphics clip.
 *
 * Boolean algebra for rasterizer scanlines, which are
 * received from CGraphicsPath.
 */
class CClipMulti
{
public:
	typedef agg::scanline_p8 scanline_type;
	typedef agg::rasterizer_scanline_aa<agg::rasterizer_sl_clip_dbl>  clip_rasterizer;

	clip_rasterizer m_rasterizer;
	
	/**
	 * @brief Interfaces for implementation graphics renderer scanlines.
	 *
	 * Scan lines can be stored in rasterizer as in case clip1
	 * or in storage otherwise(clip2).
	 */
	agg::scanline_storage_aa8 m_storage1;
	agg::scanline_storage_aa8 m_storage2;

	long m_lCurStorage;
	
	bool m_bIsClip;
	bool m_bIsClip2;

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

	bool IsClip()	const;
	bool IsClip2()	const;

	void Reset();
};

}

#endif // _BUILD_CLIP_H_
