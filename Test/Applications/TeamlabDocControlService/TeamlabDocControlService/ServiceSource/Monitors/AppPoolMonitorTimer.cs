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
using System.Net;
using System.IO;

namespace TeamlabDocControlService.ServiceSource.Monitors
{
    public enum TError { TE_NONE, TE_503, TE_404, TE_TIMEOUT };
    public delegate void TeamlabAppPoolTimerEventHandler(object sender, TError error, DateTime time);

    class AppPoolMonitorTimer: MonitorByTimer
    {
        private string mUrl; 

        public event TeamlabAppPoolTimerEventHandler OnMeasure;

        public AppPoolMonitorTimer(string url, double interval, Logger logger)
            : base (interval, logger)
        {
            mUrl = url;
        }
        protected override void OnTimer(object state)
        {
            TError error = checkGetError (mUrl, mLogger);
            OnMeasure(this, error, DateTime.Now);
        }

        private TError checkGetError (string url, Logger log)
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

                log.LogError("AppPoolMonitorTimer.checkGetError(): GET " + sURL + " ... FAILED: WebException: " + ex.Message + "; WebExcepttionStatus = " + ex.Status.ToString());

                if (TError.TE_503 == bResult)
                   log.LogError ("*** Error 503  - service temporarily unavailable ***");
                if (TError.TE_TIMEOUT == bResult)
                   log.LogError ("*** Error TimeOut - no response ***");
            }
            catch (Exception ex)
            {
                log.LogError("AppPoolMonitorTimer.checkGetError(): GET " + sURL + " ... Exception: " + ex.Message);
            }

            return bResult;
        }
    }
}
