using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Globalization;

namespace ASCOfficeEWSEditor.Editor.NumberFormat
{
    internal class NumberFormatParser
    {
        public static object MakeDisplayValue(Calculation.Formulas.DataObjects.IDataObject cell_data, string number_format_string)
        {
            string sNumFormatLower = number_format_string.ToLower();
            if (sNumFormatLower == "general" || sNumFormatLower == "@" || number_format_string.Length == 0)
            {
                Calculation.Formulas.DataObjects.ObjectNumber numeric_value = cell_data as Calculation.Formulas.DataObjects.ObjectNumber;
                if (numeric_value == null)
                {
                    return cell_data.ToString();
                }
                number_format_string = DecodeGeneralFormat(numeric_value.Value);
            }

            Calculation.Formulas.DataObjects.ObjectNumber cell_number_data = cell_data as Calculation.Formulas.DataObjects.ObjectNumber;
            if (cell_number_data != null)
            {
                bool has_date_format = false;
                string adapted_number_format = "";

                string[] arrFormats = number_format_string.Split(new string[] { ";" }, StringSplitOptions.RemoveEmptyEntries);
                int nLength = arrFormats.Length;
                if (1 == nLength)
                {
                    adapted_number_format = AdaptNumberFormatString(cell_number_data.Value.ToString(), arrFormats[0], out has_date_format);
                }
                else if (2 == nLength)
                {
                    if (0 <= cell_number_data.Value)
                        adapted_number_format = AdaptNumberFormatString(cell_number_data.Value.ToString(), arrFormats[0], out has_date_format);
                    else
                        adapted_number_format = AdaptNumberFormatString(cell_number_data.Value.ToString(), arrFormats[1], out has_date_format);
                }
                else if (4 == nLength)
                {
                    if (0 < cell_number_data.Value)
                        adapted_number_format = AdaptNumberFormatString(cell_number_data.Value.ToString(), arrFormats[0], out has_date_format);
                    else if (0 > cell_number_data.Value)
                        adapted_number_format = AdaptNumberFormatString(cell_number_data.Value.ToString(), arrFormats[1], out has_date_format);
                    else if (0 == cell_number_data.Value)
                        adapted_number_format = AdaptNumberFormatString(cell_number_data.Value.ToString(), arrFormats[2], out has_date_format);
                    else
                        adapted_number_format = AdaptNumberFormatString(cell_number_data.Value.ToString(), arrFormats[3], out has_date_format);
                }

                if(has_date_format)
                {
                    double ticks = cell_number_data.Value * 8.64e4;
                    ticks = Math.Round(ticks);
                    ticks = ticks * 1e7;
                    if(ticks >= DateTime.MinValue.Ticks && ticks <= DateTime.MaxValue.Ticks)
                    {
                        DateTime date = new DateTime((long)ticks);
                        try
                        {
                            date = date.AddDays(693593);
                        }
                        catch
                        {
                            return "#####";
                        }
                        return date.ToString(adapted_number_format, new CultureInfo(0x409));
                    }
                    else
                    {
                        return "#####";
                    }
                }

                return cell_number_data.Value.ToString(adapted_number_format, new CultureInfo(0x409));
            }

            Calculation.Formulas.DataObjects.ObjectString cell_text_data = cell_data as Calculation.Formulas.DataObjects.ObjectString;
            if(cell_text_data != null)
            {
                string positive_format;
                string negative_format;
                string zero_format;
                string text_format;
                if (SplitFormatString(number_format_string, out positive_format, out negative_format, out zero_format, out text_format))
                {
                    if (text_format.Length != 0)
                    {
                        return ApplyTextFormat(cell_text_data, text_format);
                    }
                }
            }

            return cell_data.ToString();
        }

