using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor
{
    public struct TableColumn
    {
        public string m_strName;
        public string m_strId;
    }

    public class Table
    {
        internal Table(Worksheet worksheet, OOX.TableFile oTableFile, XmlNode xml_worksheet, XmlNode sheet_data, string rId)
        {
            this.worksheet = worksheet;
            this.ns_manager = worksheet.Workbook.NamespaceManager;

            this.m_oTableFile = oTableFile;
            this.xml_table_file = this.m_oTableFile.GetTableOrCreate();

            this.table_node = xml_worksheet.SelectSingleNode("main:tableParts", this.ns_manager);
            this.rId = rId;

            if (0 < xml_table_file.ChildNodes.Count && "table" == xml_table_file.Name)
            {
                foreach (XmlNode oXmlNode in xml_table_file.ChildNodes)
                {
                    switch (oXmlNode.Name)
                    {
                        case "autoFilter":
                            TableAutoFilter = new AutoFilter(worksheet, xml_table_file, sheet_data);
                            break;
                        case "tableColumns":
                            table_columns_node = oXmlNode;
                            foreach (XmlNode oTableColumnNode in oXmlNode.ChildNodes)
                            {
                                TableColumn oTableColumn = new TableColumn();
                                if (null == oTableColumnNode.Attributes["id"] || null == oTableColumnNode.Attributes["name"])
                                    continue;
                                oTableColumn.m_strId = oTableColumnNode.Attributes["id"].Value;
                                oTableColumn.m_strName = oTableColumnNode.Attributes["name"].Value;

                                this.addTableColumnToMap(oTableColumn);
                            }
                            break;
                        case "tableStyleInfo":
                            XmlAttribute oTableStyle = oXmlNode.Attributes["name"];
                            if (null != oTableStyle)
                                TableStyle = oTableStyle.Value;
                            break;
                    }
                }
            }
        }

        internal Table(Worksheet worksheet, OOX.TableFile oTableFile, XmlNode xml_worksheet, Table oTable)
        {
            this.worksheet = worksheet;
            this.ns_manager = worksheet.Workbook.NamespaceManager;

            this.m_oTableFile = oTableFile;
            this.xml_table_file = this.m_oTableFile.GetTableOrCreate();

            this.rId = m_oTableFile.rId;

            this.TableStyle = oTable.TableStyle;

            this.table_node = xml_worksheet.SelectSingleNode("main:tableParts", this.ns_manager);
            int nCountTables = 0;
            if (null == this.table_node)
            {
                this.table_node = xml_worksheet.OwnerDocument.CreateElement("tableParts", ns_manager.LookupNamespace("main"));
                xml_worksheet.AppendChild(this.table_node);
            }
            else
                nCountTables = System.Convert.ToInt32(this.table_node.Attributes["count"].Value);

            ++nCountTables;

            XmlElement oElem = (XmlElement)this.table_node;
            oElem.SetAttribute("count", nCountTables.ToString());

            XmlElement oElementTablePart = xml_worksheet.OwnerDocument.CreateElement("tablePart", ns_manager.LookupNamespace("main"));
            oElementTablePart.SetAttribute("id", ns_manager.LookupNamespace("mainr"), this.rId);
            this.table_node.AppendChild(oElementTablePart);

            // Write AutoFilter To Xml
            XmlElement oElementTableFile = (XmlElement)this.xml_table_file;
            oElementTableFile.SetAttribute("ref", oTable.TableAutoFilter.m_oRefRange.ToString());
            int nMaxIdFile = MaxIdFile(xml_worksheet) + 1;
            oElementTableFile.SetAttribute("id", nMaxIdFile.ToString());
            oElementTableFile.SetAttribute("name", "Table" + nMaxIdFile.ToString());
            oElementTableFile.SetAttribute("displayName", "Table" + nMaxIdFile.ToString());
            foreach (XmlNode oXmlNodeCopy in oTable.xml_table_file.ChildNodes)
            {
                oElementTableFile.AppendChild(oXmlNodeCopy.Clone());
            }

            TableAutoFilter = new AutoFilter(worksheet, xml_table_file, xml_worksheet.SelectSingleNode("main:sheetData", ns_manager), oTable.TableAutoFilter);
        }

        private Table(Worksheet worksheet, OOX.TableFile oTableFile, XmlNode xml_worksheet, Cells.CellRange oFilterRange)
        {
            this.worksheet = worksheet;
            this.ns_manager = worksheet.Workbook.NamespaceManager;

            this.m_oTableFile = oTableFile;
            this.xml_table_file = this.m_oTableFile.GetTableOrCreate();

            this.rId = m_oTableFile.rId;

            // set default TableStyle   
            this.TableStyle = "TableStyleMedium2";

            this.table_node = xml_worksheet.SelectSingleNode("main:tableParts", this.ns_manager);

            int nCountTables = 0;
            if (null == this.table_node)
            {
                this.table_node = xml_worksheet.OwnerDocument.CreateElement("tableParts", ns_manager.LookupNamespace("main"));
                xml_worksheet.AppendChild(this.table_node);
            }
            else
                nCountTables = System.Convert.ToInt32(this.table_node.Attributes["count"].Value);

            ++nCountTables;

            XmlElement oElem = (XmlElement)this.table_node;
            oElem.SetAttribute("count", nCountTables.ToString());

            XmlElement oElementTablePart = xml_worksheet.OwnerDocument.CreateElement("tablePart", ns_manager.LookupNamespace("main"));
            oElementTablePart.SetAttribute("id", ns_manager.LookupNamespace("mainr"), this.rId);
            this.table_node.AppendChild(oElementTablePart);

            // Write AutoFilter To Xml
            XmlElement oElementTableFile = (XmlElement)this.xml_table_file;
            oElementTableFile.SetAttribute("ref", oFilterRange.ToString());

            if (0 == oElementTableFile.ChildNodes.Count)
            {
                // Auto Filter
                XmlElement oAutoFilterElem = xml_worksheet.OwnerDocument.CreateElement("autoFilter", ns_manager.LookupNamespace("main"));
                oAutoFilterElem.SetAttribute("ref", oFilterRange.ToString());
                oElementTableFile.AppendChild(oAutoFilterElem);

                // Create Columns
                table_columns_node = xml_worksheet.OwnerDocument.CreateElement("tableColumns", ns_manager.LookupNamespace("main"));
                ((XmlElement)table_columns_node).SetAttribute("count", (oFilterRange.ColumnLast - oFilterRange.ColumnFirst + 1).ToString());
                for (int nIndex = oFilterRange.ColumnFirst, i = 1; nIndex <= oFilterRange.ColumnLast; ++nIndex, ++i)
                {
                    Range oRange = new Range(Helpers.ReferencesFormat.column2str(nIndex, true) + oFilterRange.RowFirst, worksheet);
                    string strValue = oRange.DispValue.ToString();
                    if (string.IsNullOrEmpty(strValue))
                        strValue = "Столбец" + i.ToString();

                    TableColumn oTableColumn = new TableColumn();
                    oTableColumn.m_strId = i.ToString();
                    oTableColumn.m_strName = strValue;
                    strValue = this.addTableColumnToMap(oTableColumn);

                    oRange.ValueType = "s";
                    oRange.updateHeaders = false;
                    oRange.Value = strValue;

                    XmlElement oTableColumnElement = xml_worksheet.OwnerDocument.CreateElement("tableColumn", ns_manager.LookupNamespace("main"));
                    oTableColumnElement.SetAttribute("id", i.ToString());
                    oTableColumnElement.SetAttribute("name", strValue);
                    table_columns_node.AppendChild(oTableColumnElement);
                }
                oElementTableFile.AppendChild(table_columns_node);

                //Style Table
                XmlElement oTableStyleInfo = xml_worksheet.OwnerDocument.CreateElement("tableStyleInfo", ns_manager.LookupNamespace("main"));
                oTableStyleInfo.SetAttribute("name", this.TableStyle);
                oTableStyleInfo.SetAttribute("showFirstColumn", "0");
                oTableStyleInfo.SetAttribute("showLastColumn", "0");
                oTableStyleInfo.SetAttribute("showRowStripes", "1");
                oTableStyleInfo.SetAttribute("showColumnStripes", "0");
                oElementTableFile.AppendChild(oTableStyleInfo);
            }

            int nMaxIdFile = MaxIdFile(xml_worksheet) + 1;
            oElementTableFile.SetAttribute("id", nMaxIdFile.ToString());
            oElementTableFile.SetAttribute("name", "Table" + nMaxIdFile.ToString());
            oElementTableFile.SetAttribute("displayName", "Table" + nMaxIdFile.ToString());

            TableAutoFilter = AutoFilter.CreateNew(worksheet, xml_table_file, xml_worksheet.SelectSingleNode("main:sheetData", ns_manager), oFilterRange);
        }

        internal static Table CreateNew(Worksheet worksheet, OOX.TableFile oTableFile, XmlNode xml_worksheet, Cells.CellRange oFilterRange)
        {
            return new Table(worksheet, oTableFile, xml_worksheet, oFilterRange);
        }

        public void RemoveFromXml(int nCountTables)
        {
            if (null != this.table_node)
            {
                XmlNode existent_table = this.table_node.SelectSingleNode("main:tablePart[@mainr:id='" + rId + "']", this.ns_manager);
                if (null != existent_table)
                {
                    existent_table.ParentNode.RemoveChild(existent_table);

                    this.table_node.Attributes["count"].Value = (nCountTables - 1).ToString();
                }
            }

            this.m_oTableFile.Remove();
        }

        public Dictionary<int, bool> ChangeAutoFilter(AutoFilterElementSendData oChangeObject)
        {
            return this.TableAutoFilter.ChangeAutoFilter(oChangeObject);
        }

        public bool isCellHeader(int row, int col)
        {
            if (null == TableAutoFilter || null == TableAutoFilter.m_oRefRange)
                return false;

            if (row == TableAutoFilter.m_oRefRange.RowFirst &&
                TableAutoFilter.m_oRefRange.ColumnFirst <= col && col <= TableAutoFilter.m_oRefRange.ColumnLast)
                return true;

            return false;
        }

        public void changeCellHeader(int col)
        {
            if (null == TableAutoFilter || null == TableAutoFilter.m_oRefRange)
                return;

            Range oRange = new Range(Helpers.ReferencesFormat.column2str(col, true) + TableAutoFilter.m_oRefRange.RowFirst, worksheet);
            string strValue = oRange.DispValue.ToString();

            int nColId = col - TableAutoFilter.m_oRefRange.ColumnFirst + 1;
            string strColId = nColId.ToString();
            foreach (var pair in m_mapTableColumns)
            {
                if (pair.Value.m_strId == strColId)
                {
                    TableColumn oTableColumn = pair.Value;
                    m_mapTableColumns.Remove(oTableColumn.m_strName);

                    oTableColumn.m_strName = strValue;
                    strValue = addTableColumnToMap(oTableColumn);

                    XmlElement oXmlTableColumnNode = (XmlElement)table_columns_node.SelectSingleNode("main:tableColumn[@id = '" + strColId + "']", ns_manager);
                    if (null != oXmlTableColumnNode)
                        oXmlTableColumnNode.SetAttribute("name", strValue);

                    oRange.ValueType = "s";
                    oRange.updateHeaders = false;
                    oRange.Value = strValue;
                    break;
                }
            }
        }

        private int MaxIdFile(XmlNode xml_worksheet)
        {
            int nMaxIdFile = 0;

            string template_path = "../tables/table.xml";
            int last_slash_pos = template_path.LastIndexOf("/");
            string folder_path = template_path.Substring(0, last_slash_pos + 1); /* (last_slash_pos == -1) is taken into account here too */
            XmlNode target_folder = Helpers.OOXNavigate.getFolderReferenceByPath(folder_path, xml_worksheet.ParentNode.ParentNode, true);
            foreach (XmlNode oNodeChild in target_folder.ChildNodes)
            {
                int nId = System.Convert.ToInt32(oNodeChild.ChildNodes[0].Attributes["id"].Value);
                nMaxIdFile = Math.Max(nMaxIdFile, nId);
            }

            return nMaxIdFile;
        }

        private string addTableColumnToMap(TableColumn oTableColumn)
        {
            if (m_mapTableColumns.ContainsKey(oTableColumn.m_strName))
            {
                int nIndex = 2;
                while (m_mapTableColumns.ContainsKey(oTableColumn.m_strName + nIndex))
                    ++nIndex;
                oTableColumn.m_strName = oTableColumn.m_strName + nIndex;
            }

            m_mapTableColumns.Add(oTableColumn.m_strName, oTableColumn);
            return oTableColumn.m_strName;
        }

        public string TableStyle { get; private set; }
        public AutoFilter TableAutoFilter { get; private set; }
        public string rId { get; set; }

        private XmlNode table_node;
        private XmlNode table_columns_node;
        private XmlNode xml_table_file;
        private Worksheet worksheet;
        private XmlNamespaceManager ns_manager;
        private OOX.TableFile m_oTableFile;

        private Dictionary<string, TableColumn>  m_mapTableColumns = new Dictionary<string,TableColumn>();
    }

    public class TableStyle
    {
        // Summary:
        //     Specifies the line style for the border.
        public enum XlLineStyle
        {
            // Summary:
            //     No line.
            xlLineStyleNone = -4142,
            //
            // Summary:
            //     Double line.
            xlDouble = -4119,
            //
            // Summary:
            //     Dotted line.
            xlDot = -4118,
            //
            // Summary:
            //     Dashed line.
            xlDash = -4115,
            //
            // Summary:
            //     Continuous line.
            xlContinuous = 1,
            //
            // Summary:
            //     Alternating dashes and dots.
            xlDashDot = 4,
            //
            // Summary:
            //     Dash followed by two dots.
            xlDashDotDot = 5,
            //
            // Summary:
            //     Slanted dashes.
            xlSlantDashDot = 13,
        };

        public struct TableStyleElemBorder
        {
            public XlLineStyle LineStyle;
            public string Color;
            public int? Size;
        };

        public struct TableStyleElem
        {
            public bool? Bold;
            public bool? Italic;
            public bool? Underline;
            public string Color;
            public string BgColor;

            public TableStyleElemBorder EdgeLeft;
            public TableStyleElemBorder EdgeTop;
            public TableStyleElemBorder EdgeBottom;
            public TableStyleElemBorder EdgeRight;
            public TableStyleElemBorder InsideVertical;
            public TableStyleElemBorder InsideHorizontal;
        };

        public static TableStyle GetDefaultTableStyle(string strName)
        {
            TableStyle oDefaultTableStyle = new TableStyle();

            string strXml = ResourceExcelDefaultStyles.ResourceManager.GetString(strName);
            if (string.IsNullOrEmpty(strXml))
                return null;

            XmlDocument oDoc = new XmlDocument();
            oDoc.LoadXml(strXml);

            oDefaultTableStyle.HeaderStyle = LoadStyleElem(oDoc, "headerRow");
            oDefaultTableStyle.FirstRowStyle = LoadStyleElem(oDoc, "firstRowStripe");
            oDefaultTableStyle.SecondRowStyle = LoadStyleElem(oDoc, "secondRowStripe");
            oDefaultTableStyle.WholeTableStyle = LoadStyleElem(oDoc, "wholeTable");

            return oDefaultTableStyle;
        }

        public static TableStyleElem LoadStyleElem(XmlDocument oDoc, string strName)
        {
            TableStyleElem oElem = new TableStyleElem();
            XmlNode oXmlNode = oDoc.SelectSingleNode("/TableStyle/TableStyleElement[@name='" + strName + "']");
            if (null != oXmlNode)
            {
                if (null != oXmlNode.Attributes["Bold"])
                    oElem.Bold = "true" == oXmlNode.Attributes["Bold"].Value;
                if (null != oXmlNode.Attributes["Italic"])
                    oElem.Italic = "true" == oXmlNode.Attributes["Italic"].Value;
                if (null != oXmlNode.Attributes["Underline"])
                    oElem.Underline = "true" == oXmlNode.Attributes["Underline"].Value;
                if (null != oXmlNode.Attributes["Color"])
                    oElem.Color = oXmlNode.Attributes["Color"].Value;
                if (null != oXmlNode.Attributes["BgColor"])
                    oElem.BgColor = oXmlNode.Attributes["BgColor"].Value;

                oElem.EdgeTop = LoadStyleElemBorder(oXmlNode, "EdgeTop");
                oElem.EdgeLeft = LoadStyleElemBorder(oXmlNode, "EdgeLeft");
                oElem.EdgeBottom = LoadStyleElemBorder(oXmlNode, "EdgeBottom");
                oElem.EdgeRight = LoadStyleElemBorder(oXmlNode, "EdgeRight");
                oElem.InsideVertical = LoadStyleElemBorder(oXmlNode, "InsideVertical");
                oElem.InsideHorizontal = LoadStyleElemBorder(oXmlNode, "InsideHorizontal");
            }

            return oElem;
        }

        public static TableStyleElemBorder LoadStyleElemBorder(XmlNode oXmlNode, string strBorderName)
        {
            TableStyleElemBorder oElemBorder = new TableStyleElemBorder();
            XmlNode oXmlNodeBorder = oXmlNode.SelectSingleNode(strBorderName);
            if (null != oXmlNodeBorder)
            {
                if (null != oXmlNodeBorder.Attributes["LineStyle"])
                    oElemBorder.LineStyle = (XlLineStyle)System.Convert.ToInt32(oXmlNodeBorder.Attributes["LineStyle"].Value);
                if (null != oXmlNodeBorder.Attributes["Color"])
                    oElemBorder.Color = oXmlNodeBorder.Attributes["Color"].Value;
                if (null != oXmlNodeBorder.Attributes["Weight"])
                {
                    oElemBorder.Size = System.Convert.ToInt32(oXmlNodeBorder.Attributes["Weight"].Value);
                    if (-4138 == oElemBorder.Size)
                        oElemBorder.Size = 2;
                    else
                        oElemBorder.Size /= 2;
                }
            }

            return oElemBorder;
        }

        public TableStyleElem HeaderStyle { get; private set; }
        public TableStyleElem FirstRowStyle { get; private set; }
        public TableStyleElem SecondRowStyle { get; private set; }
        public TableStyleElem WholeTableStyle { get; private set; }
    }
}
