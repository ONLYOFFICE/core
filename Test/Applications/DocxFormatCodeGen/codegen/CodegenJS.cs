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

namespace codegen
{
    class CodegenJS
    {
        Dictionary<string, GenClass> m_mapProcessedClasses = new Dictionary<string, GenClass>();
        public void Process(string sOutputDir, List<GenClass> aGenClasses)
        {
            StringBuilder oJsSer = new StringBuilder();
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
            string sFileJs = "SerializeChart.js";
            oJsSer.AppendFormat("\"use strict\";\r\n");
            oJsSer.AppendFormat(Utils.gc_sFilePrefix);
            
            //enums
            ProcessEnums(oJsSer, aEnums);
            //ser enums
            ProcessSerEnums(oJsSer, aClasses);
            //ToBin
            oJsSer.AppendFormat("function {0}(memory){{\r\n", Utils.gc_sBinaryChartWriter);
            oJsSer.AppendFormat("this.memory = memory;\r\n");
            oJsSer.AppendFormat("this.bs = new BinaryCommonWriter(this.memory);\r\n");
            oJsSer.AppendFormat("}}\r\n");
            for (int i = 0; i < aClasses.Count; ++i)
                ProcessToBin(oJsSer, aClasses[i]);
            //FromBin
            oJsSer.AppendFormat("function {0}(stream){{\r\n", Utils.gc_sBinaryChartReader);
            oJsSer.AppendFormat("this.stream = stream;\r\n");
            oJsSer.AppendFormat("this.bcr = new Binary_CommonReader(this.stream);\r\n");
            oJsSer.AppendFormat("}}\r\n");
            for (int i = 0; i < aClasses.Count; ++i)
                ProcessFromBin(oJsSer, aClasses[i]);
            File.WriteAllText(Path.Combine(sOutputDir, sFileJs), oJsSer.ToString());
        }
        public void ProcessEnums(StringBuilder sb, List<GenClass> aGenClasses)
        {
            for (int i = 0; i < aGenClasses.Count; ++i)
            {
                GenClass oGenClass = aGenClasses[i];
                for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                {
                    GenMember oGenMember = oGenClass.aMembers[j];
                    if (!oGenMember.bInternal)
                        sb.AppendFormat("var {0} = {1};\r\n", Utils.GetEnumElemName(oGenClass.sName, oGenMember.sName), j);
                }
                sb.AppendFormat("\r\n");
            }
        }
        public void ProcessSerEnums(StringBuilder sb, List<GenClass> aGenClasses)
        {
            for (int i = 0; i < aGenClasses.Count; ++i)
            {
                GenClass oGenClass = aGenClasses[i];
                int nCounter = 0;
                for (int j = 0; j < oGenClass.aMembers.Count; ++j)
                {
                    GenMember oGenMember = oGenClass.aMembers[j];
                    if (!oGenMember.bInternal)
                    {
                        sb.AppendFormat("var {0} = {1};\r\n", Utils.GetSerEnumElemName(oGenClass.sName, oGenMember.sName), nCounter++);
                        if (null != oGenMember.aArrayTypes)
                        {
                            for (int k = 0; k < oGenMember.aArrayTypes.Count; ++k)
                            {
                                sb.AppendFormat("var {0} = {1};\r\n", Utils.GetSerEnumElemName(oGenClass.sName, oGenMember.aArrayTypes[k].sName), nCounter++);
                            }
                        }
                    }
                }
                sb.AppendFormat("\r\n");
            }
        }
        public void ProcessFromBin(StringBuilder sb, GenClass oGenClass)
        {
            if (oGenClass.bIsRoot)
            {
                sb.AppendFormat("{0}.prototype.ExternalRead{1} = function(length, val){{\r\n", Utils.gc_sBinaryChartReader, oGenClass.sName);
                sb.AppendFormat("var res = c_oSerConstants.ReadOk;\r\n");
                sb.AppendFormat("var oThis = this;\r\n");
                sb.AppendFormat("res = this.bcr.Read1(length, function(t, l){{\r\n");
                sb.AppendFormat("return oThis.Read{0}(t, l, val);\r\n", oGenClass.sName);
                sb.AppendFormat("}});\r\n");
                sb.AppendFormat("return res;\r\n");
                sb.AppendFormat("}}\r\n");
            }
            sb.AppendFormat("{0}.prototype.Read{1} = function(type, length, val){{\r\n", Utils.gc_sBinaryChartReader, oGenClass.sName);
            sb.AppendFormat("var res = c_oSerConstants.ReadOk;\r\n");
            sb.AppendFormat("var oThis = this;\r\n");
            int nCounter = 0;
            for (int j = 0; j < oGenClass.aMembers.Count; ++j)
            {
                GenMember oGenMember = oGenClass.aMembers[j];
                if (!oGenMember.bInternal)
                {
                    if (null != oGenMember.aArrayTypes)
                    {
                        for (int k = 0; k < oGenMember.aArrayTypes.Count; ++k)
                        {
                            nCounter = ProcessMemberFromBin(sb, "val", oGenClass, oGenMember.aArrayTypes[k], oGenMember, nCounter);
                        }
                    }
                    else
                    {
                        nCounter = ProcessMemberFromBin(sb, "val", oGenClass, oGenMember, null, nCounter);
                    }
                }
            }
            sb.AppendFormat("else\r\nres = c_oSerConstants.ReadUnknown;\r\nreturn res;\r\n}}\r\n", oGenClass.sName);
        }
        int ProcessMemberFromBin(StringBuilder sb, string sVal, GenClass oGenClass, GenMember oGenMember, GenMember oGenMemberContainer, int nCounter)
        {
            if (0 != nCounter)
                sb.AppendFormat("else ");
            sb.AppendFormat("if({0} === type){{\r\n", Utils.GetSerEnumElemName(oGenClass.sName, oGenMember.sName));
            if (oGenMember.bToDo || !CanProcessToDoString(oGenMember))
                sb.AppendFormat("//todo\r\n");
            sb.AppendFormat("var oNewVal;\r\n");
            if (!ProcessFromBinToDo(sb, oGenMember, "oNewVal"))
            {
                if (null != oGenMember.sType)
                {
                    GenClass oTempClass;
                    if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTempClass))
                    {
                        if (oTempClass.bIsEnum)
                        {
                            sb.AppendFormat("oNewVal = {0};\r\n", ProcessJsTypeFromBin(sb, typeof(byte)));
                        }
                        else
                        {
                            sb.AppendFormat("oNewVal = {{}};\r\n");
                            sb.AppendFormat("res = this.bcr.Read1(length, function(t, l){{\r\n");
                            sb.AppendFormat("return oThis.Read{0}(t, l, oNewVal);\r\n", oTempClass.sName);
                            sb.AppendFormat("}});\r\n");
                        }
                    }
                }
                else if (null != oGenMember.oSystemType)
                {
                    sb.AppendFormat("oNewVal = {0};\r\n", ProcessJsTypeFromBin(sb, oGenMember.oSystemType));
                }
                else
                    sb.AppendFormat("res = c_oSerConstants.ReadUnknown;\r\n");
            }
            if (null != oGenMemberContainer)
            {
                sb.AppendFormat("if(null == {0}.{1})\r\n", sVal, Utils.GetMemberElemName(oGenMemberContainer.sArrayTypesElementName));
                sb.AppendFormat("{0}.{1} = [];\r\n", sVal, Utils.GetMemberElemName(oGenMemberContainer.sArrayTypesElementName));
                sb.AppendFormat("{0}.{1}.push({2});\r\n", sVal, Utils.GetMemberElemName(oGenMemberContainer.sArrayTypesElementName), Utils.GetEnumElemName(oGenMemberContainer.sArrayTypesEnumName, oGenMember.sName));
                sb.AppendFormat("if(null == {0}.{1})\r\n", sVal, Utils.GetMemberElemName(oGenMemberContainer.sName));
                sb.AppendFormat("{0}.{1} = [];\r\n", sVal, Utils.GetMemberElemName(oGenMemberContainer.sName));
                sb.AppendFormat("{0}.{1}.push(oNewVal);\r\n", sVal, Utils.GetMemberElemName(oGenMemberContainer.sName));
            }
            else
            {
                if (oGenMember.bIsArray)
                {
                    sb.AppendFormat("if(null == {0}.{1})\r\n", sVal, Utils.GetMemberElemName(oGenMember.sName));
                    sb.AppendFormat("{0}.{1} = [];\r\n", sVal, Utils.GetMemberElemName(oGenMember.sName));
                    sb.AppendFormat("{0}.{1}.push(oNewVal);\r\n", sVal, Utils.GetMemberElemName(oGenMember.sName));
                }
                else
                    sb.AppendFormat("{0}.{1} = oNewVal;\r\n", sVal, Utils.GetMemberElemName(oGenMember.sName));
            }
            sb.AppendFormat("}}\r\n");
            nCounter++;
            return nCounter;
        }
        bool ProcessFromBinToDo(StringBuilder sb, GenMember oGenMember, string oVal)
        {
            bool bRes = false;
            if (oGenMember.bToDoString)
            {
                bRes = true;
                if ("spPr" == oGenMember.sName)
                {
                    sb.AppendFormat("res = c_oSerConstants.ReadUnknown;\r\n");
                    //sb.AppendFormat("var oPPTXContentLoader = new CPPTXContentLoader();\r\n");
                    //sb.AppendFormat("{0} = oPPTXContentLoader.ReadShapeProperty(this.stream);\r\n", oVal);
                }
                else if ("txPr" == oGenMember.sName || "rich" == oGenMember.sName)
                {
                    sb.AppendFormat("res = c_oSerConstants.ReadUnknown;\r\n");
                    //sb.AppendFormat("{0} = new CTitle();\r\n", oVal);
                    //sb.AppendFormat("var oPPTXContentLoader = new CPPTXContentLoader();\r\n");
                    //sb.AppendFormat("oPPTXContentLoader.ReadTextBody(null, this.stream,  {0});\r\n", oVal);
                }
                else if ("clrMapOvr" == oGenMember.sName)
                {
                    sb.AppendFormat("res = c_oSerConstants.ReadUnknown;\r\n");
                    //sb.AppendFormat("var clrMapOvr = this.ReadClrOverride(length);\r\n");
                }
                else
                    bRes = false;
            }
            return bRes;
        }
        string ProcessJsTypeFromBin(StringBuilder sb, Type oType)
        {
            string sRes;
            //todo
            switch (Type.GetTypeCode(oType))
            {
                case TypeCode.Boolean:
                    sRes = "this.stream.GetBool()";
                    break;
                case TypeCode.Byte:
                case TypeCode.SByte:
                    sRes = "this.stream.GetUChar()";
                    break;
                case TypeCode.Int16:
                case TypeCode.Int32:
                case TypeCode.Int64:
                case TypeCode.UInt16:
                case TypeCode.UInt32:
                case TypeCode.UInt64:
                    sRes = "this.stream.GetULongLE()";
                    break;
                case TypeCode.Single:
                case TypeCode.Double:
                    sRes = "this.stream.GetDoubleLE()";
                    break;
                default: sRes = "this.stream.GetString2LE(length)"; break;
            }
            return sRes;
        }
        public void ProcessToBin(StringBuilder sb, GenClass oGenClass)
        {
            sb.AppendFormat("{0}.prototype.Write{1} = function(oVal){{\r\n", Utils.gc_sBinaryChartWriter, oGenClass.sName);
            sb.AppendFormat("var oThis = this;\r\n");
            for (int i = 0; i < oGenClass.aMembers.Count; ++i)
            {
                GenMember oGenMember = oGenClass.aMembers[i];
                if (!oGenMember.bInternal)
                {
                    if (oGenMember.bIsArray)
                    {
                        sb.AppendFormat("if(null != oVal.{0})\r\n{{\r\n", Utils.GetMemberElemName(oGenMember.sName));
                        sb.AppendFormat("for(var i = 0, length = oVal.{0}.length; i < length; ++i)\r\n", Utils.GetMemberElemName(oGenMember.sName));
                        sb.AppendFormat("{{\r\n");
                        if (null != oGenMember.aArrayTypes)
                        {
                            sb.AppendFormat("var eType = oVal.{0}[i];\r\n", Utils.GetMemberElemName(oGenMember.sArrayTypesElementName));
                            sb.AppendFormat("this.toBin{0}(eType, oVal.{1}[i]);\r\n", oGenMember.sArrayTypesEnumName, Utils.GetMemberElemName(oGenMember.sName));
                        }
                        else
                        {
                            sb.AppendFormat("var oCurVal = oVal.{0}[i];\r\n", Utils.GetMemberElemName(oGenMember.sName));
                            ProcessMemberToBin(sb, oGenClass, oGenMember, "oCurVal");
                        }
                        sb.AppendFormat("}}\r\n");
                        sb.AppendFormat("}}\r\n");
                    }
                    else
                    {
                        sb.AppendFormat("var oCurVal = oVal.{0};\r\n", Utils.GetMemberElemName(oGenMember.sName));
                        ProcessMemberToBin(sb, oGenClass, oGenMember, "oCurVal");
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
            sb.AppendFormat("{0}.prototype.toBin{1} = function(eType, oVal){{\r\n", Utils.gc_sBinaryChartWriter, oGenMember.sArrayTypesEnumName);
            sb.AppendFormat("var oThis = this;\r\n");
            sb.AppendFormat("switch(eType)\r\n{{\r\n");
            for (int i = 0; i < oGenMember.aArrayTypes.Count; ++i)
            {
                GenMember oTempMember = oGenMember.aArrayTypes[i];
                sb.AppendFormat("case {0}:\r\n{{\r\n", Utils.GetEnumElemName(oGenMember.sArrayTypesEnumName, oTempMember.sName));
                ProcessMemberToBin(sb, oGenClass, oTempMember, "oVal");
                sb.AppendFormat("}}\r\nbreak;\r\n");
            }
            sb.AppendFormat("}}\r\n");
            sb.AppendFormat("}}\r\n");
        }
        void ProcessMemberToBin(StringBuilder sb, GenClass oGenClass, GenMember oGenMember, string oVal)
        {
            sb.AppendFormat("if(null != {0})\r\n", oVal);
            sb.AppendFormat("{{\r\n");
            sb.AppendFormat("this.bs.WriteItem({0}, function(){{\r\n", Utils.GetSerEnumElemName(oGenClass.sName, oGenMember.sName));
            if (oGenMember.bToDo || !CanProcessToDoString(oGenMember))
                sb.AppendFormat("//todo\r\n");
            if (!ProcessToBinToDo(sb, oGenMember, oVal))
            {
                if (null != oGenMember.sType)
                {
                    GenClass oTemp;
                    if (m_mapProcessedClasses.TryGetValue(oGenMember.sType, out oTemp))
                    {
                        if (oTemp.bIsEnum)
                            ProcessJsTypeToBin(sb, oVal, typeof(byte));
                        else
                            sb.AppendFormat("oThis.Write{0}({1});\r\n", oGenMember.sType, oVal);
                    }
                }
                else if (null != oGenMember.oSystemType)
                {
                    ProcessJsTypeToBin(sb, oVal, oGenMember.oSystemType);
                }
            }
            sb.AppendFormat("}});\r\n", Utils.GetSerEnumElemName(oGenClass.sName, oGenMember.sName));
            sb.AppendFormat("}}\r\n");
        }
        bool ProcessToBinToDo(StringBuilder sb, GenMember oGenMember, string oVal)
        {
            bool bRes = false;
            if (oGenMember.bToDoString)
            {
                bRes = true;
                if ("spPr" == oGenMember.sName)
                    sb.AppendFormat("window.global_pptx_content_writer.WriteSpPr(oThis.memory, {0});\r\n", oVal);
                else if ("txPr" == oGenMember.sName || "rich" == oGenMember.sName)
                    sb.AppendFormat("window.global_pptx_content_writer.WriteTextBody(oThis.memory, {0});\r\n", oVal);
                else if ("clrMapOvr" == oGenMember.sName)
                    sb.AppendFormat("//todo;\r\n", oVal);
                else
                    bRes = false;
            }
            return bRes;
        }
        void ProcessJsTypeToBin(StringBuilder sb, string sCodeMember, Type oType)
        {
            //todo
            switch (Type.GetTypeCode(oType))
            {
                case TypeCode.Boolean:
                    sb.AppendFormat("oThis.memory.WriteBool({0});\r\n", sCodeMember);
                    break;
                case TypeCode.Byte:
                case TypeCode.SByte:
                    sb.AppendFormat("oThis.memory.WriteByte({0});\r\n", sCodeMember);
                    break;
                case TypeCode.Int16:
                case TypeCode.Int32:
                case TypeCode.Int64:
                case TypeCode.UInt16:
                case TypeCode.UInt32:
                case TypeCode.UInt64:
                    sb.AppendFormat("oThis.memory.WriteLong({0});\r\n", sCodeMember);
                    break;
                case TypeCode.Single:
                case TypeCode.Double:
                    sb.AppendFormat("oThis.memory.WriteDouble2({0});\r\n", sCodeMember);
                    break;
                default: sb.AppendFormat("oThis.memory.WriteString3({0});\r\n", sCodeMember); break;
            }
        }
        bool CanProcessToDoString(GenMember oGenMember)
        {
            bool bRes = true;
            if (oGenMember.bToDoString)
            {
                bRes = false;
                if ("spPr" == oGenMember.sName || "clrMapOvr" == oGenMember.sName || "txPr" == oGenMember.sName || "rich" == oGenMember.sName)
                    bRes = true;
            }
            return bRes;
        }
    }
}
