using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs;
using AVSOfficeEWSEditor.Editor.Cells;
using System.Text.RegularExpressions;
using System.Globalization;

namespace AVSOfficeEWSEditor.Editor.Charts
{
    public class Series
    {
        XmlNamespaceManager xmlManager;
        XmlNode ser_xml;

        private BasicChar basicChart;
        private Worksheet m_oReferredSheet;
        private string m_strReferredRange = "";

        public Series(XmlNode ser_xml, BasicChar basicChart, XmlNamespaceManager ns_manager)
        {
            this.xmlManager = ns_manager;
            this.ser_xml = ser_xml;
            this.basicChart = basicChart;
        }

        public List<double> GetValues()
        {
            List<double> list = new List<double>();
            XmlNode valuesNode = ser_xml.SelectSingleNode("c:val | c:bubbleSize | c:yVal", xmlManager);
            XmlNode cache = valuesNode.SelectSingleNode("c:numRef/c:numCache", xmlManager);

            if (cache != null)
            {
                XmlNodeList values = cache.SelectNodes("c:pt/c:v", xmlManager);
                foreach (XmlNode value in values)
                {
                    double val = 0;
                    bool bRes = double.TryParse(value.InnerText, out val);
                    if (!bRes)
                        bRes = double.TryParse(value.InnerText.Replace(".", ","), out val);

                    list.Add(val);
                }
            }
            // кешированных данных нет, читаем формулу
            else
            {
                string[] intervalParts = interval.Split(new char[] { ';' });       // Sheet1!$D$5;Sheet1!$F$26;Sheet1!$L$5

                for (int i = 0; i < intervalParts.Length; i++)
                {
                    string[] currPart = intervalParts[i].Split(new char[] { '!' });     // Sheet1!$D$5

                    if (currPart.Length == 2)
                    {
                        string formula = currPart[1].Replace("$", "");
                        Range range = new Range(formula, basicChart.parent_sheet.Workbook.ActiveWorksheet);
                        List<string> chartCells = range.ExistentCells();

                        for (int j = 0; j < chartCells.Count; j++)
                        {
                            range = new Range(chartCells[j], basicChart.parent_sheet.Workbook.ActiveWorksheet);
                            double value = 0;
                            bool bRes = double.TryParse(range.DispValue.ToString(), out value);
                            if (!bRes)
                                bRes = double.TryParse(range.DispValue.ToString().Replace(".", ","), out value);
                            list.Add(value);
                        }
                    }
                }
            }

            return list;
        }

        public List<string> GetCategories()
        {
            List<string> list = new List<string>();
            XmlNode valuesNode = ser_xml.SelectSingleNode("c:cat", xmlManager);

            if (valuesNode != null)
            {
                XmlNode cache = valuesNode.SelectSingleNode("c:strRef/c:strCache", xmlManager);

                if (cache != null)
                {
                    XmlNodeList values = cache.SelectNodes("c:pt/c:v", xmlManager);
                    foreach (XmlNode value in values)
                    {
                        list.Add(value.InnerText);
                    }
                }
                // кешированных данных нет, читаем формулу
                else
                {
                    string[] intervalParts = interval.Split(new char[] { ';' });       // Sheet1!$D$5;Sheet1!$F$26;Sheet1!$L$5

                    for (int i = 0; i < intervalParts.Length; i++)
                    {
                        string[] currPart = intervalParts[i].Split(new char[] { '!' });     // Sheet1!$D$5

                        if (currPart.Length == 2)
                        {
                            string formula = currPart[1].Replace("$", "");
                            Range range = new Range(formula, basicChart.parent_sheet.Workbook.ActiveWorksheet);
                            List<string> chartCells = range.ExistentCells();

                            for (int j = 0; j < chartCells.Count; j++)
                            {
                                range = new Range(chartCells[j], basicChart.parent_sheet.Workbook.ActiveWorksheet);
                                list.Add(range.DispValue.ToString());
                            }
                        }
                    }
                }
            }

            return list;
        }

