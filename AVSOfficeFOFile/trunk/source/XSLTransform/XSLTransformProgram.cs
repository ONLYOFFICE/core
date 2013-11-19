using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Xsl;
using System.IO;

namespace XSLTransform
{
    class XSLTransformProgram
    {

        private const string sourceFile = @"e:\Work\AVS\ActiveX\AVS\Sources\AVSOfficeStudio\AVSOfficeFOFile\trunk\test\test5\In\fontseffects.xml";
        private const string stylesheet = @"e:\Work\AVS\ActiveX\AVS\Sources\AVSOfficeStudio\AVSOfficeFOFile\trunk\source\xslt\oox2fo.xslt";
        private const string outputFile = @"e:\Work\AVS\ActiveX\AVS\Sources\AVSOfficeStudio\AVSOfficeFOFile\trunk\test\test5\Out\fontseffects.fo";

        private static string[] inputDocxFiles =
            {
                "_rels/.rels",
                "word/styles.xml",
                "docProps/app.xml",
                "docProps/core.xml",
                "word/_rels/document.xml.rels",
                "word/document.xml",
                "word/numbering.xml",
                "word/footnotes.xml",
                "word/endnotes.xml"
            };

        private const string xmlnsOox = "urn:oox";
        private const string ooxPackage = "oox:package";
        private const string ooxPart = "oox:part";
        private const string ooxPartNameAttr = "oox:name";

        static XmlNode GetFirstXmlElement(XmlNodeList nodeList)
        { 
            foreach(XmlNode currentNode in nodeList)
            {
                if (currentNode.NodeType == XmlNodeType.Element)
                    return currentNode;
            }
            return null;        
        }

        static void MergeDocxIntoSingleXML(string inputDocxDir, string outputXMLFileName, string [] additionalRelationships)
        {
            MergeDocxIntoSingleXML(inputDocxDir, outputXMLFileName, additionalRelationships, "");
        }

        static bool CopyMediaFilesFromDocx(string OOXPath, string FOPath)
        {
            string outputPath = Path.GetFullPath(Path.Combine(
                Path.GetDirectoryName(FOPath),                
                ".\\" + Path.GetFileName(FOPath) + ".files\\"
                ));
            string inputPath = Path.GetFullPath(Path.Combine(OOXPath, ".\\word\\media"));

           
            string[] mediaFiles;
            try
            {
                mediaFiles = Directory.GetFiles(inputPath);
            }
            catch (Exception)
            {
                return true;
            }

            if (mediaFiles.Count() != 0)
            {
                try
                {
                    Directory.CreateDirectory(outputPath);
                }
                catch (Exception exc)
                {
                    Console.WriteLine(exc.Message);
                    return false;
                }
            }
            
            foreach (string mediaFileName in mediaFiles)
            {
                try
                {
                    string newFileName = Path.Combine(outputPath, Path.GetFileName(mediaFileName));
                    File.Copy(mediaFileName, newFileName, true);
                }
                catch (Exception exc)
                {
                    Console.WriteLine(exc.Message);
                    return false;
                }
            }
            return true;
        }

