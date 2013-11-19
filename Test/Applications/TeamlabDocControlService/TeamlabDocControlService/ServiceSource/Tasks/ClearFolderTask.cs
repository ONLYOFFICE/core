using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using System.Xml;


namespace TeamlabDocControlService.ServiceSource.Monitors
{
    class ClearFolderTask: ServiceTask
    {
        private string mFolder = "";

        public ClearFolderTask(Logger log)
            : base (log)
        {
        }

        public override TaskType GetTaskType()
        {
            return TaskType.Async;
        }

        public override void SetParams(String parameters)
        {
            base.SetParams(parameters);

            String error = "";
            XmlDocument xmlDoc = new XmlDocument();
            try
            {
                mFolder = "";
                xmlDoc.LoadXml(mStartParams);
                XmlNodeList urlElements = xmlDoc.GetElementsByTagName("folder");
                if (null == urlElements || 0 == urlElements.Count)
                    throw new Exception("Request has no element folder");
                mFolder = urlElements[0].InnerText;
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
            String sFolder = mFolder;
            if (sFolder.Length > 0)
            {
                try
                {
                    bool bRes = ServiceHelpers.EmptyFolder(sFolder, mLogger);
                    if (!bRes)
                        throw new Exception("EmptyFolder() failed. See.");
                    if (null != mLogger)
                        mLogger.LogInformation("ClearFolderTask.ClearFolderTask() succeded");
                }
                catch (Exception ex)
                {
                    string msg = "ClearFolderTask.ClearFolderTask() error ";
                    msg += ex.Message;
                    if (null != mLogger)
                        mLogger.LogError(msg);
                }
            }
        }
    }
}
