#include "CDocRelationshipsXml.h"
#include "CDocRelationships.h"
#include <string>

CDocRelationshipsXml::CDocRelationshipsXml()
    : CXmlFile()
{

}

void CDocRelationshipsXml::SetDefoult()
{
    CDocRelationships *oDocRelationships = new CDocRelationships;
    oDocRelationships->SetDefoult();
    SetXmlStructure(oDocRelationships);
}

void CDocRelationshipsXml::AddLinkToFile(std::wstring sPathFile, int nId)
{
    CElement *oRelationship = new CElement(L"Relationship", L"", L"", false);
    oRelationship->AddArgument(L"Id", L"rId" + std::to_wstring(nId));
    oRelationship->AddArgument(L"Type", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink");
    oRelationship->AddArgument(L"Target", sPathFile);
    oRelationship->AddArgument(L"TargetMode", L"External");

    GetXmlStructure()->AddChildren(oRelationship);
}
