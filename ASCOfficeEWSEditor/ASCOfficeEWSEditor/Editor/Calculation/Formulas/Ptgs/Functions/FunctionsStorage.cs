using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    /// <summary>
    /// Functions information storage
    /// </summary>
    public class FunctionsStorage
    {
        internal FunctionsStorage()
        {
            descriptions = new Dictionary<string, FunctionCreator>();
            FillIn();
        }

        private void FillIn()
        {
            #region Math and trigonometry
            descriptions.Add("SUM", new FunctionCreator("Math and trigonometry", typeof(PtgFuncSum)));
            descriptions.Add("SIN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncSin)));
            descriptions.Add("COS", new FunctionCreator("Math and trigonometry", typeof(PtgFuncCos)));
            descriptions.Add("TAN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncTan)));
            descriptions.Add("PI", new FunctionCreator("Math and trigonometry", typeof(PtgFuncPi)));
            descriptions.Add("SQRT", new FunctionCreator("Math and trigonometry", typeof(PtgFuncSqrt)));
            descriptions.Add("POWER", new FunctionCreator("Math and trigonometry", typeof(PtgFuncPower)));
            descriptions.Add("SIGN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncSign)));
            descriptions.Add("ABS", new FunctionCreator("Math and trigonometry", typeof(PtgFuncAbs)));
            descriptions.Add("EVEN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncEven)));
            descriptions.Add("ODD", new FunctionCreator("Math and trigonometry", typeof(PtgFuncOdd)));
            descriptions.Add("FLOOR", new FunctionCreator("Math and trigonometry", typeof(PtgFuncFloor)));
            descriptions.Add("CEILING", new FunctionCreator("Math and trigonometry", typeof(PtgFuncCeiling)));
            descriptions.Add("ROUNDDOWN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncRoundDown)));
            descriptions.Add("ROUNDUP", new FunctionCreator("Math and trigonometry", typeof(PtgFuncRoundUp)));
            descriptions.Add("ROUND", new FunctionCreator("Math and trigonometry", typeof(PtgFuncRound)));
            descriptions.Add("MROUND", new FunctionCreator("Math and trigonometry", typeof(PtgFuncMRound)));
            descriptions.Add("MOD", new FunctionCreator("Math and trigonometry", typeof(PtgFuncMod)));
            descriptions.Add("INT", new FunctionCreator("Math and trigonometry", typeof(PtgFuncInt)));

            descriptions.Add("ACOS", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("ACOSH", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("ASIN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("ASINH", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("ATAN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("ATANH", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("ATAN2", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("COMBIN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("COSH", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("DEGREES", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("EXP", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("FACT", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("FACTDOUBLE", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("GCD", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("LCM", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("LN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("LOG", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("LOG10", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("MDETERM", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("MINVERSE", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("MMULT", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("MULTINOMIAL", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("PRODUCT", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("QUOTIENT", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("RADIANS", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("RAND", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("RANDBETWEEN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("ROMAN", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SERIESSUM", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SINH", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SQRTPI", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SUBTOTAL", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SUMIF", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SUMIFS", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SUMPRODUCT", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SUMSQ", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SUMXMY2", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SUMX2MY2", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("SUMX2PY2", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("TANH", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            descriptions.Add("TRUNC", new FunctionCreator("Math and trigonometry", typeof(PtgFuncUnsupported)));
            #endregion

            #region Statistical
            descriptions.Add("COUNT", new FunctionCreator("Statistical", typeof(PtgFuncCount)));
            descriptions.Add("AVERAGE", new FunctionCreator("Statistical", typeof(PtgFuncAverage)));
            descriptions.Add("MIN", new FunctionCreator("Statistical", typeof(PtgFuncMin)));
            descriptions.Add("MAX", new FunctionCreator("Statistical", typeof(PtgFuncMax)));
            descriptions.Add("AVEDEV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("AVERAGEA", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("AVERAGEIF", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("AVERAGEIFS", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("BETADIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("BETAINV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("BINOMDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("CHIDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("CHIINV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("CHITEST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("CONFIDENCE", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("CORREL", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUNTA", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUNTBLANK", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUNTIF", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUNTIFS", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("COVAR", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("CRITBINOM", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("DEVSQ", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("EXPONDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("FDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("FINV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("FISHER", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("FISHERINV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("FORECAST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("FREQUENCY", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("FTEST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("GAMMADIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("GAMMAINV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("GAMMALN", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("GEOMEAN", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("GROWTH", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("HARMEAN", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("HYPGEOMDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("INTERCEPT", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("KURT", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("LARGE", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("LINEST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("LOGINV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("LOGNORMDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("MAXA", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("MEDIAN", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("MINA", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("MODE", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("NEGBINOMDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("NORMDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("NORMINV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("NORMSDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("NORMSINV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("PEARSON", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("PERCENTILE", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("PERCENTRANK", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("PERMUT", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("POISSON", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("PROB", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("QUARTILE", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("RANK", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("RSQ", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("SKEW", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("SLOPE", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("SMALL", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("STANDARDIZE", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("STDEV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("STDEVA", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("STDEVP", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("STDEVPA", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("STEYX", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("TDIST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("TINV", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("TREND", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("TRIMMEAN", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("TTEST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("VAR", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("VARA", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("VARP", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("VARPA", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("WEIBULL", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            descriptions.Add("ZTEST", new FunctionCreator("Statistical", typeof(PtgFuncUnsupported)));
            #endregion

            #region Logical (Full)
            descriptions.Add("NOT", new FunctionCreator("Logical", typeof(PtgFuncNot)));
            descriptions.Add("AND", new FunctionCreator("Logical", typeof(PtgFuncAnd)));
            descriptions.Add("OR", new FunctionCreator("Logical", typeof(PtgFuncOr)));
            descriptions.Add("TRUE", new FunctionCreator("Logical", typeof(PtgFuncTrue)));
            descriptions.Add("FALSE", new FunctionCreator("Logical", typeof(PtgFuncFalse)));
            descriptions.Add("IF", new FunctionCreator("Logical", typeof(PtgFuncIf)));
            descriptions.Add("IFERROR", new FunctionCreator("Logical", typeof(PtgFuncIfError)));
            #endregion

            #region Text and data
            descriptions.Add("CONCATENATE", new FunctionCreator("Text and data", typeof(PtgFuncConcatenate)));
            descriptions.Add("UPPER", new FunctionCreator("Text and data", typeof(PtgFuncUpper)));
            descriptions.Add("LOWER", new FunctionCreator("Text and data", typeof(PtgFuncLower)));
            descriptions.Add("T", new FunctionCreator("Text and data", typeof(PtgFuncT)));
            descriptions.Add("REPT", new FunctionCreator("Text and data", typeof(PtgFuncRept)));
            descriptions.Add("EXACT", new FunctionCreator("Text and data", typeof(PtgFuncExact)));
            descriptions.Add("ASC", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("BAHTTEXT", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("CHAR", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("CLEAN", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("CODE", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("DOLLAR", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("FIND", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("FINDB", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("FIXED", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("JIS", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("LEFT", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("LEFTB", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("LEN", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("LENB", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("MID", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("MIDB", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("PHONETIC", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("PROPER", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("REPLACE", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("REPLACEB", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("RIGHT", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("RIGHTB", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("SEARCH", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("SEARCHB", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("SUBSTITUTE", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("TEXT", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("TRIM", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            descriptions.Add("VALUE", new FunctionCreator("Text and data", typeof(PtgFuncUnsupported)));
            #endregion

            #region Information
            descriptions.Add("N", new FunctionCreator("Information", typeof(PtgFuncN)));
            descriptions.Add("ISBLANK", new FunctionCreator("Information", typeof(PtgFuncIsBlank)));
            descriptions.Add("ISERR", new FunctionCreator("Information", typeof(PtgFuncIsErr)));
            descriptions.Add("ISERROR", new FunctionCreator("Information", typeof(PtgFuncIsError)));
            descriptions.Add("ISLOGICAL", new FunctionCreator("Information", typeof(PtgFuncIsLogical)));
            descriptions.Add("ISNA", new FunctionCreator("Information", typeof(PtgFuncIsNa)));
            descriptions.Add("ISNONTEXT", new FunctionCreator("Information", typeof(PtgFuncIsNontext)));
            descriptions.Add("ISNUMBER", new FunctionCreator("Information", typeof(PtgFuncIsNumber)));
            descriptions.Add("ISREF", new FunctionCreator("Information", typeof(PtgFuncIsRef)));
            descriptions.Add("ISTEXT", new FunctionCreator("Information", typeof(PtgFuncIsText)));
            descriptions.Add("ISODD", new FunctionCreator("Information", typeof(PtgFuncIsOdd)));
            descriptions.Add("ISEVEN", new FunctionCreator("Information", typeof(PtgFuncIsEven)));
            descriptions.Add("NA", new FunctionCreator("Information", typeof(PtgFuncNa)));
            descriptions.Add("TYPE", new FunctionCreator("Information", typeof(PtgFuncType)));
            descriptions.Add("ERROR.TYPE", new FunctionCreator("Information", typeof(PtgFuncErrorType)));
            descriptions.Add("INFO", new FunctionCreator("Information", typeof(PtgFuncUnsupported)));
            descriptions.Add("CELL", new FunctionCreator("Information", typeof(PtgFuncUnsupported)));
            #endregion

            #region Engineering
            descriptions.Add("BESSELI", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("BESSELJ", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("BESSELK", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("BESSELY", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("BIN2DEC", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("BIN2HEX", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("BIN2OCT", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("COMPLEX", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("CONVERT", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("DEC2BIN", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("DEC2HEX", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("DEC2OCT", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("DELTA", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("ERF", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("ERFC", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("GESTEP", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("HEX2BIN", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("HEX2DEC", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("HEX2OCT", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMABS", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMAGINARY", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMARGUMENT", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMCONJUGATE", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMCOS", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMDIV", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMEXP", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMLN", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMLOG10", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMLOG2", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMPOWER", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMPRODUCT", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMREAL", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMSIN", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMSQRT", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMSUB", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("IMSUM", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("OCT2BIN", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("OCT2DEC", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            descriptions.Add("OCT2HEX", new FunctionCreator("Engineering", typeof(PtgFuncUnsupported)));
            #endregion

            #region Lookup and reference
            descriptions.Add("ADDRESS", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("AREAS", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("CHOOSE", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("COLUMN", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("COLUMNS", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("HLOOKUP", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("HYPERLINK", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("INDEX", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("INDIRECT", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("LOOKUP", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("MATCH", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("OFFSET", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("ROW", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("ROWS", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("RTD", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("TRANSPOSE", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            descriptions.Add("VLOOKUP", new FunctionCreator("Lookup and reference", typeof(PtgFuncUnsupported)));
            #endregion

            #region Cube
            descriptions.Add("CUBEKPIMEMBER", new FunctionCreator("Cube", typeof(PtgFuncUnsupported)));
            descriptions.Add("CUBEMEMBER", new FunctionCreator("Cube", typeof(PtgFuncUnsupported)));
            descriptions.Add("CUBEMEMBERPROPERTY", new FunctionCreator("Cube", typeof(PtgFuncUnsupported)));
            descriptions.Add("CUBERANKEDMEMBER", new FunctionCreator("Cube", typeof(PtgFuncUnsupported)));
            descriptions.Add("CUBESET", new FunctionCreator("Cube", typeof(PtgFuncUnsupported)));
            descriptions.Add("CUBESETCOUNT", new FunctionCreator("Cube", typeof(PtgFuncUnsupported)));
            descriptions.Add("CUBEVALUE", new FunctionCreator("Cube", typeof(PtgFuncUnsupported)));
            #endregion

            #region Date and time
            descriptions.Add("DATE", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("DATEVALUE", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("DAY", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("DAYS360", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("EDATE", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("EOMONTH", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("HOUR", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("MINUTE", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("MONTH", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("NETWORKDAYS", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("NOW", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("SECOND", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("TIME", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("TIMEVALUE", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("TODAY", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("WEEKDAY", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("WEEKNUM", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("WORKDAY", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("YEAR", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            descriptions.Add("YEARFRAC", new FunctionCreator("Date and time", typeof(PtgFuncUnsupported)));
            #endregion

            #region Financial
            descriptions.Add("ACCRINT", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("ACCRINTM", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("AMORDEGRC", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("AMORLINC", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUPDAYBS", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUPDAYS", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUPDAYSNC", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUPNCD", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUPNUM", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("COUPPCD", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("CUMIPMT", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("CUMPRINC", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("DB", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("DDB", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("DISC", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("DOLLARDE", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("DOLLARFR", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("DURATION", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("EFFECT", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("FV", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("FVSCHEDULE", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("INTRATE", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("IPMT", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("IRR", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("ISPMT", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("LOGEST", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("MDURATION", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("MIRR", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("NOMINAL", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("NPER", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("NPV", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("ODDFPRICE", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("ODDFYIELD", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("ODDLPRICE", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("ODDLYIELD", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("PMT", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("PPMT", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("PRICE", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("PRICEDISC", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("PRICEMAT", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("PV", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("RATE", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("RECEIVED", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("SLN", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("SYD", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("TBILLEQ", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("TBILLPRICE", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("TBILLYIELD", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("VDB", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("XIRR", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("XNPV", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("YIELD", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("YIELDDISC", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            descriptions.Add("YIELDMAT", new FunctionCreator("Financial", typeof(PtgFuncUnsupported)));
            #endregion
        }

        /// <summary>
        /// Obtains the list of supported by the current version of the editor functions
        /// </summary>
        /// <returns>List of function descriptors</returns>
        public List<FunctionDescription> GetFunctionsList()
        {
            List<FunctionDescription> result_list = new List<FunctionDescription>();

            foreach (FunctionCreator creator in descriptions.Values)
            {
                if (creator.function_type.GetConstructor(new Type[0]) != null)
                {
                    PtgFunc func = (PtgFunc)Activator.CreateInstance(creator.function_type);
                    result_list.Add(new FunctionDescription(func.ToString(), creator.group_name, func.ArgumentsTip));
                }
            }
            return result_list;
        }

        internal PtgFunc CreateInstance(string func_name)
        {
            FunctionCreator desc;
            if(!descriptions.TryGetValue(func_name, out desc))
            {
                return null;
            }

            if (desc.function_type.GetConstructor(new Type[0]) != null)
            {
                return (PtgFunc)Activator.CreateInstance(desc.function_type);
            }
            else
            {
                object[] func_name_params = new object[1];
                func_name_params[0] = func_name;
                return (PtgFunc)Activator.CreateInstance(desc.function_type, func_name_params);
            }
        }

        private Dictionary<string, FunctionCreator> descriptions;
    }

    internal class FunctionCreator
    {
        internal FunctionCreator(string group_name, Type function_type)
        {
            this.group_name = group_name;
            this.function_type = function_type;
        }

        internal string group_name;
        internal Type function_type;
    }

}

namespace ASCOfficeEWSEditor.Editor
{
    /// <summary>
    /// Function description
    /// </summary>
    public class FunctionDescription
    {
        internal FunctionDescription(string func_name, string group_name, string arguments_tip)
        {
            FuncName = func_name;
            ArgumentsTip = arguments_tip;
            GroupName = group_name;
        }

        /// <summary>
        /// International function name
        /// </summary>
        public string FuncName { get; private set; }
        /// <summary>
        /// Arguments tip
        /// </summary>
        public string ArgumentsTip { get; private set; }
        /// <summary>
        /// Group name
        /// </summary>
        public string GroupName { get; private set; }
    }


}
