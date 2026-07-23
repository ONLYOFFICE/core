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

#include "CTextCurveUp.h"

namespace OOXMLShapes
{
			CTextCurveUp::CTextCurveUp()
			{
				LoadFromXML(
					_T("<ooxml-shape>")
    _T("<avLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
      _T("<gd name=\"adj\" fmla=\"val 45977\" />")
    _T("</avLst>")
    _T("<gdLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
      _T("<gd name=\"a\" fmla=\"pin 0 adj 56338\" />")
      _T("<gd name=\"dy\" fmla=\"*/ a h 100000\" />")
      _T("<gd name=\"gd1\" fmla=\"*/ dy 3 4\" />")
      _T("<gd name=\"gd2\" fmla=\"*/ dy 5 4\" />")
      _T("<gd name=\"gd3\" fmla=\"*/ dy 3 8\" />")
      _T("<gd name=\"gd4\" fmla=\"*/ dy 1 8\" />")
      _T("<gd name=\"gd5\" fmla=\"+- h 0 gd3\" />")
      _T("<gd name=\"gd6\" fmla=\"+- gd4 h 0\" />")
      _T("<gd name=\"y0\" fmla=\"+- t dy 0\" />")
      _T("<gd name=\"y1\" fmla=\"+- t gd1 0\" />")
      _T("<gd name=\"y2\" fmla=\"+- t gd2 0\" />")
      _T("<gd name=\"y3\" fmla=\"+- t gd3 0\" />")
      _T("<gd name=\"y4\" fmla=\"+- t gd4 0\" />")
      _T("<gd name=\"y5\" fmla=\"+- t gd5 0\" />")
      _T("<gd name=\"y6\" fmla=\"+- t gd6 0\" />")
      _T("<gd name=\"x1\" fmla=\"+- l wd3 0\" />")
      _T("<gd name=\"x2\" fmla=\"+- r 0 wd3\" />")
    _T("</gdLst>")
    _T("<ahLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
      _T("<ahXY gdRefY=\"adj\" minY=\"0\" maxY=\"56338\">")
        _T("<pos x=\"l\" y=\"y0\" />")
      _T("</ahXY>")
    _T("</ahLst>")
    _T("<pathLst xmlns=\"http://schemas.openxmlformats.org/drawingml/2006/main\">")
      _T("<path>")
        _T("<moveTo>")
          _T("<pt x=\"l\" y=\"y0\" />")
        _T("</moveTo>")
        _T("<cubicBezTo>")
          _T("<pt x=\"x1\" y=\"y2\" />")
          _T("<pt x=\"x2\" y=\"y1\" />")
          _T("<pt x=\"r\" y=\"t\" />")
        _T("</cubicBezTo>")
      _T("</path>")
      _T("<path>")
        _T("<moveTo>")
          _T("<pt x=\"l\" y=\"y5\" />")
        _T("</moveTo>")
        _T("<cubicBezTo>")
          _T("<pt x=\"x1\" y=\"y6\" />")
          _T("<pt x=\"x2\" y=\"y6\" />")
          _T("<pt x=\"r\" y=\"y5\" />")
        _T("</cubicBezTo>")
      _T("</path>")
    _T("</pathLst>")

					_T("</ooxml-shape>")
				);
			}
}
