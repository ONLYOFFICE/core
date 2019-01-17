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
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;

using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Xml;

public partial class _Default : System.Web.UI.Page 
{
    private Socket server = null;
    private string sServer = "127.0.0.1";
    private string sPort = "15555";
    
    // connect
    private void connect ()
    {
        try
        {
            IPEndPoint ipep = new IPEndPoint(IPAddress.Parse(sServer), Int32.Parse(sPort));
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            server.Connect(ipep);
        }
        catch (SocketException e1)
        {
            lblStatusResult.Text = e1.Message;
            return;
        }
        catch (Exception e2)
        {
            lblStatusResult.Text = e2.Message;
            return;
        }

        string sHello = receive (server);
        if (!sHello.Equals("TLDCS_HELLO\r\n"))
        {
            lblStatusResult.Text = "no hello msg from service";
        }
        else
        {
            lblStatusResult.Text = "connected";
        }
    }
    private string receive(Socket server)
    {
        string recvStr = "";
        byte[] data = new byte[1024];
        try
        {
            while (true)
            {
                int recv = server.Receive(data);
                if (recv > 0)
                {
                    recvStr += Encoding.UTF8.GetString(data, 0, recv);
                }
                else
                    throw new Exception("Receive: available bytes more than was read");

                if (0 == server.Available)
                    break;
            }
        }
        catch (Exception e)
        {
            lblStatusResult.Text = e.Message;
        }
        return recvStr;
    }
    bool send(string msg)
    {
        bool res = true;
        try
        {
            server.Send(Encoding.UTF8.GetBytes(msg));
        }
        catch (Exception err)
        {
            res = false;
            lblStatusResult.Text = err.Message;
        }
        return res;
    }

    protected void Page_Load(object sender, EventArgs e)
    {
        connect();
    }

    protected float getProcessorUsageFromResponse(string response)
    {
        string responseString = "";
        string error = "";
        XmlDocument xmlDoc = new XmlDocument(); //* create an xml document object.

        try
        {
            xmlDoc.LoadXml(response);
            //XmlElement rootElement = xmlDoc.GetElementBy ("tdcs");
            XmlNodeList typeElements = xmlDoc.GetElementsByTagName("tresponse");
            if (null == typeElements || 0 == typeElements.Count)
                throw new Exception("Request has no element tresponse");
            responseString = typeElements[0].InnerText;

        }
        catch (XmlException ex)
        {
            error = ex.Message;
            responseString = "invalid_request";
        }
        catch (Exception ex)
        {
            error = ex.Message;
            responseString = "request_handling_error";
        }
        return float.Parse(responseString);
    }
    protected void btnProcessorUsage_Click(object sender, EventArgs e)
    {
        // get processor usage
        // connect to service
        // "<tdcs><type>processor-monitor</type><id>id34567</id><params></params></tdcs>"
        lblProcessorUsage.Text = "Checking..";
        send("<tdcs><type>processor-monitor</type><id>id34567</id><params></params></tdcs>");
        string response = receive(server);
        float perc = getProcessorUsageFromResponse(response);
        lblProcessorUsage.Text = perc.ToString();
    }
    protected void btnStatusCheck_Click(object sender, EventArgs e)
    {
        // get status from 
    }
}
