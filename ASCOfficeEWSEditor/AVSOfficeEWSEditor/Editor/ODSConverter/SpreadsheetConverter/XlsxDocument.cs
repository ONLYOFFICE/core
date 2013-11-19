using System;
using System.Collections.Generic;
using System.Text;
using CleverAge.OdfConverter.OdfConverterLib;
using System.Xml;
using CleverAge.OdfConverter.OdfZipUtils;
using System.Diagnostics;
using System.Collections;
using System.Text.RegularExpressions;

namespace CleverAge.OdfConverter.Spreadsheet
{
    class XlsxDocument : OoxDocument
    {
        protected int _partId = 0;

        private const string SPREADSHEET_ML_NS = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
        private Hashtable styleCellNumber;
        private Hashtable intConditionalWithStyle;

        public XlsxDocument(string fileName)
            : base(fileName)
        {
        }

        protected override List<RelationShip> CopyPart(XmlReader xtr, XmlTextWriter xtw, string ns, string partName, ZipReader archive)
        {
            bool isInRel = false;
            bool extractRels = ns.Equals(RELATIONSHIP_NS);
            bool isCell = false;
            List<RelationShip> rels = new List<RelationShip>();

            int idSheet = 0;
            int idRow = 1;
            int idFont = 0;
            int idFill = 0;
            int idBorder = 0;
            int idXf = 0;
            int idCellStyle = 0;
            int idDxf = 0;
            int idSi = 0;
            int idCf = 0;

            int RowNumber = 0;
            int PrevRowNumber = 0;

            int ColNumber = 0;
            int PrevColNumber = 0;

            String ConditionalCell = "";
            String ConditionalCellRow = "";
            List<int> ConditionalCellRowList = new List<int>();
            Boolean CheckIfBigConditional = new Boolean();
            intConditionalWithStyle = new Hashtable();
            string s = "";
            int ConditionalStyle = -1;

            XmlReader SearchConditionalReader;

            RelationShip rel = new RelationShip();

            if (!(partName.Contains(".vml")))
            {
                while (xtr.Read())
                {
                    switch (xtr.NodeType)
                    {
                        case XmlNodeType.Attribute:
                            break;
                        case XmlNodeType.CDATA:
                            xtw.WriteCData(xtr.Value);
                            break;
                        case XmlNodeType.Comment:
                            xtw.WriteComment(xtr.Value);
                            break;
                        case XmlNodeType.DocumentType:
                            xtw.WriteDocType(xtr.Name, null, null, null);
                            break;
                        case XmlNodeType.Element:
                            if (String.Compare(xtr.LocalName, "sheetData") == 0)
                            {
                                SearchConditionalReader = XmlReader.Create(archive.GetEntry(partName));
                                ConditionalCell = Conditional(SearchConditionalReader, ns, partName);
                                CheckIfBigConditional = CheckIfBigConditionalRow(ConditionalCell);

                                if (!CheckIfBigConditional)
                                {
                                    ConditionalCellRowList = ConditionalCellRowNumberList(ConditionalCell);
                                }

                                ConditionalCellRowList.Sort();

                            }

                            if (extractRels && xtr.LocalName == "Relationship" && xtr.NamespaceURI == RELATIONSHIP_NS)
                            {
                                isInRel = true;
                                rel = new RelationShip();
                            }

                            if (xtr.LocalName.Equals("row") && (ConditionalCell != ""))
                            {

                                if (xtr.HasAttributes)
                                {
                                    while (xtr.MoveToNextAttribute())
                                    {
                                        if (xtr.LocalName.Equals("r"))
                                        {
                                            RowNumber = System.Int32.Parse(xtr.Value.ToString());
                                        }
                                    }

                                    InsertEmptyRowsWithConditional(ConditionalCellRowList, ConditionalCell, xtw, RowNumber, PrevRowNumber, CheckIfBigConditional, false);
                                    xtr.MoveToElement();

                                    xtw.WriteStartElement(xtr.Prefix, xtr.LocalName, xtr.NamespaceURI);

                                    if (xtr.HasAttributes)
                                    {
                                        while (xtr.MoveToNextAttribute())
                                        {
                                            xtw.WriteAttributeString(xtr.Prefix, xtr.LocalName, xtr.NamespaceURI, xtr.Value);
                                        }
                                    }

                                    xtr.MoveToElement();

                                }
                                PrevRowNumber = RowNumber;
                            }
                            else
                            {

                                isCell = xtr.LocalName.Equals("c") && xtr.NamespaceURI.Equals(SPREADSHEET_ML_NS);

                                if (xtr.HasAttributes && isCell && ConditionalCell != "" && ConditionalCellRowList.Contains(RowNumber))
                                {
                                    while (xtr.MoveToNextAttribute())
                                    {
                                        if (xtr.LocalName == "r")
                                        {
                                            ColNumber = GetColId(xtr.Value);
                                            InsertCellInRow(RowNumber, ConditionalCell, xtw, PrevColNumber, ColNumber);
                                            PrevColNumber = ColNumber;
                                        }

                                    }
                                    xtr.MoveToElement();
                                }

                                xtw.WriteStartElement(xtr.Prefix, xtr.LocalName, xtr.NamespaceURI);

                                if (xtr.HasAttributes)
                                {
                                    while (xtr.MoveToNextAttribute())
                                    {
                                        if (extractRels && isInRel)
                                        {
                                            if (xtr.LocalName == "Type")
                                                rel.Type = xtr.Value;
                                            else if (xtr.LocalName == "Target")
                                                rel.Target = xtr.Value;
                                            else if (xtr.LocalName == "Id")
                                                rel.Id = xtr.Value;

                                        }

                                        string value = xtr.Value;
                                        // normalize type ST_OnOff
                                        if (value == "on" || value == "true")
                                            value = "1";
                                        else if (value == "off" || value == "false")
                                            value = "0";

                                        xtw.WriteAttributeString(xtr.Prefix, xtr.LocalName, xtr.NamespaceURI, value);

                                        switch (xtr.LocalName)
                                        {
                                            case "r":
                                                if (isCell)
                                                {
                                                    string coord = GetColId(value).ToString(System.Globalization.CultureInfo.InvariantCulture)
                                                        + "|"
                                                        + GetRowId(value).ToString(System.Globalization.CultureInfo.InvariantCulture);

                                                    xtw.WriteAttributeString(NS_PREFIX, "p", PACKAGE_NS, coord);
                                                    if (ConditionalCell != "" && ConditionalCellRowList.Contains(GetRowId(value)))
                                                    {
                                                        ConditionalStyle = CheckIfConditional(GetRowId(value), GetColId(value), ConditionalCell);
                                                        if (ConditionalStyle != -1)
                                                        {
                                                            xtw.WriteAttributeString(NS_PREFIX, "ConditionalStyle", PACKAGE_NS, ConditionalStyle.ToString());
                                                        }

                                                    }
                                                    //xtw.WriteAttributeString(NS_PREFIX, "c", PACKAGE_NS, GetColId(value).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                                    //xtw.WriteAttributeString(NS_PREFIX, "r", PACKAGE_NS, GetRowId(value).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                                }
                                                break;
                                            case "s":
                                                s = xtr.Value;
                                                break;
                                        }

                                    }


                                    if (ConditionalStyle > -1)
                                    {

                                        if (intConditionalWithStyle.Contains(ConditionalStyle))
                                        {
                                            String OldValue = Convert.ToString(intConditionalWithStyle[ConditionalStyle]);

                                            if (!("|" + OldValue).Contains("|" + s + "|"))
                                            {
                                                intConditionalWithStyle.Remove(ConditionalStyle);
                                                intConditionalWithStyle.Add(ConditionalStyle, (OldValue + s + "|"));

                                            }
                                        }
                                        else
                                        {
                                            intConditionalWithStyle.Add(ConditionalStyle, (s + "|"));
                                        }
                                    }

                                    ConditionalStyle = -1;

                                    xtr.MoveToElement();

                                }

                            }

                            if (isInRel)
                            {
                                isInRel = false;
                                rels.Add(rel);
                            }

                            switch (xtr.LocalName)
                            {
                                // reset id counters
                                case "sheets": idSheet = 0; break;
                                case "sheetData": idRow = 1; break;
                                case "fonts": idFont = 0; break;
                                case "fills": idFill = 0; break;
                                case "borders": idBorder = 0; break;
                                case "cellXfs":
                                case "cellStyleXfs":
                                    idXf = 0;
                                    break;

                                case "cellStyles": idCellStyle = 0; break;
                                case "dxfs": idDxf = 0; break;
                                case "sst": idSi = 0; break;

                                // add id values
                                case "sheet":
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idSheet++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "row":
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idRow++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "font":
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idFont++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "fill":
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idFill++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "border":
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idBorder++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "xf":
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idXf++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "cellStyle":
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idCellStyle++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "dxf":
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idDxf++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "si": // sharedStrings
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idSi++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "worksheet":
                                case "chartSpace":
                                    xtw.WriteAttributeString(NS_PREFIX, "part", PACKAGE_NS, _partId.ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;

                                case "conditionalFormatting":
                                    xtw.WriteAttributeString(NS_PREFIX, "part", PACKAGE_NS, _partId.ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    xtw.WriteAttributeString(NS_PREFIX, "id", PACKAGE_NS, (idCf++).ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    if (intConditionalWithStyle.Contains(idCf - 1))
                                    {
                                        xtw.WriteAttributeString(NS_PREFIX, "ConditionalInheritance", PACKAGE_NS, Convert.ToString(intConditionalWithStyle[idCf - 1]));
                                    }
                                    break;

                                case "col":
                                case "sheetFormatPr":
                                case "mergeCell":
                                case "drawing":
                                case "hyperlink":

                                case "ser":
                                case "val":
                                case "xVal":
                                case "cat":
                                case "plotArea":
                                case "grouping":
                                case "spPr":
                                case "errBars":
                                    xtw.WriteAttributeString(NS_PREFIX, "part", PACKAGE_NS, _partId.ToString(System.Globalization.CultureInfo.InvariantCulture));
                                    break;
                            }

                            if (xtr.IsEmptyElement)
                            {
                                if (ConditionalCell != "" && string.Compare(xtr.LocalName.ToString(), "sheetData") == 0)
                                {
                                    InsertEmptyRowsWithConditional(ConditionalCellRowList, ConditionalCell, xtw, 65537, 0, CheckIfBigConditional, false);
                                }

                                xtw.WriteEndElement();
                            }
                            break;
                        case XmlNodeType.EndElement:
                            if (string.Compare(xtr.LocalName.ToString(), "sheetData") == 0 && (ConditionalCell != ""))
                            {

                                InsertEmptyRowsWithConditional(ConditionalCellRowList, ConditionalCell, xtw, 65537, PrevRowNumber, CheckIfBigConditional, true);

                            }
                            if (string.Compare(xtr.LocalName.ToString(), "row") == 0 && (ConditionalCell != ""))
                            {
                                InsertCellInRow(RowNumber, ConditionalCell, xtw, PrevColNumber, 257);

                            }

                            xtw.WriteEndElement();
                            break;
                        case XmlNodeType.EntityReference:
                            xtw.WriteEntityRef(xtr.Name);
                            break;
                        case XmlNodeType.ProcessingInstruction:
                            xtw.WriteProcessingInstruction(xtr.Name, xtr.Value);
                            break;
                        case XmlNodeType.SignificantWhitespace:
                            xtw.WriteWhitespace(xtr.Value);
                            break;
                        case XmlNodeType.Text:
                            xtw.WriteString(xtr.Value);
                            break;
                        case XmlNodeType.Whitespace:
                            xtw.WriteWhitespace(xtr.Value);
                            break;
                        case XmlNodeType.XmlDeclaration:
                            // omit XML declaration
                            break;
                        default:
                            Debug.Assert(false);
                            break;
                    }
                }
            }

            _partId++;

            return rels;
        }

        protected override List<string> Namespaces
        {
            get
            {
                // define the namespaces that are to be copied
                List<string> namespaces = new List<string>();
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties");
                namespaces.Add("http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/custom-properties");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/calcChain");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLink");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartsheet");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotTable");
                namespaces.Add("http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition");
                namespaces.Add("http://schemas.openxmlformats.org/package/2006/content-types");
                namespaces.Add("http://schemas.openxmlformats.org/spreadsheetml/2006/main");

                return namespaces;
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

        public string NumbersToChars(int num)
        {
            string letter1 = "";
            num--;
            if (num > 25)
            {
                int val2 = ((int)Math.Floor(num / 26.0));
                letter1 = NumbersToChars(val2);
                num = num - val2 * 26;
            }

            return "" + letter1 + "" + ((char)(num + 65)) + "";
        }

        protected string Conditional(XmlReader xtr, string ns, string partName)
        {
            bool isInRel = false;
            bool extractRels = ns.Equals(RELATIONSHIP_NS);
            bool isCell = false;
            List<RelationShip> rels = new List<RelationShip>();

            int idFont = 0;
            int idFill = 0;
            int idBorder = 0;
            int idXf = 0;
            int idCellStyle = 0;
            int idDxf = 0;
            int idSi = 0;
            int idCf = 0;
            String ConditionalCellSheet = "";
            int idConditional = 0;

            bool isOffice14ConditionalFormatting = false;

            RelationShip rel = new RelationShip();

            int sqrefExist = 0;

            while (xtr.Read())
            {
                switch (xtr.NodeType)
                {
                    case XmlNodeType.Attribute:
                        break;
                    case XmlNodeType.CDATA:
                        break;
                    case XmlNodeType.Comment:
                        break;
                    case XmlNodeType.DocumentType:
                        break;
                    case XmlNodeType.Element:

                        if (String.Compare(xtr.LocalName, "conditionalFormatting") == 0)
                        {
                            if (String.Compare(xtr.Name, "x14:conditionalFormatting") == 0)
                                isOffice14ConditionalFormatting = true;
                            if (!isOffice14ConditionalFormatting)
                            {
                                if (xtr.HasAttributes)
                                {
                                    while (xtr.MoveToNextAttribute())
                                    {

                                        if (String.Compare(xtr.LocalName, "sqref") == 0)
                                        {

                                            string Cell = ConditionalCellNumber(xtr.Value, idConditional);


                                            if (String.Compare(Cell, "false") != 0)
                                            {
                                                sqrefExist = 1;
                                                ConditionalCellSheet = ConditionalCellSheet + " " + ConditionalCellNumber(xtr.Value, idConditional);
                                            }
                                        }
                                        string value = xtr.Value;
                                    }
                                    idConditional++;
                                }
                            }
                            xtr.MoveToElement();
                        }
                        //Vijayeta, 2010
                        if (isOffice14ConditionalFormatting)
                        {
                            if (isOffice14ConditionalFormatting == true && String.Compare(xtr.Name, "xm:sqref") == 0)
                            {
                                string Cell = ConditionalCellNumber(xtr.Value, idConditional);
                                if (String.Compare(Cell, "false") != 0)
                                {
                                    sqrefExist = 1;
                                    ConditionalCellSheet = ConditionalCellSheet + " " + ConditionalCellNumber(xtr.ReadInnerXml(), idConditional);
                                }
                                string value = xtr.Value;
                                idConditional++;
                            }
                            xtr.MoveToElement();
                        }
                        //Vijayeta, 2010
                        break;
                    case XmlNodeType.EndElement:
                        break;
                    case XmlNodeType.EntityReference:
                        break;
                    case XmlNodeType.ProcessingInstruction:
                        break;
                    case XmlNodeType.SignificantWhitespace:
                        break;
                    case XmlNodeType.Text:
                        break;
                    case XmlNodeType.Whitespace:
                        break;
                    case XmlNodeType.XmlDeclaration:
                        break;
                    default:
                        break;
                }
            }


            return ConditionalCellSheet;
        }

        private string ConditionalCellNumber(string sqref, int idConditional)
        {
            string result = "";
            int RowStart = 0;
            int RowEnd = 0;
            int ColStart = 0;
            int ColEnd = 0;


            foreach (string value in sqref.Split())
            {
                if (value.Contains(":"))
                {
                    ColStart = GetColId(value.Substring(0, value.IndexOf(':')));
                    RowStart = GetRowId(value.Substring(0, value.IndexOf(':')));

                    ColEnd = GetColId(value.Substring(value.IndexOf(':') + 1));
                    RowEnd = GetRowId(value.Substring(value.IndexOf(':') + 1));

                    if (ColStart <= 256 && RowStart <= 65536)
                    {

                        if (ColEnd > 256) ColEnd = 256;
                        if (RowEnd > 65536) RowEnd = 65536;

                        result = result + ColStart + "|" + RowStart + "-" + ColEnd + "|" + RowEnd + " " + idConditional.ToString() + "= ";


                    }
                    else
                    {
                        result = "false";
                    }

                }
                else
                {
                    ColStart = GetColId(value);
                    RowStart = GetRowId(value);

                    if (ColStart <= 256 && RowStart <= 65536)
                        result = result + GetColId(value) + "|" + GetRowId(value) + " " + idConditional.ToString() + "= ";
                    else
                        result = "false";
                }
            }

            return result;
        }

        private string ConditionalCellRowNumber(string ConditionalCell)
        {
            string result = "|";


            foreach (string value in ConditionalCell.Split())
            {
                if (value.Contains("-"))
                {
                    string CellStart = value.Substring(0, value.IndexOf('-'));
                    string CellEnd = value.Substring(value.IndexOf('-') + 1);

                    for (int CellNr = System.Int32.Parse(CellStart.Substring(CellStart.IndexOf('|') + 1)); CellNr <= System.Int32.Parse(CellEnd.Substring(CellEnd.IndexOf('|') + 1)); CellNr++)
                    {
                        if (!result.Contains("|" + CellNr + "|"))
                        {
                            result = result + CellNr + "|";
                        }
                    }
                }
                else if (value.Contains("|"))
                {
                    if (!result.Contains("|" + value.Substring(value.IndexOf('|') + 1) + "|"))
                    {
                        result = result + value.Substring(value.IndexOf('|') + 1) + "|";
                    }
                }
            }

            return result;
        }

        private void InsertEmptyRowsWithConditional(List<int> intListRow, String ConditionalCell, XmlTextWriter xtw, int RowNumber, int PrevRowNumber, Boolean CheckIfBigConditional, Boolean InsertRowWithConditionalAfterLastRow)
        {

            if (CheckIfBigConditional)
            {
                for (int count = PrevRowNumber + 1; count < RowNumber; count++)
                {

                    xtw.WriteStartElement(null, "row", SPREADSHEET_ML_NS);

                    xtw.WriteStartAttribute(null, "r", null);
                    xtw.WriteString(count.ToString());
                    xtw.WriteEndAttribute();

                    if (InsertRowWithConditionalAfterLastRow)
                    {

                        xtw.WriteStartAttribute(null, "oox:ConditionalRepeat", null);
                        xtw.WriteString("continue");
                        xtw.WriteEndAttribute();


                    }

                    InsertCellInRow(count, ConditionalCell, xtw, 0, 256);

                    xtw.WriteEndElement();
                    if (InsertRowWithConditionalAfterLastRow)
                    {
                        break;
                    }

                }
            }
            else
            {

                for (int count = 0; count < intListRow.Count; count++)
                {

                    if (PrevRowNumber < intListRow[count] && intListRow[count] < RowNumber)
                    {
                        xtw.WriteStartElement(null, "row", SPREADSHEET_ML_NS);
                        xtw.WriteStartAttribute(null, "r", null);
                        xtw.WriteString(intListRow[count].ToString());
                        xtw.WriteEndAttribute();

                        if (InsertRowWithConditionalAfterLastRow && (intListRow.Count + PrevRowNumber - 65536) >= 0)
                        {

                            xtw.WriteStartAttribute(null, "oox:ConditionalRepeat", null);
                            xtw.WriteString("continue");
                            xtw.WriteEndAttribute();


                        }


                        InsertCellInRow(intListRow[count], ConditionalCell, xtw, 0, 256);

                        xtw.WriteEndElement();
                        if (InsertRowWithConditionalAfterLastRow && (intListRow.Count + PrevRowNumber - 65536) >= 0)
                        {
                            break;
                        }

                    }
                    else if (RowNumber < intListRow[count])
                    {
                        break;
                    }

                }
            }

        }

        private void InsertCellInRow(int RowNumber, String ConditionalCell, XmlTextWriter xtw, int CollNrStart, int CollNrEnd)
        {

            List<int> intListCol = new List<int>();
            String ConditionalCellStyle = "";
            String Style = "";
            styleCellNumber = new Hashtable();


            foreach (string value in ConditionalCell.Split())
            {
                if (value.Contains("-"))
                {
                    ConditionalCellStyle = ConditionalCell.Substring(ConditionalCell.IndexOf(value) + value.Length);
                    Style = ConditionalCellStyle.Substring(0, ConditionalCellStyle.IndexOf('='));

                    string CellStart = value.Substring(0, value.IndexOf('-'));
                    string CellEnd = value.Substring(value.IndexOf('-') + 1);

                    int RowStart = System.Int32.Parse(CellStart.Substring(CellStart.IndexOf('|') + 1));
                    int ColStart = System.Int32.Parse(CellStart.Substring(0, CellStart.IndexOf('|')));

                    int RowEnd = System.Int32.Parse(CellEnd.Substring(CellEnd.IndexOf('|') + 1));
                    int ColEnd = System.Int32.Parse(CellEnd.Substring(0, CellEnd.IndexOf('|')));

                    if ((RowStart <= RowNumber) && (RowNumber <= RowEnd))
                    {
                        for (int CollNr = ColStart; CollNr <= ColEnd; CollNr++)
                        {
                            if (!intListCol.Contains(CollNr))
                            {
                                if (CollNrStart < CollNr && CollNr < CollNrEnd)
                                {
                                    intListCol.Add(CollNr);
                                    styleCellNumber.Add(CollNr, System.Int32.Parse(Style));
                                }

                            }
                        }
                    }
                }
                else if (value.Contains("|"))
                {

                    ConditionalCellStyle = ConditionalCell.Substring(ConditionalCell.IndexOf(value) + value.Length);
                    Style = ConditionalCellStyle.Substring(0, ConditionalCellStyle.IndexOf('='));

                    int CollNr = System.Int32.Parse(value.Substring(0, value.IndexOf('|')));

                    int Row = System.Int32.Parse(value.Substring(value.IndexOf('|') + 1));

                    if (Row == RowNumber)
                    {

                        if (!intListCol.Contains(CollNr))
                        {
                            if (CollNrStart < CollNr && CollNr < CollNrEnd)
                            {
                                intListCol.Add(CollNr);
                                styleCellNumber.Add(CollNr, System.Int32.Parse(Style));
                            }
                        }

                    }
                }
            }

            intListCol.Sort();

            for (int count = 0; count < intListCol.Count; count++)
            {

                string cell = NumbersToChars(intListCol[count]) + RowNumber.ToString();


                xtw.WriteStartElement(null, "c", null);

                xtw.WriteStartAttribute(null, "r", null);
                xtw.WriteString(cell);
                xtw.WriteEndAttribute();

                string coord = intListCol[count].ToString(System.Globalization.CultureInfo.InvariantCulture)
                             + "|"
                             + RowNumber.ToString(System.Globalization.CultureInfo.InvariantCulture);

                xtw.WriteAttributeString(NS_PREFIX, "p", PACKAGE_NS, coord);

                xtw.WriteAttributeString(NS_PREFIX, "ConditionalStyle", PACKAGE_NS, styleCellNumber[intListCol[count]].ToString());

                xtw.WriteEndElement();

            }

        }


        private List<int> ConditionalCellRowNumberList(string ConditionalCell)
        {
            List<int> result = new List<int>();

            foreach (string value in ConditionalCell.Split())
            {
                if (value.Contains("-"))
                {
                    string CellStart = value.Substring(0, value.IndexOf('-'));
                    string CellEnd = value.Substring(value.IndexOf('-') + 1);

                    int Start = System.Int32.Parse(CellStart.Substring(CellStart.IndexOf('|') + 1));
                    int End = System.Int32.Parse(CellEnd.Substring(CellEnd.IndexOf('|') + 1));

                    if (End > 65537)
                    {
                        End = 65537;
                    }

                    if (Start < 65537)
                    {

                        for (int CellNr = Start; CellNr <= End; CellNr++)
                        {
                            if (!result.Contains(CellNr))
                            {

                                result.Add(CellNr);
                            }
                        }
                    }
                }
                else if (value.Contains("|"))
                {
                    int CellNr = System.Int32.Parse(value.Substring(value.IndexOf('|') + 1));

                    if (!result.Contains(System.Int32.Parse(value.Substring(value.IndexOf('|') + 1))) && CellNr < 65537)
                    {

                        result.Add(System.Int32.Parse(value.Substring(value.IndexOf('|') + 1)));
                    }

                }
            }

            return result;
        }

        private bool CheckIfBigConditionalRow(string ConditionalCell)
        {

            Boolean result = false;

            foreach (string value in ConditionalCell.Split())
            {
                if (value.Contains("-"))
                {
                    string CellStart = value.Substring(0, value.IndexOf('-'));
                    string CellEnd = value.Substring(value.IndexOf('-') + 1);

                    int Start = System.Int32.Parse(CellStart.Substring(CellStart.IndexOf('|') + 1));
                    int End = System.Int32.Parse(CellEnd.Substring(CellEnd.IndexOf('|') + 1));


                    if (Start == 1 && End >= 65536)
                    {
                        result = true;
                        break;
                    }
                }

            }

            return result;
        }

        private int CheckIfConditional(int RowNumber, int ColNumber, String ConditionalCell)
        {

            List<int> intListCol = new List<int>();
            List<int> intListColStyle = new List<int>();
            String ConditionalCellStyle = "";
            String Style = "-1";



            foreach (string value in ConditionalCell.Split())
            {
                if (value.Contains("-"))
                {
                    ConditionalCellStyle = ConditionalCell.Substring(ConditionalCell.IndexOf(value + " ") + value.Length);
                    Style = ConditionalCellStyle.Substring(0, ConditionalCellStyle.IndexOf('='));

                    string CellStart = value.Substring(0, value.IndexOf('-'));
                    string CellEnd = value.Substring(value.IndexOf('-') + 1);

                    int RowStart = System.Int32.Parse(CellStart.Substring(CellStart.IndexOf('|') + 1));
                    int ColStart = System.Int32.Parse(CellStart.Substring(0, CellStart.IndexOf('|')));

                    int RowEnd = System.Int32.Parse(CellEnd.Substring(CellEnd.IndexOf('|') + 1));
                    int ColEnd = System.Int32.Parse(CellEnd.Substring(0, CellEnd.IndexOf('|')));

                    if ((RowStart <= RowNumber) && (RowNumber <= RowEnd) && (ColStart <= ColNumber) && (ColNumber <= ColEnd))
                    {
                        break;
                    }
                    Style = "-1";
                }
                else if (value.Contains("|"))
                {

                    ConditionalCellStyle = ConditionalCell.Substring(ConditionalCell.IndexOf(value + " ") + value.Length);
                    Style = ConditionalCellStyle.Substring(0, ConditionalCellStyle.IndexOf('='));

                    int CollNr = System.Int32.Parse(value.Substring(0, value.IndexOf('|')));

                    int Row = System.Int32.Parse(value.Substring(value.IndexOf('|') + 1));

                    if ((Row == RowNumber) && (CollNr == ColNumber))
                    {
                        break;
                    }

                    Style = "-1";
                }

            }

            return Int32.Parse(Style.ToString());




        }


    }
}