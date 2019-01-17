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
using System.Threading.Tasks;
using System.IO;

namespace Joiner
{
    class Program
    {
        static void Main(string[] args)
        {
            for (int nType = 0; nType < 3; nType++)
            {
                string sPathConfig = "";
                string sDstFile = "";

                string sPathBase = Directory.GetCurrentDirectory() + "/../../../";
                if (true)
                    sPathBase = Directory.GetCurrentDirectory() + "/";

                if (nType == 0)
                {
                    sPathConfig = sPathBase + "sdk_configs/webword.json";
                    sDstFile = sPathBase + "../../word/sdk-all.js";
                }
                else if (nType == 1)
                {
                    sPathConfig = sPathBase + "sdk_configs/webexcel.json";
                    sDstFile = sPathBase + "../../cell/sdk-all.js";
                }
                else
                {
                    sPathConfig = sPathBase + "sdk_configs/webpowerpoint.json";
                    sDstFile = sPathBase + "../../slide/sdk-all.js";
                }

                Dictionary<string, bool> map_files = new Dictionary<string, bool>();

                StreamReader oReader = new StreamReader(sPathConfig);
                string sConfigSource = oReader.ReadToEnd();

                List<int> arCommon = new List<int>();
                List<int> arWord = new List<int>();
                List<int> arPowerPoint = new List<int>();
                List<int> arExcel = new List<int>();

                List<string> arFiles = new List<string>();
                List<int> arFindIndex = new List<int>();

                int nStart = 0;

                nStart = 0;
                while ((nStart = sConfigSource.IndexOf("/common/", nStart + 1)) >= 0)
                {
                    int nFind = sConfigSource.IndexOf('\"', nStart);
                    arCommon.Add(nStart);

                    string sFile = sConfigSource.Substring(nStart, nFind - nStart);
                    if (!map_files.ContainsKey(sFile))
                    {
                        arFiles.Add(sFile);
                        arFindIndex.Add(nStart);
                        map_files.Add(sFile, true);
                    }
                }

                nStart = 0;
                while ((nStart = sConfigSource.IndexOf("/word/", nStart + 1)) >= 0)
                {
                    int nFind = sConfigSource.IndexOf('\"', nStart);
                    arWord.Add(nStart);

                    string sFile = sConfigSource.Substring(nStart, nFind - nStart);
                    if (!map_files.ContainsKey(sFile))
                    {
                        arFiles.Add(sFile);
                        arFindIndex.Add(nStart);
                        map_files.Add(sFile, true);
                    }
                }

                nStart = 0;
                while ((nStart = sConfigSource.IndexOf("/slide/", nStart + 1)) >= 0)
                {
                    int nFind = sConfigSource.IndexOf('\"', nStart);
                    arWord.Add(nStart);

                    string sFile = sConfigSource.Substring(nStart, nFind - nStart);
                    if (!map_files.ContainsKey(sFile))
                    {
                        arFiles.Add(sFile);
                        arFindIndex.Add(nStart);
                        map_files.Add(sFile, true);
                    }
                }

                nStart = 0;
                while ((nStart = sConfigSource.IndexOf("/cell/", nStart + 1)) >= 0)
                {
                    int nFind = sConfigSource.IndexOf('\"', nStart);
                    arExcel.Add(nStart);

                    string sFile = sConfigSource.Substring(nStart, nFind - nStart);
                    if (!map_files.ContainsKey(sFile))
                    {
                        arFiles.Add(sFile);
                        arFindIndex.Add(nStart);
                        map_files.Add(sFile, true);
                    }
                }

                string[] _filesResult = arFiles.ToArray();
                int[] _findResult = arFindIndex.ToArray();

                int nLengthFilesCheck = _filesResult.Length;
                for (int i = 0; i < nLengthFilesCheck; ++i)
                {
                    for (int j = i + 1; j < nLengthFilesCheck; ++j)
                    {
                        if (_findResult[i] > _findResult[j])
                        {
                            int nTmp = _findResult[i];
                            _findResult[i] = _findResult[j];
                            _findResult[j] = nTmp;

                            string sTmp = _filesResult[i];
                            _filesResult[i] = _filesResult[j];
                            _filesResult[j] = sTmp;
                        }
                    }
                }

                StringBuilder oBuilder = new StringBuilder();

                List<string> arConcat = new List<string>();
                for (int i = 0; i < nLengthFilesCheck; i++)
                {
                    string sFileCandidate = _filesResult[i];
                    if ((sFileCandidate.IndexOf("/build/") >= 0) ||
                        (sFileCandidate.IndexOf("/Build/") >= 0) ||
                        (sFileCandidate.IndexOf("3rdparty") >= 0) ||
                        (sFileCandidate.IndexOf("-all.js") >= 0) ||
                        (sFileCandidate.IndexOf("apiExport.js") >= 0) ||
                        (sFileCandidate.LastIndexOf(".js") != (sFileCandidate.Length - 3)))
                        continue;

                    /*
                    if ((sFileCandidate.IndexOf("/api.js") >= 0) ||
                        (sFileCandidate.IndexOf("/apiCommon.js") >= 0))
                    {
                        arConcat.Add(sFileCandidate);
                        continue;
                    }
                    */ 


                    StreamReader oReader2 = new StreamReader(sPathBase + "../.." + _filesResult[i]);
                    oBuilder.Append(oReader2.ReadToEnd());

                    oBuilder.Append("\n\n");
                }

                for (int i = 0; i < arConcat.Count; i++)
                {
                    StreamReader oReader2 = new StreamReader(sPathBase + "../.." + arConcat[i]);
                    oBuilder.Append(oReader2.ReadToEnd());

                    oBuilder.Append("\n\n");
                }

                StreamWriter oWriter = new StreamWriter(sDstFile, false, Encoding.UTF8);

                oWriter.Write(oBuilder.ToString());
                oWriter.Close();
            }
        }
    }
}
