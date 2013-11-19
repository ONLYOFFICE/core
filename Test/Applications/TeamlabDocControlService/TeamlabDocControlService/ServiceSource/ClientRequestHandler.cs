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
