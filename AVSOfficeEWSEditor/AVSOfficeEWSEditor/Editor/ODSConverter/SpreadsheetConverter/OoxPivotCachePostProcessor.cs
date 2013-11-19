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
RefNo-1 27-Dec-2007 Sateesh Reddy               Changes done for localization and error regarding obj ref not set to an instance of obj.  
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
*/

using System;
using System.Xml;
using System.Collections;
using System.Globalization;
using System.IO;
using CleverAge.OdfConverter.OdfConverterLib;

namespace CleverAge.OdfConverter.Spreadsheet
{

    public class OoxPivotCachePostProcessor : AbstractPostProcessor
    {
        private Stack pivotContext;
        private const string PXSI_NAMESPACE = "urn:cleverage:xmlns:post-processings:pivotTable";
        private const string REL_NAMESPACE = "http://schemas.openxmlformats.org/package/2006/relationships";
        private const string EXCEL_NAMESPACE = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
        private bool isPxsi;

        //if there are 2 variables with simillar names but one ends with "Text" (i.e. pivotTable and pivotTableText)
        //one contains cell values other (ending with Text) cell text as it is displayed

        //<pxsi:pivotTable> variables
        private bool isInPivotTable;
        private bool isSheetNum;
        private string cacheSheetNum;
        private bool isColStart;
        private string cacheColStart;
        private bool isColEnd;
        private string cacheColEnd;
        private bool isRowStart;
        private string cacheRowStart;
        private bool isRowEnd;
        private string cacheRowEnd;
        private string[,] pivotTable;
        private string[,] pivotTableText;
        private Hashtable[] fieldNames;
        private Hashtable[] fieldNamesText;
        private Hashtable[,] fieldItems;
        private Hashtable[,] fieldItemsText;
        private Hashtable[,] fieldItemsLowerCaseText; //field items are case insensitive and here are stored lower-cased field items (as cell text)
        //fieldItems[i,j] (dimesion i equals number of pivotFields, dimesion j equals 2) 
        //fieldItems variable for each pivotField contains hashtable with unique values of this pivotField and a sequential number
        //at fieldItems[i,0] pivotField[i] values are the key and sequential number are the key values
        //at fieldItems[i,1] sequential number is the key and pivotField[i] values are the key values  

        //<pxsi:pivotCell> variables
        private bool isInPivotCell;
        private Hashtable pivotCells;
        private Hashtable pivotCellsText;
        private bool isPivotCellSheet;
        private string pivotCellSheet;
        private bool isPivotCellCol;
        private string pivotCellCol;
        private bool isPivotCellRow;
        private string pivotCellRow;
        private bool isInPivotCellVal;
        private string pivotCellVal;
        private bool isInPivotCellText;
        private string pivotCellText;

        //<pxsi:cacheRecords> variables
        private bool isInCacheRecords;

        //<pxsi:pivotFields> variables
        private bool isInPivotFields;
        private bool isPivotNames;
        private string pivotNames;
        private bool isPivotAxes;
        private string pivotAxes;
        private bool isPivotSort;
        private string pivotSort;
        private bool isPivotHide;
        private string pivotHide;
        private bool isPivotSubtotals;
        private string pivotSubtotals;
        private bool isPivotBlanks;
        private string pivotBlanks;
        private bool isPivotOutlines;
        private string pivotOutlines;
        private bool isPivotEmptyLines;
        private string pivotEmptyLines;
        //<pxsi:cacheFields> variables
        private bool isInCacheFields;

        //<pxsi:fieldPos> variables
        private bool isInFieldPos;
        private bool isFieldPosName;
        private string fieldPosName;
        private bool isFieldPosAttribute;
        private string fieldPosAttribute;

        //<pxsi:pageItem> variables
        private bool isInPageItem;
        private bool isPageField;
        private string pageField;
        private bool isPageItem;
        private string pageItem;

        //<pxsi:filterType> variables
        private bool isInFilterType;
        private bool isFilterField;
        private string filterField;
        private bool isCondition;
        private string condition;
        private bool isFilterValue;
        private string filterValue;
        private IFormatProvider provider;

        public OoxPivotCachePostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.pivotContext = new Stack();
            this.isPxsi = false;
            this.provider = new NumberFormatInfo();
            //<pxsi:pivotTable> variables
            this.isInPivotTable = false;
            this.isSheetNum = false;
            this.cacheSheetNum = "";
            this.isColStart = false;
            this.cacheColStart = "";
            this.isColEnd = false;
            this.cacheColEnd = "";
            this.isRowStart = false;
            this.cacheRowStart = "";
            this.isRowEnd = false;
            this.cacheRowEnd = "";

            //<pxsi:pivotCell> variables
            this.isInPivotCell = false;
            this.isPivotCellSheet = false;
            this.pivotCellSheet = "";
            this.isPivotCellCol = false;
            this.pivotCellCol = "";
            this.isPivotCellRow = false;
            this.pivotCellRow = "";
            this.isInPivotCellVal = false;
            this.pivotCellVal = "";
            this.isInPivotCellText = false;
            this.pivotCellText = "";
            this.pivotCells = new Hashtable();
            this.pivotCellsText = new Hashtable();

            //<pxsi:cacheRecords> variables
            this.isInCacheRecords = false;

