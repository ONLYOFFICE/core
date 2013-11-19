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
