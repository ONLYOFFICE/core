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

using System.Xml;
using System.Collections;
using System;
using System.IO;
//added by sonata for mulltilevel grouping
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Text;
using System.Text.RegularExpressions;
using OdfConverter.Transforms;
using OdfConverter.Transforms.Test;
using System.Globalization;
namespace CleverAge.OdfConverter.OdfConverterLib
{

    /// <summary>
    /// An <c>XmlWriter</c> implementation for characters post processings
    public class OdfCharactersPostProcessor : AbstractPostProcessor
    {

        private const string TEXT_NAMESPACE = "urn:oasis:names:tc:opendocument:xmlns:text:1.0";
        private const string PCHAR_NAMESPACE = "urn:cleverage:xmlns:post-processings:characters";

        private Stack currentNode;
        private Stack store;

        public OdfCharactersPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.currentNode = new Stack();
            this.store = new Stack();
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            Element e = null;
            if (TEXT_NAMESPACE.Equals(ns) && "span".Equals(localName))
            {
                e = new Span(prefix, localName, ns);
            }
            else
            {
                e = new Element(prefix, localName, ns);
            }

            this.currentNode.Push(e);

            if (InSpan())
            {
                StartStoreElement();
            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
        }


        public override void WriteEndElement()
        {

            if (IsSpan())
            {
                WriteStoredSpan();
            }
            if (InSpan())
            {
                EndStoreElement();
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }
            this.currentNode.Pop();
        }

        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            this.currentNode.Push(new Attribute(prefix, localName, ns));

            if (InSpan())
            {
                StartStoreAttribute();
            }
            else
            {
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }
        }


        public override void WriteEndAttribute()
        {
            if (InSpan())
            {
                EndStoreAttribute();
            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }
            this.currentNode.Pop();
        }


        public override void WriteString(string text)
        {

            //for getting system date  (text ==":::current:::")
            if (InSpan() && (!(text == ":::current:::")))
            {
                StoreString(text);
            }
            else if (text.Contains("svg-x") || text.Contains("svg-y"))
            {

                this.nextWriter.WriteString(EvalExpression(text));

            }

            else if (text == ":::current:::")
            {

                this.nextWriter.WriteString(EvalgetCurrentSysDate(text));

            }
            // added by vipul for Shape Rotation
            //Start
            else if (text.Contains("draw-transform"))
            {

                this.nextWriter.WriteString(EvalRotationExpression(text));

            }
            //sonata: Word free from shapes
            else if (text.Contains("WordshapesEnhance-Path"))
            {

                this.nextWriter.WriteString(EvalWordEnhacePath(text));

            }
            else if (text.Contains("Wordshapes-draw-modifier"))
            {

                this.nextWriter.WriteString(EvalWordModifier(text));

            }
            else if (text.Contains("WordshapesFormula"))
            {

                this.nextWriter.WriteString(EvalWordShapesFormula(text));

            }
            else if (text.Contains("TableCord"))
            {

                this.nextWriter.WriteString(EvalTableCord(text));

            }
            else if (text.Contains("Group-Transform"))
            {

                this.nextWriter.WriteString(EvalGroupingExpression(text));

            }
            //End
            else if (text.Contains("shade-tint"))
            {

                this.nextWriter.WriteString(EvalShadeExpression(text));

            }
            //Shadow calculation
            else if (text.Contains("shadow-offset-x") || text.Contains("shadow-offset-y"))
            {

                this.nextWriter.WriteString(EvalShadowExpression(text));
            }
            else if (text.Contains("transFileName:"))
            {

                this.nextWriter.WriteString(EvaltransFileName(text));
            }

            //Image Cropping Calculation Added by Sonata-15/11/2007
            else if (text.Contains("image-props"))
            {
                EvalImageCrop(text);
            }
            //Callout Adjustments Calculation Added by Sonata
            else if (text.Contains("Callout-Adj"))
            {
                this.nextWriter.WriteString(EvalCalloutAdjustment(text));
            }
            else if (IsDate())
            {
                try
                {
                    DateTime dateTime = DateTime.Parse(text, CultureInfo.InvariantCulture);
                    this.nextWriter.WriteString(dateTime.ToLocalTime().ToString("s", CultureInfo.InvariantCulture));
                }
                catch (FormatException)
                {
                }
            }
            else
            {
                //2008-10-28/divo: Fix no longer needed, spaces are correctly translated in XSL
                //          The bug fix below was not nice because <text:s> nodes were created everywhere, 
                //          not only for text in the content, header/footer etc of the document
                //
                //added by clam for bug 1785583 (a leading blank was lost)
                //Start
                //if (text.StartsWith(" ") && text.Trim().Length > 0)
                //{
                //    try
                //    {
                //        this.nextWriter.WriteStartElement("text", "s", ((CleverAge.OdfConverter.OdfConverterLib.AbstractPostProcessor.Element)this.currentNode.Peek()).Ns);
                //        this.nextWriter.WriteEndElement();
                //        text = text.Substring(1);
                //    }
                //    catch (Exception)
                //    {
                //    }
                //}
                //End

                this.nextWriter.WriteString(text);
            }
        }
        /*
         * General methods */
        //sonata: Word free from shapes

