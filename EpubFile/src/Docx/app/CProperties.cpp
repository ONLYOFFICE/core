#include "CProperties.h"

CProperties::CProperties()
    :CElement(L"Properties")
{
}

void CProperties::SetDefoult()
{
    AddArgument(L"xmlns", L"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties");
    AddArgument(L"xmlns:vt", L"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes");

    CElement *oTemplate = new CElement(L"Template", L"Normal.dotm");
    AddChildren(oTemplate);

    CElement *oTotalTime = new CElement(L"TotalTime", L"0");
    AddChildren(oTotalTime);

    CElement *oPages = new CElement(L"Pages", L"1");
    AddChildren(oPages);

    CElement *oWords = new CElement(L"Words", L"0");
    AddChildren(oWords);

    CElement *oCharacters = new CElement(L"Characters", L"0");
    AddChildren(oCharacters);

    CElement *oApplication = new CElement(L"Application", L"Microsoft Office Word");
    AddChildren(oApplication);

    CElement *oDocSecurity = new CElement(L"DocSecurity", L"0");
    AddChildren(oDocSecurity);

    CElement *oLines = new CElement(L"Lines", L"0");
    AddChildren(oLines);

    CElement *oParagraphs = new CElement(L"Paragraphs", L"0");
    AddChildren(oParagraphs);

    CElement *oScaleCrop = new CElement(L"ScaleCrop", L"false");
    AddChildren(oScaleCrop);

    CElement *oCompany = new CElement(L"Company");
    AddChildren(oCompany);

    CElement *oLinksUpToDate = new CElement(L"LinksUpToDate", L"false");
    AddChildren(oLinksUpToDate);

    CElement *oCharactersWithSpaces = new CElement(L"CharactersWithSpaces", L"0");
    AddChildren(oCharactersWithSpaces);

    CElement *oSharedDoc = new CElement(L"SharedDoc", L"false");
    AddChildren(oSharedDoc);

    CElement *oHyperlinksChanged = new CElement(L"HyperlinksChanged", L"false");
    AddChildren(oHyperlinksChanged);

    CElement *oAppVersion = new CElement(L"AppVersion", L"16.0000");
    AddChildren(oAppVersion);
}
