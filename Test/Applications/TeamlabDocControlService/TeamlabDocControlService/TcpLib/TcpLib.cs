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
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Collections;

namespace TcpLib
{
  /// <SUMMARY>
  /// This class holds useful information for keeping track of each client connected
  /// to the server, and provides the means for sending/receiving data to the remote
  /// host.
  /// </SUMMARY>
  public class ConnectionState
  {
    internal Socket _conn;
    internal TcpServer _server;
    internal TcpServiceProvider _provider;
    internal byte[] _buffer;

    /// <SUMMARY>
    /// Tells you the IP Address of the remote host.
    /// </SUMMARY>
    public EndPoint RemoteEndPoint
    {
      get{ return _conn.RemoteEndPoint; }
    }

    /// <SUMMARY>
    /// Returns the number of bytes waiting to be read.
    /// </SUMMARY>
    public int AvailableData
    {
      get{ return _conn.Available; }
    }

    /// <SUMMARY>
    /// Tells you if the socket is connected.
    /// </SUMMARY>
    public bool Connected
    {
      get{ return _conn.Connected; }
    }

    /// <SUMMARY>
    /// Reads data on the socket, returns the number of bytes read.
    /// </SUMMARY>
    public int Read(byte[] buffer, int offset, int count)
    {
      try
      {
        if(_conn.Available > 0)
          return _conn.Receive(buffer, offset, count, SocketFlags.None);
        else return 0;
      }
      catch
      {
        return 0;
      }
    }

    /// <SUMMARY>
    /// Sends Data to the remote host.
    /// </SUMMARY>
    public bool Write(byte[] buffer, int offset, int count)
    {
      try
      {
        _conn.Send(buffer, offset, count, SocketFlags.None);
        return true;
      }
      catch
      {
        return false;
      }
    }


    /// <SUMMARY>
    /// Ends connection with the remote host.
    /// </SUMMARY>
    public void EndConnection()
    {
      if(_conn != null && _conn.Connected)
      {
        _conn.Shutdown(SocketShutdown.Both);
        _conn.Close();
      }
      _server.DropConnection(this);
    }
  }



  /// <SUMMARY>
  /// Allows to provide the server with the actual code that is goint to service
  /// incoming connections.
  /// </SUMMARY>
  public abstract class TcpServiceProvider:ICloneable
  {
    /// <SUMMARY>
    /// Provides a new instance of the object.
    /// </SUMMARY>
    public virtual object Clone()
    {
      throw new Exception("Derived clases must override Clone method.");
    }

    /// <SUMMARY>
    /// Gets executed when the server accepts a new connection.
    /// </SUMMARY>
    public abstract void OnAcceptConnection(ConnectionState state);

    /// <SUMMARY>
    /// Gets executed when the server detects incoming data.
    /// This method is called only if OnAcceptConnection has already finished.
    /// </SUMMARY>
    public abstract void OnReceiveData(ConnectionState state);

    /// <SUMMARY>
    /// Gets executed when the server needs to shutdown the connection.
    /// </SUMMARY>
    public abstract void OnDropConnection(ConnectionState state);
  }



  public class TcpServer
  {
    private int _port;
    private Socket _listener;
    private IPAddress _ip = null;
    private TcpServiceProvider _provider;
    private ArrayList _connections;
    private int _maxConnections = 100;

    private AsyncCallback ConnectionReady;
    private WaitCallback AcceptConnection;
    private AsyncCallback ReceivedDataReady;

    /// <SUMMARY>
    /// Initializes server. To start accepting connections call Start method.
    /// </SUMMARY>
    public TcpServer(TcpServiceProvider provider, String ip, int port)
    {
      _provider = provider;
      _port = port;
      if (0 != ip.Length)
      {
          _ip = IPAddress.Parse (ip);
      }
      else
      {
          IPAddress ipAddress = getIp4Address(Dns.GetHostName());// Dns.GetHostEntry(Dns.GetHostName()).AddressList[0];
          _ip = ipAddress;
      }
      _listener = new Socket(_ip.AddressFamily /*AddressFamily.InterNetwork*/, SocketType.Stream,
        ProtocolType.Tcp);
      _connections = new ArrayList();
      ConnectionReady = new AsyncCallback(ConnectionReady_Handler);
      AcceptConnection = new WaitCallback(AcceptConnection_Handler);
      ReceivedDataReady = new AsyncCallback(ReceivedDataReady_Handler);
    }


