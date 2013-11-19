using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using CleverAge.OdfConverter.OdfZipUtils;
using System.Xml;
using System.Diagnostics;
using CleverAge.OdfConverter.OdfConverterLib;

namespace CleverAge.OdfConverter.OdfConverterLib
{
    /// <summary>
    /// An abstract base class for converting files according to Open Packaging Conventions standard
    /// to a single XML stream.
    /// This XML stream has the following form:
    ///     &lt;?xml version="1.0" encoding="utf-8" standalone="yes"?&gt;
    ///     &lt;oox:package xmlns:oox="urn:oox"&gt;
    ///         &lt;oox:part oox:name="_rels/.rels"&gt;
    ///             [content of package part _rels/.rels]
    ///         &lt;/oox:part&gt;
    ///         &lt;oox:part oox:name="docProps/app.xml" oox:type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties" oox:rId="rId3"&gt;
    ///             [content of package part docProps/app.xml]
    ///         &lt;/oox:part&gt;
    ///         &lt;oox:part oox:name="docProps/core.xml" oox:type="http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties" oox:rId="rId2"&gt;
    ///             [content of package part docProps/core.xml]
    ///         &lt;/oox:part&gt;
    ///         &lt;oox:part oox:name="word/_rels/document.xml.rels"&gt;
    ///             [content of package part word/_rels/document.xml.rels]
    ///         &lt;/oox:part&gt;
    ///         &lt;oox:part oox:name="word/document.xml" oox:type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" oox:rId="rId1"&gt;  
    ///             [content of package part word/document.rels]
    ///         &lt;/oox:part&gt;
    ///     &lt;/oox:package>
    /// </summary>
    public abstract class OoxDocument : IDisposable
    {
        protected struct RelationShip
        {
            public string Id;
            public string Target;
            public string Type;
        }

        protected string _fileName;
        protected Stream _stream;
        private bool _disposed = false;
        protected Dictionary<string, bool> _copiedParts;

        // use a similar schema than defined by "http://schemas.microsoft.com/office/2006/xmlPackage"
        protected const string PACKAGE_NS = "urn:oox";
        protected const string NS_PREFIX = "oox";
        protected const string RELATIONSHIP_NS = "http://schemas.openxmlformats.org/package/2006/relationships";

        protected const string CONTENT_TYPES = "[Content_Types].xml";
        protected const string CONTENT_TYPES_NS = "http://schemas.openxmlformats.org/package/2006/content-types";

        protected const string R_PREFIX = "xmlns";
        protected const string R_TYPES_NS = "http://www.w3.org/2000/xmlns";

        public OoxDocument(string fileName)
        {
            this._fileName = fileName;
        }

