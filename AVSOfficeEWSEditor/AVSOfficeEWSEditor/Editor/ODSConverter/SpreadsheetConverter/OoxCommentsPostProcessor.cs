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
using System.IO;
using System.Collections;
using CleverAge.OdfConverter.OdfConverterLib;

namespace CleverAge.OdfConverter.Spreadsheet
{

    public class OoxCommentsPostProcessor : AbstractPostProcessor
    {
        private Stack commentsContext;
        private const string PXSI_NAMESPACE = "urn:cleverage:xmlns:post-processings:comments";
        private const string EXCEL_NAMESPACE = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
        private bool isInCommentMark;
        private bool isInComment;
        private bool isString;
        private string stringNumber;
        private Element commentsElement;
        private bool isPxsi;
        private string noteId;
        private bool isInNoteIdAttribute;

        public OoxCommentsPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.commentsContext = new Stack();
            this.isInCommentMark = false;
            this.isInComment = false;
            this.isString = false;
            this.stringNumber = "0";
            this.isPxsi = false;
            this.noteId = "";
            this.isInNoteIdAttribute = false;
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {


            if (PXSI_NAMESPACE.Equals(ns) && "commentmark".Equals(localName))
            {
                this.isInCommentMark = true;
            }

            if (isInCommentMark)
            {
                this.commentsContext.Push(new Element(prefix, localName, ns));
            }
            else if (EXCEL_NAMESPACE.Equals(ns) && "comment".Equals(localName))
            {
                this.isInComment = true;
                this.nextWriter.WriteStartElement(prefix, localName, ns);

            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
        }
        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            if (isInCommentMark)
            {
                this.commentsContext.Push(new Attribute(prefix, localName, ns));
            }
            else
            {
                if (isInComment && localName.Equals("noteId"))
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
            if (isInCommentMark)
            {
                Node parentNode = ((Node)commentsContext.Peek());
                if (parentNode is Element)
                {
                    Element parentElement = ((Element)parentNode);
                    commentsContext.Pop();
                    parentElement.AddChild(text);
                    commentsContext.Push(parentElement);
                }
                else
                {
                    Attribute parentAttribute = ((Attribute)parentNode);
                    commentsContext.Pop();
                    parentAttribute.Value = text;
                    commentsContext.Push(parentAttribute);
                }
            }
            else if (isInComment && isInNoteIdAttribute)
            {
                this.noteId = text;
            }
            // This condition is to check for Image Path(Link to file option) 
            else if (text.Contains("Image-Path"))
            {
                string imagePathLocation = ImagePath(text);
                this.nextWriter.WriteString(imagePathLocation);
            }
            // Image cropping Added by Sonata
            else if (text.Contains("image-properties"))
            {
                string[] arrVal = new string[7];
                arrVal = text.Split(':');

                string source = arrVal[1].ToString();
                double top = double.Parse(arrVal[2].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                double right = double.Parse(arrVal[3].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                double bottom = double.Parse(arrVal[4].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                double left = double.Parse(arrVal[5].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                string measureType = arrVal[6].ToString();

                string tempFileName = AbstractConverter.inputTempFileName.ToString();
                ZipResolver zipResolverObj = new ZipResolver(tempFileName);
                OdfZipUtils.ZipArchiveWriter zipobj = new OdfZipUtils.ZipArchiveWriter(zipResolverObj);
                string widht_height_res = zipobj.ImageCopyBinary(source);
                zipResolverObj.Dispose();
                zipobj.Close();


                string[] arrValues = new string[3];
                arrValues = widht_height_res.Split(':');
                double width = double.Parse(arrValues[0].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                double height = double.Parse(arrValues[1].ToString(), System.Globalization.CultureInfo.InvariantCulture);
                double res = double.Parse(arrValues[2].ToString(), System.Globalization.CultureInfo.InvariantCulture);

                double cx = width * 2.54 / res;
                double cy = height * 2.54 / res;

                int pptLeft = 0;
                int pptRight = 0;
                int pptTop = 0;
                int pptBottom = 0;

                if (measureType == "in")
                {
                    pptLeft = (int)(left * 100000 / cx * 2.54);
                    pptRight = (int)(right * 100000 / cx * 2.54);
                    pptTop = (int)(top * 100000 / cy * 2.54);
                    pptBottom = (int)(bottom * 100000 / cy * 2.54);
                }
                else if (measureType == "cm")
                {
                    pptLeft = (int)(left * 100000 / cx);
                    pptRight = (int)(right * 100000 / cx);
                    pptTop = (int)(top * 100000 / cy);
                    pptBottom = (int)(bottom * 100000 / cy);
                }


                WriteStartAttribute("l");
                this.WriteString(pptLeft.ToString());
                WriteEndAttribute();
                WriteStartAttribute("r");
                this.WriteString(pptRight.ToString());
                WriteEndAttribute();
                WriteStartAttribute("t");
                this.WriteString(pptTop.ToString());
                WriteEndAttribute();
                WriteStartAttribute("b");
                this.WriteString(pptBottom.ToString());
                WriteEndAttribute();
            }
            else
            {
                this.nextWriter.WriteString(text);
            }
        }
        public override void WriteEndAttribute()
        {
            if (isInCommentMark)
            {
                Attribute attribute = ((Attribute)commentsContext.Peek());
                commentsContext.Pop();
                Element parentElement = ((Element)commentsContext.Peek());
                commentsContext.Pop();
                parentElement.AddAttribute(attribute);
                commentsContext.Push(parentElement);

            }
            else if (isInComment && isInNoteIdAttribute)
            {
                Element element = this.searchElement(this.noteId);
                Attribute attribute = (Attribute)element.GetAttribute("ref", "");

                this.isInNoteIdAttribute = false;
                attribute.Write(this.nextWriter);


            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }
        }

        private Element searchElement(string noteId)
        {

            foreach (Element element in commentsContext)
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
            if (isInCommentMark)
            {
                this.isInCommentMark = false;
            }
            else if (isInComment)
            {
                this.isInComment = false;
                this.nextWriter.WriteEndElement();
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }
        }

        //Resolve relative path to absolute path
        private string ImagePath(string text)
        {
            string[] arrVal = new string[2];
            arrVal = text.Split(':');
            string source = arrVal[1].ToString();
            string address = "";

            if (arrVal.Length == 2)
            {
                string returnInputFilePath = "";

                // for Commandline tool
                for (int i = 0; i < Environment.GetCommandLineArgs().Length; i++)
                {
                    if (Environment.GetCommandLineArgs()[i].ToString().ToUpper() == "/I")
                        returnInputFilePath = Path.GetDirectoryName(Environment.GetCommandLineArgs()[i + 1]);
                }

                //for addins
                if (returnInputFilePath == "")
                {
                    returnInputFilePath = Environment.CurrentDirectory;
                }

                string linkPathLocation = Path.GetFullPath(Path.Combine(returnInputFilePath, source)).Replace("\\", "/").Replace(" ", "%20");
                address = "file:///" + linkPathLocation;

            }
            return address.ToString();

        }
        //End

    }
}
