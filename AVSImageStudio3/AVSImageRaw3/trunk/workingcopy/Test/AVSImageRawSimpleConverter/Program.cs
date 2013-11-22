using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

// $(SolutionDir)utils\xsd.exe $(SolutionDir)utils\ImageRaw3Options.xsd /c /n:ImageRaw3OptionsWrapper /l:CS /o:$(ProjectDir)

namespace AVSImageRawSimpleConverter
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
