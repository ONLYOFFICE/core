using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor
{
    public struct AutoFilterElementSendData
    {
        /// <summary>
        /// Видимые элементы (только для _type = 0)
        /// </summary>
        public List<string> _arrVisibleElementsArray;
        /// <summary>
        /// Тип автофильтра
        /// </summary>
        public int _type;
        /// <summary>
        /// Первое условие сравнения (только для _type = 1)
        /// </summary>
        public int _condition1;
        /// <summary>
        /// Первое значение, с которым сравниваем (только для _type = 1)
        /// </summary>
        public string _value1;
        /// <summary>
        /// Второе условие сравнения (только для _type = 1)
        /// </summary>
        public int _condition2;
        /// <summary>
        /// Второе значение, с которым сравниваем (только для _type = 1)
        /// </summary>
        public string _value2;
        /// <summary>
        /// Оператор для двух сравнений (или/и)
        /// 0 - Or
        /// 1 - And
        /// (только для _type = 1)
        /// </summary>
        public int _operator;

        /// <summary>
        /// Индекс колонки, по которой автофильтр
        /// </summary>
        public int _colId;
    };

    public enum filterOperator
    {
        equal = 1,
        notEqual = 2,
        greaterThan = 3,
        greaterThanOrEqual = 4,
        lessThan = 5,
        lessThanOrEqual = 6
    }

    /// <summary>
    /// AutoFilter temporarily hides rows based on a filter criteria,
    /// which is applied column by column to a table of data  in the worksheet.
    /// This collection expresses AutoFilter settings. 
    /// </summary>
    public class AutoFilter
    {
        public AutoFilter(Worksheet worksheet, XmlNode xml_table_file, XmlNode xml_worksheet)
        {
            this.worksheet = worksheet;
            this.xml_worksheet = xml_worksheet;
            this.ns_manager = worksheet.Workbook.NamespaceManager;
            this.m_oRefRange = Cells.CellRange.Empty;

            this.autofilter_xml = xml_table_file.SelectSingleNode("main:autoFilter", ns_manager);
            if (null != this.autofilter_xml)
            {
                m_oRefRange = new AVSOfficeEWSEditor.Editor.Cells.CellRange(autofilter_xml.Attributes["ref"].Value, this.worksheet.Workbook);

                foreach (XmlNode oXmlNodeFilterColumn in autofilter_xml.ChildNodes)
                {
                    if ("filterColumn" == oXmlNodeFilterColumn.Name)
                    {
                        FilterColumn oFilterColumn = new FilterColumn(oXmlNodeFilterColumn, ns_manager);
                        m_arrFilterColumns.Add(oFilterColumn);
                    }
                }
            }
        }

        public AutoFilter(Worksheet worksheet, XmlNode xml_table_file, XmlNode xml_worksheet, AutoFilter oAutoFilter)
        {
            this.worksheet = worksheet;
            this.xml_worksheet = xml_worksheet;
            this.ns_manager = worksheet.Workbook.NamespaceManager;

            this.m_oRefRange = oAutoFilter.m_oRefRange;
            this.autofilter_xml = xml_table_file.SelectSingleNode("main:autoFilter", ns_manager);
            this.m_arrFilterColumns.AddRange(oAutoFilter.m_arrFilterColumns);
        }

        private AutoFilter(Worksheet worksheet, XmlNode xml_table_file, XmlNode xml_worksheet, Cells.CellRange oFilterRange)
        {
            this.worksheet = worksheet;
            this.xml_worksheet = xml_worksheet;
            this.ns_manager = worksheet.Workbook.NamespaceManager;
            this.m_oRefRange = oFilterRange;

            this.autofilter_xml = xml_table_file.SelectSingleNode("main:autoFilter", ns_manager);
        }

        public XmlNode AutoFilterXml
        {
            get { return autofilter_xml; }
            set { autofilter_xml = value.Clone(); }
        }

        public static AutoFilter CreateNew(Worksheet worksheet, XmlNode xml_table_file, XmlNode xml_worksheet, Cells.CellRange oFilterRange)
        {
            return new AutoFilter(worksheet, xml_table_file, xml_worksheet, oFilterRange);
        }

        public Dictionary<int, bool> ChangeAutoFilter(AutoFilterElementSendData oChangeObject)
        {
            Dictionary<int, bool> arrRows = new Dictionary<int, bool>();

            XmlNodeList oXmlNodesRowList = xml_worksheet.SelectNodes("main:row[@r > " + m_oRefRange.RowFirst + " and @r <= " + m_oRefRange.RowLast + "]", ns_manager);
            string strColumn = Helpers.ReferencesFormat.column2str(m_oRefRange.ColumnFirst + oChangeObject._colId, true);

            FilterColumn oFilterColumn = null;
            int nIndex = 0;
            for (; nIndex < m_arrFilterColumns.Count; ++nIndex)
            {
                if (m_arrFilterColumns[nIndex].colId == oChangeObject._colId)
                {
                    oFilterColumn = m_arrFilterColumns[nIndex];
                    break;
                }
            }

            if (null == oFilterColumn)
            {
                oFilterColumn = FilterColumn.CreateFilterColumn(oChangeObject._colId, autofilter_xml, ns_manager);
                m_arrFilterColumns.Add(oFilterColumn);
            }

            oFilterColumn.m_nFilterType = oChangeObject._type;
            switch (oChangeObject._type)
            {
                case 0:
                    oFilterColumn.m_oFilter = new Filters();
                    Filters oFilters = (Filters)oFilterColumn.m_oFilter;
                    if (null != oFilters)
                    {
                        oFilters.CreateFilters(autofilter_xml, ns_manager, oChangeObject._colId.ToString());
                        oFilters.FilterValues = oChangeObject._arrVisibleElementsArray;
                    }
                    break;
                case 1:
                    oFilterColumn.m_oFilter = new CustomFilters();
                    CustomFilters oCustomFilter = (CustomFilters)oFilterColumn.m_oFilter;
                    if (null != oCustomFilter)
                    {
                        oCustomFilter.CreateFilters(autofilter_xml, ns_manager, oChangeObject._colId.ToString());
                        oCustomFilter.ChangeFilter(oChangeObject);
                    }
                    break;
            }
            bool isAutoFilter = false;
            foreach (XmlElement oRowElement in oXmlNodesRowList)
            {
                try
                {
                    int row = System.Convert.ToInt32(oRowElement.GetAttribute("r"));
                    Range oRange = new Range(strColumn + row, worksheet);

                    string strValue = oRange.DispValue.ToString();

                    if (oFilterColumn.m_oFilter.isVisible(strValue))
                    {
                        oRowElement.RemoveAttribute("hidden");

                        arrRows.Add(row, false);
                    }
                    else
                    {
                        oRowElement.SetAttribute("hidden", "1");
                        isAutoFilter = true;

                        arrRows.Add(row, true);
                    }
                }
                catch { }
            }
            if (false == isAutoFilter && 0 == oFilterColumn.m_nFilterType)
            {
                oFilterColumn.RemoveFilterColumn();
                m_arrFilterColumns.RemoveAt(nIndex);
            }

            return arrRows;
        }

        private XmlNode autofilter_xml;

        /// <summary>
        /// Reference to the cell range to which the AutoFilter is applied.
        /// </summary>
        public Cells.CellRange m_oRefRange { get; private set; }
        /// <summary>
        /// Filters Columns
        /// </summary>
        public List<FilterColumn> m_arrFilterColumns = new List<FilterColumn>();

        private XmlNode xml_worksheet;
        private Worksheet worksheet;
        private XmlNamespaceManager ns_manager;
    }

    /// <summary>
    /// The filterColumn collection identifies a particular column in the AutoFilter
    /// range and specifies filter information  that has been applied to this column.
    /// If a column in the AutoFilter range has no criteria specified, then there is
    /// no corresponding filterColumn collection expressed for that column.
    /// </summary>
    public class FilterColumn
    {
        private FilterColumn(int nColId, XmlNode oXmlNodeAutoFilter, XmlNamespaceManager ns_manager)
        {
            colId = nColId;

            XmlElement oFilterColumn = oXmlNodeAutoFilter.OwnerDocument.CreateElement("filterColumn", ns_manager.LookupNamespace("main"));
            oFilterColumn.SetAttribute("colId", nColId.ToString());
            m_oXmlNodeFilterColumn = oFilterColumn;

            oXmlNodeAutoFilter.AppendChild(m_oXmlNodeFilterColumn);
        }
        internal FilterColumn(XmlNode oXmlNodeFilterColumn, XmlNamespaceManager ns_manager)
        {
            m_oXmlNodeFilterColumn = oXmlNodeFilterColumn;

            colId = System.Convert.ToInt32(oXmlNodeFilterColumn.Attributes["colId"].Value);
            foreach (XmlNode oXmlNodeFilter in oXmlNodeFilterColumn.ChildNodes)
            {
                switch (oXmlNodeFilter.Name)
                {
                    case "filters":
                        m_oFilter = new Filters(oXmlNodeFilter, ns_manager);
                        m_nFilterType = 0;
                        break;
                    case "colorFilter":
                        m_oFilter = new ColorFilter(oXmlNodeFilter);
                        break;
                    case "customFilters":
                        m_oFilter = new CustomFilters(oXmlNodeFilter);
                        m_nFilterType = 1;
                        break;
                }
            }
        }

        public static FilterColumn CreateFilterColumn(int nColId, XmlNode oXmlNodeAutoFilter, XmlNamespaceManager ns_manager)
        {
            FilterColumn oFilterColumn = new FilterColumn(nColId, oXmlNodeAutoFilter, ns_manager);

            return oFilterColumn;
        }

        public void RemoveFilterColumn()
        {
            m_oXmlNodeFilterColumn.ParentNode.RemoveChild(m_oXmlNodeFilterColumn);
        }

        public Filter m_oFilter = null;
        public int colId = -1;

        public int m_nFilterType = -1;

        private XmlNode m_oXmlNodeFilterColumn;
    }

    public abstract class Filter
    {
        public abstract bool isVisible(string strValue);

        protected XmlNode m_oXmlFilterNode;
    }
    /// <summary>
    /// When multiple values are chosen to filter by, or when a group
    /// of date values are chosen to filter by, this element  groups those criteria together.
    /// </summary>
    public class Filters : Filter
    {
        internal Filters()
        {
        }

        internal Filters(XmlNode oXmlNodeFiltersColumn, XmlNamespaceManager ns_manager)
        {
            m_oXmlFilterNode = oXmlNodeFiltersColumn;

            foreach (XmlNode oFilterNode in oXmlNodeFiltersColumn.ChildNodes)
            {
                if (oFilterNode.Name == "filter")
                    m_arrFilterValues.Add(oFilterNode.Attributes["val"].Value);
            }
        }

        internal void CreateFilters(XmlNode autofilter_xml, XmlNamespaceManager ns_manager, string strColId)
        {
            this.ns_manager = ns_manager;
            m_oXmlFilterNode = autofilter_xml.SelectSingleNode("main:filterColumn[@colId = '" + strColId + "']", ns_manager);
            if (null == m_oXmlFilterNode)
            {
                m_oXmlFilterNode = autofilter_xml.OwnerDocument.CreateElement("filterColumn", ns_manager.LookupNamespace("main"));
                XmlElement oElem = (XmlElement)m_oXmlFilterNode;
                oElem.SetAttribute("colId", strColId);
                autofilter_xml.AppendChild(m_oXmlFilterNode);
            }
        }

        public List<string> FilterValues
        {
            get { return m_arrFilterValues; }
            set
            {
                foreach (XmlNode oChildNode in m_oXmlFilterNode.ChildNodes)
                {
                    m_oXmlFilterNode.RemoveChild(oChildNode);
                }

                XmlNode oNode = m_oXmlFilterNode.OwnerDocument.CreateElement("filters", ns_manager.LookupNamespace("main"));

                if (null == value)
                {
                    m_arrFilterValues = null;
                    return;
                }

                value.Sort();

                foreach (string strValue in value)
                {
                    XmlElement oElementValue = m_oXmlFilterNode.OwnerDocument.CreateElement("filter", this.ns_manager.LookupNamespace("main"));
                    oElementValue.SetAttribute("val", strValue);
                    oNode.AppendChild(oElementValue);

                    m_arrFilterValues.Add(strValue);
                }

                m_oXmlFilterNode.AppendChild(oNode);
            }
        }

        public override bool isVisible(string strValue)
        {
            if (null == m_arrFilterValues)
                return true;
            return (-1 != m_arrFilterValues.IndexOf(strValue));
        }

        private List<string> m_arrFilterValues = new List<string>();
        private XmlNamespaceManager ns_manager;
    }
    /// <summary>
    /// This element specifies the color to filter by and whether to use the cell's
    /// fill or font color in the filter criteria. If  the cell's font or fill color
    /// does not match the color specified in the criteria, the rows corresponding
    /// to those cells  are hidden from view.
    /// </summary>
    public class ColorFilter : Filter
    {
        internal ColorFilter(XmlNode oXmlNodeColorFilterColumn)
        {
            m_oXmlFilterNode = oXmlNodeColorFilterColumn;

            if (null != oXmlNodeColorFilterColumn.Attributes["dxfId"])
                dxfId = System.Convert.ToInt32(oXmlNodeColorFilterColumn.Attributes["dxfId"].Value);
            if (null != oXmlNodeColorFilterColumn.Attributes["cellColor"])
                cellColor = ("1" == oXmlNodeColorFilterColumn.Attributes["cellColor"].Value);
        }

        public override bool isVisible(string strValue)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Id of differential format record (dxf) in the Styles Part which expresses the color value to  filter by.
        /// </summary>
        public int dxfId  { get; private set; }

        /// <summary>
        /// Flag indicating whether or not to filter by the cell's fill color.
        /// '1' indicates to filter by cell  fill.
        /// '0' indicates to filter by the cell's font color.
        /// For rich text in cells, if the color specified appears in the cell at all,
        /// it shall be included in  the filter.
        /// </summary>
        public bool cellColor { get; private set; }
    }

    /// <summary>
    /// When there is more than one custom filter criteria to apply (an 'and' or 'or'
    /// joining two criteria), then this  element groups the customFilter elements together.
    /// </summary>
    public class CustomFilters : Filter
    {
        internal CustomFilters()
        {
        }
        internal CustomFilters(XmlNode oXmlNodeCustomFiltersColumn)
        {
            m_oXmlFilterNode = oXmlNodeCustomFiltersColumn;

            XmlAttribute oAnd = oXmlNodeCustomFiltersColumn.Attributes["and"];
            if (null == oAnd)
                and = -1;
            else
                and = System.Convert.ToInt32(oAnd.Value);

            if (1 > oXmlNodeCustomFiltersColumn.ChildNodes.Count)
                return;

            oCustomFilter1 = new CustomFilter(oXmlNodeCustomFiltersColumn.ChildNodes[0]);

            if (2 == oXmlNodeCustomFiltersColumn.ChildNodes.Count)
                oCustomFilter2 = new CustomFilter(oXmlNodeCustomFiltersColumn.ChildNodes[1]);
        }

        public override bool isVisible(string strValue)
        {
            switch (this.and)
            {
                case -1:
                    return oCustomFilter1.isCondition(strValue);
                case 0: // Or
                    if (null != oCustomFilter2)
                        return oCustomFilter1.isCondition(strValue) || oCustomFilter2.isCondition(strValue);
                    else
                        return oCustomFilter1.isCondition(strValue);
                case 1: // And
                    if (null != oCustomFilter2)
                        return oCustomFilter1.isCondition(strValue) && oCustomFilter2.isCondition(strValue);
                    else
                        return oCustomFilter1.isCondition(strValue);
            }

            return false;
        }

        internal void CreateFilters(XmlNode autofilter_xml, XmlNamespaceManager ns_manager, string strColId)
        {
            this.ns_manager = ns_manager;
            m_oXmlFilterNode = autofilter_xml.SelectSingleNode("main:filterColumn[@colId = '" + strColId + "']", ns_manager);
            if (null == m_oXmlFilterNode)
            {
                m_oXmlFilterNode = autofilter_xml.OwnerDocument.CreateElement("filterColumn", ns_manager.LookupNamespace("main"));
                XmlElement oElem = (XmlElement)m_oXmlFilterNode;
                oElem.SetAttribute("colId", strColId);
                autofilter_xml.AppendChild(m_oXmlFilterNode);
            }
        }

        internal void ChangeFilter(AutoFilterElementSendData oChangeObject)
        {
            this.and = oChangeObject._operator;

            this.oCustomFilter1 = new CustomFilter();
            this.oCustomFilter1.operatorFilter = (filterOperator)oChangeObject._condition1;
            this.oCustomFilter1.val = oChangeObject._value1;


            if (-1 != this.and && 0 != oChangeObject._condition2)
            {
                this.oCustomFilter2 = new CustomFilter();
                this.oCustomFilter2.operatorFilter = (filterOperator)oChangeObject._condition2;
                this.oCustomFilter2.val = oChangeObject._value2;
            }
            else
                this.and = -1;

            foreach (XmlNode oChildNode in m_oXmlFilterNode.ChildNodes)
            {
                m_oXmlFilterNode.RemoveChild(oChildNode);
            }
            XmlElement oNode = m_oXmlFilterNode.OwnerDocument.CreateElement("customFilters", ns_manager.LookupNamespace("main"));
            if (-1 != this.and)
                oNode.SetAttribute("and", this.and.ToString());

            if (null != oCustomFilter1)
            {
                XmlElement oElementValue1 = m_oXmlFilterNode.OwnerDocument.CreateElement("customFilter", this.ns_manager.LookupNamespace("main"));
                oElementValue1.SetAttribute("operator", oCustomFilter1.operatorFilter.ToString());
                oElementValue1.SetAttribute("val", oCustomFilter1.val.ToString());
                oNode.AppendChild(oElementValue1);
            }
            if (null != oCustomFilter2)
            {
                XmlElement oElementValue2 = m_oXmlFilterNode.OwnerDocument.CreateElement("customFilter", this.ns_manager.LookupNamespace("main"));
                oElementValue2.SetAttribute("operator", oCustomFilter2.operatorFilter.ToString());
                oElementValue2.SetAttribute("val", oCustomFilter2.val.ToString());
                oNode.AppendChild(oElementValue2);
            }

            m_oXmlFilterNode.AppendChild(oNode);
        }

        public int and { get; set; }

        public CustomFilter oCustomFilter1 = null;
        public CustomFilter oCustomFilter2 = null;

        private XmlNamespaceManager ns_manager;
    }

    /// <summary>
    /// A custom AutoFilter specifies an operator and a value.
    /// There can be at most two customFilters specified, and in
    /// that case the parent element specifies whether the two conditions
    /// are joined by 'and' or 'or'. For any cells  whose values do not meet
    /// the specified criteria, the corresponding rows shall be hidden from view when the  fitler is applied. 
    /// </summary>
    public class CustomFilter
    {
        internal CustomFilter()
        {
        }

        internal CustomFilter(XmlNode oXmlNodeCustomFilter)
        {
            m_oXmlNodeCustomFilter = oXmlNodeCustomFilter;

            string strValOperator = oXmlNodeCustomFilter.Attributes["operator"].Value;

            switch (strValOperator)
            {
                case "equal":
                    operatorFilter = filterOperator.equal;
                    break;
                case "notEqual":
                    operatorFilter = filterOperator.notEqual;
                    break;
                case "greaterThan":
                    operatorFilter = filterOperator.greaterThan;
                    break;
                case "greaterThanOrEqual":
                    operatorFilter = filterOperator.greaterThanOrEqual;
                    break;
                case "lessThan":
                    operatorFilter = filterOperator.lessThan;
                    break;
                case "lessThanOrEqual":
                    operatorFilter = filterOperator.lessThanOrEqual;
                    break;
            }

            val = oXmlNodeCustomFilter.Attributes["val"].Value;
        }

        public bool isCondition(string strValue)
        {
            bool aIsDigit = false;
            bool bIsDigit = false;

            int aDigit = 0;
            int bDigit = 0;

            try
            {
                aDigit = System.Convert.ToInt32(strValue);
            }
            catch
            {
                aIsDigit = false;
            }
            try
            {
                bDigit = System.Convert.ToInt32(val);
            }
            catch
            {
                bIsDigit = false;
            }

            if (true == bIsDigit)
            {
                if (false == aIsDigit && 1 <= (int)operatorFilter && (int)operatorFilter >= 6)
                    return false;
            }
            if (true == aIsDigit)
            {
                if (false == bIsDigit && 1 <= (int)operatorFilter && (int)operatorFilter >= 6)
                    return false;
            }

            switch (operatorFilter)
            {
                case filterOperator.equal:
                    return strValue == val;
                case filterOperator.notEqual:
                    return strValue != val;
                case filterOperator.greaterThan:
                    return aDigit > bDigit;
                case filterOperator.greaterThanOrEqual:
                    return aDigit >= bDigit;
                case filterOperator.lessThan:
                    return aDigit < bDigit;
                case filterOperator.lessThanOrEqual:
                    return aDigit <= bDigit;
            }

            return false;
        }

        /// <summary>
        /// Operator used by the filter comparison.
        /// equal, lessThan, lessThanOrEqual, notEqual, greaterThanOrEqual, greaterThan
        /// </summary>
        public filterOperator operatorFilter { get; set; }

        /// <summary>
        /// Top or bottom value used in the filter criteria.
        /// </summary>
        public string val { get; set; }

        private XmlNode m_oXmlNodeCustomFilter;
    }
}
