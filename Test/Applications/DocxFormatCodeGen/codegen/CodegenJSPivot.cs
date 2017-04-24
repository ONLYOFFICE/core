/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

namespace codegen
{
    class CodegenJSXml
    {
        Dictionary<string, GenClassPivot> m_mapProcessedClasses = new Dictionary<string, GenClassPivot>();
        string gc_sEnumFromXmlPrefix = "FromXml_";
        string gc_sEnumToXmlPrefix = "ToXml_";
        string gc_sNamespaceToXml = " xmlns=\\\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\\\" xmlns:r=\\\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\\\"";

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
                for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                {
                    GenMemberPivot oGenMember = oGenClass.aMembers[j];
                    sb.AppendFormat("var {0} = {1};\r\n", Utils.GetEnumElemName(oGenClass.sName, oGenMember.sName), j);
                }
                sb.AppendFormat("\r\n");
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
                        sb.AppendFormat("if(\"{0}\"===val)\r\nres={1};\r\n", member.sName, Utils.GetEnumElemName(oGenClass.sName, member.sName));
                    else
                        sb.AppendFormat("else if(\"{0}\"===val)\r\nres={1};\r\n", member.sName, Utils.GetEnumElemName(oGenClass.sName, member.sName));
                }
                sb.AppendFormat("return res;\r\n}}\r\n");
                sb.AppendFormat("function {0}{1}(val)\r\n{{\r\n", gc_sEnumToXmlPrefix, oGenClass.sName);
                sb.AppendFormat("var res = \"\";\r\n");
                for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                {
                    GenMemberPivot member = oGenClass.aMembers[j];
                    if (0 == j)
                        sb.AppendFormat("if({0}===val)\r\nres=\"{1}\";\r\n", Utils.GetEnumElemName(oGenClass.sName, member.sName), member.sName);
                    else
                        sb.AppendFormat("else if({0}===val)\r\nres=\"{1}\";\r\n", Utils.GetEnumElemName(oGenClass.sName, member.sName), member.sName);
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
            sb.AppendFormat("}}\r\n", oGenClass.sName);

            if (aAttributes.Count > 0)
            {
                sb.AppendFormat("{0}.prototype.readAttributes = function(attr, uq) {{\r\n", oGenClass.sName);
                ProcessAttributesFromXml(sb, oGenClass, aAttributes);
                sb.AppendFormat("}};\r\n", oGenClass.sName);
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
                sb.AppendFormat("}};\r\n", oGenClass.sName);

                if (bNeedTextNode)
                {
                    sb.AppendFormat("{0}.prototype.onTextNode = function(text, uq) {{\r\n", oGenClass.sName);
                    ProcessOnTextNodeFromXml(sb, oGenClass, aMembers);
                    sb.AppendFormat("}};\r\n", oGenClass.sName);
                }
                if (bNeedDoubleArray)
                {
                    sb.AppendFormat("{0}.prototype.onEndNode = function(prevContext, elem) {{\r\n", oGenClass.sName);
                    ProcessOnEndNodeFromXml(sb, oGenClass, aMembers);
                    sb.AppendFormat("}};\r\n", oGenClass.sName);
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
                sb.AppendFormat("var res = \"\";\r\n");
                ProcessToXml(sb, oGenClass);
                sb.AppendFormat("return res;\r\n");
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
                        sb.AppendFormat("this.{0} = null;//{1}\r\n", oGenMember.sName, oGenMember.sDefAttribute.ToLower());
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
                                    sb.AppendFormat("this.{0} = null;//{1}\r\n", oTempMember.sName, oTempMember.sDefAttribute.ToLower());
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
                                sb.AppendFormat("this.{0} = null;//{1}\r\n", oGenMember.sName, oGenMember.sDefAttribute.ToLower());
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
                sb.AppendFormat("newContext.readAttributes(attr, uq);\r\n", sName, sCodeName);
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
                        }
                        sb.AppendFormat("this.{0} = [];\r\n", oGenMember.sName);
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
                    string sCodeElem;
                    if (oGenMember.nArrayRank > 0)
                    {
                        if (false == oGenMember.bIsArrayTypesHidden)
                        {
                            sb.AppendFormat("if(null !== {0}){{\r\n", oGenMember.sName);
                        }
                        sb.AppendFormat("for(var i = 0; i < this.{0}.length; ++i){{\r\n", oGenMember.sName);
                        sb.AppendFormat("var elem = this.{0}[i];\r\n", oGenMember.sName);
                        sCodeElem = "elem";
                        bNullCheck = false;
                    }
                    else
                    {
                        sCodeElem = "this." + oGenMember.sName;
                    }
                    if (null != oGenMember.aArrayTypes)
                    {
                        if (false == oGenMember.bIsArrayTypesHidden)
                        {
                            if (bNullCheck)
                                sb.AppendFormat("if(null !== {0}){{\r\n", sCodeElem);
                            sb.AppendFormat("writer.WriteXmlNodeEnd(\"{0}\", true);\r\n", getNameWithPrefix(oGenClass, oGenMember));
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
                            sb.AppendFormat("}}\r\n", oGenMember.sName);
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
                sb.AppendFormat("if(null !== {0}){{\r\n", sElemName);
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
                            sb.AppendFormat("res += {0}.toXml(writer, \"{1}\");\r\n", sElemName, sElemXmlName);
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
                        sb.AppendFormat("writer.WriteXmlAttributeString(\"{0}\", {1});\r\n", sName, sVal);
                    }
                    else
                    {
                        sb.AppendFormat("writer.WriteXmlNodeStart(\"{0}\", true);\r\n", sName);
                        sb.AppendFormat("writer.WriteXmlString({0});\r\n", sVal);
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
    }
}
