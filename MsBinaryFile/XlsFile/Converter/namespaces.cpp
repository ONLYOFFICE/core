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

#include "namespaces.h"

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
