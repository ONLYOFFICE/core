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
using System.Globalization;
using System.IO;
using CleverAge.OdfConverter.OdfConverterLib;

namespace CleverAge.OdfConverter.Spreadsheet
{
    /** This post processor handles crashes caused by inapropriate colfields in Pivot Tables**/
    public class OoxPivotCrashHandlingPostProcessor : AbstractPostProcessor
    {
        private ArrayList fieldNamesText;
        private int PivotTableElementDepth;
        private bool isPivotTableAttribute;
        private bool fieldState;
        private bool fieldElementState;
        private bool fieldAttributeState;
        private String fieldAttributeText;
        private string pivotCellText;
        private Stack fields;
        private Stack pivotCacheStack;
        public OoxPivotCrashHandlingPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.fieldState = false;
            this.fieldElementState = false;
            this.fieldAttributeState = false;
            this.fieldNamesText = new ArrayList();
            this.fields = new Stack();
            this.pivotCacheStack = new Stack();
            this.PivotTableElementDepth = 0;
            this.isPivotTableAttribute = false;
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            //pivotCache elements need to be processed in a postprocessor in order to avoid putting empty pivotCaches elements 
            if ("pivotCaches".Equals(localName))
            {
                this.pivotCacheStack.Push(new Element(prefix, localName, ns));
            }
            else if (this.pivotCacheStack.Count > 0)
            {
                this.pivotCacheStack.Push(new Element(prefix, localName, ns));
            }
            else if ("pivotCell".Equals(localName) || this.PivotTableElementDepth > 0)
            {
                this.PivotTableElementDepth++;
                this.nextWriter.WriteStartElement(prefix, localName, ns);

            }
            //put colfields on stack instead of processing them
            else if ("colFields".Equals(localName))
            {
                this.fieldState = true;
                fields.Push(new Element(prefix, localName, ns));
            }
            //put fields in colfields on stack instead of processing them 
            else if (fieldState && "field".Equals(localName))
            {
                this.fieldElementState = true;
                fields.Push(new Element(prefix, localName, ns));
            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
        }
        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            if (this.PivotTableElementDepth > 0)
            {
                this.isPivotTableAttribute = true;
            }
            if (this.pivotCacheStack.Count > 0)
            {
                this.pivotCacheStack.Push(new Attribute(prefix, localName, ns));
            }
            //put field attributes on stack instead of processing them
            else if (fieldState)
            {
                if ("table-source-field-name".Equals(localName))
                {
                    this.fieldAttributeState = true;
                }
                fields.Push(new Attribute(prefix, localName, ns));
            }
            else
            {
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }
        }
        public override void WriteString(string text)
        {
            if (this.pivotCacheStack.Count > 0)
            {
                if (this.pivotCacheStack.Peek() is Element)
                {
                    Element element = ((Element)this.pivotCacheStack.Pop());
                    element.AddChild(text);
                    this.pivotCacheStack.Push(element);
                }
                else
                {
                    Attribute attribute = ((Attribute)this.pivotCacheStack.Pop());
                    attribute.Value = text;
                    this.pivotCacheStack.Push(attribute);
                }
            }
            //collect all pivot table fields' names
            else if (this.PivotTableElementDepth > 0 && !isPivotTableAttribute)
            {
                this.pivotCellText += text;
                this.nextWriter.WriteString(text);
            }
            //collect field attribute value
            else if (fieldState)
            {
                this.fieldAttributeText += text;
            }
            else
            {
                this.nextWriter.WriteString(text);
            }
        }
        public override void WriteEndAttribute()
        {
            if (this.isPivotTableAttribute)
            {
                this.isPivotTableAttribute = false;
            }
            if (this.pivotCacheStack.Count > 0)
            {
                Attribute attribute = ((Attribute)this.pivotCacheStack.Pop());
                Element element = ((Element)this.pivotCacheStack.Pop());
                element.AddAttribute(attribute);
                this.pivotCacheStack.Push(element);
            }
            //Add field attribute value
            else if (fieldState)
            {
                Attribute attribute = ((Attribute)fields.Pop());
                if (fieldAttributeState)
                {
                    //in pxsi:table-source-field-name attribute "pxsi" string needs to be removed(it was added to ensure proper text processing)
                    attribute.Value = this.fieldAttributeText.Replace("pxsi", "");
                    this.fieldAttributeState = false;
                }
                else
                {
                    attribute.Value = this.fieldAttributeText;
                }
                this.fieldAttributeText = "";
                if (fieldElementState)
                {
                    //if field element is processed put field attribute value on stack
                    fields.Push(attribute);
                }
                else
                {
                    //if colfields element is processed while being on stack add attribute to it
                    Element MainElement = ((Element)fields.Pop());
                    MainElement.AddAttribute(attribute);
                    fields.Push(MainElement);
                }
            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }
        }
        public override void WriteEndElement()
        {
            if (this.pivotCacheStack.Count > 0)
            {
                Element element = ((Element)this.pivotCacheStack.Pop());
                if (this.pivotCacheStack.Count > 0)
                {
                    Element mainElement = ((Element)this.pivotCacheStack.Pop());
                    mainElement.AddChild(element);
                    this.pivotCacheStack.Push(mainElement);
                }
                else
                {
                    if (element.HasChild())
                    {
                        element.Write(this.nextWriter);
                    }
                }
            }
            //collect pivot table field names
            else if (this.PivotTableElementDepth > 0)
            {
                this.fieldNamesText.Add(this.pivotCellText);
                this.pivotCellText = "";
                this.PivotTableElementDepth--;
                this.nextWriter.WriteEndElement();
            }
            //process colfields and field elements only if table-source-field-name value is a proper pivot table field name
            else if (fieldState)
            {
                if (fieldElementState)
                {
                    Attribute[] attribute = new Attribute[20];
                    Attribute fieldName = new Attribute("", "", "");
                    int i = 0;
                    if (fields.Peek() is Attribute)
                    {
                        while (fields.Peek() is Attribute)
                        {
                            i++;
                            attribute[i] = ((Attribute)fields.Pop());
                            if ("table-source-field-name".Equals(attribute[i].Name))
                            {
                                fieldName = attribute[i];
                            }
                        }
                        if (this.fieldNamesText.Contains(fieldName.Value))
                        {
                            Element element = ((Element)fields.Pop());
                            for (int j = 1; j < i + 1; j++)
                            {
                                element.AddAttribute(attribute[j]);
                            }
                            Element mainElement = ((Element)fields.Pop());
                            mainElement.AddChild(element);
                            fields.Push(mainElement);
                        }
                        else
                        {
                            Element element = ((Element)fields.Pop());
                        }
                    }
                    else
                    {
                        Element element = ((Element)fields.Pop());
                    }
                    this.fieldElementState = false;
                }
                else
                {

                    Element mainElement = ((Element)fields.Pop());
                    if (mainElement.HasElement())
                    {
                        mainElement.Write(this.nextWriter);
                    }
                    this.fieldState = false;
                }
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }
        }
    }
}