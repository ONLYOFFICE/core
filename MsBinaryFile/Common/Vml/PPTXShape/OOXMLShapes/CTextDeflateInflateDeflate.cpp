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

#include "CTextDeflateInflateDeflate.h"

namespace OOXMLShapes
{
			CTextDeflateInflateDeflate::CTextDeflateInflateDeflate()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 25000\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 3000 adj 47000\" />")
					  _T("<gd name=\"dy\" fmla=\"*/ a h 100000\" />")
					  _T("<gd name=\"del\" fmla=\"*/ h 3 100\" />")
					  _T("<gd name=\"ey1\" fmla=\"*/ h 30 100\" />")
					  _T("<gd name=\"ey2\" fmla=\"*/ h 36 100\" />")
					  _T("<gd name=\"ey3\" fmla=\"*/ h 63 100\" />")
					  _T("<gd name=\"ey4\" fmla=\"*/ h 70 100\" />")
					  _T("<gd name=\"by\" fmla=\"+- b 0 dy\" />")
					  _T("<gd name=\"yh1\" fmla=\"+- dy 0 del\" />")
					  _T("<gd name=\"yl1\" fmla=\"+- dy del 0\" />")
					  _T("<gd name=\"yh2\" fmla=\"+- by 0 del\" />")
					  _T("<gd name=\"yl2\" fmla=\"+- by del 0\" />")
					  _T("<gd name=\"y1\" fmla=\"+- yh1 yh1 ey1\" />")
					  _T("<gd name=\"y2\" fmla=\"+- yl1 yl1 ey2\" />")
					  _T("<gd name=\"y3\" fmla=\"+- yh2 yh2 ey3\" />")
					  _T("<gd name=\"y4\" fmla=\"+- yl2 yl2 ey4\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj\" minY=\"3000\" maxY=\"47000\">")
						_T("<pos x=\"hc\" y=\"dy\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"t\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"t\" />")
						_T("</lnTo>")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"ey1\" />")
						_T("</moveTo>")
						_T("<quadBezTo>")
						  _T("<pt x=\"hc\" y=\"y1\" />")
						  _T("<pt x=\"r\" y=\"ey1\" />")
						_T("</quadBezTo>")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"ey2\" />")
						_T("</moveTo>")
						_T("<quadBezTo>")
						  _T("<pt x=\"hc\" y=\"y2\" />")
						  _T("<pt x=\"r\" y=\"ey2\" />")
						_T("</quadBezTo>")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"ey3\" />")
						_T("</moveTo>")
						_T("<quadBezTo>")
						  _T("<pt x=\"hc\" y=\"y3\" />")
						  _T("<pt x=\"r\" y=\"ey3\" />")
						_T("</quadBezTo>")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"ey4\" />")
						_T("</moveTo>")
						_T("<quadBezTo>")
						  _T("<pt x=\"hc\" y=\"y4\" />")
						  _T("<pt x=\"r\" y=\"ey4\" />")
						_T("</quadBezTo>")
					  _T("</path>")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"b\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"b\" />")
						_T("</lnTo>")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
}