            //<pxsi:pivotFields> variables
            this.isInPivotFields = false;
            this.isPivotNames = false;
            this.pivotNames = "";
            this.isPivotAxes = false;
            this.pivotAxes = "";
            this.isPivotSort = false;
            this.pivotSort = "";
            this.isPivotHide = false;
            this.pivotHide = "";
            this.isPivotSubtotals = false;
            this.pivotSubtotals = "";
            this.isPivotBlanks = false;
            this.pivotBlanks = "";
            this.isPivotOutlines = false;
            this.pivotOutlines = "";
            this.isPivotEmptyLines = false;
            this.pivotEmptyLines = "";

            //<pxsi:cacheFields> variables
            this.isInCacheFields = false;

            //<pxsi:fieldPos> variables
            this.isInFieldPos = false;
            this.isFieldPosName = false;
            this.fieldPosName = "";
            this.isFieldPosAttribute = false;
            this.fieldPosAttribute = "";

            //<pxsi:pageItem> variables
            this.isInPageItem = false;
            this.isPageField = false;
            this.pageField = "";
            this.isPageItem = false;
            this.pageItem = "";

            //<pxsi:filterType> variables
            this.isInFilterType = false;
            this.isFilterField = false;
            this.filterField = "";
            this.isCondition = false;
            this.condition = "";
            this.isFilterValue = false;
            this.filterValue = "";

        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {

            if (PXSI_NAMESPACE.Equals(ns) && "pivotCell".Equals(localName))
            {
                this.pivotContext = new Stack();
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInPivotCell = true;
                this.isPxsi = true;
            }
            else if (PXSI_NAMESPACE.Equals(ns) && "val".Equals(localName))
            {
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInPivotCellVal = true;
                //this is a child element of <pxsi:pivotCell> so do not modify isPxsi variable
            }
            else if (PXSI_NAMESPACE.Equals(ns) && "text".Equals(localName))
            {
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInPivotCellText = true;
                //this is child element of <pxsi:pivotCell> so do not modify isPxsi variable
            }
            else if (PXSI_NAMESPACE.Equals(ns) && "pivotTable".Equals(localName))
            {
                this.pivotContext = new Stack();
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInPivotTable = true;
                this.isPxsi = true;

                //reset <pxsi:pivotTable> variables
                this.cacheSheetNum = "";
                this.cacheColStart = "";
                this.cacheColEnd = "";
                this.cacheRowStart = "";
                this.cacheRowEnd = "";

                //reset <pxsi:pivotFields> variables
                this.pivotNames = "";
                this.pivotAxes = "";
                this.pivotSort = "";
                this.pivotHide = "";
                this.pivotSubtotals = "";
                this.pivotBlanks = "";
                this.pivotOutlines = "";
                this.pivotEmptyLines = "";
            }
            else if (PXSI_NAMESPACE.Equals(ns) && "pivotFields".Equals(localName))
            {
                this.pivotContext = new Stack();
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInPivotFields = true;
                this.isPxsi = true;
            }
            else if (PXSI_NAMESPACE.Equals(ns) && "fieldPos".Equals(localName))
            {
                this.pivotContext = new Stack();
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInFieldPos = true;
                this.isPxsi = true;
            }
            else if (PXSI_NAMESPACE.Equals(ns) && "cacheRecords".Equals(localName))
            {
                this.pivotContext = new Stack();
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInCacheRecords = true;
                this.isPxsi = true;
            }
            else if (PXSI_NAMESPACE.Equals(ns) && "cacheFields".Equals(localName))
            {
                this.pivotContext = new Stack();
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInCacheFields = true;
                this.isPxsi = true;
            }
            else if (PXSI_NAMESPACE.Equals(ns) && "pageItem".Equals(localName))
            {
                this.pivotContext = new Stack();
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInPageItem = true;
                this.isPxsi = true;
            }
            else if (PXSI_NAMESPACE.Equals(ns) && "filterType".Equals(localName))
            {
                this.pivotContext = new Stack();
                this.pivotContext.Push(new Element(prefix, localName, ns));
                this.isInFilterType = true;
                this.isPxsi = true;
            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
        }

        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            if (isInPivotTable)
            {
                if (PXSI_NAMESPACE.Equals(ns) && "sheetNum".Equals(localName))
                    this.isSheetNum = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "colStart".Equals(localName))
                    this.isColStart = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "colEnd".Equals(localName))
                    this.isColEnd = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "rowStart".Equals(localName))
                    this.isRowStart = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "rowEnd".Equals(localName))
                    this.isRowEnd = true;
            }
            else if (isInPivotCell)
            {
                if (PXSI_NAMESPACE.Equals(ns) && "col".Equals(localName))
                    this.isPivotCellCol = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "row".Equals(localName))
                    this.isPivotCellRow = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "sheetNum".Equals(localName))
                    this.isPivotCellSheet = true;
            }
            else if (isInPivotFields)
            {
                if (PXSI_NAMESPACE.Equals(ns) && "names".Equals(localName))
                    this.isPivotNames = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "axes".Equals(localName))
                    this.isPivotAxes = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "sort".Equals(localName))
                    this.isPivotSort = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "hide".Equals(localName))
                    this.isPivotHide = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "subtotals".Equals(localName))
                    this.isPivotSubtotals = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "blanks".Equals(localName))
                    this.isPivotBlanks = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "outlines".Equals(localName))
                    this.isPivotOutlines = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "emptyLines".Equals(localName))
                    this.isPivotEmptyLines = true;
            }
            else if (isInCacheFields)
            {
            }
            else if (isInFieldPos)
            {
                if (PXSI_NAMESPACE.Equals(ns) && "name".Equals(localName))
                    this.isFieldPosName = true;
                if (PXSI_NAMESPACE.Equals(ns) && "attribute".Equals(localName))
                    this.isFieldPosAttribute = true;
            }
            else if (isInPageItem)
            {
                if (PXSI_NAMESPACE.Equals(ns) && "pageField".Equals(localName))
                    this.isPageField = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "pageItem".Equals(localName))
                    this.isPageItem = true;
            }
            else if (isInFilterType)
            {
                if (PXSI_NAMESPACE.Equals(ns) && "filterField".Equals(localName))
                    this.isFilterField = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "condition".Equals(localName))
                    this.isCondition = true;
                else if (PXSI_NAMESPACE.Equals(ns) && "filterValue".Equals(localName))
                    this.isFilterValue = true;
            }
            else
            {
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }
        }

        public override void WriteString(string text)
        {
            if (isInPivotTable)
            {
                if (isSheetNum)
                    this.cacheSheetNum += text;
                else if (isColStart)
                    this.cacheColStart += text;
                else if (isColEnd)
                    this.cacheColEnd += text;
                else if (isRowStart)
                    this.cacheRowStart += text;
                else if (isRowEnd)
                    this.cacheRowEnd += text;
            }
            else if (isInPivotCell)
            {
                if (isPivotCellCol)
                    this.pivotCellCol += text;
                else if (isPivotCellRow)
                    this.pivotCellRow += text;
                else if (isPivotCellSheet)
                    this.pivotCellSheet += text;
                else if (isInPivotCellVal)
                    this.pivotCellVal += text;
                else if (isInPivotCellText)
                    this.pivotCellText += text;
            }
            else if (isInPivotFields)
            {
                if (isPivotNames)
                    this.pivotNames += text;
                else if (isPivotAxes)
                    this.pivotAxes += text;
                else if (isPivotSort)
                    this.pivotSort += text;
                else if (isPivotHide)
                    this.pivotHide += text;
                else if (isPivotSubtotals)
                    this.pivotSubtotals += text;
                else if (isPivotBlanks)
                    this.pivotBlanks += text;
                else if (isPivotOutlines)
                    this.pivotOutlines += text;
                else if (isPivotEmptyLines)
                    this.pivotEmptyLines += text;
            }
            else if (isInCacheFields)
            {
            }
            else if (isInFieldPos)
            {
                if (isFieldPosName)
                    this.fieldPosName += text;
                if (isFieldPosAttribute)
                    this.fieldPosAttribute += text;
            }
            else if (isInPageItem)
            {
                if (isPageField)
                    this.pageField += text;
                else if (isPageItem)
                    this.pageItem += text;
            }
            else if (isInFilterType)
            {
                if (isFilterField)
                    this.filterField += text;
                else if (isCondition)
                    this.condition += text;
                else if (isFilterValue)
                    this.filterValue += text;
            }
            else
            {
                this.nextWriter.WriteString(text);
            }
        }

        public override void WriteEndAttribute()
        {
            if (isSheetNum)
                this.isSheetNum = false;
            else if (isColStart)
                this.isColStart = false;
            else if (isColEnd)
                this.isColEnd = false;
            else if (isRowStart)
                this.isRowStart = false;
            else if (isRowEnd)
                this.isRowEnd = false;
            else if (isPivotCellCol)
                this.isPivotCellCol = false;
            else if (isPivotCellRow)
                this.isPivotCellRow = false;
            else if (isPivotCellSheet)
                this.isPivotCellSheet = false;
            else if (isPivotNames)
                this.isPivotNames = false;
            else if (isPivotAxes)
                this.isPivotAxes = false;
            else if (isPivotSort)
                this.isPivotSort = false;
            else if (isPivotHide)
                this.isPivotHide = false;
            else if (isPivotSubtotals)
                this.isPivotSubtotals = false;
            else if (isPivotBlanks)
                this.isPivotBlanks = false;
            else if (isPivotOutlines)
                this.isPivotOutlines = false;
            else if (isPivotEmptyLines)
                this.isPivotEmptyLines = false;
            else if (isInCacheFields)
            {
            }
            else if (isFieldPosName)
                this.isFieldPosName = false;
            else if (isFieldPosAttribute)
                this.isFieldPosAttribute = false;
            else if (isPageField)
                this.isPageField = false;
            else if (isPageItem)
                this.isPageItem = false;
            else if (isFilterField)
                this.isFilterField = false;
            else if (isCondition)
                this.isCondition = false;
            else if (isFilterValue)
                this.isFilterValue = false;
            else if (!isPxsi)
                this.nextWriter.WriteEndAttribute();
        }

        public override void WriteEndElement()
        {
            if (isPxsi)
            {
                Element element = (Element)pivotContext.Pop();

                if (PXSI_NAMESPACE.Equals(element.Ns) && "pivotTable".Equals(element.Name))
                {
                    //Console.WriteLine(" Row start: " + cacheRowStart + " Row end: " + cacheRowEnd + " Col start: " + cacheColStart + " Col end: " + cacheColEnd + " sheet: " + cacheSheetNum);
                    this.pivotTable = new string[Convert.ToInt32(cacheRowEnd) - Convert.ToInt32(cacheRowStart) + 1, Convert.ToInt32(cacheColEnd) - Convert.ToInt32(cacheColStart) + 1];
                    this.pivotTableText = new string[Convert.ToInt32(cacheRowEnd) - Convert.ToInt32(cacheRowStart) + 1, Convert.ToInt32(cacheColEnd) - Convert.ToInt32(cacheColStart) + 1];

                    CreatePivotTable(Convert.ToInt32(cacheSheetNum), Convert.ToInt32(cacheRowStart), Convert.ToInt32(cacheRowEnd), Convert.ToInt32(cacheColStart), Convert.ToInt32(cacheColEnd));

                    this.isInPivotTable = false;

                    //other <pivotTable> variables are being used in other postprocessor commands
                    //so they are not erased here

                    this.isPxsi = false;
                }
                else if (PXSI_NAMESPACE.Equals(element.Ns) && "pivotCell".Equals(element.Name))
                {
                    if (!pivotCells.Contains(pivotCellSheet + "#" + pivotCellCol + ":" + pivotCellRow))
                    {
                        pivotCells.Add(pivotCellSheet + "#" + pivotCellCol + ":" + pivotCellRow, pivotCellVal);
                        pivotCellsText.Add(pivotCellSheet + "#" + pivotCellCol + ":" + pivotCellRow, pivotCellText);
                    }
                    this.isInPivotCell = false;
                    this.pivotCellSheet = "";
                    this.pivotCellCol = "";
                    this.pivotCellRow = "";
                    this.pivotCellVal = "";
                    this.pivotCellText = "";
                    this.isPxsi = false;
                }
                else if (PXSI_NAMESPACE.Equals(element.Ns) && "val".Equals(element.Name))
                {
                    this.isInPivotCellVal = false;
                    //this is a child element of <pxsi:pivotCell> so do not modify isPxsi variable
                    //pivotCellVal variable is handled by <pxsi:pivotCell> end element method
                }
                else if (PXSI_NAMESPACE.Equals(element.Ns) && "text".Equals(element.Name))
                {
                    this.isInPivotCellText = false;
                    //this is a child element of <pxsi:pivotCell> so do not modify isPxsi variable
                    //pivotCellText variable is handled by <pxsi:pivotCell> end element method
                }
                else if (PXSI_NAMESPACE.Equals(element.Ns) && "pivotFields".Equals(element.Name))
                {
                    int nameNum;

                    string[] names = pivotNames.Split('~');
                    string[] axes = pivotAxes.Split('~');
                    string[] sort = pivotSort.Split('~');
                    string[] hide = pivotHide.Split('~');
                    string[] subtotals = pivotSubtotals.Split('~');
                    string[] blanks = pivotBlanks.Split('~');
                    string[] outlines = pivotOutlines.Split('~');
                    string[] emptyLines = pivotEmptyLines.Split('~');

                    //Console.WriteLine("names: " + pivotNames);

                    //for each cache column output <pivotField> element
                    for (int col = 0; col < fieldNamesText[1].Count; col++)
                    {
                        //get field name
                        string name = fieldNamesText[1][col].ToString();
                        //Console.WriteLine("Column: " + name);

                        //search position of field with this name inside <table:data-pilot-table>
                        //field with empty name is not counted
                        //if field with the same name occurs more than once, then get first occurence position
                        nameNum = -1;
                        for (int i = 0; i < names.Length; i++)
                        {
                            if (name.Equals(names[i]))
                            {
                                nameNum = i;
                                break;
                            }
                        }

                        this.nextWriter.WriteStartElement("pivotField", EXCEL_NAMESPACE);

                        //default attribute for not used fields
                        if (nameNum == -1)
                        {
                            this.nextWriter.WriteStartAttribute("showAll");
                            this.nextWriter.WriteString("0");
                            this.nextWriter.WriteEndAttribute();
                        }

                        //if field is in use
                        if (nameNum != -1)
                        {

                            //show empty fields
                            if ("false".Equals(blanks[nameNum]))
                            {
                                this.nextWriter.WriteStartAttribute("showAll");
                                this.nextWriter.WriteString("0");
                                this.nextWriter.WriteEndAttribute();
                            }

                            if ("0".Equals(outlines[nameNum]))
                            {
                                this.nextWriter.WriteStartAttribute("outline");
                                this.nextWriter.WriteString("0");
                                this.nextWriter.WriteEndAttribute();
                            }
                            else if ("bottom".Equals(outlines[nameNum]))
                            {
                                this.nextWriter.WriteStartAttribute("subtotalTop");
                                this.nextWriter.WriteString("0");
                                this.nextWriter.WriteEndAttribute();
                            }

                            if ("1".Equals(emptyLines[nameNum]))
                            {
                                this.nextWriter.WriteStartAttribute("insertBlankRow");
                                this.nextWriter.WriteString("1");
                                this.nextWriter.WriteEndAttribute();
                            }

                            //insert field type attributes
                            if (axes[nameNum].Contains("axis"))
                            {
                                this.nextWriter.WriteStartAttribute("axis");
                                //remove ",data" after axis type if occurs
                                this.nextWriter.WriteString(axes[nameNum].Replace(",data", ""));
                                this.nextWriter.WriteEndAttribute();
                            }
                            if (axes[nameNum].Contains("data"))
                            {
                                this.nextWriter.WriteStartAttribute("dataField");
                                this.nextWriter.WriteString("1");
                                this.nextWriter.WriteEndAttribute();
                            }


                            //if field is sorted insert sortType attribute
                            if (sort[nameNum] != "")
                            {
                                this.nextWriter.WriteStartAttribute("sortType");
                                this.nextWriter.WriteString(sort[nameNum]);
                                this.nextWriter.WriteEndAttribute();
                            }


                            //if field has subtotals insert appropriate attributes                        
                            if (subtotals[nameNum] != "")
                            {
                                string[] totals = subtotals[nameNum].Split(';');

                                for (int total = 0; total < totals.Length; total++)
                                {
                                    this.nextWriter.WriteStartAttribute(totals[total] + "Subtotal");
                                    this.nextWriter.WriteString("1");
                                    this.nextWriter.WriteEndAttribute();
                                }
                            }
                            else
                            {
                                this.nextWriter.WriteStartAttribute("defaultSubtotal");
                                this.nextWriter.WriteString("0");
                                this.nextWriter.WriteEndAttribute();
                            }

                            //output items for all fields being used because even data field
                            //has to got listet items when used twice
                            this.nextWriter.WriteStartElement("items", EXCEL_NAMESPACE);
                            OutputItems(name, hide[nameNum], subtotals[nameNum]);
                            this.nextWriter.WriteEndElement();
                        }

                        this.nextWriter.WriteEndElement();//</pivotField>

                    }

                    this.isInPivotFields = false;
                    //do not reset <pxsi:pivotFields> variables as they are being used further
                    this.isPxsi = false;
                }
                else if (PXSI_NAMESPACE.Equals(element.Ns) && "cacheFields".Equals(element.Name))
                {
                    string nameVal;
                    string name;
                    string[] names = pivotNames.Split('~');
                    string[] axes = pivotAxes.Split('~');

                    for (int col = 0; col < fieldNamesText[1].Count; col++)
                    {
                        //get field name
                        //Start of RefNo-1:for localization and error regarding obj ref not set to an instance of obj.
                        //nameVal = fieldNames[1][col].ToString();
                        //name = fieldNamesText[1][col].ToString();
                        nameVal = Convert.ToString(fieldNames[1][col], CultureInfo.InvariantCulture);
                        name = Convert.ToString(fieldNamesText[1][col], CultureInfo.InvariantCulture);
                        //End of RefNo-1

                        this.nextWriter.WriteStartElement("cacheField", EXCEL_NAMESPACE);

                        //insert showAll attribute
                        this.nextWriter.WriteStartAttribute("name");
                        try
                        {
                            //if name is of float type round it's represetation to 9 decimal places
                            //and output it wit a comma as separator
                            double realName = Convert.ToDouble(nameVal, provider);
                            realName = Math.Round(realName, 9);
                            this.nextWriter.WriteString(realName.ToString());
                        }
                        catch
                        {
                            this.nextWriter.WriteString(name);
                        }

                        this.nextWriter.WriteEndAttribute();

                        this.nextWriter.WriteStartElement("sharedItems", EXCEL_NAMESPACE);
                        InsertSharedItemAttributes(col);

                        //insert "count" attribute
                        this.nextWriter.WriteStartAttribute("count");
                        this.nextWriter.WriteString((string)this.fieldItems[col, 0].Count.ToString());
                        this.nextWriter.WriteEndAttribute();

                        for (int i = 0; i < this.fieldItems[col, 1].Count; i++)
                        {
                            // <e> - error, <d> - date and <b> - bool not handled for now
                            try
                            {
                                //if field value is a number
                                Convert.ToDouble(this.fieldItems[col, 1][i].ToString(), provider);
                                this.nextWriter.WriteStartElement("n", EXCEL_NAMESPACE);
                                this.nextWriter.WriteStartAttribute("v");
                                this.nextWriter.WriteString(this.fieldItems[col, 1][i].ToString());
                                this.nextWriter.WriteEndAttribute();
                            }
                            catch
                            {
                                //if field value is a string
                                if (this.fieldItems[col, 1][i].ToString().Length == 0)
                                    //blank value
                                    this.nextWriter.WriteStartElement("m", EXCEL_NAMESPACE);
                                else
                                {
                                    this.nextWriter.WriteStartElement("s", EXCEL_NAMESPACE);
                                    this.nextWriter.WriteStartAttribute("v");
                                    this.nextWriter.WriteString(this.fieldItems[col, 1][i].ToString());
                                    this.nextWriter.WriteEndAttribute();
                                }
                            }

                            this.nextWriter.WriteEndElement(); //item
                        }
                        this.nextWriter.WriteEndElement();//</sharedItems>
                        this.nextWriter.WriteEndElement();//</cacheField>

                    }
                    this.isInCacheFields = false;
                    this.isPxsi = false;
                }
                else if (PXSI_NAMESPACE.Equals(element.Ns) && "fieldPos".Equals(element.Name))
                {
                    //process only if name of pivot table field is the same as its cell value
                    if (fieldNamesText[0][this.fieldPosName] != null)
                    {
                        //Start of RefNo-1:for localization and error regarding obj ref not set to an instance of obj.
                        //string fieldNum = fieldNamesText[0][this.fieldPosName].ToString();
                        string fieldNum = Convert.ToString(fieldNamesText[0][this.fieldPosName], CultureInfo.InvariantCulture);
                        //End of RefNo-1

                        this.nextWriter.WriteStartAttribute(this.fieldPosAttribute);
                        this.nextWriter.WriteString(fieldNum);
                        this.nextWriter.WriteEndAttribute();
                    }
                    this.fieldPosName = "";
                    this.fieldPosAttribute = "";
                    this.isInFieldPos = false;
                    this.isPxsi = false;

                }
                else if (PXSI_NAMESPACE.Equals(element.Ns) && "cacheRecords".Equals(element.Name))
                {
                    string value;

                    int rows = Convert.ToInt32(cacheRowEnd) - Convert.ToInt32(cacheRowStart) + 1;
                    int cols = Convert.ToInt32(cacheColEnd) - Convert.ToInt32(cacheColStart) + 1;

                    for (int row = 0; row < rows; row++)
                    {
                        this.nextWriter.WriteStartElement("r", EXCEL_NAMESPACE);
                        for (int col = 0; col < cols; col++)
                        {
                            try
                            {
                                //if this is number value
                                Convert.ToDouble(pivotTable[row, col], provider);
                                this.nextWriter.WriteStartElement("n", EXCEL_NAMESPACE);
                                this.nextWriter.WriteStartAttribute("v");
                                this.nextWriter.WriteString(pivotTable[row, col]);
                                this.nextWriter.WriteEndAttribute();
                                this.nextWriter.WriteEndElement();
                            }
                            catch
                            {
                                //if this is string value search for case insensitive field Item position
                                this.nextWriter.WriteStartElement("x", EXCEL_NAMESPACE);
                                this.nextWriter.WriteStartAttribute("v");
                                value = pivotTable[row, col].ToString().ToLower();
                                this.nextWriter.WriteString(this.fieldItemsLowerCaseText[col, 0][value].ToString());
                                this.nextWriter.WriteEndAttribute();
                                this.nextWriter.WriteEndElement();
                            }
                        }
                        this.nextWriter.WriteEndElement();
                    }
                    this.isInCacheRecords = false;
                    this.isPxsi = false;

                }
                else if (PXSI_NAMESPACE.Equals(element.Ns) && "pageItem".Equals(element.Name))
                {
                    int fieldNum = Convert.ToInt32(fieldNamesText[0][pageField]);
                    this.nextWriter.WriteStartAttribute("item");
                    //output value increased by 1
                    this.nextWriter.WriteString((Convert.ToInt32(fieldItems[fieldNum, 0][pageItem])).ToString());
                    this.nextWriter.WriteEndAttribute();

                    this.isInPageItem = false;
                    this.pageField = "";
                    this.pageItem = "";
                    this.isPxsi = false;
                }
                else if (PXSI_NAMESPACE.Equals(element.Ns) && "filterType".Equals(element.Name))
                {

                    int fieldNum = Convert.ToInt32(this.filterField);
                    string name = fieldNamesText[1][fieldNum].ToString();

                    string[] names = pivotNames.Split('~');
                    string[] axes = pivotAxes.Split('~');

                    int nameNum = -1;
                    for (int i = 0; i < names.Length; i++)
                    {
                        if (name.Equals(names[i]))
                        {
                            nameNum = i;
                            break;
                        }
                    }

                    //filter type attribute
                    this.nextWriter.WriteStartAttribute("type");
                    if (axes[nameNum].Contains("axis"))
                        this.nextWriter.WriteString("caption" + this.condition);
                    else
                        this.nextWriter.WriteString("value" + this.condition);
                    this.nextWriter.WriteEndAttribute();

                    //field type dependant attribute
                    if (axes[nameNum].Contains("axis"))
                    {
                        this.nextWriter.WriteStartAttribute("stringValue1");
                        this.nextWriter.WriteString(filterValue);
                        this.nextWriter.WriteEndAttribute();
                    }
                    else
                    {
                        this.nextWriter.WriteStartAttribute("iMeasureFld");
                        this.nextWriter.WriteString("0");
                        this.nextWriter.WriteEndAttribute();
                    }

                    this.isInFilterType = false;
                    this.filterField = "";
                    this.condition = "";
                    this.filterValue = "";
                    this.isPxsi = false;
                }
                //else
                //{
                //    this.nextWriter.WriteEndElement();
                //}
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }
        }

        private void CreatePivotTable(int sheetNum, int rowStart, int rowEnd, int colStart, int colEnd)
        {

            int rows = rowEnd - rowStart + 1;
            int cols = colEnd - colStart + 1;
            int[] index = new int[cols];

            this.fieldNames = new Hashtable[2];
            this.fieldNamesText = new Hashtable[2];
            this.fieldItems = new Hashtable[cols, 2];
            this.fieldItemsText = new Hashtable[cols, 2];
            this.fieldItemsLowerCaseText = new Hashtable[cols, 2];

            //initialize Hashtables
            for (int i = 0; i < 2; i++)
            {
                fieldNames[i] = new Hashtable();
                fieldNamesText[i] = new Hashtable();

                for (int field = 0; field < cols; field++)
                {
                    fieldItems[field, i] = new Hashtable();
                    fieldItemsText[field, i] = new Hashtable();
                    fieldItemsLowerCaseText[field, i] = new Hashtable();
                }
            }

            //fill fieldNamesText (from cells text value)
            for (int col = 0; col < cols; col++)
            {
                int keyCol = Convert.ToInt32(colStart) + col;
                int keyRow = Convert.ToInt32(rowStart) - 1;

                string key = sheetNum + "#" + keyCol.ToString() + ":" + keyRow.ToString();
                string modifiedFieldName = null;
                if (pivotCellsText.ContainsKey(key))
                {
                    if (!fieldNamesText[0].ContainsKey((string)pivotCellsText[key]))
                    {
                        fieldNames[0].Add((string)pivotCells[key], col);
                        fieldNames[1].Add(col, (string)pivotCells[key]);

                        fieldNamesText[0].Add((string)pivotCellsText[key], col);
                        fieldNamesText[1].Add(col, (string)pivotCellsText[key]);

                    }
                    //Field with the same name occurs more than once, then increment second occurence of the field
                    //like Year to Year2...
                    else if (fieldNamesText[0].ContainsKey((string)pivotCellsText[key]))
                    {
                        if (fieldNamesText[0].ContainsKey((string)pivotCellsText[key]))
                        {
                            int i = 2, j = 1;
                            modifiedFieldName = string.Concat((string)pivotCells[key], i);
                        myLabel:
                            if (fieldNamesText[0].Contains((string)modifiedFieldName))
                            {
                                j = j++;
                                modifiedFieldName = string.Concat((string)pivotCells[key], i + j);
                                i = i + j;
                                if (fieldNamesText[0].Contains((string)modifiedFieldName))
                                    goto myLabel;
                            }
                        }
                        fieldNames[0].Add(modifiedFieldName, col);
                        fieldNames[1].Add(col, modifiedFieldName);

                        fieldNamesText[0].Add(modifiedFieldName, col);
                        fieldNamesText[1].Add(col, modifiedFieldName);
                    }
                    //End
                }
            }

            //fill pivotTable and fieldItems (from cells value)
            for (int row = 0; row < rows; row++)
                //the number of fields matches number of non-empty cells in the first row of the source
                //if source range is wider then columns starting with an empty cell are not treated as a field
                for (int col = 0; col < fieldNames[0].Count; col++)
                {
                    int keyCol = Convert.ToInt32(colStart) + col;
                    int keyRow = Convert.ToInt32(rowStart) + row;

                    string key = sheetNum + "#" + keyCol.ToString() + ":" + keyRow.ToString();

                    if (pivotCells.ContainsKey(key))
                    {
                        this.pivotTable[row, col] = (string)pivotCells[key];
                        this.pivotTableText[row, col] = (string)pivotCellsText[key];

                        //Search for items based on cell text (Known Issue: [ 1803689 ] Duplicated field items in Calc pilot tables)
                        //fieldItems are case insensitive so compare lower-cased text
                        if (!fieldItemsLowerCaseText[col, 0].ContainsKey((string)pivotCellsText[key].ToString().ToLower()))
                        {
                            //Console.WriteLine("fielditem-" + (string)pivotCells[key]);
                            fieldItems[col, 0].Add((string)pivotCells[key], index[col]);
                            fieldItems[col, 1].Add(index[col], (string)pivotCells[key]);

                            fieldItemsText[col, 0].Add((string)pivotCellsText[key], index[col]);
                            fieldItemsText[col, 1].Add(index[col], (string)pivotCellsText[key]);

                            fieldItemsLowerCaseText[col, 0].Add((string)pivotCellsText[key].ToString().ToLower(), index[col]);
                            fieldItemsLowerCaseText[col, 1].Add(index[col], (string)pivotCellsText[key].ToString().ToLower());

                            index[col]++;
                        }
                    }
                    else
                    {
                        this.pivotTable[row, col] = "";
                        this.pivotTableText[row, col] = "";

                        if (!fieldItems[col, 0].ContainsKey(""))
                        {
                            //fieldItems[col].Add("", index[col]);
                            fieldItems[col, 0].Add("", index[col]);
                            fieldItems[col, 1].Add(index[col], "");

                            fieldItemsText[col, 0].Add("", index[col]);
                            fieldItemsText[col, 1].Add(index[col], "");

                            fieldItemsLowerCaseText[col, 0].Add("", index[col]);
                            fieldItemsLowerCaseText[col, 1].Add(index[col], "");
                            index[col]++;
                        }
                    }
                }
        }

        private void InsertSharedItemAttributes(int field)
        {
            bool containsDouble = false;

            //attributes
            bool containsBlank = false;
            //bool containsDate;        //not checked for now but it is in specification
            bool containsInteger = false;
            //bool containsNonDate;     //can be omitted without loss of functionality
            bool containsNumber = false;
            bool containsString = false;
            bool longText = false;      //not checked for now but it is in specification
            //double minValue;
            //double maxValue;
            //minDate                   //not checked for now but it is in specification
            //maxDate                   //not checked for now but it is in specification

            //analize items
            for (int i = 0; i < this.fieldItems[field, 1].Count; i++)
            {
                //check weather an item is an integer
                try
                {
                    Convert.ToDouble(this.fieldItems[field, 1][i].ToString(), provider);
                    if (this.fieldItems[field, 1][i].ToString().Contains("."))
                    {
                        containsNumber = true;
                        containsDouble = true;

                    }

                    //check weather an item is an integer
                    try
                    {
                        Convert.ToInt32(this.fieldItems[field, 1][i]);
                        containsNumber = true;
                        containsInteger = true;
                    }
                    catch
                    {
                    }
                }
                catch
                {
                    string text = this.fieldItems[field, 1][i].ToString();

                    if (text.Length == 0)
                    {
                        containsBlank = true;
                    }
                    else if (text.Length > 255)
                        longText = true;
                    else
                        containsString = true;
                }
            }

            if (containsBlank)
            {
                this.nextWriter.WriteStartAttribute("containsBlank");
                this.nextWriter.WriteString("1");
                this.nextWriter.WriteEndAttribute();
            }
            if (containsInteger && !containsDouble)
            {
                this.nextWriter.WriteStartAttribute("containsInteger");
                this.nextWriter.WriteString("1");
                this.nextWriter.WriteEndAttribute();
            }
            if (containsNumber)
            {
                this.nextWriter.WriteStartAttribute("containsNumber");
                this.nextWriter.WriteString("1");
                this.nextWriter.WriteEndAttribute();
            }
            if (containsNumber && containsString)
            {
                this.nextWriter.WriteStartAttribute("containsMixedTypes");
                this.nextWriter.WriteString("1");
                this.nextWriter.WriteEndAttribute();
            }
            if (containsNumber && !containsString && !containsBlank)
            {
                this.nextWriter.WriteStartAttribute("containsSemiMixedTypes");
                this.nextWriter.WriteString("0");
                this.nextWriter.WriteEndAttribute();
            }
            if (!containsString)
            {
                this.nextWriter.WriteStartAttribute("containsString");
                this.nextWriter.WriteString("0");
                this.nextWriter.WriteEndAttribute();
            }
        }

        private void OutputItems(string name, string hide, string subtotals)
        {
            int fieldNum = Convert.ToInt32(fieldNamesText[0][name]);

            int items = fieldItems[fieldNum, 0].Count;

            this.nextWriter.WriteStartAttribute("count");
            this.nextWriter.WriteString((items + 1).ToString());
            this.nextWriter.WriteEndAttribute();

            Hashtable[] numbers = new Hashtable[2];
            Hashtable strings = new Hashtable();

            for (int i = 0; i < 2; i++)
                numbers[i] = new Hashtable();

            bool isBlank = false;
            foreach (string key in fieldItems[fieldNum, 0].Keys)
            {

                try
                {
                    //numbers with high precision can be rounded during conversion to double and back to string!!! (i.e 18.399999999999999 -> 18,4  )
                    Convert.ToDouble(key, provider);
                    numbers[0].Add(key, key);
                    numbers[1].Add(Convert.ToDouble(key, provider).ToString(), key);
                }
                catch
                {
                    if (key.Length == 0)
                        isBlank = true;
                    else
                        strings.Add(key, key);
                }

            }

            double[] sortedNumbers = new double[numbers[0].Count];
            string[] sortedStrings = new string[strings.Count];

            //sort numbers and strings
            int count = 0;
            foreach (string key in numbers[0].Keys)
            {
                sortedNumbers[count] = Convert.ToDouble(key, provider);
                count++;
            }
            Array.Sort(sortedNumbers);

            count = 0;
            foreach (string key in strings.Keys)
            {
                sortedStrings[count] = key;
                count++;
            }
            Array.Sort(sortedStrings);

            //output sorted item types in order: numbers, strings, blank
            int itemNum;
            string itemText;

            //Console.WriteLine("ITEMS");
            //Console.WriteLine("name: " + name);
            for (int i = 0; i < sortedNumbers.Length; i++)
            {
                double sortedNumber = Convert.ToDouble(sortedNumbers[i]);

                //sorted number can be rounded during conversion to double and back to string!!! (i.e 18.399999999999999 -> 18,4  )
                //so take string value from number[1] hashtable based on sortedNumber
                string number = numbers[1][sortedNumber.ToString()].ToString();

                //find number of this item and take textItem under this number
                itemNum = Convert.ToInt32(fieldItems[fieldNum, 0][number]);
                itemText = fieldItemsText[fieldNum, 1][itemNum].ToString();

                //check if this item is hidden (Calc uses items text, Excel items value [])
                bool hidden = false;
                if (hide.Contains(";" + itemText + ";"))
                    hidden = true;

                OutputItem(name, itemNum, hidden);
            }

            for (int i = 0; i < sortedStrings.Length; i++)
            {
                itemNum = Convert.ToInt32(fieldItems[fieldNum, 0][sortedStrings[i].ToString()]);

                //check if this item is hidden
                bool hidden = false;
                if (hide.Contains(";" + sortedStrings[i] + ";"))
                    hidden = true;

                OutputItem(name, itemNum, hidden);
            }

            if (isBlank)
            {
                itemNum = Convert.ToInt32(fieldItems[fieldNum, 0][""]);

                //check if this item is hidden
                bool hidden = false;
                if (hide.Contains(";;"))
                    hidden = true;

                OutputItem(name, itemNum, hidden);
            }

            //output subtotal items
            if (!"".Equals(subtotals))
            {
                string[] subItems = subtotals.Split(';');

                for (int item = 0; item < subItems.Length; item++)
                {
                    this.nextWriter.WriteStartElement("item", EXCEL_NAMESPACE);
                    this.nextWriter.WriteStartAttribute("t");
                    this.nextWriter.WriteString(subItems[item]);
                    this.nextWriter.WriteEndAttribute();
                    this.nextWriter.WriteEndElement();
                }
            }
        }

        private void OutputItem(string name, int itemNum, bool hide)
        {
            //Console.WriteLine("name: " + name + " item: " + itemNum);
            int fieldNum = Convert.ToInt32(fieldNamesText[0][name]);

            this.nextWriter.WriteStartElement("item", EXCEL_NAMESPACE);

            //if this item is hidden
            if (hide)
            {
                this.nextWriter.WriteStartAttribute("h");
                this.nextWriter.WriteString("1");
                this.nextWriter.WriteEndAttribute();
            }

            this.nextWriter.WriteStartAttribute("x");
            this.nextWriter.WriteString(itemNum.ToString());
            this.nextWriter.WriteEndAttribute();
            this.nextWriter.WriteEndElement();
        }

    }
}
