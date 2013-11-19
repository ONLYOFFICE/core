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
using System.IO;
using CleverAge.OdfConverter.OdfConverterLib;

namespace CleverAge.OdfConverter.Spreadsheet
{

    public class OoxPhysicalPathPostProcessor : AbstractPostProcessor
    {
        private Stack relationshipsContext;
        private const string PXSI_NAMESPACE = "urn:cleverage:xmlns:post-processings:path";
        private const string REL_NAMESPACE = "http://schemas.openxmlformats.org/package/2006/relationships";
        private bool isInPhysicalPath;
        private bool isPxsi;
        private bool isTarget;


        public OoxPhysicalPathPostProcessor(XmlWriter nextWriter)
            : base(nextWriter)
        {
            this.isInPhysicalPath = false;
            this.relationshipsContext = new Stack();
            this.isPxsi = false;
            this.isTarget = false;
        }

        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            if (isInPhysicalPath)
            {
                //Console.WriteLine("<"+ localName +">");
            }

            if (PXSI_NAMESPACE.Equals(ns) && "physicalPath".Equals(localName))
            {
                //Console.WriteLine("START");
                this.relationshipsContext = new Stack();
                this.relationshipsContext.Push(new Element(prefix, localName, ns));
                this.isInPhysicalPath = true;
                //Console.WriteLine("<" + localName + ">");
            }
            else if (REL_NAMESPACE.Equals(ns) && "Relationship".Equals(localName) && isInPhysicalPath)
            {
                this.isPxsi = false;
                //Console.WriteLine("pxsi CLOSED");
                this.relationshipsContext.Push(new Element(prefix, localName, ns));
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
            else
            {
                this.nextWriter.WriteStartElement(prefix, localName, ns);
            }
        }

        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {

            if ("pxsi".Equals(localName) && isInPhysicalPath)
            {
                //Console.WriteLine("pxsi OPEN");
                this.isPxsi = true;
            }
            else if ("Target".Equals(localName) && isInPhysicalPath)
            {
                this.isTarget = true;
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }
            else
            {
                this.nextWriter.WriteStartAttribute(prefix, localName, ns);
            }

            if (isInPhysicalPath)
            {
                //Console.WriteLine("-"+localName);
            }

        }
        public override void WriteString(string text)
        {
            if (isPxsi) { ;}
            else if (isInPhysicalPath && isTarget)
            {
                // this.nextWriter.WriteString(String.Concat("file:///",Path.GetFullPath(text))); --> code originally present
                /*Defect Id        :1784784
                 * Code Changed by :Vijayeta Tilak
                 * Date            :26th Dec '07
                 * Description     :This part of code was added because when a file contains OLE object, the absolute path,
                 *              on converting through commandline would be realtive to the path of project and not relative to the path where the file resides. Hence causing an error.
                 *              i.e. for an object in d:\excel\abc.xls, thro command line, the method GetFullPath for the passed parameter 'text' returns 'd:\xyz\Source\Shell\OdfConverterTest\bin', where the .exe file is present.
                 *              To overcome this problem the following code is added
                 */
                string returnInputFilePath = "";
                string linkPathLocation = "";
                //If the OLE object is present in a network.
                if (text.Substring(0, 2).Contains("\\"))
                {
                    this.nextWriter.WriteString(String.Concat("file:///", text));
                }
                //If the OLE object is present in a local machine.
                else
                {
                    // for Commandline tool
                    for (int i = 0; i < Environment.GetCommandLineArgs().Length; i++)
                    {
                        if (Environment.GetCommandLineArgs()[i].ToString().ToUpper() == "/I")
                        {
                            returnInputFilePath = Path.GetDirectoryName(Environment.GetCommandLineArgs()[i + 1]);
                        }
                    }
                    linkPathLocation = Path.GetFullPath(Path.Combine(returnInputFilePath, text.ToString())).Replace("\\", "/").Replace(" ", "%20");
                    //for addins
                    if (returnInputFilePath == "")
                    {
                        linkPathLocation = "";
                        linkPathLocation = Path.GetFullPath(text.ToString()).Replace("\\", "/").Replace(" ", "%20");
                    }
                    this.nextWriter.WriteString(String.Concat("file:///", linkPathLocation));
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
                //Console.WriteLine("2pxsi CLOSED");
                this.isPxsi = false;
            }
            else if (isTarget)
            {
                this.isTarget = false;
                this.nextWriter.WriteEndAttribute();
            }
            else
            {
                this.nextWriter.WriteEndAttribute();
            }

            if (isInPhysicalPath)
            {
                //Console.WriteLine("END attribute");
            }
        }

        public override void WriteEndElement()
        {

            if (isInPhysicalPath)
            {
                Element element = ((Element)relationshipsContext.Peek());
                relationshipsContext.Pop();
                //ending of sst element ends a sharedstring
                //Console.WriteLine("</"+ element.Name+">");
                if (PXSI_NAMESPACE.Equals(element.Ns) && "physicalPath".Equals(element.Name))
                {
                    this.isInPhysicalPath = false;
                    //Console.WriteLine("KONIEC");
                }
                else
                {
                    this.nextWriter.WriteEndElement();
                }
            }
            else
            {
                this.nextWriter.WriteEndElement();
            }
        }
    }
}
