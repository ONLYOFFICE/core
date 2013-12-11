using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Helpers
{
    internal class OOXNavigate
    {
        static public XmlNode FindWorkbookTag(XmlDocument document)
        {
            XmlNamespaceManager ns_manager = new XmlNamespaceManager(document.NameTable);
            ns_manager.AddNamespace("types", "http://schemas.openxmlformats.org/package/2006/content-types");
            ns_manager.AddNamespace("main", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            XmlNode wbpath_attr = document.SelectSingleNode("/root/file[@name = '[Content_Types].xml']/types:Types/types:Override[@ContentType = 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml']/@PartName",
                                                    ns_manager);
            if (null == wbpath_attr)
            {
                return wbpath_attr;
            }
            XmlNode wb_file = getReferenceByPath(wbpath_attr.InnerText, document.DocumentElement);
            return null == wb_file ? wb_file : wb_file.SelectSingleNode("main:workbook", ns_manager);
        }

        static public XmlNode getReferenceByPath(string path, XmlNode current_dir)
        {
            XmlNode target_dir = getFolderReferenceByPath(path, current_dir, false);

            int last_slash_pos = path.LastIndexOf("/");
            string jump_file;
            if(-1 != last_slash_pos)
            {
                jump_file = path.Substring(last_slash_pos + 1);
            }
            else
            {
                jump_file = path;
            }
            return target_dir.SelectSingleNode("file[@name = '" + jump_file + "']"); // null is possible
        }

        static public XmlNode getFolderReferenceByPath(string path, XmlNode current_dir, bool create_absent_folders)
        {
            if (0 != path.Length && path.StartsWith("/"))
            {
                current_dir = current_dir.OwnerDocument.DocumentElement; // Starts from root
                path = path.Substring(1);
            }
            int last_slash_pos = path.LastIndexOf("/");
            if (-1 != last_slash_pos)
            {
                string[] jump_dirs = path.Substring(0, last_slash_pos).Split(new Char[] {'/'});
                foreach (string jump_dir in jump_dirs)
                {
                    if (".." == jump_dir)
                    {
                        current_dir = current_dir.ParentNode ?? current_dir;
                    }
                    else if ("." == jump_dir)
                    {
                        continue;
                    }
                    else if (0 != jump_dir.Length)
                    {
                        XmlElement new_current_dir = (XmlElement)current_dir.SelectSingleNode("dir[@name = '" + jump_dir + "']");
                        if(new_current_dir != null)
                        {
                            current_dir = new_current_dir;
                        }
                        else if (create_absent_folders)
                        {
                            new_current_dir = current_dir.OwnerDocument.CreateElement("dir");
                            new_current_dir.SetAttribute("name", jump_dir);
                            current_dir.AppendChild(new_current_dir);
                            current_dir = new_current_dir;
                        }
                    }
                }
            }
            return current_dir;
        }

        static public XmlNode AddDirectory(string rel_dir_path_template, XmlNode directory, bool make_unique)
        {
            string rel_dir_path = make_unique ? CreateUniqueFilePath(rel_dir_path_template, directory) : rel_dir_path_template;
            int last_slash_pos = rel_dir_path.LastIndexOf("/");
            XmlNode target_folder = getFolderReferenceByPath(rel_dir_path.Substring(0, last_slash_pos + 1),
                                                             directory, true);
            string dir_name = rel_dir_path.Substring(last_slash_pos + 1);
            XmlElement file_tag = (XmlElement)target_folder.SelectSingleNode("file[@dir = '" + dir_name + "']");
            if (file_tag == null)
            {
                file_tag = directory.OwnerDocument.CreateElement("dir");
                file_tag.SetAttribute("name", dir_name);
                target_folder.AppendChild(file_tag);
            }

            return file_tag;
        }

        /// <summary>
        /// Adds a new file or finds existing file
        /// </summary>
        /// <param name="rel_file_path_template"></param>
        /// <param name="directory"></param>
        /// <param name="make_unique">Whether create a unique name of the file (appending ordering number) or create as specified and not rewrite</param>
        /// <returns></returns>
        static public XmlNode AddFile(string rel_file_path_template, XmlNode directory, bool make_unique)
        {
            string rel_file_path = make_unique ? CreateUniqueFilePath(rel_file_path_template, directory) : rel_file_path_template;
            int last_slash_pos = rel_file_path.LastIndexOf("/");
            XmlNode target_folder = getFolderReferenceByPath(rel_file_path.Substring(0, last_slash_pos + 1) /* (last_slash_pos == -1) is taken into account here too */,
                                                             directory, true);
            string file_name = rel_file_path.Substring(last_slash_pos + 1);/* (last_slash_pos == -1) is taken into account here */
            XmlElement file_tag = (XmlElement)target_folder.SelectSingleNode("file[@name = '" + file_name + "']");
            if (file_tag == null)
            {
                file_tag = directory.OwnerDocument.CreateElement("file");
                file_tag.SetAttribute("name", file_name);
                target_folder.AppendChild(file_tag);
            }

            return file_tag;
        }

        static public XmlNode AddBinFile(string rel_file_path_template, XmlNode directory, bool make_unique, int data_id)
        {
            string rel_file_path = make_unique ? CreateUniqueFilePath(rel_file_path_template, directory) : rel_file_path_template;
            int last_slash_pos = rel_file_path.LastIndexOf("/");
            XmlNode target_folder = getFolderReferenceByPath(rel_file_path.Substring(0, last_slash_pos + 1), directory, true);
            string file_name = rel_file_path.Substring(last_slash_pos + 1);
            XmlElement file_tag = (XmlElement)target_folder.SelectSingleNode("file[@name = '" + file_name + "']");
            if (file_tag == null)
            {
                file_tag = directory.OwnerDocument.CreateElement("file");
                file_tag.SetAttribute("name", file_name);
                file_tag.SetAttribute("format", "bin");
                file_tag.SetAttribute("bin_data_id", data_id.ToString());
                target_folder.AppendChild(file_tag);
            }

            return file_tag;
        }

        static public XmlNode DeleteBinFile(string rel_file_path_template, XmlNode directory)
        {
            string rel_file_path = rel_file_path_template;
            int last_slash_pos = rel_file_path.LastIndexOf("/");
            XmlNode target_folder = getFolderReferenceByPath(rel_file_path.Substring(0, last_slash_pos + 1), directory, true);
            string file_name = rel_file_path.Substring(last_slash_pos + 1);
            XmlElement file_tag = (XmlElement)target_folder.SelectSingleNode("file[@name = '" + file_name + "']");
            if (file_tag != null)
            {                
                target_folder.RemoveChild(file_tag);
            }

            return file_tag;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="template_path">May include any type of file path. Must end with filename</param>
        /// <param name="base_path_folder">Must point to 'dir' node that is the base for the 'template_path'</param>
        /// <returns></returns>
        static public string CreateUniqueFilePath(string template_path, XmlNode base_path_folder)
        {
            int last_slash_pos = template_path.LastIndexOf("/");
            string folder_path = template_path.Substring(0, last_slash_pos + 1); /* (last_slash_pos == -1) is taken into account here too */
            XmlNode target_folder = getFolderReferenceByPath(folder_path, base_path_folder, true);
            string template_file_name = template_path.Substring(last_slash_pos + 1);/* (last_slash_pos == -1) is taken into account here */

            int last_dot_pos = template_file_name.LastIndexOf(".");
            string prefix_name = last_dot_pos != -1 ? template_file_name.Substring(0, last_dot_pos) : template_file_name;
            string suffix_name = last_dot_pos != -1 ? template_file_name.Substring(last_dot_pos) : "";

            int max_id = 0;
            foreach (XmlNode other_file_name in target_folder.SelectNodes("file[starts-with(@name, '" + prefix_name + "')]/@name"))
            {
                string other_name = other_file_name.InnerText;
                int last_dot_pos_other = other_name.LastIndexOf(".");
                string must_be_num = other_name.Substring(prefix_name.Length, (last_dot_pos_other == -1 ? other_name.Length : last_dot_pos_other) - prefix_name.Length);
                int id;
                if (int.TryParse(must_be_num, out id) && id > max_id)
                {
                    max_id = id;
                }
            }
            return folder_path + prefix_name + (max_id + 1).ToString() + suffix_name;
        }


        static public string DetermineAbsolutePath(XmlNode file_or_dir_tag)
        {
            XmlElement current_file_elem = (XmlElement)file_or_dir_tag;
            string absolute_path = "";
            while(current_file_elem != null)
            {
                switch(current_file_elem.Name)
                {
                    case "dir":
                        absolute_path = current_file_elem.GetAttribute("name") + '/' + absolute_path;
                        break;
                    case "file":
                        absolute_path = current_file_elem.GetAttribute("name") + absolute_path;
                        break;
                    case "root":
                        return "/" + absolute_path;

                }
                current_file_elem = (XmlElement)current_file_elem.ParentNode;
            }
            return "/" + absolute_path;
        }

        /// <summary>
        /// Tries to determine relative path to base directory. If it is impossible it returns absolute path
        /// </summary>
        /// <param name="file_or_dir_tag"></param>
        /// <param name="base_dir"></param>
        /// <returns></returns>
        static public string DetermineRelativePath(XmlNode file_or_dir_tag, XmlNode base_dir)
        {
            XmlElement current_file_elem = (XmlElement)file_or_dir_tag;
            string rel_path = "";
            while (current_file_elem != null)
            {
                if(base_dir.Equals(current_file_elem))
                {
                    return rel_path;
                }
                switch (current_file_elem.Name)
                {
                    case "dir":
                        rel_path = current_file_elem.GetAttribute("name") + '/' + rel_path;
                        break;
                    case "file":
                        rel_path = current_file_elem.GetAttribute("name") + rel_path;
                        break;
                    case "root":
                        return "/" + rel_path;

                }
                current_file_elem = (XmlElement)current_file_elem.ParentNode;
            }
            return "/" + rel_path;
        }
    }
}
