using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;

namespace ASCOfficeEWSEditor.XMLDataSpecific
{
    internal class XMLInt
    {
        internal static int ParseOrDefault(string str_value, int default_val)
        {
            int ret_val;
            if (!int.TryParse(str_value, out ret_val))
            {
                return default_val;
            }
            return ret_val;
        }
        internal static int ParseOrDefaultAndReport(string str_value, int default_val, Editor.Workbook workbook, string param_name)
        {
            int ret_val;
            if (!int.TryParse(str_value, out ret_val))
            {
                workbook.ReportError(new Editor.Events.InternalError("Wrong OOX format. " + param_name + " is not int"));
                return default_val;
            }
            return ret_val;
        }
    }

    internal class XMLDouble
    {
        internal static double ParseOrDefault(string str_value, double default_val)
        {
            double ret_val;
            if (!TryParse(str_value, out ret_val))
            {
                return default_val;
            }
            return ret_val;
        }

        internal static bool TryParse(string str_value, out double ret_val)
        {
            return double.TryParse(str_value, NumberStyles.Float | NumberStyles.AllowThousands, new CultureInfo(0x409), out ret_val);
        }

        internal static string ToString(double value)
        {
            return value.ToString(new CultureInfo(0x409));
        }

    }
    internal class XMLVariant
    {
        internal static string ToString(object value)
        {
            if(value is Double)
            {
                return XMLDouble.ToString((double)value);

            }
            else if(value is Boolean)
            {
                return value.ToString().ToLower();
            }
            return value.ToString();
        }

    }

    internal class XMLBoolean
    {
        internal static bool ParseOrDefault(string str_value, bool default_val)
        {
            bool ret_val;
            if (!TryParse(str_value, out ret_val))
            {
                return default_val;
            }
            return ret_val;
        }

        internal static bool TryParse(string str_value, out bool ret_val)
        {
            switch(str_value.ToLower())
            {
                case "0":
                case "false":
                {
                    ret_val = false;
                    return true;
                }
                case "1":
                case "true":
                {
                    ret_val = true;
                    return true;
                }
                default:
                {
                    ret_val = false;
                    return false;
                }
            }
        }

        internal static string ToString(bool value)
        {
            return value ? "true" : "false";
        }

    }
}
