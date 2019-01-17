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
