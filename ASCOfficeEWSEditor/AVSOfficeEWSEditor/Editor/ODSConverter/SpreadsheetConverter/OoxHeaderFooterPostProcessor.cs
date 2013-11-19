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
    /// <summary>
    /// A postprocessor, which cuts header or footer content if its too long.
    /// </summary>
    public class OoxHeaderFooterPostProcessor : AbstractPostProcessor
    {
        private string wholeString;
        private bool cutString;
        private bool attribute;
        private ArrayList HeaderFooterStrings;
        private const string DEFAULT_NAMESPACE = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";

        public OoxHeaderFooterPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.wholeString = "";
            this.cutString = false;
            this.HeaderFooterStrings = new ArrayList();
            this.attribute = false;
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            //check if we are in a header or footer element
            if ((localName.Equals("oddHeader") && ns.Equals(DEFAULT_NAMESPACE)) || (localName.Equals("oddFooter") && ns.Equals(DEFAULT_NAMESPACE)) || (localName.Equals("evenHeader") && ns.Equals(DEFAULT_NAMESPACE)) || (localName.Equals("evenFooter") && ns.Equals(DEFAULT_NAMESPACE)))
            {
                this.cutString = true;
            }
            this.nextWriter.WriteStartElement(prefix, localName, ns);
        }

        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            this.attribute = true;
            this.nextWriter.WriteStartAttribute(prefix, localName, ns);
        }

        public override void WriteString(string text)
        {
            //if we are in a header or footer we collect whole string from it
            if (this.cutString && !this.attribute)
            {
                this.wholeString = this.wholeString + "" + text;
            }
            else
            {
                this.nextWriter.WriteString(text);
            }
        }
        public override void WriteEndAttribute()
        {
            this.attribute = false;
            this.nextWriter.WriteEndAttribute();
        }
        public override void WriteEndElement()
        {
            if (this.cutString)
            {
                //if whole string is too long then we put it in HeaderFooterStrings ArrayList, and then use it to cut it
                if (this.wholeString.Length > 255)
                {
                    Add(this.HeaderFooterStrings, this.wholeString);
                    this.nextWriter.WriteString(CuttedString(this.HeaderFooterStrings, this.wholeString));
                }
                else
                {
                    this.nextWriter.WriteString(this.wholeString);
                }
                this.cutString = false;
                this.wholeString = "";
            }
            this.nextWriter.WriteEndElement();
        }

        //this method adds the string to ArrayList
        private void Add(ArrayList list, string text)
        {
            //We add to ArrayList parts of string between '&' as separate elements
            if (text.IndexOf('&', 1) > 0 && text.StartsWith("&"))
            {
                list.Add(text.Substring(0, text.IndexOf('&', 1)));
                Add(list, text.Substring(text.IndexOf('&', 1)));
            }
        }

        //This method cuts the string if it's too long 
        private string CuttedString(ArrayList list, string text)
        {
            string textFromList = "";
            bool isFinished = false;
            foreach (string textElement in list)
            {
                //put the string together until it's not longer than 256 chars
                if (!isFinished && textFromList.Length + textElement.Length < 256)
                {
                    textFromList = textFromList + "" + textElement;
                }

                //add the remaining part of the string only if it doesn't contain important data(which would've been partly lost)
                else if (!isFinished)
                {
                    string textToAdd = textElement.Substring(0, 256 - textFromList.Length);
                    if (textToAdd.Length < 7 || textToAdd.IndexOf('"') > textToAdd.LastIndexOf('"') - 1)
                    {

                    }
                    else
                    {
                        textFromList = textFromList + "" + textToAdd;
                    }
                    isFinished = true;
                }
            }
            return textFromList;
        }
    }
}
