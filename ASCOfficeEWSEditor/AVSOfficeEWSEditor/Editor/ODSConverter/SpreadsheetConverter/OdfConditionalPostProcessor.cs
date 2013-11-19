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
RefNo-1 08-sep-2008 Sandeep S     New feature   Changes for formula implementation.
RefNo-2 23-Jan-2009 Sandeep S     1828899       Changes done to replace Named range 'C' and 'R' with _C and _R.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
*/

using System;
using System.Xml;
using System.Collections;
using CleverAge.OdfConverter.OdfConverterLib;
using CleverAge.OdfConverter.OdfZipUtils;
using System.Text.RegularExpressions;
#if !MONO
using System.Drawing;
using System.Windows.Forms;
#endif

namespace CleverAge.OdfConverter.Spreadsheet
{
    /// <summary>
    /// Postprocessor which allows to move sharedstrings into cells.
    /// </summary>
    public class OdfConditionalPostProcessor : AbstractPostProcessor
    {

        private const string PXSI_NAMESPACE = "urn:cleverage:xmlns:post-processings:shared-strings";

        private string ElementName = "";
        private Hashtable stringSqref;
        private string StyleName;
        private string StringValue;

        public OdfConditionalPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {

            stringSqref = new Hashtable();

        }

        //Start of Refno-1
        protected class Item : IComparable
        {
            private int intPossition;
            public int Possition
            {
                get { return intPossition; }
                set { intPossition = value; }
            }
            private string strFunction;
            public string Function
            {
                get { return strFunction; }
                set { strFunction = value; }
            }
            private int intFunction;
            public int FunctionType
            {
                get { return intFunction; }
                set { intFunction = value; }
            }

            public Item(int intPos, string strFun, int intFun)
            {
                this.intPossition = intPos;
                this.strFunction = strFun;
                this.FunctionType = intFun;
            }

            public Int32 CompareTo(Object objItem)
            {
                Item ItemInstance = (Item)objItem;
                if (this.intPossition > ItemInstance.intPossition)
                {
                    return 1;
                }
                else
                {
                    return -1;
                }
            }
        }
        //End of Refno-1
        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            if ("ConditionalCell".Equals(localName))
            {
                ElementName = "ConditionalCell";
            }
            else if ("conditionalFormatting".Equals(localName))
            {
                ElementName = "conditionalFormatting";
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }


        }

        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            if ("StyleName".Equals(localName))
            {
                ElementName = "StyleName";
            }
            else if (ElementName.Equals("ConditionalCell"))
            {

            }

