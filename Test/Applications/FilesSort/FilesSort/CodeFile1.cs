using System.IO;
using System.Text;

public class FormatChecker
{
    public class FileFormats
    {
        public static int FromString(string sFormat)
        {
            switch (sFormat.ToLower())
            {
                case "docx": return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
                case "doc": return AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
                case "odt": return AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
                case "rtf": return AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
                case "txt":
                case "xml":
                case "xslt": return AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
                case "html": return AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
                case "mht": return AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT;
                case "epub": return AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB;
                case "fb2": return AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2;
                case "mobi": return AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI;

                case "pptx": return AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
                case "ppt": return AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT;
                case "odp": return AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
                case "ppsx": return AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX;

                case "xlsx": return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
                case "xls": return AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
                case "ods": return AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
                case "csv": return AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;

                case "jpeg":
                case "jpe":
                case "jpg": return AVS_OFFICESTUDIO_FILE_IMAGE_JPG;
                case "tif":
                case "tiff": return AVS_OFFICESTUDIO_FILE_IMAGE_TIFF;
                case "tga": return AVS_OFFICESTUDIO_FILE_IMAGE_TGA;
                case "gif": return AVS_OFFICESTUDIO_FILE_IMAGE_GIF;
                case "png": return AVS_OFFICESTUDIO_FILE_IMAGE_PNG;
                case "emf": return AVS_OFFICESTUDIO_FILE_IMAGE_EMF;
                case "wmf": return AVS_OFFICESTUDIO_FILE_IMAGE_WMF;
                case "bmp": return AVS_OFFICESTUDIO_FILE_IMAGE_BMP;
                case "cr2": return AVS_OFFICESTUDIO_FILE_IMAGE_CR2;
                case "pcx": return AVS_OFFICESTUDIO_FILE_IMAGE_PCX;
                case "ras": return AVS_OFFICESTUDIO_FILE_IMAGE_RAS;
                case "psd": return AVS_OFFICESTUDIO_FILE_IMAGE_PSD;
                case "ico": return AVS_OFFICESTUDIO_FILE_IMAGE_ICO;

                case "pdf": return AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
                case "swf": return AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF;
                case "djvu": return AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU;
                case "xps": return AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS;
                case "svg": return AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SVG;
                case "htmlr": return AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR;
                case "doct": return AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY;
                case "xlst": return AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY;
                case "pptt": return AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY;
                default: return AVS_OFFICESTUDIO_FILE_UNKNOWN;
            }
        }
        public static string ToString(int nFormat)
        {
            switch (nFormat)
            {
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX: return "docx";
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC: return "doc";
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT: return "odt";
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF: return "rtf";
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT: return "txt";
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML: return "html";
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT: return "mht";
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB: return "epub";
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2: return "fb2";
                case AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI: return "mobi";

                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX: return "pptx";
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT: return "ppt";
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP: return "odp";
                case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX: return "ppsx";

                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX: return "xlsx";
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS: return "xls";
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS: return "ods";
                case AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV: return "csv";

                case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF: return "pdf";
                case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF: return "swf";
                case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU: return "djvu";
                case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS: return "xps";
                case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SVG: return "svg";
                case AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR: return "htmlr";

                case AVS_OFFICESTUDIO_FILE_OTHER_HTMLZIP: return "zip";
                case AVS_OFFICESTUDIO_FILE_OTHER_JSON: return "json";

                case AVS_OFFICESTUDIO_FILE_IMAGE: return "jpg";
                case AVS_OFFICESTUDIO_FILE_IMAGE_JPG: return "jpg";
                case AVS_OFFICESTUDIO_FILE_IMAGE_TIFF: return "tiff";
                case AVS_OFFICESTUDIO_FILE_IMAGE_TGA: return "tga";
                case AVS_OFFICESTUDIO_FILE_IMAGE_GIF: return "gif";
                case AVS_OFFICESTUDIO_FILE_IMAGE_PNG: return "png";
                case AVS_OFFICESTUDIO_FILE_IMAGE_EMF: return "emf";
                case AVS_OFFICESTUDIO_FILE_IMAGE_WMF: return "wmf";
                case AVS_OFFICESTUDIO_FILE_IMAGE_BMP: return "bmp";
                case AVS_OFFICESTUDIO_FILE_IMAGE_CR2: return "cr2";
                case AVS_OFFICESTUDIO_FILE_IMAGE_PCX: return "pcx";
                case AVS_OFFICESTUDIO_FILE_IMAGE_RAS: return "ras";
                case AVS_OFFICESTUDIO_FILE_IMAGE_PSD: return "psd";
                case AVS_OFFICESTUDIO_FILE_IMAGE_ICO: return "ico";

                case AVS_OFFICESTUDIO_FILE_CANVAS_WORD:
                case AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET:
                case AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION: return "bin";
                case AVS_OFFICESTUDIO_FILE_OTHER_OLD_DOCUMENT:
                case AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY: return "doct";
                case AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY: return "xlst";
                case AVS_OFFICESTUDIO_FILE_OTHER_OLD_PRESENTATION:
                case AVS_OFFICESTUDIO_FILE_OTHER_OLD_DRAWING:
                case AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY: return "pptt";
                default: return "";
            }
        }
        public const int AVS_OFFICESTUDIO_FILE_UNKNOWN = 0x0000;

        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT = 0x0040;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x0001;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x0002;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x0003;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x0004;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x0005;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x0006;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x0007;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x0008;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2 = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x0009;
        public const int AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI = AVS_OFFICESTUDIO_FILE_DOCUMENT + 0x000a;

