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

#include "CVerticalScroll.h"

namespace OOXMLShapes
{
			CVerticalScroll::CVerticalScroll()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
					_T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"adj\" fmla=\"val 12500\" />")
					_T("</avLst>")
					_T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<gd name=\"a\" fmla=\"pin 0 adj 25000\" />")
					  _T("<gd name=\"ch\" fmla=\"*/ ss a 100000\" />")
					  _T("<gd name=\"ch2\" fmla=\"*/ ch 1 2\" />")
					  _T("<gd name=\"ch4\" fmla=\"*/ ch 1 4\" />")
					  _T("<gd name=\"x3\" fmla=\"+- ch ch2 0\" />")
					  _T("<gd name=\"x4\" fmla=\"+- ch ch 0\" />")
					  _T("<gd name=\"x6\" fmla=\"+- r 0 ch\" />")
					  _T("<gd name=\"x7\" fmla=\"+- r 0 ch2\" />")
					  _T("<gd name=\"x5\" fmla=\"+- x6 0 ch2\" />")
					  _T("<gd name=\"y3\" fmla=\"+- b 0 ch\" />")
					  _T("<gd name=\"y4\" fmla=\"+- b 0 ch2\" />")
					_T("</gdLst>")
					_T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<ahXY gdRefY=\"adj\" minY=\"0\" maxY=\"25000\">")
						_T("<pos x=\"l\" y=\"ch\" />")
					  _T("</ahXY>")
					_T("</ahLst>")
					_T("<cxnLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<cxn ang=\"3cd4\">")
						_T("<pos x=\"hc\" y=\"t\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"0\">")
						_T("<pos x=\"ch\" y=\"vc\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd4\">")
						_T("<pos x=\"hc\" y=\"b\" />")
					  _T("</cxn>")
					  _T("<cxn ang=\"cd2\">")
						_T("<pos x=\"x6\" y=\"vc\" />")
					  _T("</cxn>")
					_T("</cxnLst>")
					_T("<rect l=\"ch\" t=\"ch\" r=\"x6\" b=\"y4\" xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\" />")
					_T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
					  _T("<path stroke=\"false\" extrusionOk=\"false\">")
						_T("<moveTo>")
						  _T("<pt x=\"ch2\" y=\"b\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"cd4\" swAng=\"-5400000\" />")
						_T("<lnTo>")
						  _T("<pt x=\"ch2\" y=\"y4\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ch4\" hR=\"ch4\" stAng=\"cd4\" swAng=\"-10800000\" />")
						_T("<lnTo>")
						  _T("<pt x=\"ch\" y=\"y3\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"ch\" y=\"ch2\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"cd2\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"x7\" y=\"t\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"3cd4\" swAng=\"cd2\" />")
						_T("<lnTo>")
						  _T("<pt x=\"x6\" y=\"ch\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"x6\" y=\"y4\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"0\" swAng=\"cd4\" />")
						_T("<close />")
						_T("<moveTo>")
						  _T("<pt x=\"x4\" y=\"ch2\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"0\" swAng=\"cd4\" />")
						_T("<arcTo wR=\"ch4\" hR=\"ch4\" stAng=\"cd4\" swAng=\"cd2\" />")
						_T("<close />")
					  _T("</path>")
					  _T("<path fill=\"darkenLess\" stroke=\"false\" extrusionOk=\"false\">")
						_T("<moveTo>")
						  _T("<pt x=\"x4\" y=\"ch2\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"0\" swAng=\"cd4\" />")
						_T("<arcTo wR=\"ch4\" hR=\"ch4\" stAng=\"cd4\" swAng=\"cd2\" />")
						_T("<close />")
						_T("<moveTo>")
						  _T("<pt x=\"ch\" y=\"y4\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"0\" swAng=\"3cd4\" />")
						_T("<arcTo wR=\"ch4\" hR=\"ch4\" stAng=\"3cd4\" swAng=\"cd2\" />")
						_T("<close />")
					  _T("</path>")
					  _T("<path fill=\"none\" extrusionOk=\"false\">")
						_T("<moveTo>")
						  _T("<pt x=\"ch\" y=\"y3\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"ch\" y=\"ch2\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"cd2\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"x7\" y=\"t\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"3cd4\" swAng=\"cd2\" />")
						_T("<lnTo>")
						  _T("<pt x=\"x6\" y=\"ch\" />")
						_T("</lnTo>")
						_T("<lnTo>")
						  _T("<pt x=\"x6\" y=\"y4\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"0\" swAng=\"cd4\" />")
						_T("<lnTo>")
						  _T("<pt x=\"ch2\" y=\"b\" />")
						_T("</lnTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"cd4\" swAng=\"cd2\" />")
						_T("<close />")
						_T("<moveTo>")
						  _T("<pt x=\"x3\" y=\"t\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"3cd4\" swAng=\"cd2\" />")
						_T("<arcTo wR=\"ch4\" hR=\"ch4\" stAng=\"cd4\" swAng=\"cd2\" />")
						_T("<lnTo>")
						  _T("<pt x=\"x4\" y=\"ch2\" />")
						_T("</lnTo>")
						_T("<moveTo>")
						  _T("<pt x=\"x6\" y=\"ch\" />")
						_T("</moveTo>")
						_T("<lnTo>")
						  _T("<pt x=\"x3\" y=\"ch\" />")
						_T("</lnTo>")
						_T("<moveTo>")
						  _T("<pt x=\"ch2\" y=\"y3\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"ch4\" hR=\"ch4\" stAng=\"3cd4\" swAng=\"cd2\" />")
						_T("<lnTo>")
						  _T("<pt x=\"ch\" y=\"y4\" />")
						_T("</lnTo>")
						_T("<moveTo>")
						  _T("<pt x=\"ch2\" y=\"b\" />")
						_T("</moveTo>")
						_T("<arcTo wR=\"ch2\" hR=\"ch2\" stAng=\"cd4\" swAng=\"-5400000\" />")
						_T("<lnTo>")
						  _T("<pt x=\"ch\" y=\"y3\" />")
						_T("</lnTo>")
					  _T("</path>")
					_T("</pathLst>")
					_T("</ooxml-shape>")
				);
			}
}
