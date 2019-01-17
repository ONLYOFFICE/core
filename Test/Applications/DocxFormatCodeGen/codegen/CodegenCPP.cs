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
    class CodegenCPP
    {
        StringBuilder m_oDocxSerH = new StringBuilder();
        StringBuilder m_oDocxSerCPP = new StringBuilder();
        StringBuilder m_oDocxFromToBinaryH = new StringBuilder();
        StringBuilder m_oDocxFromToBinaryCPP = new StringBuilder();
        StringBuilder m_oDocxTypesH = new StringBuilder();
        Dictionary<string, GenClass> m_mapProcessedClasses = new Dictionary<string, GenClass>();
         
        string gc_sNamespaceToXml = " xmlns:c=\\\"http://schemas.openxmlformats.org/drawingml/2006/chart\\\" xmlns:a=\\\"http://schemas.openxmlformats.org/drawingml/2006/main\\\" xmlns:r=\\\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\\\" xmlns:mc=\\\"http://schemas.openxmlformats.org/markup-compatibility/2006\\\" xmlns:c14=\\\"http://schemas.microsoft.com/office/drawing/2007/8/2/chart\\\"";
        string gc_sTypePattern = "et_";
        string gc_sEnumFromXmlPrefix = "FromXml_";
        string gc_sEnumToXmlPrefix = "ToXml_";
        public void Process(string sDirCppXmlOut, string sDirCppBinOut, List<GenClass> aGenClasses)
        {
            //собираем список используемых типов
            List<GenClass> aEnums = new List<GenClass>();
            List<GenClass> aClasses = new List<GenClass>();
            for (int i = 0; i < aGenClasses.Count; ++i)
            {
                GenClass oGenClass = aGenClasses[i];
                if (oGenClass.bIsEnum)
                    aEnums.Add(oGenClass);
                else
                    aClasses.Add(oGenClass);
                m_mapProcessedClasses[oGenClass.sName] = oGenClass;
            }

            m_oDocxSerH.AppendFormat(Utils.gc_sFilePrefix);
            m_oDocxSerCPP.AppendFormat(Utils.gc_sFilePrefix);
            m_oDocxFromToBinaryH.AppendFormat(Utils.gc_sFilePrefix);
            m_oDocxFromToBinaryCPP.AppendFormat(Utils.gc_sFilePrefix);
            m_oDocxTypesH.AppendFormat(Utils.gc_sFilePrefix);

            string sFileDocxSerH = "ChartSerialize.h";
            string sFileDocxSerCPP = "ChartSerialize.cpp";
            string sFileDocxFromToBinaryH = "ChartFromToBinary.h";
            string sFileDocxFromToBinaryCPP = "ChartFromToBinary.cpp";
            string sFileDocxTypesH = "DocxTypes.h";

            //from/to xml
            m_oDocxSerH.AppendFormat("#pragma once\r\n#ifndef OOX_CHARTGEN_FILE_INCLUDE_H_\r\n#define OOX_CHARTGEN_FILE_INCLUDE_H_\r\n");
            m_oDocxSerH.AppendFormat("\r\n");
            m_oDocxSerH.AppendFormat("#include \"../CommonInclude.h\"\r\n");
            m_oDocxSerH.AppendFormat("\r\n");
            m_oDocxSerH.AppendFormat("namespace OOX{{\r\nnamespace Spreadsheet{{\r\n");
            m_oDocxSerCPP.AppendFormat("#include \"{0}\"\r\nnamespace OOX{{\r\nnamespace Spreadsheet{{\r\n", sFileDocxSerH);
            
            //в начале все enum
            for (int i = 0; i < aEnums.Count; ++i)
                ProcessClass(aEnums[i]);
            //декларация классов
            for (int i = 0; i < aClasses.Count; ++i)
                m_oDocxSerH.AppendFormat("class {0};\r\n", aClasses[i].sName);
            for (int i = 0; i < aClasses.Count; ++i)
                ProcessClass(aClasses[i]);
            m_oDocxSerCPP.AppendFormat("}}\r\n}}");
            m_oDocxSerH.AppendFormat("}}\r\n}}\r\n#endif");

            //from/to bin
            m_oDocxFromToBinaryH.AppendFormat("#pragma once\r\n");
            m_oDocxFromToBinaryH.AppendFormat("#include \"CommonWriter.h\"\r\n");
            m_oDocxFromToBinaryH.AppendFormat("#include \"..\\..\\Common\\DocxFormat\\Source\\XlsxFormat\\Chart\\{0}\"\r\n", sFileDocxSerH);
            m_oDocxFromToBinaryH.AppendFormat("#include \"..\\Writer\\BinaryCommonReader.h\"\r\n", sFileDocxSerH);
            m_oDocxFromToBinaryH.AppendFormat("using namespace OOX::Spreadsheet;\r\n");
            ProcessFromToBinaryH(m_oDocxFromToBinaryH, aEnums, aClasses);
            m_oDocxFromToBinaryCPP.AppendFormat("#include \"stdafx.h\"\r\n");
            m_oDocxFromToBinaryCPP.AppendFormat("#include \"{0}\"\r\n", sFileDocxFromToBinaryH);
            m_oDocxFromToBinaryCPP.AppendFormat("#include \"../../ASCOfficePPTXFile/Editor/BinReaderWriterDefines.h\"\r\n");
            m_oDocxFromToBinaryCPP.AppendFormat("#include \"../Common/BinReaderWriterDefines.h\"\r\n");
            m_oDocxFromToBinaryCPP.AppendFormat("using namespace OOX::Spreadsheet;\r\n");
            ProcessFromToBinaryCPP(m_oDocxFromToBinaryCPP, aEnums, aClasses);

            File.WriteAllText(Path.Combine(sDirCppXmlOut, sFileDocxSerH), m_oDocxSerH.ToString());
            File.WriteAllText(Path.Combine(sDirCppXmlOut, sFileDocxSerCPP), m_oDocxSerCPP.ToString());
            File.WriteAllText(Path.Combine(sDirCppXmlOut, sFileDocxTypesH), m_oDocxTypesH.ToString());
            File.WriteAllText(Path.Combine(sDirCppBinOut, sFileDocxFromToBinaryH), m_oDocxFromToBinaryH.ToString());
            File.WriteAllText(Path.Combine(sDirCppBinOut, sFileDocxFromToBinaryCPP), m_oDocxFromToBinaryCPP.ToString());
        }
        string GetCPPType(GenMember oGenMember)
        {
            string sRes = null;
            if (null != oGenMember.sType)
                sRes = oGenMember.sType;
            else if (null != oGenMember.oSystemType)
            {
                switch (Type.GetTypeCode(oGenMember.oSystemType))
                {
                    case TypeCode.Boolean: sRes = "bool"; break;
                    case TypeCode.Byte: sRes = "unsigned char"; break;
                    case TypeCode.SByte: sRes = "char"; break;
                    case TypeCode.Int16:
                    case TypeCode.Int32:
                    case TypeCode.Int64: sRes = "long"; break;
                    case TypeCode.UInt16:
                    case TypeCode.UInt32:
                    case TypeCode.UInt64: sRes = "unsigned long"; break;
                    case TypeCode.Single:
                    case TypeCode.Double: sRes = "double"; break;
                    case TypeCode.Object: sRes = "void"; break;
                    default: sRes = "CString"; break;
                }
            }
            return sRes;
        }
#region From/to xml
        void ProcessClass(GenClass oGenClass)
        {
            if (oGenClass.bIsEnum)
            {
                //.h
                m_oDocxSerH.AppendFormat("enum {0}\r\n{{\r\n", oGenClass.sName);
                for (int i = 0; i < oGenClass.aMembers.Count; ++i)
                {
                    GenMember member = oGenClass.aMembers[i];
                    if (0 != i)
                        m_oDocxSerH.Append(",\r\n");
                    m_oDocxSerH.AppendFormat("{0} = {1}", Utils.GetEnumElemName(oGenClass.sName, member.sName), i);
                }
                m_oDocxSerH.AppendFormat("\r\n}};\r\n");
                //.cpp
                if (!oGenClass.bInternal)
                {
                    m_oDocxSerCPP.AppendFormat("bool {0}{1}(CString& val, {2}& eOut)\r\n{{\r\n", gc_sEnumFromXmlPrefix, oGenClass.sName, oGenClass.sName);
                    if (oGenClass.aMembers.Count > 0)
                    {
                        m_oDocxSerCPP.AppendFormat("bool bRes = true;\r\n");
                        for (int i = 0; i < oGenClass.aMembers.Count; ++i)
                        {
                            GenMember member = oGenClass.aMembers[i];
                            if (0 == i)
                                m_oDocxSerCPP.AppendFormat("if(_T(\"{0}\")==val)\r\neOut={1};\r\n", member.sName, Utils.GetEnumElemName(oGenClass.sName, member.sName));
                            else
                                m_oDocxSerCPP.AppendFormat("else if(_T(\"{0}\")==val)\r\neOut={1};\r\n", member.sName, Utils.GetEnumElemName(oGenClass.sName, member.sName));
                        }
                        m_oDocxSerCPP.AppendFormat("else\r\nbRes=false;\r\n");
                        m_oDocxSerCPP.AppendFormat("return bRes;\r\n");
                    }
                    else
                        m_oDocxSerCPP.AppendFormat("return false;\r\n");
                    m_oDocxSerCPP.AppendFormat("}}\r\n");

                    m_oDocxSerCPP.AppendFormat("bool {0}{1}({2}& val, CString& sOut)\r\n{{\r\n", gc_sEnumToXmlPrefix, oGenClass.sName, oGenClass.sName);
                    if (oGenClass.aMembers.Count > 0)
                    {
                        m_oDocxSerCPP.AppendFormat("bool bRes = true;\r\n");
                        for (int i = 0; i < oGenClass.aMembers.Count; ++i)
                        {
                            GenMember member = oGenClass.aMembers[i];
                            if (0 == i)
                                m_oDocxSerCPP.AppendFormat("if({0}==val)\r\nsOut=_T(\"{1}\");\r\n", Utils.GetEnumElemName(oGenClass.sName, member.sName), member.sName);
                            else
                                m_oDocxSerCPP.AppendFormat("else if({0}==val)\r\nsOut=_T(\"{1}\");\r\n", Utils.GetEnumElemName(oGenClass.sName, member.sName), member.sName);
                        }
                        m_oDocxSerCPP.AppendFormat("else\r\nbRes=false;\r\n");
                        m_oDocxSerCPP.AppendFormat("return bRes;\r\n");
                    }
                    else
                        m_oDocxSerCPP.AppendFormat("return false;\r\n");
                    m_oDocxSerCPP.AppendFormat("}}\r\n");
                }
            }
            else
            {
                m_oDocxTypesH.AppendFormat("{0},\r\n", gc_sTypePattern + oGenClass.sName.ToLower());
                //.h
                m_oDocxSerH.AppendFormat("class {0}\r\n{{\r\n", oGenClass.sName);
                m_oDocxSerH.AppendFormat("public:\r\n", oGenClass.sName);
                ProcessProperty(m_oDocxSerH, oGenClass);
                m_oDocxSerH.AppendFormat("{0}();\r\n", oGenClass.sName);
                m_oDocxSerH.AppendFormat("~{0}();\r\n", oGenClass.sName);
                m_oDocxSerH.AppendFormat("void fromXML(XmlUtils::CXmlLiteReader& oReader);\r\n");
                m_oDocxSerH.AppendFormat("void toXML(CString& sNodeName, XmlUtils::CStringWriter& writer) const;\r\n");
                m_oDocxSerH.AppendFormat("EElementType getType();\r\n");
                List<GenMember> aAttributes = new List<GenMember>();
                for (int i = 0; i < oGenClass.aMembers.Count; ++i)
                {
                    GenMember oGenMember = oGenClass.aMembers[i];
                    if (null != oGenMember.aArrayTypes)
                        m_oDocxSerH.AppendFormat("private: void toXML(XmlUtils::CStringWriter& writer, bool bIsAttribute, {0} eType, void* pVal) const;\r\n", oGenMember.sArrayTypesEnumName);
                    if (!oGenMember.bInternal && oGenMember.bIsAttribute)
                            aAttributes.Add(oGenMember);
                }
                if (aAttributes.Count > 0)
                    m_oDocxSerH.AppendFormat("private: void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);\r\n");
                m_oDocxSerH.AppendFormat("}};\r\n");
                //.cpp
                ProcessConstructor(m_oDocxSerCPP, oGenClass);
                ProcessDestructor(m_oDocxSerCPP, oGenClass);
                m_oDocxSerCPP.AppendFormat("void {0}::fromXML(XmlUtils::CXmlLiteReader& oReader){{\r\n", oGenClass.sName);
                ProcessFromXml(m_oDocxSerCPP, oGenClass);
                m_oDocxSerCPP.AppendFormat("}}\r\n");
                m_oDocxSerCPP.AppendFormat("void {0}::toXML(CString& sNodeName, XmlUtils::CStringWriter& writer) const{{\r\n", oGenClass.sName);
                ProcessToXml(m_oDocxSerCPP, oGenClass);
                m_oDocxSerCPP.AppendFormat("}}\r\n");
                m_oDocxSerCPP.AppendFormat("EElementType {0}::getType(){{return {1};}}\r\n", oGenClass.sName, gc_sTypePattern + oGenClass.sName.ToLower());
                for (int i = 0; i < oGenClass.aMembers.Count; ++i)
                {
                    GenMember oGenMember = oGenClass.aMembers[i];
                    if (null != oGenMember.aArrayTypes)
                        ProcessArrayTypesToXml(m_oDocxSerCPP, oGenClass, oGenMember, oGenClass.sName);
                }
                if (aAttributes.Count > 0)
                {
                    m_oDocxSerCPP.AppendFormat("void {0}::ReadAttributes(XmlUtils::CXmlLiteReader& oReader){{\r\n", oGenClass.sName);
                    m_oDocxSerCPP.AppendFormat("WritingElement_ReadAttributes_Start_No_NS( oReader )\r\n");
                    ProcessMembersFromXml(m_oDocxSerCPP, aAttributes, "wsName");
                    m_oDocxSerCPP.AppendFormat("WritingElement_ReadAttributes_End( oReader )\r\n");
                    m_oDocxSerCPP.AppendFormat("}}\r\n");
                }
            }
        }
        void ProcessProperty(StringBuilder sb, GenClass oGenClass)
        {
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMember oGenMember = oGenClass.aMembers[i];
                string sMemeberName = Utils.GetMemberElemName(oGenMember.sName);
                string sCPPType = GetCPPType(oGenMember);
                if (oGenMember.bIsAttribute)
                    sb.Append("//Attribute\r\n");
                else
                    sb.Append("//Member\r\n");
                if (oGenMember.bIsArray)
                    sb.AppendFormat("CAtlArray<{0}*> {1};\r\n", sCPPType, sMemeberName);
                else
                    sb.AppendFormat("{0}* {1};\r\n", sCPPType, sMemeberName);
            }
        }
        void ProcessConstructor(StringBuilder sb, GenClass oGenClass)
        {
            sb.AppendFormat("{0}::{0}()\r\n{{\r\n", oGenClass.sName);
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMember oGenMember = oGenClass.aMembers[i];
                string sMemeberName = Utils.GetMemberElemName(oGenMember.sName);
                if (!oGenMember.bIsArray)
                {
                    //todo подумать нужны ли в конструкторе DefAttribute
                    if (null != oGenMember.sDefAttribute)
                    {
                        string sTypeClass = "";
                        bool bIsEnum = false;
                        if (null != oGenMember.sType)
                        {
                            GenClass oTemp;
                            if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTemp))
                            {
                                sTypeClass = oTemp.sName;
                                bIsEnum = oTemp.bIsEnum;
                            }
                        }
                        string sCPPType = GetCPPType(oGenMember);
                        if (null != sCPPType)
                        {
                            sb.AppendFormat("/*{0} = new {1};\r\n", sMemeberName, sCPPType);
                            if (bIsEnum)
                            {
                                sb.AppendFormat("*{0} = {1};*/\r\n", sMemeberName, Utils.GetEnumElemName(sTypeClass, oGenMember.sDefAttribute));
                            }
                            else
                            {
                                //todo без строк
                                sb.AppendFormat("CString s_{0}(\"{1}\");\r\n", sMemeberName, oGenMember.sDefAttribute);
                                sb.AppendFormat("{{\r\n", sMemeberName, oGenMember.sDefAttribute);
                                ProcessCPPTypeFromXml(sb, "s_" + sMemeberName, "*" + sMemeberName, oGenMember.oSystemType);
                                sb.AppendFormat("}}*/\r\n", sMemeberName, oGenMember.sDefAttribute);
                            }
                        }
                    }
                    //else
                        sb.AppendFormat("{0} = NULL;\r\n", sMemeberName);
                }
            }
            sb.AppendFormat("}}\r\n");
        }
        void ProcessDestructor(StringBuilder sb, GenClass oGenClass)
        {
            sb.AppendFormat("{0}::~{0}()\r\n{{\r\n", oGenClass.sName);
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMember oGenMember = oGenClass.aMembers[i];
                string sMemeberName = Utils.GetMemberElemName(oGenMember.sName);
                if (oGenMember.bIsArray)
                {
                    sb.AppendFormat("for(int i = 0, length = {0}.GetCount(); i < length; ++i)\r\n", sMemeberName);
                    sb.AppendFormat("delete {0}[i];\r\n", sMemeberName);
                    sb.AppendFormat("{0}.RemoveAll();\r\n", sMemeberName);
                }
                else
                    sb.AppendFormat("if(NULL != {0})\r\ndelete {1};\r\n", sMemeberName, sMemeberName);
            }
            sb.AppendFormat("}}\r\n");
        }
        void ProcessFromXml(StringBuilder sb, GenClass oGenClass)
        {
            List<GenMember> aAttributes = new List<GenMember>();
            List<GenMember> aMembers = new List<GenMember>();
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMember oGenMember = oGenClass.aMembers[i];
                if (!oGenMember.bInternal)
                {
                    if (oGenMember.bIsAttribute)
                        aAttributes.Add(oGenMember);
                    else
                        aMembers.Add(oGenMember);
                }
            }
            if (aAttributes.Count > 0)
                sb.AppendFormat("ReadAttributes(oReader);\r\n");
            sb.AppendFormat("if ( oReader.IsEmptyNode() )\r\n");
            sb.AppendFormat("return;\r\n");
            if (aMembers.Count > 0)
            {
                sb.AppendFormat("int nParentDepth = oReader.GetDepth();\r\n");
                sb.AppendFormat("while( oReader.ReadNextSiblingNode( nParentDepth ) )\r\n{{\r\n");
                sb.AppendFormat("CString sName = XmlUtils::GetNameNoNS(oReader.GetName());\r\n");
                ProcessMembersFromXml(sb, aMembers, "sName");
                sb.AppendFormat("}}\r\n");
            }
            else
            {
                sb.AppendFormat("if(!oReader.IsEmptyNode())\r\n");
                sb.AppendFormat("oReader.ReadTillEnd();\r\n");
            }
        }
        void ProcessMembersFromXml(StringBuilder sb, List<GenMember> aMembers, string sCodeName)
        {
            int nCounter = 0;
            for (int i = 0; i < aMembers.Count; ++i)
            {
                GenMember oGenMember = aMembers[i];
                if (null != oGenMember.aArrayTypes)
                {
                    for (int j = 0; j < oGenMember.aArrayTypes.Count; ++j)
                    {
                        nCounter = ProcessMemberFromXml(sb, oGenMember.aArrayTypes[j], oGenMember, sCodeName, nCounter);
                    }
                }
                else
                {
                    nCounter = ProcessMemberFromXml(sb, oGenMember, null, sCodeName, nCounter);
                }
            }
        }
        int ProcessMemberFromXml(StringBuilder sb, GenMember oGenMember, GenMember oGenMemberContainer, string sCodeName, int nCounter)
        {
            if (0 != nCounter)
                sb.AppendFormat("else ");
            sb.AppendFormat("if(_T(\"{0}\") == {1})\r\n", oGenMember.sName, sCodeName);
            sb.AppendFormat("{{\r\n");
            if (oGenMember.bToDo || !CanProcessToDoString(oGenMember))
                sb.AppendFormat("//todo\r\n");
            sb.AppendFormat("{0}* pNewElem = new {0};\r\n", GetCPPType(oGenMember));
            if (null != oGenMember.oSystemType)
            {
                sb.AppendFormat("CString sVal = {0};\r\n", GetValueToDoFromXml(oGenMember));
                ProcessCPPTypeFromXml(sb, "sVal", "*pNewElem", oGenMember.oSystemType);
            }
            else if (null != oGenMember.sType)
            {
                GenClass oTemp;
                if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTemp))
                {
                    if (oTemp.bIsEnum)
                    {
                        sb.AppendFormat("{0} eVal;\r\n", oTemp.sName);
                        sb.AppendFormat("if({0}(CString({1}), eVal))\r\n", gc_sEnumFromXmlPrefix + oTemp.sName, GetValueToDoFromXml(oGenMember));
                        sb.AppendFormat("*pNewElem = eVal;\r\n", gc_sEnumFromXmlPrefix + oTemp.sName);
                    }
                    else
                        sb.AppendFormat("pNewElem->fromXML(oReader);\r\n");
                }
            }
            if (null != oGenMemberContainer)
            {
                sb.AppendFormat("{0}* eElemtype = new {0};\r\n", oGenMemberContainer.sArrayTypesEnumName);
                sb.AppendFormat("*eElemtype = {0};\r\n", Utils.GetEnumElemName(oGenMemberContainer.sArrayTypesEnumName, oGenMember.sName));
                sb.AppendFormat("{0}.Add(pNewElem);\r\n", Utils.GetMemberElemName(oGenMemberContainer.sName));
                sb.AppendFormat("{0}.Add(eElemtype);\r\n", Utils.GetMemberElemName(oGenMemberContainer.sArrayTypesElementName));
            }
            else
            {
                if (oGenMember.bIsArray)
                    sb.AppendFormat("{0}.Add(pNewElem);\r\n", Utils.GetMemberElemName(oGenMember.sName));
                else
                    sb.AppendFormat("{0} = pNewElem;\r\n", Utils.GetMemberElemName(oGenMember.sName));
            }
            sb.AppendFormat("}}\r\n");
            nCounter++;
            return nCounter;
        }
        bool CanProcessToDoString(GenMember oGenMember)
        {
            bool bRes = true;
            if (oGenMember.bToDoString)
            {
                bRes = false;
                if ("spPr" == oGenMember.sName || "txPr" == oGenMember.sName || "rich" == oGenMember.sName || "clrMapOvr" == oGenMember.sName)
                    bRes = true;
            }
            return bRes;
        }
        string GetValueToDoFromXml(GenMember oGenMember)
        {
            string sRes;
            if (oGenMember.bToDoString)
            {
                if ("spPr" == oGenMember.sName || "clrMapOvr" == oGenMember.sName)
                    sRes = "oReader.GetOuterXml()";
                else if ("txPr" == oGenMember.sName || "rich" == oGenMember.sName)
                    sRes = "oReader.GetInnerXml()";
                else
                    sRes = "oReader.GetOuterXml()";
            }
            else
            {
                if (oGenMember.bIsAttribute)
                    sRes = "oReader.GetText()";
                else
                    sRes = "oReader.GetText2()";
            }
            return sRes;
        }
        bool ProcessToXmlToDo(StringBuilder sb, GenMember oGenMember, string sElemName, string sPVal)
        {
            bool bRes = false;
            if (oGenMember.bToDoString)
            {
                bRes = true;
                if ("spPr" == oGenMember.sName || "clrMapOvr" == oGenMember.sName)
                    sb.AppendFormat("writer.WriteString(*{0});\r\n", sPVal);
                else if ("txPr" == oGenMember.sName || "rich" == oGenMember.sName)
                    sb.AppendFormat("writer.WriteString(_T(\"<{0}>\") + *{1} + _T(\"</{0}>\"));\r\n", sElemName, sPVal);
                else
                    sb.AppendFormat("writer.WriteString(*{0});\r\n", sPVal);
                
            }
            return bRes;
        }

        void ProcessToXml(StringBuilder sb, GenClass oGenClass)
        {
            List<GenMember> aAttributes = new List<GenMember>();
            List<GenMember> aMembers = new List<GenMember>();
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMember oGenMember = oGenClass.aMembers[i];
                if (!oGenMember.bInternal)
                {
                    if (oGenMember.bIsAttribute)
                        aAttributes.Add(oGenMember);
                    else
                        aMembers.Add(oGenMember);
                }
            }

            sb.AppendFormat("writer.WriteString(CString(_T(\"<\")));\r\n");
            sb.AppendFormat("writer.WriteString(sNodeName);\r\n");
            for (int i = 0; i < aAttributes.Count; ++i)
            {
                GenMember oGenMember = aAttributes[i];
                ProcessMemberToXml(sb, oGenClass, oGenMember, Utils.GetMemberElemName(oGenMember.sName), null);
            }
            if (oGenClass.bIsRoot)
                sb.AppendFormat("writer.WriteString(CString(_T(\"{0}\")));\r\n", gc_sNamespaceToXml);
            if (aMembers.Count > 0)
            {
                sb.AppendFormat("writer.WriteString(CString(_T(\">\")));\r\n");
                for (int i = 0; i < aMembers.Count; ++i)
                {
                    GenMember oGenMember = aMembers[i];
                    if (oGenMember.bIsArray)
                    {
                        sb.AppendFormat("for(int i = 0, length = {0}.GetCount(); i < length; ++i)\r\n", Utils.GetMemberElemName(oGenMember.sName));
                        sb.AppendFormat("{{\r\n");
                        if (null != oGenMember.aArrayTypes)
                        {
                            sb.AppendFormat("{0} eType = *{1}[i];\r\n", oGenMember.sArrayTypesEnumName, Utils.GetMemberElemName(oGenMember.sArrayTypesElementName));
                            sb.AppendFormat("toXML(writer, false, eType, {0}[i]);\r\n", Utils.GetMemberElemName(oGenMember.sName));
                        }
                        else
                        {
                            sb.AppendFormat("{0}* pElem = {1}[i];\r\n", GetCPPType(oGenMember), Utils.GetMemberElemName(oGenMember.sName));
                            ProcessMemberToXml(sb, oGenClass, oGenMember, "pElem", null);
                        }
                        sb.AppendFormat("}}\r\n");
                    }
                    else
                        ProcessMemberToXml(sb, oGenClass, oGenMember, Utils.GetMemberElemName(oGenMember.sName), null);
                }
                sb.AppendFormat("writer.WriteString(CString(_T(\"</\")));\r\n");
                sb.AppendFormat("writer.WriteString(sNodeName);\r\n");
                sb.AppendFormat("writer.WriteString(CString(_T(\">\")));\r\n");
            }
            else
                sb.AppendFormat("writer.WriteString(CString(_T(\"/>\")));\r\n");
        }
        void ProcessArrayTypesToXml(StringBuilder sb, GenClass oGenClass, GenMember oGenMember, string sClassName)
        {
            sb.AppendFormat("void {0}::toXML(XmlUtils::CStringWriter& writer, bool bIsAttribute, {1} eType, void* pVal) const{{\r\n", sClassName, oGenMember.sArrayTypesEnumName);
            sb.AppendFormat("switch(eType)\r\n{{\r\n");
            for (int i = 0; i < oGenMember.aArrayTypes.Count; ++i)
            {
                GenMember oTempMember = oGenMember.aArrayTypes[i];
                sb.AppendFormat("case {0}:\r\n{{\r\n", Utils.GetEnumElemName(oGenMember.sArrayTypesEnumName, oTempMember.sName));
                sb.AppendFormat("{0}* pTypeVal = static_cast<{0}*>(pVal);\r\n", GetCPPType(oTempMember));
                ProcessMemberToXml(sb, oGenClass, oTempMember, "pTypeVal", "bIsAttribute");
                sb.AppendFormat("}}\r\nbreak;\r\n");
            }
            sb.AppendFormat("}}\r\n");
            sb.AppendFormat("}}\r\n");
        }
        void ProcessMemberToXml(StringBuilder sb, GenClass oGenClass, GenMember oGenMember, string sPVal, string sIsAttribute)
        {
            string sElemName = oGenMember.sName;
            bool bIsAttribute = oGenMember.bIsAttribute;
            string sMemberNamespace = null;
            GenClass oGenClassMember = null;
            if (null != oGenMember.sNamespace)
                sMemberNamespace = oGenMember.sNamespace;
            else if(null != oGenMember.sType && m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oGenClassMember))
                sMemberNamespace = oGenClassMember.sNamespace;
            else
                sMemberNamespace = oGenClass.sNamespace;
            if (null != sMemberNamespace && oGenMember.bQualified)
            {
                string sShortNamespace;
                if (Utils.m_mapNamespaceToPrefix.TryGetValue(sMemberNamespace, out sShortNamespace))
                    sElemName = sShortNamespace + sElemName;
            }

            sb.AppendFormat("if(NULL != {0})\r\n", sPVal);
            sb.AppendFormat("{{\r\n");
            if (oGenMember.bToDo || !CanProcessToDoString(oGenMember))
                sb.AppendFormat("//todo\r\n");
            if (!ProcessToXmlToDo(sb, oGenMember, sElemName, sPVal))
            {
                if (null != oGenMember.oSystemType)
                {
                    sb.AppendFormat("CString sXml;\r\n");
                    sb.AppendFormat("CString sVal;\r\n");
                    ProcessCPPTypeToXml(sb, "*" + sPVal, "sVal", oGenMember.oSystemType);
                    if (null != sIsAttribute)
                        sb.AppendFormat("if(true == {0})\r\n", sIsAttribute);
                    if (null != sIsAttribute || bIsAttribute)
                        sb.AppendFormat("sXml.Format(_T(\" {0}=\\\"%s\\\"\"), sVal);\r\n", sElemName);
                    if (null != sIsAttribute)
                        sb.AppendFormat("else\r\n", sIsAttribute);
                    if (null != sIsAttribute || !bIsAttribute)
                    {
                        if (typeof(string) == oGenMember.oSystemType)
                        {
                            sb.AppendFormat("if(-1 != sVal.Find(' ') || -1 != sVal.Find('\\n'))\r\n");
                            sb.AppendFormat("sXml.Format(_T(\"<{0} xml:space=\\\"preserve\\\">%s</{0}>\"), sVal);\r\n", sElemName);
                            sb.AppendFormat("else\r\n");
                            sb.AppendFormat("sXml.Format(_T(\"<{0}>%s</{0}>\"), sVal);\r\n", sElemName);
                        }
                        else
                            sb.AppendFormat("sXml.Format(_T(\"<{0}>%s</{0}>\"), sVal);\r\n", sElemName);
                    }
                    sb.AppendFormat("writer.WriteString(sXml);\r\n");
                }
                else if (null != oGenMember.sType)
                {
                    if (null != oGenClassMember || m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oGenClassMember))
                    {
                        if (oGenClassMember.bIsEnum)
                        {
                            sb.AppendFormat("CString sEnumVal;\r\n");
                            sb.AppendFormat("if({0}(*{1}, sEnumVal))\r\n", gc_sEnumToXmlPrefix + oGenClassMember.sName, sPVal);
                            sb.AppendFormat("{{\r\n");
                            sb.AppendFormat("CString sVal;\r\n");
                            if (null != sIsAttribute)
                                sb.AppendFormat("if(true == {0})\r\n", sIsAttribute);
                            if (null != sIsAttribute || bIsAttribute)
                                sb.AppendFormat("sVal.Format(_T(\" {0}=\\\"%s\\\"\"), sEnumVal);\r\n", sElemName);
                            if (null != sIsAttribute)
                                sb.AppendFormat("else\r\n", sIsAttribute);
                            if (null != sIsAttribute || !bIsAttribute)
                                sb.AppendFormat("sVal.Format(_T(\"<{0}>%s</{0}>\"), sEnumVal);\r\n", sElemName);
                            sb.AppendFormat("writer.WriteString(sVal);\r\n");
                            sb.AppendFormat("}}\r\n");
                        }
                        else
                        {
                            if (null != sIsAttribute)
                                sb.AppendFormat("if(false == {0})\r\n", sIsAttribute);
                            if (null != sIsAttribute || !bIsAttribute)
                                sb.AppendFormat("{0}->toXML(CString(_T(\"{1}\")), writer);\r\n", sPVal, sElemName);
                        }
                    }
                }
            }
            sb.AppendFormat("}}\r\n");
        }
        void ProcessCPPTypeFromXml(StringBuilder sb, string sCodeVal, string sCodeMember, Type oType)
        {
            //todo
            switch (Type.GetTypeCode(oType))
            {
                case TypeCode.Boolean:
                    sb.AppendFormat("CString sValLower = {0}.MakeLower();\r\n", sCodeVal);
                    sb.AppendFormat("if(_T(\"false\") == sValLower || _T(\"0\") == sValLower || _T(\"off\") == sValLower)\r\n");
                    sb.AppendFormat("{0} = false;\r\n", sCodeMember);
                    sb.AppendFormat("else\r\n");
                    sb.AppendFormat("{0} = true;\r\n", sCodeMember);
                    break;
                case TypeCode.Byte:
                case TypeCode.SByte:
                case TypeCode.Int16:
                case TypeCode.Int32:
                case TypeCode.Int64:
                case TypeCode.UInt16:
                case TypeCode.UInt32:
                case TypeCode.UInt64:
                    sb.AppendFormat("{0} = _ttoi({1});\r\n", sCodeMember, sCodeVal);
                    break;
                case TypeCode.Single:
                case TypeCode.Double:
                    sb.AppendFormat("{0} = _tstof({1});\r\n", sCodeMember, sCodeVal);
                    break;
                default: sb.AppendFormat("{0} = {1};\r\n", sCodeMember, sCodeVal); ; break;
            }
        }
        void ProcessCPPTypeToXml(StringBuilder sb, string sCodeVal, string sCodeMember, Type oType)
        {
            //todo
            switch (Type.GetTypeCode(oType))
            {
                case TypeCode.Boolean://todo на запись "false", "off"
                    sb.AppendFormat("if({0})\r\n", sCodeVal);
                    sb.AppendFormat("{0} = _T(\"1\");\r\n", sCodeMember);
                    sb.AppendFormat("else\r\n");
                    sb.AppendFormat("{0} = _T(\"0\");\r\n", sCodeMember);
                    break;
                case TypeCode.Byte:
                case TypeCode.SByte:
                case TypeCode.Int16:
                case TypeCode.Int32:
                case TypeCode.Int64:
                case TypeCode.UInt16:
                case TypeCode.UInt32:
                case TypeCode.UInt64:
                    sb.AppendFormat("{0}.Format(_T(\"%d\"), {1});\r\n", sCodeMember, sCodeVal);
                    break;
                case TypeCode.Single:
                case TypeCode.Double:
                    sb.AppendFormat("{0} = SpreadsheetCommon::WriteDouble({1});\r\n", sCodeMember, sCodeVal);
                    break;
                default: sb.AppendFormat("{0} = XmlUtils::EncodeXmlString({1});\r\n", sCodeMember, sCodeVal); ; break;
            }
        }