        public const int AVS_OFFICESTUDIO_FILE_PRESENTATION = 0x0080;
        public const int AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX = AVS_OFFICESTUDIO_FILE_PRESENTATION + 0x0001;
        public const int AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT = AVS_OFFICESTUDIO_FILE_PRESENTATION + 0x0002;
        public const int AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP = AVS_OFFICESTUDIO_FILE_PRESENTATION + 0x0003;
        public const int AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX = AVS_OFFICESTUDIO_FILE_PRESENTATION + 0x0004;

        public const int AVS_OFFICESTUDIO_FILE_SPREADSHEET = 0x0100;
        public const int AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX = AVS_OFFICESTUDIO_FILE_SPREADSHEET + 0x0001;
        public const int AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS = AVS_OFFICESTUDIO_FILE_SPREADSHEET + 0x0002;
        public const int AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS = AVS_OFFICESTUDIO_FILE_SPREADSHEET + 0x0003;
        public const int AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV = AVS_OFFICESTUDIO_FILE_SPREADSHEET + 0x0004;

        public const int AVS_OFFICESTUDIO_FILE_CROSSPLATFORM = 0x0200;
        public const int AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0001;
        public const int AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0002;
        public const int AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0003;
        public const int AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0004;
        public const int AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SVG = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0005;
        public const int AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLR = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0006;
        public const int AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRMenu = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0007;
        public const int AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_HTMLRCanvas = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM + 0x0008;

        public const int AVS_OFFICESTUDIO_FILE_IMAGE = 0x0400;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_JPG = AVS_OFFICESTUDIO_FILE_IMAGE + 0x0001;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_TIFF = AVS_OFFICESTUDIO_FILE_IMAGE + 0x0002;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_TGA = AVS_OFFICESTUDIO_FILE_IMAGE + 0x0003;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_GIF = AVS_OFFICESTUDIO_FILE_IMAGE + 0x0004;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_PNG = AVS_OFFICESTUDIO_FILE_IMAGE + 0x0005;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_EMF = AVS_OFFICESTUDIO_FILE_IMAGE + 0x0006;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_WMF = AVS_OFFICESTUDIO_FILE_IMAGE + 0x0007;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_BMP = AVS_OFFICESTUDIO_FILE_IMAGE + 0x0008;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_CR2 = AVS_OFFICESTUDIO_FILE_IMAGE + 0x0009;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_PCX = AVS_OFFICESTUDIO_FILE_IMAGE + 0x000a;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_RAS = AVS_OFFICESTUDIO_FILE_IMAGE + 0x000b;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_PSD = AVS_OFFICESTUDIO_FILE_IMAGE + 0x000c;
        public const int AVS_OFFICESTUDIO_FILE_IMAGE_ICO = AVS_OFFICESTUDIO_FILE_IMAGE + 0x000d;