        private static string DecodeGeneralFormat(double number)
        {
            if(number == 0.0)
            {
                return "0";
            }
            int mantisa_num_digits = (int)Math.Ceiling(Math.Log10(Math.Abs(number)));
            string suffix = "";
            int number_of_significant_fixed = 10;

            if (mantisa_num_digits > 0)
            {
                if (mantisa_num_digits == 10 || mantisa_num_digits == 11)
                {
                    return "0";
                }
                else if (mantisa_num_digits > 11)
                {
                    number *= CalculationTables.Power.Pow10(1 - mantisa_num_digits);
                    suffix = "E+00";
                    if(mantisa_num_digits > 99)
                    {
                        number_of_significant_fixed = 5;
                    }
                    else
                    {
                        number_of_significant_fixed = 6;
                    }
                    mantisa_num_digits = 1;
                }
            }
            else
            {
                if(mantisa_num_digits < -3)
                {
                    number *= CalculationTables.Power.Pow10(1 - mantisa_num_digits);
                    suffix = "E+00";
                    if (mantisa_num_digits < -97)
                    {
                        number_of_significant_fixed = 5;
                    }
                    else
                    {
                        number_of_significant_fixed = 6;
                    }
                }
                mantisa_num_digits = 1;
            }
            string number_format_string = "0";
            int nine_digits = (int)((number - Math.Truncate(number)) * CalculationTables.Power.Pow10(number_of_significant_fixed - mantisa_num_digits) + 0.5 * Math.Sign(number));
            for (int i = 0; i < number_of_significant_fixed - mantisa_num_digits; ++i)
            {
                if (number_format_string.Length != 1)
                {
                    number_format_string += '0';
                }
                else
                {
                    int rem;
                    nine_digits = Math.DivRem(nine_digits, 10, out rem);
                    if (rem != 0)
                    {
                        number_format_string += ".0";
                    }
                }
            }
            return number_format_string + suffix;
        }

