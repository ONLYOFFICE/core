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

#include "CMathMultiply.h"

namespace OOXMLShapes
{
			CMathMultiply::CMathMultiply()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj1\" fmla=\"val 23520\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a1\" fmla=\"pin 0 adj1 51965\" />")
					  _T("<gd name=\"th\" fmla=\"*/ ss a1 100000\" />")
					  _T("<gd name=\"a\" fmla=\"at2 w h\" />")
					  _T("<gd name=\"sa\" fmla=\"sin 1 a\" />")
					  _T("<gd name=\"ca\" fmla=\"cos 1 a\" />")
					  _T("<gd name=\"ta\" fmla=\"tan 1 a\" />")
					  _T("<gd name=\"dl\" fmla=\"mod w h 0\" />")
					  _T("<gd name=\"rw\" fmla=\"*/ dl 51965 100000\" />")
					  _T("<gd name=\"lM\" fmla=\"+- dl 0 rw\" />")
					  _T("<gd name=\"xM\" fmla=\"*/ ca lM 2\" />")
					  _T("<gd name=\"yM\" fmla=\"*/ sa lM 2\" />")
					  _T("<gd name=\"dxAM\" fmla=\"*/ sa th 2\" />")
					  _T("<gd name=\"dyAM\" fmla=\"*/ ca th 2\" />")
					  _T("<gd name=\"xA\" fmla=\"+- xM 0 dxAM\" />")
					  _T("<gd name=\"yA\" fmla=\"+- yM dyAM 0\" />")
					  _T("<gd name=\"xB\" fmla=\"+- xM dxAM 0\" />")
					  _T("<gd name=\"yB\" fmla=\"+- yM 0 dyAM\" />")
					  _T("<gd name=\"xBC\" fmla=\"+- hc 0 xB\" />")
					  _T("<gd name=\"yBC\" fmla=\"*/ xBC ta 1\" />")
					  _T("<gd name=\"yC\" fmla=\"+- yBC yB 0\" />")
					  _T("<gd name=\"xD\" fmla=\"+- r 0 xB\" />")
					  _T("<gd name=\"xE\" fmla=\"+- r 0 xA\" />")
					  _T("<gd name=\"yFE\" fmla=\"+- vc 0 yA\" />")
					  _T("<gd name=\"xFE\" fmla=\"*/ yFE 1 ta\" />")
					  _T("<gd name=\"xF\" fmla=\"+- xE 0 xFE\" />")
					  _T("<gd name=\"xL\" fmla=\"+- xA xFE 0\" />")
					  _T("<gd name=\"yG\" fmla=\"+- b 0 yA\" />")
					  _T("<gd name=\"yH\" fmla=\"+- b 0 yB\" />")
					  _T("<gd name=\"yI\" fmla=\"+- b 0 yC\" />")
					  _T("<gd name=\"xC2\" fmla=\"+- r 0 xM\" />")
					  _T("<gd name=\"yC3\" fmla=\"+- b 0 yM\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj1\" minY=\"0\" maxY=\"51965\">")
						_T("<pos x=\"l\" y=\"th\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"xM\" y=\"yM\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"xC2\" y=\"yM\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"xC2\" y=\"yC3\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"xM\" y=\"yC3\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"xA\" t=\"yB\" r=\"xE\" b=\"yH\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path>")
						_T("<moveTo>")
						  _T("<pt x=\"xA\" y=\"yA\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xB\" y=\"yB\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"hc\" y=\"yC\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xD\" y=\"yB\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xE\" y=\"yA\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xF\" y=\"vc\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xE\" y=\"yG\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xD\" y=\"yH\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"hc\" y=\"yI\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xB\" y=\"yH\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xA\" y=\"yG\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"xL\" y=\"vc\" />")
						_T("</lnTo>")
						_T("<close />")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
}