        static void MergeDocxIntoSingleXML(string inputDocxDir, string outputXMLFileName, string[] additionalRelationships, string foFileName)
        {
            XmlDocument outputXML = new XmlDocument();
            XmlDeclaration xmlDecl = outputXML.CreateXmlDeclaration("1.0", "utf-8", "yes");
            outputXML.AppendChild(xmlDecl);
            XmlNode rootNode = outputXML.CreateNode(XmlNodeType.Element, ooxPackage, xmlnsOox);


            var inputFiles = new List<string>();
            inputFiles.AddRange(inputDocxFiles);
            inputFiles.AddRange(additionalRelationships);

            foreach (string docxItem in inputFiles)
            {
                XmlNode partNode = outputXML.CreateNode(XmlNodeType.Element, ooxPart, xmlnsOox);
                XmlNode partNameAttr = outputXML.CreateNode(XmlNodeType.Attribute, ooxPartNameAttr, xmlnsOox);
                partNameAttr.Value = docxItem;
                partNode.Attributes.SetNamedItem(partNameAttr);
                
                XmlDocument partXmlDocument = new XmlDocument();
                string partXmlFileName = Path.Combine(inputDocxDir + "\\", docxItem);
                bool partXMLExist;
                try
                {
                    partXmlDocument.Load(partXmlFileName);
                    partXMLExist = true;
                }
                catch (Exception)
                {
                    partXMLExist = false;
                }

                if (partXMLExist)
                {
                    partNode.AppendChild(outputXML.ImportNode(GetFirstXmlElement(partXmlDocument.ChildNodes), true));
                    rootNode.AppendChild(partNode);
                }
            }

            // добавляем имя результирующего FO - файла

            if ((foFileName != "") && (foFileName != null))
            {
                XmlNode partNode = outputXML.CreateNode(XmlNodeType.Element, ooxPart, xmlnsOox);
                
                XmlNode partNameAttr = outputXML.CreateNode(XmlNodeType.Attribute, ooxPartNameAttr, xmlnsOox);
                partNameAttr.Value = "AvsProperties";
                partNode.Attributes.SetNamedItem(partNameAttr);

                XmlNode propertyRootNode = outputXML.CreateNode(XmlNodeType.Element, "oox:AvsProperties", xmlnsOox);
                XmlNode propertyOutputFOFileNameNode = outputXML.CreateNode(XmlNodeType.Element, "oox:OutputFOFileName", xmlnsOox);
                XmlNode propertyOutputFOFileNameAttrib = outputXML.CreateNode(XmlNodeType.Attribute, "value", null);
                propertyOutputFOFileNameAttrib.Value = foFileName;
                propertyOutputFOFileNameNode.Attributes.SetNamedItem(propertyOutputFOFileNameAttrib);
                propertyRootNode.AppendChild(propertyOutputFOFileNameNode);
                partNode.AppendChild(propertyRootNode);
                rootNode.AppendChild(partNode);            
            }

            outputXML.AppendChild(rootNode);
            //Console.WriteLine(outputXML.OuterXml);
            XmlWriter xmlWriter = XmlWriter.Create(outputXMLFileName);
            outputXML.WriteTo(xmlWriter);
            xmlWriter.Close();

            outputXML.Save(Path.Combine(Path.GetDirectoryName(outputXMLFileName), Path.GetFileNameWithoutExtension(outputXMLFileName) + "_r.xml"));
        }


        static void DoXSLTTransform(string inputXMLFile, string inputXSLTFile, string outputXMLFile)
        {
            Console.WriteLine("Input XML: " + inputXMLFile + "\n\nInput XSL Transform: " + inputXSLTFile + "\n\nOutput File: " + outputXMLFile+"\n");
#if DEBUG
            XslCompiledTransform xslt = new XslCompiledTransform(true);
#else
            XslCompiledTransform xslt = new XslCompiledTransform(false);
#endif

            try
            {
                Console.Write("Load XSL Transform xml...");
                xslt.Load(inputXSLTFile);
                Console.WriteLine("ok");
                Console.Write("Create output file...");
                FileStream outputStream = new FileStream(outputXMLFile, FileMode.Create);
                Console.WriteLine("ok");
                Console.Write("do XSL Transform...");
                xslt.Transform(inputXMLFile, null, outputStream);
                outputStream.Close();
                Console.WriteLine("ok ");

                XmlDocument xmlDocumentFormatted = new XmlDocument();
                xmlDocumentFormatted.Load(outputXMLFile);
                xmlDocumentFormatted.Save(Path.Combine(Path.GetDirectoryName(outputXMLFile), Path.GetFileNameWithoutExtension(outputXMLFile) + "_r.fo"));
               
            }
            catch (Exception ex)
            {
                Console.WriteLine("ERROR: " + ex.Message);
                return;
            }
            
        }

        private static string[] CalcAdditionalRelationships(string docxPath)
        {
            return CalcAdditionalRelationships(docxPath, "document");
        }

        private static string[] GetRelationshipValues(XmlDocument xmlDoc, string relationshipType)
        {
            var resultArray = new List<string>();
            string queryString = "//*[@Type='" + relationshipType + "']/@Target";
            
            try
            {
                XmlNodeList selectedNodes = xmlDoc.SelectNodes(queryString);
                foreach (XmlNode currentNode in selectedNodes)
                {
                    resultArray.Add("word/" + currentNode.InnerText);
                    resultArray.Add("word/_rels/" + currentNode.InnerText + ".rels");
                }
            }
            catch(System.Xml.XPath.XPathException)
            {
            }

            return resultArray.ToArray();
        }

