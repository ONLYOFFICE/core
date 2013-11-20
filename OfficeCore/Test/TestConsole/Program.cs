using System;
using System.Collections.Generic;
using System.Text;

namespace TestConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            TestFonts();

        }

        static void TestImage()
        {
            OfficeCore.CImageGdipFileClass oImage = new OfficeCore.CImageGdipFileClass();
            oImage.OpenFile(@"C:\Documents and Settings\All Users\Documents\My Pictures\Sample Pictures\Sunset.jpg");

            //oImage.Resize(1200, 900, 0); // default
            //oImage.Resize(1200, 900, 3); // bilinear
            oImage.Resize(1200, 900, 4); // bicubic
            //oImage.Resize(1200, 900, 5); // nearest

            //oImage.SaveFile(@"C:\test\test.bmp", 1);
            //oImage.SaveFile(@"C:\test\test.gif", 2); 
            //oImage.SaveFile(@"C:\test\test.jpg", 3);
            oImage.SaveFile(@"C:\test\test.png", 4);
            //oImage.SaveFile(@"C:\test\test.tiff", 5);
        }

        static void TestFonts()
        {
            OfficeCore.CWinFontsClass oFonts = new OfficeCore.CWinFontsClass();

            oFonts.Init("", false, true);
            //oFonts.Init("", true, true);
            //oFonts.Init("", false, false);

            //oFonts.Init(@"X:\AVS\Sources\TeamlabOffice\trunk\OfficeWeb\Fonts\native", false, true);
            //oFonts.Init(@"X:\AVS\Sources\TeamlabOffice\trunk\OfficeWeb\Fonts\native", true, true);
            //oFonts.Init(@"X:\AVS\Sources\TeamlabOffice\trunk\OfficeWeb\Fonts\native", false, false);

            oFonts.SetAdditionalParam("BinaryFonts", @"C:\test\font_selection.bin");
            oFonts.SetAdditionalParam("AllFonts.js", @"C:\test\AllFonts.js");
            
        }
    }
}
