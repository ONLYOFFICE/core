using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;
using System.Net;
using System.IO;
using System.DirectoryServices;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections;
using System.Xml;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    class RecycleAppPoolTask : ServiceTask
    {
        private string mPath = "";

        public RecycleAppPoolTask (Logger log)
            : base (log)
        {
        }

        public override TaskType GetTaskType()
        {
            return TaskType.Sync;
        }

        public override void SetParams(String parameters)
        {
            base.SetParams(parameters);

            String error = "";
            XmlDocument xmlDoc = new XmlDocument();
            try
            {
                mPath = "";
                xmlDoc.LoadXml(mStartParams);
                XmlNodeList pathElements = xmlDoc.GetElementsByTagName("path");
                if (null != pathElements || 0 != pathElements.Count)
                {
                    if (pathElements[0].InnerText.Length > 0)
                        mPath = pathElements[0].InnerText;
                }
            }
            catch (XmlException ex)
            {
                error = ex.Message;
            }
            catch (Exception ex)
            {
                error = ex.Message;
            }

            if (error.Length > 0 && null != mLogger)
            {
                mLogger.LogError(error);
            }

        }

        protected override void DoWork()
        {
            try
            {
                if (0 == mPath.Length)
                    mPath = ConfigurationSettings.AppSettings["apppoolname"];

                if (0 == mPath.Length)
                    new Exception("AppPoolState: directory path is empty");

                string strMachineName = System.Environment.MachineName;
                string fullPath = "IIS://" + strMachineName + "/W3SVC/AppPools/" + mPath;

                DirectoryEntry w3svc = new DirectoryEntry(fullPath);
                int intStatus = (int)w3svc.InvokeGet("AppPoolState");
                switch (intStatus)
                {
                    case 2:
                        //"Running";
                        if (null != mLogger)
                            mLogger.LogInformation("AppPoolState: running.");
                        w3svc.Invoke("Recycle", null);
                        break;
                    case 4:
                        //"Stopped";
                        new Exception ("AppPoolState: stopped. Recycling cancelled.");
                        break;
                    default:
                        //"Unknown";
                        String sCode = String.Format("AppPoolState: unknown ({0}). Recycling cancelled.", intStatus);
                        new Exception(sCode);
                        break;
                }
            }
            catch (DirectoryServicesCOMException ex)
            {
                SetResult(ex.Message);
                if (null != mLogger)
                    mLogger.LogError("Recycle DirectoryServicesCOMException:" + ex.Message);
            }
            catch (TargetInvocationException ex)
            {
                SetResult(ex.Message);
                if (null != mLogger)
                    mLogger.LogError("Recycle TargetInvocationException:" + ex.InnerException.Message);
            }
            catch (Exception ex)
            {
                // http://www.mattwrock.com/post/2011/07/26/recycling-an-application-pool-with-c-sharp.aspx
                // !!! enable the Windows feature: IIS Metabase and IIS 6 configuration compatibility
                // Win 7 - "control panel -> Programs and features"
                // Win 2008 server - "server management -> features"
                SetResult(ex.Message);
                if (null != mLogger)
                    mLogger.LogError("Recycle Exception:" + ex.Message);
            }
        }
    }
}
