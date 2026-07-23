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
using System.Diagnostics;

namespace TeamlabDocControlService.ServiceSource
{
    class Logger
    {
        protected string mFilename, mDirectory;
        protected StreamWriter mFile = null;
        protected DateTime mDateTime;

        public bool WriteInformation = true;
        public bool WriteWarning = true;
        public bool WriteError = true;

        // syncronization
        private Object locker = new Object();

        protected void CloseFile()
        {
            if (null != mFile)
            {
                mFile.Flush();
                mFile.Close();
                mFile = null;
            }
        }

        // creates new file if it is not exists or if the last file was created yesterday
        protected bool CreateNewFile()
        {
            bool res = true;
            try
            {
                CloseFile();
                FileStream stream = new FileStream(mDirectory + "\\" + mFilename + mDateTime.Date.ToString() + ".log", FileMode.Append);
                mFile = new StreamWriter(stream);
                res = (null != mFile);
            }
            catch (System.Exception ex)
            {
                Trace.TraceError(ex.Message);
                res = false;
            }
            return res;
        }
        protected bool WriteToFile (string line, string type)
        {
            bool needRecreateFile = false;
            DateTime now = DateTime.Now;
            if ((null == mDateTime) 
                || (0 != DateTime.Compare (mDateTime.Date, now.Date)))
            {
                needRecreateFile = true;
                mDateTime = now;
            }

            if (needRecreateFile || (null == mFile))
            {
               CreateNewFile();
            }
            if (null == mFile)
                return false;


            mFile.WriteLine(mDateTime.TimeOfDay.ToString() + " " + type + " : " + line);
            return true;
        }

        public Logger(string directory, string filename)
        {
            mFilename = filename;
            mDirectory = directory;
        }
        public void LogError (string line)
        {
            if (WriteError)
            {
                lock (locker)
                {
                    WriteToFile(line, "Error");
                }
            }
        }
        public void LogWarning (string line)
        {
            if (WriteWarning)
            {
                lock (locker)
                {
                    WriteToFile(line, "Warning");
                }
            }
        }
        public void LogInformation (string line)
        {
            if (WriteInformation)
            {
                lock (locker)
                {
                    WriteToFile(line, "Info");
                }
            }
        }
    }
}
