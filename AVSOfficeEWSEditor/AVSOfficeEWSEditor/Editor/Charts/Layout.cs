using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Charts
{
    /// <summary>
    /// Manual object layout
    /// </summary>
    public class Layout
    {
        private XmlNode layout_xml;
        private XmlNamespaceManager ns_manager;
        private Chart.XY default_element_position;

        internal Layout(XmlNode legend_xml, Chart chart, Chart.XY default_element_position, XmlNamespaceManager ns_manager)
        {
            this.layout_xml = legend_xml;
            this.default_element_position = default_element_position;
            this.ns_manager = ns_manager;
            Target = GetTarget();
            TopLeft = GetTopLeft(chart.Width, chart.Height);
            Width = GetWidth(chart.Width);
            Height = GetHeight(chart.Height);
        }

        private LayoutTarget GetTarget()
        {
            XmlElement target_xml = (XmlElement)layout_xml.SelectSingleNode("c:layoutTarget", ns_manager);
            if (target_xml != null)
            {
                switch (target_xml.GetAttribute("val"))
                {
                    case "inner": return LayoutTarget.Inner;
                    case "outer": return LayoutTarget.Outer;
                }
            }
            return LayoutTarget.Outer; // default
        }

        private Chart.XY GetTopLeft(double chart_width, double chart_height)
        {
            #region Calculate X
            double x = 0;
            XmlElement x_tag = (XmlElement)layout_xml.SelectSingleNode("c:x", ns_manager);
            if (x_tag != null)
            {
                LayoutMode xmode = GetLayoutMode("c:xMode");

                double.TryParse(x_tag.GetAttribute("val"), out x);
                x *= chart_width;
                if (xmode == LayoutMode.Factor)
                {
                    x += default_element_position.x;
                }
            }
            #endregion
            #region Calculate Y
            double y = 0;
            XmlElement y_tag = (XmlElement)layout_xml.SelectSingleNode("c:y", ns_manager);
            if (y_tag != null)
            {
                LayoutMode ymode = GetLayoutMode("c:yMode");

                double.TryParse(y_tag.GetAttribute("val"), out y);
                y *= chart_height;
                if (ymode == LayoutMode.Factor)
                {
                    y += default_element_position.y;
                }
            }
            #endregion
            return new Chart.XY(x, y);
        }

        private LayoutMode GetLayoutMode(string element_name)
        {
            XmlElement mode_tag = (XmlElement)layout_xml.SelectSingleNode(element_name, ns_manager);
            if (mode_tag != null && mode_tag.GetAttribute("val") == "edge")
            {
                return LayoutMode.Edge;
            }
            return LayoutMode.Factor;
        }

        private double GetWidth(double chart_width)
        {
            double w = 0;
            XmlElement w_tag = (XmlElement)layout_xml.SelectSingleNode("c:w", ns_manager);
            if (w_tag != null)
            {
                LayoutMode wmode = GetLayoutMode("c:wMode");

                double.TryParse(w_tag.GetAttribute("val"), out w);
                w *= chart_width;
                if (wmode == LayoutMode.Edge)
                {
                    w -= TopLeft.x;
                }
            }
            return w;
        }

        private double GetHeight(double chart_height)
        {
            double h = 0;
            XmlElement h_tag = (XmlElement)layout_xml.SelectSingleNode("c:h", ns_manager);
            if (h_tag != null)
            {
                LayoutMode hmode = GetLayoutMode("c:hMode");

                double.TryParse(h_tag.GetAttribute("val"), out h);
                h *= chart_height;
                if (hmode == LayoutMode.Edge)
                {
                    h -= TopLeft.y;
                }
            }
            return h;
        }

        private enum LayoutMode
        {
            Edge,
            Factor
        }


        /// <summary>
        /// Layout target
        /// </summary>
        public LayoutTarget Target { get; private set; }

        /// <summary>
        /// Top left corner position
        /// </summary>
        public Chart.XY TopLeft { get; private set; }

        /// <summary>
        /// Width
        /// </summary>
        public double Width { get; private set; }

        /// <summary>
        /// Height
        /// </summary>
        public double Height { get; private set; }


        /// <summary>
        /// Layout target
        /// </summary>
        public enum LayoutTarget
        {
            /// <summary>
            /// Inner
            /// </summary>
            Inner,
            /// <summary>
            /// Outer (default)
            /// </summary>
            Outer
        }

    }
}