        public void InsertValues(List<double> values)
        {    
            DeleteValues();
            XmlNode valuesNode = ser_xml.SelectSingleNode("c:val | c:bubbleSize | c:yVal", xmlManager);
            XmlNode cache = valuesNode.SelectSingleNode("c:numRef/c:numCache", xmlManager);
            XmlElement valCountNode = (XmlElement)cache.SelectSingleNode("c:ptCount", xmlManager);
            valCountNode.SetAttribute("val", values.Count.ToString());

            XmlElement nodeTemplate = ser_xml.OwnerDocument.CreateElement("node");
            nodeTemplate.InnerXml = Resource1.empty_chartSeriesItem;

            // значения для дефолтного элемента, потом добавляем остальные
            XmlNode newValueNode = nodeTemplate.SelectSingleNode("c:seriesItem/c:pt", xmlManager);

            // остальные значения диаграммы
            for (int i = 0; i < values.Count; i++)
            {
                XmlNode newItem = newValueNode.Clone();
                XmlElement tmp = (XmlElement)newItem;
                tmp.SetAttribute("idx", i.ToString());
                XmlNode nextValue = newItem.SelectSingleNode("c:v", xmlManager);
                nextValue.InnerText = values[i].ToString();
                cache.AppendChild(newItem);
            }

        }

        public void DeleteValues()
        {
            XmlNode mainNode = ser_xml.SelectSingleNode("c:val | c:bubbleSize | c:yVal", xmlManager);
            XmlNode cacheNode = mainNode.SelectSingleNode("c:numRef/c:numCache", xmlManager);
            XmlElement valCountNode = (XmlElement)cacheNode.SelectSingleNode("c:ptCount", xmlManager);
            valCountNode.SetAttribute("val", "0");
            XmlNodeList valuesNodeList = cacheNode.SelectNodes("c:pt", xmlManager);
            for (int i = 0; i < valuesNodeList.Count; i++)
            {
                cacheNode.RemoveChild(valuesNodeList[i]);
            }
        }

