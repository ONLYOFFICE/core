/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using System.IO;
using System.Collections;
using System.CodeDom;
using System.CodeDom.Compiler;

namespace codegen
{
    public class GenMember
    {
        public string sName;
        public string sType;
        public Type oSystemType;
        public string sNamespace;
        public bool bIsAttribute;
        public string sDefAttribute;
        public bool bQualified;//нужно ли при записи в xml писать prefix
        public bool bIsArray;
        public List<GenMember> aArrayTypes;
        public string sArrayTypesElementName;
        public string sArrayTypesEnumName;
        public bool bInternal;//отличает массив с типами

        public bool bToDo;
        public bool bToDoString;
        public GenMember(string _sName)
        {
            sName = _sName;
            sType = null;
            oSystemType = null;
            sNamespace = null;
            bIsAttribute = false;
            bQualified = true;
            bIsArray = false;
            sArrayTypesElementName = null;
            sArrayTypesEnumName = null;
            aArrayTypes = null;
            bInternal = false;

            bToDo = false;
            bToDoString = false;
        }
    }
    public class GenClass
    {
        public string sName;
        public string sNamespace;
        public bool bIsEnum;
        public List<GenMember> aMembers = new List<GenMember>();
        public bool bInternal;//отличает enum типов
        public bool bIsRoot;
        public GenClass(string _sName, string _sNamespace)
        {
            sName = _sName;
            sNamespace = _sNamespace;
            bIsEnum = false;
            bInternal = false;
            bIsRoot = false;
        }
    }
    class CodeGen
    {
        Dictionary<string, GenClass> m_mapGeneratedClasses = new Dictionary<string, GenClass>();
        int m_nItemsChoiceTypeCount = 0;
        public void Start(string sDirIn, string sDirCppXmlOut, string sDirCppBinOut, string sDirJsBinOut, ValidationEventHandler oValidationEventHandler)
        {
            string sChartNamespace = "http://purl.oclc.org/ooxml/drawingml/chart";

            string[] aFiles = Directory.GetFiles(sDirIn);
            XmlSchemaSet schemaSet = new XmlSchemaSet();
            schemaSet.ValidationEventHandler += oValidationEventHandler;
            for (int i = 0; i < aFiles.Length; i++)
            {
                string sFile = aFiles[i];
                if (".xsd" == Path.GetExtension(sFile))
                    schemaSet.Add(null, sFile);
            }
            schemaSet.Compile();
            XmlSchema chartSchema = null;
            XmlSchemas schemas = new XmlSchemas();
            foreach (XmlSchema schema in schemaSet.Schemas())
            {
                if (schema.TargetNamespace == sChartNamespace)
                {
                    chartSchema = schema;
                    schemas.Add(schema);
                }
            }
            if (null != chartSchema)
            {
                CodeNamespace ns = new CodeNamespace();
                XmlCodeExporter exporter = new XmlCodeExporter(ns);

                CodeGenerationOptions generationOptions = CodeGenerationOptions.None;


                XmlSchemaImporter importer = new XmlSchemaImporter(schemas, generationOptions, new ImportContext(new CodeIdentifiers(), false));

                foreach (XmlSchemaElement element in chartSchema.Elements.Values)
                {
                    XmlTypeMapping mapping = importer.ImportTypeMapping(element.QualifiedName);
                    exporter.ExportTypeMapping(mapping);
                }
                CodeGenerator.ValidateIdentifiers(ns);

                //Microsoft.CSharp.CSharpCodeProvider oProvider;

                //// output the C# code
                //Microsoft.CSharp.CSharpCodeProvider codeProvider = new Microsoft.CSharp.CSharpCodeProvider();

                //using (StringWriter writer = new StringWriter())
                //{
                //    codeProvider.GenerateCodeFromNamespace(ns, writer, new CodeGeneratorOptions());
                //    string sCode = writer.GetStringBuilder().ToString();
                //}

                List<GenClass> aGenClasses = PreProcess(ns, chartSchema);

                aGenClasses = FilterClasses(aGenClasses);

                (new CodegenCPP()).Process(sDirCppXmlOut, sDirCppBinOut, aGenClasses);
                (new CodegenJS()).Process(sDirJsBinOut, aGenClasses);
            }
        }
        List<GenClass> FilterClasses(List<GenClass> aInput)
        {
            List<GenClass> aRes = new List<GenClass>();
            string[] aTargetNamespaces = new string[] { "http://purl.oclc.org/ooxml/drawingml/chart", "http://purl.oclc.org/ooxml/officeDocument/relationships", "http://purl.oclc.org/ooxml/officeDocument/sharedTypes", "http://schemas.openxmlformats.org/markup-compatibility/2006", "http://schemas.microsoft.com/office/drawing/2007/8/2/chart" };
            string[] aTodoNamespaces = new string[] { "http://purl.oclc.org/ooxml/officeDocument/relationships" };
            string[] aStringNamespaces = new string[] { "http://purl.oclc.org/ooxml/drawingml/chartDrawing", "http://purl.oclc.org/ooxml/drawingml/main" };
            Dictionary<string, GenClass> mapAllClasses = new Dictionary<string, GenClass>();
            for (int i = 0; i < aInput.Count; ++i)
            {
                GenClass oGenClass = aInput[i];
                if (0 == aTargetNamespaces.Length || -1 != Array.IndexOf(aTargetNamespaces, oGenClass.sNamespace))
                    aRes.Add(oGenClass);
                mapAllClasses[oGenClass.sName] = oGenClass;
            }
            //специально для chart
            for (int i = 0; i < aRes.Count; ++i)
            {
                GenClass oGenClass = aRes[i];
                for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                {
                    GenMember oGenMember = oGenClass.aMembers[j];
                    string sNamespace = oGenMember.sNamespace;
                    GenClass oTempClass;
                    if (null == sNamespace && null != oGenMember.sType && mapAllClasses.TryGetValue(oGenMember.sType, out oTempClass))
                        sNamespace = oTempClass.sNamespace;
                    if (null != sNamespace)
                    {
                        if (-1 != Array.IndexOf(aTodoNamespaces, sNamespace))
                            oGenMember.bToDo = true;
                        if (-1 != Array.IndexOf(aStringNamespaces, sNamespace))
                        {
                            oGenMember.bToDoString = true;
                            oGenMember.sType = null;
                            oGenMember.oSystemType = typeof(string);
                        }
                    }
                    if (null != oGenMember.aArrayTypes)
                    {
                        for (int k = 0; k < oGenMember.aArrayTypes.Count; ++k)
                        {
                            GenMember oGenMemberArrayTypes = oGenMember.aArrayTypes[k];
                            sNamespace = oGenMemberArrayTypes.sNamespace;
                            if (null == sNamespace && null != oGenMemberArrayTypes.sType && mapAllClasses.TryGetValue(oGenMemberArrayTypes.sType, out oTempClass))
                                sNamespace = oTempClass.sNamespace;
                            if (null != sNamespace)
                            {
                                if (-1 != Array.IndexOf(aTodoNamespaces, sNamespace))
                                    oGenMemberArrayTypes.bToDo = true;
                                if (-1 != Array.IndexOf(aStringNamespaces, sNamespace))
                                {
                                    oGenMemberArrayTypes.bToDoString = true;
                                    oGenMemberArrayTypes.sType = null;
                                    oGenMemberArrayTypes.oSystemType = typeof(string);
                                }
                            }
                        }
                    }
                }
            }
            return aRes;
        }
        List<GenClass> PreProcess(CodeNamespace code, XmlSchema schema)
        {
            List<GenClass> aGenClasses = new List<GenClass>();
            for (int i = 0; i < code.Types.Count; ++i)
            {
                GenClass oNewClass = PreProcessClass(aGenClasses, code.Types[i]);
                if (null != oNewClass)
                    aGenClasses.Add(oNewClass);
            }
            return aGenClasses;
        }
        GenClass PreProcessClass(List<GenClass> aGenClasses, CodeTypeDeclaration type)
        {
            GenClass oGenClass = null;
            //получаем xml namespace
            string sNamespace = null;
            bool bIncludeInSchema = true;
            bool bIsRoot = false;
            for (int i = 0; i < type.CustomAttributes.Count; i++)
            {
                CodeAttributeDeclaration attribute = type.CustomAttributes[i];
                if (attribute.Name == "System.Xml.Serialization.XmlTypeAttribute")
                {
                    foreach (CodeAttributeArgument argument in attribute.Arguments)
                    {
                        if (argument.Name == "Namespace")
                            sNamespace = ((CodePrimitiveExpression)argument.Value).Value.ToString();
                        else if (argument.Name == "IncludeInSchema")
                            bIncludeInSchema = Convert.ToBoolean(((CodePrimitiveExpression)argument.Value).Value);
                        //todo argument.Name == "TypeName"
                    }
                }
                else if (attribute.Name == "System.Xml.Serialization.XmlRootAttribute")
                {
                    bIsRoot = true;
                    string sName = null;//todo
                    foreach (CodeAttributeArgument argument in attribute.Arguments)
                    {
                        if (argument.Name == "Namespace")
                            sNamespace = ((CodePrimitiveExpression)argument.Value).Value.ToString();
                        else if ("" == argument.Name)
                            sName = ((CodePrimitiveExpression)argument.Value).Value.ToString();
                    }
                }
            }
            if(bIncludeInSchema)
            {
                oGenClass = new GenClass(type.Name, sNamespace);
                oGenClass.bIsRoot = bIsRoot;
                int nItemsElementName = 0;
                if (type.IsEnum)
                {
                    oGenClass.bIsEnum = true;
                    for (int i = 0; i < type.Members.Count; ++i)
                    {
                        CodeTypeMember member = type.Members[i];
                        GenMember oGenMember = new GenMember(member.Name);
                        for (int j = 0; j < member.CustomAttributes.Count; j++)
                        {
                            CodeAttributeDeclaration attribute = member.CustomAttributes[j];
                            if (attribute.Name == "System.Xml.Serialization.XmlEnumAttribute")
                                ParseArguments(attribute.Arguments, oGenMember);
                        }
                        oGenClass.aMembers.Add(oGenMember);
                    }
                }
                else
                {
                    for (int i = 0; i < type.Members.Count; ++i)
                    {
                        CodeTypeMember member = type.Members[i];
                        //CodeMemberField пропускаем
                        CodeMemberProperty codeMemberProperty = member as CodeMemberProperty;
                        if (codeMemberProperty != null)
                        {
                            GenMember oNewGenMember = PreProcessProperty(aGenClasses, codeMemberProperty, oGenClass, ref nItemsElementName);
                            if (null != oNewGenMember)
                                oGenClass.aMembers.Add(oNewGenMember);
                        }
                    }
                }
            }
            return oGenClass;
        }
        GenMember PreProcessProperty(List<GenClass> aGenClasses, CodeMemberProperty codeMemberProperty, GenClass oGenClass, ref int nItemsElementName)
        {
            GenMember oGenMember = new GenMember(codeMemberProperty.Name);
            bool bIgnore = false;
            InitMemberType(oGenMember, codeMemberProperty.Type.BaseType);
            if (null != codeMemberProperty.Type.ArrayElementType)
                oGenMember.bIsArray = true;
            List<GenMember> aWrappedMemebers = null;
            for (int i = 0; i < codeMemberProperty.CustomAttributes.Count; i++)
            {
                CodeAttributeDeclaration attribute = codeMemberProperty.CustomAttributes[i];
                if (attribute.Name == "System.Xml.Serialization.XmlAttributeAttribute")
                {
                    oGenMember.bQualified = false;
                    oGenMember.bIsAttribute = true;
                    ParseArguments(attribute.Arguments, oGenMember);
                    //todo могут быть повторы имен атрибутов и child nodes.
                }
                else if (attribute.Name == "System.ComponentModel.DefaultValueAttribute")
                {
                    if (attribute.Arguments.Count > 0)
                    {
                        CodeExpression oCodeExpression = attribute.Arguments[attribute.Arguments.Count - 1].Value;
                        //todo other
                        if (oCodeExpression is CodePrimitiveExpression)
                            oGenMember.sDefAttribute = ((oCodeExpression as CodePrimitiveExpression)).Value.ToString();
                        else if (oCodeExpression is CodeFieldReferenceExpression)
                            oGenMember.sDefAttribute = ((oCodeExpression as CodeFieldReferenceExpression)).FieldName;
                    }
                }
                else if (attribute.Name == "System.Xml.Serialization.XmlIgnoreAttribute")
                    bIgnore = true;
                else if (attribute.Name == "System.Xml.Serialization.XmlElementAttribute")
                {
                    GenMember oTemp = oGenMember;
                    if (null != oGenMember.oSystemType && oGenMember.oSystemType == typeof(object))
                        oTemp = new GenMember(null);
                    ParseArguments(attribute.Arguments, oTemp);
                    if (null != oGenMember.oSystemType && oGenMember.oSystemType == typeof(object))
                    {
                        if (null != oTemp.sName)
                        {
                            if (false == oGenMember.bIsArray)
                            {
                                bIgnore = true;
                                oGenClass.aMembers.Add(oTemp);
                            }
                            else
                            {
                                bIgnore = false;
                                if (null == oGenMember.aArrayTypes)
                                    oGenMember.aArrayTypes = new List<GenMember>();
                                oGenMember.aArrayTypes.Add(oTemp);
                            }
                        }
                    }
                }
                else if (attribute.Name == "System.Xml.Serialization.XmlArrayItemAttribute")
                {
                    GenMember oWrapMemeber = new GenMember(null);
                    ParseArguments(attribute.Arguments, oWrapMemeber);
                    if (null != oWrapMemeber.sName)
                    {
                        if (null == aWrappedMemebers)
                            aWrappedMemebers = new List<GenMember>();
                        aWrappedMemebers.Add(oWrapMemeber);
                    }
                }
                else if (attribute.Name == "System.Xml.Serialization.XmlArrayAttribute")
                {
                    ParseArguments(attribute.Arguments, oGenMember);
                }
                //todo не всегда прописан
                //else if (attribute.Name == "System.Xml.Serialization.XmlChoiceIdentifierAttribute")
                //{
                //    if (attribute.Arguments.Count > 0)
                //    {
                //        CodePrimitiveExpression oPrimitiveExpression = attribute.Arguments[0].Value as CodePrimitiveExpression;
                //        oGenMember.sChoiceIdentifier = oPrimitiveExpression.Value.ToString();
                //    }
                //}
            }
            if (bIgnore)
                return null;
            else
            {
                if (oGenMember.bIsArray && null != aWrappedMemebers)
                {
                    //todo не проверен случай
                    //[System.Xml.Serialization.XmlArrayItemAttribute("ahPolar", typeof(CT_PolarAdjustHandle), IsNullable=false)]
                    //[System.Xml.Serialization.XmlArrayItemAttribute("ahXY", typeof(CT_XYAdjustHandle), IsNullable=false)]
                    //public object[] ahLst {

                    //создаем wrap class чтобы не работать с двумерными массивами
                    string sNewName = Utils.GetClassName(oGenMember.sName);
                    GenClass oNewGenClass = new GenClass(sNewName, oGenMember.sNamespace);
                    if (null == oNewGenClass.sNamespace)
                        oNewGenClass.sNamespace = oGenClass.sNamespace;
                    //помещаем класс указанный в атрибутах в aMembers
                    for (int i = 0; i < aWrappedMemebers.Count; ++i)
                    {
                        GenMember oWrappedMemeber = aWrappedMemebers[i];
                        if (null == oWrappedMemeber.sType && null == oWrappedMemeber.oSystemType)
                        {
                            oWrappedMemeber.sType = oGenMember.sType;
                            oWrappedMemeber.oSystemType = oGenMember.oSystemType;
                        }
                        oWrappedMemeber.bIsArray = true;
                        oWrappedMemeber.sNamespace = oNewGenClass.sNamespace;
                        oNewGenClass.aMembers.Add(oWrappedMemeber);
                    }
                    //проверяем нет ли такого типа
                    bool bExist = false;
                    bool bNeedCreate = true;
                    GenClass oPrevGenClass;
                    if (m_mapGeneratedClasses.TryGetValue(sNewName, out oPrevGenClass))
                    {
                        bExist = true;
                        if (oNewGenClass.sName == oPrevGenClass.sName && oNewGenClass.sNamespace == oPrevGenClass.sNamespace && oNewGenClass.aMembers.Count == oPrevGenClass.aMembers.Count)
                        {
                            bNeedCreate = false;
                            for (int i = 0; i < oNewGenClass.aMembers.Count; ++i)
                            {
                                GenMember oGenMember1 = oNewGenClass.aMembers[i];
                                GenMember oGenMember2 = oPrevGenClass.aMembers[i];
                                if (oGenMember1.sName != oGenMember2.sName || oGenMember1.sType != oGenMember2.sType || oGenMember1.oSystemType != oGenMember2.oSystemType)
                                    bNeedCreate = true;
                            }
                        }
                    }
                    if(bNeedCreate)
                    {
                        if (bExist)
                        {
                            int nCount = 1;
                            GenClass oTemp;
                            while (m_mapGeneratedClasses.TryGetValue(sNewName = Utils.GetClassName(oGenMember.sName + nCount), out oTemp))
                                nCount++;
                            oNewGenClass.sName = sNewName;
                        }
                        aGenClasses.Add(oNewGenClass);
                        m_mapGeneratedClasses[oNewGenClass.sName] = oNewGenClass;
                    }
                    //меняем oGenMember, чтобы он ссылался на oNewGenClass
                    if (codeMemberProperty.Type.ArrayElementType.ArrayRank > 0)
                        oGenMember.bIsArray = true;
                    else
                        oGenMember.bIsArray = false;
                    oGenMember.oSystemType = null;
                    oGenMember.sType = oNewGenClass.sName;
                }
                if (oGenMember.bIsArray && null != oGenMember.aArrayTypes)
                {
                    //добавляем enum для member и дополнительный массив для типов
                    GenClass oNewEnum = new GenClass(Utils.gc_sItemsChoiceType + m_nItemsChoiceTypeCount++, oGenClass.sNamespace);
                    oNewEnum.bInternal = true;
                    oNewEnum.bIsEnum = true;
                    for (int i = 0; i < oGenMember.aArrayTypes.Count; ++i)
                    {
                        oNewEnum.aMembers.Add(new GenMember(oGenMember.aArrayTypes[i].sName));
                    }
                    GenMember oNewPairArray = new GenMember(Utils.gc_sItemsElementName + nItemsElementName++);
                    oNewPairArray.bInternal = true;
                    oNewPairArray.bIsArray = true;
                    oNewPairArray.sType = oNewEnum.sName;
                    oGenMember.sArrayTypesElementName = oNewPairArray.sName;
                    oGenMember.sArrayTypesEnumName = oNewEnum.sName;
                    oGenClass.aMembers.Add(oNewPairArray);
                    aGenClasses.Add(oNewEnum);
                }
                return oGenMember;
            }
        }
        void ParseArguments(CodeAttributeArgumentCollection oArguments, GenMember oGenMember)
        {
            CodePrimitiveExpression oPrimitiveExpression = null;
            CodeTypeOfExpression oTypeOfExpression = null;
            string sNamespace = null;
            bool? bForm = null;
            foreach (CodeAttributeArgument argument in oArguments)
            {
                if ("" == argument.Name)
                {
                    if (argument.Value is CodePrimitiveExpression)
                        oPrimitiveExpression = argument.Value as CodePrimitiveExpression;
                    else if (argument.Value is CodeTypeOfExpression)
                        oTypeOfExpression = argument.Value as CodeTypeOfExpression;
                }
                else if ("Namespace" == argument.Name)
                    sNamespace = ((CodePrimitiveExpression)argument.Value).Value.ToString();
                else if ("Form" == argument.Name)
                {
                    string sValue = ((CodeFieldReferenceExpression)argument.Value).FieldName;
                    if ("Qualified" == sValue)
                        bForm = true;
                    else if ("Unqualified" == sValue)
                        bForm = false;
                }
            }
            if (null != oPrimitiveExpression)
                oGenMember.sName = oPrimitiveExpression.Value.ToString();
            if(null != oTypeOfExpression)
                InitMemberType(oGenMember, oTypeOfExpression.Type.BaseType);
            if (null != sNamespace)
                oGenMember.sNamespace = sNamespace;
            if (bForm.HasValue)
                oGenMember.bQualified = bForm.Value;
        }
        void InitMemberType(GenMember oGenMember, string sBaseType)
        {
            if (-1 != sBaseType.IndexOf("System."))
            {
                oGenMember.oSystemType = Type.GetType(sBaseType);
                if (null == oGenMember.oSystemType)
                    oGenMember.oSystemType = typeof(string);
            }
            else
                oGenMember.sType = sBaseType;
        }
    }
}
