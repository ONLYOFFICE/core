using System;
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
