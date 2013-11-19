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
