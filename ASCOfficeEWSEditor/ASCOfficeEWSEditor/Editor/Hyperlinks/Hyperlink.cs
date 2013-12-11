using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Hyperlinks
{
    /// <summary>
    /// Hyperlink object
    /// </summary>
    public class Hyperlink
    {
        internal Hyperlink( Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
        }
        internal Hyperlink(XmlElement hyperlink_xml, Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            InitFromXml(hyperlink_xml, parent_sheet);
        }

        private void InitFromXml(XmlElement hyperlink_xml, Worksheet parent_sheet)
        {
            ns_manager = parent_sheet.Workbook.NamespaceManager;
            Anchor = hyperlink_xml.GetAttribute("ref");
            string rId = hyperlink_xml.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
            if(rId.Length != 0)
            {
                Target = parent_sheet.Relationships.GetTarget(rId);
                ReferredSheet = null;
                ReferredRange = "";
            }
            else
            {
                Target = "";
                string location = hyperlink_xml.GetAttribute("location");
                int start_pos = 0;
                Worksheet ws_from;
                Worksheet ws_to;
                if(Calculation.Formulas.ParsedFormula.extract_3D_part(location, ref start_pos, out ws_from, out ws_to, parent_sheet.Workbook))
                {
                    ReferredSheet = ws_from;
                    ReferredRange = location.Substring(start_pos);
                }
            }
        }

        /// <summary>
        /// Whether the hyperlink is of Location (true) or Address (false) type
        /// </summary>
        public bool IsLocationOrAddressType { get { return ReferredSheet != null; } }

        /// <summary>
        /// Reference target like file path or Internet address
        /// </summary>
        public string Target { get; set; }
        /// <summary>
        /// Location target. Referred worksheet.
        /// </summary>
        public Worksheet ReferredSheet { get; set; }
        /// <summary>
        /// Location target. Referred range of cells.
        /// </summary>
        public string ReferredRange { get; set; }

        /// <summary>
        /// Location (reference) of the hyperlink.
        /// </summary>
        public string Anchor { get; set; }

        internal Cells.Cell CellAnchor 
        {
            get
            {
                return new Cells.CellRange(Anchor, parent_sheet.Workbook).TopLeft;
            }
        }

        private Worksheet parent_sheet;
        private XmlNamespaceManager ns_manager;
    }
}