        public const int AVS_OFFICESTUDIO_FILE_OTHER = 0x0800;
        public const int AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE = AVS_OFFICESTUDIO_FILE_OTHER + 0x0001;
        public const int AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO = AVS_OFFICESTUDIO_FILE_OTHER + 0x0002;
        public const int AVS_OFFICESTUDIO_FILE_OTHER_HTMLZIP = AVS_OFFICESTUDIO_FILE_OTHER + 0x0003;
        public const int AVS_OFFICESTUDIO_FILE_OTHER_OLD_DOCUMENT = AVS_OFFICESTUDIO_FILE_OTHER + 0x0004;
        public const int AVS_OFFICESTUDIO_FILE_OTHER_OLD_PRESENTATION = AVS_OFFICESTUDIO_FILE_OTHER + 0x0005;
        public const int AVS_OFFICESTUDIO_FILE_OTHER_OLD_DRAWING = AVS_OFFICESTUDIO_FILE_OTHER + 0x0006;
        public const int AVS_OFFICESTUDIO_FILE_OTHER_TEAMLAB_INNER = AVS_OFFICESTUDIO_FILE_OTHER + 0x0007;
        public const int AVS_OFFICESTUDIO_FILE_OTHER_JSON = AVS_OFFICESTUDIO_FILE_OTHER + 0x0008;               // Для mail-merge

        public const int AVS_OFFICESTUDIO_FILE_TEAMLAB = 0x1000;
        public const int AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY = AVS_OFFICESTUDIO_FILE_TEAMLAB + 0x0001;
        public const int AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY = AVS_OFFICESTUDIO_FILE_TEAMLAB + 0x0002;
        public const int AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY = AVS_OFFICESTUDIO_FILE_TEAMLAB + 0x0003;

