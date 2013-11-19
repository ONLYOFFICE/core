using System;
using System.Collections.Generic;
using System.Text;

namespace TeamlabDocControlRemote
{
    public class TeamlabDocControlRemoteObject: MarshalByRefObject 
    {
        public void TeamlabDocControlRemoteObject
        { 
        }
        public string getXML (string param)
        {
            string xml = "<xml/>";
            return xml;
        }
    }
}
