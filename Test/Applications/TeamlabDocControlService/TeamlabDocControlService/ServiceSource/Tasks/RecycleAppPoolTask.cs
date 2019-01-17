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