        public const int AVS_OFFICESTUDIO_FILE_CANVAS = 0x2000;
        public const int AVS_OFFICESTUDIO_FILE_CANVAS_WORD = AVS_OFFICESTUDIO_FILE_CANVAS + 0x0001;
        public const int AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET = AVS_OFFICESTUDIO_FILE_CANVAS + 0x0002;
        public const int AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION = AVS_OFFICESTUDIO_FILE_CANVAS + 0x0003;
    }
    private static int GetFileFormatBySignature(byte[] aBuffer)
    {
        int nLength = aBuffer.Length;

        if ((5 <= nLength) && '{' == aBuffer[0] && '\\' == aBuffer[1] && 'r' == aBuffer[2] && 't' == aBuffer[3] && 'f' == aBuffer[4])
            return FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;

        string sText = Encoding.ASCII.GetString(aBuffer, 0, 20);
        if (-1 != sText.IndexOf("%PDF-"))
            return FileFormats.AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
        else if (0 == sText.IndexOf("DOCY"))
            return FileFormats.AVS_OFFICESTUDIO_FILE_CANVAS_WORD;
        else if (0 == sText.IndexOf("XLSY"))
            return FileFormats.AVS_OFFICESTUDIO_FILE_CANVAS_SPREADSHEET;
        else if (0 == sText.IndexOf("PPTY"))
            return FileFormats.AVS_OFFICESTUDIO_FILE_CANVAS_PRESENTATION;

        bool tagOpen = false;

        for (int i = 0; i < nLength - 11 && i < 100; i++)
        {
            if (0x3C == aBuffer[i])
                tagOpen = true;
            else if (0x3E == aBuffer[i])
                tagOpen = false;
            else if (tagOpen && 0x46 == aBuffer[i] && 0x69 == aBuffer[i + 1] && 0x63 == aBuffer[i + 2]
                             && 0x74 == aBuffer[i + 3] && 0x69 == aBuffer[i + 4] && 0x6F == aBuffer[i + 5]
                             && 0x6E == aBuffer[i + 6] && 0x42 == aBuffer[i + 7] && 0x6F == aBuffer[i + 8]
                             && 0x6F == aBuffer[i + 9] && 0x6B == aBuffer[i + 10])
                return FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2;
            else if (tagOpen && (0x48 == aBuffer[i] || 0x68 == aBuffer[i]) && (0x54 == aBuffer[i + 1] || 0x74 == aBuffer[i + 1])
                             && (0x4d == aBuffer[i + 2] || 0x6d == aBuffer[i + 2]) && (0x4c == aBuffer[i + 3] || 0x6c == aBuffer[i + 3]))
                return FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
        }

        if ((8 <= nLength) && 0x41 == aBuffer[0] && 0x54 == aBuffer[1] && 0x26 == aBuffer[2] && 0x54 == aBuffer[3] && 0x46 == aBuffer[4] && 0x4f == aBuffer[5] && 0x52 == aBuffer[6] && 0x4d == aBuffer[7])
            return FileFormats.AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU;

        if (68 <= nLength && (('B' == aBuffer[60] && 'O' == aBuffer[61] && 'O' == aBuffer[62] && 'K' == aBuffer[63] && 'M' == aBuffer[64] && 'O' == aBuffer[65] && 'B' == aBuffer[66] && 'I' == aBuffer[67])
                           || ('T' == aBuffer[60] && 'E' == aBuffer[61] && 'X' == aBuffer[62] && 't' == aBuffer[63] && 'R' == aBuffer[64] && 'E' == aBuffer[65] && 'A' == aBuffer[66] && 'D' == aBuffer[67])))
            return FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI;

        if ((3 <= nLength) && (0xFF == aBuffer[0]) && (0xD8 == aBuffer[1]) && (0xFF == aBuffer[2]))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_JPG;

        if ((34 <= nLength) && (0x42 == aBuffer[0]) && (0x4D == aBuffer[1]) &&
            (0x00 == aBuffer[6]) && (0x00 == aBuffer[7]) && (0x01 == aBuffer[26]) && (0x00 == aBuffer[27]) &&
            ((0x00 == aBuffer[28]) || (0x01 == aBuffer[28]) || (0x04 == aBuffer[28]) || (0x08 == aBuffer[28]) ||
            (0x10 == aBuffer[28]) || (0x18 == aBuffer[28]) || (0x20 == aBuffer[28])) && (0x00 == aBuffer[29]) &&
            ((0x00 == aBuffer[30]) || (0x01 == aBuffer[30]) || (0x02 == aBuffer[30]) || (0x03 == aBuffer[30]) ||
            (0x04 == aBuffer[30]) || (0x05 == aBuffer[30])) && (0x00 == aBuffer[31]) && (0x00 == aBuffer[32]) && (0x00 == aBuffer[33]))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_BMP;

        if ((4 <= nLength) && "GIF8" == System.Text.Encoding.ASCII.GetString(aBuffer, 0, "GIF8".Length))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_GIF;
        if ((6 <= nLength) && ("GIF87a" == System.Text.Encoding.ASCII.GetString(aBuffer, 0, "GIF87a".Length) || "GIF89a" == System.Text.Encoding.ASCII.GetString(aBuffer, 0, "GIF89a".Length)))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_GIF;

        if ((16 <= nLength) && (0x89 == aBuffer[0]) && (0x50 == aBuffer[1]) && (0x4E == aBuffer[2]) && (0x47 == aBuffer[3])
            && (0x0D == aBuffer[4]) && (0x0A == aBuffer[5]) && (0x1A == aBuffer[6]) && (0x0A == aBuffer[7])
            && (0x00 == aBuffer[8]) && (0x00 == aBuffer[9]) && (0x00 == aBuffer[10]) && (0x0D == aBuffer[11])
            && (0x49 == aBuffer[12]) && (0x48 == aBuffer[13]) && (0x44 == aBuffer[14]) && (0x52 == aBuffer[15]))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_PNG;

        if ((10 <= nLength) && (0x49 == aBuffer[0]) && (0x49 == aBuffer[1]) && (0x2A == aBuffer[2])
            && (0x00 == aBuffer[3]) && (0x10 == aBuffer[4]) && (0x00 == aBuffer[5]) && (0x00 == aBuffer[6])
            && (0x00 == aBuffer[7]) && (0x43 == aBuffer[8]) && (0x52 == aBuffer[9]))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_CR2;

        if (4 <= nLength)
        {
            if (((0x49 == aBuffer[0]) && (0x49 == aBuffer[1]) && (0x2A == aBuffer[2]) && (0x00 == aBuffer[3])) ||
                ((0x4D == aBuffer[0]) && (0x4D == aBuffer[1]) && (0x00 == aBuffer[2]) && (0x2A == aBuffer[3])) ||
                ((0x49 == aBuffer[0]) && (0x49 == aBuffer[1]) && (0x2A == aBuffer[2]) && (0x00 == aBuffer[3])))
                return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_TIFF;
        }

        if (6 <= nLength)
        {
            if (((0xD7 == aBuffer[0]) && (0xCD == aBuffer[1]) && (0xC6 == aBuffer[2]) && (0x9A == aBuffer[3]) && (0x00 == aBuffer[4]) && (0x00 == aBuffer[5])) ||
                ((0x01 == aBuffer[0]) && (0x00 == aBuffer[1]) && (0x09 == aBuffer[2]) && (0x00 == aBuffer[3]) && (0x00 == aBuffer[4]) && (0x03 == aBuffer[5])))
                return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_WMF;
        }

        if ((44 <= nLength) && (0x01 == aBuffer[0]) && (0x00 == aBuffer[1]) && (0x00 == aBuffer[2]) && (0x00 == aBuffer[3]) &&
            (0x20 == aBuffer[40]) && (0x45 == aBuffer[41]) && (0x4D == aBuffer[42]) && (0x46 == aBuffer[43]))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_EMF;

        if ((4 <= nLength) && (0x0A == aBuffer[0]) && (0x00 == aBuffer[1] || 0x01 == aBuffer[1] ||
            0x02 == aBuffer[1] || 0x03 == aBuffer[1] || 0x04 == aBuffer[1] || 0x05 == aBuffer[1]) &&
            (0x01 == aBuffer[3] || 0x02 == aBuffer[3] || 0x04 == aBuffer[3] || 0x08 == aBuffer[3]))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_PCX;

        if ((17 <= nLength) && ((0x01 == aBuffer[1] && 0x01 == aBuffer[2]) || (0x00 == aBuffer[1] && 0x02 == aBuffer[2]) ||
            (0x00 == aBuffer[1] && 0x03 == aBuffer[2]) || (0x01 == aBuffer[1] && 0x09 == aBuffer[2]) ||
            (0x00 == aBuffer[1] && 0x0A == aBuffer[2]) || (0x00 == aBuffer[1] && 0x0B == aBuffer[2]))
            && (0x08 == aBuffer[16] || 0x10 == aBuffer[16] || 0x18 == aBuffer[16] || 0x20 == aBuffer[16]))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_TGA;

        if ((4 <= nLength) && (0x59 == aBuffer[0]) && (0xA6 == aBuffer[1]) && (0x6A == aBuffer[2]) && (0x95 == aBuffer[3]))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_RAS;

        if ((13 <= nLength) && (0x38 == aBuffer[0]) && (0x42 == aBuffer[1]) && (0x50 == aBuffer[2])
            && (0x53 == aBuffer[3]) && (0x00 == aBuffer[4]) && (0x01 == aBuffer[5]) && (0x00 == aBuffer[6])
            && (0x00 == aBuffer[7]) && (0x00 == aBuffer[8]) && (0x00 == aBuffer[9]) && (0x00 == aBuffer[10])
            && (0x00 == aBuffer[11]) && (0x00 == aBuffer[12]))
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_PSD;

        if (4 <= nLength && 0x00 == aBuffer[0] && 0x00 == aBuffer[1] && 0x01 == aBuffer[2] && 0x00 == aBuffer[3])
            return FileFormats.AVS_OFFICESTUDIO_FILE_IMAGE_ICO;

        if ((4 <= nLength) && "<svg" == System.Text.Encoding.ASCII.GetString(aBuffer, 0, "<svg".Length))
            return FileFormats.AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SVG;

        return FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN;
    }
    private static int GetFileFormatByZip(Ionic.Zip.ZipFile oZipFile)
    {

        if (oZipFile.ContainsEntry("[Content_Types].xml"))
        {
            Ionic.Zip.ZipEntry oEntry = oZipFile["[Content_Types].xml"];
            using (MemoryStream oMemoryStream = new MemoryStream((int)oEntry.UncompressedSize))
            {
                oEntry.Extract(oMemoryStream);
                oMemoryStream.Position = 0;
                string sContent = System.Text.UTF8Encoding.UTF8.GetString(oMemoryStream.GetBuffer(), 0, (int)oMemoryStream.Length);
                if (-1 != sContent.IndexOf("application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.openxmlformats-officedocument.wordprocessingml.template.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.ms-word.document.macroEnabled.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.ms-word.template.macroEnabledTemplate.main+xml"))
                    return FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
                else if (-1 != sContent.IndexOf("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.openxmlformats-officedocument.spreadsheetml.template.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.ms-excel.sheet.macroEnabled.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.ms-excel.template.macroEnabled.main+xml"))
                    return FileFormats.AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
                else if (-1 != sContent.IndexOf("application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.openxmlformats-officedocument.presentationml.template.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.ms-powerpoint.presentation.macroEnabled.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.ms-powerpoint.slideshow.macroEnabled.main+xml") ||
                    -1 != sContent.IndexOf("application/vnd.ms-powerpoint.template.macroEnabled.main+xml"))
                    return FileFormats.AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
                else if (-1 != sContent.IndexOf("application/vnd.openxmlformats-officedocument.presentationml.slideshow.main+xml"))
                    return FileFormats.AVS_OFFICESTUDIO_FILE_PRESENTATION_PPSX;
            }
        }
        if (oZipFile.ContainsEntry("mimetype"))
        {
            Ionic.Zip.ZipEntry oEntry = oZipFile["mimetype"];
            using (MemoryStream oMemoryStream = new MemoryStream((int)oEntry.UncompressedSize))
            {
                oEntry.Extract(oMemoryStream);
                oMemoryStream.Position = 0;
                string sContent = System.Text.ASCIIEncoding.ASCII.GetString(oMemoryStream.GetBuffer(), 0, (int)oMemoryStream.Length);
                if (-1 != sContent.IndexOf("application/vnd.oasis.opendocument.text"))
                    return FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
                else if (-1 != sContent.IndexOf("application/vnd.oasis.opendocument.spreadsheet"))
                    return FileFormats.AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
                else if (-1 != sContent.IndexOf("application/vnd.oasis.opendocument.presentation"))
                    return FileFormats.AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
                else if (-1 != sContent.IndexOf("application/epub+zip"))
                    return FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB;
            }
        }
        if (oZipFile.ContainsEntry("_rels/.rels"))
        {
            Ionic.Zip.ZipEntry oEntry = oZipFile["_rels/.rels"];
            using (MemoryStream oMemoryStream = new MemoryStream((int)oEntry.UncompressedSize))
            {
                oEntry.Extract(oMemoryStream);
                oMemoryStream.Position = 0;
                string sContent = System.Text.ASCIIEncoding.ASCII.GetString(oMemoryStream.GetBuffer(), 0, (int)oMemoryStream.Length);
                if (-1 != sContent.IndexOf("http://schemas.microsoft.com/xps/2005/06/fixedrepresentation"))
                    return FileFormats.AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS;
            }
        }

        if (oZipFile.ContainsEntry("_rels/.rels/[0].piece"))
            return FileFormats.AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS;
        if (oZipFile.ContainsEntry("Editor.bin"))
        {
            Ionic.Zip.ZipEntry oEntry = oZipFile["Editor.bin"];
            int nFormat = FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN;
            using (MemoryStream oMemoryStream = new MemoryStream((int)oEntry.UncompressedSize))
            {
                oEntry.Extract(oMemoryStream);
                oMemoryStream.Position = 0;
                int nSignatureLength = 4;
                if (oMemoryStream.Length >= nSignatureLength)
                {
                    byte[] aSignature = new byte[nSignatureLength];
                    oMemoryStream.Read(aSignature, 0, nSignatureLength);
                    string sSignature = System.Text.ASCIIEncoding.ASCII.GetString(aSignature);
                    switch (sSignature)
                    {
                        case "DOCY": nFormat = FileFormats.AVS_OFFICESTUDIO_FILE_TEAMLAB_DOCY; break;
                        case "XLSY": nFormat = FileFormats.AVS_OFFICESTUDIO_FILE_TEAMLAB_XLSY; break;
                        case "PPTY": nFormat = FileFormats.AVS_OFFICESTUDIO_FILE_TEAMLAB_PPTY; break;
                    }
                }
            }
            if (FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN != nFormat)
                return nFormat;
        }
        else if (oZipFile.ContainsEntry("Editor.xml"))
        {
            return FileFormats.AVS_OFFICESTUDIO_FILE_OTHER_OLD_PRESENTATION;
        }
        else if (oZipFile.ContainsEntry("Editor.svg"))
        {
            return FileFormats.AVS_OFFICESTUDIO_FILE_OTHER_OLD_DRAWING;
        }
        else if (oZipFile.ContainsEntry("Editor.html.arch"))
        {
            return FileFormats.AVS_OFFICESTUDIO_FILE_OTHER_OLD_DOCUMENT;
        }
        return FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN;
    }
    private static int GetFileFormatStorage(OpenMcdf.CompoundFile oCompoundFile)
    {
        int format = FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN;
        try
        {
            oCompoundFile.RootStorage.GetStream("WordDocument");
            format = FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
        }
        catch
        {
        }
        if (FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN == format)
        {
            try
            {
                oCompoundFile.RootStorage.GetStream("Workbook");
                format = FileFormats.AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
            }
            catch
            {
            }
        }
        if (FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN == format)
        {
            try
            {
                oCompoundFile.RootStorage.GetStream("PowerPoint Document");
                format = FileFormats.AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT;
            }
            catch
            {
            }
        }
        if (FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN == format)
        {
            try
            {
                oCompoundFile.RootStorage.GetStorage("\x0006DataSpaces");
                format = FileFormats.AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO;
            }
            catch
            {
            }
        }
        return format;
    }
    public static int GetFileFormat(string sPath)
    {
        int format = FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN;
        try
        {

            if (new FileInfo(sPath).Length == 0)
                format = FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
            else
            {

                using (FileStream fs = new FileStream(sPath, FileMode.Open, FileAccess.Read))
                {
                    byte[] aBuffer = new byte[100];
                    fs.Read(aBuffer, 0, aBuffer.Length);
                    format = GetFileFormatBySignature(aBuffer);
                }

                if (FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN == format)
                {
                    try
                    {
                        OpenMcdf.CompoundFile oCompoundFile = new OpenMcdf.CompoundFile(sPath);
                        format = GetFileFormatStorage(oCompoundFile);
                        oCompoundFile.Close();
                    }
                    catch
                    {
                    }
                    if (FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN == format)
                    {
                        try
                        {
                            if (Ionic.Zip.ZipFile.IsZipFile(sPath))
                            {
                                using (Ionic.Zip.ZipFile oZipFile = Ionic.Zip.ZipFile.Read(sPath))
                                {
                                    format = GetFileFormatByZip(oZipFile);
                                }
                            }
                        }
                        catch
                        {
                        }
                    }

                    if (FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN == format)
                    {
                        string sExtention = Path.GetExtension(sPath).ToLower();

                        if (".mht" == sExtention)
                            format = FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT;
                        else if (".txt" == sExtention || ".xml" == sExtention || ".xslt" == sExtention)
                            format = FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
                        else if (".csv" == sExtention)
                            format = FileFormats.AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
                        else if (".svg" == sExtention)
                            format = FileFormats.AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SVG;
                        else if (".html" == sExtention || ".htm" == sExtention)
                            format = FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
                    }
                }
            }
        }
        catch
        {
        }
        return format;
    }
    public static int GetFileFormat(byte[] aBuffer)
    {
        int format = FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN;
        try
        {

            if (aBuffer.Length == 0)
                format = FileFormats.AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
            else
            {

                format = GetFileFormatBySignature(aBuffer);
                if (FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN == format)
                {
                    try
                    {
                        using (MemoryStream ms = new MemoryStream(aBuffer))
                        {
                            OpenMcdf.CompoundFile oCompoundFile = new OpenMcdf.CompoundFile(ms);
                            format = GetFileFormatStorage(oCompoundFile);
                            oCompoundFile.Close();
                        }
                    }
                    catch
                    {
                    }
                    if (FileFormats.AVS_OFFICESTUDIO_FILE_UNKNOWN == format)
                    {
                        try
                        {
                            using (MemoryStream ms = new MemoryStream(aBuffer))
                            {
                                if (Ionic.Zip.ZipFile.IsZipFile(ms, false))
                                {
                                    using (Ionic.Zip.ZipFile oZipFile = Ionic.Zip.ZipFile.Read(ms))
                                    {
                                        format = GetFileFormatByZip(oZipFile);
                                    }
                                }

                            }
                        }
                        catch
                        {
                        }
                    }
                }
            }
        }
        catch
        {
        }
        return format;
    }
}