using System;
using System.Collections.Generic;
using System.Text;

namespace AllFontsGen
{
    class Program
    {
        static void Main(string[] args)
        {
            string sDirectory = "";
            string sDstFile = "";

            if (args.Length == 0)
                return;

            if (args.Length == 1)
                sDstFile = args[0];
            else
            {
                sDirectory = args[0];
                sDstFile = args[1];
            }

            OfficeCore.CWinFontsClass oFonts = new OfficeCore.CWinFontsClass();
            oFonts.Init(sDirectory, false, false);
            oFonts.SetAdditionalParam("AllFonts.js", sDstFile);
        }
    }
}
