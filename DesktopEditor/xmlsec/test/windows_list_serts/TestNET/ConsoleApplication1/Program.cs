using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Xml;
using System.Security;
using System.Security.Cryptography;
using System.Security.Cryptography.Xml;
using System.Security.Cryptography.X509Certificates;
using System.IO;

namespace Sha1_C14N
{
    class Program
    {
        static void Main(string[] args)
        {
            XmlDocument docStripped = new XmlDocument();
            //docStripped.Load(@"D:\GIT\core\DesktopEditor\xmlsec\test\windows_list_serts\Debug\debug\document.xml");
            docStripped.Load("D:\\444.txt");

            XmlDsigC14NTransform t = new XmlDsigC14NTransform();
            t.LoadInput(docStripped);
            Stream s = (Stream)t.GetOutput(typeof(Stream));

            BinaryReader br = new BinaryReader(s);
            byte[] b = br.ReadBytes((int)s.Length);

            File.Delete("D:\\1.txt");
            var fileStream = File.Create("D:\\1.txt");
            s.Seek(0, SeekOrigin.Begin);
            s.CopyTo(fileStream);
            fileStream.Close();

            s.Seek(0, SeekOrigin.Begin);

            SHA1 sha1 = SHA1.Create();
            byte[] hash = sha1.ComputeHash(s);
            string base64String = Convert.ToBase64String(hash);

            FileStream fRes = File.Create("D:\\res.bin");
            fRes.Write(hash, 0, hash.Length);
            fRes.Close();

            SHA1 sha33 = SHA1.Create();
            byte[] hash33 = sha33.ComputeHash(b);
            string base64String2 = Convert.ToBase64String(hash33);

            FileStream s1 = File.OpenRead(@"D:\GIT\core\DesktopEditor\xmlsec\test\windows_list_serts\Debug\debug\document2.xml");
            SHA1 sha11 = SHA1.Create();
            byte[] hash11 = sha11.ComputeHash(s1);
            string base64_11 = Convert.ToBase64String(hash11);
            
            X509Store my = new X509Store(StoreName.My, StoreLocation.CurrentUser);
            my.Open(OpenFlags.ReadOnly);

            // Find the certificate we’ll use to sign
            RSACryptoServiceProvider csp = null;
            foreach (X509Certificate2 cert in my.Certificates)
            {
                string sName = cert.Subject;
                if (cert.Subject.Contains("Oleg.Korshul"))
                {
                    csp = (RSACryptoServiceProvider)cert.PrivateKey;
                    break;
                }
            }

            byte[] signedData = csp.SignHash(hash11, CryptoConfig.MapNameToOID("SHA1"));
            string signedDataBase64 = Convert.ToBase64String(signedData);

            Console.WriteLine(base64_11);
        }
    }
}
