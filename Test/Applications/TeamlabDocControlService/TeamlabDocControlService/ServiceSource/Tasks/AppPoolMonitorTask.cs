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
using System.Threading;
using System.Net;
using System.IO;
using System.Xml;

namespace TeamlabDocControlService.ServiceSource.Monitors
{

    class AppPoolMonitorTask: ServiceTask
    {
        string mUrl = "";

        public AppPoolMonitorTask(Logger logger)
            : base (logger)
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
                mUrl = "";
                xmlDoc.LoadXml (mStartParams);
                XmlNodeList urlElements = xmlDoc.GetElementsByTagName("url");
                if (null == urlElements || 0 == urlElements.Count)
                    throw new Exception("Request has no element url");
                mUrl = urlElements[0].InnerText;
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
            String sUrl = mUrl;
            while (true)
            {
                while (NeedToPause())
                {
                    Thread.Sleep(1000);
                }
                if (NeedToStop())
                    break;

                if (mUrl.Length > 0)
                {
                    TError value = checkGetError(sUrl, mLogger);
                    DateTime time = DateTime.Now;
                    SetResult(value.ToString());
                }

                Thread.Sleep(60 * 1000);
            }
        }

        private TError checkGetError(string url, Logger log)
        {
            TError bResult = TError.TE_NONE;

            string sURL = url;

            try
            {
                HttpWebRequest wrGETURL;
                wrGETURL = ((HttpWebRequest)HttpWebRequest.Create(sURL));

                Stream objStream;

                HttpWebResponse responce = ((HttpWebResponse)wrGETURL.GetResponse());
                objStream = responce.GetResponseStream();
                responce.Close();
            }
            catch (System.Net.WebException ex)
            {
                if (ex.Status == WebExceptionStatus.Timeout)
                {
                    bResult = TError.TE_TIMEOUT;
                }
                else if (ex.Status == WebExceptionStatus.ProtocolError)
                {
                    HttpStatusCode statusCode = ((HttpWebResponse)(ex.Response)).StatusCode;
                    switch (statusCode)
                    {
                        case HttpStatusCode.ServiceUnavailable:
                            bResult = TError.TE_503;
                            break;
                        case HttpStatusCode.NotFound:
                            bResult = TError.TE_404;
                            break;
                    }
                }

                if (null != log)
                    log.LogError("AppPoolMonitorTimer.checkGetError(): GET " + sURL + " ... FAILED: WebException: " + ex.Message + "; WebExcepttionStatus = " + ex.Status.ToString());

                if (TError.TE_503 == bResult)
                {
                    if (null != log)
                        log.LogError("*** Error 503  - service temporarily unavailable ***");
                }
                if (TError.TE_TIMEOUT == bResult)
                {
                    if (null != log)
                        log.LogError("*** Error TimeOut - no response ***");
                }
            }
            catch (Exception ex)
            {
                if (null != log)
                    log.LogError("AppPoolMonitorTimer.checkGetError(): GET " + sURL + " ... Exception: " + ex.Message);
            }

            return bResult;
        }
    }
}
