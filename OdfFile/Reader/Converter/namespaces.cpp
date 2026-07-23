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

#include "namespaces.h"

namespace cpdoccore { 
namespace oox {
namespace xmlns {

const office_xmlns rels = {L"", L"http://schemas.openxmlformats.org/package/2006/relationships"};
const office_xmlns types = {L"", L"http://schemas.openxmlformats.org/package/2006/content-types"};

const office_xmlns ve = {L"ve", L"http://schemas.openxmlformats.org/markup-compatibility/2006"};
const office_xmlns o = {L"o", L"urn:schemas-microsoft-com:office:office"};
const office_xmlns r = {L"r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"};
const office_xmlns m = {L"m", L"http://schemas.openxmlformats.org/officeDocument/2006/math"};
const office_xmlns v = {L"v", L"urn:schemas-microsoft-com:vml"};
const office_xmlns wp = {L"wp", L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"};
const office_xmlns w10 = {L"w10", L"urn:schemas-microsoft-com:office:word"};
const office_xmlns w = {L"w", L"http://schemas.openxmlformats.org/wordprocessingml/2006/main"};
const office_xmlns wne = {L"wne", L"http://schemas.microsoft.com/office/word/2006/wordml"};

office_xmlns const * const word_ns[] = { &ve, &o, &r, &m, &v, &wp, &w10, &w, &wne };

}
}
}
