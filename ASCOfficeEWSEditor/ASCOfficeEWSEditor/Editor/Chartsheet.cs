using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;
using System.IO;
using AVSOfficeEWSEditor.Editor.Charts;
using AVSOfficeEWSEditor.Editor.Events;

namespace AVSOfficeEWSEditor.Editor
{
    /// <summary>
    /// Represents a chartsheet.
    /// </summary>
    public class Chartsheet : Sheet
    {
        #region XML references
        private XmlNode xml_chartsheet;
        #endregion
        #region tree references
        #endregion

        internal OOX.Relationships Relationships { get; private set; }

        private OOX.ChartsheetFile chartsheet_file;
        
        /// <summary>
        /// Create from existent chartsheet
        /// </summary>
        internal Chartsheet(string sheet_path, XmlNode xml_workbook_sheet, Sheets container, Workbook workbook)
            : base(container, workbook, xml_workbook_sheet)
        {
            chartsheet_file = new OOX.ChartsheetFile(sheet_path, workbook.ContentTypes, workbook.Relationships, workbook);
            xml_chartsheet = chartsheet_file.GetRootTag();
            Relationships = new OOX.Relationships(xml_chartsheet.ParentNode);

            FindChart();
        }

        /// <summary>
        /// Permanently removes the chartsheet from the workbook
        /// </summary>
        /// <returns>Whether the operation successful. Return false if the removed sheet is the only sheet in the workbook</returns>
        public override bool Remove()
        {
            if (RemoveSheet())
            {
                chartsheet_file.Remove();
                return true;
            }
            return false;
        }

        private void FindChart()
        {
            #region Find drawing file
            XmlElement drawing_node = (XmlElement)xml_chartsheet.SelectSingleNode("main:drawing", ns_manager);
            if(drawing_node == null)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. Required 'drawing' tag is absent inside chartsheet."));
            }
            string drawing_rId = drawing_node.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
            string drawing_path = Relationships.GetTarget(drawing_rId);

            XmlNode drawing_file_xml = Helpers.OOXNavigate.getReferenceByPath(drawing_path, xml_chartsheet.ParentNode.ParentNode);
            if (drawing_file_xml == null)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. File '" + drawing_path + "' not found."));
            }
            #endregion

            #region Find chart file
            XmlNode anchor_xml = drawing_file_xml.SelectSingleNode("xdr:wsDr/xdr:absoluteAnchor", ns_manager);
            if (anchor_xml == null)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. Required 'absoluteAnchor' tag is absent inside <xdr:wsDr>."));
            }
            XmlNode anchor_chart_xml = anchor_xml.SelectSingleNode("xdr:graphicFrame/a:graphic/a:graphicData/c:chart", ns_manager);
            if (anchor_chart_xml == null)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. Required 'chart' tag is absent beneath <absoluteAnchor>."));
            }
            string chart_rId = drawing_node.GetAttribute("id", ns_manager.LookupNamespace("mainr"));

            OOX.Relationships drawing_relationships = new OOX.Relationships(drawing_file_xml);

            string chart_path = drawing_relationships.GetTarget(chart_rId);
            XmlNode chart_file_xml = Helpers.OOXNavigate.getReferenceByPath(chart_path, drawing_file_xml.ParentNode);
            if (chart_file_xml == null)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong OOX format. File '" + chart_path + "' not found."));
            }
            #endregion

            Chart = new Chart(chart_file_xml, anchor_xml, this);
        }

        /// <summary>
        /// Chart
        /// </summary>
        public Chart Chart { get; private set; }


        /// <summary>
        /// Overridden method of determination whether the sheet has data
        /// </summary>
        public override bool HasSheetData { get { return false; } }

    }
}
