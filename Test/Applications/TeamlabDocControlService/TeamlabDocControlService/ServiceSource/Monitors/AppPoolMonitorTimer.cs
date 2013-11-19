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