        private static object ApplyTextFormat(Calculation.Formulas.DataObjects.ObjectString cell_text_data, string format_string)
        {
            string result = "";
            for (int format_pos = 0; format_pos < format_string.Length; ++format_pos)
            {
                char symb = format_string[format_pos];
                switch (symb)
                {
                    #region "as is" symbols
                    case '$':
                    case '-':
                    case '+':
                    case '/':
                    case '(':
                    case ')':
                    case ':':
                    case ' ':
                        result += format_string[format_pos];
                        break;

                    case '\\':
                        if (format_pos + 1 < format_string.Length)
                        {
                            result += format_string[++format_pos];
                        }
                        break;
                    case '\"':
                        if (format_pos + 1 < format_string.Length)
                        {
                            int next_quot_pos = format_string.IndexOf('\"', format_pos + 1);
                            if (next_quot_pos == -1)
                            {
                                return "";
                            }
                            if (next_quot_pos - format_pos > 1)
                            {
                                result += format_string.Substring(format_pos + 1, next_quot_pos - format_pos - 1);
                            }
                            format_pos = next_quot_pos;
                        }
                        break;

                    #endregion
                    #region alignment symbols
                    case '_':
                    case '*':
                        if (format_pos + 1 < format_string.Length)
                        {
                            ++format_pos;
                        }
                        else
                        {
                            result += symb;
                        }
                        break;
                    #endregion

                    case '@':
                        result += cell_text_data.Value;
                        break;
                }
            }
            return result;
        }
        private static Regex rx_seconds_fwd = new Regex("\\Gm*([^hmdys]|(\".*\")|\\\\.|_.|\\*.)*s");
        private static Regex rx_ampm_fwd = new Regex("(AM/PM)|(A/P)");
        private static string AdaptNumberFormatString(string strValue, string format_string, out bool has_date_format)
        {
            has_date_format = false;
            string adapted_format_string = "";
            bool h_found = false;
            for (int format_pos = 0; format_pos < format_string.Length; ++format_pos)
            {
                char symb = format_string[format_pos];
                switch (symb)
                {
                    #region ? -> #
                    case '?':
                        adapted_format_string += '#';
                        break;
                    #endregion
                    #region alignment symbols
                    case '_':
                    case '*':
                        if (format_pos + 1 < format_string.Length)
                        {
                            ++format_pos;
                        }
                        else
                        {
                            adapted_format_string += symb;
                        }
                        break;
                    #endregion
                    #region "as is" symbols
                    case '\\':
                        if (format_pos + 1 < format_string.Length)
                        {
                            adapted_format_string += "\"" + format_string[++format_pos] + "\"";
                        }
                        else
                        {
                            adapted_format_string += symb;
                        }
                        break;

                    case '\"':
                        if (format_pos + 1 < format_string.Length)
                        {
                            int next_quot_pos = format_string.IndexOf('\"', format_pos + 1);
                            if (next_quot_pos == -1)
                            {
                                adapted_format_string += format_string.Substring(format_pos);
                                format_pos = format_string.Length;
                                break;
                            }
                            adapted_format_string += format_string.Substring(format_pos, next_quot_pos - format_pos + 1);
                            format_pos = next_quot_pos;
                        }
                        else
                        {
                            adapted_format_string += symb;
                        }
                        break;
                    #endregion
                    #region color and conditions
                    case '[':
                        if (format_pos + 1 < format_string.Length)
                        {
                            int next_quot_pos = format_string.IndexOf(']', format_pos + 1);
                            if (next_quot_pos == -1)
                            {
                                next_quot_pos = format_string.Length - 1;
                            }
                            format_pos = next_quot_pos;
                        }
                        else
                        {
                            adapted_format_string += symb;
                        }
                        break;
                    #endregion
                    #region Date and time
                    case 'A':
                        if(has_date_format)
                        {
                            if (format_pos + 4 < format_string.Length && format_string.Substring(format_pos, 5) == "AM/PM")
                            {
                                adapted_format_string += "tt";
                                format_pos += 4;
                            }
                            if (format_pos + 2 < format_string.Length && format_string.Substring(format_pos, 3) == "A/P")
                            {
                                adapted_format_string += 't';
                                format_pos += 2;
                            }
                        }
                        break;
                    case 'h':
                        if (format_pos + 1 < format_string.Length && rx_ampm_fwd.Match(format_string, format_pos + 1).Success)
                        {
                            adapted_format_string += 'h';
                        }
                        else
                        {
                            adapted_format_string += 'H';
                        }
                        h_found = true;
                        has_date_format = true;
                        break;
                    case 'M':
                        adapted_format_string += 'M';
                        has_date_format = true;
                        break;
                    case 'm':
                        if (h_found || format_pos + 1 < format_string.Length && rx_seconds_fwd.Match(format_string, format_pos + 1).Success)
                        {
                            adapted_format_string += 'm';
                        }
                        else
                        {
                            adapted_format_string += 'M';
                        }
                        has_date_format = true;
                        break;
                    case 'D':
                    case 'd':
                    case 'Y':
                    case 'y':
                        has_date_format = true;
                        adapted_format_string += symb.ToString().ToLower();
                        break;
                    case 's':
                        has_date_format = true;
                        adapted_format_string += symb;
                        break;
                    case '0':
                        if (has_date_format)
                        {
                            adapted_format_string += 'f';
                        }
                        else
                        {
                            adapted_format_string += symb;
                        }
                        break;
                    #endregion
                    #region @
                    case '@':
                        adapted_format_string += strValue;
                        break;
                    #endregion
                    default:
                        adapted_format_string += symb;
                        break;
                }
            }
            
            return adapted_format_string;
        }

        private static bool HasDateFormat(string format_string)
        {
            bool has;
            AdaptNumberFormatString("", format_string, out has);
            return has;
        }

        internal static string IncreaseDecimal(string format_string)
        {
            if (!HasDateFormat(format_string))
            {
                int period_pos = FindLastSymbolPosition(format_string, ".");
                if(period_pos != -1)
                {
                    return format_string.Insert(period_pos + 1, "0");
                }
                else
                {
                    int zero_pos = FindLastSymbolPosition(format_string, "0#?");
                    if (zero_pos != -1)
                    {
                        return format_string.Insert(zero_pos + 1, ".0");
                    }
                }
            }
            return format_string;
        }

        internal static string DecreaseDecimal(string format_string)
        {
            if (!HasDateFormat(format_string))
            {
                int period_pos = FindLastSymbolPosition(format_string, ".");
                if (period_pos != -1 && period_pos + 1 != format_string.Length)
                {
                    char symb_after_period = format_string[period_pos + 1];
                    if (symb_after_period == '0' || symb_after_period == '#' || symb_after_period == '?')
                    {
                        if (period_pos + 2 == format_string.Length || FindLastSymbolPosition(format_string.Substring(period_pos + 2), "0#?") == -1)
                        {
                            return format_string.Remove(period_pos, 2);
                        }
                        else
                        {
                            return format_string.Remove(period_pos + 1, 1);
                        }
                    }
                }
            }
            return format_string;
        }

