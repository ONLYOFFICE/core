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
using System.Text;
using TcpLib;

namespace TeamlabDocControlService.TcpServer
{
    public delegate string ControlTcpRequestEventHandler (string requestXml);

	/// <SUMMARY>
    /// ControlServiceProvider. It handles requests from client aspx code and returns a xml to it (realizes IPC mechanism)
	/// </SUMMARY>
	public class ServiceProvider: TcpServiceProvider
	{
        public event ControlTcpRequestEventHandler Requested;

		private string _receivedStr;

		public override object Clone()
		{
            ServiceProvider provider = new ServiceProvider();
            if (null != Requested)
            {
                provider.Requested = Requested.Clone() as ControlTcpRequestEventHandler;
            }

            return provider;
		}

		public override void OnAcceptConnection(ConnectionState state)
		{
			_receivedStr = "";
			if(!state.Write(Encoding.UTF8.GetBytes("TLDCS_HELLO\r\n"), 0, 13))
				state.EndConnection(); //if write fails... then close connection
		}


		public override void OnReceiveData(ConnectionState state)
		{
            
			while(state.AvailableData > 0)
			{
                byte[] buffer = new byte[state.AvailableData];
                int readBytes = state.Read(buffer, 0, state.AvailableData);
                if (readBytes > 0)
                {
                    _receivedStr += Encoding.UTF8.GetString(buffer, 0, readBytes);
                }
                else
                {
                    state.EndConnection(); //If read fails then close connection
                    return;
                }
			}
            
            //
            if (_receivedStr.Length > 0)
            {
                // parse string ()
                string sRequest = _receivedStr;
                _receivedStr = "";

                string sResponse = "";
                if (null != Requested)
                    sResponse = Requested(sRequest);

                if (sResponse.Length > 0)
                {
                    // send response
                    state.Write(Encoding.UTF8.GetBytes(sResponse), 0, sResponse.Length);
                }                                
            }
		}


		public override void OnDropConnection(ConnectionState state)
		{
			//Nothing to clean here
		}
	}
}
