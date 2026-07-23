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

#include "CSwooshArrow.h"

namespace OOXMLShapes
{
			CSwooshArrow::CSwooshArrow()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj1\" fmla=\"val 25000\" />")
					  _T("<gd name=\"adj2\" fmla=\"val 16667\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a1\" fmla=\"pin 1 adj1 75000\" />")
					  _T("<gd name=\"maxAdj2\" fmla=\"*/ 70000 w ss\" />")
					  _T("<gd name=\"a2\" fmla=\"pin 0 adj2 maxAdj2\" />")
					  _T("<gd name=\"ad1\" fmla=\"*/ h a1 100000\" />")
					  _T("<gd name=\"ad2\" fmla=\"*/ ss a2 100000\" />")
					  _T("<gd name=\"xB\" fmla=\"+- r 0 ad2\" />")
					  _T("<gd name=\"yB\" fmla=\"+- t ssd8 0\" />")
					  _T("<gd name=\"alfa\" fmla=\"*/ cd4 1 14\" />")
					  _T("<gd name=\"dx0\" fmla=\"tan ssd8 alfa\" />")
					  _T("<gd name=\"xC\" fmla=\"+- xB 0 dx0\" />")
					  _T("<gd name=\"dx1\" fmla=\"tan ad1 alfa\" />")
					  _T("<gd name=\"yF\" fmla=\"+- yB ad1 0\" />")
					  _T("<gd name=\"xF\" fmla=\"+- xB dx1 0\" />")
					  _T("<gd name=\"xE\" fmla=\"+- xF dx0 0\" />")
					  _T("<gd name=\"yE\" fmla=\"+- yF ssd8 0\" />")
					  _T("<gd name=\"dy2\" fmla=\"+- yE 0 t\" />")
					  _T("<gd name=\"dy22\" fmla=\"*/ dy2 1 2\" />")
					  _T("<gd name=\"dy3\" fmla=\"*/ h 1 20\" />")
					  _T("<gd name=\"yD\" fmla=\"+- t dy22 dy3\" />")
					  _T("<gd name=\"dy4\" fmla=\"*/ hd6 1 1\" />")
					  _T("<gd name=\"yP1\" fmla=\"+- hd6 dy4 0\" />")
					  _T("<gd name=\"xP1\" fmla=\"val wd6\" />")
					  _T("<gd name=\"dy5\" fmla=\"*/ hd6 1 2\" />")
					  _T("<gd name=\"yP2\" fmla=\"+- yF dy5 0\" />")
					  _T("<gd name=\"xP2\" fmla=\"val wd4\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj1\" minY=\"1\" maxY=\"75000\">")
						_T("<pos x=\"xF\" y=\"yF\" />")
					  _T("</ahXY>")
					  _T("<ahXY gdRefX=\"adj2\" minX=\"0\" maxX=\"maxAdj2\">")
						_T("<pos x=\"xB\" y=\"yB\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"l\" y=\"b\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"xC\" y=\"t\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"r\" y=\"yD\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"xE\" y=\"yE\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"l\" t=\"t\" r=\"r\" b=\"b\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"l\" y=\"b\" />")
						_T("</moveTo>")
						_T("<quadBezTo>")
						  _T("<pt x=\"xP1\" y=\"yP1\" />")
						  _T("<pt x=\"xB\" y=\"yB\" />")
						_T("</quadBezTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xC\" y=\"t\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"r\" y=\"yD\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xE\" y=\"yE\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xF\" y=\"yF\" />")
						_T("</lnTo>")
						_T("<quadBezTo>")
						  _T("<pt x=\"xP2\" y=\"yP2\" />")
						  _T("<pt x=\"l\" y=\"b\" />")
						_T("</quadBezTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
}
