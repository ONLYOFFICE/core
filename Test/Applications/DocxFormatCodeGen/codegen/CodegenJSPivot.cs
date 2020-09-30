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
using System.IO;
using System.Text.RegularExpressions;

namespace codegen
{
    class CodegenJSXml
    {
        Dictionary<string, GenClassPivot> m_mapProcessedClasses = new Dictionary<string, GenClassPivot>();
        string gc_sEnumFromXmlPrefix = "FromXml_";
        string gc_sEnumToXmlPrefix = "ToXml_";
        string gc_sNamespaceToXml = " xmlns=\\\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\\\" xmlns:r=\\\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\\\"";
        string[] gc_numeric = { "Null", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine" };

        public void Process(string sOutputDir, List<GenClassPivot> aGenClasses)
        {
            StringBuilder oJsSer = new StringBuilder();
            List<GenClassPivot> aEnums = new List<GenClassPivot>();
            List<GenClassPivot> aClasses = new List<GenClassPivot>();
            for (int i = 0; i < aGenClasses.Count; ++i)
            {
                GenClassPivot oGenClass = aGenClasses[i];
                if (oGenClass.bIsEnum)
                    aEnums.Add(oGenClass);
                else
                    aClasses.Add(oGenClass);
                m_mapProcessedClasses[oGenClass.sName] = oGenClass;
            }
            string sFileJs = "PivotTables.js";
            //string sFileJs = "Workbook.js";
            oJsSer.AppendFormat("\"use strict\";\r\n");
            oJsSer.AppendFormat(Utils.gc_sFilePrefix);
            oJsSer.AppendFormat("function getBoolFromXml(val){{return \"0\"!==val && \"false\"!==val && \"off\"!==val;}}\r\n");

            //enums
            ProcessEnums(oJsSer, aEnums);
            //FromXml
            for (int i = 0; i < aClasses.Count; ++i)
                ProcessClass(oJsSer, aClasses[i]);
            File.WriteAllText(Path.Combine(sOutputDir, sFileJs), oJsSer.ToString());
        }
        public void ProcessEnums(StringBuilder sb, List<GenClassPivot> aGenClasses)
        {
            for (int i = 0; i < aGenClasses.Count; ++i)
            {
                GenClassPivot oGenClass = aGenClasses[i];
                if (oGenClass.aMembers.Count > 0)
                {
                    sb.AppendFormat("var {0} = {{\r\n", GetEnumClassName(oGenClass.sName));
                    for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                    {
                        GenMemberPivot oGenMember = oGenClass.aMembers[j];
                        sb.AppendFormat("{0}: {1}", GetEnumElemName(oGenMember.sName), j);
                        if (j + 1 < oGenClass.aMembers.Count)
                        {
                            sb.AppendFormat(",");
                        }
                        sb.AppendFormat("\r\n");
                    }
                    sb.AppendFormat("}};\r\n");
                }
            }
            sb.AppendFormat("var prot;\r\n");
            for (int i = 0; i < aGenClasses.Count; ++i)
            {
                GenClassPivot oGenClass = aGenClasses[i];
                if (oGenClass.aMembers.Count > 0)
                {
                    sb.AppendFormat("window['AscCommonExcel']['{0}'] = window['AscCommonExcel'].{0} = {0};\r\n", GetEnumClassName(oGenClass.sName));
                    sb.AppendFormat("prot = {0};\r\n", GetEnumClassName(oGenClass.sName));
                    for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                    {
                        GenMemberPivot oGenMember = oGenClass.aMembers[j];
                        sb.AppendFormat("prot['{0}'] = prot.{0};\r\n", GetEnumElemName(oGenMember.sName));
                    }
                    sb.AppendFormat("\r\n");
                }
            }
            for (int i = 0; i < aGenClasses.Count; ++i)
            {
                GenClassPivot oGenClass = aGenClasses[i];
                sb.AppendFormat("function {0}{1}(val)\r\n{{\r\n", gc_sEnumFromXmlPrefix, oGenClass.sName);
                sb.AppendFormat("var res = -1;\r\n");
                for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                {
                    GenMemberPivot member = oGenClass.aMembers[j];
                    if (0 == j)
                        sb.AppendFormat("if(\"{0}\"===val)\r\nres={1};\r\n", member.sName, GetEnumElem(oGenClass.sName, member.sName));
                    else
                        sb.AppendFormat("else if(\"{0}\"===val)\r\nres={1};\r\n", member.sName, GetEnumElem(oGenClass.sName, member.sName));
                }
                sb.AppendFormat("return res;\r\n}}\r\n");
                sb.AppendFormat("function {0}{1}(val)\r\n{{\r\n", gc_sEnumToXmlPrefix, oGenClass.sName);
                sb.AppendFormat("var res = \"\";\r\n");
                for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                {
                    GenMemberPivot member = oGenClass.aMembers[j];
                    if (0 == j)
                        sb.AppendFormat("if({0}===val)\r\nres=\"{1}\";\r\n", GetEnumElem(oGenClass.sName, member.sName), member.sName);
                    else
                        sb.AppendFormat("else if({0}===val)\r\nres=\"{1}\";\r\n", GetEnumElem(oGenClass.sName, member.sName), member.sName);
                }
                sb.AppendFormat("return res;\r\n}}\r\n");
                sb.AppendFormat("\r\n");

            }
        }
        public void ProcessClass(StringBuilder sb, GenClassPivot oGenClass)
        {
            List<GenMemberPivot> aAttributes = new List<GenMemberPivot>();
            List<GenMemberPivot> aMembers = new List<GenMemberPivot>();
            bool bNeedTextNode = false;
            bool bNeedDoubleArray = false;
            InfoFromMember(oGenClass, ref aAttributes, ref aMembers, ref bNeedTextNode, ref bNeedDoubleArray);

            sb.AppendFormat("function {0}(){{\r\n", oGenClass.sName);
            ProcessProperty(sb, aAttributes, aMembers, bNeedTextNode, bNeedDoubleArray);
            sb.AppendFormat("}}\r\n");

            if (aAttributes.Count > 0)
            {
                sb.AppendFormat("{0}.prototype.readAttributes = function(attr, uq) {{\r\n", oGenClass.sName);
                ProcessAttributesFromXml(sb, oGenClass, aAttributes);
                sb.AppendFormat("}};\r\n");
            }
            if (aMembers.Count > 0)
            {
                sb.AppendFormat("{0}.prototype.onStartNode = function(elem, attr, uq) {{\r\n", oGenClass.sName);
                sb.AppendFormat("var newContext = this;\r\n");
                int nCounter = 0;
                if (oGenClass.isRoot() && ProcessRootFromXml(sb, oGenClass, "elem"))
                {
                    nCounter++;
                }
                ProcessOnStartNodeFromXml(sb, oGenClass, aMembers, nCounter, "elem");
                sb.AppendFormat("else {{\r\n");
                sb.AppendFormat("newContext = null;\r\n");
                sb.AppendFormat("}}\r\n");
                sb.AppendFormat("return newContext;\r\n");
                sb.AppendFormat("}};\r\n");

                if (bNeedTextNode)
                {
                    sb.AppendFormat("{0}.prototype.onTextNode = function(text, uq) {{\r\n", oGenClass.sName);
                    ProcessOnTextNodeFromXml(sb, oGenClass, aMembers);
                    sb.AppendFormat("}};\r\n");
                }
                if (bNeedDoubleArray)
                {
                    sb.AppendFormat("{0}.prototype.onEndNode = function(prevContext, elem) {{\r\n", oGenClass.sName);
                    ProcessOnEndNodeFromXml(sb, oGenClass, aMembers);
                    sb.AppendFormat("}};\r\n");
                }
            }
            if (aAttributes.Count > 0 || aMembers.Count > 0)
            {
                if (oGenClass.isRoot())
                {
                    sb.AppendFormat("{0}.prototype.toXml = function(writer) {{\r\n", oGenClass.sName);
                }
                else
                {
                    sb.AppendFormat("{0}.prototype.toXml = function(writer, name) {{\r\n", oGenClass.sName);
                }
                ProcessToXml(sb, oGenClass);
                sb.AppendFormat("}};\r\n");
            }
        }
        public void InfoFromMember(GenClassPivot oGenClass, ref List<GenMemberPivot> aAttributes, ref List<GenMemberPivot> aMembers, ref bool bNeedTextNode, ref bool bNeedDoubleArray)
        {
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMemberPivot oGenMember = oGenClass.aMembers[i];
                if (true == oGenMember.bIsAttribute)
                {
                    aAttributes.Add(oGenMember);
                }
                else
                {
                    if (oGenMember.nArrayRank.HasValue && oGenMember.nArrayRank > 0)
                    {
                        bNeedDoubleArray = true;
                    }

                    if (null != oGenMember.aArrayTypes)
                    {
                        for (int j = 0; j < oGenMember.aArrayTypes.Count; ++j)
                        {
                            InfoFromMemberElem(oGenMember.aArrayTypes[j], ref bNeedTextNode);
                        }
                    }
                    else
                    {
                        InfoFromMemberElem(oGenMember, ref bNeedTextNode);
                    }
                    aMembers.Add(oGenMember);
                }
            }
        }
        public void InfoFromMemberElem(GenMemberPivot oGenMember, ref bool bNeedTextNode)
        {
            if (null != oGenMember.oSystemType)
            {
                bNeedTextNode = true;
            }
            else if (null != oGenMember.sType)
            {
                GenClassPivot oTemp;
                if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTemp))
                {
                    if (oTemp.bIsEnum)
                    {
                        bNeedTextNode = true;
                    }
                }
            }
        }
        void ProcessProperty(StringBuilder sb, List<GenMemberPivot> aAttributes, List<GenMemberPivot> aMembers, bool bNeedTextNode, bool bNeedDoubleArray)
        {
            if (aAttributes.Count > 0)
            {
                sb.AppendFormat("//Attributes\r\n");
                for (int i = 0; i < aAttributes.Count; ++i)
                {
                    GenMemberPivot oGenMember = aAttributes[i];
                    if (!string.IsNullOrEmpty(oGenMember.sDefAttribute))
                    {
                        sb.AppendFormat("this.{0} = {1};\r\n", oGenMember.sName, oGenMember.sDefAttribute.ToLower());
                    }
                    else
                    {
                        sb.AppendFormat("this.{0} = null;\r\n", oGenMember.sName);
                    }
                }
            }
            if (aMembers.Count > 0)
            {
                sb.AppendFormat("//Members\r\n");
                for (int i = 0; i < aMembers.Count; ++i)
                {
                    GenMemberPivot oGenMember = aMembers[i];
                    if (oGenMember.nArrayRank.HasValue)
                    {
                        if (false != oGenMember.bIsArrayTypesHidden)
                            sb.AppendFormat("this.{0} = [];\r\n", oGenMember.sName);
                        else
                            sb.AppendFormat("this.{0} = null;\r\n", oGenMember.sName);
                    }
                    else
                    {
                        if (null != oGenMember.aArrayTypes)
                        {
                            for (int j = 0; j < oGenMember.aArrayTypes.Count; ++j)
                            {
                                GenMemberPivot oTempMember = oGenMember.aArrayTypes[j];
                                if (!string.IsNullOrEmpty(oTempMember.sDefAttribute))
                                {
                                    sb.AppendFormat("this.{0} = {1};\r\n", oTempMember.sName, oTempMember.sDefAttribute.ToLower());
                                }
                                else
                                {
                                    sb.AppendFormat("this.{0} = null;\r\n", oTempMember.sName);
                                }
                            }
                        }
                        else
                        {
                            if (!string.IsNullOrEmpty(oGenMember.sDefAttribute))
                            {
                                sb.AppendFormat("this.{0} = {1};\r\n", oGenMember.sName, oGenMember.sDefAttribute.ToLower());
                            }
                            else
                            {
                                sb.AppendFormat("this.{0} = null;\r\n", oGenMember.sName);
                            }
                        }
                    }
                }
            }
            if (bNeedTextNode || bNeedDoubleArray)
            {
                sb.AppendFormat("//internal\r\n");
                if (bNeedTextNode)
                    sb.AppendFormat("this._curElem = null;\r\n");
                if (bNeedDoubleArray)
                    sb.AppendFormat("this._curArray = null;\r\n");
            }

        }
        bool ProcessRootFromXml(StringBuilder sb, GenClassPivot oGenClass, string sCodeName)
        {
            string sName = oGenClass.sRootName;
            if (!string.IsNullOrEmpty(sName))
            {
                sb.AppendFormat("if(\"{0}\" === {1}){{\r\n", sName, sCodeName);
                sb.AppendFormat("if(newContext.readAttributes){{\r\n");
                sb.AppendFormat("newContext.readAttributes(attr, uq);\r\n");
                sb.AppendFormat("}}\r\n");
                sb.AppendFormat("}}\r\n", sName, sCodeName);
                return true;
            }
            return false;
        }
        void ProcessOnStartNodeFromXml(StringBuilder sb, GenClassPivot oGenClass, List<GenMemberPivot> aMembers, int nCounter, string sCodeName)
        {
            for (int i = 0; i < aMembers.Count; ++i)
            {
                GenMemberPivot oGenMember = aMembers[i];
                if (null != oGenMember.aArrayTypes)
                {
                    if (false == oGenMember.bIsArrayTypesHidden)
                    {
                        if (0 != nCounter)
                            sb.AppendFormat("else ");
                        //todo rework
                        sb.AppendFormat("if(\"{0}\" === {1}){{\r\n", getNameWithPrefix(oGenClass, oGenMember), sCodeName);
                        if (oGenMember.nArrayRank > 0)
                        {
                            sb.AppendFormat("this._curArray = [];\r\n", getNameWithPrefix(oGenClass, oGenMember), sCodeName);
                        }
                        else
                        {
                            sb.AppendFormat("//todo check name duplication\r\n", getNameWithPrefix(oGenClass, oGenMember), sCodeName);
                            sb.AppendFormat("this.{0} = [];\r\n", oGenMember.sName);
                        }
                        sb.AppendFormat("}}\r\n", getNameWithPrefix(oGenClass, oGenMember), sCodeName);
                        nCounter++;
                    }
                    for (int j = 0; j < oGenMember.aArrayTypes.Count; ++j)
                    {
                        nCounter = ProcessOnStartNodeFromXmlMember(sb, oGenClass, oGenMember.aArrayTypes[j], oGenMember, sCodeName, nCounter);
                    }
                }
                else
                {
                    nCounter = ProcessOnStartNodeFromXmlMember(sb, oGenClass, oGenMember, null, sCodeName, nCounter);
                }
            }
        }
        int ProcessOnStartNodeFromXmlMember(StringBuilder sb, GenClassPivot oGenClass, GenMemberPivot oGenMember, GenMemberPivot oGenMemberContainer, string sCodeName, int nCounter)
        {
            if (0 != nCounter)
                sb.AppendFormat("else ");
            sb.AppendFormat("if(\"{0}\" === {1}){{\r\n", getNameWithPrefix(oGenClass, oGenMember), sCodeName);

            if (null != oGenMember.oSystemType)
            {
                sb.AppendFormat("this._curElem = {0};\r\n", sCodeName);
            }
            else if (null != oGenMember.sType)
            {
                GenClassPivot oTemp;
                if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTemp))
                {
                    if (oTemp.bIsEnum)
                    {
                        sb.AppendFormat("this._curElem = {0};\r\n", sCodeName);
                    }
                    else
                    {
                        sb.AppendFormat("newContext = new {0}();\r\n", oGenMember.sType);
                        sb.AppendFormat("if(newContext.readAttributes){{\r\n");
                        sb.AppendFormat("newContext.readAttributes(attr, uq);\r\n", oGenMember.sType);
                        sb.AppendFormat("}}\r\n");
                        GenMemberPivot oTargetMember = null != oGenMemberContainer ? oGenMemberContainer : oGenMember;
                        if (oTargetMember.nArrayRank.HasValue)
                        {
                            if (oTargetMember.nArrayRank > 0)
                            {
                                sb.AppendFormat("this._curArray.push(newContext);\r\n");
                            }
                            else
                            {
                                sb.AppendFormat("this.{0}.push(newContext);\r\n", oTargetMember.sName);
                            }
                        }
                        else
                        {
                            //oGenMember not mistake
                            //right way to store oGenMemberContainer in one object, but we prefer separete types
                            sb.AppendFormat("this.{0} = newContext;\r\n", oGenMember.sName);
                        }

                    }
                }
            }
            sb.AppendFormat("}}\r\n");
            nCounter++;
            return nCounter;
        }
        void ProcessOnTextNodeFromXml(StringBuilder sb, GenClassPivot oGenClass, List<GenMemberPivot> aMembers)
        {
            int nCounter = 0;
            for (int i = 0; i < aMembers.Count; ++i)
            {
                GenMemberPivot oGenMember = aMembers[i];
                if (null != oGenMember.aArrayTypes)
                {
                    for (int j = 0; j < oGenMember.aArrayTypes.Count; ++j)
                    {
                        nCounter = ProcessOnTextNodeFromXmlMember(sb, oGenClass, oGenMember.aArrayTypes[j], nCounter);
                    }
                }
                else
                {
                    nCounter = ProcessOnTextNodeFromXmlMember(sb, oGenClass, oGenMember, nCounter);
                }
            }
        }
        int ProcessOnTextNodeFromXmlMember(StringBuilder sb, GenClassPivot oGenClass, GenMemberPivot oGenMember, int nCounter)
        {
            if (null != oGenMember.oSystemType)
            {
                ProcessOnTextNodeFromXmlMemberPrefix(sb, oGenClass, oGenMember, nCounter);
                ProcessOnTextNodeFromXmlMemberElem(sb, oGenMember, ProcessJsTypeFromXml(oGenMember.oSystemType, "text"));
                nCounter = ProcessOnTextNodeFromXmlMemberPostfix(sb, nCounter);
            }
            else if (null != oGenMember.sType)
            {
                GenClassPivot oTemp;
                if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTemp))
                {
                    if (oTemp.bIsEnum)
                    {
                        ProcessOnTextNodeFromXmlMemberPrefix(sb, oGenClass, oGenMember, nCounter);
                        sb.AppendFormat("var val = {0}(text);\r\n", gc_sEnumFromXmlPrefix + oTemp.sName);
                        sb.AppendFormat("if(-1 !== val){{\r\n");
                        ProcessOnTextNodeFromXmlMemberElem(sb, oGenMember, "val");
                        sb.AppendFormat("}}\r\n");
                        nCounter = ProcessOnTextNodeFromXmlMemberPostfix(sb, nCounter);
                    }
                }
            }
            
            return nCounter;
        }
        void ProcessOnTextNodeFromXmlMemberPrefix(StringBuilder sb, GenClassPivot oGenClass, GenMemberPivot oGenMember, int nCounter)
        {
            if (0 != nCounter)
                sb.AppendFormat("else ");
            sb.AppendFormat("if(\"{0}\" === this._curElem){{\r\n", getNameWithPrefix(oGenClass, oGenMember));
        }
        int ProcessOnTextNodeFromXmlMemberPostfix(StringBuilder sb, int nCounter)
        {
            sb.AppendFormat("}}\r\n");
            nCounter++;
            return nCounter;
        }
        void ProcessOnTextNodeFromXmlMemberElem(StringBuilder sb, GenMemberPivot oGenMember, string sCodeName)
        {
            if (oGenMember.nArrayRank.HasValue)
            {
                if (oGenMember.nArrayRank > 0)
                {
                    sb.AppendFormat("this._curArray.push({0});\r\n", sCodeName);
                }
                else
                {
                    sb.AppendFormat("this.{0}.push({1});\r\n", oGenMember.sName, sCodeName);
                }
            }
            else
                sb.AppendFormat("this.{0} = {1};\r\n", oGenMember.sName, sCodeName);
        }
        void ProcessOnEndNodeFromXml(StringBuilder sb, GenClassPivot oGenClass, List<GenMemberPivot> aMembers)
        {
            int nCounter = 0;
            for (int i = 0; i < aMembers.Count; ++i)
            {
                GenMemberPivot oGenMember = aMembers[i];
                if (oGenMember.nArrayRank > 0)
                {
                    if (0 != nCounter)
                        sb.AppendFormat("else ");
                    sb.AppendFormat("if(\"{0}\" === elem){{\r\n", getNameWithPrefix(oGenClass, oGenMember));
                    sb.AppendFormat("if(this._curArray && this._curArray.length > 0){{\r\n");
                    sb.AppendFormat("if(!this.{0}){{\r\n", oGenMember.sName);
                    sb.AppendFormat("this.{0} = [];\r\n", oGenMember.sName);
                    sb.AppendFormat("}}\r\n");
                    sb.AppendFormat("this.{0}.push(this._curArray);\r\n", oGenMember.sName);
                    sb.AppendFormat("this._curArray  = null;\r\n");
                    sb.AppendFormat("}}\r\n");
                    sb.AppendFormat("}}\r\n");
                    nCounter++;
                }
            }
        }
        void ProcessAttributesFromXml(StringBuilder sb, GenClassPivot oGenClass, List<GenMemberPivot> aAttributes)
        {
            sb.AppendFormat("if(attr()){{\r\n");
            sb.AppendFormat("var vals = attr();\r\n");
            sb.AppendFormat("var val;\r\n");
            for (int i = 0; i < aAttributes.Count; ++i)
            {
                GenMemberPivot oGenMember = aAttributes[i];
                sb.AppendFormat("val = vals[\"{0}\"];\r\n", getNameWithPrefix(oGenClass, oGenMember));
                sb.AppendFormat("if(undefined !== val){{\r\n");
                if (null != oGenMember.oSystemType)
                {
                    sb.AppendFormat("this.{0} = {1};\r\n", oGenMember.sName, ProcessJsTypeFromXml(oGenMember.oSystemType, "val"));
                }
                else if (null != oGenMember.sType)
                {
                    GenClassPivot oTemp;
                    if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTemp))
                    {
                        if (oTemp.bIsEnum)
                        {
                            sb.AppendFormat("val = {0}(val);\r\n", gc_sEnumFromXmlPrefix + oTemp.sName);
                            sb.AppendFormat("if(-1 !== val){{\r\n");
                            sb.AppendFormat("this.{0} = val;\r\n", oGenMember.sName);
                            sb.AppendFormat("}}\r\n");
                        }
                        //else
                        //    sb.AppendFormat("pNewElem->fromXML(oReader);\r\n");
                    }
                }
                sb.AppendFormat("}}\r\n");
            }
            sb.AppendFormat("}}\r\n");
        }

        string ProcessJsTypeFromXml(Type oType, string sVal)
        {
            string sRes;
            //todo
            switch (Type.GetTypeCode(oType))
            {
                case TypeCode.Boolean:
                    sRes = "getBoolFromXml(" + sVal + ")";
                    break;
                case TypeCode.Byte:
                case TypeCode.SByte:
                case TypeCode.Int16:
                case TypeCode.Int32:
                case TypeCode.Int64:
                case TypeCode.UInt16:
                case TypeCode.UInt32:
                case TypeCode.UInt64:
                case TypeCode.Single:
                case TypeCode.Double:
                    sRes = sVal + " - 0";
                    break;
                default: sRes = "uq(" + sVal + ")"; break;
            }
            return sRes;
        }

        void ProcessToXml(StringBuilder sb, GenClassPivot oGenClass)
        {
            List<GenMemberPivot> aAttributes = new List<GenMemberPivot>();
            List<GenMemberPivot> aMembers = new List<GenMemberPivot>();
            bool bNeedTextNode = false;
            bool bNeedDoubleArray = false;
            InfoFromMember(oGenClass, ref aAttributes, ref aMembers, ref bNeedTextNode, ref bNeedDoubleArray);

            string sCodeName;
            if (oGenClass.isRoot())
            {
                sCodeName = "\"" + oGenClass.sRootName + "\"";
                sb.AppendFormat("writer.WriteXmlString(\"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\" standalone=\\\"yes\\\"?>\");\r\n");
                sb.AppendFormat("writer.WriteXmlNodeStart({0});\r\n", sCodeName);
                sb.AppendFormat("writer.WriteXmlString(\"{0}\");\r\n", gc_sNamespaceToXml);
                
            }
            else
            {
                sCodeName = "name";
                sb.AppendFormat("writer.WriteXmlNodeStart({0});\r\n", sCodeName);
            }

            for (int i = 0; i < aAttributes.Count; ++i)
            {
                GenMemberPivot oGenMember = aAttributes[i];
                ProcessMemberToXml(sb, oGenClass, oGenMember, "this." + oGenMember.sName, true);
            }

            if (aMembers.Count > 0)
            {
                sb.AppendFormat("writer.WriteXmlNodeEnd({0}, true);\r\n", sCodeName);
                for (int i = 0; i < aMembers.Count; ++i)
                {
                    GenMemberPivot oGenMember = aMembers[i];
                    bool bNullCheck = true;
                    string sCodeElem  = "this." + oGenMember.sName;
                    if (oGenMember.nArrayRank > 0)
                    {
                        if (false == oGenMember.bIsArrayTypesHidden)
                        {
                            sb.AppendFormat("if(null !== {0}){{\r\n", sCodeElem);
                        }
                        sb.AppendFormat("for(var i = 0; i < {0}.length; ++i){{\r\n", sCodeElem);
                        sb.AppendFormat("var elem = {0}[i];\r\n", sCodeElem);
                        sCodeElem = "elem";
                        bNullCheck = false;
                    }
                    if (null != oGenMember.aArrayTypes)
                    {
                        if (false == oGenMember.bIsArrayTypesHidden)
                        {
                            if (bNullCheck)
                                sb.AppendFormat("if(null !== {0}){{\r\n", sCodeElem);
                            sb.AppendFormat("writer.WriteXmlNodeStart(\"{0}\", true);\r\n", getNameWithPrefix(oGenClass, oGenMember));
                        }
                        if (oGenMember.nArrayRank.HasValue)
                        {
                            string sCodeSubElem;
                            if (oGenMember.nArrayRank > 0)
                            {
                                sb.AppendFormat("for(var j = 0; j < {0}.length; ++j){{\r\n", sCodeElem);
                                sb.AppendFormat("var subelem = {0}[j];\r\n", sCodeElem);
                                sCodeSubElem = "subelem";
                            }
                            else
                            {
                                sb.AppendFormat("for(var i = 0; i < {0}.length; ++i){{\r\n", sCodeElem);
                                sb.AppendFormat("var elem = {0}[i];\r\n", sCodeElem);
                                sCodeSubElem = "elem";
                            }
                            if (1 == oGenMember.aArrayTypes.Count)
                            {
                                GenMemberPivot oTempMember = oGenMember.aArrayTypes[0];
                                ProcessMemberToXml(sb, oGenClass, oTempMember, sCodeSubElem, false);
                            }
                            else
                            {
                                for (int j = 0; j < oGenMember.aArrayTypes.Count; ++j)
                                {
                                    GenMemberPivot oTempMember = oGenMember.aArrayTypes[j];
                                    if (0 != j)
                                        sb.AppendFormat("else ");
                                    sb.AppendFormat("if({0} instanceof {1}){{\r\n", sCodeSubElem, oTempMember.sType);
                                    sb.AppendFormat("{0}.toXml(writer, \"{1}\");\r\n", sCodeSubElem, getNameWithPrefix(oGenClass, oTempMember));
                                    sb.AppendFormat("}}\r\n");
                                }
                            }
                            sb.AppendFormat("}}\r\n");
                        }
                        else
                        {
                            //right way to store oGenMemberContainer in one object, but we prefer separete types
                            for (int j = 0; j < oGenMember.aArrayTypes.Count; ++j)
                            {
                                GenMemberPivot oTempMember = oGenMember.aArrayTypes[j];
                                ProcessMemberToXml(sb, oGenClass, oTempMember, "this." + oTempMember.sName, true);
                            }
                        }
                        if (false == oGenMember.bIsArrayTypesHidden)
                        {
                            sb.AppendFormat("writer.WriteXmlNodeEnd(\"{0}\");\r\n", getNameWithPrefix(oGenClass, oGenMember));
                            if (bNullCheck)
                                sb.AppendFormat("}}\r\n");
                        }
                    }
                    else
                    {
                        ProcessMemberToXml(sb, oGenClass, oGenMember, sCodeElem, true);
                    }
                    if (oGenMember.nArrayRank > 0)
                    {
                        sb.AppendFormat("}}\r\n");
                        if (false == oGenMember.bIsArrayTypesHidden)
                        {
                            sb.AppendFormat("}}\r\n");
                        }
                    }
                }

                sb.AppendFormat("writer.WriteXmlNodeEnd({0});\r\n", sCodeName);
            }
            else
                sb.AppendFormat("writer.WriteXmlNodeEnd({0}, true, true);\r\n", sCodeName);
        }
        void ProcessMemberToXml(StringBuilder sb, GenClassPivot oGenClass, GenMemberPivot oGenMember, string sElemName, bool checkNull)
        {
            string sElemXmlName = getNameWithPrefix(oGenClass, oGenMember);
            bool bIsAttribute = true == oGenMember.bIsAttribute;

            if (checkNull)
            {
                if (!string.IsNullOrEmpty(oGenMember.sDefAttribute))
                {
                    sb.AppendFormat("if({0} !== {1}){{\r\n", oGenMember.sDefAttribute.ToLower(), sElemName);
                }
                else
                {
                    sb.AppendFormat("if(null !== {0}){{\r\n", sElemName);
                }
                
            }

            if (null != oGenMember.oSystemType)
            {
                ProcessJsTypeToXml(sb, sElemXmlName, oGenMember.oSystemType, sElemName, bIsAttribute);
            }
            else if (null != oGenMember.sType)
            {
                GenClassPivot oGenClassMember = null;
                m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oGenClassMember);
                if (null != oGenClassMember || m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oGenClassMember))
                {
                    if (oGenClassMember.bIsEnum)
                    {
                        string sElemNameEnum = gc_sEnumToXmlPrefix + oGenClassMember.sName + "("+ sElemName + ")";
                        ProcessJsTypeToXml(sb, sElemXmlName, oGenMember.oSystemType, sElemNameEnum, bIsAttribute);
                    }
                    else
                    {
                        if (!bIsAttribute)
                            sb.AppendFormat("{0}.toXml(writer, \"{1}\");\r\n", sElemName, sElemXmlName);
                    }
                }
            }
            if (checkNull)
            {
                sb.AppendFormat("}}\r\n");
            }
        }
        void ProcessJsTypeToXml(StringBuilder sb, string sName, Type oType, string sVal, bool bAttribute)
        {
            switch (Type.GetTypeCode(oType))
            {
                case TypeCode.Boolean:
                    if(bAttribute)
                    {
                        sb.AppendFormat("writer.WriteXmlAttributeBool(\"{0}\", {1});\r\n", sName, sVal);
                    }
                    else
                    {
                        sb.AppendFormat("writer.WriteXmlNodeStart(\"{0}\", true);\r\n", sName);
                        sb.AppendFormat("writer.WriteXmlBool({0});\r\n", sVal);
                        sb.AppendFormat("writer.WriteXmlNodeEnd(\"{0}\");\r\n", sName);
                    }
                    break;
                case TypeCode.Byte:
                case TypeCode.SByte:
                case TypeCode.Int16:
                case TypeCode.Int32:
                case TypeCode.Int64:
                case TypeCode.UInt16:
                case TypeCode.UInt32:
                case TypeCode.UInt64:
                case TypeCode.Single:
                case TypeCode.Double:
                    if (bAttribute)
                    {
                        sb.AppendFormat("writer.WriteXmlAttributeNumber(\"{0}\", {1});\r\n", sName, sVal);
                    }
                    else
                    {
                        sb.AppendFormat("writer.WriteXmlNodeStart(\"{0}\", true);\r\n", sName);
                        sb.AppendFormat("writer.WriteXmlNumber({0});\r\n", sVal);
                        sb.AppendFormat("writer.WriteXmlNodeEnd(\"{0}\");\r\n", sName);
                    }
                    break;
                default:
                    if (bAttribute)
                    {
                        sb.AppendFormat("writer.WriteXmlAttributeStringEncode(\"{0}\", {1});\r\n", sName, sVal);
                    }
                    else
                    {
                        sb.AppendFormat("writer.WriteXmlNodeStart(\"{0}\", true);\r\n", sName);
                        sb.AppendFormat("writer.WriteXmlStringEncode({0});\r\n", sVal);
                        sb.AppendFormat("writer.WriteXmlNodeEnd(\"{0}\");\r\n", sName);
                    }
                    break;
            }
        }

        string getNameWithPrefix(GenClassPivot oGenClass, GenMemberPivot oGenMember)
        {
            string sShortNamespace = "";
            if (true == oGenMember.bQualified)
            {
                string sMemberNamespace = null;
                GenClassPivot oGenClassMember = null;
                if (null != oGenMember.sNamespace)
                    sMemberNamespace = oGenMember.sNamespace;
                else if (null != oGenMember.sType && m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oGenClassMember))
                    sMemberNamespace = oGenClassMember.sNamespace;
                else
                    sMemberNamespace = oGenClass.sNamespace;
                if (null != sMemberNamespace)
                {
                    Utils.m_mapNamespaceToPrefix.TryGetValue(sMemberNamespace, out sShortNamespace);
                }
            }
            return sShortNamespace + oGenMember.sName;
        }
        string GetEnumClassName(string sEnumName)
        {
            int index = sEnumName.LastIndexOf('_');
            return "c_oAsc" + (index >= 0 ? sEnumName.Substring(index + 1) : sEnumName);
        }
        string GetEnumElemName(string sElemName)
        {
            string first = sElemName.Substring(0, 1);
            int x = 0;
            if(Int32.TryParse(first, out x))
            {
                first = gc_numeric[x];
            }
            else
            {
                first = first.ToUpper();
            }
            string name = first + sElemName.Substring(1);
            //https://stackoverflow.com/questions/1661197/what-characters-are-valid-for-javascript-variable-names/9337047#9337047
            Match isMatch = Regex.Match(name, @"^(?!(?:do|if|in|for|let|new|try|var|case|else|enum|eval|false|null|this|true|void|with|break|catch|class|const|super|throw|while|yield|delete|export|import|public|return|static|switch|typeof|default|extends|finally|package|private|continue|debugger|function|arguments|interface|protected|implements|instanceof)$)[$A-Z\\_a-z\xaa\xb5\xba\xc0-\xd6\xd8-\xf6\xf8-\u02c1\u02c6-\u02d1\u02e0-\u02e4\u02ec\u02ee\u0370-\u0374\u0376\u0377\u037a-\u037d\u0386\u0388-\u038a\u038c\u038e-\u03a1\u03a3-\u03f5\u03f7-\u0481\u048a-\u0527\u0531-\u0556\u0559\u0561-\u0587\u05d0-\u05ea\u05f0-\u05f2\u0620-\u064a\u066e\u066f\u0671-\u06d3\u06d5\u06e5\u06e6\u06ee\u06ef\u06fa-\u06fc\u06ff\u0710\u0712-\u072f\u074d-\u07a5\u07b1\u07ca-\u07ea\u07f4\u07f5\u07fa\u0800-\u0815\u081a\u0824\u0828\u0840-\u0858\u08a0\u08a2-\u08ac\u0904-\u0939\u093d\u0950\u0958-\u0961\u0971-\u0977\u0979-\u097f\u0985-\u098c\u098f\u0990\u0993-\u09a8\u09aa-\u09b0\u09b2\u09b6-\u09b9\u09bd\u09ce\u09dc\u09dd\u09df-\u09e1\u09f0\u09f1\u0a05-\u0a0a\u0a0f\u0a10\u0a13-\u0a28\u0a2a-\u0a30\u0a32\u0a33\u0a35\u0a36\u0a38\u0a39\u0a59-\u0a5c\u0a5e\u0a72-\u0a74\u0a85-\u0a8d\u0a8f-\u0a91\u0a93-\u0aa8\u0aaa-\u0ab0\u0ab2\u0ab3\u0ab5-\u0ab9\u0abd\u0ad0\u0ae0\u0ae1\u0b05-\u0b0c\u0b0f\u0b10\u0b13-\u0b28\u0b2a-\u0b30\u0b32\u0b33\u0b35-\u0b39\u0b3d\u0b5c\u0b5d\u0b5f-\u0b61\u0b71\u0b83\u0b85-\u0b8a\u0b8e-\u0b90\u0b92-\u0b95\u0b99\u0b9a\u0b9c\u0b9e\u0b9f\u0ba3\u0ba4\u0ba8-\u0baa\u0bae-\u0bb9\u0bd0\u0c05-\u0c0c\u0c0e-\u0c10\u0c12-\u0c28\u0c2a-\u0c33\u0c35-\u0c39\u0c3d\u0c58\u0c59\u0c60\u0c61\u0c85-\u0c8c\u0c8e-\u0c90\u0c92-\u0ca8\u0caa-\u0cb3\u0cb5-\u0cb9\u0cbd\u0cde\u0ce0\u0ce1\u0cf1\u0cf2\u0d05-\u0d0c\u0d0e-\u0d10\u0d12-\u0d3a\u0d3d\u0d4e\u0d60\u0d61\u0d7a-\u0d7f\u0d85-\u0d96\u0d9a-\u0db1\u0db3-\u0dbb\u0dbd\u0dc0-\u0dc6\u0e01-\u0e30\u0e32\u0e33\u0e40-\u0e46\u0e81\u0e82\u0e84\u0e87\u0e88\u0e8a\u0e8d\u0e94-\u0e97\u0e99-\u0e9f\u0ea1-\u0ea3\u0ea5\u0ea7\u0eaa\u0eab\u0ead-\u0eb0\u0eb2\u0eb3\u0ebd\u0ec0-\u0ec4\u0ec6\u0edc-\u0edf\u0f00\u0f40-\u0f47\u0f49-\u0f6c\u0f88-\u0f8c\u1000-\u102a\u103f\u1050-\u1055\u105a-\u105d\u1061\u1065\u1066\u106e-\u1070\u1075-\u1081\u108e\u10a0-\u10c5\u10c7\u10cd\u10d0-\u10fa\u10fc-\u1248\u124a-\u124d\u1250-\u1256\u1258\u125a-\u125d\u1260-\u1288\u128a-\u128d\u1290-\u12b0\u12b2-\u12b5\u12b8-\u12be\u12c0\u12c2-\u12c5\u12c8-\u12d6\u12d8-\u1310\u1312-\u1315\u1318-\u135a\u1380-\u138f\u13a0-\u13f4\u1401-\u166c\u166f-\u167f\u1681-\u169a\u16a0-\u16ea\u16ee-\u16f0\u1700-\u170c\u170e-\u1711\u1720-\u1731\u1740-\u1751\u1760-\u176c\u176e-\u1770\u1780-\u17b3\u17d7\u17dc\u1820-\u1877\u1880-\u18a8\u18aa\u18b0-\u18f5\u1900-\u191c\u1950-\u196d\u1970-\u1974\u1980-\u19ab\u19c1-\u19c7\u1a00-\u1a16\u1a20-\u1a54\u1aa7\u1b05-\u1b33\u1b45-\u1b4b\u1b83-\u1ba0\u1bae\u1baf\u1bba-\u1be5\u1c00-\u1c23\u1c4d-\u1c4f\u1c5a-\u1c7d\u1ce9-\u1cec\u1cee-\u1cf1\u1cf5\u1cf6\u1d00-\u1dbf\u1e00-\u1f15\u1f18-\u1f1d\u1f20-\u1f45\u1f48-\u1f4d\u1f50-\u1f57\u1f59\u1f5b\u1f5d\u1f5f-\u1f7d\u1f80-\u1fb4\u1fb6-\u1fbc\u1fbe\u1fc2-\u1fc4\u1fc6-\u1fcc\u1fd0-\u1fd3\u1fd6-\u1fdb\u1fe0-\u1fec\u1ff2-\u1ff4\u1ff6-\u1ffc\u2071\u207f\u2090-\u209c\u2102\u2107\u210a-\u2113\u2115\u2119-\u211d\u2124\u2126\u2128\u212a-\u212d\u212f-\u2139\u213c-\u213f\u2145-\u2149\u214e\u2160-\u2188\u2c00-\u2c2e\u2c30-\u2c5e\u2c60-\u2ce4\u2ceb-\u2cee\u2cf2\u2cf3\u2d00-\u2d25\u2d27\u2d2d\u2d30-\u2d67\u2d6f\u2d80-\u2d96\u2da0-\u2da6\u2da8-\u2dae\u2db0-\u2db6\u2db8-\u2dbe\u2dc0-\u2dc6\u2dc8-\u2dce\u2dd0-\u2dd6\u2dd8-\u2dde\u2e2f\u3005-\u3007\u3021-\u3029\u3031-\u3035\u3038-\u303c\u3041-\u3096\u309d-\u309f\u30a1-\u30fa\u30fc-\u30ff\u3105-\u312d\u3131-\u318e\u31a0-\u31ba\u31f0-\u31ff\u3400-\u4db5\u4e00-\u9fcc\ua000-\ua48c\ua4d0-\ua4fd\ua500-\ua60c\ua610-\ua61f\ua62a\ua62b\ua640-\ua66e\ua67f-\ua697\ua6a0-\ua6ef\ua717-\ua71f\ua722-\ua788\ua78b-\ua78e\ua790-\ua793\ua7a0-\ua7aa\ua7f8-\ua801\ua803-\ua805\ua807-\ua80a\ua80c-\ua822\ua840-\ua873\ua882-\ua8b3\ua8f2-\ua8f7\ua8fb\ua90a-\ua925\ua930-\ua946\ua960-\ua97c\ua984-\ua9b2\ua9cf\uaa00-\uaa28\uaa40-\uaa42\uaa44-\uaa4b\uaa60-\uaa76\uaa7a\uaa80-\uaaaf\uaab1\uaab5\uaab6\uaab9-\uaabd\uaac0\uaac2\uaadb-\uaadd\uaae0-\uaaea\uaaf2-\uaaf4\uab01-\uab06\uab09-\uab0e\uab11-\uab16\uab20-\uab26\uab28-\uab2e\uabc0-\uabe2\uac00-\ud7a3\ud7b0-\ud7c6\ud7cb-\ud7fb\uf900-\ufa6d\ufa70-\ufad9\ufb00-\ufb06\ufb13-\ufb17\ufb1d\ufb1f-\ufb28\ufb2a-\ufb36\ufb38-\ufb3c\ufb3e\ufb40\ufb41\ufb43\ufb44\ufb46-\ufbb1\ufbd3-\ufd3d\ufd50-\ufd8f\ufd92-\ufdc7\ufdf0-\ufdfb\ufe70-\ufe74\ufe76-\ufefc\uff21-\uff3a\uff41-\uff5a\uff66-\uffbe\uffc2-\uffc7\uffca-\uffcf\uffd2-\uffd7\uffda-\uffdc][$A-Z\\_a-z\xaa\xb5\xba\xc0-\xd6\xd8-\xf6\xf8-\u02c1\u02c6-\u02d1\u02e0-\u02e4\u02ec\u02ee\u0370-\u0374\u0376\u0377\u037a-\u037d\u0386\u0388-\u038a\u038c\u038e-\u03a1\u03a3-\u03f5\u03f7-\u0481\u048a-\u0527\u0531-\u0556\u0559\u0561-\u0587\u05d0-\u05ea\u05f0-\u05f2\u0620-\u064a\u066e\u066f\u0671-\u06d3\u06d5\u06e5\u06e6\u06ee\u06ef\u06fa-\u06fc\u06ff\u0710\u0712-\u072f\u074d-\u07a5\u07b1\u07ca-\u07ea\u07f4\u07f5\u07fa\u0800-\u0815\u081a\u0824\u0828\u0840-\u0858\u08a0\u08a2-\u08ac\u0904-\u0939\u093d\u0950\u0958-\u0961\u0971-\u0977\u0979-\u097f\u0985-\u098c\u098f\u0990\u0993-\u09a8\u09aa-\u09b0\u09b2\u09b6-\u09b9\u09bd\u09ce\u09dc\u09dd\u09df-\u09e1\u09f0\u09f1\u0a05-\u0a0a\u0a0f\u0a10\u0a13-\u0a28\u0a2a-\u0a30\u0a32\u0a33\u0a35\u0a36\u0a38\u0a39\u0a59-\u0a5c\u0a5e\u0a72-\u0a74\u0a85-\u0a8d\u0a8f-\u0a91\u0a93-\u0aa8\u0aaa-\u0ab0\u0ab2\u0ab3\u0ab5-\u0ab9\u0abd\u0ad0\u0ae0\u0ae1\u0b05-\u0b0c\u0b0f\u0b10\u0b13-\u0b28\u0b2a-\u0b30\u0b32\u0b33\u0b35-\u0b39\u0b3d\u0b5c\u0b5d\u0b5f-\u0b61\u0b71\u0b83\u0b85-\u0b8a\u0b8e-\u0b90\u0b92-\u0b95\u0b99\u0b9a\u0b9c\u0b9e\u0b9f\u0ba3\u0ba4\u0ba8-\u0baa\u0bae-\u0bb9\u0bd0\u0c05-\u0c0c\u0c0e-\u0c10\u0c12-\u0c28\u0c2a-\u0c33\u0c35-\u0c39\u0c3d\u0c58\u0c59\u0c60\u0c61\u0c85-\u0c8c\u0c8e-\u0c90\u0c92-\u0ca8\u0caa-\u0cb3\u0cb5-\u0cb9\u0cbd\u0cde\u0ce0\u0ce1\u0cf1\u0cf2\u0d05-\u0d0c\u0d0e-\u0d10\u0d12-\u0d3a\u0d3d\u0d4e\u0d60\u0d61\u0d7a-\u0d7f\u0d85-\u0d96\u0d9a-\u0db1\u0db3-\u0dbb\u0dbd\u0dc0-\u0dc6\u0e01-\u0e30\u0e32\u0e33\u0e40-\u0e46\u0e81\u0e82\u0e84\u0e87\u0e88\u0e8a\u0e8d\u0e94-\u0e97\u0e99-\u0e9f\u0ea1-\u0ea3\u0ea5\u0ea7\u0eaa\u0eab\u0ead-\u0eb0\u0eb2\u0eb3\u0ebd\u0ec0-\u0ec4\u0ec6\u0edc-\u0edf\u0f00\u0f40-\u0f47\u0f49-\u0f6c\u0f88-\u0f8c\u1000-\u102a\u103f\u1050-\u1055\u105a-\u105d\u1061\u1065\u1066\u106e-\u1070\u1075-\u1081\u108e\u10a0-\u10c5\u10c7\u10cd\u10d0-\u10fa\u10fc-\u1248\u124a-\u124d\u1250-\u1256\u1258\u125a-\u125d\u1260-\u1288\u128a-\u128d\u1290-\u12b0\u12b2-\u12b5\u12b8-\u12be\u12c0\u12c2-\u12c5\u12c8-\u12d6\u12d8-\u1310\u1312-\u1315\u1318-\u135a\u1380-\u138f\u13a0-\u13f4\u1401-\u166c\u166f-\u167f\u1681-\u169a\u16a0-\u16ea\u16ee-\u16f0\u1700-\u170c\u170e-\u1711\u1720-\u1731\u1740-\u1751\u1760-\u176c\u176e-\u1770\u1780-\u17b3\u17d7\u17dc\u1820-\u1877\u1880-\u18a8\u18aa\u18b0-\u18f5\u1900-\u191c\u1950-\u196d\u1970-\u1974\u1980-\u19ab\u19c1-\u19c7\u1a00-\u1a16\u1a20-\u1a54\u1aa7\u1b05-\u1b33\u1b45-\u1b4b\u1b83-\u1ba0\u1bae\u1baf\u1bba-\u1be5\u1c00-\u1c23\u1c4d-\u1c4f\u1c5a-\u1c7d\u1ce9-\u1cec\u1cee-\u1cf1\u1cf5\u1cf6\u1d00-\u1dbf\u1e00-\u1f15\u1f18-\u1f1d\u1f20-\u1f45\u1f48-\u1f4d\u1f50-\u1f57\u1f59\u1f5b\u1f5d\u1f5f-\u1f7d\u1f80-\u1fb4\u1fb6-\u1fbc\u1fbe\u1fc2-\u1fc4\u1fc6-\u1fcc\u1fd0-\u1fd3\u1fd6-\u1fdb\u1fe0-\u1fec\u1ff2-\u1ff4\u1ff6-\u1ffc\u2071\u207f\u2090-\u209c\u2102\u2107\u210a-\u2113\u2115\u2119-\u211d\u2124\u2126\u2128\u212a-\u212d\u212f-\u2139\u213c-\u213f\u2145-\u2149\u214e\u2160-\u2188\u2c00-\u2c2e\u2c30-\u2c5e\u2c60-\u2ce4\u2ceb-\u2cee\u2cf2\u2cf3\u2d00-\u2d25\u2d27\u2d2d\u2d30-\u2d67\u2d6f\u2d80-\u2d96\u2da0-\u2da6\u2da8-\u2dae\u2db0-\u2db6\u2db8-\u2dbe\u2dc0-\u2dc6\u2dc8-\u2dce\u2dd0-\u2dd6\u2dd8-\u2dde\u2e2f\u3005-\u3007\u3021-\u3029\u3031-\u3035\u3038-\u303c\u3041-\u3096\u309d-\u309f\u30a1-\u30fa\u30fc-\u30ff\u3105-\u312d\u3131-\u318e\u31a0-\u31ba\u31f0-\u31ff\u3400-\u4db5\u4e00-\u9fcc\ua000-\ua48c\ua4d0-\ua4fd\ua500-\ua60c\ua610-\ua61f\ua62a\ua62b\ua640-\ua66e\ua67f-\ua697\ua6a0-\ua6ef\ua717-\ua71f\ua722-\ua788\ua78b-\ua78e\ua790-\ua793\ua7a0-\ua7aa\ua7f8-\ua801\ua803-\ua805\ua807-\ua80a\ua80c-\ua822\ua840-\ua873\ua882-\ua8b3\ua8f2-\ua8f7\ua8fb\ua90a-\ua925\ua930-\ua946\ua960-\ua97c\ua984-\ua9b2\ua9cf\uaa00-\uaa28\uaa40-\uaa42\uaa44-\uaa4b\uaa60-\uaa76\uaa7a\uaa80-\uaaaf\uaab1\uaab5\uaab6\uaab9-\uaabd\uaac0\uaac2\uaadb-\uaadd\uaae0-\uaaea\uaaf2-\uaaf4\uab01-\uab06\uab09-\uab0e\uab11-\uab16\uab20-\uab26\uab28-\uab2e\uabc0-\uabe2\uac00-\ud7a3\ud7b0-\ud7c6\ud7cb-\ud7fb\uf900-\ufa6d\ufa70-\ufad9\ufb00-\ufb06\ufb13-\ufb17\ufb1d\ufb1f-\ufb28\ufb2a-\ufb36\ufb38-\ufb3c\ufb3e\ufb40\ufb41\ufb43\ufb44\ufb46-\ufbb1\ufbd3-\ufd3d\ufd50-\ufd8f\ufd92-\ufdc7\ufdf0-\ufdfb\ufe70-\ufe74\ufe76-\ufefc\uff21-\uff3a\uff41-\uff5a\uff66-\uffbe\uffc2-\uffc7\uffca-\uffcf\uffd2-\uffd7\uffda-\uffdc0-9\u0300-\u036f\u0483-\u0487\u0591-\u05bd\u05bf\u05c1\u05c2\u05c4\u05c5\u05c7\u0610-\u061a\u064b-\u0669\u0670\u06d6-\u06dc\u06df-\u06e4\u06e7\u06e8\u06ea-\u06ed\u06f0-\u06f9\u0711\u0730-\u074a\u07a6-\u07b0\u07c0-\u07c9\u07eb-\u07f3\u0816-\u0819\u081b-\u0823\u0825-\u0827\u0829-\u082d\u0859-\u085b\u08e4-\u08fe\u0900-\u0903\u093a-\u093c\u093e-\u094f\u0951-\u0957\u0962\u0963\u0966-\u096f\u0981-\u0983\u09bc\u09be-\u09c4\u09c7\u09c8\u09cb-\u09cd\u09d7\u09e2\u09e3\u09e6-\u09ef\u0a01-\u0a03\u0a3c\u0a3e-\u0a42\u0a47\u0a48\u0a4b-\u0a4d\u0a51\u0a66-\u0a71\u0a75\u0a81-\u0a83\u0abc\u0abe-\u0ac5\u0ac7-\u0ac9\u0acb-\u0acd\u0ae2\u0ae3\u0ae6-\u0aef\u0b01-\u0b03\u0b3c\u0b3e-\u0b44\u0b47\u0b48\u0b4b-\u0b4d\u0b56\u0b57\u0b62\u0b63\u0b66-\u0b6f\u0b82\u0bbe-\u0bc2\u0bc6-\u0bc8\u0bca-\u0bcd\u0bd7\u0be6-\u0bef\u0c01-\u0c03\u0c3e-\u0c44\u0c46-\u0c48\u0c4a-\u0c4d\u0c55\u0c56\u0c62\u0c63\u0c66-\u0c6f\u0c82\u0c83\u0cbc\u0cbe-\u0cc4\u0cc6-\u0cc8\u0cca-\u0ccd\u0cd5\u0cd6\u0ce2\u0ce3\u0ce6-\u0cef\u0d02\u0d03\u0d3e-\u0d44\u0d46-\u0d48\u0d4a-\u0d4d\u0d57\u0d62\u0d63\u0d66-\u0d6f\u0d82\u0d83\u0dca\u0dcf-\u0dd4\u0dd6\u0dd8-\u0ddf\u0df2\u0df3\u0e31\u0e34-\u0e3a\u0e47-\u0e4e\u0e50-\u0e59\u0eb1\u0eb4-\u0eb9\u0ebb\u0ebc\u0ec8-\u0ecd\u0ed0-\u0ed9\u0f18\u0f19\u0f20-\u0f29\u0f35\u0f37\u0f39\u0f3e\u0f3f\u0f71-\u0f84\u0f86\u0f87\u0f8d-\u0f97\u0f99-\u0fbc\u0fc6\u102b-\u103e\u1040-\u1049\u1056-\u1059\u105e-\u1060\u1062-\u1064\u1067-\u106d\u1071-\u1074\u1082-\u108d\u108f-\u109d\u135d-\u135f\u1712-\u1714\u1732-\u1734\u1752\u1753\u1772\u1773\u17b4-\u17d3\u17dd\u17e0-\u17e9\u180b-\u180d\u1810-\u1819\u18a9\u1920-\u192b\u1930-\u193b\u1946-\u194f\u19b0-\u19c0\u19c8\u19c9\u19d0-\u19d9\u1a17-\u1a1b\u1a55-\u1a5e\u1a60-\u1a7c\u1a7f-\u1a89\u1a90-\u1a99\u1b00-\u1b04\u1b34-\u1b44\u1b50-\u1b59\u1b6b-\u1b73\u1b80-\u1b82\u1ba1-\u1bad\u1bb0-\u1bb9\u1be6-\u1bf3\u1c24-\u1c37\u1c40-\u1c49\u1c50-\u1c59\u1cd0-\u1cd2\u1cd4-\u1ce8\u1ced\u1cf2-\u1cf4\u1dc0-\u1de6\u1dfc-\u1dff\u200c\u200d\u203f\u2040\u2054\u20d0-\u20dc\u20e1\u20e5-\u20f0\u2cef-\u2cf1\u2d7f\u2de0-\u2dff\u302a-\u302f\u3099\u309a\ua620-\ua629\ua66f\ua674-\ua67d\ua69f\ua6f0\ua6f1\ua802\ua806\ua80b\ua823-\ua827\ua880\ua881\ua8b4-\ua8c4\ua8d0-\ua8d9\ua8e0-\ua8f1\ua900-\ua909\ua926-\ua92d\ua947-\ua953\ua980-\ua983\ua9b3-\ua9c0\ua9d0-\ua9d9\uaa29-\uaa36\uaa43\uaa4c\uaa4d\uaa50-\uaa59\uaa7b\uaab0\uaab2-\uaab4\uaab7\uaab8\uaabe\uaabf\uaac1\uaaeb-\uaaef\uaaf5\uaaf6\uabe3-\uabea\uabec\uabed\uabf0-\uabf9\ufb1e\ufe00-\ufe0f\ufe20-\ufe26\ufe33\ufe34\ufe4d-\ufe4f\uff10-\uff19\uff3f]*$");

            if (!isMatch.Success)
            {
                name = "_" + name;
                throw new ArgumentOutOfRangeException();
            }
            return name;
        }
        string GetEnumElem(string sEnumName, string sElemName)
        {
            return GetEnumClassName(sEnumName) + '.' + GetEnumElemName(sElemName);
        }
    }
}
