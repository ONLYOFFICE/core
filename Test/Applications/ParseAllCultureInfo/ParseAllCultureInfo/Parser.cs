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
﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.IO;
using System.Reflection;

namespace ParseAllCultureInfo
{
    class Parser
    {
        [DataContract]
        class ParamsSerializable
        {
            [DataMember]
            public int LCID;
            [DataMember]
            public string Name;
            [DataMember]
            public int CurrencyPositivePattern;
            [DataMember]
            public int CurrencyNegativePattern;
            [DataMember]
            public string CurrencySymbol;
            [DataMember]
            public string NumberDecimalSeparator;
            [DataMember]
            public string NumberGroupSeparator;
            [DataMember]
            public int[] NumberGroupSizes;
            [DataMember]
            public string[] DayNames;
            [DataMember]
            public string[] AbbreviatedDayNames;
            [DataMember]
            public string[] MonthNames;
            [DataMember]
            public string[] AbbreviatedMonthNames;
            [DataMember]
            public string[] MonthGenitiveNames;
            [DataMember]
            public string[] AbbreviatedMonthGenitiveNames;
            [DataMember]
            public string AMDesignator;
            [DataMember]
            public string PMDesignator;
            [DataMember]
            public string DateSeparator;
            [DataMember]
            public string TimeSeparator;
            [DataMember]
            public string ShortDatePattern;
            public ParamsSerializable(CultureInfo ci)
            {
                LCID = ci.LCID;
                Name = ci.Name;

                NumberFormatInfo ni = ci.NumberFormat;
                CurrencyPositivePattern = ni.CurrencyPositivePattern;
                CurrencyNegativePattern = ni.CurrencyNegativePattern;
                CurrencySymbol = ni.CurrencySymbol;
                NumberDecimalSeparator = ni.NumberDecimalSeparator;
                //replace Non-breaking space(0xA0) with White-space(0x20)
                NumberGroupSeparator = ni.NumberGroupSeparator.Replace((char)0xA0, (char)0x20);
                NumberGroupSizes = ni.NumberGroupSizes;

                DateTimeFormatInfo di = ci.DateTimeFormat;
                AbbreviatedDayNames = di.AbbreviatedDayNames;
                AbbreviatedMonthNames = di.AbbreviatedMonthNames;
                bool bAbbreviatedMonthGenitiveNamesEqual = true;
                for (int i = 0; i < di.AbbreviatedMonthNames.Length; ++i)
                {
                    if (di.AbbreviatedMonthGenitiveNames[i] != di.AbbreviatedMonthNames[i])
                        bAbbreviatedMonthGenitiveNamesEqual = false;
                }
                if (!bAbbreviatedMonthGenitiveNamesEqual)
                    AbbreviatedMonthGenitiveNames = di.AbbreviatedMonthGenitiveNames;
                else
                    AbbreviatedMonthGenitiveNames = null;
                AMDesignator = di.AMDesignator;
                DateSeparator = di.DateSeparator;
                DayNames = di.DayNames;
                MonthNames = di.MonthNames;
                bool bMonthGenitiveNamesEqual = true;
                for (int i = 0; i < di.MonthNames.Length; ++i)
                {
                    if (di.MonthGenitiveNames[i] != di.MonthNames[i])
                        bMonthGenitiveNamesEqual = false;
                }
                if (!bMonthGenitiveNamesEqual)
                    MonthGenitiveNames = di.MonthGenitiveNames;
                else
                    MonthGenitiveNames = null;
                PMDesignator = di.PMDesignator;
                TimeSeparator = di.TimeSeparator;
                string ShortDatePatternLower = di.ShortDatePattern.ToLower();
                int nIndexD = ShortDatePatternLower.IndexOf("d");
                int nIndexM = ShortDatePatternLower.IndexOf("m");
                int nIndexY = ShortDatePatternLower.IndexOf("y");
                int[] aTemp = new int[] {nIndexD, nIndexM, nIndexY};
                Array.Sort(aTemp);
                string sShortDatePattern = "";
                for (int i = 0; i < aTemp.Length; ++i)
                {
                    var index = aTemp[i];
                    if(nIndexD == index)
                        sShortDatePattern += "0";
                    else if (nIndexM == index)
                        sShortDatePattern += "1";
                    else if (nIndexY == index)
                        sShortDatePattern += "2";
                }
                ShortDatePattern = sShortDatePattern;
            }
        }
        public static void parse()
        {
            List<int> aLcid = new List<int>();
            Dictionary<int, CultureInfo> aInfos = new Dictionary<int, CultureInfo>();
            foreach (CultureInfo ci in CultureInfo.GetCultures(CultureTypes.AllCultures))
            {
                if (!aInfos.ContainsKey(ci.LCID))
                {
                    aLcid.Add(ci.LCID);
                    aInfos[ci.LCID] = ci;
                }

            }
            aLcid.Sort();
            StringBuilder sb = new StringBuilder();
            sb.AppendFormat("{{");
            foreach (int index in aLcid)
            {
                ParamsSerializable elem = new ParamsSerializable(aInfos[index]);
                sb.AppendFormat("{0}:{{", elem.LCID);
                Type type = elem.GetType();
                FieldInfo[] fieldInfos = type.GetFields();
                for (int i = 0, length = fieldInfos.Length; i < length; ++i)
                {
                    if (0 != i)
                        sb.Append(",");
                    FieldInfo info = fieldInfos[i];
                    object value = info.GetValue(elem);
                    if (info.FieldType.IsArray)
                    {
                        sb.AppendFormat("{0}:[", info.Name);
                        if (value is int[])
                        {
                            int[] aElems = (int[])value;
                            for (int j = 0, length2 = aElems.Length; j < length2; ++j)
                            {
                                if (0 != j)
                                    sb.AppendFormat(",");
                                sb.AppendFormat("{0}", aElems[j]);
                            }
                        }
                        else if (value is string[])
                        {
                            string[] aElems = (string[])value;
                            for (int j = 0, length2 = aElems.Length; j < length2; ++j)
                            {
                                if (0 != j)
                                    sb.AppendFormat(",");
                                sb.AppendFormat("\"{0}\"", aElems[j]);
                            }
                        }
                        sb.AppendFormat("]");
                    }
                    else
                    {
                        sb.AppendFormat("{0}:", info.Name);
                        parseValue(sb, value);
                    }
                }
                sb.Append("},\r\n");
            }
            sb.AppendFormat("}}");
            string sAll = sb.ToString();
        }
        public static void parseValue(StringBuilder sb, object value)
        {
            if (value is string)
                sb.AppendFormat("\"{0}\"", value);
            else
                sb.AppendFormat("{0}", value);
        }
    }
}
