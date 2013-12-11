using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas
{
    internal class Precision
    {
        public const int ExcelSignificantDigits = 15;
        public const int ExcelMaxExponent = 308;
        public const int ExcelMinExponent = -308;
        public const double Epsilon = 2.2204460492503131E-16; // Taken from <float.h> in C++.

        // Note: 'number' and 'significance' MUST have the same sign
        public static double Ceiling(double number, double significance)
        {
            double quotient = number / significance;
            if (quotient == 0)
            {
                return 0;
            }
            double quotientTr = Math.Truncate(quotient);

            double nolpiat = 5 * Math.Sign(quotient) * Math.Pow(10, Math.Floor(Math.Log10(Math.Abs(quotient))) - ExcelSignificantDigits);

            if (Math.Abs(quotient - quotientTr) > nolpiat)
            {
                ++quotientTr;
            }

            return quotientTr * significance;
        }

        public static double Floor(double number, double significance)
        {
            double quotient = number / significance;
            if (quotient == 0)
            {
                return 0;
            }

            double nolpiat = 5 * Math.Sign(quotient) * Math.Pow(10, Math.Floor(Math.Log10(Math.Abs(quotient))) - ExcelSignificantDigits);

            return Math.Truncate(quotient + nolpiat) * significance;
        }

        public static double SignZeroPositive(double number)
        {
            return number < 0 ? -1 : 1;
        }

        public static double CorrectPrecisionAfterMultiplyDivision(double number)
        {
            double multiplier = Math.Pow(10, Math.Floor(Math.Log10(Math.Abs(number))) - ExcelSignificantDigits + 1);
            double nolpiat = 0.5 * Math.Sign(number) * multiplier;

            return Math.Truncate((number + nolpiat) / multiplier) * multiplier;
        }
    }
}
