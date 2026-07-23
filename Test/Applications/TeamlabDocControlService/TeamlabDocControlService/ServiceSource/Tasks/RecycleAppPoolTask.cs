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