        private static string[] additionalRelationshipTypes =
        {
        	"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footer", 
	        "http://schemas.openxmlformats.org/officeDocument/2006/relationships/header"
        };


        private static string[] CalcAdditionalRelationships(string docxPath, string relsName)
        { 
            var resultArray = new List<string>();
            string inputDir = docxPath + "\\word\\_rels\\" + relsName + ".xml.rels";
            XmlDocument xmlDoc = new XmlDocument();
            try
            {
                xmlDoc.Load(inputDir);
            }
            catch (Exception)
            {
                return resultArray.ToArray();
            }

            foreach (string currentRelationshipType in additionalRelationshipTypes)
            {
                resultArray.AddRange(GetRelationshipValues(xmlDoc, currentRelationshipType));
            }
            return resultArray.ToArray();
        }

        private const string propXMLRelativePath = @"..\..\..\test\test_XSLTransform\input_xml.xml";
        //private const string propXMLRelativePath = @"..\..\..\test\test_XSL\test01input.xml";

        static void Main(string[] args)
        {
            string propXMLPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, propXMLRelativePath);
            XmlDocument propertiesXmlDocument = new XmlDocument();
            propertiesXmlDocument.Load(propXMLPath);
            XmlNamespaceManager nsManager = new XmlNamespaceManager(propertiesXmlDocument.NameTable);
            nsManager.AddNamespace("avso", "http://schemas.avsmedia.com/avso");
            foreach (XmlNode currentNode in propertiesXmlDocument.SelectNodes("/avso:InputTestXML/avso:TestDocxToFO", nsManager))
            {
                Console.WriteLine("**** START TEST ****");
                XmlAttributeCollection currentAttributes = currentNode.Attributes;

                if (null != currentAttributes.GetNamedItem("inputDocx"))
                {
                    Console.Write("Merge Docx into single XML...");

                    string inputDocxPath;
                    if (Path.IsPathRooted(currentAttributes.GetNamedItem("inputDocx").Value))
                        inputDocxPath = currentAttributes.GetNamedItem("inputDocx").Value;
                    else
                        inputDocxPath = Path.GetFullPath(Path.Combine(propXMLPath, currentAttributes.GetNamedItem("inputDocx").Value));

                    string outputMergedXMLPath;
                    if (Path.IsPathRooted(currentAttributes.GetNamedItem("mergedDocx").Value))
                        outputMergedXMLPath = currentAttributes.GetNamedItem("mergedDocx").Value;
                    else
                        outputMergedXMLPath = Path.GetFullPath(Path.Combine(propXMLPath, currentAttributes.GetNamedItem("mergedDocx").Value));

                    string outputFOFilePath = null;
                    if (currentAttributes.GetNamedItem("outputFile") != null)
                    {
                        if (Path.IsPathRooted(currentAttributes.GetNamedItem("outputFile").Value))
                            outputFOFilePath = currentAttributes.GetNamedItem("outputFile").Value;
                        else
                            outputFOFilePath = Path.GetFullPath(Path.Combine(propXMLPath, currentAttributes.GetNamedItem("outputFile").Value));
                    }

                    string[] additionalRelationships = CalcAdditionalRelationships(inputDocxPath, "document");

                    MergeDocxIntoSingleXML(inputDocxPath, outputMergedXMLPath, additionalRelationships,
                        (currentAttributes.GetNamedItem("outputFile") != null)?
                        Path.GetFileName(currentAttributes.GetNamedItem("outputFile").Value):""
                        );

                    Console.WriteLine("ok");

                    if (outputFOFilePath != null)
                    {
                        Console.Write("Copy media files...");
                        if (CopyMediaFilesFromDocx(inputDocxPath, outputFOFilePath))
                            Console.WriteLine("ok");
                        else
                            Console.WriteLine("failed");
                    }

                }

                DoXSLTTransform(
                    Path.GetFullPath(Path.Combine(propXMLPath, currentAttributes.GetNamedItem("mergedDocx").Value)),
                    Path.GetFullPath(Path.Combine(propXMLPath, currentAttributes.GetNamedItem("xsltFile").Value)),
                    Path.GetFullPath(Path.Combine(propXMLPath, currentAttributes.GetNamedItem("outputFile").Value))
                    );



                Console.WriteLine("**** END TEST ****\n");
            }
            Console.ReadKey();
        }
    }
}