        private string EvalWordModifier(string text)
        {
            string[] arrPart = text.Split(':');
            string strModf = string.Empty;
            if (arrPart.Length == 2)
            {
                strModf = arrPart[1];
                strModf = strModf.Replace(",,,,,", ",0,0,0,0,");
                strModf = strModf.Replace(",,,,", ",0,0,0,");
                strModf = strModf.Replace(",,,", ",0,0,");
                strModf = strModf.Replace(",,", ",0,");
                strModf = strModf.Replace(",", " ");

            }


            return strModf;
        }
        private string EvalWordEnhacePath(string text)
        {
            string[] arrPart = text.Split(':');
            string strPath = string.Empty;
            if (arrPart.Length == 2)
            {
                strPath = arrPart[1];
                strPath = strPath.Replace(",,,,,", ",0,0,0,0,");
                strPath = strPath.Replace(",,,,", ",0,0,0,");
                strPath = strPath.Replace(",,,", ",0,0,");
                strPath = strPath.Replace(",,", ",0,");
                strPath = strPath.Replace("h", "");
                strPath = strPath.Replace("d", "");
                strPath = strPath.Replace("at", "A");
                strPath = strPath.Replace("ar", "B");
                strPath = strPath.Replace("v", "E");
                strPath = strPath.Replace("c", "C");
                strPath = strPath.Replace("nf", "F");
                strPath = strPath.Replace("al", "U");
                strPath = strPath.Replace("wr", "V");
                strPath = strPath.Replace("wa", "W");
                strPath = strPath.Replace("ae", "T");
                strPath = strPath.Replace("qx", "X");
                strPath = strPath.Replace("qy", "Y");
                strPath = strPath.Replace("l", "L");
                strPath = strPath.Replace("x", "Z");
                strPath = strPath.Replace("m", "M");
                strPath = strPath.Replace("ns", "S");
                strPath = strPath.Replace("e", "N");
                strPath = strPath.Replace("r", "R");
                strPath = strPath.Replace("t", "P");



                strPath = Translate_CommandComma(strPath);
                strPath = Translate_CommaCommand(strPath);
                strPath = regexpres(strPath);



                strPath = strPath.Replace("@", " ?f");
                strPath = strPath.Replace("N", "N ");
                strPath = strPath.Replace("A", "A ");
                strPath = strPath.Replace("B", "B ");
                strPath = strPath.Replace("X", "X ");
                strPath = strPath.Replace("Y", "Y ");
                strPath = strPath.Replace("M", "M ");
                strPath = strPath.Replace("L", "L ");
                strPath = strPath.Replace("T", "T ");
                strPath = strPath.Replace("U", "U ");
                strPath = strPath.Replace("F", "F ");
                strPath = strPath.Replace("S", "S ");
                strPath = strPath.Replace("C", "C ");
                strPath = strPath.Replace("Z", "Z ");
                strPath = strPath.Replace("V", "V ");
                strPath = strPath.Replace("W", "W ");
                strPath = strPath.Replace("R", "R ");
                strPath = strPath.Replace("E", "E ");
                strPath = strPath.Replace("P", "P ");
                strPath = strPath.Replace(",", " ");
                strPath = TranslateRCommand(strPath);
                strPath = TranslateVCommand(strPath);


            }

            if (strPath.Trim() == "M 10800 0 A 0 0 21600 21600 10800 0 10800 0 Z N")
            {
                strPath = "M 10800 0 L 10799 0 C 4835 0 0 4835 0 10799 0 16764 4835 21600 10800 21600 16764 21600 21600 16764 21600 10800 21600 4835 16764 0 10800 0 Z N";
            }

            return strPath.Trim();
        }
        private string TranslateRCommand(string strpath)
        {

            return Regex.Replace(strpath, "[-0-9]+[ ]+[-0-9]+[ ]+[RP]+[ ]+[-0-9]+[ ]+[-0-9]+[ ]", delegate(Match match)
            {
                string strMatch = match.ToString();
                string[] arrRValues = strMatch.Split(' ');
                long dblX = long.Parse(arrRValues[0], System.Globalization.CultureInfo.InvariantCulture) + long.Parse(arrRValues[3], System.Globalization.CultureInfo.InvariantCulture);
                long dblY = long.Parse(arrRValues[1], System.Globalization.CultureInfo.InvariantCulture) + long.Parse(arrRValues[4], System.Globalization.CultureInfo.InvariantCulture);
                if (strMatch.Contains("R"))
                {
                    strMatch = arrRValues[0] + " " + arrRValues[1] + " L " + dblX.ToString(System.Globalization.CultureInfo.InvariantCulture) + " " + dblY.ToString(System.Globalization.CultureInfo.InvariantCulture) + " ";
                }
                else if (strMatch.Contains("P"))
                {
                    strMatch = arrRValues[0] + " " + arrRValues[1] + " M" + dblX.ToString(System.Globalization.CultureInfo.InvariantCulture) + " " + dblY.ToString(System.Globalization.CultureInfo.InvariantCulture) + " ";
                }

                return strMatch;
            });
        }
        private string TranslateVCommand(string strpath)
        {

            return Regex.Replace(strpath, "[-0-9]+[ ]+[-0-9]+[ ]+[E]+[ ]+[-0-9]+[ ]+[-0-9]+[ ]+[-0-9]+[ ]+[-0-9]+[ ]+[-0-9]+[ ]+[-0-9]+[ ]", delegate(Match match)
            {
                string strMatch = match.ToString();
                string[] arrVValues = strMatch.Split(' ');
                long dblX0 = Convert.ToInt64(arrVValues[0], System.Globalization.CultureInfo.InvariantCulture);
                long dblY0 = Convert.ToInt64(arrVValues[1], System.Globalization.CultureInfo.InvariantCulture);
                long dblX1 = Convert.ToInt64(arrVValues[3], System.Globalization.CultureInfo.InvariantCulture);
                long dblY1 = Convert.ToInt64(arrVValues[4], System.Globalization.CultureInfo.InvariantCulture);
                long dblX2 = Convert.ToInt64(arrVValues[5], System.Globalization.CultureInfo.InvariantCulture);
                long dblY2 = Convert.ToInt64(arrVValues[6], System.Globalization.CultureInfo.InvariantCulture);
                long dblX3 = Convert.ToInt64(arrVValues[7], System.Globalization.CultureInfo.InvariantCulture);
                long dblY3 = Convert.ToInt64(arrVValues[8], System.Globalization.CultureInfo.InvariantCulture);

                strMatch = arrVValues[0] + " " + arrVValues[1] + " C " +
                                 (dblX0 + dblX1).ToString(System.Globalization.CultureInfo.InvariantCulture) + " " +
                                (dblY0 + dblY1).ToString(System.Globalization.CultureInfo.InvariantCulture) + " " +
                                (dblX0 + dblX2).ToString(System.Globalization.CultureInfo.InvariantCulture) + " " +
                                (dblY0 + dblY2).ToString(System.Globalization.CultureInfo.InvariantCulture) + " " +
                                (dblX0 + dblX3).ToString(System.Globalization.CultureInfo.InvariantCulture) + " " +
                                (dblY0 + dblY3).ToString(System.Globalization.CultureInfo.InvariantCulture) + " ";


                return strMatch;
            });
        }
        private string Translate_CommandComma(string strpath)
        {
            char[] arrCommands = { 'A', 'B', 'C', 'F', 'L', 'M', 'N', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'R', 'E', 'P' };
            return Regex.Replace(strpath, "[ABCFLMNSTUVWXYZREP]+[,]", delegate(Match match)
            {
                string strMatch = match.ToString();
                strMatch = strMatch.Substring(0, strMatch.IndexOf(',')) + "0,";
                return strMatch;
            });
        }
        private string Translate_CommaCommand(string strpath)
        {
            char[] arrCommands = { 'A', 'B', 'C', 'F', 'L', 'M', 'N', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'R', 'E', 'P' };
            return Regex.Replace(strpath, "[,]+[ABCFLMNSTUVWXYZREP]", delegate(Match match)
            {
                string strMatch = match.ToString();
                strMatch = ",0 " + strMatch.Substring(strMatch.IndexOfAny(arrCommands));
                return strMatch;
            });
        }
        private string regexpres(string strpath)
        {
            char[] arrCommands = { 'A', 'B', 'C', 'F', 'L', 'M', 'N', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'R', 'E', 'P' };
            return Regex.Replace(strpath, "[-0-9]+[ABCFLMNSTUVWXYZREP]", delegate(Match match)
            {
                string strMatch = match.ToString();
                strMatch = strMatch.Substring(0, strMatch.IndexOfAny(arrCommands)) + " " + strMatch.Substring(strMatch.IndexOfAny(arrCommands));
                return strMatch;
            });
        }
        private string EvalWordShapesFormula(string text)
        {

            string[] arrPart = text.Split(':');
            string strFormula = string.Empty;
            string[] strShapeSize = arrPart[2].Split(',');
            string width = strShapeSize[0];
            string height = strShapeSize[1];
            string[] arrFormulaVals = arrPart[1].Split(' ');

            if (arrPart.Length == 3)
            {
                if (arrFormulaVals[0] == "sum")
                {
                    strFormula = arrFormulaVals[1] + " + " + arrFormulaVals[2] + " - " + arrFormulaVals[3];
                }
                else if (arrFormulaVals[0] == "if")
                {
                    strFormula = "if(" + arrFormulaVals[1] + ", " + arrFormulaVals[2] + " , " + arrFormulaVals[3] + ")";
                }
                else if (arrFormulaVals[0] == "prod")
                {
                    strFormula = arrFormulaVals[1] + " * " + arrFormulaVals[2] + " / " + arrFormulaVals[3];
                }
                else if (arrFormulaVals[0] == "abs")
                {
                    strFormula = "abs(" + arrFormulaVals[1] + ")";
                }
                else if (arrFormulaVals[0] == "val")
                {
                    strFormula = arrFormulaVals[1];
                }
                else if (arrFormulaVals[0] == "mid")
                {

                    strFormula = "(" + arrFormulaVals[1] + " + " + arrFormulaVals[2] + ") / " + 2;
                }
                else if (arrFormulaVals[0] == "sqrt")
                {

                    strFormula = "sqrt(abs(" + arrFormulaVals[1] + "))";
                }
                else if (arrFormulaVals[0] == "max")
                {

                    strFormula = "max(" + arrFormulaVals[1] + ", " + arrFormulaVals[2] + ")";
                }
                else if (arrFormulaVals[0] == "min")
                {

                    strFormula = "min(" + arrFormulaVals[1] + ", " + arrFormulaVals[2] + ")";
                }
                else if (arrFormulaVals[0] == "cos")
                {

                    strFormula = arrFormulaVals[1] + " * " + "cos(" + arrFormulaVals[2] + ")";
                }

                else if (arrFormulaVals[0] == "sin")
                {

                    strFormula = arrFormulaVals[1] + " * " + "sin(" + arrFormulaVals[2] + ")";
                }

                else if (arrFormulaVals[0] == "tan")
                {

                    strFormula = arrFormulaVals[1] + " * " + "tan(" + arrFormulaVals[2] + ")";
                }
                else if (arrFormulaVals[0] == "mod")
                {

                    strFormula = "sqrt(abs((" + arrFormulaVals[1] + " *" + arrFormulaVals[1] + ")+(" +
                                  arrFormulaVals[2] + " *" + arrFormulaVals[2] + ")+(" +
                                  arrFormulaVals[3] + " *" + arrFormulaVals[3] + ")))";
                }
                else if (arrFormulaVals[0] == "sumangle")
                {

                    strFormula = arrFormulaVals[1] + " + " + arrFormulaVals[2] + " * 65536 - " +
                                  arrFormulaVals[3] + " *65536";
                }

                else if (arrFormulaVals[0] == "sinatan2")
                {
                    strFormula = arrFormulaVals[1] + " * sin(atan2(" + arrFormulaVals[2] + "," + arrFormulaVals[3] + "))";
                }
                else if (arrFormulaVals[0] == "cosatan2")
                {
                    strFormula = arrFormulaVals[1] + " * cos(atan2(" + arrFormulaVals[2] + "," + arrFormulaVals[3] + "))";
                }

                else if (arrFormulaVals[0] == "ellipse")
                {

                    strFormula = arrFormulaVals[3] + " *sqrt( abs(1-((" +
                                  arrFormulaVals[1] + " *" + arrFormulaVals[1] + ")/(" +
                                   arrFormulaVals[2] + " *" + arrFormulaVals[2] + "))))";
                }

                else if (arrFormulaVals[0] == "atan2")
                {
                    strFormula = "atan2(" + arrFormulaVals[2] + "," + arrFormulaVals[1] + ")";
                }
            }
            strFormula = strFormula.Replace("#", "$");
            //1 px   = 0.0264cm at 96dpi (Windows default) 
            strFormula = strFormula.Replace("pixelwidth", "width * 0.0264");
            strFormula = strFormula.Replace("pixellinewidth", "width * 0.0264");
            strFormula = strFormula.Replace("pixelheight", "height * 0.0264");
            strFormula = strFormula.Replace("pixellineheight", "height * 0.0264");

            strFormula = strFormula.Replace("pixelWidth", "width * 0.0264");
            strFormula = strFormula.Replace("pixellineWidth", "width * 0.0264");
            strFormula = strFormula.Replace("pixelHeight", "height * 0.0264");
            strFormula = strFormula.Replace("pixellineHeight", "height * 0.0264");

            strFormula = strFormula.Replace("pixelLineWidth", "width * 0.0264");
            strFormula = strFormula.Replace("pixelLineHeight", "height * 0.0264");
            strFormula = strFormula.Replace("linedrawn", "1");
            strFormula = strFormula.Replace("lineDrawn", "1");

            strFormula = strFormula.Replace("emuwidth", "width");
            strFormula = strFormula.Replace("emuheight", "height");
            strFormula = strFormula.Replace("emuwidth2", "width");
            strFormula = strFormula.Replace("emuheight2", "height");

            strFormula = strFormula.Replace("@", "?f");

            return strFormula;
        }
        private string EvalTableCord(string text)
        {
            string[] arrPart = text.Split('@');
            string[] arrVal = arrPart[0].Split(':');
            string[] arrVal1 = arrPart[1].Split(':');
            double dblTempCell = 0.0;
            double dblXY = 0.0;
            double dblRetXY = 0.0;
            double dblSUBXY = 0.0;
            dblXY = Double.Parse(arrVal1[1], System.Globalization.CultureInfo.InvariantCulture);

            if (arrVal1[0] == "1")
            {

                dblRetXY = dblXY;
            }

            else
            {
                for (int intCount = 1; intCount < arrVal.Length - 2; intCount++)
                {
                    dblTempCell = Double.Parse(arrVal[intCount], System.Globalization.CultureInfo.InvariantCulture);
                    dblSUBXY += dblTempCell;

                }
                dblRetXY = dblXY + dblSUBXY;


            }

            string str = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.#####}", dblRetXY / 360000);
            return str + "cm";
        }

