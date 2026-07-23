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
