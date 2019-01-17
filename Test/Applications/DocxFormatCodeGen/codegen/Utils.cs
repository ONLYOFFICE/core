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
﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace codegen
{
    class Utils
    {
        public static string gc_sBinaryChartReader = "BinaryChartReader";
        public static string gc_sBinaryChartWriter = "BinaryChartWriter";
        public static string gc_sFilePrefix = "//Generated code\r\n";
        public static string gc_sMemberPrefix = "m_";
        public static string gc_sSerToBinEnumPrefix = "c_oSer";
        public static string gc_sItemsChoiceType = "ItemsChoiceType";
        public static string gc_sItemsElementName = "ItemsElementName";
        public static Dictionary<string, string> m_mapNamespaceToPrefix = new Dictionary<string, string>() { { "http://purl.oclc.org/ooxml/drawingml/chart", "c:" }, { "http://purl.oclc.org/ooxml/drawingml/main", "a:" }, { "http://purl.oclc.org/ooxml/officeDocument/relationships", "r:" }, { "http://schemas.openxmlformats.org/markup-compatibility/2006", "mc:" }, { "http://schemas.microsoft.com/office/drawing/2007/8/2/chart", "c14:" } };
        public static string GetEnumElemName(string sEnumName, string sElemName)
        {
            return sEnumName.ToLower() + sElemName.ToUpper();
        }
        public static string GetSerEnumElemName(string sEnumName, string sElemName)
        {
            return GetEnumElemName(gc_sSerToBinEnumPrefix + sEnumName, sElemName);
        }
        public static string GetMemberElemName(string sElemName)
        {
            return gc_sMemberPrefix + sElemName;
        }
        public static string GetClassName(string sElemName)
        {
            return "CT_" + sElemName;
        }
    }
}