            else
            {
                if ("sqref".Equals(localName) && ElementName.Equals("conditionalFormatting"))
                {
                    ElementName = "sqref";
                }
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }


        }

        public override void WriteString(string text)
        {
            if (ElementName.Equals("ConditionalCell"))
            {
                StringValue = text;
            }
            else if (ElementName.Equals("StyleName"))
            {
                StyleName = text;
            }
            else if (ElementName.Equals("sqref"))
            {

                this.nextWriter.WriteString(ConditionalModification(stringSqref[text].ToString()));
                ElementName = "";
            }
            //Start of RefNo-1
            else if (text.StartsWith("sonataOdfFormula"))
            {
                this.nextWriter.WriteString(GetFormula(text.Substring(16)));
            }
            else if (text.StartsWith("sonataColumnWidth:"))
            {
                this.nextWriter.WriteString(GetColumnWidth(text.Substring(18)));
            }
            else if (text.StartsWith("sonataRevisionHeaderGuid"))
            {
                Guid revisionHeaderGuid = System.Guid.NewGuid();
                string revisionHeader = revisionHeaderGuid.ToString();
                this.nextWriter.WriteString(revisionHeaderGuid.ToString().ToUpper(System.Globalization.CultureInfo.InvariantCulture));
            }

            //End of RefNo-1
            else
            {
                this.nextWriter.WriteString(text);
            }

        }

        public override void WriteEndAttribute()
        {
            if (ElementName.Equals("StyleName"))
            {
                ElementName = "ConditionalCell";
            }
            else if (ElementName.Equals("ConditionalCell"))
            {

            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }
        }

        public override void WriteEndElement()
        {
            if (ElementName.Equals("ConditionalCell"))
            {
                ElementName = "";
                if (!stringSqref.ContainsKey(StyleName))
                {
                    stringSqref.Add(StyleName, StringValue);
                }
                else
                {
                    stringSqref[StyleName] = stringSqref[StyleName] + " " + StringValue;
                }
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }

        }


        protected int GetRowId(string value)
        {
            int result = 0;

            foreach (char c in value)
            {
                if (c >= '0' && c <= '9')
                {
                    result = (10 * result) + (c - '0');
                }
            }

            return result;
        }

        protected int GetColId(string value)
        {
            int result = 0;

            foreach (char c in value)
            {
                if (c >= 'A' && c <= 'Z')
                {
                    result = (26 * result) + (c - 'A' + 1);
                }
                else
                {
                    break;
                }
            }

            return result;
        }

        protected string ConditionalModification(string value)
        {
            string result = "";
            string prevCell = "";
            int prevRowNum = -1;
            int prevColNum = -1;
            int repeat = 0;

            int thisRowNum = -1;
            int thisColNum = -1;


            //result = value;
            foreach (string single in value.Split())
            {
                if (single.Contains(":"))
                {
                    if (result != "")
                    {
                        if (repeat > 0)
                        {
                            result = result + ":" + prevCell + " " + single;
                            prevCell = "";
                            prevRowNum = -1;
                            prevColNum = -1;
                            repeat = 0;
                        }
                        else
                        {
                            result = result + " " + single;
                        }

                    }
                    else
                    {
                        result = single;
                    }
                }
                else
                {
                    if (result != "")
                    {
                        thisRowNum = GetRowId(single);
                        thisColNum = GetColId(single);
                        if (prevCell != "")
                        {
                            if (thisColNum == prevColNum && thisRowNum == (prevRowNum + 1))
                            {
                                prevCell = single;
                                prevRowNum = thisRowNum;
                                prevColNum = thisColNum;
                                repeat = repeat + 1;
                            }
                            else if (repeat > 0)
                            {
                                result = result + ":" + prevCell + " " + single;
                                prevCell = single;
                                prevRowNum = thisRowNum;
                                prevColNum = thisColNum;
                                repeat = 0;
                            }
                            else
                            {
                                result = result + " " + single;
                                prevCell = single;
                                prevRowNum = thisRowNum;
                                prevColNum = thisColNum;
                            }
                        }
                    }
                    else
                    {
                        result = single;
                        prevCell = single;
                        prevRowNum = thisRowNum;
                        prevColNum = thisColNum;
                    }
                }
            }

            if (repeat > 0 && prevCell != "")
            {
                result = result + ":" + prevCell;
            }


            return result;
        }

        //Start of RefNo-1 : Methods for formula translation
        protected string GetFormula(string strOdfFormula)
        {
            string strOoxFormula = "";
            if (strOdfFormula != "")
            {
                //TODO : chk for the other namespaces:Eg Engginering formulas.
                if (strOdfFormula.StartsWith("oooc:"))
                {
                    strOoxFormula = TranslateToOoxFormula(strOdfFormula.Substring(6));
                }
                //Added: By Vijayeta,Pradeep
                //Date:  1/7/2009, 3/5/2009
                //Desc: Crash Because of Formula, formula begins with 'msoxl:' for the ODS files created by office SP2                
                //File:  Quote.xlsx, Testfeatures M4 and M5 Datarange.ods,Testfeatures_Formula.ods,Text.ods
                else if (strOdfFormula.StartsWith("msoxl:"))
                {
                    // strOoxFormula = TranslateToOoxFormula(strOdfFormula.Substring(7));
                    // Earlier code commented to accomodate SAP.DE.EN.xlsx
                    strOoxFormula = strOdfFormula.Substring(7);
                }
                // To translate ODF1.2 formula
                else if (strOdfFormula.StartsWith("of:"))
                {
                    strOoxFormula = TranslateToOoxFormula(strOdfFormula.Substring(4));
                }
                // To translate formulas having any namespace 
                // Above conditions are not deleted to avoid regression issues for complex formulas,
                // can be deleted later if below condition works for all the namespaces
                else if (strOdfFormula.IndexOf(":=") > 0)
                {
                    strOoxFormula = TranslateToOoxFormula(strOdfFormula.Substring(strOdfFormula.IndexOf(":=") + 2));
                }
                // To translate formulas not having any name space
                else
                {
                    if (strOdfFormula.StartsWith("="))
                    {
                        strOdfFormula = strOdfFormula.Substring(1);
                    }
                    strOoxFormula = TranslateToOoxFormula(strOdfFormula);
                }
            }
            else
            {
                strOoxFormula = strOdfFormula;
            }
            return strOoxFormula;
        }

        protected string TranslateToOoxFormula(string strOdfFormula)
        {
            string strFinalFormula = "";
            string strExpToTrans = "";

            //TODO : chk for other representations starting with &_;
            //to replace &quot; with " operator.
            strOdfFormula = strOdfFormula.Replace("&quot;", "\"");
            //to replace parameter seperation operator.
            strOdfFormula = strOdfFormula.Replace(';', ',');
            //to replace space between keyword and fuction.
            strOdfFormula = strOdfFormula.Replace(" (", "(");
            //to replace union operator 
            strOdfFormula = strOdfFormula.Replace('!', ' ');
            //replace &apos; with ' operator
            strOdfFormula = strOdfFormula.Replace("&apos;", "'");
            //Start of RefNo-2
            //replace c with _c
            strOdfFormula = Regex.Replace(strOdfFormula, @"\b[C|c]\b", "_C");
            strOdfFormula = strOdfFormula.Replace("$_C$", "$C$");
            strOdfFormula = strOdfFormula.Replace("$_C", "$C");
            strOdfFormula = strOdfFormula.Replace("_C$", "C$");
            //replace r with _r
            strOdfFormula = Regex.Replace(strOdfFormula, @"\b[R|r]\b", "_R");
            strOdfFormula = strOdfFormula.Replace("$_R$", "$R$");
            strOdfFormula = strOdfFormula.Replace("$_R", "$R");
            strOdfFormula = strOdfFormula.Replace("_R$", "R$");
            //End of RefNo-2
            /*The functions whose names end with _ADD return the same results as the corresponding Microsoft Excel functions. Use the functions without _ADD to get results based on international standards. For example, the WEEKNUM function calculates the week number of a given date based on international standard ISO 6801, while WEEKNUM_ADD returns the same week number as Microsoft Excel.
             XML representation contains the namespace com.sun.star.sheet.addin.Analysis.getWeeknum(*/
            strOdfFormula = strOdfFormula.Replace("com.sun.star.sheet.addin.Analysis.get", "");
            strOdfFormula = strOdfFormula.Replace("com.sun.star.sheet.addin.DateFunctions.get", "");
            strOdfFormula = strOdfFormula.Replace("com.sun.star.sheet.addin.DateFunctions.getDiff", "");

            strOdfFormula = strOdfFormula.Replace("ERRORTYPE(", "ERROR.TYPE(");
            //chk for parameter discripency

            //TODO : chk for the '[' within the string
            //to translate cell reference
            if (strOdfFormula.Contains("[") && strOdfFormula.Contains("]"))
            {
                while (strOdfFormula.Contains("[") && strOdfFormula.Contains("]"))
                {
                    int intStart = strOdfFormula.IndexOf('[');
                    int intEnd = strOdfFormula.IndexOf(']');
                    int intQuot = strOdfFormula.IndexOf('"');
                    //chk for the '[' within the string
                    if (intQuot != -1 && (intQuot < intStart || intQuot < intEnd))
                    {
                        int intQuotEnd = strOdfFormula.IndexOf('"', intQuot + 1);
                        strFinalFormula = strFinalFormula + strOdfFormula.Substring(0, intQuotEnd + 1);
                        strOdfFormula = strOdfFormula.Substring(intQuotEnd + 1);
                    }
                    else
                    {
                        if (intEnd > intStart)
                        {
                            strFinalFormula = strFinalFormula + strOdfFormula.Substring(0, intStart);
                            strExpToTrans = strOdfFormula.Substring(intStart + 1, intEnd - intStart - 1);
                            strOdfFormula = strOdfFormula.Substring(intEnd + 1);

                            strFinalFormula = strFinalFormula + TranslateToOoxCellRef(strExpToTrans);
                        }
                        else
                        {
                            strFinalFormula = strFinalFormula + strOdfFormula.Substring(0, intEnd + 1);
                            strOdfFormula = strOdfFormula.Substring(intEnd + 1);
                        }
                    }

                    if (!strOdfFormula.Contains("["))
                    {
                        strFinalFormula = strFinalFormula + strOdfFormula;
                    }
                }
            }
            else
            {
                strFinalFormula = strOdfFormula;
            }
            strFinalFormula = TransFormulaParms(strFinalFormula);
            //Added: By Vijayeta, 
            //Date:  1/7/2009
            //Desc:  Formula has absolute address(sheet2)
            //File:  Define_Name_Formulas.ods
            string returnInputFileWithPath = OdfConverter.OdfConverterLib.AbstractConverter.inputFileName;
            if (returnInputFileWithPath != null && strFinalFormula.Contains(returnInputFileWithPath))
            {
                strFinalFormula = strFinalFormula.Replace(String.Concat('\'', returnInputFileWithPath, '\''), "[0]!");
            }
            return strFinalFormula = TransFormulaParms(strFinalFormula);

        }

        protected string TranslateToOoxCellRef(string strOdfCellRef)
        {
            if (strOdfCellRef.StartsWith("."))
            {
                if (strOdfCellRef.Contains(":") && !strOdfCellRef.Contains(":."))
                {
                    strOdfCellRef = strOdfCellRef.Remove(0, 1);
                    return strOdfCellRef = strOdfCellRef.Replace('.', '!');
                }
                else
                    return strOdfCellRef = strOdfCellRef.Replace(".", "");
            }
            else
            {
                //TODO : chk for sheet name
                strOdfCellRef = strOdfCellRef.Replace(":.", ":");
                //return strOdfCellRef = strOdfCellRef.Replace('.', '!');

                strOdfCellRef = strOdfCellRef.Replace('.', '!');
                bool blnNotConverted = true;
                if (strOdfCellRef.Contains(":"))
                {
                    int intRngSeparator = strOdfCellRef.IndexOf(':');
                    if (strOdfCellRef.Substring(0, intRngSeparator).Contains("!") && strOdfCellRef.Substring(intRngSeparator + 1).Contains("!"))
                    {
                        blnNotConverted = false;
                        string strSheetRefFrm = strOdfCellRef.Substring(0, intRngSeparator);
                        string strSheetRefTo = strOdfCellRef.Substring(intRngSeparator + 1);

                        string strFrmRef = strSheetRefFrm.Substring(strSheetRefFrm.IndexOf('!') + 1);
                        strSheetRefFrm = strSheetRefFrm.Substring(0, strSheetRefFrm.IndexOf('!'));
                        strSheetRefFrm = strSheetRefFrm.Replace("$", "");
                        string strToRef = strSheetRefTo.Substring(strSheetRefTo.IndexOf('!') + 1);
                        strSheetRefTo = strSheetRefTo.Substring(0, strSheetRefTo.IndexOf('!'));
                        strSheetRefTo = strSheetRefTo.Replace("$", "");

                        if (strSheetRefTo.StartsWith("'"))
                        {
                            strSheetRefTo = strSheetRefTo.Remove(0, 1);
                        }
                        if (strSheetRefTo.EndsWith("'"))
                        {
                            strSheetRefTo = strSheetRefTo.Remove(strSheetRefTo.Length - 1, 1);
                        }
                        if (strSheetRefFrm.StartsWith("'"))
                        {
                            strSheetRefFrm = strSheetRefFrm.Remove(0, 1);
                        }
                        if (strSheetRefFrm.EndsWith("'"))
                        {
                            strSheetRefFrm = strSheetRefFrm.Remove(strSheetRefFrm.Length - 1, 1);
                        }
                        if (strFrmRef == strToRef)
                        {
                            strOdfCellRef = "'" + strSheetRefFrm + ":" + strSheetRefTo + "'!" + strFrmRef;
                        }
                        else
                        {
                            strOdfCellRef = "'" + strSheetRefFrm + ":" + strSheetRefTo + "'!" + strFrmRef + ":" + strToRef;
                        }
                    }
                }

                if (blnNotConverted)
                {
                    if (strOdfCellRef.Substring(0, strOdfCellRef.IndexOf('!')).StartsWith("$"))
                    {
                        strOdfCellRef = strOdfCellRef.Replace("$", "");
                    }
                    if (!strOdfCellRef.Substring(0, strOdfCellRef.IndexOf('!')).StartsWith("'"))
                    {
                        strOdfCellRef = "'" + strOdfCellRef.Replace("!", "'!");
                    }
                }

                return strOdfCellRef;
            }
        }

        protected string TransFormulaParms(string strFormula)
        {
            string strTransFormula = "";
            string strFormulaToTrans = "";


            //chking for key word
            if (strFormula.Contains("ADDRESS(") || strFormula.Contains("CEILING(") || strFormula.Contains("FLOOR(") || strFormula.Contains("ROUND(") || strFormula.Contains("ROUNDDOWN(") || strFormula.Contains("ROUNDUP(") || strFormula.Contains("IF(") || strFormula.ToUpperInvariant().Contains("ACCRINTM(") || strFormula.Contains("INDEX("))
            {
                while (strFormula != "")
                {
                    string strExpression = "";

                    //TODO : Include other formulas. need to chk which formula is comming first.
                    if (strFormula.Contains("ADDRESS(") || strFormula.Contains("CEILING(") || strFormula.Contains("FLOOR(") || strFormula.Contains("ROUND(") || strFormula.Contains("ROUNDDOWN(") || strFormula.Contains("ROUNDUP(") || strFormula.Contains("IF(") || strFormula.ToUpperInvariant().Contains("ACCRINTM(") || strFormula.Contains("INDEX("))
                    {
                        ArrayList arrFunctionPos = new ArrayList();

                        string strFunction = "";
                        int intFunction = 0;
                        int intIndex = 0;
                        bool blnFunNotIf = false;

                        arrFunctionPos.Add(new Item(strFormula.IndexOf("ADDRESS("), "ADDRESS(", 1));//add 4th parm 1
                        arrFunctionPos.Add(new Item(strFormula.IndexOf("CEILING("), "CEILING(", 2));//remove 3rd parm
                        arrFunctionPos.Add(new Item(strFormula.IndexOf("FLOOR("), "FLOOR(", 2));//remove 3rd parm
                        arrFunctionPos.Add(new Item(strFormula.IndexOf("ROUND("), "ROUND(", 3));//add 2nd parm 0
                        arrFunctionPos.Add(new Item(strFormula.IndexOf("ROUNDDOWN("), "ROUNDDOWN(", 3));//add 2nd parm 0
                        arrFunctionPos.Add(new Item(strFormula.IndexOf("ROUNDUP("), "ROUNDUP(", 3));//add 2nd parm 0
                        arrFunctionPos.Add(new Item(strFormula.IndexOf("IF("), "IF(", 4));//add 2nd parm TRUE
                        arrFunctionPos.Add(new Item(strFormula.ToUpperInvariant().IndexOf("ACCRINTM("), "ACCRINTM(", 5));//add 4th parm 1000
                        arrFunctionPos.Add(new Item(strFormula.IndexOf("INDEX("), "INDEX(", 3));//add 2nd parm 0

                        arrFunctionPos.Sort();

                        //int[] arrIndex = new int[3];

                        foreach (object obj in arrFunctionPos)
                        {
                            Item objItem = (Item)obj;
                            if (objItem.Possition >= 0)
                            {
                                strFunction = objItem.Function;
                                intFunction = objItem.FunctionType;
                                intIndex = objItem.Possition;
                                break;
                            }
                        }
                        if (intFunction == 4)
                        {
                            if (strFormula.Substring(0, intIndex).EndsWith("COUNT") || strFormula.Substring(0, intIndex).EndsWith("SUM"))
                            {
                                blnFunNotIf = true;
                            }
                        }

                        string strConvertedExp = "";
                        strTransFormula = strTransFormula + strFormula.Substring(0, intIndex);
                        strFormulaToTrans = strFormula.Substring(intIndex);
                        if (blnFunNotIf)
                        {
                            strTransFormula = strTransFormula + strFormulaToTrans.Substring(0, strFunction.Length);
                            strFormula = strFormulaToTrans.Substring(strFunction.Length);
                        }
                        else
                        {

                            if (strFormulaToTrans.Contains(")"))
                            {
                                strExpression = GetExpression(strFormulaToTrans);
                            }
                            if (strExpression != "")
                            {
                                if (intFunction == 1)
                                {
                                    strConvertedExp = AddRemoveParm(strExpression, 4, 5, true, false, "1");
                                }
                                else if (intFunction == 2)
                                {
                                    strConvertedExp = AddRemoveParm(strExpression, 3, 4, false, true, "");
                                }
                                else if (intFunction == 3)
                                {
                                    strConvertedExp = AddRemoveParm(strExpression, 2, 0, true, false, "0");
                                }
                                else if (intFunction == 4)
                                {
                                    strConvertedExp = AddRemoveParm(strExpression, 2, 0, true, false, "TRUE");
                                }
                                else if (intFunction == 5)
                                {
                                    strConvertedExp = AddRemoveParm(strExpression, 4, 0, true, false, "1000");
                                }
                                //use converted and send remaining for chk
                                strTransFormula = strTransFormula + strConvertedExp;
                                strFormula = strFormulaToTrans.Remove(0, strExpression.Length);
                            }
                            else
                            {
                                //remove the key word and send remaing for chk
                                strTransFormula = strTransFormula + strFormulaToTrans.Substring(0, strFunction.Length);
                                strFormula = strFormulaToTrans.Substring(strFunction.Length);
                            }
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

        protected string AddRemoveParm(string strExpresion, int intPossition, int intMaxParm, bool blnParmAdd, bool blnParmRemove, string strParmAdd)
        {
            string strTransFormula = "";
            string strFormulaKeyword = strExpresion.Substring(0, strExpresion.IndexOf('('));
            strExpresion = strExpresion.Substring(strExpresion.IndexOf('(') + 1);
            strExpresion = strExpresion.Substring(0, strExpresion.Length - 1);

            ArrayList arlParms = GetParms(strExpresion);

            //if (intMaxParm > arlParms.Count)
            //{
            if (blnParmAdd == true && blnParmRemove == false)
            {
                strTransFormula = strFormulaKeyword + "(";

                for (int i = 0; i < arlParms.Count; i++)
                {
                    if (intPossition == i + 1 && intMaxParm > arlParms.Count)
                    {
                        strTransFormula = strTransFormula + strParmAdd + "," + arlParms[i].ToString() + ",";
                    }
                    else
                    {
                        strTransFormula = strTransFormula + arlParms[i].ToString() + ",";
                    }
                }
                if (intPossition == arlParms.Count + 1 && intMaxParm == 0)
                {
                    strTransFormula = strTransFormula + strParmAdd + ",";
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
                        strTransFormula = strTransFormula + arlParms[i].ToString() + ",";
                    }
                }
                strTransFormula = strTransFormula.Substring(0, strTransFormula.Length - 1) + ")";
            }
            //}
            //else
            //{
            //    strTransFormula = strFormulaKeyword + "(" + strExpresion + ")";
            //}
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

            string[] strArrParm = strExpression.Split(',');
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
                        strExpToVald = strExpToVald + "," + strArrParm[i].ToString();
                    }
                    if (strExpToVald.StartsWith("\"") && strExpToVald.EndsWith("\""))
                    {
                        if (IsValidString(strExpToVald) && IsValidExpression(strExpToVald))
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
        //End of RefNo-1

        protected string GetColumnWidth(string text)
        {
            // default width in ODS for Arial 10 is 0.8925in or  80.925px            

            string fontName = text.Split('|')[0].ToString(System.Globalization.CultureInfo.InvariantCulture);
            string fontSize = text.Split('|')[1].ToString(System.Globalization.CultureInfo.InvariantCulture);
            char[] letters = new char[] { 'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E', 'e', 'F', 'f', 'G', 'g', 'H', 'h', 'I', 'i', 'J', 'j', 'K', 'k', 'L', 'l', 'M', 'm', 'N', 'n', 'O', 'P', 'p', 'Q', 'q', 'R', 'r', 'S', 's', 'T', 't', 'U', 'u', 'V', 'v', 'W', 'w', 'X', 'x', 'Y', 'y', 'Z', 'z' };
            double dblFontSizePt = Convert.ToDouble(fontSize.TrimEnd(letters), System.Globalization.CultureInfo.InvariantCulture);
            double dblPixelWidth = 80.925;
            double dblMaxDigitWidth = MaxDigitWidth(fontName, dblFontSizePt);
            double dblCharWidth = Math.Floor((dblPixelWidth - 5) / dblMaxDigitWidth * 100 + 0.5) / 100.0;
            // formula from OpenXml spec §3.3.1.12:
            //
            // To translate from pixels to character width, use this calculation:
            // =Truncate(({pixels}-5)/{Maximum Digit Width} * 100+0.5)/100

            //double dblCharWidth = ((dblPixelWidth - 5) / dblMaxDigitWidth * 100 + 0.5) / 100.0;

            // formula from OpenXml spec §3.3.1.12:
            //
            // width = Truncate([{Number of Characters} * {Maximum Digit Width} + {5 pixel 
            //                       padding}]/{Maximum Digit Width}*256)/256

            double columnWidth = Math.Floor((dblCharWidth * dblMaxDigitWidth + 5) / dblMaxDigitWidth * 256) / 256.0;
            return columnWidth.ToString(System.Globalization.CultureInfo.InvariantCulture);
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
            //Graphics g = System.Drawing.Graphics.FromHwnd(new IntPtr());
            //float fSizePxl = (float)Math.Round(g.DpiX * dblSizePt / 72);           
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
    }
}
