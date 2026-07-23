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