#endregion
#region From/to bin
        void ProcessFromToBinaryH(StringBuilder sb, List<GenClass> aEnums, List<GenClass> aClasses)
        {
            sb.AppendFormat("namespace BinXlsxRW {{\r\n");
            sb.AppendFormat("class {0} : public Binary_CommonReader<{0}>\r\n{{\r\n", Utils.gc_sBinaryChartReader);
            sb.AppendFormat("LPSAFEARRAY m_pArray;\r\n");
            sb.AppendFormat("PPTXFile::IAVSOfficeDrawingConverter* m_pOfficeDrawingConverter;\r\n");
            sb.AppendFormat("public:\r\n{0}(Streams::CBufferedStream& oBufferedStream, LPSAFEARRAY pArray, PPTXFile::IAVSOfficeDrawingConverter* pOfficeDrawingConverter);\r\n", Utils.gc_sBinaryChartReader);
            for (int i = 0; i < aClasses.Count; ++i)
            {
                GenClass oGenClass = aClasses[i];
                if (oGenClass.bIsRoot)
                    sb.AppendFormat("public: int Read{0}(long length, {0}* poResult);\r\n", aClasses[i].sName);
                sb.AppendFormat("private: int Read{0}(BYTE type, long length, void* poResult);\r\n", aClasses[i].sName);
            }
            sb.AppendFormat("}};\r\n");

            sb.AppendFormat("class {0}\r\n{{\r\n", Utils.gc_sBinaryChartWriter);
            sb.AppendFormat("BinaryCommonWriter m_oBcw;\r\n");
            sb.AppendFormat("PPTXFile::IAVSOfficeDrawingConverter* m_pOfficeDrawingConverter;\r\n");
            sb.AppendFormat("public:\r\n{0}(Streams::CBufferedStream &oCBufferedStream, PPTXFile::IAVSOfficeDrawingConverter* pOfficeDrawingConverter);\r\n", Utils.gc_sBinaryChartWriter);
            for (int i = 0; i < aClasses.Count; ++i)
            {
                GenClass oGenClass = aClasses[i];
                sb.AppendFormat("public: void Write{0}({0}& oVal);\r\n", aClasses[i].sName);
                for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                {
                    GenMember oGenMember = oGenClass.aMembers[j];
                    if (!oGenMember.bInternal)
                    {
                        if (null != oGenMember.aArrayTypes)
                            sb.AppendFormat("private: void toBin({0} eType, void* pVal);\r\n", oGenMember.sArrayTypesEnumName);
                    }
                }
            }
            sb.AppendFormat("}};\r\n");

            sb.AppendFormat("}}");
        }
        void ProcessFromToBinaryCPP(StringBuilder sb, List<GenClass> aEnums, List<GenClass> aClasses)
        {
            sb.AppendFormat("namespace BinXlsxRW{{\r\n");
            for (int i = 0; i < aClasses.Count; ++i)
            {
                GenClass oGenClass = aClasses[i];
                int nCounter = 0;
                for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                {
                    GenMember oGenMember = oGenClass.aMembers[j];
                    if (!oGenMember.bInternal)
                    {
                        sb.AppendFormat("BYTE {0} = {1};\r\n", Utils.GetSerEnumElemName(oGenClass.sName, oGenMember.sName), nCounter++);
                        if (null != oGenMember.aArrayTypes)
                        {
                            for (int k = 0; k < oGenMember.aArrayTypes.Count; ++k)
                            {
                                sb.AppendFormat("BYTE {0} = {1};\r\n", Utils.GetSerEnumElemName(oGenClass.sName, oGenMember.aArrayTypes[k].sName), nCounter++);
                            }
                        }
                    }
                }
                sb.AppendFormat("\r\n");
            }
            sb.AppendFormat("{0}::{0}(Streams::CBufferedStream& oBufferedStream, LPSAFEARRAY pArray, PPTXFile::IAVSOfficeDrawingConverter* pOfficeDrawingConverter):Binary_CommonReader(oBufferedStream),m_pArray(pArray),m_pOfficeDrawingConverter(pOfficeDrawingConverter)\r\n{{}}\r\n", Utils.gc_sBinaryChartReader);
            for (int i = 0; i < aClasses.Count; ++i)
                ProcessFromBin(sb, aClasses[i]);

            sb.AppendFormat("{0}::{0}(Streams::CBufferedStream &oBufferedStream, PPTXFile::IAVSOfficeDrawingConverter* pOfficeDrawingConverter):m_oBcw(oBufferedStream),m_pOfficeDrawingConverter(pOfficeDrawingConverter)\r\n{{}}\r\n", Utils.gc_sBinaryChartWriter);
            for (int i = 0; i < aClasses.Count; ++i)
                ProcessToBin(sb, aClasses[i]);
            sb.AppendFormat("}}");
        }
        void ProcessToBin(StringBuilder sb, GenClass oGenClass)
        {
            sb.AppendFormat("void {0}::Write{1}({1}& oVal)\r\n{{\r\n", Utils.gc_sBinaryChartWriter, oGenClass.sName);
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMember oGenMember = oGenClass.aMembers[i];
                if (!oGenMember.bInternal)
                {
                    if (oGenMember.bIsArray)
                    {
                        sb.AppendFormat("for(int i = 0, length = oVal.{0}.GetCount(); i < length; ++i)\r\n", Utils.GetMemberElemName(oGenMember.sName));
                        sb.AppendFormat("{{\r\n");
                        if (null != oGenMember.aArrayTypes)
                        {
                            sb.AppendFormat("{0} eType = *oVal.{1}[i];\r\n", oGenMember.sArrayTypesEnumName, Utils.GetMemberElemName(oGenMember.sArrayTypesElementName));
                            sb.AppendFormat("toBin(eType, oVal.{0}[i]);\r\n", Utils.GetMemberElemName(oGenMember.sName));
                        }
                        else
                        {
                            sb.AppendFormat("{0}* pVal = oVal.{1}[i];\r\n", GetCPPType(oGenMember), Utils.GetMemberElemName(oGenMember.sName));
                            ProcessMemberToBin(sb, oGenClass, oGenMember, "", "pVal");
                        }
                        sb.AppendFormat("}}\r\n");
                    }
                    else
                    {
                        ProcessMemberToBin(sb, oGenClass, oGenMember, "oVal.", Utils.GetMemberElemName(oGenMember.sName));
                    }
                }
            }
            sb.AppendFormat("}}\r\n");
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMember oGenMember = oGenClass.aMembers[i];
                if (!oGenMember.bInternal)
                {
                    if (null != oGenMember.aArrayTypes)
                        ProcessArrayTypesToBin(sb, oGenClass, oGenMember);
                }
            }
        }
        void ProcessArrayTypesToBin(StringBuilder sb, GenClass oGenClass, GenMember oGenMember)
        {
            sb.AppendFormat("void {0}::toBin({1} eType, void* pVal){{\r\n", Utils.gc_sBinaryChartWriter, oGenMember.sArrayTypesEnumName);
            sb.AppendFormat("switch(eType)\r\n{{\r\n");
            for (int i = 0; i < oGenMember.aArrayTypes.Count; ++i)
            {
                GenMember oTempMember = oGenMember.aArrayTypes[i];
                sb.AppendFormat("case {0}:\r\n{{\r\n", Utils.GetEnumElemName(oGenMember.sArrayTypesEnumName, oTempMember.sName));
                sb.AppendFormat("{0}* pTypeVal = static_cast<{0}*>(pVal);\r\n", GetCPPType(oTempMember));
                ProcessMemberToBin(sb, oGenClass, oTempMember, "", "pTypeVal");
                sb.AppendFormat("}}\r\nbreak;\r\n");
            }
            sb.AppendFormat("}}\r\n");
            sb.AppendFormat("}}\r\n");
        }
        void ProcessMemberToBin(StringBuilder sb, GenClass oGenClass, GenMember oGenMember, string pValPrefix, string pVal)
        {
            sb.AppendFormat("if(NULL != {0})\r\n", pValPrefix + pVal);
            sb.AppendFormat("{{\r\n");

            sb.AppendFormat("int nCurPos = m_oBcw.WriteItemStart({0});\r\n", Utils.GetSerEnumElemName(oGenClass.sName, oGenMember.sName));
            if (oGenMember.bToDo || !CanProcessToDoString(oGenMember))
                sb.AppendFormat("//todo\r\n");
            if(!ProcessMemberToBinTodo(sb, oGenMember, pValPrefix, pVal))
            {
                if (null != oGenMember.sType)
                {
                    GenClass oTemp;
                    if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTemp))
                    {
                        if (oTemp.bIsEnum)
                        {
                            sb.AppendFormat("int nVal = (int)(*{0});\r\n", pValPrefix + pVal);
                            ProcessCPPTypeToBin(sb, "&nVal", typeof(byte));
                        }
                        else
                            sb.AppendFormat("Write{0}(*{1});\r\n", oGenMember.sType, pValPrefix + pVal);
                    }
                }
                else if (null != oGenMember.oSystemType)
                {
                    ProcessCPPTypeToBin(sb, pValPrefix + pVal, oGenMember.oSystemType);
                }
            }
            sb.AppendFormat("m_oBcw.WriteItemEnd(nCurPos);\r\n");
            sb.AppendFormat("}}\r\n");
        }
        void ProcessCPPTypeToBin(StringBuilder sb, string sCodeMember, Type oType)
        {
            //todo
            switch (Type.GetTypeCode(oType))
            {
                case TypeCode.Boolean:
                    sb.AppendFormat("m_oBcw.m_oStream.WriteBool(*{0});\r\n", sCodeMember);
                    break;
                case TypeCode.Byte:
                case TypeCode.SByte:
                    sb.AppendFormat("m_oBcw.m_oStream.WriteByte(*{0});\r\n", sCodeMember);
                    break;
                case TypeCode.Int16:
                case TypeCode.Int32:
                case TypeCode.Int64:
                case TypeCode.UInt16:
                case TypeCode.UInt32:
                case TypeCode.UInt64:
                    sb.AppendFormat("m_oBcw.m_oStream.WriteLong(*{0});\r\n", sCodeMember);
                    break;
                case TypeCode.Single:
                case TypeCode.Double:
                    sb.AppendFormat("m_oBcw.m_oStream.WriteDouble(*{0});\r\n", sCodeMember);
                    break;
                default: sb.AppendFormat("m_oBcw.m_oStream.WriteString3(*{0});\r\n", sCodeMember); break;
            }
        }
        bool ProcessMemberToBinTodo(StringBuilder sb, GenMember oGenMember, string pValPrefix, string pVal)
        {
            bool bRes = false;
            if ("spPr" == oGenMember.sName || "txPr" == oGenMember.sName || "rich" == oGenMember.sName || "clrMapOvr" == oGenMember.sName)
            {
                bRes = true;
                sb.AppendFormat("LPSAFEARRAY pBinaryObj = NULL;\r\n");

                if ("spPr" == oGenMember.sName)
                {
                    sb.AppendFormat("BSTR bstrXml = (*{0}).AllocSysString();\r\n", pValPrefix + pVal);
                    sb.AppendFormat("HRESULT hRes = m_pOfficeDrawingConverter->GetRecordBinary(XMLWRITER_RECORD_TYPE_SPPR, bstrXml, &pBinaryObj);\r\n");
                }
                else if ("txPr" == oGenMember.sName || "rich" == oGenMember.sName)
                {
                    sb.AppendFormat("BSTR bstrXml = (_T(\"<c:rich xmlns:c=\\\"http://schemas.openxmlformats.org/drawingml/2006/chart\\\" xmlns:a=\\\"http://schemas.openxmlformats.org/drawingml/2006/main\\\" xmlns:r=\\\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\\\">\") + *{0} + _T(\"</c:rich>\")).AllocSysString();\r\n", pValPrefix + pVal);
                    sb.AppendFormat("HRESULT hRes = m_pOfficeDrawingConverter->GetTxBodyBinary(bstrXml, &pBinaryObj);\r\n");
                }
                else if ("clrMapOvr" == oGenMember.sName)
                {
                    sb.AppendFormat("BSTR bstrXml = (*{0}).AllocSysString();\r\n", pValPrefix + pVal);
                    sb.AppendFormat("HRESULT hRes = m_pOfficeDrawingConverter->GetRecordBinary(XMLWRITER_RECORD_TYPE_CLRMAPOVR, bstrXml, &pBinaryObj);\r\n");
                }
                sb.AppendFormat("SysFreeString(bstrXml);\r\n");
                sb.AppendFormat("if(S_OK == hRes && NULL != pBinaryObj && pBinaryObj->rgsabound[0].cElements > 0)\r\n");
                sb.AppendFormat("m_oBcw.m_oStream.WritePointer((BYTE*)pBinaryObj->pvData, pBinaryObj->rgsabound[0].cElements);\r\n");
                sb.AppendFormat("RELEASEARRAY(pBinaryObj);\r\n");
            }
            return bRes;
        }
        void ProcessFromBin(StringBuilder sb, GenClass oGenClass)
        {
            if (oGenClass.bIsRoot)
            {
                sb.AppendFormat("int {0}::Read{1}(long length, {1}* poResult)\r\n{{\r\n", Utils.gc_sBinaryChartReader, oGenClass.sName);
                sb.AppendFormat("return Read1(length, &{0}::Read{1}, this, poResult);\r\n", Utils.gc_sBinaryChartReader, oGenClass.sName);
                sb.AppendFormat("}}\r\n");
            }
            sb.AppendFormat("int {0}::Read{1}(BYTE type, long length, void* poResult)\r\n{{\r\n", Utils.gc_sBinaryChartReader, oGenClass.sName);
            sb.AppendFormat("int res = c_oSerConstants::ReadOk;\r\n");
            sb.AppendFormat("{0}* poVal = static_cast<{0}*>(poResult);\r\n", oGenClass.sName);
            int nCounter = 0;
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMember oGenMember = oGenClass.aMembers[i];
                if (!oGenMember.bInternal)
                {
                    if (null != oGenMember.aArrayTypes)
                    {
                        for (int j = 0; j < oGenMember.aArrayTypes.Count; ++j)
                        {
                            nCounter = ProcessMemberFromBin(sb, oGenClass, oGenMember.aArrayTypes[j], oGenMember, nCounter);
                        }
                    }
                    else
                    {
                        nCounter = ProcessMemberFromBin(sb, oGenClass, oGenMember, null, nCounter);
                    }
                }
            }
            sb.AppendFormat("else\r\nres = c_oSerConstants::ReadUnknown;\r\nreturn res;\r\n");
            sb.AppendFormat("}}\r\n");
        }
        int ProcessMemberFromBin(StringBuilder sb, GenClass oGenClass, GenMember oGenMember, GenMember oGenMemberContainer, int nCounter)
        {
            if (0 != nCounter)
                sb.AppendFormat("else ");
            sb.AppendFormat("if({0} == type)\r\n{{\r\n", Utils.GetSerEnumElemName(oGenClass.sName, oGenMember.sName));
            sb.AppendFormat("{0}* pNewElem = new {0};\r\n", GetCPPType(oGenMember));
            if (oGenMember.bToDo || !CanProcessToDoString(oGenMember))
                sb.AppendFormat("//todo\r\n");
            if(!ProcessMemberFromBinTodo(sb, oGenMember, "pNewElem"))
            {
                if (null != oGenMember.sType)
                {
                    GenClass oTempClass;
                    if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTempClass))
                    {
                        if (oTempClass.bIsEnum)
                        {
                            sb.AppendFormat("*pNewElem = ({0})", oTempClass.sName);
                            ProcessCPPTypeFromBin(sb, typeof(byte));
                            sb.AppendFormat(";\r\n");
                        }
                        else
                            sb.AppendFormat("res = Read1(length, &{0}::Read{1}, this, pNewElem);\r\n", Utils.gc_sBinaryChartReader, oGenMember.sType);
                    }
                }
                else if (null != oGenMember.oSystemType)
                {
                    sb.AppendFormat("*pNewElem = ");
                    ProcessCPPTypeFromBin(sb, oGenMember.oSystemType);
                }
                else
                    sb.AppendFormat("res = c_oSerConstants::ReadUnknown;\r\n");
            }
            if (null != oGenMemberContainer)
            {
                sb.AppendFormat("{0}* eElemtype = new {0};\r\n", oGenMemberContainer.sArrayTypesEnumName);
                sb.AppendFormat("*eElemtype = {0};\r\n", Utils.GetEnumElemName(oGenMemberContainer.sArrayTypesEnumName, oGenMember.sName));
                sb.AppendFormat("poVal->{0}.Add(eElemtype);\r\n", Utils.GetMemberElemName(oGenMemberContainer.sArrayTypesElementName));
                sb.AppendFormat("poVal->{0}.Add(pNewElem);\r\n", Utils.GetMemberElemName(oGenMemberContainer.sName));
            }
            else
            {
                if (oGenMember.bIsArray)
                    sb.AppendFormat("poVal->{0}.Add(pNewElem);\r\n", Utils.GetMemberElemName(oGenMember.sName));
                else
                    sb.AppendFormat("poVal->{0} = pNewElem;\r\n", Utils.GetMemberElemName(oGenMember.sName));
            }
            sb.AppendFormat("}}\r\n");
            nCounter++;
            return nCounter;
        }
        void ProcessCPPTypeFromBin(StringBuilder sb, Type oType)
        {
            //todo
            switch (Type.GetTypeCode(oType))
            {
                case TypeCode.Boolean:
                    sb.AppendFormat("m_oBufferedStream.ReadBool();\r\n");
                    break;
                case TypeCode.Byte:
                case TypeCode.SByte:
                    sb.AppendFormat("m_oBufferedStream.ReadByte();\r\n");
                    break;
                case TypeCode.Int16:
                case TypeCode.Int32:
                case TypeCode.Int64:
                case TypeCode.UInt16:
                case TypeCode.UInt32:
                case TypeCode.UInt64:
                    sb.AppendFormat("m_oBufferedStream.ReadLong();\r\n");
                    break;
                case TypeCode.Single:
                case TypeCode.Double:
                    sb.AppendFormat("m_oBufferedStream.ReadDouble();\r\n");
                    break;
                default: sb.AppendFormat("m_oBufferedStream.ReadString2(length);\r\n"); break;
            }
        }
        bool ProcessMemberFromBinTodo(StringBuilder sb, GenMember oGenMember, string sElemName)
        {
            bool bRes = true;
            if ("spPr" == oGenMember.sName || "txPr" == oGenMember.sName || "rich" == oGenMember.sName || "clrMapOvr" == oGenMember.sName)
            {
                sb.AppendFormat("if(length > 0)\r\n");
                sb.AppendFormat("{{\r\n");
                sb.AppendFormat("BSTR bstrXml = NULL;\r\n");
                if ("spPr" == oGenMember.sName)
                    sb.AppendFormat("HRESULT hRes = m_pOfficeDrawingConverter->GetRecordXml(m_pArray, m_oBufferedStream.GetPosition(), length, XMLWRITER_RECORD_TYPE_SPPR, XMLWRITER_DOC_TYPE_CHART, &bstrXml);\r\n");
                else if ("txPr" == oGenMember.sName || "rich" == oGenMember.sName)
                    sb.AppendFormat("HRESULT hRes = m_pOfficeDrawingConverter->GetTxBodyXml(m_pArray, m_oBufferedStream.GetPosition(), length, &bstrXml);\r\n");
                else if ("clrMapOvr" == oGenMember.sName)
                    sb.AppendFormat("HRESULT hRes = m_pOfficeDrawingConverter->GetRecordXml(m_pArray, m_oBufferedStream.GetPosition(), length, XMLWRITER_RECORD_TYPE_CLRMAPOVR, XMLWRITER_DOC_TYPE_CHART, &bstrXml);\r\n");
                sb.AppendFormat("if (S_OK == hRes && NULL != bstrXml)\r\n");
                sb.AppendFormat("{{\r\n");
                sb.AppendFormat("*{0} = bstrXml;\r\n", sElemName);
                sb.AppendFormat("SysFreeString(bstrXml);\r\n");
                sb.AppendFormat("}}\r\n");
                sb.AppendFormat("m_oBufferedStream.Skip(length);\r\n");
                sb.AppendFormat("}}\r\n");
            }
            else
                bRes = false;
            return bRes;
        }
#endregion
    }
}