package docbuilder;

public class FileTypes {
    public class Document {
        private static final int MASK = 0x0040;
        public static final int DOCX = MASK + 0x0001;
        public static final int DOC = MASK + 0x0002;
        public static final int ODT = MASK + 0x0003;
        public static final int RTF = MASK + 0x0004;
        public static final int TXT = MASK + 0x0005;
        public static final int DOTX = MASK + 0x000c;
        public static final int OTT = MASK + 0x000f;
        public static final int HTML = MASK + 0x0012;
        public static final int OFORM_PDF = MASK + 0x0017;
    }

    public class Presentation {
        private static final int MASK = 0x0080;
        public static final int PPTX = MASK + 0x0001;
        public static final int PPT = MASK + 0x0002;
        public static final int ODP = MASK + 0x0003;
        public static final int PPSX = MASK + 0x0004;
        public static final int POTX = MASK + 0x0007;
        public static final int OTP = MASK + 0x000a;
    }

    public class Spreadsheet {
        private static final int MASK = 0x0100;
        public static final int XLSX = MASK + 0x0001;
        public static final int XLS = MASK + 0x0002;
        public static final int ODS = MASK + 0x0003;
        public static final int CSV = MASK + 0x0004;
        public static final int XLTX = MASK + 0x0006;
        public static final int OTS = MASK + 0x0009;
    }

    public class Graphics {
        private static final int PDF_MASK = 0x0200;
        public static final int PDF = PDF_MASK + 0x0001;
        public static final int PDFA = PDF_MASK + 0x0009;

        private static final int IMAGE_MASK = 0x0400;
        public static final int JPG = IMAGE_MASK + 0x0001;
        public static final int PNG = IMAGE_MASK + 0x0005;
        public static final int BMP = IMAGE_MASK + 0x0008;
    }
}
