/*
 * Copyright (c) 2006, Clever Age
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Clever Age nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
Modification Log
LogNo. |Date       |ModifiedBy   |BugNo.   |Modification                                                      |
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
RefNo-1 22-Jan-2008 Sandeep S     1832335   to avoid New line inserted in note content after roundtrip conversions                 
 * RefNo-2  15-Sep-2008 Sandeep S   New feature   Changes for formula implementation.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
*/

using System;
using System.Xml;
using System.Collections;
using CleverAge.OdfConverter.OdfConverterLib;
using CleverAge.OdfConverter.OdfZipUtils;
using System.Text.RegularExpressions;
#if !MONO
using System.Windows.Forms;
using System.Drawing;
#endif

namespace CleverAge.OdfConverter.Spreadsheet
{
    /// <summary>
    /// Postprocessor which allows to move sharedstrings into cells.
    /// </summary>
    public class OdfSharedStringsPostProcessor : AbstractPostProcessor
    {
        private Stack sharedStringContext;
        private const string PXSI_NAMESPACE = "urn:cleverage:xmlns:post-processings:shared-strings";
        private bool isInSharedString;
        private bool isString;
        private string stringNumber;
        private Element sharedStringElement;
        private bool isPxsi;
        private bool isPxsiNumber;
        private bool v;
        private bool sst;
        private Hashtable stringCellValue;

        public OdfSharedStringsPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.sharedStringContext = new Stack();
            this.isInSharedString = false;
            this.isString = false;
            this.isPxsi = false;
            this.isPxsiNumber = false;
            this.v = false;
            stringCellValue = new Hashtable();

        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {

            if ("si".Equals(localName))
            {
                this.isPxsi = true;
            }

            else if ("v".Equals(localName))
            {
                this.v = true;
            }
            else if ("sst".Equals(localName))
            {
                this.sst = true;
            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }


        }

        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            if (this.isPxsi)
            {
                if ("number".Equals(localName))
                {
                    this.isPxsiNumber = true;
                }
            }
            else if (this.v)
            {
            }
            else if (this.sst)
            {
            }
            else
            {
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }

        }

