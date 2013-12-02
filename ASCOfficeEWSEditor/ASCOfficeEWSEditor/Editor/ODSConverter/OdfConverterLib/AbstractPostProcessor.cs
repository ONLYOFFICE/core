using System;
using System.Xml;
using System.Collections;

namespace CleverAge.OdfConverter.OdfConverterLib
{

    /// <summary>
    /// An <c>XmlWriter</c> implementation for table post processings
    public abstract class AbstractPostProcessor : XmlWriter
    {
        protected XmlWriter nextWriter;

        protected AbstractPostProcessor(XmlWriter nextWriter)
        {
            this.nextWriter = nextWriter;

            if (nextWriter == null)
            {
                throw new Exception("nextWriter can's be null");
            }
        }

        protected override void Dispose(bool disposing)
        {
            if (this.nextWriter != null)
            {
                this.nextWriter.Close();
            }
            this.nextWriter = null;
            base.Dispose(disposing);
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            this.nextWriter.WriteStartElement(prefix, localName, ns);
        }

        public override void WriteEndElement()
        {
            this.nextWriter.WriteEndElement();
        }

        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            this.nextWriter.WriteStartAttribute(prefix, localName, ns);
        }

        public override void WriteEndAttribute()
        {
            this.nextWriter.WriteEndAttribute();
        }

        public override void WriteString(string text)
        {
            this.nextWriter.WriteString(text);
        }

        public override void WriteFullEndElement()
        {
            this.WriteEndElement();
        }

        public override void WriteStartDocument()
        {
            this.nextWriter.WriteStartDocument();
        }

        public override void WriteStartDocument(bool b)
        {
            this.nextWriter.WriteStartDocument(b);
        }

        public override void WriteEndDocument()
        {
            this.nextWriter.WriteEndDocument();
        }

        public override void WriteDocType(string name, string pubid, string sysid, string subset)
        {
            this.nextWriter.WriteDocType(name, pubid, sysid, subset);
        }

        public override void WriteCData(string s)
        {
            this.nextWriter.WriteCData(s);
        }

        public override void WriteComment(string s)
        {
            this.nextWriter.WriteComment(s);
        }

        public override void WriteProcessingInstruction(string name, string text)
        {
            this.nextWriter.WriteProcessingInstruction(name, text);
        }

        public override void WriteEntityRef(string name)
        {
            this.nextWriter.WriteEntityRef(name);
        }

        public override void WriteCharEntity(char c)
        {
            this.nextWriter.WriteCharEntity(c);
        }

        public override void WriteWhitespace(string s)
        {
            this.nextWriter.WriteWhitespace(s);
        }

        public override void WriteSurrogateCharEntity(char lowChar, char highChar)
        {
            this.nextWriter.WriteSurrogateCharEntity(lowChar, highChar);
        }

        public override void WriteChars(char[] buffer, int index, int count)
        {
            this.nextWriter.WriteChars(buffer, index, count);
        }

        public override void WriteRaw(char[] buffer, int index, int count)
        {
            this.nextWriter.WriteRaw(buffer, index, count);
        }

        public override void WriteRaw(string data)
        {
            this.nextWriter.WriteRaw(data);
        }

        public override void WriteBase64(byte[] buffer, int index, int count)
        {
            this.nextWriter.WriteBase64(buffer, index, count);
        }

        public override WriteState WriteState
        {
            get
            {
                return this.nextWriter.WriteState;
            }
        }

        public override void Close()
        {
            this.nextWriter.Close();
        }

        public override void Flush()
        {
            this.nextWriter.Flush();
        }

        public override string LookupPrefix(string ns)
        {
            return this.nextWriter.LookupPrefix(ns);
        }

        /// <summary>
        /// Simple representation of elements or attributes nodes
        /// </summary>
        protected class Node
        {

            private string name;
            public string Name
            {
                set { this.name = value; }
                get { return this.name; }
            }

            private string prefix;
            public string Prefix
            {
                set { this.prefix = value; }
                get { return this.prefix; }
            }

            private string ns;
            public string Ns
            {
                set { this.ns = value; }
                get { return this.ns; }
            }

            public Node(string prefix, string name, string ns)
            {
                this.prefix = prefix;
                this.name = name;
                this.ns = ns;
            }

