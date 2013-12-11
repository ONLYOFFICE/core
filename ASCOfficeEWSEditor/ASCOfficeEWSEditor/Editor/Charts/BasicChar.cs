using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Charts
{
    public class BasicChar
    {
        XmlNamespaceManager xmlManager;
        XmlNode barChart;
        string chartName;
        public Chart chart { get; private set; }
        public List<Series> seriesList;        
        public Sheet parent_sheet { get; private set; }

        public BasicChar(XmlNode bar_chart_xml, string chartName, Chart chart, XmlNamespaceManager ns_manager)
        {
            this.parent_sheet = chart.parent_sheet;
            this.xmlManager = ns_manager;
            this.barChart = bar_chart_xml;
            this.chartName = chartName;
            this.chart = chart;
            seriesList = new List<Series>();

            InitSubAreas(bar_chart_xml);
        }

        public bool AddSeries(int count)
        {
            bool bResult = true;

            XmlNodeList allSeries = barChart.SelectNodes("c:ser", xmlManager);

            for (int i = 0; i < count; i++)
            {
                XmlNode lastSeries = allSeries[allSeries.Count - 1].Clone();
                XmlElement idxNode = (XmlElement)lastSeries.SelectSingleNode("c:idx", xmlManager);
                XmlElement orderNode = (XmlElement)lastSeries.SelectSingleNode("c:order", xmlManager);

                idxNode.SetAttribute("val", (allSeries.Count + i).ToString());
                orderNode.SetAttribute("val", (allSeries.Count + i).ToString());

                barChart.AppendChild(lastSeries);
                InitSubAreas(barChart);
            }

            return bResult;
        }

        private void InitSubAreas(XmlNode bar_chart)
        {
            // ищём серии и берём значения
            XmlNodeList allSeries = bar_chart.SelectNodes("c:ser", xmlManager);
            if (allSeries == null)
                return;

            seriesList = new List<Series>();
            foreach (XmlNode series in allSeries)
            {
                seriesList.Add(new Series(series, this, xmlManager));
            }
        }

        public string chartType
        {
            get
            {
                string retVal = "";
                if (barChart != null)
                {
                    XmlElement barDir = (XmlElement)barChart.SelectSingleNode("c:barDir", xmlManager);
                    if (barDir != null)
                        retVal = barDir.GetAttribute("val");
                    else if (chartName == "areaChart" || chartName == "pieChart" || chartName == "bubbleChart" ||
                             chartName == "lineChart" || chartName == "scatterChart" || chartName == "radarChart" ||
                             chartName == "doughnutChart" || chartName == "stockChart")
                        retVal = chartName;
                }

                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value))
                {
                    XmlElement barDir = (XmlElement)barChart.SelectSingleNode("c:barDir", xmlManager);
                    if (barDir != null)
                        barDir.SetAttribute("val", value);
                }
            }
        }

        public string seriesType
        {
            get
            {
                string retVal = "";
                if (barChart != null)
                {
                    XmlElement grouping = (XmlElement)barChart.SelectSingleNode("c:grouping", xmlManager);
                    if (grouping != null)
                        retVal = grouping.GetAttribute("val");
                }

                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value))
                {
                    XmlElement grouping = (XmlElement)barChart.SelectSingleNode("c:grouping", xmlManager);
                    if (grouping != null)
                        grouping.SetAttribute("val", value);

                    if ((value == "stacked") || (value == "percentStacked"))
                    {
                        XmlElement overlap = (XmlElement)barChart.SelectSingleNode("c:overlap", xmlManager);
                        if (overlap != null)
                            overlap.SetAttribute("val", "100");
                    }
                }
            }
        }
    }
}
