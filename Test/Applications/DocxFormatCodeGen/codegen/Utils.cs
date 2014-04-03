using System;
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
    }
}