            public Node(Node node)
            {
                this.prefix = node.Prefix;
                this.name = node.Name;
                this.ns = node.Ns;
            }

        }

        protected class Attribute : Node
        {
            private string value;

            public Attribute(string prefix, string name, string ns)
                : base(prefix, name, ns)
            {
                this.value = "";
            }

            public Attribute(string prefix, string name, string val, string ns)
                : base(prefix, name, ns)
            {
                this.value = val;
            }

            public Attribute(Node node)
                : base(node)
            {
                this.value = "";
            }

            public string Value
            {
                get { return this.value; }
                set { this.value = value; if (value == null) this.value = ""; }
            }

            public void Write(XmlWriter writer)
            {
                writer.WriteStartAttribute(this.Prefix, this.Name, this.Ns);
                writer.WriteString(this.value);
                writer.WriteEndAttribute();
            }
        }

        protected class Element : Node, IComparable
        {
            private ArrayList attributes;
            private ArrayList children;

            public Element(string prefix, string name, string ns)
                : base(prefix, name, ns)
            {
                this.attributes = new ArrayList();
                this.children = new ArrayList();
            }

            public Element(Node node)
                : base(node)
            {
                this.attributes = new ArrayList();
                this.children = new ArrayList();
            }

            public int CompareTo(object obj)
            {
                if (obj is Element)
                {
                    Element el = (Element)obj;
                    return this.Name.CompareTo(el.Name);
                }
                else if (obj is string)
                {
                    string s = (String)obj;
                    return this.Name.CompareTo(s);
                }
                return 1;
            }

            public ArrayList Attributes
            {
                get { return this.attributes; }
                set { this.attributes = value; }
            }

            public ArrayList Children
            {
                get { return this.children; }
                set { this.children = value; }
            }

            public void AddAttribute(Attribute attribute)
            {
                this.attributes.Add(attribute);
            }

            public void RemoveAttribute(Attribute attribute)
            {
                if (this.attributes.Contains(attribute))
                {
                    this.attributes.Remove(attribute);
                }
            }

            public void AddChild(Object child)
            {
                this.children.Add(child);
            }

            public void AddFirstChild(Object element)
            {
                this.children.Insert(0, element);
            }

            public void AddFirstChild(Element element)
            {
                this.children.Insert(0, element);
            }

            public void AddChildIfNotExist(Element element)
            {
                if (this.GetChild(element.Name, element.Ns) == null)
                {
                    this.children.Add(element);
                }
            }

            public void AddChild(string text)
            {
                this.children.Add(text);
            }

            public void RemoveChild(Object child)
            {
                if (this.children.Contains(child))
                {
                    this.children.Remove(child);
                }
            }

            public Element GetChild(string name, string ns)
            {
                foreach (object node in this.children)
                {
                    if (node is Element)
                    {
                        Element element = (Element)node;
                        if (element.Name.Equals(name) && element.Ns.Equals(ns))
                        {
                            return element;
                        }
                    }
                }
                return null;
            }

            public string GetTextChild()
            {
                foreach (object node in this.children)
                {
                    if (node is string)
                    {
                        string child = (string)node;
                        return child;
                    }
                }
                return "";
            }

            public Element GetChildWithAttribute(string name, string ns, string attrName, string attrNs, string attrValue)
            {
                foreach (object node in this.children)
                {
                    if (node is Element)
                    {
                        Element element = ((Element)node);
                        if (element.Name.Equals(name) && element.Ns.Equals(ns) && attrValue.Equals(element.GetAttributeValue(attrName, attrNs)))
                        {
                            return element;
                        }
                    }
                }
                return null;
            }

            public ArrayList GetChildren(string name, string ns)
            {
                ArrayList elements = new ArrayList();

                foreach (object node in this.children)
                {
                    if (node is Element)
                    {
                        Element element = (Element)node;
                        if (element.Name.Equals(name) && element.Ns.Equals(ns))
                        {
                            elements.Add(element);
                        }
                    }
                }
                return elements;
            }


            public ArrayList GetChildElements()
            {
                ArrayList elements = new ArrayList();

                foreach (object node in this.children)
                {
                    if (node is Element)
                    {
                        Element element = (Element)node;
                        elements.Add(element);
                    }
                }
                return elements;
            }

