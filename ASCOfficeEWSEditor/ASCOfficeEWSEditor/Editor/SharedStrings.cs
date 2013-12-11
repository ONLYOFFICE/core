using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.XPath;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor
{
    internal class SharedStrings
    {
        public SharedStrings(OOX.ContentTypes content_types, OOX.Relationships relationships, Workbook workbook)
        {
            this.workbook = workbook;
            sst_file = new OOX.SharedStringsFile(content_types, relationships, workbook);
            ns_manager = workbook.NamespaceManager;
        }

        public string GetString(int id)
        {
            XmlElement sst_tag = sst_file.GetSST();
            if(sst_tag == null)
            {
                return null;
            }

            XmlNode ssi_text = sst_tag.SelectSingleNode("main:si[position() = " + (id + 1).ToString() + "]", ns_manager);
            if (ssi_text == null)
            {
                return null;
            }
            return ssi_text.InnerText;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="ssi"></param>
        /// <param name="used_times">Whether the passed string is going to be used only once or many times</param>
        /// <returns></returns>
        public int SetString(string ssi, int used_times)
        {
            XmlElement sst_tag = sst_file.GetSSTOrCreate();
            int exact_si_index = FindExactString(ssi);
            if (exact_si_index == -1)
            {
                XmlElement si_tag = sst_tag.OwnerDocument.CreateElement("si", ns_manager.LookupNamespace("main"));
                XmlElement t_tag = sst_tag.OwnerDocument.CreateElement("t", ns_manager.LookupNamespace("main"));
                t_tag.InnerText = ssi;
                si_tag.AppendChild(t_tag);
                sst_tag.AppendChild(si_tag);
                XPathNavigator navigator = sst_tag.CreateNavigator();

                UpdateUsageCounter(used_times);
                return UpdateTotalCounter() - 1;
            }
            else // the string with the same text already exists
            {
                if (!sst_tag.HasAttribute("uniqueCount"))
                {
                    UpdateTotalCounter();
                }
                UpdateUsageCounter(used_times);
                return exact_si_index;
            }
        }

        private int FindExactString(string ssi)
        {
            XmlElement sst_tag = sst_file.GetSSTOrCreate();

            int counter = 0;
            foreach (XmlNode si_tag in sst_tag.SelectNodes("main:si", ns_manager))
            {
                XmlNode t_tag = si_tag.SelectSingleNode("main:t", ns_manager);
                if(t_tag != null && t_tag.InnerText == ssi)
                {
                    return counter;
                }
                counter++;
            }

            return -1;
        }


        private int UpdateTotalCounter()
        {
            XmlElement sst_tag = sst_file.GetSSTOrCreate();
            XPathNavigator navigator2 = sst_tag.CreateNavigator();
            int uniqueCount = (int)(double)navigator2.Evaluate("count(main:si)", ns_manager);
            sst_tag.SetAttribute("uniqueCount", uniqueCount.ToString());
            return uniqueCount;
        }

        public int UpdateUsageCounter(int correction_value)
        {
            XmlElement sst_tag = sst_file.GetSSTOrCreate();
            int count = correction_value;
            if (sst_tag.HasAttribute("count"))
            {
                count += XMLInt.ParseOrDefaultAndReport(sst_tag.GetAttribute("count"), 0, workbook, "sst/@count");
            }
            sst_tag.SetAttribute("count", count.ToString());
            return count;
        }

        private OOX.SharedStringsFile sst_file;
        #region Tree references
        private Workbook workbook;
        #endregion
        XmlNamespaceManager ns_manager;
    }
}
