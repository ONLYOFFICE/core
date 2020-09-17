#include "CCoreProperties.h"

CCoreProperties::CCoreProperties()
    : CElement(L"coreProperties",L"", L"cp")
{

}

void CCoreProperties::SetDefoult()
{
    AddArgument(L"xmlns:cp", L"http://schemas.openxmlformats.org/package/2006/metadata/core-properties");
    AddArgument(L"xmlns:dc", L"http://purl.org/dc/elements/1.1/");
    AddArgument(L"xmlns:dcterms", L"http://purl.org/dc/terms/");
    AddArgument(L"xmlns:dcmitype", L"http://purl.org/dc/dcmitype/");
    AddArgument(L"xmlns:xsi", L"http://www.w3.org/2001/XMLSchema-instance");

    CElement *oTitle = new CElement(L"title", L"", L"dc");
    AddChildren(oTitle);

    CElement *oSubject = new CElement(L"subject", L"Green", L"dc");
    AddChildren(oSubject);

    CElement *oCreator = new CElement(L"creator", L"", L"dc");
    AddChildren(oCreator);

    CElement *oKeywords = new CElement(L"keywords", L"", L"cp");
    AddChildren(oKeywords);

    CElement *oDescription = new CElement(L"description", L"", L"dc");
    AddChildren(oDescription);

    CElement *oLastModifiedBy = new CElement(L"lastModifiedBy", L"Green", L"cp");
    AddChildren(oLastModifiedBy);

    CElement *oRevision = new CElement(L"revision", L"", L"cp");
    AddChildren(oRevision);

    CElement *oCreated = new CElement(L"created", L"", L"dcterms", false);
    oCreated->AddArgument(L"xsi:type", L"dcterms:W3CDTF");
    oCreated->setValue(L"2020-07-17T09:14:00Z");
    AddChildren(oCreated);

    CElement *oModified = new CElement(L"modified", L"", L"dcterms", false);
    oModified->AddArgument(L"xsi:type", L"dcterms:W3CDTF");
    oModified->setValue(L"2020-07-17T09:14:00Z");
    AddChildren(oModified);
}
