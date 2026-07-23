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

#if defined __cplusplus

#include <string>
#include <vector>
#include <iostream>
#include <boost/make_shared.hpp>
#include <sstream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <algorithm>
#include <list>
#include <math.h>
#include <boost/lexical_cast.hpp>
#include <iosfwd>
#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>
#include <unordered_map>
#include <boost/regex.hpp>
#include <string.h>
#include <fstream>
#include <memory>
#include <unordered_set>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//#include "../../../XlsFile/Format/Binary/CFStream.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/Begin.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/BiffRecord.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/BiffRecordContinued.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/Continue.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/ContinueFrt12.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/End.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/EndObject.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/Font.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/MsoDrawing.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/StartObject.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/SXAddl.h"
//#include "../../../XlsFile/Format/Logic/Biff_records/Theme.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/BiffAttribute.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/BiffString.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/BiffStructure.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/BitMarkedStructs.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/Boolean.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/Cell.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/CellRangeRef.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/CellRef.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/DXFN.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/FrtHeader.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/FrtHeaderOld.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/LongRGB.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/OperandPtg.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/ParsedFormula.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/Ptg.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/Xnum.h"
//#include "../../../XlsFile/Format/Logic/Biff_structures/ODRAW/OfficeArtRecord.h"
//#include "../../../XlsFile/Converter/oox_rels.h"
//#include "../../../XlsFile/Converter/xlsx_conversion_context.h"
//#include "../../../XlsFile/Converter/ShapeType.h"
//#include "../../../XlsFile/Format/Logic/CompositeObject.h"
//#include "../../../XlsFile/Format/Logic/GlobalWorkbookInfo.h"
//#include "../../../XlsFile/Format/Auxiliary/HelpFunc.h"
//#include "../../../Common/ODraw/CustomShape.h"
//#include "../../../Common/Utils/OptPtr.h"
//#include "../../../Common/Utils/simple_xml_writer.h"

#endif
