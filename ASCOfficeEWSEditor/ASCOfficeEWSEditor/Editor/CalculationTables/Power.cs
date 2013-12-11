using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.CalculationTables
{
    internal class Power
    {
        internal static double Pow10(int degree)
        {
            switch(degree)
            {
                case 0:
                    return 1.0;
                case 1:
                    return 10.0;
                case 2:
                    return 100.0;
                case 3:
                    return 1000.0;
                case 4:
                    return 10000.0;
                case 5:
                    return 100000.0;
                case 6:
                    return 1e6;
                case 7:
                    return 1e7;
                case 8:
                    return 1e8;
                case 9:
                    return 1e9;
                case 10:
                    return 1e10;
                case 11:
                    return 1e11;
                case 12:
                    return 1e12;
                case 13:
                    return 1e13;
                case 14:
                    return 1e14;
                case 15:
                    return 1e15;
                case -1:
                    return 0.1;
                case -2:
                    return 0.01;
                case -3:
                    return 0.001;
                case -4:
                    return 0.0001;
                case -5:
                    return 0.00001;
                case -6:
                    return 1e-6;
                case -7:
                    return 1e-7;
                case -8:
                    return 1e-8;
                case -9:
                    return 1e-9;
                case -10:
                    return 1e-10;
                case -11:
                    return 1e-11;
                case -12:
                    return 1e-12;
                case -13:
                    return 1e-13;
                case -14:
                    return 1e-14;
                case -15:
                    return 1e-15;
                default:
                    return Math.Pow(10, degree);
            }
        }
    }
}
