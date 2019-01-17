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
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.ServiceProcess;
using System.Text;
using System.IO;
using System.Threading;

using TeamlabDocControlService.ServiceSource;
using TeamlabDocControlService.TcpServer;
using TeamlabDocControlService.ServiceSource.Monitors;
using System.Configuration;

namespace TeamlabDocControlService
{
    
    public partial class TeamlabDocService : ServiceBase
    {

        public static ServiceSharedObject gServiceObject;
        private ServiceProvider mCommService;
        private TcpLib.TcpServer mServer;

        private SettingsReader mSettingsReader;
        private Logger mLogger;

        private Thread mThread = null;
        private volatile bool mThreadShouldStop = false;

        private TaskManager mTaskManager = null;
        private ClientRequestHandler mRequestHandler = null;


        [Conditional("DEBUG_SERVICE")]
        private static void DebugMode()
        {
            Debugger.Break();
        }

        public TeamlabDocService()
        {
            InitializeComponent();

            if (null != gServiceObject)
                gServiceObject = new ServiceSharedObject (this);
        }

        private void ThreadProc(object obj)
        {
            //TeamlabDocService service = (TeamlabDocService) obj;

            // load configuration

            // start task manager
            mTaskManager = new TaskManager();
            mRequestHandler = new ClientRequestHandler(mTaskManager);

            // start monitors
            int nPort = Int32.Parse  ( ConfigurationSettings.AppSettings["port"] ); // tcp port number - loaded from configuration

            // start ipc server
            ServiceProvider commService = new ServiceProvider();
            commService.Requested += new ControlTcpRequestEventHandler(OnRequest);
            TcpLib.TcpServer server = new TcpLib.TcpServer(commService, ConfigurationSettings.AppSettings["ip"], nPort);
            //Debugger.Launch();
            server.Start();

            // todo: use event/mutex/monitor
            while (!mThreadShouldStop)
            {
                // do work
                Thread.Sleep(1);
            }
            server.Stop();

            mTaskManager.RemoveAllTasks();
        }

        protected string OnRequest (string request)
        {
            string sResponse = request;

            if (null != mRequestHandler)
            {
                sResponse = mRequestHandler.OnRequest(request);
            }
            else
            { 

            }
            // temp
            return sResponse;
             // parse request

        }
        public void debugStart()
        {
            OnStart(null);
        }
        protected override void OnStart(string[] args)
        {
            //DebugMode();

            mThread = new Thread (new ParameterizedThreadStart(ThreadProc));
            mThreadShouldStop = false;
            mThread.Start(this);
            //gServiceObject.OnStart();

            /*
            string serviceName = System.Reflection.Assembly.GetExecutingAssembly().Location;
            string servicePath = System.IO.Path.GetDirectoryName(serviceName);

            // get settings or set default settings
            mSettingsReader = new SettingsReader(serviceName + ".config");
            string xml = mSettingsReader.Read();

            mSettingsReader.Changed += new TeamlabSettingsEventHandler(OnSettingsChanged);
            
            // create logger
            string tempPath = Path.GetTempPath();
            tempPath += "\\TeamlabService";

            try
            {
                if (!Directory.Exists(tempPath))
                    Directory.CreateDirectory(tempPath);
            }
            catch (Exception ex)
            {
                Trace.TraceError("Can't create folder for logging ({1})", ex.Message);
            }
            
            Logger mLogger = new Logger(tempPath, "log");

            // create monitors (configuration, filesystem, processor usage, licensing server, user counter & other)
            // create task manager (clear database, clear folder, enter key)
            // 
            // 
            // start 
             * */
        }

        protected override void OnStop()
        {
            mThreadShouldStop = true;

            // Wait until oThread finishes. Join also has overloads
            // that take a millisecond interval or a TimeSpan object.
            mThread.Join();
            mThread = null;


            /*
            gServiceObject.OnStop();
             */
            
            // stop monotoring
            //mSettingsReader.Stop ();

        }

        // event handler for event recieved from settingsReader
        protected void OnSettingsChanged (object sender, string xmlSettings)
        {
            // TODO: apply settings
        }
    }
}
