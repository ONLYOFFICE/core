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