        //To create the folder for the transition sound
        private string EvaltransFileName(string text)
        {
            string[] arrVal = text.Split(':');

            string returnInputFilePath = "";

            // To get the input file name to be attached to the folder for transition sound.
            // for Commandline tool
            //for (int i = 0; i < Environment.GetCommandLineArgs().Length; i++)
            //{
            //    if (Environment.GetCommandLineArgs()[i].ToString().ToUpper() == "/I")
            //        returnInputFilePath = Path.GetFileNameWithoutExtension(Environment.GetCommandLineArgs()[i + 1]);
            //}

            // To get the temporary file name to be attached to the folder for transition sound.
            //for addins
            if (returnInputFilePath == "")
            {
                returnInputFilePath = System.IO.Path.GetFileNameWithoutExtension(OdfConverter.OdfConverterLib.AbstractConverter.inputTempFileName);
                returnInputFilePath = arrVal[1] + returnInputFilePath + arrVal[2];
            }

            return returnInputFilePath;

        }

        private string EvalgetCurrentSysDate(string text)
        {
            string str = "";
            str = DateTime.Now.ToShortDateString();
            return str;
        }

        private string EvalShadeExpression(string text)
        {
            string[] arrVal = new string[5];
            arrVal = text.Split(':');

            double dblRed = Double.Parse(arrVal[1], System.Globalization.CultureInfo.InvariantCulture);
            double dblGreen = Double.Parse(arrVal[2], System.Globalization.CultureInfo.InvariantCulture);
            double dblBlue = Double.Parse(arrVal[3], System.Globalization.CultureInfo.InvariantCulture);
            double dblShade = Double.Parse(arrVal[4], System.Globalization.CultureInfo.InvariantCulture);

            double sR;
            if (dblRed < 10)
            {
                sR = 2.4865 * dblRed;
            }
            else
            {
                sR = (Math.Pow(((dblRed + 14.025) / 269.025), 2.4)) * 8192;
            }
            double sG;
            if (dblGreen < 10)
            {
                sG = 2.4865 * dblGreen;

            }
            else
            {

                sG = (Math.Pow(((dblGreen + 14.025) / 269.025), 2.4)) * 8192;

            }

            double sB;
            if (dblBlue < 10)
            {
                sB = 2.4865 * dblBlue;
            }
            else
            {
                sB = (Math.Pow(((dblBlue + 14.025) / 269.025), 2.4)) * 8192;
            }

            double NewRed;
            double sRead;
            sRead = (sR * dblShade / 100);

            if (sRead < 10)
            {
                NewRed = 0;

            }
            else if (sRead < 24)
            {
                NewRed = (12.92 * 255 * sR / 8192);

            }
            else if (sRead <= 8192)
            {
                NewRed = ((Math.Pow(sRead / 8192, 1 / 2.4) * 1.055) - 0.055) * 255;
            }
            else
            {
                NewRed = 255;
            }
            NewRed = Math.Round(NewRed);

            double NewGreen;
            double sGreen;
            sGreen = (sG * dblShade / 100);

            if (sGreen < 0)
            {
                NewGreen = 0;

            }
            else if (sGreen < 24)
            {
                NewGreen = (12.92 * 255 * dblGreen / 8192);

            }
            else if (sGreen < 8193)
            {
                NewGreen = ((Math.Pow(sGreen / 8192, 1 / 2.4) * 1.055) - 0.055) * 255;
            }
            else
            {
                NewGreen = 255;
            }
            NewGreen = Math.Round(NewGreen);

            double NewBlue;
            double sBlue;
            sBlue = (sB * dblShade / 100);

            if (sBlue < 0)
            {
                NewBlue = 0;

            }
            else if (sBlue < 24)
            {
                NewBlue = (12.92 * 255 * dblBlue / 8192);

            }
            else if (sBlue < 8193)
            {
                NewBlue = ((Math.Pow(sBlue / 8192, 1 / 2.4) * 1.055) - 0.055) * 255; ;
            }
            else
            {
                NewBlue = 255;
            }
            NewBlue = Math.Round(NewBlue);

            int intRed;
            int intGreen;
            int intBlue;
            intRed = (int)NewRed;
            intGreen = (int)NewGreen;
            intBlue = (int)NewBlue;

            string hexRed;
            string hexGreen;
            string hexBlue;

            hexRed = String.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:x}", intRed);
            hexGreen = String.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:x}", intGreen);
            hexBlue = String.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:x}", intBlue);


            //modified by chhavi:for ODF1.1 conformance
            return ('#' + hexRed.ToUpper().ToString().PadLeft(2, '0') + hexGreen.ToUpper().ToString().PadLeft(2, '0') + hexBlue.ToUpper().ToString().PadLeft(2, '0'));

        }

        //Added by sonata\Vipul for multilevel grouping
        private string EvalGroupingExpression(string text)
        {

            List<OoxShape> _shapes = new List<OoxShape>();

            string strRet = "";
            string strLineRet = "";
            string strShapeCordinates = "";
            string[] arrgroupShape = text.Split('$');
            string[] arrgroup = arrgroupShape[0].Split('@');

            //group cordinates
            long dblgrpX;
            long dblgrpY;
            long dblgrpCX;
            long dblgrpCY;
            long dblgrpChX;
            long dblgrpChY;
            long dblgrpChCX;
            long dblgrpChCY;
            long dblgrpRot;
            long dblgrpFlipH;
            long dblgrpFlipV;

            //shape cordinates
            long dblShapeX;
            long dblShapeY;
            long dblShapeCX;
            long dblShapeCY;
            long dblShapeRot;
            long dblShapeFlipH;
            long dblShapeFlipV;

            OoxTransform TopLevelgroup;
            OoxTransform Targetgroup = new OoxTransform(5495925, 3286125, 1419225, 657225, 0, 1, 1);
            OoxTransform shapeCord;
            OoxTransform InnerLevelgroup;
            OoxTransform Tempgroup;

            string[] arrShapeCordinates;
            string[] arrInnerGroup;
            string[] arrFinalRet;

            if (arrgroup.Length >= 4)
            {
                //Multilevel group

                //top level Group cordinates
                arrInnerGroup = arrgroup[1].Split(':');

                dblgrpX = long.Parse(arrInnerGroup[0], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpY = long.Parse(arrInnerGroup[1], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpCX = long.Parse(arrInnerGroup[2], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpCY = long.Parse(arrInnerGroup[3], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpChX = long.Parse(arrInnerGroup[4], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpChY = long.Parse(arrInnerGroup[5], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpChCX = long.Parse(arrInnerGroup[6], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpChCY = long.Parse(arrInnerGroup[7], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpRot = long.Parse(arrInnerGroup[8], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpFlipH = long.Parse(arrInnerGroup[9], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpFlipV = long.Parse(arrInnerGroup[10], System.Globalization.CultureInfo.InvariantCulture);

                TopLevelgroup = new OoxTransform(dblgrpChX, dblgrpChY, dblgrpChCX, dblgrpChCY, dblgrpX, dblgrpY, dblgrpCX, dblgrpCY, dblgrpRot, 1, 1);

                for (int intCount = 2; intCount < arrgroup.Length - 1; intCount++)
                {
                    arrInnerGroup = arrgroup[intCount].Split(':');

                    Tempgroup = TopLevelgroup;

                    //Inner level Group cordinates
                    dblgrpX = long.Parse(arrInnerGroup[0], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpY = long.Parse(arrInnerGroup[1], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpCX = long.Parse(arrInnerGroup[2], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpCY = long.Parse(arrInnerGroup[3], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpChX = long.Parse(arrInnerGroup[4], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpChY = long.Parse(arrInnerGroup[5], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpChCX = long.Parse(arrInnerGroup[6], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpChCY = long.Parse(arrInnerGroup[7], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpRot = long.Parse(arrInnerGroup[8], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpFlipH = long.Parse(arrInnerGroup[9], System.Globalization.CultureInfo.InvariantCulture);
                    dblgrpFlipV = long.Parse(arrInnerGroup[10], System.Globalization.CultureInfo.InvariantCulture);
                    InnerLevelgroup = new OoxTransform(dblgrpChX, dblgrpChY, dblgrpChCX, dblgrpChCY, dblgrpX, dblgrpY, dblgrpCX, dblgrpCY, dblgrpRot, 1, 1);

                    Targetgroup = new OoxTransform(Tempgroup, InnerLevelgroup);

                    TopLevelgroup = Targetgroup;

                }

                arrShapeCordinates = arrgroupShape[1].Split(':');

                //shape cordinates
                dblShapeX = long.Parse(arrShapeCordinates[1], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeY = long.Parse(arrShapeCordinates[2], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeCX = long.Parse(arrShapeCordinates[3], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeCY = long.Parse(arrShapeCordinates[4], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeRot = long.Parse(arrShapeCordinates[5], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeFlipH = long.Parse(arrShapeCordinates[6], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeFlipV = long.Parse(arrShapeCordinates[7], System.Globalization.CultureInfo.InvariantCulture);
                if (arrgroupShape[1].Contains("Line"))
                {
                    if (dblShapeFlipH == 0)
                        dblShapeFlipH = -1;
                    if (dblShapeFlipV == 0)
                        dblShapeFlipV = -1;
                }

                _shapes.Clear();
                if (arrgroupShape[1].Contains("Line"))
                {
                    shapeCord = OoxTransform.CreateLine(dblShapeX, dblShapeY, dblShapeCX, dblShapeCY, dblShapeRot, dblShapeFlipH, dblShapeFlipV);
                }
                else
                {
                    shapeCord = new OoxTransform(dblShapeX, dblShapeY, dblShapeCX, dblShapeCY, dblShapeRot, 1, 1);
                }


                _shapes.Add(new OoxShape(new OoxTransform(Targetgroup, shapeCord)));

            }
            else if (arrgroup.Length == 3)
            {
                //  single level group
                arrInnerGroup = arrgroup[1].Split(':');

                dblgrpX = long.Parse(arrInnerGroup[0], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpY = long.Parse(arrInnerGroup[1], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpCX = long.Parse(arrInnerGroup[2], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpCY = long.Parse(arrInnerGroup[3], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpChX = long.Parse(arrInnerGroup[4], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpChY = long.Parse(arrInnerGroup[5], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpChCX = long.Parse(arrInnerGroup[6], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpChCY = long.Parse(arrInnerGroup[7], System.Globalization.CultureInfo.InvariantCulture);
                dblgrpRot = long.Parse(arrInnerGroup[8], System.Globalization.CultureInfo.InvariantCulture);

                TopLevelgroup = new OoxTransform(dblgrpChX, dblgrpChY, dblgrpChCX, dblgrpChCY, dblgrpX, dblgrpY, dblgrpCX, dblgrpCY, dblgrpRot, 1, 1);

                arrShapeCordinates = arrgroupShape[1].Split(':');

                //shape cordinates
                dblShapeX = long.Parse(arrShapeCordinates[1], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeY = long.Parse(arrShapeCordinates[2], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeCX = long.Parse(arrShapeCordinates[3], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeCY = long.Parse(arrShapeCordinates[4], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeRot = long.Parse(arrShapeCordinates[5], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeFlipH = long.Parse(arrShapeCordinates[6], System.Globalization.CultureInfo.InvariantCulture);
                dblShapeFlipV = long.Parse(arrShapeCordinates[7], System.Globalization.CultureInfo.InvariantCulture);

                if (arrgroupShape[1].Contains("Line"))
                {
                    if (dblShapeFlipH == 0)
                        dblShapeFlipH = -1;
                    if (dblShapeFlipV == 0)
                        dblShapeFlipV = -1;
                }
                _shapes.Clear();

                if (arrgroupShape[1].Contains("Line"))
                {
                    shapeCord = OoxTransform.CreateLine(dblShapeX, dblShapeY, dblShapeCX, dblShapeCY, dblShapeRot, dblShapeFlipH, dblShapeFlipV);
                }
                else
                {
                    shapeCord = new OoxTransform(dblShapeX, dblShapeY, dblShapeCX, dblShapeCY, dblShapeRot, 1, 1);
                }

                _shapes.Add(new OoxShape(new OoxTransform(TopLevelgroup, shapeCord)));

            }

            foreach (OoxShape shape in _shapes)
            {
                strRet = shape.OoxTransform.Odf;
                strLineRet = shape.OoxTransform.LineOdf;
            }

            if (arrgroupShape[1].Contains("Line"))
            {
                arrFinalRet = strLineRet.Split('@');

                if (arrgroup[0].Contains("X1"))
                {
                    if (arrFinalRet[1].Contains("NaN"))
                    {
                        strShapeCordinates = "0cm";
                    }
                    else
                    {
                        strShapeCordinates = arrFinalRet[0];
                    }

                }
                else if (arrgroup[0].Contains("Y1"))
                {
                    if (arrFinalRet[2].Contains("NaN"))
                    {
                        strShapeCordinates = "0cm";
                    }
                    else
                    {
                        strShapeCordinates = arrFinalRet[1];
                    }
                }
                else if (arrgroup[0].Contains("X2"))
                {
                    if (arrFinalRet[3].Contains("NaN"))
                    {
                        strShapeCordinates = "0cm";
                    }
                    else
                    {
                        strShapeCordinates = arrFinalRet[2];
                    }
                }
                else if (arrgroup[0].Contains("Y2"))
                {
                    if (arrFinalRet[3].Contains("NaN"))
                    {
                        strShapeCordinates = "0cm";
                    }
                    else
                    {
                        strShapeCordinates = arrFinalRet[3];
                    }
                }
            }

            else
            {
                arrFinalRet = strRet.Split('@');
                if (arrFinalRet[0] == "YESROT")
                {
                    if (arrgroup[0].Contains("Width"))
                    {
                        if (arrFinalRet[1].Contains("NaN"))
                        {
                            strShapeCordinates = "0cm";
                        }
                        else
                        {
                            strShapeCordinates = arrFinalRet[1];
                        }

                    }
                    else if (arrgroup[0].Contains("Height"))
                    {
                        if (arrFinalRet[2].Contains("NaN"))
                        {
                            strShapeCordinates = "0cm";
                        }
                        else
                        {
                            strShapeCordinates = arrFinalRet[2];
                        }
                    }
                    else if (arrgroup[0].Contains("DrawTranform"))
                    {
                        if (arrFinalRet[3].Contains("NaN"))
                        {
                            strShapeCordinates = "0cm";
                        }
                        else
                        {
                            strShapeCordinates = arrFinalRet[3];
                        }
                    }
                }
                else if (arrFinalRet[0] == "NOROT")
                {
                    if (arrgroup[0].Contains("Width"))
                    {
                        if (arrFinalRet[1].Contains("NaN"))
                        {
                            strShapeCordinates = "0cm";
                        }
                        else
                        {
                            strShapeCordinates = arrFinalRet[1];
                        }
                    }
                    else if (arrgroup[0].Contains("Height"))
                    {
                        if (arrFinalRet[2].Contains("NaN"))
                        {
                            strShapeCordinates = "0cm";
                        }
                        else
                        {
                            strShapeCordinates = arrFinalRet[2];
                        }
                    }
                    else if (arrgroup[0].Contains("SVGX"))
                    {
                        if (arrFinalRet[3].Contains("NaN"))
                        {
                            strShapeCordinates = "0cm";
                        }
                        else
                        {
                            strShapeCordinates = arrFinalRet[3];
                        }
                    }
                    else if (arrgroup[0].Contains("SVGY"))
                    {
                        if (arrFinalRet[4].Contains("NaN"))
                        {
                            strShapeCordinates = "0cm";
                        }
                        else
                        {
                            strShapeCordinates = arrFinalRet[4];
                        }
                    }
                }
            }

            return string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", strShapeCordinates);
        }

        private string EvalExpression(string text)
        {
            string[] arrVal = new string[4];
            arrVal = text.Split(':');
            Double x = 0;
            if (arrVal.Length == 5)
            {
                double arrValue1 = Double.Parse(arrVal[1], System.Globalization.CultureInfo.InvariantCulture);
                double arrValue2 = Double.Parse(arrVal[2], System.Globalization.CultureInfo.InvariantCulture);
                double arrValue3 = Double.Parse(arrVal[3], System.Globalization.CultureInfo.InvariantCulture);
                double arrValue4 = Double.Parse(arrVal[4], System.Globalization.CultureInfo.InvariantCulture);

                if (arrVal[0].Contains("svg-x1"))
                {
                    x = Math.Round(((arrValue1 -
                        Math.Cos(arrValue4) * arrValue2 +
                        Math.Sin(arrValue4) * arrValue3) / 360000), 2);
                }
                else if (arrVal[0].Contains("svg-y1"))
                {
                    x = Math.Round(((arrValue1 -
                        Math.Sin(arrValue4) * arrValue2 -
                        Math.Cos(arrValue4) * arrValue3) / 360000), 2);
                }
                else if (arrVal[0].Contains("svg-x2"))
                {
                    x = Math.Round(((arrValue1 +
                        Math.Cos(arrValue4) * arrValue2 -
                        Math.Sin(arrValue4) * arrValue3) / 360000), 2);
                }
                else if (arrVal[0].Contains("svg-y2"))
                {
                    x = Math.Round(((arrValue1 +
                        Math.Sin(arrValue4) * arrValue2 +
                        Math.Cos(arrValue4) * arrValue3) / 360000), 2);
                }

            }
            string strRet = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", x);

            return strRet + "cm";
        }

        // added by vipul for Shape Rotation
        //Start
        private string EvalRotationExpression(string text)
        {
            string[] arrVal = new string[7];
            string strRotate;
            string strTranslate;

            arrVal = text.Split(':');

            Double dblRadius = 0.0;
            Double dblXf = 0.0;
            Double dblYf = 0.0;
            Double dblalpha = 0.0;
            Double dblbeta = 0.0;
            Double dblgammaDegree = 0.0;
            Double dblgammaR = 0.0;
            Double dblRotAngle = 0.0;
            Double dblX2 = 0.0;
            Double dblY2 = 0.0;
            Double centreX = 0.0;
            Double centreY = 0.0;

            if (arrVal.Length == 8)
            {
                double arrValueX = Double.Parse(arrVal[1], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueY = Double.Parse(arrVal[2], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueCx = Double.Parse(arrVal[3], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueCy = Double.Parse(arrVal[4], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueFlipH = Double.Parse(arrVal[5], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueFlipV = Double.Parse(arrVal[6], System.Globalization.CultureInfo.InvariantCulture);
                double arrValueRot = Double.Parse(arrVal[7], System.Globalization.CultureInfo.InvariantCulture);


                if (arrVal[0].Contains("draw-transform"))
                {

                    centreX = arrValueX + (arrValueCx / 2);
                    centreY = arrValueY + (arrValueCy / 2);

                    if (arrValueFlipH == 1.0)
                    {
                        dblXf = arrValueX + ((centreX - arrValueX) * 2);
                    }
                    else
                    {
                        dblXf = arrValueX;
                    }

                    if (arrValueFlipV == 1.0)
                    {
                        dblYf = arrValueY + ((centreY - arrValueY) * 2);
                    }
                    else
                    {
                        dblYf = arrValueY;
                    }
                    dblRadius = Math.Sqrt((arrValueCx * arrValueCx) + (arrValueCy * arrValueCy)) / 2.0;

                    if ((arrValueFlipH == 0.0 && arrValueFlipV == 1.0) || (arrValueFlipH == 0.0 && arrValueFlipV == 1.0))
                    {
                        dblalpha = 360.00 - ((arrValueRot / 60000.00) % 360);
                    }
                    else
                    {
                        dblalpha = (arrValueRot / 60000.00) % 360;
                    }
                    if (dblalpha > 180.00)
                    {
                        dblRotAngle = (360.00 - dblalpha) / 180.00 * Math.PI;
                    }
                    else
                    {
                        dblRotAngle = (-1.00 * dblalpha) / 180.00 * Math.PI;
                    }
                    if (Math.Abs(centreY - dblYf) > 0)
                    {
                        dblbeta = Math.Atan(Math.Abs(centreX - dblXf) / Math.Abs(centreY - dblYf)) * (180.00 / Math.PI);
                    }

                    if (Math.Abs(dblbeta - dblalpha) > 0)
                    {
                        dblgammaDegree = ((dblbeta - dblalpha) % ((int)((dblbeta - dblalpha) / Math.Abs(dblbeta - dblalpha)) * 360)) + 90.00;
                    }
                    else
                    {
                        dblgammaDegree = 90.00;
                    }

                    dblgammaR = (360.00 - dblgammaDegree) / 180.00 * Math.PI;
                    dblX2 = Math.Round((centreX + (dblRadius * Math.Cos(dblgammaR))) / 360036.00, 3);
                    dblY2 = Math.Round((centreY + (dblRadius * Math.Sin(dblgammaR))) / 360036.00, 3);

                }
            }

            strRotate = "rotate (" + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.########}", dblRotAngle) + ")";

            strTranslate = "translate (" + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", dblX2) + "cm " + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", dblY2) + "cm)";

            return strRotate + " " + strTranslate;
        }
        //End 

        //Resolve relative path to absolute path
        private string HyperLinkPath(string text)
        {
            string[] arrVal = new string[2];
            arrVal = text.Split(':');
            string source = arrVal[1].ToString();
            string address = "";

            if (arrVal.Length == 2)
            {
                string returnInputFilePath = "";

                // for Commandline tool
                for (int i = 0; i < Environment.GetCommandLineArgs().Length; i++)
                {
                    if (Environment.GetCommandLineArgs()[i].ToString().ToUpper() == "/I")
                        returnInputFilePath = Path.GetDirectoryName(Environment.GetCommandLineArgs()[i + 1]);
                }

                //for addins
                if (returnInputFilePath == "")
                {
                    returnInputFilePath = Environment.CurrentDirectory;
                }

                string linkPathLocation = Path.GetFullPath(Path.Combine(returnInputFilePath, source)).Replace("\\", "/").Replace(" ", "%20");
                address = "/" + linkPathLocation;

            }
            return address.ToString();

        }
        //End

        //Image Cropping Added by Sonata-15/11/2007
        private void EvalImageCrop(string text)
        {
            string[] arrVal = new string[6];
            arrVal = text.Split(':');
            string source = arrVal[1].ToString();
            int left = int.Parse(arrVal[2].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            int right = int.Parse(arrVal[3].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            int top = int.Parse(arrVal[4].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            int bottom = int.Parse(arrVal[5].ToString(), System.Globalization.CultureInfo.InvariantCulture);

            string tempFileName = AbstractConverter.inputTempFileName.ToString();
            ZipResolver zipResolverObj = new ZipResolver(tempFileName);
            OdfZipUtils.ZipArchiveWriter zipobj = new OdfZipUtils.ZipArchiveWriter(zipResolverObj);
            string widht_height_res = zipobj.ImageCopyBinary(source);
            zipResolverObj.Dispose();
            zipobj.Close();

            string[] arrValues = new string[3];
            arrValues = widht_height_res.Split(':');
            double width = Double.Parse(arrValues[0].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            double height = Double.Parse(arrValues[1].ToString(), System.Globalization.CultureInfo.InvariantCulture);
            double res = Double.Parse(arrValues[2].ToString(), System.Globalization.CultureInfo.InvariantCulture);

            double cx = width * 2.54 / res;
            double cy = height * 2.54 / res;

            double odpLeft = left * cx / 100000;
            double odpRight = right * cx / 100000;
            double odpTop = top * cy / 100000;
            double odpBottom = bottom * cy / 100000;

            string result = string.Concat("rect(", string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.##}", odpTop) + "cm" + " " + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.##}", odpRight) + "cm" + " " + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.##}", odpBottom) + "cm" + " " + string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.##}", odpLeft) + "cm", ")");

            this.nextWriter.WriteString(result);
        }
        //End

        //Callout Adjustments Calculation Added by Sonata
        private string EvalCalloutAdjustment(string text)
        {
            string[] arrVal = new string[16];
            arrVal = text.Split(':');
            string drawMod = "";

            string callAdjVal = (arrVal[0].ToString());

            double X = Double.Parse(arrVal[1], System.Globalization.CultureInfo.InvariantCulture);
            double Y = Double.Parse(arrVal[2], System.Globalization.CultureInfo.InvariantCulture);
            double CX = Double.Parse(arrVal[3], System.Globalization.CultureInfo.InvariantCulture);
            double CY = Double.Parse(arrVal[4], System.Globalization.CultureInfo.InvariantCulture);
            int flipH = int.Parse(arrVal[5], System.Globalization.CultureInfo.InvariantCulture);
            int flipV = int.Parse(arrVal[6], System.Globalization.CultureInfo.InvariantCulture);
            double rot = Double.Parse(arrVal[7], System.Globalization.CultureInfo.InvariantCulture);

            string strviewdyFinal = "";
            string strviewdyFinal1 = "";
            string strviewdyFinal2 = "";
            string strviewdyFinal3 = "";

            string strviewdxFinal = "";
            string strviewdxFinal1 = "";
            string strviewdxFinal2 = "";
            string strviewdxFinal3 = "";

            double xCenter = (X + CX / 2);
            double yCenter = (Y + CY / 2);
            double ang = ((rot / 60000) * (Math.PI / 180.0));

            double xCtrBy2;
            if (flipH == 1)
            {
                xCtrBy2 = ((-1) * (CX / 2));
            }
            else
            {
                xCtrBy2 = (CX / 2);
            }

            double yCtrBy2;
            if (flipV == 1)
            {
                yCtrBy2 = ((-1) * (CY / 2));
            }
            else
            {
                yCtrBy2 = (CY / 2);
            }

            double X1;
            X1 = (xCenter - Math.Cos(ang) * xCtrBy2 + Math.Sin(ang) * yCtrBy2);

            double Y1;
            Y1 = (yCenter - Math.Sin(ang) * xCtrBy2 - Math.Cos(ang) * yCtrBy2);

            double X2;
            X2 = (xCenter + Math.Cos(ang) * xCtrBy2 - Math.Sin(ang) * yCtrBy2);

            double Y2;
            Y2 = (yCenter + Math.Sin(ang) * xCtrBy2 + Math.Cos(ang) * yCtrBy2);

            X1 = Math.Round((X1 / 360000), 3);
            Y1 = Math.Round((Y1 / 360000), 3);
            X2 = Math.Round((X2 / 360000), 3);
            Y2 = Math.Round((Y2 / 360000), 3);

            double width;
            width = (X2 - X1);

            double height;
            height = (Y2 - Y1);

            if (callAdjVal.ToString().Trim() == "Callout-AdjNotline")
            {
                double fm1 = 0.0;
                double fm2 = 0.0;

                if (arrVal[8] != "")
                {
                    fm1 = Double.Parse(arrVal[8], System.Globalization.CultureInfo.InvariantCulture);
                }
                if (arrVal[9] != "")
                {
                    fm2 = Double.Parse(arrVal[9], System.Globalization.CultureInfo.InvariantCulture);
                }

                double dxPos;
                dxPos = (width * fm1 / 100000);

                double dyPos;
                dyPos = (height * fm2 / 100000);

                double dxFinal;
                dxFinal = ((width / 2) + dxPos);

                double dyFinal;
                dyFinal = ((height / 2) + dyPos);

                double viewWidth = 21600;
                double viewHeight = 21600;

                double viewdxFinal;
                viewdxFinal = (dxFinal / width * viewWidth);

                double viewdyFinal;
                viewdyFinal = (dyFinal / height * viewHeight);

                strviewdxFinal = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdxFinal);
                strviewdyFinal = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdyFinal);

                drawMod = strviewdxFinal + " " + strviewdyFinal;
            }

            if ((callAdjVal.ToString().Trim() == "Callout-AdjLine1") || (callAdjVal.ToString().Trim() == "Callout-AdjLine2") || (callAdjVal.ToString().Trim() == "Callout-AdjLine3"))
            {
                double fm1 = 0.0;
                double fm2 = 0.0;
                double fm3 = 0.0;
                double fm4 = 0.0;
                double fm5 = 0.0;
                double fm6 = 0.0;
                double fm7 = 0.0;
                double fm8 = 0.0;

                if (callAdjVal.ToString().Trim() == "Callout-AdjLine1")
                {
                    if (arrVal[8] != "")
                    {
                        fm1 = Double.Parse(arrVal[8], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[9] != "")
                    {
                        fm2 = Double.Parse(arrVal[9], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[10] != "")
                    {
                        fm3 = Double.Parse(arrVal[10], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[11] != "")
                    {
                        fm4 = Double.Parse(arrVal[11], System.Globalization.CultureInfo.InvariantCulture);
                    }
                }

                if (callAdjVal.ToString().Trim() == "Callout-AdjLine2")
                {
                    if (arrVal[8] != "")
                    {
                        fm1 = Double.Parse(arrVal[8], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[9] != "")
                    {
                        fm2 = Double.Parse(arrVal[9], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[10] != "")
                    {
                        fm3 = Double.Parse(arrVal[10], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[11] != "")
                    {
                        fm4 = Double.Parse(arrVal[11], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[12] != "")
                    {
                        fm5 = Double.Parse(arrVal[12], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[13] != "")
                    {
                        fm6 = Double.Parse(arrVal[13], System.Globalization.CultureInfo.InvariantCulture);
                    }
                }

                if (callAdjVal.ToString().Trim() == "Callout-AdjLine3")
                {
                    if (arrVal[8] != "")
                    {
                        fm1 = Double.Parse(arrVal[8], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[9] != "")
                    {
                        fm2 = Double.Parse(arrVal[9], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[10] != "")
                    {
                        fm3 = Double.Parse(arrVal[10], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[11] != "")
                    {
                        fm4 = Double.Parse(arrVal[11], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[12] != "")
                    {
                        fm5 = Double.Parse(arrVal[12], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[13] != "")
                    {
                        fm6 = Double.Parse(arrVal[13], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[14] != "")
                    {
                        fm7 = Double.Parse(arrVal[14], System.Globalization.CultureInfo.InvariantCulture);
                    }
                    if (arrVal[15] != "")
                    {
                        fm8 = Double.Parse(arrVal[15], System.Globalization.CultureInfo.InvariantCulture);
                    }
                }

                double viewWidth = 21600;
                double viewHeight = 21600;

                double dxPos;
                dxPos = (width * fm1 / 100);
                double dxFinal;
                dxFinal = ((width / 2) + dxPos);
                double viewdxFinal;
                viewdxFinal = (dxFinal / width * viewWidth);

                viewdxFinal = (viewdxFinal / 1000);

                double dyPos;
                dyPos = (height * fm2 / 100);
                double dyFinal;
                dyFinal = ((height / 2) + dyPos);
                double viewdyFinal;
                viewdyFinal = (dyFinal / height * viewHeight);

                viewdyFinal = (viewdyFinal / 1000);

                double dxPos1;
                dxPos1 = (width * fm3 / 100);
                double dxFinal1;
                dxFinal1 = ((width / 2) + dxPos1);
                double viewdxFinal1;
                viewdxFinal1 = (dxFinal1 / width * viewWidth);

                viewdxFinal1 = (viewdxFinal1 / 1000);

                double dyPos1;
                dyPos1 = (height * fm4 / 100);
                double dyFinal1;
                dyFinal1 = ((height / 2) + dyPos1);
                double viewdyFinal1;
                viewdyFinal1 = (dyFinal1 / height * viewHeight);

                viewdyFinal1 = (viewdyFinal1 / 1000);

                double dxPos2;
                dxPos2 = (width * fm5 / 100);
                double dxFinal2;
                dxFinal2 = ((width / 2) + dxPos2);
                double viewdxFinal2;
                viewdxFinal2 = (dxFinal2 / width * viewWidth);

                viewdxFinal2 = (viewdxFinal2 / 1000);

                double dyPos2;
                dyPos2 = (height * fm6 / 100);
                double dyFinal2;
                dyFinal2 = ((height / 2) + dyPos2);
                double viewdyFinal2;
                viewdyFinal2 = (dyFinal2 / height * viewHeight);

                viewdyFinal2 = (viewdyFinal2 / 1000);

                double dxPos3;
                dxPos3 = (width * fm7 / 100);
                double dxFinal3;
                dxFinal3 = ((width / 2) + dxPos3);
                double viewdxFinal3;
                viewdxFinal3 = (dxFinal3 / width * viewWidth);

                viewdxFinal3 = (viewdxFinal3 / 1000);

                double dyPos3;
                dyPos3 = (height * fm8 / 100);
                double dyFinal3;
                dyFinal3 = ((height / 2) + dyPos3);
                double viewdyFinal3;
                viewdyFinal3 = (dyFinal3 / height * viewHeight);

                viewdyFinal3 = (viewdyFinal3 / 1000);

                strviewdyFinal = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdyFinal);
                strviewdyFinal1 = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdyFinal1);
                strviewdyFinal2 = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdyFinal2);
                strviewdyFinal3 = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdyFinal3);

                strviewdxFinal = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdxFinal);
                strviewdxFinal1 = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdxFinal1);
                strviewdxFinal2 = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdxFinal2);
                strviewdxFinal3 = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", viewdxFinal3);

                if (callAdjVal.ToString().Trim() == "Callout-AdjLine1")
                {
                    drawMod = strviewdyFinal1 + " " + strviewdxFinal1 + " " + strviewdyFinal + " " + strviewdxFinal;
                }

                if (callAdjVal.ToString().Trim() == "Callout-AdjLine2")
                {
                    drawMod = strviewdyFinal2 + " " + strviewdxFinal2 + " " + strviewdyFinal1 + " " + strviewdxFinal1 + " " + strviewdyFinal + " " + strviewdxFinal;
                }

                if (callAdjVal.ToString().Trim() == "Callout-AdjLine3")
                {
                    drawMod = strviewdyFinal3 + " " + strviewdxFinal3 + " " + strviewdyFinal2 + " " + strviewdxFinal2 + " " + strviewdyFinal1 + " " + strviewdxFinal1 + " " + strviewdyFinal + " " + strviewdxFinal;
                }
            }

            return drawMod.ToString();
        }
        //End

        // added for Shadow calculation
        private string EvalShadowExpression(string text)
        {
            string[] arrVal = new string[2];
            arrVal = text.Split(':');
            double x = 0;
            if (arrVal.Length == 3)
            {
                double arrDist = Double.Parse(arrVal[1], System.Globalization.CultureInfo.InvariantCulture);
                double arrDir = Double.Parse(arrVal[2], System.Globalization.CultureInfo.InvariantCulture);

                arrDir = (arrDir / 60000) * (Math.PI / 180.0);
                if (arrVal[0].Contains("shadow-offset-x"))
                {
                    x = Math.Round(Math.Sin((arrDir)) * (arrDist / 360000), 3);
                    //x = Math.Sin((arrDir)) * (arrDist / 360000);
                }
                else if (arrVal[0].Contains("shadow-offset-y"))
                {
                    x = Math.Round(Math.Cos((arrDir)) * (arrDist / 360000), 3);
                    //x =Math.Cos((arrDir)) * (arrDist / 360000);
                }
            }
            string strRet = string.Format(System.Globalization.CultureInfo.InvariantCulture, "{0:0.####}", x);

            return strRet + "cm";
        }

        public void WriteStoredSpan()
        {
            Element e = (Element)this.store.Peek();

            if (e is Span)
            {
                Span span = (Span)e;

                if (span.HasSymbol)
                {
                    string code = span.GetChild("symbol", PCHAR_NAMESPACE).GetAttributeValue("code", PCHAR_NAMESPACE);
                    span.Replace(span.GetChild("symbol", PCHAR_NAMESPACE), char.ConvertFromUtf32(int.Parse(code, System.Globalization.NumberStyles.HexNumber)));
                    // do not write the span if embedded in another span (it will be written afterwards)
                    if (!IsEmbeddedSpan())
                    {
                        span.Write(nextWriter);
                    }
                }
                else
                {
                    if (this.store.Count < 2)
                    {
                        span.Write(nextWriter);
                    }
                }
            }
            else
            {
                if (this.store.Count < 2)
                {
                    e.Write(nextWriter);
                }
            }
        }

        private void StartStoreElement()
        {
            Element element = (Element)this.currentNode.Peek();

            if (this.store.Count > 0)
            {
                Element parent = (Element)this.store.Peek();
                parent.AddChild(element);
            }

            this.store.Push(element);
        }


        private void EndStoreElement()
        {
            Element e = (Element)this.store.Pop();
        }


        private void StartStoreAttribute()
        {
            Element parent = (Element)store.Peek();
            Attribute attr = (Attribute)this.currentNode.Peek();
            parent.AddAttribute(attr);
            this.store.Push(attr);
        }


        private void StoreString(string text)
        {
            Node node = (Node)this.currentNode.Peek();

            if (node is Element)
            {
                /* This condition is to check tab carector in a string and 
                replace with text:tab node */
                if (text.Contains("\t"))
                {
                    char[] a = new char[] { '\t' };
                    string[] p = text.Split(a);

                    for (int i = 0; i < p.Length; i++)
                    {
                        Element element = (Element)this.store.Peek();
                        element.AddChild(p[i].ToString());
                        if (i < p.Length - 1)
                        {
                            WriteStartElement("text", "tab", element.Ns);
                            WriteEndElement();
                        }
                    }
                }
                else
                {
                    Element element = (Element)this.store.Peek();
                    //added by Sonata
                    if (text.Contains(":::currentDate:::"))
                    {
                        string strDate = "";
                        string[] strVal = text.Split('@');
                        if (strVal[1] == "datetime1")
                        {
                            strDate = DateTime.Now.ToString("dd/MM/yyyy");

                        }
                        else if (strVal[1] == "datetime2")
                        {
                            strDate = DateTime.Now.ToLongDateString();
                        }
                        else
                        {
                            strDate = DateTime.Now.ToShortDateString();
                        }
                        element.AddChild(strDate);
                    }
                    //added by Sonata
                    else
                    {
                        element.AddChild(text);
                    }
                }

            }
            else
            {
                Attribute attr = (Attribute)store.Peek();
                // This condition is to check for hyperlink relative path 
                if (text.Contains("hyperlink-path"))
                {
                    string hPath = HyperLinkPath(text);
                    attr.Value += hPath;
                }
                //Sona: Added code for rotation
                else if (text.Contains("draw-transform"))
                {
                    string rotStr = EvalRotationExpression(text);
                    attr.Value += rotStr;
                }
                else if (text.Contains("svg-x") || text.Contains("svg-y"))
                {
                    attr.Value += EvalExpression(text);
                }
                //sonata: Word free from shapes

                else if (text.Contains("WordshapesEnhance-Path"))
                {

                    attr.Value += EvalWordEnhacePath(text);

                }
                else if (text.Contains("WordshapesFormula"))
                {

                    attr.Value += EvalWordShapesFormula(text);

                }
                else if (text.Contains("Wordshapes-draw-modifier"))
                {

                    attr.Value += EvalWordModifier(text);

                }
                else
                {
                    attr.Value += text;
                }

            }
        }

        private void EndStoreAttribute()
        {
            this.store.Pop();
        }

        /// <summary>
        /// Returns true if the current node contains a date.
        /// This is needed for special post-processing of XSD date time values into an ODF-compliant format
        /// (ODF only accepts the format YYYY-MM-DDThh:mm:ss without trailing TZD).
        /// 
        /// Additionaly, OOo interpretes times as local times -> we convert from UTC to local time 
        /// (Of course this only gives the correct value if the document is converted in the same time zone 
        /// for which the time value has been written, but it is better than nothing). This allows to keep e.g. the 
        /// correct value for CREATIONDATE fields when not moving within time zones
        /// </summary>
        /// <returns></returns>
        private bool IsDate()
        {
            if (this.currentNode.Count > 0)
            {
                Element element = this.currentNode.Peek() as Element;

                if (element != null
                    && (element.Prefix.Equals("meta") && (element.Name.Equals("creation-date") || element.Name.Equals("print-date"))
                        || element.Prefix.Equals("dc") && element.Name.Equals("date")))
                {
                    return true;
                }
            }
            return false;
        }

        private bool IsSpan()
        {
            Node node = (Node)this.currentNode.Peek();
            if (node is Element)
            {
                Element element = (Element)node;
                if ("span".Equals(element.Name) && TEXT_NAMESPACE.Equals(element.Ns))
                {
                    return true;
                }
            }
            return false;
        }


        private bool InSpan()
        {
            return IsSpan() || this.store.Count > 0;
        }


        private bool IsEmbeddedSpan()
        {
            Node node = (Node)this.currentNode.ToArray().GetValue(1);
            if (node is Element)
            {
                Element element = (Element)node;
                if ("span".Equals(element.Name) && TEXT_NAMESPACE.Equals(element.Ns))
                {
                    return true;
                }
            }
            return false;
        }


        protected class Span : Element
        {
            public Span(Element e)
                :
                base(e.Prefix, e.Name, e.Ns) { }

            public Span(string prefix, string localName, string ns)
                :
                base(prefix, localName, ns) { }

            public bool HasSymbol
            {
                get
                {
                    return HasSymbolNode(this);
                }
            }

            private bool HasSymbolNode(Element e)
            {
                if (e.GetChild("symbol", PCHAR_NAMESPACE) != null)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }


    }
}
