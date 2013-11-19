using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class ColsWalker
    {
        public ColsWalker(int colFirst, int colLast, Worksheet parent_sheet)
        {
            this.colFirst = colFirst;
            this.colLast = colLast;
            this.parent_sheet = parent_sheet;
            this.sheetData_tag = parent_sheet.SheetData;

            ns_manager = parent_sheet.Workbook.NamespaceManager;
        }


        /// <summary>
        /// Create absent cols in the range and update all the cols according to the passed setter.
        /// </summary>
        /// <param name="setter"></param>
        public void SetCols(IColsSetter setter)
        {
            string columnFirst_num_str = colFirst.ToString();
            string columnLast_num_str = colLast.ToString();
            XmlNode cols_node = (XmlElement)sheetData_tag.SelectSingleNode("../main:cols", ns_manager);
            if (null == cols_node)
            {
                cols_node = sheetData_tag.OwnerDocument.CreateElement("cols", ns_manager.LookupNamespace("main"));
                sheetData_tag.ParentNode.InsertBefore(cols_node, sheetData_tag);
            }
            foreach (XmlElement col_elem in sheetData_tag.SelectNodes("../main:cols/main:col", ns_manager))
            {
                int min = XMLInt.ParseOrDefaultAndReport(col_elem.GetAttribute("min"), 1, parent_sheet.Workbook, "col/@min");
                int max = XMLInt.ParseOrDefaultAndReport(col_elem.GetAttribute("max"), Helpers.ReferencesFormat.maxCol, parent_sheet.Workbook, "col/@max");
                if (min > max)
                {
                    continue;
                }
                if (max < colFirst) // 1
                {
                    continue;
                }
                else if (min < colFirst && max <= colLast) // obvious: max >= columnFirst // 2
                {
                    if(!setter.WouldLeftTheSame(col_elem))
                    {
                        XmlElement right_part = (XmlElement)col_elem.CloneNode(false);
                        col_elem.SetAttribute("max", (colFirst - 1).ToString());
                        right_part.SetAttribute("min", columnFirst_num_str);
                        cols_node.InsertAfter(right_part, col_elem);
                    }
                }
                else if (min < colFirst && max > colLast) // 5
                {
                    if (!setter.WouldLeftTheSame(col_elem))
                    {
                        XmlElement right_part = (XmlElement)col_elem.CloneNode(false);
                        XmlElement middle_part = (XmlElement)col_elem.CloneNode(false);
                        col_elem.SetAttribute("max", (colFirst - 1).ToString());
                        middle_part.SetAttribute("min", columnFirst_num_str);
                        middle_part.SetAttribute("max", columnLast_num_str);
                        right_part.SetAttribute("min", (colFirst + 1).ToString());
                        cols_node.InsertAfter(middle_part, col_elem);
                        cols_node.InsertAfter(right_part, middle_part);
                    }
                }
                else if (min <= colLast && max > colLast) // 3
                {
                    if (!setter.WouldLeftTheSame(col_elem))
                    {
                        XmlElement left_part = (XmlElement)col_elem.CloneNode(false);
                        col_elem.SetAttribute("min", (colLast + 1).ToString());
                        left_part.SetAttribute("max", columnLast_num_str);
                        cols_node.InsertBefore(left_part, col_elem);
                    }
                }
                else if (min <= colLast && max <= colLast) // 4
                {
                    continue;
                }
                else // 6
                {
                    continue;
                }

            }
            string standard_width_str = XMLDouble.ToString(parent_sheet.StandardWidth);
            int last_updated_column = colFirst - 1;
            foreach (XmlElement col_elem in sheetData_tag.SelectNodes("../main:cols/main:col[" +
                            "@min >= " + columnFirst_num_str + " and @min <= " + columnLast_num_str + " and " +
                            "@max >= " + columnFirst_num_str + " and @max <= " + columnLast_num_str + "]", ns_manager))
            {
                int min = XMLInt.ParseOrDefault(col_elem.GetAttribute("min"), 1);
                int max = XMLInt.ParseOrDefault(col_elem.GetAttribute("max"), Helpers.ReferencesFormat.maxCol);
                if (min > last_updated_column + 1)
                {
                    XmlElement col_ins = (XmlElement)sheetData_tag.OwnerDocument.CreateElement("col", ns_manager.LookupNamespace("main"));
                    col_ins.SetAttribute("min", (last_updated_column + 1).ToString());
                    col_ins.SetAttribute("max", (min - 1).ToString());
                    col_ins.SetAttribute("width", standard_width_str);

                    setter.Set(col_ins);

                    cols_node.InsertBefore(col_ins, col_elem);
                }
                setter.Set(col_elem);
                last_updated_column = max;
            }
            if (last_updated_column != colLast)
            {
                XmlElement col_ins = (XmlElement)sheetData_tag.OwnerDocument.CreateElement("col", ns_manager.LookupNamespace("main"));
                col_ins.SetAttribute("min", (last_updated_column + 1).ToString());
                col_ins.SetAttribute("max", columnLast_num_str);
                col_ins.SetAttribute("width", standard_width_str);

                setter.Set(col_ins);

                cols_node.InsertBefore(col_ins, cols_node.SelectSingleNode("main:col[@min > " + columnLast_num_str + "]", ns_manager));
            }
        }

        private int colFirst;
        private int colLast;
        private XmlNamespaceManager ns_manager;
        private XmlNode sheetData_tag;
        private Worksheet parent_sheet;
    }
}
