using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.Helpers;

namespace ASCOfficeEWSEditor.Editor.OOX
{
    internal class XmlFile
    {
        public XmlFile(ContentTypes content_types, Relationships relationships, XmlNode start_dir, string file_path, 
            OOX.Relationships.FileType rel_type, OOX.ContentTypes.FileType content_type)
        {
            content_types_link = content_types;
            relationships_link = relationships;
            start_dir_tag = start_dir;
            this.file_path = file_path;
            this.rel_type = rel_type;
            this.content_type = content_type;
            file_tag = Helpers.OOXNavigate.getReferenceByPath(file_path, start_dir);
            rId = relationships.GetId(file_path);
        }

        public XmlNode GetFileTag()
        {
            return file_tag;
        }

        public XmlNode GetContainerDirTag()
        {
            return file_tag == null ? null : file_tag.ParentNode;
        }

        public XmlNode GetRootTag()
        {
            return file_tag == null ? null : file_tag.FirstChild;
        }

        public XmlNode GetFileTagOrCreate()
        {
            return GetFileTag() ?? CreateFileTag();
        }
        
        public XmlNode CreateFileTag()
        {
            file_tag = Helpers.OOXNavigate.AddFile(file_path, start_dir_tag, false);

            #region File registration
            rId = relationships_link == null ? "" : relationships_link.AddTarget(file_path, rel_type, false);
            if (content_types_link != null)
            {
                content_types_link.RegisterFile(file_tag, content_type); // Register in [Content_Types]
            }
            #endregion

            return file_tag;
        }

        public void Remove()
        {
            if (file_tag != null)
            {
                relationships_link.RemoveTarget(rId);
                content_types_link.DeRegisterFile(file_tag);
                file_tag.ParentNode.RemoveChild(file_tag);
                file_tag = null;
            }
        }

        internal void AppendXmlSection(XmlNode section_node, List<string> sections_names_backward, XmlNamespaceManager ns_manager)
        {
            XmlNode main_tag = GetRootTag();
            if(main_tag != null)
            {
                string inserted_section_name = section_node.Name;
                string query = "main:" + inserted_section_name;

                bool append_every_following_node = false;
                XmlNode where_to_insert_after = null;
                foreach (string section_name in sections_names_backward)
                {
                    if (append_every_following_node)
                    {
                        where_to_insert_after = main_tag.SelectSingleNode("main:" + section_name, ns_manager);
                        if (where_to_insert_after != null)
                        {
                            break;
                        }
                    }
                    if (section_name == inserted_section_name)
                    {
                        append_every_following_node = true;
                    }
                }
                main_tag.InsertAfter(section_node, where_to_insert_after);
            }
        }


        public string rId { get; private set; }

        #region Info for creation/deletion
        private ContentTypes content_types_link;
        private Relationships relationships_link;
        private XmlNode start_dir_tag;
        private string file_path;
        private OOX.Relationships.FileType rel_type;
        private OOX.ContentTypes.FileType content_type;
        #endregion

        private XmlNode file_tag;
    }
}
