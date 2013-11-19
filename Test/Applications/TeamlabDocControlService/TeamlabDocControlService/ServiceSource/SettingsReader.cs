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
