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
