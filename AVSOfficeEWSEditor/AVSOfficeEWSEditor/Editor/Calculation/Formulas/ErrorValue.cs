using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas
{
    /// <summary>
    /// Represents an error value of calculations
    /// </summary>
    public class ErrorValue : IComparable<ErrorValue>
    {
        private ErrorValue(Values value)
        {
            this.value = value;
        }

        /// <summary>
        /// "#DIV/0!" Indicates when any number (including zero) or any error code is divided by zero.
        /// </summary>
        public static readonly ErrorValue division_by_zero = new ErrorValue(Values.division_by_zero);
        /// <summary>
        /// "#N/A" Indicates when a designated value is not available.
        /// </summary>
        public static readonly ErrorValue not_available = new ErrorValue(Values.not_available);
        /// <summary>
        /// "#NAME?" Indicates when what looks like a name is used, but no such name has been defined.
        /// </summary>
        public static readonly ErrorValue wrong_name = new ErrorValue(Values.wrong_name);
        /// <summary>
        /// "#NULL!" Indicates when two areas are required to intersect, but do not.
        /// </summary>
        public static readonly ErrorValue null_value = new ErrorValue(Values.null_value);
        /// <summary>
        /// "#NUM!" Indicates when an argument to a function has a compatible type, but has a value that is outside the domain over which that function is defined.
        /// </summary>
        public static readonly ErrorValue not_numeric = new ErrorValue(Values.not_numeric);
        /// <summary>
        /// "#REF!" Indicates when a cell reference cannot be evaluated
        /// </summary>
        public static readonly ErrorValue bad_reference = new ErrorValue(Values.bad_reference);
        /// <summary>
        /// "#VALUE!" Indicates when an incompatible type argument is passed to a function, or an incompatible type operand is used with an operator.
        /// </summary>
        public static readonly ErrorValue wrong_value_type = new ErrorValue(Values.wrong_value_type);
        /// <summary>
        /// "#UNSUPPORTED_FUNCTION!" Indicates when an unsupported function is used in a formula.
        /// </summary>
        internal static readonly ErrorValue unsupported_function = new ErrorValue(Values.unsupported_function);

        /// <summary>
        /// Error value string representation
        /// </summary>
        /// <returns>string equivalent of the value</returns>
        public override string ToString()
        {
            switch (value)
            {
                case Values.division_by_zero: return "#DIV/0!";
                case Values.not_available: return "#N/A";
                case Values.wrong_name: return "#NAME?";
                case Values.null_value: return "#NULL!";
                case Values.not_numeric: return "#NUM!";
                case Values.bad_reference: return "#REF!";
                case Values.wrong_value_type: return "#VALUE!";
                case Values.unsupported_function: return "#UNSUPPORTED_FUNCTION!";
            }
            return base.ToString();
        }

        /// <summary>
        /// Converts the string representation of an error value to its ErrorValue equivalent.
        /// </summary>
        /// <param name="str_value">A string containing an error value to convert.</param>
        /// <returns>An ErrorValue equivalent to the error value contained in str_value or null if str_value is in wrong format</returns>
        public static ErrorValue Parse(string str_value)
        {
            switch (str_value.ToUpper())
            {
                case "#DIV/0!": return division_by_zero;
                case "#N/A": return not_available;
                case "#NAME?": return wrong_name;
                case "#NULL!": return null_value;
                case "#NUM!": return not_numeric;
                case "#REF!": return bad_reference;
                case "#VALUE!": return wrong_value_type;
                default: return not_available;
            }
        }

        /// <summary>
        /// Converts the string representation of an error value to its ErrorValue equivalent.
        /// </summary>
        /// <param name="str_value">A string containing an error value to convert</param>
        /// <param name="def_val">Default value</param>
        /// <returns>An ErrorValue equivalent to the error value contained in str_value or the passed default value if str_value is in wrong format</returns>
        public static ErrorValue ParseOrDefault(string str_value, ErrorValue def_val)
        {
            return Parse(str_value) ?? def_val;
        }

        #region IComparable<ErrorValue> Members

        /// <summary>
        /// Compares the current object with another object of the same type.
        /// </summary>
        /// <param name="other"></param>
        /// <returns></returns>
        public int CompareTo(ErrorValue other)
        {
            return value.CompareTo(other.value);
        }

        #endregion

        internal int Code
        {
            get
            {
                switch(value)
                {
                    case Values.null_value:
                        return 1;
                    case Values.division_by_zero:
                        return 2;
                    case Values.wrong_value_type:
                        return 3;
                    case Values.bad_reference:
                        return 4;
                    case Values.wrong_name:
                        return 5;
                    case Values.not_numeric:
                        return 6;
                    case Values.not_available:
                        return 7;
                }
                return 0;
            }
        }

        private enum Values
        {
            division_by_zero,
            not_available,
            wrong_name,
            null_value,
            not_numeric,
            bad_reference,
            wrong_value_type,
            unsupported_function
        }

        private Values value;

    }
}