        ~OoxDocument()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        public void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    if (_stream != null)
                        _stream.Close();
                }
                _disposed = true;
            }
        }

        /// <summary>
        /// A stream containing all XML parts of the document whose namespaces are defined in property <code>Namespaces</code>.
        /// </summary>
        public Stream OpenXML
        {
            get
            {
                try
                {
                    if (_stream != null)
                    {
                        _stream.Seek(0L, SeekOrigin.Begin);
                    }
                    else
                    {
                        ZipReader archive = ZipFactory.OpenArchive(_fileName);

                        _stream = new MemoryStream();
                        _copiedParts = new Dictionary<string, bool>();
                        XmlTextWriter xtw = new XmlTextWriter(new StreamWriter(_stream, Encoding.UTF8));

                        xtw.WriteStartDocument(true);
                        //xtw.WriteProcessingInstruction("mso-application", "progid='Word.Document'");
                        xtw.WriteStartElement(NS_PREFIX, "package", PACKAGE_NS);

                        // copy [Content_Types].xml
                        XmlReader reader = XmlReader.Create(archive.GetEntry(CONTENT_TYPES));
                        xtw.WriteStartElement(NS_PREFIX, "part", PACKAGE_NS);
                        xtw.WriteAttributeString(NS_PREFIX, "name", PACKAGE_NS, CONTENT_TYPES);
                        xtw.WriteAttributeString(NS_PREFIX, "type", PACKAGE_NS, CONTENT_TYPES_NS);
                        CopyPart(reader, xtw, CONTENT_TYPES_NS, CONTENT_TYPES, archive);
                        _copiedParts.Add(CONTENT_TYPES, true);
                        reader.Close();
                        xtw.WriteEndElement();
                        xtw.Flush();

                        // copy the whole document from its root
                        CopyLevel(archive, "_rels/.rels", xtw, this.Namespaces);

                        xtw.WriteEndDocument();
                        xtw.Flush();

                        // reset stream
                        _stream.Seek(0L, SeekOrigin.Begin);
                    }

                    return _stream;
                }
                catch (Exception ex)
                {
                    throw new NotAnOoxDocumentException("Could not create XML stream from input document.", ex);
                }
            }
        }

        protected virtual void CopyLevel(ZipReader archive, string relFile, XmlTextWriter xtw, List<string> namespaces)
        {
            XmlReader reader;
            List<RelationShip> rels;
            XmlReaderSettings xrs = new XmlReaderSettings();
            xrs.IgnoreWhitespace = false;

            try
            {
                // copy relationship part and extract relationships
                reader = XmlReader.Create(archive.GetEntry(relFile));

                if (!_copiedParts.ContainsKey(relFile))
                {
                    xtw.WriteStartElement(NS_PREFIX, "part", PACKAGE_NS);
                    xtw.WriteAttributeString(NS_PREFIX, "name", PACKAGE_NS, relFile);
                    xtw.WriteAttributeString(NS_PREFIX, "type", PACKAGE_NS, RELATIONSHIP_NS);
                    rels = CopyPart(reader, xtw, RELATIONSHIP_NS, relFile, archive);
                    _copiedParts.Add(relFile, true);
                    reader.Close();
                    xtw.WriteEndElement();
                    xtw.Flush();

                    // copy referenced parts
                    foreach (RelationShip rel in rels)
                    {
                        if (namespaces.Contains(rel.Type))
                        {
                            try
                            {
                                string basePath = relFile.Substring(0, relFile.LastIndexOf("_rels/"));
                                string path = rel.Target.Substring(0, rel.Target.LastIndexOf('/') + 1);
                                string file = rel.Target.Substring(rel.Target.LastIndexOf('/') + 1);

                                path = CombinePath(basePath, path);
                                // if there is a relationship part for the current part 
                                // copy relationships and referenced files recursively
                                CopyLevel(archive, path + "_rels/" + file + ".rels", xtw, namespaces);

                                string partName = CombinePath(basePath, rel.Target);
                                reader = XmlReader.Create(archive.GetEntry(basePath + rel.Target));

                                if (!_copiedParts.ContainsKey(partName))
                                {
                                    xtw.WriteStartElement(NS_PREFIX, "part", PACKAGE_NS);
                                    xtw.WriteAttributeString(NS_PREFIX, "name", PACKAGE_NS, partName);
                                    xtw.WriteAttributeString(NS_PREFIX, "type", PACKAGE_NS, rel.Type);
                                    xtw.WriteAttributeString(NS_PREFIX, "rId", PACKAGE_NS, rel.Id);

                                    CopyPart(reader, xtw, rel.Type, partName, archive);
                                    _copiedParts.Add(partName, true);
                                    reader.Close();

                                    xtw.WriteEndElement();
                                    xtw.Flush();
                                }
                            }
                            catch (ZipEntryNotFoundException)
                            {
                            }
                        }
                        else
                        {
                            System.Diagnostics.Trace.WriteLine(string.Format("WARNING: Namespace {0} not included.", rel.Type));
                        }
                    }
                }
            }
            catch (ZipEntryNotFoundException)
            {
            }
        }

        protected virtual List<RelationShip> CopyPart(XmlReader xtr, XmlTextWriter xtw, string ns, string partName, ZipReader archive)
        {
            bool isInRel = false;
            bool extractRels = ns.Equals(RELATIONSHIP_NS);
            List<RelationShip> rels = new List<RelationShip>();

            RelationShip rel = new RelationShip();

            while (xtr.Read())
            {
                switch (xtr.NodeType)
                {
                    case XmlNodeType.Attribute:
                        break;
                    case XmlNodeType.CDATA:
                        xtw.WriteCData(xtr.Value);
                        break;
                    case XmlNodeType.Comment:
                        xtw.WriteComment(xtr.Value);
                        break;
                    case XmlNodeType.DocumentType:
                        xtw.WriteDocType(xtr.Name, null, null, null);
                        break;
                    case XmlNodeType.Element:
                        if (extractRels && xtr.LocalName == "Relationship" && xtr.NamespaceURI == RELATIONSHIP_NS)
                        {
                            isInRel = true;
                            rel = new RelationShip();
                        }

                        xtw.WriteStartElement(xtr.Prefix, xtr.LocalName, xtr.NamespaceURI);

                        if (xtr.HasAttributes)
                        {
                            while (xtr.MoveToNextAttribute())
                            {
                                if (extractRels && isInRel)
                                {
                                    if (xtr.LocalName == "Type")
                                        rel.Type = xtr.Value;
                                    else if (xtr.LocalName == "Target")
                                        rel.Target = xtr.Value;
                                    else if (xtr.LocalName == "Id")
                                        rel.Id = xtr.Value;

                                }

                                if (!xtr.LocalName.StartsWith("rsid"))
                                {
                                    xtw.WriteAttributeString(xtr.Prefix, xtr.LocalName, xtr.NamespaceURI, xtr.Value);
                                }
                            }
                            xtr.MoveToElement();
                        }

                        if (isInRel)
                        {
                            isInRel = false;
                            rels.Add(rel);
                        }

                        if (xtr.IsEmptyElement)
                        {
                            xtw.WriteEndElement();
                        }
                        break;
                    case XmlNodeType.EndElement:
                        xtw.WriteEndElement();
                        break;
                    case XmlNodeType.EntityReference:
                        xtw.WriteEntityRef(xtr.Name);
                        break;
                    case XmlNodeType.ProcessingInstruction:
                        xtw.WriteProcessingInstruction(xtr.Name, xtr.Value);
                        break;
                    case XmlNodeType.SignificantWhitespace:
                        xtw.WriteWhitespace(xtr.Value);
                        break;
                    case XmlNodeType.Text:
                        xtw.WriteString(xtr.Value);
                        break;
                    case XmlNodeType.Whitespace:
                        xtw.WriteWhitespace(xtr.Value);
                        break;
                    case XmlNodeType.XmlDeclaration:
                        // omit XML declaration
                        break;
                    default:
                        Debug.Assert(false);
                        break;
                }
            }
            return rels;
        }

        /// <summary>
        /// A list of namespaces to be copied from the archive into the intermediate XML format
        /// </summary>
        protected abstract List<string> Namespaces
        {
            get;
        }

        private string CombinePath(string basePath, string subfolder)
        {
            if (basePath.EndsWith("/"))
            {
                basePath = basePath.Substring(0, basePath.Length - 1);
            }

            while (subfolder.StartsWith("../"))
            {
                if (basePath.Contains("/"))
                {
                    basePath = basePath.Substring(0, basePath.LastIndexOf('/'));
                }
                else
                {
                    basePath = "";
                }

                subfolder = subfolder.Substring(subfolder.IndexOf('/') + 1);
            }

            if (!basePath.Equals(""))
            {
                basePath = basePath + "/";
            }

            return basePath + subfolder;
        }
    }
}