        public override void WriteString(string text)
        {
            if (this.isPxsiNumber)
            {
                this.stringNumber = text;
            }
            else if (this.isPxsi)
            {
                if (!stringCellValue.ContainsKey(this.stringNumber))
                {
                    stringCellValue.Add(this.stringNumber, text);
                }
            }
            else if (this.v)
            {
                //int index = Convert.ToInt32(text);

                if (stringCellValue.ContainsKey(text))
                {
                    this.nextWriter.WriteString(stringCellValue[text].ToString());
                }

            }
            else if (this.sst)
            {
            }
            //Code Added by Sateesh Reddy
            //Description:pivot table with 2 same column names
            else if (text.Contains("ExcelPivotField_"))
            {
                string[] arrVal = new string[3];
                arrVal = text.Split('_');
                string currentValue = arrVal[1].ToString();
                string[] cacheValues = arrVal[2].Split('+');
                string[] sheetValues = arrVal[3].Split('+');

                if (cacheValues.Length == sheetValues.Length)
                {
                    int count = 0;

                    for (int i = 1; i < sheetValues.Length; i++)
                    {
                        int b = 2;
                        for (int k = 1; k < sheetValues.Length; k++)
                        {
                            if (sheetValues[i].Equals(sheetValues[k]))
                            {
                                count++;
                                if (count > 1)
                                {
                                    string modifiedValue = string.Concat(sheetValues[k], b);
                                    if (sheetValues[i].Equals(modifiedValue))
                                    { }
                                    else
                                    {
                                        sheetValues.SetValue(string.Concat(sheetValues[k], b), k);
                                        b = b + 1;
                                    }
                                }
                            }
                        }
                        count = 0;
                    }
                    int j = 1;
                    for (int i = 1; i < cacheValues.Length; i++)
                    {
                        if (cacheValues[i].Equals(currentValue))
                        {
                            j = i;
                        }
                    }

                    string excelPivotField = sheetValues[j].ToString();

                    if (excelPivotField != null)
                    {
                        this.nextWriter.WriteString(excelPivotField);
                    }
                    else
                    {
                        this.nextWriter.WriteString(currentValue);
                    }
                }
                else
                {
                    this.nextWriter.WriteString(currentValue);
                }

            }
            //End Of Code for Pivot table names.


            /*
              Bug No.         :1805599
              Code Modified By:Vijayeta
              Date            :6th Nov '07
              Description     :If New Line(\n) is present in the text content, Insert empty <text:p> nodes
           */

            else if (text.Contains("SonataAnnotation"))
            {

                string[] content = text.Split('|');
                string style = content[content.Length - 1];
                string textContent = content[1];
                if (textContent.Contains("\n"))
                {
                    char[] newLine = new char[] { '\n' };
                    string[] p = textContent.Split(newLine);
                    for (int i = 0; i < p.Length; i++)
                    {
                        if (p[i] == "")
                        {
                            //RefNo-1
                            //WriteStartElement("text", "p", "urn:oasis:names:tc:opendocument:xmlns:text:1.0");
                            WriteStartElement("text", "span", "urn:oasis:names:tc:opendocument:xmlns:text:1.0");
                            WriteEndElement();
                            //RefNo-1
                            if (i + 1 < p.Length)
                            {
                                WriteEndElement();
                                //RefNo-1
                                WriteStartElement("text", "p", "urn:oasis:names:tc:opendocument:xmlns:text:1.0");
                            }
                        }
                        if (p[i] != "")
                        {
                            //RefNo-1
                            //WriteStartElement("text", "p", "urn:oasis:names:tc:opendocument:xmlns:text:1.0");
                            WriteStartElement("text", "span", "urn:oasis:names:tc:opendocument:xmlns:text:1.0");
                            nextWriter.WriteStartAttribute("text", "style-name", "urn:oasis:names:tc:opendocument:xmlns:text:1.0");
                            nextWriter.WriteValue(style);
                            nextWriter.WriteEndAttribute();
                            this.nextWriter.WriteString(p[i]);
                            WriteEndElement();
                            //RefNo-1
                            if (i + 1 < p.Length)
                            {
                                WriteEndElement();
                                //RefNo-1
                                WriteStartElement("text", "p", "urn:oasis:names:tc:opendocument:xmlns:text:1.0");
                            }
                        }
                    }

                }
            }
            //End of modification for the bug 1805599

            else if (text.StartsWith("sonataColumnWidth:"))
            {
                this.nextWriter.WriteString(GetColumnWidth(text.Substring(18)));
            }
            else if (text.StartsWith("sonataChartWidth:"))
            {
                this.nextWriter.WriteString(GetChartWidth(text.Substring(17)));
            }

             // Image Cropping   Added by Sonata
            else if (text.Contains("image-props"))
            {
                string[] arrVal = new string[6];
                arrVal = text.Split(':');
                string source = arrVal[1].ToString();
                int left = int.Parse(arrVal[2].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                int right = int.Parse(arrVal[3].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                int top = int.Parse(arrVal[4].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                int bottom = int.Parse(arrVal[5].ToString(), System.Globalization.CultureInfo.InvariantCulture);

                string tempFileName = AbstractConverter.inputTempFileName.ToString();
                ZipResolver resolverObj = new ZipResolver(tempFileName);
                ZipArchiveWriter zipObj = new ZipArchiveWriter(resolverObj);
                string imgaeValues = zipObj.ImageCopyBinary(source);
                zipObj.Close();
                resolverObj.Dispose();

                string[] arrValues = new string[3];
                arrValues = imgaeValues.Split(':');
                double width = double.Parse(arrValues[0].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                double height = double.Parse(arrValues[1].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                double res = double.Parse(arrValues[2].ToString(), System.Globalization.CultureInfo.InvariantCulture);


                double cx = width * 2.54 / res;
                double cy = height * 2.54 / res;

                double odpLeft = (left * cx / 100000) / 2.54;
                double odpRight = (right * cx / 100000) / 2.54;
                double odpTop = (top * cy / 100000) / 2.54;
                double odpBottom = (bottom * cy / 100000) / 2.54;

                string result = string.Concat("rect(", string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.##}", odpTop) + "in" + " " + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.##}", odpRight) + "in" + " " + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.##}", odpBottom) + "in" + " " + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.##}", odpLeft) + "in", ")");
                this.nextWriter.WriteString(result);



            }
            //Strat of RefNo-2
            else if (text.StartsWith("sonataOoxFormula"))
            {

                this.nextWriter.WriteString(GetFormula(text.Substring(16)));

            }
            //End of RefNo-2
            else
            {

                this.nextWriter.WriteString(text);
            }


        }

        public override void WriteEndAttribute()
        {
            if (this.isPxsiNumber)
            {
                this.isPxsiNumber = false;
            }
            else if (this.isPxsi)
            {

            }

            else if (this.v)
            {
            }
            else if (this.sst)
            {
            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }



        }

        public override void WriteEndElement()
        {
            if (this.isPxsi)
            {
                this.isPxsi = false;
            }
            else if (this.v)
            {
                this.v = false;
            }
            else if (this.sst)
            {
                this.sst = false;
            }

            else
            {
                this.nextWriter.WriteEndElement();
            }



        }

        //Strat of RefNo-2
        protected string GetFormula(string strOoxFormula)
        {
            int intIndexSheetName = strOoxFormula.IndexOf("##shtName##");
            string strFormulaToTrans = "";
            if (intIndexSheetName != -1)
            {
                strFormulaToTrans = strOoxFormula.Substring(0, intIndexSheetName);
                string[,] arrSheetName = GetSheetNames(strOoxFormula.Substring(intIndexSheetName + 11));
                for (int intCnt = 0; intCnt < arrSheetName.Length / 2; intCnt++)
                {
                    if (arrSheetName[intCnt, 0].ToString() != arrSheetName[intCnt, 1].ToString())
                    {
                        strFormulaToTrans = strFormulaToTrans.Replace("'" + arrSheetName[intCnt, 0].ToString() + "':", "'" + arrSheetName[intCnt, 1].ToString() + "':");
                        strFormulaToTrans = strFormulaToTrans.Replace("'" + arrSheetName[intCnt, 0].ToString() + "'!", "'" + arrSheetName[intCnt, 1].ToString() + "'!");
                        strFormulaToTrans = strFormulaToTrans.Replace(arrSheetName[intCnt, 0].ToString() + ":", arrSheetName[intCnt, 1].ToString() + ":");
                        strFormulaToTrans = strFormulaToTrans.Replace(arrSheetName[intCnt, 0].ToString() + "!", arrSheetName[intCnt, 1].ToString() + "!");
                    }
                }
            }
            else
                strFormulaToTrans = strOoxFormula;

            if (strFormulaToTrans != "")
            {
                strFormulaToTrans = TransOoxRefToOdf(strFormulaToTrans);

                strFormulaToTrans = TransOoxParmsToOdf(strFormulaToTrans);
            }


            return strFormulaToTrans;
        }

        protected string TransOoxRefToOdf(string strExpresion)
        {
            string strOoxExpression = strExpresion;
            string strFnAnalysis = "(BESSELI(BESSELJ(BESSELK(BESSELY(BIN2DEC(BIN2HEX(BIN2OCT(COMPLEX(CONVERT(DEC2BIN(DEC2HEX(DEC2OCT(DELTA(ERF(ERFC(GESTEP(HEX2BIN(HEX2DEC(HEX2OCT(IMABS(IMAGINARY(IMARGUMENT(IMCONJUGATE(IMCOS(IMDIV(IMEXP(IMLN(IMLOG10(IMLOG2(IMPOWER(IMPRODUCT(IMREAL(IMSIN(IMSQRT(IMSUB(IMSUM(OCT2BIN(OCT2DEC(OCT2HEX(EDATE(EOMONTH(NETWORKDAYS(WEEKNUM(WORKDAY(YEARFRAC(ISEVEN(ISODD(FACTDOUBLE(GCD(LCM(MROUND(MULTINOMIAL(QUOTIENT(RANDBETWEEN(SERIESSUM(SQRTPI(ACCRINT(ACCRINTM(AMORDEGRC(AMORLINC(COUPDAYBS(COUPDAYS(COUPDAYSNC(COUPNCD(COUPNUM(COUPPCD(DOLLARDE(DOLLARFR(FVSCHEDULE(INTRATE(MDURATION(ODDFPRICE(ODDFYIELD(ODDLPRICE(ODDLYIELD(PRICEDISC(PRICEMAT(RECEIVED(TBILLEQ(TBILLPRICE(TBILLYIELD(XIRR(YIELDDISC(YIELDMAT(CUMIPMT(CUMPRINC(EFFECT(NOMINAL(";
            //DISC(PRICE(YIELD(DURATION
            string strFnDate = "(DAYSINMONTH(DAYSINYEAR(ISLEAPYEAR(WEEKSINYEAR(";
            string strFnDateDif = "(MONTHS(WEEKS(YEARS(";
            ArrayList arlFnToChange = new ArrayList();

            //split the expression on charecters and add only cell reference string to array.
            ArrayList arlCellRef = new ArrayList();
            char[] arrOperators = { ',', '^', '*', '/', '+', '-', '&', '=', '<', '>', '=', '%', '(', ')', '{', '}', ';' };

            string[] arrContents = strOoxExpression.Split(arrOperators, StringSplitOptions.RemoveEmptyEntries);

            string strVal = "";
            //int intResult;
            for (int i = 0; i < arrContents.Length; i++)
            {
                //if the value is string i.e. starts with ", no need to consider.
                if (strVal != "")
                {
                    strVal = strVal + arrContents[i].ToString();
                    if (IsValidString(strVal))
                    {
                        strVal = "";
                    }
                }
                else if (arrContents[i].ToString().StartsWith("\""))
                {
                    strVal = arrContents[i].ToString();
                    if (IsValidString(strVal))
                    {
                        strVal = "";
                    }
                }
                //if its a number, then no need to consider.
                else if (!(Regex.IsMatch(arrContents[i].ToString().Trim(), @"[^0-9.]")))
                {
                    strVal = "";
                }
                //if it contains any of the cell reference chars : or !.. consider
                else if (arrContents[i].ToString().Contains(":") || arrContents[i].ToString().Contains("!"))
                {
                    arlCellRef.Add(arrContents[i].ToString().Trim());
                    strVal = "";
                }
                //if the string starts with $ or alphabet and ends with number, then its a cell address.
                else
                {
                    strVal = arrContents[i].ToString().Trim();

                    char chrFirst = strVal[0];
                    char chrLast = strVal[strVal.Length - 1];
                    int intCharFirst = (int)chrFirst;

                    if (strVal.Length <= 12 && (char.IsLetter(strVal, 0) || strVal[0] == '$') && char.IsNumber(strVal, strVal.Length - 1) && (strVal.ToUpperInvariant() != "IMLOG10" || strVal.ToUpperInvariant() != "IMLOG2" || strVal.ToUpperInvariant() != "SUMX2MY2" || strVal.ToUpperInvariant() != "SUMX2PY2" || strVal.ToUpperInvariant() != "SUMXMY2"))
                    {
                        arlCellRef.Add(strVal);
                    }
                    else
                    {
                        if (strFnDateDif.Contains("(" + arrContents[i].ToString().Trim().ToUpperInvariant() + "("))
                        {
                            if (!arlFnToChange.Contains(arrContents[i].ToString().Trim() + "(1"))
                                arlFnToChange.Add(arrContents[i].ToString().Trim() + "(1");
                        }
                        else if (strFnDate.Contains("(" + arrContents[i].ToString().Trim().ToUpperInvariant() + "("))
                        {
                            if (!arlFnToChange.Contains(arrContents[i].ToString().Trim() + "(2"))
                                arlFnToChange.Add(arrContents[i].ToString().Trim() + "(2");
                        }
                        else if (strFnAnalysis.Contains("(" + arrContents[i].ToString().Trim().ToUpperInvariant() + "("))
                        {
                            if (!arlFnToChange.Contains(arrContents[i].ToString().Trim() + "(3"))
                                arlFnToChange.Add(arrContents[i].ToString().Trim() + "(3");
                        }
                    }

                    //chk for key word and replace with com.addin
                    strVal = "";
                }
            }
            //store the oox and corresponding odf represention in 2-dimentional array
            string[,] arrOoxToOdf = new string[arlCellRef.Count, 2];
            int intCnt = 0;
            foreach (object obj in arlCellRef)
            {
                string strOoxCellRef = obj.ToString();
                arrOoxToOdf[intCnt, 0] = strOoxCellRef;
                //if reference not contains ! then no reference with sheetname exists.
                if (!strOoxCellRef.Contains("!"))
                {
                    strOoxCellRef = "[." + strOoxCellRef;
                    strOoxCellRef = strOoxCellRef.Replace(" ", "]![.");
                    strOoxCellRef = strOoxCellRef.Replace(":", ":.");
                    strOoxCellRef = strOoxCellRef + "]";
                    arrOoxToOdf[intCnt, 1] = strOoxCellRef;
                }
                //reference with sheetname
                else
                {
                    string strSheetRange = "";
                    bool blnSheetRange = false;
                    strSheetRange = strOoxCellRef.Substring(0, strOoxCellRef.IndexOf('!'));
                    int intFrom = strOoxCellRef.IndexOf('!');
                    //sheetname with special char
                    if (strSheetRange.Contains("'"))
                    {
                        bool blnInvalid = true;
                        while (blnInvalid)
                        {
                            if (IsValidString(strSheetRange, "'"))
                            {
                                blnInvalid = false;
                            }
                            else
                            {
                                strSheetRange = strOoxCellRef.Substring(0, strOoxCellRef.IndexOf('!', intFrom + 1));
                            }
                        }
                        if (!blnInvalid)
                        {
                            //if sheetname contains :, eg.Sheet1:Sheet3!B2 --> [Sheet1.B2:Sheet3.B2]
                            if (strSheetRange.Contains(":"))
                            {
                                blnSheetRange = true;
                                string strCellRef = strOoxCellRef.Substring(strOoxCellRef.IndexOf('!', intFrom));
                                strSheetRange = "[" + strSheetRange.Substring(0, strSheetRange.IndexOf(':') - 1) + "." + strCellRef + ":" + strSheetRange.Substring(strSheetRange.IndexOf(':')) + "." + strCellRef + "]";
                                arrOoxToOdf[intCnt, 1] = strSheetRange;
                            }
                        }

                    }
                    else
                    {
                        if (strSheetRange.Contains(":"))
                        {
                            blnSheetRange = true;
                            string strCellRef = strOoxCellRef.Substring(strOoxCellRef.IndexOf('!') + 1);
                            strSheetRange = "[" + strSheetRange.Substring(0, strSheetRange.IndexOf(':')) + "." + strCellRef + ":" + strSheetRange.Substring(strSheetRange.IndexOf(':') + 1) + "." + strCellRef + "]";
                            arrOoxToOdf[intCnt, 1] = strSheetRange;
                        }
                    }
                    //if not sheet range
                    if (!blnSheetRange)
                    {
                        strOoxCellRef = "[" + strOoxCellRef;
                        if (strOoxCellRef.Contains("'"))
                        {
                            //chk for special sheet name
                            //strOoxCellRef = strOoxCellRef.Replace('!', '.');
                            string[] arrSheetName = strOoxCellRef.Split('!');
                            string strChkValid = "";
                            for (int i = 0; i < arrSheetName.Length - 1; i++)
                            {
                                strChkValid = strChkValid + arrSheetName[i].ToString();
                                if (IsValidString(strChkValid, "'"))
                                {
                                    strOoxCellRef = strOoxCellRef.Replace(strChkValid + "!", strChkValid + ".");
                                    strChkValid = "";
                                }
                                else
                                {
                                    strChkValid = strChkValid + "!";
                                }
                            }

                        }
                        else
                        {
                            strOoxCellRef = strOoxCellRef.Replace('!', '.');
                        }
                        strOoxCellRef = strOoxCellRef.Replace(":", ":.");
                        strOoxCellRef = strOoxCellRef + "]";

                        //intersection space in oox should be replaced with ! in odf
                        if (strOoxCellRef.Contains(" "))
                        {
                            string[] arrSheetName = strOoxCellRef.Split(' ');
                            string strChkValid = "";
                            for (int i = 0; i < arrSheetName.Length - 1; i++)
                            {
                                strChkValid = strChkValid + arrSheetName[i].ToString();
                                if (IsValidString(strChkValid, "'"))
                                {
                                    strOoxCellRef = strOoxCellRef.Replace(strChkValid + " ", strChkValid + "]![");
                                    strChkValid = "";
                                }
                                else
                                {
                                    strChkValid = strChkValid + " ";
                                }
                            }
                        }
                        arrOoxToOdf[intCnt, 1] = strOoxCellRef;
                    }
                }
                intCnt = intCnt + 1;
            }

            //replace the oox cell refrence with odf cell ref.
            string strOdfFromula = "";
            int intStartIndex = 0;
            //int intEndIndex = 0;

            for (int intReplace = 0; intReplace < arrOoxToOdf.Length / 2; intReplace++)
            {

                intStartIndex = strOoxExpression.IndexOf(arrOoxToOdf[intReplace, 0].ToString());

                strOdfFromula = strOdfFromula + strOoxExpression.Substring(0, intStartIndex);
                strOdfFromula = strOdfFromula + arrOoxToOdf[intReplace, 1].ToString();

                strOoxExpression = strOoxExpression.Remove(0, intStartIndex + arrOoxToOdf[intReplace, 0].ToString().Length);


            }
            strOdfFromula = strOdfFromula + strOoxExpression;

            foreach (object objFn in arlFnToChange)
            {
                string strFnToChange = objFn.ToString();
                string strLast = strFnToChange.Substring(strFnToChange.Length - 1);
                strFnToChange = strFnToChange.Remove(strFnToChange.Length - 1, 1);

                switch (strLast)
                {
                    case "1":
                        strOdfFromula = strOdfFromula.Replace(strFnToChange, "com.sun.star.sheet.addin.DateFunctions.getDiff" + strFnToChange);
                        break;
                    case "2":
                        strOdfFromula = strOdfFromula.Replace(strFnToChange, "com.sun.star.sheet.addin.DateFunctions.get" + strFnToChange);
                        break;
                    case "3":
                        strOdfFromula = strOdfFromula.Replace(strFnToChange, "com.sun.star.sheet.addin.Analysis.get" + strFnToChange);
                        break;
                }
            }

            if (strOdfFromula.Contains("ERROR.TYPE("))
            {
                strOdfFromula = strOdfFromula.Replace("ERROR.TYPE(", "ERRORTYPE(");
            }
            return strOdfFromula;
        }

        protected string TransOoxParmsToOdf(string strExpresion)
        {
            string[] arrParms = strExpresion.Split(',');
            string strOdfExpression = "";
            for (int intComaIndex = 0; intComaIndex < arrParms.Length; intComaIndex++)
            {
                if (arrParms[intComaIndex].ToString().Contains("\"") || arrParms[intComaIndex].ToString().Contains("'"))
                {
                    int intIndexOfDQ = arrParms[intComaIndex].ToString().IndexOf("\"");
                    int intIndexOfSQ = arrParms[intComaIndex].ToString().IndexOf("'");

                    if (intIndexOfDQ < intIndexOfSQ)
                    {
                        if (IsValidString(arrParms[intComaIndex].ToString()))
                            strOdfExpression = strOdfExpression + arrParms[intComaIndex].ToString() + ";";
                        else
                        {
                            bool blnChk = false;
                            string strValid = arrParms[intComaIndex].ToString();
                            while (!blnChk && intComaIndex < arrParms.Length - 1)
                            {
                                intComaIndex++;
                                strValid = strValid + "," + arrParms[intComaIndex].ToString();
                                blnChk = IsValidString(strValid);
                            }
                            strOdfExpression = strOdfExpression + strValid + ";";
                        }

                    }
                    else
                    {
                        if (IsValidString(arrParms[intComaIndex].ToString(), "'"))
                            strOdfExpression = strOdfExpression + arrParms[intComaIndex].ToString() + ";";
                        else
                        {
                            bool blnChk = false;
                            string strValid = arrParms[intComaIndex].ToString();
                            while (!blnChk && intComaIndex < arrParms.Length - 1)
                            {
                                intComaIndex++;
                                strValid = strValid + "," + arrParms[intComaIndex].ToString();
                                blnChk = IsValidString(strValid, "'");
                            }
                            strOdfExpression = strOdfExpression + strValid + ";";
                        }
                    }
                }
                else
                {
                    strOdfExpression = strOdfExpression + arrParms[intComaIndex].ToString() + ";";
                }
            }

            strOdfExpression = "oooc:=" + strOdfExpression.Remove(strOdfExpression.Length - 1, 1);
            //strOdfExpression = strOdfExpression.Replace("\"", "&quot;"); //not required
            // strOdfExpression = strOdfExpression.Replace("'", "&apos;");//not required
            //replace > < & ' " chars with equivalent representation//not required

            //add remove parms 
            strOdfExpression = TransFormulaParms(strOdfExpression);

            //replace            
            /*
             * ERROR.TYPE( --> ERRORTYPE(
             * */

            //Add com.sun.star.sheet.addin.Analysis.get for required functions



            //com.sun.star.sheet.addin.DateFunctions.get
            /*
             * DAYSINMONTH(DAYSINYEAR(ISLEAPYEAR(WEEKSINYEAR(
             * */

            //com.sun.star.sheet.addin.DateFunctions.getDiff
            /*
             * MONTHS(WEEKS(YEARS(
             *
             */

            return strOdfExpression;
        }

        protected string[,] GetSheetNames(string strSheetNames)
        {

            string sheetNames = strSheetNames.Remove(strSheetNames.Length - 2, 2);

            string[] arrSpeperator = { "::" };


            string[] sheetArrayWithSplChars = sheetNames.Split(arrSpeperator, StringSplitOptions.None);

            string[,] sheetArray = new string[sheetArrayWithSplChars.Length, 2];

            for (int i = 0; i < sheetArrayWithSplChars.Length; i++)
            {
                int intSepIndex = sheetArrayWithSplChars[i].IndexOf(':');
                sheetArray[i, 0] = sheetArrayWithSplChars[i].Substring(0, intSepIndex).Replace("'", "''");
                sheetArray[i, 1] = sheetArrayWithSplChars[i].Substring(intSepIndex + 1);
            }

            return sheetArray;
        }

        protected bool IsValidString(string strExpression)
        {
            int intQoutes;
            MatchCollection matches = Regex.Matches(strExpression, @"[""]");
            intQoutes = matches.Count;
            intQoutes = intQoutes % 2;

            if (intQoutes == 0)
                return true;
            else
                return false;
        }

        protected bool IsValidString(string strExpression, string strChkFor)
        {
            int intQoutes;
            MatchCollection matches = Regex.Matches(strExpression, @"[" + strChkFor + "]");
            intQoutes = matches.Count;
            intQoutes = intQoutes % 2;

            if (intQoutes == 0)
                return true;
            else
                return false;
        }

        protected string TransFormulaParms(string strFormula)
        {
            string strTransFormula = "";
            string strFormulaToTrans = "";
            string strExpression = "";

            //chking for key word
            if (strFormula.Contains("ADDRESS(") || strFormula.Contains("PERCENTAGERANK("))
            {
                while (strFormula != "")
                {
                    //TODO : Include other formulas.
                    if (strFormula.Contains("ADDRESS(") || strFormula.Contains("PERCENTAGERANK("))
                    {
                        int[] arrIndex = new int[2];

                        string strFunction = "";
                        int intFunction = 0;
                        int intIndex = 0;

                        arrIndex[0] = strFormula.IndexOf("ADDRESS(");//remove 4th parm:1
                        arrIndex[1] = strFormula.IndexOf("PERCENTAGERANK(");//remove 3rd parm
                        //arrIndex[2] = strFormula.IndexOf("FLOOR(");//remove 3rd parm


                        if (arrIndex[0] >= 0 && arrIndex[0] > arrIndex[1])
                        {
                            strFunction = "ADDRESS(";
                            intFunction = 1;
                            intIndex = arrIndex[0];

                        }
                        else if (arrIndex[1] >= 0 && arrIndex[1] > arrIndex[0])
                        {
                            strFunction = "PERCENTAGERANK(";
                            intFunction = 2;
                            intIndex = arrIndex[1];
                        }
                        //else if (arrIndex[2] >= 0 && arrIndex[2] > arrIndex[1] && arrIndex[2] > arrIndex[0])
                        //{
                        //    strFunction = "FLOOR(";
                        //    intFunction = 3;
                        //    intIndex = arrIndex[2];
                        //}
                        //|| strFormula.Contains("CELING(") || strFormula.Contains("FLOOR("))

                        string strConvertedExp = "";
                        strTransFormula = strTransFormula + strFormula.Substring(0, intIndex);
                        strFormulaToTrans = strFormula.Substring(intIndex);
                        if (strFormulaToTrans.Contains(")"))
                        {
                            strExpression = GetExpression(strFormulaToTrans);
                        }
                        if (strExpression != "")
                        {
                            //TODO : if function with one parm and second to be added.
                            if (strExpression.Contains(";"))
                            {
                                if (intFunction == 1)
                                {
                                    strConvertedExp = AddRemoveParm(strExpression, 4, false, true, "");
                                }
                                else
                                {
                                    strConvertedExp = AddRemoveParm(strExpression, 3, false, true, "");
                                }
                            }
                            else
                            {
                                strConvertedExp = strExpression;
                            }
                            //use converted and send remaining for chk
                            strTransFormula = strTransFormula + strConvertedExp;
                            strFormula = strFormulaToTrans.Remove(0, strExpression.Length);
                        }
                        else
                        {
                            //remove the key word and send remaing for chk
                            strTransFormula = strTransFormula + strFormulaToTrans.Substring(0, strFunction.Length);
                            strFormula = strFormulaToTrans.Substring(strFormula.Length);
                        }
                    }
                    else
                    {
                        strTransFormula = strTransFormula + strFormula;
                        strFormula = "";
                    }
                }
            }
            else
            {
                strTransFormula = strFormula;
            }
            return strTransFormula;
        }

        protected string AddRemoveParm(string strExpresion, int intPossition, bool blnParmAdd, bool blnParmRemove, string strParmAdd)
        {
            string strTransFormula = "";
            string strFormulaKeyword = strExpresion.Substring(0, strExpresion.IndexOf('('));
            strExpresion = strExpresion.Substring(strExpresion.IndexOf('(') + 1);
            strExpresion = strExpresion.Substring(0, strExpresion.Length - 1);

            ArrayList arlParms = GetParms(strExpresion);

            if (arlParms.Count >= intPossition)
            {
                if (blnParmAdd == true && blnParmRemove == false)
                {
                    strTransFormula = strFormulaKeyword + "(";
                    for (int i = 0; i < arlParms.Count; i++)
                    {
                        if (intPossition == i + 1)
                        {
                            strTransFormula = strTransFormula + strParmAdd + ";" + arlParms[i].ToString() + ";";
                        }
                        else
                        {
                            strTransFormula = strTransFormula + arlParms[i].ToString() + ";";
                        }
                    }
                    strTransFormula = strTransFormula.Substring(0, strTransFormula.Length - 1) + ")";
                }
                else if (blnParmAdd == false && blnParmRemove == true)
                {
                    strTransFormula = strFormulaKeyword + "(";
                    for (int i = 0; i < arlParms.Count; i++)
                    {
                        if (intPossition != i + 1)
                        {
                            strTransFormula = strTransFormula + arlParms[i].ToString() + ";";
                        }
                    }
                    strTransFormula = strTransFormula.Substring(0, strTransFormula.Length - 1) + ")";
                }
            }
            else
            {
                strTransFormula = strFormulaKeyword + "(" + strExpresion + ")";
            }
            return strTransFormula;

        }

        protected string GetExpression(string strExpression)
        {
            string strChkValidExp = "";
            bool blnValidExp = false;
            int intChkFrom = 0;

            while (!blnValidExp)
            {
                if (intChkFrom == 0)
                    intChkFrom = strExpression.IndexOf(')');
                else
                    intChkFrom = strExpression.IndexOf(')', intChkFrom + 1);
                strChkValidExp = strExpression.Substring(0, intChkFrom + 1);

                blnValidExp = IsValidExpression(strChkValidExp);
            }

            return strChkValidExp;


        }

        protected ArrayList GetParms(string strExpression)
        {
            ArrayList arlParms = new ArrayList();

            string[] strArrParm = strExpression.Split(';');
            string strExpToVald = "";
            if (strArrParm.Length > 0)
            {

                for (int i = 0; i < strArrParm.Length; i++)
                {
                    if (strExpToVald == "")
                    {
                        strExpToVald = strArrParm[i].ToString();
                    }
                    else
                    {
                        strExpToVald = strExpToVald + ";" + strArrParm[i].ToString();
                    }
                    if (strExpToVald.StartsWith("\""))
                    {
                        if (IsValidString(strExpToVald))
                        {
                            arlParms.Add(strExpToVald);
                            strExpToVald = "";
                        }
                    }
                    else if (IsValidExpression(strExpToVald))
                    {
                        strExpToVald = TransFormulaParms(strExpToVald);
                        arlParms.Add(strExpToVald);
                        strExpToVald = "";
                    }
                }
            }

            return arlParms;

        }

        protected bool IsValidExpression(string strExpression)
        {
            int intOpenBracket;
            int intClosedBracket;
            int intOpenFlower;
            int intClosedFlower;
            int intQoutes;


            MatchCollection matches = Regex.Matches(strExpression, @"[""]");
            intQoutes = matches.Count;
            intQoutes = intQoutes % 2;

            if (intQoutes == 0)
            {
                string strChkQoutes = strExpression;
                while (strChkQoutes.Contains(@""""))
                {
                    int intStart = 0;
                    int intEnd = 0;
                    intStart = strChkQoutes.IndexOf('"');
                    intEnd = strChkQoutes.IndexOf('"', intStart + 1);

                    string strRemvChars = strChkQoutes.Substring(intStart + 1, intEnd - intStart - 1);

                    Regex r = new Regex(@"[(){}]");
                    strRemvChars = r.Replace(strRemvChars, " ");

                    strChkQoutes = strChkQoutes.Substring(0, intStart) + strRemvChars + strChkQoutes.Substring(intEnd + 1);
                    //strChkQoutes = strChkQoutes.Substring(0, intStart);
                }
                strExpression = strChkQoutes;
            }

            matches = Regex.Matches(strExpression, @"[/(]");
            intOpenBracket = matches.Count;
            matches = Regex.Matches(strExpression, @"[/)]");
            intClosedBracket = matches.Count;
            matches = Regex.Matches(strExpression, @"[/{]");
            intOpenFlower = matches.Count;
            matches = Regex.Matches(strExpression, @"[/}]");
            intClosedFlower = matches.Count;
            matches = Regex.Matches(strExpression, @"[""]");
            intQoutes = matches.Count;
            intQoutes = intQoutes % 2;

            if (intOpenBracket == intClosedBracket && intOpenFlower == intClosedFlower && intQoutes == 0)
            {
                return true;
            }
            else
            {
                return false;
            }


        }

        //End of RefNo-2
        private string GetChartWidth(string text)
        {
            string fontName = text.Split('|')[1].ToString(System.Globalization.CultureInfo.InvariantCulture);
            string fontSize = text.Split('|')[2].ToString(System.Globalization.CultureInfo.InvariantCulture);
            double defColCount = Convert.ToDouble(text.Split('|')[3], System.Globalization.CultureInfo.InvariantCulture);
            double customColWidth = Convert.ToDouble(text.Split('|')[4], System.Globalization.CultureInfo.InvariantCulture);
            double startOffset = Convert.ToDouble(text.Split('|')[5], System.Globalization.CultureInfo.InvariantCulture);
            double endOffset = Convert.ToDouble(text.Split('|')[6], System.Globalization.CultureInfo.InvariantCulture);
            string defColumnWidth = GetColumnWidth1(false, fontName, fontSize, 0.0);
            double defColumnWidthPX = Convert.ToDouble(defColumnWidth);
            string customColumnWidth = GetColumnWidth1(true, fontName, fontSize, customColWidth);
            double customColumnWidthPX = Convert.ToDouble(customColumnWidth);
            double totalWidthPX = ((defColCount * defColumnWidthPX) + customColumnWidthPX);
            double totalWidthCM = (totalWidthPX / 96.21212121 * 2.54) + endOffset - startOffset;
            return totalWidthCM.ToString(System.Globalization.CultureInfo.InvariantCulture) + "cm";
        }

        protected string GetColumnWidth(string text)
        {
            const double CALC_DPI = 0.7874;
            double preDefinedWidth = 0.0;
            bool isWidDefined = Convert.ToBoolean(text.Split('|')[0].ToString(System.Globalization.CultureInfo.InvariantCulture));
            string fontName = text.Split('|')[1].ToString(System.Globalization.CultureInfo.InvariantCulture);
            string fontSize = text.Split('|')[2].ToString(System.Globalization.CultureInfo.InvariantCulture);
            char[] letters = new char[] { 'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E', 'e', 'F', 'f', 'G', 'g', 'H', 'h', 'I', 'i', 'J', 'j', 'K', 'k', 'L', 'l', 'M', 'm', 'N', 'n', 'O', 'P', 'p', 'Q', 'q', 'R', 'r', 'S', 's', 'T', 't', 'U', 'u', 'V', 'v', 'W', 'w', 'X', 'x', 'Y', 'y', 'Z', 'z' };
            double dblFontSizePt = Convert.ToDouble(fontSize.TrimEnd(letters), System.Globalization.CultureInfo.InvariantCulture);
            double dblMaxDigitWidth = MaxDigitWidth(fontName, dblFontSizePt);
            if (isWidDefined)
            {
                preDefinedWidth = Convert.ToDouble(text.Split('|')[3], System.Globalization.CultureInfo.InvariantCulture);
            }
            double iWidthInPixel = WidthToPixel(dblMaxDigitWidth, preDefinedWidth, isWidDefined);
            double dblWidthInPt = CALC_DPI * iWidthInPixel;
            double columnWidthInch = (dblWidthInPt / 72);
            //double columnWidthInch = (iWidthInPixel / 96.21212);
            double columnWidthCM = columnWidthInch * 2.54;
            return string.Concat(System.Math.Round(columnWidthCM, 3).ToString(System.Globalization.CultureInfo.InvariantCulture), "cm");
        }

        protected string GetColumnWidth1(bool isWidDefined, string fontName, string fontSize, double customWid)
        {
            const double CALC_DPI = 0.7874;
            double preDefinedWidth = 0.0;
            char[] letters = new char[] { 'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E', 'e', 'F', 'f', 'G', 'g', 'H', 'h', 'I', 'i', 'J', 'j', 'K', 'k', 'L', 'l', 'M', 'm', 'N', 'n', 'O', 'P', 'p', 'Q', 'q', 'R', 'r', 'S', 's', 'T', 't', 'U', 'u', 'V', 'v', 'W', 'w', 'X', 'x', 'Y', 'y', 'Z', 'z' };
            double dblFontSizePt = Convert.ToDouble(fontSize.TrimEnd(letters), System.Globalization.CultureInfo.InvariantCulture);
            double dblMaxDigitWidth = MaxDigitWidth(fontName, dblFontSizePt);
            if (isWidDefined)
            {
                preDefinedWidth = Convert.ToDouble(customWid, System.Globalization.CultureInfo.InvariantCulture);
            }
            double iWidthInPixel = WidthToPixel(dblMaxDigitWidth, preDefinedWidth, isWidDefined);
            //double dblWidthInPt = CALC_DPI * iWidthInPixel;
            //double columnWidthInch = (dblWidthInPt / 72);
            ////double columnWidthInch = (iWidthInPixel / 96.21212);
            //double columnWidthCM = columnWidthInch * 2.54;
            return iWidthInPixel.ToString(System.Globalization.CultureInfo.InvariantCulture);
        }


        protected double MaxDigitWidth(string fontName, double dblSizePt)
        {
#if MONO
            // Avoid dependency on X11 on Linux here... just approximate
            return dblSizePt * 0.75;
#else
            double dblMaxDigitWidth = 0.0;

            // Excel does not use the specified font size, 
            // instead, font size is rounded to an integral pixel size
            // check if this works on non-Windows platforms, otherwise 96dpi could do fine here
            //  Graphics g = System.Drawing.Graphics.FromHwnd(new IntPtr());
            float fSizePxl = (float)Math.Round(96 * dblSizePt / 72);
            float fSizePt = (72 * fSizePxl / 96);
            FontStyle fontStyle;
            if (fontName == "Monotype Corsiva")
                fontStyle = FontStyle.Italic;
            else
                fontStyle = FontStyle.Regular;

            Font font = new Font(fontName, fSizePt, fontStyle, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            for (int i = 0; i < 10; i++)
            {
                // use a Label on a .NET 2.0 Form to measure the width of a digit
                Form f = new Form();
                Label l = new Label();
                l.UseCompatibleTextRendering = false;
                l.Font = font;
                l.AutoSize = true;
                f.Controls.Add(l);
                l.Text = Convert.ToString(i) + Convert.ToString(i);
                f.ResumeLayout(false);
                f.PerformLayout();
                int iWidth2 = l.Width;

                l.Text = Convert.ToString(i);
                f.PerformLayout();

                // we measure twice so we can remove the padding
                int iWidth1 = l.Width;

                if (iWidth2 - iWidth1 > dblMaxDigitWidth)
                {
                    dblMaxDigitWidth = iWidth2 - iWidth1;
                }
            }
            return dblMaxDigitWidth;
#endif
        }

        protected double WidthToPixel(double dblMaxDigitWidth, double preDefinedWidth, bool isWidDefined)
        {
            double colWidPt = 0.0;
            if (isWidDefined)
                colWidPt = preDefinedWidth;
            else
                colWidPt = System.Math.Truncate((((8 * dblMaxDigitWidth) + 5) / dblMaxDigitWidth) * 256) / 256;

            // formula from OpenXml spec §3.3.1.12:
            //
            // To translate the value of width in the file into the column width value at runtime
            // (expressed in terms of pixels), use this calculation:
            //
            // =Truncate(((256 * {width} + Truncate(128/{Maximum Digit Width}))/256)*{Maximum Digit Width})
            double colWidPx = (int)Math.Truncate(((256 * colWidPt + Math.Truncate(128.0 / dblMaxDigitWidth)) / 256) * dblMaxDigitWidth);
            return colWidPx;
        }

    }
}
