/* 
 * Copyright (c) 2006, Clever Age
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Clever Age nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

using System;
using System.Text;
using System.Xml;
using System.Collections;

namespace CleverAge.OdfConverter.OdfConverterLib
{

    /// <summary>
    /// Extra spaces post processor
    /// </summary>
    public class OoxSpacesPostProcessor : AbstractPostProcessor
    {

        private const string W_NAMESPACE = "http://schemas.openxmlformats.org/wordprocessingml/2006/main";
        private const string PXS_NAMESPACE = "urn:cleverage:xmlns:post-processings:extra-spaces";
        private const string XML_NAMESPACE = "http://www.w3.org/XML/1998/namespace";
        private const string DEFAULT_NAMESPACE = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
        private const string DRAWING_NAMESPACE = "http://schemas.openxmlformats.org/drawingml/2006/main";

        private Stack currentNode;
        private Stack context;
        private Stack store;

        public OoxSpacesPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.currentNode = new Stack();
            this.context = new Stack();
            this.store = new Stack();
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            this.currentNode.Push(new Element(prefix, localName, ns));

            if (IsS())
            {
                StartS();
            }
            else if (IsT())
            {
                StartT();
            }
            else if (IsP())
            {
                StartP();
            }
            else if (IsSi())
            {
                StartSi();
            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
        }


        public override void WriteEndElement()
        {
            if (IsS())
            {
                EndS();
            }
            else if (IsT())
            {
                EndT();
            }
            else if (IsP())
            {
                EndP();
            }
            else if (IsSi())
            {
                EndSi();
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }
            this.currentNode.Pop();
        }


        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            this.currentNode.Push(new Attribute(prefix, localName, ns));
            if (InS())
            {
                StartSAttribute();
            }
            else if (InT())
            {
                StartTAttribute();
            }
            else
            {
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }
        }


        public override void WriteEndAttribute()
        {
            if (InS())
            {
                EndSAttribute();
            }
            else if (InT())
            {
                EndTAttribute();
            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }
            this.currentNode.Pop();
        }


        public override void WriteString(string text)
        {
            if (InS())
            {
                WriteSString(text);
            }
            else if (InT())
            {
                WriteTString(text.Replace("\n", "\n "));
            }
            else
            {
                this.nextWriter.WriteString(text);
            }
        }

        /**
         * text elements
         */
        private bool IsT()
        {
            Element e = (Element)this.currentNode.Peek();
            return "t".Equals(e.Name) && (W_NAMESPACE.Equals(e.Ns) || DEFAULT_NAMESPACE.Equals(e.Ns) || DRAWING_NAMESPACE.Equals(e.Ns));
        }

        private bool InT()
        {
            return this.context.Contains("t");
        }

        private void StartT()
        {
            Element e = (Element)this.currentNode.Peek();
            this.store.Push(e);
            this.context.Push("t");
        }

        private void EndT()
        {
            Element t = (Element)this.store.Pop();
            if (this.preserveSpace && t.GetAttribute("space", XML_NAMESPACE) == null && !t.Ns.Equals(DRAWING_NAMESPACE))
            {
                this.preserveSpace = false;
                t.AddAttribute(new Attribute("xml", "space", "preserve", XML_NAMESPACE));
            }
            t.Write(nextWriter);

            if (this.isFirstTextNode)
            {
                this.isFirstTextNode = false;
            }

            this.context.Pop();
        }

        private void StartTAttribute()
        {
            Attribute a = (Attribute)this.currentNode.Peek();
            Element t = (Element)this.store.Peek();
            t.AddAttribute(a);
            this.store.Push(a);
        }

        private void EndTAttribute()
        {
            this.store.Pop();
        }

        private void WriteTString(string val)
        {
            Node node = (Node)this.currentNode.Peek();

            if (node is Element)
            {
                Element t = (Element)node;
                string trim = Clean(val);

                // Two more cases to handle :
                // 1. there shouldn't be a white space as the first character
                // of a paragraph or string item.
                if (this.isFirstTextNode && trim.IndexOf('\u0020') == 0)
                {
                    trim = trim.Substring(1);
                }
                // 2. remove any begining white space following a <text:s/>
                else if (this.followS)
                {
                    this.followS = false;

                    if (trim.IndexOf('\u0020') == 0)
                    {
                        trim = trim.Substring(1);
                    }
                }

                t.AddChild(trim);
            }
            else
            {
                Attribute a = (Attribute)node;
                a.Value += val;
            }
        }

        // Removes n-1 consecutive white spaces
        private string Clean(string val)
        {
            StringBuilder sb = new StringBuilder();
            char[] c = val.ToCharArray();
            int i = 0;

            while (i < c.Length)
            {
                if (c[i] != '\u000A')
                {
                    if (i < c.Length - 1)
                    {
                        if (!((c[i] == '\u0020' || c[i] == '\u000A') &&
                              (c[i + 1] == '\u0020' || c[i + 1] == '\u000A')))
                        {
                            sb.Append(c[i]);
                        }
                    }
                    else
                    {
                        sb.Append(c[i]);
                    }
                }
                i++;
            }

            return sb.ToString();
        }


        /**
         * Convert extra spaces tag to a sequence of white space characters
         */
        private int cSpaces;
        private bool followS = false;
        private bool preserveSpace = false;

        private bool IsS()
        {
            Element e = (Element)this.currentNode.Peek();
            return "s".Equals(e.Name) && PXS_NAMESPACE.Equals(e.Ns);
        }

        private bool InS()
        {
            return this.context.Contains("s");
        }

        private void StartS()
        {
            this.context.Push("s");
        }

        private void EndS()
        {
            this.context.Pop();
            string spaces = null;

            if (this.cSpaces > 0)
            {
                spaces = new string('\u0020', this.cSpaces);
            }
            else
            {
                spaces = new string('\u0020', 1);
            }
            this.cSpaces = 0;

            if (InT())
            {
                Element t = (Element)this.store.Peek();
                t.AddChild(spaces);
                this.preserveSpace = true;
                this.followS = true;
            }
        }

        private void StartSAttribute()
        {
            this.context.Push("c-attr");
        }

        private void EndSAttribute()
        {
            this.context.Pop();
        }

        private void WriteSString(string val)
        {
            if ("c-attr".Equals(this.context.Peek().ToString()))
            {
                Attribute attr = (Attribute)this.currentNode.Peek();
                if ("c".Equals(attr.Name))
                {
                    this.cSpaces = int.Parse(val);
                }
            }
        }

        // There's one more special thing to do : 
        // remove the eventual first white space on the first text node of the
        // first run of a paragraph or string item.

        /**
         * Paragraphs and String items
         */
        private bool isFirstTextNode = false;

        private bool IsP()
        {
            Element e = (Element)this.currentNode.Peek();
            return "p".Equals(e.Name) && W_NAMESPACE.Equals(e.Ns);
        }

        private void StartP()
        {
            Element p = (Element)this.currentNode.Peek();
            this.nextWriter.WriteStartElement(p.Prefix, p.Name, p.Ns);
            this.isFirstTextNode = true;
        }

        private void EndP()
        {
            this.nextWriter.WriteEndElement();
            this.isFirstTextNode = false;
        }

        private bool IsSi()
        {
            Element e = (Element)this.currentNode.Peek();
            return "si".Equals(e.Name) && DEFAULT_NAMESPACE.Equals(e.Ns);
        }

        private void StartSi()
        {
            Element si = (Element)this.currentNode.Peek();
            this.nextWriter.WriteStartElement(si.Prefix, si.Name, si.Ns);
            this.isFirstTextNode = true;
        }

        private void EndSi()
        {
            this.nextWriter.WriteEndElement();
            this.isFirstTextNode = false;
        }

        private void PrintValues(IEnumerable myCollection)
        {
            foreach (Object obj in myCollection)
                Console.WriteLine("{0}", obj);
        }

    }
}
