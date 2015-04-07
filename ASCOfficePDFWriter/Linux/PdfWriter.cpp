#include "../PdfWriterLib/PdfWriterLib.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../version.h"

int main(int argc, char *argv[])
{
    CPdfWriterLib pdfWriter;

    // check arguments
     if (argc < 3)
     {
         // print out help topic

         std::cout << std::endl;
         std::cout << std::endl;
         std::cout << "-------------------------------------------------------------------------------" << std::endl;
         std::cout << "\t\tASCPdfWriter (Version: " << STRVER << ")" << std::endl;
         std::cout << "-------------------------------------------------------------------------------" << std::endl;
         std::cout << std::endl;
         std::cout << "USAGE: ASCPdfWriter \"path_to_file_1\" \"path_to_file_2\" [\"path_to_font_selection\"] " << std::endl;
         std::cout << "WHERE:" << std::endl;
         std::cout << "\t\"path_to_file_1\" is a path to file to be converted" << std::endl;
         std::cout << "\t\"path_to_file_2\" is a path to the PDF output file" << std::endl;
         std::cout << "\t\"path_to_font_selection\" is a path to 'font_selection.bin' location" << std::endl;
         return 1;
     }

    CString pathBin     = argv[1];
    CString pathPdf     = argv[2];
    CString pathFonts;

    if (argc > 3) pathFonts = argv[3];
    else
        pathFonts = FileSystem::Directory::GetFolderPath(pathBin) + FILE_SEPARATOR_STR + _T("Fonts");

    pdfWriter.SetFontDir(pathFonts);

    pdfWriter.OnlineWordToPdf(pathBin, pathPdf,  1);

    return 0;
}