        internal static string IncreaseDecimalGeneral(Calculation.Formulas.DataObjects.IDataObject data_value)
        {
            Calculation.Formulas.DataObjects.ObjectNumber numeric_value = data_value as Calculation.Formulas.DataObjects.ObjectNumber;
            if (numeric_value == null)
            {
                return "General";
            }

            return IncreaseDecimal(DecodeGeneralFormat(numeric_value.Value));
        }

        internal static string DecreaseDecimalGeneral(Calculation.Formulas.DataObjects.IDataObject data_value)
        {
            Calculation.Formulas.DataObjects.ObjectNumber numeric_value = data_value as Calculation.Formulas.DataObjects.ObjectNumber;
            if (numeric_value == null)
            {
                return "General";
            }

            return DecreaseDecimal(DecodeGeneralFormat(numeric_value.Value));
        }

        private static int FindLastSymbolPosition(string format_string, string searched_symbols)
        {
            int last_found_position = -1;
            for (int format_pos = 0; format_pos < format_string.Length; ++format_pos)
            {
                char symb = format_string[format_pos];
                switch (symb)
                {
                    #region alignment symbols
                    case '_':
                    case '*':
                        if (format_pos + 1 < format_string.Length)
                        {
                            ++format_pos;
                        }
                        break;
                    #endregion
                    #region "as is" symbols
                    case '\\':
                        if (format_pos + 1 < format_string.Length)
                        {
                            ++format_pos;
                        }
                        break;
                    case '\"':
                        if (format_pos + 1 < format_string.Length)
                        {
                            int next_quot_pos = format_string.IndexOf('\"', format_pos + 1);
                            if (next_quot_pos == -1)
                            {
                                format_pos = format_string.Length;
                                break;
                            }
                            format_pos = next_quot_pos;
                        }
                        break;
                    #endregion
                    #region color and conditions
                    case '[':
                        if (format_pos + 1 < format_string.Length)
                        {
                            int next_quot_pos = format_string.IndexOf(']', format_pos + 1);
                            if (next_quot_pos == -1)
                            {
                                next_quot_pos = format_string.Length - 1;
                            }
                            format_pos = next_quot_pos;
                        }
                        break;
                    #endregion
                    // Stop searching on exponent found
                    case 'E':
                    case 'e':
                        return last_found_position;
                    default:
                        if (searched_symbols.LastIndexOf(symb) != -1)
                        {
                            last_found_position = format_pos;
                        }
                        break;
                }
            }
            return last_found_position;
        }
        private static Regex sections_splitter = new Regex("^(?<positive>([^;]|\\\\;|(\".*\")|_;|\\*;)+)(;(?<negative>([^;]|\\\\;|(\".*\")|_;|\\*;)*)(;(?<zero>([^;]|\\\\;|(\".*\")|_;|\\*;)*)(;(?<text>([^;]|\\\\;|(\".*\")|_;|\\*;)*))?)?)?$");
        internal static bool SplitFormatString(string format_string, out string positive, out string negative, out string zero, out string text)
        {
            Match match = sections_splitter.Match(format_string);
            if (match.Success)
            {
                GroupCollection groups = match.Groups;
                positive = groups["positive"].Value;
                negative = groups["negative"].Value;
                zero = groups["zero"].Value;
                text = groups["text"].Value;
                return true;
            }
            positive = negative = zero = text = "";
            return false;
        }

        internal static string MergeFormatString(string positive, string negative, string zero, string text)
        {
            if(text.Length != 0)
            {
                return positive + ";" + negative + ";" + zero + ";" + text;
            }
            else if (zero.Length != 0)
            {
                return positive + ";" + negative + ";" + zero;
            }
            else if (negative.Length != 0)
            {
                return positive + ";" + negative;
            }
            else
            {
                return positive;
            }
        }
    }
}
