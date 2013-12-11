using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Charts
{
    /// <summary>
    /// Legend
    /// </summary>
    public class Legend
    {
        private XmlNode legend_xml;
        private XmlNamespaceManager ns_manager;

        internal Legend(XmlNode legend_xml, Chart chart, XmlNamespaceManager ns_manager)
        {
            this.legend_xml = legend_xml;
            this.ns_manager = ns_manager;
            #region Layout
            XmlNode layout_xml = legend_xml.SelectSingleNode("c:layout/c:manualLayout", ns_manager);
            ManualLayout = layout_xml != null ? new Layout(layout_xml, chart, new Chart.XY(chart.Width / 2, chart.em * 4), ns_manager) : null;
            #endregion
        }

        public bool show
        {
            get
            {
                return (string.IsNullOrEmpty(legend_xml.InnerXml)) ? false : true;
            }
            set
            {
                if (!value)
                    legend_xml.InnerXml = "";
            }
        }

        /// <summary>
        /// This element specifies that other chart elements shall be allowed to overlap this chart element.
        /// </summary>        
        public bool overlay
        {
            get
            {
                XmlElement pos_xml = (XmlElement)legend_xml.SelectSingleNode("c:overlay", ns_manager);
                if (pos_xml != null)
                    return Misc.StringToBool(pos_xml.GetAttribute("val"));
                
                return false; // default
            }
            set
            {
                XmlElement pos_xml = (XmlElement)legend_xml.SelectSingleNode("c:overlay", ns_manager);
                if (pos_xml != null)
                    pos_xml.SetAttribute("val", value ? "1" : "0");
            }
        }

        public string position
        {
            get
            {
                XmlElement pos_xml = (XmlElement)legend_xml.SelectSingleNode("c:legendPos", ns_manager);
                if (pos_xml != null)
                    return pos_xml.GetAttribute("val");
                else
                    return "r"; // default
            }
            set
            {
                if (!string.IsNullOrEmpty(value))
                {
                    XmlElement pos_xml = (XmlElement)legend_xml.SelectSingleNode("c:legendPos", ns_manager);
                    if (pos_xml != null)
                        pos_xml.SetAttribute("val", value);
                }
            }
        }

        /// <summary>
        /// Manually set layout
        /// </summary>
        public Layout ManualLayout { get; private set; }

    }
}