            public ArrayList GetTextChildren()
            {
                ArrayList elements = new ArrayList();

                foreach (object node in this.children)
                {
                    if (node is string)
                    {
                        string element = (string)node;
                        elements.Add(element);
                    }
                }
                return elements;
            }

            public ArrayList GetChildElementsWithText()
            {
                ArrayList elements = new ArrayList();

                foreach (object node in this.children)
                {
                    if (node is Element)
                    {
                        Element element = (Element)node;
                        if (element.GetTextChild() != null)
                        {
                            elements.Add(element);
                        }
                    }
                }
                return elements;
            }

            public Attribute GetAttribute(string name, string ns)
            {
                foreach (Attribute attribute in this.attributes)
                {
                    if (attribute.Name.Equals(name) && attribute.Ns.Equals(ns))
                    {
                        return attribute;
                    }
                }
                return null;
            }

            public string GetAttributeValue(string name, string ns)
            {
                foreach (Attribute attribute in this.attributes)
                {
                    if (attribute.Name.Equals(name) && attribute.Ns.Equals(ns))
                    {
                        return attribute.Value;
                    }
                }
                return "";
            }

            public void SortChildren()
            {
                this.children.Sort();
            }

            public bool HasChild()
            {
                return this.children.Count > 0;
            }

            public bool HasElement()
            {
                return this.GetChildElements().Count > 0;
            }

            public bool HasElementWithText()
            {
                return this.GetChildElementsWithText().Count > 0;
            }

            public bool IsEqualTo(Element e)
            {
                if (e == null) return false;
                foreach (Attribute attribute in Attributes)
                {
                    string value = attribute.Value;
                    string comparedValue = e.GetAttributeValue(attribute.Name, attribute.Ns);
                    if ((value == null || "".Equals(value)) && comparedValue != null && !"".Equals(comparedValue))
                    {
                        return false;
                    }
                    if (!value.Equals(comparedValue))
                    {
                        return false;
                    }
                }
                foreach (Attribute attribute in e.Attributes)
                {
                    string value = attribute.Value;
                    string comparedValue = e.GetAttributeValue(attribute.Name, attribute.Ns);
                    if ((value == null || "".Equals(value)) && comparedValue != null && !"".Equals(comparedValue))
                    {
                        return false;
                    }
                    if (!value.Equals(comparedValue))
                    {
                        return false;
                    }
                }
                return true;
            }

            public void Replace(object child, object newChild)
            {
                // replace a string child
                if ((child is string) && (newChild is string))
                {
                    if (this.GetTextChildren().Contains(child))
                    {
                        int index = this.children.IndexOf(child);
                        this.children.Remove(child);
                        this.children.Insert(index, newChild);
                    }
                }
                // replace an attribute child
                else if (((child as Attribute) != null) && ((newChild as Attribute) != null))
                {
                    if (this.attributes.Contains(child))
                    {
                        int index = this.attributes.IndexOf(child);
                        this.attributes.Remove(child);
                        this.attributes.Insert(index, newChild);
                    }
                }
                // replace an element child
                else if (this.children.Contains(child))
                {
                    int index = this.children.IndexOf(child);
                    this.children.Remove(child);
                    this.children.Insert(index, newChild);
                }
            }

            public void Write(XmlWriter writer)
            {
                writer.WriteStartElement(this.Prefix, this.Name, this.Ns);
                foreach (Attribute attribute in this.attributes)
                {
                    attribute.Write(writer);
                }
                foreach (object node in this.children)
                {
                    if (node is Element)
                    {
                        ((Element)node).Write(writer);
                    }
                    else if (node is string)
                    {
                        writer.WriteString((string)node);
                    }
                    else
                    {
                        throw new Exception("Child node must be of type Element or string");
                    }
                }
                writer.WriteEndElement();
            }

            public Element Clone()
            {
                Element result = new Element(this);
                // copy attributes
                foreach (Attribute attr in this.attributes)
                {
                    Attribute attr2 = new Attribute(attr);
                    attr2.Value = attr.Value;
                    result.AddAttribute(attr2);
                }
                // copy children
                foreach (Object obj in this.children)
                {
                    if (obj is Element)
                    {
                        Element child = (Element)obj;
                        result.AddChild(child.Clone());
                    }
                    else if (obj is string)
                    {
                        string child = (string)obj;
                        result.AddChild(child.Clone());
                    }
                }
                return result;
            }

        }
    }
}
