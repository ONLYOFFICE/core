using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;

namespace ASCOfficeEWSEditor.Editor.FileFormats
{
    internal class XlsxLoader : ASCOfficeUtils.IExtractedFileEvent
    {
        internal XlsxLoader(XmlDocument xml_recv_document, List<byte[]> binary_data_storage)
        {
            this.xml_recv_document = xml_recv_document;
            this.binary_data_storage = binary_data_storage;
        }

        #region IExtractedFileEvent Members

        public void ExtractedFile(string file_name, ref Array binary_data)
        {
            if(file_name.EndsWith("/") && binary_data.Length == 0) // If directory
            {

            }
            else // If file
            {
                XmlNode dir_xml = Helpers.OOXNavigate.getFolderReferenceByPath(file_name, xml_recv_document.DocumentElement, true);
                string ext = Path.GetExtension(file_name).ToLower();
                switch (ext)
                {
                    case ".xml":
                    case ".rels":
                    {
                        XmlElement file_tag = xml_recv_document.CreateElement("file");
                        file_tag.SetAttribute("name", Path.GetFileName(file_name));
                        dir_xml.AppendChild(file_tag);

                        //XmlDocument xml_from_file = new XmlDocument();
                        //xml_from_file.Load(new StreamReader(new MemoryStream((byte[])binary_data)));

                        //XmlNode imported_branch = xml_recv_document.ImportNode(xml_from_file.DocumentElement, true);
                        //file_tag.AppendChild(imported_branch);

                        string sXml = System.Text.Encoding.UTF8.GetString((byte[])binary_data);
                        //удаляем символы переноса строк, иначе они будут считаться отдельными node
                        ReplaceSpecialChars(ref sXml);
                        int nHeadIndex = sXml.IndexOf("?>");
                        if (-1 != nHeadIndex)
                            file_tag.InnerXml = sXml.Remove(0, nHeadIndex + 2);
                        else
                            file_tag.InnerXml = sXml;

                        break;
                    }
                    default: // Binary and unsupported data
                    {
                        XmlElement file_tag = xml_recv_document.CreateElement("file");
                        file_tag.SetAttribute("name", Path.GetFileName(file_name));
                        dir_xml.AppendChild(file_tag);
			            file_tag.SetAttribute("format", "bin");
                        binary_data_storage.Add((byte[])binary_data);
			            file_tag.SetAttribute("bin_data_id", (binary_data_storage.Count - 1).ToString());
                        break;
                    }
                }

            }
        }

        #endregion

        static internal void LoadXML(XmlDocument xml_doc, string dir_path, List<byte[]> binary_data_storage)
        {
             if(!Directory.Exists(dir_path))
             {
                 throw new Events.ReportableException(new Events.InternalError("The specified path \"" + dir_path + "\" is not found"));
             }
             loadDir(xml_doc.DocumentElement, dir_path, binary_data_storage);
        }

        static private void loadDir(XmlElement cur_dir, string path_of_dir, List<byte[]> binary_data_storage)
        {
            foreach (string entry in Directory.GetDirectories(path_of_dir))
            {
                XmlElement dir_tag = cur_dir.OwnerDocument.CreateElement("dir");
		        dir_tag.SetAttribute("name", Path.GetFileName(entry));
		        cur_dir.AppendChild(dir_tag);
                loadDir(dir_tag, entry, binary_data_storage);
            }

            foreach (string file_name in Directory.GetFiles(path_of_dir))
            {
                string ext = Path.GetExtension(file_name).ToLower();
                switch(ext)
                {
                    case ".xml":
                    case ".rels":
                    {
                        XmlElement file_tag = cur_dir.OwnerDocument.CreateElement("file");
                        file_tag.SetAttribute("name", Path.GetFileName(file_name));
                        cur_dir.AppendChild(file_tag);

                        XmlDocument xml_from_file = new XmlDocument();
                        xml_from_file.Load(file_name);
                        XmlNode imported_branch = cur_dir.OwnerDocument.ImportNode(xml_from_file.DocumentElement, true);
                        file_tag.AppendChild(imported_branch);
                        break;
                    }
                    default: // Binary and unsupported data
                    {
                        XmlElement file_tag = cur_dir.OwnerDocument.CreateElement("file");
                        file_tag.SetAttribute("name", Path.GetFileName(file_name));
                        cur_dir.AppendChild(file_tag);
			            file_tag.SetAttribute("format", "bin");

                        using (FileStream bin_file = File.OpenRead(file_name))
                        {
                            byte[] bin_file_buffer = new byte[bin_file.Length];
                            bin_file.Read(bin_file_buffer, 0, (int)bin_file.Length);
                            binary_data_storage.Add(bin_file_buffer);
                            file_tag.SetAttribute("bin_data_id", (binary_data_storage.Count - 1).ToString());
                        }
                        break;
                    }
                }
            }
        }

        // Грамотно удаляем конструкции \r\n
        //  <textNode>\r\n
        //      <textNode>\r\n</textNode>\r\n
        //      <textNode></textNode>\r\n
        //  </textNode>
        // Нельзя удалять только случай 2
        private void ReplaceSpecialChars(ref string strXml)
        {
            strXml = strXml.Replace("\r", "");
            strXml = strXml.Replace("\t", "");

            for (int nIndex = 2; nIndex < strXml.Length - 2; ++nIndex)
            {
                if ('\n' == strXml[nIndex])
                {
                    if ('>' != strXml[nIndex - 1] || '<' != strXml[nIndex + 1])
                        continue;
                    if ('/' != strXml[nIndex + 2] || '/' == strXml[nIndex - 2])
                    {
                        strXml = strXml.Remove(nIndex--, 1);
                        continue;
                    }

                    for (int nIndex2 = nIndex - 1; nIndex2 >= 0; --nIndex2)
                    {
                        if ('<' == strXml[nIndex2])
                        {
                            if ('/' == strXml[nIndex2 + 1])
                                strXml = strXml.Remove(nIndex--, 1);

                            break;
                        }
                    }
                }
            }
        }

        private XmlDocument xml_recv_document;
        private List<byte[]> binary_data_storage;
        private const int initial_memory_stream_buffer = 4096;
    }
}
