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
using System.Security.Permissions;
using System.IO;
using System.Diagnostics;

namespace TeamlabDocControlService.ServiceSource
{
    public delegate void TeamlabSettingsEventHandler (object sender, string xmlSettings);

    class SettingsReader
    {

        private FileSystemWatcher mWatcher = null;
        private string mFilename;

        // Summary:
        //     Occurs when settings are changed.
        public event TeamlabSettingsEventHandler Changed;

        protected void OnChange (object source, FileSystemEventArgs e)
        {
            string xml = Read();
            Changed(this, xml);
        }

        public string Read()
        {
            string xml = "";
            try
            {
                StreamReader reader = new StreamReader(mFilename);
                xml = reader.ReadToEnd();
                reader.Close();
            }
            catch (Exception ex)
            {
                Trace.TraceError(ex.Message);
            }
            return xml;
        }
        // read service settings
        [PermissionSet(SecurityAction.Demand, Name="FullTrust")]
        public SettingsReader (string filename)
        {
            mFilename = filename;
            mWatcher = new FileSystemWatcher();
            mWatcher.NotifyFilter = NotifyFilters.LastWrite;
            mWatcher.Path = mFilename;
            mWatcher.Changed += new FileSystemEventHandler(OnChange);
        }

        public void Start ()
        {
            // fire event on start
            //OnChange(this, null);
            mWatcher.EnableRaisingEvents = true;
        }
        public void Stop ()
        {
            mWatcher.EnableRaisingEvents = false;
        }

    }
}
