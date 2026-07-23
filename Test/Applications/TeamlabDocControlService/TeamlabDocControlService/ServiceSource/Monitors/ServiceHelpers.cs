/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    class ServiceHelpers
    {
        public static string ExtractRequestParams(string request)
        {
         /* Request syntax:
         * 
         * <tdcs>
         *  <type>memory-monitor</type>
         *  <params>
         *   ...
         *  </params>
         * </tdcs>
         * 
         */
            string param = "";
            string error = "";
            XmlDocument xmlDoc = new XmlDocument(); //* create an xml document object.

            try
            {
                xmlDoc.LoadXml(request);
                XmlNodeList paramElement = xmlDoc.GetElementsByTagName("params");
                if (null == paramElement || (0 == paramElement.Count))
                    throw new Exception ("Request has no element params");
                param = paramElement[0].InnerXml;               

            }
            catch (XmlException ex)
            {
                error = ex.Message;
            }
            catch (Exception ex)
            {
                error = ex.Message;
            }
            if (error.Length > 0)
            {
                param = "<params><error>" + error + "</error></params>";
            }

            return param;
        }
        public static double CalculateFolderSize(string folder, Logger log)
        {
            double folderSize = 0.0f;
            try
            {
                //Checks if the path is valid or not         
                if (!Directory.Exists(folder))
                {
                    return folderSize;
                }
                else
                {
                    try
                    {
                        foreach (string file in Directory.GetFiles(folder))
                        {
                            if (File.Exists(file))
                            {
                                FileInfo finfo = new FileInfo(file);
                                folderSize += finfo.Length;
                            }
                        }
                        foreach (string dir in Directory.GetDirectories(folder))
                        {
                            folderSize += CalculateFolderSize(dir, log);
                        }
                    }
                    catch (NotSupportedException ex)
                    {
                        log.LogError(ex.ToString());
                    }
                }
            }
            catch (UnauthorizedAccessException ex)
            {
                log.LogError(ex.ToString());
            }
            return folderSize;
        }
        public static bool EmptyFolder(string folder, Logger log)
        {
            bool bResult = true;
            DirectoryInfo directory = new DirectoryInfo (folder);
            try
            {
                foreach (System.IO.FileInfo file in directory.GetFiles()) 
                    file.Delete();
                foreach (System.IO.DirectoryInfo subDirectory in directory.GetDirectories()) 
                    subDirectory.Delete(true);
            }
            catch (Exception ex)
            {
                bResult = false;
                if (null != log)
                    log.LogError(ex.ToString());
            }
            return bResult;
        }
    }
}
