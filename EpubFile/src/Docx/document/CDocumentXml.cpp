#include "CDocumentXml.h"
#include "../../../../DesktopEditor/xml/include/xmlutils.h"
#include "CDocument.h"

CDocumentXml::CDocumentXml()
    : CXmlFile()
{
}

void CDocumentXml::SetDefoult()
{
    CDocument *oDocument = new CDocument();
    oDocument->SetDefoult();
    SetXmlStructure(oDocument);
}

bool CDocumentXml::AddParagraph(std::wstring sText, bool bNewPAge)
{
    if (IsEmpty())
        return false;

    CElement* oDocumentStructure = GetXmlStructure();
    if (oDocumentStructure->GetName() != L"document")
        return false;

    CDocument *oDocument = static_cast<CDocument*>(oDocumentStructure);
    if (!oDocument->AddParagraph(sText, bNewPAge))
        return false;

    return true;
}

bool CDocumentXml::AddParagraph(CParagraph *oParagraph)
{
    if (IsEmpty() || oParagraph == NULL)
        return false;

    CElement* oDocumentStructure = GetXmlStructure();
    if (oDocumentStructure->GetName() != L"document")
        return false;

    CDocument *oDocument = static_cast<CDocument*>(oDocumentStructure);
    if (!oDocument->AddParagraph(oParagraph))
        return false;

    return true;
}

bool CDocumentXml::AddElement(CElement *oElement, bool bEnd)
{
    if (IsEmpty() || oElement == NULL)
        return false;

    CElement* oDocumentStructure = GetXmlStructure();
    if (oDocumentStructure->GetName() != L"document")
        return false;

    CDocument *oDocument = static_cast<CDocument*>(oDocumentStructure);
    if (bEnd)
        oDocument->AddChildren(oElement, oDocument->GetCountChildrens() - 1);
    else
        oDocument->AddChildren(oElement, 0);
    return true;
}

CParagraph* CDocumentXml::GetParagraph(int nIndex)
{
    return static_cast<CDocument*>(GetXmlStructure())->GetParagraph(nIndex);
}