        public string name
        {
            get
            {
                string retVal = "";
                if (ser_xml != null)
                {
                    XmlElement name = (XmlElement)ser_xml.SelectSingleNode("c:tx/c:v", xmlManager);
                    if (name != null)
                        retVal = name.InnerText;
                }
                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value) && (ser_xml != null))
                {
                    XmlElement name = (XmlElement)ser_xml.SelectSingleNode("c:tx/c:v", xmlManager);
                    if (name != null)
                        name.InnerText = value;
                }
            }
        }

        public string markerType
        {
            get
            {
                string retVal = "";
                if (ser_xml != null)
                {
                    XmlElement marker = (XmlElement)ser_xml.SelectSingleNode("c:marker/c:symbol", xmlManager);
                    if (marker != null)
                        retVal = marker.GetAttribute("val");
                }
                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value) && (ser_xml != null))
                {
                    XmlElement marker = (XmlElement)ser_xml.SelectSingleNode("c:marker/c:symbol", xmlManager);
                    if (marker != null)
                        marker.SetAttribute("val", value);
                }
            }
        }

        public int markerSize
        {
            get
            {
                int retVal = 0;
                if (ser_xml != null)
                {
                    XmlElement marker = (XmlElement)ser_xml.SelectSingleNode("c:marker/c:size", xmlManager);
                    if (marker != null)
                        retVal = int.Parse(marker.GetAttribute("val"));
                }
                return retVal;
            }
            set
            {
                if (ser_xml != null)
                {
                    XmlElement marker = (XmlElement)ser_xml.SelectSingleNode("c:marker/c:size", xmlManager);
                    if (marker != null)
                        marker.SetAttribute("val", value.ToString());
                }
            }
        }

        public string color
        {
            get
            {
                string retVal = "";
                if (ser_xml != null)
                {
                    XmlElement color = (XmlElement)ser_xml.SelectSingleNode("c:spPr/a:solidFill/a:srgbClr", xmlManager);
                    if (color != null)
                        retVal = color.GetAttribute("val");
                }
                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value) && (ser_xml != null))
                {
                    XmlElement color = (XmlElement)ser_xml.SelectSingleNode("c:spPr/a:solidFill/a:srgbClr", xmlManager);
                    if (color != null)
                        color.SetAttribute("val", value);
                }
            }
        }

        public bool showVal
        {
            get
            {
                XmlElement show = (XmlElement)ser_xml.SelectSingleNode("c:dLbls/c:showVal", xmlManager);
                if (show != null)
                    return Misc.StringToBool(show.GetAttribute("val"));

                return false; // default
            }
            set
            {
                if (value)
                {
                    XmlElement show = (XmlElement)ser_xml.SelectSingleNode("c:dLbls/c:showVal", xmlManager);
                    if (show != null)
                        show.SetAttribute("val", "1");
                }
            }
        }

        public string borderColor
        {
            get
            {
                string retVal = "";
                if (ser_xml != null)
                {
                    XmlElement color = (XmlElement)ser_xml.SelectSingleNode("c:spPr/a:ln/a:solidFill/a:srgbClr", xmlManager);
                    if (color != null)
                        retVal = color.GetAttribute("val");
                }
                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value) && (ser_xml != null))
                {
                    XmlElement color = (XmlElement)ser_xml.SelectSingleNode("c:spPr/a:ln/a:solidFill/a:srgbClr", xmlManager);
                    if (color != null)
                        color.SetAttribute("val", value);
                }
            }
        }

        public string interval
        {
            get
            {
                string retVal = "";
                if (ser_xml != null)
                {
                    XmlNode valuesNode = ser_xml.SelectSingleNode("c:val | c:bubbleSize | c:yVal", xmlManager);
                    XmlNode intervalNode = valuesNode.SelectSingleNode("c:numRef/c:f", xmlManager);

                    if (intervalNode != null)
                        retVal = intervalNode.InnerText;
                }
                return retVal;
            }
            set
            {
                if (!string.IsNullOrEmpty(value) && (ser_xml != null))
                {
                    XmlNode valuesNode = ser_xml.SelectSingleNode("c:val | c:bubbleSize | c:yVal", xmlManager);
                    XmlNode intervalNode = valuesNode.SelectSingleNode("c:numRef/c:f", xmlManager);

                    if (intervalNode != null)
                        intervalNode.InnerText = value;
                }
            }
        }

        public Worksheet ReferredSheet
        {
            get
            {
                parceLocation();
                return m_oReferredSheet;
            }
            set
            {
                m_oReferredSheet = value;
            }
        }
        public Cells.CellRange ReferredRange
        {
            get
            {
                parceLocation();
                return new Cells.CellRange(m_strReferredRange, this.basicChart.chart.parent_sheet.Workbook);
            }
            set
            {
            }
        }

        private void parceLocation()
        {
            XmlNode valuesNode = ser_xml.SelectSingleNode("c:val | c:bubbleSize | c:yVal", xmlManager);
            XmlNode oIntervalNode = valuesNode.SelectSingleNode("c:numRef/c:f", xmlManager);

            string strLocation = "";
            if (oIntervalNode != null)
                strLocation = oIntervalNode.InnerText;
            int start_pos = 0;
            Worksheet ws_from;
            Worksheet ws_to;

            if (Calculation.Formulas.ParsedFormula.extract_3D_part(strLocation, ref start_pos, out ws_from, out ws_to, this.basicChart.chart.parent_sheet.Workbook))
            {
                m_oReferredSheet = ws_from;
                m_strReferredRange = strLocation.Substring(start_pos);
            }
            else
            {
                // разбиваем на отдельные ячейки
                //string[] spl = strLocation.Split(new char[]{','});
                //strLocation = "";
                //for (int i = 0; i < spl.Length; i++)
                //{
                //    strLocation
                //} 
            }
        }

        public void ChangeValues(List<string> cells, Workbook wb)
        {           
            for (int i = 0; i < cells.Count; i++)
            {
                if (ReferredRange.Contains(new Cells.Cell(cells[i])))
                {
                    Range range = new Range(cells[i], wb.ActiveWorksheet);
                    XmlNode valuesNode = ser_xml.SelectSingleNode("c:val | c:bubbleSize | c:yVal", xmlManager);

                    XmlNode numRef = valuesNode.SelectSingleNode("c:numRef", xmlManager);
                    XmlNode numCache = numRef.SelectSingleNode("c:numCache", xmlManager);

                    if (numCache == null)  // вставляем кеш
                    {
                        List<double> listValues = GetValues();  // по интервалу

                        numRef.InnerXml += "<c:numCache><c:formatCode>General</c:formatCode><c:ptCount val='1'/><c:pt idx='0'><c:v>0</c:v></c:pt></c:numCache>";
                        numCache = numRef.SelectSingleNode("c:numCache", xmlManager);
                        
                        InsertValues(listValues);
                    }

                    XmlNodeList values = numCache.SelectNodes("c:pt/c:v", xmlManager);

                    int index = ReferredRange.CellIndex(new Cells.Cell(cells[i]));
                    double newValue = 0;
                    if (double.TryParse(range.DispValue.ToString(), out newValue))
                        values[index].InnerText = newValue.ToString();
                    else // пробуем работать с $, €, р, £, ¥
                    {                           
                        string sOutVal = "";
                        string sNumFormat = "";
                        
                        int m_cnInfo = 0x409;
                        CultureInfo oCulture = new CultureInfo(m_cnInfo);

                        //Scientific
                        Match oScientifictMatch = Regex.Match(range.DispValue.ToString(), @"^([+-]?)(\d+|\d+\.\d*|\d*\.\d+)[Ee]([+-]?)(\d+)$");
                        if (true == oScientifictMatch.Success)
                        {
                            string sSing1 = oScientifictMatch.Groups[1].Value;
                            string sV1 = oScientifictMatch.Groups[2].Value;
                            string sSing2 = oScientifictMatch.Groups[3].Value;
                            string sV2 = oScientifictMatch.Groups[4].Value;
                            double dVal = double.Parse(sV1, System.Globalization.NumberStyles.Float, oCulture);
                            if ("-" == sSing1)
                                dVal = -dVal;
                            int nCount = int.Parse(sV2);
                            for (int k = 0; k < nCount; k++)
                            {
                                if ("-" == sSing2)
                                    dVal /= 10;
                                else
                                    dVal *= 10;
                            }
                            sOutVal = dVal.ToString(oCulture);
                            
                            if (double.TryParse(sOutVal, out newValue))
                                values[index].InnerText = sOutVal;
                            else
                                values[index].InnerText = "0";
                            return;
                        }

                        //проценты
                        Match oPersentMatch = Regex.Match(range.DispValue.ToString(), @"^([+-]?)(\d+|\d+\.\d*|\d*\.\d+)%$");
                        if (true == oPersentMatch.Success)
                        {
                            string sSing1 = oPersentMatch.Groups[1].Value;
                            string sV1 = oPersentMatch.Groups[2].Value;
                            double dVal = double.Parse(sV1, System.Globalization.NumberStyles.Float, oCulture);
                            if ("-" == sSing1)
                                dVal = -dVal;
                            dVal /= 100;
                            sOutVal = dVal.ToString(oCulture);

                            if (double.TryParse(sOutVal, out newValue))
                                values[index].InnerText = sOutVal;
                            else
                                values[index].InnerText = "0";
                            return;
                        }
                        //денежный
                        Match oMoneyMatch = Regex.Match(range.DispValue.ToString(), @"^([+-]?)(\D?)([+-]?)(\d+|(\d{1,2}(,\d{3})*)+|(\d{1,2}(,\d{3})*)+\.\d*|(\d{1,2}(,\d{3})*)*\.\d+)(\D?)$");
                        if (true == oMoneyMatch.Success)
                        {
                            string sSign1 = oMoneyMatch.Groups[1].Value;
                            string sSign = oMoneyMatch.Groups[2].Value;
                            string sSign2 = oMoneyMatch.Groups[3].Value;
                            string sValue = oMoneyMatch.Groups[4].Value.Replace(",", "");
                            string sSign5 = oMoneyMatch.Groups[11].Value;
                            double dVal = double.Parse(sValue, System.Globalization.NumberStyles.Float, oCulture);
                            if ("-" == sSign1 || "-" == sSign2)
                                dVal = -dVal;

                            if (sSign.Length == 0)
                            {
                                sOutVal = dVal.ToString(oCulture);

                                if (double.TryParse(sOutVal, out newValue))
                                    values[index].InnerText = sOutVal;
                                else
                                    values[index].InnerText = "0";
                                return;
                            }
                            else
                            {
                                if (sSign5 == "")
                                {
                                    if ('$' == sSign[0])
                                        sNumFormat = "$#,##0.00";
                                    else if (8364 == (int)sSign[0])
                                        sNumFormat = "€#,##0.00";
                                    else if (163 == (int)sSign[0])
                                        sNumFormat = "&pound;#,##0.00";
                                    else if (165 == (int)sSign[0])
                                        sNumFormat = "&yen;#,##0.00";
                                }
                                else
                                    if (sSign == "" && sSign5.Equals("р"))
                                    {
                                        sNumFormat = "#,##0.00р.";
                                    }
                                if ("" != sNumFormat)
                                {
                                    sOutVal = dVal.ToString(oCulture);

                                    if (double.TryParse(sOutVal, out newValue))
                                        values[index].InnerText = sOutVal;
                                    else
                                        values[index].InnerText = "0";
                                }
                                return;
                            }
                        }
                        values[index].InnerText = "0";
                    }
                }
            }
        }

        public void ClearAutoFilterValues(List<string> cells, Workbook wb)
        {
            // получение значений интервала
            Range range = new Range(ReferredRange, wb.ActiveWorksheet);
            List<string> chartCells = range.ExistentCells();
            List<double> newValueList = new List<double>();

            for (int i = 0; i < chartCells.Count; i++)
            {
                if (!cells.Contains(chartCells[i]))
                {
                    range = new Range(chartCells[i], wb.ActiveWorksheet);
                    double newValue = 0;
                    double.TryParse(range.DispValue.ToString(), out newValue);
                    newValueList.Add(newValue);
                }
            }
            InsertValues(newValueList);
        }
    }
}