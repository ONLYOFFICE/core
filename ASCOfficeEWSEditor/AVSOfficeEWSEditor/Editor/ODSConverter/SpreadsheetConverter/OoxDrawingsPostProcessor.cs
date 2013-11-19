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
using System.Xml;
using System.Collections;
using CleverAge.OdfConverter.OdfConverterLib;

namespace CleverAge.OdfConverter.Spreadsheet
{
    public class OoxDrawingsPostProcessor : AbstractPostProcessor
    {
        private Stack drawingsContext;
        private const string PXSI_NAMESPACE = "urn:cleverage:xmlns:post-processings:drawings";
        private const string VML_NAMESPACE = "urn:schemas-microsoft-com:vml";
        private const string EXCEL_NAMESPACE = "urn:schemas-microsoft-com:office:excel";
        private bool isInDrawingMark;
        private bool isInDrawing;
        private bool isInDrawingMarkChildren;
        private bool isInClientData;
        private string noteId;
        private bool isInNoteIdAttribute;

        public OoxDrawingsPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.drawingsContext = new Stack();
            this.isInDrawingMark = false;
            this.isInDrawing = false;
            this.isInDrawingMarkChildren = false;
            this.isInClientData = false;
            this.noteId = "";
            this.isInNoteIdAttribute = false;
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {

            if (isInClientData)
            {

                Element positionsElement = this.searchElement(noteId);
                if (positionsElement != null)
                {
                    foreach (object node in positionsElement.Children)
                    {
                        if (node is Element)
                        {
                            Element nodeElement = ((Element)node);
                            nodeElement.Write(this.nextWriter);
                        }
                        else if (node is string)
                        {
                            string nodeString = ((string)node);
                            this.nextWriter.WriteString(nodeString);
                        }
                    }
                }
            }


            if (PXSI_NAMESPACE.Equals(ns) && "commentDrawingMark".Equals(localName))
            {
                this.isInDrawingMark = true;
            }
            if (EXCEL_NAMESPACE.Equals(ns) && "ClientData".Equals(localName))
            {
                this.isInClientData = true;
            }

            if (isInDrawingMark && EXCEL_NAMESPACE.Equals(ns))
            {
                this.isInDrawingMarkChildren = true;
            }

            if (isInDrawingMark)
            {
                this.drawingsContext.Push(new Element(prefix, localName, ns));
            }

            else if (VML_NAMESPACE.Equals(ns) && "shape".Equals(localName))
            {
                this.isInDrawing = true;
                this.nextWriter.WriteStartElement(prefix, localName, ns);

            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
        }
        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {

            if (isInDrawingMark)
            {
                this.drawingsContext.Push(new Attribute(prefix, localName, ns));
            }
            else
            {
                if (isInDrawing && localName.Equals("noteId"))
                {
                    this.isInNoteIdAttribute = true;
                }

                if (!isInNoteIdAttribute)
                {
                    this.nextWriter.WriteStartAttribute(prefix, localName, ns);
                }
            }
        }
        public override void WriteString(string text)
        {
            if (isInDrawingMark)
            {
                Node parentNode = ((Node)drawingsContext.Peek());
                if (parentNode is Element)
                {
                    Element parentElement = ((Element)parentNode);
                    drawingsContext.Pop();
                    parentElement.AddChild(text);
                    drawingsContext.Push(parentElement);
                }
                else
                {
                    Attribute parentAttribute = ((Attribute)parentNode);
                    drawingsContext.Pop();
                    parentAttribute.Value = text;
                    drawingsContext.Push(parentAttribute);
                }
            }
            else if (isInDrawing && isInNoteIdAttribute)
            {
                this.noteId = text;
            }
            else
            {
                this.nextWriter.WriteString(text);
            }
        }
        public override void WriteEndAttribute()
        {
            if (isInDrawingMark)
            {
                Attribute attribute = ((Attribute)drawingsContext.Peek());
                drawingsContext.Pop();
                Element parentElement = ((Element)drawingsContext.Peek());
                drawingsContext.Pop();
                parentElement.AddAttribute(attribute);
                drawingsContext.Push(parentElement);

            }
            else if (isInDrawing && isInNoteIdAttribute)
            {
                this.isInNoteIdAttribute = false;

            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }
        }

        private Element searchElement(string noteId)
        {
            foreach (Element element in drawingsContext)
            {
                String noteId2 = element.GetAttributeValue("noteId", "");
                if (noteId2.Equals(noteId))
                {
                    return element;
                }
            }
            return null;
        }

        public override void WriteEndElement()
        {
            if (isInDrawingMark)
            {

                if (isInDrawingMarkChildren)
                {
                    Element element = ((Element)drawingsContext.Peek());
                    drawingsContext.Pop();

                    Element parentElement = ((Element)drawingsContext.Peek());
                    drawingsContext.Pop();
                    parentElement.AddChild(element);
                    drawingsContext.Push(parentElement);


                    this.isInDrawingMarkChildren = false;

                }
                else
                {
                    this.isInDrawingMark = false;

                }

            }
            else if (isInDrawing)
            {

                if (isInClientData)
                {
                    this.isInDrawing = false;
                    this.isInClientData = false;
                }

                this.nextWriter.WriteEndElement();
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }
        }


    }
}