      protected IPAddress getIp4Address(string hostname)
      {
          IPAddress[] addrs = Dns.GetHostEntry(hostname).AddressList;
          for (int addrnum = 0; addrnum < addrs.Length; addrnum++)
          {
              if (!addrs[addrnum].IsIPv6LinkLocal)
                  return addrs[addrnum];
          }
          return null;
      }

      /// <SUMMARY>
      /// Start accepting connections.
      /// A false return value tell you that the port is not available.
      /// </SUMMARY>
      public bool Start()
    {
      try
      {
        //_listener.Bind(new IPEndPoint(IPAddress.Any, _port));
          //IPAddress ipaddr = IPAddress.Parse(_ip);
          _listener.Bind(new IPEndPoint(_ip, _port));
        _listener.Listen(100);
        _listener.BeginAccept(ConnectionReady, null);
        return true;
      }
      catch (SocketException ex)
      {
        return false;
      }
    }


    /// <SUMMARY>
    /// Callback function: A new connection is waiting.
    /// </SUMMARY>
    private void ConnectionReady_Handler(IAsyncResult ar)
    {
      lock(this)
      {
        if(_listener == null) return;
        Socket conn = _listener.EndAccept(ar);
        if(_connections.Count >= _maxConnections)
        {
          //Max number of connections reached.
          string msg = "TLDCS_ERROR: 001 Server busy";
          conn.Send(Encoding.UTF8.GetBytes(msg), 0, msg.Length, SocketFlags.None);
          conn.Shutdown(SocketShutdown.Both);
          conn.Close();
        }
        else
        {
          //Start servicing a new connection
          ConnectionState st = new ConnectionState();
          st._conn = conn;
          st._server = this;
          st._provider = (TcpServiceProvider) _provider.Clone();
          st._buffer = new byte[4];
          _connections.Add(st);
          //Queue the rest of the job to be executed latter
          ThreadPool.QueueUserWorkItem(AcceptConnection, st);
        }
        //Resume the listening callback loop
        _listener.BeginAccept(ConnectionReady, null);
      }
    }


    /// <SUMMARY>
    /// Executes OnAcceptConnection method from the service provider.
    /// </SUMMARY>
    private void AcceptConnection_Handler(object state)
    {
      ConnectionState st = state as ConnectionState;
      try{ st._provider.OnAcceptConnection(st); }
      catch {
        //report error in provider... Probably to the EventLog
      }
      //Starts the ReceiveData callback loop
      if(st._conn.Connected)
        st._conn.BeginReceive(st._buffer, 0, 0, SocketFlags.None,
          ReceivedDataReady, st);
    }


    /// <SUMMARY>
    /// Executes OnReceiveData method from the service provider.
    /// </SUMMARY>
    private void ReceivedDataReady_Handler(IAsyncResult ar)
    {
      ConnectionState st = ar.AsyncState as ConnectionState;
      
      //Im considering the following condition as a signal that the
      //remote host droped the connection.
      if (st._conn.Available == 0)
      {
          DropConnection(st);
      }
      else
      {
          st._conn.EndReceive(ar);
          try 
          { 
              st._provider.OnReceiveData(st); 
          }
          catch
          {
              //report error in the provider
          }
          //Resume ReceivedData callback loop
          if (st._conn.Connected)
              st._conn.BeginReceive(st._buffer, 0, 0, SocketFlags.None,
                ReceivedDataReady, st);
      }
    }


    /// <SUMMARY>
    /// Shutsdown the server
    /// </SUMMARY>
    public void Stop()
    {
      lock(this)
      {
        _listener.Close();
        _listener = null;
        //Close all active connections
        foreach(object obj in _connections)
        {
          ConnectionState st = obj as ConnectionState;
          try{ st._provider.OnDropConnection(st);    }
          catch{
            //some error in the provider
          }
          st._conn.Shutdown(SocketShutdown.Both);
          st._conn.Close();
        }
        _connections.Clear();
      }
    }


    /// <SUMMARY>
    /// Removes a connection from the list
    /// </SUMMARY>
    internal void DropConnection(ConnectionState st)
    {
      lock(this)
      {
        st._conn.Shutdown(SocketShutdown.Both);
        st._conn.Close();
        if(_connections.Contains(st))
            _connections.Remove(st);
      }
    }


    public int MaxConnections
    {
      get
      {
        return _maxConnections;
      }
      set
      {
        _maxConnections = value;
      }
    }


    public int CurrentConnections
    {
      get
      {
        lock(this){ return _connections.Count; }
      }
    }
  }
}