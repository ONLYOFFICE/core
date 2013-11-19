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
