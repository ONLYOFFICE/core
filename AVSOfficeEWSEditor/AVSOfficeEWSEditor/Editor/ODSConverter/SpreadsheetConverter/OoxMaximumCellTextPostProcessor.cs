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

    public class OoxMaximumCellTextPostProcessor : AbstractPostProcessor
    {
        private Stack sharedStringsContext;
        private const string PXSI_NAMESPACE = "urn:cleverage:xmlns:post-processings:cellText";
        private const string EXCEL_NAMESPACE = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
        private const int MAX_CELL_TEXT_LENGTH = 32000;
        private bool isInMaxLength;
        private int totalStringLength;
        private bool isPxsi;
        private bool isInRun;
        private bool isAtribute;
        private bool closeText;
        private bool closeRun;


        public OoxMaximumCellTextPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.isInMaxLength = false;
            this.totalStringLength = 0;
            this.sharedStringsContext = new Stack();
            this.isPxsi = false;
            this.isAtribute = false;
            this.isInRun = false;
            this.closeText = false;
            this.closeRun = false;
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            if (PXSI_NAMESPACE.Equals(ns) && "maxlength".Equals(localName))
            {
                this.sharedStringsContext = new Stack();
                this.sharedStringsContext.Push(new Element(prefix, localName, ns));
                this.totalStringLength = 0;
                this.isInMaxLength = true;
                this.isPxsi = true;
                this.isAtribute = false;
                this.closeText = false;
                this.closeRun = false;
            }
            else if (isInMaxLength)
            {
                this.sharedStringsContext.Push(new Element(prefix, localName, ns));

                if (this.totalStringLength < MAX_CELL_TEXT_LENGTH)
                {
                    if (EXCEL_NAMESPACE.Equals(ns) && "r".Equals(localName))
                    {
                        this.isInRun = true;
                    }

                    this.nextWriter.WriteStartElement(prefix, localName, ns);
                }
            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
        }

        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            if ("pxsi".Equals(localName))
            {
                this.isPxsi = true;
            }
            if (isInMaxLength)
            {
                this.isAtribute = true;
                if (this.totalStringLength < MAX_CELL_TEXT_LENGTH)
                {
                    this.nextWriter.WriteStartAttribute(prefix, localName, ns);
                }
            }
            else
            {
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }
        }

        public override void WriteString(string text)
        {
            if (isInMaxLength)
            {
                if (this.totalStringLength < MAX_CELL_TEXT_LENGTH)
                {
                    if (!isAtribute)
                    {
                        Element element = ((Element)sharedStringsContext.Peek());

                        if (EXCEL_NAMESPACE.Equals(element.Ns) && "t".Equals(element.Name))
                        {
                            if (this.totalStringLength < MAX_CELL_TEXT_LENGTH)
                            {
                                if (this.totalStringLength + text.Length <= MAX_CELL_TEXT_LENGTH)
                                {
                                    this.nextWriter.WriteString(text);
                                }
                                else
                                {
                                    int length = totalStringLength + text.Length - MAX_CELL_TEXT_LENGTH;
                                    this.nextWriter.WriteString(text.Substring(0, length));
                                    this.closeText = true;
                                    if (isInRun) this.closeRun = true;
                                }
                            }
                            this.totalStringLength += text.Length;
                        }
                        else
                        {
                            this.nextWriter.WriteString(text);
                        }
                    }

                    if (isAtribute)
                    {
                        this.nextWriter.WriteString(text);
                    }
                }
            }
            else
            {
                this.nextWriter.WriteString(text);
            }
        }

        public override void WriteEndAttribute()
        {
            if (isPxsi)
            {
                this.isPxsi = false;
            }
            else if (isInMaxLength)
            {
                this.isAtribute = false;
                this.nextWriter.WriteEndAttribute();
            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }
        }

        public override void WriteEndElement()
        {

            if (isInMaxLength)
            {
                Element element = ((Element)sharedStringsContext.Pop());

                if ("maxlength".Equals(element.Name))
                {
                    this.isInMaxLength = false;
                    this.sharedStringsContext.Clear();
                }
                else if (EXCEL_NAMESPACE.Equals(element.Ns) && "t".Equals(element.Name))
                {
                    if (this.totalStringLength < MAX_CELL_TEXT_LENGTH || closeText)
                    {
                        this.nextWriter.WriteEndElement();

                        if (closeText) this.closeText = false;
                    }
                }
                else if (EXCEL_NAMESPACE.Equals(element.Ns) && "r".Equals(element.Name))
                {
                    if (this.totalStringLength < MAX_CELL_TEXT_LENGTH || closeRun)
                    {
                        this.nextWriter.WriteEndElement();

                        if (closeRun) this.closeRun = false;
                    }

                }
                else
                {
                    if (totalStringLength < MAX_CELL_TEXT_LENGTH)
                    {
                        this.nextWriter.WriteEndElement();
                    }
                }
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }
        }
    }
}
