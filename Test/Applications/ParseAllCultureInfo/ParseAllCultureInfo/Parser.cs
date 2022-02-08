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
            public int UseAMPM;
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
                UseAMPM = -1 != di.LongTimePattern.IndexOf("t") ? 1 : 0;
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
                    {
                        if (nIndexD + 1 < ShortDatePatternLower.Length && 'd' == ShortDatePatternLower[nIndexD + 1])
                        {
                            sShortDatePattern += "1";
                        }
                        else
                        {
                            sShortDatePattern += "0";
                        }
                    } else if (nIndexM == index)
                    {
                        if (nIndexM + 1 < ShortDatePatternLower.Length && 'm' == ShortDatePatternLower[nIndexM + 1])
                        {
                            sShortDatePattern += "3";
                        }
                        else
                        {
                            sShortDatePattern += "2";
                        }
                    }
                    else if (nIndexY == index)
                    {
                        if (nIndexY + 2 < ShortDatePatternLower.Length && 'y' == ShortDatePatternLower[nIndexY + 2])
                        {
                            sShortDatePattern += "5";
                        }
                        else
                        {
                            sShortDatePattern += "4";
                        }
                    }
                }
                ShortDatePattern = sShortDatePattern;
            }
        }
        static Dictionary<int, int> g_mapUsedValues = new Dictionary<int, int>() {
            {4, 1},
            {5, 1},
            {7, 1},
            {8, 1},
            {9, 1},
            {10, 1},
            {11, 1},
            {12, 1},
            {14, 1},
            {16, 1},
            {17, 1},
            {18, 1},
            {21, 1},
            {22, 1},
            {25, 1},
            {29, 1},
            {31, 1},
            {34, 1},
            {36, 1},
            {38, 1},
            {39, 1},
            {42, 1},
            {44, 1},
            {63, 1},
            {80, 1},
            {1026, 1},
            {1028, 1},
            {1029, 1},
            {1031, 1},
            {1032, 1},
            {1033, 1},
            {1035, 1},
            {1036, 1},
            {1038, 1},
            {1040, 1},
            {1041, 1},
            {1042, 1},
            {1043, 1},
            {1045, 1},
            {1046, 1},
            {1049, 1},
            {1050, 1},
            {1051, 1},
            {1053, 1},
            {1055, 1},
            {1058, 1},
            {1060, 1},
            {1062, 1},
            {1063, 1},
            {1066, 1},
            {1068, 1},
            {1087, 1},
            {1104, 1},
            {2052, 1},
            {2055, 1},
            {2057, 1},
            {2058, 1},
            {2060, 1},
            {2064, 1},
            {2070, 1},
            {2073, 1},
            {2077, 1},
            {2092, 1},
            {2128, 1},
            {3076, 1},
            {3079, 1},
            {3081, 1},
            {3082, 1},
            {3084, 1},
            {3152, 1},
            {4100, 1},
            {4103, 1},
            {4105, 1},
            {4106, 1},
            {4108, 1},
            {4122, 1},
            {5124, 1},
            {5127, 1},
            {5129, 1},
            {5130, 1},
            {5132, 1},
            {6153, 1},
            {6154, 1},
            {6156, 1},
            {7177, 1},
            {7178, 1},
            {7180, 1},
            {8201, 1},
            {8202, 1},
            {8204, 1},
            {9225, 1},
            {9226, 1},
            {9228, 1},
            {10249, 1},
            {10250, 1},
            {10252, 1},
            {11273, 1},
            {11274, 1},
            {11276, 1},
            {12297, 1},
            {12298, 1},
            {12300, 1},
            {13321, 1},
            {13322, 1},
            {13324, 1},
            {14345, 1},
            {14346, 1},
            {14348, 1},
            {15369, 1},
            {15370, 1},
            {15372, 1},
            {16393, 1},
            {16394, 1},
            {17417, 1},
            {17418, 1},
            {18441, 1},
            {18442, 1},
            {19466, 1},
            {20490, 1},
            {21514, 1},
            {22538, 1},
            {23562, 1},
            {29740, 1},
            {30724, 1},
            {30764, 1},
            {30800, 1},
            {31748, 1},
            {31824, 1}
        };
        public static void parse()
        {
            List<int> aLcid = new List<int>();
            Dictionary<int, CultureInfo> aInfos = new Dictionary<int, CultureInfo>();
            foreach (CultureInfo ci in CultureInfo.GetCultures(CultureTypes.AllCultures))
            {
                if (!aInfos.ContainsKey(ci.LCID) && (false || g_mapUsedValues.ContainsKey(ci.LCID)))
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
