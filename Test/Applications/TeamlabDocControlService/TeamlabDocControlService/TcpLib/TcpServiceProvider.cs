using System;
using System.Text;
using TcpLib;

namespace TeamlabDocControlService.TcpServer
{
    public delegate string ControlTcpRequestEventHandler (string requestXml);

	/// <SUMMARY>
    /// ControlServiceProvider. It handles requests from client aspx code and returns a xml to it (realizes IPC mechanism)
	/// </SUMMARY>
	public class ServiceProvider: TcpServiceProvider
	{
        public event ControlTcpRequestEventHandler Requested;

		private string _receivedStr;

		public override object Clone()
		{
            ServiceProvider provider = new ServiceProvider();
            if (null != Requested)
            {
                provider.Requested = Requested.Clone() as ControlTcpRequestEventHandler;
            }

            return provider;
		}

		public override void OnAcceptConnection(ConnectionState state)
		{
			_receivedStr = "";
			if(!state.Write(Encoding.UTF8.GetBytes("TLDCS_HELLO\r\n"), 0, 13))
				state.EndConnection(); //if write fails... then close connection
		}


		public override void OnReceiveData(ConnectionState state)
		{
            
			while(state.AvailableData > 0)
			{
                byte[] buffer = new byte[state.AvailableData];
                int readBytes = state.Read(buffer, 0, state.AvailableData);
                if (readBytes > 0)
                {
                    _receivedStr += Encoding.UTF8.GetString(buffer, 0, readBytes);
                }
                else
                {
                    state.EndConnection(); //If read fails then close connection
                    return;
                }
			}
            
            //
            if (_receivedStr.Length > 0)
            {
                // parse string ()
                string sRequest = _receivedStr;
                _receivedStr = "";

                string sResponse = "";
                if (null != Requested)
                    sResponse = Requested(sRequest);

                if (sResponse.Length > 0)
                {
                    // send response
                    state.Write(Encoding.UTF8.GetBytes(sResponse), 0, sResponse.Length);
                }                                
            }
		}


		public override void OnDropConnection(ConnectionState state)
		{
			//Nothing to clean here
		}
	}
}
