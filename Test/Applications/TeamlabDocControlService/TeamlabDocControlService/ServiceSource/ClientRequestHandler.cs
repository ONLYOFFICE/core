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
using System.Xml;
using TeamlabDocControlService.ServiceSource.Monitors;
using TeamlabDocControlService.ServiceSource.RequestResolvers;

namespace TeamlabDocControlService.ServiceSource
{
    class ClientRequestHandler
    {
        private TaskManager mTaskManager;

        // params: logger, data_storage
        public ClientRequestHandler (TaskManager taskManager)
        {
            mTaskManager = taskManager;
        }

        /* Request syntax:
         * 
         * <tdcs>
         *  <type>memory-monitor</type>
         *  <params>
         *   ...
         *  </params>
         * </tdcs>
         * 
         * <tdcs>
         * or
         * <type>status</type><params></params></tdcs>
         */
        public string OnRequest (string request)
        {
            // parse xml
            string typeString = "";
            string response = "";
            string error = "";
            XmlDocument xmlDoc = new XmlDocument(); //* create an xml document object.

            try
            {
                xmlDoc.LoadXml(request);
                //XmlElement rootElement = xmlDoc.GetElementBy ("tdcs");
                XmlNodeList typeElements = xmlDoc.GetElementsByTagName("type");
                if (null == typeElements || 0  == typeElements.Count)
                    throw new Exception("Request has no element type");
                typeString = typeElements[0].InnerText;
                IRequestResolver resolver = mTaskManager.GetRequestResolverByName(typeString);
                if (null == resolver)
                    throw new Exception("Request type has no resolver");

                // resolver
                response = resolver.ProcessRequest(request); // error?

            }
            catch (XmlException ex)
            {
                error = ex.Message;
                request = "invalid_request";
            }
            catch (Exception ex)
            {
                error = ex.Message;
                request = "request_handling_error";
            }

            return BuildResponse(typeString, request, response, error);
        }

        /*
         * Build response from inner request, response and error.
         * 
         * Response syntax:
         * 
         * <tdcs type='response' timestamp='30 oct 2012 GMT+04'>
         *  <trequest>
         *   <tdcs>
         *    <type>memory-monitor</type>
         *    <params>
         *     ...
         *    </params>
         *   </tdcs>
         *  <trequest>
         *  <tresponse>
         *  ...
         *  </tresponse>
         *  <error>
         *  ...
         *  </error>
         * </tdcs>
         */
        // 
        private string BuildResponse (string resolverName, string request, string response, string error)
        {
            XmlDocument xmlDoc = new XmlDocument();
            XmlElement root = (XmlElement) xmlDoc.AppendChild(xmlDoc.CreateElement("tdcs"));
            root.SetAttribute("type", "response");
            root.SetAttribute("timestamp", System.DateTime.Now.ToString("o"));
            root.AppendChild(xmlDoc.CreateElement("trequest")).InnerXml = request;
            root.AppendChild(xmlDoc.CreateElement("tresponse")).InnerXml = response;
            if (error.Length > 0)
                root.AppendChild(xmlDoc.CreateElement("error")).InnerText = error;
            return xmlDoc.OuterXml;
        }
    }
}
