/* 
 * Copyright (c) 2006-2008 Clever Age
 * Copyright (c) 2006-2009 DIaLOGIKa
 *
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
 *     * Neither the names of copyright holders, nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

using System;
using System.Collections;
using System.IO;
using System.Xml;
using System.Text;
using System.Drawing;
using System.Diagnostics;
using System.Reflection;
using System.Collections.Generic;

namespace CleverAge.OdfConverter.OdfZipUtils
{
    /// <summary>
    /// Zip archiving states
    /// </summary>
    internal enum ProcessingState
    {
        /// <summary>
        /// Not archiving
        /// </summary>
        None,
        /// <summary>
        /// Waiting for an entry
        /// </summary>
        EntryWaiting,
        /// <summary>
        /// Processing an entry
        /// </summary>
        EntryStarted
    }

    /// <summary>
    /// An <c>XmlWriter</c> implementation for serializing the xml stream to a zip archive.
    /// All the necessary information for creating the archive and its entries is picked up 
    /// from the incoming xml stream and must conform to the following specification :
    /// 
    /// TODO : XML schema
    /// 
    /// example :
    /// 
    /// <c>&lt;pzip:archive pzip:target="path"&gt;</c>
    /// 	<c>&lt;pzip:entry pzip:target="relativePath"&gt;</c>
    /// 		<c>&lt;-- xml fragment --&lt;</c>
    /// 	<c>&lt;/pzip:entry&gt;</c>
    /// 	<c>&lt;-- other zip entries --&lt;</c>
    /// <c>&lt;/pzip:archive&gt;</c>
    /// 
    /// </summary>
    public class ZipArchiveWriter : XmlWriter
    {
        private class PZIP
        {
            public const string Namespace = "urn:cleverage:xmlns:post-processings:zip";
            public const string Prefix = "pzip";

            public const string EntryElement = "entry";
            public const string ArchiveElement = "archive";
            public const string CopyElement = "copy";
            public const string ExtractElement = "extract";
            public const string ImportElement = "import";

            public const string SourceAttr = "source";
            public const string TargetAttr = "target";
            public const string ContentTypeAttr = "content-type";
            public const string ContentAttr = "content";
            public const string CompressionAttr = "compression";
        }

        /// <summary>
        /// The zip archive
        /// </summary>
        private ZipWriter _zipOutputStream;
        private ProcessingState _processingState = ProcessingState.None;
        private Stack<Element> _elements;

        private Element _currentPzipElement;
        private Node _currentPzipAttribute;

        /// <summary>
        /// A delegate <c>XmlWriter</c> that actually feeds the zip output stream. 
        /// </summary>
        private XmlWriter _delegateWriter = null;
        /// <summary>
        /// The delegate settings
        /// </summary>
        private XmlWriterSettings _delegateSettings = null;
        private XmlResolver _resolver;

        /// <summary>
        /// Table of binary files to be copied from the source to the target package
        /// </summary>
        private Dictionary<string, string> _binaries;

        /// <summary>
        /// A list of files to be extracted from the source package
        /// </summary>
        private Dictionary<string, string> _extractFileList;

        /// <summary>
        /// A list of files to be imported from teh file system to the target package
        /// </summary>
        private Dictionary<string, string> _importFileList;

        private string _outputFile;

        private struct ImageValue
        {
            public int Width;
            public int Height;
            public float VerticalResolution;
            public float HorizontalResolution;
        }

        private Dictionary<string, ImageValue> _imageValues = new System.Collections.Generic.Dictionary<string, ImageValue>();


        /// <summary>
        /// Constructor
        /// </summary>
        public ZipArchiveWriter(XmlResolver xmlResolver)
        {
            _elements = new Stack<Element>();

            _delegateSettings = new XmlWriterSettings();
            _delegateSettings.OmitXmlDeclaration = false;
            _delegateSettings.CloseOutput = false;
            _delegateSettings.Encoding = Encoding.UTF8;
            _delegateSettings.Indent = false;
            // We have to use a new delegate per entry in the archive, 
            // so that XML conformance will be checked at the document level.
            //
            // It is not possible to check XML conformance at the document level with a single delegate 
            // that writes all the entries. We would have to use ConformanceLevel.Fragment then 
            // which would lead to a missing XML declaration.
            _delegateSettings.ConformanceLevel = ConformanceLevel.Document;

            _resolver = xmlResolver;
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public ZipArchiveWriter(XmlResolver xmlResolver, string outputFile)
            : this(xmlResolver)
        {
            this._outputFile = outputFile;
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                // Dispose managed resources
                if (_delegateWriter != null)
                {
                    _delegateWriter.Close();
                }
                if (_zipOutputStream != null)
                {
                    _zipOutputStream.Dispose();
                }
            }
            base.Dispose(disposing);
        }

        private void processPackageInstructions(Element element)
        {
            switch (_currentPzipElement.Name)
            {
                case PZIP.ArchiveElement:
                    // Prevent nested archive creation
                    if (_processingState == ProcessingState.None)
                    {
                        if (_currentPzipElement.Attributes.ContainsKey(PZIP.TargetAttr))
                        {
                            Debug.WriteLine("Creating archive : " + _currentPzipElement.Attributes[PZIP.TargetAttr].Value);
                            _zipOutputStream = ZipFactory.CreateArchive(_currentPzipElement.Attributes[PZIP.TargetAttr].Value);
                            _processingState = ProcessingState.EntryWaiting;

                            _binaries = new Dictionary<string, string>();
                            _extractFileList = new Dictionary<string, string>();
                            _importFileList = new Dictionary<string, string>();
                        }
                        else
                        {
                            //TODO throw exception
                        }
                    }
                    break;
                case PZIP.EntryElement:
                    // Prevent nested entry creation
                    if (_processingState == ProcessingState.EntryWaiting)
                    {
                        if (_currentPzipElement.Attributes.ContainsKey(PZIP.TargetAttr))
                        {
                            Debug.WriteLine("creating new part : " + _currentPzipElement.Attributes[PZIP.TargetAttr].Value);
                            CompressionMethod compressionMethod = CompressionMethod.Deflated;
                            if (_currentPzipElement.Attributes.ContainsKey(PZIP.CompressionAttr) && _currentPzipElement.Attributes[PZIP.CompressionAttr].Value.Equals("none"))
                            {
                                compressionMethod = CompressionMethod.Stored;
                            }
                            _zipOutputStream.AddEntry(_currentPzipElement.Attributes["target"].Value, compressionMethod);

                            if (_currentPzipElement.Attributes.ContainsKey(PZIP.ContentTypeAttr) && _currentPzipElement.Attributes[PZIP.ContentTypeAttr].Value.Equals("text/plain")
                                && _currentPzipElement.Attributes.ContainsKey(PZIP.ContentAttr))
                            {
                                Encoding enc = new System.Text.ASCIIEncoding();
                                byte[] buffer = enc.GetBytes(_currentPzipElement.Attributes[PZIP.ContentAttr].Value);
                                _zipOutputStream.Write(buffer, 0, buffer.Length);
                            }
                            else
                            {
                                _delegateWriter = XmlWriter.Create(_zipOutputStream, _delegateSettings);
                                _delegateWriter.WriteStartDocument();

                                _processingState = ProcessingState.EntryStarted;
                            }
                        }
                    }
                    break;
                case PZIP.CopyElement:
                    if (_processingState != ProcessingState.None)
                    {
                        if (_currentPzipElement.Attributes.ContainsKey(PZIP.SourceAttr)
                            && _currentPzipElement.Attributes.ContainsKey(PZIP.TargetAttr))
                        {
                            string source = _currentPzipElement.Attributes[PZIP.SourceAttr].Value;
                            string target = _currentPzipElement.Attributes[PZIP.TargetAttr].Value;

                            Debug.WriteLine(string.Format("copy source={0} target={1}", source, target));

                            if (_binaries != null && !_binaries.ContainsKey(target))
                            {
                                //target is the key because there are cases where one file has to be
                                //copied twice to different locations
                                _binaries.Add(target, source);
                            }
                        }
                    }
                    break;
                case PZIP.ExtractElement:
                    if (_processingState != ProcessingState.None)
                    {
                        if (_currentPzipElement.Attributes.ContainsKey(PZIP.SourceAttr)
                            && _currentPzipElement.Attributes.ContainsKey(PZIP.TargetAttr))
                        {
                            string source = _currentPzipElement.Attributes[PZIP.SourceAttr].Value;
                            string target = _currentPzipElement.Attributes[PZIP.TargetAttr].Value;

                            Debug.WriteLine(string.Format("copy source={0} target={1}", source, target));

                            if (_extractFileList != null && !_extractFileList.ContainsKey(target))
                            {
                                //target is the key because there are cases where one file has to be
                                //copied twice to different locations
                                _extractFileList.Add(target, source);
                            }
                        }
                    }
                    break;
                case PZIP.ImportElement:
                    if (_processingState != ProcessingState.None)
                    {
                        if (_currentPzipElement.Attributes.ContainsKey(PZIP.SourceAttr)
                            && _currentPzipElement.Attributes.ContainsKey(PZIP.TargetAttr))
                        {
                            string source = _currentPzipElement.Attributes[PZIP.SourceAttr].Value;
                            string target = _currentPzipElement.Attributes[PZIP.TargetAttr].Value;

                            Debug.WriteLine(string.Format("copy source={0} target={1}", source, target));

                            if (_importFileList != null && !_importFileList.ContainsKey(target))
                            {
                                //target is the key because there are cases where one file has to be
                                //copied twice to different locations
                                _importFileList.Add(target, source);
                            }
                        }
                    }
                    break;
            }
        }

        /// <summary>
        /// Delegates <c>WriteStartElement</c> calls when the element's prefix does not 
        /// match a zip command.  
        /// </summary>
        /// <param name="prefix"></param>
        /// <param name="localName"></param>
        /// <param name="ns"></param>
        public override void WriteStartElement(string prefix, string localName, string ns)
        {
            // check for pending pzip instructions
            if (_currentPzipElement != null && PZIP.Namespace.Equals(_currentPzipElement.Namespace))
            {
                processPackageInstructions(_currentPzipElement);
                _currentPzipElement = null;
            }

            Debug.WriteLine("[startElement] prefix=" + prefix + " localName=" + localName + " ns=" + ns);
            Element newElement = new Element(prefix, localName, ns);
            _elements.Push(newElement);

            // not a zip processing instruction
            if (!PZIP.Namespace.Equals(ns))
            {
                if (_delegateWriter != null)
                {
                    Debug.WriteLine("{WriteStartElement=" + localName + "} delegate");
                    _delegateWriter.WriteStartElement(prefix, localName, ns);
                }
                else
                {
                    Debug.WriteLine("[WriteStartElement=" + localName + " } delegate is null");
                }
            }
            else
            {
                // remember the current pzip node
                _currentPzipElement = newElement;
            }
        }

        /// <summary>
        /// Delegates <c>WriteEndElement</c> calls when the element's prefix does not 
        /// match a zip command. 
        /// Otherwise, close the archive or flush the delegate writer. 
        /// </summary>
        public override void WriteEndElement()
        {
            // check for pending pzip instructions
            if (_currentPzipElement != null && PZIP.Namespace.Equals(_currentPzipElement.Namespace))
            {
                processPackageInstructions(_currentPzipElement);
                _currentPzipElement = null;
            }

            Node elt = _elements.Pop();

            if (!elt.Namespace.Equals(PZIP.Namespace))
            {
                Debug.WriteLine("delegate - </" + elt.Name + ">");
                if (_delegateWriter != null)
                {
                    _delegateWriter.WriteEndElement();
                }
            }
            else
            {
                switch (elt.Name)
                {
                    case PZIP.ArchiveElement:
                        if (_processingState == ProcessingState.EntryWaiting)
                        {
                            if (_zipOutputStream != null)
                            {
                                // Copy binaries before closing the archive
                                copyBinaries();
                                //Added by Sonata - Copy Audio files before closing archive
                                extractFiles();
                                importFiles();
                                Debug.WriteLine("[closing archive]");
                                _zipOutputStream.Close();
                                _zipOutputStream = null;
                            }
                            _processingState = ProcessingState.None;
                        }
                        break;
                    case PZIP.EntryElement:
                        if (_processingState == ProcessingState.EntryStarted)
                        {
                            if (_delegateWriter != null)
                            {
                                Debug.WriteLine("[end part]");
                                _delegateWriter.WriteEndDocument();
                                _delegateWriter.Flush();
                                _delegateWriter.Close();
                                _delegateWriter = null;
                            }
                            _processingState = ProcessingState.EntryWaiting;
                        }
                        break;
                    case PZIP.CopyElement:
                        break;
                    case PZIP.ExtractElement:
                        break;
                    case PZIP.ImportElement:
                        break;
                }
            }
        }

        public override void WriteStartAttribute(string prefix, string localName, string ns)
        {
            Element elt = _elements.Peek();
            Debug.WriteLine("[WriteStartAttribute] prefix=" + prefix + " localName=" + localName + " ns=" + ns + " element=" + elt.Name);

            if (!elt.Namespace.Equals(PZIP.Namespace))
            {
                if (_delegateWriter != null)
                {
                    _delegateWriter.WriteStartAttribute(prefix, localName, ns);
                }
            }
            else if (ns.Equals(PZIP.Namespace))
            {
                // we only store attributes tied to zip processing instructions
                _currentPzipAttribute = new Node(prefix, localName, ns);
                elt.Attributes.Add(localName, _currentPzipAttribute);
            }
        }

        public override void WriteEndAttribute()
        {
            Node elt = _elements.Peek();
            Debug.WriteLine("[WriteEndAttribute] element=" + elt.Name);

            if (!elt.Namespace.Equals(PZIP.Namespace))
            {
                if (_delegateWriter != null)
                {
                    _delegateWriter.WriteEndAttribute();
                }
            }
            else
            {
                _currentPzipAttribute = null;
            }
        }

        // TODO: throw an exception if "target" attribute not set
        public override void WriteString(string text)
        {
            string strDefaultValue = "0";
            //clam: compute correct cropping size
            try
            {
                if (text.StartsWith("COMPUTEOOXCROPPING[") && _elements.Peek().Name == "srcRect")
                {
                    char[] sep = { ',', ']', '[' };
                    string[] arrValues = text.Split(sep);
                    string filename = arrValues[2];
                    string strValue = arrValues[1];
                    string strType = arrValues[3];
                    strDefaultValue = arrValues[4];
                    double value = 0;

                    System.Globalization.CultureInfo c = System.Threading.Thread.CurrentThread.CurrentCulture;
                    System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-us");

                    if (strValue.Contains("cm")) value = Double.Parse(strValue.Replace("cm", ""));

                    System.Threading.Thread.CurrentThread.CurrentCulture = c;


                    if (value != 0)
                    {
                        ImageValue iv;
                        if (_imageValues.ContainsKey(filename))
                        {
                            iv = _imageValues[filename];
                        }
                        else
                        {
                            Stream sourceStream = getStream(filename);
                            System.Drawing.Bitmap im = new System.Drawing.Bitmap(sourceStream);

                            iv = new ImageValue();
                            iv.Width = im.Width;
                            iv.Height = im.Height;
                            iv.HorizontalResolution = im.HorizontalResolution;
                            iv.VerticalResolution = im.VerticalResolution;

                            _imageValues.Add(filename, iv);
                        }

                        try
                        {
                            switch (strType)
                            {
                                case "t":
                                    text = Math.Floor(value / 2.54 / iv.Height * iv.VerticalResolution * 1000 * 100).ToString();
                                    break;

                                case "b":
                                    text = Math.Floor(value / 2.54 / iv.Height * iv.VerticalResolution * 1000 * 100).ToString();
                                    break;

                                case "l":
                                    text = Math.Floor(value / 2.54 / iv.Width * iv.HorizontalResolution * 1000 * 100).ToString();
                                    break;

                                case "r":
                                    text = Math.Floor(value / 2.54 / iv.Width * iv.HorizontalResolution * 1000 * 100).ToString();
                                    break;

                                default:
                                    break;

                            }

                        }
                        catch (Exception)
                        {
                            text = strDefaultValue;
                        }

                    }
                    else
                    {
                        text = "0";
                    }

                }
                else if (text.StartsWith("urn:odf-converter:makeOdfPath#"))
                {
                    text = MakeOdfPath(text);
                }
                else if (text.StartsWith("COMPUTEODFCROPPING[") && _elements.Peek().Name == "graphic-properties")
                {

                    string strDefault = "0cm 0cm 0cm 0cm";

                    System.Globalization.CultureInfo c = System.Threading.Thread.CurrentThread.CurrentCulture;
                    System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-us");

                    try
                    {
                        char[] sep = { ',', ']', '[' };
                        string[] arrValues = text.Split(sep);
                        string filename = arrValues[5];
                        strDefault = arrValues[6];

                        double r = Double.Parse(arrValues[1]);
                        double l = Double.Parse(arrValues[2]);
                        double t = Double.Parse(arrValues[3]);
                        double b = Double.Parse(arrValues[4]);
                        double r2, l2, t2, b2;

                        ImageValue iv;
                        if (_imageValues.ContainsKey(filename))
                        {
                            iv = _imageValues[filename];
                        }
                        else
                        {
                            Stream sourceStream = getStream(filename);
                            System.Drawing.Bitmap im = new System.Drawing.Bitmap(sourceStream);

                            iv = new ImageValue();
                            iv.Width = im.Width;
                            iv.Height = im.Height;
                            iv.HorizontalResolution = im.HorizontalResolution;
                            iv.VerticalResolution = im.VerticalResolution;

                            _imageValues.Add(filename, iv);
                        }

                        r2 = r * 2.54 * iv.Width / iv.HorizontalResolution / 1000 / 100;
                        l2 = l * 2.54 * iv.Width / iv.HorizontalResolution / 1000 / 100;
                        t2 = t * 2.54 * iv.Height / iv.VerticalResolution / 1000 / 100;
                        b2 = b * 2.54 * iv.Height / iv.VerticalResolution / 1000 / 100;

                        text = String.Concat("rect(", t2.ToString(), "cm ", r2.ToString(), "cm ", b2.ToString(), "cm ", l2.ToString(), "cm)");

                    }
                    catch (Exception)
                    {
                        text = strDefault;
                    }
                    finally
                    {
                        System.Threading.Thread.CurrentThread.CurrentCulture = c;
                    }

                }
            }
            catch (Exception)
            {
                text = strDefaultValue;
            }

            Element elt = _elements.Peek();

            if (!elt.Namespace.Equals(PZIP.Namespace))
            {
                if (_delegateWriter != null)
                {
                    _delegateWriter.WriteString(text);
                }
            }
            else
            {
                if (_currentPzipAttribute != null)
                {
                    // store attribute value (concatenation is needed because attribute value may consist of 
                    // more than one node when using XSL attribute value templates (i.e. curly braces {})
                    _currentPzipAttribute.Value += text;
                    //
                }
            }
        }

        private string MakeOdfPath(string text)
        {
            int pos = "urn:odf-converter:makeOdfPath#".Length;
            string wordPath = text.Remove(0, pos);
            DirectoryInfo outputPath = new DirectoryInfo(this._outputFile);

            if (wordPath.StartsWith("\\\\"))
            {
                //it's a windows network path
                string server = wordPath.Remove(0, 2);
                server = server.Remove(server.IndexOf("\\"));
                if (outputPath.FullName.StartsWith("\\\\" + server))
                {
                    //on the same server
                    string relativePath = MakeWinPathRelative(outputPath.FullName, wordPath);
                    return relativePath.Replace("\\", "/");
                }
                else
                {
                    return wordPath;
                }
            }
            else
            {
                //mounted local or network drive
                //transform it into the ODF format
                string odfPath = "/" + wordPath.Replace("\\", "/");
                return odfPath;
            }
        }

        private string MakeWinPathRelative(string absolutePath, string relativeTo)
        {
            string[] absoluteDirectories = absolutePath.Split('\\');
            string[] relativeDirectories = relativeTo.Split('\\');

            //Get the shortest of the two paths
            int length = absoluteDirectories.Length < relativeDirectories.Length ? absoluteDirectories.Length : relativeDirectories.Length;

            //Use to determine where in the loop we exited
            int lastCommonRoot = -1;
            int index;

            //Find common root
            for (index = 0; index < length; index++)
                if (absoluteDirectories[index] == relativeDirectories[index])
                    lastCommonRoot = index;
                else
                    break;

            //If we didn't find a common prefix then throw
            if (lastCommonRoot == -1)
                throw new ArgumentException("Paths do not have a common base");

            //Build up the relative path
            StringBuilder relativePath = new StringBuilder();

            //Add on the ..
            for (index = lastCommonRoot + 1; index < absoluteDirectories.Length; index++)
                if (absoluteDirectories[index].Length > 0)
                    relativePath.Append("..\\");

            //Add on the folders
            for (index = lastCommonRoot + 1; index < relativeDirectories.Length - 1; index++)
                relativePath.Append(relativeDirectories[index] + "\\");
            relativePath.Append(relativeDirectories[relativeDirectories.Length - 1]);

            return relativePath.ToString();
        }

        public override void WriteFullEndElement()
        {
            this.WriteEndElement();
        }

        public override void WriteStartDocument()
        {
            // nothing to do here
        }

        public override void WriteStartDocument(bool b)
        {
            // nothing to do here
        }

        public override void WriteEndDocument()
        {
            // nothing to do here
        }

        public override void WriteDocType(string name, string pubid, string sysid, string subset)
        {
            // nothing to do here
        }

        public override void WriteCData(string s)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteCData(s);
            }
        }

        public override void WriteComment(string s)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteComment(s);
            }
        }

        public override void WriteProcessingInstruction(string name, string text)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteProcessingInstruction(name, text);
            }
        }

        public override void WriteEntityRef(String name)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteEntityRef(name);
            }
        }

        public override void WriteCharEntity(char c)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteCharEntity(c);
            }
        }

        public override void WriteWhitespace(string s)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteWhitespace(s);
            }
        }

        public override void WriteSurrogateCharEntity(char lowChar, char highChar)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteSurrogateCharEntity(lowChar, highChar);
            }
        }

        public override void WriteChars(char[] buffer, int index, int count)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteChars(buffer, index, count);
            }
        }

        public override void WriteRaw(char[] buffer, int index, int count)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteRaw(buffer, index, count);
            }
        }

        public override void WriteRaw(string data)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteRaw(data);
            }
        }

        public override void WriteBase64(byte[] buffer, int index, int count)
        {
            if (_delegateWriter != null)
            {
                _delegateWriter.WriteBase64(buffer, index, count);
            }
        }

        public override WriteState WriteState
        {
            get
            {
                if (_delegateWriter != null)
                {
                    return _delegateWriter.WriteState;
                }
                else
                {
                    return WriteState.Start;
                }
            }
        }

        public override void Close()
        {
            // zipStream and delegate are closed elsewhere.... if everything else is fine
            if (_delegateWriter != null)
            {
                _delegateWriter.Close();
                _delegateWriter = null;
            }
            if (_zipOutputStream != null)
            {
                _zipOutputStream.Close();
                _zipOutputStream = null;
            }

        }

        public override void Flush()
        {
            // nothing to do
        }

        public override string LookupPrefix(String ns)
        {
            if (_delegateWriter != null)
            {
                return _delegateWriter.LookupPrefix(ns);
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Transfer binary files between two zip archives. 
        /// </summary>
        private void copyBinaries()
        {
            foreach (string target in _binaries.Keys)
            {
                if (_zipOutputStream != null)
                {
                    // we also add empty streams to the target package
                    _zipOutputStream.AddEntry(target);

                    string source = _binaries[target];
                    Stream sourceStream = getStream(source);

                    if (sourceStream != null)
                    {
                        int bytesCopied = streamCopy(sourceStream, _zipOutputStream);
                        Debug.WriteLine("CopyBinary : " + source + " --> " + target + ", bytes copied = " + bytesCopied);
                    }
                }
            }
        }

        private const int BUFFER_SIZE = 4096;

        #region new code for Extract and Import

        //Added by Sonata
        private void extractFiles()
        {
            foreach (string target in _extractFileList.Keys)
            {
                extractFile(target, _extractFileList[target]);
            }
        }

        /// <summary>
        /// Extract files from the input PPTX zip archive to the specified physical directory
        /// The source archive is handled by the resolver, while the destination correspond to physical dir 
        /// </summary>
        /// <param name="source">Relative path inside the source archive</param>
        /// <param name="destination">Destination folder name and file name seperated by '|' character</param>
        private void extractFile(string destination, string source)
        {
            // Retrive the Target folder name and Filename
            string targetFolderName = destination.Substring(0, destination.IndexOf('|'));
            string targetFileName = destination.Substring(destination.IndexOf('|') + 1);

            //GetOutputFilepath - gets the path where the output ODP file will be copied
            string strCurrentDirectory = getOutputFilePath();
            string strDestinationFilePath = strCurrentDirectory.Replace("\\", "//") + "//" + targetFolderName;

            //get the zipArchive stream
            try
            {
                Stream inputStream = getStream(source);
                if (inputStream != null)
                {
                    Directory.CreateDirectory(strDestinationFilePath);
                    strDestinationFilePath = strDestinationFilePath + "//" + targetFileName;
                    if (!File.Exists(strDestinationFilePath))
                    {
                        FileStream fsFile = new FileStream(strDestinationFilePath, FileMode.Create);
                        streamCopy(inputStream, fsFile);
                        fsFile.Close();
                    }
                }
            }

            catch (IOException e)
            {
                ZipException exZip = new ZipException(e.Message);
                throw exZip;
            }
        }


        private void importFiles()
        {
            foreach (string target in _importFileList.Keys)
            {
                importFile(target, _importFileList[target]);
            }
        }


        /// <summary>
        /// Imoprt files from the specified physical directory to otuput PPTX zip archive
        /// The destination archive is handled by the resolver, while the source corresponds to physical dir 
        /// </summary>
        /// <param name="source">Source file pysical path - Relative/absolute </param>
        /// <param name="destination">Destination path inside zip archive</param>        
        private void importFile(string destination, string source)
        {
            string inputFilePath = "";

            //Resolve relative path
            if (source.StartsWith("../"))
            {
                inputFilePath = Path.GetFullPath(Path.Combine(getInputFilePath(), source.Remove(0, 3))).Replace("/", "//").Replace("%20", " ");
            }
            else
            {
                inputFilePath = source.Replace("/", "//").Replace("%20", " ");
            }

            try
            {
                //Copy referd audio fiels from external directory to ppt/media
                if (_zipOutputStream != null)
                {
                    if (File.Exists(inputFilePath))
                    {
                        _zipOutputStream.AddEntry(destination);
                        FileStream fsSourceFile = new FileStream(inputFilePath, FileMode.Open, FileAccess.Read);

                        int bytesCopied = streamCopy(fsSourceFile, _zipOutputStream);
                        Debug.WriteLine("CopyBinary : " + inputFilePath + " --> " + destination + ", bytes copied = " + bytesCopied);
                    }

                    else
                    {
                        _zipOutputStream.AddEntry(destination);

                        string CurrDir = Environment.CurrentDirectory;
                        inputFilePath = Path.GetFullPath(Path.Combine(CurrDir + "\\", source.Remove(0, 3))).Replace("/", "//").Replace("%20", " ");
                        if (File.Exists(inputFilePath))
                        {
                            FileStream fsSourceFile = new FileStream(inputFilePath, FileMode.Open, FileAccess.Read);
                            int bytesCopied = streamCopy(fsSourceFile, _zipOutputStream);
                            Debug.WriteLine("CopyBinary : " + inputFilePath + " --> " + destination + ", bytes copied = " + bytesCopied);
                        }
                    }
                }
            }
            catch (IOException e)
            {
                ZipException exZip = new ZipException(e.Message);
                throw exZip;
            }
        }

        /// <summary>
        /// Get the physical directory path of the input file (PPTX or ODP) to transformed        /// 
        /// </summary>
        /// <returns>Physical path of the input file </returns>
        private string getInputFilePath()
        {
            string returnInputFilePath = "";

            // for Commandline tool
            for (int i = 0; i < Environment.GetCommandLineArgs().Length; i++)
            {
                if (Environment.GetCommandLineArgs()[i].ToString().ToUpper() == "/I")
                    returnInputFilePath = Path.GetDirectoryName(Environment.GetCommandLineArgs()[i + 1]);
            }

            //for addinds
            if (returnInputFilePath == "")
            {
                returnInputFilePath = Environment.CurrentDirectory;
            }

            return returnInputFilePath;

        }

        /// <summary>
        /// Get the physical directory path of the output file
        /// </summary>
        /// <returns>Physical path of the output file </returns>
        private string getOutputFilePath()
        {
            // TODO: refactor this terribly ugly part of code. It doesn't work anyway.       
            string returnOutputFilePath = "";
            string tempOutputFilePath = "";
            bool varBatch = false;

            for (int i = 0; i < Environment.GetCommandLineArgs().Length; i++)
            {
                if (Environment.GetCommandLineArgs()[i].ToString().ToUpper() == "/I")
                    tempOutputFilePath = Path.GetFullPath(Environment.GetCommandLineArgs()[i + 1]);
                if (Environment.GetCommandLineArgs()[i].ToString().ToUpper() == "/O")
                    returnOutputFilePath = Path.GetFullPath(Environment.GetCommandLineArgs()[i + 1]);
                if (Environment.GetCommandLineArgs()[i].ToString().ToUpper().Contains("BATCH"))
                    varBatch = true;

            }
            // for addins
            if (returnOutputFilePath == "" && tempOutputFilePath == "")
            {
                return Environment.CurrentDirectory;
            }

            // if /O is not specified /I will remain the pool to copy output file
            if (returnOutputFilePath == "" && tempOutputFilePath != "")
            {
                returnOutputFilePath = tempOutputFilePath;
            }
            // if /O is specified /O will be the pool to copy output file
            if (returnOutputFilePath != "")
            {

            }


            if (varBatch == true)
            {
                return returnOutputFilePath;
            }
            else
            {
                if (Directory.Exists(returnOutputFilePath))
                {
                    return returnOutputFilePath;
                }
                else
                {
                    return returnOutputFilePath.Substring(0, returnOutputFilePath.LastIndexOf(Path.DirectorySeparatorChar));
                }
            }
        }

        #endregion

        private Stream getStream(string source)
        {
            //get stream to embedded resource
            if (source.StartsWith("#CER#") && source.EndsWith("#"))
            {
                int len = source.Length;
                int pos1 = 5;
                int pos2 = source.IndexOf("#", 5, len - 5) + 1;

                Assembly exeAssem = Assembly.GetExecutingAssembly();
                string targetAssemPath = Path.GetDirectoryName(exeAssem.GetModules()[0].FullyQualifiedName);
                if (targetAssemPath != "")
                    targetAssemPath += Path.DirectorySeparatorChar;
                targetAssemPath += source.Substring(pos1, pos2 - pos1 - 1);
#if STATIC
		if (targetAssemPath.EndsWith(".dll"))
		    targetAssemPath = targetAssemPath.Substring (0, targetAssemPath.Length - 4) + "Static.dll";
#endif
                Assembly targetAssem = System.Reflection.Assembly.LoadFile(targetAssemPath);
                string res = source.Substring(pos2, len - pos2 - 1);

                return targetAssem.GetManifestResourceStream(res);
            }
            //get stream to file in zip
            else
            {
                Uri absoluteUri = _resolver.ResolveUri(null, source);
                return (Stream)_resolver.GetEntity(absoluteUri, null, Type.GetType("System.IO.Stream"));
            }
        }

        private int streamCopy(Stream source, Stream destination)
        {
            if (source != null && destination != null)
            {
                byte[] data = new byte[BUFFER_SIZE];
                int length = (int)source.Length;
                int bytesCopied = 0;

                while (length > 0)
                {
                    int read = source.Read(data, 0, System.Math.Min(BUFFER_SIZE, length));
                    bytesCopied += read;

                    if (read < 0)
                    {
                        throw new EndOfStreamException("Unexpected end of stream");
                    }

                    length -= read;
                    destination.Write(data, 0, read);
                }

                source.Close();
                return bytesCopied;
            }

            return -1;
        }

        /// <summary>
        /// Simple representation of elements or attributes nodes
        /// </summary>
        private class Node
        {
            private string _name;
            private string _prefix;
            private string _namespace;
            private string _value;

            public string Name
            {
                get { return _name; }
            }

            public string Prefix
            {
                get { return _prefix; }
            }

            public string Namespace
            {
                get { return _namespace; }
            }

            public string Value
            {
                get { return _value; }
                set { _value = value; }
            }

            public Node(string prefix, string name, string ns)
            {
                this._prefix = prefix;
                this._name = name;
                this._namespace = ns;
            }
        }

        private class Element : Node
        {
            private Dictionary<string, Node> _attributes;

            public Element(string prefix, string name, string ns)
                : base(prefix, name, ns)
            {
            }

            public Dictionary<string, Node> Attributes
            {
                get
                {
                    // lazy initialization
                    if (_attributes == null)
                    {
                        _attributes = new Dictionary<string, Node>();
                    }
                    return _attributes;
                }
            }
        }

        //Added by Sonata for Getting Height,Width,Resolution of the image-15/11/2007   
        /// <summary>
        /// Transfer a binary file between two zip archives. 
        /// The source archive is handled by the resolver.
        /// This method returns image properties like height,width, and resolution
        /// </summary>
        /// <param name="source">Relative path inside the source archive</param>
        public string ImageCopyBinary(String source)
        {
            Stream sourceStream = getStream(source);

            System.Drawing.Image img;
            img = System.Drawing.Image.FromStream(sourceStream);

            return img.Width.ToString(System.Globalization.CultureInfo.InvariantCulture) + ":" + img.Height.ToString(System.Globalization.CultureInfo.InvariantCulture) + ":" + img.VerticalResolution.ToString(System.Globalization.CultureInfo.InvariantCulture);

        }
    }

}
