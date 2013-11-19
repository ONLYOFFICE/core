using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.Editor.Charts;

namespace AVSOfficeEWSEditor.Editor.Charts
{
    /// <summary>
    /// Plotting area
    /// </summary>
    public class PlotArea
    {
        private XmlNode plot_area_xml;
        private XmlNamespaceManager ns_manager;
        public BasicChar basicChart;

        internal PlotArea(XmlNode plot_area_xml, Chart chart, XmlNamespaceManager ns_manager)
        {
            this.plot_area_xml = plot_area_xml;
            this.ns_manager = ns_manager;

            InitSeries(plot_area_xml, chart, ns_manager);
        }

        private void InitSeries(XmlNode plot_area_xml, Chart chart, XmlNamespaceManager ns_manager)
        {
            XmlNode bar_xml = plot_area_xml.SelectSingleNode(@"c:barChart | c:bar3DChart | c:lineChart | c:line3DChart | c:areaChart | c:pieChart | 
                                                               c:bubbleChart | c:scatterChart | c:radarChart | c:doughnutChart | c:stockChart",
                                                             ns_manager);
            if (bar_xml != null)
                basicChart = new BasicChar(bar_xml, bar_xml.LocalName, chart, ns_manager);
        }

        public string xTitle
        {
            get
            {
                string retVal = "";

                XmlElement xAxisNode = FindAxisX();
                if (xAxisNode != null)
                {
                    XmlNodeList titleNodes = xAxisNode.SelectNodes("c:title/c:tx/c:rich/a:p/a:r/a:t", ns_manager);
                    for (int i = 0; i < titleNodes.Count; i++)
                    {
                        retVal += ((i > 0) ? " " : "") + titleNodes[i].InnerText;
                    }                       
                }

                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value))
                {
                    XmlElement xAxisNode = FindAxisX();
                    if (xAxisNode != null)
                    {
                        XmlNode title = xAxisNode.SelectSingleNode("c:title/c:tx/c:rich/a:p/a:r/a:t", ns_manager);
                        if (title != null)
                            title.InnerText = value.ToString();
                    }
                }
            }
        }

        public string yTitle
        {
            get
            {
                string retVal = "";

                XmlElement yAxisNode = FindAxisY();
                if (yAxisNode != null)
                {
                    XmlNodeList titleNodes = yAxisNode.SelectNodes("c:title/c:tx/c:rich/a:p/a:r/a:t", ns_manager);
                    for (int i = 0; i < titleNodes.Count; i++)
                    {
                        retVal += ((i > 0) ? " " : "") + titleNodes[i].InnerText;
                    }                       
                }

                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value))
                {
                    XmlElement yAxisNode = FindAxisY();
                    if (yAxisNode != null)
                    {
                        XmlNode title = yAxisNode.SelectSingleNode("c:title/c:tx/c:rich/a:p/a:r/a:t", ns_manager);
                        if (title != null)
                            title.InnerText = value.ToString();
                    }
                }
            }
        }

        public bool xShow
        {
            get
            {
                XmlElement xAxisNode = FindAxisX();
                if (xAxisNode != null)
                {
                    XmlElement show = (XmlElement)xAxisNode.SelectSingleNode("c:delete", ns_manager);
                    if (show != null)
                        return !Misc.StringToBool(show.GetAttribute("val"));                    
                }
                return true; // default
            }
            set
            {
                if (!value)
                {
                    XmlElement xAxisNode = FindAxisX();
                    if (xAxisNode != null)
                    {
                        XmlElement show = (XmlElement)xAxisNode.SelectSingleNode("c:delete", ns_manager);
                        if (show != null)
                            show.SetAttribute("val", "1");
                    }                    
                }
            }
        }

        public bool yShow
        {
            get
            {
                XmlElement yAxisNode = FindAxisY();
                if (yAxisNode != null)
                {
                    XmlElement show = (XmlElement)yAxisNode.SelectSingleNode("c:delete", ns_manager);
                    if (show != null)
                        return !Misc.StringToBool(show.GetAttribute("val"));
                }
                return true; // default
            }
            set
            {
                if (!value)
                {
                    XmlElement yAxisNode = FindAxisY();
                    if (yAxisNode != null)
                    {
                        XmlElement show = (XmlElement)yAxisNode.SelectSingleNode("c:delete", ns_manager);
                        if (show != null)
                            show.SetAttribute("val", "1");
                    }
                }
            }
        }

        public bool xGrid
        {
            get
            {
                XmlElement xAxisNode = FindAxisX();
                if (xAxisNode != null)
                {
                    XmlNode gridNode = (XmlElement)xAxisNode.SelectSingleNode("c:majorGridlines", ns_manager);
                    return (gridNode != null);
                }
                return false; // default
            }
            set
            {
                if (!value)
                {
                    XmlElement xAxisNode = FindAxisX();
                    if (xAxisNode != null)
                    {
                        XmlNode gridNode = (XmlElement)xAxisNode.SelectSingleNode("c:majorGridlines", ns_manager);
                        if (gridNode != null)
                            xAxisNode.RemoveChild(gridNode);                        
                    }                    
                }
            }
        }

        public bool yGrid
        {
            get
            {
                XmlElement yAxisNode = FindAxisY();
                if (yAxisNode != null)
                {
                    XmlNode gridNode = (XmlElement)yAxisNode.SelectSingleNode("c:majorGridlines", ns_manager);
                    return (gridNode != null);
                }
                return false; // default
            }
            set
            {
                if (!value)
                {
                    XmlElement yAxisNode = FindAxisY();
                    if (yAxisNode != null)
                    {
                        XmlNode gridNode = (XmlElement)yAxisNode.SelectSingleNode("c:majorGridlines", ns_manager);
                        if (gridNode != null)
                            yAxisNode.RemoveChild(gridNode);
                    }
                }
            }
        }

        XmlElement FindAxisX()
        {
            XmlElement axisNode = (XmlElement)plot_area_xml.SelectSingleNode("c:catAx", ns_manager);
            if (axisNode != null)
                return axisNode;
            else
            {
                XmlNodeList valAxisList = plot_area_xml.SelectNodes("c:valAx", ns_manager);
                for (int i = 0; i < valAxisList.Count; i++)
                {
                    // проверяем позицию оси
                    XmlNode posAxisNode = valAxisList[i].SelectSingleNode("c:axPos", ns_manager);
                    if (posAxisNode != null)
                    {
                        if (((XmlElement)posAxisNode).GetAttribute("val") == "b")
                            return (XmlElement)valAxisList[i];
                    }
                }
            }

            return axisNode;
        }

        XmlElement FindAxisY()
        {
            XmlElement axisNode = (XmlElement)plot_area_xml.SelectSingleNode("c:catAx", ns_manager);
            if (axisNode != null)
                return (XmlElement)plot_area_xml.SelectSingleNode("c:valAx", ns_manager);
            else
            {
                XmlNodeList valAxisList = plot_area_xml.SelectNodes("c:valAx", ns_manager);
                for (int i = 0; i < valAxisList.Count; i++)
                {
                    // проверяем позицию оси
                    XmlNode posAxisNode = valAxisList[i].SelectSingleNode("c:axPos", ns_manager);
                    if (posAxisNode != null)
                    {
                        if (((XmlElement)posAxisNode).GetAttribute("val") == "l")
                            return (XmlElement)valAxisList[i];
                    }
                }
            }

            return null;
        }
    }
}
