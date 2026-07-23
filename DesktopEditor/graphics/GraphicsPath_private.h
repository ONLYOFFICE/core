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
#ifndef _BUILD_GRAPHICSPATH_PRIVATE_H_
#define _BUILD_GRAPHICSPATH_PRIVATE_H_

#include "../agg-2.4/include/agg_basics.h"
#include "../agg-2.4/include/agg_path_storage.h"

#include "../agg-2.4/include/agg_rasterizer_scanline_aa.h"
#include "../agg-2.4/include/agg_conv_transform.h"
#include "../agg-2.4/include/agg_conv_stroke.h"
#include "../agg-2.4/include/agg_conv_contour.h"
#include "../agg-2.4/include/agg_conv_unclose_polygon.h"

#include "../agg-2.4/include/agg_bezier_arc.h"
#include "../agg-2.4/include/agg_conv_curve.h"
#include "../agg-2.4/include/agg_renderer_base.h"
#include "../agg-2.4/include/agg_renderer_scanline.h"

#include "GraphicsPath.h"
#include "Matrix_private.h"

namespace Aggplus
{
	class CGraphicsPath_private
	{
	public:
		agg::path_storage m_agg_ps;
		bool m_bEvenOdd;
		bool m_bIsMoveTo;

		CMatrix* m_pTransform;

	public:
		CGraphicsPath_private()
		{
			m_bEvenOdd = false;
			m_bIsMoveTo = false;
			m_pTransform = NULL;
		}
	};

	class CGraphicsPathSimpleConverter_private
	{
	public:
		bool m_bEvenOdd;
		bool m_bIsMoveTo;
		bool m_bIsClosed;

		agg::path_storage m_agg_ps;

	public:
		CGraphicsPathSimpleConverter_private()
		{
			m_bEvenOdd	= false;
			m_bIsMoveTo	= false;
			m_bIsClosed	= false;
		}
	};
}

#endif // _BUILD_GRAPHICSPATH_PRIVATE_H_
