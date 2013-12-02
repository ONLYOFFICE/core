using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.FileFormats
{
    internal class XlsxSaver : ASCOfficeUtils.IRequestFileEvent
    {
        internal XlsxSaver(XmlDocument xml_source_document, List<byte[]> binary_data_storage)
        {
            this.xml_source_document = xml_source_document;
            this.binary_data_storage = binary_data_storage;
            xml_current_element = (XmlElement)xml_source_document.DocumentElement.FirstChild;
        }

        #region IRequestFileEvent Members

        public bool RequestFile(out string file_name, out Array arr)
        {
            if (xml_current_element == null)
            {
                file_name = "";
                arr = new byte[0];
                return false;
            }

            string node_type = xml_current_element.Name;
            string node_name = xml_current_element.GetAttribute("name");
            if ("dir" == node_type)
            {
                file_name = DetermineFilePathInXml(xml_current_element) + '/';
                arr = new byte[0];

                XmlNode first_child = xml_current_element.FirstChild;
                if(first_child != null)
                {
                    xml_current_element = (XmlElement)first_child;
                }
                else
                {
                    xml_current_element = Following(xml_current_element);
                }
                return true;
            }
            else if ("file" == node_type)
            {
                file_name = DetermineFilePathInXml(xml_current_element);

                string file_format = xml_current_element.GetAttribute("format");
                if (file_format == "bin")
                {
                    string bin_data_id_str = xml_current_element.GetAttribute("bin_data_id");
                    int bin_data_id = XMLInt.ParseOrDefault(bin_data_id_str, -1);
                    if(bin_data_id >= 0 && bin_data_id < binary_data_storage.Count)
                    {
                        arr = binary_data_storage[bin_data_id];
                    }
                    else // Wrong binary reference
                    {
                        xml_current_element = Following(xml_current_element);
                        return RequestFile(out file_name, out arr);
                    }
                }
                else
                {
                    using (MemoryStream mem_stream = new MemoryStream(initial_memory_stream_buffer))
                    {
                        using (XmlWriter writer = XmlWriter.Create(mem_stream))
                        {   
                            xml_current_element.WriteContentTo(writer);
                        }
                        arr = mem_stream.ToArray();
                    }
                }
                xml_current_element = Following(xml_current_element);
                return true;
            }
            else // Unknown type of storage
            {
                xml_current_element = Following(xml_current_element);
                return RequestFile(out file_name, out arr);
            }
        }

        #endregion

        private string DetermineFilePathInXml(XmlElement file_or_dir_xml)
        {
            string path = file_or_dir_xml.GetAttribute("name");
            file_or_dir_xml = (XmlElement)file_or_dir_xml.ParentNode;
            while(file_or_dir_xml.Name != "root")
            {
                string node_name = file_or_dir_xml.GetAttribute("name");
                path = node_name + '/' + path;
                file_or_dir_xml = (XmlElement)file_or_dir_xml.ParentNode;
            }
            return path;
        }

        private XmlElement Following(XmlNode file_or_dir_xml)
        {
            XmlNode cur_elem_xml = file_or_dir_xml;
            while (cur_elem_xml != null)
            {
                XmlNode next_sibling_xml = cur_elem_xml.SelectSingleNode("following-sibling::*");
                if (next_sibling_xml != null)
                {
                    return (XmlElement)next_sibling_xml;
                }
                cur_elem_xml = cur_elem_xml.ParentNode;
            }
            return null;
        }

        static internal void Save(XmlDocument xml_doc, string dir_path, List<byte[]> binary_data_storage)
        {
            Directory.CreateDirectory(dir_path);
            storeDir(xml_doc.DocumentElement, dir_path, binary_data_storage);
        }

        static private void storeDir(XmlElement cur_dir, string path_for_dir, List<byte[]> binary_data_storage)
        {
            if(null == cur_dir)
            {
	            return;
            }

	        string node_type = cur_dir.Name;
	        string node_name = cur_dir.GetAttribute("name");
	        if("dir" == node_type)
	        {
		        string dir_name = path_for_dir + "/" + node_name;
                if (!Directory.Exists(dir_name))
                {
                    Directory.CreateDirectory(dir_name);
                }
		        XmlNodeList children = cur_dir.ChildNodes;
                foreach(XmlNode item in children)
                {
			        storeDir((XmlElement)item, dir_name, binary_data_storage);
		        }
	        }
	        else if("root" == node_type)
	        {
		        XmlNodeList children = cur_dir.ChildNodes;
                foreach(XmlNode item in children)
                {
			        storeDir((XmlElement)item, path_for_dir, binary_data_storage);
		        }
	        }
	        else if("file" == node_type)
	        {
		        string file_format = cur_dir.GetAttribute("format");
		        if(file_format == "bin")
		        {
                    string bin_data_id_str = cur_dir.GetAttribute("bin_data_id");
                    int bin_data_id = XMLInt.ParseOrDefault(bin_data_id_str, -1);
                    if (bin_data_id >= 0 && bin_data_id < binary_data_storage.Count)
                    {
                        using (FileStream bin_file = File.Create(path_for_dir + "/" + node_name))
                        {
                            bin_file.Write(binary_data_storage[bin_data_id], 0, binary_data_storage[bin_data_id].Length);
                        }
                    }
                }
		        else
		        {
                    using(XmlWriter writer = XmlWriter.Create(path_for_dir + "/" + node_name))
                    {
                        cur_dir.WriteContentTo(writer);
                    }
		        }
	        }
        }

        private XmlDocument xml_source_document;
        private XmlElement xml_current_element;
        private List<byte[]> binary_data_storage;
        private const int initial_memory_stream_buffer = 4096;
    }
}
