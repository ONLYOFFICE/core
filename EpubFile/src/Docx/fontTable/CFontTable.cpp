#include "CFontTable.h"
#include "CFont.h"
#include "../../../../DesktopEditor/common/File.h"


CFontTable::CFontTable()
    : CElement(L"fonts", L"", L"w", false)
{

}

void CFontTable::SetDefoult()
{
    AddArgument(L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006");
    AddArgument(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
    AddArgument(L"xmlns:w", L"http://schemas.openxmlformats.org/wordprocessingml/2006/main");
    AddArgument(L"xmlns:w14", L"http://schemas.microsoft.com/office/word/2010/wordml");
    AddArgument(L"xmlns:w15", L"http://schemas.microsoft.com/office/word/2012/wordml");
    AddArgument(L"xmlns:w16se", L"http://schemas.microsoft.com/office/word/2015/wordml/symex");
    AddArgument(L"mc:Ignorable", L"w14 w15 w16se");

    CElement *oFont1 = new CElement(L"font", L"", L"w");
        oFont1->AddArgument(L"name", L"Calibri");

        CElement *oPanose11 = new CElement(L"panose1", L"", L"w");
        oPanose11->AddArgument(L"val", L"020F0502020204030204");
        oFont1->AddChildren(oPanose11);

        CElement *oCharset1 = new CElement(L"charset", L"", L"w");
        oCharset1->AddArgument(L"val", L"CC");
        oFont1->AddChildren(oCharset1);

        CElement *oFamily1 = new CElement(L"family", L"", L"w");
        oFamily1->AddArgument(L"val", L"swiss");
        oFont1->AddChildren(oFamily1);

        CElement *oPitch1 = new CElement(L"pitch", L"", L"w");
        oPitch1->AddArgument(L"val", L"variable");
        oFont1->AddChildren(oPitch1);

        CElement *oSig1 = new CElement(L"sig", L"", L"w");
        oSig1->AddArgument(L"usb0", L"E4002EFF");
        oSig1->AddArgument(L"usb1", L"C000247B");
        oSig1->AddArgument(L"usb2", L"00000009");
        oSig1->AddArgument(L"usb3", L"00000000");
        oSig1->AddArgument(L"csb0", L"000001FF");
        oSig1->AddArgument(L"csb1", L"00000000");
        oFont1->AddChildren(oSig1);

    AddChildren(oFont1);

    CElement *oFont2 = new CElement(L"font", L"", L"w");
        oFont2->AddArgument(L"name", L"PMingLiU");

        CElement *oAltName2 = new CElement(L"altName", L"", L"w");
        std::string sVal = "新細明體";
        oAltName2->AddArgument(L"val", NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sVal.c_str(), (LONG)strlen((const char*)sVal.c_str())));
        oFont2->AddChildren(oAltName2);

        CElement *oPanose12 = new CElement(L"panose1", L"", L"w");
        oPanose12->AddArgument(L"val", L"02010601000101010101");
        oFont2->AddChildren(oPanose12);

        CElement *oCharset2 = new CElement(L"charset", L"", L"w");
        oCharset2->AddArgument(L"val", L"88");
        oFont2->AddChildren(oCharset2);

        CElement *oFamily2 = new CElement(L"family", L"", L"w");
        oFamily2->AddArgument(L"val", L"auto");
        oFont2->AddChildren(oFamily2);

        CElement *oPitch2 = new CElement(L"pitch", L"", L"w");
        oPitch2->AddArgument(L"val", L"variable");
        oFont2->AddChildren(oPitch2);

        CElement *oSig2 = new CElement(L"sig", L"", L"w");
        oSig2->AddArgument(L"usb0", L"00000000");
        oSig2->AddArgument(L"usb1", L"08080000");
        oSig2->AddArgument(L"usb2", L"00000010");
        oSig2->AddArgument(L"usb3", L"00000000");
        oSig2->AddArgument(L"csb0", L"00100000");
        oSig2->AddArgument(L"csb1", L"00000000");
        oFont2->AddChildren(oSig2);

    AddChildren(oFont2);

    CElement *oFont3 = new CElement(L"font", L"", L"w");
        oFont3->AddArgument(L"name", L"Times New Roman");

        CElement *oPanose13 = new CElement(L"panose1", L"", L"w");
        oPanose13->AddArgument(L"val", L"02020603050405020304");
        oFont3->AddChildren(oPanose13);

        CElement *oCharset3 = new CElement(L"charset", L"", L"w");
        oCharset3->AddArgument(L"val", L"CC");
        oFont3->AddChildren(oCharset3);

        CElement *oFamily3 = new CElement(L"family", L"", L"w");
        oFamily3->AddArgument(L"val", L"roman");
        oFont3->AddChildren(oFamily3);

        CElement *oPitch3 = new CElement(L"pitch", L"", L"w");
        oPitch3->AddArgument(L"val", L"variable");
        oFont3->AddChildren(oPitch3);

        CElement *oSig3 = new CElement(L"sig", L"", L"w");
        oSig3->AddArgument(L"usb0", L"E0002EFF");
        oSig3->AddArgument(L"usb1", L"C000785B");
        oSig3->AddArgument(L"usb2", L"00000009");
        oSig3->AddArgument(L"usb3", L"00000000");
        oSig3->AddArgument(L"csb0", L"000001FF");
        oSig3->AddArgument(L"csb1", L"00000000");
        oFont3->AddChildren(oSig3);

    AddChildren(oFont3);

    CElement *oFont4 = new CElement(L"font", L"", L"w");
        oFont4->AddArgument(L"name", L"Calibri Light");

        CElement *oPanose14 = new CElement(L"panose1", L"", L"w");
        oPanose14->AddArgument(L"val", L"020F0302020204030204");
        oFont4->AddChildren(oPanose14);

        CElement *oCharset4 = new CElement(L"charset", L"", L"w");
        oCharset4->AddArgument(L"val", L"CC");
        oFont4->AddChildren(oCharset4);

        CElement *oFamily4 = new CElement(L"family", L"", L"w");
        oFamily4->AddArgument(L"val", L"swiss");
        oFont4->AddChildren(oFamily4);

        CElement *oPitch4 = new CElement(L"pitch", L"", L"w");
        oPitch4->AddArgument(L"val", L"variable");
        oFont4->AddChildren(oPitch4);

        CElement *oSig4 = new CElement(L"sig", L"", L"w");
        oSig4->AddArgument(L"usb0", L"E4002EFF");
        oSig4->AddArgument(L"usb1", L"C000247B");
        oSig4->AddArgument(L"usb2", L"00000009");
        oSig4->AddArgument(L"usb3", L"00000000");
        oSig4->AddArgument(L"csb0", L"000001FF");
        oSig4->AddArgument(L"csb1", L"00000000");
        oFont4->AddChildren(oSig4);

    AddChildren(oFont4);
}
