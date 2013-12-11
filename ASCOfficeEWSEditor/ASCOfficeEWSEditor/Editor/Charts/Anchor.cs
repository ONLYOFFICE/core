using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Charts
{
    /// <summary>
    /// Absolute anchor of a drawing displacement
    /// </summary>
    public class AbsoluteAnchor
    {
        private XmlNode anchor_xml;
        private Sheet parent_sheet;

        internal AbsoluteAnchor(XmlNode anchor_xml, Sheet parent_sheet)
        {
            this.anchor_xml = anchor_xml;
            this.parent_sheet = parent_sheet;
        }

        /// <summary>
        /// Width of the chart. Measured in points
        /// </summary>
        public double Width
        {
            get
            {
                XmlElement ext_xml = (XmlElement)anchor_xml.SelectSingleNode("xdr:ext", parent_sheet.Workbook.NamespaceManager);
                if (ext_xml != null)
                {
                    long cx;
                    if (long.TryParse(ext_xml.GetAttribute("cx"), out cx))
                    {
                        return cx / 12700.0;
                    }
                }
                return 0;
            }
        }

        /// <summary>
        /// Height of the chart. Measured in points
        /// </summary>
        public double Height
        {
            get
            {
                XmlElement ext_xml = (XmlElement)anchor_xml.SelectSingleNode("xdr:ext", parent_sheet.Workbook.NamespaceManager);
                if (ext_xml != null)
                {
                    long cy;
                    if (long.TryParse(ext_xml.GetAttribute("cy"), out cy))
                    {
                        return cy / 12700.0;
                    }
                }
                return 0;
            }
        }

        /// <summary>
        /// X position of the chart. Measured in points
        /// </summary>
        public double PosX
        {
            get
            {
                XmlElement ext_xml = (XmlElement)anchor_xml.SelectSingleNode("xdr:pos", parent_sheet.Workbook.NamespaceManager);
                if (ext_xml != null)
                {
                    long x;
                    if (long.TryParse(ext_xml.GetAttribute("x"), out x))
                    {
                        return x / 12700.0;
                    }
                }
                return 0;
            }
        }

        /// <summary>
        /// Y position of the chart. Measured in points
        /// </summary>
        public double PosY
        {
            get
            {
                XmlElement ext_xml = (XmlElement)anchor_xml.SelectSingleNode("xdr:pos", parent_sheet.Workbook.NamespaceManager);
                if (ext_xml != null)
                {
                    long y;
                    if (long.TryParse(ext_xml.GetAttribute("y"), out y))
                    {
                        return y / 12700.0;
                    }
                }
                return 0;
            }
        }
    }


    /// <summary>
    /// Two cells anchor of a drawing displacement
    /// </summary>
    public class TwoCellAnchor
    {
        private XmlNode anchor_xml;
        private Sheet parent_sheet;

        internal TwoCellAnchor(XmlNode anchor_xml, Sheet parent_sheet)
        {
            this.anchor_xml = anchor_xml;
            this.parent_sheet = parent_sheet;

            LeftColumn = GetIntFromNode("xdr:from/xdr:col", 0);
            RightColumn = GetIntFromNode("xdr:to/xdr:col", 0);
            TopRow = GetIntFromNode("xdr:from/xdr:row", 0);
            BottomRow = GetIntFromNode("xdr:to/xdr:row", 0);

            //            
            xmlManager = parent_sheet.Workbook.NamespaceManager;
            from = (XmlElement)anchor_xml.SelectSingleNode("xdr:from", xmlManager);
            to = (XmlElement)anchor_xml.SelectSingleNode("xdr:to", xmlManager);
            frame = (XmlElement)anchor_xml.SelectSingleNode("xdr:graphicFrame", xmlManager);
            sheet_id = 0;
            chart_guid = Guid.NewGuid().ToString();
        }

        //
        public int sheet_id;
        public string chart_guid;
        public XmlElement from, to, frame;
        XmlNamespaceManager xmlManager;

        #region xdr:from
        public int from_col
        {
            get
            {
                int retVal = -1;
                if (from != null)
                {
                    XmlElement col = (XmlElement)from.SelectSingleNode("xdr:col", xmlManager);
                    if (col != null)
                        retVal = int.Parse(col.InnerText);
                }

                return retVal;
            }
            set
            {
                if (from != null)
                {
                    XmlElement col = (XmlElement)from.SelectSingleNode("xdr:col", xmlManager);
                    if (col != null)
                        col.InnerText = value.ToString();
                }
            }
        }

        public int from_colOff
        {
            get
            {
                int retVal = -1;
                if (from != null)
                {
                    XmlElement colOff = (XmlElement)from.SelectSingleNode("xdr:colOff", xmlManager);
                    if (colOff != null)
                        retVal = int.Parse(colOff.InnerText);
                }

                return retVal;
            }
            set
            {
                if (from != null)
                {
                    XmlElement colOff = (XmlElement)from.SelectSingleNode("xdr:colOff", xmlManager);
                    if (colOff != null)
                        colOff.InnerText = value.ToString();
                }
            }
        }

        public int from_row
        {
            get
            {
                int retVal = -1;
                if (from != null)
                {
                    XmlElement row = (XmlElement)from.SelectSingleNode("xdr:row", xmlManager);
                    if (row != null)
                        retVal = int.Parse(row.InnerText);
                }

                return retVal;
            }
            set
            {
                if (from != null)
                {
                    XmlElement row = (XmlElement)from.SelectSingleNode("xdr:row", xmlManager);
                    if (row != null)
                        row.InnerText = value.ToString();
                }
            }
        }

        public int from_rowOff
        {
            get
            {
                int retVal = -1;
                if (from != null)
                {
                    XmlElement rowOff = (XmlElement)from.SelectSingleNode("xdr:rowOff", xmlManager);
                    if (rowOff != null)
                        retVal = int.Parse(rowOff.InnerText);
                }

                return retVal;
            }
            set
            {
                if (from != null)
                {
                    XmlElement rowOff = (XmlElement)from.SelectSingleNode("xdr:rowOff", xmlManager);
                    if (rowOff != null)
                        rowOff.InnerText = value.ToString();
                }
            }
        }
        #endregion

        #region xdr:to
        public int to_col
        {
            get
            {
                int retVal = -1;
                if (to != null)
                {
                    XmlElement col = (XmlElement)to.SelectSingleNode("xdr:col", xmlManager);
                    if (col != null)
                        retVal = int.Parse(col.InnerText);
                }

                return retVal;
            }
            set
            {
                if (to != null)
                {
                    XmlElement col = (XmlElement)to.SelectSingleNode("xdr:col", xmlManager);
                    if (col != null)
                        col.InnerText = value.ToString();
                }
            }
        }

        public int to_colOff
        {
            get
            {
                int retVal = -1;
                if (to != null)
                {
                    XmlElement colOff = (XmlElement)to.SelectSingleNode("xdr:colOff", xmlManager);
                    if (colOff != null)
                        retVal = int.Parse(colOff.InnerText);
                }

                return retVal;
            }
            set
            {
                if (to != null)
                {
                    XmlElement colOff = (XmlElement)to.SelectSingleNode("xdr:colOff", xmlManager);
                    if (colOff != null)
                        colOff.InnerText = value.ToString();
                }
            }
        }

        public int to_row
        {
            get
            {
                int retVal = -1;
                if (to != null)
                {
                    XmlElement row = (XmlElement)to.SelectSingleNode("xdr:row", xmlManager);
                    if (row != null)
                        retVal = int.Parse(row.InnerText);
                }

                return retVal;
            }
            set
            {
                if (to != null)
                {
                    XmlElement row = (XmlElement)to.SelectSingleNode("xdr:row", xmlManager);
                    if (row != null)
                        row.InnerText = value.ToString();
                }
            }
        }

        public int to_rowOff
        {
            get
            {
                int retVal = -1;
                if (to != null)
                {
                    XmlElement rowOff = (XmlElement)to.SelectSingleNode("xdr:rowOff", xmlManager);
                    if (rowOff != null)
                        retVal = int.Parse(rowOff.InnerText);
                }

                return retVal;
            }
            set
            {
                if (to != null)
                {
                    XmlElement rowOff = (XmlElement)to.SelectSingleNode("xdr:rowOff", xmlManager);
                    if (rowOff != null)
                        rowOff.InnerText = value.ToString();
                }
            }
        }
        #endregion

        #region xdr:graphicFrame
        public int frame_id
        {
            get
            {
                int retVal = -1;
                if (frame != null)
                {
                    XmlElement id = (XmlElement)frame.SelectSingleNode("xdr:nvGraphicFramePr/xdr:cNvPr", xmlManager);
                    if (id != null)
                        retVal = int.Parse(id.GetAttribute("id"));
                }

                return retVal;
            }
            set
            {
                if (frame != null)
                {
                    XmlElement id = (XmlElement)frame.SelectSingleNode("xdr:nvGraphicFramePr/xdr:cNvPr", xmlManager);
                    if (id != null)
                        id.SetAttribute("id", value.ToString());
                }
            }
        }

        public string chart_rid
        {
            get
            {
                string retVal = "";
                if (frame != null)
                {
                    XmlElement rid = (XmlElement)frame.SelectSingleNode("a:graphic/a:graphicData/c:chart", xmlManager);
                    if (rid != null)
                        retVal = rid.GetAttribute("id", xmlManager.LookupNamespace("mainr"));
                }

                return retVal;
            }
            set
            {
                if (frame != null)
                {
                    XmlElement rid = (XmlElement)frame.SelectSingleNode("a:graphic/a:graphicData/c:chart", xmlManager);
                    if (rid != null)
                        rid.SetAttribute("r:id", value.ToString());
                }
            }
        }
        #endregion

        #region chart_xOff
        public int chart_xOff
        {
            get
            {
                int retVal = -1;
                if (frame != null)
                {
                    XmlElement cw = (XmlElement)frame.SelectSingleNode("xdr:xfrm/a:off", xmlManager);
                    if (cw != null)
                        retVal = int.Parse(cw.GetAttribute("x"));
                }
                return retVal;
            }

            set
            {
                if (frame != null)
                {
                    XmlElement cw = (XmlElement)frame.SelectSingleNode("xdr:xfrm/a:off", xmlManager);
                    if (cw != null)
                        cw.SetAttribute("x", value.ToString());
                }
            }
        }
        #endregion

        #region chart_yOff
        public int chart_yOff
        {
            get
            {
                int retVal = -1;
                if (frame != null)
                {
                    XmlElement ch = (XmlElement)frame.SelectSingleNode("xdr:xfrm/a:off", xmlManager);
                    if (ch != null)
                        retVal = int.Parse(ch.GetAttribute("y"));
                }
                return retVal;
            }

            set
            {
                if (frame != null)
                {
                    XmlElement ch = (XmlElement)frame.SelectSingleNode("xdr:xfrm/a:off", xmlManager);
                    if (ch != null)
                        ch.SetAttribute("y", value.ToString());
                }
            }
        }
        #endregion

        #region chart_width
        public int chart_width
        {
            get
            {
                int retVal = -1;
                if (frame != null)
                {
                    XmlElement cw = (XmlElement)frame.SelectSingleNode("xdr:xfrm/a:ext", xmlManager);
                    if (cw != null)
                        retVal = int.Parse(cw.GetAttribute("cx"));
                }
                return retVal;
            }

            set
            {
                if (frame != null)
                {
                    XmlElement cw = (XmlElement)frame.SelectSingleNode("xdr:xfrm/a:ext", xmlManager);
                    if (cw != null)
                        cw.SetAttribute("cx", value.ToString());
                }
            }
        }
        #endregion

        #region chart_height
        public int chart_height
        {
            get
            {
                int retVal = -1;
                if (frame != null)
                {
                    XmlElement ch = (XmlElement)frame.SelectSingleNode("xdr:xfrm/a:ext", xmlManager);
                    if (ch != null)
                        retVal = int.Parse(ch.GetAttribute("cy"));
                }
                return retVal;
            }

            set
            {
                if (frame != null)
                {
                    XmlElement ch = (XmlElement)frame.SelectSingleNode("xdr:xfrm/a:ext", xmlManager);
                    if (ch != null)
                        ch.SetAttribute("cy", value.ToString());
                }
            }
        }
        #endregion

        //

        private int GetIntFromNode(string path, int default_value)
        {
            int num = default_value;
            XmlElement num_xml = (XmlElement)anchor_xml.SelectSingleNode(path, parent_sheet.Workbook.NamespaceManager);
            if (num_xml != null)
            {
                int.TryParse(num_xml.InnerText, out num);
            }
            return num;
        }

        /// <summary>
        /// Left column (zero-based)
        /// </summary>
        internal int LeftColumn { get; private set; }
        /// <summary>
        /// Right column (zero-based)
        /// </summary>
        internal int RightColumn { get; private set; }
        /// <summary>
        /// Top row (zero-based)
        /// </summary>
        internal int TopRow { get; private set; }
        /// <summary>
        /// Bottom row (zero-based)
        /// </summary>
        internal int BottomRow { get; private set; }



        /// <summary>
        /// Reference of the cell where the top left corner of the anchor is located.
        /// </summary>
        public string TopLeftRef
        {
            get
            {
                return new Cells.Cell(TopRow + 1, LeftColumn + 1).ToString();
            }
        }

        /// <summary>
        /// Reference of the cell where the bottom right corner of the anchor is located.
        /// </summary>
        public string BottomRightRef
        {
            get
            {
                return new Cells.Cell(BottomRow + 1, RightColumn + 1).ToString();
            }
        }

        /// <summary>
        /// Offset in points inside a cell where the top left corner is located
        /// </summary>
        public Chart.XY TopLeftOffset
        {
            get
            {
                return GetCornerOffset("xdr:from");
            }
        }

        /// <summary>
        /// Offset in points inside a cell where the bottom right corner is located
        /// </summary>
        public Chart.XY BottomRightOffset
        {
            get
            {
                return GetCornerOffset("xdr:to");
            }
        }

        private Chart.XY GetCornerOffset(string to_from_tag_name)
        {
            int col_off = 0;
            XmlElement col_xml = (XmlElement)anchor_xml.SelectSingleNode(to_from_tag_name + "/xdr:colOff", parent_sheet.Workbook.NamespaceManager);
            if (col_xml != null)
            {
                int.TryParse(col_xml.InnerText, out col_off);
            }
            int row_off = 0;
            XmlElement row_xml = (XmlElement)anchor_xml.SelectSingleNode(to_from_tag_name + "/xdr:rowOff", parent_sheet.Workbook.NamespaceManager);
            if (row_xml != null)
            {
                int.TryParse(row_xml.InnerText, out row_off);
            }

            return new Chart.XY(col_off / 12700.0, row_off / 12700.0);
        }


    }
}
