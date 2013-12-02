using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Charts
{
    /// <summary>
    /// Chart
    /// </summary>
    public class Chart
    {
        public XmlNode chart_file_xml;
        public XmlNode anchor_xml;
        public Sheet parent_sheet { get; private set; }

        internal Chart(XmlNode chart_file_xml, XmlNode anchor_xml, Sheet parent_sheet)
        {
            this.chart_file_xml = chart_file_xml;
            this.anchor_xml = anchor_xml;
            this.parent_sheet = parent_sheet;

            InitSubAreas();
        }

        private void InitSubAreas()
        {
            #region Common variables
            XmlNamespaceManager ns_manager = parent_sheet.Workbook.NamespaceManager;
            XmlNode chart_xml = chart_file_xml.SelectSingleNode("c:chartSpace/c:chart", ns_manager);
            if (chart_xml == null)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. Required 'chart' tag is absent inside \"" + ((XmlElement)chart_file_xml).GetAttribute("name") + "\"."));
            }
            #endregion

            #region Width / Height

            Height = GetHeight();
            Width = GetWidth();
            em = parent_sheet.Workbook.DefaultFontSize;

            #endregion

            #region Legend
            XmlNode legend_xml = chart_xml.SelectSingleNode("c:legend", ns_manager);
            Legend = legend_xml != null ? new Legend(legend_xml, this, ns_manager) : null;
            #endregion

            #region Layout
            XmlNode layout_xml = chart_xml.SelectSingleNode("c:layout/c:manualLayout", ns_manager);
            ManualLayout = layout_xml != null ? new Layout(layout_xml, this, new XY(0, 0), ns_manager) : null;
            #endregion

            #region Anchors
            AbsoluteAnchor = GetAbsoluteAnchor();
            TwoCellAnchor = GetTwoCellAnchor();
            #endregion

            #region Plot area

            XmlNode plot_area_xml = chart_xml.SelectSingleNode("c:plotArea", ns_manager);
            if (plot_area_xml == null)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. Required 'plotArea' tag is absent inside chart."));
            }
            PlotArea = new PlotArea(plot_area_xml, this, ns_manager);

            #endregion
        }

        public void DeleteLegend()
        {
            XmlNamespaceManager ns_manager = parent_sheet.Workbook.NamespaceManager;
            XmlNode chart_xml = chart_file_xml.SelectSingleNode("c:chartSpace/c:chart", ns_manager);
            XmlNode legend_xml = chart_xml.SelectSingleNode("c:legend", ns_manager);
            chart_xml.RemoveChild(legend_xml);
            Legend = null;
        }

        public string title
        {
            get
            {
                // имя может быть составным, например, на разных языках
                string retVal = "";

                XmlNodeList titleNodes = chart_file_xml.SelectNodes("c:chartSpace/c:chart/c:title/c:tx/c:rich/a:p/a:r/a:t", parent_sheet.Workbook.NamespaceManager);
                for (int i = 0; i < titleNodes.Count; i++)
                {
                    retVal += ((i > 0) ? " " : "") + titleNodes[i].InnerText;
                }                

                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value))
                {
                    XmlNode titleNode = (XmlElement)chart_file_xml.SelectSingleNode("c:chartSpace/c:chart/c:title/c:tx/c:rich/a:p/a:r/a:t", parent_sheet.Workbook.NamespaceManager);
                    if (titleNode != null)
                        titleNode.InnerText = value;
                }
            }
        }

        #region Extraction methods

        private AbsoluteAnchor GetAbsoluteAnchor()
        {
            if (anchor_xml.Name == "xdr:absoluteAnchor")
            {
                return new AbsoluteAnchor(anchor_xml, parent_sheet);
            }
            return null;

        }

        private TwoCellAnchor GetTwoCellAnchor()
        {
            if (anchor_xml.Name == "xdr:twoCellAnchor")
            {
                return new TwoCellAnchor(anchor_xml, parent_sheet);
            }
            return null;
        }

        #endregion


        #region public extraction methods

        /// <summary>
        /// Width of the chart
        /// </summary>
        /// <returns>width in points</returns>
        public double GetWidth()
        {
            if (AbsoluteAnchor != null)
            {
                return AbsoluteAnchor.Width;
            }
            else if (TwoCellAnchor != null)
            {
                int font0size = parent_sheet.Workbook.DefaultFontSize;
                Worksheet ws = parent_sheet as Worksheet;
                if (ws != null)
                {
                    Cells.CellRange cell_range = new Cells.CellRange(TwoCellAnchor.TopRow, TwoCellAnchor.LeftColumn, TwoCellAnchor.BottomRow, TwoCellAnchor.RightColumn);
                    Range range = new Range(cell_range, ws);
                    double left_inset = TwoCellAnchor.TopLeftOffset.x;
                    double right_inset = ws.Cell(cell_range.BottomRight).Width * font0size - TwoCellAnchor.BottomRightOffset.x;

                    return range.Width * font0size - left_inset - right_inset;
                }
            }
            return 0;
        }

        /// Height of the chart
        /// </summary>
        /// <returns>height in points</returns>
        public double GetHeight()
        {
            if (AbsoluteAnchor != null)
            {
                return AbsoluteAnchor.Height;
            }
            else if (TwoCellAnchor != null)
            {
                int font0size = parent_sheet.Workbook.DefaultFontSize;
                Worksheet ws = parent_sheet as Worksheet;
                if (ws != null)
                {
                    Cells.CellRange cell_range = new Cells.CellRange(TwoCellAnchor.TopRow, TwoCellAnchor.LeftColumn, TwoCellAnchor.BottomRow, TwoCellAnchor.RightColumn);
                    Range range = new Range(cell_range, ws);
                    double top_inset = TwoCellAnchor.TopLeftOffset.y;
                    double bottom_inset = ws.Cell(cell_range.BottomRight).Height * font0size - TwoCellAnchor.BottomRightOffset.y;

                    return range.Height * font0size - top_inset - bottom_inset;
                }
            }
            return 0;
        }

        #endregion


        #region Properties
        /// <summary>
        /// Absolute anchor (applicable for chartsheets' charts only. For embedded charts returns null)
        /// </summary>
        public AbsoluteAnchor AbsoluteAnchor { get; private set; }

        /// <summary>
        /// Two cells anchor (applicable for embedded charts only. For chartsheets' charts charts returns null)
        /// </summary>
        public TwoCellAnchor TwoCellAnchor { get; private set; }

        /// <summary>
        /// Legend
        /// </summary>
        public Legend Legend { get; private set; }

        /// <summary>
        /// Manually set layout
        /// </summary>
        public Layout ManualLayout { get; private set; }

        /// <summary>
        /// Width of the chart in points
        /// </summary>
        public double Width { get; private set; }

        /// <summary>
        /// Height of the chart in points
        /// </summary>
        public double Height { get; private set; }

        /// <summary>
        /// em size in points
        /// </summary>
        public double em { get; private set; }

        /// <summary>
        /// Plot area
        /// </summary>
        public PlotArea PlotArea { get; private set; }

        #endregion



        /// <summary>
        /// This struct is a simple readonly pair of doubles. Main purpose - coordinates encapsulation
        /// </summary>
        public struct XY
        {
            internal XY(double x, double y)
            {
                this.x = x;
                this.y = y;
            }
            /// <summary>
            /// Typically X coordinate in points but may be used differently
            /// </summary>
            public readonly double x;
            /// <summary>
            /// Typically Y coordinate in points but may be used differently
            /// </summary>
            public readonly double y;
        }
    }
}
