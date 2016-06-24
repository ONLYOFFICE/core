/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once
#include "../PPTXShape.h"

namespace OOXMLShapes
{
	class CBlockArc : public CPPTXShape
	{
		public:
			CBlockArc()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj1\" fmla=\"val 10800000\" />")
					  _T("<gd name=\"adj2\" fmla=\"val 0\" />")
					  _T("<gd name=\"adj3\" fmla=\"val 25000\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"stAng\" fmla=\"pin 0 adj1 21599999\" />")
					  _T("<gd name=\"istAng\" fmla=\"pin 0 adj2 21599999\" />")
					  _T("<gd name=\"a3\" fmla=\"pin 0 adj3 50000\" />")
					  _T("<gd name=\"sw11\" fmla=\"+- istAng 0 stAng\" />")
					  _T("<gd name=\"sw12\" fmla=\"+- sw11 21600000 0\" />")
					  _T("<gd name=\"swAng\" fmla=\"?: sw11 sw11 sw12\" />")
					  _T("<gd name=\"iswAng\" fmla=\"+- 0 0 swAng\" />")
					  _T("<gd name=\"wt1\" fmla=\"sin wd2 stAng\" />")
					  _T("<gd name=\"ht1\" fmla=\"cos hd2 stAng\" />")
					  _T("<gd name=\"wt3\" fmla=\"sin wd2 istAng\" />")
					  _T("<gd name=\"ht3\" fmla=\"cos hd2 istAng\" />")
					  _T("<gd name=\"dx1\" fmla=\"cat2 wd2 ht1 wt1\" />")
					  _T("<gd name=\"dy1\" fmla=\"sat2 hd2 ht1 wt1\" />")
					  _T("<gd name=\"dx3\" fmla=\"cat2 wd2 ht3 wt3\" />")
					  _T("<gd name=\"dy3\" fmla=\"sat2 hd2 ht3 wt3\" />")
					  _T("<gd name=\"x1\" fmla=\"+- hc dx1 0\" />")
					  _T("<gd name=\"y1\" fmla=\"+- vc dy1 0\" />")
					  _T("<gd name=\"x3\" fmla=\"+- hc dx3 0\" />")
					  _T("<gd name=\"y3\" fmla=\"+- vc dy3 0\" />")
					  _T("<gd name=\"dr\" fmla=\"*/ ss a3 100000\" />")
					  _T("<gd name=\"iwd2\" fmla=\"+- wd2 0 dr\" />")
					  _T("<gd name=\"ihd2\" fmla=\"+- hd2 0 dr\" />")
					  _T("<gd name=\"wt2\" fmla=\"sin iwd2 istAng\" />")
					  _T("<gd name=\"ht2\" fmla=\"cos ihd2 istAng\" />")
					  _T("<gd name=\"wt4\" fmla=\"sin iwd2 stAng\" />")
					  _T("<gd name=\"ht4\" fmla=\"cos ihd2 stAng\" />")
					  _T("<gd name=\"dx2\" fmla=\"cat2 iwd2 ht2 wt2\" />")
					  _T("<gd name=\"dy2\" fmla=\"sat2 ihd2 ht2 wt2\" />")
					  _T("<gd name=\"dx4\" fmla=\"cat2 iwd2 ht4 wt4\" />")
					  _T("<gd name=\"dy4\" fmla=\"sat2 ihd2 ht4 wt4\" />")
					  _T("<gd name=\"x2\" fmla=\"+- hc dx2 0\" />")
					  _T("<gd name=\"y2\" fmla=\"+- vc dy2 0\" />")
					  _T("<gd name=\"x4\" fmla=\"+- hc dx4 0\" />")
					  _T("<gd name=\"y4\" fmla=\"+- vc dy4 0\" />")
					  _T("<gd name=\"sw0\" fmla=\"+- 21600000 0 stAng\" />")
					  _T("<gd name=\"da1\" fmla=\"+- swAng 0 sw0\" />")
					  _T("<gd name=\"g1\" fmla=\"max x1 x2\" />")
					  _T("<gd name=\"g2\" fmla=\"max x3 x4\" />")
					  _T("<gd name=\"g3\" fmla=\"max g1 g2\" />")
					  _T("<gd name=\"ir\" fmla=\"?: da1 r g3\" />")
					  _T("<gd name=\"sw1\" fmla=\"+- cd4 0 stAng\" />")
					  _T("<gd name=\"sw2\" fmla=\"+- 27000000 0 stAng\" />")
					  _T("<gd name=\"sw3\" fmla=\"?: sw1 sw1 sw2\" />")
					  _T("<gd name=\"da2\" fmla=\"+- swAng 0 sw3\" />")
					  _T("<gd name=\"g5\" fmla=\"max y1 y2\" />")
					  _T("<gd name=\"g6\" fmla=\"max y3 y4\" />")
					  _T("<gd name=\"g7\" fmla=\"max g5 g6\" />")
					  _T("<gd name=\"ib\" fmla=\"?: da2 b g7\" />")
					  _T("<gd name=\"sw4\" fmla=\"+- cd2 0 stAng\" />")
					  _T("<gd name=\"sw5\" fmla=\"+- 32400000 0 stAng\" />")
					  _T("<gd name=\"sw6\" fmla=\"?: sw4 sw4 sw5\" />")
					  _T("<gd name=\"da3\" fmla=\"+- swAng 0 sw6\" />")
					  _T("<gd name=\"g9\" fmla=\"min x1 x2\" />")
					  _T("<gd name=\"g10\" fmla=\"min x3 x4\" />")
					  _T("<gd name=\"g11\" fmla=\"min g9 g10\" />")
					  _T("<gd name=\"il\" fmla=\"?: da3 l g11\" />")
					  _T("<gd name=\"sw7\" fmla=\"+- 3cd4 0 stAng\" />")
					  _T("<gd name=\"sw8\" fmla=\"+- 37800000 0 stAng\" />")
					  _T("<gd name=\"sw9\" fmla=\"?: sw7 sw7 sw8\" />")
					  _T("<gd name=\"da4\" fmla=\"+- swAng 0 sw9\" />")
					  _T("<gd name=\"g13\" fmla=\"min y1 y2\" />")
					  _T("<gd name=\"g14\" fmla=\"min y3 y4\" />")
					  _T("<gd name=\"g15\" fmla=\"min g13 g14\" />")
					  _T("<gd name=\"it\" fmla=\"?: da4 t g15\" />")
					  _T("<gd name=\"x5\" fmla=\"+/ x1 x4 2\" />")
					  _T("<gd name=\"y5\" fmla=\"+/ y1 y4 2\" />")
					  _T("<gd name=\"x6\" fmla=\"+/ x3 x2 2\" />")
					  _T("<gd name=\"y6\" fmla=\"+/ y3 y2 2\" />")
					  _T("<gd name=\"cang1\" fmla=\"+- stAng 0 cd4\" />")
					  _T("<gd name=\"cang2\" fmla=\"+- istAng cd4 0\" />")
					  _T("<gd name=\"cang3\" fmla=\"+/ cang1 cang2 2\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahPolar gdRefAng=\"adj1\" minAng=\"0\" maxAng=\"21599999\">")
						_T("<pos x=\"x1\" y=\"y1\" />")
					  _T("</ahPolar>")
					  _T("<ahPolar gdRefR=\"adj3\" minR=\"0\" maxR=\"50000\" gdRefAng=\"adj2\" minAng=\"0\" maxAng=\"21599999\">")
						_T("<pos x=\"x2\" y=\"y2\" />")
					  _T("</ahPolar>")
					_T("</ahLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"cang1\">")
						_T("<pos x=\"x5\" y=\"y5\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cang2\">")
						_T("<pos x=\"x6\" y=\"y6\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cang3\">")
						_T("<pos x=\"hc\" y=\"vc\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"il\" t=\"it\" r=\"ir\" b=\"ib\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"x1\" y=\"y1\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"wd2\" hR=\"hd2\" stAng=\"stAng\" swAng=\"swAng\" />")
						_T("<lnTo>")
						  _T("<pt x=\"x2\" y=\"y2\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"iwd2\" hR=\"ihd2\" stAng=\"istAng\" swAng=\"iswAng\" />")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
	};
}